/**************************************************************************************/
/***																				***/
/***									wsft.h										***/
/***																				***/
/**************************************************************************************/

#include	<windows.h>
#include	<stdio.h>
//#include	<process.h>
#include <shlwapi.h>

#include "wsslog.h"

#ifndef	_WSFT_H	//	{{
#define	_WSFT_H

//	ファイル情報のフラグの値
#define WSFT_INFO_FILE_FLAG_WRITE	0x0001
#define WSFT_INFO_FILE_FLAG_RESERVE	0x0002

#define WSFT_OPT_SAADDR		"WSFTOPT_SADDR"
#define WSFT_OPT_RPORT		"WSFTOPT_RPORT"
#define WSFT_OPT_WPORT		"WSFTOPT_WPORT"
#define WSFT_OPT_TIMEOUT	"WSFTOPT_TIMEOUT"
#define WSFT_OPT_RETRY		"WSFTOPT_RETRY"

extern __declspec(thread) char g_szExeName[_MAX_PATH];	// Exeのフルパス名

int _stdcall wsft_start( HWND hwnd ) ;
int _stdcall wsft_setserv( unsigned long addr, int rport, int wport ) ;
int _stdcall wsft_setservstr(const char *servstr, int rport, int wport) ;
int _stdcall wsft_setopt( char *opt, unsigned long value ) ;
unsigned long _stdcall wsft_getopt( const char *opt ) ;
int _stdcall wsft_infofile( unsigned short fid, unsigned short rec, unsigned long *fsize, unsigned short *fr, unsigned short *flag ) ;
int _stdcall wsft_readfile( unsigned short fid, unsigned long rec, char *buf, unsigned long bufsize ) ;
int _stdcall wsft_writefile( unsigned short fid, unsigned short rec, char *buf, unsigned long bufsize ) ;
int _stdcall wsft_readaddr( unsigned long address, unsigned short size, char *buf, unsigned long bufsize ) ;
int _stdcall wsft_writeaddr( unsigned long address, char *buf, unsigned long bufsize ) ;
int _stdcall wsft_end( void ) ;
unsigned short _stdcall wsft_getver( void ) ;
long _stdcall wsft_dllversion();
int _stdcall wsft_resume();
int _stdcall wsft_stop();

//	2001/09/26日 追加 --->
int GetHost( unsigned long addr ) ;
//	<--- ここまで

//>>>**02.03.14 misako add**
int _stdcall wsft_read_rec( unsigned short fid, unsigned long starec, unsigned long endrec,char *buf, unsigned long recsize ) ;
//<<<**02.03.14 misako**
#endif	//	_WSFT_H	}}
