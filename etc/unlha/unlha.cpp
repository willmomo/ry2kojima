#include <windows.h>
#include <tchar.h>
#include <time.h>
#include "unlha32.h"

LRESULT OnCreate(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	// 解凍用コマンドラインの作成
	char szCmdLine[MAX_PATH];
	wsprintf(szCmdLine, "e %s -gm1 -n1 -x1m2r2a2 %s", _T("UNLHAVC.LZH"), _T("unpack"));

	char szOutput[8192];

	int retLha = Unlha(NULL, szCmdLine, szOutput, sizeof(szOutput));
	if (retLha != 0) {
		OutputDebugString(szOutput);
		return 0;
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
		return OnCreate(hwnd, wParam, lParam);

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	TCHAR szClassName[] = _T("unlha");

	// ウィンドウクラスの登録
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szClassName;
	wcex.hIconSm = NULL;

	if (!RegisterClassEx(&wcex)) {
		return -1;
	}

	// ウィンドウの作成
	HWND hwnd = CreateWindowEx(
		0,
		szClassName, _T("Unlha"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	if (!hwnd) {
		return -2;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// メッセージループ
	MSG msg;
	while (1) {
		BOOL ret = GetMessage(&msg, NULL, 0, 0);
		if (ret <= 0) {
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
