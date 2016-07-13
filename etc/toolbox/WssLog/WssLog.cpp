//---------------------------------------------------------------------
// 概要
//	DLLのエントリーポイントと公開関数
//
// 作成者
//	纐纈
//
// 備考
//
//
//---------------------------------------------------------------------
// WssLog.cpp : DLL アプリケーション用のエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <assert.h>
#include "resource.h"
#include "wsslog.h"

///#pragma data_seg(".shared")
long s_traceLevel = LOGLV_WARNING;	// 通常はWARNING以上のログのみ記録
BOOL s_bAttached = FALSE;			// 初回Attach済み? TRUE=済み
///#pragma data_seg()

#define BUFFER_SIZE	2048

static char s_buff[BUFFER_SIZE];

//#define	MTX_SYSLOG		"mtx_syslog"		// 排他共有テーブル

//------------------------------------------------------------
// イニファイル情報
//------------------------------------------------------------
char m_MTX_SYSLOG[_MAX_PATH];			// 排他共有テーブル名
char m_BasePath[_MAX_PATH];				// 基本パス
char m_LogPath[_MAX_PATH];				// ログパス
int  m_nLogLevel;						// ログ レベル
int  m_nSaveDays;						// ログ保存期間

HANDLE g_hmtxSyslog;					// ログ書込み排他用mutex

// ダンプ用データ
BOOL m_bDump = FALSE;					// ダンプ
void *m_pDumpAdr;						// ダンプアドレス
long m_lDumpSize;						// ダンプサイズ

void GetIniFile();
BOOL MakeDirectory(LPTSTR lpPathName);
BOOL Dump_Start(FILE *fp);
BOOL DeleteOldLog();


//---------------------------------------------------------------------
// 概要
//	外部に公開するログ関数
//
// 引数
//	const char* sikibetu	: ログ識別文字列
//	long  loglevel				: ログレベル
//	const char *format		 :ログ文字列
//
// 戻り値
//	なし
//
// 機能説明
//	フォーマットに従った文字列をログファイルに出力する
//
// 備考
//	なし
//---------------------------------------------------------------------


void WINAPI Winssa_Log(const char *sikibetu,long loglevel,const char * format, ...)
{
	//---- [2003/03/19 R.Kojima] --------------------------------------
	// vsprintfの内部で浮動小数点数を使用するケースがあっても、コード上
	// に浮動小数点数を使用する場所がないと、リンカは浮動小数点ライブラ
	// リをリンクしない。
	// この状態で、書式に%fを使用すると"R6002 floating point not
	// loaded"エラーが発生してしまう。
	// 変数を使って浮動小数点数を使用する処理を書いて、無理やり浮動小数
	// 点ライブラリをリンクさせる。
	volatile double force_a_linker_to_link_a_floating_point_library;

	force_a_linker_to_link_a_floating_point_library = 1.1;

	va_list arg;
	char buf[256];
	char logfname[32];

	char Path_Log[_MAX_PATH];

	int	iRet;

	if (loglevel == LOGLV_ALWAYS) {
		;										//　ログレベル：必ずログを出す(注).頻繁に起こるものに関しては使用禁止
	} else if (loglevel < s_traceLevel) {			//　ログレベルが小さいのでログはとらない
		return;
	}

	DWORD dwRet = WaitForSingleObject(g_hmtxSyslog, 3000);
	if (dwRet == WAIT_OBJECT_0 || dwRet == WAIT_ABANDONED) {	//  成功

		va_start(arg, format);

		vsprintf(s_buff, format, arg);

		SYSTEMTIME	ptm;
		GetLocalTime(&ptm);

		sprintf(buf,"%02d:%02d:%02d [%-6s]{%d}:",
				ptm.wHour, ptm.wMinute, ptm.wSecond, sikibetu, loglevel);

		sprintf(Path_Log,"%s%04d%02d%02d", m_LogPath, ptm.wYear, ptm.wMonth, ptm.wDay);

		iRet = _access(Path_Log,0);
		if(iRet != 0){						// 記憶フォルダがない
			MakeDirectory(Path_Log);
		}

		PathAddBackslash(Path_Log);

		//  デバッグ出力
		OutputDebugString(buf);
		OutputDebugString(s_buff);
		OutputDebugString("\n");

		//　ログファイル名を作成
		sprintf(logfname,"%04d%02d%02d.log",
					ptm.wYear, ptm.wMonth, ptm.wDay);

		strcat(Path_Log, logfname);

		// ログファイルに出力
		FILE* fp = fopen(Path_Log, "a");
		if (fp) {
			fprintf(fp, "%s", buf);
			fprintf(fp, "%s", s_buff);
			fprintf(fp, "\n");
			if (m_bDump) {
				m_bDump = FALSE;
				Dump_Start(fp);
			}
			fclose(fp);
		}

		ReleaseMutex(g_hmtxSyslog);		//  Mutexの解放
	}
	else {
		assert( 0 );
	}

	force_a_linker_to_link_a_floating_point_library = 2.2;
}

//---------------------------------------------------------------------
// 概要
//	外部に公開するログ関数
//
// 引数
//	const char* sikibetu	: ログ識別文字列
//	long  loglevel			: ログレベル
//	const void *buffer		 :ダンプデータのアドレス
//	long  size				 :ダンプデータのサイズ
//
// 戻り値
//	なし
//
// 機能説明
//	ダンプデータをログファイルに出力する
//
// 備考
//	なし
//---------------------------------------------------------------------


void WINAPI Winssa_Dump(const char *sikibetu,long loglevel, const void* buffer, long size)
{
	m_bDump = TRUE;			// ダンプモード
	m_pDumpAdr = (void *)buffer;	// ダンプアドレス
	m_lDumpSize = size;		// ダンプサイズ
	
	Winssa_Log(sikibetu, loglevel, "<Dump>Start");
}

void WINAPI WinssaVB_Dump(const char *sikibetu,long loglevel, const void* buffer, long size)
{
	Winssa_Dump(sikibetu, loglevel, buffer, size);
}

//---------------------------------------------------------------------
// 概要
//	外部に公開するログ関数
//
// 引数
//	const char* sikibetu	: ログ識別文字列
//	long  loglevel				: ログレベル
//	const char *format		 :ログ文字列
//
// 戻り値
//	なし
//
// 機能説明
//	フォーマットに従った文字列をログファイルに出力する
//
// 備考
//	なし
//---------------------------------------------------------------------

void WINAPI WinssaVB_Log(const char *sikibetu,long loglevel,const char * format)
{
	Winssa_Log(sikibetu, loglevel, format);
}

//---------------------------------------------------------------------
//	[概要]
//	ログレベルの変更

void WINAPI Winssa_Loglevel(long loglevel)
{
	WLSetLogLevel(loglevel);
}

//---------------------------------------------------------------------
//	[概要]
//	ログレベルの設定

void WINAPI WLSetLogLevel(long nNewLogLevel)
{
	if (nNewLogLevel < LOGLV_DEBUG || nNewLogLevel > LOGLV_FATALERR)
		return;

	InterlockedExchange(&s_traceLevel, nNewLogLevel);
}

//---------------------------------------------------------------------
//	[概要]
//	ログレベルの取得

long WINAPI WLGetLogLevel()
{
	long nWork;
	InterlockedExchange(&nWork, s_traceLevel);
	return nWork;
}

//---------------------------------------------------------------------
//	[概要]
//	DLLエントリーポイント

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH) {

		// 初回process attachだけ、ログレベルを設定する
		if (s_bAttached == FALSE) {
			s_bAttached = TRUE;
			GetIniFile();
			WLSetLogLevel(m_nLogLevel);

			g_hmtxSyslog = CreateMutex(NULL, FALSE, m_MTX_SYSLOG);	// ログファイル用mutex

			// 保存日数を超えたログを削除
			if (m_nSaveDays > 0) {
				DeleteOldLog();
			}
		}

	} else if (fdwReason == DLL_PROCESS_DETACH) {
		if (g_hmtxSyslog != NULL) {
			ReleaseMutex(g_hmtxSyslog);
			CloseHandle(g_hmtxSyslog);
		}
	}

    return TRUE;
}

//---------------------------------------------------------------------
//	[概要]
//	初期化ファイル（WinSousa.ini）からログパスとログレベルの取得

void GetIniFile()
{
	char cPath[_MAX_PATH];
	char cDrive[_MAX_DRIVE];
	char cDir[_MAX_DIR];
	char cName[_MAX_FNAME];
	char cLogPath[_MAX_PATH];

	// モジュールのフルパスを取得する。 
	GetModuleFileName (NULL, cPath, _MAX_PATH);

	// パス名を構成要素に分解する。
	_splitpath( cPath, cDrive, cDir, cName, NULL );

	// モジュール格納ディレクトリをセットする。
	strcpy(m_BasePath, cDrive);
	strcat(m_BasePath, cDir);

	// VBの場合はカレントディレクトリとする。
	// "C:\Program Files\Microsoft Visual Studio\VB98\VB6.EXE"
	// "C:\Program Files\Microsoft Visual Studio\"のファイルを読みにいくのを防ぐ。
	if (_stricmp(cName, "vb6") == 0) {
		// カレントディレクトリをセットする。
		GetCurrentDirectory(_MAX_PATH, m_BasePath);
		strcat(m_BasePath, "\\");
	}

	// WinSousa.iniをセットする。
	strcpy(cPath, m_BasePath);
	strcat(cPath, "WinSousa.ini");

	// ログＰＡＴＨの取得
	GetPrivateProfileString("Log", "LogPath", "", cLogPath, _MAX_PATH, cPath);
	if (strlen(cLogPath) == 0) {
		GetPrivateProfileString("Pathinf", "LogPath", "Log", cLogPath, _MAX_PATH, cPath);
	}

	// ログフルパス＝.\Log\
	// 基本パスセット
	strcpy(m_LogPath, m_BasePath);
	PathAddBackslash(m_LogPath);
	//ログＰＡＴＨセット
	strcat(m_LogPath, cLogPath);
	PathAddBackslash(m_LogPath);

	// LogLevelの取得
	m_nLogLevel = GetPrivateProfileInt("Log", "LogLevel", 2, cPath);

	// 保存日数の取得
	m_nSaveDays = GetPrivateProfileInt("Log", "SaveDays", 90, cPath);

	// ミューテックス名を作成する。(C:\temp\temp.exe → mtx_C:/temp/)
	// ミューテックス名　"mtx_" + m_BasePath
	strcpy(m_MTX_SYSLOG, "mtx_");
	strcat(m_MTX_SYSLOG, m_BasePath);

	// ミューテックス名に[\]を使用できないため、　[\]を[/]に置き換える
	for (int i=0 ; i<(int)strlen(m_MTX_SYSLOG) ; i++) {
		if (m_MTX_SYSLOG[i] == '\\')
			m_MTX_SYSLOG[i] = '/';
	}
}

//---------------------------------------------------------------------
//	[概要]
//	 多階層ディレクトリの作成。
//
// 戻り値
//	TRUE:正常　FALSE:異常
//
BOOL MakeDirectory(LPTSTR lpPathName)
{
	int	iRet;

	// ディレクトリの有無チェック
	iRet = _access(lpPathName,0);
	// ディレクトリが存在なら 戻る
	if (iRet == 0) return TRUE;

	int i,iLen;
	char cDir[_MAX_PATH];

	strcpy(cDir, lpPathName);
	PathAddBackslash(cDir);
	iLen = strlen(cDir);

	// 階層ディレクトリを順次作成する
	for (i=4 ; i<iLen ; i++) {
		if (cDir[i] == '\\' || cDir[i] == '/') {
			cDir[i] = 0;
			// ディレクトリの有無チェック
			iRet = _access(cDir,0);
			// なければ作成する
			if (iRet != 0) {
				// ディレクトリを新規作成
				iRet = _mkdir(cDir);
				// 失敗したらエラーで戻る
				if (iRet != 0) return FALSE;
			}
			cDir[i] = '\\';
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------
//	[概要]
//	 データをダンプ形式で出力する
//
// 戻り値
//	TRUE:正常　FALSE:異常
//
BOOL Dump_Start(FILE *fp)
{
	// ヘッダー部の出力
	char cHead[] = "     +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F 0123456789ABCDEF \n";
	OutputDebugString(cHead);
	fprintf(fp, cHead);
	
	int i, ix;
	char cwk[10];
	char *cHex = &cHead[5];
	char *cAsc = &cHead[5+16*3];
	BOOL bKanji = FALSE;
	unsigned char* pstr = (unsigned char*)m_pDumpAdr;
	long size = m_lDumpSize;

	memset(cHex, 0 , sizeof(cHex));
	memset(cAsc, 0 , sizeof(cAsc));
	for (i=0 ; i<size ; i++, pstr++) {
		ix = i % 16;
		if (ix == 0) {
			memset(cHead, ' ', sizeof(cHead)-2);
			sprintf(cwk, "%04X", i);
			memcpy(cHead, cwk, 4);
		}
		sprintf(cwk, "%02X", *pstr);
		memcpy(&cHex[ix*3], cwk, 2);

		if (!bKanji) {
			if (IsDBCSLeadByte(*pstr)) {	// 漢字の１バイト目？
				if (i+1 < size) {
					cAsc[ix] = *pstr;
					cAsc[ix+1] = *(pstr+1);
					bKanji = !bKanji;
				}
				else {
					cAsc[ix] = '.';
				}
			}
			else if (!iscntrl(*pstr)) {	//制御文字
				cAsc[ix] = *pstr;
			}
			else {
				cAsc[ix] = '.';
			}
		}
		else {
			bKanji = !bKanji;
		}
		if (ix == 15 || i+1 == size) {
			OutputDebugString(cHead);
			fprintf(fp, cHead);
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------
// 概要
//	保存日付の取得
//
// 戻り値
//	なし
//
void GetSaveDate(char *cSaveDate)
{
	time_t lNow;
	struct tm stTm;
	
	// システム時刻を得ます。
	time( &lNow );

	// 時刻値を現地時刻に変換します。
	stTm = *localtime( &lNow );

	// 日付の引き算
	stTm.tm_mday -= m_nSaveDays;

	// 時刻構造体 (不完全な場合もあります) を正規化された値を持つ完全に定義された構造体に変換する
	mktime(&stTm);

	sprintf(cSaveDate,"%04d%02d%02d", stTm.tm_year+1900, stTm.tm_mon+1, stTm.tm_mday);
}

//---------------------------------------------------------------------
// 概要
//	日付を比較して、保存期間より古いファイルを削除する
//
// 戻り値
//	なし
//
BOOL DeleteLog(const char *cFileName, const char* cSaveDate)
{
	char cDir[_MAX_PATH];
	char cPath[_MAX_PATH];

	//"20"を探す
	if (strlen(cFileName)==8 && memcmp(cFileName, "20", 2) == 0) {
		//ファイル名の日付が保存期間より古いなら
		if (strcmp(cFileName, cSaveDate) <= 0) {
			// ディレクトリ名の作成
			sprintf(cDir, "%s%s", m_LogPath, cFileName);
			// ファイル名の作成
			sprintf(cPath, "%s%s\\%s.log", m_LogPath, cFileName, cFileName);

			// ファイルを削除する
			if (_unlink(cPath) == -1) {
				Winssa_Log("WSSLOG", LOGLV_WARNING, "ファイルを削除できません。ファイル=[%s]", cPath);
				return FALSE;
			}
			//ディレクトリを削除する
			if (_rmdir(cDir) == -1) {
				Winssa_Log("WSSLOG", LOGLV_WARNING, "ディレクトリを削除できません。ディレクトリ=[%s]", cDir);
				return FALSE;
			}
		}
	}
	return TRUE;
}


//---------------------------------------------------------------------
// 概要
//	保存期間を超えたLogファイルの削除
//
// 戻り値
//	TRUE:正常　FALSE:異常
//
BOOL DeleteOldLog()
{
	// 保存日付の取得
	char cSaveDate[10];
	GetSaveDate(cSaveDate);

	// ログ格納フォルダのテンプレートを追加する
	char cFind[_MAX_PATH];
	sprintf(cFind, "%s*", m_LogPath);

    // 指定したパターンのファイルを探します。 
    struct _finddata_t stFind;
    long hFile;

    hFile = _findfirst( cFind, &stFind );
	if (hFile != -1L ) {
		do {
			//保存期間を超えたファイルを削除する
			DeleteLog(stFind.name, cSaveDate);
		}
		// 残りのファイルを探します。
		while( _findnext( hFile, &stFind ) == 0 );

		//リソースを開放する
		_findclose( hFile );
	}

	return TRUE;
}

