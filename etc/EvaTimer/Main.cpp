/////////////////////////////////////////////////////////////////////////////
// EvaTimer Ver.1.3

#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include "resource.h"
#include "definc.h"

/////////////////////////////////////////////////////////////////////////////
// グローバル変数

TCHAR gszAppName[64];	// アプリケーション名
TCHAR gszWinName[64];	// ウィンドウのタイトル

/////////////////////////////////////////////////////////////////////////////
// About ダイアログ プロシージャ

BOOL CALLBACK AboutDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WORD wID;

	switch (uMsg) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		wID = LOWORD(wParam);
		if (wID == IDOK) {
			EndDialog(hwndDlg, 0);
			return TRUE;
		} else if (wID == IDCANCEL) {
			EndDialog(hwndDlg, -1);
			return TRUE;
		}
		break;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Property ダイアログ プロシージャ

BOOL CALLBACK PropertyDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WORD wID;

	switch (uMsg) {
	case WM_INITDIALOG:
		PROP_OnInitDialog(hwndDlg);
		return TRUE;
	case WM_COMMAND:
		wID = LOWORD(wParam);
		if (wID == IDOK) {
			PROP_OnOk(hwndDlg);
			EndDialog(hwndDlg, 0);
			return TRUE;
		} else if (wID == IDCANCEL) {
			EndDialog(hwndDlg, -1);
			return TRUE;
		}
		break;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// メイン ウィンドウ プロシージャ

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WORD wID;
	WORD wNotifyCode;

	switch (uMsg) {
	case WM_CREATE:
		OnCreate(hwnd);
		break;
	case WM_POWERBROADCAST:
		OnPowerBroadcast(hwnd, wParam, lParam);
		break;
	case WM_COMMAND:
		wID = LOWORD(wParam);
		wNotifyCode = HIWORD(wParam);
		if (wID == IDM_HELP_ABOUT) {
			DialogBox((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
				"About", hwnd, (DLGPROC)AboutDialogProc);
		} else if (wID == IDM_CLOCK) {
			OnClock(hwnd);
		} else if (wID == IDM_EXIT) {
			DestroyWindow(hwnd);
		} else if (wID == IDM_ACTION_START) {
			OnActionStart(hwnd);
		} else if (wID == IDM_ACTION_PROPERTY) {
			OnActionProperty(hwnd);
		} else if (wID == IDM_ACTION_TOPMOST) {
			OnActionTopmost(hwnd);
		} else if (wID == IDW_START) {
			if (wNotifyCode == BN_CLICKED) {
				START_OnBnClicked(hwnd, (HWND)lParam);
			} else {
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
		} else if (wID == IDW_SETTEI) {
			if (wNotifyCode == BN_CLICKED) {
				SETTEI_OnBnClicked(hwnd, (HWND)lParam);
			} else {
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
		} else {
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		break;
	case WM_TIMER:
		OnTimer(hwnd);
		break;
	case WM_PAINT:
		OnPaint(hwnd);
		break;
	case WM_DESTROY:
		OnDestroy(hwnd);
		break;
	case WM_DRAWITEM:
		OnDrawItem(hwnd, (UINT)wParam, (LPDRAWITEMSTRUCT)lParam);
		break;
	case WM_LBUTTONDOWN:
		OnLButtonDown(hwnd, wParam, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		OnLButtonUp(hwnd, wParam, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(hwnd, wParam, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONDOWN:
		OnRButtonDown(hwnd, wParam, LOWORD(lParam), HIWORD(lParam));
		break;
	case WMU_TIMEUP:
		OnUTimeUp(hwnd);
		break;
	case WMU_NOTIFYICON:
		OnUNotifyIcon(hwnd, wParam, lParam);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// from CONTROL ウィンドウ プロシージャ

LRESULT CALLBACK FromWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CREATE:
		FROM_OnCreate(hwnd);
		break;
	case WM_DESTROY:
		FROM_OnDestroy(hwnd);
		break;
	case WM_TIMER:
		FROM_OnTimer(hwnd);
		break;
	case WM_PAINT:
		FROM_OnPaint(hwnd);
		break;
	case WMU_FROMCONTROL:
		FROM_OnUFromControl(hwnd, wParam, lParam);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 吹き出し ウィンドウ プロシージャ

LRESULT CALLBACK FukiWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CREATE:
		FUKI_OnCreate(hwnd);
		break;
	case WM_PAINT:
		FUKI_OnPaint(hwnd);
		break;
	case WM_DESTROY:
		FUKI_OnDestroy(hwnd);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// インスタンスの初期化

inline BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	DWORD dwStyle = WS_EX_TOOLWINDOW | ((gfTopMost) ? WS_EX_TOPMOST : 0);
	HWND hwnd = CreateWindowEx(
		dwStyle,
		gszAppName,
		gszWinName,
		WS_POPUPWINDOW,
		gdwLeft,
		gdwTop,
		MAIN_WINDOW_WIDTH,
		MAIN_WINDOW_HEIGHT,
		NULL,
		NULL,
		hInstance,
		0);

	if (!hwnd)
		return FALSE;

	ShowWindow(hwnd, nCmdShow);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// アプリケーションの初期化

BOOL InitApplication(HINSTANCE hInstance)
{
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPL));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = gszAppName;

	BOOL fRtn = RegisterClass(&wc);

	wc.style = 0;
	wc.lpfnWndProc = FromWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "FROM_CONTROL";

	fRtn |= RegisterClass(&wc);

	wc.style = 0;
	wc.lpfnWndProc = FukiWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "FUKIDASHI";

	fRtn |= RegisterClass(&wc);

	return fRtn;
}

/////////////////////////////////////////////////////////////////////////////
// グローバル変数の初期化

inline void InitGlobal(HINSTANCE hInstance)
{
	LoadString(hInstance, IDS_APPNAME, gszAppName, sizeof(gszAppName));
	LoadString(hInstance, IDS_WINNAME, gszWinName, sizeof(gszWinName));

	GetRegistory();
}

/////////////////////////////////////////////////////////////////////////////
// プログラム スタート

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	HACCEL haccel;

	InitGlobal(hInstance);				// グローバル変数の初期化

	if (hPrevInstance == NULL)			// ほかのインスタンス?
		if (!InitApplication(hInstance))	// 共有の項目
			return FALSE;			// 初期化に失敗

	if (!InitInstance(hInstance, nCmdShow))		// このインスタンスの初期化
		return FALSE;

	haccel = LoadAccelerators(hInstance, "default");	// アクセラレータテーブルをロードする

	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, haccel, &msg)) {
			TranslateMessage(&msg);		// 仮想キー コードを変換
			DispatchMessage(&msg);		// メッセージをウィンドウにディスパッチ
		}
	}

	return (int)msg.wParam;			// PostQuitMessageの戻り値
}
