#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <direct.h>
#include <time.h>
#include "mainFrm.h"

//-----------------------------------------------------------------------------
//	プログラムスタート
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// メインフレームの登録
	if (MainFrm_RegisterClass(hInstance) == FALSE) {
		return -1;
	}

	// メインフレームの作成
	if (MainFrm_CreateWindow(hInstance, nCmdShow) == FALSE) {
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

	return result;
}
