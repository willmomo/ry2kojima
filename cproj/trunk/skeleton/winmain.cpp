//-----------------------------------------------------------------------------
// なにもしないけどそこそこ動くスケルトンプログラム
//-----------------------------------------------------------------------------
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <process.h>
#include "resource.h"

// タスクトレイアイコンからのメッセージ
#define WMU_NOTIFYICONMESSAGE	(WM_APP + 1)

//-----------------------------------------------------------------------------
// メインフレームのウィンドウプロシージャ
//-----------------------------------------------------------------------------
static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static NOTIFYICONDATA s_nid;

	switch (uMsg) {
		case WM_CREATE:
			ZeroMemory(&s_nid, sizeof(s_nid));
			s_nid.cbSize = sizeof(s_nid);
			s_nid.hWnd = hwnd;
			s_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
			s_nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			s_nid.uCallbackMessage = WMU_NOTIFYICONMESSAGE;
			lstrcpy(s_nid.szTip, _T("Skeleton"));
			Shell_NotifyIcon(NIM_ADD, &s_nid);
			return 0;

		case WM_DESTROY:
			Shell_NotifyIcon(NIM_DELETE, &s_nid);
			PostQuitMessage(0);
			return 0;

		case WM_COMMAND:
			if (LOWORD(wParam) == ID_NOTIFYICONPOPUP_EXIT) {
				DestroyWindow(hwnd);
				return 0;
			}
			break;

		case WMU_NOTIFYICONMESSAGE:
			if (lParam == WM_RBUTTONUP) {
				POINT pt;
				GetCursorPos(&pt);
				HMENU hmenu = LoadMenu(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDR_NOTIFY_ICON_MENU));
				if (hmenu) {
					SetForegroundWindow(hwnd);
					TrackPopupMenu(GetSubMenu(hmenu, 0), TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
					PostMessage(hwnd, WM_NULL, 0, 0);

					DestroyMenu(hmenu);
				}
			}
			return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// プログラムスタート
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// ウィンドウクラスの登録
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _T("SkeletonMainFrm");
	if (!RegisterClass(&wc)) {
		return -1;
	}

	// メインウィンドウの作成
	HWND hwnd = CreateWindow(wc.lpszClassName, _T("Skeleton Main Frame"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	if (hwnd == NULL) {
		return -1;
	}

	// TODO: ウィンドウを表示するときは、次の行を有効化
	// ShowWindow(hwnd, nCmdShow);

	// メッセージループ
	MSG msg;
	while (true) {
		BOOL bRet = GetMessage(&msg, NULL, 0, 0);
		if (bRet == 0 || bRet == -1) {
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
