//---------------------------------------------------------------------
// 二択ダイアログ
//---------------------------------------------------------------------
#include "SelectDlg.h"

//---------------------------------------------------------------------
// WM_COMMAND メッセージ処理
//---------------------------------------------------------------------
static BOOL OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	switch (LOWORD(wParam)) {
	case IDCANCEL:
		EndDialog(hwnd, 0);
		return TRUE;

	case IDC_BUTTON1:
		EndDialog(hwnd, 1);
		return TRUE;

	case IDC_BUTTON2:
		EndDialog(hwnd, 2);
		return TRUE;
	}

	return FALSE;
}

//---------------------------------------------------------------------
// WM_INITDIALOG メッセージ処理
//---------------------------------------------------------------------
static BOOL OnInitDialog(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	SetFocus(GetDlgItem(hwnd, IDCANCEL));

	return FALSE;
}

//---------------------------------------------------------------------
// ダイアログプロシージャ
//---------------------------------------------------------------------
static BOOL CALLBACK Main_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
		return OnInitDialog(hwnd, wParam, lParam);

	case WM_COMMAND:
		return OnCommand(hwnd, wParam, lParam);
	}

	return FALSE;
}

//---------------------------------------------------------------------
// プログラムスタート
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, Main_DlgProc);
}
