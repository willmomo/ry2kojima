//
// プロセスに関するコード
//
// process : Version 0.2015.12.23
//
// r.kojima
//
#include "process.h"
#include <assert.h>

//=====================================================================
//
//  process クラスの実装
//
//=====================================================================

//---------------------------------------------------------------------
// GetProcessWindow関数で使用するコールバック
//
// [引数]
// HWND hwnd		列挙されたウィンドウへのハンドル
// LPARAM lParam	rkProcessへのポインタ。キャストして使う
//
// [備考]
// 列挙されたウィンドウと同じプロセスIDを持つものを見つけた場合
// そのハンドルをメンバー変数に保存し、列挙を終了する。
//---------------------------------------------------------------------
BOOL CALLBACK kjm::process::process_EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	kjm::process* p = reinterpret_cast<kjm::process*>(lParam);

	DWORD dwProcessId;
	GetWindowThreadProcessId(hwnd, &dwProcessId);
	if (dwProcessId == p->m_pi.dwProcessId) {
		p->m_hwnd = hwnd;
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------
// デフォルト・コンストラクタ
//---------------------------------------------------------------------
kjm::process::process() {
	memset(&m_si, 0, sizeof(m_si));
	memset(&m_pi, 0, sizeof(m_pi));
	memset(&m_szCommandLine, 0, sizeof(m_szCommandLine));
}

// スタートアップ情報(wShowWindow)を設定する
// ShowWindow API の引数と同じ
//
void kjm::process::set_ShowWindow(WORD newShowWindow) {
	m_si.wShowWindow = newShowWindow;
	m_si.dwFlags |= STARTF_USESHOWWINDOW;
}

// スタートアップ情報(wShowWindow)をクリア(未使用に)する
//
void kjm::process::clear_ShowWindow() {
	m_si.wShowWindow = 0;
	m_si.dwFlags &= ~STARTF_USESHOWWINDOW;
}

//---------------------------------------------------------------------
// プロセスIDからプロセスを終了させるユーティリティ関数
//---------------------------------------------------------------------
BOOL kjm::process::terminateProcess(DWORD dwProcessId, UINT uExitCode) {
	BOOL bRet = FALSE;
	kjm::process proc;
	if (proc.openProcess(PROCESS_TERMINATE, FALSE, dwProcessId)) {
		bRet = proc.kill(uExitCode);
	}
	return bRet;
}

// プロセスの実行
//
BOOL kjm::process::createProcess(const kjm::_tstring& commandLine) {
	if (m_pi.hProcess)	return FALSE;	// すでに何か実行している

	lstrcpy(m_szCommandLine, commandLine.c_str());
	m_si.cb = sizeof(m_si);
	BOOL ret = CreateProcess(NULL, m_szCommandLine, NULL, NULL, TRUE, 0, NULL, NULL, &m_si, &m_pi);
	return ret;
}

//---------------------------------------------------------------------
// プロセスハンドルを閉じる
//
// [説明]
// プロセスハンドルを閉じる。プロセスの終了ではないので注意。
// プロセスの終了は、プロセス間の取り決めによる。
//---------------------------------------------------------------------
void kjm::process::Close() {
	if (m_pi.hThread)	CloseHandle(m_pi.hThread);
	if (m_pi.hProcess)	CloseHandle(m_pi.hProcess);
	memset(&m_pi, 0, sizeof(m_pi));
}

//---------------------------------------------------------------------
// プロセスの終了コードを取得する
//---------------------------------------------------------------------
DWORD kjm::process::getExitCode() {
	DWORD exitCode = 0;
	::GetExitCodeProcess(this->m_pi.hProcess, &exitCode);
	return exitCode;
}

//---------------------------------------------------------------------
// プロセスに関連付けられたウィンドウを返す
//---------------------------------------------------------------------
HWND kjm::process::GetProcessWindow() {
	assert(m_pi.hProcess);

	if (!m_pi.hProcess)	return NULL;

	m_hwnd = NULL;
	EnumWindows(kjm::process::process_EnumWindowsProc, reinterpret_cast<LPARAM>(this));
	return m_hwnd;
}

// 簡易プロセス実行
//
// bWaitOnReturn == true: 正常に実行出来たら 0、失敗したら -1
// bWaitOnReturn != true: 実行したプロセスの戻り値
//
int kjm::process::run(const kjm::_tstring& strCommand, int intWindowStyle, bool bWaitOnReturn) {
	int result;

	kjm::process proc;
	proc.set_ShowWindow(intWindowStyle);
	BOOL ret = proc.createProcess(strCommand);
	if (bWaitOnReturn) {
		proc.waitForTermination();
		result = proc.getExitCode();
	} else {
		result = (ret ? 0 : -1);
	}
	return result;
}

//=====================================================================
//
//  Process クラスの実装
//
//=====================================================================

