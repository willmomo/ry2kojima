//---------------------------------------------------------------------------------------
// kjmsub ver.0.2010.7.29
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

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include <cassert>
#include <ctime>

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
	// 共通のログ処理
	//-------------------------------------------------------------------------
	class log {
	public:
		// デフォルトコンストラクタ
		log() : m_ident() {};

		// デストラクタ
		~log() {};

		// エラーログ
		void writeError(LPCTSTR pszFormat, ...);

		// 情報ログ
		void writeInfo(LPCTSTR pszFormat, ...);

		// 全ログ
		void writeAll(LPCTSTR pszFormat, ...);

		// 古いログの削除
		void deleteOldLog();

		// 識別子の取得
		_tstring get_Ident() { return m_ident; };

		// 識別子の設定
		void set_Ident(const _tstring& value) { m_ident = value; };

	private:
		_tstring m_ident;
	};

	//-------------------------------------------------------------------------
	// ユーティリティ関数用のstaticクラス
	// [備考]
	// すべての関数は、staticで宣言されます。
	//-------------------------------------------------------------------------
	class util {
	public:
		// バッファからCRC16を計算する
		static unsigned short kjm::util::calcCRC16(unsigned char *p, unsigned int n);

		// ファイルハンドルからCRC16を計算する
		static unsigned short kjm::util::calcCRC16(FILE* fp);

		// ファイル名からCRC16を計算する
		static unsigned short kjm::util::calcCRC16(const _tstring& pfname);

		// 文字列終端の改行を取り除く
		static _tstring& chomp(_tstring& s);

		// CopyFile APIと同様の動きをする関数(エラー箇所を詳細に特定したいときに使用)
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
		static int copyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists);

		// Windowsをシャットダウンさせる
		static int exitWindows();

		// 環境変数を展開する
		static DWORD expandEnvironmentStrings(LPCTSTR lpSrc, std::vector<TCHAR>& rDst);

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

		// 指定ファイルのサイズを返す(GetFileSize APIのラッパー)
		// [引数]  lpszFileName  ファイル名
		// [戻り値] 64bitのファイルサイズ
		static __int64 getFileSize(LPCTSTR lpszFileName);

		// ファイルのバージョンリソースを取得
		static _tstring getFileVersion(const _tstring &strFileName);

		// Win32API GetModuleFileName を安全に使う関数
		static _tstring getModuleFileName(HMODULE hModule = NULL);

		// ワークエリアの中心にウィンドウを移動する
		static BOOL moveCenterWorkarea(HWND hwnd);

		// SetComputerNameEx関数(vc6用)
		static BOOL __SetComputerNameEx(COMPUTER_NAME_FORMAT NameType, LPCTSTR lpBuffer);

		// PathRenameExtensionのラッパー
		static _tstring& pathRenameExtension(_tstring& strPath, LPCTSTR pszExt);

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
		virtual _tstring toString();

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

	private:
		///time_t m_time;
		FILETIME m_ft;
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

		// 見つかったファイルのファイル名部分を取得
		virtual _tstring getFileName() { return m_findData.cFileName; };

		// 見つかったファイルのフルパス名を取得
		virtual _tstring getFullName() { return path::renameFileSpec(m_strWildcard, m_findData.cFileName); };

		// 見つかったファイルの最終更新日時を取得
		virtual dateTime getLastWriteTime() { return dateTime(m_findData.ftLastWriteTime); };

		// 見つかったファイルのサイズを取得
		virtual __int64 getFileSize() {
			LARGE_INTEGER li = {m_findData.nFileSizeLow, m_findData.nFileSizeHigh};
			return li.QuadPart;
		};

	private:
		_tstring m_strWildcard;

		HANDLE m_hFind;

		WIN32_FIND_DATA m_findData;
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

}

