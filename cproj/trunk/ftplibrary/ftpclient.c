/*-----------------------------------------------------------------------------
	Visual Studio を使うときは、このソースファイルに、プリンコンパイル済み
	ヘッダを使わないように設定してください。
-----------------------------------------------------------------------------*/

#if defined(_WIN32)
#pragma warning(disable : 4996)		/* OS9で警告になるのでこちら */
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <mbstring.h>				/* OS9でエラーになるのでこちら */
#include "resource.h"
#endif

#if defined(_OS9000)
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <modes.h>
#include <UNIX\os9def.h>

#define _mbstok	strtok				/* OS9では、漢字がちゃんと扱えない可能性が出ます */

#define MAX_PATH	MAXPATHLEN
#endif

#include <stdio.h>
#include <assert.h>
#include "__os__.h"
#include "ftp_cmd.h"

#define FTP_MAX_RETRY	3
#define FTP_RETRY_WAIT	0

/*-----------------------------------------------------------------------------
// put コマンド相当
// @return  0 正常終了
// @return -1 接続エラー
// @return -2 TYPE コマンドエラー
// @return -3 PORT コマンドエラー
// @return -4 STOR コマンドエラー
// @return -5 データポートの接続エラー
// @return -6 PASV コマンドエラー
// @return -7 データポートへの接続エラー
// @return -8 データ送信エラー
// @return -9 ファイルオープンエラー
-----------------------------------------------------------------------------*/
int ftp_put_file_one(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char *local_name) {
	FTP_CTRL ftpCtrl;
	FILE* fp;
	int result = 0;

	/* FTP用擬似クラスの構築 */
	ftp_construct(&ftpCtrl);

	printf("ftp_put_file_one(%s, %s, %s, %c, %d, %s, %s)\n", serv, user, pass, mode, pasv, retr, local_name);

	/* FTP の接続シーケンスを実施 */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* TYPE command の送受信 */
	if (ftp_command_TYPE(&ftpCtrl, mode) != 0) {
		result = -2;
		goto quit_command;
	}

	if (pasv == FTP_USE_ACTIVE) {
		/* ACTIVE モードの動作 */
		/* データコネクション用のソケットを作成して bind , listen しておく */
		ftp_listen_data_waiting_port(&ftpCtrl);

		/* PORT command の送受信 */
		if (ftp_command_PORT(&ftpCtrl) != 0) {
			result = -3;
			goto quit_command;
		}

		/* STOR command の送受信 */
		if (ftp_command_STOR(&ftpCtrl, retr) != 0) {
			result = -4;
			goto quit_command;
		}

		/* data waiting socket で接続を待つ */
		if (ftp_data_accept(&ftpCtrl) != 0) {
			result = -5;
			goto quit_command;
		}
	} else {
		/* PASSIVEモードの動作 */
		/* PASIV command の送受信 */
		if (ftp_command_PASV(&ftpCtrl) != 0) {
			result = -6;
			goto quit_command;
		}

		if (ftp_data_connect(&ftpCtrl) != 0) {
			result = -7;
			goto quit_command;
		}

		/* STOR command の送受信 */
		if (ftp_command_STOR(&ftpCtrl, retr) != 0) {
			/* この状態で、gracefully close を行うと、
			   応答が戻ってこないので、shutdown して、さっさとクローズしてしまう */
			ftp_close_data_socket(&ftpCtrl);

			result = -4;
			goto quit_command;
		}
	}

	/* command socket で受信を待ちつつ、data socket でファイルの内容を送信する */
	fp = fopen(local_name, "rb");
	if (fp) {
		if (ftp_data_send(&ftpCtrl, fp) != 0) {
			result = -8;
		}
		fclose(fp);
	} else {
		printf("ファイル '%s' がオープンできません。\n", local_name);
		result = -9;
	}


quit_command:

	/* QUIT command の送受信(レスポンスに対しては、何もしない) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp用擬似クラスの破棄 */
	ftp_destroy(&ftpCtrl);

	return result;
}

/*-----------------------------------------------------------------------------
// put コマンド相当
// @return  0 正常終了
// @return -1 接続エラー
// @return -2 TYPE コマンドエラー
// @return -3 PORT コマンドエラー
// @return -4 STOR コマンドエラー
// @return -5 データポートの接続エラー
// @return -6 PASV コマンドエラー
// @return -7 データポートへの接続エラー
// @return -8 データ送信エラー
// @return -9 ファイルオープンエラー
-----------------------------------------------------------------------------*/
int ftp_put_buffer_one(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char *pBuf, size_t cbSize) {
	FTP_CTRL ftpCtrl;
	int result = 0;

	/* FTP用擬似クラスの構築 */
	ftp_construct(&ftpCtrl);

	printf("ftp_put_buffer_one(%s, %s, %s, %c, %d, %s, %p, %u)\n", serv, user, pass, mode, pasv, retr, pBuf, cbSize);

	/* FTP の接続シーケンスを実施 */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* TYPE command の送受信 */
	if (ftp_command_TYPE(&ftpCtrl, mode) != 0) {
		result = -2;
		goto quit_command;
	}

	if (pasv == FTP_USE_ACTIVE) {
		/* ACTIVE モードの動作 */
		/* データコネクション用のソケットを作成して bind , listen しておく */
		ftp_listen_data_waiting_port(&ftpCtrl);

		/* PORT command の送受信 */
		if (ftp_command_PORT(&ftpCtrl) != 0) {
			result = -3;
			goto quit_command;
		}

		/* STOR command の送受信 */
		if (ftp_command_STOR(&ftpCtrl, retr) != 0) {
			result = -4;
			goto quit_command;
		}

		/* data waiting socket で接続を待つ */
		if (ftp_data_accept(&ftpCtrl) != 0) {
			result = -5;
			goto quit_command;
		}
	} else {
		/* PASSIVEモードの動作 */
		/* PASIV command の送受信 */
		if (ftp_command_PASV(&ftpCtrl) != 0) {
			result = -6;
			goto quit_command;
		}

		if (ftp_data_connect(&ftpCtrl) != 0) {
			result = -7;
			goto quit_command;
		}

		/* STOR command の送受信 */
		if (ftp_command_STOR(&ftpCtrl, retr) != 0) {
			/* この状態で、gracefully close を行うと、
			   応答が戻ってこないので、shutdown して、さっさとクローズしてしまう */
			ftp_close_data_socket(&ftpCtrl);

			result = -4;
			goto quit_command;
		}
	}

	/* command socket で受信を待ちつつ、data socket でファイルの内容を送信する */
	if (ftp_data_send_buffer(&ftpCtrl, pBuf, cbSize) != 0) {
		result = -8;
	}

quit_command:

	/* QUIT command の送受信(レスポンスに対しては、何もしない) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp用擬似クラスの破棄 */
	ftp_destroy(&ftpCtrl);

	return result;
}

/*-----------------------------------------------------------------------------
// put コマンド相当
// @returns         最後に実行した ftp_get_file 関数の戻り値。
-----------------------------------------------------------------------------*/
int ftp_put_file(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char *local_name) {
	int i;
	int ret;

	for (i = 0; i <= FTP_MAX_RETRY; i++) {
		ret = ftp_put_file_one(serv, user, pass, mode, pasv, retr, local_name);
		if (ret == 0) {
			break;
		}

		syslog("ftp_put_file(%s, %s, %s, %d, %d, %s, %s) try %d error.",
			serv, user, pass, mode, pasv, retr, local_name, (i + 1));
		DebugPrintf(5, "ftp_put_file(%s, %s, %s, %d, %d, %s, %s) try %d error.\n",
			serv, user, pass, mode, pasv, retr, local_name, (i + 1));

		if (FTP_RETRY_WAIT >= 10)	Sleep(FTP_RETRY_WAIT);
	}

	return ret;
}

/*-----------------------------------------------------------------------------
// put コマンド相当
// @returns         最後に実行した ftp_get_file 関数の戻り値。
-----------------------------------------------------------------------------*/
int ftp_put_buffer(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char *pBuf, size_t cbSize) {
	int i;
	int ret;

	for (i = 0; i <= FTP_MAX_RETRY; i++) {
		ret = ftp_put_buffer_one(serv, user, pass, mode, pasv, retr, pBuf, cbSize);
		if (ret == 0) {
			break;
		}

		if (FTP_RETRY_WAIT >= 10)	Sleep(FTP_RETRY_WAIT);
	}

	return ret;
}


/*-----------------------------------------------------------------------------
// get コマンド相当
// @return -1 接続エラー
// @return -2 TYPE コマンドエラー
// @return -3 PORT コマンドエラー
// @return -4 RETR コマンドエラー
// @return -5 データポート接続待ちエラー
// @return -6 PASV コマンドエラー
// @return -7 データポートへの接続エラー
// @return -8 データ受信エラー
// @return -9 ファイルオープンエラー
-----------------------------------------------------------------------------*/
int ftp_get_file_one(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char *local_name) {
	FTP_CTRL ftpCtrl;
	FILE *fp = NULL;
	int result = 0;

	/* FTP用擬似クラスの構築 */
	ftp_construct(&ftpCtrl);

	printf("ftp_get_file_one(%s, %s, %s, %c, %d, %s, %s)\n", serv, user, pass, mode, pasv, retr, local_name);

	/* FTP の接続シーケンスを実施 */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* TYPE command の送受信 */
	if (ftp_command_TYPE(&ftpCtrl, mode) != 0) {
		result = -2;
		goto quit_command;
	}

	/* ローカルのファイルを準備する。
	   ファイルの準備ができないときは、早々にQUITで終了 */
	if ((fp = fopen(local_name, "wb")) == NULL) {
		printf("ファイル '%s' がオープンできません。\n", local_name);
		result = -9;
		goto quit_command;
	}

	if (pasv == FTP_USE_ACTIVE) {
		/* ACTIVE モードの動作 */
		/* データコネクション用のソケットを作成して bind , listen しておく */
		ftp_listen_data_waiting_port(&ftpCtrl);

		/* PORT command の送受信 */
		if (ftp_command_PORT(&ftpCtrl) != 0) {
			result = -3;
			goto quit_command;
		}

		/* RETR command の送受信 */
		if (ftp_command_RETR(&ftpCtrl, retr) != 0) {
			result = -4;
			goto quit_command;
		}

		/* data waiting socket で接続を待つ */
		if (ftp_data_accept(&ftpCtrl) != 0) {
			result = -5;
			goto quit_command;
		}
	} else {
		/* PASSIVEモードの動作 */
		/* PASIV command の送受信 */
		if (ftp_command_PASV(&ftpCtrl) != 0) {
			result = -6;
			goto quit_command;
		}

		if (ftp_data_connect(&ftpCtrl) != 0) {
			result = -7;
			goto quit_command;
		}

		/* RETR command の送受信 */
		if (ftp_command_RETR(&ftpCtrl, retr) != 0) {
			/* この状態で、gracefully close を行うと、
			   応答が戻ってこないので、shutdown して、さっさとクローズしてしまう */
			ftp_close_data_socket(&ftpCtrl);

			result = -4;
			goto quit_command;
		}
	}

	/* command socket で受信を待ちつつ、data socket で受信データをファイルに保存 */
	if (ftp_data_recv(&ftpCtrl, fp) != 0) {
		result = -8;
	}

quit_command:
	if (fp) fclose(fp);

	/* QUIT command の送受信(レスポンスに対しては、何もしない) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp用擬似クラスの破棄 */
	ftp_destroy(&ftpCtrl);

	/* 正常終了以外は、作成してしまったかもしれないファイルの削除を試みる */
	if (result != 0) {
		remove(local_name);
	}

	return result;
}

/*-----------------------------------------------------------------------------
// get コマンド相当
// @returns         最後に実行した ftp_get_file 関数の戻り値。
-----------------------------------------------------------------------------*/
int ftp_get_file(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char *local_name) {
	int i;
	int ret;

	for (i = 0; i <= FTP_MAX_RETRY; i++) {
		ret = ftp_get_file_one(serv, user, pass, mode, pasv, retr, local_name);
		if (ret == 0) {
			break;
		}

		syslog("ftp_get_file(%s, %s, %s, %d, %d, %s, %s) try %d error.",
			serv, user, pass, mode, pasv, retr, local_name, (i + 1));
		DebugPrintf(5, "ftp_get_file(%s, %s, %s, %d, %d, %s, %s) try %d error.\n",
			serv, user, pass, mode, pasv, retr, local_name, (i + 1));

		if (FTP_RETRY_WAIT >= 10)	Sleep(FTP_RETRY_WAIT);
	}

	return ret;
}

/*-----------------------------------------------------------------------------
// get コマンド相当
// @return >=0 受信サイズ。bufSizeと同じときは、バッファのサイズが足りない可能性あり。
// @return -1 接続エラー
// @return -2 TYPE コマンドエラー
// @return -3 PORT コマンドエラー
// @return -4 RETR コマンドエラー
// @return -5 データポート接続待ちエラー
// @return -6 PASV コマンドエラー
// @return -7 データポートへの接続エラー
// @return -8 データ受信エラー
-----------------------------------------------------------------------------*/
int ftp_get_file2(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, char* pBuf, int bufSize) {
	FTP_CTRL ftpCtrl;
	int result = 0;

	/* FTP用擬似クラスの構築 */
	ftp_construct(&ftpCtrl);

	printf("ftp_get_file2(%s, %s, %s, %c, %d, %s, %p, %d)\n", serv, user, pass, mode, pasv, retr, pBuf, bufSize);

	/* FTP の接続シーケンスを実施 */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* TYPE command の送受信 */
	if (ftp_command_TYPE(&ftpCtrl, mode) != 0) {
		result = -2;
		goto quit_command;
	}

	if (pasv == FTP_USE_ACTIVE) {
		/* ACTIVE モードの動作 */
		/* データコネクション用のソケットを作成して bind , listen しておく */
		ftp_listen_data_waiting_port(&ftpCtrl);

		/* PORT command の送受信 */
		if (ftp_command_PORT(&ftpCtrl) != 0) {
			result = -3;
			goto quit_command;
		}

		/* RETR command の送受信 */
		if (ftp_command_RETR(&ftpCtrl, retr) != 0) {
			result = -4;
			goto quit_command;
		}

		/* data waiting socket で接続を待つ */
		if (ftp_data_accept(&ftpCtrl) != 0) {
			result = -5;
			goto quit_command;
		}
	} else {
		/* PASSIVEモードの動作 */
		/* PASIV command の送受信 */
		if (ftp_command_PASV(&ftpCtrl) != 0) {
			result = -6;
			goto quit_command;
		}

		if (ftp_data_connect(&ftpCtrl) != 0) {
			result = -7;
			goto quit_command;
		}

		/* RETR command の送受信 */
		if (ftp_command_RETR(&ftpCtrl, retr) != 0) {
			/* この状態で、gracefully close を行うと、
			   応答が戻ってこないので、shutdown して、さっさとクローズしてしまう */
			ftp_close_data_socket(&ftpCtrl);

			result = -4;
			goto quit_command;
		}
	}

	/* command socket で受信を待ちつつ、data socket で受信データをファイルに保存 */
	result = ftp_data_recv2(&ftpCtrl, pBuf, bufSize);
	if (result < 0) {
		result = -8;
	}

quit_command:

	/* QUIT command の送受信(レスポンスに対しては、何もしない) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp用擬似クラスの破棄 */
	ftp_destroy(&ftpCtrl);

	return result;
}

/*---------------------------------------------------------------------
	dir 相当
---------------------------------------------------------------------*/
int ftp_list_file(const char *serv, const char *user, const char *pass, int pasv, const char* retr) {
	FTP_CTRL ftpCtrl;

	/* FTP用擬似クラスの構築 */
	ftp_construct(&ftpCtrl);

	printf("ftp_list_file(%s, %s, %s, %d, %s)\n", serv, user, pass, pasv, retr);

	/* FTP の接続シーケンスを実施 */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* TYPE command の送受信(ASCII固定) */
	if (ftp_command_TYPE(&ftpCtrl, FTP_MODE_ASCII) != 0) {
		goto quit_command;
	}

	if (pasv == FTP_USE_ACTIVE) {
		/* ACTIVE モードの動作 */
		/* データコネクション用のソケットを作成して bind , listen しておく */
		ftp_listen_data_waiting_port(&ftpCtrl);

		/* PORT command の送受信 */
		if (ftp_command_PORT(&ftpCtrl) != 0) {
			goto quit_command;
		}

		/* LIST command の送受信 */
		if (ftp_command_LIST(&ftpCtrl, retr) != 0) {
			goto quit_command;
		}

		/* data waiting socket で接続を待つ */
		if (ftp_data_accept(&ftpCtrl) != 0) {
			goto quit_command;
		}
	} else {
		/* PASSIVEモードの動作 */
		/* PASIV command の送受信 */
		if (ftp_command_PASV(&ftpCtrl) != 0) {
			goto quit_command;
		}

		if (ftp_data_connect(&ftpCtrl) != 0) {
			goto quit_command;
		}

		/* LIST command の送受信 */
		if (ftp_command_LIST(&ftpCtrl, retr) != 0) {
			/* この状態で、gracefully close を行うと、
			   応答が戻ってこないので、shutdown して、さっさとクローズしてしまう */
			ftp_close_data_socket(&ftpCtrl);

			goto quit_command;
		}
	}

	/* command socket で受信を待ちつつ、data socket で受信データを画面に表示 */
	ftp_data_recv(&ftpCtrl, stdout);

quit_command:

	/* QUIT command の送受信(レスポンスに対しては、何もしない) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp用擬似クラスの破棄 */
	ftp_destroy(&ftpCtrl);

	return 0;
}

/*---------------------------------------------------------------------
	ディレクトリリストを取得する(対応するコマンドなし)
---------------------------------------------------------------------*/
int ftp_nlst_file(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr) {
	FTP_CTRL ftpCtrl;

	/* FTP用擬似クラスの構築 */
	ftp_construct(&ftpCtrl);

	printf("ftp_nlst_file(%s, %s, %s, %c, %d, %s)\n", serv, user, pass, mode, pasv, retr);

	/* FTP の接続シーケンスを実施 */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* TYPE command の送受信 */
	if (ftp_command_TYPE(&ftpCtrl, mode) != 0) {
		goto quit_command;
	}

	if (pasv == FTP_USE_ACTIVE) {
		/* ACTIVE モードの動作 */
		/* データコネクション用のソケットを作成して bind , listen しておく */
		ftp_listen_data_waiting_port(&ftpCtrl);

		/* PORT command の送受信 */
		if (ftp_command_PORT(&ftpCtrl) != 0) {
			goto quit_command;
		}

		/* NLST command の送受信 */
		if (ftp_command_NLST(&ftpCtrl, retr) != 0) {
			goto quit_command;
		}

		/* data waiting socket で接続を待つ */
		if (ftp_data_accept(&ftpCtrl) != 0) {
			goto quit_command;
		}
	} else {
		/* PASSIVEモードの動作 */
		/* PASIV command の送受信 */
		if (ftp_command_PASV(&ftpCtrl) != 0) {
			goto quit_command;
		}

		if (ftp_data_connect(&ftpCtrl) != 0) {
			goto quit_command;
		}

		/* NLST command の送受信 */
		if (ftp_command_NLST(&ftpCtrl, retr) != 0) {
			/* この状態で、gracefully close を行うと、
			   応答が戻ってこないので、shutdown して、さっさとクローズしてしまう */
			ftp_close_data_socket(&ftpCtrl);

			goto quit_command;
		}
	}

	/* command socket で受信を待ちつつ、data socket で受信データを画面に表示 */
	ftp_data_recv(&ftpCtrl, stdout);

quit_command:

	/* QUIT command の送受信(レスポンスに対しては、何もしない) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp用擬似クラスの破棄 */
	ftp_destroy(&ftpCtrl);

	return 0;
}


/*-----------------------------------------------------------------------------
// ディレクトリリストを取得する(対応するコマンドなし)
// @return  0 正常終了
// @return -1 接続エラー
// @return -2 TYPE コマンドエラー
// @return -3 PORT コマンドエラー
// @return -4 NLST コマンドエラー
// @return -5 データポート接続待ちエラー
// @return -6 PASV コマンドエラー
// @return -7 データポートへの接続エラー
// @return -8 データ受信エラー
// @return -9 ファイルオープンエラー
-----------------------------------------------------------------------------*/
int ftp_nlst_file2_one(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char* outputFile) {
	FTP_CTRL ftpCtrl;
	FILE* fp;
	int result = 0;

	/* FTP用擬似クラスの構築 */
	ftp_construct(&ftpCtrl);

	printf("ftp_nlst_file2_one(%s, %s, %s, %c, %d, %s, %s)\n", serv, user, pass, mode, pasv, retr, outputFile);

	/* FTP の接続シーケンスを実施 */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* TYPE command の送受信 */
	if (ftp_command_TYPE(&ftpCtrl, mode) != 0) {
		result = -2;
		goto quit_command;
	}

	if (pasv == FTP_USE_ACTIVE) {
		/* ACTIVE モードの動作 */
		/* データコネクション用のソケットを作成して bind , listen しておく */
		ftp_listen_data_waiting_port(&ftpCtrl);

		/* PORT command の送受信 */
		if (ftp_command_PORT(&ftpCtrl) != 0) {
			result = -3;
			goto quit_command;
		}

		/* NLST command の送受信 */
		if (ftp_command_NLST(&ftpCtrl, retr) != 0) {
			result = -4;
			goto quit_command;
		}

		/* data waiting socket で接続を待つ */
		if (ftp_data_accept(&ftpCtrl) != 0) {
			result = -5;
			goto quit_command;
		}
	} else {
		/* PASSIVEモードの動作 */
		/* PASIV command の送受信 */
		if (ftp_command_PASV(&ftpCtrl) != 0) {
			result = -6;
			goto quit_command;
		}

		if (ftp_data_connect(&ftpCtrl) != 0) {
			result = -7;
			goto quit_command;
		}

		/* NLST command の送受信 */
		if (ftp_command_NLST(&ftpCtrl, retr) != 0) {
			/* この状態で、gracefully close を行うと、
			   応答が戻ってこないので、shutdown して、さっさとクローズしてしまう */
			ftp_close_data_socket(&ftpCtrl);

			result = -4;
			goto quit_command;
		}
	}

	/* command socket で受信を待ちつつ、data socket で受信データを画面に表示 */
	fp = fopen(outputFile, "wb");
	if (fp != NULL) {
		if (ftp_data_recv(&ftpCtrl, fp) != 0) {
			result = -8;
		}
		fclose(fp);
	} else {
		printf("%s がオープンできません。\n", outputFile);
		result = -9;
	}

quit_command:

	/* QUIT command の送受信(レスポンスに対しては、何もしない) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp用擬似クラスの破棄 */
	ftp_destroy(&ftpCtrl);

	/* 正常終了以外は、作成してしまったかもしれないファイルの削除を試みる */
	if (result != 0) {
		remove(outputFile);
	}

	return result;
}


/*-----------------------------------------------------------------------------
// ディレクトリリストを取得する(対応するコマンドなし)
// @returns         最後に実行した ftp_nlst_file2 関数の戻り値。
-----------------------------------------------------------------------------*/
int ftp_nlst_file2(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char* outputFile) {
	int i;
	int ret;

	for (i = 0; i <= FTP_MAX_RETRY; i++) {
		ret = ftp_nlst_file2_one(serv, user, pass, mode, pasv, retr, outputFile);
		if (ret == 0) {
			break;
		}

		syslog("ftp_nlst_file2(%s, %s, %s, %d, %d, %s, %s) try %d error.",
			serv, user, pass, mode, pasv, retr, outputFile, (i + 1));
		DebugPrintf(5, "ftp_nlst_file2(%s, %s, %s, %d, %d, %s, %s) try %d error.\n",
			serv, user, pass, mode, pasv, retr, outputFile, (i + 1));

		if (FTP_RETRY_WAIT >= 10)	Sleep(FTP_RETRY_WAIT);
	}

	return ret;
}

/*-----------------------------------------------------------------------------
// del コマンド相当
// @return  0 正常終了
// @return -1 接続エラー
// @return -2 dele コマンドエラー
-----------------------------------------------------------------------------*/
int ftp_dele_file(const char *serv, const char *user, const char *pass, const char* remote_filename) {
	FTP_CTRL ftpCtrl;
	int result = 0;

	/* ftp用擬似クラスの構築 */
	ftp_construct(&ftpCtrl);

	printf("ftp_dele_file(%s, %s, %s, %s)\n", serv, user, pass, remote_filename);

	/* FTP サーバーに接続 */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* DELE command の送受信(結果にかかわらずQUIT処理に移行) */
	if (ftp_command_DELE(&ftpCtrl, remote_filename) != 0) {
		result = -2;
	}

	/* QUIT command の送受信(レスポンスに対しては、何もしない) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp用擬似クラスの破棄 */
	ftp_destroy(&ftpCtrl);

	return result;
}


/*-----------------------------------------------------------------------------
// ファイルの更新日付取得
// @return  0 正常終了
// @return -1 接続エラー
// @return -2 dele コマンドエラー
-----------------------------------------------------------------------------*/
int ftp_mdtm_file(const char *serv, const char *user, const char *pass, const char* remote_filename, char* response) {
	FTP_CTRL ftpCtrl;
	int result = 0;

	/* ftp用擬似クラスの構築 */
	ftp_construct(&ftpCtrl);

	printf("ftp_mdtm_file(%s, %s, %s, %s, %s)\n", serv, user, pass, remote_filename, response);

	/* FTP サーバーに接続 */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* MDTM command の送受信(結果にかかわらずQUIT処理に移行) */
	if (ftp_command_MDTM(&ftpCtrl, remote_filename, response) != 0) {
		result = -2;
	}

	/* QUIT command の送受信(レスポンスに対しては、何もしない) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp用擬似クラスの破棄 */
	ftp_destroy(&ftpCtrl);

	return result;
}


/*-----------------------------------------------------------------------------
// xmkd コマンド相当
// @return  0 正常終了
// @return -1 接続エラー
// @return -2 xmkd コマンドエラー
//
// 必要に応じてパスの中間ディレクトリを作成します。
-----------------------------------------------------------------------------*/
int ftp_xmkd_file_one(const char *serv, const char *user, const char *pass, const char* remote_filename) {
	FTP_CTRL ftpCtrl;
	int result = 0;
	char make_path[MAX_PATH];	/* 実際に作成するディレクトリ */
	char work[MAX_PATH];
	char* p = NULL;				/* フォルダを検索するためのパス */

	/* ftp用擬似クラスの構築 */
	ftp_construct(&ftpCtrl);

	printf("ftp_xmkd_file_one(%s, %s, %s, %s)\n", serv, user, pass, remote_filename);

	/* FTP サーバーに接続 */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* 作業バッファにコピー */
	strcpy(work, remote_filename);

	/* 指定パスが絶対パスの場合は、先頭に / を入れた状態で処理を開始 */
	if (work[0] == '/') {
		strcpy(make_path, "/");
		p = &work[1];
	} else {
		strcpy(make_path, "");
		p = &work[0];
	}

	p = _mbstok(p, "/");
	while (p) {
		strcat(make_path, p);

		/* 最後の XMKD command の成否を返すようにする */
		result = 0;
		if (ftp_command_XMKD(&ftpCtrl, make_path) != 0) {
			result = -2;
		}

		p = _mbstok(NULL, "/");
		strcat(make_path, "/");
	}

	/* QUIT command の送受信(レスポンスに対しては、何もしない) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp用擬似クラスの破棄 */
	ftp_destroy(&ftpCtrl);

	return result;
}

/*-----------------------------------------------------------------------------
// xmkd コマンド相当
// @returns         最後に実行した ftp_get_file 関数の戻り値。
-----------------------------------------------------------------------------*/
int ftp_xmkd_file(const char *serv, const char *user, const char *pass, const char* remote_filename) {
	int i;
	int ret;

	for (i = 0; i <= FTP_MAX_RETRY; i++) {
		ret = ftp_xmkd_file_one(serv, user, pass, remote_filename);

		printf("ftp_xmkd_file: ret(%d) recv_status(%d)\n", ret, ftp_get_recv_status());

		if (ret == 0 || (ret != 0 && ftp_get_recv_status() != -2)) {
			break;
		}

		syslog("ftp_xmkd_file(%s, %s, %s, %s) try %d error.",
			serv, user, pass, remote_filename, (i + 1));
		DebugPrintf(5, "ftp_xmkd_file(%s, %s, %s, %s) try %d error.\n",
			serv, user, pass, remote_filename, (i + 1));

		if (FTP_RETRY_WAIT >= 10)	Sleep(FTP_RETRY_WAIT);
	}

	return ret;
}

/*-----------------------------------------------------------------------------
// xrmd コマンド相当
// @return  0 正常終了
// @return -1 接続エラー
// @return -2 xmkd コマンドエラー
-----------------------------------------------------------------------------*/
int ftp_xrmd_file(const char *serv, const char *user, const char *pass, const char* remote_filename) {
	FTP_CTRL ftpCtrl;
	int result = 0;

	/* ftp用擬似クラスの構築 */
	ftp_construct(&ftpCtrl);

	printf("ftp_xrmd_file(%s, %s, %s, %s)\n", serv, user, pass, remote_filename);

	/* FTP サーバーに接続 */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* XRMD command の送受信(結果にかかわらずQUIT処理に移行) */
	if (ftp_command_XRMD(&ftpCtrl, remote_filename) != 0) {
		result = -2;
	}

	/* QUIT command の送受信(レスポンスに対しては、何もしない) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp用擬似クラスの破棄 */
	ftp_destroy(&ftpCtrl);

	return result;
}

/*-----------------------------------------------------------------------------
// RNFR + RNTO コマンド相当
// @return  0 正常終了
// @return -1 接続エラー
// @return -2 xmkd コマンドエラー
-----------------------------------------------------------------------------*/
int ftp_rename_file(const char *serv, const char *user, const char *pass, const char* from_filename, const char* to_filename) {
	FTP_CTRL ftpCtrl;
	int result = 0;

	/* ftp用擬似クラスの構築 */
	ftp_construct(&ftpCtrl);

	printf("ftp_rename_file(%s, %s, %s, %s, %s)\n", serv, user, pass, from_filename, to_filename);

	/* FTP サーバーに接続 */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* RNFR を送受信 */
	if (ftp_command_RNFR(&ftpCtrl, from_filename) != 0) {
		result = -2;
	}

	/* RNTO を送受信 */
	if (result == 0 && ftp_command_RNTO(&ftpCtrl, to_filename) != 0) {
		result = -3;
	}

	/* QUIT command の送受信(レスポンスに対しては、何もしない) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp用擬似クラスの破棄 */
	ftp_destroy(&ftpCtrl);

	return result;
}

/*---------------------------------------------------------------------
	SIZEコマンドでファイルのサイズを取得
---------------------------------------------------------------------*/
int ftp_size_file(const char *serv, const char *user, const char *pass, const char* remote_filename) {
	FTP_CTRL ftpCtrl;
	int result = 0;

	/* ftp用擬似クラスの構築 */
	ftp_construct(&ftpCtrl);

	printf("ftp_size_file(%s, %s, %s, %s)\n", serv, user, pass, remote_filename);

	/* FTP サーバーに接続 */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* SIZE command の送受信(結果にかかわらずQUIT処理に移行) */
	result = ftp_command_SIZE(&ftpCtrl, remote_filename);

	/* QUIT command の送受信(レスポンスに対しては、何もしない) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp用擬似クラスの破棄 */
	ftp_destroy(&ftpCtrl);

	/* return 0; */
	return result;
}

/*---------------------------------------------------------------------
	HELPコマンドでコマンドリストを取得
---------------------------------------------------------------------*/
int ftp_help(const char *serv, const char *user, const char *pass, const char *cmd) {
	FTP_CTRL ftpCtrl;

	/* ftp用擬似クラスの構築 */
	ftp_construct(&ftpCtrl);

	printf("ftp_help(%s, %s, %s, %s)\n", serv, user, pass, cmd);

	/* FTP サーバーに接続 */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* HELP command の送受信(結果にかかわらずQUIT処理に移行) */
	ftp_command_HELP(&ftpCtrl, cmd);

	/* QUIT command の送受信(レスポンスに対しては、何もしない) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp用擬似クラスの破棄 */
	ftp_destroy(&ftpCtrl);

	return 0;
}

/*---------------------------------------------------------------------
	プログラムスタート
---------------------------------------------------------------------*/
#if defined(TEST)
int main(int argc, char** argv) {
	char* p = NULL;
	int size = 0;

#if defined(_WIN32)
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif

#if 1
	printf("テストスタート\n");

	//ftp_nlst_file2("192.168.120.23", "sankoftp", "UvGyVM", FTP_MODE_ASCII, FTP_USE_ACTIVE, "/mnt/array1", "out.txt");
#if 0
	{
		FILE* fp;
		fp = fopen("out.txt", "r");
		if (fp) {
			char buf[512];

			while (fgets(buf, sizeof(buf), fp) != NULL) {
				int i;
				char remote[512];
				char local[512];

				buf[strlen(buf) - 1] = '\0';

				strcpy(remote, buf);
				strcat(strcpy(local, "."), buf);
				for (i = 0; local[i] != '\0'; i++) {
					if (local[i] == '/') {
						local[i] = '\\';
					}
				}

				ftp_get_file("192.168.150.2", "wssadmin", "wssadmin", FTP_MODE_ASCII, FTP_USE_ACTIVE, remote, local);
			}

			fclose(fp);
		}
	}
#endif
#endif
	if (argc != 9) {
		printf("%s <mdtm|get|get2|put|list|nlst|nlst2|dele|xmkd|xrmd|size|help>\n"
			"<サーバー> <ユーザー> <パスワード> <A or I> <A or P> <リモートファイル> <ローカルファイル>\n", argv[0]);
		return 0;
	}

	/*
	*/

	if (strcmp(argv[1], "get") == 0) {
		ftp_get_file(
			argv[2],	/* サーバー */
			argv[3],	/* ユーザー */
			argv[4],	/* パスワード */
			argv[5][0],	/* 'A' or 'I' */
			(argv[6][0] == 'A' ? FTP_USE_ACTIVE : FTP_USE_PASSIVE),	/* 'A' or 'P' */
			argv[7],	/* リモートファイル */
			argv[8]		/* ローカルファイル */
		);
	} else if (strcmp(argv[1], "get2") == 0) {
		char buf[8192];

		ftp_get_file2(
			argv[2],	/* サーバー */
			argv[3],	/* ユーザー */
			argv[4],	/* パスワード */
			argv[5][0],	/* 'A' or 'I' */
			(argv[6][0] == 'A' ? FTP_USE_ACTIVE : FTP_USE_PASSIVE),	/* 'A' or 'P' */
			argv[7],	/* リモートファイル */
			buf,		/* バッファへのポインタ */
			sizeof(buf)	/* バッファのサイズ */
		);
	} else if (strcmp(argv[1], "put") == 0) {
		ftp_put_file(
			argv[2],	/* サーバー */
			argv[3],	/* ユーザー */
			argv[4],	/* パスワード */
			argv[5][0],	/* 'A' or 'I' */
			(argv[6][0] == 'A' ? FTP_USE_ACTIVE : FTP_USE_PASSIVE),	/* 'A' or 'P' */
			argv[7],	/* リモートファイル */
			argv[8]		/* ローカルファイル */
		);
	} else if (strcmp(argv[1], "list") == 0) {
		ftp_list_file(
			argv[2],	/* サーバー */
			argv[3],	/* ユーザー */
			argv[4],	/* パスワード */
			(argv[6][0] == 'A' ? FTP_USE_ACTIVE : FTP_USE_PASSIVE),	/* 'A' or 'P' */
			argv[7]		/* リモートファイル */
		);
	} else if (strcmp(argv[1], "nlst") == 0) {
		ftp_nlst_file(
			argv[2],	/* サーバー */
			argv[3],	/* ユーザー */
			argv[4],	/* パスワード */
			argv[5][0],	/* 'A' or 'I' */
			(argv[6][0] == 'A' ? FTP_USE_ACTIVE : FTP_USE_PASSIVE),	/* 'A' or 'P' */
			argv[7]		/* リモートファイル */
		);
	} else if (strcmp(argv[1], "nlst2") == 0) {
		ftp_nlst_file2(
			argv[2],	/* サーバー */
			argv[3],	/* ユーザー */
			argv[4],	/* パスワード */
			argv[5][0],	/* 'A' or 'I' */
			(argv[6][0] == 'A' ? FTP_USE_ACTIVE : FTP_USE_PASSIVE),	/* 'A' or 'P' */
			argv[7],	/* リモートファイル */
			argv[8]		/* ローカルファイル */
		);
	} else if (strcmp(argv[1], "dele") == 0) {
		ftp_dele_file(
			argv[2],	/* サーバー */
			argv[3],	/* ユーザー */
			argv[4],	/* パスワード */
			argv[7]		/* リモートファイル */
		);
	} else if (strcmp(argv[1], "mdtm") == 0) {
		char response[80];
		ftp_mdtm_file(
			argv[2],	/* サーバー */
			argv[3],	/* ユーザー */
			argv[4],	/* パスワード */
			argv[7],		/* リモートファイル */
			response
		);
	} else if (strcmp(argv[1], "xmkd") == 0) {
		ftp_xmkd_file(
			argv[2],	/* サーバー */
			argv[3],	/* ユーザー */
			argv[4],	/* パスワード */
			argv[7]		/* リモートファイル */
		);
	} else if (strcmp(argv[1], "xrmd") == 0) {
		ftp_xrmd_file(
			argv[2],	/* サーバー */
			argv[3],	/* ユーザー */
			argv[4],	/* パスワード */
			argv[7]		/* リモートファイル */
		);
	} else if (strcmp(argv[1], "size") == 0) {
		ftp_size_file(
			argv[2],	/* サーバー */
			argv[3],	/* ユーザー */
			argv[4],	/* パスワード */
			argv[7]		/* リモートファイル */
		);
	} else if (strcmp(argv[1], "help") == 0) {
		ftp_help(
			argv[2],	/* サーバー */
			argv[3],	/* ユーザー */
			argv[4],	/* パスワード */
			argv[7]		/* コマンド名 */
		);
	}

#if defined(_WIN32)
	WSACleanup();
#endif
	return 0;
}
#endif

#if 0	/*toshi*/

#define CPL 16

void
hexdump(char *ptr, int len)
{
	int		col;
	long	i;
	char	c, chr[CPL+1], *buf = ptr;
	
	chr[CPL] = '\0';
	printf("ptr=0x%08X len=0x%08X(%d)\n", (unsigned long)ptr, len, len);
	for (i = 0; i < len; i++) {
		col = i % CPL;
		c = buf[i];
		
		if (col == 0)
			printf("%08X: ", i);
		printf("%02X ", (unsigned char)c);
		if (c >= 0x7f || c <= 0x1f)
			chr[col] = '.';
		else
			chr[col] = c;
		if (col == CPL - 1) {
			printf("[%s]\n", chr);
		}
	}
	printf("\n");
}

int main(int argc, char** argv)
{
	char buf[10];

	memset(buf, 0, sizeof(buf));

/*		ftp_put_file("192.168.130.113", "super", "user", 'I', FTP_USE_ACTIVE, "/r0/cc.bin", "/mhc1/cc.bin"); */
/*		ftp_get_file("192.168.130.113", "super", "user", 'I', FTP_USE_ACTIVE, "/r0/cc.bin", "/mhc1/dd.bin"); */
/*		ftp_get_file2("192.168.130.113", "super", "user", 'I', FTP_USE_ACTIVE, "/mhc1/aa.bin", buf, sizeof(buf));*/
/*		ftp_nlst_file2("192.168.130.113", "super", "user", 'I', FTP_USE_ACTIVE, "/r0", "/mhc1/dir.txt");*/
/*		ftp_dele_file("192.168.130.113", "super", "user", "/mhc1/dd.bin");*/
/*		ftp_xmkd_file("172.17.101.10", "wssadmin", "wssadmin", "aaa");*/
/*		ftp_xmkd_file("172.17.101.19", "admin", "password", "array1/backup/bbb");*/
/*		ftp_xrmd_file("172.17.101.10", "wssadmin", "wssadmin", "aaa");*/
/*		ftp_xrmd_file("192.168.130.113", "super", "user", "aaa");*/

/*	hexdump(buf, sizeof(buf));*/
}
#endif	/*toshi*/

