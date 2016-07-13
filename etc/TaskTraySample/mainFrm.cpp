//-----------------------------------------------------------------------------
// メインフレームのためのソースコード
//
// 2012/03/14 r.kojima
//-----------------------------------------------------------------------------
#include "mainFrm.h"
#include "kjmlib.h"

#include <process.h>
#include "resource.h"

#define MAINFRM_CLASSNAME	_T("TasktraySampleMainFrm")

#define ID_TASKTRAYICON	1

#define MYWM_TASKTRAY	(WM_APP + 0)	// タスクトレイからのメッセージ

static kjm::taskTrayIcon s_taskTrayIcon;
static HANDLE s_hThreadWorker = NULL;
static bool s_loop = true;

//-----------------------------------------------------------------------------
// ワーカースレッド
//-----------------------------------------------------------------------------
static unsigned __stdcall worker(void* parg) {
	while (s_loop) {
		Sleep(100);
	}
	return 0;
}

//-----------------------------------------------------------------------------
// WM_CREATE メッセージ
//-----------------------------------------------------------------------------
static LRESULT OnCreate(HWND hwnd) {
	// タスクトレイにアイコンを追加
	s_taskTrayIcon.addIcon(hwnd, ID_TASKTRAYICON, 
		(HICON)LoadImage(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDI_MAINFRM), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR),
		MYWM_TASKTRAY, _T("Task Tray Sample"));

	// ワーカースレッド起動
	s_loop = true;
	s_hThreadWorker = (HANDLE)_beginthreadex(NULL, 0, worker, NULL, 0, NULL);

	return 0;
}

//-----------------------------------------------------------------------------
// WM_DESTROY メッセージ
//-----------------------------------------------------------------------------
static LRESULT OnDestroy(HWND hwnd) {
	// ワーカースレッド終了
	if (s_hThreadWorker != NULL) {
		s_loop = false;
		WaitForSingleObject(s_hThreadWorker, INFINITE);
	}

	// タスクトレイからアイコンを削除
	s_taskTrayIcon.deleteIcon();

	PostQuitMessage(0);
	return 0;
}

//-----------------------------------------------------------------------------
// [終了]メニュー処理
//-----------------------------------------------------------------------------
static LRESULT OnPopupExit(HWND hwnd) {
	DestroyWindow(hwnd);
	return 0;
}

//-----------------------------------------------------------------------------
// WM_COMMAND メッセージ
//-----------------------------------------------------------------------------
static LRESULT OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	WORD wID = LOWORD(wParam);

	if (wID == IDM_POPUP_EXIT) {
		return OnPopupExit(hwnd);
	}

	return DefWindowProc(hwnd, WM_COMMAND, wParam, lParam);
}

//-----------------------------------------------------------------------------
// MYWM_TASKTRAY メッセージ
//-----------------------------------------------------------------------------
static LRESULT OnMyTaskTray(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	if (lParam == WM_RBUTTONUP) {
		HMENU hmenu = LoadMenu(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDR_POPUP_MENU));
		if (hmenu) {
			HMENU hmenuSub = GetSubMenu(hmenu, 0);
			if (hmenuSub) {
				POINT point;
				GetCursorPos(&point);

				SetForegroundWindow(hwnd);
				TrackPopupMenu(hmenuSub, TPM_LEFTBUTTON | TPM_LEFTALIGN, point.x, point.y, 0, hwnd, NULL);
				PostMessage(hwnd, WM_NULL, 0, 0);
			}
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// ウィンドウプロシージャ
//-----------------------------------------------------------------------------
static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_CREATE:
			return OnCreate(hwnd);

		case WM_DESTROY:
			return OnDestroy(hwnd);

		case WM_COMMAND:
			return OnCommand(hwnd, wParam, lParam);

		case MYWM_TASKTRAY:
			return OnMyTaskTray(hwnd, wParam, lParam);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// ウィンドウの作成
//-----------------------------------------------------------------------------
BOOL MainFrm_CreateWindow(HINSTANCE hInstance, int nCmdShow) {
	HWND hwnd = CreateWindow(
		MAINFRM_CLASSNAME, _T("Task Tray Sample"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	if (hwnd != NULL) {
		/// ShowWindow(hwnd, nCmdShow);
	}

	return (hwnd != NULL) ? TRUE : FALSE;
}

//-----------------------------------------------------------------------------
// ウィンドウクラスの作成
//-----------------------------------------------------------------------------
BOOL MainFrm_RegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX wc = {sizeof(wc)};
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_MAINFRM), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	wc.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_MAINFRM), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = MAINFRM_CLASSNAME;

	return RegisterClassEx(&wc);
}
