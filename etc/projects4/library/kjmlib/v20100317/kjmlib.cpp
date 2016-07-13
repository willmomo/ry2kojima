//=====================================================================
// 何度も作ってる codesnipet
//=====================================================================
#include "kjmlib.h"
#include <time.h>
#include <assert.h>


//---------------------------------------------------------------------
// 安全な sprintf 
//
// [引数]
// 一番目の引数が、vector<char>への参照になっている以外、
// sprintf関数と同じです。
//---------------------------------------------------------------------
int sprintf_vec(std::vector<char>& buf, const char* format, ...) {
	va_list marker;

	// バッファを指定されていない時、256バイト割り当てておく
	if (buf.size() == 0) {
		buf.resize(256);
	}

	va_start(marker, format);
	while (1) {

		// vsnprintf で文字列を作成。
		// 不十分なバッファ上にデータを作成している間、ループを繰り返す。
		int n = _vsnprintf(buf.begin(), buf.size(), format, marker);
		if (n != -1 && n != buf.size()) {
			break;
		}

		buf.resize(buf.size() + 256);
	}
	va_end(marker);

	return buf.size();
}


//---------------------------------------------------------------------
// __int64 を出力可能にする ostream ヘルパ
//---------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, __int64 i) {
	TCHAR buf[32];
	_snprintf(buf, sizeof(buf), "%I64d", i);
	os << buf;
	return os;
}


//=====================================================================
// 時間クラス
//=====================================================================

//---------------------------------------------------------------------
// 1970/1/1 0:00:00 からの経過時間で初期化するコンストラクタ
//---------------------------------------------------------------------
kjm::time::time(DWORD src) {
	struct tm* ptm = gmtime((time_t*)&src);
	if (ptm) {
		SYSTEMTIME st;
		memset(&st, 0, sizeof(st));
		st.wYear = ptm->tm_year + 1900;
		st.wMonth = ptm->tm_mon + 1;
		st.wDay = ptm->tm_mday;
		st.wHour = ptm->tm_hour;
		st.wMinute = ptm->tm_min;
		st.wSecond = ptm->tm_sec;

		SystemTimeToFileTime(&st, &m_ft);
	} else {
		memset(&m_ft, 0, sizeof(m_ft));
	}
}

//---------------------------------------------------------------------
// 文字列を単純な YYYY/MM/DD HH:MM:SS 文字列に変換します。
//---------------------------------------------------------------------
_tstring kjm::time::to_string() {
	std::vector<TCHAR> temp_v(80);
	SYSTEMTIME st;

	FileTimeToSystemTime(&m_ft, &st);
	sprintf_vec(temp_v, _T("%04d/%02d/%02d %02d:%02d:%02d"),
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	return temp_v.begin();
}

//---------------------------------------------------------------------
// UTC時間をローカル時間に変換します。
//---------------------------------------------------------------------
kjm::time& kjm::time::to_local_time() {
	FILETIME newFt;

	FileTimeToLocalFileTime(&m_ft, &newFt);

	m_ft = newFt;
	return *this;
}


//---------------------------------------------------------------------
// ローカル時間をUTC時間に変換します。
//---------------------------------------------------------------------
kjm::time& kjm::time::to_UTC_time() {
	FILETIME newFt;

	LocalFileTimeToFileTime(&m_ft, &newFt);

	m_ft = newFt;
	return *this;
}


//---------------------------------------------------------------------
// GetLocalTime
// 現在のローカル日時を取得します。
//---------------------------------------------------------------------
kjm::time kjm::time::get_local_time() {
	kjm::time t;
	SYSTEMTIME st;

	::GetLocalTime(&st);
	::SystemTimeToFileTime(&st, &t.m_ft);

	return t;
}


//=====================================================================
// イベントログにアクセスするクラス
//=====================================================================

//---------------------------------------------------------------------
// イベントログを開く
//---------------------------------------------------------------------
bool kjm::eventlog::open_event_log(LPCTSTR lpUNCServerName, LPCTSTR lpSourceName) {
	// 事前条件チェック
	assert(m_hEventLog == NULL);

	m_hEventLog = OpenEventLog(lpUNCServerName, lpSourceName);
	return (m_hEventLog != NULL);
}


//---------------------------------------------------------------------
// 保存されたイベントログを開く
//---------------------------------------------------------------------
bool kjm::eventlog::open_backup_event_log(LPCTSTR lpUNCServerName, LPCTSTR lpFileName) {
	// 事前条件チェック
	assert(m_hEventLog == NULL);

	m_hEventLog = OpenBackupEventLog(lpUNCServerName, lpFileName);
	return (m_hEventLog != NULL);
}


//---------------------------------------------------------------------
// イベントログを閉じる
//---------------------------------------------------------------------
bool kjm::eventlog::close_event_log() {
	BOOL bRet = TRUE;
	if (m_hEventLog != NULL) {
		bRet = CloseEventLog(m_hEventLog);
		m_hEventLog = NULL;
	}
	return (bRet != FALSE);
}


//---------------------------------------------------------------------
// イベントログを読む
//
// [戻り値]
//  1 レコード読み込んだ。
//  0 レコードがなかった(EOF)。
// -1 エラーが発生した。
//---------------------------------------------------------------------
int kjm::eventlog::read_event_log(DWORD dwReadFlags, DWORD dwRecordOffset, kjm::eventlogrecord& evlogrec) {
	int ret = 0;
	DWORD nBytesRead;
	DWORD nMinNumberOfBytesNeeded;
	std::vector<char> work(1);

	while (1) {
		BOOL bRet = ReadEventLog(m_hEventLog, dwReadFlags, dwRecordOffset, work.begin(), work.size(), &nBytesRead, &nMinNumberOfBytesNeeded);
		if (bRet != FALSE) {
			ret = 1;	// 正常のときは、レコード情報を保存してループ終了
			evlogrec.assign(work);
			break;
		}
		
		DWORD dwErr = GetLastError();
		if (dwErr == ERROR_HANDLE_EOF) {
			ret = 0;	// レコードがないのでループ終了
			break;
		} else if (dwErr != ERROR_INSUFFICIENT_BUFFER) {
			ret = -1;	// バッファが足りない以外のエラーは、異常終了
			break;
		}

		work.resize(nMinNumberOfBytesNeeded);
	}

	return ret;
}


//=====================================================================
// イベントログの一レコードを表現するクラス
//=====================================================================

//---------------------------------------------------------------------
// イベントソースの名前を取得
//---------------------------------------------------------------------
std::string kjm::eventlogrecord::get_source_name() {
	char* pSourceName = &m_evlogrec[sizeof(EVENTLOGRECORD)];
	return pSourceName;
}


//---------------------------------------------------------------------
// レコードデータをアサインする
//---------------------------------------------------------------------
void kjm::eventlogrecord::assign(const std::vector<char>& data) {
	m_evlogrec = data;

	EVENTLOGRECORD* p = (EVENTLOGRECORD*)m_evlogrec.begin();
}
