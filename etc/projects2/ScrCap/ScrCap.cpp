//---------------------------------------------------------------------
// 簡単画面キャプチャ
//---------------------------------------------------------------------
#include "ScrCap.h"

//---------------------------------------------------------------------
// 定数
//---------------------------------------------------------------------
#define ID_TASKTRAYICON	100				// タスクトレイアイコンのID
#define MYWM_NOTIFYICON	(WM_USER + 1)	// タスクトレイアイコンからのメッセージ

#define ID_HK_END		100				// アプリケーション終了のホットキー
#define ID_HK_CAPTURE	101				// 画面キャプチャのホットキー

//---------------------------------------------------------------------
// グローバル変数
//---------------------------------------------------------------------
NOTIFYICONDATA g_nid;

//---------------------------------------------------------------------
// デスクトップをキャプチャする
//---------------------------------------------------------------------
void DoDesktopCapture(HWND hwnd) {
	// 画面全体の矩形を取得
	RECT rc;
	SetRect(&rc, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

	// 画面全体を DIB にする
	HDIB hdib = CopyScreenToDIB(&rc);
	if (hdib) {
		static DWORD s_dwSerial = 0;
		SYSTEMTIME st;
		GetLocalTime(&st);

		// 現在日時 + 通し番号でファイル名を作る。
		TCHAR fname[MAX_PATH];
		wsprintf(fname, _T("%04d%02d%02d_%02d%02d%02d_%03d.bmp"),
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, s_dwSerial);
		s_dwSerial++;

		SaveDIB(hdib, kjm::path::rename_file_spec(kjm::util::get_module_file_name(), fname).c_str());
		DestroyDIB(hdib);
	}
}

//---------------------------------------------------------------------
// WM_COMMAND メッセージ処理
//---------------------------------------------------------------------
BOOL OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	WORD wNotifyCode = HIWORD(wParam);	// notification code 
	WORD wID = LOWORD(wParam);			// item, control, or accelerator identifier 
	HWND hwndCtl = (HWND) lParam;		// handle of control 

	switch (wID) {
	case IDOK:
	case IDCANCEL:
	case IDM_POPUP_EXIT:
		DestroyWindow(hwnd);
		return TRUE;
	}

	return FALSE;
}

//---------------------------------------------------------------------
// WM_DESTROY メッセージ処理
//---------------------------------------------------------------------
BOOL OnDestroy(HWND hwnd) {
	// ホットキーを取り除く
	UnregisterHotKey(hwnd, ID_HK_END);
	UnregisterHotKey(hwnd, ID_HK_CAPTURE);

	// タスクトレイアイコンを削除
	Shell_NotifyIcon(NIM_DELETE, &g_nid);

	// メッセージループの終了を指示
	PostQuitMessage(0);

	return TRUE;
}

//---------------------------------------------------------------------
// WM_HOTKEY メッセージ処理
//---------------------------------------------------------------------
BOOL OnHotKey(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	int idHotKey = (int) wParam;				// identifier of hot key 
	UINT fuModifiers = (UINT) LOWORD(lParam);	// key-modifier flags 
	UINT uVirtKey = (UINT) HIWORD(lParam);		// virtual-key code 

	switch (idHotKey) {
	case ID_HK_END:
		PostMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDM_POPUP_EXIT, 0), 0);
		break;

	case ID_HK_CAPTURE:
		DoDesktopCapture(hwnd);
		break;
	}

	return TRUE;
}

//---------------------------------------------------------------------
// WM_INITDIALOG メッセージ処理
//---------------------------------------------------------------------
BOOL OnInitDialog(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	HWND hwndFocus = (HWND) wParam;	// handle of control to receive focus 
	LONG lInitParam = lParam;		// initialization parameter 

	// タスクトレイアイコンを登録
	ZeroMemory(&g_nid, sizeof(g_nid));
	g_nid.cbSize = sizeof(g_nid);
	g_nid.hWnd = hwnd;
	g_nid.uID = ID_TASKTRAYICON;
	g_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	g_nid.uCallbackMessage = MYWM_NOTIFYICON;
	g_nid.hIcon = LoadIcon(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDI_MAIN));
	lstrcpyn(g_nid.szTip, _T("スクリーン・キャプチャ"), countof(g_nid.szTip));
	
	Shell_NotifyIcon(NIM_ADD, &g_nid);

	// ホットキーを登録
	RegisterHotKey(hwnd, ID_HK_END, MOD_CONTROL, VK_END);			// Ctrl + End
	RegisterHotKey(hwnd, ID_HK_CAPTURE, MOD_CONTROL, VK_SNAPSHOT);	// Ctrl + PrtSc

	return FALSE;	// フォーカスはシステムに任せる
}

//---------------------------------------------------------------------
// MYWM_NOTIFYICON メッセージ処理
//---------------------------------------------------------------------
BOOL OnMyNotifyIcon(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	UINT uID = wParam;
	UINT uMouseMsg = lParam;

	if (uMouseMsg == WM_RBUTTONUP) {
		HMENU hmenu = LoadMenu(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDR_POPUPMENU));
		if (hmenu != NULL) {
			HMENU hmenuSub = GetSubMenu(hmenu, 0);
			if (hmenuSub) {
				POINT pt;
				GetCursorPos(&pt);
				TrackPopupMenu(hmenuSub, TPM_LEFTALIGN | TPM_BOTTOMALIGN, pt.x, pt.y, 0, hwnd, NULL);
				PostMessage(hwnd, WM_NULL, 0, 0);
			}
			DestroyMenu(hmenu);
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------
// ダイアログプロシージャ
//---------------------------------------------------------------------
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
		return OnCommand(hwnd, wParam, lParam);

	case WM_DESTROY:
		return OnDestroy(hwnd);

	case WM_HOTKEY:
		return OnHotKey(hwnd, wParam, lParam);

	case WM_INITDIALOG:
		return OnInitDialog(hwnd, wParam, lParam);

	case MYWM_NOTIFYICON:
		return OnMyNotifyIcon(hwnd, wParam, lParam);
	}

	return FALSE;
}

//---------------------------------------------------------------------
// プログラムスタート
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// 起動時に非表示のダイアログを作りたいので、モードレスダイアログを使用
	HWND hdlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DlgProc);
	if (hdlg == NULL) {
		return -1;
	}

	MSG msg;
	while (1) {
		BOOL bRet = GetMessage(&msg, NULL, 0, 0);
		if (bRet == 0 || bRet == -1) {
			break;
		}

		if (!IsDialogMessage(hdlg, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}
