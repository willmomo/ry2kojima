//---------------------------------------------------------------------------
// AppBar を扱うためのクラス。
// kjm 名前空間だが、kjmlibとは、わけて存在。
//
// ver.0.2012.08.20
//---------------------------------------------------------------------------
#include "AppBar.h"


BOOL kjm::appBar::isRegistered() {
	return m_fRegistered;
}

BOOL kjm::appBar::regist(HWND hwnd, UINT uCallbackMessage) {
	m_abd.cbSize = sizeof(m_abd);
	m_abd.hWnd = hwnd;
	m_abd.uCallbackMessage = uCallbackMessage;

	return m_fRegistered = (BOOL)SHAppBarMessage(ABM_NEW, &m_abd);
}

void kjm::appBar::remove() {
	SHAppBarMessage(ABM_REMOVE, &m_abd);
}

RECT kjm::appBar::queryPos(int uEdge, SIZE size) {
	APPBARDATA abd = m_abd;

	abd.uEdge = uEdge;
	SetRect(&abd.rc, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	switch (uEdge) {
		case ABE_TOP:		abd.rc.bottom = abd.rc.top + size.cy;	break;
		case ABE_BOTTOM:	abd.rc.top = abd.rc.bottom - size.cy;	break;
		case ABE_LEFT:		abd.rc.right = abd.rc.left + size.cx;	break;
		case ABE_RIGHT:		abd.rc.left = abd.rc.right - size.cx;	break;
	}

	SHAppBarMessage(ABM_QUERYPOS, &abd);

	switch (uEdge) {
		case ABE_TOP:		abd.rc.bottom = abd.rc.top + size.cy;	break;
		case ABE_BOTTOM:	abd.rc.top = abd.rc.bottom - size.cy;	break;
		case ABE_LEFT:		abd.rc.right = abd.rc.left + size.cx;	break;
		case ABE_RIGHT:		abd.rc.left = abd.rc.right - size.cx;	break;
	}
	return abd.rc;
}

void kjm::appBar::setPos(int uEdge, SIZE size) {
	m_abd.uEdge = uEdge;
	m_abd.rc = queryPos(uEdge, size);

	SHAppBarMessage(ABM_SETPOS, &m_abd);

	MoveWindow(m_abd.hWnd, m_abd.rc.left, m_abd.rc.top, m_abd.rc.right - m_abd.rc.left, m_abd.rc.bottom - m_abd.rc.top, TRUE);
}

void kjm::appBar::callbackMessage(WPARAM wParam, LPARAM lParam) {
	if (wParam == ABN_STATECHANGE) {
		TCHAR msg[256];
		wsprintf(msg, _T("AppBar_Callback: ABN_STATECHANGE\n"));
		OutputDebugString(msg);
	} else if (wParam == ABN_FULLSCREENAPP) {
		TCHAR msg[256];
		wsprintf(msg, _T("AppBar_Callback: ABN_FULLSCREENAPP with %d\n"), lParam);
		OutputDebugString(msg);

		if ((BOOL)lParam) {
			m_hwndZOrder = GetWindow(m_abd.hWnd, GW_HWNDPREV);
			SetWindowPos(m_abd.hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);            
		} else {
			SetWindowPos(m_abd.hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
			//SetWindowPos(m_abd.hWnd, m_hwndZOrder, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
			m_hwndZOrder = NULL;
		}
	} else if (wParam == ABN_POSCHANGED) {
		TCHAR msg[256];
		wsprintf(msg, _T("AppBar_Callback: ABN_POSCHANGED\n"));
		OutputDebugString(msg);

		SIZE size = {0, 0};
		switch (m_abd.uEdge) {
			case ABE_TOP:
			case ABE_BOTTOM:
				size.cy = m_abd.rc.bottom - m_abd.rc.top;
				break;
			case ABE_LEFT:
			case ABE_RIGHT:
				size.cx = m_abd.rc.right - m_abd.rc.left;
				break;
		}
		setPos(m_abd.uEdge, size);
	} else if (wParam == ABN_WINDOWARRANGE) {
		TCHAR msg[256];
		wsprintf(msg, _T("AppBar_Callback: ABN_WINDOWARRANGE with %d\n"), lParam);
		OutputDebugString(msg);

		if ((BOOL)lParam) {
			ShowWindow(m_abd.hWnd, SW_HIDE);
		} else {
			ShowWindow(m_abd.hWnd, SW_SHOW);
		}
	}
}
