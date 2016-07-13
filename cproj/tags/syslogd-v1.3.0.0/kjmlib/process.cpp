//---------------------------------------------------------------------
// class kjm::process の実装
//---------------------------------------------------------------------
#include "kjmlib.h"

//---------------------------------------------------------------------
// デフォルト コンストラクタ
//---------------------------------------------------------------------
kjm::process::process() {
	memset(&m_si, 0, sizeof(m_si));
	memset(&m_pi, 0, sizeof(m_pi));

	m_si.cb = sizeof(m_si);
}

//---------------------------------------------------------------------
// 指定したプロセスを実行する
//---------------------------------------------------------------------
BOOL kjm::process::run(const _tstring& pCommandLine) {
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

//---------------------------------------------------------------------
// プロセスハンドルを閉じる。
// プロセスの終了を待つのは、wait関数なので注意。
//---------------------------------------------------------------------
void kjm::process::close() {
	if (m_pi.hThread != NULL) {
		CloseHandle(m_pi.hThread);
	}

	if (m_pi.hProcess != NULL) {
		CloseHandle(m_pi.hProcess);
	}

	memset(&m_pi, 0, sizeof(m_pi));
}
