/**
 * CAppBarのインプリメント
 */

#pragma warning(push, 4)
#pragma warning(disable: 4100)

#include "appbar.h"

/**
 * constructer
 */

CAppBar::CAppBar()
{
	m_fRegisterd = false;
	memset(&m_abd, 0, sizeof(m_abd));
}

/**
 * destructer
 */

CAppBar::~CAppBar()
{
	if (m_abd.uEdge != 0 && m_abd.uEdge != ABE_FLOAT) {
		if (this->m_fRegisterd) {
			Undock();
		}
	}

	Detach();
}

/**
 * attach
 */

void CAppBar::Attach(const APPBARCTRL* pabc)
{
	m_abd.cbSize = sizeof(m_abd);
	m_abd.hWnd = pabc->hwnd;
	m_abd.uEdge = pabc->uEdge;
	m_abd.uCallbackMessage = pabc->uMsg;

	m_sizeFloating = pabc->m_sizeFloat;
	m_sizeDocking = pabc->m_sizeDock;

	if (m_abd.uEdge != ABE_FLOAT) {
		Dock(m_abd.uEdge);
	}
}

/**
 * detach
 */

void CAppBar::Detach()
{
	if (m_abd.uEdge != 0 && m_abd.uEdge != ABE_FLOAT) {
		if (this->m_fRegisterd) {
			Undock();
		}
	}

	memset(&m_abd, 0, sizeof(m_abd));
}

/**
 * 所属するWindowを移動する
 */

void CAppBar::MoveWindow(const RECT* lprc, BOOL bRepaint/*=TRUE*/)
{
	int nWidth = lprc->right - lprc->left;
	int nHeight = lprc->bottom - lprc->top;

	::MoveWindow(m_abd.hWnd, lprc->left, lprc->top, nWidth, nHeight, bRepaint);

	if (m_abd.uEdge != ABE_FLOAT) {
		m_abd.rc = *lprc;
		::SHAppBarMessage(ABM_SETPOS, &m_abd);

		if (m_abd.uEdge == ABE_TOP || m_abd.uEdge == ABE_BOTTOM) {
			m_sizeDocking.cy = nHeight;
		} else {
			m_sizeDocking.cx = nWidth;
		}
	} else {
		m_sizeFloating.cx = nWidth;
		m_sizeFloating.cy = nHeight;
	}
}


/**
 * 新しい端に適した矩形を計算する
 */

void CAppBar::QueryPos(UINT uNewEdge, RECT* lprc)
{
	RECT rcTemp;

	// 新しいedgeがfloatなら、矩形の調整は不要。

	if (uNewEdge == ABE_FLOAT) {
		return;
	}

	// 現在のedgeがfloatなら、workareaの矩形をベースに調整

	if (m_abd.uEdge == ABE_FLOAT) {
		::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcTemp, 0);
	} else {
		APPBARDATA abd = m_abd;

		// スクリーン全体の座標を設定

		::SetRect(&abd.rc, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
		abd.uEdge = uNewEdge;

		::SHAppBarMessage(ABM_QUERYPOS, &abd);

		rcTemp = abd.rc;
	}

	// 調整後の矩形を元に座標変換する

	switch (uNewEdge) {
	case ABE_TOP:
		rcTemp.bottom = rcTemp.top + m_sizeDocking.cy;
		break;

	case ABE_BOTTOM:
		rcTemp.top = rcTemp.bottom - m_sizeDocking.cy;
		break;

	case ABE_LEFT:
		rcTemp.right = rcTemp.left + m_sizeDocking.cx;
		break;

	case ABE_RIGHT:
		rcTemp.left = rcTemp.right - m_sizeDocking.cx;
		break;
	}

	*lprc = rcTemp;
}

/**
 * 指定されたEdgeにドッキング
 * 実際のところ、システムに登録するだけ
 */

void CAppBar::Dock()
{
	assert(m_abd.hWnd);		// Attach 済み確認

	if (!m_fRegisterd) {
		SHAppBarMessage(ABM_NEW, &m_abd);
		m_fRegisterd = true;
	}
}

/**
 * 端に dock させる
 */

void CAppBar::Dock(UINT uEdge)
{
	if (uEdge == ABE_FLOAT) {
		Undock();
		return;
	}

	m_abd.uEdge = uEdge;

	if (!m_fRegisterd) {
		SHAppBarMessage(ABM_NEW, &m_abd);
		m_fRegisterd = true;

		{
			m_abd.lParam = TRUE;
			BOOL fRet = SHAppBarMessage(ABM_SETAUTOHIDEBAR, &m_abd);
			TCHAR buf[512];
			wsprintf(buf, "fRet = %d\n", fRet);
			OutputDebugString(buf);
		}
	}

	RECT rcTemp = m_abd.rc;
	QueryPos(uEdge, &rcTemp);

	MoveWindow(&rcTemp);
}

/**
 * システムから切り離す
 */

void CAppBar::Undock()
{
	assert(m_abd.hWnd);		// Attach 済み確認

	if (m_fRegisterd) {
		SHAppBarMessage(ABM_REMOVE, &m_abd);
		m_fRegisterd = false;
	}
};

/**
 * スクリーン座標から、DockすべきEdgeを返す
 */

UINT CAppBar::PtToEdge(LPPOINT lppt)
{
	RECT rcWork;

	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, 0);

	if (lppt->y < rcWork.top + GetSystemMetrics(SM_CYICON)) {
		return ABE_TOP;
	} else if (lppt->y > rcWork.bottom - GetSystemMetrics(SM_CYICON)) {
		return ABE_BOTTOM;
	} else if (lppt->x < rcWork.left + GetSystemMetrics(SM_CXICON)) {
		return ABE_LEFT;
	} else if (lppt->x > rcWork.right - GetSystemMetrics(SM_CXICON)) {
		return ABE_RIGHT;
	}

	return ABE_FLOAT;
}

/**
 * WM_ENTERSIZMOVE メッセージ処理
 * ABE_FLOATの時は、左上座標とマウスの位置のオフセットを記録する
 * ABE_FLOAT以外の時は、マウスの位置がm_sizeFloatingの中央になるように
 * オフセットを設定する。
 */

void CAppBar::OnEnterSizeMove()
{
	if (!m_abd.hWnd)	return;		// Attach されていないなら何もしない

	m_uEdgeTarget = m_abd.uEdge;
	m_rcTarget = m_abd.rc;

	if (m_abd.uEdge != ABE_FLOAT) {
		m_ptOffset.x = -(m_sizeFloating.cx / 2);
		m_ptOffset.y = -(m_sizeFloating.cy / 2);
	} else {
		POINT pt;
		RECT rcWnd;

		GetCursorPos(&pt);
		GetWindowRect(m_abd.hWnd, &rcWnd);
		m_ptOffset.x = rcWnd.left - pt.x;
		m_ptOffset.y = rcWnd.top - pt.y;
	}
}

/**
 * WM_EXITSIZEMOVE メッセージ処理
 */

void CAppBar::OnExitSizeMove()
{
	if (!m_abd.hWnd)	return;		// Attach されていないなら何もしない

	m_abd.uEdge = m_uEdgeTarget;

	if (m_abd.uEdge == ABE_FLOAT) {
		Undock();
	} else {
		Dock();
	}

	MoveWindow(&m_rcTarget);
}

/**
 * WM_SIZING メッセージ処理
 */

void CAppBar::OnSizing(RECT* lprc)
{
	assert(m_abd.hWnd);		// Attach 済み確認

	m_rcTarget = *lprc;		// 移動後座標の保存
}

/**
 * WM_MOVING メッセージ処理
 * マウスポインタにあわせて、Windowの矩形も変化させる
 */

void CAppBar::OnMoving(LPRECT lprc)
{
	if (!m_abd.hWnd)	return;		// Attach されていないなら何もしない

	POINT pt;

	GetCursorPos(&pt);
	m_uEdgeTarget = PtToEdge(&pt);

	switch (m_uEdgeTarget) {
	case ABE_FLOAT:
		lprc->left = 0;
		lprc->right = m_sizeFloating.cx;
		lprc->top = 0;
		lprc->bottom = m_sizeFloating.cy;
		OffsetRect(lprc, pt.x + m_ptOffset.x, pt.y + m_ptOffset.y);
		break;
	}

	QueryPos(m_uEdgeTarget, lprc);

	m_rcTarget = *lprc;
}

/**
 * WM_NCHITTEST メッセージ処理
 */

UINT CAppBar::OnNCHitTest(WPARAM wParam, LPARAM lParam)
{
	if (!m_abd.hWnd)	return 0;		// Attach されていないなら何もしない

	// 本来の hit 位置を取得
	UINT hit = ::DefWindowProc(m_abd.hWnd, WM_NCHITTEST, wParam, lParam);

	switch (m_abd.uEdge) {
	case ABE_TOP:
		if (
			///(hit == HTBOTTOM) ||
			(hit == HTBOTTOMLEFT) ||
			(hit == HTBOTTOMRIGHT) ||
			(hit == HTLEFT) ||
			(hit == HTRIGHT) ||
			(hit == HTTOP) ||
			(hit == HTTOPLEFT) ||
			(hit == HTTOPRIGHT)) {
			hit = HTCLIENT;
		}
		break;

	case ABE_BOTTOM:
		if (
			(hit == HTBOTTOM) ||
			(hit == HTBOTTOMLEFT) ||
			(hit == HTBOTTOMRIGHT) ||
			(hit == HTLEFT) ||
			(hit == HTRIGHT) ||
			///(hit == HTTOP) ||
			(hit == HTTOPLEFT) ||
			(hit == HTTOPRIGHT)) {
			hit = HTCLIENT;
		}
		break;

	case ABE_LEFT:
		if (
			(hit == HTBOTTOM) ||
			(hit == HTBOTTOMLEFT) ||
			(hit == HTBOTTOMRIGHT) ||
			(hit == HTLEFT) ||
			///(hit == HTRIGHT) ||
			(hit == HTTOP) ||
			(hit == HTTOPLEFT) ||
			(hit == HTTOPRIGHT)) {
			hit = HTCLIENT;
		}
		break;

	case ABE_RIGHT:
		if (
			(hit == HTBOTTOM) ||
			(hit == HTBOTTOMLEFT) ||
			(hit == HTBOTTOMRIGHT) ||
			///(hit == HTLEFT) ||
			(hit == HTRIGHT) ||
			(hit == HTTOP) ||
			(hit == HTTOPLEFT) ||
			(hit == HTTOPRIGHT)) {
			hit = HTCLIENT;
		}
		break;
	}

	return hit;
}

/**
 * WM_WINDOWPOSCHANGED メッセージ処理
 */

void CAppBar::OnWindowPosChanged()
{
	if (!m_abd.hWnd)	return;		// Attach されていないなら何もしない

	// edge に dock 中の時だけ処理する

	if (m_abd.uEdge != ABE_FLOAT) {
		::SHAppBarMessage(ABM_WINDOWPOSCHANGED, &m_abd);
	}
}

/**
 * WM_ACTIVATE メッセージ処理
 */

void CAppBar::OnActivate()
{
	if (!m_abd.hWnd)	return;		// Attach されていないなら何もしない

	// edge に dock 中の時だけ処理する

	if (m_abd.uEdge != ABE_FLOAT) {
		::SHAppBarMessage(ABM_ACTIVATE, &m_abd);
	}
}

/**
 * AppBar 通知メッセージ処理
 */

void CAppBar::OnAppBarMessage(WPARAM wParam)
{
	RECT rc;

	switch (wParam) {
	case ABN_POSCHANGED:	// 位置の再計算指示
		QueryPos(m_abd.uEdge, &rc);		// 位置を再計算して
		MoveWindow(&rc);				// 移動する
		break;
	}
}
