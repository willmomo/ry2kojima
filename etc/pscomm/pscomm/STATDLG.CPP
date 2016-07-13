/////////////////////////////////////////////////////////////////////////////
//	STATUS DIALOG

#include "main.h"
#include "ext.h"

/////////////////////////////////////////////////////////////////////////////
//	Status・ダイアログ・プロシージャ

BOOL CALLBACK StatusDlgProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (wParam == IDOK) {
			PostMessage(GetParent(hwndDlg), WM_CLOSESTATUSDLG, 0, 0L);
			EndDialog(hwndDlg, 0);
			return TRUE;
		} else if (wParam == IDCANCEL) {
			PostMessage(GetParent(hwndDlg), WM_CLOSESTATUSDLG, 0, 0L);
			EndDialog(hwndDlg, -1);
			return TRUE;
		}
		break;
	}

	return FALSE;
}
