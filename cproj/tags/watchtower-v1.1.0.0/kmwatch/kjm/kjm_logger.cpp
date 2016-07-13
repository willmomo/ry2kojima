#include "kjm_logger.h"

// コンストラクタ

kjm::logger::logger() {

	std::vector<TCHAR> buf;
	DWORD ret;
	
	// %TEMP% を初期出力フォルダにする
	buf.clear();
	do {
		buf.resize(buf.size() + MAX_PATH);
		ret = ::GetTempPath(buf.size(), &buf[0]);
	} while (ret > buf.size());
	m_path = &buf[0];

	// 実行ファイルの名前を初期ファイル名にする
	buf.clear();
	do {
		buf.resize(buf.size() + MAX_PATH);
		ret = ::GetModuleFileName(NULL, &buf[0], buf.size());
	} while (ret == buf.size());
	LPTSTR p = ::PathFindFileNameA(&buf[0]);
	::PathRemoveExtension(p);
	m_name = p;

	// ファイル名部分に依存する mutex を作成
	TCHAR mutex_name[MAX_PATH];
	lstrcat(lstrcpy(mutex_name, m_name.c_str()), _T(".mutex"));
	m_mutex = CreateMutex(NULL, FALSE, mutex_name);
}

// コピーコンストラクタ

kjm::logger::logger(const kjm::logger& rhs) {
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
	TCHAR fname[MAX_PATH];

	// 出力ファイル名の生成
	PathAppend(lstrcpy(fname, m_path.c_str()), m_name.c_str());
	lstrcat(fname, _T(".log"));

	// ファイルのサイズを取得
	ULARGE_INTEGER size = {0};
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(fname, &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		FindClose(hFind);
		size.HighPart = data.nFileSizeHigh;
		size.LowPart = data.nFileSizeLow;
	}

	// ファイルのサイズが 1M を超えたときはローテーション
	if (size.QuadPart > (1 * 1024 * 1024)) {

		DWORD dwWait = WaitForSingleObject(m_mutex, 200);

		PathAppend(lstrcpy(fname, m_path.c_str()), m_name.c_str());
		TCHAR fname1[MAX_PATH];
		TCHAR fname2[MAX_PATH];
		_stprintf(fname2, _T("%s_9.log"), fname);
		DeleteFile(fname2);
		for (int i = 8; i > 0; i--) {
			_stprintf(fname1, _T("%s_%d.log"), fname, i);
			_stprintf(fname2, _T("%s_%d.log"), fname, i + 1);
			MoveFile(fname1, fname2);
		}
		_stprintf(fname1, _T("%s.log"), fname);
		_stprintf(fname2, _T("%s_1.log"), fname);
		MoveFile(fname1, fname2);

		ReleaseMutex(m_mutex);
	}
}

// ログ出力
//
// level: 0=TRACE 1=DEBUG 2=INFO 3=WARN 4=ERROR 5=FATAL

void kjm::logger::write(int level, LPCTSTR fmt, va_list argptr) {
	const static TCHAR s_loglevel[] = {'T', 'D', 'I', 'W', 'E', 'F'};
	TCHAR fname[MAX_PATH];

	// 現在日時
	SYSTEMTIME st;
	GetLocalTime(&st);

	// 出力ファイル名の生成
	PathAppend(lstrcpy(fname, m_path.c_str()), m_name.c_str());
	lstrcat(fname, _T(".log"));

	// 書込み排他でファイルを開く
	DWORD dwWait = WaitForSingleObject(m_mutex, 200);

	for (int n = 0; n < 10; n++) {
		FILE* fp = _tfsopen(fname, _T("a"), _SH_DENYWR);
		if (fp) {
			_ftprintf(fp, _T("%04d/%02d/%02d %02d:%02d:%02d.%03d [%04x:%04x] %c/ "),
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				GetCurrentProcessId(), GetCurrentThreadId(),
				s_loglevel[level]);
			_vftprintf(fp, fmt, argptr);
			_fputts(_T("\n"), fp);
			fclose(fp);
			break;
		}
		Sleep(50);
	}

	ReleaseMutex(m_mutex);

	rotate();
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
