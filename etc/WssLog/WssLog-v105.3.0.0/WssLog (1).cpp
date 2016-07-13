//-----------------------------------------------------------------------------
//	DLLのエントリーポイントと公開関数
//
// 作成者
//	纐纈
//
// [備考]
// 2008.08.04 小島
//
// USE_THREAD を define した場合、スレッドを利用した遅延書込みを行う
// モジュールが作成できる。
// 本格的に使うようになった場合は、スレッドタイプとそうじゃないタイプの
// 両方をランタイムで切り分けるように工夫が欲しいところ。
//
//-----------------------------------------------------------------------------
// WssLog.cpp : DLL アプリケーション用のエントリ ポイントを定義します。
//

#pragma warning(disable : 4786)

#include <windows.h>
#include <shlwapi.h>
#include <time.h>
#include <assert.h>
#include <queue>
#include "wsslog.h"
#include "FixIniPath.h"
#include "kjmlib.h"
#include "syslogc.h"
#include "resource.h"


///#pragma data_seg(".shared")
long s_traceLevel = LOGLV_WARNING;	// 通常はWARNING以上のログのみ記録
BOOL s_bAttached = FALSE;			// 初回Attach済み? TRUE=済み
///#pragma data_seg()

#define BUFFER_SIZE	20480

static char s_buff[BUFFER_SIZE];

//#define	MTX_SYSLOG		"mtx_syslog"		// 排他共有テーブル

//-----------------------------------------------------------------------------
// イニファイル情報
//-----------------------------------------------------------------------------
char m_MTX_SYSLOG[MAX_PATH];			// 排他共有テーブル名
char m_appName[MAX_PATH];
char m_BasePath[MAX_PATH];				// 基本パス
char m_LogPath[MAX_PATH];				// ログパス
int  m_nLogLevel;						// ログ レベル
int  m_nSaveDays;						// ログ保存期間
int  m_logNameType;						// 0 = YYYYMMDD.log , 1 = YYYYMMDD_HH.log
std::string m_syslogServer;				// if not empty, syslog をつかって、ログを書き出すモード
HANDLE g_hmtxSyslog;					// ログ書込み排他用mutex

// ダンプ用データ
BOOL m_bDump = FALSE;					// ダンプ
void *m_pDumpAdr;						// ダンプアドレス
long m_lDumpSize;						// ダンプサイズ

std::queue<std::string> g_logQ;
syslog_client g_syslogc;

HINSTANCE g_hinstDLL;

//-----------------------------------------------------------------------------
// syslog_client の worker thread の実装
//-----------------------------------------------------------------------------
unsigned syslog_client::worker() {

	int qsize;
	std::string msg;
	char logfname[32];
	char Path_Log[MAX_PATH];

	while (m_loop) {

		lock();
		if (!g_logQ.empty()) {
			msg = g_logQ.front();
			g_logQ.pop();
			qsize = g_logQ.size();
		} else {
			msg.erase();
		}
		unlock();

		if (msg.empty()) {
			Sleep(200);
			continue;
		}
#if 1
		// ここで、syslogとfile出力を切り分ける
#if 0
		if (!m_syslogServer.empty()) {

			printf("send sntpd %s\n", m_syslogServer.c_str());

			SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
			kjm::in_socket sock;
			if (sock.open_socket(SOCK_DGRAM)) {

				printf("s = %d\n", s);

				std::vector<wchar_t> buf2((msg.length() + 1) * 2);
				std::vector<char> buf3(buf2.size());
				MultiByteToWideChar(CP_ACP, 0, msg.c_str(), -1, buf2.begin(), buf2.size());
				WideCharToMultiByte(CP_UTF8, 0, buf2.begin(), -1, buf3.begin(), buf3.size(), NULL, NULL);

				int ret = sock.send_to(m_syslogServer.c_str(), 514, buf3.begin(), lstrlen(buf3.begin()));

				printf("sendto result = %d\n", ret);
			}
		}
#endif
		{
			DWORD dwRet = WaitForSingleObject(g_hmtxSyslog, 3000);

			if (dwRet == WAIT_OBJECT_0 || dwRet == WAIT_ABANDONED) {	//  成功
				SYSTEMTIME	ptm;
				GetLocalTime(&ptm);

				static FILETIME s_ft;
				FILETIME ft;
				SystemTimeToFileTime(&ptm, &ft);

				sprintf(Path_Log,"%s%04d%02d%02d", m_LogPath, ptm.wYear, ptm.wMonth, ptm.wDay);

				kjm::create_directory(Path_Log);

				PathAddBackslash(Path_Log);

				//  デバッグ出力
				OutputDebugString(msg.c_str());
				OutputDebugString("\n");

				//　ログファイル名を作成
				if (m_logNameType == 1) {
					sprintf(logfname,"%04d%02d%02d_%02d.log", ptm.wYear, ptm.wMonth, ptm.wDay, ptm.wHour);
				} else {
					sprintf(logfname,"%04d%02d%02d.log", ptm.wYear, ptm.wMonth, ptm.wDay);
				}

				PathAppend(Path_Log, logfname);

				// ログファイルに出力
				FILE* fp = fopen(Path_Log, "a");
				if (fp) {
					if (*(__int64*)&ft < *(__int64*)&s_ft) {
						// 時間の逆流を検知
						SYSTEMTIME st1, st2;
						FileTimeToSystemTime(&s_ft, &st1);
						FileTimeToSystemTime(&ft, &st2);

						fprintf(fp, "<%02d:%02d:%02d.%03d", ptm.wHour, ptm.wMinute, ptm.wSecond, ptm.wMilliseconds);
						fprintf(fp, " %4d> ", qsize);
						fprintf(fp, "<< 時間の逆流を検知しました: %04d/%02d/%02d %02d:%02d:%02d.%03d → %04d/%02d/%02d %02d:%02d:%02d.%03d >>",
							st1.wYear, st1.wMonth, st1.wDay, st1.wHour, st1.wMinute, st1.wSecond, st1.wMilliseconds,
							st2.wYear, st2.wMonth, st2.wDay, st2.wHour, st2.wMinute, st2.wSecond, st2.wMilliseconds);
						fprintf(fp, "\n");
					}

					s_ft = ft;

					fprintf(fp, "<%02d:%02d:%02d.%03d", ptm.wHour, ptm.wMinute, ptm.wSecond, ptm.wMilliseconds);
					fprintf(fp, " %4d> ", qsize);
					fprintf(fp, "%s", msg.c_str());
					fprintf(fp, "\n");

					fclose(fp);
				} else {
				}

				ReleaseMutex(g_hmtxSyslog);		//  Mutexの解放
			} else {
				assert( 0 );
			}
		}
#endif
	}

	return 0;
}


//-----------------------------------------------------------------------------
// ログの保存先パスを取得。
//
// [引数]
// pszBuffer  パス名を取得するバッファへのポインタ
// nSize      バッファのサイズ
//
// [戻り値]
// パス名を受け取るために必要なバッファのサイズ。
//
// [説明]
// pszBuffer が NULL の場合、必要なバッファを返します。
//
// nSize で指定されたエリアが不十分な場合、nSize - 1 文字だけコピーします。
//-----------------------------------------------------------------------------
int WSSLOG_API WLGetLogPath(char* pszBuffer, int nSize) {
	if (pszBuffer) {
		memset(pszBuffer, 0, nSize);
		strncpy(pszBuffer, m_LogPath, nSize - 1);
	}
	return strlen(m_LogPath) + 1;
}


//-----------------------------------------------------------------------------
// ログの保存先パスを設定。
//
// [引数]
// pszPath  新しいログの保存先
//
// [説明]
// 新しいログの保存先に環境変数が含まれていた場合、展開されます。
//-----------------------------------------------------------------------------
void WSSLOG_API WLSetLogPath(const char* pszPath) {

	// 指定されたパスが、NULL 又は、空文字列のときは、カレントディレクトリを設定。
	if ((pszPath == NULL) || (lstrlen(pszPath) == 0)) {
		GetCurrentDirectory(MAX_PATH, m_LogPath);
		PathAddBackslash(m_LogPath);
		return;
	}

	char work[MAX_PATH];

	// 環境変数を展開します。
	ExpandEnvironmentStrings(pszPath, m_LogPath, MAX_PATH);

	if (PathIsRelative(m_LogPath)) {
		// 相対パスを指定されたときは、カレントディレクトリからの絶対パスに変換します。
		GetCurrentDirectory(MAX_PATH, work);
		PathAppend(work, m_LogPath);
		lstrcpy(m_LogPath, work);
	} else {
		// 絶対パスを指定されていても、ドライブ名が入っていないときがある
		// ドライブ名が入っていないときは、カレントディレクトリのドライブを割り当てる。
		if (PathGetDriveNumber(m_LogPath) == -1) {
			GetCurrentDirectory(MAX_PATH, work);
			PathStripToRoot(work);
			PathAppend(work, m_LogPath);
			lstrcpy(m_LogPath, work);
		}
	}

	// パス名の終端に \ をつけておきます。
	PathAddBackslash(m_LogPath);
}


//-----------------------------------------------------------------------------
// iniファイル(WinSousa.ini)から、ログパスとログレベルの取得
//
// [説明]
// 鶏と卵の関係になるため、GetIniFile.dllの関数は、使えないので注意。
//-----------------------------------------------------------------------------
void GetIniFile() {
	char cPath[MAX_PATH];
	char cLogPath[MAX_PATH];

	// WinSousa.iniをセットする。
	PathAppend(strcpy(cPath, m_BasePath), "WinSousa.ini");

	// ログＰＡＴＨの取得
	GetPrivateProfileString("Log", "LogPath", "", cLogPath, _MAX_PATH, cPath);
	if (strlen(cLogPath) == 0) {
		GetPrivateProfileString("Pathinf", "LogPath", "Log", cLogPath, _MAX_PATH, cPath);
	}

	// モジュール名.ini に LogPath の記述がある場合、そちらを優先する
	{
		char temp[MAX_PATH];
		char szModuleName[MAX_PATH];
		GetModuleFileName(NULL, szModuleName, sizeof(szModuleName));
		PathRenameExtension(szModuleName, ".ini");
		GetPrivateProfileString("WssLog", "LogPath", "", temp, sizeof(temp), szModuleName);
		if (lstrlen(temp) != 0) {
			lstrcpy(cLogPath, temp);
		}
	}


	// ログフルパス
	{
		TCHAR temp[MAX_PATH];
		PathAppend(lstrcpy(temp, m_BasePath), cLogPath);
		WLSetLogPath(temp);
	}

	// ログファイル名の形
	m_logNameType = GetPrivateProfileInt(_T("Log"), _T("LogNameType"), 1, cPath);

	// LogLevelの取得
	m_nLogLevel = GetPrivateProfileInt(_T("Log"), _T("LogLevel"), 2, cPath);

	// 保存日数の取得
	m_nSaveDays = GetPrivateProfileInt(_T("Log"), _T("SaveDays"), DEFAULT_LOG_SAVEDAYS, cPath);

	// ミューテックス名を作成する。(C:\temp\temp.exe → mtx_C:/temp/)
	// ミューテックス名　"mtx_" + m_BasePath
	lstrcat(lstrcpy(m_MTX_SYSLOG, _T("mtx_")), m_BasePath);

	// ミューテックス名に[\]を使用できないため、　[\]を[/]に置き換える
	for (int i=0 ; i<(int)lstrlen(m_MTX_SYSLOG) ; i++) {
		if (m_MTX_SYSLOG[i] == '\\') {
			m_MTX_SYSLOG[i] = '/';
		}
	}

	// レジストリから syslog mode かどうか、取得する。
	kjm::registry reg;
	if (reg.reg_open_key_ex(HKEY_LOCAL_MACHINE, _T("Software\\Sanko\\Wss"), KEY_READ) == ERROR_SUCCESS) {
		m_syslogServer = reg.get_string(_T("SyslogServer"), _T(""));
	}

}

//-----------------------------------------------------------------------------
// 保存日付の取得
//-----------------------------------------------------------------------------
void GetSaveDate(LPTSTR cSaveDate) {
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


//-----------------------------------------------------------------------------
// 保存期間を超えた Log フォルダを削除する。
//
// [戻り値]
// 常に TRUE 。
//-----------------------------------------------------------------------------
BOOL WSSLOG_API WLDeleteOldLog() {
	Winssa_Log("WSSLOG", LOGLV_INFO, "<%s> WLDeleteOldLog() enter.", m_appName);

	// 保存日付の取得
	char cSaveDate[16];
	GetSaveDate(cSaveDate);
	Winssa_Log("WSSLOG", LOGLV_INFO, "<%s> WLDeleteOldLog(): '%s' より古い日付フォルダを削除します。", m_appName, cSaveDate);

	// ログフォルダを検索するワイルドカードを用意
	char wc[MAX_PATH];
	PathAppend(lstrcpy(wc, m_LogPath), "*.*");
	Winssa_Log("WSSLOG", LOGLV_INFO, "<%s> WLDeleteOldLog(): '%s' を検索します。", m_appName, wc);

	// ログフォルダを列挙
	WIN32_FIND_DATA findData;
	HANDLE hFind = FindFirstFile(wc, &findData);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				// ディレクトリだけを対象にする
				if ((lstrcmp(findData.cFileName, ".") != 0) && (lstrcmp(findData.cFileName, "..") != 0)) {
					// '.' と '..' 以外を対象にする
					char fullName[MAX_PATH];
					PathAppend(lstrcpy(fullName, m_LogPath), findData.cFileName);

					if (lstrcmp(findData.cFileName, cSaveDate) <= 0) {
						Winssa_Log("WSSLOG", LOGLV_INFO, "<%s> WLDeleteOldLog(): フォルダ '%s' を削除します。", m_appName, fullName);

						if (!kjm::util::remove_folder(fullName)) {
							Winssa_Log("WSSLOG", LOGLV_ERR, "<%s> WLDeleteOldLog(): error %u : フォルダの削除に失敗しました。", m_appName, GetLastError());
						}
					} else {
						// 残すファイルはログに残しません。
						/// Winssa_Log("WSSLOG", LOGLV_INFO, "<%s> WLDeleteOldLog(): フォルダ '%s' は残します。", m_appName, fullName);
					}
				}
			}
		} while (FindNextFile(hFind, &findData));
		FindClose(hFind);
	} else {
		Winssa_Log("WSSLOG", LOGLV_ERR, "<%s> WLDeleteOldLog(): error %u : フォルダの検索に失敗しました。", m_appName, GetLastError());
	}

	Winssa_Log("WSSLOG", LOGLV_INFO, "<%s> WLDeleteOldLog() leave.", m_appName);
	return TRUE;
}


//-----------------------------------------------------------------------------
// 外部に公開するログ関数
//
// [引数]
//	const char* sikibetu	: ログ識別文字列
//	long  loglevel				: ログレベル
//	const char *format		 :ログ文字列
//
// [機能説明]
//	フォーマットに従った文字列をログファイルに出力する
//-----------------------------------------------------------------------------
void WINAPI Winssa_Log(const char *sikibetu,long loglevel,const char * format, ...) {

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

	if (loglevel == LOGLV_ALWAYS) {
		;										//　ログレベル：必ずログを出す(注).頻繁に起こるものに関しては使用禁止
	} else if (loglevel < s_traceLevel) {			//　ログレベルが小さいのでログはとらない
		return;
	}

#if defined(USE_THREAD)
	// スレッドモードの時は、ログ文字列を作成して、queueにpushする。
	{
		va_start(arg, format);

		vsprintf(s_buff, format, arg);

		SYSTEMTIME	ptm;
		GetLocalTime(&ptm);

		sprintf(buf,"%02d:%02d:%02d.%03d [%-6s]{%04X:%04X}L%d:",
				ptm.wHour, ptm.wMinute, ptm.wSecond, ptm.wMilliseconds, sikibetu, 
				GetCurrentProcessId(), GetCurrentThreadId(), loglevel);

		// ログ文字列を作成して、queueにpushする。
		std::string msg = buf;
		msg += s_buff;

		g_syslogc.lock();
		g_logQ.push(msg);
		g_syslogc.unlock();
	}
#else
	char logfname[MAX_PATH];
	char Path_Log[MAX_PATH];

	// 通常モードの時は、この場でファイルに書き出す。
	DWORD dwRet = WaitForSingleObject(g_hmtxSyslog, 3000);
	if (dwRet == WAIT_OBJECT_0 || dwRet == WAIT_ABANDONED) {	//  成功

		va_start(arg, format);

		_vsnprintf(s_buff, sizeof(s_buff), format, arg);

		SYSTEMTIME	ptm;
		GetLocalTime(&ptm);

		static FILETIME s_ft;
		FILETIME ft;
		SystemTimeToFileTime(&ptm, &ft);

		sprintf(buf,"%02d:%02d:%02d.%03d [%-6s]{%04X:%04X}L%d:",
				ptm.wHour, ptm.wMinute, ptm.wSecond, ptm.wMilliseconds, sikibetu,
				GetCurrentProcessId(), GetCurrentThreadId(), loglevel);

		sprintf(Path_Log,"%s%04d%02d%02d", m_LogPath, ptm.wYear, ptm.wMonth, ptm.wDay);

		kjm::create_directory(Path_Log);

		PathAddBackslash(Path_Log);

		//  デバッグ出力
		OutputDebugString(buf);
		OutputDebugString(s_buff);
		OutputDebugString("\n");

		//　ログファイル名を作成
		if (m_logNameType == 1) {
			sprintf(logfname,"%04d%02d%02d_%02d.log", ptm.wYear, ptm.wMonth, ptm.wDay, ptm.wHour);
		} else {
			sprintf(logfname,"%04d%02d%02d.log", ptm.wYear, ptm.wMonth, ptm.wDay);
		}

		PathAppend(Path_Log, logfname);

		// ログファイルに出力
		FILE* fp = fopen(Path_Log, "a");
		if (fp) {
			if (*(__int64*)&ft < *(__int64*)&s_ft) {
				// 時間の逆流を検知
				SYSTEMTIME st1, st2;
				FileTimeToSystemTime(&s_ft, &st1);
				FileTimeToSystemTime(&ft, &st2);

				fprintf(fp, "%s", buf);
				fprintf(fp, "<%s> << 時間の逆流を検知しました: %04d/%02d/%02d %02d:%02d:%02d.%03d → %04d/%02d/%02d %02d:%02d:%02d.%03d >>",
					m_appName,
					st1.wYear, st1.wMonth, st1.wDay, st1.wHour, st1.wMinute, st1.wSecond, st1.wMilliseconds,
					st2.wYear, st2.wMonth, st2.wDay, st2.wHour, st2.wMinute, st2.wSecond, st2.wMilliseconds);
				fprintf(fp, "\n");
			}

			s_ft = ft;

			fprintf(fp, "%s", buf);
			fprintf(fp, "%s", s_buff);
			fprintf(fp, "\n");

			fclose(fp);
		}

		ReleaseMutex(g_hmtxSyslog);		//  Mutexの解放
	}
	else {
		assert( 0 );
	}
#endif

	force_a_linker_to_link_a_floating_point_library = 2.2;
}


//-----------------------------------------------------------------------------
// TraceLog を wsslog で置き換えるためのログ開始関数
//
// ダミーです。常に成功を返します。
//-----------------------------------------------------------------------------
int WINAPI TraceLogStart(unsigned char mainver, unsigned char minnerver, DWORD stTraceInfo) {
	return 0;
}


//-----------------------------------------------------------------------------
// TraceLog を wsslog で置き換えるためのログ開始関数
//
// WLDeleteOldLog に置き換わります。
//-----------------------------------------------------------------------------
int WINAPI TraceLogDeleteOldFile(short index/* = 0*/) {
	WLDeleteOldLog();
	return 0;
}


//-----------------------------------------------------------------------------
// TraceLog を wsslog で置き換えるためのログ関数
//
// tracestatus は、0～19までがログレベルで、それ以外の値は、識別情報になっている。
// (tracestatus % 20) として、
//   15 以上, LOGLV_ALWAYS
//   12 以上, LOGLV_INFO
//    9 以上, LOGLV_ERR
//    6 以上, LOGLV_WARNING
//    3 以上, LOGLV_DEBUG
//    0 以上, LOGLV_TRACE
// に置き換えて Winssa_Log に渡す。
//-----------------------------------------------------------------------------
void WINAPI TraceLog(short tracestatus, const char *format, ...) {
	std::vector<char> buffer(256);
	va_list marker;

	va_start(marker, format);
	while (_vsnprintf(&buffer[0], buffer.size(), format, marker) == -1) {
		buffer.resize(buffer.size() * 2);
	}
	va_end(marker);

	int loglv;
	int tracelevel = tracestatus % 20;
	if (tracelevel >=15)		loglv = LOGLV_ALWAYS;
	else if (tracelevel >= 12)	loglv = LOGLV_INFO;
	else if (tracelevel >= 9)	loglv = LOGLV_ERR;
	else if (tracelevel >= 6)	loglv = LOGLV_WARNING;
	else if (tracelevel >= 3)	loglv = LOGLV_DEBUG;
	else						loglv = LOGLV_TRACE;

	Winssa_Log("TRCLOG", loglv, "%s", &buffer[0]);
}


//-----------------------------------------------------------------------------
//	外部に公開するログ関数
//
// [引数]
//	const char* sikibetu	: ログ識別文字列
//	long  loglevel				: ログレベル
//	const char *format		 :ログ文字列
//
// [機能説明]
//	フォーマットに従った文字列をログファイルに出力する
//-----------------------------------------------------------------------------
void WINAPI WinssaVB_Log(const char *sikibetu,long loglevel,const char * format) {
	Winssa_Log(sikibetu, loglevel, format);
}


//-----------------------------------------------------------------------------
//	外部に公開するログ関数
//
// [引数]
//	const char* sikibetu	: ログ識別文字列
//	long  loglevel			: ログレベル
//	const void *buffer		 :ダンプデータのアドレス
//	long  size				 :ダンプデータのサイズ
//
// 機能説明
//	ダンプデータをログファイルに出力する
//-----------------------------------------------------------------------------
void WINAPI Winssa_Dump(const char *sikibetu,long loglevel, const void* buffer, long size) {
	m_bDump = TRUE;			// ダンプモード
	m_pDumpAdr = (void *)buffer;	// ダンプアドレス
	m_lDumpSize = size;		// ダンプサイズ

	Winssa_Log(sikibetu, loglevel, "<Dump>Start");
}


//-----------------------------------------------------------------------------
// VB用のダンプデータをログする関数
//-----------------------------------------------------------------------------
void WINAPI WinssaVB_Dump(const char *sikibetu,long loglevel, const void* buffer, long size) {
	Winssa_Dump(sikibetu, loglevel, buffer, size);
}


//-----------------------------------------------------------------------------
// ログファイルのタイプを設定する
//-----------------------------------------------------------------------------
void WSSLOG_API WLSetLogNameType(int nNameType) {
	m_logNameType = nNameType;
}


//-----------------------------------------------------------------------------
// 現在のログファイルのタイプを取得する
//-----------------------------------------------------------------------------
long WSSLOG_API WLGetLogNameType() {
	return m_logNameType;
}


//-----------------------------------------------------------------------------
// ログレベルの取得
//-----------------------------------------------------------------------------
long WINAPI WLGetLogLevel() {
	long nWork;
	InterlockedExchange(&nWork, s_traceLevel);
	return nWork;
}


//-----------------------------------------------------------------------------
// ログレベルの設定
//-----------------------------------------------------------------------------
void WINAPI WLSetLogLevel(long nNewLogLevel) {
	if (nNewLogLevel < LOGLV_DEBUG || nNewLogLevel > LOGLV_FATALERR)
		return;

	InterlockedExchange(&s_traceLevel, nNewLogLevel);
}


//-----------------------------------------------------------------------------
// ログレベルの設定
//
// [備考]
// WLSetLogLevel 関数を利用してください。
//-----------------------------------------------------------------------------
void WINAPI Winssa_Loglevel(long loglevel) {
	WLSetLogLevel(loglevel);
}


//-----------------------------------------------------------------------------
// キューに溜まったログがなくなるまで待つ
//
// [備考]
// スレッドモードの時のみ有効な関数。
// 通常モードの時は、何もしない。
//-----------------------------------------------------------------------------
void WSSLOG_API WLFlushLog() {
#if defined(USE_THREAD)
	while (1) {
		g_syslogc.lock();
		bool bEmpty = g_logQ.empty();
		g_syslogc.unlock();

		if (bEmpty) {
			break;
		} else {
			Sleep(50);
		}
	}
#endif
}

//-----------------------------------------------------------------------------
// 自分自身の識別情報を返す。
//-----------------------------------------------------------------------------
void WSSLOG_API WLGetMyName(char *buf, int size) {
	GetModuleFileName(g_hinstDLL, buf, size);
	lstrcat(buf, "|RK edition");
//	strcpy(buf, "WLGetMyName called");
}

//-----------------------------------------------------------------------------
// 保存日数を取得(default: 90 (days))
//-----------------------------------------------------------------------------
int WSSLOG_API WLGetSaveDays() {
	return m_nSaveDays;
}

//-----------------------------------------------------------------------------
// 保存日数を設定
//-----------------------------------------------------------------------------
void WSSLOG_API WLSetSaveDays(int newValue) {
	m_nSaveDays = newValue;
}

//-----------------------------------------------------------------------------
//	[概要]
//	DLLエントリーポイント
//-----------------------------------------------------------------------------
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {

	UNREFERENCED_PARAMETER(lpvReserved);

	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		g_hinstDLL = hinstDLL;

		// winsock を初期化
#if 0
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif
		// アプリケーション名とINIファイルの読込先を決定する
		FixIniPath(m_appName, m_BasePath);

		// 初回process attachだけ、ログレベルを設定する
		if (s_bAttached == FALSE) {
			s_bAttached = TRUE;
			GetIniFile();
			WLSetLogLevel(m_nLogLevel);

			g_hmtxSyslog = CreateMutex(NULL, FALSE, m_MTX_SYSLOG);	// ログファイル用mutex

		}

#if defined(USE_THREAD)
		// syslog client用スレッド起動!
		g_syslogc.run();
#endif

		break;

	case DLL_PROCESS_DETACH:

#if defined(USE_THREAD)
		// syslog client用スレッド終了!
		g_syslogc.end();
		g_syslogc.wait(5000);
#endif

		// プロセス終了時に、ミューテックスを破棄する。
		if (g_hmtxSyslog != NULL) {
			ReleaseMutex(g_hmtxSyslog);
			CloseHandle(g_hmtxSyslog);
			g_hmtxSyslog = NULL;
		}
#if 0
		WSACleanup();
#endif
		break;
	}

	return TRUE;
}
