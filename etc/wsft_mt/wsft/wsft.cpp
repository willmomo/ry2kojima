/**************************************************************************************/
/***																				***/
/***								wsft.cpp										***/
/***																				***/
/**************************************************************************************/
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include "wsft.h"
#include "wsft_def.h"
//　変更　***　FUMIYA.K  02.01.07 >>

#include "wsini_def.h"
#include "wsft_err.h"
#include "vbstructs.h"

#include "tracex.h"
#include "GetIniFile.h"

#include <map>
#include <string>

__declspec(thread) static St_SocketInfo SockInfo ;

#define FILE_COUNT_MAX		(10000)

#pragma data_seg(".shared")
LONG g_Suspended = TRUE;		// 通信再開ﾌﾗｸﾞ
#pragma data_seg()

extern DWORD g_dwTOCount[2];	//[0]:全受信発行回数 [1]:タイムアウト発生回数

std::string GetExeName()
{
	char szExeName[_MAX_PATH];	// Exeのフルパス名
	std::string exeName;

	// 実行モジュールのファイル名をグローバル変数に格納　**FUMIYA
	GetModuleFileName(NULL, szExeName, sizeof(szExeName));
	PathFindFileName(szExeName);

	exeName.assign(szExeName);

	return exeName;
}

/*--------------------------------------------------------------/
		ファイル番号を内部的なインデックスに変換する

	[引数]
		unsigned long	fid		;	SMFファイル番号

	[戻り値]
		ファイル番号に対応した０から始まるインデックス
/--------------------------------------------------------------*/
int FileNoToIndex( unsigned long fid )
{
	int		ret = 0 ;

	return((fid < FILE_COUNT_MAX) ?fid :0);

}

/*----------------------------------------------------------------------------------/
							トレース出力の開始

	[関数名]
		TraceStart

	[引数]
		なし

	[戻り値]
		int				:	==0 正常 !=0 エラー

/----------------------------------------------------------------------------------*/
int TraceStart()
{
	TRACEX_OPT opt;

	strcpy(opt.dir, "d:\\work");	// 出力先ディレクトリ
	strcpy(opt.pre, "wsft");		// ファイル名先頭
	strcpy(opt.suf, ".log");		// ファイル名後方
	TraceXStart(&opt);

	return 0;
}

/*--------------------------------------------------------------/
						設定ファイル読み込み

	[関数名]
		ReadIniFile

	[引数]
		StartFlag	:	Wsft_Start関数から呼ばれたかどうか
						古いログファイルの削除処理の有無が切り替わります

	[戻り値]
		== 0		:	正常
		!= 0		:	エラー(エラー番号)

/--------------------------------------------------------------*/
int ReadIniFile()
{
	const u_short VERSION = 0x100;		// このDLLがサポートする電文のバージョン

	TraceStart() ;

	// ヴァージョンに定数値入力　**FUMIYA
	SockInfo.ver = VERSION;

	// INIファイルよりMAXSIZE取得　**FUMIYA
	long lData;
	GetWinSousaInt("Wsft", "maxsize", &lData);
	SockInfo.maxsize = (unsigned short)lData;

	// INIファイルよりアドレス取得　**FUMIYA
	GetWinSousaStr("Wsft", "addr", SockInfo.Addr, sizeof(SockInfo.Addr));

	// デフォルト値の読込
	// INIファイルよりデフォルト読み込みポート取得　**FUMIYA
	GetWinSousaInt("Wsft", "Port_Read", &lData);
	SockInfo.IniInfo.rPort = (unsigned short)lData;

	// INIファイルよりデフォルト書込みポート取得　**FUMIYA
	GetWinSousaInt("Wsft", "Port_Write", &lData);
	SockInfo.IniInfo.wPort = (unsigned short)lData;

	// INIファイルよりデフォルトタイムアウト取得　**FUMIYA
	GetWinSousaInt("Wsft", "timeoutsec", &lData);
	SockInfo.IniInfo.timeout = lData;

	// INIファイルよりデフォルトリトライ回数取得　**FUMIYA
	GetWinSousaInt("Wsft", "retrycount", &lData);
	SockInfo.IniInfo.retrycount = lData;

	return 0;
}

/*--------------------------------------------------------------/
					プロトコル初期化処理

	[関数名]
		wsft_start

	[引数]
		HWND hwnd	:	呼出元のウィンドウハンドル

	[戻り値]
		== 0		:	正常
		!= 0		:	エラー(エラー番号)

/--------------------------------------------------------------*/
int __stdcall wsft_start(HWND hwnd)
{
	int	ret = 0 ;

	//	ログ出力
	TraceI("wsft_start() : 初期化処理を行います。");

	//	デフォルト値で設定しておく
	SockInfo.rPort = SockInfo.IniInfo.rPort ;
	SockInfo.wPort = SockInfo.IniInfo.wPort ;
	SockInfo.timeout = SockInfo.IniInfo.timeout ;
	SockInfo.retrycount = SockInfo.IniInfo.retrycount ;

	//	サーバー名も設定ファイルの内容で初期化する
	SockInfo.saAddr = getIPAddr( SockInfo.Addr );

	//	シーケンス番号を初期化
	SockInfo.seqnum = 0 ;
	SockInfo.StartFlag = TRUE  ;

	return ret ;
}

/*--------------------------------------------------------------/
		通信対象サーバIPアドレスの設定(str から登録)

	[関数名]
		wsft_set_serveraddr_str

	[引数]
		char				*str	:	対象サーバーIPの文字列

	[戻り値]
		== 0		:	正常
		!= 0		:	エラー(エラー番号)

/--------------------------------------------------------------*/
int wsft_set_serveraddr_str(const char *addrstr)
{
	int		ret = 0;

	//	サーバーの設定
	//	文字列の保存してIPアドレスを求める
	strcpy(SockInfo.Addr, addrstr);

	//
	SockInfo.saAddr = getIPAddr( SockInfo.Addr );
	if (SockInfo.saAddr == 0xFFFFFFFF) {
		TraceE("wsft_setserv(): ホスト名[%s]は解決できませんでした", SockInfo.Addr);
		ret = WSFTERR_OPTION_HOSTNAME;
	}

	return ret ;
}

/*--------------------------------------------------------------/
		通信対象サーバIPアドレスの設定(u_long から登録)

	[関数名]
		wsft_set_serveraddr

	[引数]
		u_long				addr	:	対象サーバーIP

	[戻り値]
		== 0		:	正常
		!= 0		:	エラー(エラー番号)

/--------------------------------------------------------------*/
int wsft_set_serveraddr(u_long addr)
{
	int		ret = 0;
	char	addrstr[64];

	//	u_longでセットされたアドレスを文字列に変換する
	//	wsft_setservstr()内でコールされている inet_addr() は
	//	0xAABBCCDDでも正常に動作する
	sprintf(addrstr, "0x%08lx", addr);
	return wsft_set_serveraddr_str(addrstr);
}

/*--------------------------------------------------------------/
					通信対象サーバーの設定

	[関数名]
		wsft_setserv

	[引数]
		char			*servstr	:	対象サーバーIPの文字列
		int				rport		:	読み込み用ポート番号
		int				wport		:	書き込み用ポート番号

	[戻り値]
		== 0		:	正常
		!= 0		:	エラー(エラー番号)

/--------------------------------------------------------------*/
int _stdcall wsft_setservstr(const char *servstr, int rport, int wport)
{
	int	ret = 0;

	//	必ずwsft_startを呼び出す必要がある
	if( SockInfo.StartFlag == FALSE ){
		TraceE("wsft_setservstr() : 初期化処理が行われていません。" ) ;
		ret = WSFTERR_INIT_NO_INIT ;
		return ret ;
	}

	ret = wsft_set_serveraddr_str(servstr);			// 文字列でサーバIP設定
	if (ret != 0)	return ret;

	ret = wsft_setopt(WSFT_OPT_RPORT, rport);		// 読込みポート
	if (ret != 0)	return ret;

	ret = wsft_setopt(WSFT_OPT_WPORT, wport);		// 書込みポート
	return ret ;
}
/*--------------------------------------------------------------/
					通信対象サーバーの設定

	[関数名]
		wsft_setserv

	[引数]
		unsigned long	addr	:	対象サーバーのアドレス
		int				rport	:	読み込み用ポート番号
		int				wport	:	書き込み用ポート番号

	[戻り値]
		== 0		:	正常
		!= 0		:	エラー(エラー番号)

/--------------------------------------------------------------*/
int _stdcall wsft_setserv(u_long addr, int rport, int wport)
{
	int	ret = 0;

	//	必ずwsft_startを呼び出す必要がある
	if( SockInfo.StartFlag == FALSE ){
		TraceE("wsft_setserv() : 初期化処理が行われていません。" ) ;
		ret = WSFTERR_INIT_NO_INIT ;
		return ret ;
	}

	ret = wsft_set_serveraddr(addr);				// 数値でサーバIP設定
	if (ret != 0)	return ret;

	ret = wsft_setopt(WSFT_OPT_RPORT, rport);		// 読込みポート
	if (ret != 0)	return ret;

	ret = wsft_setopt(WSFT_OPT_WPORT, wport);		// 書込みポート
	return ret ;
}

/*--------------------------------------------------------------/
				タイムアウトなどのオプションの設定

	[関数名]
		wsft_setopt

	[引数]
		char	*opt	:	設定するオプションの指定
		int		value	:	設定するオプションの値

	[戻り値]
		== 0		:	正常
		!= 0		:	エラー(エラー番号)

/--------------------------------------------------------------*/
int _stdcall wsft_setopt(char *opt, u_long value)
{
	int	ret = 0 ;

	//	必ずwsft_startを呼び出す必要がある
	if( SockInfo.StartFlag == FALSE ){
		TraceE("wsft_setopt() : 初期化処理が行われていません。" ) ;
		ret = WSFTERR_INIT_NO_INIT ;
		return ret ;
	}

	//	通信先アドレスの設定
	if( stricmp( opt, WSFT_OPT_SAADDR ) == 0 ){
		TraceD("wsft_setopt() : 通信するサーバーを [%ld] に設定します。", value ) ;
		ret = wsft_set_serveraddr(value);

	//	読み込みポート変更
	} else if( stricmp( opt, WSFT_OPT_RPORT ) == 0 ){
		// 読込みポート番号設定
		// 0: デフォルト値使用 / 1-65535: 設定 / その他: エラー(ポートは変更しない)
		if (value == 0) {
			SockInfo.rPort = SockInfo.IniInfo.rPort;
		} else if (1 <= value && value <= 65535) {
			SockInfo.rPort = (u_short)value;
		} else {
			TraceE("wsft_setopt(): 読込みポート[%d]は、無効。無視しました。", value);
			ret = WSFTERR_OPTION_RPORT;
		}

	//	書き込みポート変更
	} else if( stricmp( opt, WSFT_OPT_WPORT ) == 0 ){
		// 書込みポート番号設定
		// 0: デフォルト値使用 / 1-65535: 設定 / その他: エラー(ポートは変更しない)
		if (value == 0) {
			SockInfo.wPort = SockInfo.IniInfo.wPort;
		} else if (1 <= value && value <= 65535) {
			SockInfo.wPort = (u_short)value;
		} else {
			TraceE("wsft_setopt(): 書込みポート[%d]は、無効。無視しました。", value);
			ret = WSFTERR_OPTION_WPORT;
		}

	//	タイムアウト時間
	} else if( stricmp( opt, WSFT_OPT_TIMEOUT ) == 0 ){
		if (SockInfo.timeout != (int)value) {
			// 値変更時のみログに残す
			TraceF("wsft_setopt() : タイムアウト時間を [%ld] に設定します。", value ) ;
		}
		SockInfo.timeout = value ;

	//	リトライ回数
	} else if( stricmp( opt, WSFT_OPT_RETRY ) == 0 ){
		TraceF("wsft_setopt() : リトライ回数を [%ld] に設定します。", value ) ;
		if( value <= 0 ){
			TraceE("wsft_setopt() : リトライ回数に範囲外の値 [%ld] が指定されました。", value ) ;
			return WSFTERR_OPTION_RETRY ;
		}else{
			SockInfo.retrycount = value ;
		}

	//	該当する項目名が存在しないとき
	} else {
		TraceE("wsft_setopt() : 指定された設定項目<%s>はサポートされていません。", opt ) ;
		ret = WSFTERR_OPTION_NAME ;
	}

	return ret ;
}


/*--------------------------------------------------------------/
			タイムアウトなどのオプションの設定値を取得

	[関数名]
		wsft_getopt

	[引数]
		char	*opt	:	取得するオプションの指定

	[戻り値]
		指定されたオプションの設定値

/--------------------------------------------------------------*/
u_long _stdcall wsft_getopt(const char* opt)
{
	unsigned long	ret = 0 ;

	//	必ずwsft_startを呼び出す必要がある
	if( SockInfo.StartFlag == FALSE ){
		TraceE("wsft_getopt() : 初期化処理が行われていません。" ) ;
		ret = -1 ;
		return ret ;
	}
	//	<--- ここまで

	//	通信先アドレスの設定
	if( stricmp( opt, WSFT_OPT_SAADDR ) == 0 ){
		ret = SockInfo.saAddr ;
		TraceD("wsft_getopt() : 通信先アドレス [%ld] を返します。", ret ) ;

	//	読み込みポート変更
	} else if( stricmp( opt, WSFT_OPT_RPORT ) == 0 ){
		ret = SockInfo.rPort ;
		TraceD("wsft_getopt() : 読み込み用ポート番号 [%ld] を返します。", ret ) ;

	//	書き込みポート変更
	} else if( stricmp( opt, WSFT_OPT_WPORT ) == 0 ){
		ret = SockInfo.wPort ;
		TraceD("wsft_getopt() : 書き込み用ポート番号 [%ld] を返します。", ret ) ;

	//	タイムアウト時間
	} else if( stricmp( opt, WSFT_OPT_TIMEOUT ) == 0 ){
		ret = SockInfo.timeout ;
		TraceD("wsft_getopt() : タイムアウト時間 [%ld] を返します。", ret ) ;

	//	リトライ回数
	} else if( stricmp( opt, WSFT_OPT_RETRY ) == 0 ){
		ret = SockInfo.retrycount ;
		TraceD("wsft_getopt() : リトライ回数 [%ld] を返します。", ret ) ;

	//	該当する項目名が存在しないとき
	} else {
		TraceE("wsft_getopt() : 指定された設定項目<%s>はサポートされていません。", opt ) ;
		//	エラー値は-1を返す
		ret = -1 ;
	}

	return ret ;
}


/*---------------------------------------------------------------------------/
						ファイル情報取得

	[関数名]
		wsft_infofile

	[引数]
		unsigned short	fid		:	情報を取得するファイルのsmfファイル番号
		unsigned short	rec		:	レコード指定(0の時は全レコード)
		unsigned long	*fsize	:	ファイル（レコード）サイズを返す
		unsigned long	*fr		:	通信時のフレーム数を返す
		unsigned long	*flag	:	フラグを返す
	[戻り値]
		== 0		:	正常
		!= 0		:	エラー(エラー番号)

/--------------------------------------------------------------------------*/
int _stdcall wsft_infofile(u_short fid, u_short rec, u_long* fsize, u_short *fr, u_short *flag)
{
	//	必ずwsft_startを呼び出す必要がある
	if (SockInfo.StartFlag == FALSE) {
		TraceE("wsft_infofile() : 初期化処理が行われていません。" ) ;
		return WSFTERR_INIT_NO_INIT;
	}

	TraceD("wsft_infofile() : ファイル情報を取得します。 ファイル番号=[%ld], レコード=[%ld]", fid, rec);

	int ret = SetServer(&SockInfo, SockInfo.rPort);		// ソケットの作成
	if (ret == 0) {
		//	ファイル通信
		ret = com_infofile(&SockInfo, fid, rec, fsize, fr, flag, 'F');
		//	ソケットの解放
		CloseSocket( &SockInfo ) ;
	}

	TraceD("wsft_infofile() : 戻り値 (%ld) で関数[wsft_infofile]を終了します。", ret ) ;

	return ret ;
}


/*----------------------------------------------------------------------------------/
									ファイルを読み込む

	[関数名]
		wsft_readfile

	[引数]
		unsigned short	fid		:	読み込むファイルのsmfファイル番号
		unsigned short	rec		:	レコード指定(0の時は全レコード=ファイル)
		char			*buf	:	バッファ
		long			bufsize	:	バッファサイズ

	[戻り値]
		==0						:	正常終了
		!=0						:	エラー（エラー番号）

/----------------------------------------------------------------------------------*/
int _stdcall wsft_readfile(u_short fid, u_long rec, char* buf, u_long bufsize)
{
	int	ret = 0 ;

	//	必ずwsft_startを呼び出す必要がある
	if( SockInfo.StartFlag == FALSE ){
		TraceE("wsft_readfile() : 初期化処理が行われていません。" ) ;
		ret = WSFTERR_INIT_NO_INIT ;
		return ret ;
	}

	TraceD("wsft_readfile() : ファイルを取得します。 ファイル番号=[%ld], レコード=[%ld]", fid, rec ) ;

	//	サーバー設定
	ret = SetServer(&SockInfo, SockInfo.rPort ) ;


	// Return file size on SMF in (long*)buf, if bufsize is 0
	if (bufsize == 0) {
		u_long fsize;
		u_short fr, flag;
		ret = com_infofile( &SockInfo, fid, rec, &fsize, &fr, &flag, 'F' ) ;
		if( ret != 0 ){
			TraceD("com_readfile() : com_infofile関数を戻り値 (%d) で終了します。", ret ) ;
		} else {
			*(long*)buf = fsize;
		}
		return ret ;
	}

	if( ret == 0 ){
		//	ファイル通信
		ret = com_readfile( &SockInfo, fid, rec, buf, bufsize, 'F' ) ;
		//	ソケットの解放
		CloseSocket( &SockInfo ) ;
	}

	TraceD("wsft_readfile() : 戻り値 (%ld) で関数[wsft_readfile]を終了します。", ret ) ;

	return ret ;
}

//*******************************************************************************
// 関数名：wsft_read_rec
// 戻り値：==0 正常	!=0 ｴﾗｰｺｰﾄﾞ
// 引  数：fid		ﾌｧｲﾙ番号
//		   starec   読み始めﾚｺｰﾄﾞNo(０は指定できません)
//		   endrec   読み終わりﾚｺｰﾄﾞNo(０は指定できません)
//		   buf		読み込みﾊﾞｯﾌｧ
//		   recsize  1ﾚｺｰﾄﾞｻｲｽﾞ
// 内  容：引数で指定された読み始めﾚｺｰﾄﾞから読み終わりﾚｺｰﾄﾞを読み込みます。
// 作成日：02.03.14 M.NAKAYAMA
//********************************************************************************
int _stdcall wsft_read_rec( unsigned short fid, unsigned long starec,
						   unsigned long endrec, char *buf, unsigned long recsize )
{
	int	ret = 0 ;
	unsigned long i = 0;
	unsigned long cnt = 0;
	unsigned long bufsize = 0;
	unsigned long rec = 0;

	//	必ずwsft_startを呼び出す必要がある
	if( SockInfo.StartFlag == FALSE )
	{
		TraceE("wsft_readfile() : 初期化処理が行われていません。" ) ;
		ret = WSFTERR_INIT_NO_INIT ;
		return ret ;
	}

//読み込むﾚｺｰﾄﾞ件数を取得
	//読み始めより読み終わりが前だとだめ！
	if( (starec > endrec) || starec == 0 || endrec == 0 )
	{
		TraceD("wsft_read_rec() : 引数が無効です" ) ;
		return -1;
	}
	//ﾚｺｰﾄﾞ件数
	cnt = endrec - starec;

	TraceD("wsft_read_rec() : ファイルを取得します。 ファイル番号=[%ld], レコード=[%ld]～[%ld]", fid, starec, endrec ) ;

	//	サーバー設定
	ret = SetServer(&SockInfo, SockInfo.rPort ) ;

	// レコードをまとめて読み込む関数追加。 October 4, 2003 r.kojima
	ret = com_readfile2(&SockInfo, fid, starec, recsize, cnt + 1, buf, recsize * (cnt + 1), 'F');

	//	ソケットの解放
	CloseSocket( &SockInfo ) ;

	TraceD("wsft_read_rec() : 戻り値 (%ld) で関数[wsft_read_rec]を終了します。", ret ) ;

	return ret ;
}

/*----------------------------------------------------------------------------------/
								ファイルを書き込む

	[関数名]
		wsft_writefile

	[引数]
		unsigned short	fid		:	書き込むファイルのsmfファイル番号
		unsigned short	rec		:	レコード指定(0の時は全レコード=ファイル)
		char			*buf	:	バッファ
		unsigned long	bufsize	:	バッファサイズ

	[戻り値]
		==0						:	正常終了
		!=0						:	エラー（エラー番号）

/----------------------------------------------------------------------------------*/
int _stdcall wsft_writefile(u_short fid, u_short rec, char* buf, u_long bufsize)
{
	int	ret = 0 ;
	short	idx ;
	long	stime ;

	static std::map<int, DWORD> LastWriteTime;


	//	必ずwsft_startを呼び出す必要がある
	if( SockInfo.StartFlag == FALSE ){
		TraceE("wsft_writefile() : 初期化処理が行われていません。" ) ;
		ret = WSFTERR_INIT_NO_INIT ;
		return ret ;
	}

	idx = FileNoToIndex( fid ) ;
	//	前回の書き込み要求からの経過時間を取得する
	stime = (GetTickCount() - LastWriteTime[idx]) ;
	if (stime < 3000) {	// 3秒経過してないときは待つ
		TraceD("wsft_writefile() : 前回の書き込み要求から３秒以上経過していないため３秒間待機します。");
		Sleep(4000 - stime);	// 余裕を持って4秒分の待ち時間を用意
	}

	TraceW("wsft_writefile(fid:=%u, rec:=%u, buf:=0x%p, bufsize:=%u): write to %s (port %u)",
		fid, rec, buf, bufsize, inet_ntoa(*(IN_ADDR*)&SockInfo.saAddr), SockInfo.wPort);

	//TraceW("wsft_writefile(): fid=%u rec=%u bufsize=%u", fid, rec, bufsize);

	//	サーバー設定
	//TraceW("wsft_writefile(): 書き込み先...server=%s port=%ld.",inet_ntoa(*(IN_ADDR*)&SockInfo.saAddr), SockInfo.wPort);

//	SockInfo.timeout += 3000;	// FROMアクセスの可能性があるので+3secのタイムアウトに変更
	ret = SetServer( &SockInfo, SockInfo.wPort ) ;
//	SockInfo.timeout -= 3000;	// 次に備えてさっさと戻しておく

	if( ret == 0 ){
		//	ファイル通信
		ret = com_writefile( &SockInfo, fid, rec, buf, bufsize, 'F' ) ;
		//	ソケットの解放
		CloseSocket( &SockInfo ) ;
		//	最後の更新を行った時間を取得しておく
		LastWriteTime[idx] = GetTickCount() ;
	}

	TraceD("wsft_writefile() : 戻り値 (%ld) で関数[wsft_writefile]を終了します。", ret ) ;

	return ret ;
}


/*----------------------------------------------------------------------------------/
							アドレス指定によるファイル取得

	[関数名]
		wsft_readaddr

	[引数]
		unsigned long	address	:	読み込むアドレス
		unsigned short	size	:	読み込むサイズ
		char			*buf	:	バッファ
		unsigned long	bufsize	:	バッファサイズ

	[戻り値]
		==0						:	正常終了
		!=0						:	エラー（エラー番号）

/----------------------------------------------------------------------------------*/
int _stdcall wsft_readaddr(u_long address, u_short size, char* buf, u_long bufsize)
{
	int	ret = 0 ;

	//	必ずwsft_startを呼び出す必要がある
	if( SockInfo.StartFlag == FALSE ){
		TraceE("wsft_readaddr() : 初期化処理が行われていません。" ) ;
		ret = WSFTERR_INIT_NO_INIT ;
		return ret ;
	}

	TraceD("wsft_readfile() : アドレス指定によるファイル取得を開始します。 開始アドレス=[%ld], サイズ=[%ld]", address, size ) ;

	//	サーバー設定
	ret = SetServer( &SockInfo, SockInfo.rPort ) ;

	if( ret == 0 ){
		//	ファイル通信(アドレス)
		ret = com_readfile( &SockInfo, address, size, buf, bufsize, 'A' ) ;
		//	ソケットの解放
		CloseSocket( &SockInfo ) ;
	}

	TraceD("wsft_readaddr() : 戻り値 (%ld) で関数[wsft_readaddr]を終了します。", ret ) ;

	return ret ;
}


/*----------------------------------------------------------------------------------/
							アドレス指定による書き込み

	[関数名]
		wsft_writeaddr

	[引数]
		unsigned short	fid		:	書き込むファイルのsmfファイル番号
		char			*buf	:	バッファ
		unsigned long	bufsize	:	バッファサイズ

	[戻り値]
		==0						:	正常終了
		!=0						:	エラー（エラー番号）

/----------------------------------------------------------------------------------*/
int _stdcall wsft_writeaddr(u_long address, char* buf, u_long bufsize)
{
	int	ret = 0 ;

	//	必ずwsft_startを呼び出す必要がある
	if( SockInfo.StartFlag == FALSE ){
		TraceE("wsft_writeaddr() : 初期化処理が行われていません。" ) ;
		ret = WSFTERR_INIT_NO_INIT ;
		return ret ;
	}

	TraceD("wsft_writefile() : アドレス指定によるファイル書き込みを開始します。 開始アドレス=[%ld], サイズ=[%ld]", address, bufsize ) ;

	//	サーバー設定
	ret = SetServer( &SockInfo, SockInfo.wPort ) ;

	if( ret == 0 ){
		//	ファイル通信
		ret = com_writefile( &SockInfo, address, bufsize, buf, bufsize, 'A' ) ;
		//	ソケットの解放
		CloseSocket( &SockInfo ) ;
	}

	TraceD("wsft_addrfile() : 戻り値 (%ld) で関数[wsft_addrfile]を終了します。", ret ) ;

	return ret ;
}


//---------------------------------------------------------------------
// wsftの終了

int _stdcall wsft_end( void )
{
	int	ret = 0 ;

	//	必ずwsft_startを呼び出す必要がある
	if( SockInfo.StartFlag == FALSE ){
		TraceE("wsft_end() : 初期化処理が行われていません。" ) ;
		ret = WSFTERR_INIT_NO_INIT ;
		return ret ;
	}

	TraceI("wsft_end() : リソースの開放を行います。" ) ;

	SockInfo.StartFlag = FALSE ;

	return ret ;
}

//---------------------------------------------------------------------
// サポートするプロトコルバージョンを返す

u_short _stdcall wsft_getver()
{
	return SockInfo.ver;
}


long _stdcall wsft_dllversion()
{
	return 0x03141406;
}

//---------------------------------------------------------------------
//	通信を再開する
//
//	このﾌﾗｸﾞのｺﾝﾄﾛｰﾙはWssComが行います。

int _stdcall wsft_resume()
{
	LONG prevValue = InterlockedExchange(&g_Suspended, TRUE);
	if (prevValue != TRUE) {

		Winssa_Log(LOGC_WSFT, LOGLV_INFO, "(%s)wsft_resume(): 通信を再開します。", GetExeName().c_str());
	}

	return 0;
}

//---------------------------------------------------------------------
//	通信を停止する
//
//	このﾌﾗｸﾞのｺﾝﾄﾛｰﾙはWssComが行います。

int _stdcall wsft_stop()
{
	LONG prevValue = InterlockedExchange(&g_Suspended, FALSE);
	if (prevValue != FALSE) {

		Winssa_Log(LOGC_WSFT, LOGLV_INFO, "(%s)wsft_stop(): 通信を停止します。", GetExeName().c_str());
	}

	return 0;
}

//---------------------------------------------------------------------
//  DLL_PROCESS_ATTACH処理

static BOOL OnProcessAttach(HINSTANCE hinstDLL)
{
	// とりあえずログの初期化
	TraceXStart(NULL);

	// 通信用構造体のデータをINIファイルから読込　**FUMIYA
	ReadIniFile();

	//WinSockのイニシャライズを行う
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 0);
	int rc = WSAStartup( wVersionRequested, &wsaData ) ;
	if (rc != 0) {
		TraceF("%s(%d): WinSockの初期化に失敗しました。", __FILE__, __LINE__);
		return FALSE;
	}
	//バージョンチェックを行う
	if (wsaData.wVersion != wVersionRequested) {
		TraceF("%s(%d): WinSockのバージョンが違います。", __FILE__, __LINE__);
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------
// DLL_PROCESS_DETACH処理
static BOOL OnProcessDetach(HINSTANCE hinstDLL)
{
	if (g_dwTOCount[0])
		TraceF("CloseSocket: タイムアウト発生率 %.3f (%u/%u)",
			(double)g_dwTOCount[1] / (double)g_dwTOCount[0] * 100.0, g_dwTOCount[1], g_dwTOCount[0]);
	else
		TraceF("CloseSocket: タイムアウト発生率 ... 1回も通信していませんね");

	// プロセスのリソースを解放し、WinSockの使用を停止します。　**FUMIYA
	WSACleanup();

	return TRUE;
}

//---------------------------------------------------------------------
// DLLのエントリーポイント

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		return OnProcessAttach(hinstDLL);
	case DLL_PROCESS_DETACH:
		return OnProcessDetach(hinstDLL);
	}

	return TRUE;
}
