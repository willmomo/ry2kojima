//---------------------------------------------------------------------
// KillProc.exe : 指定したプロセスを強制終了するアプリケーション
//---------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>
#include <tlhelp32.h>

#include "kjmlib.h"

//---------------------------------------------------------------------
// グローバル変数
//---------------------------------------------------------------------
kjm::log g_log;			// アプリケーション全体のログ
TCHAR g_name[MAX_PATH];		// -nで指定された実行ファイル名


//---------------------------------------------------------------------
// 指定した実行ファイル名を持つアプリケーションを強制終了する
//
// [引数]
// lpszExeName	強制終了するプロセスの実行ファイル名(イメージ名)
//
// [戻り値]
// 削除したプロセスの数。
// -1 : スナップショットの作成に失敗
//---------------------------------------------------------------------
int killProcesses(const _tstring& lpszExeName) {
	g_log.write(_T("killProcesses('%s') enter."), lpszExeName.c_str());

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		g_log.write(_T("killProcesses: CreateToolhelp32Snapshot() error: %u."), GetLastError());
		return -1;
	}

	PROCESSENTRY32 pe;
	ZeroMemory(&pe, sizeof(pe));
	pe.dwSize = sizeof(pe);

	int nKilled = 0;
	BOOL ret = Process32First(hSnap, &pe);
	while (ret) {
		if (lstrcmpi(lpszExeName.c_str(), pe.szExeFile) == 0) {
			kjm::process::terminateProcess(pe.th32ProcessID, 0);
		}
		ret = Process32Next(hSnap, &pe);
	}

	if (CloseHandle(hSnap) == FALSE) {
		g_log.write(_T("killProcesses: CloseHandle(hSnap = 0x%p) error: %u."), hSnap, GetLastError());
	}

	g_log.write(_T("killProcesses() leave with %d."), nKilled);
	return nKilled;
}


//---------------------------------------------------------------------
// プログラム・スタート
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	g_log.write("---- '%s' start. ----", kjm::util::getModuleFileName().c_str());

	_tstring imageName;
	kjm::commandLine cl;
	cl.parse(__argc, __targv, _T("?vn"));
	std::vector<_tstring> opts = cl.getOptList();

	for (std::vector<_tstring>::size_type i = 0; i < opts.size(); i++) {
		if (opts[i] == _T("name") || opts[i] == _T("n")) {
			imageName = cl.getOptArg(opts[i]);
		}
	}

	if (imageName.empty()) {
		MessageBox(NULL, _T("usage: KillProc --name=<image name>"), _T("usage: KillProc"), MB_OK | MB_ICONINFORMATION);
		return 0;
	}

	// -nオプションで指定された実行ファイルのプロセスを強制終了する。
	if (!imageName.empty()) {
		killProcesses(imageName);
	}

	g_log.write("---- '%s' end. ----", kjm::util::getModuleFileName().c_str());
	return 0;
}
