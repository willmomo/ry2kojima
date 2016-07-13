//---------------------------------------------------------------------
// GetSystemPowerStatus を使って、バッテリの変化を表示・記録する。
//
// <備考>
// アイコンは、 http://www.iconfinder.net/ から、拾ってきました。
//---------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "resource.h"


#define CLASSNAME_MAINFRM	_T("MainFrmGetSystemPowerStatus")


#define RGB_BLACK	(RGB(0, 0, 0))


//---------------------------------------------------------------------
// TextColor を受け取り、文字列長を省略できる TextOut 関数
//---------------------------------------------------------------------
void TextOutEx(HDC hdc, int nXStart, int nYStart, LPCTSTR lpString, COLORREF crTextColor) {
	SetTextColor(hdc, crTextColor);
	TextOut(hdc, nXStart, nYStart, lpString, lstrlen(lpString));
}


//---------------------------------------------------------------------
// SYSTEM_POWER_STATUSの保存
//---------------------------------------------------------------------
void SaveSystemPowerStatus(DWORD dwPowerEvent, DWORD dwData) {
	FILE* fp = _tfopen(_T("SystemPowerStatus.csv"), _T("a+"));
	if (fp) {
		SYSTEM_POWER_STATUS sps;
		GetSystemPowerStatus(&sps);

		SYSTEMTIME stNow;
		GetLocalTime(&stNow);
		_ftprintf(fp, _T("%d/%d/%d %d:%02d:%02d,%d,%d,%d,%d,%d,%d,%d\n"),
			stNow.wYear, stNow.wMonth, stNow.wDay,
			stNow.wHour, stNow.wMinute, stNow.wSecond,
			dwPowerEvent, dwData,
			sps.ACLineStatus,
			sps.BatteryFlag,
			sps.BatteryLifePercent,
			sps.BatteryLifeTime,
			sps.BatteryFullLifeTime
		);

		fclose(fp);
	}
}


//---------------------------------------------------------------------
// 汎用的描画処理
//---------------------------------------------------------------------
void Draw(HWND hwnd, HDC hdc) {
	COLORREF color1;
	TCHAR txtBuf[512];
	POINT pt = {4, 4};
	SIZE size;

	// 電源状態を取得する。
	SYSTEM_POWER_STATUS sps;
	GetSystemPowerStatus(&sps);

	// 表示用フォントの作成。
	HFONT hFontSys = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	GetObject(hFontSys, sizeof(lf), &lf);
	lf.lfHeight = -MulDiv(11, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	lf.lfWeight = FW_BOLD;
	//lstrcpy(lf.lfFaceName, _T("ＭＳ Ｐゴシック"));
	//lstrcpy(lf.lfFaceName, _T("メイリオ"));
	//lf.lfCharSet = ANSI_CHARSET;
	//lstrcpy(lf.lfFaceName, _T("Arial"));
	//lstrcpy(lf.lfFaceName, _T("GauFontRoot"));

	HFONT hFont = CreateFontIndirect(&lf);
	HFONT hFontOld;

	hFontOld = (HFONT)SelectObject(hdc, hFont);

	// 一番長いテキストを基準に、座標の変化量を決める
	GetTextExtentPoint32(hdc, _T(" Charge status"), lstrlen(_T(" Charge status")), &size);
	size.cy += 1;

	// バッテリーが無いときは、画面中央にNO SYSTEM BATTERYと描画
	if (sps.BatteryFlag & BATTERY_FLAG_NO_BATTERY) {
		RECT rcClient;
		GetClientRect(hwnd, &rcClient);
		
		SetTextColor(hdc, RGB(220, 0, 0));
		DrawText(hdc, _T("NO SYSTEM BATTERY"), -1, &rcClient, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		SelectObject(hdc, hFontOld);
		DeleteObject(hFont);

		return;
	}

	// AC status の描画

	switch (sps.ACLineStatus) {
	case AC_LINE_OFFLINE:
		lstrcpy(txtBuf, _T("Offline"));
		color1 = RGB(220, 0, 0);
		break;
	case AC_LINE_ONLINE:
		lstrcpy(txtBuf, _T("Online"));
		color1 = RGB(0, 180, 0);
		break;
	case AC_LINE_BACKUP_POWER:
		lstrcpy(txtBuf, _T("Backuped"));
		color1 = RGB(200, 150, 0);
		break;
	case AC_LINE_UNKNOWN:
		lstrcpy(txtBuf, _T("Unknown"));
		color1 = RGB(200, 150, 0);
		break;
	default:
		wsprintf(txtBuf, _T("(raw value : %d)"), sps.ACLineStatus);
		color1 = RGB_BLACK;
		break;
	}
	TextOutEx(hdc, pt.x, pt.y, _T("AC status"), RGB_BLACK);
	TextOutEx(hdc, pt.x + size.cx + 12, pt.y, txtBuf, color1);

	pt.y += size.cy;

	TextOutEx(hdc, pt.x, pt.y, _T("Battery"), RGB_BLACK);
	
	pt.y += size.cy;

	// Charge status の描画

	TextOutEx(hdc, pt.x, pt.y, _T(" Charge status"), RGB_BLACK);
	TextOutEx(hdc, pt.x + size.cx + 12, pt.y, _T("Charging"), (sps.BatteryFlag & BATTERY_FLAG_CHARGING) ? RGB(220, 170, 0) : RGB(210, 210, 210));

	pt.y += size.cy;

	// BatteryLifePercent の描画

	if (sps.BatteryFlag & BATTERY_FLAG_HIGH) {
		wsprintf(txtBuf, _T("%d%%%s"), sps.BatteryLifePercent, _T(" [High]"));
		color1 = RGB(0, 180, 0);
	} else if (sps.BatteryFlag & BATTERY_FLAG_LOW) {
		wsprintf(txtBuf, _T("%d%%%s"), sps.BatteryLifePercent, _T(" [Low]"));
		color1 = RGB(220, 170, 0);
	} else if (sps.BatteryFlag & BATTERY_FLAG_CRITICAL) {
		wsprintf(txtBuf, _T("%d%%%s"), sps.BatteryLifePercent, _T(" [Critical]"));
		color1 = RGB(220, 0, 0);
	} else {
		wsprintf(txtBuf, _T("%d%%%s"), sps.BatteryLifePercent, _T(" [Normal]"));
		color1 = RGB_BLACK;
	}

	TextOutEx(hdc, pt.x, pt.y, _T(" Life Percent"), RGB_BLACK);
	TextOutEx(hdc, pt.x + size.cx + 12, pt.y, txtBuf, color1);
	
	pt.y += size.cy;

	// BatteryLifeTime の描画

	if (sps.BatteryLifeTime == BATTERY_LIFE_UNKNOWN) {
		wsprintf(txtBuf, _T("unknown"));
	} else if (sps.BatteryLifeTime >= 3600) {
		wsprintf(txtBuf, _T("%dh %dm %ds"), sps.BatteryLifeTime / 3600, (sps.BatteryLifeTime % 3600) / 60, (sps.BatteryLifeTime % 3600) % 60);
	} else if (sps.BatteryLifeTime >= 60) {
		wsprintf(txtBuf, _T("%dm %ds"), sps.BatteryLifeTime / 60, sps.BatteryLifeTime % 60);
	} else {
		wsprintf(txtBuf, _T("%d sec"), sps.BatteryLifeTime);
	}

	TextOutEx(hdc, pt.x, pt.y, _T(" Life Time"), RGB_BLACK);
	TextOutEx(hdc, pt.x + size.cx + 12, pt.y, txtBuf, RGB_BLACK);

	pt.y += size.cy;

	// BatteryFullLifeTime の描画

	DWORD dwflt;
	if (sps.BatteryFullLifeTime == BATTERY_LIFE_UNKNOWN && sps.BatteryLifeTime != BATTERY_LIFE_UNKNOWN) {
		dwflt = (sps.BatteryLifeTime * 100) / sps.BatteryLifePercent;
	} else {
		dwflt = sps.BatteryFullLifeTime;
	}

	if (dwflt == BATTERY_LIFE_UNKNOWN) {
		wsprintf(txtBuf, _T("unknown"));
	} else if (dwflt >= 3600) {
		wsprintf(txtBuf, _T("%dh %dm %ds"), dwflt / 3600, (dwflt % 3600) / 60, (dwflt % 3600) % 60);
	} else if (dwflt >= 60) {
		wsprintf(txtBuf, _T("%dm %ds"), dwflt / 60, dwflt % 60);
	} else {
		wsprintf(txtBuf, _T("%d sec"), dwflt);
	}

	TextOutEx(hdc, pt.x, pt.y, _T(" Full Life Time"), RGB_BLACK);
	TextOutEx(hdc, pt.x + size.cx + 12, pt.y, txtBuf, RGB_BLACK);

	pt.y += size.cy;

	// BatteryLifeTime からの推測値のときは、[estimate time]の表示を追加。

	if (sps.BatteryFullLifeTime == BATTERY_LIFE_UNKNOWN && sps.BatteryLifeTime != BATTERY_LIFE_UNKNOWN) {
		TextOutEx(hdc, pt.x + size.cx + 12, pt.y, _T("[estimate time]"), RGB_BLACK);
	}

	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
}


//---------------------------------------------------------------------
// WM_CREATEメッセージ処理
//---------------------------------------------------------------------
LRESULT OnCreate(HWND hwnd, WPARAM wParam, LPARAM lParam) {

	// 起動時の電源状態を記録
	SaveSystemPowerStatus(0, 0);

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

	return 0;
}


//---------------------------------------------------------------------
// WM_PAINTメッセージ処理
//---------------------------------------------------------------------
LRESULT OnPaint(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;

	hdc = BeginPaint(hwnd, &ps);

	Draw(hwnd, hdc);

	EndPaint(hwnd, &ps);
	return 0;
}


//---------------------------------------------------------------------
// WM_POWERBROADCASTメッセージ処理
//---------------------------------------------------------------------
LRESULT OnPowerBroadcast(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	DWORD dwPowerEvent = (DWORD)wParam;
	DWORD dwData = (DWORD)lParam;

	SaveSystemPowerStatus(dwPowerEvent, dwData);

	// ウィンドウの再描画を促す
	InvalidateRect(hwnd, NULL, TRUE);
	return TRUE;
}


//---------------------------------------------------------------------
// ウィンドウプロシージャ
//---------------------------------------------------------------------
LRESULT CALLBACK MainFrm_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CREATE:
		return OnCreate(hwnd, wParam, lParam);

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		return OnPaint(hwnd, wParam, lParam);

	case WM_POWERBROADCAST:
		return OnPowerBroadcast(hwnd, wParam, lParam);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


//---------------------------------------------------------------------
// プログラムスタート
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// ウィンドウクラスの登録
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainFrm_WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_MAIN), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	wc.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_MAIN), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASSNAME_MAINFRM;
	if (!RegisterClassEx(&wc)) {
		return -1;
	}

	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	//DWORD dwStyle = WS_OVERLAPPEDWINDOW;

	RECT rc = {0, 0, 243, 122};
	AdjustWindowRect(&rc, dwStyle, FALSE);

	// メインフレームの作成
	HWND hwnd = CreateWindow(CLASSNAME_MAINFRM, 
		_T("System Power Status"),
		dwStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
		NULL, NULL, hInstance, NULL);
	if (hwnd == NULL) {
		return -1;
	}

	// ウィンドウを指定の状態に変更
	ShowWindow(hwnd, nCmdShow);

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
