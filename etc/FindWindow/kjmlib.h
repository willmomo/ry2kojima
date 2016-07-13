//---------------------------------------------------------------------
// kjmlib : Version 0.2011.8.31
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

#include <lm.h>
#pragma comment(lib, "netapi32.lib")

#include <shlobj.h>
// kjm::util::createShortcutで使用する.

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <assert.h>
#include <process.h>
#include <time.h>

#if defined(USE_WSSLOG)
#include "wsslog.h"
// #pragma comment(lib, "wsslog.lib")
#endif

#if !defined(countof)
#define countof(A)	(sizeof((A)) / sizeof((A)[0]))
#endif

// WinMain 使用時の __argv を使い分けるマクロ
#if defined(UNICODE)
#define __targv		__wargv
#else
#define __targv		__argv
#endif

namespace kjm {

	// TCHARを使用した文字列クラス
	typedef std::basic_string<TCHAR>	_tstring;

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
	// パス名を扱う文字列
	//-------------------------------------------------------------------------
	class PathString {
	private:
		std::basic_string<TCHAR> m_path;

	public:
		// コンストラクタ
		PathString() : m_path() {};
		PathString( const PathString& src ) : m_path( src.m_path ) {};
		PathString( LPCTSTR src ) : m_path( src ) {};
		PathString(const std::basic_string<TCHAR>& src) : m_path(src) {};

		// デストラクタ
		virtual ~PathString() {};

	public:
		// 文字列への const pointer を戻す
		LPCTSTR c_str() const { return m_path.c_str(); };

		// 内部文字列を返す
		std::basic_string<TCHAR> getString() const { return m_path; }

		// 文字列にパスを結合する
		PathString& Append( const PathString& more ) {
			std::vector<TCHAR> buf( MAX_PATH );
			
			::lstrcpy(&buf[0], m_path.c_str() );
			::PathAppend(&buf[0], more.c_str() );

			m_path = &buf[0];

			return *this;
		};

	public:
		// 代入演算子
		PathString& operator= ( const PathString& src ) {
			if (this == &src)	return *this;	// 同一オブジェクト代入チェック
			m_path = src.c_str();
			return *this;
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
		// PathRenameExtensionのラッパー
		static std::basic_string<TCHAR>& pathRenameExtension(std::basic_string<TCHAR>& strPath, LPCTSTR pszExt);

		//---------------------------------------------------------------------
		// Unicode 文字列を ANSI 文字列に変換
		//---------------------------------------------------------------------
		static std::string toAnsi(const std::wstring& src);

		// ANSI 文字列を Unicode 文字列に変換
		static std::wstring toUnicode(const std::string& src);

		// 文字列終端の改行を取り除く
		static std::basic_string<TCHAR>& chomp(std::basic_string<TCHAR>& s);

		//-------------------------------------------------------------
		// コピー元の直下にあるすべてのファイルをコピー先にコピーする
		//
		// [引数]
		// pszSrcPath  コピー元フォルダの名前
		// pszDstPath  コピー先フォルダの名前
		//
		// [戻り値]
		// 正常終了: NO_ERROR
		// エラー終了: Win32エラーコード
		//-------------------------------------------------------------
		static DWORD CopyFiles(LPCTSTR pszSrcPath, LPCTSTR pszDstPath);

		// システム標準のエラーメッセージを作成
		static std::basic_string<TCHAR> formatMessageBySystem(DWORD dwErr);

		// 汎用的なフォーマットメッセージの処理
		static DWORD formatMessage( DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, std::vector<TCHAR>& buffer, va_list* Arguments );

#if 0	// 不要関数
		// プロセスのモジュール名を取得(GetModuleFileName 関数のラッパー)
		///static std::basic_string<TCHAR> getModuleFileName();
#endif

		// Win32API GetModuleFileName を安全に使う関数
		static std::basic_string<TCHAR> getModuleFileName(HMODULE hModule = NULL);

		// string を返す GetComputerName 関数
		static std::basic_string<TCHAR> getComputerName();

		// プロセス名をstringで取得する関数
		static std::basic_string<TCHAR> GetModuleName() {
			return std::basic_string<TCHAR>(::PathFindFileName(kjm::util::getModuleFileName().c_str()));
		}

		// 環境変数を展開する
		static DWORD expandEnvironmentStrings(LPCTSTR lpSrc, std::vector<TCHAR>& rDst);

		// ini ファイルから文字列を取得する(GetPrivateProfileString 関数のラッパー)
		static std::basic_string<TCHAR> getPrivateProfileString(const std::basic_string<TCHAR>& section,
			const std::basic_string<TCHAR>& key, const std::basic_string<TCHAR>& defValue, const std::basic_string<TCHAR>& iniFile);

		static DWORD get_ipaddr(LPCTSTR lpName);	// ホスト名または、IP文字列からIPアドレスを取得

		// ディレクトリを再帰的に削除する
		static bool remove_folder(LPCTSTR pszPath);

		// 文字列を返す安全な sprintf 関数
		static std::basic_string<TCHAR> sprintf_str(LPCTSTR format, ...);

		// ファイル名部分を取得
		static std::basic_string<TCHAR> findFileName(const std::basic_string<TCHAR>& path);

		// パス文字列にパスを追加
		static std::basic_string<TCHAR> append(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& more);

		// パス文字列の終端 \ を削除する
		static std::basic_string<TCHAR> removeBackslash(const std::basic_string<TCHAR>& path);

		// パス文字列の拡張子を変更する
		static std::basic_string<TCHAR> renameExtension(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& ext);

		// パス文字列のファイル名を変更する
		static std::basic_string<TCHAR> renameFileSpec(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& fname);

		// パス文字列のファイル名部分を削除する
		static std::basic_string<TCHAR> removeFileSpec(const std::basic_string<TCHAR>& path);

		// ディレクトリかどうか調べる
		static bool isDirectory(const std::basic_string<TCHAR>& path) { return (PathIsDirectory(path.c_str())) ? true : false; }

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
		static bool deleteFiles(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildcard);

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
	class timeSpan {
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

		// デストラクタ
		virtual ~dateTime() {}

		// 代入演算子
		dateTime& operator=(const dateTime& rhs);

		// 内部数値を int64 で取得
		__int64 get_Value() const { return *(__int64*)&m_ft; }

		// dateTime の引き算
		timeSpan operator -(const dateTime& rhs) { return kjm::timeSpan(get_Value() - rhs.get_Value()); }

		// ローカル時間に変更
		dateTime toLocalTime();

		// 日付をフォーマットして出力
		_tstring toLongDateString();

		// 時間をフォーマットして出力
		_tstring toLongTimeString();

	public:
		// ローカル現在日時を持つdateTimeオブジェクトを生成
		static dateTime now();

		// UTC現在日時を持つ dateTime オブジェクトを生成
		static dateTime utcNow();

	private:
		FILETIME m_ft;
		int m_kind;		// DTK_UNSPECIFIED, DTK_UTC, DTK_LOCAL
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
		// ファイルまたはディレクトリ、およびその内容を新しい場所にコピーします。
		static BOOL copy(LPCTSTR sourceDirName, LPCTSTR destDirName);

		// ファイルを列挙します。
		static std::vector<std::basic_string<TCHAR> > getFiles(
			const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildCard = _T("*.*"));

		// ディレクトリを列挙します。
		static std::vector<std::basic_string<TCHAR> > getSubFolders(
			const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildCard = _T("*.*"));

	private:
		directory() {};
		~directory() {};
	};

	//-------------------------------------------------------------------------
	// 現在の環境およびプラットフォームに関する情報、およびそれらを操作する手
	// 段を提供します。(.NETの部分的実装)
	//-------------------------------------------------------------------------
	class environment {
	public:

		// ローカル コンピュータの NetBIOS 名を取得します。
		static _tstring get_MachineName() { return kjm::util::getComputerName(); }

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

	// mutex 管理クラス
	//
	// note: 作成したハンドルは、デストラクタで破棄される。
	//       また、lock(WaitForSingleObject)したままのハンドルは、
	//       必要回数unlock(ReleaseMutex)してから、破棄される。
	//
	class mutex {
	private:
		HANDLE m_hMutex;		// 管理する mutex へのハンドル
		DWORD m_create_status;	// mutex 作成時の状態変数
		DWORD m_last_error;		// API使用時のエラー保持
		int m_lock_count;		// lockした回数

		// 面倒なので、インスタンスの複製は不許可。
		mutex(const mutex &src) {};
		mutex& operator=(const mutex &src) {};

	public:
		// mutex 作成時に同名の mutex が存在したときは、ture を返す
		//
		bool is_exists() { return (m_create_status == ERROR_ALREADY_EXISTS); };

		// mutex作成済みかどうかのテスト
		//
		bool is_open() { return (m_hMutex != NULL); };

	public:
		// コンストラクタとデストラクタ
		//
		mutex() : m_hMutex(NULL), m_create_status(0), m_lock_count(0) {};
		mutex(const std::basic_string<TCHAR>& name) : m_hMutex(NULL), m_create_status(0), m_lock_count(0) {
			create(name);	// 例外は起こりえない。チェックは、is_openでチェック
		};
		virtual ~mutex() { close(); };

		// mutex の作成
		// 
		// arg : name = mutex の名前
		// ret : true = 正常終了、false = 以上終了
		// exp : mutex_error
		// note:
		//
		bool create(const std::basic_string<TCHAR>& name) {
			// 初期化済み instance の時は、例外を throw
			if (m_hMutex)	throw mutex_error(mutex_error::already_initialized);

			// mutex の作成
			m_hMutex = ::CreateMutex(NULL, FALSE, name.c_str());
			m_create_status = ::GetLastError();
			return (m_hMutex != 0);
		};
		
		// mutex を閉じる
		//
		// arg : なし
		// ret : なし
		// exp : なし
		// note: 未初期化のインスタンスに使用してもok
		//       lock中のオブジェクトに対しては、unlockしてからクローズする。
		//
		void close() {
			if (m_hMutex) {
				while (m_lock_count)
					if (!unlock())
						break;

				::CloseHandle(m_hMutex);
				m_hMutex = NULL;
			}
		};

		// 指定時間シグナル状態になるのを待つ
		//
		// arg : dwMillisec = タイムアウト時間(ms)
		// ret : WaitForSingleObject API 参照
		// exp : mutex_error
		// note: mutexが初期化済みの場合は、WaitForSingleObjectを呼ぶだけ。
		//
		DWORD lock(DWORD dwMillisec) {
			if (!m_hMutex)	throw mutex_error(mutex_error::not_initialized);
			
			DWORD dw = ::WaitForSingleObject(m_hMutex, dwMillisec);
			if (dw == WAIT_OBJECT_0 || dw == WAIT_ABANDONED) {
				++m_lock_count;
			}

			return dw;
		};

		// mutexを開放する
		//
		// arg : なし
		// ret : ReleaseMutex API 参照
		// exp : mutex_error
		// note: mutexが初期化済みの場合は、ReleaseMutex を呼ぶだけ。
		//
		BOOL unlock() {
			if (!m_hMutex)	throw mutex_error(mutex_error::not_initialized);
			
			BOOL ret = ::ReleaseMutex(m_hMutex);
			if (ret) {
				--m_lock_count;
			}

			return ret;
		};
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
		std::basic_string<TCHAR> m_longOption;	// ロングオプション文字列(未使用)
		std::basic_string<TCHAR> m_shortOption;	// ショートオプション文字列
		int m_optionArgInfo;					// 0 = オプションなし, 1 = オプションをとりうる, 2 = オプションが必須

		// デフォルトコンストラクタ
		optionInfo() : m_longOption(), m_shortOption(), m_optionArgInfo(kjm::no_argument) {}

		// 初期化つきコンストラクタ
		optionInfo(const std::basic_string<TCHAR>& longOption, const std::basic_string<TCHAR>& shortOption, int optionArgInfo)
			: m_longOption(longOption), m_shortOption(shortOption), m_optionArgInfo(optionArgInfo) {}

		// コピーコンストラクタ
		optionInfo(const kjm::optionInfo& src)
			: m_longOption(src.m_longOption), m_shortOption(src.m_shortOption), m_optionArgInfo(src.m_optionArgInfo) {}

		// 代入演算子
		optionInfo& operator=(const kjm::optionInfo& rhs);

		// ショートオプションが同じかどうか調べる(参照バージョン)
		bool isSameShortOption(const std::basic_string<TCHAR>& name) const { return (m_shortOption == name); }

		// ロングオプションが同じかどうか調べる(参照バージョン)
		bool isSameLongOption(const std::basic_string<TCHAR>& name) const { return (m_longOption == name); }

#if _MSC_VER <= 1500	// VC++2008
		// ショートオプションが同じかどうか調べる(ポインタバージョン)
		bool isSameShortOption_P(const std::basic_string<TCHAR>* name) const { return (m_shortOption == *name); }

		// ロングオプションが同じかどうか調べる(ポインタバージョン)
		bool isSameLongOption_P(const std::basic_string<TCHAR>* name) const { return (m_longOption == *name); }
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

	public:
		// オプションがあるかどうか調べる
		bool hasOption(const std::basic_string<TCHAR>& key) { return (m_options.find(key) != m_options.end()) ? true : false; }

		// オプションの引数を取得する
		std::basic_string<TCHAR> getOptArgument(const std::basic_string<TCHAR>& key);

		// オプション以外の引数の数を取得する
		int get_argCount() { return this->m_arguments.size(); }

		// N 番目のオプション以外の引数を取得する
		std::basic_string<TCHAR> get_argument(int n) { return this->m_arguments.at(n); }

	private:
		// オプションとその引数
		std::map<std::basic_string<TCHAR>, std::basic_string<TCHAR> > m_options;

		// オプション以外の引数
		std::vector<std::basic_string<TCHAR> > m_arguments;
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
		iniFile() : m_fileName() {};

		// コピーコンストラクタ
		iniFile(const iniFile& src) : m_fileName(src.m_fileName) {};

		// ファイル名を与えるコンストラクタ
		iniFile(const std::basic_string<TCHAR>& szFileName) : m_fileName(szFileName) {};

		// デストラクタ
		virtual ~iniFile() {};

		// 代入演算子
		kjm::iniFile& operator=(const iniFile& rhs) {
			if (this == &rhs) return *this;
			this->m_fileName = rhs.m_fileName;
			return *this;
		};

		// セクション一覧を取得
		std::vector<std::basic_string<TCHAR> > getSectionNames() const;

		// セクション内のキー一覧を取得
		std::vector<std::basic_string<TCHAR> > getKeyNames(const std::basic_string<TCHAR>& strSectionName) const;

		// iniファイルから値を取得
		std::basic_string<TCHAR> getString(const std::basic_string<TCHAR>& strSection,
			const std::basic_string<TCHAR>& strKey, const std::basic_string<TCHAR>& strDefault = _T(""), BOOL* pfUseDefault = NULL) const;

		// iniファイルに値を書き込む
		BOOL writeString(const std::basic_string<TCHAR>& strSection, const std::basic_string<TCHAR>& strKey, const std::basic_string<TCHAR>& strValue);

		// 別のiniファイルの内容をマージする
		void mergeIniFile(const iniFile& newIniFile);

		// iniファイル名を取得
		std::basic_string<TCHAR> get_fileName() const { return m_fileName; };

		// iniファイル名を設定
		void set_fileName(const std::basic_string<TCHAR>& value) { m_fileName = value; };

	private:
		std::basic_string<TCHAR> m_fileName;	// iniファイルの名前
	};

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
			m_hThread = ( HANDLE )_beginthreadex( NULL, 0, thread_func, this, 0, NULL );
		};

		/*!
		 * スレッドを終了させる。派生クラスで定義する。
		 * 終了方法は、worker method の実装に依存する。
		 */
		virtual void end() = 0;

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
}

#endif
