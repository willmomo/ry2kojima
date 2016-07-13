/*-----------------------------------------------------------------------------
	Visual Studio ���g���Ƃ��́A���̃\�[�X�t�@�C���ɁA�v�����R���p�C���ς�
	�w�b�_���g��Ȃ��悤�ɐݒ肵�Ă��������B
-----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------
  [note]
  <2010/06/10 r.kojima>
  �^�C���A�E�g���ςɂ����BFTP_CTRL::to ��ݒ肷���ok�B
  �f�t�H���g�̃^�C���A�E�g�́A5 �b�B

  <2010/06/07 r.kojima>
  \n ���g���ƁAOS-9000�Ő����������ł��Ȃ����Ƃ������̂ŁA
  \x0a ���g���悤�ɂ��悤�B
---------------------------------------------------------------------*/
#if defined(_WIN32)
#pragma warning(disable : 4996)
#endif

#include "ftp_cmd.h"
#include "__os__.h"


/*---------------------------------------------------------------------
	�O���[�o���ϐ�
---------------------------------------------------------------------*/
static char s_buffer[RECVBUF_SIZE];	/* �\�P�b�g���\�z����ۂɁA�o�b�t�@�������������̂ŃO���[�o����(���� get_response �̒�) */
static int s_recv_status = 0;		/* */

int ftp_get_recv_status() { return s_recv_status; }

/*---------------------------------------------------------------------
	���҂����f�[�^������������ԂŎ�M���鎖�����邽�ߕW���� recv() ��
	����ɗp�ӂ��܂���
---------------------------------------------------------------------*/
static int _recv(int s, char *buf, int len, int flags)
{
	int n, rdy, ret;

	n = 0;
	rdy = _gs_rdy(s);						/* ��M�ς݃f�[�^�� */
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
	�z�X�g������AIP�A�h���X���擾����
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
	�o�b�t�@�����s���o��

	[�߂�l]
	0 = �s�����o����
	-1 = �s�����o���Ȃ�
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
	��M�o�b�t�@���烌�X�|���X�����o��

	[�߂�l]
	���X�|���X�R�[�h��Ԃ��B
	���X�|���X�s�����o���Ȃ������Ƃ��́A-1��Ԃ��B
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
	command socket ����̃��X�|���X����M����

	[�߂�l]
	-1 = select �ŃG���[
	-2 = select �Ń^�C���A�E�g
	-3 = recv �ŃG���[
	-4 = recv �� disconnected
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
					break;	/* ���X�|���X����M�ł����̂ŁA�I�� */
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
	�D���Ƀ\�P�b�g�����
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
	welcome message �̎�M
---------------------------------------------------------------------*/
int ftp_recv_welcome(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
		if (code == 220) {
			/* ���퉞�� */
			result = 0;
		} else {
			/* �ُ퉞�� */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	USER �R�}���h�̑��M
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
	USER �R�}���h�̉�����M
---------------------------------------------------------------------*/
int ftp_recv_USER(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
		if (code == 331) {
			/* ���퉞�� */
			result = 0;
		} else {
			/* �ُ퉞�� */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	USER �R�}���h�̑���M
---------------------------------------------------------------------*/
int ftp_command_USER(const FTP_CTRL *pFtpCtrl, const char *user) {
	ftp_send_USER(pFtpCtrl, user);
	return ftp_recv_USER(pFtpCtrl);
}

/*---------------------------------------------------------------------
	PASS �R�}���h�̑��M
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
	PASS �R�}���h�̉�����M
---------------------------------------------------------------------*/
int ftp_recv_PASS(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
		if (code == 230) {
			/* ���퉞�� */
			result = 0;
		} else {
			/* �ُ퉞�� */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	PASS �R�}���h�̑���M
---------------------------------------------------------------------*/
int ftp_command_PASS(const FTP_CTRL *pFtpCtrl, const char *pass) {
	ftp_send_PASS(pFtpCtrl, pass);	/* PASS �R�}���h���M */
	return ftp_recv_PASS(pFtpCtrl);
}

/*---------------------------------------------------------------------
	TYPE �R�}���h�̑��M
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
	TYPE �R�}���h�̉�����M
---------------------------------------------------------------------*/
int ftp_recv_TYPE(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
		if (code == 200) {
			/* ���퉞�� */
			result = 0;
		} else {
			/* �ُ퉞�� */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	TYPE �R�}���h�̑���M
---------------------------------------------------------------------*/
int ftp_command_TYPE(const FTP_CTRL *pFtpCtrl, int mode) {
	ftp_send_TYPE(pFtpCtrl, mode);	/* TYPE �R�}���h���M */
	return ftp_recv_TYPE(pFtpCtrl);
}

/*---------------------------------------------------------------------
	ABOR �R�}���h�̑��M(�g�p���тȂ�)
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
	ABOR �R�}���h�̉�����M(�g�p���тȂ�)
---------------------------------------------------------------------*/
int ftp_recv_ABOR(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
		if (code == 200) {
			/* ���퉞�� */
			result = 0;
		} else {
			/* �ُ퉞�� */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	ABOR �R�}���h�̑���M
---------------------------------------------------------------------*/
int ftp_command_ABOR(const FTP_CTRL *pFtpCtrl) {
	ftp_send_ABOR(pFtpCtrl);
	return ftp_recv_ABOR(pFtpCtrl);
}

/*---------------------------------------------------------------------
	LIST �R�}���h�̑��M
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
	LIST �R�}���h�̉�����M
---------------------------------------------------------------------*/
int ftp_recv_LIST(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
		if (code == 150) {
			/* ACTIVE���[�h�̐��퉞�� */
			result = 0;
		} else if (code == 125) {
			/* PASV���[�h�̐��퉞�� */
			result = 0;
		} else {
			/* �ُ퉞�� */

			/* ACTIVE ���[�h�̂Ƃ��́Adata waiting socket �Őڑ���҂����ɏ����� */
			/* PASV   ���[�h�̂Ƃ��́Adata socket �� gracefully close �����Ă͂����Ȃ��I */

			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	LIST �R�}���h�̑���M
---------------------------------------------------------------------*/
int ftp_command_LIST(const FTP_CTRL *pFtpCtrl, const char *parg) {
	ftp_send_LIST(pFtpCtrl, parg);	/* LIST �R�}���h���M */
	return ftp_recv_LIST(pFtpCtrl);
}

/*---------------------------------------------------------------------
	NLST �R�}���h�̑��M
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
	NLST �R�}���h�̉�����M
---------------------------------------------------------------------*/
int ftp_recv_NLST(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
		if (code == 150) {
			/* ACTIVE���[�h�̐��퉞�� */
			result = 0;
		} else if (code == 125) {
			/* PASV���[�h�̐��퉞�� */
			result = 0;
		} else {
			/* �ُ퉞�� */

			/* ACTIVE ���[�h�̂Ƃ��́Adata waiting socket �Őڑ���҂����ɏ����� */
			/* PASV   ���[�h�̂Ƃ��́Adata socket �� gracefully close �����Ă͂����Ȃ��I */

			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	NLST �R�}���h�̑���M
---------------------------------------------------------------------*/
int ftp_command_NLST(const FTP_CTRL *pFtpCtrl, const char *parg) {
	ftp_send_NLST(pFtpCtrl, parg);	/* LIST �R�}���h���M */
	return ftp_recv_NLST(pFtpCtrl);
}

/*---------------------------------------------------------------------
	PASV �R�}���h�̑��M
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
	PASV �R�}���h�̉�����M
---------------------------------------------------------------------*/
int ftp_recv_PASV(FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
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
			/* �ُ퉞�� */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	PASV �R�}���h�̑���M
---------------------------------------------------------------------*/
int ftp_command_PASV(FTP_CTRL *pFtpCtrl) {
	ftp_send_PASV(pFtpCtrl);
	return ftp_recv_PASV(pFtpCtrl);
}

/*---------------------------------------------------------------------
	PORT �R�}���h�̑��M
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
			/* �|�[�g�ԍ����擾 */
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
	PORT �R�}���h�̎�M
---------------------------------------------------------------------*/
int ftp_recv_PORT(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
		if (code == 200) {
			/* ���퉞�� */
			result = 0;
		} else {
			/* �ُ퉞�� */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	PORT �R�}���h�̑���M
---------------------------------------------------------------------*/
int ftp_command_PORT(const FTP_CTRL *pFtpCtrl) {
	ftp_send_PORT(pFtpCtrl);
	return ftp_recv_PORT(pFtpCtrl);
}

/*---------------------------------------------------------------------
	STOR �R�}���h�̑��M
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
	STOR �R�}���h�̉�����M
---------------------------------------------------------------------*/
int ftp_recv_STOR(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
		if (code == 150) {
			/* ACTIVE���[�h�̐��퉞�� */
			result = 0;
		} else if (code == 125) {
			/* PASV���[�h�̐��퉞�� */
			result = 0;
		} else {
			/* �ُ퉞�� */

			/* ACTIVE ���[�h�̂Ƃ��́Adata waiting socket �Őڑ���҂����ɏ����� */
			/* PASV   ���[�h�̂Ƃ��́Adata socket �� gracefully close �����Ă͂����Ȃ��I */

			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	STOR �R�}���h�̑���M
---------------------------------------------------------------------*/
int ftp_command_STOR(const FTP_CTRL *pFtpCtrl, const char *parg) {
	ftp_send_STOR(pFtpCtrl, parg);	/* STOR �R�}���h���M */
	return ftp_recv_STOR(pFtpCtrl);
}

/*---------------------------------------------------------------------
	RETR �R�}���h�̑��M
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
	RETR �R�}���h�̉�����M
---------------------------------------------------------------------*/
int ftp_recv_RETR(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
		if (code == 150) {
			/* ACTIVE���[�h�̐��퉞�� */
			result = 0;
		} else if (code == 125) {
			/* PASV���[�h�̐��퉞�� */
			result = 0;
		} else {
			/* �ُ퉞�� */

			/* ACTIVE ���[�h�̂Ƃ��́Adata waiting socket �Őڑ���҂����ɏ����� */
			/* PASV   ���[�h�̂Ƃ��́Adata socket �� gracefully close �����Ă͂����Ȃ��I */

			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	RETR �R�}���h�̑���M
---------------------------------------------------------------------*/
int ftp_command_RETR(const FTP_CTRL *pFtpCtrl, const char *parg) {
	ftp_send_RETR(pFtpCtrl, parg);	/* RETR�R�}���h���M */
	return ftp_recv_RETR(pFtpCtrl);
}

/*---------------------------------------------------------------------
	DELE �R�}���h�̑��M
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
	DELE �R�}���h�̎�M
---------------------------------------------------------------------*/
int ftp_recv_DELE(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
		if (code == 200) {
			/* ���퉞�� */
			result = 0;
		} else {
			/* �ُ퉞�� */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	DELE �R�}���h�̑���M
---------------------------------------------------------------------*/
int ftp_command_DELE(const FTP_CTRL *pFtpCtrl, const char *remote_filename) {
	ftp_send_DELE(pFtpCtrl, remote_filename);	/* DELE �R�}���h���M */
	return ftp_recv_DELE(pFtpCtrl);
}

/*---------------------------------------------------------------------
	MDTM �R�}���h�̑��M
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
	MDTM �R�}���h�̎�M
---------------------------------------------------------------------*/
int ftp_recv_MDTM(const FTP_CTRL *pFtpCtrl, char* pBuf) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
		if (200 <= code && code < 300) {
			/* ���퉞�� */
			strcpy(pBuf, line_buf);
			result = 0;
		} else {
			/* �ُ퉞�� */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	MDTM �R�}���h�̑���M
---------------------------------------------------------------------*/
int ftp_command_MDTM(const FTP_CTRL *pFtpCtrl, const char *remote_filename, char* pBuf) {
	ftp_send_MDTM(pFtpCtrl, remote_filename);	/* DELE �R�}���h���M */
	return ftp_recv_MDTM(pFtpCtrl, pBuf);
}

/*---------------------------------------------------------------------
	XMKD �R�}���h�̑��M
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
	XMKD �R�}���h�̎�M
---------------------------------------------------------------------*/
int ftp_recv_XMKD(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
		if (200 <= code && code < 300) {
			/* 2xx �̉����́A���퉞�� */
			result = 0;
		} else {
			/* �ُ퉞�� */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	XMKD �R�}���h�̑���M
---------------------------------------------------------------------*/
int ftp_command_XMKD(const FTP_CTRL *pFtpCtrl, const char *remote_filename) {
	ftp_send_XMKD(pFtpCtrl, remote_filename);	/* XMKD �R�}���h���M */
	return ftp_recv_XMKD(pFtpCtrl);
}

/*---------------------------------------------------------------------
	XRMD �R�}���h�̑��M
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
	XRMD �R�}���h�̎�M
---------------------------------------------------------------------*/
int ftp_recv_XRMD(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
		if (200 <= code && code < 300) {
			/* 2xx �̉����́A���퉞�� */
			result = 0;
		} else {
			/* �ُ퉞�� */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	XRMD �R�}���h�̑���M
---------------------------------------------------------------------*/
int ftp_command_XRMD(const FTP_CTRL *pFtpCtrl, const char *remote_filename) {
	ftp_send_XRMD(pFtpCtrl, remote_filename);	/* XRMD �R�}���h���M */
	return ftp_recv_XRMD(pFtpCtrl);
}

/*---------------------------------------------------------------------
	RNFR �R�}���h�̑��M
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
	RNFR �R�}���h�̎�M
---------------------------------------------------------------------*/
int ftp_recv_RNFR(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
		if (200 <= code && code < 400) {
			/* 2xx �̉����́A���퉞�� */
			result = 0;
		} else {
			/* �ُ퉞�� */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	RNFR �R�}���h�̑���M
---------------------------------------------------------------------*/
int ftp_command_RNFR(const FTP_CTRL *pFtpCtrl, const char *from_filename) {
	ftp_send_RNFR(pFtpCtrl, from_filename);	/* RNFR �R�}���h���M */
	return ftp_recv_RNFR(pFtpCtrl);
}

/*---------------------------------------------------------------------
	RNTO �R�}���h�̑��M
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
	RNTO �R�}���h�̎�M
---------------------------------------------------------------------*/
int ftp_recv_RNTO(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
		if (200 <= code && code < 300) {
			/* 2xx �̉����́A���퉞�� */
			result = 0;
		} else {
			/* �ُ퉞�� */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	RNTO �R�}���h�̑���M
---------------------------------------------------------------------*/
int ftp_command_RNTO(const FTP_CTRL *pFtpCtrl, const char *to_filename) {
	ftp_send_RNTO(pFtpCtrl, to_filename);	/* RNTO �R�}���h���M */
	return ftp_recv_RNTO(pFtpCtrl);
}

/*---------------------------------------------------------------------
	SIZE �R�}���h�̑��M
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
	SIZE �R�}���h�̎�M

	response code �̌��̃t�@�C���T�C�Y��Ԃ��B
---------------------------------------------------------------------*/
int ftp_recv_SIZE(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
		if (code == 213) {
			/* ���퉞�� */
			result = atoi(&line_buf[4]);
		} else {
			/* �ُ퉞�� */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	SIZE �R�}���h�̑���M
---------------------------------------------------------------------*/
int ftp_command_SIZE(const FTP_CTRL *pFtpCtrl, const char *remote_filename) {
	ftp_send_SIZE(pFtpCtrl, remote_filename);
	return ftp_recv_SIZE(pFtpCtrl);
}

/*---------------------------------------------------------------------
	HELP �R�}���h�̑��M
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
	HELP �R�}���h�̎�M
---------------------------------------------------------------------*/
int ftp_recv_HELP(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result;
	int code;

	if ((code = ftp_recv_response(pFtpCtrl, line_buf)) > 0) {
		/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
		if (code == 200) {
			/* ���퉞�� */
			result = 0;
		} else {
			/* �ُ퉞�� */
			result = -1;
		}
	} else {
		result = -2;
	}

	return result;
}

/*---------------------------------------------------------------------
	HELP �R�}���h�̑���M
---------------------------------------------------------------------*/
int ftp_command_HELP(const FTP_CTRL *pFtpCtrl, const char *cmd) {
	ftp_send_HELP(pFtpCtrl, cmd);
	return ftp_recv_HELP(pFtpCtrl);
}

/*---------------------------------------------------------------------
	QUIT �R�}���h�̑��M
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
	QUIT �R�}���h�̉�����M
---------------------------------------------------------------------*/
int ftp_recv_QUIT(const FTP_CTRL *pFtpCtrl) {
	char line_buf[512];
	int result = 0;

	/* ���X�|���X�̎�M�͂��邪�A�������Ȃ� */
	ftp_recv_response(pFtpCtrl, line_buf);

	return result;
}

/*---------------------------------------------------------------------
	QUIT �R�}���h�̑���M
---------------------------------------------------------------------*/
int ftp_command_QUIT(const FTP_CTRL *pFtpCtrl) {
	ftp_send_QUIT(pFtpCtrl);		/* QUIT �R�}���h���M */
	return ftp_recv_QUIT(pFtpCtrl);
}

/*---------------------------------------------------------------------
	FTP �̐ڑ��V�[�P���X

	[����]
	serv	�T�[�o�[��
	user	���[�U�[��
	pass	�p�X���[�h

	[�߂�l]
	0 ����I��
	!0 �ُ�I��
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
		printf("connect ���s ( %d )\n", WSAGetLastError());
		pFtpCtrl->command_socket = closesocket(pFtpCtrl->command_socket);
		return -1;
	}

	/* welcome response �̎�M��҂� */
	if (ftp_recv_welcome(pFtpCtrl) != 0) {
		pFtpCtrl->command_socket = closesocket(pFtpCtrl->command_socket);
		return -1;
	}

	/* USER command �̑���M */
	if (ftp_command_USER(pFtpCtrl, user) != 0) {
		pFtpCtrl->command_socket = closesocket(pFtpCtrl->command_socket);
		return -1;
	}

	/* PASS command �̑���M */
	if (ftp_command_PASS(pFtpCtrl, pass) != 0) {
		pFtpCtrl->command_socket = closesocket(pFtpCtrl->command_socket);
		return -1;
	}

	return 0;
}

/*---------------------------------------------------------------------
	FTP �� data waiting socket �̏���

	[���l]
	�I�I�������s�𖳎����Ă��܂��I�I
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
	FTP �̃f�[�^�|�[�g�ڑ����F�V�[�P���X
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
	�f�[�^�|�[�g�ւ̐ڑ��V�[�P���X

	[���l]
	FTP�T�[�o�[�Ɏw�肳�ꂽ�|�[�g�ɐڑ�����A��A�̃V�[�P���X�������B
	PASV���[�h�̂Ƃ��Ɏg�p�B
---------------------------------------------------------------------*/
int ftp_data_connect(FTP_CTRL *pFtpCtrl) {
	struct sockaddr_in myname;

	printf("%s (%u) �ɐڑ����Ă��܂��B\n", pFtpCtrl->pasv_ip, pFtpCtrl->pasv_port);

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
	�f�[�^�|�[�g�ł̎�M����

	[�߂�l]
	-1 select �ŃG���[
	-2 select �Ń^�C���A�E�g
	-4 data socket �� disconnected
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
	opt.pd_iotime = pFtpCtrl->to * 100;						/* ��M�^�C���A�E�g�l��ݒ肷��(1�b) */
	_os_ss_popt(sock, opt_size, &opt);
#endif
#endif
	/* data socket �̊�����҂� */
	while (pFtpCtrl->data_socket != INVALID_SOCKET) {
		printf("[%d ms] ftp_data_recv: [data] CTRL(%d) DATA(%d)\n", GetTickCount(), pFtpCtrl->command_socket, pFtpCtrl->data_socket);
#if 0
#if defined(_OS9000)
		ret = recv(pFtpCtrl->data_socket, buf, sizeof(buf), 0);

		printf("ftp_data_recv: [data] DATA SOCKET RECV(%d BYTE)\n", ret);

		if (ret > 0) {
			ret = fwrite(buf, sizeof(char), ret, fp);
		} else if (ret == 0) {
			printf("ftp_data_recv: [data] �S��M����\n");
			ftp_close_data_socket(pFtpCtrl);
		} else {
			if (errno == ETIMEDOUT) {	/* ��M�^�C���A�E�g */
				printf("ftp_data_recv: [data] recv �Ń^�C���A�E�g\n");
				ftp_close_data_socket(pFtpCtrl);
				result = -2;
			} else {
				printf("ftp_data_recv: [data] recv �G���[ (%d)\n", errno);
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
		/* data_socket ���AINVLAID_SOCKET �̂Ƃ��́A
		   �f�[�^����M���I���āA�\�P�b�g��������Ԃł��B
		   command socket ����ATransfer complete ������̂�҂�Ԃł� */

		to = pFtpCtrl->to;

		ret = select(nfds + 1, &rfds, NULL, NULL, &to);

		printf("[%d ms] ftp_data_recv: [data] select return = %d.\n", GetTickCount(), ret);

		if (ret > 0) {
			if (FD_ISSET(pFtpCtrl->data_socket, &rfds)) {
				/* �T�[�o�[����f�[�^�������� */
				ret = recv(pFtpCtrl->data_socket, buf, sizeof(buf), 0);

				printf("ftp_data_recv: [data] DATA SOCKET RECV(%d BYTE)\n", ret);

				if (ret > 0) {
					ret = fwrite(buf, sizeof(char), ret, fp);
				} else {
					printf("ftp_data_recv: [data] �S��M����\n");
					ftp_close_data_socket(pFtpCtrl);
				}
			}
		} else if (ret == 0) {
			printf("ftp_data_recv: [data] select �Ń^�C���A�E�g\n");
			ftp_close_data_socket(pFtpCtrl);
			result = -2;
			break;
		} else {
			printf("ftp_data_recv: [data] select �G���[\n");
			ftp_close_data_socket(pFtpCtrl);
			result = -1;
			break;
		}
/*#endif*/
	}

	/* �R�}���h�\�P�b�g�̌���������M */
	do {
		printf("[%d ms] ftp_data_recv: [ctrl] CTRL(%d) DATA(%d)\n", GetTickCount(), pFtpCtrl->command_socket, pFtpCtrl->data_socket);
#if 0
#if defined(_OS9000)
		ret = _recv(pFtpCtrl->command_socket, buf, sizeof(buf), 0);

		printf("ftp_data_recv: [ctrl] CTRL SOCKET RECV(%d BYTE)\n", ret);

		if (ret > 0) {
			buf[ret] = 0;
			if ((code = get_response(buf, line_buf)) > 0) {
				/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
				if (code == 150) {
					/* ���퉞�� */
					assert(0);
					break;
				} else if (code == 226) {
					/* ���M���� */
					break;
				} else {
					/* �ُ퉞�� */
					/* data waiting socket �Őڑ���҂K�v�Ȃ�! */
					break;
				}
			}
		} else {
			if (errno == ETIMEDOUT) {	/* ��M�^�C���A�E�g */
				printf("ftp_data_recv: [ctrl] select �Ń^�C���A�E�g\n");
				result = -2;
			} else {
				printf("ftp_data_recv: [ctrl] select �G���[ (%d)\n", errno);
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
				/* �T�[�o�[����A����������? */
				ret = _recv(pFtpCtrl->command_socket, buf, sizeof(buf), 0);

				printf("ftp_data_recv: [ctrl] CTRL SOCKET RECV(%d BYTE)\n", ret);

				if (ret > 0) {
					buf[ret] = 0;
					if ((code = get_response(buf, line_buf)) > 0) {
						/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
						if (code == 150) {
							/* ���퉞�� */
							assert(0);
							break;
						} else if (code == 226) {
							/* ���M���� */
							break;
						} else {
							/* �ُ퉞�� */
							/* data waiting socket �Őڑ���҂K�v�Ȃ�! */
							break;
						}
					}
				}
			}
		} else if (ret == 0) {
			printf("ftp_data_recv: [ctrl] select �Ń^�C���A�E�g\n");
			result = -2;
			break;
		} else {
			printf("ftp_data_recv: [ctrl] select �G���[\n");
			result = -1;
			break;
		}
/*#endif*/
	} while (1);

	return result;
}

/*---------------------------------------------------------------------
	�f�[�^�|�[�g�ł̎�M����

	[�߂�l]
	-1 select �ŃG���[
	-2 select �Ń^�C���A�E�g
	-4 data socket �� disconnected
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
	opt.pd_iotime = pFtpCtrl->to * 100;						/* ��M�^�C���A�E�g�l��ݒ肷��(1�b) */
	_os_ss_popt(sock, opt_size, &opt);
#endif
#endif
	/* data socket �̊�����҂� */
	while (pFtpCtrl->data_socket != INVALID_SOCKET) {
		printf("[%d ms] ftp_data_recv2: [data] CTRL(%d) DATA(%d)\n", GetTickCount(), pFtpCtrl->command_socket, pFtpCtrl->data_socket);
#if 0
#if defined(_OS9000)
		ret = recv(pFtpCtrl->data_socket, buf, sizeof(buf), 0);

		printf("ftp_data_recv2: [data] DATA SOCKET RECV(%d BYTE)\n", ret);

		if (ret > 0) {
			ret = fwrite(buf, sizeof(char), ret, fp);
		} else if (ret == 0) {
			printf("ftp_data_recv2: [data] �S��M����\n");
			ftp_close_data_socket(pFtpCtrl);
		} else {
			if (errno == ETIMEDOUT) {	/* ��M�^�C���A�E�g */
				printf("ftp_data_recv2: [data] recv �Ń^�C���A�E�g\n");
				ftp_close_data_socket(pFtpCtrl);
				result = -2;
			} else {
				printf("ftp_data_recv2: [data] recv �G���[ (%d)\n", errno);
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
		/* data_socket ���AINVLAID_SOCKET �̂Ƃ��́A
		   �f�[�^����M���I���āA�\�P�b�g��������Ԃł��B
		   command socket ����ATransfer complete ������̂�҂�Ԃł� */

		to = pFtpCtrl->to;

		ret = select(nfds + 1, &rfds, NULL, NULL, &to);

		printf("[%d ms] ftp_data_recv2: [data] select return = %d.\n", GetTickCount(), ret);

		if (ret > 0) {
			if (FD_ISSET(pFtpCtrl->data_socket, &rfds)) {
				/* �T�[�o�[����f�[�^�������� */
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
					printf("ftp_data_recv2: [data] �S��M����\n");
					ftp_close_data_socket(pFtpCtrl);
				}
			}
		} else if (ret == 0) {
			printf("ftp_data_recv2: [data] select �Ń^�C���A�E�g\n");
			ftp_close_data_socket(pFtpCtrl);
			result = -2;
			break;
		} else {
			printf("ftp_data_recv2: [data] select �G���[\n");
			ftp_close_data_socket(pFtpCtrl);
			result = -1;
			break;
		}
/*#endif*/
	}

	/* �R�}���h�\�P�b�g�̌���������M */
	do {
		printf("[%d ms] ftp_data_recv2: [ctrl] CTRL(%d) DATA(%d)\n", GetTickCount(), pFtpCtrl->command_socket, pFtpCtrl->data_socket);
#if 0
#if defined(_OS9000)
		ret = _recv(pFtpCtrl->command_socket, buf, sizeof(buf), 0);

		printf("ftp_data_recv2: [ctrl] CTRL SOCKET RECV(%d BYTE)\n", ret);

		if (ret > 0) {
			buf[ret] = 0;
			if ((code = get_response(buf, line_buf)) > 0) {
				/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
				if (code == 150) {
					/* ���퉞�� */
					assert(0);
					break;
				} else if (code == 226) {
					/* ���M���� */
					break;
				} else {
					/* �ُ퉞�� */
					/* data waiting socket �Őڑ���҂K�v�Ȃ�! */
					break;
				}
			}
		} else {
			if (errno == ETIMEDOUT) {	/* ��M�^�C���A�E�g */
				printf("ftp_data_recv2: [ctrl] select �Ń^�C���A�E�g\n");
				result = -2;
			} else {
				printf("ftp_data_recv2: [ctrl] select �G���[ (%d)\n", errno);
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
				/* �T�[�o�[����A����������? */
				ret = _recv(pFtpCtrl->command_socket, buf, sizeof(buf), 0);

				printf("ftp_data_recv2: [ctrl] CTRL SOCKET RECV(%d BYTE)\n", ret);

				if (ret > 0) {
					buf[ret] = 0;
					if ((code = get_response(buf, line_buf)) > 0) {
						/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
						if (code == 150) {
							/* ���퉞�� */
							assert(0);
							break;
						} else if (code == 226) {
							/* ���M���� */
							break;
						} else {
							/* �ُ퉞�� */
							/* data waiting socket �Őڑ���҂K�v�Ȃ�! */
							break;
						}
					}
				}
			}
		} else if (ret == 0) {
			printf("ftp_data_recv2: [ctrl] select �Ń^�C���A�E�g\n");
			result = -2;
			break;
		} else {
			printf("ftp_data_recv2: [ctrl] select �G���[\n");
			result = -1;
			break;
		}
/*#endif*/
	} while (1);

	return result;
}

/*---------------------------------------------------------------------
	�f�[�^�|�[�g�ł̎�M����

	[�߂�l]
	-1 select �ŃG���[
	-2 select �Ń^�C���A�E�g
	-4 data socket �� disconnected
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
			/* data_socket ���AINVLAID_SOCKET �̂Ƃ��́A
			   �f�[�^����M���I���āA�\�P�b�g��������Ԃł��B
			   command socket ����ATransfer complete ������̂�҂�Ԃł� */
		}

		to = pFtpCtrl->to;

		ret = select(nfds + 1, &rfds, NULL, NULL, &to);

		printf("[%d ms] ftp_data_recv: select return = %d.\n", GetTickCount(), ret);

		if (ret > 0) {
			if (pFtpCtrl->data_socket != INVALID_SOCKET && FD_ISSET(pFtpCtrl->data_socket, &rfds)) {
				/* �T�[�o�[����f�[�^�������� */
				ret = recv(pFtpCtrl->data_socket, buf, sizeof(buf), 0);

				printf("ftp_data_recv: DATA SOCKET RECV(%d BYTE)\n", ret);

				if (ret > 0) {
					ret = fwrite(buf, sizeof(char), ret, fp);
				} else {
					ftp_close_data_socket(pFtpCtrl);
				}
			}
			if (FD_ISSET(pFtpCtrl->command_socket, &rfds)) {
				/* �T�[�o�[����A����������? */
				ret = _recv(pFtpCtrl->command_socket, buf, sizeof(buf), 0);

				printf("ftp_data_recv: CTRL SOCKET RECV(%d BYTE)\n", ret);

				if (ret > 0) {
					buf[ret] = 0;
					if ((code = get_response(buf, line_buf)) > 0) {
						/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
						if (code == 150) {
							/* ���퉞�� */
							assert(0);
							break;
						} else if (code == 226) {
							/* ���M���� */
							break;
						} else {
							/* �ُ퉞�� */
							/* data waiting socket �Őڑ���҂K�v�Ȃ�! */
							goto quit_command;
						}
					}
				}
			}
		} else if (ret == 0) {
			printf("ftp_data_recv: select �Ń^�C���A�E�g\n");
			result = -2;
			break;
		} else {
			printf("ftp_data_recv: select �G���[\n");
			result = -1;
			break;
		}
	} while (1);

quit_command:

	return result;
}

/*---------------------------------------------------------------------
	�f�[�^�|�[�g�ł̎�M����

	[�߂�l]
	-1 select �ŃG���[
	-2 select �Ń^�C���A�E�g
	-4 data socket �� disconnected
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
			/* data_socket ���AINVLAID_SOCKET �̂Ƃ��́A
			   �f�[�^����M���I���āA�\�P�b�g��������Ԃł��B
			   command socket ����ATransfer complete ������̂�҂�Ԃł� */
		}

		to = pFtpCtrl->to;

		ret = select(nfds + 1, &rfds, NULL, NULL, &to);

		printf("[%d ms] ftp_data_recv2: select return = %d.\n", GetTickCount(), ret);

		if (ret > 0) {
			if (pFtpCtrl->data_socket != INVALID_SOCKET && FD_ISSET(pFtpCtrl->data_socket, &rfds)) {
				/* �T�[�o�[����f�[�^�������� */
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
				/* �T�[�o�[����A����������? */
				ret = _recv(pFtpCtrl->command_socket, buf, sizeof(buf), 0);

				printf("ftp_data_recv2: CTRL SOCKET RECV(%d BYTE)\n", ret);

				if (ret > 0) {
					buf[ret] = 0;
					if ((code = get_response(buf, line_buf)) > 0) {
						/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
						if (code == 150) {
							/* ���퉞�� */
							assert(0);
							break;
						} else if (code == 226) {
							/* ���M���� */
							break;
						} else {
							/* �ُ퉞�� */
							/* data waiting socket �Őڑ���҂K�v�Ȃ�! */
							goto quit_command;
						}
					}
				}
			}
		} else if (ret == 0) {
			printf("ftp_data_recv2: select �Ń^�C���A�E�g\n");
			result = -2;
			break;
		} else {
			printf("ftp_data_recv2: select �G���[\n");
			result = -1;
			break;
		}
	} while (1);

quit_command:

	return result;
}

/*---------------------------------------------------------------------
	�f�[�^�|�[�g�ł̑��M����

	[�߂�l]
	-1 select �ŃG���[
	-2 select �Ń^�C���A�E�g
	-4 data socket �� disconnected
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
			/* data_socket ���AINVLAID_SOCKET �̂Ƃ��́A
			   �f�[�^�𑗐M���I���āA�\�P�b�g�������Ԃł��B
			   command socket ����ATransfer complete ������̂�҂�Ԃł� */
		}

		to = pFtpCtrl->to;

		ret = select(nfds + 1, &rfds, (pFtpCtrl->data_socket == INVALID_SOCKET ? NULL : &wfds), NULL, &to);
		if (ret > 0) {
			if (FD_ISSET(pFtpCtrl->command_socket, &rfds)) {
				/* �T�[�o�[����A����������? */
				ret = _recv(pFtpCtrl->command_socket, buf, sizeof(buf), 0);
				if (ret > 0) {
					buf[ret] = 0;
					if ((code = get_response(buf, line_buf)) > 0) {
						/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
						if (code == 150) {
							/* ���퉞�� */
							assert(0);
							break;
						} else if (code == 226) {
							/* ���M���� */
							break;
						} else {
							/* �ُ퉞�� */
							/* data waiting socket �Őڑ���҂K�v�Ȃ�! */
							goto quit_command;
						}
					}
				}
			}
			if (pFtpCtrl->data_socket != INVALID_SOCKET && FD_ISSET(pFtpCtrl->data_socket, &wfds)) {
				/* �T�[�o�[�Ƀf�[�^�𑗂邼 */
				ret = fread(buf, sizeof(char), sizeof(buf), fp);
				if (ret > 0) {
					send(pFtpCtrl->data_socket, buf, ret, 0);
				} else {
					ftp_close_data_socket(pFtpCtrl);
				}
			}
		} else if (ret == 0) {
			printf("ftp_data_send: select �Ń^�C���A�E�g\n");
			result = -2;
			break;
		} else {
			printf("ftp_data_send: select �G���[\n");
			result = -1;
			break;
		}
	} while (1);

quit_command:

	return result;
}

/*---------------------------------------------------------------------
	�f�[�^�|�[�g�ł̑��M����

	[�߂�l]
	-1 select �ŃG���[
	-2 select �Ń^�C���A�E�g
	-4 data socket �� disconnected

	[����]
	FILE* �̑���ɁA�o�b�t�@�ւ̃|�C���^�Ƃ��̃T�C�Y���󂯎��
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
	int rp = 0;		/* pBuf ����̓ǂݍ��݈ʒu */

	do {
		FD_ZERO(&rfds);
		FD_SET(pFtpCtrl->command_socket, &rfds);
		nfds = pFtpCtrl->command_socket;

		if (pFtpCtrl->data_socket != INVALID_SOCKET) {
			FD_ZERO(&wfds);
			FD_SET(pFtpCtrl->data_socket, &wfds);
			nfds = max(nfds, pFtpCtrl->data_socket);
			/* data_socket ���AINVLAID_SOCKET �̂Ƃ��́A
			   �f�[�^�𑗐M���I���āA�\�P�b�g�������Ԃł��B
			   command socket ����ATransfer complete ������̂�҂�Ԃł� */
		}

		to = pFtpCtrl->to;

		ret = select(nfds + 1, &rfds, (pFtpCtrl->data_socket == INVALID_SOCKET ? NULL : &wfds), NULL, &to);
		if (ret > 0) {
			if (FD_ISSET(pFtpCtrl->command_socket, &rfds)) {
				/* �T�[�o�[����A����������? */
				ret = _recv(pFtpCtrl->command_socket, buf, sizeof(buf), 0);
				if (ret > 0) {
					buf[ret] = 0;
					if ((code = get_response(buf, line_buf)) > 0) {
						/* ���X�|���X����M�ł����̂ŁA���e���`�F�b�N */
						if (code == 150) {
							/* ���퉞�� */
							assert(0);
							break;
						} else if (code == 226) {
							/* ���M���� */
							break;
						} else {
							/* �ُ퉞�� */
							/* data waiting socket �Őڑ���҂K�v�Ȃ�! */
							goto quit_command;
						}
					}
				}
			}
			if (pFtpCtrl->data_socket != INVALID_SOCKET && FD_ISSET(pFtpCtrl->data_socket, &wfds)) {
				/* �T�[�o�[�Ƀf�[�^�𑗂邼 */

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
			printf("ftp_data_send_buffer: select �Ń^�C���A�E�g\n");
			result = -2;
			break;
		} else {
			printf("ftp_data_send_buffer: select �G���[\n");
			result = -1;
			break;
		}
	} while (1);

quit_command:

	return result;
}

/*---------------------------------------------------------------------
	FTP�p�^���N���X�̍\�z
---------------------------------------------------------------------*/
void ftp_construct(FTP_CTRL* pFtpCtrl) {
	s_buffer[0] = '\0';		/* ��M�o�b�t�@���N���A */

	pFtpCtrl->command_socket = INVALID_SOCKET;
	pFtpCtrl->data_waiting_socket = INVALID_SOCKET;
	pFtpCtrl->data_socket = INVALID_SOCKET;

	pFtpCtrl->to.tv_sec = 5;	/* 5�b */
	pFtpCtrl->to.tv_usec = 0;

	pFtpCtrl->pasv_ip[0] = 0;
	pFtpCtrl->pasv_port = 0;
}

/*---------------------------------------------------------------------
	FTP �p�^���N���X::command socket �����
---------------------------------------------------------------------*/
void ftp_close_command_socket(FTP_CTRL* pFtpCtrl) {
	shutdown(pFtpCtrl->command_socket, SD_BOTH);
	closesocket(pFtpCtrl->command_socket);
	pFtpCtrl->command_socket = INVALID_SOCKET;
}

/*---------------------------------------------------------------------
	FTP �p�^���N���X::data socket �����
---------------------------------------------------------------------*/
void ftp_close_data_socket(FTP_CTRL* pFtpCtrl) {
	shutdown(pFtpCtrl->data_socket, SD_BOTH);
	closesocket(pFtpCtrl->data_socket);
	pFtpCtrl->data_socket = INVALID_SOCKET;
}

/*---------------------------------------------------------------------
	FTP�p�^���N���X::data waiting socket �����
---------------------------------------------------------------------*/
void ftp_close_data_waiting_socket(FTP_CTRL* pFtpCtrl) {
	shutdown(pFtpCtrl->data_waiting_socket, SD_BOTH);
	closesocket(pFtpCtrl->data_waiting_socket);
	pFtpCtrl->data_waiting_socket = INVALID_SOCKET;
}

/*---------------------------------------------------------------------
	FTP�p�^���N���X�̔j��
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
	FTP�p�^���N���X::data waiting socket ��D���ɕ���
---------------------------------------------------------------------*/
void ftp_close_data_waiting_socket_gracefully(FTP_CTRL* pFtpCtrl) {
	pFtpCtrl->data_waiting_socket = socket_gracefully_close(pFtpCtrl->data_waiting_socket);
}

/*---------------------------------------------------------------------
	FTP �p�^���N���X::data socket ��D���ɕ���
---------------------------------------------------------------------*/
void ftp_close_data_socket_gracefully(FTP_CTRL* pFtpCtrl) {
	pFtpCtrl->data_socket = socket_gracefully_close(pFtpCtrl->data_socket);
}
