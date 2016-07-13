/**
 * GlobalMemoryStatus サンプル
 */

#include "kjmutil.h"
#include <stdio.h>
#include "meter.h"
#include "appbar.h"
#include "resource.h"

#define MAINFRM_CLASSNAME	"GMS_MAINFRM"

#define ID_TIMER	101

#define WM_APPBARMSG	(WM_USER + 1)

///////////////////////////////////////////////////////////////////////

HFONT g_hfont;
rkMeter g_meter[2];		// Page, Phys用のメータークラス

/**
 * About dialog procedure
 */

BOOL CALLBACK About_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_INITDIALOG:
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			EndDialog(hwnd, 0);
			return TRUE;
		}
		break;
	}

	return FALSE;
}

/**
 * 保存した座標を読み込んでウィンドウを移動する
 * @param hwnd
 */

void LoadPosition(HWND hwnd, UINT* puEdge)
{
	POINT pt;
	pt.x = kjm::RegGetValue(HKEY_CURRENT_USER, "Software\\kjmutil\\gmf", "window left", 0);
	pt.y = kjm::RegGetValue(HKEY_CURRENT_USER, "Software\\kjmutil\\gmf", "window top", 0);
	*puEdge = kjm::RegGetValue(HKEY_CURRENT_USER, "Software\\kjmutil\\gmf", "edge", ABE_FLOAT);

	kjm::Rectangle rcWindow;
	GetWindowRect(hwnd, &rcWindow);
	POINT ptRB;
	ptRB.x = pt.x + rcWindow.get_Width();
	ptRB.y = pt.y + rcWindow.get_Height();

	kjm::Rectangle rcWork;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, 0);
	if (PtInRect(&rcWork, pt) == 0 && PtInRect(&rcWork, ptRB) == 0) {
		pt.x = pt.y = 0;
	}

	SetWindowPos(hwnd, NULL, pt.x, pt.y, 0, 0, SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER);
}

/**
 * Windowの座標を保存する
 * @param hwnd
 */

void SavePosition(HWND hwnd, UINT uEdge)
{
	kjm::Rectangle rc;
	GetWindowRect(hwnd, &rc);

	kjm::RegSetValue(HKEY_CURRENT_USER, "Software\\kjmutil\\gmf", "window left", rc.get_Left());
	kjm::RegSetValue(HKEY_CURRENT_USER, "Software\\kjmutil\\gmf", "window top", rc.get_Top());
	kjm::RegSetValue(HKEY_CURRENT_USER, "Software\\kjmutil\\gmf", "edge", uEdge);
}

/**
 * 描画に必要なグローバルGDIオブジェクトを作成
 * @param hwnd
 * @return 常にTRUE
 */

BOOL CreateGDIObject(HWND hwnd)
{
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(lf), &lf);
	lf.lfHeight = -11;
	lf.lfCharSet = ANSI_CHARSET;
	lstrcpy(lf.lfFaceName, "Arial");
	g_hfont = CreateFontIndirect(&lf);

	g_meter[0].Offset(5, 5);
	g_meter[1].Offset(5, 5 + g_meter[0].MeterHeight());

	return TRUE;
}

/**
 * 描画に必要だったグローバルGDIオブジェクトを破棄
 * @param hwnd
 * @return 常にTRUE
 */

BOOL DeleteGDIObject(HWND hwnd)
{
	DeleteObject(g_hfont);

	return TRUE;
}

/**
 * WM_PAINTメッセージ処理
 * @param hwnd
 * @param ps
 */

static void OnDraw(HWND hwnd, PAINTSTRUCT &ps)
{
	MEMORYSTATUS ms;
	ms.dwLength = sizeof(ms);
	GlobalMemoryStatus(&ms);

	double phys;		// 物理メモリの空き割合
	double page;		// ページメモリの空き割合
	double virt;		// 仮想メモリの空き割合
	
	phys = (double)ms.dwAvailPhys / ms.dwTotalPhys;
	page = (double)ms.dwAvailPageFile / ms.dwTotalPageFile;
	virt = (double)ms.dwAvailVirtual / ms.dwTotalVirtual;

	char buf[256];

	HPEN hpenWhite = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	HPEN hpenOld;

	hpenOld = (HPEN)SelectObject(ps.hdc, hpenWhite);

	g_meter[0].DrawMeter(ps.hdc, (int)(page * 100));
	g_meter[1].DrawMeter(ps.hdc, (int)(phys * 100));

	DeleteObject(hpenWhite);

	HFONT hfontOld = (HFONT)SelectObject(ps.hdc, g_hfont);
	SetTextColor(ps.hdc, GetSysColor(COLOR_CAPTIONTEXT));
	SetBkMode(ps.hdc, TRANSPARENT);

	sprintf(buf, "%.1f%%", page*100);
	TextOut(ps.hdc, g_meter[0].MeterRight() + 7, g_meter[0].MeterTop(), buf, lstrlen(buf));
	sprintf(buf, "%.1f%%", phys*100);
	TextOut(ps.hdc, g_meter[1].MeterRight() + 7, g_meter[1].MeterTop(), buf, lstrlen(buf));

	sprintf(buf, "%.1f MB", (double)ms.dwAvailPageFile / 1024 / 1024);
	TextOut(ps.hdc, g_meter[0].MeterRight() + 7 + 35, g_meter[0].MeterTop(), buf, lstrlen(buf));
	sprintf(buf, "%.1f MB", (double)ms.dwAvailPhys / 1024 / 1024);
	TextOut(ps.hdc, g_meter[1].MeterRight() + 7 + 35, g_meter[1].MeterTop(), buf, lstrlen(buf));
	
	kjm::Rectangle rc;
	rc.left = g_meter[0].MeterLeft();
	rc.top = g_meter[0].MeterTop();
	rc.right = g_meter[0].MeterRight() + 1;
	rc.bottom = g_meter[1].MeterBottom() + 1;
	rc.Inflate(2, 2);
	DrawEdge(ps.hdc, &rc, BDR_SUNKENOUTER, BF_RECT);

	SelectObject(ps.hdc, hfontOld);
}

/**
 * メインフレームのウィンドウプロシージャ
 * @param hwnd ウィンドウのハンドル
 * @param uMsg メッセージ
 * @param wParam メッセージの追加情報1
 * @param lParam メッセージの追加情報2
 * @return メッセージによりけり
 */

LRESULT CALLBACK MainFrm_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static CAppBar s_ab;

	switch (uMsg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_POPUP_ABOUT:
			{
				HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
				DialogBox(hinst, MAKEINTRESOURCE(IDD_ABOUT), hwnd, About_DlgProc);
			}
			return 0;

		case IDM_POPUP_EXIT:
			// WM_CLOSEを送信して自身を終了させる
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			return 0;
		}
		break;

	case WM_CREATE:
		{
			UINT uEdge;
			kjm::Rectangle rcWnd;

			// 現在のWindow RectangleをAppBarの幅と高さに設定
			GetWindowRect(hwnd, &rcWnd);

			CAppBar::APPBARCTRL abc = {
				hwnd, ABE_FLOAT, WM_APPBARMSG,
				{rcWnd.get_Width(), rcWnd.get_Height()},
				{rcWnd.get_Width(), rcWnd.get_Height()}
			};

			s_ab.Attach(&abc);

			LoadPosition(hwnd, &uEdge);
			CreateGDIObject(hwnd);

			s_ab.Dock(uEdge);

			SetTimer(hwnd, ID_TIMER, 1000, NULL);
		}

		return 0;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;

			BeginPaint(hwnd, &ps);
			OnDraw(hwnd, ps);
			EndPaint(hwnd, &ps);
		}
		return 0;

	case WM_NCHITTEST:
		{
			UINT ret = s_ab.OnNCHitTest(wParam, lParam);
			if (ret == HTCLIENT)
				ret = HTCAPTION;

			return ret;
		}

	case WM_NCRBUTTONDOWN:
		{
			POINTS pts = MAKEPOINTS(lParam);
			HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);

			HMENU hmenu = LoadMenu(hinst, MAKEINTRESOURCE(IDR_POPUP));
			if (hmenu) {
				HMENU hmenuSub = GetSubMenu(hmenu, 0);
				if (hmenuSub) {
					TrackPopupMenu(hmenuSub, TPM_LEFTALIGN, pts.x, pts.y, 0, hwnd, NULL);
				}

				DestroyMenu(hmenu);
			}
		}
		return 0;

	case WM_ENTERSIZEMOVE:
		s_ab.OnEnterSizeMove();
		break;

	case WM_EXITSIZEMOVE:
		s_ab.OnExitSizeMove();
		break;

	case WM_SIZING:
		s_ab.OnSizing((RECT*)lParam);
		break;

	case WM_MOVING:
		s_ab.OnMoving((RECT*)lParam);
		break;

	case WM_WINDOWPOSCHANGED:
		s_ab.OnWindowPosChanged();
		break;

	case WM_ACTIVATE:
		s_ab.OnActivate();
		break;

	case WM_APPBARMSG:
		s_ab.OnAppBarMessage(wParam);
		break;

	case WM_TIMER:
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_ENDSESSION:
		// 終了しないときは、DefWindowProcに後を任せる
		if (!(BOOL)wParam)
			break;

		// 終了するときは、WM_CLOSEの処理に流れる
		// DefWindowProcで、DestroyWindowは、呼ばれないが
		// PostQuitMessageもDestroyWindowも呼ぶ必要が無いので良い。

	case WM_CLOSE:
		KillTimer(hwnd, ID_TIMER);
		SavePosition(hwnd, s_ab.GetEdge());
		DeleteGDIObject(hwnd);
		s_ab.Detach();

		// DestroyWindowの呼び出しは、DefWindowProcに任せる。
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/**
 * プログラム・スタート
 * @param hInstance handle to current instance
 * @param hPrevInstance handle to previous instance
 * @param lpCmdLine pointer to command line
 * @param nCmdShow show state of window
 * @return 正常終了のときは0、エラーのときは-1を返します。
 */

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ウィンドウクラスの登録

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainFrm_WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_ACTIVECAPTION + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = MAINFRM_CLASSNAME;

	if (!RegisterClass(&wc))
		return -1;

	// MainFrmの作成
	// WS_EX_TOOLWINDOWを指定することで、タスクバーに表示されなくなります。

	HWND hwnd = CreateWindowEx(
		WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
		MAINFRM_CLASSNAME, "",
		WS_POPUP | WS_DLGFRAME,
		CW_USEDEFAULT, CW_USEDEFAULT, 210, 43,
		NULL, NULL, hInstance, NULL);

	if (!hwnd)
		return -1;

	// ウィンドウの表示と初回描画

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// メッセージループ
	// while (GetMessage()){} は、エラーでもループを続けるので禁止

	BOOL ret;
	MSG msg;
	while ((ret = GetMessage(&msg, NULL, 0, 0)) != 0) {

		// GetMessageでエラー?
		if (ret == -1)
			break;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
