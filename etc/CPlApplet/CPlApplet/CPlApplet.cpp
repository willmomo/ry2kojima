#include <windows.h>
#include <windowsx.h>
#include <cpl.h>
#include "resource.h"
#include "CPlApplet.h"

// �A�v���P�[�V�������Ŏg�p����C���X�^���X�n���h��
HINSTANCE hInst = NULL;

BOOL DlgOnInit(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	HICON hIcon;

	// �A�C�R����ݒ肵�܂�
	hIcon = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_SMALL), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);

	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

	return TRUE;
}

void DlgOnCommand(HWND hwnd, int idCtl, HWND hwndCtrl, UINT codeNotify)
{
	switch (idCtl) {
	case IDC_BUTTON_TEST:
		break;
	case IDCANCEL:
		// �_�C�A���O�{�b�N�X��j�����܂�
		EndDialog(hwnd, idCtl);
		break;
	default:
		break;
	}
}

LRESULT CALLBACK MainDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_INITDIALOG:
		return DlgOnInit(hwnd, (HWND)wParam, lParam);
	case WM_COMMAND:
		DlgOnCommand(hwnd, LOWORD(wParam), (HWND)lParam, HIWORD(wParam));
		break;
	default:
		return FALSE;
		break;
	}

	return TRUE;
}

void CPLAPPLET_API CPlSample(HWND hwnd)
{
	MessageBox(hwnd, "CPlSample�̃e�X�g", "CPlApplet sample", MB_OK);
}

LONG CALLBACK CPlApplet(HWND hwnd, UINT uMsg, LONG lParam1, LONG lParam2)
{
	LPCPLINFO lpCPlInfo;

	switch (uMsg) {
	case CPL_INIT:
		// �C���X�^���X���擾���܂�
		hInst = GetModuleHandle("CPlApplet.cpl");
		return TRUE;
	case CPL_GETCOUNT:
		// �A�v���b�g��1��ނ̂��߁A1��Ԃ��܂�
		return 1;
		break;
	case CPL_INQUIRE:
		{
			// �_�C�A���O�{�b�N�X�̐ݒ���s���܂��B
			lpCPlInfo = (LPCPLINFO)lParam2;
			lpCPlInfo->lData = 0;
			lpCPlInfo->idIcon = IDI_GENERIC;
			lpCPlInfo->idName = IDS_NAME;
			lpCPlInfo->idInfo = IDS_INFO;
		}
		break;
	case CPL_DBLCLK:
		// �_�C�A���O�{�b�N�X��\�����܂�
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)MainDlgProc);
		break;
	case CPL_STOP:
		// �_�C�A���O�{�b�N�X�I����̏������s���܂��B
		break;
	case CPL_EXIT:
		// FreeLibrary()�֐����Ă΂��O�̏������s���܂�
		break;
	default:
		break;
	}

	return 0;
}
