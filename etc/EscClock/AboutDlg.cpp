#include "EscClock.h"


//---------------------------------------------------------------------
// ダイアログプロシージャ
//---------------------------------------------------------------------
static BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		{
			WORD wNotifyCode = HIWORD(wParam);
			WORD wID = LOWORD(wParam);
			HWND hwndCtl = (HWND)lParam;

			if (wID == IDOK || wID == IDCANCEL) {
				EndDialog(hwnd, 0);
				return TRUE;
			}
		}
		break;
	}

	return FALSE;
}


//---------------------------------------------------------------------
// バージョン情報ダイアログの表示
//---------------------------------------------------------------------
int AboutDlg_DialogBox(HINSTANCE hInstance, HWND hwndParent) {
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUT), hwndParent, DlgProc);
}

