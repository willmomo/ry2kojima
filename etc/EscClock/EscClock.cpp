#include "EscClock.h"


//---------------------------------------------------------------------
// プログラム スタート
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	if (!MainWnd_RegisterClass(hInstance)) {
		return -1;
	}

	if (!MainWnd_CreateWindow(hInstance)) {
		return -1;
	}

	MSG msg;
	while (1) {
		BOOL bRet = GetMessage(&msg, NULL, 0, 0);
		if (bRet == 0 || bRet == -1) {
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
