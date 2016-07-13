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

#include "kjmsub.h"

#include "resource.h"


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
// メインプロシージャ
//---------------------------------------------------------------------
BOOL CALLBACK Main_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
		{
			WORD wID = LOWORD(wParam);
			if (wID == IDOK) {
				EndDialog(hwnd, 1);
				return TRUE;
			} else if (wID == IDCANCEL) {
				EndDialog(hwnd, 0);
				return TRUE;
			}
		}
		break;
	}

	return FALSE;
}


//---------------------------------------------------------------------
// プログラム スタート
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	int errorlevel = 0;

	_tsetlocale(LC_ALL, _T("japan"));

	int nRet = DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, Main_DlgProc);

	// [はい]を選択された時、再起動される。
	if (nRet == 1) {
		kjm::util::exitWindows();
	}

	return errorlevel;
}
