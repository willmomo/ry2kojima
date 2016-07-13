/*---------------------------------------------------------------------
	ftp_cmd.c 用のヘッダ
---------------------------------------------------------------------*/
#if !defined(__FTP_CMD_H)
#define __FTP_CMD_H


#if defined(_WIN32)
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#endif

#if defined(_OS9000)
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <modes.h>
#include <UNIX\os9def.h>
#include <ctype.h>
#include <stdlib.h>
#include <sg_codes.h>
#include <spf\spf.h>
#include "__os__.h"
#endif

#include <stdio.h>
#include <assert.h>

#if defined(USE_WSSLOG)
#include "wsslog.h"
#endif

#define FTP_MODE_ASCII		'A'
#define FTP_MODE_BINARY		'I'

#define FTP_USE_ACTIVE		0
#define FTP_USE_PASSIVE		1

#define RECVBUF_SIZE	4096

/* ソケット用のヘルパ関数 */

unsigned long get_ipaddr(const char *hostname);
int read_line(char *buffer, char *line_buf);
int get_response(const char *buf, char *line_buf);
SOCKET socket_gracefully_close(SOCKET s);

/* 疑似クラスとして扱う為の構造体と関数 */

typedef struct {
	SOCKET command_socket;
	SOCKET data_waiting_socket;
	SOCKET data_socket;

	/* select に使用するタイムアウト時間(すべてのやり取りで使用される) */
	struct timeval to;	/* default: 5sec */

	/* PASV コマンドを使った場合の応答を保存するエリア */
	char pasv_ip[32];
	unsigned short pasv_port;
} FTP_CTRL;

#if defined(__cplusplus)
extern "C" {
#endif

int ftp_get_recv_status();

void ftp_construct(FTP_CTRL *pFtpCtrl);
void ftp_destroy(FTP_CTRL *pFtpCtrl);
void ftp_close_data_waiting_socket(FTP_CTRL *pFtpCtrl);
void ftp_close_data_waiting_socket_gracefully(FTP_CTRL *pFtpCtrl);
void ftp_close_data_socket(FTP_CTRL *pFtpCtrl);
void ftp_close_data_socket_gracefully(FTP_CTRL *pFtpCtrl);

/* 特定コマンドの送信と受信を処理する関数 */

int ftp_command_USER(const FTP_CTRL *pFtpCtrl, const char *user);
int ftp_command_PASS(const FTP_CTRL *pFtpCtrl, const char *pass);
int ftp_command_TYPE(const FTP_CTRL *pFtpCtrl, int mode);
int ftp_command_PASV(      FTP_CTRL *pFtpCtrl);
int ftp_command_DELE(const FTP_CTRL *pFtpCtrl, const char *remote_filename);
int ftp_command_MDTM(const FTP_CTRL *pFtpCtrl, const char *remote_filename, char* pBuf);
int ftp_command_XMKD(const FTP_CTRL *pFtpCtrl, const char *remote_filename);
int ftp_command_XRMD(const FTP_CTRL *pFtpCtrl, const char *remote_filename);
int ftp_command_RNFR(const FTP_CTRL *pFtpCtrl, const char *from_filename);
int ftp_command_RNTO(const FTP_CTRL *pFtpCtrl, const char *to_filename);
int ftp_command_QUIT(const FTP_CTRL *pFtpCtrl);
int ftp_command_PORT(const FTP_CTRL *pFtpCtrl);
int ftp_command_ABOR(const FTP_CTRL *pFtpCtrl);
int ftp_command_SIZE(const FTP_CTRL *pFtpCtrl, const char *remote_filename);
int ftp_command_HELP(const FTP_CTRL *pFtpCtrl, const char *cmd);

int ftp_command_LIST(const FTP_CTRL *pFtpCtrl, const char *parg);
int ftp_command_NLST(const FTP_CTRL *pFtpCtrl, const char *parg);
int ftp_command_STOR(const FTP_CTRL *pFtpCtrl, const char *parg);
int ftp_command_RETR(const FTP_CTRL *pFtpCtrl, const char *parg);

/* 一連のシーケンスを処理する関数 */

int ftp_connect(FTP_CTRL *pFtpCtrl, const char *serv, const char *user, const char* pass);
int ftp_listen_data_waiting_port(FTP_CTRL *pFtpCtrl);
int ftp_data_accept(FTP_CTRL *pFtpCtrl);
int ftp_data_connect(FTP_CTRL *pFtpCtrl);
int ftp_data_recv(FTP_CTRL *pFtpCtrl, FILE* fp);
int ftp_data_recv2(FTP_CTRL *pFtpCtrl, char* pBuf, int bufSize);
int ftp_data_send(FTP_CTRL *pFtpCtrl, FILE* fp);
int ftp_data_send_buffer(FTP_CTRL *pFtpCtrl, const char *pBuf, size_t cbSize);

/* ftpclient.c で定義されるクライアントとしての処理関数 */

int ftp_put_file(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char *local_name);
int ftp_put_buffer(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char *pBuf, size_t cbSize);
int ftp_get_file(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char *local_name);
int ftp_get_file_retry(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char *local_name, int maxRetry, int nWait);
int ftp_get_file2(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, char* pBuf, int bufSize);
int ftp_list_file(const char *serv, const char *user, const char *pass, int pasv, const char* retr);
int ftp_nlst_file(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr);
int ftp_nlst_file2(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char* outputFile);
int ftp_nlst_file2_retry(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char* outputFile, int maxRetry, int nWait);
int ftp_dele_file(const char *serv, const char *user, const char *pass, const char* remote_filename);
int ftp_mdtm_file(const char *serv, const char *user, const char *pass, const char* remote_filename, char* response);
int ftp_xmkd_file(const char *serv, const char *user, const char *pass, const char* remote_filename);
int ftp_xrmd_file(const char *serv, const char *user, const char *pass, const char* remote_filename);
int ftp_rename_file(const char *serv, const char *user, const char *pass, const char* from_filename, const char* to_filename);
int ftp_size_file(const char *serv, const char *user, const char *pass, const char* remote_filename);
int ftp_help(const char *serv, const char *user, const char *pass, const char *cmd);

#if defined(_OS9000)
void syslog(char *format, ...);
void DebugPrintf(int LogLevel, char *format, ...);
#endif

#if defined(__cplusplus)
}
#endif

#endif
