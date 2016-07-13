///////////////////////////////////////////////////////////////////////
// dblblk.h -- 

#pragma once

#define ID_TRAYICON		1
#define WM_NOTIFYICONMESSAGE	(WM_USER + 1)

//////// main frame class ////////

class CMainWin : public CFrameWnd
{
public:
	CMainWin(LPCTSTR lpszClassName);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnPopupExit();
	afx_msg LRESULT OnNotifyIconMessage(WPARAM wParam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()
};

//////// dblblk application class ////////

class CApp : public CWinApp
{
public:
	BOOL InitInstance();
};
