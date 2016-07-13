/* ファイルをロックするアプリケーション */

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "kjmutil.h"
#include "resource.h"


HANDLE g_hFile = INVALID_HANDLE_VALUE;


/* [ファイルを閉じる]ボタンを押した時の処理 */
BOOL MainDlg_Button3_OnClick(HWND hwnd, HWND hwndCtl) {
	BOOL fResult = TRUE;

	CloseHandle(g_hFile);
	g_hFile = INVALID_HANDLE_VALUE;

	EnableWindow(GetDlgItem(hwnd, IDC_BUTTON2), TRUE);
	EnableWindow(GetDlgItem(hwnd, IDC_BUTTON3), FALSE);

	EnableWindow(GetDlgItem(hwnd, IDC_CHECK1), TRUE);
	EnableWindow(GetDlgItem(hwnd, IDC_CHECK2), TRUE);
	EnableWindow(GetDlgItem(hwnd, IDC_CHECK3), TRUE);
	EnableWindow(GetDlgItem(hwnd, IDC_BUTTON1), TRUE);
	EnableWindow(GetDlgItem(hwnd, IDC_EDIT1), TRUE);

	return fResult;
}


/* [ファイルを開く]ボタンを押した時の処理 */
BOOL MainDlg_Button2_OnClick(HWND hwnd, HWND hwndCtl) {
	BOOL fResult = TRUE;

	TCHAR fileName[MAX_PATH];
	GetDlgItemText(hwnd, IDC_EDIT1, fileName, MAX_PATH);
	if (lstrlen(fileName) == 0) {
		MessageBox(hwnd, _T("ファイル名を入力してください。"), _T("locker error"), MB_OK | MB_ICONSTOP);
		return fResult;
	}

	DWORD dwShareMode = 0;
	if (Button_GetCheck(GetDlgItem(hwnd, IDC_CHECK1)) == BST_CHECKED) dwShareMode |= FILE_SHARE_READ;
	if (Button_GetCheck(GetDlgItem(hwnd, IDC_CHECK2)) == BST_CHECKED) dwShareMode |= FILE_SHARE_WRITE;
	if (Button_GetCheck(GetDlgItem(hwnd, IDC_CHECK3)) == BST_CHECKED) dwShareMode |= FILE_SHARE_DELETE;

	g_hFile = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, dwShareMode, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (g_hFile != INVALID_HANDLE_VALUE) {
		EnableWindow(GetDlgItem(hwnd, IDC_BUTTON2), FALSE);
		EnableWindow(GetDlgItem(hwnd, IDC_BUTTON3), TRUE);

		EnableWindow(GetDlgItem(hwnd, IDC_CHECK1), FALSE);
		EnableWindow(GetDlgItem(hwnd, IDC_CHECK2), FALSE);
		EnableWindow(GetDlgItem(hwnd, IDC_CHECK3), FALSE);
		EnableWindow(GetDlgItem(hwnd, IDC_BUTTON1), FALSE);
		EnableWindow(GetDlgItem(hwnd, IDC_EDIT1), FALSE);
	} else {
		kjmutil::err lastErr;
		TCHAR msg[512];
		wsprintf(msg, _T("エラー %d : %s\nファイル '%s' が、オープンできません。"), lastErr.get_number(), lastErr.get_description().c_str(), fileName);
		MessageBox(hwnd, msg, _T("locker error"), MB_OK | MB_ICONSTOP);
	}

	return fResult;
}


/* [参照]ボタンを押した時の処理 */
BOOL MainDlg_Button1_OnClick(HWND hwnd, HWND hwndCtl) {
	BOOL fResult = TRUE;

	TCHAR fileName[MAX_PATH];
	GetDlgItemText(hwnd, IDC_EDIT1, fileName, MAX_PATH);

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = GetWindowInstance(hwnd);
	ofn.lpstrFilter = _T("すべてのファイル(*.*)\0*.*\0");
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = _T("ロックするファイルの選択");
	ofn.Flags = OFN_HIDEREADONLY;

	if (GetOpenFileName(&ofn)) {
		SetDlgItemText(hwnd, IDC_EDIT1, fileName);
	} else {
		DWORD err = CommDlgExtendedError();
		if (err != 0) {
			TCHAR msg[256];
			wsprintf(msg, _T("システムエラー %d が、発生しました。"), err);
			MessageBox(hwnd, msg, _T("locker error."), MB_OK | MB_ICONSTOP);
		}
	}

	return fResult;
}


/* WM_COMMANDメッセージ処理 */
BOOL MainDlg_OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	BOOL fResult = FALSE;

	WORD wNotifyCode = HIWORD(wParam);
	WORD wID = LOWORD(wParam);
	HWND hwndCtl = (HWND)lParam;

	switch (wID) {
	case IDCANCEL:
		if (g_hFile != INVALID_HANDLE_VALUE)
			MainDlg_Button3_OnClick(hwnd, NULL);

		EndDialog(hwnd, 0);
		fResult = TRUE;
		break;

	case IDC_BUTTON1:
		fResult = MainDlg_Button1_OnClick(hwnd, hwndCtl);
		break;

	case IDC_BUTTON2:
		fResult = MainDlg_Button2_OnClick(hwnd, hwndCtl);
		break;

	case IDC_BUTTON3:
		fResult = MainDlg_Button3_OnClick(hwnd, hwndCtl);
		break;
	}

	return fResult;
}


/* WM_GETMINMAXINFO メッセージ処理
   縦方向にリサイズできないようにします。
*/
BOOL MainDlg_GetMinMaxInfo(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	static int s_minX = 0;
	LPMINMAXINFO lpmmi = (LPMINMAXINFO) lParam;

	kjmutil::rect rcWindow = kjmutil::rect::get_window_rect(hwnd);
	if (s_minX == 0) {
		s_minX = rcWindow.get_width();
	}

	lpmmi->ptMinTrackSize.x = s_minX;
	lpmmi->ptMinTrackSize.y = lpmmi->ptMaxTrackSize.y = (rcWindow.bottom - rcWindow.top);
	return TRUE;
}


/* WM_SIZE メッセージ処理 */
BOOL MainDlg_Size(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	DWORD fwSizeType = wParam;
	WORD nWidth = LOWORD(lParam);
	WORD nHeight = HIWORD(lParam);

	kjmutil::rect rcClient = kjmutil::rect::get_client_rect(hwnd);

	kjmutil::rect rcButton1 = kjmutil::rect::get_window_rect(GetDlgItem(hwnd, IDC_BUTTON1)).screen_to_client(hwnd);
	MoveWindow(GetDlgItem(hwnd, IDC_BUTTON1),
		rcClient.right - 12 - rcButton1.get_width(),
		rcButton1.top,
		rcButton1.get_width(),
		rcButton1.get_height(),
		TRUE);

	kjmutil::rect rcEdit1 = kjmutil::rect::get_window_rect(GetDlgItem(hwnd, IDC_EDIT1)).screen_to_client(hwnd);
	MoveWindow(GetDlgItem(hwnd, IDC_EDIT1),
		rcEdit1.left,
		rcEdit1.top,
		rcClient.right - rcEdit1.left - rcButton1.get_width() - (12 * 2),
		rcEdit1.get_height(),
		TRUE);

	kjmutil::rect rcButton2 = kjmutil::rect::get_window_rect(GetDlgItem(hwnd, IDC_BUTTON2)).screen_to_client(hwnd);
	kjmutil::rect rcButton3 = kjmutil::rect::get_window_rect(GetDlgItem(hwnd, IDC_BUTTON3)).screen_to_client(hwnd);

	kjmutil::rect rcUnion = kjmutil::rect::union_rect(&rcButton2, &rcButton3);
	rcButton2.offset(((rcClient.right - rcUnion.get_width()) / 2) - rcUnion.left, 0);
	rcButton3.offset(((rcClient.right - rcUnion.get_width()) / 2) - rcUnion.left, 0);

	kjmutil::move_window(GetDlgItem(hwnd, IDC_BUTTON2), rcButton2, TRUE);
	kjmutil::move_window(GetDlgItem(hwnd, IDC_BUTTON3), rcButton3, TRUE);

	return TRUE;
}


/* ダイアログ・プロシージャ */
BOOL CALLBACK MainDlg_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	BOOL fResult = FALSE;

	switch (uMsg) {
	case WM_COMMAND:
		fResult = MainDlg_OnCommand(hwnd, wParam, lParam);
		break;

	case WM_GETMINMAXINFO:
		fResult = MainDlg_GetMinMaxInfo(hwnd, wParam, lParam);
		break;

	case WM_SIZE:
		fResult = MainDlg_Size(hwnd, wParam, lParam);
		break;
	}

	return fResult;
}


/* プログラム・スタート */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDlg_DlgProc);
}
