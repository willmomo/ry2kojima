#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <pbt.h>
#include "x.h"
#include "resource.h"
#include "definc.h"

static HWND ghwndEdit;

/////////////////////////////////////////////////////////////////////////////
// WM_CREATEèàóù

void FUKI_OnCreate(HWND hwnd)
{
	XRect rc;
	GetClientRect(hwnd, &rc);
	rc.InflateRect(-5, -5);

	ghwndEdit = CreateWindow(
		"EDIT",
		"",
		WS_CHILD | WS_VISIBLE | ES_MULTILINE,
		25, 5,
		190-2, 50-2,
		hwnd,
		(HMENU)IDW_EDIT,
		(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
		NULL);
}

/////////////////////////////////////////////////////////////////////////////
// WM_PAINTèàóù

void FUKI_OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);

	if (glpSerifu) {
		HRGN hrgn = CreateRectRgn(0, 0, 10, 10);
		GetWindowRgn(hwnd, hrgn);
		FrameRgn(ps.hdc, hrgn, (HBRUSH)GetStockObject(BLACK_BRUSH), 1, 1);
		DeleteObject(hrgn);
		XRect rc;
		GetClientRect(hwnd, &rc);
		rc.InflateRect(-5, -5);
		SetWindowText(ghwndEdit, glpSerifu);
	}

	EndPaint(hwnd, &ps);
}

/////////////////////////////////////////////////////////////////////////////
// WM_DESTROYèàóù

void FUKI_OnDestroy(HWND hwnd)
{
	PostQuitMessage(0);
}
