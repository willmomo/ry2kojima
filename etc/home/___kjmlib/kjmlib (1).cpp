/**
	@file
	@brief	kjmlib.h で宣言しているクラスの実装。
*/

#include "kjmlib.h"
#include <vector>
#include <stdarg.h>
#include <time.h>
#include <assert.h>

#include <shlwapi.h>


//-------------------------------------------------------------------------------
// kjm::file
//-------------------------------------------------------------------------------

/**
	デフォルトコンストラクタ
*/
kjm::file::file() : m_fp(NULL)
{
}

/**
	デストラクタ
*/
kjm::file::~file()
{
	this->fclose();
}

/**
	ファイルを開く

	@param	fname	ファイル名へのポインタ。詳細は、fopen 関数を参照
	@param	mode	ファイルを開くモードへのポインタ。詳細は、fopen 関数を参照
	@retval	true	正常にファイルオープン
	@retval	false	ファイルオープンに失敗
*/
bool kjm::file::fopen(const char* fname, const char* mode)
{
	assert(m_fp == NULL);

	m_fp = ::fopen(fname, mode);

	return (m_fp != NULL);
}

/**
	ファイルを閉じる

	@note	ファイルをオープンしていなくても呼び出せる。
*/
void kjm::file::fclose()
{
	if (m_fp) {
		::fclose(m_fp);
		m_fp = NULL;
	}
}


/**
	一行書き込み

	@param	str	書き込む文字列へのポインタ
	@retval	fputs 関数を参照
*/
int kjm::file::fputs(const char* str)
{
	assert(m_fp != NULL);

	int ret;

	ret = ::fputs(str, m_fp);
	if (ret < 0)	return ret;

	return ret;
}


/**
	一行読み込み

	@retval	改行、または、EOFまで、読み込んだ文字列を返す。
*/
std::string kjm::file::fgets()
{
	std::string result;
	char buf[14];

	while (1) {
		// 改行 or EOFまで、読み込む
		char* p = ::fgets(buf, sizeof(buf), m_fp);
		if (p == NULL)	return result;

		// 読み込んだデータの最後が改行なら、一行読み込んだことになるので
		// 読み込み終了。
		if (buf[strlen(p) - 1] == '\n') {
			result += buf;
			break;
		}

		// 次の一文字を先読み
		int c = ::fgetc(m_fp);
		if (feof(m_fp)) {
			// ファイル終端まで読み込んでいるので、一行読み込んだとして終了
			result += buf;
			break;
		} else {
			::ungetc(c, m_fp);
			result += buf;
		}
	}

	return result;
}


/**
	フォーマット書き込み

	@param	format	printf 関数を参照
*/
void kjm::file::fprintf(const char* format, ...)
{
	assert(m_fp != NULL);

	::va_list marker;
	va_start(marker, format);
	this->vfprintf(format, marker);
	va_end(marker);
}


/**
	フォーマット書き込み
	@param format	vfprintf関数を参照
	@param argptr	vfprintf関数を参照
*/
void kjm::file::vfprintf(const char* format, va_list argptr)
{
	assert(m_fp != NULL);

	::vfprintf(m_fp, format, argptr);
}


//-------------------------------------------------------------------------------
// kjm::time
//-------------------------------------------------------------------------------

/**
	デフォルトコンストラクタ
*/
kjm::time::time() : m_t(0)
{
}


/**
	コピーコンストラクタ
*/
kjm::time::time(const kjm::time& src) : m_t(src.m_t)
{
}


/**
	time_t で初期化するコンストラクタ
*/
kjm::time::time(time_t t) : m_t(t)
{
}


/**
	デストラクタ
*/
kjm::time::~time()
{
}


/**
	現在時間を保持する time を返す関数
*/
kjm::time kjm::time::now()
{
	return kjm::time(::time(NULL));
}


/**
	日時をフォーマットする
	@param	format	strftime 関数のヘルプを参照。
	@return	strftime 関数によってフォーマットされた文字列を返す。
*/
std::string kjm::time::strftime(const char* format)
{
	std::vector<char> buf;

	do {
		buf.resize(buf.size() + 256);
	} while (::strftime(buf.begin(), buf.size(), format, ::localtime(&m_t)) == 0);

	return std::string(buf.begin());
}


/**
	代入演算子
	@param	rhs	コピー元(Right Hand Side)
	@return	lhs(Left Hand Side)(自分自身)を返す。
*/
kjm::time& kjm::time::operator =(const kjm::time& rhs)
{
	if (this == &rhs)	return *this;	// 同一オブジェクトコピーの回避
	this->m_t = rhs.m_t;
	return *this;
}


//-------------------------------------------------------------------------------
// kjm::win32
//-------------------------------------------------------------------------------

/**
	ExpandEnvironmentStrings のラッパー

	@param	src	環境変数を含んだ文字列へのポインタ
	@retval	環境変数を展開した後の文字列
*/
std::string kjm::win32::ExpandEnvironmentStrings(const char* src)
{
	DWORD dwNeeded = ::ExpandEnvironmentStrings(src, NULL, 0);

	std::vector<char> dst(dwNeeded);

	::ExpandEnvironmentStrings(src, dst.begin(), dst.size());

	return std::string(dst.begin());
}


/**
	GetModuleFileName のラッパー
	@param hModule	GetModuleFileName を参照。デフォルトNULL。
	@retval	モジュールのファイル名
*/
std::string kjm::win32::GetModuleFileName(HMODULE hModule)
{
	DWORD ret;
	std::vector<char> work;
	do {
		work.resize(work.size() + MAX_PATH);
		ret = ::GetModuleFileName(hModule, work.begin(), work.size());
	} while (ret == work.size());

	return work.begin();
}


//-------------------------------------------------------------------------------
// kjm::UrlComponents
//-------------------------------------------------------------------------------

/**
	InternetCrackUrl のラッパー

	@param	lpszUrl	URL形式の文字列へのポインタ
	@retval	TRUE	変換に成功
	@retval FALSE	変換に失敗
	@note	TRUE を戻した場合のみ、メンバ変数に意味のある値が入る。
	@warning 分解された文字列は、おのおの1024バイト以内である必要がある。
*/
BOOL kjm::UrlComponents::CrackUrl(const char* lpszUrl)
{
	char bufScheme[1024];
	char bufHostName[1024];
	char bufUserName[1024];
	char bufPassword[1024];
	char bufUrlPath[1024];
	char bufExtraInfo[1024];

	URL_COMPONENTS uc;

	memset(&uc, 0, sizeof(uc));

	uc.dwStructSize = sizeof(uc);
	uc.lpszScheme = bufScheme;
	uc.dwSchemeLength = sizeof(bufScheme);
	uc.lpszHostName = bufHostName;
	uc.dwHostNameLength = sizeof(bufHostName);
	uc.lpszUserName = bufUserName;
	uc.dwUserNameLength = sizeof(bufUserName);
	uc.lpszPassword = bufPassword;
	uc.dwPasswordLength = sizeof(bufPassword);
	uc.lpszUrlPath = bufUrlPath;
	uc.dwUrlPathLength = sizeof(bufUrlPath);
	uc.lpszExtraInfo = bufExtraInfo;
	uc.dwExtraInfoLength = sizeof(bufExtraInfo);

	BOOL ret = ::InternetCrackUrl(lpszUrl, 0, 0, &uc);
	if (ret) {
		m_szScheme = uc.lpszScheme;
		m_nScheme = uc.nScheme;
		m_szHostName = uc.lpszHostName;
		m_nPort = uc.nPort;
		m_szUserName = uc.lpszUserName;
		m_szPassword = uc.lpszPassword;
		m_szUrlPath = uc.lpszUrlPath;
		m_szExtraInfo = uc.lpszExtraInfo;
	}

	return ret;
}


//-------------------------------------------------------------------------------
// kjm::Internet
//-------------------------------------------------------------------------------

/**
	デフォルトコンストラクタ
*/
kjm::Internet::Internet() : m_hInternet(NULL), m_lastError(ERROR_SUCCESS)
{
}


/**
	デストラクタ
*/
kjm::Internet::~Internet()
{
	InetCloseHandle();
}


/**
	InternetOpen のラッパー
	@param lpszAgent InternetOpen 関数のヘルプを参照。
	@param dwAccessType InternetOpen 関数のヘルプを参照。
	@param lpszProxyName InternetOpen 関数のヘルプを参照。
	@param lpszProxyBypass InternetOpen 関数のヘルプを参照。
	@param dwFlags InternetOpen 関数のヘルプを参照。
	@retval TRUE 正常終了
	@retval FALSE エラー終了。エラーの詳細は、get_lastError メンバで参照
*/
BOOL kjm::Internet::InetOpen(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxyName, LPCSTR lpszProxyBypass, DWORD dwFlags)
{
	assert(m_hInternet == NULL);
	if  (!(m_hInternet == NULL))	return FALSE;

	m_hInternet = ::InternetOpen(lpszAgent, dwAccessType, lpszProxyName, lpszProxyBypass, dwFlags);
	m_lastError = ::GetLastError();
	return (m_hInternet != NULL);
}


/**
	InternetConnect のラッパー
	@param rInetSession		InetOpen を使って初期化された、kjm::Internet への参照。
	@param lpszServerName	InternetConnect 関数のヘルプを参照。
	@param nServerPort		InternetConnect 関数のヘルプを参照。
	@param lpszUsername		InternetConnect 関数のヘルプを参照。
	@param lpszPassword		InternetConnect 関数のヘルプを参照。
	@param dwService		InternetConnect 関数のヘルプを参照。
	@param dwFlags			InternetConnect 関数のヘルプを参照。
	@param dwContext		InternetConnect 関数のヘルプを参照。
	@retval TRUE 正常終了
	@retval FALSE エラー終了。エラーの詳細は、get_lastError メンバで参照
*/
BOOL kjm::Internet::InetConnect(kjm::Internet& rInetSession, LPCSTR lpszServerName, INTERNET_PORT nServerPort,
								LPCSTR lpszUsername, LPCSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD dwContext)
{
	assert(m_hInternet == NULL);
	if  (!(m_hInternet == NULL))	return FALSE;

	m_hInternet = ::InternetConnect(rInetSession.m_hInternet, lpszServerName, nServerPort, lpszUsername, lpszPassword, dwService, dwFlags, dwContext);
	m_lastError = ::GetLastError();
	return (m_hInternet != NULL);
}


/**
	FtpOpenFile関数のラッパー
	@param rFtpSession	InetConnect を使って初期化された、kjm::Internet への参照。
	@param lpszFileName	FtpOpenFile 関数のヘルプを参照。
	@param fdwAccess	FtpOpenFile 関数のヘルプを参照。
	@param dwFlags		FtpOpenFile 関数のヘルプを参照。
	@param dwContext	FtpOpenFile 関数のヘルプを参照。
	@retval TRUE 正常終了
	@retval FALSE エラー終了。エラーの詳細は、get_lastError メンバで参照
*/
BOOL kjm::Internet::InetFtpOpenFile(kjm::Internet& rFtpSession, LPCSTR lpszFileName, DWORD fdwAccess, DWORD dwFlags, DWORD dwContext)
{
	assert(m_hInternet == NULL);
	if  (!(m_hInternet == NULL))	return FALSE;

	m_hInternet = ::FtpOpenFile(rFtpSession.m_hInternet, lpszFileName, fdwAccess, dwFlags, dwContext);
	m_lastError = ::GetLastError();
	return (m_hInternet != NULL);
}


/**
	FtpGetFile関数のラッパー
	@param	lpszRemoteFile				FtpGetFile関数のヘルプを参照。
	@param	lpszNewFile					FtpGetFile関数のヘルプを参照。
	@param	fFailIfExists				FtpGetFile関数のヘルプを参照。
	@param	dwLocalFlagsAndAttributes	FtpGetFile関数のヘルプを参照。
	@param	dwInternetFlags				FtpGetFile関数のヘルプを参照。
	@param	dwContext					FtpGetFile関数のヘルプを参照。
	@return	FtpGetFile関数の戻り値を参照。エラーの詳細は、get_lastErrorメンバで参照可能。
	@warning	InetConnectメンバで初期化した、インスタンスのみで使用可能。
*/
BOOL kjm::Internet::InetFtpGetFile(LPCSTR lpszRemoteFile, LPCSTR lpszNewFile, BOOL fFailIfExists, DWORD dwLocalFlagsAndAttributes, DWORD dwInternetFlags, DWORD dwContext)
{
	assert(m_hInternet != NULL);
	if  (!(m_hInternet != NULL))	return FALSE;

	BOOL ret = ::FtpGetFile(m_hInternet, lpszRemoteFile, lpszNewFile, fFailIfExists, dwLocalFlagsAndAttributes, dwInternetFlags, dwContext);
	m_lastError = ::GetLastError();
	return ret;
}


/**
	FtpPutFile関数のラッパー
	@param	lpszLocalFile		FtpPutFile関数のヘルプを参照。
	@param	lpszNewRemoteFile	FtpPutFile関数のヘルプを参照。
	@param	dwFlags				FtpPutFile関数のヘルプを参照。
	@param	dwContext			FtpPutFile関数のヘルプを参照。
	@return	FtpPutFile関数の戻り値を参照。エラーの詳細は、get_lastErrorメンバで参照可能。
	@warning	InetConnectメンバで初期化した、インスタンスのみで使用可能。
*/
BOOL kjm::Internet::InetFtpPutFile(LPCSTR lpszLocalFile, LPCSTR lpszNewRemoteFile, DWORD dwFlags, DWORD dwContext)
{
	assert(m_hInternet != NULL);
	if  (!(m_hInternet != NULL))	return FALSE;

	BOOL ret = ::FtpPutFile(m_hInternet, lpszLocalFile, lpszNewRemoteFile, dwFlags, dwContext);
	m_lastError = ::GetLastError();
	return ret;
}


/**
	InternetReadFile関数のラッパー
	@param lpBuffer					InternetReadFile 関数のヘルプを参照。
	@param dwNumberOfBytesToRead	InternetReadFile 関数のヘルプを参照。
	@param lpNumberOfBytesRead		InternetReadFile 関数のヘルプを参照。
	@return InternetReadFile 関数の戻り値を参照。エラーの詳細は、get_lastError メンバで参照。
	@warning InetFtpOpenFile メンバで初期化した、インスタンスのみで使用可能。
*/
BOOL kjm::Internet::InetReadFile(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead) 
{
	assert(m_hInternet != NULL);
	if  (!(m_hInternet != NULL))	return FALSE;

	BOOL ret = ::InternetReadFile(m_hInternet, lpBuffer, dwNumberOfBytesToRead, lpNumberOfBytesRead);
	m_lastError = ::GetLastError();
	return ret;
}


/**
	InternetReadFile関数のラッパー。指定された、vector@<char@>のバッファにすべて読み込む。
	@param buffer	読み込むためのバッファ
	@return InternetReadFile 関数の戻り値を参照。エラーの詳細は、get_lastError メンバで参照。
	@warning InetFtpOpenFile メンバで初期化した、インスタンスのみで使用可能。
	@note
	引数の buffer は、クリアされます。
*/
BOOL kjm::Internet::InetReadFile(std::vector<char>& buffer)
{
	assert(m_hInternet != NULL);
	if  (!(m_hInternet != NULL))	return FALSE;

	char temp[9];
	buffer.clear();

	DWORD dwRead;
	BOOL ret;
	do {
		dwRead = 0;
		ret = InetReadFile(temp, sizeof(temp), &dwRead);

		if (ret && dwRead > 0) {
			buffer.insert(buffer.end(), &temp[0], &temp[dwRead]);
		}
	} while (ret && dwRead != 0);

	return ret;
}


/**
	InternetWriteFile関数のラッパー
	@param	lpBuffer					InternetWriteFile 関数のヘルプを参照。
	@param	dwNumberOfBytesToWrite		InternetWriteFile 関数のヘルプを参照。
	@param	lpdwNumberOfBytesWritten	InternetWriteFile 関数のヘルプを参照。
	@return InternetWriteFile 関数の戻り値を参照。エラーの詳細は、get_lastError メンバで参照。
*/
BOOL kjm::Internet::InetWriteFile(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpdwNumberOfBytesWritten)
{
	assert(m_hInternet != NULL);
	if  (!(m_hInternet != NULL))	return FALSE;

	BOOL ret = ::InternetWriteFile(m_hInternet, lpBuffer, dwNumberOfBytesToWrite, lpdwNumberOfBytesWritten);
	m_lastError = ::GetLastError();
	return ret;
}


/**
	InternetQueryDataAvailable関数のラッパー
	@param	lpdwNumberOfBytesAvailable	InternetQueryDataAvailable 関数のヘルプを参照。
	@param	dwFlags						InternetQueryDataAvailable 関数のヘルプを参照。
	@param	dwContext					InternetQueryDataAvailable 関数のヘルプを参照。
	@return InetQueryDataAvailable 関数の戻り値を参照。エラーの詳細は、get_lastError メンバで参照。
*/
BOOL kjm::Internet::InetQueryDataAvailable(LPDWORD lpdwNumberOfBytesAvailable, DWORD dwFlags, DWORD dwContext)
{
	assert(m_hInternet != NULL);
	if  (!(m_hInternet != NULL))	return FALSE;

	BOOL ret = ::InternetQueryDataAvailable(m_hInternet, lpdwNumberOfBytesAvailable, dwFlags, dwContext);
	m_lastError = ::GetLastError();
	return ret;
}


/**
	InternetCloseHandle関数のラッパー
	@retval	TRUE	正常終了
	@retval FALSE	エラー終了
	@note
	ハンドルを開いていない状態で呼び出しても、正常に動作する。
*/
BOOL kjm::Internet::InetCloseHandle()
{
	BOOL ret = TRUE;
	if (m_hInternet) {
		ret = ::InternetCloseHandle(m_hInternet);
		m_lastError = ::GetLastError();
		m_hInternet = NULL;
	}
	return ret;
}


/**
	内部でAPIを使用した直後の GetLastError 関数の戻り値を返す
	@return 最後に実行した API の、GetLastError() 関数の値。
*/
DWORD kjm::Internet::get_lastError()
{
	return m_lastError;
}


//-------------------------------------------------------------------------------
// kjm::command_line_info
//-------------------------------------------------------------------------------

/// デフォルトコンストラクタ
kjm::command_line_info::command_line_info()
{
}


/// デストラクタ
kjm::command_line_info::~command_line_info()
{
}

/**
	コマンドラインの解析
	@param	argc	main関数のargc, WinMain関数の__argcを渡す
	@param	argv	main関数のargv, WinMain関数の__argvを渡す
*/
void kjm::command_line_info::parse_command_line(int argc, char** argv)
{
	for (int i = 1; i < argc; i++) {
		bool flag = (argv[i][0] == '/' || argv[i][0] == '-') ? true : false;

		if (i + 1 < argc) {
			if (parse_command(flag, &argv[i][(flag) ? 1 : 0], argv[i + 1])) {
				++i;
			}
		} else {
			parse_command(flag, &argv[i][(flag) ? 1 : 0], NULL);
		}
	}
}


//-------------------------------------------------------------------------------
// kjm::logfile
//-------------------------------------------------------------------------------

/**
	デフォルトコンストラクタ
	@note
	デフォルトでログファイルの名前を、%temp%\<実行ファイル名>.logで初期化する。
*/
kjm::logfile::logfile()
{
	std::vector<char> path(MAX_PATH);

	::PathAppend(
		lstrcpy(path.begin(), kjm::win32::ExpandEnvironmentStrings("%temp%").c_str()),
		::PathFindFileName(kjm::win32::GetModuleFileName().c_str()));

	::PathRenameExtension(path.begin(), ".log");

	this->m_fname = path.begin();
}


/// デストラクタ
kjm::logfile::~logfile()
{
}


/**
	シグネチャを出力する
*/
void kjm::logfile::logsig()
{
	log_f("%s", kjm::win32::GetModuleFileName().c_str());
}


/**
	フォーマット付きログ出力
*/
void kjm::logfile::log_f(const char* format, ...)
{
	kjm::file f;
	if (f.fopen(m_fname.c_str(), "a+")) {
		f.fprintf("[%s][p%04X][t%04X] ", kjm::time::now().strftime("%Y/%m/%d %H:%M:%S").c_str(), ::GetCurrentProcessId(), ::GetCurrentThreadId());

		::va_list marker;
		va_start(marker, format);
		f.vfprintf(format, marker);
		va_end(marker);

		f.fputs("\n");
	}
}
