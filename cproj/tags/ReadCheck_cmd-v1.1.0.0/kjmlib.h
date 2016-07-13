//---------------------------------------------------------------------
// kjmlib : Version 0.2014.2.19
//
// kjmlib.h : ユーティリティ的に使えるライブラリ
//
// ほかのコードとバッティングしないように、namespace kjm で、作成してあります。
// stlベースの処理ばかりなので、dll と exe をまたいで使用することは、できません。
//
// USE_WSSLOG を define しておくと、wsslog.dll を使って、ログを残す。
//
// kjmlib.cpp とセットで、複製して使用してください。
// shlwapi.lib をリンクしてください。
//
// 2007.09.05
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
#include <sql.h>
#include <sqlext.h>

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <iomanip>

#include <assert.h>
#include <process.h>
#include <time.h>
#include <share.h>
#include <locale.h>
#include <sys/stat.h>

#if defined(USE_WSSLOG)
#include "wsslog.h"
// #pragma comment(lib, "wsslog.lib")
#endif


#if defined(USE_MSXML6)
#import <msxml6.dll> named_guids
#endif


// Windows の DOS 窓でUnicodeを出力できるようにする。
// Windows 下の諸々の処理よりも、中国語なども表示できるようになり便利
// printf系、cout系どちらにも有効
// MBCSの時は何もしない処理になる。
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

typedef std::basic_string<TCHAR>	_tstring;

namespace kjm {

	// TCHARを使用した文字列クラス
	typedef std::basic_string<TCHAR>	_tstring;
	typedef std::vector<_tstring>		_tstrings;

	//-------------------------------------------------------------------------
	// このネームスペースで使われる、一般的な例外型
	//-------------------------------------------------------------------------
	class exception {
	private:
		exception() {};

	public:
		exception(DWORD errCode, const _tstring& errMsg) : m_errCode(errCode), m_errMsg(errMsg) {}

		virtual ~exception() {}

	public:
		DWORD m_errCode;
		_tstring m_errMsg;
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

#if 0	// 不要関数
		// プロセスのモジュール名を取得(GetModuleFileName 関数のラッパー)
		///static std::basic_string<TCHAR> getModuleFileName();
#endif

		// 1行読み込み関数
		//
		// @return 0 正常終了
		// @return 1 eofで読み込み終了
		// @erturn -1 errorで読み込み終了
		static int getline(FILE *fp, kjm::_tstring &str);

		// Win32API GetModuleFileName を安全に使う関数
		static std::basic_string<TCHAR> getModuleFileName(HMODULE hModule = NULL);

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

	public:
		time() : m_time(0) {}
		time(time_t t) : m_time(t) {}
		time(LPCTSTR time_text) { parse(time_text); }
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
	};

	//-------------------------------------------------------------------------
	// 時差を扱うクラス
	//-------------------------------------------------------------------------

// FILETIME を扱いやすい時間に変換するための定数
/***
#define FT_1USEC			(__int64)10
#define FT_1MSEC			(FT_1USEC * 1000)
#define FT_1SEC				(FT_1MSEC * 1000)
#define FT_1MIN				(FT_1SEC  *   60)	// FILETIME型の1分の値
#define FT_1HOUR			(FT_1MIN  *   60)	// FILETIME型の1時間の値
#define FT_1DAY				(FT_1HOUR *   24)	// FILETIME型の1日の値
#define FT_19700101			(116444736000000000)		// time_t の開始地点を表すFILETIMEの値
#define FT_19700101_SEC		(FT_19700101 / FT_1SEC)
***/

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

#define DTK_UNSPECIFIED	0	// DateTimeKind.Unspecified 相当
#define DTK_UTC			1	// DateTimeKind.Utc 相当
#define DTK_LOCAL		2	// DateTimeKind.Local 相当

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
	public:
		// デフォルトコンストラクタ
		dateTime() : m_kind(DTK_UNSPECIFIED) { memset(&m_ft, 0, sizeof(m_ft)); }

		// コピーコンストラクタ
		dateTime(const dateTime& src) : m_ft(src.m_ft), m_kind(src.m_kind) {}

		// FILETIMEで初期化するコンストラクタ
		dateTime(FILETIME src, int kind = DTK_UNSPECIFIED) : m_ft(src), m_kind(kind) {}

		// SYSTEMTIME で初期化するコンストラクタ
		dateTime(const SYSTEMTIME& st, kjm::dateTimeKind kind = kjm::unspecified) : m_kind(kind) { ::SystemTimeToFileTime(&st, &m_ft); }

		// デストラクタ
		virtual ~dateTime() {}

		// 内部数値を int64 で取得
		__int64 get_Value() const { return *(__int64*)&m_ft; }
		
		// 現在時刻を__int64で返す
		__int64 getTime() const { return *(__int64*)&m_ft; };

		// ローカル時間に変更
		dateTime toLocalTime();

		// 日付をフォーマットして出力
		_tstring toLongDateString() const;

		// 時間をフォーマットして出力
		_tstring toLongTimeString();

		// 日時をフォーマットして出力
		_tstring toString(const _tstring& format);

		// 日時をフォーマットして出力
		_tstring toString() const;

		// 時間を文字列(YYYYMMDD)に変換
		virtual _tstring toString_yyyymmdd() const;

		// 日付を足した dateTime を返す
		dateTime addDays(int nDays) const {
			LONGLONG ll = get_Value() + (kjm::timeSpan::TicksPerDay * nDays);
			return dateTime(*(FILETIME*)&ll);
		}

		// 代入演算子
		dateTime& operator=(const dateTime& rhs);

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

		// 分を取得
		int get_Minute() const;

		// 時を取得
		int get_Hour() const;

		// 日を取得
		int get_Day() const;
	
		// 月を取得
		int get_Month() const;

		// 曜日を取得
		int get_Week() const;

	public:
		// ローカル現在日時を持つdateTimeオブジェクトを生成
		static dateTime now();

		// UTC現在日時を持つ dateTime オブジェクトを生成
		static dateTime utcNow();

		// 現在のローカル日時を取得する
		static dateTime getLocalTime() {
			SYSTEMTIME st;
			::GetLocalTime(&st);
			return dateTime(st, kjm::local);
		}

	private:
		FILETIME m_ft;
		int m_kind;		// DTK_UNSPECIFIED, DTK_UTC, DTK_LOCAL
	};

	//-------------------------------------------------------------------------
	// C言語の fopen を安全に使うクラス
	//-------------------------------------------------------------------------
	class cfile {
	public:
		// デフォルトコンストラクタ
		cfile() : m_fp(NULL) {}

		// 開く付きコンストラクタ
		cfile(const _tstring& fname, const _tstring& mode) : m_fp(NULL) {
			this->open(fname, mode);
		}

		// デストラクタ
		virtual ~cfile();

		// fopenの代わり
		virtual bool open(const _tstring& fname, const _tstring& mode);

		// fcloseの代わり
		virtual bool close();

		// ファイルが開いているかどうか調べる
		virtual bool is_open() const {
			return (m_fp != NULL);
		}

		// 一行読込
		virtual int getline(_tstring& lineBuf) {
			return kjm::util::getline(m_fp, lineBuf);
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

	private:
		file() {}
		~file() {}
	};

	//=========================================================================
	// パス名に関する処理
	//=========================================================================
	class path {
	public:
		// 指定したパス文字列の拡張子(ピリオドを含む)を返す。
		static kjm::_tstring getExtension(const kjm::_tstring& path);

		// 2つの文字列を1つのパスに結合します。
		static kjm::_tstring combine(const kjm::_tstring& path1, const kjm::_tstring& path2);

		// 2つの文字列を1つのパスに結合します。
		static kjm::_tstring append(const kjm::_tstring& path1, const kjm::_tstring& path2) {
			return kjm::path::combine(path1, path2);
		}

		// フルパスからファイル名を取り除く
		static kjm::_tstring removeFileSpec(const kjm::_tstring& strPath) {
			TCHAR szTemp[MAX_PATH];
			PathRemoveFileSpec(lstrcpy(szTemp, strPath.c_str()));
			return szTemp;
		}

		// フルパスからファイル名を変更する
		static kjm::_tstring renameFileSpec(const kjm::_tstring& strPath, const kjm::_tstring& strFileSpec) {
			return kjm::path::append(kjm::path::removeFileSpec(strPath), strFileSpec);
		}

		// ファイルが存在するかどうか調べる
		static bool fileExists(const kjm::_tstring& strPath) {
			return (PathFileExists(strPath.c_str()) != FALSE);
		}

		// ファイル名部分を取得
		static kjm::_tstring getFileName(const kjm::_tstring& path);
		
		// フルパスから、ファイル名を取り出す
		static kjm::_tstring findFileName(const kjm::_tstring& strPath) {
			return PathFindFileName(strPath.c_str());
		}

		// ディレクトリかどうか調べる
		static bool isDirectory(const _tstring& strPath) {
			return (PathIsDirectory(strPath.c_str()) != FALSE);
		}

		// 相対パスを作成する
		static _tstring relativePathTo(const _tstring& from, DWORD attrFrom, const _tstring& to, DWORD attrTo);

		// PathRemoveExtension 関数のラッパー
		static _tstring removeExtension(const _tstring& strPath);
	private:
		path() {}
		~path() {}
	};

	//=========================================================================
	// ディレクトリやサブディレクトリを通じて、作成、移動、および列挙するための
	// 静的メソッドを公開します。
	//=========================================================================
	class directory {
	public:
		// ファイルまたはディレクトリ、およびその内容を新しい場所にコピーします。
		static BOOL copy(LPCTSTR sourceDirName, LPCTSTR destDirName);

		// ファイルを列挙します。
		static std::vector<std::basic_string<TCHAR> > getFiles(
			const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildCard = _T("*.*"));

		// ディレクトリを列挙します。
		static std::vector<std::basic_string<TCHAR> > getSubFolders(
			const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildCard = _T("*.*"));

		// ディレクトリを作成する。
		static void createDirectory(const _tstring& path);

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
	public:
		// デフォルトコンストラクタ
		fileInfo() : m_hasInfo(false) {}

		// コピーコンストラクタ
		fileInfo(const fileInfo& src) : m_fullPath(src.m_fullPath), m_lastWriteTime(src.m_lastWriteTime), m_fileSize(src.m_fileSize), m_dwFileAttributes(src.m_dwFileAttributes), m_hasInfo(src.m_hasInfo) {}

		// WIN32_FIND_DATA で初期化するコンストラクタ
		fileInfo(const _tstring& strParentPath, const WIN32_FIND_DATA& rFindData) {
			m_fullPath = path::append(strParentPath, rFindData.cFileName);
			m_lastWriteTime = dateTime(rFindData.ftLastWriteTime, kjm::utc);

			LARGE_INTEGER li = {rFindData.nFileSizeLow, rFindData.nFileSizeHigh};
			m_fileSize = li.QuadPart;

			m_dwFileAttributes = rFindData.dwFileAttributes;

			m_hasInfo = true;	// [情報を持っている]で、初期化
		}

		// ファイル名で初期化するコンストラクタ
		fileInfo(const _tstring& strName) : m_fullPath(strName) {
			findFile ff;
			if (ff.findFirstFile(strName)) {
				m_lastWriteTime = ff.getLastWriteTime();
				m_fileSize = ff.getFileSize();
				m_dwFileAttributes = ff.getFileAttributes();
	
				m_hasInfo = true;	// [情報を持っている]で、初期化
			} else {
				m_hasInfo = false;	// [情報を持っていない]で、初期化
			}
		}

		// デストラクタ
		virtual ~fileInfo() {}

		// 代入演算子
		fileInfo& operator =(const fileInfo& rhs) {
			if (this != &rhs) m_fullPath = rhs.m_fullPath;
			return *this;
		}

		/// ファイルが存在するかどうかを示す値を取得します。
		bool exists() const { return kjm::path::fileExists(this->m_fullPath); }

		/// 絶対パスを取得
		_tstring fullName() const { return m_fullPath; }

		// ファイル名部分を返す
		//_tstring getFileName() { return path::findFileName(m_fullPath); };
		_tstring name() const { return kjm::path::findFileName(m_fullPath); }

		/// <summary>最終更新日時を取得</summary>
		/// <returns>最終更新日時を持つ kjm::dateTime オブジェクト。</returns>
		dateTime lastWriteTime() const { return kjm::dateTime(m_lastWriteTime).toLocalTime(); }

		// ファイルの属性を返す
		fileAttributes attributes() {
			assert(m_hasInfo != false);
			return (kjm::fileAttributes)m_dwFileAttributes;
		}

		// ファイルのサイズを返す
		__int64 length() const {
			assert(m_hasInfo != false);
			return m_fileSize;
		}

		// ファイルが「アーカイブ可能」かどうか調べる
		bool isArchive() const {
			assert(m_hasInfo != false);
			return ((m_dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0);
		}

		// ファイルが「隠しファイル」かどうか調べる
		bool isHidden() const {
			assert(m_hasInfo != false);
			return ((m_dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0);
		}

		// ファイルが「読み取り専用」かどうか調べる
		bool isReadonly() const {
			assert(m_hasInfo != false);
			return ((m_dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0);
		}

		// ファイルが「システム」かどうか調べる
		bool isSystem() const {
			assert(m_hasInfo != false);
			return ((m_dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0);
		}

		// ファイルのバージョン情報を返す
		_tstring getFileVersion() { return kjm::util::getFileVersion(m_fullPath); }

	private:
		_tstring m_fullPath;		// ファイルのフルパス
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
	// プロセスの起動に関するクラス
	//-------------------------------------------------------------------------
	class process {
	public:
		// デフォルト コンストラクタ
		process();

		// デストラクタ
		virtual ~process() { this->Close(); }

	public:
		// スタートアップ情報をクリアする
		void ClearSI() { memset(&m_si, 0, sizeof(m_si)); }

		// スタートアップ情報(wShowWindow)を取得する
		WORD GetSIShowWindow() const { return m_si.wShowWindow; }

		// スタートアップ情報(wShowWindow)を設定する
		void SetSIShowWindow(WORD newShowWindow);

		// スタートアップ情報(wShowWindow)をクリア(未使用に)する
		void ClearSIShowWindow();

		// プロセスID取得
		DWORD GetProcessId() const { return m_pi.dwProcessId; }

		// 実行ファイルのコマンドライン取得
		LPCTSTR GetCommandLine() const { return m_szCommandLine; }

		// プロセスの終了コードを取得
		DWORD getExitCode();

		// プロセスハンドルが有効かどうか調べる
		bool IsHandleAvailable() { return (m_pi.hThread != NULL && m_pi.hProcess != NULL) ? true : false; }

		// プロセスが動作中かどうか調べる
		bool IsStillActive() { return (IsHandleAvailable() && (getExitCode() == STILL_ACTIVE)) ? true : false; }

	public:
		// プロセスの実行
		virtual BOOL createProcess(const std::basic_string<TCHAR>& commandLine);

		// OpenProcess APIのラッパー
		//
		// [引数]
		// dwDesiredAccess  アクセスフラグ
		// bInheritHandle   継承フラグ
		// dwProcessId      プロセスID
		//
		// [戻り値]
		// 関数が成功すると、true が返る。
		// 関数が失敗すると、false が返る。
		bool openProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId) {
			memset(&m_pi, 0, sizeof(m_pi));

			m_pi.dwProcessId = dwProcessId;
			m_pi.hProcess = ::OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);

			return m_pi.hProcess != NULL;
		};

		// プロセスハンドルを閉じる。プロセスの終了ではないので注意。
		// プロセスの終了は、プロセス間の取り決めによる。
		void Close();

		// プロセスの終了を待つ
		virtual DWORD waitForTermination(DWORD dwMilliseconds = INFINITE) { return ::WaitForSingleObject(m_pi.hProcess, dwMilliseconds); }

		// プロセスの起動完了を待つ
		virtual DWORD waitForInputIdle(DWORD dwMilliseconds = INFINITE) { return ::WaitForInputIdle(m_pi.hProcess, dwMilliseconds); }

		// プロセスに関連付けられたウィンドウを返す
		HWND GetProcessWindow();

		// プロセスを強制終了
		virtual BOOL kill(UINT uExitCode) {
			return ::TerminateProcess(m_pi.hProcess, uExitCode);
		}

		// プロセスIDからプロセスを終了させるユーティリティ関数
		//
		// [引数]
		// dwProcessId  プロセスID
		// uExitCode    exit code for the process
		// pLog         ログオブジェクトへのポインタ。!NULL のとき、これを使用してログを残す。
		//
		// [戻り値]
		// 関数が成功すると、TRUE が返る。
		// 関数が失敗すると、FALSE が返る。
		static BOOL terminateProcess(DWORD dwProcessId, UINT uExitCode);

	private:
		HWND m_hwnd;				// GetProcessWindow関数で使用

		STARTUPINFO m_si;					// スタートアップ情報
		PROCESS_INFORMATION m_pi;			// プロセス情報
		TCHAR m_szCommandLine[MAX_PATH];	// 実行コマンドライン記録用
											// Create関数実行後有効
		
		static BOOL CALLBACK process_EnumWindowsProc(HWND hwnd, LPARAM lParam);
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

	//-------------------------------------------------------------------------
	// コマンドライン解析に渡すオプション情報構造体
	//-------------------------------------------------------------------------
	static const int no_argument = 0;
	static const int optional_argument = 1;
	static const int required_argument = 2;

	struct optionInfo {
		_tstring m_longOption;	// ロングオプション文字列
		_tstring m_shortOption;	// ショートオプション文字列
		int m_optionArgInfo;					// 0 = オプションなし, 1 = オプションをとりうる, 2 = オプションが必須
		_tstring m_optDescription;	// オプションの説明
		_tstring m_argDescription;	// 引数の説明

		// デフォルトコンストラクタ
		optionInfo() : m_longOption(), m_shortOption(), m_optionArgInfo(kjm::no_argument), m_optDescription(), m_argDescription() {}

		// 初期化つきコンストラクタ
		optionInfo(const _tstring& longOption, const _tstring& shortOption, int optionArgInfo, const _tstring& optDescription = _T(""), const _tstring& argDescription = _T(""))
			: m_longOption(longOption), m_shortOption(shortOption), m_optionArgInfo(optionArgInfo), m_optDescription(optDescription), m_argDescription(argDescription) {}

		// コピーコンストラクタ
		optionInfo(const kjm::optionInfo& src)
			: m_longOption(src.m_longOption), m_shortOption(src.m_shortOption), m_optionArgInfo(src.m_optionArgInfo), m_optDescription(src.m_optDescription), m_argDescription(src.m_argDescription) {}

		// 代入演算子
		optionInfo& operator=(const kjm::optionInfo& rhs);

		// ショートオプションが同じかどうか調べる(参照バージョン)
		bool isSameShortOption(const _tstring& name) const { return (m_shortOption == name); }

		// ロングオプションが同じかどうか調べる(参照バージョン)
		bool isSameLongOption(const _tstring& name) const { return (m_longOption == name); }

#if _MSC_VER <= 1500	// VC++2008
		// ショートオプションが同じかどうか調べる(ポインタバージョン)
		bool isSameShortOption_P(const _tstring* name) const { return (m_shortOption == *name); }

		// ロングオプションが同じかどうか調べる(ポインタバージョン)
		bool isSameLongOption_P(const _tstring* name) const { return (m_longOption == *name); }
#endif
	};

	typedef std::vector<kjm::optionInfo>	optionInfoList;

	//-------------------------------------------------------------------------
	// コマンドライン解析クラス
	//-------------------------------------------------------------------------
	class cmdLine {
	public:
		// デフォルトコンストラクタ
		cmdLine() {}

		// デストラクタ
		virtual ~cmdLine() {}

		// コマンドライン解析
		virtual int parse(int argc, TCHAR** argv, const optionInfoList& opts);

		// コマンドライン解析
		virtual int parse(int argc, TCHAR** argv) { return parse(argc, argv, optionInfoList()); }

		// コマンドライン解析
		virtual int chkarg(int argc, TCHAR **argv, const _tstring &shortOpts);

	public:
		// オプションがあるかどうか調べる
		bool hasOption(const _tstring &key) const
			{ return (m_options.find(key) != m_options.end()); }

		// オプションの引数を取得する
		_tstring getOptArgument(const _tstring &key) const
			{ return (hasOption(key) ? (*m_options.find(key)).second : _T("")); }

		// オプションの引数を取得する	
		_tstring get_option_args(TCHAR key) { return getOptArgument(_tstring(1, key)); }

		// オプション以外の引数の数を取得する
		int get_argCount() { return this->m_arguments.size(); }

		// N 番目のオプション以外の引数を取得する
		_tstring get_argument(int n) { return this->m_arguments.at(n); }

		// オプションがある場合、引数なしのときは 1 、引数有のときは 2 を返す。オプションがなければ 0
		int get_option_state(const _tstring &key)
			{ return (this->hasOption(key)) ? 0 : ( (this->m_options[key].empty()) ? 1 : 2 ); }

		// オプションがある場合、引数なしのときは 1 、引数有のときは 2 を返す。オプションがなければ 0
		int get_option_state(TCHAR key) { return get_option_state(_tstring(1, key)); }

	private:
		// オプションとその引数
		std::map<_tstring, _tstring> m_options;

		// オプション以外の引数
		std::vector<_tstring> m_arguments;
	};

	//-----------------------------------------------------------------
	// コマンドライン引数解析
	//-----------------------------------------------------------------
	class commandLine {
	public:
		// デフォルト コンストラクタ
		commandLine() {}

		// 引数解析付きコンストラクタ
		commandLine(int argc, TCHAR** argv, TCHAR* hasArgShortOpts) { parse(argc, argv, hasArgShortOpts); }

		// コピーコンストラクタ
		commandLine(const commandLine& src) : m_optList(src.m_optList), m_argList(src.m_argList) {}

		// デストラクタ
		virtual ~commandLine() {}

		// 引数解析
		//
		// @arg  hasArgShortOpts  引数を持つことがあるショートオプションを列挙した文字列へのポインタ
		//
		int parse(int argc, TCHAR** argv, TCHAR* hasArgShortOpts);

		// 引数一覧を取得する
		std::vector<_tstring> getArgList() { return m_argList; }

		// オプション以外の引数の数を返す
		int getArgListCount() { return m_argList.size(); };

		// オプション一覧を取得する
		std::vector<_tstring> getOptList() {
			std::vector<_tstring> result;
			std::map<_tstring, _tstring>::iterator i;
			for (i = m_optList.begin(); i != m_optList.end(); i++) {
				result.push_back((*i).first);
			}
			return result;
		}

		// オプションがあったかどうか調べる
		bool hasKey(const _tstring& key) {
			return (m_optList.find(key) != m_optList.end());
		}

		// オプションの引数を取得する
		_tstring getOptArg(const _tstring& key) {
			return hasKey(key) ? m_optList[key] : _T("");
		}

		// 代入演算子
		commandLine& operator =(const commandLine& rhs) {
			if (this != &rhs) {
				m_optList = rhs.m_optList;
				m_argList = rhs.m_argList;
			}
			return *this;
		}

	private:
		// 解析したオプションを保存する変数
		std::map<_tstring, _tstring> m_optList;

		// オプション以外の引数を保存する変数
		std::vector<_tstring> m_argList;
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
		int get_maxLogSize() const { return this->m_maxLogSize; }
		void set_maxLogSize(int value) { this->m_maxLogSize = value; }

	private:
		// ログをシフトする
		void shift(const _tstring& fname);

	private:
		int m_maxLogSize;			// ログをシフトする単位(0でシフトしない)
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

	//-------------------------------------------------------------------------
	// GetTickCountで経過時間を計るクラス
	//-------------------------------------------------------------------------
	class tickCounter {
	public:
		// デフォルトコンストラクタ
		tickCounter() : m_t1(GetTickCount()), m_t2(0) {};

		// デストラクタ
		~tickCounter() {};

		// 計測開始
		void start() { m_t1 = GetTickCount(); m_t2 = 0; };

		// 計測終了
		void stop() { m_t2 = GetTickCount(); };

		// 経過時間(ms)
		DWORD elapse() { return (m_t2 == 0) ? (GetTickCount() - m_t1) : (m_t2 - m_t1); };

		// 経過時間(sec)
		DWORD elapseSec() { return elapse() / 1000; };

		// 経過時間(min)
		DWORD elapseMin() { return elapseSec() / 60; };

	private:
		DWORD m_t1;	// 開始した時の時間(コンストラクタ、startで更新)
		DWORD m_t2;	// 終了した時の時間(stopで更新)
	};

	//---------------------------------------------------------------------
	// ODBCを扱うユーティリティクラス
	//---------------------------------------------------------------------
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

	//-------------------------------------------------------------------------
	// 超簡単にXMLからデータを読込むclass
	//
	// このクラスを利用するときは、
	// プロジェクト全体に、USE_MSXML6 を定義してください。
	//
	// CoInitialize(NULL); と CoUninitialize(); は、アプリケーションで行ってください。
	//-------------------------------------------------------------------------
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
		bool still_active() { 
			DWORD code;
			GetExitCodeThread( m_hThread, &code );
			return ( code == STILL_ACTIVE );
		};

		//! スレッドを起動する。
		void run() {
			m_loop = true;
			m_hThread = ( HANDLE )_beginthreadex( NULL, 0, thread_func, this, 0, NULL );
		}

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

		/*!
		 * 共通のスレッド関数。
		 * argsには、threadクラスから派生したクラスオブジェクトへの
		 * ポインタを受け取る。p->worker() を実行するだけ。
		 */
		static unsigned __stdcall thread_func( void* args ) {
			return ( ( thread* )args )->worker();
		};
	};
#endif

	//-----------------------------------------------------------------------------
	// コマンドラインプログラムの基本形
	//-----------------------------------------------------------------------------
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

#endif
