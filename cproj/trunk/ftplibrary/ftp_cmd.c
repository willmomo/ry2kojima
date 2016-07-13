/*-----------------------------------------------------------------------------
	Visual Studio を使うときは、このソースファイルに、プリンコンパイル済み
	ヘッダを使わないように設定してください。
-----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------
  [note]
  <2010/06/10 r.kojima>
  タイムアウトを可変にした。FTP_CTRL::to を設定すればok。
  デフォルトのタイムアウトは、5 秒。

  <2010/06/07 r.kojima>
  \n を使うと、OS-9000で正しく処理できないことが多いので、
  \x0a を使うようにしよう。
---------------------------------------------------------------------*/
#if defined(_WIN32)
#pragma warning(disable : 4996)
#endif

#include "ftp_cmd.h"
#include "__os__.h"


/*---------------------------------------------------------------------
	グローバル変数
---------------------------------------------------------------------*/
static char s_buffer[RECVBUF_SIZE];	/* ソケットを構築する際に、バッファを初期化したのでグローバル化(元は get_response の中) */
static int s_recv_status = 0;		/* */

int ftp_get_recv_status() { return s_recv_status; }

/*---------------------------------------------------------------------
	期待したデータが結合した状態で受信する事があるため標準の recv() の
	代わりに用意しました
---------------------------------------------------------------------*/
static int _recv(int s, char *buf, int len, int flags)
{
	int n, rdy, ret;

	n = 0;
	rdy = _gs_rdy(s);						/* 受信済みデータ数 */
	while((n < rdy) && (n < len)){
		ret = recv(s, &buf[n], 1, flags);
		if(ret <= 0){
			return(ret);
		}
		if(buf[n] == 0x0a){
			n++;
			break;
		}
		n++;
	}

	return(n);
}

/*---------------------------------------------------------------------
	ホスト名から、IPアドレスを取得する
---------------------------------------------------------------------*/
unsigned long get_ipaddr(const char *hostname) {
	unsigned long result;
	struct hostent* phe;

	result = inet_addr(hostname);
	if (result == INADDR_NONE) {
		phe = gethostbyname(hostname);
		if (phe) {
			result = *(unsigned long*)phe->h_addr;
		}
	}

	return result;
}

/*---------------------------------------------------------------------
	バッファから一行取り出す

	[戻り値]
	0 = 行が取り出せた
	-1 = 行が取り出せない
---------------------------------------------------------------------*/
int read_line(char *buffer, char *line_buf) {
	char *src_p = buffer;
	char *dst_p = line_buf;
	int result = -1;

	while (*src_p != '\0') {
		if ((*(dst_p++) = *(src_p++)) == '\x0a') {
			*dst_p = '\0';
			memmove(buffer, src_p, strlen(src_p) + 1);
			result = 0;
			break;
		}
	}

	return result;
}

/*---------------------------------------------------------------------
	受信バッファからレスポンスを取り出す

	[戻り値]
	レスポンスコードを返す。
	レスポンス行を取り出せなかったときは、-1を返す。
---------------------------------------------------------------------*/
int get_response(const char *buf, char *line_buf) {
	int result = -1;

	strcat(s_buffer, buf);
	while (read_line(s_buffer, line_buf) == 0) {
#if defined(USE_WSSLOG)
		Winssa_Log("FTPCMD", LOGLV_INFO, "%s", line_buf);
#else
		printf("%s", line_buf);
#endif
		if (isdigit(line_buf[0]) && isdigit(line_buf[1]) && isdigit(line_buf[2]) && (line_buf[3] == ' '))	{
			result = atoi(line_buf);
			break;
		}
	}

	return result;
}

/*---------------------------------------------------------------------
	command socket からのレスポンスを受信する

	[戻り値]
	-1 = select でエラー
	-2 = select でタイムアウト
	-3 = recv でエラー
	-4 = recv で disconnected
---------------------------------------------------------------------*/
int ftp_recv_response(const FTP_CTRL *pFtpCtrl, char *response) {
	fd_set rfds;
	struct timeval to;
	char buf[512];
	int ret;
	int result = 0;
	s_recv_status = 0;

	do {
		FD_ZERO(&rfds);
		FD_SET(pFtpCtrl->command_socket, &rfds);
		to = pFtpCtrl->to;
		ret = select(pFtpCtrl->command_socket + 1, &rfds, NULL, NULL, &to);
		if (ret > 0) {
			ret = _recv(pFtpCtrl->command_socket, buf, sizeof(buf), 0);
			if (ret > 0) {
				buf[ret] = 0;
				if ((result = get_response(buf, response)) > 0) {
					break;	/* レスポンスが受信できたので、終了 */
				}
			} else if (ret == 0) {
#if defined(USE_WSSLOG)
				Winssa_Log("FTPCMD", LOGLV_INFO, "ftp_recv_response: the connection has been closed.");
#else
				printf("ftp_recv_response: the connection has been closed.\n");
#endif
				result = -4;
				s_recv_status = -4;
				break;
			} else {
#if defined(USE_WSSLOG)
				Winssa_Log("FTPCMD", LOGLV_INFO, "ftp_recv_response: recv error : %d", WSAGetLastError());
#else
				printf("ftp_recv_response: recv error : %d\n", WSAGetLastError());
#endif
				result = -3;
				s_recv_status = -3;
				break;
			}
		} else if (ret == 0) {
#if defined(USE_WSSLOG)
			Winssa_Log("FTPCMD", LOGLV_INFO, "ftp_recv_response: select timeout.");
#else
			printf("ftp_recv_response: select timeout.\n");
#endif
			result = -2;
			s_recv_status = -2;
			break;
		} else {
#if defined(USE_WSSLOG)
			Winssa_Log("FTPCMD", LOGLV_INFO, "ftp_recv_response: select error : %d", WSAGetLastError());
#else
			printf("ftp_recv_response: select error : %d\n", WSAGetLastError());
#endif
			result = -1;
			s_recv_status = -1;
			break;
		}
	} while (1);

	return result;
}

/*---------------------------------------------------------------------
	優美にソケットを閉じる
---------------------------------------------------------------------*/
SOCKET socket_gracefully_close(SOCKET s) {
	char buf[512];

	if (s == INVALID_SOCKET) return s;

	shutdown(s, SD_SEND);
	while (recv(s, buf, sizeof(buf), 0) > 0)
		;
	shutdown(s, SD_BOTH);

	closesocket(s);

	return INVALID_SOCKET;
}

/*---------------------------------------------------------------------
	welcome message の受信
---------------------------------------------------------------------*/
int ftp_recv_welcome(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* レスポンスが受信できたので、内容をチェック */
		if (code == 220) {
			/* 正常応答 */
			result = 0;
		} else {
			/* 異常応答 */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	USER コマンドの送信
---------------------------------------------------------------------*/
int ftp_send_USER(const FTP_CTRL *pFtpCtrl, const char *user) {
	char buf[80];
	int len;
	int result = 0;

	printf(">USER %s\n", user);
	len = sprintf(buf, "USER %s\x0d\x0a", user);
	if (send(pFtpCtrl->command_socket, buf, len, 0) != len) {
		printf("ftp_send_USER: send error : %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

/*---------------------------------------------------------------------
	USER コマンドの応答受信
---------------------------------------------------------------------*/
int ftp_recv_USER(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* レスポンスが受信できたので、内容をチェック */
		if (code == 331) {
			/* 正常応答 */
			result = 0;
		} else {
			/* 異常応答 */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	USER コマンドの送受信
---------------------------------------------------------------------*/
int ftp_command_USER(const FTP_CTRL *pFtpCtrl, const char *user) {
	ftp_send_USER(pFtpCtrl, user);
	return ftp_recv_USER(pFtpCtrl);
}

/*---------------------------------------------------------------------
	PASS コマンドの送信
---------------------------------------------------------------------*/
int ftp_send_PASS(const FTP_CTRL *pFtpCtrl, const char *pass) {
	char buf[80];
	int len;
	int result = 0;

	printf(">PASS %s\n", pass);
	len = sprintf(buf, "PASS %s\x0d\x0a", pass);
	if (send(pFtpCtrl->command_socket, buf, len, 0) != len) {
		printf("ftp_send_PASS: send error : %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

/*---------------------------------------------------------------------
	PASS コマンドの応答受信
---------------------------------------------------------------------*/
int ftp_recv_PASS(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* レスポンスが受信できたので、内容をチェック */
		if (code == 230) {
			/* 正常応答 */
			result = 0;
		} else {
			/* 異常応答 */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	PASS コマンドの送受信
---------------------------------------------------------------------*/
int ftp_command_PASS(const FTP_CTRL *pFtpCtrl, const char *pass) {
	ftp_send_PASS(pFtpCtrl, pass);	/* PASS コマンド送信 */
	return ftp_recv_PASS(pFtpCtrl);
}

/*---------------------------------------------------------------------
	TYPE コマンドの送信
---------------------------------------------------------------------*/
int ftp_send_TYPE(const FTP_CTRL *pFtpCtrl, int mode /* 'I' or 'A' */) {
	char buf[80];
	int len;
	int result = 0;

#if defined(USE_WSSLOG)
	Winssa_Log("FTPCMT", LOGLV_INFO, ">TYPE %c", mode);
#else
	printf(">TYPE %c\n", mode);
#endif
	len = sprintf(buf, "TYPE %c\x0d\x0a", mode);
	if (send(pFtpCtrl->command_socket, buf, len, 0) != len) {
		printf("ftp_send_TYPE: send error : %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

/*---------------------------------------------------------------------
	TYPE コマンドの応答受信
---------------------------------------------------------------------*/
int ftp_recv_TYPE(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* レスポンスが受信できたので、内容をチェック */
		if (code == 200) {
			/* 正常応答 */
			result = 0;
		} else {
			/* 異常応答 */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	TYPE コマンドの送受信
---------------------------------------------------------------------*/
int ftp_command_TYPE(const FTP_CTRL *pFtpCtrl, int mode) {
	ftp_send_TYPE(pFtpCtrl, mode);	/* TYPE コマンド送信 */
	return ftp_recv_TYPE(pFtpCtrl);
}

/*---------------------------------------------------------------------
	ABOR コマンドの送信(使用実績なし)
---------------------------------------------------------------------*/
int ftp_send_ABOR(const FTP_CTRL *pFtpCtrl) {
	char buf[80];
	int len;
	int result = 0;

	printf(">ABOR\n");
	len = sprintf(buf, "ABOR\x0d\x0a");
	if (send(pFtpCtrl->command_socket, buf, len, 0) != len) {
		printf("ftp_send_ABOR: send error : %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

/*---------------------------------------------------------------------
	ABOR コマンドの応答受信(使用実績なし)
---------------------------------------------------------------------*/
int ftp_recv_ABOR(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* レスポンスが受信できたので、内容をチェック */
		if (code == 200) {
			/* 正常応答 */
			result = 0;
		} else {
			/* 異常応答 */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	ABOR コマンドの送受信
---------------------------------------------------------------------*/
int ftp_command_ABOR(const FTP_CTRL *pFtpCtrl) {
	ftp_send_ABOR(pFtpCtrl);
	return ftp_recv_ABOR(pFtpCtrl);
}

/*---------------------------------------------------------------------
	LIST コマンドの送信
---------------------------------------------------------------------*/
int ftp_send_LIST(const FTP_CTRL *pFtpCtrl, const char *retr) {
	char buf[512];
	int len;
	int result = 0;

	printf(">LIST %s\n", retr);
	len = sprintf(buf, "LIST %s\x0d\x0a", retr);
	if (send(pFtpCtrl->command_socket, buf, len, 0) != len) {
		printf("ftp_send_LIST: send error : %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

/*---------------------------------------------------------------------
	LIST コマンドの応答受信
---------------------------------------------------------------------*/
int ftp_recv_LIST(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* レスポンスが受信できたので、内容をチェック */
		if (code == 150) {
			/* ACTIVEモードの正常応答 */
			result = 0;
		} else if (code == 125) {
			/* PASVモードの正常応答 */
			result = 0;
		} else {
			/* 異常応答 */

			/* ACTIVE モードのときは、data waiting socket で接続を待たずに処理す */
			/* PASV   モードのときは、data socket を gracefully close をしてはいけない！ */

			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	LIST コマンドの送受信
---------------------------------------------------------------------*/
int ftp_command_LIST(const FTP_CTRL *pFtpCtrl, const char *parg) {
	ftp_send_LIST(pFtpCtrl, parg);	/* LIST コマンド送信 */
	return ftp_recv_LIST(pFtpCtrl);
}

/*---------------------------------------------------------------------
	NLST コマンドの送信
---------------------------------------------------------------------*/
int ftp_send_NLST(const FTP_CTRL *pFtpCtrl, const char *retr) {
	char buf[512];
	int len;
	int result = 0;

	printf(">NLST %s\n", retr);
	len = sprintf(buf, "NLST %s\x0d\x0a", retr);
	if (send(pFtpCtrl->command_socket, buf, len, 0) != len) {
		printf("ftp_send_NLST: send error : %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

/*---------------------------------------------------------------------
	NLST コマンドの応答受信
---------------------------------------------------------------------*/
int ftp_recv_NLST(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* レスポンスが受信できたので、内容をチェック */
		if (code == 150) {
			/* ACTIVEモードの正常応答 */
			result = 0;
		} else if (code == 125) {
			/* PASVモードの正常応答 */
			result = 0;
		} else {
			/* 異常応答 */

			/* ACTIVE モードのときは、data waiting socket で接続を待たずに処理す */
			/* PASV   モードのときは、data socket を gracefully close をしてはいけない！ */

			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	NLST コマンドの送受信
---------------------------------------------------------------------*/
int ftp_command_NLST(const FTP_CTRL *pFtpCtrl, const char *parg) {
	ftp_send_NLST(pFtpCtrl, parg);	/* LIST コマンド送信 */
	return ftp_recv_NLST(pFtpCtrl);
}

/*---------------------------------------------------------------------
	PASV コマンドの送信
---------------------------------------------------------------------*/
int ftp_send_PASV(const FTP_CTRL *pFtpCtrl) {
	char buf[80];
	int len;
	int result = 0;

	printf(">PASV\n");
	len = sprintf(buf, "PASV\x0d\x0a");
	if (send(pFtpCtrl->command_socket, buf, len, 0) != len) {
		printf("ftp_send_PASV: send error : %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

/*---------------------------------------------------------------------
	PASV コマンドの応答受信
---------------------------------------------------------------------*/
int ftp_recv_PASV(FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* レスポンスが受信できたので、内容をチェック */
		if (code == 227) {
			char *p1, *p2;
			char *ip1, *ip2, *ip3, *ip4, *port1, *port2;
			p1 = strchr(line_buf, '(') + 1;
			p2 = strchr(p1 + 1, ')');
			*p2 = 0;

			ip1 = strtok(p1, ",");
			ip2 = strtok(NULL, ",");
			ip3 = strtok(NULL, ",");
			ip4 = strtok(NULL, ",");
			port1 = strtok(NULL, ",");
			port2 = strtok(NULL, ",");

			sprintf(pFtpCtrl->pasv_ip, "%s.%s.%s.%s", ip1, ip2, ip3, ip4);
			pFtpCtrl->pasv_port = (unsigned)atoi(port1) * 256 + (unsigned)atoi(port2);

			result = 0;
		} else {
			/* 異常応答 */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	PASV コマンドの送受信
---------------------------------------------------------------------*/
int ftp_command_PASV(FTP_CTRL *pFtpCtrl) {
	ftp_send_PASV(pFtpCtrl);
	return ftp_recv_PASV(pFtpCtrl);
}

/*---------------------------------------------------------------------
	PORT コマンドの送信
---------------------------------------------------------------------*/
int ftp_send_PORT(const FTP_CTRL *pFtpCtrl) {
	char buf[80];
	int len;
	int result = 0;
	unsigned ip;
	unsigned short port;
	struct sockaddr_in sin;
	int lenlen = sizeof(sin);

	if ( getsockname(pFtpCtrl->command_socket, (struct sockaddr *)&sin, &lenlen) < 0 ){
		/*error("getsockname failed.\n");*/
	}
	ip = htonl(sin.sin_addr.s_addr);
			/* ポート番号を取得 */
	if ( getsockname(pFtpCtrl->data_waiting_socket, (struct sockaddr *)&sin, &lenlen) < 0 ){
		/*error("getsockname failed.\n");*/
	}
	port = ntohs(sin.sin_port);

#if defined(USE_WSSLOG)
	Winssa_Log("FTPCMD", LOGLV_INFO, "PORT %d,%d,%d,%d,%d,%d",
		(ip & 0xff000000) >> 24,
		(ip & 0x00ff0000) >> 16,
		(ip & 0x0000ff00) >>  8,
		(ip & 0x000000ff),
		port / 256, port % 256);
#else
	printf("PORT %d,%d,%d,%d,%d,%d\n",
		(ip & 0xff000000) >> 24,
		(ip & 0x00ff0000) >> 16,
		(ip & 0x0000ff00) >>  8,
		(ip & 0x000000ff),
		port / 256, port % 256);
#endif

	len = sprintf(buf, "PORT %d,%d,%d,%d,%d,%d\x0d\x0a",
		(ip & 0xff000000) >> 24,
		(ip & 0x00ff0000) >> 16,
		(ip & 0x0000ff00) >>  8,
		(ip & 0x000000ff),
		port / 256, port % 256);

	if (send(pFtpCtrl->command_socket, buf, len, 0) != len) {
		printf("ftp_send_PORT: send error : %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

/*---------------------------------------------------------------------
	PORT コマンドの受信
---------------------------------------------------------------------*/
int ftp_recv_PORT(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* レスポンスが受信できたので、内容をチェック */
		if (code == 200) {
			/* 正常応答 */
			result = 0;
		} else {
			/* 異常応答 */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	PORT コマンドの送受信
---------------------------------------------------------------------*/
int ftp_command_PORT(const FTP_CTRL *pFtpCtrl) {
	ftp_send_PORT(pFtpCtrl);
	return ftp_recv_PORT(pFtpCtrl);
}

/*---------------------------------------------------------------------
	STOR コマンドの送信
---------------------------------------------------------------------*/
int ftp_send_STOR(const FTP_CTRL *pFtpCtrl, const char *stor) {
	char buf[512];
	int len;
	int result = 0;

#if defined(USE_WSSLOG)
	Winssa_Log("FTPCMD", LOGLV_INFO, ">STOR %s", stor);
#else
	printf(">STOR %s\n", stor);
#endif
	len = sprintf(buf, "STOR %s\x0d\x0a", stor);
	if (send(pFtpCtrl->command_socket, buf, len, 0) != len) {
		printf("ftp_send_STOR: send error : %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

/*---------------------------------------------------------------------
	STOR コマンドの応答受信
---------------------------------------------------------------------*/
int ftp_recv_STOR(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* レスポンスが受信できたので、内容をチェック */
		if (code == 150) {
			/* ACTIVEモードの正常応答 */
			result = 0;
		} else if (code == 125) {
			/* PASVモードの正常応答 */
			result = 0;
		} else {
			/* 異常応答 */

			/* ACTIVE モードのときは、data waiting socket で接続を待たずに処理す */
			/* PASV   モードのときは、data socket を gracefully close をしてはいけない！ */

			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	STOR コマンドの送受信
---------------------------------------------------------------------*/
int ftp_command_STOR(const FTP_CTRL *pFtpCtrl, const char *parg) {
	ftp_send_STOR(pFtpCtrl, parg);	/* STOR コマンド送信 */
	return ftp_recv_STOR(pFtpCtrl);
}

/*---------------------------------------------------------------------
	RETR コマンドの送信
---------------------------------------------------------------------*/
int ftp_send_RETR(const FTP_CTRL *pFtpCtrl, const char *retr) {
	char buf[512];
	int len;
	int result = 0;

	printf(">RETR %s\n", retr);
	len = sprintf(buf, "RETR %s\x0d\x0a", retr);
	if (send(pFtpCtrl->command_socket, buf, len, 0) != len) {
		printf("ftp_send_RETR: send error : %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

/*---------------------------------------------------------------------
	RETR コマンドの応答受信
---------------------------------------------------------------------*/
int ftp_recv_RETR(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* レスポンスが受信できたので、内容をチェック */
		if (code == 150) {
			/* ACTIVEモードの正常応答 */
			result = 0;
		} else if (code == 125) {
			/* PASVモードの正常応答 */
			result = 0;
		} else {
			/* 異常応答 */

			/* ACTIVE モードのときは、data waiting socket で接続を待たずに処理す */
			/* PASV   モードのときは、data socket を gracefully close をしてはいけない！ */

			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	RETR コマンドの送受信
---------------------------------------------------------------------*/
int ftp_command_RETR(const FTP_CTRL *pFtpCtrl, const char *parg) {
	ftp_send_RETR(pFtpCtrl, parg);	/* RETRコマンド送信 */
	return ftp_recv_RETR(pFtpCtrl);
}

/*---------------------------------------------------------------------
	DELE コマンドの送信
---------------------------------------------------------------------*/
int ftp_send_DELE(const FTP_CTRL *pFtpCtrl, const char *arg) {
	char buf[512];
	int len;
	int result = 0;

	printf(">DELE %s\n", arg);
	len = sprintf(buf, "DELE %s\x0d\x0a", arg);
	if (send(pFtpCtrl->command_socket, buf, len, 0) != len) {
		printf("ftp_send_DELE: send error : %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

/*---------------------------------------------------------------------
	DELE コマンドの受信
---------------------------------------------------------------------*/
int ftp_recv_DELE(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* レスポンスが受信できたので、内容をチェック */
		if (code == 200) {
			/* 正常応答 */
			result = 0;
		} else {
			/* 異常応答 */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	DELE コマンドの送受信
---------------------------------------------------------------------*/
int ftp_command_DELE(const FTP_CTRL *pFtpCtrl, const char *remote_filename) {
	ftp_send_DELE(pFtpCtrl, remote_filename);	/* DELE コマンド送信 */
	return ftp_recv_DELE(pFtpCtrl);
}

/*---------------------------------------------------------------------
	MDTM コマンドの送信
---------------------------------------------------------------------*/
int ftp_send_MDTM(const FTP_CTRL *pFtpCtrl, const char *arg) {
	char buf[512];
	int len;
	int result = 0;

	printf(">MDTM %s\n", arg);
	len = sprintf(buf, "MDTM %s\x0d\x0a", arg);
	if (send(pFtpCtrl->command_socket, buf, len, 0) != len) {
		printf("ftp_send_MDTM: send error : %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

/*---------------------------------------------------------------------
	MDTM コマンドの受信
---------------------------------------------------------------------*/
int ftp_recv_MDTM(const FTP_CTRL *pFtpCtrl, char* pBuf) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* レスポンスが受信できたので、内容をチェック */
		if (200 <= code && code < 300) {
			/* 正常応答 */
			strcpy(pBuf, line_buf);
			result = 0;
		} else {
			/* 異常応答 */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	MDTM コマンドの送受信
---------------------------------------------------------------------*/
int ftp_command_MDTM(const FTP_CTRL *pFtpCtrl, const char *remote_filename, char* pBuf) {
	ftp_send_MDTM(pFtpCtrl, remote_filename);	/* DELE コマンド送信 */
	return ftp_recv_MDTM(pFtpCtrl, pBuf);
}

/*---------------------------------------------------------------------
	XMKD コマンドの送信
---------------------------------------------------------------------*/
int ftp_send_XMKD(const FTP_CTRL *pFtpCtrl, const char *arg) {
	char buf[512];
	int len;
	int result = 0;

	printf(">XMKD %s\n", arg);
	len = sprintf(buf, "XMKD %s\x0d\x0a", arg);
	if (send(pFtpCtrl->command_socket, buf, len, 0) != len) {
		printf("ftp_send_XMKD: send error : %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

/*---------------------------------------------------------------------
	XMKD コマンドの受信
---------------------------------------------------------------------*/
int ftp_recv_XMKD(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* レスポンスが受信できたので、内容をチェック */
		if (200 <= code && code < 300) {
			/* 2xx の応答は、正常応答 */
			result = 0;
		} else {
			/* 異常応答 */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	XMKD コマンドの送受信
---------------------------------------------------------------------*/
int ftp_command_XMKD(const FTP_CTRL *pFtpCtrl, const char *remote_filename) {
	ftp_send_XMKD(pFtpCtrl, remote_filename);	/* XMKD コマンド送信 */
	return ftp_recv_XMKD(pFtpCtrl);
}

/*---------------------------------------------------------------------
	XRMD コマンドの送信
---------------------------------------------------------------------*/
int ftp_send_XRMD(const FTP_CTRL *pFtpCtrl, const char *arg) {
	char buf[512];
	int len;
	int result = 0;

	printf(">XRMD %s\n", arg);
	len = sprintf(buf, "XRMD %s\x0d\x0a", arg);
	if (send(pFtpCtrl->command_socket, buf, len, 0) != len) {
		printf("ftp_send_XRMD: send error : %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

/*---------------------------------------------------------------------
	XRMD コマンドの受信
---------------------------------------------------------------------*/
int ftp_recv_XRMD(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* レスポンスが受信できたので、内容をチェック */
		if (200 <= code && code < 300) {
			/* 2xx の応答は、正常応答 */
			result = 0;
		} else {
			/* 異常応答 */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	XRMD コマンドの送受信
---------------------------------------------------------------------*/
int ftp_command_XRMD(const FTP_CTRL *pFtpCtrl, const char *remote_filename) {
	ftp_send_XRMD(pFtpCtrl, remote_filename);	/* XRMD コマンド送信 */
	return ftp_recv_XRMD(pFtpCtrl);
}

/*---------------------------------------------------------------------
	RNFR コマンドの送信
---------------------------------------------------------------------*/
int ftp_send_RNFR(const FTP_CTRL *pFtpCtrl, const char *arg) {
	char buf[512];
	int len;
	int result = 0;

	printf(">RNFR %s\n", arg);
	len = sprintf(buf, "RNFR %s\x0d\x0a", arg);
	if (send(pFtpCtrl->command_socket, buf, len, 0) != len) {
		printf("ftp_send_RNFR: send error : %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

/*---------------------------------------------------------------------
	RNFR コマンドの受信
---------------------------------------------------------------------*/
int ftp_recv_RNFR(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* レスポンスが受信できたので、内容をチェック */
		if (200 <= code && code < 400) {
			/* 2xx の応答は、正常応答 */
			result = 0;
		} else {
			/* 異常応答 */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	RNFR コマンドの送受信
---------------------------------------------------------------------*/
int ftp_command_RNFR(const FTP_CTRL *pFtpCtrl, const char *from_filename) {
	ftp_send_RNFR(pFtpCtrl, from_filename);	/* RNFR コマンド送信 */
	return ftp_recv_RNFR(pFtpCtrl);
}

/*---------------------------------------------------------------------
	RNTO コマンドの送信
---------------------------------------------------------------------*/
int ftp_send_RNTO(const FTP_CTRL *pFtpCtrl, const char *arg) {
	char buf[512];
	int len;
	int result = 0;

	printf(">RNTO %s\n", arg);
	len = sprintf(buf, "RNTO %s\x0d\x0a", arg);
	if (send(pFtpCtrl->command_socket, buf, len, 0) != len) {
		printf("ftp_send_RNTO: send error : %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

/*---------------------------------------------------------------------
	RNTO コマンドの受信
---------------------------------------------------------------------*/
int ftp_recv_RNTO(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* レスポンスが受信できたので、内容をチェック */
		if (200 <= code && code < 300) {
			/* 2xx の応答は、正常応答 */
			result = 0;
		} else {
			/* 異常応答 */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	RNTO コマンドの送受信
---------------------------------------------------------------------*/
int ftp_command_RNTO(const FTP_CTRL *pFtpCtrl, const char *to_filename) {
	ftp_send_RNTO(pFtpCtrl, to_filename);	/* RNTO コマンド送信 */
	return ftp_recv_RNTO(pFtpCtrl);
}

/*---------------------------------------------------------------------
	SIZE コマンドの送信
---------------------------------------------------------------------*/
int ftp_send_SIZE(const FTP_CTRL *pFtpCtrl, const char *arg) {
	char buf[512];
	int len;
	int result = 0;

	printf(">SIZE %s\n", arg);
	len = sprintf(buf, "SIZE %s\x0d\x0a", arg);
	if (send(pFtpCtrl->command_socket, buf, len, 0) != len) {
		printf("ftp_send_SIZE: send error : %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

/*---------------------------------------------------------------------
	SIZE コマンドの受信

	response code の後ろのファイルサイズを返す。
---------------------------------------------------------------------*/
int ftp_recv_SIZE(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* レスポンスが受信できたので、内容をチェック */
		if (code == 213) {
			/* 正常応答 */
			result = atoi(&line_buf[4]);
		} else {
			/* 異常応答 */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	SIZE コマンドの送受信
---------------------------------------------------------------------*/
int ftp_command_SIZE(const FTP_CTRL *pFtpCtrl, const char *remote_filename) {
	ftp_send_SIZE(pFtpCtrl, remote_filename);
	return ftp_recv_SIZE(pFtpCtrl);
}

/*---------------------------------------------------------------------
	HELP コマンドの送信
---------------------------------------------------------------------*/
int ftp_send_HELP(const FTP_CTRL *pFtpCtrl, const char *cmd) {
	char buf[512];
	int len;
	int result = 0;

	printf(">HELP %s\n", cmd);
	len = sprintf(buf, "HELP %s\x0d\x0a", cmd);
	if (send(pFtpCtrl->command_socket, buf, len, 0) != len) {
		printf("ftp_send_HELP: send error : %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

/*---------------------------------------------------------------------
	HELP コマンドの受信
---------------------------------------------------------------------*/
int ftp_recv_HELP(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* レスポンスが受信できたので、内容をチェック */
		if (code == 200) {
			/* 正常応答 */
			result = 0;
		} else {
			/* 異常応答 */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	HELP コマンドの送受信
---------------------------------------------------------------------*/
int ftp_command_HELP(const FTP_CTRL *pFtpCtrl, const char *cmd) {
	ftp_send_HELP(pFtpCtrl, cmd);
	return ftp_recv_HELP(pFtpCtrl);
}

/*---------------------------------------------------------------------
	QUIT コマンドの送信
---------------------------------------------------------------------*/
int ftp_send_QUIT(const FTP_CTRL *pFtpCtrl) {
	char buf[512];
	int len;
	int result = 0;

	printf(">QUIT\n");
	len = sprintf(buf, "QUIT\x0d\x0a");
	if (send(pFtpCtrl->command_socket, buf, len, 0) != len) {
		printf("ftp_send_QUIT: send error : %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

/*---------------------------------------------------------------------
	QUIT コマンドの応答受信
---------------------------------------------------------------------*/
int ftp_recv_QUIT(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result = 0;

	/* レスポンスの受信はするが、何もしない */
	ftp_recv_response(pFtpCtrl, line_buf);

	return result;
}

/*---------------------------------------------------------------------
	QUIT コマンドの送受信
---------------------------------------------------------------------*/
int ftp_command_QUIT(const FTP_CTRL *pFtpCtrl) {
	ftp_send_QUIT(pFtpCtrl);		/* QUIT コマンド送信 */
	return ftp_recv_QUIT(pFtpCtrl);
}

/*---------------------------------------------------------------------
	FTP の接続シーケンス

	[引数]
	serv	サーバー名
	user	ユーザー名
	pass	パスワード

	[戻り値]
	0 正常終了
	!0 異常終了
---------------------------------------------------------------------*/
int ftp_connect(FTP_CTRL *pFtpCtrl, const char *serv, const char *user, const char* pass) {
	struct sockaddr_in myname;
	int ret;

	printf("Connected to %s\n", serv);

	pFtpCtrl->command_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (pFtpCtrl->command_socket == INVALID_SOCKET) {
		printf("ftp_connect: socket error : %d\n", WSAGetLastError());
		return -1;
	}

	memset(&myname, 0, sizeof(myname));

	myname.sin_family = AF_INET;
	myname.sin_addr.s_addr = get_ipaddr(serv);
	myname.sin_port = htons(21);

	ret = connect(pFtpCtrl->command_socket, (struct sockaddr*)&myname, sizeof(myname));
	if (ret != 0) {
		printf("connect 失敗 ( %d )\n", WSAGetLastError());
		pFtpCtrl->command_socket = closesocket(pFtpCtrl->command_socket);
		return -1;
	}

	/* welcome response の受信を待つ */
	if (ftp_recv_welcome(pFtpCtrl) != 0) {
		pFtpCtrl->command_socket = closesocket(pFtpCtrl->command_socket);
		return -1;
	}

	/* USER command の送受信 */
	if (ftp_command_USER(pFtpCtrl, user) != 0) {
		pFtpCtrl->command_socket = closesocket(pFtpCtrl->command_socket);
		return -1;
	}

	/* PASS command の送受信 */
	if (ftp_command_PASS(pFtpCtrl, pass) != 0) {
		pFtpCtrl->command_socket = closesocket(pFtpCtrl->command_socket);
		return -1;
	}

	return 0;
}

/*---------------------------------------------------------------------
	FTP の data waiting socket の準備

	[備考]
	！！処理失敗を無視しています！！
---------------------------------------------------------------------*/
int ftp_listen_data_waiting_port(FTP_CTRL *pFtpCtrl) {
	struct sockaddr_in data_addr;

	pFtpCtrl->data_waiting_socket = socket(AF_INET, SOCK_STREAM, 0);
	memset(&data_addr, 0, sizeof(data_addr));
	data_addr.sin_family = AF_INET;
	data_addr.sin_addr.s_addr = INADDR_ANY;
	data_addr.sin_port = 0;
	if (bind(pFtpCtrl->data_waiting_socket, (struct sockaddr *)&data_addr, sizeof(data_addr)) < 0 ){
		/*error("bind failed.\n");*/
	}
	if (listen(pFtpCtrl->data_waiting_socket, SOMAXCONN) == -1 ){
		/*error("listen failed.\n");*/
	}

	return 0;
}

/*---------------------------------------------------------------------
	FTP のデータポート接続承認シーケンス
---------------------------------------------------------------------*/
int ftp_data_accept(FTP_CTRL *pFtpCtrl) {
	int ret;
	fd_set rfds;
	struct timeval to;
	struct sockaddr_in from;
	int fromlen = sizeof(from);

	FD_ZERO(&rfds);
	FD_SET(pFtpCtrl->data_waiting_socket, &rfds);
	to = pFtpCtrl->to;

	ret = select(pFtpCtrl->data_waiting_socket + 1, &rfds, NULL, NULL, &to);
	if (ret > 0) {
		pFtpCtrl->data_socket = accept(pFtpCtrl->data_waiting_socket, (struct sockaddr *)&from, &fromlen);
		if (pFtpCtrl->data_socket == INVALID_SOCKET) {
			printf("ftp_data_accept: accept error : %d\n", WSAGetLastError());
		}
	} else if (ret == 0) {
		printf("ftp_data_accept: select timeout\n");
	} else {
		printf("ftp_data_accept: select error : %d\n", WSAGetLastError());
	}

	return (pFtpCtrl->data_socket != INVALID_SOCKET) ? 0 : -1;
}

/*---------------------------------------------------------------------
	データポートへの接続シーケンス

	[備考]
	FTPサーバーに指定されたポートに接続する、一連のシーケンスを処理。
	PASVモードのときに使用。
---------------------------------------------------------------------*/
int ftp_data_connect(FTP_CTRL *pFtpCtrl) {
	struct sockaddr_in myname;

	printf("%s (%u) に接続しています。\n", pFtpCtrl->pasv_ip, pFtpCtrl->pasv_port);

	pFtpCtrl->data_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (pFtpCtrl->data_socket == INVALID_SOCKET) {
		printf("ftp_data_connect: socket error : %d\n", WSAGetLastError());
		return -1;
	}

	memset(&myname, 0, sizeof(myname));

	myname.sin_family = AF_INET;
	myname.sin_addr.s_addr = get_ipaddr(pFtpCtrl->pasv_ip);
	myname.sin_port = htons(pFtpCtrl->pasv_port);

	if (connect(pFtpCtrl->data_socket, (struct sockaddr*)&myname, sizeof(myname)) != 0) {
		printf("ftp_data_connect: connect error : %d\n", WSAGetLastError());
		closesocket(pFtpCtrl->data_socket);
		pFtpCtrl->data_socket = INVALID_SOCKET;
		return -1;
	}

	return 0;
}

/*---------------------------------------------------------------------
	データポートでの受信処理

	[戻り値]
	-1 select でエラー
	-2 select でタイムアウト
	-4 data socket で disconnected
---------------------------------------------------------------------*/
int ftp_data_recv(FTP_CTRL *pFtpCtrl, FILE* fp) {
	fd_set rfds;
	unsigned int nfds;
	struct timeval to;
	int ret;
	char buf[RECVBUF_SIZE];
	char line_buf[RECVBUF_SIZE];
	int result = 0;
	int code;
#if 0
#if defined(_OS9000)
	struct spf_popts opt;
	u_int32 opt_size;
#endif
#endif
	printf("ftp_data_recv()\n");

#if 0
#if defined(_OS9000)
	opt_size = sizeof(opt);
	_os_gs_popt(sock, &opt_size, &opt);
	opt.pd_iotime = pFtpCtrl->to * 100;						/* 受信タイムアウト値を設定する(1秒) */
	_os_ss_popt(sock, opt_size, &opt);
#endif
#endif
	/* data socket の完結を待つ */
	while (pFtpCtrl->data_socket != INVALID_SOCKET) {
		printf("[%d ms] ftp_data_recv: [data] CTRL(%d) DATA(%d)\n", GetTickCount(), pFtpCtrl->command_socket, pFtpCtrl->data_socket);
#if 0
#if defined(_OS9000)
		ret = recv(pFtpCtrl->data_socket, buf, sizeof(buf), 0);

		printf("ftp_data_recv: [data] DATA SOCKET RECV(%d BYTE)\n", ret);

		if (ret > 0) {
			ret = fwrite(buf, sizeof(char), ret, fp);
		} else if (ret == 0) {
			printf("ftp_data_recv: [data] 全受信完了\n");
			ftp_close_data_socket(pFtpCtrl);
		} else {
			if (errno == ETIMEDOUT) {	/* 受信タイムアウト */
				printf("ftp_data_recv: [data] recv でタイムアウト\n");
				ftp_close_data_socket(pFtpCtrl);
				result = -2;
			} else {
				printf("ftp_data_recv: [data] recv エラー (%d)\n", errno);
				ftp_close_data_socket(pFtpCtrl);
				result = -1;
			}
		}
#else
#endif
#endif
		FD_ZERO(&rfds);
		FD_SET(pFtpCtrl->data_socket, &rfds);
		nfds = pFtpCtrl->data_socket;
		/* data_socket が、INVLAID_SOCKET のときは、
		   データを受信し終えて、ソケットが閉じた状態です。
		   command socket から、Transfer complete が来るのを待つ状態です */

		to = pFtpCtrl->to;

		ret = select(nfds + 1, &rfds, NULL, NULL, &to);

		printf("[%d ms] ftp_data_recv: [data] select return = %d.\n", GetTickCount(), ret);

		if (ret > 0) {
			if (FD_ISSET(pFtpCtrl->data_socket, &rfds)) {
				/* サーバーからデータが来たぞ */
				ret = recv(pFtpCtrl->data_socket, buf, sizeof(buf), 0);

				printf("ftp_data_recv: [data] DATA SOCKET RECV(%d BYTE)\n", ret);

				if (ret > 0) {
					ret = fwrite(buf, sizeof(char), ret, fp);
				} else {
					printf("ftp_data_recv: [data] 全受信完了\n");
					ftp_close_data_socket(pFtpCtrl);
				}
			}
		} else if (ret == 0) {
			printf("ftp_data_recv: [data] select でタイムアウト\n");
			ftp_close_data_socket(pFtpCtrl);
			result = -2;
			break;
		} else {
			printf("ftp_data_recv: [data] select エラー\n");
			ftp_close_data_socket(pFtpCtrl);
			result = -1;
			break;
		}
/*#endif*/
	}

	/* コマンドソケットの言い分を受信 */
	do {
		printf("[%d ms] ftp_data_recv: [ctrl] CTRL(%d) DATA(%d)\n", GetTickCount(), pFtpCtrl->command_socket, pFtpCtrl->data_socket);
#if 0
#if defined(_OS9000)
		ret = _recv(pFtpCtrl->command_socket, buf, sizeof(buf), 0);

		printf("ftp_data_recv: [ctrl] CTRL SOCKET RECV(%d BYTE)\n", ret);

		if (ret > 0) {
			buf[ret] = 0;
			if ((code = get_response(buf, line_buf)) > 0) {
				/* レスポンスが受信できたので、内容をチェック */
				if (code == 150) {
					/* 正常応答 */
					assert(0);
					break;
				} else if (code == 226) {
					/* 送信完了 */
					break;
				} else {
					/* 異常応答 */
					/* data waiting socket で接続を待つ必要なし! */
					break;
				}
			}
		} else {
			if (errno == ETIMEDOUT) {	/* 受信タイムアウト */
				printf("ftp_data_recv: [ctrl] select でタイムアウト\n");
				result = -2;
			} else {
				printf("ftp_data_recv: [ctrl] select エラー (%d)\n", errno);
				result = -1;
			}
		}
#else
#endif
#endif
		FD_ZERO(&rfds);
		FD_SET(pFtpCtrl->command_socket, &rfds);
		nfds = pFtpCtrl->command_socket;

		to = pFtpCtrl->to;

		ret = select(nfds + 1, &rfds, NULL, NULL, &to);

		printf("[%d ms] ftp_data_recv: [ctrl] select return = %d.\n", GetTickCount(), ret);

		if (ret > 0) {
			if (FD_ISSET(pFtpCtrl->command_socket, &rfds)) {
				/* サーバーから、何か来たぞ? */
				ret = _recv(pFtpCtrl->command_socket, buf, sizeof(buf), 0);

				printf("ftp_data_recv: [ctrl] CTRL SOCKET RECV(%d BYTE)\n", ret);

				if (ret > 0) {
					buf[ret] = 0;
					if ((code = get_response(buf, line_buf)) > 0) {
						/* レスポンスが受信できたので、内容をチェック */
						if (code == 150) {
							/* 正常応答 */
							assert(0);
							break;
						} else if (code == 226) {
							/* 送信完了 */
							break;
						} else {
							/* 異常応答 */
							/* data waiting socket で接続を待つ必要なし! */
							break;
						}
					}
				}
			}
		} else if (ret == 0) {
			printf("ftp_data_recv: [ctrl] select でタイムアウト\n");
			result = -2;
			break;
		} else {
			printf("ftp_data_recv: [ctrl] select エラー\n");
			result = -1;
			break;
		}
/*#endif*/
	} while (1);

	return result;
}

/*---------------------------------------------------------------------
	データポートでの受信処理

	[戻り値]
	-1 select でエラー
	-2 select でタイムアウト
	-4 data socket で disconnected
---------------------------------------------------------------------*/
int ftp_data_recv2(FTP_CTRL *pFtpCtrl, char* pBuf, int bufSize) {
	fd_set rfds;
	unsigned int nfds;
	struct timeval to;
	int ret;
	char buf[RECVBUF_SIZE];
	char line_buf[RECVBUF_SIZE];
	int result = 0;
	int code;
	int wp = 0;
#if 0
#if defined(_OS9000)
	struct spf_popts opt;
	u_int32 opt_size;
#endif
#endif
	printf("ftp_data_recv2()\n");

#if 0
#if defined(_OS9000)
	opt_size = sizeof(opt);
	_os_gs_popt(sock, &opt_size, &opt);
	opt.pd_iotime = pFtpCtrl->to * 100;						/* 受信タイムアウト値を設定する(1秒) */
	_os_ss_popt(sock, opt_size, &opt);
#endif
#endif
	/* data socket の完結を待つ */
	while (pFtpCtrl->data_socket != INVALID_SOCKET) {
		printf("[%d ms] ftp_data_recv2: [data] CTRL(%d) DATA(%d)\n", GetTickCount(), pFtpCtrl->command_socket, pFtpCtrl->data_socket);
#if 0
#if defined(_OS9000)
		ret = recv(pFtpCtrl->data_socket, buf, sizeof(buf), 0);

		printf("ftp_data_recv2: [data] DATA SOCKET RECV(%d BYTE)\n", ret);

		if (ret > 0) {
			ret = fwrite(buf, sizeof(char), ret, fp);
		} else if (ret == 0) {
			printf("ftp_data_recv2: [data] 全受信完了\n");
			ftp_close_data_socket(pFtpCtrl);
		} else {
			if (errno == ETIMEDOUT) {	/* 受信タイムアウト */
				printf("ftp_data_recv2: [data] recv でタイムアウト\n");
				ftp_close_data_socket(pFtpCtrl);
				result = -2;
			} else {
				printf("ftp_data_recv2: [data] recv エラー (%d)\n", errno);
				ftp_close_data_socket(pFtpCtrl);
				result = -1;
			}
		}
#else
#endif
#endif
		FD_ZERO(&rfds);
		FD_SET(pFtpCtrl->data_socket, &rfds);
		nfds = pFtpCtrl->data_socket;
		/* data_socket が、INVLAID_SOCKET のときは、
		   データを受信し終えて、ソケットが閉じた状態です。
		   command socket から、Transfer complete が来るのを待つ状態です */

		to = pFtpCtrl->to;

		ret = select(nfds + 1, &rfds, NULL, NULL, &to);

		printf("[%d ms] ftp_data_recv2: [data] select return = %d.\n", GetTickCount(), ret);

		if (ret > 0) {
			if (FD_ISSET(pFtpCtrl->data_socket, &rfds)) {
				/* サーバーからデータが来たぞ */
				ret = recv(pFtpCtrl->data_socket, buf, sizeof(buf), 0);

				printf("ftp_data_recv2: [data] DATA SOCKET RECV(%d BYTE)\n", ret);

				if (ret > 0) {
					/*ret = fwrite(buf, sizeof(char), ret, fp);*/
					int moveSize;
					
					moveSize = ((wp + ret) <= bufSize) ? ret : (bufSize - wp);
					memmove(pBuf + wp, buf, moveSize);
					wp += moveSize;
					result = wp;
				} else {
					printf("ftp_data_recv2: [data] 全受信完了\n");
					ftp_close_data_socket(pFtpCtrl);
				}
			}
		} else if (ret == 0) {
			printf("ftp_data_recv2: [data] select でタイムアウト\n");
			ftp_close_data_socket(pFtpCtrl);
			result = -2;
			break;
		} else {
			printf("ftp_data_recv2: [data] select エラー\n");
			ftp_close_data_socket(pFtpCtrl);
			result = -1;
			break;
		}
/*#endif*/
	}

	/* コマンドソケットの言い分を受信 */
	do {
		printf("[%d ms] ftp_data_recv2: [ctrl] CTRL(%d) DATA(%d)\n", GetTickCount(), pFtpCtrl->command_socket, pFtpCtrl->data_socket);
#if 0
#if defined(_OS9000)
		ret = _recv(pFtpCtrl->command_socket, buf, sizeof(buf), 0);

		printf("ftp_data_recv2: [ctrl] CTRL SOCKET RECV(%d BYTE)\n", ret);

		if (ret > 0) {
			buf[ret] = 0;
			if ((code = get_response(buf, line_buf)) > 0) {
				/* レスポンスが受信できたので、内容をチェック */
				if (code == 150) {
					/* 正常応答 */
					assert(0);
					break;
				} else if (code == 226) {
					/* 送信完了 */
					break;
				} else {
					/* 異常応答 */
					/* data waiting socket で接続を待つ必要なし! */
					break;
				}
			}
		} else {
			if (errno == ETIMEDOUT) {	/* 受信タイムアウト */
				printf("ftp_data_recv2: [ctrl] select でタイムアウト\n");
				result = -2;
			} else {
				printf("ftp_data_recv2: [ctrl] select エラー (%d)\n", errno);
				result = -1;
			}
		}
#else
#endif
#endif
		FD_ZERO(&rfds);
		FD_SET(pFtpCtrl->command_socket, &rfds);
		nfds = pFtpCtrl->command_socket;

		to = pFtpCtrl->to;

		ret = select(nfds + 1, &rfds, NULL, NULL, &to);

		printf("[%d ms] ftp_data_recv2: [ctrl] select return = %d.\n", GetTickCount(), ret);

		if (ret > 0) {
			if (FD_ISSET(pFtpCtrl->command_socket, &rfds)) {
				/* サーバーから、何か来たぞ? */
				ret = _recv(pFtpCtrl->command_socket, buf, sizeof(buf), 0);

				printf("ftp_data_recv2: [ctrl] CTRL SOCKET RECV(%d BYTE)\n", ret);

				if (ret > 0) {
					buf[ret] = 0;
					if ((code = get_response(buf, line_buf)) > 0) {
						/* レスポンスが受信できたので、内容をチェック */
						if (code == 150) {
							/* 正常応答 */
							assert(0);
							break;
						} else if (code == 226) {
							/* 送信完了 */
							break;
						} else {
							/* 異常応答 */
							/* data waiting socket で接続を待つ必要なし! */
							break;
						}
					}
				}
			}
		} else if (ret == 0) {
			printf("ftp_data_recv2: [ctrl] select でタイムアウト\n");
			result = -2;
			break;
		} else {
			printf("ftp_data_recv2: [ctrl] select エラー\n");
			result = -1;
			break;
		}
/*#endif*/
	} while (1);

	return result;
}

/*---------------------------------------------------------------------
	データポートでの受信処理

	[戻り値]
	-1 select でエラー
	-2 select でタイムアウト
	-4 data socket で disconnected
---------------------------------------------------------------------*/
int ftp_data_recv_old(FTP_CTRL *pFtpCtrl, FILE* fp) {
	fd_set rfds;
	unsigned int nfds;
	struct timeval to;
	int ret;
	char buf[RECVBUF_SIZE];
	char line_buf[RECVBUF_SIZE];
	int result = 0;
	int code;

	printf("ftp_data_recv()\n");

	do {
		printf("[%d ms] ftp_data_recv: CTRL(%d) DATA(%d)\n", GetTickCount(), pFtpCtrl->command_socket, pFtpCtrl->data_socket);

		FD_ZERO(&rfds);
		FD_SET(pFtpCtrl->command_socket, &rfds);
		nfds = pFtpCtrl->command_socket;

		if (pFtpCtrl->data_socket != INVALID_SOCKET) {
			/* FD_ZERO(&rfds); */
			FD_SET(pFtpCtrl->data_socket, &rfds);
			nfds = max(nfds, pFtpCtrl->data_socket);
			/* data_socket が、INVLAID_SOCKET のときは、
			   データを受信し終えて、ソケットが閉じた状態です。
			   command socket から、Transfer complete が来るのを待つ状態です */
		}

		to = pFtpCtrl->to;

		ret = select(nfds + 1, &rfds, NULL, NULL, &to);

		printf("[%d ms] ftp_data_recv: select return = %d.\n", GetTickCount(), ret);

		if (ret > 0) {
			if (pFtpCtrl->data_socket != INVALID_SOCKET && FD_ISSET(pFtpCtrl->data_socket, &rfds)) {
				/* サーバーからデータが来たぞ */
				ret = recv(pFtpCtrl->data_socket, buf, sizeof(buf), 0);

				printf("ftp_data_recv: DATA SOCKET RECV(%d BYTE)\n", ret);

				if (ret > 0) {
					ret = fwrite(buf, sizeof(char), ret, fp);
				} else {
					ftp_close_data_socket(pFtpCtrl);
				}
			}
			if (FD_ISSET(pFtpCtrl->command_socket, &rfds)) {
				/* サーバーから、何か来たぞ? */
				ret = _recv(pFtpCtrl->command_socket, buf, sizeof(buf), 0);

				printf("ftp_data_recv: CTRL SOCKET RECV(%d BYTE)\n", ret);

				if (ret > 0) {
					buf[ret] = 0;
					if ((code = get_response(buf, line_buf)) > 0) {
						/* レスポンスが受信できたので、内容をチェック */
						if (code == 150) {
							/* 正常応答 */
							assert(0);
							break;
						} else if (code == 226) {
							/* 送信完了 */
							break;
						} else {
							/* 異常応答 */
							/* data waiting socket で接続を待つ必要なし! */
							goto quit_command;
						}
					}
				}
			}
		} else if (ret == 0) {
			printf("ftp_data_recv: select でタイムアウト\n");
			result = -2;
			break;
		} else {
			printf("ftp_data_recv: select エラー\n");
			result = -1;
			break;
		}
	} while (1);

quit_command:

	return result;
}

/*---------------------------------------------------------------------
	データポートでの受信処理

	[戻り値]
	-1 select でエラー
	-2 select でタイムアウト
	-4 data socket で disconnected
---------------------------------------------------------------------*/
int ftp_data_recv2_old(FTP_CTRL *pFtpCtrl, char* pBuf, int bufSize) {
	fd_set rfds;
	unsigned int nfds;
	struct timeval to;
	int ret;
	char buf[RECVBUF_SIZE];
	char line_buf[RECVBUF_SIZE];
	int result = 0;
	int code;
	int wp = 0;

	printf("ftp_data_recv2()\n");

	do {
		printf("[%d ms] ftp_data_recv2: CTRL(%d) DATA(%d)\n", GetTickCount(), pFtpCtrl->command_socket, pFtpCtrl->data_socket);

		FD_ZERO(&rfds);
		FD_SET(pFtpCtrl->command_socket, &rfds);
		nfds = pFtpCtrl->command_socket;

		if (pFtpCtrl->data_socket != INVALID_SOCKET) {
			/* FD_ZERO(&rfds); */
			FD_SET(pFtpCtrl->data_socket, &rfds);
			nfds = max(nfds, pFtpCtrl->data_socket);
			/* data_socket が、INVLAID_SOCKET のときは、
			   データを受信し終えて、ソケットが閉じた状態です。
			   command socket から、Transfer complete が来るのを待つ状態です */
		}

		to = pFtpCtrl->to;

		ret = select(nfds + 1, &rfds, NULL, NULL, &to);

		printf("[%d ms] ftp_data_recv2: select return = %d.\n", GetTickCount(), ret);

		if (ret > 0) {
			if (pFtpCtrl->data_socket != INVALID_SOCKET && FD_ISSET(pFtpCtrl->data_socket, &rfds)) {
				/* サーバーからデータが来たぞ */
				ret = recv(pFtpCtrl->data_socket, buf, sizeof(buf), 0);

				printf("ftp_data_recv2: DATA SOCKET RECV(%d BYTE)\n", ret);

				if (ret > 0) {
					/*ret = fwrite(buf, sizeof(char), ret, fp);*/
					int moveSize;
					
					moveSize = ((wp + ret) <= bufSize) ? ret : (bufSize - wp);
					memmove(pBuf + wp, buf, moveSize);
					wp += moveSize;
					result = wp;
				} else {
					ftp_close_data_socket(pFtpCtrl);
				}
			}
			if (FD_ISSET(pFtpCtrl->command_socket, &rfds)) {
				/* サーバーから、何か来たぞ? */
				ret = _recv(pFtpCtrl->command_socket, buf, sizeof(buf), 0);

				printf("ftp_data_recv2: CTRL SOCKET RECV(%d BYTE)\n", ret);

				if (ret > 0) {
					buf[ret] = 0;
					if ((code = get_response(buf, line_buf)) > 0) {
						/* レスポンスが受信できたので、内容をチェック */
						if (code == 150) {
							/* 正常応答 */
							assert(0);
							break;
						} else if (code == 226) {
							/* 送信完了 */
							break;
						} else {
							/* 異常応答 */
							/* data waiting socket で接続を待つ必要なし! */
							goto quit_command;
						}
					}
				}
			}
		} else if (ret == 0) {
			printf("ftp_data_recv2: select でタイムアウト\n");
			result = -2;
			break;
		} else {
			printf("ftp_data_recv2: select エラー\n");
			result = -1;
			break;
		}
	} while (1);

quit_command:

	return result;
}

/*---------------------------------------------------------------------
	データポートでの送信処理

	[戻り値]
	-1 select でエラー
	-2 select でタイムアウト
	-4 data socket で disconnected
---------------------------------------------------------------------*/
int ftp_data_send(FTP_CTRL *pFtpCtrl, FILE* fp) {
	fd_set rfds;
	fd_set wfds;
	unsigned int nfds;
	struct timeval to;
	int ret;
	char buf[RECVBUF_SIZE];
	char line_buf[RECVBUF_SIZE];
	int result = 0;
	int code;

	do {
		FD_ZERO(&rfds);
		FD_SET(pFtpCtrl->command_socket, &rfds);
		nfds = pFtpCtrl->command_socket;

		if (pFtpCtrl->data_socket != INVALID_SOCKET) {
			FD_ZERO(&wfds);
			FD_SET(pFtpCtrl->data_socket, &wfds);
			nfds = max(nfds, pFtpCtrl->data_socket);
			/* data_socket が、INVLAID_SOCKET のときは、
			   データを送信し終えて、ソケットを閉じた状態です。
			   command socket から、Transfer complete が来るのを待つ状態です */
		}

		to = pFtpCtrl->to;

		ret = select(nfds + 1, &rfds, (pFtpCtrl->data_socket == INVALID_SOCKET ? NULL : &wfds), NULL, &to);
		if (ret > 0) {
			if (FD_ISSET(pFtpCtrl->command_socket, &rfds)) {
				/* サーバーから、何か来たぞ? */
				ret = _recv(pFtpCtrl->command_socket, buf, sizeof(buf), 0);
				if (ret > 0) {
					buf[ret] = 0;
					if ((code = get_response(buf, line_buf)) > 0) {
						/* レスポンスが受信できたので、内容をチェック */
						if (code == 150) {
							/* 正常応答 */
							assert(0);
							break;
						} else if (code == 226) {
							/* 送信完了 */
							break;
						} else {
							/* 異常応答 */
							/* data waiting socket で接続を待つ必要なし! */
							goto quit_command;
						}
					}
				}
			}
			if (pFtpCtrl->data_socket != INVALID_SOCKET && FD_ISSET(pFtpCtrl->data_socket, &wfds)) {
				/* サーバーにデータを送るぞ */
				ret = fread(buf, sizeof(char), sizeof(buf), fp);
				if (ret > 0) {
					send(pFtpCtrl->data_socket, buf, ret, 0);
				} else {
					ftp_close_data_socket(pFtpCtrl);
				}
			}
		} else if (ret == 0) {
			printf("ftp_data_send: select でタイムアウト\n");
			result = -2;
			break;
		} else {
			printf("ftp_data_send: select エラー\n");
			result = -1;
			break;
		}
	} while (1);

quit_command:

	return result;
}

/*---------------------------------------------------------------------
	データポートでの送信処理

	[戻り値]
	-1 select でエラー
	-2 select でタイムアウト
	-4 data socket で disconnected

	[説明]
	FILE* の代わりに、バッファへのポインタとそのサイズを受け取る
---------------------------------------------------------------------*/
int ftp_data_send_buffer(FTP_CTRL *pFtpCtrl, const char *pBuf, size_t cbSize) {
	fd_set rfds;
	fd_set wfds;
	unsigned int nfds;
	struct timeval to;
	int ret;
	char buf[RECVBUF_SIZE];
	char line_buf[RECVBUF_SIZE];
	int result = 0;
	int code;
	int rp = 0;		/* pBuf からの読み込み位置 */

	do {
		FD_ZERO(&rfds);
		FD_SET(pFtpCtrl->command_socket, &rfds);
		nfds = pFtpCtrl->command_socket;

		if (pFtpCtrl->data_socket != INVALID_SOCKET) {
			FD_ZERO(&wfds);
			FD_SET(pFtpCtrl->data_socket, &wfds);
			nfds = max(nfds, pFtpCtrl->data_socket);
			/* data_socket が、INVLAID_SOCKET のときは、
			   データを送信し終えて、ソケットを閉じた状態です。
			   command socket から、Transfer complete が来るのを待つ状態です */
		}

		to = pFtpCtrl->to;

		ret = select(nfds + 1, &rfds, (pFtpCtrl->data_socket == INVALID_SOCKET ? NULL : &wfds), NULL, &to);
		if (ret > 0) {
			if (FD_ISSET(pFtpCtrl->command_socket, &rfds)) {
				/* サーバーから、何か来たぞ? */
				ret = _recv(pFtpCtrl->command_socket, buf, sizeof(buf), 0);
				if (ret > 0) {
					buf[ret] = 0;
					if ((code = get_response(buf, line_buf)) > 0) {
						/* レスポンスが受信できたので、内容をチェック */
						if (code == 150) {
							/* 正常応答 */
							assert(0);
							break;
						} else if (code == 226) {
							/* 送信完了 */
							break;
						} else {
							/* 異常応答 */
							/* data waiting socket で接続を待つ必要なし! */
							goto quit_command;
						}
					}
				}
			}
			if (pFtpCtrl->data_socket != INVALID_SOCKET && FD_ISSET(pFtpCtrl->data_socket, &wfds)) {
				/* サーバーにデータを送るぞ */

				if (rp >= cbSize) {
					ret = 0;
				} else {
					if (cbSize - rp < sizeof(buf)) {
						memmove(buf, &pBuf[rp], cbSize - rp);
						rp += (ret = cbSize - rp);
					} else {
						memmove(buf, &pBuf[rp], sizeof(buf));
						rp += (ret = sizeof(buf));
					}
				}
				
				/*** ret = fread(buf, sizeof(char), sizeof(buf), fp); ***/

				if (ret > 0) {
					send(pFtpCtrl->data_socket, buf, ret, 0);
				} else {
					ftp_close_data_socket(pFtpCtrl);
				}
			}
		} else if (ret == 0) {
			printf("ftp_data_send_buffer: select でタイムアウト\n");
			result = -2;
			break;
		} else {
			printf("ftp_data_send_buffer: select エラー\n");
			result = -1;
			break;
		}
	} while (1);

quit_command:

	return result;
}

/*---------------------------------------------------------------------
	FTP用疑似クラスの構築
---------------------------------------------------------------------*/
void ftp_construct(FTP_CTRL* pFtpCtrl) {
	s_buffer[0] = '\0';		/* 受信バッファをクリア */

	pFtpCtrl->command_socket = INVALID_SOCKET;
	pFtpCtrl->data_waiting_socket = INVALID_SOCKET;
	pFtpCtrl->data_socket = INVALID_SOCKET;

	pFtpCtrl->to.tv_sec = 5;	/* 5秒 */
	pFtpCtrl->to.tv_usec = 0;

	pFtpCtrl->pasv_ip[0] = 0;
	pFtpCtrl->pasv_port = 0;
}

/*---------------------------------------------------------------------
	FTP 用疑似クラス::command socket を閉じる
---------------------------------------------------------------------*/
void ftp_close_command_socket(FTP_CTRL* pFtpCtrl) {
	shutdown(pFtpCtrl->command_socket, SD_BOTH);
	closesocket(pFtpCtrl->command_socket);
	pFtpCtrl->command_socket = INVALID_SOCKET;
}

/*---------------------------------------------------------------------
	FTP 用疑似クラス::data socket を閉じる
---------------------------------------------------------------------*/
void ftp_close_data_socket(FTP_CTRL* pFtpCtrl) {
	shutdown(pFtpCtrl->data_socket, SD_BOTH);
	closesocket(pFtpCtrl->data_socket);
	pFtpCtrl->data_socket = INVALID_SOCKET;
}

/*---------------------------------------------------------------------
	FTP用疑似クラス::data waiting socket を閉じる
---------------------------------------------------------------------*/
void ftp_close_data_waiting_socket(FTP_CTRL* pFtpCtrl) {
	shutdown(pFtpCtrl->data_waiting_socket, SD_BOTH);
	closesocket(pFtpCtrl->data_waiting_socket);
	pFtpCtrl->data_waiting_socket = INVALID_SOCKET;
}

/*---------------------------------------------------------------------
	FTP用疑似クラスの破棄
---------------------------------------------------------------------*/
void ftp_destroy(FTP_CTRL* pFtpCtrl) {
	/*
	pFtpCtrl->command_socket      = socket_gracefully_close(pFtpCtrl->command_socket);
	pFtpCtrl->data_socket         = socket_gracefully_close(pFtpCtrl->data_socket);
	pFtpCtrl->data_waiting_socket = socket_gracefully_close(pFtpCtrl->data_waiting_socket);
	*/

	ftp_close_command_socket(pFtpCtrl);
	ftp_close_data_socket(pFtpCtrl);
	ftp_close_data_waiting_socket(pFtpCtrl);
}

/*---------------------------------------------------------------------
	FTP用疑似クラス::data waiting socket を優美に閉じる
---------------------------------------------------------------------*/
void ftp_close_data_waiting_socket_gracefully(FTP_CTRL* pFtpCtrl) {
	pFtpCtrl->data_waiting_socket = socket_gracefully_close(pFtpCtrl->data_waiting_socket);
}

/*---------------------------------------------------------------------
	FTP 用疑似クラス::data socket を優美に閉じる
---------------------------------------------------------------------*/
void ftp_close_data_socket_gracefully(FTP_CTRL* pFtpCtrl) {
	pFtpCtrl->data_socket = socket_gracefully_close(pFtpCtrl->data_socket);
}
