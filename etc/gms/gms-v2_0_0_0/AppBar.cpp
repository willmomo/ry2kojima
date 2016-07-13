/**
 * CAppBar�̃C���v�������g
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
 * ��������Window���ړ�����
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
 * �V�����[�ɓK������`���v�Z����
 */

void CAppBar::QueryPos(UINT uNewEdge, RECT* lprc)
{
	RECT rcTemp;

	// �V����edge��float�Ȃ�A��`�̒����͕s�v�B

	if (uNewEdge == ABE_FLOAT) {
		return;
	}

	// ���݂�edge��float�Ȃ�Aworkarea�̋�`���x�[�X�ɒ���

	if (m_abd.uEdge == ABE_FLOAT) {
		::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcTemp, 0);
	} else {
		APPBARDATA abd = m_abd;

		// �X�N���[���S�̂̍��W��ݒ�

		::SetRect(&abd.rc, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
		abd.uEdge = uNewEdge;

		::SHAppBarMessage(ABM_QUERYPOS, &abd);

		rcTemp = abd.rc;
	}

	// ������̋�`�����ɍ��W�ϊ�����

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
 * �w�肳�ꂽEdge�Ƀh�b�L���O
 * ���ۂ̂Ƃ���A�V�X�e���ɓo�^���邾��
 */

void CAppBar::Dock()
{
	assert(m_abd.hWnd);		// Attach �ς݊m�F

	if (!m_fRegisterd) {
		SHAppBarMessage(ABM_NEW, &m_abd);
		m_fRegisterd = true;
	}
}

/**
 * �[�� dock ������
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
 * �V�X�e������؂藣��
 */

void CAppBar::Undock()
{
	assert(m_abd.hWnd);		// Attach �ς݊m�F

	if (m_fRegisterd) {
		SHAppBarMessage(ABM_REMOVE, &m_abd);
		m_fRegisterd = false;
	}
};

/**
 * �X�N���[�����W����ADock���ׂ�Edge��Ԃ�
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
 * WM_ENTERSIZMOVE ���b�Z�[�W����
 * ABE_FLOAT�̎��́A������W�ƃ}�E�X�̈ʒu�̃I�t�Z�b�g���L�^����
 * ABE_FLOAT�ȊO�̎��́A�}�E�X�̈ʒu��m_sizeFloating�̒����ɂȂ�悤��
 * �I�t�Z�b�g��ݒ肷��B
 */

void CAppBar::OnEnterSizeMove()
{
	if (!m_abd.hWnd)	return;		// Attach ����Ă��Ȃ��Ȃ牽�����Ȃ�

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
 * WM_EXITSIZEMOVE ���b�Z�[�W����
 */

void CAppBar::OnExitSizeMove()
{
	if (!m_abd.hWnd)	return;		// Attach ����Ă��Ȃ��Ȃ牽�����Ȃ�

	m_abd.uEdge = m_uEdgeTarget;

	if (m_abd.uEdge == ABE_FLOAT) {
		Undock();
	} else {
		Dock();
	}

	MoveWindow(&m_rcTarget);
}

/**
 * WM_SIZING ���b�Z�[�W����
 */

void CAppBar::OnSizing(RECT* lprc)
{
	assert(m_abd.hWnd);		// Attach �ς݊m�F

	m_rcTarget = *lprc;		// �ړ�����W�̕ۑ�
}

/**
 * WM_MOVING ���b�Z�[�W����
 * �}�E�X�|�C���^�ɂ��킹�āAWindow�̋�`���ω�������
 */

void CAppBar::OnMoving(LPRECT lprc)
{
	if (!m_abd.hWnd)	return;		// Attach ����Ă��Ȃ��Ȃ牽�����Ȃ�

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
 * WM_NCHITTEST ���b�Z�[�W����
 */

UINT CAppBar::OnNCHitTest(WPARAM wParam, LPARAM lParam)
{
	if (!m_abd.hWnd)	return 0;		// Attach ����Ă��Ȃ��Ȃ牽�����Ȃ�

	// �{���� hit �ʒu���擾
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
 * WM_WINDOWPOSCHANGED ���b�Z�[�W����
 */

void CAppBar::OnWindowPosChanged()
{
	if (!m_abd.hWnd)	return;		// Attach ����Ă��Ȃ��Ȃ牽�����Ȃ�

	// edge �� dock ���̎�������������

	if (m_abd.uEdge != ABE_FLOAT) {
		::SHAppBarMessage(ABM_WINDOWPOSCHANGED, &m_abd);
	}
}

/**
 * WM_ACTIVATE ���b�Z�[�W����
 */

void CAppBar::OnActivate()
{
	if (!m_abd.hWnd)	return;		// Attach ����Ă��Ȃ��Ȃ牽�����Ȃ�

	// edge �� dock ���̎�������������

	if (m_abd.uEdge != ABE_FLOAT) {
		::SHAppBarMessage(ABM_ACTIVATE, &m_abd);
	}
}

/**
 * AppBar �ʒm���b�Z�[�W����
 */

void CAppBar::OnAppBarMessage(WPARAM wParam)
{
	RECT rc;

	switch (wParam) {
	case ABN_POSCHANGED:	// �ʒu�̍Čv�Z�w��
		QueryPos(m_abd.uEdge, &rc);		// �ʒu���Čv�Z����
		MoveWindow(&rc);				// �ړ�����
		break;
	}
}
