#include "RunCheck.h"


#define ID_TIMER	1


DWORD g_startTime;
kjm::Process g_proc;


static BOOL OnInitDialog(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	g_startTime = GetTickCount();

	g_proc.set_ShowWindow(SW_SHOW);
	g_proc.Create("exp gmc/mpcadmin@mpc file=d:\\home\\kojima\\work\\expdat.dmp log=d:\\home\\kojima\\work\\expdat.log");
	SetTimer(hwnd, ID_TIMER, 1000, NULL);
	return TRUE;
}


static BOOL OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	WORD wNotifyCode = HIWORD(wParam);
	WORD wID = LOWORD(wParam);
	HWND hwndCtl = (HWND)lParam;

	switch (wID) {
	case IDOK:
	case IDCANCEL:
		g_proc.Close();
		KillTimer(hwnd, ID_TIMER);
		EndDialog(hwnd, 0);
		return TRUE;
	}

	return FALSE;
}


static BOOL OnTimer(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	DWORD t = (GetTickCount() - g_startTime) / 1000;

	DWORD h = 0, m = 0, s = 0;
	s = t % 60;
	t = t / 60;
	m = t % 60;
	h = t / 60;
	SetDlgItemInt(hwnd, IDC_TIME_H, h, FALSE);
	SetDlgItemInt(hwnd, IDC_TIME_M, m, FALSE);
	SetDlgItemInt(hwnd, IDC_TIME_S, s, FALSE);

	DWORD exitCode = 0;
	g_proc.GetExitCodeProcess(&exitCode);
	if (exitCode != STILL_ACTIVE) {
		OnCommand(hwnd, MAKELONG(IDOK, 0), 0);
	}
	
	return TRUE;
}


BOOL CALLBACK MainDlg_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
		return OnInitDialog(hwnd, wParam, lParam);

	case WM_COMMAND:
		return OnCommand(hwnd, wParam, lParam);

	case WM_TIMER:
		return OnTimer(hwnd, wParam, lParam);
	}

	return FALSE;
}
