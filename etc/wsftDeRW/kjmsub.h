//---------------------------------------------------------------------------------------
// kjmsub (20101217)
//---------------------------------------------------------------------------------------
/*
	【kjm::log 対応するログライブラリについて】
	ver.0.4.0.0 から追加。
	USE_WSSLOG を定義することで、WssLog.dll を使ってログを出力できるようになります。

	【kjm::util::getFileSize について】
	ver.0.3.0.0 から追加。

	【kjm::util::exitWindows について】
	ver.0.2.0.0 から追加。

	【GetComputerNameExについて】
	
	Visual C++ 6では、GetComputerNameEx関数が定義されていません。
	kjm::util::__GetComputerNameExの用に実装することで、利用することができます。

	コンピュータ名には、NetBIOS名とDNSホスト名がありますが、Windows2000以降では、DNSホス
	ト名がメインになります。また、物理的なコンピュータ名とクラスタ機能を使ったとき(だと思
	う)に付く、論理的なコンピュータ名があります。

	一般的なコンピュータ名を取得するには、第一引数にComputerNamePhysicalDnsHostname を指
	定して、GetComputerNameEx関数を呼び出します。

	【Visual C++ のバージョン判定について】

	_MSC_VER == 1200	Visual C++ 6.0
	_MSC_VER == 1310	Visual C++ .NET 2003
	_MSC_VER == 1400	Visual C++ 2005
	_MSC_VER == 1500	Visual C++ 2008
*/
#pragma once

#pragma warning(disable : 4786)

#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#endif

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "version.lib")

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <locale>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <cassert>
#include <ctime>
#include <process.h>

#if defined(USE_WSSLOG)
#include "wsslog.h"
#pragma comment(lib, "wsslog.lib")
#endif

#if defined(UNICODE)
#define _tstring			std::wstring
#define _tout				std::wcout
#define _tcout				std::wcout
#define _otstream			std::wostream
#define _otstringstream		std::wostringstream
#define _tifstream			std::wifstream
#define _tofstream			std::wofstream
#else
#define _tstring			std::string
#define _tout				std::cout
#define _tcout				std::cout
#define _otstream			std::ostream
#define _otstringstream		std::ostringstream
#define _tifstream			std::ifstream
#define _tofstream			std::ofstream
#endif

// STL の setlocale(LC_ALL, _T("japan")) 相当処理は、以下の通り。
// std::locale コンストラクタは常に、_MBCS文字列を受け取る。
#define STL_SETLOCALE_JAPAN		std::locale::global(std::locale("japanese"))

// _countof は、少なくとも vs2008 から標準で定義されていた
#if !defined(_countof)
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

// winbase.h で宣言されている、Get/SetComputerNameEx用の引数の値
// Visual C++ 6.0 以下のときに有効にする。
#if _MSC_VER <= 1200
typedef enum _COMPUTER_NAME_FORMAT {
    ComputerNameNetBIOS,
    ComputerNameDnsHostname,
    ComputerNameDnsDomain,
    ComputerNameDnsFullyQualified,
    ComputerNamePhysicalNetBIOS,
    ComputerNamePhysicalDnsHostname,
    ComputerNamePhysicalDnsDomain,
    ComputerNamePhysicalDnsFullyQualified,
    ComputerNameMax
} COMPUTER_NAME_FORMAT;
#endif

// FILETIME を扱いやすい時間に変換するための定数
#define FT_1USEC			(__int64)10
#define FT_1MSEC			(FT_1USEC * 1000)
#define FT_1SEC				(FT_1MSEC * 1000)
#define FT_1MIN				(FT_1SEC  *   60)	// FILETIME型の1分の値
#define FT_1HOUR			(FT_1MIN  *   60)	// FILETIME型の1時間の値
#define FT_1DAY				(FT_1HOUR *   24)	// FILETIME型の1日の値
#define FT_19700101			(116444736000000000)		// time_t の開始地点を表すFILETIMEの値
#define FT_19700101_SEC		(FT_19700101 / FT_1SEC)

// __int64 を iostream に出力するための関数
#if _MSC_VER < 1500		// 少なくとも、vs2008からは、__int64を受け取るostreamが存在するので、無効にする。
_otstream& operator<<(_otstream& os, __int64 i);
#endif

namespace kjm {
	/******************************************************************
		ディレクトリ内のファイル一覧を取得
	******************************************************************/
	bool dir(LPCTSTR pszPath, LPCTSTR pszWildcard, std::vector<_tstring>& result);

	//-------------------------------------------------------------------------
	// HBRUSH を管理するクラス
	//-------------------------------------------------------------------------
	class Brush {
	public:
		// デフォルトコンストラクタ
		Brush() : m_hbr(NULL) {};
		
		// ソリッドブラシを作成するコンストラクタ
		Brush(COLORREF crColor) : m_hbr(NULL) { createSolidBrush(crColor); };

		// デストラクタ
		virtual ~Brush() { deleteObject(); };

		// CreateSolidBrush のラッパー
		bool createSolidBrush(COLORREF crColor) {
			return ((m_hbr = CreateSolidBrush(crColor)) != NULL);
		};

		// DeleteObject のラッパー
		bool deleteObject() {
			bool bRet = true;
			if (m_hbr != NULL) {
				bRet = (DeleteObject(m_hbr) != FALSE);
				assert(bRet != FALSE);	// エラーになるとは、何事?
				m_hbr = NULL;
			}
			return bRet;
		};

		// HGDIOBJ へのキャスト
		operator HGDIOBJ () { return m_hbr; };

	private:
		// コピーと代入を許可しない
		Brush(const Brush& src) {};
		Brush& operator =(const Brush& rhs) { return *this; };

	private:
		HBRUSH m_hbr;
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
		virtual void set_prefix(const _tstring& strPrefix);

		// ログファイルの出力先を設定する
		virtual void set_log_path(const _tstring& strPath);

		// ログファイルの出力先を取得する
		virtual _tstring get_log_path();

	private:
		_tstring m_ident;
		_tstring m_strBaseFileName;	// 基本になるログファイル名
		_tstring m_strPrefix;		// ログファイルの先頭に付ける名前
		_tstring m_strIdent;		// ログの識別子
		_tstring m_strLogPath;		// ログの出力先(コンストラクタで%TEMP%に設定される)
	};

	//-------------------------------------------------------------------------
	// kjm::util::copyFolder の詳細情報取得クラス
	//-------------------------------------------------------------------------
	struct copyFolderInfo {
		_tstring srcName;	// エラー発生時のコピー元ファイル名
		_tstring dstName;	// エラー発生時のコピー先ファイル名
		int copyFileError;	// kjm::util::copyFile のエラーコード
		DWORD dwError;		// GetLastError のエラーコード

		copyFolderInfo() : srcName(), dstName(), copyFileError(0), dwError(0) {};
	};

	//-------------------------------------------------------------------------
	// 現在の環境およびプラットフォームに関する情報、およびそれらを操作する手段
	// を提供します。
	//-------------------------------------------------------------------------
	class environment {
	private:
		environment() {}
		~environment() {}

	public:
		// 指定した文字列に埋め込まれている各環境変数の名前を、その変数の値を表す文字列で置換し、置換後の文字列全体を返します。
		static _tstring expandEnvironmentVariables(const _tstring& name);
	};

	//-------------------------------------------------------------------------
	// ユーティリティ関数用のstaticクラス
	// [備考]
	// すべての関数は、staticで宣言されます。
	//-------------------------------------------------------------------------
	class util {
	public:
		// バッファからCRC16を計算する
		static unsigned short calcCRC16(unsigned char *p, unsigned int n);

		// ファイルハンドルからCRC16を計算する
		static unsigned short calcCRC16(FILE* fp);

		// ファイル名からCRC16を計算する
		static unsigned short calcCRC16(const _tstring& pfname);

		// 文字列終端の改行を取り除く
		static _tstring& chomp(_tstring& s);

		// CopyFile APIと同様の動きをする関数(エラー箇所を詳細に特定したいときに使用)
		// [戻り値]  1   正常終了(コピー先の読み込み属性を落とした)
		// [戻り値]  0   正常終了
		// [戻り値] -1   コピー元ファイルのオープンに失敗
		// [戻り値] -2   コピー先ファイルのオープンに失敗
		// [戻り値] -3   コピー元ファイルの読み込みに失敗
		// [戻り値] -4   コピー先ファイルの書き込みに失敗
		// [戻り値] -5   コピー先ファイルに指定サイズ書き込みできない
		// [戻り値] -6   コピー元ファイルのタイムスタンプ取得失敗
		// [戻り値] -7   コピー先ファイルのタイムスタンプ更新失敗
		// [戻り値] -8   コピー元ファイルを閉じるのに失敗
		// [戻り値] -9   コピー先ファイルを閉じるのに失敗
		// [戻り値] -10  コピー元ファイルの属性取得失敗
		// [戻り値] -11  コピー先ファイルの属性更新失敗
		// [戻り値] -12  コピー先ファイルの属性取得失敗
		// [戻り値] -13  コピー先ファイルの読み込み属性リセット失敗
		static int copyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists, BOOL bFailIfReadonly=TRUE);

		// コピー処理は CopyFile API を使う。
		// コピー先の読込属性を落とす機能付き。
		//
		// [戻り値]  1   正常終了(コピー先の読み込み属性を落とした)
		// [戻り値]  0   正常終了
		// [戻り値] -1   CopyFile APIの失敗
		// [戻り値] -12  コピー先ファイルの属性取得失敗
		// [戻り値] -13  コピー先ファイルの読み込み属性リセット失敗
		static int copyFile2(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists, BOOL bFailIfReadonly=TRUE);

		// フォルダをコピーする。
		//
		// pCopyFolderInfo が !NULL の場合、エラー発生時の詳細情報を記録する。
		static int copyFolder(const _tstring& srcFolder, const _tstring& dstFolder, bool (*funcIsExclude)(const _tstring& strFname) = NULL, copyFolderInfo* pCopyFolderInfo = NULL, bool ignoreCopyError = false);

		// ディレクトリを再帰的に作成する(
		static bool createDirectory(const _tstring& strPathName);

		// Windowsをシャットダウンさせる
		static int exitWindows();

		// 環境変数を展開する
		static DWORD expandEnvironmentStrings(LPCTSTR lpSrc, std::vector<TCHAR>& rDst);
		//static DWORD expandEnvironmentStrings();

		// システム標準のエラーメッセージを作成
		static _tstring formatMessageBySystem(DWORD dwErr);

		// 汎用的なフォーマットメッセージの処理
		static DWORD formatMessage( DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, std::vector<TCHAR>& buffer, va_list* Arguments );

		// GetComputerNameEx関数(vc6用)
		static BOOL __GetComputerNameEx(COMPUTER_NAME_FORMAT NameType, LPTSTR lpBuffer, LPDWORD nSize);

		// stringを返すGetComputerNameEx関数
		// [例外] throw ::GetLastError();
		static _tstring getComputerNameEx(COMPUTER_NAME_FORMAT NameType);

		// stringを返すGetDlgItemText関数
		// [引数] hDlg        ダイアログボックスのハンドル
		// [引数] nIDDlgItem  コントロールの識別子
		// [例外] throw ::GetLastError();
		static _tstring getDlgItemText(HWND hDlg, int nIDDlgItem);

		static _tstring getEnvironmentVariable(LPCTSTR lpName);

		// 指定ファイルのサイズを返す(GetFileSize APIのラッパー)
		// [引数]  lpszFileName  ファイル名
		// [戻り値] 64bitのファイルサイズ
		static __int64 getFileSize(LPCTSTR lpszFileName);

		// ファイルのバージョンリソースを取得
		static _tstring getFileVersion(const _tstring &strFileName);

		// カレントモジュール(実行ファイル)のバージョンリソースを取得
		static _tstring getFileVersion() { return kjm::util::getFileVersion(kjm::util::getModuleFileName()); }

		// Win32API GetModuleFileName を安全に使う関数
		static _tstring getModuleFileName(HMODULE hModule = NULL);

		// GetPrivateProfileString関数のラッパー
		static _tstring getPrivateProfileString(LPCTSTR lpSectionName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPCTSTR lpFileName);

		// テンポラリファイル用のディレクトリのパスを取得します。
		static _tstring getTempPath() {
			std::vector<TCHAR> work(MAX_PATH);
			GetTempPath(work.size(), &work[0]);
			return &work[0];
		}

		// LoadString関数のラッパー
		static _tstring load_string(UINT uID, HINSTANCE hInstance = NULL);

		// ワークエリアの中心にウィンドウを移動する
		static BOOL moveCenterWorkarea(HWND hwnd);

		// PathRenameExtensionのラッパー
		static _tstring& pathRenameExtension(_tstring& strPath, LPCTSTR pszExt);

		// ディレクトリを再帰的に削除する
		static bool removeFolder(LPCTSTR pszPath);

		// SetComputerNameEx関数(vc6用)
		static BOOL __SetComputerNameEx(COMPUTER_NAME_FORMAT NameType, LPCTSTR lpBuffer);

		// SetLayeredWindowAttributes関数(vc6用)
		static BOOL setLayeredWindowAttributes(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);

		// 文字列を置換する。VBのStrReplaceのような関数。
		static _tstring strReplace(const _tstring& src, const _tstring& strFind, const _tstring& strReplace);

		// Unicode 文字列を Ansi 文字列に変換
		static std::string toAnsi(const std::wstring& src);

		// Ansi 文字列を Unicode 文字列に変換
		static std::wstring toUnicode(const std::string& src);

		// 数値を human readable format な文字列にする
		static _tstring toHumanReadable(__int64 value);

		// 文字列を小文字化する
		static _tstring toLower(const _tstring& src);

		// 文字列を大文字化する
		static _tstring toUpper(const _tstring& src);

		// 安全な vsprintf
		static int vsprintf_vec(std::vector<TCHAR>& buf, LPCTSTR format, va_list argptr);

		// 安全な sprintf 
		//
		// [引数]
		// 一番目の引数が、vector<TCHAR>への参照になっている以外、
		// sprintf関数と同じです。
		static int sprintf_vec(std::vector<TCHAR>& buf, LPCTSTR format, ...);

		// 内部トレースのためのログオブジェクトを指定する
		static void set_logObj(kjm::log* pLogObj);

		//---------------------------------------------------------------------
		// pszPathで指定したディレクトリ内の、現在日時からnDayより古いファイル
		// を削除します。
		//
		// 削除対象は、pszPath直下のファイルだけです。
		//---------------------------------------------------------------------
		static void cleanupDir(LPCTSTR pszPath, int nDay);

		// UNICODE文字列をShift JIS文字列に変換
		static std::string convertToAscii(const std::wstring& src);

		// Shift JIS文字列をUNICODE文字列に変換
		static std::wstring convertToUnicode(const std::string& src);

	private:
		util() {};
		~util() {};
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
	public:
		// デフォルトコンストラクタ
		dateTime(kjm::dateTimeKind kind = kjm::unspecified) : m_kind(kind) { memset(&m_ft, 0, sizeof(&m_ft)); }; 
		
		/// コピーコンストラクタ
		dateTime(const kjm::dateTime& src) : m_kind(src.m_kind), m_ft(src.m_ft) {}

		// time_t で初期化するコンストラクタ
		dateTime(time_t t, kjm::dateTimeKind kind = kjm::unspecified) : m_kind(kind) { *(__int64*)&m_ft = (__int64)t * FT_1SEC + FT_19700101; };

		// FILETIME で初期化するコンストラクタ
		dateTime(FILETIME ft, kjm::dateTimeKind kind = kjm::unspecified) : m_ft(ft), m_kind(kind) {};

		// SYSTEMTIME で初期化するコンストラクタ
		dateTime(const SYSTEMTIME& st, kjm::dateTimeKind kind = kjm::unspecified) : m_kind(kind) { ::SystemTimeToFileTime(&st, &m_ft); }
		
		// 日時を表す文字列文字列(yyyy/mm/dd hh:mm:ss)で初期化するコンストラクタ
		dateTime(LPCTSTR time_text, kjm::dateTimeKind kind = kjm::unspecified) : m_kind(kind) { parse(time_text); };
		
		// デストラクタ
		virtual ~dateTime() {};

		// 保持する日付を、ctime 関数で文字列にして返す。
		_tstring ctime() {
			TCHAR buffer[32];
			time_t t = getTimeT();
			::_tctime_s(buffer, _countof(buffer), &t);
			return util::chomp(_tstring(buffer));
		}

		/// UTCをローカルタイムに変換する
		dateTime &toLocalTime() {
			if (this->m_kind == kjm::local) return *this;
			this->m_kind = kjm::local;
			FileTimeToLocalFileTime(&m_ft, &m_ft);
			return *this;
		}

		/// <summary>UTC に変換する</summary>
		dateTime& toUniversalTime() {
			if (this->m_kind == kjm::utc) return *this;
			this->m_kind = kjm::utc;
			::LocalFileTimeToFileTime(&m_ft, &m_ft);
			return *this;
		}

		// 文字列に変換
		virtual _tstring toString() const;

		// 時間を文字列(YYYYMMDD)に変換
		virtual _tstring toString_yyyymmdd() const;

		// 日時を持っていないかどうか調べる
		bool isEmpty() { return (getTime() == 0); };

		// 年を返す
		int year() const {
			SYSTEMTIME st;
			FileTimeToSystemTime(&m_ft, &st);
			return st.wYear;
		}
		
		// 月を返す
		int month() const {
			SYSTEMTIME st;
			FileTimeToSystemTime(&m_ft, &st);
			return st.wMonth;
		}
		
		// 日を返す
		int date() const {
			SYSTEMTIME st;
			FileTimeToSystemTime(&m_ft, &st);
			return st.wDay;
		}
		
		// 時を返す
		int hour() const {
			SYSTEMTIME st;
			FileTimeToSystemTime(&m_ft, &st);
			return st.wHour;
		}
		
		// 分を返す
		int minute() const {
			SYSTEMTIME st;
			FileTimeToSystemTime(&m_ft, &st);
			return st.wMinute;
		}
		
		// 秒を返す
		int second() const {
			SYSTEMTIME st;
			FileTimeToSystemTime(&m_ft, &st);
			return st.wSecond;
		}
		
		// 曜日を返す
		int weekday() const {
			SYSTEMTIME st;
			FileTimeToSystemTime(&m_ft, &st);
			return st.wDayOfWeek;
		}

		// 秒を追加する
		dateTime& addSecond(int sec) {
			*(__int64*)&m_ft *= (sec * FT_1SEC);
			return *this;
		}

		// 現在時間をtime_tで返す
		time_t getTimeT() const { return getTime() / FT_1SEC - FT_19700101_SEC; };
	
		// 時間をtime_tで設定する
		void setTimeT(time_t value) { setTime(value * FT_19700101); };
		
		// 現在時刻を__int64で返す
		__int64 getTime() const { return *(__int64*)&m_ft; };

		// 時間を__int64で設定する
		void setTime(__int64 value) { *(__int64*)&m_ft = value; };

		// 時間をフォーマットして文字列にする
		_tstring strftime(LPCTSTR format);

		// 日時を表す文字列文字列(yyyy/mm/dd hh:mm:ss)でオブジェクトを初期化する
		void parse(LPCTSTR time_text);

		// 代入演算子(time_t型)
		void operator=(time_t t) { setTimeT(t); };
		
		// 代入演算子(日時を表す文字列(yyyy/mm/dd hh:mm:ss))
		void operator=(LPCTSTR time_text) { parse(time_text); };

		// 時間差を計算する
		double operator-(const dateTime& rhs) const { time_t t = getTimeT(); return difftime(t, rhs.getTimeT()); };
		
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

		// ファイルに保存する
		bool save(FILE* fp);
		
		// ファイルから読み込む
		bool load(FILE* fp);

	public:
		/// このインスタンスが表す時刻の種類 (現地時刻、世界協定時刻 (UTC)、または、そのどちらでもない) を示す値を取得します。
		kjm::dateTimeKind get_kind() { return this->m_kind; }

	public:

		// 現在日時を保持するインスタンスを返す。
		static dateTime getCurrentTime() {
			return dateTime(::time(NULL), kjm::utc);
		};

		// 現在のローカル日時を取得する
		static dateTime getLocalTime() {
			SYSTEMTIME st;
			::GetLocalTime(&st);
			return dateTime(st, kjm::local);
		}

		// 現在のUTC日時を取得する
		static dateTime getSystemTime() {
			SYSTEMTIME st;
			::GetSystemTime(&st);
			return dateTime(st, kjm::utc);
		}

	private:
		///time_t m_time;
		FILETIME m_ft;
		kjm::dateTimeKind m_kind;
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
		int getArgListCount() const { return m_argList.size(); };

		// オプション一覧を取得する
		std::vector<_tstring> getOptList() const;

		// オプションがあったかどうか調べる
		bool hasKey(const _tstring& key) const { return (m_optList.find(key) != m_optList.end()); }

		// オプションの引数を取得する
		_tstring getOptArg(const _tstring& key) const;

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
		// イベントログのレコード番号を返す
		DWORD RecordNumber() { return getRaw()->RecordNumber; };
		
		// イベントログの作成日時を返す
		dateTime TimeGenerated() { return dateTime(getRaw()->TimeGenerated, kjm::utc); };
		
		// イベントログのカテゴリを返す
		WORD EventCategory() { return getRaw()->EventCategory; };
		
		// イベントログのカテゴリ文字列を返す
		_tstring EventCategoryText() { return DispMessage(get_SourceName().c_str(), _T("CategoryMessageFile"), NULL, EventCategory()); };
		
		// イベントログのID文字列を返す
		_tstring EventIDText() {
			std::vector<TCHAR*> Args;
			GetArgs(getRaw(), Args);

			return DispMessage(get_SourceName().c_str(), _T("EventMessageFile"), reinterpret_cast<TCHAR **>(&Args[0]), get_EventID());
		};
		
		// イベントログのデータ長を返す
		DWORD DataLength() { return getRaw()->DataLength; };
		
		// イベントログデータへのオフセットを返す
		DWORD DataOffset() { return getRaw()->DataOffset; };
		
		// イベントログのコンピュータ名を取得
		_tstring ComputerName();
		
		// イベントログのデータを取得
		std::vector<BYTE> Data();

		// イベントの日付時刻を取得
		dateTime get_TimeWritten() { return dateTime(getRaw()->TimeWritten, kjm::utc); };

		// イベントタイプ(種類)を取得
		// @retval EVENTLOG_ERROR_TYPE, EVENTLOG_WARNING_TYPE, EVENTLOG_INFORMATION_TYPE, EVENTLOG_AUDIT_SUCCESS, EVENTLOG_AUDIT_FAILURE のどれか
		WORD get_EventType() { return getRaw()->EventType; };

		// イベントソースを取得
		_tstring get_SourceName() { return ((LPTSTR)&m_raw_record[0] + sizeof(EVENTLOGRECORD)); };

		// イベントIDを取得
		DWORD get_EventID() {
			EVENTLOGRECORD* p = getRaw();
			return p->EventID;
		};

	public:
		// デフォルトコンストラクタ
		eventLogRecord() {};

		// 初期化付きコンストラクタ
		eventLogRecord(LPCTSTR lpSourceName, const EVENTLOGRECORD* src);

		// コピーコンストラクタ
		eventLogRecord( const eventLogRecord& src )
			: m_source_name( src.m_source_name ), m_raw_record( src.m_raw_record ) {};
			
		// デストラクタ
		virtual ~eventLogRecord() {};

		// 代入演算子
		kjm::eventLogRecord& operator= ( const kjm::eventLogRecord& src );

	private:
		_tstring m_source_name;		// イベントソース
		std::vector<BYTE> m_raw_record;	// 生情報
		
		// 生情報をキャストする
		EVENTLOGRECORD* getRaw() { return (EVENTLOGRECORD*)&m_raw_record[0]; };

		_tstring DispMessage(LPCTSTR SourceName, LPCTSTR EntryName, TCHAR **Args, DWORD MessageId);

		int GetArgs(const EVENTLOGRECORD *pBuf, std::vector<TCHAR*>& args );

		BOOL GetModuleNameFromSourceName(LPCTSTR SourceName, LPCTSTR EntryName, std::vector<TCHAR>& ExpandedName);
	};

	//-----------------------------------------------------------------
	// イベントログを管理するクラス
	//-----------------------------------------------------------------
	class eventLog {
	public:
		// デフォルトコンストラクタ
		eventLog() : m_hEventLog(NULL) {};

		// デストラクタ
		virtual ~eventLog() { closeEventLog(); };

	public:
		// イベントログを開く(OpenEventLog API参照)
		//
		// lpSourceNameには通常、"Application", "Security", "System"のどれかを指定する。
		BOOL openEventLog(LPCTSTR lpUNCServerName, LPCTSTR lpSourceName);

		// バックアップイベントログを開く
		BOOL OpenBackupEventLog( LPCTSTR lpUNCServerName, LPCTSTR lpSourceName, LPCTSTR lpFileName );

		// イベントログを閉じる
		BOOL closeEventLog();

		// イベントを1レコード読み込む
		BOOL readEventLog(DWORD dwReadFlags, DWORD dwRecordOffset, eventLogRecord& record);
		
		// イベントログをバックアップする
		BOOL BackupEventLog( LPCTSTR lpBackupFileName ) {
			return ::BackupEventLog( m_hEventLog, lpBackupFileName );
		};

	private:
		// コピーコンストラクタ(コピー禁止)
		eventLog(const eventLog& src) {};

		// 代入演算子(代入禁止)
		eventLog& operator=(const eventLog& rhs) { return *this; };

	private:
		HANDLE m_hEventLog;
		_tstring m_uncServerName;
		_tstring m_sourceName;
		_tstring m_fileName;
	};

	//-------------------------------------------------------------------------
	// Path関連関数用スタティッククラス
	//-------------------------------------------------------------------------
	class path {
	public:
		// パスをくっつける
		static _tstring append(const _tstring& strPath, const _tstring& strMore) {
			TCHAR szTemp[MAX_PATH];
			PathAppend(lstrcpy(szTemp, strPath.c_str()), strMore.c_str());
			return szTemp;
		}

		// PathRemoveExtension 関数のラッパー
		static _tstring removeExtension(const _tstring& strPath) {
			TCHAR szTemp[MAX_PATH];
			PathRemoveExtension(lstrcpy(szTemp, strPath.c_str()));
			return szTemp;
		}

		// フルパスからファイル名を取り除く
		static _tstring removeFileSpec(const _tstring& strPath) {
			TCHAR szTemp[MAX_PATH];
			PathRemoveFileSpec(lstrcpy(szTemp, strPath.c_str()));
			return szTemp;
		}

		// フルパスからファイル名を変更する
		static _tstring renameFileSpec(const _tstring& strPath, const _tstring& strFileSpec) {
			return path::append(path::removeFileSpec(strPath), strFileSpec);
		}

		// 拡張子を変更する
		static _tstring renameExtension(const _tstring& pszPath, const _tstring& pszExt) {
			TCHAR szTemp[MAX_PATH];
			PathRenameExtension(lstrcpy(szTemp, pszPath.c_str()), pszExt.c_str());
			return szTemp;
		}

		// ファイルが存在するかどうか調べる
		static bool fileExists(const _tstring& strPath) {
			return (PathFileExists(strPath.c_str()) != FALSE);
		}

		// フルパスから、ファイル名を取り出す
		static _tstring findFileName(const _tstring& strPath) {
			return PathFindFileName(strPath.c_str());
		}

		// ディレクトリかどうか調べる
		static bool isDirectory(const _tstring& strPath) {
			return (PathIsDirectory(strPath.c_str()) != FALSE);
		}

	private:
		path() {};
		~path() {};
	};

	//-------------------------------------------------------------------------
	// ファイル検索をサポートするクラス
	//-------------------------------------------------------------------------
	class findFile {
	public:
		// デフォルトコンストラクタ
		findFile() : m_hFind(INVALID_HANDLE_VALUE), m_strWildcard(_T("")) {};

		// デストラクタ
		virtual ~findFile() { findClose(); };

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
		};

		// 見つかったファイルが、ディレクトリかどうか調べる
		virtual bool isDirectory() {
			return ((m_findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
		};

		// 見つかったファイルが、読み込み専用かどうか調べる
		virtual bool isReadonly() {
			return ((m_findData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0);
		}

		// 見つかったファイルのファイル名部分を取得
		virtual _tstring getFileName() { return m_findData.cFileName; };

		// 見つかったファイルのフルパス名を取得
		virtual _tstring getFullName() { return path::renameFileSpec(m_strWildcard, m_findData.cFileName); };

		// 見つかったファイルの属性を返す
		virtual DWORD getFileAttributes() { return m_findData.dwFileAttributes; }

		// 見つかったファイルのサイズを取得
		virtual __int64 getFileSize() {
			LARGE_INTEGER li = {m_findData.nFileSizeLow, m_findData.nFileSizeHigh};
			return li.QuadPart;
		};

		dateTime getLastWriteTime() const { return dateTime(m_findData.ftLastWriteTime, kjm::utc); }

		// 見つかったファイルの全情報を WIN32_FIND_DATA 型にコピーする
		virtual void copyWin32FindData(WIN32_FIND_DATA& rBuf) { rBuf = m_findData; }

	private:
		_tstring m_strWildcard;

		HANDLE m_hFind;

		WIN32_FIND_DATA m_findData;
	};

	enum fileAttributes {
		readOnly = FILE_ATTRIBUTE_READONLY,						// ReadOnly = 1,
		hidden = FILE_ATTRIBUTE_HIDDEN,							// Hidden = 2,
		system = FILE_ATTRIBUTE_SYSTEM,							// System = 4,
		directory = FILE_ATTRIBUTE_DIRECTORY,					// Directory = 16,
		archive = FILE_ATTRIBUTE_ARCHIVE,						// Archive = 32,
		device = FILE_ATTRIBUTE_DEVICE,							// Device = 64,
		normal = FILE_ATTRIBUTE_NORMAL,							// Normal = 128,
		temporary = FILE_ATTRIBUTE_TEMPORARY,					// Temporary = 256,
		sparseFile = FILE_ATTRIBUTE_SPARSE_FILE,				// SparseFile = 512,
		reparsePoint = FILE_ATTRIBUTE_REPARSE_POINT,			// ReparsePoint = 1024,
		compressed = FILE_ATTRIBUTE_COMPRESSED,					// Compressed = 2048,
		offline = FILE_ATTRIBUTE_OFFLINE,						// Offline = 4096,
		notContentIndexed = FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,	// NotContentIndexed = 8192,
		encrypted = FILE_ATTRIBUTE_ENCRYPTED,					// Encrypted = 16384,
	};


	class file {
	private:
		file() {}
		~file() {}

	public:
		// ファイルの属性を取得します。
		static kjm::fileAttributes getAttributes(const _tstring &path) { return (kjm::fileAttributes)::GetFileAttributes(path.c_str()); }

		static void setAttributes(const _tstring& path, kjm::fileAttributes fa) { ::SetFileAttributes(path.c_str(), fa); }
	};

	//-------------------------------------------------------------------------
	// ファイル を表現するクラス
	//-------------------------------------------------------------------------
	class fileInfo {
	public:
		// デフォルトコンストラクタ
		fileInfo() : m_hasInfo(false) {};

		// コピーコンストラクタ
		fileInfo(const fileInfo& src) : m_fullPath(src.m_fullPath), m_lastWriteTime(src.m_lastWriteTime), m_fileSize(src.m_fileSize), m_dwFileAttributes(src.m_dwFileAttributes), m_hasInfo(src.m_hasInfo) {};

		// WIN32_FIND_DATA で初期化するコンストラクタ
		fileInfo(const _tstring& strParentPath, const WIN32_FIND_DATA& rFindData) {
			m_fullPath = path::append(strParentPath, rFindData.cFileName);
			m_lastWriteTime = dateTime(rFindData.ftLastWriteTime, kjm::utc);

			LARGE_INTEGER li = {rFindData.nFileSizeLow, rFindData.nFileSizeHigh};
			m_fileSize = li.QuadPart;

			m_dwFileAttributes = rFindData.dwFileAttributes;

			m_hasInfo = true;	// [情報を持っている]で、初期化
		};

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
		};

		// デストラクタ
		virtual ~fileInfo() {};

		// 代入演算子
		fileInfo& operator =(const fileInfo& rhs) {
			if (this != &rhs) m_fullPath = rhs.m_fullPath;
			return *this;
		};

		/// ファイルが存在するかどうかを示す値を取得します。
		bool exists() const { return kjm::path::fileExists(this->m_fullPath); }

		/// 絶対パスを取得
		_tstring fullName() const { return m_fullPath; };

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
		bool isArchive() {
			assert(m_hasInfo != false);
			return ((m_dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0);
		}

		// ファイルが「隠しファイル」かどうか調べる
		bool isHidden() {
			assert(m_hasInfo != false);
			return ((m_dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0);
		}

		// ファイルが「読み取り専用」かどうか調べる
		bool isReadonly() {
			assert(m_hasInfo != false);
			return ((m_dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0);
		}

		// ファイルが「システム」かどうか調べる
		bool isSystem() {
			assert(m_hasInfo != false);
			return ((m_dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0);
		}

		// ファイルのバージョン情報を返す
		_tstring getFileVersion() { return kjm::util::getFileVersion(m_fullPath); };

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

	//-------------------------------------------------------------------------
	// ディレクトリ を表現するクラス
	//-------------------------------------------------------------------------
	class folderInfo {
	public:
		// デフォルトコンストラクタ
		folderInfo() {};

		// コピーコンストラクタ
		folderInfo(const folderInfo& src) : m_fullPath(src.m_fullPath) {};

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
		_tstring getFileName() { return path::findFileName(m_fullPath); };

		// ディレクトリ直下のファイル一覧を取得
		std::vector<fileInfo> getFiles(const _tstring& strWC = _T("*.*"));

		// ディレクトリ直下のディレクトリ一覧を取得
		std::vector<folderInfo> getSubFolders();

		// ディレクトリのフルパスを返す
		_tstring getFullPath() { return m_fullPath; };

	public:
		_tstring m_fullPath;	// ディレクトリのフルパス
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
		iniFile(const _tstring& szFileName) : m_fileName(szFileName) {};

		// デストラクタ
		virtual ~iniFile() {};

		// 代入演算子
		kjm::iniFile& operator=(const iniFile& rhs) {
			if (this == &rhs) return *this;
			this->m_fileName = rhs.m_fileName;
			return *this;
		};

		// セクション一覧を取得
		std::vector<_tstring> getSectionNames() const;

		// セクション内のキー一覧を取得
		std::vector<_tstring> getKeyNames(const _tstring& strSectionName) const;

		// iniファイルから値を取得
		_tstring getString(const _tstring& strSection, const _tstring& strKey, const _tstring& strDefault, BOOL* pfUseDefault = NULL) const;

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
	};

	//-------------------------------------------------------------------------
	// ライブラリモジュールを管理するクラス
	//-------------------------------------------------------------------------
	class library {
	private:
		HINSTANCE m_hLibModule;

	public:
		// 管理しているインスタンスのハンドルを取得
		HINSTANCE getInstance() { return m_hLibModule; };

	public:
		// コンストラクタ
		library() : m_hLibModule( NULL ) {};
		
		// デストラクタ
		virtual ~library() { FreeLibrary(); };

	public:
		// ライブラリをロードする
		BOOL LoadLibraryEx( LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags );
		
		// ライブラリを解放する
		BOOL FreeLibrary();
	};

	//-------------------------------------------------------------------------
	// レジストリを管理するクラス
	//-------------------------------------------------------------------------
	class registry {
	private:
		HKEY m_hKey;

	public:
		// ハンドルを返す
		HKEY getKey() { return m_hKey; };

	public:
		// デフォルトコンストラクタ
		registry() : m_hKey( NULL ) {};
		
		// デストラクタ
		virtual ~registry() { RegCloseKey(); };

	public:
		// キーを開く
		LONG RegOpenKeyEx( HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired );
		
		// キーを閉じる
		LONG RegCloseKey();
		
		// キーの値を取得
		LONG RegQueryValueEx( LPCTSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, std::vector<BYTE>& data );
	};

	// ディレクトリやサブディレクトリを通じて、作成、移動、および列挙するための静的メソッドを公開します。このクラスは継承できません。
	class directory {
	public:
		// 指定したパスがディスク上の既存のディレクトリを参照しているかどうかを確認します。
		static bool exists(const _tstring& path) { return (::PathFileExists(path.c_str()) != FALSE); }

	private:
		directory() {};
		~directory() {};
	};

	//-------------------------------------------------------------------------
	// スレッド管理クラス
	//-------------------------------------------------------------------------
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
#if defined(_MT)
	class thread {
	private:
		bool m_loop;
		HANDLE m_hThread;
		CRITICAL_SECTION* m_cs;

	public:
		thread() : m_hThread( NULL ), m_cs(NULL), m_loop(false) {
		};

		virtual ~thread() {
			close();
		};

		void close() {
			if (m_hThread != NULL) {
				CloseHandle(m_hThread);
				m_hThread = NULL;
			}
		};

		void lock() { if (m_cs) EnterCriticalSection( m_cs ); };
		void unlock() { if (m_cs) LeaveCriticalSection( m_cs ); };

		// ロックに使用するクリティカルセクションを設定する
		void setCriticalSection(CRITICAL_SECTION* pcs) { m_cs = pcs; };

		// スレッドが動作中かどうか調べる
		bool still_active() { 
			DWORD code;
			GetExitCodeThread( m_hThread, &code );
			return ( code == STILL_ACTIVE );
		};

		// スレッドを起動する。
		void run() {
			m_loop = true;
			m_hThread = ( HANDLE )_beginthreadex( NULL, 0, thread_func, this, 0, NULL );
		};

		// スレッドを終了させる。派生クラスで定義する。
		// 終了方法は、worker method の実装に依存する。
		virtual void end() {
			m_loop = false;
		};

		// スレッドの一時停止
		DWORD suspend() {
			return SuspendThread( m_hThread );
		};

		// スレッドの再開
		DWORD resume() {
			return ResumeThread( m_hThread );
		};

		// スレッドに呼び出される関数。派生クラスで定義する。
		// end method の呼出で終了できるように実装する。
		virtual unsigned worker() = 0;

		// スレッドの終了を待つ。
		DWORD wait(DWORD timeout=INFINITE)
			{ return WaitForSingleObject(m_hThread, timeout); }

		bool still_loop() {
			return m_loop;
		};

		/*!
		 * 共通のスレッド関数。
		 * argsには、threadクラスから派生したクラスオブジェクトへの
		 * ポインタを受け取る。p->worker() を実行するだけ。
		 */
		static unsigned __stdcall thread_func(void* args) {
			return ((thread*)args)->worker();
		}
	};
#endif

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

	class process {
	public:
		process();
		virtual ~process() { close(); };

		virtual BOOL run(LPCTSTR pCommandLine);

		virtual DWORD wait(DWORD dwTimeout = INFINITE) {
			return WaitForSingleObject(m_pi.hProcess, dwTimeout);
		};

		virtual void close();

	private:
		STARTUPINFO m_si;
		PROCESS_INFORMATION m_pi;
	};


#define ENCODE_SHIFT_JIS			932				// shift_jis	日本語 (シフト JIS)
#define ENCODE_UTF_16				1200			// utf-16		Unicode
#define ENCODE_UTF_16_BIG_ENDIAN	1201			// unicodeFFFE	Unicode (Big-Endian)
#define ENCODE_UTF_32				12000	
#define ENCODE_UTF_32_BIG_ENDIAN	12001
#define ENCODE_US_ASCII				20127			// us-ascii		US-ASCII
#define ENCODE_JIS					50220			// iso-2022-jp	日本語 (JIS)
#define ENCODE_EUC_JP				51932			// euc-jp		日本語 (EUC)
#define ENCODE_UTF_8				65001			// utf-8		Unicode (UTF-8)
#define ENCODE_UTF_8N				-(ENCODE_UTF_8)	// utf-8(BOM付き)

	// 以下のページのソースコードを元に、C++用に書き換えました。
	// http://www.geocities.jp/gakaibon/tips/csharp2008/charset-check-samplecode4.html
	class encoding {
	private:
		encoding() {}
		~encoding() {}

	public:
		static int GetTextEncoding(const std::vector<BYTE>& bs);

		static bool IsJis(const std::vector<BYTE>& bs);

		static bool IsAscii(const std::vector<BYTE>& bs);

		static bool IsSJIS(const std::vector<BYTE>& bs, int& sjis);

		static bool IsEUC(const std::vector<BYTE>& bs, int& euc);

		static bool IsUTF8(const std::vector<BYTE>& bs, int& utf8, bool& bomFlag);

		static HRESULT convertINetString(
			LPDWORD lpdwMode,
			DWORD dwSrcEncoding,
			DWORD dwDstEncoding,
			LPCSTR lpSrcStr,
			LPINT lpnSrcSize,
			LPBYTE lpDstStr,
			LPINT lpnDstSize
		);
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
	// class mutex の投げる例外用の型
	//-------------------------------------------------------------------------
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

		// 面倒なので、インスタンスの複製は不許可。
		mutex(const mutex &src) {};
		mutex& operator=(const mutex &src) {};

	public:

		// mutex 作成時に同名の mutex が存在したときは、ture を返す
		bool is_exists() { return (m_create_status == ERROR_ALREADY_EXISTS); };

		// mutex作成済みかどうかのテスト
		bool is_open() { return (m_hMutex != NULL); };

	public:

		// コンストラクタとデストラクタ
		mutex() : m_hMutex(NULL), m_create_status(0), m_lock_count(0) {};
		mutex(const _tstring& name) : m_hMutex(NULL), m_create_status(0), m_lock_count(0) {
			create(name);	// 例外は起こりえない。チェックは、is_openでチェック
		};
		virtual ~mutex() { close(); };

		// mutex の作成
		// 
		// arg : name = mutex の名前
		// ret : true = 正常終了、false = 以上終了
		// exp : mutex_error
		// note:
		bool create(const _tstring& name) {
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
		BOOL unlock() {
			if (!m_hMutex)	throw mutex_error(mutex_error::not_initialized);
			
			BOOL ret = ::ReleaseMutex(m_hMutex);
			if (ret) {
				--m_lock_count;
			}

			return ret;
		};
	};

	//-------------------------------------------------------------------------
	// class memfile の投げる例外用の型
	//-------------------------------------------------------------------------
	struct memfile_error {
		enum error_code {
			not_initialized = 1,	// memfile が未初期化
			already_initialized		// memfile が初期化済み
		};
		error_code error;

		memfile_error(error_code e) : error(e) {};
	};

	//-------------------------------------------------------------------------
	// メモリマップドファイル管理クラス
	//
	// note: 作成したハンドルは、デストラクタで破棄される。
	//-------------------------------------------------------------------------
	template<class T> class memfile {
	private:
		HANDLE m_hFile;
		DWORD m_create_status;
		T* m_pt;

		// 面倒なので、インスタンスの複製は不許可。
		memfile<T>(const memfile<T> &src) {};
		memfile<T>& operator=(const memfile<T> &src) {};

	public:
		// memfile作成時に同名のmemfileが存在したときは、ture を返す
		bool is_exists() { return (m_create_status == ERROR_ALREADY_EXISTS); };

		// memfile作成済みかどうかのテスト
		bool is_open() { return (m_hFile != NULL); };

	public:
		// コンストラクタとデストラクタ
		memfile<T>() : m_hFile(NULL), m_create_status(0), m_pt(NULL) {};
		memfile<T>(LPCTSTR name, DWORD size = 0) : m_hFile(NULL), m_create_status(0), m_pt(NULL) {
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
		bool create(LPCTSTR name, DWORD size) {
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
		bool open(LPCTSTR name) {
			// 初期化済みinstanceの時は、例外をthrow
			if (m_hFile) throw memfile_error(memfile_error::already_initialized);

			// メモリマップドファイルを開く
			m_hFile = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, name);
			return (m_hFile != NULL);
		};

		// メモリマップドファイルを閉じる
		//
		// arg : なし
		// ret : なし
		// exp : なし
		// note: 未初期化のinstanceで使用してもok
		void close() {
			if (m_hFile) {
				unmapViewOfFile();

				::CloseHandle(m_hFile);
				m_hFile = NULL;
			}
		};

		// メモリマップドファイルのポインタを取得
		T* mapViewOfFile(DWORD dwDesiredAccess=FILE_MAP_ALL_ACCESS, DWORD dwFileOffsetHigh=0, DWORD dwFileOffsetLow=0, DWORD dwNumberOfBytesToMap=0) {
			return (m_pt = (T*)MapViewOfFile(m_hFile, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap));
		}

		// メモリマップドファイルのポインタを解放
		void unmapViewOfFile() {
			if (m_pt != NULL) {
				UnmapViewOfFile(m_pt);
				m_pt = NULL;
			}
		}

		// メモリマップドファイルからデータを読み込む
		//
		// arg : buffer = 読み込むバッファへのポインタ
		//       offset = 先頭位置へのオフセット
		//       size = 読み込む要素数
		//       pmtx = 排他制御に使用するmutexへのポインタ(NULL可)
		//       timeout = 排他タイムアウト時間
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

	class csvFile {
	public:
		csvFile() {}
		virtual ~csvFile() { if (is_open()) close(); }

	public:
		void open(LPCTSTR fname);
		void close() { m_ifs.close(); }
		int is_open() { return m_ifs.is_open(); }
		bool getline();

		_tstring operator[](int index) { return m_cols.at(index); }
		std::vector<_tstring>::size_type col_count() { return m_cols.size(); };

	private:
		void parse_csv(const _tstring& line);

	private:
		_tifstream m_ifs;
		std::vector<_tstring> m_cols;
	};
}

