
#include <windows.h>
#include <mmsystem.h>
#include "sounddata.h"
#pragma comment(lib, "winmm.lib")

#define MAINFRM_CLASSNAME	"MainFrm_SndPlay"

int g_index = 0;
CSoundData* g_soundData[3];

static WORD g_mciDeviceID = 0;

static LRESULT OnCreate(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	g_soundData[0] = new CSoundData(hwnd, "b10.mp3");
	g_soundData[1] = new CSoundData(hwnd, "s1.mp3");
	g_soundData[2] = new CSoundData(hwnd, "s40.mp3");

	return 0;
}

static void OnMciNotify(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	if (g_index > 2)	return;

//	g_soundData[g_index]->Stop();
	++g_index;

	if (g_index > 2) {
		g_soundData[0]->Stop();
		g_soundData[1]->Stop();
		g_soundData[2]->Stop();
		return;
	}

	g_soundData[g_index]->Play();
}

static void OnLButtonDown(HWND hwnd, int fwKeys, int xPos, int yPos)
{
	g_index = 0;
	g_soundData[g_index]->Play();
}

static void OnDestroy(HWND hwnd)
{
	delete g_soundData[0];
	delete g_soundData[1];
	delete g_soundData[2];

	PostQuitMessage(0);
}

LRESULT CALLBACK MainFrm_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CREATE:
		return OnCreate(hwnd, wParam, lParam);

	case MM_MCINOTIFY:
		OnMciNotify(hwnd, wParam, lParam);
		return 0;

	case WM_LBUTTONDOWN:
		OnLButtonDown(hwnd, wParam, LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_DESTROY:
		OnDestroy(hwnd);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ウィンドウクラスの登録
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainFrm_WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = MAINFRM_CLASSNAME;
	if (RegisterClass(&wc) == 0) {
		return -1;
	}

	// メインフレームの作成
	HWND hwnd = CreateWindow(
		MAINFRM_CLASSNAME,
		"SndPlay1",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	if (hwnd == NULL) {
		return -1;
	}
	ShowWindow(hwnd, nCmdShow);

	// メッセージループ
	MSG msg;
	while (true) {
		BOOL ret = GetMessage(&msg, NULL, 0, 0);
		if (ret <= 0) {
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
