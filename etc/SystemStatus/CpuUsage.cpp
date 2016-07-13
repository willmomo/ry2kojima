#include "SystemStatus.h"


static HWND s_hwnd;
static kjm::cpuUsage s_cpuUsage;
static INT s_usage = 0;
static INT s_target = 0;

//-----------------------------------------------------------------------------
// 画面描画
//-----------------------------------------------------------------------------
static void Draw(HWND hwnd, HDC hdc) {

	if (s_target > s_usage) {
		s_usage += (((s_target - s_usage) / 2) + 1);
	} else if (s_target < s_usage) {
		s_usage -= (((s_usage - s_target) / 2) - 1);
	
	}
	// 表示用フォントの作成。
	HFONT hFontSys = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	GetObject(hFontSys, sizeof(lf), &lf);
	lf.lfHeight = -MulDiv(10, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	lf.lfWeight = FW_BOLD;
	//lstrcpy(lf.lfFaceName, _T("ＭＳ Ｐゴシック"));
	//lstrcpy(lf.lfFaceName, _T("メイリオ"));
	//lf.lfCharSet = ANSI_CHARSET;
	//lstrcpy(lf.lfFaceName, _T("Arial"));
	//lstrcpy(lf.lfFaceName, _T("GauFontRoot"));

	HFONT hFont = CreateFontIndirect(&lf);
	HFONT hFontOld;

	hFontOld = (HFONT)SelectObject(hdc, hFont);

	RECT rc;
	GetClientRect(hwnd, &rc);

	int R = 255 * min(s_usage * 2, 100) / 100;
	int G = 255 * (100 - max(s_usage - 50, 0) * 2) / 100;
	//HBRUSH hbrBk = CreateSolidBrush(RGB(R, G, 128));
	kjm::Brush brBk(RGB(R, G, 128));
	HBRUSH hbrBkOld = (HBRUSH)SelectObject(hdc, brBk);

	HPEN hpen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	HPEN hpenOld = (HPEN)SelectObject(hdc, hpen);

	RECT rc2;
	rc2.left = rc.left;
	rc2.right = rc.right;
	rc2.top = rc.bottom - ((double)(rc.bottom - rc.top) * (double)s_usage / 100.0);
	rc2.bottom = rc.bottom;
	Rectangle(hdc, rc2.left, rc2.top, rc2.right, rc2.bottom);

	TCHAR buf[32];
	wsprintf(buf, _T("%d%%"), s_usage);

	//SetBkColor(hdc, RGB(R, G, 128));
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, buf, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	SelectObject(hdc, hpenOld);
	SelectObject(hdc, hbrBkOld);

	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
	DeleteObject(hpen);
}


//-----------------------------------------------------------------------------
// WM_TIMER メッセージ処理
//-----------------------------------------------------------------------------
static LRESULT OnTimer(HWND hwnd) {
	static DWORD s_lastCall = 0;

	if ((GetTickCount() - s_lastCall) >= 2000) {
		s_target = s_cpuUsage.GetCPUUsage();
		s_lastCall = GetTickCount();
	}

	// 目標としている使用率と現在表示中の使用率が違うときだけ、再描画を行う。
	if (s_target != s_usage) {
		InvalidateRect(hwnd, NULL, TRUE);
	}

	return 0;
}


//-----------------------------------------------------------------------------
// WM_PAINT メッセージ処理
//-----------------------------------------------------------------------------
static LRESULT OnPaint(HWND hwnd) {
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);

	Draw(hwnd, ps.hdc);

	EndPaint(hwnd, &ps);

	return 0;
}


//-----------------------------------------------------------------------------
// CPU usage 用ウィンドウプロシージャ
//-----------------------------------------------------------------------------
static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT ret;

	switch (uMsg) {
	case WM_CREATE:
		s_cpuUsage.StartPerf();
		SetTimer(hwnd, 1, 200, NULL);
		return 0;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		s_cpuUsage.StopPerf();
		return 0;

	case WM_TIMER:
		return OnTimer(hwnd);

	case WM_PAINT:
		return OnPaint(hwnd);

	case WM_NCHITTEST:
		// システムにHit Testさせて、クライアントの中だったら、逃げるように移動する。
		ret = DefWindowProc(hwnd, uMsg, wParam, lParam);
		if (ret == HTCLIENT) {
			MainFrm_MovePosition();
		}
		return ret;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


//-----------------------------------------------------------------------------
// CPU usage 表示用ウィンドウの作成
//-----------------------------------------------------------------------------
BOOL CpuUsage_CreateWindow(HINSTANCE hInstance, HWND hwndParent) {
	s_hwnd = CreateWindow(CLASSNAME_CPUUSAGE, 
		_T(""),
		WS_BORDER | WS_VISIBLE | WS_CHILD,
		0, 0, 50+1, 122,
		hwndParent, NULL, hInstance, NULL);
	if (s_hwnd == NULL) {
		return FALSE;
	}
	return TRUE;
}


//-----------------------------------------------------------------------------
// CPU usage 表示用子ウィンドウを登録
//-----------------------------------------------------------------------------
BOOL CpuUsage_RegistClass(HINSTANCE hInstance) {
	// ウィンドウクラスの登録
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASSNAME_CPUUSAGE;
	
	return RegisterClassEx(&wc);
}
