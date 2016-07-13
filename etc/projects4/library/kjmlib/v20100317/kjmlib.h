//---------------------------------------------------------------------
// 何度も作ってる codesnipet
//---------------------------------------------------------------------
#pragma once

#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>

#if defined(UNICODE)
typedef std::wstring	_tstring;
#else
typedef std::string		_tstring;
#endif

// 安全な sprintf 
int sprintf_vec(std::vector<char>& buf, const char* format, ...);

// __int64 を出力可能にする ostream ヘルパ
std::ostream& operator<<(std::ostream& os, __int64 i);


namespace kjm {
	//=================================================================
	// 時間クラス
	//=================================================================
#define FT_USEC		((__int64)10)		// これで割ると、1 usec
#define FT_MSEC		(FT_USEC * 1000)	// これで割ると、1 msec
#define FT_SEC		(FT_MSEC * 1000)	// これで割ると、1 sec
#define FT_MINUTE	(FT_SEC * 60)		// これで割ると、1 minute
#define FT_HOUR		(FT_MINUTE * 60)	// これで割ると、1 hour
#define FT_DAY		(FT_HOUR * 24)		// これで割ると、1 day

	class time {
	public:
		// デフォルトコンストラクタ
		time() { memset(&m_ft, 0, sizeof(m_ft)); };

		// 1970/1/1 0:00:00 からの経過時間で初期化するコンストラクタ
		time(DWORD src);

		// デストラクタ
		~time() {};

		// 文字列を単純な YYYY/MM/DD HH:MM:SS 文字列に変換します。
		_tstring to_string();

		// 現在時間を100ns単位で返す
		__int64 get_time() const { return *(__int64*)&m_ft; };

		// UTC時間をローカル時間に変換します。
		time& to_local_time();

		// ローカル時間をUTC時間に変換します。
		time& to_UTC_time();

		__int64 operator-(const time& rhs) { return this->get_time() - rhs.get_time(); };

		// GetLocalTime
		// 現在のローカル日時を取得します。
		static kjm::time get_local_time();

	private:
		FILETIME m_ft;
	};

	//=================================================================
	// イベントログの一レコードを表現するクラス
	//=================================================================
	class eventlogrecord {
	public:
		// デフォルトコンストラクタ
		eventlogrecord() {};

		// デストラクタ
		~eventlogrecord() {};

		// レコードデータをアサインする
		void assign(const std::vector<char>& data);

		// TimeGenerated を取得
		time get_time_generated() { return time(((EVENTLOGRECORD*)m_evlogrec.begin())->TimeGenerated); };

		// TimeWritten を取得
		time get_time_written() { return time(((EVENTLOGRECORD*)m_evlogrec.begin())->TimeWritten); };

		// イベントタイプを取得
		int get_event_type() { return ((EVENTLOGRECORD*)m_evlogrec.begin())->EventType; };

		// イベントソースの名前を取得
		std::string get_source_name();

	private:
		// コピーを禁止する
		eventlogrecord(const eventlogrecord& src) {};
		eventlogrecord& operator=(const eventlogrecord& rhs) { return *this; };

	private:
		std::vector<char> m_evlogrec;
	};

	//=================================================================
	// イベントログにアクセスするクラス
	//=================================================================
	class eventlog {
	public:
		// デフォルトコンストラクタ
		eventlog() : m_hEventLog(NULL) {};

		// デストラクタ
		~eventlog() { close_event_log(); };

		// イベントログを開く
		bool open_event_log(LPCTSTR lpUNCServerName, LPCTSTR lpSourceName);

		// 保存されたイベントログを開く
		bool open_backup_event_log(LPCTSTR lpUNCServerName, LPCTSTR lpFileName);

		// イベントログを閉じる
		bool close_event_log();

		// イベントログを読む
		//
		// [戻り値]
		//  1 レコード読み込んだ。
		//  0 レコードがなかった(EOF)。
		// -1 エラーが発生した。
		int read_event_log(DWORD dwReadFlags, DWORD dwRecordOffset, eventlogrecord& evlogrec);

	private:
		// コピーを禁止する
		eventlog(const eventlog& src) {};
		eventlog& operator=(const eventlog& rhs) { return *this; };

	private:
		HANDLE m_hEventLog;
	};
};
