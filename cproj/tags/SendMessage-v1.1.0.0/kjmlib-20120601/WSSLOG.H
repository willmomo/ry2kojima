//---------------------------------------------------------------------
//	WssLog共通ヘッダ(VB用はWssLog.bas)
//
//	[概要]
//	WssLog.dllを使用する人は、これをインクルード

#pragma once


// INIファイルのデフォルト値

#define DEFAULT_LOG_SAVEDAYS	90		// [Log] SaveDays= 90 日


//　ログのレベル定義

#define LOGLV_DEBUG			0			// デバッグ用
#define	LOGLV_TRACE			1			// トレース
#define	LOGLV_WARNING		2			// ワーニング(通常はこのレベル以上を出力)
#define	LOGLV_ERR			3			// エラー
#define	LOGLV_FATALERR		4			// 致命的エラー
#define LOGLV_INFO			5			// 情報
#define	LOGLV_ALWAYS		9			// 必ずログを出す
										// (注).頻繁に起こるものに関しては使用禁止
// ログ識別コード(6文字以内で定義)

#define	LOGC_APPCTRL		"APPCTL"	// WssAppCtrl.exe
#define LOGC_COM			"COM"		// WssCom.exe
#define LOGC_SYUUKEI		"SYUKEI"	// Syukei.exe
#define LOGC_TNIO			"TNIO"		// Tnio.exe
#define	LOGC_UNSET			"UNYOU"		// Unyou.exe
#define	LOGC_GYOSET			"GYOSET"	// GyoSet.exe
#define	LOGC_SYSSET			"SYSSET"	// SysSet.exe
#define	LOGC_TOUINJI		"TOUKA"		// Toukainji.exe
#define LOGC_WINKT			"WINKT"		// WinKT.exe
#define LOGC_MPCPROC		"MPCPRC"	// mpcproc.exe
#define	LOGC_CALDLL			"CALDLL"	// WinSousaCalc.dll
#define	LOGC_FILDLL			"FILDLL"	// Wsft.dll(GN/HTソース互換化 GNでのみ使用)
#define	LOGC_WSFT			"WSFT"		// Wsft.dll
#define	LOGC_UNDLL			"OPEDLL"	// Opesend.dll
#define	LOGC_GFTP			"GFTP"		// GFTP.exe
#define	LOGC_DLG			"WSSDLG"	// WssDialog.exe
#define	LOGC_WSSHUT			"WSSHUT"	// WssShutter.exe
#define LOGC_WSSASUP		"SUPDLL"	// Wssasup.dll
#define LOGC_TNPROC			"TNPROC"	// TnProc.exe
#define LOGC_RIREKI			"RIREKI"	// rireki.dll
#define LOGC_HSCP			"HSCP"		// hscp.dll
#define LOGC_ITA			"ITA"		// ITA.dll
#define LOGC_MDSCOM			"MDSCOM"	// MDSCom.exe(A1000334)

// export された関数の定義

#if defined(WSSLOG_EXPORTS)
#define WSSLOG_API	__declspec(dllexport) __stdcall
#else
#define WSSLOG_API	__declspec(dllimport) __stdcall
#endif

#define	DllExport	__declspec(dllexport)


#if defined(__cplusplus)
extern	"C" {
#endif

void WSSLOG_API Winssa_Log(const char *sikibetu,long loglevel,const char * format, ...);
void WSSLOG_API Winssa_Loglevel(long loglevel);
void WSSLOG_API Winssa_Dump(const char *sikibetu,long loglevel,const void *buffer, long size);
void WSSLOG_API WLSetLogPath(const char* pszPath);
void WSSLOG_API WLSetLogLevel(long nNewLogLevel);
long WSSLOG_API WLGetLogLevel();
void WSSLOG_API WLFlushLog();
BOOL WSSLOG_API WLDeleteOldLog();
int WSSLOG_API WLGetLogPath(char* pszBuffer, int nSize);
void WSSLOG_API WLGetMyName(char *buf, int size);

// ログファイルの名前を "YYYYMMDD.log" または "YYYYMMDD_HH.log" のどちらかに決定する
// フラグを操作する関数。
// 値を 1 にすると、"YYYYMMDD_HH.log" になる。
// デフォルトは 1 で動作している。

void WSSLOG_API WLSetLogNameType(int nNameType);
long WSSLOG_API WLGetLogNameType();

#if defined(__cplusplus)
}
#endif

#if defined(__cplusplus)
extern "C" {
#endif

DllExport int WINAPI TraceLogStart(unsigned char mainver, unsigned char minnerver, DWORD stTraceInfo);

#if defined(__cplusplus)
DllExport int WINAPI TraceLogDeleteOldFile(short index = 0);
#else
DllExport int WINAPI TraceLogDeleteOldFile(short index);
#endif

DllExport void WINAPI TraceLog(short tracestatus, const char *format, ...);

#if defined(__cplusplus)
}
#endif

// Trace.dll を replace するための定義類
// 通常は必要ないので、REPLACE_TRACELOG_DLL が定義されているときだけ有効になる。

#if defined(REPLACE_TRACELOG_DLL)

/*	表示レベル	*/
typedef enum {
	TRACE_OUTPUT_LOG_MAX = 0,	/*	トレース専用ログ(最大)		*/
	TRACE_OUTPUT_LOG     = 1,	/*	トレース専用ログ(普通)		*/
	TRACE_OUTPUT_LOG_MIN = 2,	/*	トレース専用ログ(最小)		*/
	TRACE_DEBUG_MAX      = 3,	/*	デバッグ用ログ(最大)		*/
	TRACE_DEBUG          = 4,	/*	デバッグ用ログ(普通)		*/
	TRACE_DEBUG_MIN      = 5,	/*	デバッグ用ログ(最小)		*/
	TRACE_WARNING_MAX    = 6,	/*	警告ログ(最大)				*/
	TRACE_WARNING        = 7,	/*	デバッグ用ログ(普通)		*/
	TRACE_WARNING_MIN    = 8,	/*	デバッグ用ログ(最小)		*/
	TRACE_ERROR_SLIGHT   = 9,	/*	エラーログ(軽微)			*/
	TRACE_ERROR_FATAL    = 10,	/*	エラーログ(重大)			*/
	TRACE_ERROR_SYSTEM   = 11,	/*	エラーログ(システム絡み)	*/
	TRACE_INFO_MAX       = 12,	/*	情報表示(常に表示)			*/
	TRACE_INFO           = 13,	/*	情報表示(常に表示)			*/
	TRACE_INFO_MIN       = 14,	/*	情報表示(常に表示)			*/
	TRACE_SYSTEM_INFO	 = 15,	/*	処理開始と終了を書込むとき (書式を変えたいので)	*/
	TRACE_NO_OUTPUT      = 17,	/*	ダミー(最後=これで、非表示を実現している)	*/
	TRACE_ERROR_DATABASE = 18,	/*	データベースに起因する問題	*/
	TRACE_SYSTEM_BUG     = 19,	/*	バグなので必ず表示			*/
} ETraceStatus ;

/*	トレースレベル設定	*/
#define	TRACELEVEL_ALL_MAX		TRACE_OUTPUT_LOG_MAX	/*	全て表示する							*/
#define	TRACELEVEL_LOG_MAX		TRACE_OUTPUT_LOG_MAX	/*	ログ情報以上を表示する					*/
#define	TRACELEVEL_DEBUG_MAX	TRACE_DEBUG_MAX			/*	デバッグ情報以上を表示する				*/
#define	TRACELEVEL_WARNING_MAX	TRACE_WARNING_MAX		/*	警告情報以上を表示する					*/
#define	TRACELEVEL_ERROR_SLIGHT	TRACE_ERROR_SLIGHT		/*	エラー情報(軽微)以上を表示する			*/
#define	TRACELEVEL_ERROR_FATAL	TRACE_ERROR_FATAL		/*	エラー情報(重大)以上を表示する			*/
#define	TRACELEVEL_ERROR_SYSTEM	TRACE_ERROR_SYSTEM		/*	エラー情報(システム絡み)以上を表示する	*/
#define	TRACELEVEL_INFO			TRACE_INFO				/*	情報以上を表示する						*/
#define	TRACELEVEL_NO_OUTPUT	TRACE_NO_OUTPUT			/*	何も表示しない							*/

#endif
