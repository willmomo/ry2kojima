/**************************************************************************************/
/***																				***/
/***									wsft_err.h									***/
/***																				***/
/**************************************************************************************/

#ifndef	_WSFT_ERR_H	//	{{
#define	_WSFT_ERR_H

//	wsft.dll のエラーコード定義
#define WSFTERR_BUSY_I			0x0001
#define WSFTERR_BUSY_R			0x0002
#define WSFTERR_BUSY_S			0x0003
#define WSFTERR_BUSY_W			0x0004
#define WSFTERR_BUSY_E			0x0005

#define WSFTERR_BUFF			0x1001
#define WSFTERR_FRAME_SKIP		0x1002
#define WSFTERR_REC_TIMEOUT		0x1003
#define WSFTERR_NO_SUPPORT_SMF	0x1004
#define WSFTERR_ADDRESS			0x1005
#define WSFTERR_RECORD			0x1006
#define WSFTERR_FRAME			0x1007
#define WSFTERR_COMMAND			0x1008

#define WSFTERR_FILE_SIZE		0x2001

#define WSFTERR_TIMEOUT_I		0x2010
#define WSFTERR_TIMEOUT_R		0x2011
#define WSFTERR_TIMEOUT_S		0x2012
#define WSFTERR_TIMEOUT_W		0x2013
#define WSFTERR_TIMEOUT_E		0x2014

#define WSFTERR_REC_SIZE		0x2003

#define WSFTERR_INIT_WINSOCK	0x3001
#define WSFTERR_INIT_WSOCKVER	0x3002
#define WSFTERR_INIT_SOCKET		0x3003
#define WSFTERR_INIT_NO_INIT	0x3004
#define WSFTERR_INIT_MALLOC		0x3005
//	2001/09/20日 追加 --->
//	設定ファイルに必須項目が無いときのエラーを追加
#define WSFTERR_INIT_INIFILE	0x3006
#define WSFTERR_INIT_LOG		0x3007
//	<--- ここまで

#define WSFTERR_END_PROCCESS	0x4001

#define WSFTERR_OPTION_NAME		0x5001
#define WSFTERR_OPTION_TIMEOUT	0x5002

//	2001/09/26日 追加 --->
#define WSFTERR_OPTION_RPORT	0x5003
#define WSFTERR_OPTION_WPORT	0x5004
#define WSFTERR_OPTION_RETRY	0x5005
#define WSFTERR_OPTION_HOSTNAME	0x5006
//	<--- ここまで

#define WSFTERR_VERSION			0xF001

#define WSFTERR_BUFFSIZE		0x2005

#define WSFTERR_SEND_I			0x2020
#define WSFTERR_SEND_R			0x2021
#define WSFTERR_SEND_S			0x2022
#define WSFTERR_SEND_W			0x2023
#define WSFTERR_SEND_E			0x2024

#define WSFTERR_REC_I			0x2030
#define WSFTERR_REC_R			0x2031
#define WSFTERR_REC_S			0x2032
#define WSFTERR_REC_W			0x2033
#define WSFTERR_REC_E			0x2034

//	2001/09/26日 追加 --->
#define WSFTERR_NOT_SERVER		0x2040
//	<--- ここまで

#endif	//	_WSFT_ERR_H	}}
