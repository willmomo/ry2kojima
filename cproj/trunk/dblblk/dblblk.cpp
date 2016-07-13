///////////////////////////////////////////////////////////////////////
// dblblk.cpp --

#include <afxwin.h>
#include "resource.h"
#include "dblblk.h"
#include "dchook.h"

//////// CMainWin implement ////////

///////////////////////////////////////////////////////////////////////
// �R���X�g���N�^

CMainWin::CMainWin(LPCTSTR lpszClassName)
{
	Create(lpszClassName, _T("Double Click Blocker"));
}

///////////////////////////////////////////////////////////////////////
// WM_CREATE ���b�Z�[�W����

afx_msg int CMainWin::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	NOTIFYICONDATA nid;

	nid.cbSize = sizeof(nid);
	nid.hWnd = m_hWnd;
	nid.uID = ID_TRAYICON;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.hIcon = (HICON)GetClassLong(m_hWnd, GCL_HICON);
	nid.uCallbackMessage = WM_NOTIFYICONMESSAGE;
	lstrcpy(nid.szTip, _T("Double Click Blocker"));

	Shell_NotifyIcon(NIM_ADD, &nid);

	DcHook();

	return 0;
}

///////////////////////////////////////////////////////////////////////
// WM_DESTROY ���b�Z�[�W����

afx_msg void CMainWin::OnDestroy()
{
	DcUnhook();

	NOTIFYICONDATA nid;

	nid.cbSize = sizeof(nid);
	nid.hWnd = m_hWnd;
	nid.uID = ID_TRAYICON;

	Shell_NotifyIcon(NIM_DELETE, &nid);
}

///////////////////////////////////////////////////////////////////////
// WM_CLOSE ���b�Z�[�W����

afx_msg void CMainWin::OnClose()
{
	if (MessageBox(_T("�I�����܂��B��낵���ł���?"), 
		_T("�I���m�F"), MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) == IDYES)
	{
		DestroyWindow();
	}
}

///////////////////////////////////////////////////////////////////////
// WM_COMMAND, IDM_POPUP_EXIT ���b�Z�[�W����

afx_msg void CMainWin::OnPopupExit()
{
	SendMessage(WM_CLOSE);
}

///////////////////////////////////////////////////////////////////////
// WM_NOTIFYICONMESSAGE ���b�Z�[�W����

afx_msg LRESULT CMainWin::OnNotifyIconMessage(WPARAM wParam, LPARAM lParam)
{
	if (lParam == WM_RBUTTONDOWN)
	{
		POINT pt;
		GetCursorPos(&pt);

		SetForegroundWindow();

		CMenu menu;
		menu.LoadMenu(IDR_TRAYMENU);
		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
	}

	return 0;
}

// ���b�Z�[�W�}�b�v

BEGIN_MESSAGE_MAP(CMainWin, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_COMMAND(IDM_POPUP_EXIT, OnPopupExit)
	ON_MESSAGE(WM_NOTIFYICONMESSAGE, OnNotifyIconMessage)
END_MESSAGE_MAP()

/////// CApp implement ////////

///////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����̏�����

BOOL CApp::InitInstance()
{
	LPCTSTR lpszClassName = AfxRegisterWndClass(0, 
		LoadStandardCursor(IDC_ARROW), (HBRUSH)(COLOR_WINDOW + 1), LoadIcon(IDI_MAINFRM));

	m_pMainWnd = new CMainWin(lpszClassName);
///	m_pMainWnd->ShowWindow(m_nCmdShow);
///	m_pMainWnd->UpdateWindow();

	return TRUE;
}

//////// CApp instance ////////

CApp theApp;	// �A�v���P�[�V���������I

