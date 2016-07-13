#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <pbt.h>
#include "x.h"
#include "resource.h"
#include "definc.h"

static UINT sguIDTimer = 0;
static UINT sguCnt = 0;
static UINT sguTimeOut = 10;
HWND ghwndFukidashi;
LPCTSTR glpSerifu;
HRGN ghrgn;

/////////////////////////////////////////////////////////////////////////////
// WM_CREATE処理

void FROM_OnCreate(HWND hwnd)
{
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
	ghwndFukidashi = CreateWindowEx(
		0,
		"FUKIDASHI", "",
		WS_POPUP,
		0, 0, 220+2, 60+2, hwnd, NULL, hInstance, NULL);

	HRGN hrgn = CreateRectRgn(0, 0, 10, 10);
	HRGN hrgn1 = CreateRoundRectRgn(20, 0, 220, 60, 15, 15);
	POINT points[3] = {{20, 30},{0, 60},{20,50}};
	HRGN hrgn2 = CreatePolygonRgn(points, 3, WINDING);
	CombineRgn(hrgn, hrgn1, hrgn2, RGN_OR);
	DeleteObject(hrgn1);
	DeleteObject(hrgn2);

	ghrgn = hrgn;
	SetWindowRgn(ghwndFukidashi, hrgn, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// WM_PAINT処理

void FROM_OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	XRect rc;
	XRect rc2;

	BeginPaint(hwnd, &ps);
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
	HBITMAP hbmWaku = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_WAKU));
	HBITMAP hbm = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_MAYA));
	HDC hdcMem = CreateCompatibleDC(ps.hdc);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmWaku);

	BitBlt(ps.hdc, 0, 0, 138, 5, hdcMem, 0, 0, SRCCOPY);
	BitBlt(ps.hdc, 0, 5, 5, 143, hdcMem, 0, 5, SRCCOPY);
	BitBlt(ps.hdc, 133, 5, 5, 143, hdcMem, 133, 5, SRCCOPY);
	BitBlt(ps.hdc, 5, 133, 128, 15, hdcMem, 5, 133, SRCCOPY);

	SelectObject(hdcMem, hbm);

	BitBlt(ps.hdc, 5, 5, 123, 128, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);
	DeleteObject(hbm);
	DeleteObject(hbmWaku);
	EndPaint(hwnd, &ps);
}

/////////////////////////////////////////////////////////////////////////////
// WM_TIMER処理

void FROM_OnTimer(HWND hwnd)
{
	sguCnt++;
	if (sguCnt >= sguTimeOut) {
		ShowWindow(hwnd, SW_HIDE);
		if (glpSerifu) {
			ShowWindow(ghwndFukidashi, SW_HIDE);
			glpSerifu = NULL;
		}
		KillTimer(hwnd, ID_TIMER2);
		sguIDTimer = 0;
	} else {
		// 口パクアニメを行う
	}
}

/////////////////////////////////////////////////////////////////////////////
// WMU_FROMCONTROL処理

void FROM_OnUFromControl(HWND hwnd, DWORD dw1, DWORD dw2)
{
	if (sguIDTimer == 0) {
		XRect rc;

		// デスクトップのサイズを取得して
		// 中心にウィンドウを移動
		GetWindowRect(GetDesktopWindow(), &rc);	
		SetWindowPos(hwnd, NULL, rc.right / 2 - 70, rc.bottom / 2 - 75,
			0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);

		// FROMウィンドウの位置(グローバル)を取得して
		// FUKIDASHIウィンドウを右上に移動する
		GetWindowRect(hwnd, &rc);
		SetWindowPos(ghwndFukidashi, NULL, rc.right, rc.top, 0, 0,
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW);

		glpSerifu = (LPCTSTR)dw2;

		if (glpSerifu)
			ShowWindow(ghwndFukidashi, SW_SHOW);
		sguIDTimer = SetTimer(hwnd, ID_TIMER2, 500, NULL);
		sguCnt = 0;
		sguTimeOut = dw1;
	}
}

/////////////////////////////////////////////////////////////////////////////
// WM_DESTROY処理

void FROM_OnDestroy(HWND hwnd)
{
	if (sguIDTimer)	KillTimer(hwnd, ID_TIMER2);
	PostQuitMessage(0);
}
