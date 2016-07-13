//---------------------------------------------------------------------------------------
// kjmsub ver.0.2010.8.27
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
#include <tlhelp32.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "ws2_32.lib")

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
#define _otstream			std::wostream
#define _otstringstream		std::wostringstream
#else
#define _tstring			std::string
#define _tout				std::cout
#define _otstream			std::ostream
#define _otstringstream		std::ostringstream
#endif

// STL の setlocale(LC_ALL, _T("japan")) 相当処理は、以下の通り。
// std::locale コンストラクタは常に、_MBCS文字列を受け取る。
#define STL_SETLOCALE_JAPAN		std::locale::global(std::locale("japan"))

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
} COMPUTER_NAME_FORMAT ;
#endif

#if _MSC_VER <= 1200
#define WS_EX_LAYERED 0x80000
#define LWA_COLORKEY	1
#define LWA_ALPHA		2
#endif

// FILETIME を扱いやすい時間に変換するための定数
#define FT_1USEC			10
#define FT_1MSEC			(FT_1USEC * 1000)
#define FT_1SEC				(FT_1MSEC * 1000)
#define FT_19700101			(116444736000000000)		// time_t の開始地点を表すFILETIMEの値
#define FT_19700101_SEC		(FT_19700101 / FT_1SEC)

// __int64 を iostream に出力するための関数
#if _MSC_VER < 1500		// 少なくとも、vs2008からは、__int64を受け取るostreamが存在するので、無効にする。
_otstream& operator<<(_otstream& os, __int64 i);
#endif

namespace kjm {
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
	// CPU 使用率を取得するクラス
	//-------------------------------------------------------------------------
	class cpuUsage {
	public:
		// デフォルトコンストラクタ
		cpuUsage() {};

		// デストラクタ
		virtual ~cpuUsage() {};

		// パフォーマンス取得開始
		BOOL StartPerf() {
			m_osvi.dwOSVersionInfoSize = sizeof(m_osvi);
			GetVersionEx(&m_osvi);

			if (m_osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
				return StartPerf95();
			else if (m_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
				return StartPerfNT();

			return FALSE;
		}

		// パフォーマンスの取得終了
		BOOL StopPerf() {
			if (m_osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
				return StopPerf95();
			else if (m_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
				return StopPerfNT();

			return FALSE;
		}

		// パフォーマンスデータの取得
		INT GetCPUUsage() {
			if (m_osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
				return GetCPUUsage95();
			else if (m_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
				return GetCPUUsageNT();

			SetLastError((DWORD)CO_E_WRONGOSFORAPP);	// バージョンエラー

			return -1;
		}

	private:
		// パフォーマンスの取得開始(NT版)(ダミー関数)
		BOOL StartPerfNT() { return TRUE; }

		// パフォーマンスの取得開始(95版)
		BOOL StartPerf95();

		// パフォーマンスの取得終了(NT版)(ダミー関数)
		BOOL StopPerfNT() { return TRUE; }

		// パフォーマンスの取得終了(95版)
		BOOL StopPerf95();

		// パフォーマンスデータの取得用サポート関数(NT版)
		PVOID CounterDataPtr(PPERF_DATA_BLOCK pDataBlock, DWORD dwObjectNameTitleIndex, WCHAR* pszInstanceName, DWORD dwCounterNameTitleIndex);

		// パフォーマンスデータの取得(NT版)
		INT GetCPUUsageNT();

		// パフォーマンスデータの取得(95版)
		INT GetCPUUsage95();

	private:
		OSVERSIONINFO m_osvi;
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
		static bool createDirectory(LPCTSTR strPathName);

		// Windowsをシャットダウンさせる
		// [引数]
		// uFlags  ExitWindowsEx API を参照
		static int exitWindows(UINT uFlags);

		// 環境変数を展開する
		static DWORD expandEnvironmentStrings(LPCTSTR lpSrc, std::vector<TCHAR>& rDst);

		// システム標準のエラーメッセージを作成
		static _tstring formatMessageBySystem(DWORD dwErr);

		// 汎用的なフォーマットメッセージの処理
		static DWORD formatMessage( DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, std::vector<TCHAR>& buffer, va_list* Arguments );

		// ホスト名または、IP文字列からIPアドレスを取得
		static DWORD getAddr(LPCSTR lpName);

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

		// 指定ファイルのサイズを返す(GetFileSize APIのラッパー)
		// [引数]  lpszFileName  ファイル名
		// [戻り値] 64bitのファイルサイズ
		static __int64 getFileSize(LPCTSTR lpszFileName);

		// ファイルのバージョンリソースを取得
		static _tstring getFileVersion(const _tstring &strFileName);

		// ファイルのバージョンリソースを取得(引数なしバージョン)
		static _tstring getFileVersion() { return kjm::util::getFileVersion(kjm::util::getModuleFileName()); }

		// Win32API GetModuleFileName を安全に使う関数
		static _tstring getModuleFileName(HMODULE hModule = NULL);

		// テンポラリファイル用のディレクトリのパスを取得します。
		static _tstring getTempPath() {
			std::vector<TCHAR> work(MAX_PATH);
			GetTempPath(work.size(), &work[0]);
			return &work[0];
		}

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

	private:
		util() {};
		~util() {};
	};

	//-------------------------------------------------------------------------
	// 時間を扱うクラス
	//-------------------------------------------------------------------------
	class dateTime {
	public:
		// デフォルトコンストラクタ
		dateTime() { memset(&m_ft, 0, sizeof(&m_ft)); }; 
		
		// time_t で初期化するコンストラクタ
		dateTime(time_t t) { *(__int64*)&m_ft = (__int64)t * FT_1SEC + FT_19700101; };

		// FILETIME で初期化するコンストラクタ
		dateTime(FILETIME ft) : m_ft(ft) {};

		// SYSTEMTIME で初期化するコンストラクタ
		dateTime(const SYSTEMTIME& st) { ::SystemTimeToFileTime(&st, &m_ft); }
		
		// 日時を表す文字列文字列(yyyy/mm/dd hh:mm:ss)で初期化するコンストラクタ
		dateTime(LPCTSTR time_text) { parse(time_text); };
		
		// デストラクタ
		virtual ~dateTime() {};

		// 保持する日付を、ctime 関数で文字列にして返す。
		_tstring ctime() {
			time_t t = getTimeT();
			return util::chomp(_tstring(::_tctime(&t)));
		};

		// UTCをローカルタイムに変換する
		virtual dateTime &toLocalTime() {
			FileTimeToLocalFileTime(&m_ft, &m_ft);
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
		_tstring strftime(LPCTSTR format) {
			TCHAR buf[512];
			time_t t = getTimeT();
			::_tcsftime(buf, sizeof(buf), format, localtime(&t));
			return _tstring(buf);
		};

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

		// 現在日時を保持するインスタンスを返す。
		static dateTime getCurrentTime() {
			return dateTime(::time(NULL));
		};

		// 現在のローカル日時を取得する
		static dateTime getLocalTime() {
			SYSTEMTIME st;
			::GetLocalTime(&st);
			return dateTime(st);
		}

	private:
		///time_t m_time;
		FILETIME m_ft;
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
		// イベントログのレコード番号を返す
		DWORD RecordNumber() { return getRaw()->RecordNumber; };
		
		// イベントログの作成日時を返す
		dateTime TimeGenerated() { return dateTime( getRaw()->TimeGenerated ); };
		
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
		dateTime get_TimeWritten() { return dateTime( getRaw()->TimeWritten ); };

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

		// 拡張子の変更
		static _tstring renameExtension(const _tstring& strPath, const _tstring& newExt) {
			TCHAR szTemp[MAX_PATH];
			PathRenameExtension(lstrcpy(szTemp, strPath.c_str()), newExt.c_str());
			return szTemp;
		}

		// フルパスからファイル名を変更する
		static _tstring renameFileSpec(const _tstring& strPath, const _tstring& strFileSpec) {
			return path::append(path::removeFileSpec(strPath), strFileSpec);
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

		// 見つかったファイルの最終更新日時を取得
		virtual dateTime getLastWriteTime() { return dateTime(m_findData.ftLastWriteTime); };

		// 見つかったファイルの全情報を WIN32_FIND_DATA 型にコピーする
		virtual void copyWin32FindData(WIN32_FIND_DATA& rBuf) { rBuf = m_findData; }

	private:
		_tstring m_strWildcard;

		HANDLE m_hFind;

		WIN32_FIND_DATA m_findData;
	};

	//-------------------------------------------------------------------------
	// ファイル を表現するクラス
	//-------------------------------------------------------------------------
	class file {
	public:
		// デフォルトコンストラクタ
		file() : m_hasInfo(false) {};

		// コピーコンストラクタ
		file(const file& src) : m_fullPath(src.m_fullPath), m_lastWriteTime(src.m_lastWriteTime), m_fileSize(src.m_fileSize), m_dwFileAttributes(src.m_dwFileAttributes), m_hasInfo(src.m_hasInfo) {};

		// WIN32_FIND_DATA で初期化するコンストラクタ
		file(const _tstring& strParentPath, const WIN32_FIND_DATA& rFindData) {
			m_fullPath = path::append(strParentPath, rFindData.cFileName);
			m_lastWriteTime = dateTime(rFindData.ftLastWriteTime);

			LARGE_INTEGER li = {rFindData.nFileSizeLow, rFindData.nFileSizeHigh};
			m_fileSize = li.QuadPart;

			m_dwFileAttributes = rFindData.dwFileAttributes;

			m_hasInfo = true;	// [情報を持っている]で、初期化
		};

		// ファイル名で初期化するコンストラクタ
		file(const _tstring& strName) : m_fullPath(strName) {
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
		virtual ~file() {};

		// 代入演算子
		file& operator =(const file& rhs) {
			if (this != &rhs) m_fullPath = rhs.m_fullPath;
			return *this;
		};

		// ファイルの有無を返す
		bool exists() { return path::fileExists(m_fullPath); };

		// ファイルのフルパスを返す
		_tstring getFullPath() { return m_fullPath; };

		// ファイル名部分を返す
		_tstring getFileName() { return path::findFileName(m_fullPath); };

		// ファイルの最終更新日時を返す
		dateTime getLastWriteTime() {
			assert(m_hasInfo != false);
			return m_lastWriteTime;
		}

		// ファイルの属性を返す
		DWORD getFileAttributes() {
			assert(m_hasInfo != false);
			return m_dwFileAttributes;
		}

		// ファイルのサイズを返す
		__int64 getFileSize() {
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
	class folder {
	public:
		// デフォルトコンストラクタ
		folder() {};

		// コピーコンストラクタ
		folder(const folder& src) : m_fullPath(src.m_fullPath) {};

		// WIN32_FIND_DATA で初期化するコンストラクタ
		folder(const _tstring& strParentPath, const WIN32_FIND_DATA& rFindData) {
			m_fullPath = path::append(strParentPath, rFindData.cFileName);
		};

		// ディレクトリ名で初期化するコンストラクタ
		folder(const _tstring& strName) : m_fullPath(strName) {};

		// デストラクタ
		virtual ~folder() {};

		// 代入演算子
		folder& operator =(const folder& rhs) {
			if (this != &rhs) m_fullPath = rhs.m_fullPath;
			return *this;
		};

		// ディレクトリ直下のファイル一覧を取得
		std::vector<file> getFiles(const _tstring& strWC = _T("*.*"));

		// ディレクトリ直下のディレクトリ一覧を取得
		std::vector<folder> getSubFolders();

		// ファイル名部分を返す
		_tstring getFileName() { return path::findFileName(m_fullPath); };

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
		thread() : m_hThread( NULL ), m_cs(NULL), m_loop(true) {
		};

		virtual ~thread() {
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
			m_hThread = ( HANDLE )_beginthreadex( NULL, 0, thread_func, this, 0, NULL );
		};

		// スレッドを終了させる。派生クラスで定義する。
		// 終了方法は、worker method の実装に依存する。
		virtual void end() { m_loop = false; };

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
		DWORD wait( DWORD timeout = INFINITE ) {
			DWORD ret = WaitForSingleObject( m_hThread, timeout );
			CloseHandle( m_hThread );
			return ret;
		};

		// ループすべきかどうかを返す
		bool stillLoop() { return m_loop; };

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

	//-------------------------------------------------------------------------
	// ソケットを扱うクラス
	//-------------------------------------------------------------------------
	class wsaSocket {
	public:
		// デフォルトコンストラクタ
		wsaSocket() : m_sock(INVALID_SOCKET) {};

		// デストラクタ
		virtual ~wsaSocket() { closeSocket(); };

		// UDP ソケットの作成
		// 0 正常終了
		// -1 socket エラー
		// -2 bind エラー
		int createUdpSocket(short port = 0) {
			m_sock = socket(AF_INET, SOCK_DGRAM, 0);
			if (m_sock == INVALID_SOCKET) {
				return -1;
			}

			if (port != 0) {
				if (bindPort(port) != 0) {
					closeSocket();
					return -2;
				}
			}

			return 0;
		}

		// ソケットの破棄
		int closeSocket() {
			int ret = 0;
			if (m_sock != INVALID_SOCKET) {
				ret = closesocket(m_sock);
				m_sock = INVALID_SOCKET;
			}
			return ret;
		}

		// 受信タイムアウト時間を設定
		int setRecvTimeout(int timeout) {
			return setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
		}

		// ポートにバインドする
		int bindPort(short port) {
			sockaddr_in myaddr;
			memset(&myaddr, 0, sizeof(myaddr));
			myaddr.sin_family = AF_INET;
			myaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
			myaddr.sin_port = htons(port);

			return bind(m_sock, (sockaddr*)&myaddr, sizeof(myaddr));
		}

		// 簡単受信
		int recvFromEasy(std::vector<char>& buf) {
			char work[512];
			sockaddr_in from;
			int fromlen;

			fromlen = sizeof(from);
			int ret =recvfrom(m_sock, work, sizeof(work) - 1, 0, (sockaddr*)&from, &fromlen);

			if (ret > 0) {
				buf.assign(&work[0], &work[ret]);
			}

			return ret;
		}

		// 簡単送信
		int sendToEasy(const char* dataPtr, int dataLen, const _tstring& host, short port) {
			struct sockaddr_in to;
			memset(&to, 0, sizeof(to));
			to.sin_family = AF_INET;
			to.sin_addr.S_un.S_addr = kjm::util::getAddr(host.c_str());
			to.sin_port = htons(port);

			return sendto(m_sock, dataPtr, dataLen, 0, (sockaddr*)&to, sizeof(to));
		}

		// 簡単送信(文字列版)
		int sendToEasy(const _tstring& data, const _tstring& host, short port) {
			return sendToEasy(data.c_str(), data.size(), host, port);
		}

	public:
		// winsock 2.0 の初期化
		static int startup() {
			WSADATA wsaData;
			return WSAStartup(MAKEWORD(2, 0), &wsaData);
		}

		// wisncok の終了
		static int cleanup() { return WSACleanup(); };

	private:
		SOCKET m_sock;
	};

	//-------------------------------------------------------------------------
	// プロセスの実行と管理を行うクラス
	//-------------------------------------------------------------------------
	class process {
	public:
		// デフォルトコンストラクタ
		process() {
			memset(&m_si, 0, sizeof(m_si));
			memset(&m_pi, 0, sizeof(m_pi));
		};

		// デストラクタ
		virtual ~process() { close(); };

		// プロセス実行
		virtual BOOL run(const _tstring& pCommandLine) {
			memset(&m_si, 0, sizeof(m_si));
			memset(&m_pi, 0, sizeof(m_pi));

			m_si.cb = sizeof(m_si);
			TCHAR temp[MAX_PATH];

			BOOL bRet = CreateProcess(
				NULL,
				lstrcpy(temp, pCommandLine.c_str()),
				NULL,
				NULL,
				FALSE,
				0,
				NULL,
				NULL,
				&m_si,
				&m_pi
			);

			return bRet;
		}

		// プロセス終了を待つ
		virtual DWORD wait(DWORD dwTimeout = INFINITE) {
			return WaitForSingleObject(m_pi.hProcess, dwTimeout);
		};

		// プロセスハンドルを閉じる
		// プロセスの終了を待つのは、wait関数なので注意。
		virtual void close() {
			if (m_pi.hThread != NULL) {
				CloseHandle(m_pi.hThread);
			}

			if (m_pi.hProcess != NULL) {
				CloseHandle(m_pi.hProcess);
			}

			memset(&m_pi, 0, sizeof(m_pi));
		}

	private:
		STARTUPINFO m_si;
		PROCESS_INFORMATION m_pi;
	};

	//-------------------------------------------------------------------------
	// ツールヘルパを扱うクラス
	//-------------------------------------------------------------------------
	class toolHelp {
	public:
		// デフォルトコンストラクタ
		toolHelp() : m_hSnapshot(INVALID_HANDLE_VALUE) {};

		// デストラクタ
		virtual ~toolHelp() { closeHandle(); }

		// スナップショットの作成
		virtual bool createSnapshot() {
			this->m_hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			
			return (m_hSnapshot != INVALID_HANDLE_VALUE);
		}

		// ハンドルを閉じる
		virtual void closeHandle() {
			if (this->m_hSnapshot != INVALID_HANDLE_VALUE) {
				CloseHandle(this->m_hSnapshot);
				this->m_hSnapshot = INVALID_HANDLE_VALUE;
			}
		}

		// プロセスを列挙する(初回)
		virtual bool processFirst() {
			memset(&m_pe, 0, sizeof(m_pe));
			m_pe.dwSize = sizeof(m_pe);

			return (Process32First(m_hSnapshot, &m_pe) != FALSE);
		}

		// プロセスを列挙する(二回目以降)
		virtual bool processNext() {
			return (Process32Next(m_hSnapshot, &m_pe) != FALSE);
		}

		// 実行ファイルの名前を取得
		virtual _tstring getExeFile() {
			return m_pe.szExeFile;
		};

		// プロセスIDを取得
		virtual DWORD getProcessId() {
			return m_pe.th32ProcessID;
		};

	public:
		// プロセスリストを取得する
		static std::vector<PROCESSENTRY32> getProcessList() {
			std::vector<PROCESSENTRY32> result;
			toolHelp th;
			if (th.createSnapshot()) {
				if (th.processFirst()) {
					do {
						result.push_back(th.m_pe);
					} while (th.processNext());
				}
				th.closeHandle();
			}
			return result;
		};

	private:
		HANDLE m_hSnapshot;
		PROCESSENTRY32 m_pe;
	};
}

