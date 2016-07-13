#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <direct.h>
#include <time.h>
#include "mainFrm.h"
#include "kjmlog.h"

//-----------------------------------------------------------------------------
//	プログラムスタート
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	KLogInit();
	{
		char iniFile[MAX_PATH];
		GetModuleFileName(NULL, iniFile, MAX_PATH);
		PathRenameExtension(iniFile, ".ini");

		char exePath[MAX_PATH];
		GetModuleFileName(NULL, exePath, MAX_PATH);
		PathRemoveFileSpec(exePath);
		SetEnvironmentVariable("EXEPATH", exePath);

		char work[MAX_PATH];
		char logPath[MAX_PATH];
		char logName[MAX_PATH];

		GetPrivateProfileString("log", "path", "", work, MAX_PATH, iniFile);
		GetPrivateProfileString("log", "name", "", logName, MAX_PATH, iniFile);

		if (work[0] != '\0') {
			ExpandEnvironmentStrings(work, logPath, MAX_PATH);

			CreateDirectory(logPath, NULL);
			KLogSetLogPath(logPath);
		}

		if (logName[0] != '\0') {
			KLogSetLogName(logName);
		}

		KLogGetLogPath(work);
	}
	KLogWrite("---- %s start. ----", __argv[0]);

	// メインフレームの登録
	if (MainFrm_RegisterClass(hInstance) == FALSE) {
		KLogWrite("---- %s error end 1. ----", __argv[0]);
		return -1;
	}

	// メインフレームの作成
	if (MainFrm_CreateWindow(hInstance, nCmdShow) == FALSE) {
		KLogWrite("---- %s error end 2. ----", __argv[0]);
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

	KLogWrite("---- %s end with %d. ----", __argv[0], result);
	return result;
}
