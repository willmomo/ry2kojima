#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <direct.h>
#include <time.h>
#include "mainFrm.h"
#include "kjmlib-20130512/kjmlib.h"

//-----------------------------------------------------------------------------
//	プログラムスタート
//-----------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	{
		TCHAR iniFile[MAX_PATH];
		GetModuleFileName(NULL, iniFile, MAX_PATH);
		PathRenameExtension(iniFile, _T(".ini"));

		TCHAR exePath[MAX_PATH];
		GetModuleFileName(NULL, exePath, MAX_PATH);
		PathRemoveFileSpec(exePath);
		SetEnvironmentVariable(_T("EXEPATH"), exePath);

		TCHAR work[MAX_PATH];
		TCHAR logPath[MAX_PATH];
		TCHAR logName[MAX_PATH];

		GetPrivateProfileString(_T("log"), _T("path"), _T(""), work, MAX_PATH, iniFile);
		GetPrivateProfileString(_T("log"), _T("name"), _T(""), logName, MAX_PATH, iniFile);

		if (work[0] != '\0') {
			ExpandEnvironmentStrings(work, logPath, MAX_PATH);

			CreateDirectory(logPath, NULL);
			kjm::GetLog().set_log_path(logPath);
		}

		if (logName[0] != '\0') {
			kjm::GetLog().set_prefix(logName);
		}

		lstrcpy(&work[0], kjm::GetLog().get_log_path().c_str());
	}
	kjm::GetLog().set_Stop(false);
	kjm::GetLog().write(_T("---- %s start. ----"), __targv[0]);

	// メインフレームの登録
	if (MainFrm_RegisterClass(hInstance) == FALSE) {
		kjm::GetLog().write(_T("---- %s error end 1. ----"), __targv[0]);
		return -1;
	}

	// メインフレームの作成
	if (MainFrm_CreateWindow(hInstance, nCmdShow) == FALSE) {
		kjm::GetLog().write(_T("---- %s error end 2. ----"), __targv[0]);
		return -1;
	}

	// メッセージループ
	int result;
	while (1) {
		MSG msg;
		BOOL ret = GetMessage(&msg, NULL, 0, 0);
		if (ret == 0) {
			result = msg.wParam;
			break;
		} else if (ret < 0) {
			result = -1;
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	WSACleanup();

	kjm::GetLog().write(_T("---- %s end with %d. ----"), __targv[0], result);
	return result;
}
