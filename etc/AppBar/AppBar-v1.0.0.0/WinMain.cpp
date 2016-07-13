#include <windows.h>
#include <tchar.h>
#include "AppBar.h"

#define CLASSNAME	_T("APPBAR_MAINFRM")

#define APPBAR_MESSAGE	(WM_USER)

kjm::appBar g_appBar;

static LRESULT OnCreate(HWND hwnd) {
	g_appBar.regist(hwnd, APPBAR_MESSAGE);
	SIZE size = {100, 50};
	g_appBar.setPos(ABE_BOTTOM, size);
	return 0;
}

static LRESULT OnDestroy(HWND hwnd) {
	g_appBar.remove();
	PostQuitMessage(0);
	return 0;
}

LRESULT CALLBACK MainWnd_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_CREATE:
			return OnCreate(hwnd);

		case WM_DESTROY:
			return OnDestroy(hwnd);

		case APPBAR_MESSAGE:
			g_appBar.callbackMessage(wParam, lParam);
			return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	HWND hwnd = CreateWindowEx(
		WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
		CLASSNAME,
		_T(""),
		WS_POPUPWINDOW,
		0, 0, 0, 0,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		return FALSE;
	}

	ShowWindow(hwnd, nCmdShow);

	return TRUE;
}

BOOL InitApplication(HINSTANCE hInstance) {
	WNDCLASS wc;
	
	wc.style = 0;
	wc.lpfnWndProc = MainWnd_WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASSNAME;

	return RegisterClass(&wc);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	if (!InitApplication(hInstance)) {
		return -1;
	}

	if (!InitInstance(hInstance, nCmdShow)) {
		return -1;
	}

	MSG msg;
	while (1) {
		BOOL ret = GetMessage(&msg, NULL, 0, 0);
		if (ret == -1) {
			return -1;
		}
		if (ret == 0) {
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
