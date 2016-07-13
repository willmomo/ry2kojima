//---------------------------------------------------------------------
// TermProc.exe : 指定したプロセスを強制終了するアプリケーション
//---------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>
#include <tlhelp32.h>

#include "../kjmlib/kjmlib.h"
#if defined(_DEBUG)
#pragma comment(lib, "../kjmlib/kjmlibd.lib")
#else
#pragma comment(lib, "../kjmlib/kjmlib.lib")
#endif


//---------------------------------------------------------------------
// グローバル変数
//---------------------------------------------------------------------
kjm::log g_log;			// アプリケーション全体のログ
TCHAR g_name[MAX_PATH];	// -nで指定された実行ファイル名


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
int killProcesses(LPCTSTR lpszExeName)
{
	g_log.writeLog(_T("killProcesses('%s') enter."), lpszExeName);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		g_log.writeLog(_T("killProcesses: CreateToolhelp32Snapshot() error: %u."), GetLastError());
		return -1;
	}

	PROCESSENTRY32 pe;
	ZeroMemory(&pe, sizeof(pe));
	pe.dwSize = sizeof(pe);

	int nKilled = 0;
	BOOL ret = Process32First(hSnap, &pe);
	while (ret) {
		if (lstrcmpi(lpszExeName, pe.szExeFile) == 0) {
			kjm::process::terminateProcess(pe.th32ProcessID, 0, &g_log);
		}
		ret = Process32Next(hSnap, &pe);
	}

	if (CloseHandle(hSnap) == FALSE) {
		g_log.writeLog(_T("killProcesses: CloseHandle(hSnap = 0x%p) error: %u."), hSnap, GetLastError());
	}

	g_log.writeLog(_T("killProcesses() leave with %d."), nKilled);
	return nKilled;
}


//---------------------------------------------------------------------
// プログラム・スタート
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	g_log.writeLog("---- '%s' start. ----", kjm::win32::getModuleFileName().c_str());

	option options[] = {
		{_T("name"), required_argument, NULL, 'n'},
		{NULL, no_argument, NULL, 0}
	};

	int c;
	int opt_index;
	while ((c = getopt_long(__argc, __argv, _T("n:"), options, &opt_index)) != -1) {
		switch (c) {
		case 'n':
			lstrcpy(g_name, g_optarg);
			break;
		}
	}

	// -nオプションで指定された実行ファイルのプロセスを強制終了する。
	if (g_name[0] != '\0') {
		killProcesses(g_name);
	}

	g_log.writeLog("---- '%s' end. ----", kjm::win32::getModuleFileName().c_str());
	return 0;
}
