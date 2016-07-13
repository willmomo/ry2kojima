#include "SystemStatus.h"


#define WM_USER_NOTIFYICON	(WM_USER + 1)

static int s_nPos = 0;			// 0:左上 1:右上 2:右下 3:左下
static NOTIFYICONDATA s_nid;	// タスクトレイアイコンの情報
static HWND s_hwnd;


//-----------------------------------------------------------------------------
// Window をスムーズに移動させる
//-----------------------------------------------------------------------------
typedef struct {
	int xFrom, yFrom;
	int xTo, yTo;
} MOVEWINDOWTO_INFO;

static bool s_mooving = false;

VOID CALLBACK MoveWindowTo_TimerFunc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime) {
	MOVEWINDOWTO_INFO* pMwtInfo = (MOVEWINDOWTO_INFO*)idEvent;

	if (pMwtInfo->xFrom == pMwtInfo->xTo && pMwtInfo->yFrom == pMwtInfo->yTo) {
		KillTimer(hwnd, idEvent);
		delete pMwtInfo;
		s_mooving = false;
		return;
	}

	if (pMwtInfo->xFrom != pMwtInfo->xTo) {
		pMwtInfo->xFrom = pMwtInfo->xFrom + min((abs((pMwtInfo->xTo - pMwtInfo->xFrom) / 2) + 1), 100) * ((pMwtInfo->xTo > pMwtInfo->xFrom) ? 1 : -1);
	}

	if (pMwtInfo->yFrom != pMwtInfo->yTo) {
		pMwtInfo->yFrom = pMwtInfo->yFrom + min((abs((pMwtInfo->yTo - pMwtInfo->yFrom) / 2) + 1), 100) * ((pMwtInfo->yTo > pMwtInfo->yFrom) ? 1 : -1);
	}

	SetWindowPos(hwnd, NULL, pMwtInfo->xFrom, pMwtInfo->yFrom, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void MoveWindowTo(HWND hwnd, int xTo, int yTo) {
	RECT rcFrom;

	GetWindowRect(hwnd, &rcFrom);

	MOVEWINDOWTO_INFO* pMwtInfo = new MOVEWINDOWTO_INFO;

	pMwtInfo->xFrom = rcFrom.left;
	pMwtInfo->yFrom = rcFrom.top;
	pMwtInfo->xTo = xTo;
	pMwtInfo->yTo = yTo;

	SetTimer(hwnd, (UINT)pMwtInfo, 100, MoveWindowTo_TimerFunc);
}


//-----------------------------------------------------------------------------
// メインウィンドウを所定の隅に移動する
//-----------------------------------------------------------------------------
static void MovePosition(HWND hwnd, int nPos) {
	if (s_mooving) return;	// 移動中は何もしない
	s_mooving = true;

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

	//MoveWindow(hwnd, x, y, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, TRUE);
	MoveWindowTo(hwnd, x, y);
}

void MainFrm_MovePosition() {
	if (s_mooving) return;	// 移動中は何もしない

	if (++s_nPos > 3) {
		s_nPos = 0;
	}

	MovePosition(s_hwnd, s_nPos);
}

//-----------------------------------------------------------------------------
// WM_CREATE メッセージ処理
//-----------------------------------------------------------------------------
static LRESULT OnCreate(HWND hwnd, WPARAM wParam, LPARAM lParam) {

	if (!CpuUsage_CreateWindow(GetWindowInstance(hwnd), hwnd)) return -1;
	
	if (!SystemPowerStatus_CreateWindow(GetWindowInstance(hwnd), hwnd)) return -1;

	RECT rcWork;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, 0);

	RECT rc;
	GetWindowRect(hwnd, &rc);

	SetWindowPos(
		hwnd,
		NULL,
		rcWork.right - (rc.right - rc.left),
		rcWork.bottom - (rc.bottom - rc.top),
		0, 0,
		SWP_NOREDRAW | SWP_NOSIZE | SWP_NOZORDER);

	// タスクトレイアイコンを追加
	memset(&s_nid, 0, sizeof(s_nid));
	s_nid.cbSize = sizeof(s_nid);
	s_nid.hWnd = hwnd;
	s_nid.uID = 1;
	s_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	s_nid.uCallbackMessage = WM_USER_NOTIFYICON;
	s_nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	lstrcpy(s_nid.szTip, _T("System Status"));
	Shell_NotifyIcon(NIM_ADD, &s_nid);

	return 0;
}


//-----------------------------------------------------------------------------
// WM_USER_NOTIFYICONメッセージ処理
//-----------------------------------------------------------------------------
static LRESULT OnUserNotifyIcon(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	UINT uID = (UINT)wParam;
	UINT uMouseMsg = (UINT)lParam;

	if (uMouseMsg == WM_LBUTTONDOWN) {
		SetForegroundWindow(hwnd);
	} else if (uMouseMsg == WM_RBUTTONUP) {
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


//-----------------------------------------------------------------------------
// ウィンドウプロシージャ
//-----------------------------------------------------------------------------
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
			}
		}
		break;

	case WM_CREATE:
		return OnCreate(hwnd, wParam, lParam);

	case WM_DESTROY:
		// タスクトレイアイコンを削除
		Shell_NotifyIcon(NIM_DELETE, &s_nid);

		PostQuitMessage(0);
		return 0;

	//case WM_PAINT:
	//	return OnPaint(hwnd, wParam, lParam);

	case WM_POWERBROADCAST:
		//return OnPowerBroadcast(hwnd, wParam, lParam);
		SystemPowerStatusWnd_PostMessage(uMsg, wParam, lParam);
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

	case WM_USER_NOTIFYICON:
		return OnUserNotifyIcon(hwnd, wParam, lParam);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


//-----------------------------------------------------------------------------
// メインフレームを登録
//-----------------------------------------------------------------------------
BOOL MainFrm_RegistClass(HINSTANCE hInstance) {
	// ウィンドウクラスの登録
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_MAIN), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	wc.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_MAIN), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASSNAME_MAINFRM;

	return RegisterClassEx(&wc);
}


//---------------------------------------------------------------------
// プログラムスタート
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	if (MainFrm_RegistClass(hInstance) == FALSE) return -1;
	if (CpuUsage_RegistClass(hInstance) == FALSE) return -1;
	if (SystemPowerStatus_RegistClass(hInstance) == FALSE)	return -1;

	DWORD dwStyle = WS_POPUP;
	//DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	//DWORD dwStyle = WS_OVERLAPPEDWINDOW;

	RECT rc = {0, 0, 243 + 50, 122};
	AdjustWindowRect(&rc, dwStyle, FALSE);

	// メインフレームの作成
	s_hwnd = CreateWindowEx(
		//WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
		WS_EX_LAYERED | WS_EX_TOOLWINDOW,
		CLASSNAME_MAINFRM, 
		_T("System Status"),
		dwStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
		NULL, NULL, hInstance, NULL);
	if (s_hwnd == NULL) {
		return -1;
	}

	kjm::util::setLayeredWindowAttributes(s_hwnd, 0, (255 * 85) / 100, LWA_ALPHA);

	s_nPos = 2;
	MovePosition(s_hwnd, s_nPos);

	// ウィンドウを指定の状態に変更
	ShowWindow(s_hwnd, nCmdShow);

	// メッセージループ
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
