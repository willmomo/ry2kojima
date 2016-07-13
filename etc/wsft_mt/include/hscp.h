
#pragma once

#if !defined(__HSCP_H)
#define __HSCP_H

// hscpが必要とするヘッダ

#include <stdio.h>
#include <memory.h>
#include <windows.h>
#include <shlwapi.h>
#include <winbase.h>
#include <process.h>
#include <lm.h>
#include <ctime>
#include <assert.h>
#include "wsslog.h"

// export/import マクロ

#if defined(HSCP_EXPORTS)
#define HSCP_API	__declspec(dllexport)
#else
#define HSCP_API	__declspec(dllimport)
#endif

////////// プログラム内コメント入力マクロ //////////
/*
	以下のように、プログラム内にコメントを入れておくと：

	#pragma hsMSG(ここにTODOを入れておく)

	コンパイル時に、以下のようなメッセージが出力されます：

	ソースファイル名(行番号) : ここにTODOを入れておく

	メッセージを""で、囲む必要はありません。
*/

#define hsSTR2(n)	#n
#define hsSTR(n)	hsSTR2(n)
#define hsMSG(msg)	message(__FILE__ "(" hsSTR(__LINE__) ") : " hsSTR2(msg))
#define hsMSG0(msg)	message(hsSTR2(msg))	// 装飾なしのメッセージ

// ユーティリティマクロ

#if !defined( countof )
#define countof(A)	(sizeof((A)) / sizeof((A)[0]))
#endif

// min_num <= num <= max_num のとき、真を返すマクロ

#define	hsINRANGE(min_num, num, max_num)	(((min_num) <= (num)) && ((num) <= (max_num)))

// 構造体

// 年月日時分秒をビットフィールド{yyyy yyym mmmd dddd hhhh hnnn nnns ssss}
// で表現する形。年の開始は1910から。月、日の開始日は1から。秒は、1/2。

#if !defined(PSTM_T)
#define PSTM_T
typedef struct {
	unsigned sec	: 5;	// 秒(1/2秒)
	unsigned min	: 6;	// 分(0～59分)
	unsigned hour	: 5;	// 時(0～23時)
	unsigned mday	: 5;	// 日(1～31日)
	unsigned mon	: 4;	// 月(1～12月)
	unsigned year	: 7;	// 年(0=1910年から)
} pstm_t;
#endif
typedef pstm_t	hsPSTM;		// hscp用命名規則に沿った名前

// 年月日時分曜をビットフィールド{yyyy yyym mmmd dddd hhhh hnnn nnnw wwww}
// で表現する形。年の開始は1910から。月、日の開始日は1から。

typedef struct {
	unsigned wday	: 5;	// 曜(0=日)
	unsigned min	: 6;	// 分(0～59分)
	unsigned hour	: 5;	// 時(0～23時)
	unsigned mday	: 5;	// 日(1～31日)
	unsigned mon	: 4;	// 月(1～12月)
	unsigned year	: 7;	// 年(0=1910年から)
} pwtm_t;
typedef pwtm_t	hsPWTM;		// hscp用命名規則に沿った名前

// hscpがexportするクラスライブラリ

///////////////////////////////////////////////////////////////////////
// ロックファイルを安全に扱うクラス(hsCLockfile)

class HSCP_API hsCLockfile
{
private:
	HANDLE m_hFile;

public:
	// コンストラクタ
	hsCLockfile();

	// デストラクタ
	virtual ~hsCLockfile();

public:
	// ロックファイルの作成
	BOOL Lock(const char *pszLockfile, BOOL fDeleteIfError);

	// ロックファイルの作成
	BOOL LockEx(const char* pszLockfile, BOOL fDeleteIfError, DWORD dwWaitMs, DWORD dwRetry);

	// ロックファイルの削除
	BOOL Unlock();
};

// エクスポート

HSCP_API BOOL WINAPI hsIsEmbeddedOS();
HSCP_API HANDLE WINAPI hsCreateLockfile(const char *pszLockfileName, BOOL fDeleteIfError);
HSCP_API HANDLE WINAPI hsCreateLockfileEx(const char* pszLockfileName, BOOL fDeleteIfError, DWORD dwWaitMs, DWORD dwRetry);
HSCP_API BOOL WINAPI hsDeleteLockfile(HANDLE hFile);
HSCP_API int WINAPI IsIniEntryExists(const char* pszSection, const char* pszKey, const char* pszIniFile);
HSCP_API int WINAPI IsSharedFolder(const char *pszServerName, const char *pszShareName);
HSCP_API int WINAPI MakePSTM(int y, int m, int d, int h, int n, int s);
HSCP_API BOOL WINAPI hsDeleteFiles(const char *pszWildCard, int nNumOfSave);
HSCP_API BOOL WINAPI hsIsSpaceStr(LPCTSTR parg);

HSCP_API double DiffFileTimeNow(const FILETIME* pFileTime);
HSCP_API double DiffPsTimeNow(unsigned long ps_tim);
HSCP_API time_t ToTimePSTM(DWORD pstm);
HSCP_API DWORD GetWin32ErrorMessage(DWORD dwError, char* pszMsg, size_t size);
HSCP_API int chomp(char* p);
HSCP_API int RemoveOldFiles(const char* pszFolder, int nDays);

HSCP_API int __stdcall RunProg(const char *cmd, const char* outfile);
HSCP_API long __stdcall	check_ip( char *strAddr);
HSCP_API long __stdcall CheckBit(long data, long bit);
HSCP_API long __stdcall SetBit(long data, long bit, long setmode);

#endif

// 定数定義
#define ANS_OK				0		// 相手先のIP確認
#define ANS_NG				1		// 相手先のIPが確認できない
#define ANS_ERR_SYS			2		// CreateProcessエラー
#define ANS_ERR_TMPFILE		3		// ping結果ファイルのオープンエラー

#define TMPFILENAME			"ping.tmp"		// ping結果が格納されるファイル



//---------------------------------------------------------------------
// 新しいGetVersionInfoEx用の構造体を使いたいが、vc6, vb6環境ではかなわ
// ないためこのヘッダ(DLL)で提供する。
//
// 頭にhsが付く以外は、VS2005のものと同じ。
//
// ** 使い方 **
//
// HS_OSVERSIONINFOEX buf;
// memset(&buf, 0, sizeof(buf));
// buf.dwOSVersionInfoSize = sizeof(buf);
// GetVersionEx((LPOSVERSIONIFNO)&buf);
//
// 参考URL: http://msdn2.microsoft.com/en-us/library/ms724833.aspx
//
// 2007.09.04 r.kojima
//

// VS2005のOSVERSIONINFOEXの代わり
//
typedef struct _HS_OSVERSIONINFOEX 
{
	DWORD dwOSVersionInfoSize;
	DWORD dwMajorVersion;
	DWORD dwMinorVersion;
	DWORD dwBuildNumber;
	DWORD dwPlatformId;
	TCHAR szCSDVersion[128];
	WORD wServicePackMajor;
	WORD wServicePackMinor;
	WORD wSuiteMask;
	BYTE wProductType;
	BYTE wReserved;
} HS_OSVERSIONINFOEX, *PHS_OSVERSIONINFOEX, *LPHS_OSVERSIONINFOEX;


// wSuiteMask の定義頭にHS_が付いている以外は、VS2005のものと同じ
//
#define HS_VER_SUITE_BACKOFFICE					0x00000004	// Microsoft BackOffice components are installed. 
#define HS_VER_SUITE_BLADE						0x00000400	// Windows Server 2003, Web Edition is installed. 
#define HS_VER_SUITE_COMPUTE_SERVER				0x00004000	// Windows Server 2003, Compute Cluster Edition is installed. 
#define HS_VER_SUITE_DATACENTER					0x00000080	// Windows Server 2008 Datacenter, Windows Server 2003, Datacenter Edition or Windows 2000 Datacenter Server is installed. 
#define HS_VER_SUITE_ENTERPRISE					0x00000002	// Windows Server 2008 Enterprise, Windows Server 2003, Enterprise Edition, Windows 2000 Advanced Server, 
															// or Windows NT Server 4.0 Enterprise Edition is installed. Refer to the Remarks section for more information about this bit flag. 
#define HS_VER_SUITE_EMBEDDEDNT					0x00000040	// Windows XP Embedded is installed. 
#define HS_VER_SUITE_PERSONAL					0x00000200	// Windows Vista Home Premium, Windows Vista Home Basic, or Windows XP Home Edition is installed. 
#define HS_VER_SUITE_SINGLEUSERTS				0x00000100	// Remote Desktop is supported, but only one interactive session is supported. 
															// This value is set unless the system is running in application server mode. 
#define HS_VER_SUITE_SMALLBUSINESS				0x00000001	// Microsoft Small Business Server was once installed on the system, 
															// but may have been upgraded to another version of Windows. Refer to the Remarks section for more information about this bit flag. 
#define HS_VER_SUITE_SMALLBUSINESS_RESTRICTED	0x00000020	// Microsoft Small Business Server is installed with the restrictive client license in force. 
															// Refer to the Remarks section for more information about this bit flag. 
#define HS_VER_SUITE_STORAGE_SERVER				0x00002000	// Windows Storage Server 2003 R2 or Windows Storage Server 2003 is installed. 
#define HS_VER_SUITE_TERMINAL					0x00000010	// Terminal Services is installed. This value is always set. 
															// If VER_SUITE_TERMINAL is set but VER_SUITE_SINGLEUSERTS is not set, the system is running in application server mode.
#define HS_VER_SUITE_WH_SERVER					0x00008000	// Windows Home Server is installed. 

// wProductType の定義。頭にHS_が付いている以外は、VS2005のものと同じ
//
#define HS_VER_NT_DOMAIN_CONTROLLER	0x0000002	// The system is a domain controller. 
#define HS_VER_NT_SERVER			0x0000003	// The system is a server. Note that a server that is also a domain controller is reported as VER_NT_DOMAIN_CONTROLLER, not VER_NT_SERVER.
#define HS_VER_NT_WORKSTATION		0x0000001	// The operating system is Windows Vista, Windows XP Professional, 
												// Windows XP Home Edition, Windows 2000 Professional, or Windows NT Workstation 4.0. 
