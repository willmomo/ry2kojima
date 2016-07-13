/*
	ver.0.2015.10.21(1653)
*/

#include "kjm_logger.h"
#include "kjm_util.h"
#include "kjm_path.h"

// コンストラクタ

kjm::logger::logger() {
	m_debug_cnt = m_debug_tim = 0;

	std::vector<TCHAR> buf;
	
	// %TEMP% を初期出力フォルダにする
	m_path = kjm::util::get_TempPath();

	// 実行ファイルの名前を初期ファイル名にする
	m_name = kjm::path::removeExtension(kjm::path::findFileName(kjm::util::get_ModuleFileName()));

	// ファイル名部分に依存する mutex を作成
	TCHAR mutex_name[MAX_PATH];
	lstrcat(lstrcpy(mutex_name, m_name.c_str()), _T(".mutex"));
	m_mutex = CreateMutex(NULL, FALSE, mutex_name);
}

// コピーコンストラクタ

kjm::logger::logger(const kjm::logger& rhs) {
	m_debug_cnt = rhs.m_debug_cnt;
	m_debug_tim = rhs.m_debug_tim;
	m_path = rhs.m_path;
	m_name = rhs.m_name;
	if (rhs.m_mutex != NULL) {
		DuplicateHandle(GetCurrentProcess(), rhs.m_mutex, GetCurrentProcess(), &m_mutex, 0, FALSE, DUPLICATE_SAME_ACCESS);
	} else {
		m_mutex = NULL;
	}
}

// 代入演算子

kjm::logger& kjm::logger::operator=(const logger& rhs) {
	if (this != &rhs) {
		m_debug_cnt = rhs.m_debug_cnt;
		m_debug_tim = rhs.m_debug_tim;
		m_path = rhs.m_path;
		m_name = rhs.m_name;
		if (rhs.m_mutex != NULL) {
			DuplicateHandle(GetCurrentProcess(), rhs.m_mutex, GetCurrentProcess(), &m_mutex, 0, FALSE, DUPLICATE_SAME_ACCESS);
		} else {
			m_mutex = NULL;
		}
	}
	return *this;
}

// デストラクタ

kjm::logger::~logger() {
	if (m_mutex != NULL) CloseHandle(m_mutex);
}

// ログをローテーションする

void kjm::logger::rotate() {
	std::basic_string<TCHAR> fname;

	// 出力ファイル名の生成
	fname = kjm::path::append(m_path, m_name) + _T(".log");

	// ファイルのサイズを取得
	ULARGE_INTEGER size = {0};
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(fname.c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		FindClose(hFind);
		size.HighPart = data.nFileSizeHigh;
		size.LowPart = data.nFileSizeLow;
	}

	// ファイルのサイズが 1M を超えたときはローテーション
	if (size.QuadPart > (1 * 1024 * 1024)) {

		fname = kjm::path::append(m_path, m_name);
		TCHAR fname1[MAX_PATH];
		TCHAR fname2[MAX_PATH];
		_stprintf(fname2, _T("%s.log.9"), fname.c_str());
		DeleteFile(fname2);
		for (int i = 8; i > 0; i--) {
			_stprintf(fname1, _T("%s.log.%d"), fname.c_str(), i);
			_stprintf(fname2, _T("%s.log.%d"), fname.c_str(), i + 1);
			MoveFile(fname1, fname2);
		}
		_stprintf(fname1, _T("%s.log"), fname.c_str());
		_stprintf(fname2, _T("%s.log.1"), fname.c_str());
		BOOL bRet = MoveFile(fname1, fname2);
	}
}

// ログ出力
//
// level: 0=TRACE 1=DEBUG 2=INFO 3=WARN 4=ERROR 5=FATAL

void kjm::logger::write(int level, LPCTSTR fmt, va_list argptr) {
	const static TCHAR s_loglevel[] = {'T', 'D', 'I', 'W', 'E', 'F'};
	std::basic_string<TCHAR> fname;

	// 現在日時
	SYSTEMTIME st;
	GetLocalTime(&st);

	// 出力ファイル名の生成
	fname = kjm::path::append(m_path, m_name) + _T(".log");

	// 書込み排他でファイルを開く
	DWORD dwStart = GetTickCount();
	DWORD dwWait = WaitForSingleObject(m_mutex, INFINITE);
	if (dwWait == WAIT_OBJECT_0 || dwWait == WAIT_ABANDONED) {

		m_debug_cnt++;
		m_debug_tim += (GetTickCount() - dwStart);

		FILE* fp = _tfsopen(fname.c_str(), _T("a"), _SH_DENYNO);
		if (fp) {
			_ftprintf(fp, _T("%04d/%02d/%02d %02d:%02d:%02d.%03d [%04x:%04x] %c/ "),
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				GetCurrentProcessId(), GetCurrentThreadId(),
				s_loglevel[level]);
			_vftprintf(fp, fmt, argptr);
			_fputts(_T("\n"), fp);
			fclose(fp);
		}
		rotate();
		ReleaseMutex(m_mutex);
	} else if (dwWait == WAIT_TIMEOUT) {
		// 今は無限待ちなのでここには来ない
	} else {
		// ここにも来ないはず
	}
}

// ログ出力(fatal)

void kjm::logger::write_f(LPCTSTR fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	write(5, fmt, argptr);
	va_end(argptr);
}

// ログ出力(error)

void kjm::logger::write_e(LPCTSTR fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	write(4, fmt, argptr);
	va_end(argptr);
}

// ログ出力(warn)

void kjm::logger::write_w(LPCTSTR fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	write(3, fmt, argptr);
	va_end(argptr);
}

// ログ出力(info)

void kjm::logger::write_i(LPCTSTR fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	write(2, fmt, argptr);
	va_end(argptr);
}

// ログ出力(debug)

void kjm::logger::write_d(LPCTSTR fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	write(1, fmt, argptr);
	va_end(argptr);
}

// ログ出力(trace)

void kjm::logger::write_t(LPCTSTR fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	write(0, fmt, argptr);
	va_end(argptr);
}

// デバッグ情報の出力

void kjm::logger::print_debug() {
	_tprintf(_T("   count: %u\n"), m_debug_cnt);
	if (m_debug_cnt != 0)
		_tprintf(_T("wait avg: %u ms\n"), m_debug_tim / m_debug_cnt);
}
