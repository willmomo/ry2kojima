//---------------------------------------------------------------------
// Windowsを再起動するプログラム
//---------------------------------------------------------------------
#if defined(UNICODE)
#define _UNICODE
#endif

#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <locale.h>
#include <stdio.h>


//---------------------------------------------------------------------
// 簡易ログ
//---------------------------------------------------------------------
void LogWrite(LPCTSTR pszFormat, ...) {
	TCHAR szLogFile[MAX_PATH];
	GetTempPath(MAX_PATH, szLogFile);

	TCHAR szModuleName[MAX_PATH];
	GetModuleFileName(NULL, szModuleName, MAX_PATH);

	PathRenameExtension(szModuleName, _T(".log"));

	PathAppend(szLogFile, PathFindFileName(szModuleName));

	FILE* fp = _tfopen(szLogFile, _T("a+"));
	if (fp) {
		SYSTEMTIME st;
		GetLocalTime(&st);

		_ftprintf(fp, _T("%04d/%02d/%02d %02d:%02d:%02d.%03d "),
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

		va_list marker;
		va_start(marker, pszFormat);

		_vftprintf(fp, pszFormat, marker);

		va_end(marker);

		fclose(fp);
	}
}

//---------------------------------------------------------------------
// プログラム スタート
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	int errorlevel = 0;

	_tsetlocale(LC_ALL, _T("japan"));

	// Windows をシャットダウン
	OSVERSIONINFO ovi;
	ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&ovi);
	if (ovi.dwPlatformId == VER_PLATFORM_WIN32_NT){
		// NT対応
		HANDLE hToken;
		TOKEN_PRIVILEGES tp;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)){
			//SE_SHUTDOWN_NAME権限をもたせる
			LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
			if (GetLastError() != ERROR_SUCCESS) {
				return 1;
			}
			CloseHandle(hToken);
		}

		LogWrite(_T("ExitWindowsEx(EWX_REBOOT)[NT系] を実行します。\n"));
		if (ExitWindowsEx(EWX_REBOOT, 0) == FALSE) {
			LogWrite(_T("ExitWindowsEx(EWX_REBOOT)[NT系] に失敗しました(error %u)。\n"), GetLastError());
		}
		
	} else {
		// Windows95/98/Me 用
		LogWrite(_T("ExitWindowsEx(EWX_REBOOT)[95系] を実行します。\n"));
		if (ExitWindowsEx(EWX_REBOOT, 0) == FALSE) {
			LogWrite(_T("ExitWindowsEx(EWX_REBOOT)[95系] に失敗しました(error %u)。\n"), GetLastError());
		}
	}

	return errorlevel;
}
