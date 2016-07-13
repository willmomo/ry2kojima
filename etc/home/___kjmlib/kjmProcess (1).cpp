//---------------------------------------------------------------------
// プロセスを扱うクラス
//
// 2009/01/24 小島  新規作成
//---------------------------------------------------------------------
#include "kjmProcess.h"
#include <vector>

//---------------------------------------------------------------------
// デフォルトコンストラクタ
//---------------------------------------------------------------------
kjm::Process::Process() {
	ZeroMemory(&m_pi, sizeof(m_pi));
	ZeroMemory(&m_si, sizeof(m_si));
	m_si.cb = sizeof(m_si);
}

//---------------------------------------------------------------------
// プロセスが動作中かどうか調べる
//---------------------------------------------------------------------
bool kjm::Process::IsStillActive() {
	DWORD dwExitCode;

	::GetExitCodeProcess(m_pi.hProcess, &dwExitCode);

	return (dwExitCode == STILL_ACTIVE);
}

//---------------------------------------------------------------------
// プロセスを実行する
//
// [戻り値]
// true 正常終了。false エラー終了。
//---------------------------------------------------------------------
bool kjm::Process::Run(LPCTSTR cmdLine) {
	// すでに別のプロセス実行中の時は、エラーを返す。
	if (m_pi.hProcess != NULL) {
		return false;
	}

	// プロセス実行
	std::vector<TCHAR> work(MAX_PATH + lstrlen(cmdLine) + 1);
	lstrcpy(work.begin(), cmdLine);
	return (::CreateProcess(NULL, work.begin(), NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &m_si, &m_pi) != FALSE);
}

//---------------------------------------------------------------------
// プロセスのハンドルを閉じる
//
// [備考]
// プロセスを実行していなくても、正常に処理できます。
// デストラクタで呼んでいるので、明記しなくても構いません。
//---------------------------------------------------------------------
void kjm::Process::CloseProcessHandle() {
	// スレッドのハンドルを閉じる
	if (m_pi.hThread) {
		::CloseHandle(m_pi.hThread);
		m_pi.hThread = NULL;
	}

	// プロセスのハンドルを閉じる
	if (m_pi.hProcess) {
		::CloseHandle(m_pi.hProcess);
		m_pi.hProcess = NULL;
	}
}
