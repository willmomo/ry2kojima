//---------------------------------------------------------------------
// ����_�C�A���O
//---------------------------------------------------------------------
#include "SelectDlg.h"

//---------------------------------------------------------------------
// WM_COMMAND ���b�Z�[�W����
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
// WM_INITDIALOG ���b�Z�[�W����
//---------------------------------------------------------------------
static BOOL OnInitDialog(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	SetFocus(GetDlgItem(hwnd, IDCANCEL));

	return FALSE;
}

//---------------------------------------------------------------------
// �_�C�A���O�v���V�[�W��
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
// �v���O�����X�^�[�g
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, Main_DlgProc);
}
