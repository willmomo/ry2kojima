#include "EscClock.h"

#define WM_USER_NOTIFYICON	(WM_USER + 1)


static LPCTSTR s_pszClassName = _T("ESCCLOCK_MAINFRM");
static HWND s_hwnd;
static int s_nPos = 0;			// 0:左上 1:右上 2:右下 3:左下
static NOTIFYICONDATA s_nid;	// タスクトレイアイコンの情報


//---------------------------------------------------------------------
// メインウィンドウを所定の隅に移動する
//---------------------------------------------------------------------
static void MovePosition(HWND hwnd, int nPos) {
	RECT rcWork;
	RECT rcWindow;
	int x, y;

	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, 0);

	GetWindowRect(hwnd, &rcWindow);

	switch (nPos) {
	case 0:
		x = rcWork.left;
		y = rcWork.top;
		break;

	case 1:
		x = rcWork.right - (rcWindow.right - rcWindow.left);
		y = rcWork.top;
		break;

	case 2:
		x = rcWork.right - (rcWindow.right - rcWindow.left);
		y = rcWork.bottom - (rcWindow.bottom - rcWindow.top);
		break;

	case 3:
		x = rcWork.left;
		y = rcWork.bottom - (rcWindow.bottom - rcWindow.top);
		break;
	}

	MoveWindow(hwnd, x, y, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, TRUE);
}


//---------------------------------------------------------------------
// 画面の再描画
//---------------------------------------------------------------------
static LRESULT OnPaint(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	static int s_prevCpuUsage = 0;
	int cpuUsage = GetCPUUsage();

	// 前回のCPU使用率から、今回のCPU使用率まで緩やかに上げる工夫
	if (cpuUsage > s_prevCpuUsage) {
		cpuUsage = (s_prevCpuUsage = min(s_prevCpuUsage + 2, 100));
	} else if (cpuUsage < s_prevCpuUsage) {
		cpuUsage = (s_prevCpuUsage = max(s_prevCpuUsage - 2, 0));
	}

	LPCTSTR pszWeek[] = {_T("SUN"), _T("MON"), _T("TUE"), _T("WED"), _T("THU"), _T("FRI"), _T("SAT")};
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	RECT rcClient;
	GetClientRect(hwnd, &rcClient);

	SYSTEMTIME st;
	GetLocalTime(&st);

	TCHAR szText[96];
	wsprintf(szText, _T("%2d:%02d:%02d\n%04d/%02d/%02d (%s)"),
		st.wHour, st.wMinute, st.wSecond,
		st.wYear, st.wMonth, st.wDay,
		pszWeek[st.wDayOfWeek]);

	HFONT hFont = CreateFont(
		-MulDiv(19, GetDeviceCaps(hdc, LOGPIXELSY), 72),
		0,	// nWidth
		0,	// nEscapement
		0,	// nOrientation
		0,	// fnWeight
		0,	// fdwItalic
		0,	// fdwUnderline
		0,	// fdwStrikeOut
		DEFAULT_CHARSET,
		0,0,0,0,
		_T("Impact")
//		_T("MS UI Gothic")
		);

	HFONT hFontOld = (HFONT)SelectObject(hdc, hFont);

	int R = 255 * min(cpuUsage * 2, 100) / 100;
	int G = 255 * (100 - max(cpuUsage - 50, 0) * 2) / 100;
	HBRUSH hbrBk = CreateSolidBrush(RGB(R, G, 128));

	FillRect(hdc, &rcClient, hbrBk);

	int c = ~RGB(R, G, 0);

	//SetTextColor(hdc, ~RGB(R, G, 0) & 0x7F7F7F);
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, szText, -1, &rcClient, DT_CENTER | DT_VCENTER | DT_WORDBREAK);

	SelectObject(hdc, hFontOld);

	DeleteObject(hbrBk);
	DeleteObject(hFont);

	EndPaint(hwnd, &ps);
	return 0;
}


//---------------------------------------------------------------------
// WM_USER_NOTIFYICONメッセージ処理
//---------------------------------------------------------------------
static LRESULT OnUserNotifyIcon(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	UINT uID = (UINT)wParam;
	UINT uMouseMsg = (UINT)lParam;

	if (uMouseMsg == WM_RBUTTONUP) {
		HMENU hMenu = LoadMenu(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDR_POPUPMENU));
		if (hMenu) {
			HMENU hMenuPopup = GetSubMenu(hMenu, 0);
			if (hMenuPopup) {
				POINT pt;
				GetCursorPos(&pt);

				SetForegroundWindow(hwnd);
				TrackPopupMenu(hMenuPopup, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
				PostMessage(hwnd, WM_NULL, 0, 0);
			}
		}
	}

	return 0;
}


//---------------------------------------------------------------------
// メインウィンドウのプロシージャ
//---------------------------------------------------------------------
static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT ret;
	
	switch (uMsg) {
	case WM_COMMAND:
		{
			WORD wNotifyCode = HIWORD(wParam);
			WORD wID = LOWORD(wParam);
			HWND hwndCtl = (HWND)lParam;

			if (wID == IDM_POPUP_EXIT) {
				DestroyWindow(hwnd);
				return 0;
			} else if (wID == IDM_POPUP_ABOUT) {
				AboutDlg_DialogBox(GetWindowInstance(hwnd), hwnd);
				return 0;
			}
		}
		break;

	case WM_CREATE:
		// タスクトレイアイコンを追加
		memset(&s_nid, 0, sizeof(s_nid));
		s_nid.cbSize = sizeof(s_nid);
		s_nid.hWnd = hwnd;
		s_nid.uID = 1;
		s_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		s_nid.uCallbackMessage = WM_USER_NOTIFYICON;
		s_nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		lstrcpy(s_nid.szTip, _T("Escape Clock"));
		Shell_NotifyIcon(NIM_ADD, &s_nid);

		// パフォーマンス監視開始
		StartPerf();

		//SetTimer(hwnd, 1, 1000, NULL);
		SetTimer(hwnd, 1, 200, NULL);
		return 0;

	case WM_DESTROY:
		// タスクトレイアイコンを削除
		Shell_NotifyIcon(NIM_DELETE, &s_nid);

		// パフォーマンス監視終了
		StopPerf();

		PostQuitMessage(0);
		return 0;

	case WM_NCHITTEST:
		// システムにHit Testさせて、クライアントの中だったら、逃げるように移動する。
		ret = DefWindowProc(hwnd, uMsg, wParam, lParam);
		if (ret == HTCLIENT) {
			if (++s_nPos > 3) {
				s_nPos = 0;
			}

			MovePosition(hwnd, s_nPos);
		}

		return ret;

	case WM_TIMER:
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		return OnPaint(hwnd, wParam, lParam);

	case WM_USER_NOTIFYICON:
		return OnUserNotifyIcon(hwnd, wParam, lParam);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


//---------------------------------------------------------------------
// メインウィンドウの作成
//---------------------------------------------------------------------
BOOL MainWnd_CreateWindow(HINSTANCE hInstance) {

	// ウィンドウは左上に作成
	// 幅と高さは、250, 100 固定

	s_hwnd = CreateWindowEx(
		WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
		s_pszClassName, 
		_T("Escape Clock"), 
//		WS_OVERLAPPEDWINDOW, 
		WS_POPUPWINDOW,
		0, 0,
		250, 65,
		NULL, NULL, hInstance, NULL);

	if (s_hwnd == NULL) {
		return FALSE;
	}

	ShowWindow(s_hwnd, SW_SHOWNOACTIVATE);

	SetLayeredWindowAttributes(s_hwnd, 0, (255 * 70) / 100, LWA_ALPHA);

	MovePosition(s_hwnd, s_nPos = 0);

	return TRUE;
}


//---------------------------------------------------------------------
// メインウィンドウのウィンドウクラスを登録
//---------------------------------------------------------------------
BOOL MainWnd_RegisterClass(HINSTANCE hInstance) {

	WNDCLASS wc;

	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = s_pszClassName;

	return RegisterClass(&wc);
}
