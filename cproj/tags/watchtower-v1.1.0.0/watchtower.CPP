#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "./kmwatch/kmwatch.h"

#if defined(_DEBUG)
#pragma comment(lib, "./kmwatch/debug/kmwatch.lib")
#else
#pragma comment(lib, "./kmwatch/release/kmwatch.lib")
#endif

///////////////////////////////////////////////////////////////////////

TCHAR g_szMainClassName[64];
TCHAR g_szMainWindowName[64];

UINT WM_KMWATCH;
UINT WM_KMWATCH_MOUSE;

///////////////////////////////////////////////////////////////////////
//	[概要]
//	メインウィンドウ・プロシージャ

LRESULT CALLBACK Main_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static unsigned s_kmwatchCnt = 0;
	static unsigned s_kmwatchMouseCnt = 0;
	static unsigned s_lastTick = 0;

	switch (uMsg) {
	case WM_CREATE:
		WM_KMWATCH = RegisterWindowMessage(KMWATCH_MESSAGE);
		WM_KMWATCH_MOUSE = RegisterWindowMessage(KMWATCH_MOUSE_MESSAGE);
		SetTimer(hwnd, 101, 500, NULL);
		SetHook(hwnd);
		return 0;

	case WM_DESTROY:
		FreeHook();
		KillTimer(hwnd, 101);
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			char buf[96];

			BeginPaint(hwnd, &ps);
			wsprintf(buf, "キーボードフック: %u", s_kmwatchCnt);
			TextOut(ps.hdc, 0, 0, buf, lstrlen(buf));
			wsprintf(buf, "マウス　　フック: %u", s_kmwatchMouseCnt);
			TextOut(ps.hdc, 0, 20, buf, lstrlen(buf));
			wsprintf(buf, "アイドル時間　　: %u ms", GetTickCount() - s_lastTick);
			TextOut(ps.hdc, 0, 40, buf, lstrlen(buf));
			EndPaint(hwnd, &ps);
		}
		return 0;

	case WM_TIMER:
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	default:
		if (uMsg == WM_KMWATCH) {
			s_lastTick = GetTickCount();
			++s_kmwatchCnt;
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;
		} else if (uMsg == WM_KMWATCH_MOUSE) {
			s_lastTick = GetTickCount();
			++s_kmwatchMouseCnt;
			InvalidateRect(hwnd, NULL, TRUE);
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////
//	[概要]
//	メインウィンドウの作成

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hwnd = CreateWindowEx(
		WS_EX_TOPMOST, g_szMainClassName, g_szMainWindowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 300, 128,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		return FALSE;
	}

	//ShowWindow(hwnd, nCmdShow);
	ShowWindow(hwnd, SW_SHOWMINNOACTIVE);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////
//	[概要]
//	アプリケーションの初期化

BOOL InitApplication(HINSTANCE hInstance)
{
	LoadString(hInstance, IDS_MAINCLASSNAME, g_szMainClassName, sizeof(g_szMainClassName));
	LoadString(hInstance, IDS_MAINWINDOWNAME, g_szMainWindowName, sizeof(g_szMainWindowName));

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
	wcex.lpfnWndProc = Main_WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_MAIN), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR|LR_DEFAULTSIZE);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = g_szMainClassName;
	wcex.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_MAIN), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

	return RegisterClassEx(&wcex);
}

///////////////////////////////////////////////////////////////////////
//	[概要]
//	プログラム・スタート

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!InitApplication(hInstance)) {
		return -1;
	}

	if (!InitInstance(hInstance, nCmdShow)) {
		return -1;
	}

	MSG msg;
	while (1) {
		BOOL ret = GetMessage(&msg, NULL, 0, 0);
		if (ret == 0 || ret == -1) {
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
