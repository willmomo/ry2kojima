#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")


typedef struct {
	SIZE windowSize;	// ウィンドウの幅と高さ
	SIZE paddingLT;		// 左と上のパディング
	SIZE paddingRB;		// 右と下のパディング
	RECT rcLogical;		// パディングを考慮したコントロールの論理的矩形

	void set_logical_rect(LPCRECT prc) {
		rcLogical.left = paddingLT.cx;
		rcLogical.top = paddingLT.cy;
		rcLogical.right = rcLogical.left + (prc->right - prc->left) + paddingRB.cx;
		rcLogical.bottom = rcLogical.top + (prc->bottom - prc->top) + paddingRB.cy;
	};

	/* パディングを含めた幅 */
	int get_logical_width() {
		return paddingLT.cx + windowSize.cx + paddingRB.cx;
	};

	/* パディングを含めた高さ */
	int get_logical_height() {
		return paddingLT.cy + windowSize.cy + paddingRB.cy;
	};
} window_position_ext;


/* ID */
#define IDC_STATIC1		1
#define IDC_STATIC2		2
#define IDC_EDIT1		3
#define IDC_EDIT2		4
#define IDC_BUTTON1		5
#define IDC_BUTTON2		6
#define IDC_LISTVIEW1	7


/* グローバル変数 */
HWND g_hwndMainFrm;
HWND g_hwndStatic1;		// "フォルダ: "を表示するコントロール
HWND g_hwndStatic2;		// "ワイルドカード: "を表示するコントロール
HWND g_hwndEdit1;		// フォルダを入力するコントロール
HWND g_hwndEdit2;		// ワイルドカードを入力するコントロール
HWND g_hwndButton1;		// "実行" ボタン
HWND g_hwndButton2;		// "ファイルに保存" ボタン
HWND g_hwndListView1;	// dir の結果表示コントロール


/* ウィンドウの位置を決める拡張情報 */
window_position_ext g_wpStatic1;
window_position_ext g_wpStatic2;


/* MainFrm の WM_CREATE メッセージ処理 */
LRESULT MainFrm_OnCreate(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	// ListView コントロールを使用できるようにする
	InitCommonControls();

	/* "フォルダ: "を表示するラベル */
	g_hwndStatic1 = CreateWindow(_T("STATIC"), _T("フォルダ: "),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		0, 0, 0, 0,
		hwnd, (HMENU)IDC_STATIC1, GetWindowInstance(hwnd), NULL);
	if (g_hwndStatic1 == NULL) {
		return -1;
	}

	/* "ワイルドカード: "を表示するラベル */
	g_hwndStatic2 = CreateWindow(_T("STATIC"), _T("ワイルドカード: "),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		0, 0, 0, 0,
		hwnd, (HMENU)IDC_STATIC2, GetWindowInstance(hwnd), NULL);
	if (g_hwndStatic2 == NULL) {
		return -1;
	}

	/* フォルダを入力するエディット */
	g_hwndEdit1 = CreateWindow(_T("EDIT"), _T(""),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		130, 0, 260, 20,
		hwnd, (HMENU)IDC_EDIT1, GetWindowInstance(hwnd), NULL);
	if (g_hwndEdit1 == NULL) {
		return -1;
	}

	/* フォルダを入力するエディット */
	g_hwndEdit2 = CreateWindow(_T("EDIT"), _T(""),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		130, 20, 260, 20,
		hwnd, (HMENU)IDC_EDIT2, GetWindowInstance(hwnd), NULL);
	if (g_hwndEdit2 == NULL) {
		return -1;
	}

	// 実行ボタン
	g_hwndButton1 = CreateWindow(_T("BUTTON"), _T("実行"),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		0, 40, 130, 20,
		hwnd, (HMENU)IDC_BUTTON1, GetWindowInstance(hwnd), NULL);
	if (g_hwndButton1 == NULL) {
		return -1;
	}

	// 結果表示コントロール
	g_hwndListView1 = CreateWindow(WC_LISTVIEW, _T(""),
		WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT,
		0, 60, 390, 100,
		hwnd, (HMENU)IDC_LISTVIEW1, GetWindowInstance(hwnd), NULL);
	if (g_hwndListView1 == NULL) {
		return -1;
	}

	// ファイルに保存ボタン
	g_hwndButton2 = CreateWindow(_T("BUTTON"), _T("ファイルに保存"),
		WS_CHILD | WS_VISIBLE,
		0, 160, 130, 20,
		hwnd, (HMENU)IDC_BUTTON2, GetWindowInstance(hwnd), NULL);
	if (g_hwndButton2 == NULL) {
		return -1;
	}

	return 0;
}


/* MainFrm の WM_SIZE メッセージ処理 */
LRESULT MainFrm_OnSize(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	TCHAR windowText[512];
	HDC hdc;

	// Static1 を適切なサイズにリサイズ
	GetWindowText(g_hwndStatic1, windowText, 512);
	hdc = GetWindowDC(g_hwndStatic1);
	GetTextExtentPoint32(hdc, windowText, lstrlen(windowText), &g_wpStatic1.windowSize);
	ReleaseDC(g_hwndStatic1, hdc);
	g_wpStatic1.paddingLT.cx = g_wpStatic1.paddingLT.cy = 3;
	g_wpStatic1.paddingRB.cx = g_wpStatic1.paddingRB.cy = 3;
	MoveWindow(g_hwndStatic1, 
		g_wpStatic1.paddingLT.cx,
		g_wpStatic1.paddingLT.cy,
		g_wpStatic1.windowSize.cx,
		g_wpStatic1.windowSize.cy,
		TRUE);

	// Static2 を適切なサイズにリサイズ
	GetWindowText(g_hwndStatic2, windowText, 512);
	hdc = GetWindowDC(g_hwndStatic2);
	GetTextExtentPoint32(hdc, windowText, lstrlen(windowText), &g_wpStatic2.windowSize);
	ReleaseDC(g_hwndStatic2, hdc);
	g_wpStatic2.paddingLT.cx = g_wpStatic2.paddingLT.cy = 3;
	g_wpStatic2.paddingRB.cx = g_wpStatic2.paddingRB.cy = 3;
	MoveWindow(g_hwndStatic2, 
		g_wpStatic2.paddingLT.cx,
		g_wpStatic1.get_logical_height() + g_wpStatic2.paddingLT.cy,
		g_wpStatic2.windowSize.cx,
		g_wpStatic2.windowSize.cy,
		TRUE);

	return 0;
}


/* MainFrm のウィンドウ・プロシージャ */
LRESULT CALLBACK MainFrm_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT nResult;

	switch (uMsg) {
	case WM_CREATE:
		nResult = MainFrm_OnCreate(hwnd, wParam, lParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		nResult = 0;
		break;

	case WM_SIZE:
		nResult = MainFrm_OnSize(hwnd, wParam, lParam);
		break;

	default:
		nResult = DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	}

	return nResult;
}


/* プログラム・スタート */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASS wc;
	wc.style = 0;
	wc.lpfnWndProc = MainFrm_WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _T("MAINFRM_WDIR");

	if (!RegisterClass(&wc)) {
		return -1;
	}

	g_hwndMainFrm = CreateWindow(_T("MAINFRM_WDIR"), _T("wdir"), WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	if (g_hwndMainFrm == NULL) {
		return -1;
	}

	ShowWindow(g_hwndMainFrm, nCmdShow);

	MSG msg;
	while (1) {
		BOOL bRet = GetMessage(&msg, NULL, 0, 0);
		if (bRet == 0 || bRet == -1) {
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
