//---------------------------------------------------------------------
// kjmlib : Version 0.2015.12.8
//
// プリコンパイル済みヘッダを使用しないように設定してください。
//
// r.kojima
//---------------------------------------------------------------------

#pragma once

#pragma warning(disable : 4786)
#pragma warning(disable : 4996)

#if !defined(__KJMLIB_H)
#define __KJMLIB_H

#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#endif

#if !defined(_WINSOCKAPI_)
// winsock.h がインクルードされているときは、winsock2.h は、インクルードしません。
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#include <windows.h>
#include <tchar.h>
#include <shellapi.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "version.lib")

#include <lm.h>
#pragma comment(lib, "netapi32.lib")

// kjm::util::createShortcutで使用する.
#include <shlobj.h>

// kjm::odbcUtilで使用するヘッダ
#if defined(USE_ODBC32)
#include <sql.h>
#include <sqlext.h>
#pragma comment(lib, "odbc32.lib")
#endif

#include <string>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <iomanip>

// vs2010から：back_inserterを使うためにインクルードが必要
#include <iterator>

#include <assert.h>
#include <process.h>
#include <time.h>
#include <share.h>
#include <locale.h>
#include <sys/stat.h>
#include <io.h>
#include <fcntl.h>

#include "option.h"

#if defined(USE_WSSLOG)
#include "wsslog.h"
// #pragma comment(lib, "wsslog.lib")
#else
#define LOGLV_INFO	1
#define LOGLV_WARNING	2
#define LOGLV_ERR	3
void Winssa_Log(LPCTSTR ident, int level, LPCTSTR format, ...);
void WLDeleteOldLog();
#endif


#if defined(USE_MSXML6)
#import <msxml6.dll> named_guids
#endif


// Windows の DOS 窓でUnicodeを出力できるようにする。
// Windows 下の諸々の処理よりも、中国語なども表示できるようになり便利
// printf系、cout系どちらにも有効
// MBCSの時は何もしない処理になる。
//
// !!!! ただし、出力結果がUnicdeになるので、
// !!!! リダイレクトして作成したファイルもUnicode になる。
// !!!! コマンドラインコマンドとして使うなら、
// !!!! 下にある「STL_SETLOCALE_JAPAN」の方が扱いやすい。
//
#if defined(UNICODE)
#define SET_UNICODE_MODE	{ \
	_setmode(_fileno(stdout), _O_U16TEXT); \
	_setmode(_fileno(stderr), _O_U16TEXT); \
}
#else
#define SET_UNICODE_MODE
#endif

// STL の setlocale(LC_ALL, _T("japan")) 相当処理は、以下の通り。
// std::locale コンストラクタは常に、_MBCS文字列を受け取る。
#define STL_SETLOCALE_JAPAN		std::locale::global(std::locale("japanese"))
#define C_SETLOCALE_JAPAN		_tsetlocale(LC_ALL, _T("japanese"))

// 日本語ロケールをベースに数値カテゴリファセットだけCロケールに置き換えるというやりかた。
///#define STL_SETLOCALE_JAPAN		{ std::locale loc(std::locale("japanese"), "C", std::locale::numeric); std::locale::global(loc); }

// ただし、std::locale::global(std::locale("japanese")) で全体を日本語にしてあると、
// 2,010/10/15 みたいな文字列が出来上がってしまう。
// これを回避するために、***.imbue(std::locale("C")) する必要がある。
// _otstringstream oss;
// oss.imbue(std::locale("C"));

#if !defined(countof)
#define countof(A)	(sizeof((A)) / sizeof((A)[0]))
#endif

// WinMain 使用時の __argv を使い分けるマクロ
// !!! tchar.h に定義されているようなので、事実上のコメントアウト !!! <2012/06/01>
#if !defined(_INC_TCHAR)
#if defined(UNICODE)
#define __targv		__wargv
#else
#define __targv		__argv
#endif
#endif

// WinMain を int WINAPI tWinMain(HINSTANCE, HINSTANCE LPTSTR, int) と書けるようにするためのマクロ
// !!! tchar.h に _tWinMain があるので、そっちを使おう。
#if defined(UNICODE)
#define tWinMain	wWinMain
#else
#define tWinMain	WinMain
#endif

// __FILE__ が、常に MBCS なので、_T__FILE__ で UNICODE と両立できるようにするためのマクロ
#define	WIDEN2(x)	L ## x
#define	WIDEN(x)	WIDEN2(x)

#if defined(UNICODE)
#define _T__FILE__	WIDEN(__FILE__)
#else
#define _T__FILE__	__FILE__
#endif

#if defined(UNICODE)
#define _tout				std::wcout
#define _tcout				std::wcout
#define _terr				std::wcerr
#define _tcerr				std::wcerr
#define _otstream			std::wostream
#define _otstringstream		std::wostringstream
#define _tifstream			std::wifstream
#define _tstringstream		std::wstringstream
#else
#define _tout				std::cout
#define _tcout				std::cout
#define _terr				std::cerr
#define _tcerr				std::cerr
#define _otstream			std::ostream
#define _otstringstream		std::ostringstream
#define _tifstream			std::ifstream
#define _tstringstream		std::stringstream
#endif

// __int64 を iostream に出力するための関数
#if _MSC_VER < 1500		// 少なくとも、vs2008からは、__int64を受け取るostreamが存在するので、無効にする。
_otstream& operator<<(_otstream& os, __int64 i);
#endif


#if _MSC_VER <= 1200	// VC6 より大きい
#define INVALID_FILE_ATTRIBUTES	0xffffffff
#endif

// A <= X <= B を判定するマクロ
#define InRANGE(A, X, B)	(((A) <= (X)) && ((X) <= (B)))


#include "kjmbase.h"
#include "path.h"

namespace kjm {


	class systemException {
	public:
		systemException(DWORD code, const _tstring& message) : m_code(code), m_message(message) {};
		virtual ~systemException() {};

		// エラーコードを取得
		DWORD getCode() { return m_code; };

		// エラーメッセージを取得
		_tstring getMessage() { return m_message; };

	private:
		DWORD m_code;
		_tstring m_message;

	private:
		systemException() {};
	};

	//-----------------------------------------------------------------
	// 関数内で発生したWin32エラーを例外として流すクラス
	//-----------------------------------------------------------------
	class Win32Exception {
	public:
		Win32Exception(DWORD dwError) : m_dwError(dwError) {};
		virtual ~Win32Exception() {};

	public:
		DWORD m_dwError;
	};

	//-------------------------------------------------------------------------
	// このネームスペースで使われる、一般的な例外型
	//-------------------------------------------------------------------------
	class exception {
	public:
		exception() {};
		exception(DWORD errCode, const _tstring& errMsg) : m_errCode(errCode), m_errMsg(errMsg) {}
		virtual ~exception() {}

	public:
		DWORD m_errCode;
		_tstring m_errMsg;
	};

	//-------------------------------------------------------------------------
	// 引数の形式が無効である場合の例外
	//-------------------------------------------------------------------------
	class formatException : public kjm::exception {
	public:
		formatException() : kjm::exception(1, _T("日付と時刻の有効な文字列形式を格納していません。")) {}
		virtual ~formatException() {}
	};

	//-------------------------------------------------------------------------
	// kjmlib で使う汎用例外
	//-------------------------------------------------------------------------
	class kjmException {
	public:
		kjmException(DWORD errorCode, const std::basic_string<TCHAR>& msg) : m_errorCode(errorCode), m_msg(msg) {}
		kjmException(const kjmException& src) : m_errorCode(src.m_errorCode), m_msg(src.m_msg) {}
		virtual ~kjmException() {}

	public:
		DWORD m_errorCode;
		std::basic_string<TCHAR> m_msg;

	private:
		kjmException() {}
	};

	void setVerboseMode(bool mode);
	bool getVerboseMode();

	// OutputDebugString を簡単に使うための関数
	void ods(LPCTSTR format, ...);

	void log_info(LPCTSTR signature, LPCTSTR format, ...);	// 情報ログを残す関数
	void log_error(LPCTSTR signature, LPCTSTR format, ...);	// エラーログを残す関数

	BOOL create_directory(LPCTSTR lpPathName);	// 再帰的にディレクトリを作成できる CreateDirectory 関数
	
	//=================================================================
	// 文字コードに関する処理
	//=================================================================

	class encoding {
	private:
		encoding() {}
		~encoding() {}

	public:
		// 一般的なロケールを設定する関数
		static void setLocale() {
			_tsetlocale(LC_ALL, _T("Japanese"));
		}

		// Unicodeをasciiに変換
		// !defined(_UNICODE)の時に使われる関数
		static std::string to_mbs(const std::string& src);

		// Unicodeをasciiに変換
		// defined(_UNICODE)の時に使われる関数
		static std::string to_mbs(const std::wstring& src);
	};

	//-------------------------------------------------------------------------
	// base64 encodeを行うクラス
	//-------------------------------------------------------------------------
	class base64 {
	public:
		static std::string encode(const std::vector<char>& data);
		static std::vector<char> decode(const std::string& str);
	};

	//-------------------------------------------------------------------------
	// 難読化を行うクラス
	//-------------------------------------------------------------------------
	class crypt {
	public:
		static std::string decrypt(const std::string& str);
		static std::string encrypt(const std::string& str);
	};

	//-------------------------------------------------------------------------
	// CSV ファイルを扱うクラス
	//-------------------------------------------------------------------------
	class csvFile {
	public:
		// コンストラクタ
		csvFile() {}

		// デストラクタ
		virtual ~csvFile() { this->close(); }

		// CSV ファイルを開く
		bool open(LPCTSTR pszFileName);

		// CSV ファイルを閉じる
		void csvFile::close();

		// CSV ファイルから 1 行読み込む
		int parseCsvCol(std::vector<std::basic_string<TCHAR> >& cols);

	private:
		std::basic_ifstream<TCHAR> m_fr;
	};

	//-----------------------------------------------------------------
	// クリティカルセクションを管理するクラス
	//-----------------------------------------------------------------
	class CriticalSection : public CRITICAL_SECTION {
	public:
		//-------------------------------------------------------------
		// コンストラクタ(クリティカルセクションの初期化も兼ねる)
		//-------------------------------------------------------------
		CriticalSection() {
			::InitializeCriticalSection(this);
		};

		//-------------------------------------------------------------
		// デストラクタ
		//-------------------------------------------------------------
		~CriticalSection() {
			::DeleteCriticalSection(this);
		};

		//-------------------------------------------------------------
		// 所有権の取得
		//-------------------------------------------------------------
		VOID Enter() {
			::EnterCriticalSection(this);
		};

		//-------------------------------------------------------------
		// 所有権の破棄
		//-------------------------------------------------------------
		VOID Leave() {
			::LeaveCriticalSection(this);
		};
	};

	//-------------------------------------------------------------------------
	// netapi32.lib で提供される関数のラッパー
	// lm.h のインクルードと、netapi32.lib をリンク
	//-------------------------------------------------------------------------
	class netapi32 {
	private:
		netapi32() {}
		~netapi32() {}

	public:
		// ワークグループ名を取得する
		static std::basic_string<TCHAR> getWorkgroupName();
	};

	// API関数のラッパークラス
	//
	class win32 {
	private:
		win32() {};		// デフォルトコンストラクタ
		~win32() {};	// デストラクタ

	public:
		// ExpandEnvironmentStrings のラッパー
		//
		static std::basic_string<TCHAR> expand_environment_strings(LPCTSTR lpSrc) {
			DWORD dwSizeNeed;

			// 必要なバッファサイズを取得
			dwSizeNeed = ::ExpandEnvironmentStrings(lpSrc, NULL, 0);
			if (dwSizeNeed == 0) {
				return std::basic_string<TCHAR>();	// エラー
			}

			// 必要なバッファを確保して、再実行
			std::vector<TCHAR> buf(dwSizeNeed);
			dwSizeNeed = ::ExpandEnvironmentStrings(lpSrc, &buf[0], buf.size());
			if (dwSizeNeed != buf.size() - 1) {
				return std::basic_string<TCHAR>();	// エラー
			}

			return std::basic_string<TCHAR>(&buf[0]);	// 展開された文字列
		};

		static std::basic_string<TCHAR> format_message(DWORD dwErrorCode);	// エラーコードからシステムエラーメッセージを取得する。
		static std::basic_string<TCHAR> str_replace(const std::basic_string<TCHAR>& src, LPCTSTR pFind, LPCTSTR pReplace);	// 文字列を置換する。VBのStrReplaceのような関数。

		// ワークエリアの中心にウィンドウを移動させる。
		static BOOL MoveCenterWorkarea(HWND hwnd);
	};

	//-------------------------------------------------------------------------
	// レジストリを扱うクラス
	//-------------------------------------------------------------------------
	class registry {
	public:
		// デフォルトコンストラクタ
		registry() : m_hkey(NULL) {}

		// コピーコンストラクタ
		registry(const registry& src);

		// デストラクタ
		virtual ~registry() { reg_close_key(); }

		// RegOpenKeyEx のラッパー
		//
		// hkey        開くキーへのハンドル
		// lpSubKey    開くべきサブキーの名前へのアドレス
		// samDesired  セキュリティアクセスマスク
		LONG reg_open_key_ex(HKEY hkey, LPCTSTR lpSubKey, REGSAM samDesired);

		// RegCloseKey のラッパー
		LONG reg_close_key();

		// RegQueryValueEx のラッパー
		//
		// lpValueName  検索すべき値の名前へのアドレス
		// lpType       値のタイプのためのバッファへのアドレス
		// lpData       データのバッファへのアドレス
		// lpcbData     データのバッファのサイズへのアドレス
		LONG reg_query_value_ex(LPTSTR lpValueName, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData) const;


		// RegSetValueEx のラッパー
		//
		// lpValueName  検索すべき値の名前へのアドレス
		// lpType       値のタイプのためのバッファへのアドレス
		// lpData       データのバッファへのアドレス
		// lpcbData     データのバッファのサイズへのアドレス
		LONG reg_set_value_ex(LPCTSTR lpValueName, DWORD dwType, CONST BYTE *lpData, DWORD cbData) {
			assert(m_hkey != NULL);
			return ::RegSetValueEx(m_hkey, lpValueName, 0, dwType, lpData, cbData);
		}
		
		// RegSetValueEx のラッパー
		//
		// lpValueName  検索すべき値の名前へのアドレス
		// dwData       データのバッファへのアドレス
		LONG reg_set_value_ex(LPCTSTR lpValueName, DWORD dwData) {
			return reg_set_value_ex(lpValueName, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData));
		}
		
		// 文字列を取得する関数
		//
		// 値のタイプが
		//   REG_SZ のときは、そのまま返す。
		//   REG_EXPAND_SZ のときは、ExpandEnvironmentStrings を通してから返す。
		//   REG_DWORD のときは、_ultoa を通してから返す。
		std::basic_string<TCHAR> get_string(LPTSTR lpValueName, LPCTSTR lpDefault) const;

		// RegEnumValue のラッパー
		LONG reg_enum_value(DWORD dwIndex, kjm::_tstring& valueName, DWORD& type, std::vector<BYTE>& data);

	public:
		// アクセサ
		HKEY getKey() { return m_hkey; };
		LONG RegOpenKeyEx( HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired );
		LONG RegCloseKey();
		LONG RegQueryValueEx( LPCTSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, std::vector<BYTE>& data );

	public:
		// 代入演算子
		registry& operator =(const registry& rhs);

	private:
		HKEY m_hkey;	// オープン中のレジストリへのハンドル
	};

	//-----------------------------------------------------------------
	// ユーティリティクラス
	//
	// [概要]
	// スタティックメンバーのみなので、インスタンス化できない。
	//-----------------------------------------------------------------
	class util {
	private:
		util() {};
		~util() {};

	public:
		// CopyFileEx の簡単実装。
		static BOOL copyFileEx(LPCTSTR lpExistingFileName,
								  LPCTSTR lpNewFileName,
								  LPPROGRESS_ROUTINE lpProgressRoutine,
								  LPVOID lpData,
								  LPBOOL pbCancel,
								  DWORD dwCopyFlags);

		// 文字列を置換する。VBのStrReplaceのような関数。
		static _tstring strReplace(const _tstring& src, const _tstring& strFind, const _tstring& strReplace);

		// バッファからCRC16を計算する
		static unsigned short calcCRC16(unsigned char *p, unsigned int n);

		// ファイルハンドルからCRC16を計算する
		static unsigned short calcCRC16(FILE* fp);

		// ファイル名からCRC16を計算する
		static unsigned short calcCRC16(const _tstring& pfname);

		// PathRenameExtensionのラッパー
		static _tstring &pathRenameExtension(_tstring &strPath, LPCTSTR pszExt);

		// Unicode 文字列を ANSI 文字列に変換
		static std::string toAnsi(const std::wstring& src);

		// ANSI 文字列を Unicode 文字列に変換
		static std::wstring toUnicode(const std::string& src);

		// Unicode 文字列を ANSI 文字列に変換
		static std::string toAnsi(const std::wstring& src, int cp);

		// ANSI 文字列を Unicode 文字列に変換
		static std::wstring toUnicode(const std::string& src, int cp);

		// 文字列終端の改行を取り除く
		static std::basic_string<TCHAR>& chomp(std::basic_string<TCHAR>& s);

		// コピー元の直下にあるすべてのファイルをコピー先にコピーする
		//
		// [引数]
		// pszSrcPath  コピー元フォルダの名前
		// pszDstPath  コピー先フォルダの名前
		//
		// [戻り値]
		// 正常終了: NO_ERROR
		// エラー終了: Win32エラーコード
		static DWORD CopyFiles(LPCTSTR pszSrcPath, LPCTSTR pszDstPath);

		// システム標準のエラーメッセージを作成
		static std::basic_string<TCHAR> formatMessageBySystem(DWORD dwErr);

		// 汎用的なフォーマットメッセージの処理
		static DWORD formatMessage( DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, std::vector<TCHAR>& buffer, va_list* Arguments );

		// 1行読み込み関数
		//
		// @return 0 正常終了
		// @return 1 eofで読み込み終了
		// @erturn -1 errorで読み込み終了
		static int getline(FILE *fp, kjm::_tstring &str);

		// Win32API GetModuleFileName を安全に使う関数
		static _tstring getModuleFileName(HMODULE hModule = NULL);

		// モジュールファイルと同名のiniファイルを取得する
		static kjm::_tstring getModuleIniFile(HMODULE hModule = NULL) {
			return kjm::util::pathRenameExtension(kjm::util::getModuleFileName(), _T(".ini"));
		}

		// モジュールが存在するフォルダ名を取得する
		static kjm::_tstring getModulePath(HMODULE hModule = NULL);

		// string を返す GetComputerName 関数
		static std::basic_string<TCHAR> getComputerName();

		// プロセス名をstringで取得する関数
		static std::basic_string<TCHAR> GetModuleName() {
			return std::basic_string<TCHAR>(::PathFindFileName(kjm::util::getModuleFileName().c_str()));
		}

		// 環境変数を展開する
		static DWORD expandEnvironmentStrings(LPCTSTR lpSrc, std::vector<TCHAR>& rDst);

		// 環境変数の値を取得する
		static _tstring getEnvironmentVariable(LPCTSTR lpName);

		// ファイルのバージョンリソースを取得
		static _tstring getFileVersion(const _tstring &strFileName);

		// ファイルのバージョンリソースを取得
		static _tstring getFileVersion() {
			return kjm::util::getFileVersion(kjm::util::getModuleFileName());
		}

		// ini ファイルから文字列を取得する(GetPrivateProfileString 関数のラッパー)
		static std::basic_string<TCHAR> getPrivateProfileString(const std::basic_string<TCHAR>& section,
			const std::basic_string<TCHAR>& key, const std::basic_string<TCHAR>& defValue, const std::basic_string<TCHAR>& iniFile);
		static _tstring get_private_profile_string(const _tstring& lpSectionName, const _tstring& lpKeyName, const _tstring& lpDefault, const _tstring& lpFileName) {
			return kjm::util::getPrivateProfileString(lpSectionName, lpKeyName, lpDefault, lpFileName);
		}

		static DWORD get_ipaddr(LPCTSTR lpName);	// ホスト名または、IP文字列からIPアドレスを取得

		// ディレクトリを再帰的に削除する
		static bool remove_folder(LPCTSTR pszPath);

		// ディレクトリを再帰的に削除する
		static void removeFolder(const _tstring& pszPath);

		// 文字列を返す安全な sprintf 関数
		static std::basic_string<TCHAR> sprintf_str(LPCTSTR format, ...);

		// ファイル名部分を取得
		static std::basic_string<TCHAR> findFileName(const std::basic_string<TCHAR>& path);

		// パス文字列にパスを追加
		static std::basic_string<TCHAR> append(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& more);
		static std::basic_string<TCHAR> path_append(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& more) {
			return kjm::util::append(path, more);
		}

		// パス文字列の終端 \ を削除する
		static std::basic_string<TCHAR> removeBackslash(const std::basic_string<TCHAR>& path);

		// パス文字列の拡張子を削除する
		static std::basic_string<TCHAR> removeExtension(const std::basic_string<TCHAR>& path);

		// パス文字列の拡張子を変更する
		static std::basic_string<TCHAR> renameExtension(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& ext);

		// モジュールのパス文字列の拡張子を変更する
		static std::basic_string<TCHAR> renameExtension(const std::basic_string<TCHAR>& ext, HMODULE hModule = NULL);

		// パス文字列のファイル名を変更する
		static std::basic_string<TCHAR> renameFileSpec(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& fname);

		// パス文字列のファイル名部分を削除する
		static std::basic_string<TCHAR> removeFileSpec(const std::basic_string<TCHAR>& path);

		// ディレクトリかどうか調べる
		static bool isDirectory(const std::basic_string<TCHAR>& path) { return (PathIsDirectory(path.c_str())) ? true : false; }

		// LoadString 関数のラッパー
		static _tstring loadString(UINT uID);
		static _tstring load_string(UINT uID, HINSTANCE hInstance = NULL);

		// ファイルの有無を確認する
		static bool fileExists(const std::basic_string<TCHAR>& path);

		// カレントディレクトリを変更する
		static bool setCurrentDirectory(const std::basic_string<TCHAR>& pathName);
		
		// ショートカットの作成
		//
		// strFileName : リンクファイルの名前
		// strFile : リンクを作成するファイルの名前
		// strArgs : リンクに渡す引数
		// strDir : 作業ディレクトリ
		//
		static bool createShortcut(
			const std::basic_string<TCHAR>& strFileName,
			const std::basic_string<TCHAR>& strFile,
			const std::basic_string<TCHAR>& strArgs, 
			const std::basic_string<TCHAR>& strDir);

		// スペシャルフォルダを取得する
		// 少なくとも、vs2008から有効な関数
#if _WIN32_IE >= 0x0700
		static std::basic_string<TCHAR> getSpecialFolderLocation(int nFolder);
#endif

		// 整数を文字列に変換
		static std::basic_string<TCHAR> to_a(int value, int radix = 10);

		// 整数を文字列に変換(printf相当のフォーマットが指定できる)
		static std::basic_string<TCHAR> to_a(int value, const std::basic_string<TCHAR>& format);

		// 複数ファイルを削除出る DeleteFile 
		static bool deleteFiles(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildcard, int nDays = 0);

		// %TEMP%フォルダを取得する
		static std::basic_string<TCHAR> getTempPath();

		// 指定フォルダ以下の読み込み専用属性を外す
		static bool UnsetReadOnly(const std::basic_string<TCHAR>& path);

		// 指定の共有フォルダと共有パスの存在を保証する
		static int guaranteeSharedFolder(LPCTSTR pszSharedFolder, LPCTSTR pszSharedPath);

		// 安全な vsprintf
		static int vsprintf_vec(std::vector<TCHAR>& buf, LPCTSTR format, va_list argptr);

		// 安全な sprintf 
		//
		// [引数]
		// 一番目の引数が、vector<TCHAR>への参照になっている以外、
		// sprintf関数と同じです。
		static int sprintf_vec(std::vector<TCHAR>& buf, LPCTSTR format, ...);

		// 文字列の分割
		static std::vector<std::basic_string<TCHAR> > split(const std::basic_string<TCHAR>& src, TCHAR c);

		// 文字列に変換
		static std::basic_string<TCHAR> toString(DWORD value);

		// 文字列の比較(大文字・小文字を無視して比較)
		static bool equalsIgnoreCase(const std::basic_string<TCHAR>& lhs, const std::basic_string<TCHAR>& rhs);

		// GetFullPathName APIのラッパー
		static int getFullPathName(const kjm::_tstring& fileName, kjm::_tstring& pathPart, kjm::_tstring& filePart);

		// 数値を human readable format な文字列にする
		static _tstring toHumanReadable(__int64 value);

		// コピー処理は CopyFile API を使う。
		// コピー先の読込属性を落とす機能付き。
		//
		// [戻り値]  1   正常終了(コピー先の読み込み属性を落とした)
		// [戻り値]  0   正常終了
		// [戻り値] -1   CopyFile APIの失敗
		// [戻り値] -12  コピー先ファイルの属性取得失敗
		// [戻り値] -13  コピー先ファイルの読み込み属性リセット失敗
		static int copyFile2(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists, BOOL bFailIfReadonly=TRUE);

		// 文字列を小文字化する
		static _tstring toLower(const _tstring& src);

		// 文字列を大文字化する
		static _tstring toUpper(const _tstring& src);

		// 文字列の前方から、空白、タブ、改行文字を取り除く
		static kjm::_tstring trimLeft(const kjm::_tstring &s);

		// 文字列前後の、空白、タブ、改行文字を取り除く
		static kjm::_tstring trim(const kjm::_tstring &s);

		// _MSC_VER から対応するコンパイラの名前を取得
		static _tstring getMscVerText(int mscVer = _MSC_VER);

		// 名前の被らないフォルダを作成
		static kjm::_tstring makeTempFolder(const kjm::_tstring& basePath, const kjm::_tstring& prefix);


		static std::string toString(const std::vector<char>& data);
		static std::wstring toString(const std::vector<wchar_t>& data);
		static std::string toAnsiString(std::vector<BYTE>& data);
		static std::wstring toUnicodeString(std::vector<BYTE>& data);
		static _tstring get_environment_variable(const _tstring& lpName);
		static _tstring path_rename_extension(const _tstring& pszPath, const _tstring& pszExt);
		static _tstring get_module_filename(HMODULE hModule = NULL);

		//---------------------------------------------------------------------
		// pszPathで指定したディレクトリ内の、現在日時からnDayより古いファイル
		// を削除します。
		//
		// 削除対象は、pszPath直下のファイルだけです。
		//---------------------------------------------------------------------
		static void cleanup_dir(const _tstring& pszPath, int nDay);

		static std::basic_string<TCHAR> str_replace(
			const std::basic_string<TCHAR>& strSrc,			// 置換したい文字列を含む文字列
			const std::basic_string<TCHAR>& strFind,		// 検索する文字列
			const std::basic_string<TCHAR>& strReplace) {	// 置換する文字列

			std::basic_string<TCHAR> result = strSrc;
			std::basic_string<TCHAR>::size_type pos = 0;
			while ((pos = result.find(strFind, pos)) != std::basic_string<TCHAR>::npos) {
				result.replace(pos, strFind.length(), strReplace);
			}
			return result;
		}

		//-------------------------------------------------------------
		// SHFileOperation の FO_DELETE を使ったファイルの削除
		//-------------------------------------------------------------
		static int FODelete(LPCTSTR lpPathName) {
			// SHFileOperationの文字列は、\0\0で終わる必要があるっぽい
			TCHAR szFrom[MAX_PATH];
			szFrom[::lstrlen(::lstrcpy(szFrom, lpPathName)) + 1] = '\0';

			SHFILEOPSTRUCT fileOp;
			memset(&fileOp, 0, sizeof(fileOp));
			fileOp.wFunc = FO_DELETE;
			fileOp.pFrom = szFrom;
			fileOp.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;

			return ::SHFileOperation(&fileOp);
		};

		static BOOL MakeDirectory(LPCTSTR pszFolder) {
			if (PathIsRoot(pszFolder))	return TRUE;
			if (PathFileExists(pszFolder))	return TRUE;

			TCHAR szParent[MAX_PATH];
			PathRemoveFileSpec(lstrcpy(szParent, pszFolder));

			if (kjm::util::MakeDirectory(szParent)) {
				if (CreateDirectory(pszFolder, FALSE)) {
					return TRUE;
				}
			}

		#if defined(USE_WSSLOG)
			Winssa_Log(_T("KJMLIB"), LOGLV_ERR, _T("フォルダ作成異常: error %d: %s"), GetLastError(), pszFolder);
		#endif

			return FALSE;
		}
		//-------------------------------------------------------------
		// テンポラリ用のディレクトリパスを取得します。
		//-------------------------------------------------------------
		static std::basic_string<TCHAR> get_temp_path() {
			std::vector<TCHAR> work(MAX_PATH);
			DWORD dwRet = ::GetTempPath(work.size(), &work[0]);
			return &work[0];
		}
	};

	//-------------------------------------------------------------------------
	// パス名を扱う文字列
	//-------------------------------------------------------------------------
	class PathString {
	private:
		std::basic_string<TCHAR> m_path;

	public:
		// コンストラクタ
		PathString() : m_path() {}
		PathString( const PathString& src ) : m_path( src.m_path ) {}
		PathString( LPCTSTR src ) : m_path( src ) {}
		PathString(const std::basic_string<TCHAR>& src) : m_path(src) {}

		// デストラクタ
		virtual ~PathString() {};

	public:
		// 文字列への const pointer を戻す
		LPCTSTR c_str() const { return m_path.c_str(); }

		// 内部文字列を返す
		std::basic_string<TCHAR> getString() const { return m_path; }

		// 文字列にパスを結合する
		PathString& Append(const PathString& more);

		// 拡張子を取り除く
		PathString& removeExtension();

		// 拡張子を変更する
		PathString& renameExtension(const kjm::_tstring& newExt);

		// PathRemoveFileSpec のラッパー
		PathString& RemoveFileSpec() {
			std::vector<TCHAR> work(MAX_PATH);
			::PathRemoveFileSpec(lstrcpy(&work[0], m_path.c_str()));
			m_path = &work[0];
			return *this;
		}

		// const char* へのキャスト
		operator const TCHAR *() { return m_path.c_str(); };

		//-------------------------------------------------------------
		// ルートディレクトリを取得する
		//-------------------------------------------------------------
		std::basic_string<TCHAR> GetRoot() {
			std::vector<TCHAR> work(MAX_PATH);
			lstrcpy(&work[0], this->m_path.c_str());

			while (PathIsRoot(&work[0]) == FALSE) {
				if (!PathRemoveFileSpec(&work[0])) {
					return _T("");
				}
			}

			return &work[0];
		}
	public:
		// 代入演算子
		PathString& operator= (const PathString& src);
	};

	//-----------------------------------------------------------------
	// 時間を扱うクラス
	//-----------------------------------------------------------------
	class time {
	private:
		time_t m_time;

		time_t filetime_to_timet(const FILETIME& ft) {
			ULARGE_INTEGER ull;
			ull.LowPart = ft.dwLowDateTime;
			ull.HighPart = ft.dwHighDateTime;
			return ull.QuadPart / 10000000ULL - 11644473600ULL;
		}

		void TimetToFileTime( time_t t, LPFILETIME pft )
		{
			LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000;
			pft->dwLowDateTime = (DWORD) ll;
			pft->dwHighDateTime = ll >>32;
		}

	public:
		time() : m_time(0) {}
		time(time_t t) : m_time(t) {}
		time(LPCTSTR time_text) { parse(time_text); }
		time(SYSTEMTIME src) {
			FILETIME ft;
			SystemTimeToFileTime(&src, &ft);
			m_time = filetime_to_timet(ft);
		}
		time(FILETIME src) {
			m_time = filetime_to_timet(src);
		}
		virtual ~time() {}

		// 保持する日付を、ctime 関数で文字列にして返す。
		std::basic_string<TCHAR> ctime() {
			return kjm::util::chomp(std::basic_string<TCHAR>(::_tctime(&m_time)));
		}

		bool is_empty() { return (m_time == 0); }

		int year() const { return localtime( &m_time )->tm_year + 1900; }
		int month() const { return localtime( &m_time )->tm_mon + 1; }
		int date() const { return localtime( &m_time )->tm_mday; }
		int hour() const { return localtime( &m_time )->tm_hour; }
		int minute() const { return localtime( &m_time )->tm_min; }
		int second() const { return localtime( &m_time )->tm_sec; }
		int weekday() const { return localtime( &m_time )->tm_wday; }

		time& add_second(int sec);

		time_t get_time() const { return m_time; }
		std::basic_string<TCHAR> strftime(LPCTSTR format);

		void parse(LPCTSTR time_text );

		void operator=( time_t t ) { m_time = t; }
		void operator=( LPCTSTR time_text ) { parse( time_text ); }

		double operator-(const time& rhs) const { return difftime(m_time, rhs.m_time); }
		bool operator==( const time& rhs ) const { return m_time == rhs.m_time; }
		bool operator!=( const time& rhs ) const { return !( *this == rhs ); }
		bool operator<( const time& rhs ) const { return m_time < rhs.m_time; }
		bool operator>( const time& rhs ) const { return rhs < *this; }
		bool operator<=( const time& rhs ) const { return !( rhs < *this ); }
		bool operator>=( const time& rhs ) const { return !( *this < rhs ); }

		bool save(FILE* fp);
		bool load(FILE* fp);


	public:

		// 現在日時を保持するインスタンスを返す。
		static kjm::time GetCurrentTime() {
			return kjm::time(::time(NULL));
		}

		static kjm::time get_system_time() {
			SYSTEMTIME st;
			GetSystemTime(&st);
			return kjm::time(st);
		}

		static kjm::time get_local_time() {
			SYSTEMTIME st;
			GetLocalTime(&st);
			return kjm::time(st);
		}
		//-------------------------------------------------------------
		// 文字列に変換して返す
		//-------------------------------------------------------------
		std::basic_string<TCHAR> toString() {
			FILETIME gft;
			TimetToFileTime(m_time, &gft);

			FILETIME ft;
			SYSTEMTIME st;
			::FileTimeToLocalFileTime(&gft, &ft);
			::FileTimeToSystemTime(&ft, &st);
			
			TCHAR buf[512];
			wsprintf(buf, _T("%d年%d月%d日、%d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

			return buf;
		};
	};

	//-------------------------------------------------------------------------
	// 時差を扱うクラス
	//-------------------------------------------------------------------------

// FILETIME を扱いやすい時間に変換するための定数
#define FT_1USEC			(__int64)10
#define FT_1MSEC			(FT_1USEC * 1000)
#define FT_1SEC				(FT_1MSEC * 1000)
#define FT_1MIN				(FT_1SEC  *   60)	// FILETIME型の1分の値
#define FT_1HOUR			(FT_1MIN  *   60)	// FILETIME型の1時間の値
#define FT_1DAY				(FT_1HOUR *   24)	// FILETIME型の1日の値
#define FT_19700101			(116444736000000000)		// time_t の開始地点を表すFILETIMEの値
#define FT_19700101_SEC		(FT_19700101 / FT_1SEC)

	class timeSpan {
	public:
		static const long long TicksPerMillisecond = (long long)10000;
		static const long long TicksPerSecond = TicksPerMillisecond * (long long)1000;
		static const long long TicksPerMinute = TicksPerSecond * (long long)60;
		static const long long TicksPerHour = TicksPerMinute * (long long)60;
		static const long long TicksPerDay = TicksPerHour * (long long)24;

		static const long long TicksPer19700101 = (long long)116444736000000000;		// time_tの開始地点を表すFILETIMEの値
		static const long long TicksPerSec19700101 = TicksPer19700101 / TicksPerSecond;	// time_tの開始地点を表す秒の値

	public:
		// デフォルトコンストラクタ
		timeSpan() : m_span(0) {}

		// コピーコンストラクタ
		timeSpan(const timeSpan& src) : m_span(src.m_span) {}

		// __int64で初期化するコンストラクタ
		timeSpan(__int64 src) : m_span(src) {}

		// デストラクタ
		~timeSpan() {}

		// 代入演算子
		timeSpan& operator =(const timeSpan& rhs);

		// __int64に変換
		operator __int64 () { return m_span; }

		// ミリ秒で返す
		double totalMilliseconds() {
			__int64 result = m_span;			// original (100 nsec)

			result = result / (__int64)10;		// to usec
			result = result / (__int64)1000;	// to msec

			return (double)result;
		}

		// 秒数で返す
		double totalSeconds() { return this->totalMilliseconds() / 1000.0; }

		// 分で返す
		double totalMinutes() { return this->totalSeconds() / 60.0; }

		// 時間で返す
		double totalHours() { return this->totalMinutes() / 60.0; }

		// 日数で返す
		double totalDays() { return this->totalHours() / 24.0; }

	private:
		__int64 m_span;
	};

	/// <summary>kjm::dateTime オブジェクトが表す時刻の種類 (現地時刻、世界協定時刻 (UTC)、または、そのいずれでもない) を指定します。</summary>
	enum dateTimeKind {
		unspecified = 0,	/// 現地時刻と世界協定時刻 (UTC) のいずれも指定されていないことを表します。
		utc,				/// UTC 時刻であることを表します。
		local				/// 現地時刻であることを表します。
	};

	//-------------------------------------------------------------------------
	// 時間を扱うクラス
	//-------------------------------------------------------------------------
	class dateTime {
	private:
		FILETIME m_ft;
		SYSTEMTIME m_st;
		dateTimeKind m_kind;

	public:
		// デフォルトコンストラクタ
		dateTime() : m_ft(), m_kind(unspecified) {
			FileTimeToSystemTime(&m_ft, &m_st);
		}

		// コピーコンストラクタ
		dateTime(const dateTime& src) : m_ft(src.m_ft), m_st(src.m_st), m_kind(src.m_kind) {}

		// FILETIMEで初期化するコンストラクタ
		dateTime(FILETIME src, dateTimeKind kind = unspecified) : m_ft(src), m_kind(kind) {
			FileTimeToSystemTime(&m_ft, &m_st);
		}

		// SYSTEMTIME で初期化するコンストラクタ
		dateTime(const SYSTEMTIME& st, dateTimeKind kind = unspecified ) : m_kind(kind) {
			SystemTimeToFileTime(&st, &m_ft);
			FileTimeToSystemTime(&m_ft, &m_st);
		}

		// 年、月、日、時、分、秒で初期化するコンストラクタ
		dateTime(int year, int month, int day, int hour, int minute, int second) : m_kind(unspecified) {
			SYSTEMTIME st = {year, month, 0, day, hour, minute, second, 0};
			SystemTimeToFileTime(&st, &m_ft);
			FileTimeToSystemTime(&m_ft, &m_st);
		}

		// デストラクタ
		virtual ~dateTime() {}

		// 内部数値を int64 で取得
		__int64 get_Value() const { return *(__int64*)&m_ft; }
		
		// 現在時刻を__int64で返す
		__int64 getTime() const { return *(__int64*)&m_ft; }

		// 内部数値を FILETIME で返す
		FILETIME get_FILETIME_Value() const { return m_ft; }

		// ローカル時間に変更
		dateTime toLocalTime() const {
			if (m_kind == local) return *this;
			FILETIME ftLocal;
			FileTimeToLocalFileTime(&m_ft, &ftLocal);
			return kjm::dateTime(ftLocal, local);
		}
		
		// UTCに変換
		dateTime toUniversalTime() const {
			if (m_kind == utc) return *this;
			FILETIME ftUTC;
			LocalFileTimeToFileTime(&m_ft, &ftUTC);
			return kjm::dateTime(ftUTC, utc);
		}

		// 日付をフォーマットして出力
		_tstring toLongDateString() const;

		// 時間をフォーマットして出力
		_tstring toLongTimeString() const;

		// 日時をフォーマットして出力
		_tstring toString(const _tstring& format) const;

		// 日時をフォーマットして出力
		_tstring toString() const;

		// 時間を文字列(YYYYMMDD)に変換
		virtual _tstring toString_yyyymmdd() const;

		// 日付を足した dateTime を返す
		dateTime addDays(int nDays) const;

		// 代入演算子
		dateTime& operator=(const dateTime& rhs) {
			if (this == &rhs) return *this;
			m_ft = rhs.m_ft;
			m_st = rhs.m_st;
			m_kind = rhs.m_kind;
			return *this;
		}

		// dateTime の引き算
		timeSpan operator -(const dateTime& rhs) { return kjm::timeSpan(get_Value() - rhs.get_Value()); }
		
		// 比較演算子
		bool operator==( const dateTime& rhs ) const { return getTime() == rhs.getTime(); };
		
		// 比較演算子
		bool operator!=( const dateTime& rhs ) const { return !( *this == rhs ); };
		
		// 比較演算子
		bool operator<( const dateTime& rhs ) const { return getTime() < rhs.getTime(); };
		
		// 比較演算子
		bool operator>( const dateTime& rhs ) const { return rhs < *this; };
		
		// 比較演算子
		bool operator<=( const dateTime& rhs ) const { return !( rhs < *this ); };
		
		// 比較演算子
		bool operator>=( const dateTime& rhs ) const { return !( *this < rhs ); };

		// SYSTEMTIME へのキャスト
		operator SYSTEMTIME() const { return this->m_st; }

		// time_t へのキャスト
		// time関数で戻される時間は、UTCなので、UTCを返すようにする
		operator time_t() const {
			FILETIME ft = toUniversalTime().m_ft;;
			ULARGE_INTEGER ull;
			ull.LowPart = ft.dwLowDateTime;
			ull.HighPart = ft.dwHighDateTime;
			return ull.QuadPart / 10000000ULL - 11644473600ULL;
		}

		// 分を取得
		int get_Minute() const {
			return m_st.wMinute;
		}

		// 時を取得
		int get_Hour() const {
			return m_st.wHour;
		}

		// 日を取得
		int get_Day() const {
			return m_st.wDay;
		}
	
		// 月を取得
		int get_Month() const {
			return m_st.wMonth;
		}

		// 曜日を取得
		int get_Week() const {
			return m_st.wDayOfWeek;
		}

		// このインスタンスの値に、指定された秒数を加算した新しい dateTime を返します。
		dateTime addSeconds(LONGLONG value) const {
			LONGLONG ll = get_Value() + (kjm::timeSpan::TicksPerSecond * value);
			return dateTime(*(FILETIME*)&ll);
		}

	public:
		// 現在のローカル日時を取得する
		static dateTime getLocalTime() {
			SYSTEMTIME st;
			::GetLocalTime(&st);
			return dateTime(st, local);
		}

		// 現在のローカル日時を取得する
		static dateTime now() {
			return kjm::dateTime::getLocalTime();
		}

		// 現在のシステム日時(UTC)を取得する
		static dateTime getSystemTime() {
			SYSTEMTIME st;
			GetSystemTime(&st);
			return dateTime(st, utc);
		}

		// 現在のシステム日時(UTC)を取得する
		static dateTime utcNow() {
			return kjm::dateTime::getSystemTime();
		}

		// 文字列の日時と等価な dateTime オブジェクトを生成
		static dateTime parse(const kjm::_tstring& text);

		// 文字列を日時に変換してみる
		static bool tryParse(const kjm::_tstring& text, dateTime& result);

		// フォーマットにそって解析する関数 -- !!! 必要十分な実装しかしていません !!!
		//
		// 例) dateTime::parseExact(_T("20150915171300"), _T("yyyyMMddHHmmss"));
		// 例) dateTime::parseExact(_T("2004/08/24 20:23:06"), _T("yyyy/MM/dd HH:mm:ss"));
		// 例) dateTime::parseExact(_T("2004年08月24日20時23分06秒"), _T("yyyy年MM月dd日HH時mm分ss秒"));
		//
		// yyyy = 4桁の年(のつもり)
		// MM = 2桁の月(のつもり)
		// dd = 2桁の日(のつもり)
		// HH = 2桁の24時(のつもり)
		// mm = 2桁の分(のつもり)
		// ss = 2桁の秒(のつもり)
		//
		static dateTime parseExact(const std::basic_string<TCHAR>& s, const std::basic_string<TCHAR>& f) {
			TCHAR work[2] = {0};
			int _Y,_M,_D,_h,_m,_s;
			_Y = _M = _D = _h = _m = _s = 0;
			LPCTSTR pf = f.c_str(), ps = s.c_str();
			while (*pf != '\0') {
				if (*ps == '\0') throw formatException();
				if (*pf == 'y') {
					if (_istdigit(work[0] = *(ps++)) == 0) throw formatException();
					_Y = _Y * 10 + _ttoi(&work[0]);
				} else if (*pf == 'M') {
					if (_istdigit(work[0] = *(ps++)) == 0) throw formatException();
					_M = _M * 10 + _ttoi(&work[0]);
				} else if (*pf == 'd') {
					if (_istdigit(work[0] = *(ps++)) == 0) throw formatException();
					_D = _D * 10 + _ttoi(&work[0]);
				} else if (*pf == 'H') {
					if (_istdigit(work[0] = *(ps++)) == 0) throw formatException();
					_h = _h * 10 + _ttoi(&work[0]);
				} else if (*pf == 'm') {
					if (_istdigit(work[0] = *(ps++)) == 0) throw formatException();
					_m = _m * 10 + _ttoi(&work[0]);
				} else if (*pf == 's') {
					if (_istdigit(work[0] = *(ps++)) == 0) throw formatException();
					_s = _s * 10 + _ttoi(&work[0]);
				} else if (*pf != *(ps++)) {
					throw formatException();
				}
				++pf;
			}
			return dateTime(_Y, _M, _D, _h, _m, _s);
		}

		static bool tryParseExact(const std::basic_string<TCHAR>& s, const std::basic_string<TCHAR>& f, dateTime& result) {
			bool ret = true;
			try {
				result = kjm::dateTime::parseExact(s, f);
			} catch (exception ex) {
				ret = false;
			}
			return ret;
		}
	};

	//=====================================================================
	// FindFile系関数のラッパー
	//=====================================================================
	class FindFile {
	public:
		//-------------------------------------------------------------
		// デフォルト コンストラクタ
		//-------------------------------------------------------------
		FindFile() : m_hFindFile(INVALID_HANDLE_VALUE) {};

		//-------------------------------------------------------------
		// デストラクタ
		//-------------------------------------------------------------
		virtual ~FindFile() { Close(); };

		//-------------------------------------------------------------
		// FindFirstFile関数のラッパー
		//-------------------------------------------------------------
		bool FindFirst(const std::basic_string<TCHAR>& lpPathName, const std::basic_string<TCHAR>& lpWildcard) {
			assert(m_hFindFile == INVALID_HANDLE_VALUE);

			// 検索先フォルダを保存
			lstrcpy(this->m_findPath, lpPathName.c_str());

			::PathAppend(::lstrcpy(m_fileName, lpPathName.c_str()), lpWildcard.c_str());
			//SetLastError(ERROR_SUCCESS);
			m_hFindFile = ::FindFirstFile(m_fileName, &m_findFileData);
			return (m_hFindFile != INVALID_HANDLE_VALUE);
		};

		//-------------------------------------------------------------
		// FindNextFile関数のラッパー
		//-------------------------------------------------------------
		bool FindNext() { return (::FindNextFile(m_hFindFile, &m_findFileData) != FALSE); };

		//-------------------------------------------------------------
		// FindClose関数のラッパー
		//-------------------------------------------------------------
		bool Close() {
			bool bResult = true;
			if (m_hFindFile != INVALID_HANDLE_VALUE) {
				bResult = (::FindClose(m_hFindFile) != FALSE);
				m_hFindFile = INVALID_HANDLE_VALUE;
			}
			return bResult;
		};

	private:
		//-------------------------------------------------------------
		// コピーと代入を禁止
		//-------------------------------------------------------------
		FindFile(const FindFile& src) {};
		void operator=(const FindFile& rhs) {};

	public:
		//-------------------------------------------------------------
		// 検索に使うフルパス名を取得
		//-------------------------------------------------------------
		std::basic_string<TCHAR> GetFindFileName() { return m_fileName; };

		//-------------------------------------------------------------
		// ファイルがディレクトリかどうか調べる
		//-------------------------------------------------------------
		bool IsDirectory() { return ((m_findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0); };

		//-------------------------------------------------------------
		// ファイルが"."または".."かどうか調べる
		//-------------------------------------------------------------
		bool IsDots() { return (::lstrcmp(m_findFileData.cFileName, _T(".")) == 0) || (::lstrcmp(m_findFileData.cFileName, _T("..")) == 0); };

		//-------------------------------------------------------------
		// ファイル名を取得
		//-------------------------------------------------------------
		std::basic_string<TCHAR> GetFileName() { return m_findFileData.cFileName; };

		// 絶対パスを取得
		std::basic_string<TCHAR> get_FullName() {
			return kjm::path::combine(m_findPath, m_findFileData.cFileName);
		}

		//-------------------------------------------------------------
		// 最終更新日を取得
		//-------------------------------------------------------------
		kjm::time get_lastWriteTime() { return kjm::time(this->m_findFileData.ftLastWriteTime); };

		//-------------------------------------------------------------
		// ファイルのサイズを取得
		//-------------------------------------------------------------
		__int64 get_fileSize() {
			LARGE_INTEGER li;
			li.LowPart = this->m_findFileData.nFileSizeLow;
			li.HighPart = this->m_findFileData.nFileSizeHigh;
			return li.QuadPart;
		};

	private:
		TCHAR m_findPath[MAX_PATH];
		TCHAR m_fileName[MAX_PATH];
		HANDLE m_hFindFile;
		WIN32_FIND_DATA m_findFileData;
	};

	//-------------------------------------------------------------------------
	// C言語の fopen を安全に使うクラス
	//-------------------------------------------------------------------------
	class cfile {
	public:
		// デフォルトコンストラクタ
		cfile() : m_fp(NULL) {}

		// 開く付きコンストラクタ
		cfile(const _tstring& fname, const _tstring& mode, int shflag = 0) : m_fp(NULL) {
			this->open(fname, mode, shflag);
		}

		// デストラクタ
		virtual ~cfile();

		// fopenの代わり
		//
		// shflag _SH_DENYNO=読み取りおよび書き込みアクセスを許可します。
		//        _SH_DENYRD=ファイルへの読み取りアクセスを拒否します。
		//        _SH_DENYRW=ファイルへの読み取りおよび書き込みアクセスを拒否します。
		//        _SH_DENYWR=ファイルへの書き込みアクセスを拒否します。
		virtual bool open(const _tstring& fname, const _tstring& mode, int shflag = 0);

		// fcloseの代わり
		virtual bool close();

		// ファイルが開いているかどうか調べる
		virtual bool is_open() const {
			return (m_fp != NULL);
		}

		// fseek関数
		// origin {SEEK_CUR, SEEK_END, SEEK_SET}
		virtual int fseek(__int64 offset, int origin) {
			return _fseeki64(m_fp, offset, origin);
		}

		// 一行読込
		virtual int getline(_tstring& lineBuf) {
			return kjm::util::getline(m_fp, lineBuf);
		}

		// 一行書込み
		virtual void writeline(_tstring& lineBuf) {
			_fputts(lineBuf.c_str(), m_fp);
			_fputts(_T("\n"), m_fp);
		}

		// 指定バイト読込
		template<class T>
		int read(std::vector<T>& buffer, int count = 1) {
			buffer.resize(count);
			return fread(&buffer[0], sizeof(T), count, m_fp);
		}

		// ファイルハンドルを返す
		virtual FILE* get_handle() const {
			return m_fp;
		}
	private:
		FILE* m_fp;
	};

	//-------------------------------------------------------------------------
	// ファイルを扱うスタティッククラス
	//-------------------------------------------------------------------------
	class file {
	public:
		// 最終更新日時を取得(ローカル時間)
		static dateTime getLastWriteTime(const _tstring& path) { return getLastWriteTimeUtc(path).toLocalTime(); }

		// 最終更新日時を取得(UTC)
		static dateTime getLastWriteTimeUtc(const _tstring& path);

		// ファイルの属性を取得する。
		static DWORD getAttributes(const _tstring &path);

		// ファイルの属性を設定する。
		static void setAttributes(const _tstring& path, DWORD fa);

		// ファイルを削除する。
		static void deleteFile(const _tstring& path, bool force = false);

		// ファイルのサイズを取得する
		static __int64 getFileSize(HANDLE hFile);

		static void copy(const _tstring& sourceFileName, const _tstring& destFileName, bool overwrite=false) {
			if (::CopyFile(sourceFileName.c_str(), destFileName.c_str(), (overwrite) ? FALSE : TRUE) == FALSE) {
				DWORD code = ::GetLastError();

				kjm::_tostringstream oss;
				oss << _T("kjm::file::copy('") << sourceFileName << _T("','") << destFileName << _T("',") << overwrite << _T("): ")
					<< _T("error ") << code << _T(" : ");

				throw kjm::systemException(code, oss.str());
			}
		}
	private:
		file() {}
		~file() {}
	};

	//=========================================================================
	// ディレクトリやサブディレクトリを通じて、作成、移動、および列挙するための
	// 静的メソッドを公開します。
	//=========================================================================
	class directory {
	public:
		// コールバック関数にクラスを渡すための template 関数
		// kjm::direcoty::copy の引数として次のように記述します
		//
		// kjm::direcoty::copy(
		//		sourceDirName,
		//		destDirName,
		//		kjm::direcoty::is_cancel_callback<hoge, &hoge::callback_func>, this));
		template<typename T, bool (T::*F)()>
		static bool is_cancel_callback(void* parg) {
			T* obj = reinterpret_cast<T*>(parg);
			return (obj->*F)();
		}

		// ファイルまたはディレクトリ、およびその内容を新しい場所にコピーします。
		static BOOL copy(LPCTSTR sourceDirName, LPCTSTR destDirName, bool (*is_cancel_callback)(void*), void* callback_arg);

		static void copy(const _tstring& sourceDirName, const _tstring& destDirName, bool (*is_cancel_callback)(void*), void* callback_arg) {
			int i;
			DWORD dwRet;

			// コピー先のディレクトリを作成する
			dwRet = kjm::directory::createDirectory2(destDirName);
			if (dwRet != 0) {
				kjm::_tostringstream oss;
				oss << _T("kjm::directory::copy('") << sourceDirName << _T("','") << destDirName << _T("')")
					<< _T(": error ") << dwRet << _T(" : ディレクトリ '") << destDirName << _T("' が、作成できません。");

				throw kjm::systemException(dwRet, oss.str());
			}

			// ファイルをコピーする
			kjm::_tstrings files = kjm::directory::getFiles(sourceDirName);
			for (i = 0; i < (int)files.size(); i++) {
				if ((*is_cancel_callback)(callback_arg)) {
					break;
				}

				kjm::file::copy(
					files[i],
					kjm::path::combine(destDirName, kjm::path::getFileName(files[i]))
				);
			}

			// ディレクトリを再帰的にコピーする
			kjm::_tstrings directories = kjm::directory::getDirectories(sourceDirName);
			for (i = 0; i < (int)directories.size(); i++) {
				if ((*is_cancel_callback)(callback_arg)) {
					break;
				}

				kjm::directory::copy(
					directories[i],
					kjm::path::combine(destDirName, kjm::path::getFileName(directories[i])),
					is_cancel_callback, callback_arg
				);
			}
		}

		// ディレクトリ内のサブディレクトリを列挙する。
		static _tstrings getDirectories(const _tstring& path) {
			kjm::_tstrings result;

			WIN32_FIND_DATA data;
			HANDLE hFind = ::FindFirstFile(kjm::path::combine(path, _T("*.*")).c_str(), &data);
			if (hFind != INVALID_HANDLE_VALUE) {
				do {
					if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
						if ((::lstrcmp(data.cFileName, _T(".")) != 0) && (::lstrcmp(data.cFileName, _T("..")) != 0)) {
							result.push_back(kjm::path::combine(path, data.cFileName));
						}
					}
				} while (::FindNextFile(hFind, &data));
				::FindClose(hFind);
			}

			return result;
		}

		// ファイルを列挙します。
		static std::vector<std::basic_string<TCHAR> > getFiles(
			const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildCard = _T("*.*"));

		// ディレクトリを列挙します。
		static std::vector<std::basic_string<TCHAR> > getSubFolders(
			const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildCard = _T("*.*"));

		// ディレクトリを作成する。
		static void createDirectory(const _tstring& path);
		static DWORD createDirectory2(const _tstring& path) {
			DWORD dwRet = 0;

			// もし path がルートディレクトリなら、parent は empty になる。
			kjm::_tstring parent = kjm::path::getDirectoryName(path);

			if (!parent.empty() && kjm::directory::exists(path) == false) {
				if ((dwRet = kjm::directory::createDirectory2(parent)) == 0) {
					if (::CreateDirectory(path.c_str(), NULL) == FALSE) {
						dwRet = ::GetLastError();
					}
				}
			}

			return dwRet;
		}

		static kjm::_tstrings glob(const kjm::_tstring& pattern);

		// 指定したパスがディスク上の既存のディレクトリを参照しているかどうかを確認します。
		static bool exists(const _tstring& path) {
			return (::PathFileExists(path.c_str()) != FALSE) ? true : false;
		}

	private:
		directory() {};
		~directory() {};
	};

	//=========================================================================
	// ファイル検索をサポートするクラス
	//=========================================================================
	class findFile {
	public:
		// デフォルトコンストラクタ
		findFile() : m_hFind(INVALID_HANDLE_VALUE), m_strWildcard(_T("")) {}

		// デストラクタ
		virtual ~findFile() { findClose(); }

		// 検索開始
		virtual bool findFirstFile(const _tstring& strFileName) {
			m_strWildcard = strFileName;

			m_hFind = FindFirstFile(m_strWildcard.c_str(), &m_findData);
			return (m_hFind != INVALID_HANDLE_VALUE);
		}

		// 次を検索
		virtual bool findNextFile() {
			return (FindNextFile(m_hFind, &m_findData) != FALSE);
		}

		// 検索終了
		virtual void findClose() {
			if (m_hFind != INVALID_HANDLE_VALUE) {
				FindClose(m_hFind);
				m_hFind = INVALID_HANDLE_VALUE;
			}
		}

		// 見つかったファイルが、'.' or '..' かどうか調べる
		virtual bool isDots() {
			return ((lstrcmp(m_findData.cFileName, _T(".")) == 0) || (lstrcmp(m_findData.cFileName, _T("..")) == 0));
		}

		// 見つかったファイルが、ディレクトリかどうか調べる
		virtual bool isDirectory() {
			return ((m_findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
		}

		// 見つかったファイルが、読み込み専用かどうか調べる
		virtual bool isReadonly() {
			return ((m_findData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0);
		}

		// 見つかったファイルのファイル名部分を取得
		virtual _tstring getFileName() { return m_findData.cFileName; }

		// 見つかったファイルのフルパス名を取得
		virtual _tstring getFullName() { return path::renameFileSpec(m_strWildcard, m_findData.cFileName); }

		// 見つかったファイルの属性を返す
		virtual DWORD getFileAttributes() { return m_findData.dwFileAttributes; }

		// 見つかったファイルのサイズを取得
		virtual __int64 getFileSize() {
			LARGE_INTEGER li = {m_findData.nFileSizeLow, m_findData.nFileSizeHigh};
			return li.QuadPart;
		}

		dateTime getCreationTime() const { return dateTime(m_findData.ftCreationTime, kjm::utc); }
		dateTime getLastAccessTime() const { return dateTime(m_findData.ftLastAccessTime, kjm::utc); }
		dateTime getLastWriteTime() const { return dateTime(m_findData.ftLastWriteTime, kjm::utc); }

		// 見つかったファイルの全情報を WIN32_FIND_DATA 型にコピーする
		virtual void copyWin32FindData(WIN32_FIND_DATA& rBuf) { rBuf = m_findData; }

	private:
		_tstring m_strWildcard;

		HANDLE m_hFind;

		WIN32_FIND_DATA m_findData;
	};

	//=========================================================================
	// ファイルの属性を定義した値
	//=========================================================================
	enum fileAttributes {
		readOnly = FILE_ATTRIBUTE_READONLY,						// ReadOnly = 1,
		hidden = FILE_ATTRIBUTE_HIDDEN,							// Hidden = 2,
		system = FILE_ATTRIBUTE_SYSTEM,							// System = 4,
		directory = FILE_ATTRIBUTE_DIRECTORY,					// Directory = 16,
		archive = FILE_ATTRIBUTE_ARCHIVE,						// Archive = 32,
#if _MSC_VER > 1200	// VC++6より上
		device = FILE_ATTRIBUTE_DEVICE,							// Device = 64,
#endif
		normal = FILE_ATTRIBUTE_NORMAL,							// Normal = 128,
		temporary = FILE_ATTRIBUTE_TEMPORARY,					// Temporary = 256,
		sparseFile = FILE_ATTRIBUTE_SPARSE_FILE,				// SparseFile = 512,
		reparsePoint = FILE_ATTRIBUTE_REPARSE_POINT,			// ReparsePoint = 1024,
		compressed = FILE_ATTRIBUTE_COMPRESSED,					// Compressed = 2048,
		offline = FILE_ATTRIBUTE_OFFLINE,						// Offline = 4096,
		notContentIndexed = FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,	// NotContentIndexed = 8192,
		encrypted = FILE_ATTRIBUTE_ENCRYPTED,					// Encrypted = 16384,
	};

	//=========================================================================
	// ファイル を表現するクラス
	//=========================================================================
	class fileInfo {
	private:
		// コピーコンストラクタと代入演算子で使うメンバーコピー関数
		void copy_from_fileInfo(const fileInfo& src);

	public:
		// デフォルトコンストラクタ
		fileInfo();

		// コピーコンストラクタ
		fileInfo(const fileInfo& src);

		// WIN32_FIND_DATA で初期化するコンストラクタ
		fileInfo(const _tstring& strParentPath, const WIN32_FIND_DATA& rFindData);

		// ファイル名で初期化するコンストラクタ
		fileInfo(const _tstring& strName);

		// デストラクタ
		virtual ~fileInfo() {}

		// 代入演算子
		fileInfo& operator =(const fileInfo& rhs);

		/// ファイルが存在するかどうかを示す値を取得します。
		bool exists() const { return kjm::path::fileExists(this->m_fullPath); }

		/// 絶対パスを取得
		_tstring fullName() const { return m_fullPath; }

		// ファイル名部分を返す
		//_tstring getFileName() { return path::findFileName(m_fullPath); };
		_tstring name() const { return kjm::path::findFileName(m_fullPath); }

		// 作成日時(UTC)を返す
		dateTime creationTimeUTC() const { return m_creationTime; }

		// アクセス日時(UTC)を返す
		dateTime lastAccessTimeUTC() const { return m_lastAccessTime; }

		// 更新日時(UTC)を返す
		dateTime lastWriteTimeUTC() const { return m_lastWriteTime; }

		// 作成日時(Local)を返す
		dateTime creationTime() const { return m_creationTime.toLocalTime(); }

		// アクセス日時(Local)を返す
		dateTime lastAccessTime() const { return m_lastAccessTime.toLocalTime(); }

		// 更新日時(Local)を返す 
		dateTime lastWriteTime() const { return m_lastWriteTime.toLocalTime(); }

		// ファイルの属性を返す
		fileAttributes attributes() const { return (kjm::fileAttributes)m_dwFileAttributes; }

		// ファイルの属性を返す
		DWORD get_Attributes() const { return m_dwFileAttributes; }

		// ファイルの属性文字列を返す
		kjm::_tstring get_AttribText(const kjm::_tstring& format=_T("ASHR")) const;

		// ファイルのサイズを返す
		__int64 length() const { return m_fileSize; }

		// ファイルが「アーカイブ可能」かどうか調べる
		bool isArchive() const { return ((m_dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0); }

		// ファイルが「隠しファイル」かどうか調べる
		bool isHidden() const { return ((m_dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0); }

		// ファイルが「読み取り専用」かどうか調べる
		bool isReadonly() const { return ((m_dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0); }

		// ファイルが「システム」かどうか調べる
		bool isSystem() const { return ((m_dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0); }

		// ファイルが「symbolic link or junction」かｄどうか調べる

		bool isSymbolicLink() const {
			return ((m_dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);
		}

		// ファイルのバージョン情報を返す
		_tstring getFileVersion() const { return kjm::util::getFileVersion(m_fullPath); }

	private:
		_tstring m_fullPath;		// ファイルのフルパス

		// -- 日時関する情報 --
		dateTime m_creationTime;	// 作成日時
		dateTime m_lastAccessTime;	// アクセス日時
		dateTime m_lastWriteTime;	// 最終更新日時

		__int64 m_fileSize;			// ファイルのサイズ
		DWORD m_dwFileAttributes;	// ファイルの属性

	private:
		// 情報を保持しているかどうかのフラグ。
		// ファイルサイズ、最終更新日付などの情報を持っているときに true
		// WIN32_FIND_DATA で生成されているときは、true になる。
		bool m_hasInfo;
	};

	//=========================================================================
	// ディレクトリ を表現するクラス
	//=========================================================================
	class folderInfo {
	public:
		// デフォルトコンストラクタ
		folderInfo() {}

		// コピーコンストラクタ
		folderInfo(const folderInfo& src) : m_fullPath(src.m_fullPath) {}

		// WIN32_FIND_DATA で初期化するコンストラクタ
		folderInfo(const _tstring& strParentPath, const WIN32_FIND_DATA& rFindData) {
			m_fullPath = path::append(strParentPath, rFindData.cFileName);
		}

		// ディレクトリ名で初期化するコンストラクタ
		folderInfo(const _tstring& strName) : m_fullPath(strName) {}

		// デストラクタ
		virtual ~folderInfo() {}

		// 代入演算子
		folderInfo& operator =(const folderInfo& rhs) {
			if (this != &rhs) m_fullPath = rhs.m_fullPath;
			return *this;
		}

		// 見つかったファイルのファイル名部分を取得
		_tstring getFileName() const { return path::findFileName(m_fullPath); }

		// ディレクトリ直下のファイル一覧を取得
		std::vector<fileInfo> getFiles(const _tstring& strWC = _T("*.*")) const;

		// ディレクトリ直下のディレクトリ一覧を取得
		std::vector<folderInfo> getSubFolders() const;

		// ディレクトリのフルパスを返す
		_tstring getFullPath() const { return m_fullPath; }

		// ディレクトリが存在するかどうかを返す
		bool exist(DWORD& detail) {
			bool ret = kjm::path::fileExists(m_fullPath);
			detail = ::GetLastError();
			return ret;
		}

		// ディレクトリが存在するかどうかを返す
		bool exist() const { return kjm::path::fileExists(m_fullPath); }

		// ディレクトリを作成する
		void create() { kjm::directory::createDirectory(m_fullPath); }

		// ディレクトリの属性を返す
		DWORD get_Attributes() const { return GetFileAttributes(m_fullPath.c_str()); }

	public:
		_tstring m_fullPath;	// ディレクトリのフルパス
	};

	//-------------------------------------------------------------------------
	// 現在の環境およびプラットフォームに関する情報、およびそれらを操作する手
	// 段を提供します。(.NETの部分的実装)
	//-------------------------------------------------------------------------
	class environment {
	public:

		// ローカル コンピュータの NetBIOS 名を取得します。
		static _tstring get_MachineName() { return kjm::util::getComputerName(); }

		// 指定した文字列に埋め込まれている各環境変数の名前を、その変数の値を表す文字列で置換し、置換後の文字列全体を返します。
		static _tstring expandEnvironmentVariables(const _tstring& name);

	private:
		environment() {}
		~environment() {}
	};

	/**
	 *	@brief	NOTIFYICONDATA をラップするクラス
	 */
	class BiNotifyIcon : private NOTIFYICONDATA {
	public:

		// デフォルト・コンストラクタ
		BiNotifyIcon();

		// デストラクタ
		virtual ~BiNotifyIcon();

	public:
		// タスクトレイにアイコンを追加する。
		BOOL Add(HWND hWnd, UINT uID, UINT uFlags, UINT uCallbackMessage, HICON hIcon, LPCTSTR pszTip);

		// タスクトレイからアイコンを削除する
		BOOL Delete();

		// タスクトレイアイコンの情報を変更する。
		BOOL Modify(UINT uFlags, UINT uCallbackMessage, HICON hIcon, LPCTSTR pszTip);
	};

	
	// class mutex の投げる例外用の型
	//
	struct mutex_error {
		enum error_code {
			not_initialized = 1,	// mutex が未初期化
			already_initialized		// mutex が初期化済み
		};

		error_code error;
		mutex_error(error_code e) : error(e) {};
	};

	//-------------------------------------------------------------------------
	// mutex 管理クラス
	//
	// note: 作成したハンドルは、デストラクタで破棄される。
	//       また、lock(WaitForSingleObject)したままのハンドルは、
	//       必要回数unlock(ReleaseMutex)してから、破棄される。
	//-------------------------------------------------------------------------
	class mutex {
	private:
		HANDLE m_hMutex;		// 管理する mutex へのハンドル
		DWORD m_create_status;	// mutex 作成時の状態変数
		DWORD m_last_error;		// API使用時のエラー保持
		int m_lock_count;		// lockした回数

	public:
		// mutex 作成時に同名の mutex が存在したときは、ture を返す
		bool is_exists() { return (m_create_status == ERROR_ALREADY_EXISTS); }

		// mutex作成済みかどうかのテスト
		bool is_open() const { return (m_hMutex != NULL); }

	public:
		// デフォルトコンストラクタ
		mutex() : m_hMutex(NULL), m_create_status(0), m_lock_count(0) {}
		
		// コピーコンストラクタ
		mutex(const mutex& src);
		
		// ミューテックオブジェクト作成付きコンストラクタ
		mutex(const _tstring &name);
		
		// デストラクタ
		virtual ~mutex() { close(); }

		// Mutexの作成
		bool create(const _tstring &name);
		
		// mutex を閉じる
		void close();

		// 指定時間シグナル状態になるのを待つ
		DWORD lock(DWORD dwMillisec);

		// mutexを開放する
		BOOL unlock();
		
		// 代入演算子
		mutex& operator=(const mutex& src);

	private:
		// ハンドル複製共通関数
		void __duplicate(const mutex& src);
	};

	//-----------------------------------------------------------------
	// 簡易数学系関数
	//-----------------------------------------------------------------
	class math {
	public:
		// 百分率の計算
		template <class T>
		static double percentage(T numerator, T denominator) {
			return (denominator == 0.0) ? 100.0 : ((double)numerator / (double)denominator) * 100.0;
		}
	};

	// class memfile の投げる例外用の型
	//
	struct memfile_error {
		enum error_code {
			not_initialized = 1,	// memfile が未初期化
			already_initialized		// memfile が初期化済み
		};
		error_code error;

		memfile_error(error_code e) : error(e) {};
	};

	// メモリマップドファイル管理クラス
	//
	// note: 作成したハンドルは、デストラクタで破棄される。
	//
	template<class T> class memfile {
	private:
		HANDLE m_hFile;
		DWORD m_create_status;

		// 面倒なので、インスタンスの複製は不許可。
		memfile<T>(const memfile<T> &src) {};
		memfile<T>& operator=(const memfile<T> &src) {};

	public:
		// memfile作成時に同名のmemfileが存在したときは、ture を返す
		//
		bool is_exists() { return (m_create_status == ERROR_ALREADY_EXISTS); };

		// memfile作成済みかどうかのテスト
		//
		bool is_open() { return (m_hFile != NULL); };

	public:
		// コンストラクタとデストラクタ
		//
		memfile<T>() : m_hFile(NULL), m_create_status(0) {};
		memfile<T>(const std::string& name, DWORD size = 0) : m_hFile(NULL), m_create_status(0) {
			// どちらも例外は起こりえない。初期化の成否は、is_openで確認。
			if (size == 0)	open(name);
			else			create(name, size);
		};
		virtual ~memfile<T>() { close(); };

		// メモリマップドファイルの作成
		//
		// arg : name = メモリマップドファイルの名前
		//       size = サイズ(要素数)
		// ret : false = 失敗。true = 成功
		// exp : memfile_error
		// note:
		//
		bool create(const std::string& name, DWORD size) {
			// 初期化済みinstanceのときは、例外をthrow
			if (m_hFile) throw memfile_error(memfile_error::already_initialized);

			// メモリマップドファイルの作成
			m_hFile = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(T) * size, name.c_str());
			m_create_status = ::GetLastError();
			return (m_hFile != NULL);
		};

		// メモリマップドファイルを開く
		//
		// arg : name = メモリマップドファイルの名前
		// ret : false = 失敗。true = 成功
		// exp : memfile_error
		// note:
		//
		bool open(const std::string& name) {
			// 初期化済みinstanceの時は、例外をthrow
			if (m_hFile) throw memfile_error(memfile_error::already_initialized);

			// メモリマップドファイルを開く
			m_hFile = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, name.c_str());
			return (m_hFile != NULL);
		};

		// メモリマップドファイルを閉じる
		//
		// arg : なし
		// ret : なし
		// exp : なし
		// note: 未初期化のinstanceで使用してもok
		//
		void close() {
			if (m_hFile) {
				::CloseHandle(m_hFile);
				m_hFile = NULL;
			}
		};

		// メモリマップドファイルからデータを読み込む
		//
		// arg : buffer = 読み込むバッファへのポインタ
		//       offset = 先頭位置へのオフセット
		//       size = 読み込む要素数
		//       pmtx = 排他制御に使用するmutexへのポインタ(NULL可)
		//       timeout = 排他タイムアウト時間
		//
		int read(T* buffer, size_t offset, size_t size, mutex* pmtx, DWORD timeout) {
			// 未初期化instanceの時は、例外をthrow
			if (!m_hFile)	throw memfile_error(memfile_error::not_initialized);

			int ret = 0;
			DWORD dw = WAIT_OBJECT_0;

			// mutexへのポインタが与えられているときは、まずlock
			if (pmtx)	dw = pmtx->lock(timeout);

			if (dw == WAIT_OBJECT_0 || dw == WAIT_ABANDONED) {
				
				T* p = (T*)::MapViewOfFile(m_hFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
				if (p) {
					memmove(buffer, &p[offset], sizeof(T) * size);
					::UnmapViewOfFile(p);
				} else {
					ret = -1;
				}

				// mutexへのポインタが与えられているときは、unlock
				if (pmtx)	pmtx->unlock();
			} else {
				ret = -2;
			}

			return ret;
		}

		// メモリマップドファイルへデータを書き込む
		//
		// arg : buffer = 読み込むバッファへのポインタ
		//       offset = 先頭位置へのオフセット
		//       size = 読み込む要素数
		//       pmtx = 排他制御に使用するmutexへのポインタ(NULL可)
		//       timeout = 排他タイムアウト時間
		//
		template <class T> int write(T* buffer, size_t offset, size_t size, mutex* pmtx, DWORD timeout) {
			// 未初期化instanceの時は、例外をthrow
			if (!m_hFile)	throw memfile_error(memfile_error::not_initialized);

			int ret = 0;
			DWORD dw = WAIT_OBJECT_0;

			// mutexへのポインタが与えられているときは、まずlock
			if (pmtx)	dw = pmtx->lock(timeout);

			if (dw == WAIT_OBJECT_0 || dw == WAIT_ABANDONED) {
				
				T* p = (T*)::MapViewOfFile(m_hFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
				if (p) {
					memmove(&p[offset], buffer, sizeof(T) * size);
					::UnmapViewOfFile(p);
				} else {
					ret = -1;
				}

				// mutexへのポインタが与えられているときは、unlock
				if (pmtx)	pmtx->unlock();
			} else {
				ret = -2;
			}

			return ret;
		}
	};

	// class lockfile の投げる例外用の型
	//
	struct lockfile_error {
		enum error_code {
			not_initialized = 1	// lockfile が未初期化
		};
		error_code error;

		lockfile_error(error_code e) : error(e) {};
	};

	//-----------------------------------------------------------------
	// ライブラリモジュールを管理するクラス
	//-----------------------------------------------------------------
	class library {
	private:
		HINSTANCE m_hLibModule;

	public:
		// アクセサ
		HINSTANCE getInstance() { return m_hLibModule; };

	public:
		library() : m_hLibModule( NULL ) {};
		virtual ~library() { FreeLibrary(); };

	public:
		BOOL LoadLibraryEx( LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags );
		BOOL FreeLibrary();
	};

	//-----------------------------------------------------------------
	// ロックファイルを安全に扱うためのクラス
	//-----------------------------------------------------------------
	class Lockfile {
	public:
		//-------------------------------------------------------------
		// コンストラクタ
		//-------------------------------------------------------------
		Lockfile() : m_hFile(INVALID_HANDLE_VALUE) {};

		//-------------------------------------------------------------
		// デストラクタ
		//-------------------------------------------------------------
		virtual ~Lockfile() { Unlock(); };

		//-------------------------------------------------------------
		// ロックする
		//-------------------------------------------------------------
		BOOL Lock(const std::basic_string<TCHAR>& strLockfile);

		//-------------------------------------------------------------
		// アンロックする
		//-------------------------------------------------------------
		BOOL Unlock();

		// ロック中かどうか調べる
		bool IsLocked() { return (m_hFile != INVALID_HANDLE_VALUE); };

	private:
		//-------------------------------------------------------------
		// コピーと代入を認めない
		//-------------------------------------------------------------
		Lockfile(const Lockfile& src) {};
		void operator=(const Lockfile& lhs) {};

		//-------------------------------------------------------------
		// メンバー変数
		//-------------------------------------------------------------
		HANDLE m_hFile;
	};

	// lockfileを簡単に扱うクラス
	//
	class lockfile {
	private:
		HANDLE m_hFile;			// ロックファイルへのハンドル
		std::basic_string<TCHAR> m_fullname;	// ロックファイルへのフルパス名

	public:
		// コンストラクタ・デストラクタ
		//
		lockfile(const std::basic_string<TCHAR>& fullname) : m_hFile(INVALID_HANDLE_VALUE), m_fullname(fullname) {};
		virtual ~lockfile() { unlock(); };

		// ロックする
		//
		bool lock() {
			// すでに自分でロックしているときは、trueを返す
			if (m_hFile != INVALID_HANDLE_VALUE)	return true;

			m_hFile = ::CreateFile(m_fullname.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE, NULL);
			if (m_hFile == INVALID_HANDLE_VALUE) {

				// lockに失敗しても、ゴミの可能性があるので削除を試みる。
				::DeleteFile(m_fullname.c_str());
				m_hFile = ::CreateFile(m_fullname.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE, NULL);
			}
			
			return (m_hFile != INVALID_HANDLE_VALUE);
		};

		// ロックを解除する
		//
		void unlock() {
			if (m_hFile != INVALID_HANDLE_VALUE) {
				::CloseHandle(m_hFile);
				m_hFile = INVALID_HANDLE_VALUE;
			}
		};
	};

#if 0
	//-----------------------------------------------------------------
	// 簡易SOCKETクラス
	//-----------------------------------------------------------------
	class in_socket {
	private:
		SOCKET m_sock;

	public:
		in_socket() : m_sock(INVALID_SOCKET) {};	// コンストラクタ
		virtual ~in_socket() { close_socket(); };	// デストラクタ

		// ソケットを開く
		bool open_socket(int type) {
			assert(m_sock == INVALID_SOCKET);
			return (m_sock = socket(AF_INET, type, 0)) != INVALID_SOCKET;
		};

		// ソケットを閉じる
		int close_socket() {
			int ret = 0;
			if (m_sock != INVALID_SOCKET) {
				ret = ::closesocket(m_sock);
				m_sock = INVALID_SOCKET;
			}
			return ret;
		};

		// 伝聞送信
		int send_to(const char* to_addr, short port, const char* buf, int len) {
			assert(m_sock != INVALID_SOCKET);

			sockaddr_in to;
			memset(&to, 0, sizeof(to));
			to.sin_family = AF_INET;
			to.sin_addr.S_un.S_addr = kjm::util::get_ipaddr(to_addr);
			to.sin_port = htons(port);

			return sendto(m_sock, buf, len, 0, (sockaddr*)&to, sizeof(to));
		};
	};
#endif

	//-------------------------------------------------------------------------
	// イベントログにアクセスするためのクラス
	//
	// 現在のところ、バックアップのためのメソッドしかないため、インスタンス化
	// できるようになっていません。
	//-------------------------------------------------------------------------
	class event_log {
	private:
		// デフォルトコンストラクタ
		event_log() {}

		// デストラクタ
		~event_log() {}

	public:
		// イベントログをバックアップする
		//
		// バックアップ先のフォルダがない場合、エラーになります。
		// バックアップ先に同名ファイルがある場合、エラーになります。
		static void backup(const kjm::_tstring& sourceName, const kjm::_tstring& backupFileName);

	};


	//-------------------------------------------------------------------------
	// 基本ソケットクラス
	//-------------------------------------------------------------------------
	class baseSocket {
	public:
		// sockaddr_in を初期化するための便利関数
		// name のアドレスを返します。
		static void initSockaddrIn(sockaddr_in& name, const std::basic_string<TCHAR>& host, short port);

	public:
		// コンストラクタ
		baseSocket() : m_sock(INVALID_SOCKET) {}

		// デストラクタ
		virtual ~baseSocket() { closeSocket(); }

		// ソケットを開く
		bool openSocket(int af, int type, int protocol);

		// 接続
		bool connectSocket(const sockaddr_in& name);

		// 受信タイムアウトの設定
		bool setRecvTimeout(int msTimeout);

		// バインドする(ポート番号を指定する簡単バージョンだけ準備)
		bool bindSocket(unsigned short port);

		// 受信する
		int recvfrom(std::vector<char>& buf, sockaddr_in& from);

		// 受信する
		int recv(std::vector<char>& buf);

		// 受信する
		void recvAll(std::vector<char>& buf);

		// 全て受信したか判断する関数
		virtual bool isRecvAll(const std::vector<char>& buf);

		// 送信する
		int sendto(const std::vector<char>& buf, const sockaddr_in& to);

		// 送信する
		int send(const char* pStr);

		// 切断する
		int shutdown(int how);

		// ソケットを閉じる
		bool closeSocket();

		// 優雅に切断する
		void gracefulShutdown();

	public:
		// Winsock初期化
		static int startup();

		// Winsock後始末
		static int cleanup() { return ::WSACleanup(); }

	private:
		SOCKET m_sock;
		int m_lastError;
	};

	//-------------------------------------------------------------------------
	// UDPソケットクラス
	//-------------------------------------------------------------------------
	class udpSocket : public kjm::baseSocket {
	public:
		// コンストラクタ
		udpSocket() {}

		// デストラクタ
		virtual ~udpSocket() {}

		// ソケットを開く
		bool openSocket() { return baseSocket::openSocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); }
	};

	//-------------------------------------------------------------------------
	// TCPソケットクラス
	//-------------------------------------------------------------------------
	class tcpSocket : public kjm::baseSocket {
	public:
		// コンストラクタ
		tcpSocket() {}

		// デストラクタ
		virtual ~tcpSocket() {}

		// ソケットを開く
		bool openSocket() { return baseSocket::openSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP); }
	};



	// Convertクラス
	class convert {
	private:
		convert() {}
		~convert() {}

	public:
		static std::basic_string<TCHAR> toString(int value) {
			TCHAR str[128];
			return std::basic_string<TCHAR>(_itot(value, str, 10));
		}
	};

	//-----------------------------------------------------------------
	// イベントログの1レコード分を管理するクラス
	//-----------------------------------------------------------------
	class eventLogRecord {
	public:
		// レコード番号
		DWORD RecordNumber() { return getRaw()->RecordNumber; }

		kjm::time TimeGenerated() { return kjm::time( getRaw()->TimeGenerated ); }

		WORD EventCategory() {
			EVENTLOGRECORD* p = getRaw();
			return getRaw()->EventCategory;
		}

		std::basic_string<TCHAR> EventCategoryText() {
			return DispMessage(get_SourceName().c_str(), _T("CategoryMessageFile"), std::vector<std::basic_string<TCHAR> >(), EventCategory()); };

		std::basic_string<TCHAR> EventIDText();

		DWORD DataLength() { return getRaw()->DataLength; }
		DWORD DataOffset() { return getRaw()->DataOffset; }

		std::basic_string<TCHAR> ComputerName();

		std::vector<BYTE> Data();

		// イベントの日付時刻を取得
		kjm::time get_TimeWritten() { return kjm::time( getRaw()->TimeWritten ); };

		// イベントタイプ(種類)を取得
		// @retval EVENTLOG_ERROR_TYPE, EVENTLOG_WARNING_TYPE, EVENTLOG_INFORMATION_TYPE, EVENTLOG_AUDIT_SUCCESS, EVENTLOG_AUDIT_FAILURE のどれか
		WORD get_EventType() { return getRaw()->EventType; };

		// イベントソースを取得
		std::basic_string<TCHAR> get_SourceName() { return (LPCTSTR)(&m_raw_record[0] + sizeof(EVENTLOGRECORD)); }

		// イベントIDを取得
		DWORD get_EventID() { return getRaw()->EventID; }

	public:
		// デフォルトコンストラクタ
		eventLogRecord() {};

		// 初期化付きコンストラクタ
		eventLogRecord(LPCTSTR lpSourceName, const EVENTLOGRECORD* src);

		eventLogRecord( const eventLogRecord& src )
			: m_source_name( src.m_source_name ), m_raw_record( src.m_raw_record ) {};
		virtual ~eventLogRecord() {};

		kjm::eventLogRecord& operator= ( const kjm::eventLogRecord& src );

	private:
		std::basic_string<TCHAR> m_source_name;		// イベントソース
		std::vector<BYTE> m_raw_record;	// 生情報
		
		// 生情報をキャストする
		EVENTLOGRECORD* getRaw() { return (EVENTLOGRECORD*)&m_raw_record[0]; }

		std::basic_string<TCHAR> DispMessage(
			const std::basic_string<TCHAR>& SourceName, LPCTSTR EntryName, const std::vector<std::basic_string<TCHAR> >& strings, DWORD MessageId);

		int GetArgs(const EVENTLOGRECORD *pBuf, std::vector<std::basic_string<TCHAR> >& args );

		BOOL GetModuleNameFromSourceName(LPCTSTR SourceName, LPCTSTR EntryName, std::vector<TCHAR>& ExpandedName);
	};

	//-----------------------------------------------------------------
	// イベントログを管理するクラス
	//-----------------------------------------------------------------
	class eventLog {
	public:
		// デフォルトコンストラクタ
		eventLog() : m_hEventLog(NULL) {}

		// デストラクタ
		virtual ~eventLog() { closeEventLog(); }

	public:
		// イベントログを開く(OpenEventLog API参照)
		//
		// lpSourceNameには通常、"Application", "Security", "System"のどれかを指定する。
		BOOL openEventLog(LPCTSTR lpUNCServerName, LPCTSTR lpSourceName);

		// イベントログを開く(OpenEventLog API参照)
		BOOL openEventLog(LPCTSTR lpSourceName) { return openEventLog(NULL, lpSourceName); }

		// アプリケーションイベントログを開く
		BOOL openApplicationEventLog(LPCTSTR lpUNCServerName = NULL) { return openEventLog(lpUNCServerName, _T("Application")); }

		// システムイベントログを開く
		BOOL openSystemEventLog(LPCTSTR lpUNCServerName = NULL) { return openEventLog(lpUNCServerName, _T("System")); }

		BOOL OpenBackupEventLog( LPCTSTR lpUNCServerName, LPCTSTR lpSourceName, LPCTSTR lpFileName );

		// イベントログを閉じる
		BOOL closeEventLog();

		// イベントを1レコード読み込む
		BOOL readEventLog(DWORD dwReadFlags, DWORD dwRecordOffset, eventLogRecord& record);
		
		// イベントログをバックアップする
		BOOL BackupEventLog( LPCTSTR lpBackupFileName ) {
			return ::BackupEventLog( m_hEventLog, lpBackupFileName );
		};

		// イベントログをバックアップする
		// 異常時は、kjm::kjmException を発生します。
		static void backup(LPCTSTR sourceName, LPCTSTR backupFileName);

	private:
		// コピーコンストラクタ(コピー禁止)
		eventLog(const eventLog& src) {}

		// 代入演算子(代入禁止)
		eventLog& operator=(const eventLog& rhs) { return *this; }

	private:
		HANDLE m_hEventLog;
		std::basic_string<TCHAR> m_uncServerName;
		std::basic_string<TCHAR> m_sourceName;
		std::basic_string<TCHAR> m_fileName;
	};

	//-------------------------------------------------------------------------
	// iniファイルを処理するクラス
	//-------------------------------------------------------------------------
	class iniFile {
	public:
		// コンストラクタ
		iniFile() : m_fileName() {}

		// コピーコンストラクタ
		iniFile(const iniFile& src) : m_fileName(src.m_fileName) {}

		// ファイル名を与えるコンストラクタ
		iniFile(const _tstring& szFileName) : m_fileName(szFileName) {}

		// デストラクタ
		virtual ~iniFile() {}

		// 代入演算子
		kjm::iniFile& operator=(const iniFile& rhs) {
			if (this == &rhs) return *this;
			this->m_fileName = rhs.m_fileName;
			return *this;
		}

		// セクションデータを取得
		std::vector<TCHAR> getSectionData(const _tstring& section) const;

		// セクションデータを書き込み
		BOOL writeSectionData(const _tstring& section, const std::vector<TCHAR>& sectionData);

		// セクション一覧を取得
		_tstrings getSectionNames() const;

		// セクション内のキー一覧を取得
		_tstrings getKeyNames(const _tstring& strSectionName) const;

		// iniファイルから値を取得
		_tstring getString(const _tstring& strSection,
			const _tstring& strKey, const _tstring& strDefault = _T(""), BOOL* pfUseDefault = NULL) const;

		// iniファイルから数値を取得
		UINT getInt(const kjm::_tstring& strSection, const kjm::_tstring& strKey, UINT defValue = 0, BOOL* pfUseDefault = NULL) const;

		// iniファイルに値を書き込む
		BOOL writeString(const _tstring& strSection, const _tstring& strKey, const _tstring& strValue);

		// 別のiniファイルの内容をマージする
		void mergeIniFile(const iniFile& newIniFile);

		// iniファイル名を取得
		_tstring get_fileName() const { return m_fileName; };

		// iniファイル名を設定
		void set_fileName(const _tstring& value) { m_fileName = value; };

	private:
		_tstring m_fileName;	// iniファイルの名前

	public:
		// モジュール名に対応したiniファイル名を持つクラスを作成する
		static iniFile getModuleIniFile(HMODULE hModule = NULL) {
			return iniFile(kjm::util::getModuleIniFile(hModule));
		}
	};

	//-----------------------------------------------------------------
	// INIファイルを扱うクラス
	//-----------------------------------------------------------------
	class IniFile {
	public:
		//-------------------------------------------------------------
		// デフォルト コンストラクタ
		//-------------------------------------------------------------
		IniFile() {
			TCHAR buf[MAX_PATH];
			::GetModuleFileName(NULL, buf, sizeof(buf));
			::PathRenameExtension(buf, _T(".ini"));
			m_iniFile = buf;
		}

		//-------------------------------------------------------------
		// デストラクタ
		//-------------------------------------------------------------
		virtual ~IniFile() {}

		//-------------------------------------------------------------
		// 文字列を取得する
		//-------------------------------------------------------------
		std::basic_string<TCHAR> GetString(
			const std::basic_string<TCHAR>& strSection,
			const std::basic_string<TCHAR>& strKey,
			const std::basic_string<TCHAR>& strDefault) const {

			const int GROWSIZE = 768;	// だいたい、一回の resize で文字列が取得できるかなぁと思うサイズ

			std::vector<TCHAR> buf;
			int ret;
			do {
				buf.resize(buf.size() + GROWSIZE);
				ret = ::GetPrivateProfileString(strSection.c_str(), strKey.c_str(), strDefault.c_str(), &buf[0], buf.size(), m_iniFile.c_str());
			} while (ret == buf.size() - 1);
			return &buf[0];
		}

		//-------------------------------------------------------------
		// 値を取得する
		//-------------------------------------------------------------
		UINT GetUInt(const std::basic_string<TCHAR>& strSection, const std::basic_string<TCHAR>& strKey, UINT uDefault) const {
			return ::GetPrivateProfileInt(strSection.c_str(), strKey.c_str(), uDefault, m_iniFile.c_str());
		};

		//-------------------------------------------------------------
		// 文字列を書き込む
		//-------------------------------------------------------------
		void WriteString(const std::basic_string<TCHAR>& strSection, const std::basic_string<TCHAR>& strKey, const std::basic_string<TCHAR>& strValue) {
			::WritePrivateProfileString(strSection.c_str(), strKey.c_str(), strValue.c_str(), m_iniFile.c_str());
		};

		//-------------------------------------------------------------
		// 値(UINT)を書き込む
		//-------------------------------------------------------------
		void WriteUInt(const std::basic_string<TCHAR>& strSection, const std::basic_string<TCHAR>& strKey, UINT uValue) {
			TCHAR work[16];	// 4294967295(0xffffffff)を書き込むのに十分なバッファ
			wsprintf(work, _T("%u"), uValue);
			::WritePrivateProfileString(strSection.c_str(), strKey.c_str(), work, m_iniFile.c_str());
		};

		//-------------------------------------------------------------
		// INI ファイルの名前を設定する
		//-------------------------------------------------------------
		void set_IniFile(const std::basic_string<TCHAR>& strIniFile) { m_iniFile = strIniFile; };

		//-------------------------------------------------------------
		// INI ファイルの名前を取得する
		//-------------------------------------------------------------
		std::basic_string<TCHAR> get_IniFile() { return m_iniFile; };

	private:
		std::basic_string<TCHAR> m_iniFile;
	};

	//---------------------------------------------------------------------
	// ドライブの情報を保持するクラス
	//---------------------------------------------------------------------
	class drive {
	public:
		// デフォルトコンストラクタ
		drive() : m_driveNo(-1) {};

		// コピーコンストラクタ
		drive(const drive& src) : m_driveNo(src.m_driveNo) {};

		// ドライブ番号を指定するコンストラクタ
		drive(int driveNo) : m_driveNo(driveNo) {};

		// ドライブレターを指定するコンストラクタ
		drive(const _tstring& driveLetter);

		// デストラクタ
		virtual ~drive() {};

		// 代入演算子
		drive& operator=(const drive& rhs);

		// ドライブ番号を取得
		int get_DriveNo() { return m_driveNo; };

		// ドライブレターを取得
		_tstring get_DriveLetter() { return _tstring(1, (m_driveNo + 'A')) + _T(":"); };

		// ドライブタイプを取得
		UINT get_DriveType() { return GetDriveType(get_DriveLetter().c_str()); };

		// ドライブタイプを文字列で取得
		_tstring get_DriveTypeString();

		// ドライブにメディアが挿入されているかどうか調べる
		// どのドライブタイプにも使えるが、
		// get_DriveType() == DRIVE_REMOVABLE のとき、使ってください。
		bool isDiskInDrive();

	private:
		int m_driveNo;	// ドライブ番号(-1=未選択 0=A 1=B...)
	};

	//---------------------------------------------------------------------
	// ドライブの一覧を保持するクラス
	//---------------------------------------------------------------------
	class driveList {
	public:
		// デフォルトコンストラクタ
		driveList();

		// コピーコンストラクタ
		driveList(const driveList& src) : m_driveList(src.m_driveList) {};

		// デストラクタ
		virtual ~driveList() {};

		// 代入演算子
		driveList& operator=(const driveList& rhs);

		// ドライブ一覧の数を取得
		int get_Size() { return m_driveList.size(); };

		// ドライブオブジェクトを取得
		drive get_Drive(int index) { return m_driveList[index]; };

	private:
		std::vector<drive> m_driveList;	// PCにつながっているドライブ一覧
	};

	//-------------------------------------------------------------------------
	// 共通のログ処理
	//-------------------------------------------------------------------------
	class log {
	public:
		// デフォルトコンストラクタ
		log();

		// デストラクタ
		~log() {};

		// ログのシステム名を取得
		_tstring get_MyName();

		// ログシステムの情報をログに残す
		void writeMyName();

		// エラーログ
		void writeError(LPCTSTR pszFormat, ...);

		// 情報ログ
		void writeInfo(LPCTSTR pszFormat, ...);

		// 全ログ
		void writeAll(LPCTSTR pszFormat, ...);

		// ログの出力関数
		virtual void write(LPCTSTR format, ...);

		// ログの出力関数(画面出力あり)
		virtual void writee(LPCTSTR format, ...);

		// 古いログの削除
		void deleteOldLog();

		// 識別子の取得
		_tstring get_Ident() { return m_ident; };

		// 識別子の設定
		void set_Ident(const _tstring& value) { m_ident = value; };

		// ログファイルのプレフィクスを設定する
		// 文字列に {YMD} を含めると、日付ごとのファイルを作成するようになる。
		virtual void set_prefix(const _tstring& strPrefix);

		// ログファイルの出力先を設定する
		// 数値を指定すると、よく使う場所を指定できます。
#define LOGPATH_TEMP	0
#define LOGPATH_EXEPATH	1
		virtual void set_log_path(const _tstring& strPath);
		virtual void set_log_path(int nMode, const kjm::_tstring& more = kjm::_tstring());

		// ログの保存先を %TEMP%\<subdir> にする
		virtual void setLogPath_Temp(const _tstring& subdir = kjm::_tstring());

		// ログの保存先を スクリプトと同じ場所\<subdir> にする
		virtual void setLogPath_App(const _tstring& subdir = kjm::_tstring());

		// ログファイルの出力先を取得する
		virtual _tstring get_log_path();

		// ログのファイル名ルールを設定する
		virtual void set_LogNameType(int nNameType);

		// ログのファイル名ルールを取得する
		virtual long get_LogNameType();

		// ストップフラグへのアクセサ
		bool get_Stop() const { return this->m_stop; }
		void set_Stop(bool value) { this->m_stop = value; }

		// ログの最大サイズを設定
		int get_RotateSize() const { return this->m_rotateSize; }
		void set_RotateSize(int value) { this->m_rotateSize = value; }
		int get_maxLogSize() const { return this->get_RotateSize(); }	// obsolate
		void set_maxLogSize(int value) { this->set_RotateSize(value); }	// obsolate

		// ログのローテーション数へのアクセサ
		int get_RotateCount() const { return this->m_rotateCount; }
		void set_RotateCount(int value) { this->m_rotateCount = value; }

	private:
		// ログをシフトする
		void shift(const _tstring& fname);

	private:
		int m_rotateCount;			// ログをシフトするときの最大ファイル数
		int m_rotateSize;			// ログをシフトする単位(0でシフトしない)
		bool m_stop;				// ログを止めるとき true にする、デフォルトは true
		_tstring m_ident;
		_tstring m_strBaseFileName;	// 基本になるログファイル名
		_tstring m_strPrefix;		// ログファイルの先頭に付ける名前
		_tstring m_strIdent;		// ログの識別子
		_tstring m_strLogPath;		// ログの出力先(コンストラクタで%TEMP%に設定される)
	};

	// デフォルトで用意済みのログオブジェクトを返す関数
	// コレを使えば、特に変数宣言などなしで、運用可能。
	kjm::log& GetLog();

	//=================================================================
	// 引数が必要なオプションに引数が与えられなかった場合の例外
	//=================================================================

	class missingArgument : public std::exception {
	public:
		missingArgument(const std::string& msg) : std::exception(("missing argument: " + msg).c_str()) {}
		~missingArgument() {}
	};

	//=================================================================
	// 定義されていないオプションが与えられた場合の例外
	//=================================================================

	class invalidOption : public std::exception {
	public:
		invalidOption(const std::string& msg) : std::exception(("invalid option: " + msg).c_str()) {}
		~invalidOption() {}
	};

	//-------------------------------------------------------------------------
	// GetTickCountで経過時間を計るクラス
	//-------------------------------------------------------------------------
	class tickCounter {
	public:
		// デフォルトコンストラクタ
		tickCounter() : m_t1(GetTickCount()), m_t2(0), m_t3(GetTickCount()), m_totalCount(0), m_count(0) {}

		// 総数指定付きコンストラクタ
		tickCounter(__int64 totalCount) : m_t1(GetTickCount()), m_t2(0), m_t3(GetTickCount()), m_totalCount(totalCount), m_count(0) {}

		// デストラクタ
		~tickCounter() {}

		// 計測開始
		void start() { m_t3 = m_t1 = GetTickCount(); m_t2 = 0; }

		// 計測終了
		void stop() { m_t2 = GetTickCount(); }

		// ラップタイム記録
		// タップタイムが指定msec以上の時、ラップタイムを記録する
		// ラップした時は、true を返す
		bool lap(DWORD laptime_msec = 0) {
			bool ret = false;
			if (lapElapse() >= laptime_msec) {
				m_t3 = GetTickCount();
				ret = true;
			}
			return ret;
		}

		// 経過時間(ms)
		DWORD elapse() { return (m_t2 == 0) ? (GetTickCount() - m_t1) : (m_t2 - m_t1); }

		// 経過時間(sec)
		DWORD elapseSec() { return elapse() / 1000; }

		// 経過時間(min)
		DWORD elapseMin() { return elapseSec() / 60; }

		// lapからの経過時間(ms)
		DWORD lapElapse() { return (m_t2 == 0) ? (GetTickCount() - m_t3) : (m_t2 - m_t3); }

		// lapからの経過時間(sec)
		DWORD lapElapseSec() { return lapElapse() / 1000; }

		// lapからの経過時間(min)
		DWORD lapElapseMin() { return lapElapseSec() / 60; }

		// 残り時間を計るための総数
		__int64 m_totalCount;

		// 残り時間を計るための進捗数
		__int64 m_count;

		// 進捗率
		int progress() { return (int)((m_count * 100) / m_totalCount); }

		// 推定残り時間(msec)
		DWORD estimateRemainingTime() {
			return (DWORD)(((m_totalCount - m_count) * elapse()) / m_count);
		}

	private:
		DWORD m_t1;	// 開始した時の時間(コンストラクタ、startで更新)
		DWORD m_t2;	// 終了した時の時間(stopで更新)
		DWORD m_t3;	// ラップタイムを計る時間(lapで更新)
	};

	//---------------------------------------------------------------------
	// ODBCを扱うユーティリティクラス
	// 利用するときは、USE_ODBC32を定義する
	//---------------------------------------------------------------------
#pragma region USE_ODBC32
#if defined(USE_ODBC32)
	class odbcUtil {
	private:
		odbcUtil() {}
		~odbcUtil() {}

	public:
		//-------------------------------------------------------------
		// インストール済みのODBCドライバ一覧を取得
		//-------------------------------------------------------------
		static void getDriverList(std::vector<kjm::_tstring>& drivers);

		//-------------------------------------------------------------
		// インストール済みのODBCドライバ一覧を取得(ODBC2.0版)
		//-------------------------------------------------------------
		static void getDriverList_ODBCv2(std::vector<kjm::_tstring>& drivers);

		//-------------------------------------------------------------
		// インストール済みのドライバを名前で検索
		//
		// 見つからない時は、空文字列を返す。
		//-------------------------------------------------------------
		static kjm::_tstring findDriver(const kjm::_tstring& pattern);

		//-------------------------------------------------------------
		// ドライバがインストール済みかどうか調べる
		//-------------------------------------------------------------
		static bool isInstalled(const kjm::_tstring& driverName);
	};
#endif
#pragma endregion USE_ODBC32

	//-------------------------------------------------------------------------
	// 超簡単にXMLからデータを読込むclass
	//
	// このクラスを利用するときは、
	// プロジェクト全体に、USE_MSXML6 を定義してください。
	//
	// CoInitialize(NULL); と CoUninitialize(); は、アプリケーションで行ってください。
	//-------------------------------------------------------------------------
#pragma region USE_MSXML6
#if defined(USE_MSXML6)
	class xmlez {
	private:
		xmlez() {};
		~xmlez() {};

	public:
		// 超簡単にXMLからデータを読込む
		static kjm::_tstring getText(const kjm::_tstring& xmlFile, const kjm::_tstring &tagName);
	};
#endif
#pragma endregion USE_MSXML6

#if defined(_MT)
	/*!
	 *	スレッド管理クラス
	 */

	/*
		-- 使い方 --

		1. thread を派生する

		class my_thread : public thread { ... }

		2. 純粋仮想関数 worker() と end() を定義する
		   スレッドを終了するための具体的な処理は、派生クラスが行う。
		   使用する側は、 my_thread::end() を呼べば、スレッドが終了する。

		class my_thread : public thread {
		private:
			bool m_loop;	// 終了するためのフラグ(worker内で見る)
		public:
			my_thread() : m_loop(true) {};	// コンストラクタ
			virtual ~my_thread() {};		// デストラクタ
			unsigned worker() {
				while (m_loop) {
					Sleep(200);
					// TODO: 処理を書く
				}
				return 0;
			};
			void end() { m_loop = false; };
		};

		3. スレッドの開始と終了

		// 開始
		my_thread myThrd;	// インスタンス化
		myThrd.run();		// スレッドの実行

		// 終了
		myThrd.end();		// スレッドの終了
		myThrd.wait(5000);	// スレッド終了まで待機(5秒)

		wait() メソッドでスレッドハンドルを閉じるので、永続的なインスタンスを使用して、
		基本的にこの順番で呼び出す。
	*/

	class thread {
	private:
		HANDLE m_hThread;
		CRITICAL_SECTION m_cs;
		bool m_loop;

	public:
		thread() : m_hThread( NULL ) {
			InitializeCriticalSection( &m_cs );
		};
		virtual ~thread() {
			DeleteCriticalSection( &m_cs );
		};

		void lock() { EnterCriticalSection( &m_cs ); };
		void unlock() { LeaveCriticalSection( &m_cs ); };

		//! スレッドが動作中かどうか調べる
		bool still_active(LPDWORD pcode = NULL) { 
			DWORD code = 0;
			if (!GetExitCodeThread( m_hThread, &code )) {
				code = 0;	// エラー発生時は、動いていない事にする
			}
			if (pcode) *pcode = code;
			bool ret = ( code == STILL_ACTIVE );
			return ret;
		};

		//! スレッドを起動する。
		void run() {
			m_loop = true;

			// スレッド動作中に再度実行しようとするのは、ハンドルリークを生み出すので問題
			// スレッドが止まっていることをチェックする
			assert(still_active() == false);	// Debug ビルドなら assert で強制終了
			if (still_active()) return;			// Release なら動かしたふりをして終了
			wait();	// ハンドルを閉じてから起動処理に入る
			m_hThread = ( HANDLE )_beginthreadex( NULL, 0, thread_func, this, 0, NULL );
		}
		void begin() { run(); }

		/*!
		 * スレッドを終了させる。派生クラスで定義する。
		 * 終了方法は、worker method の実装に依存する。
		 */
		virtual void end() {
			m_loop = false;
		}

		virtual bool is_end() const {
			return !m_loop;
		}
		virtual bool still_loop() const {
			return m_loop;
		}

		//! スレッドの一時停止
		DWORD suspend() {
			return SuspendThread( m_hThread );
		};

		//! スレッドの再開
		DWORD resume() {
			return ResumeThread( m_hThread );
		};

		/*!
		 * スレッドに呼び出される関数。派生クラスで定義する。
		 * end methodの呼び出しで終了できるように実装する。
		 */
		virtual unsigned worker() = 0;

		//---------------------------------------------------------------------
		// スレッドの終了を待つ。
		//---------------------------------------------------------------------
		DWORD wait( DWORD timeout = INFINITE ) {
			DWORD ret = 0;
			if (m_hThread != NULL) {
				ret = WaitForSingleObject(m_hThread, timeout);
				CloseHandle(m_hThread);
				m_hThread = NULL;
			}
			return ret;
		}
		// スレッドの終了を待つだけ
		// ハンドルは、別途 close する必要がある。
		virtual DWORD wait_only(DWORD dwTimeout = INFINITE) {
			return WaitForSingleObject(m_hThread, dwTimeout);
		}
		// スレッドハンドルを閉じる
		// スレッドが生きていても閉じるので管理できなくなります。
		void close() {
			if (m_hThread != NULL) {
				CloseHandle(m_hThread);
				m_hThread = NULL;
			}
		}

		/*!
		 * 共通のスレッド関数。
		 * argsには、threadクラスから派生したクラスオブジェクトへの
		 * ポインタを受け取る。p->worker() を実行するだけ。
		 */
		static unsigned __stdcall thread_func( void* args ) {
			unsigned ret = ( ( thread* )args )->worker();
			return ret;
		};
	};
#endif

	//---------------------------------------------------------------------
	// コマンドラインプログラムの基本形
	//---------------------------------------------------------------------
	class program {
	protected:
		int m_argc;
		TCHAR** m_argv;
		std::vector<kjm::optionInfo> m_options;
		kjm::cmdLine m_cmdLine;

	public:
		program() {}
		virtual ~program() {}

	public:
		int run(int argc, TCHAR** argv) {
			C_SETLOCALE_JAPAN;
			this->m_argc = argc;
			this->m_argv = argv;
			int exitCode = 0;
			if (this->parseOption(exitCode) == false) {
				return exitCode;
			}
			return this->doMain();
		}

	public:
		virtual bool parseOption(int& exitCode) { return true; }

		virtual int doMain() { return 0; }

		// バージョン情報の表示
		virtual void printVersion() {
			kjm::_tstring fname = kjm::util::getModuleFileName();
			_tcout << fname << _T(" ") << kjm::util::getFileVersion(fname) << _T("\n");
		}

		// 使用法の表示
		virtual void printUsage() {
			printVersion();
			_tcout << _T("usage: \n");

			kjm::optionInfoList::const_iterator ci;
			for (ci = m_options.begin(); ci != m_options.end(); ci++) {
				_tcout << _T("  ");
				if ((*ci).m_shortOption.empty()) {
					_tcout << _T("    ");
				} else {
					_tcout << _T("-") << (*ci).m_shortOption << _T(", ");
				}
				_tcout << _T("--") << (*ci).m_longOption;
				if ((*ci).m_optionArgInfo == kjm::no_argument) {
					_tcout << _T("\n");
				} else if ((*ci).m_optionArgInfo == kjm::optional_argument) {
					if (!(*ci).m_argDescription.empty()) {
						_tcout << _T("[=") << (*ci).m_argDescription << _T("]\n");
					} else {
						_tcout << _T("[=ARGUMENT]\n");
					}
				} else {
					if (!(*ci).m_argDescription.empty()) {
						_tcout << _T("=") << (*ci).m_argDescription << _T("\n");
					} else {
						_tcout << _T("=ARGUMENT\n");
					}
				}

				if (!(*ci).m_optDescription.empty()) {
					_tcout << _T("\t") << (*ci).m_optDescription << _T("\n");
				}
			}
		}
	};
}

std::basic_ostream<TCHAR>& operator<<(std::basic_ostream<TCHAR>& lhs, kjm::PathString& p);

/****************************************************************************
  lpExistingFileNameのファイルをlpNewFileNameにコピーします。コピー失敗時には
  指定回数分リトライを行います。

  <パラメータ>
  lpExistingFileName  コピー元
  lpNewFileName       コピー先
  bFailIfExists       上書きを行うか(FALSEで上書きを行う)
  retry               再試行を何回行うか
  wait                再試行を行うまで何秒待つのか

  <戻り値>
  コピーに成功したときTRUE、コピーに失敗したときFALSEを返す。
****************************************************************************/
BOOL IOUtil__CopyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists, int retry, int wait, LPDWORD pdwErr = NULL);

#endif
