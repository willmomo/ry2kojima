//---------------------------------------------------------------------------------------
// GetComputerNameEx/SetComputerNameEx�֐��̃T���v��
//---------------------------------------------------------------------------------------
#include "mycompinfo.h"


//---------------------------------------------------------------------------------------
// �ύX�_�C�A���O�ւ̈����^
//---------------------------------------------------------------------------------------
typedef struct {
	LPCTSTR pszCaption;		// �E�B���h�E�^�C�g��
	std::basic_string<TCHAR> strCurrent;	// ���݂̒l
	std::basic_string<TCHAR> strNext;	// �V�����l
} CHANGEDLG_PARAM;


//---------------------------------------------------------------------------------------
// �ύX�_�C�A���O�̃v���V�[�W��
//---------------------------------------------------------------------------------------
BOOL CALLBACK ChangeDlg_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	static CHANGEDLG_PARAM* s_pParam = NULL;

	switch (uMsg) {
	case WM_INITDIALOG:
		{
			s_pParam = (CHANGEDLG_PARAM*)lParam;

			SetWindowText(hwnd, s_pParam->pszCaption);
			SetDlgItemText(hwnd, IDC_OLDNAME, s_pParam->strCurrent.c_str());
		}
		return FALSE;

	case WM_COMMAND:
		{
			WORD wID = LOWORD(wParam);
			if (wID == IDOK) {
				try {
					s_pParam->strNext = kjm::util::getDlgItemText(hwnd, IDC_EDIT_NEW);
					EndDialog(hwnd, wID);
				} catch (DWORD) {
					EndDialog(hwnd, IDCANCEL);
				}
				return TRUE;
			} else if (wID == IDCANCEL) {
				EndDialog(hwnd, wID);
				return TRUE;
			}
		}
		return FALSE;
	}

	return FALSE;
}


//---------------------------------------------------------------------------------------
// ���C���_�C�A���O: WM_INITDIALOG ���b�Z�[�W����
//---------------------------------------------------------------------------------------
BOOL OnInitDialog(HWND hwnd, WPARAM wParam, LPARAM lParam) {

	// �E�B���h�E�����[�N�G���A�̒��S�Ɉړ�������
	kjm::util::moveCenterWorkarea(hwnd);

	// �R���s���[�^���Ɋւ�������擾���āA���x�����X�V����B
	try {
		SetDlgItemText(hwnd, IDC_ComputerNameNetBIOS, kjm::util::getComputerNameEx(ComputerNameNetBIOS).c_str());
		SetDlgItemText(hwnd, IDC_ComputerNameDnsHostname, kjm::util::getComputerNameEx(ComputerNameDnsHostname).c_str());
		SetDlgItemText(hwnd, IDC_ComputerNameDnsDomain, kjm::util::getComputerNameEx(ComputerNameDnsDomain).c_str());
		SetDlgItemText(hwnd, IDC_ComputerNameDnsFullyQualified, kjm::util::getComputerNameEx(ComputerNameDnsFullyQualified).c_str());
		SetDlgItemText(hwnd, IDC_ComputerNamePhysicalNetBIOS, kjm::util::getComputerNameEx(ComputerNamePhysicalNetBIOS).c_str());
		SetDlgItemText(hwnd, IDC_ComputerNamePhysicalDnsHostname, kjm::util::getComputerNameEx(ComputerNamePhysicalDnsHostname).c_str());
		SetDlgItemText(hwnd, IDC_ComputerNamePhysicalDnsDomain, kjm::util::getComputerNameEx(ComputerNamePhysicalDnsDomain).c_str());
		SetDlgItemText(hwnd, IDC_ComputerNamePhysicalDnsFullyQualified, kjm::util::getComputerNameEx(ComputerNamePhysicalDnsFullyQualified).c_str());
	} catch (DWORD ex) {
		MessageBox(hwnd, kjm::util::formatMessageBySystem(ex).c_str(), TEXT("�_�C�A���O�������G���["), MB_OK | MB_ICONERROR);
		EndDialog(hwnd, -1);
	}

	return FALSE;
}


//---------------------------------------------------------------------------------------
// ���C���_�C�A���O: WM_COMMAND ���b�Z�[�W����
//---------------------------------------------------------------------------------------
BOOL OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	WORD wID = LOWORD(wParam);

	if (wID == IDOK || wID == IDCANCEL) {
		EndDialog(hwnd, 0);
		return TRUE;
	} else if (wID == IDC_CHG1) {
		try {
			CHANGEDLG_PARAM param;
			param.pszCaption = TEXT("����NetBIOS���̕ύX");
			param.strCurrent = kjm::util::getDlgItemText(hwnd, IDC_ComputerNamePhysicalNetBIOS);

			if (DialogBoxParam(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDD_CHANGE), hwnd, ChangeDlg_DlgProc, (LPARAM)&param) == IDOK) {
				if (kjm::util::__SetComputerNameEx(ComputerNamePhysicalNetBIOS, param.strNext.c_str())) {
					MessageBox(hwnd, TEXT("�ċN����A�L���ɂȂ�܂��B"), TEXT("����NetBIOS���̕ύX"), MB_OK | MB_ICONWARNING);
				} else {
					MessageBox(hwnd, kjm::util::formatMessageBySystem(GetLastError()).c_str(), TEXT("����NetBIOS���̕ύX�G���["), MB_OK | MB_ICONERROR);
				}
			}
		} catch (DWORD ex) {
			MessageBox(hwnd, kjm::util::formatMessageBySystem(ex).c_str(), TEXT("����NetBIOS���̕ύX�G���["), MB_OK | MB_ICONERROR);
		}
	} else if (wID == IDC_CHG2) {
		try {
			CHANGEDLG_PARAM param;
			param.pszCaption = TEXT("����DNS�z�X�g��");
			param.strCurrent = kjm::util::getDlgItemText(hwnd, IDC_ComputerNamePhysicalDnsHostname);

			if (DialogBoxParam(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDD_CHANGE), hwnd, ChangeDlg_DlgProc, (LPARAM)&param) == IDOK) {
				if (kjm::util::__SetComputerNameEx(ComputerNamePhysicalDnsHostname, param.strNext.c_str())) {
					MessageBox(hwnd, TEXT("�ċN����A�L���ɂȂ�܂��B"), TEXT("����DNS�z�X�g���̕ύX"), MB_OK | MB_ICONWARNING);
				} else {
					MessageBox(hwnd, kjm::util::formatMessageBySystem(GetLastError()).c_str(), TEXT("����DNS�z�X�g���̕ύX�G���["), MB_OK | MB_ICONERROR);
				}
			}
		} catch (DWORD ex) {
			MessageBox(hwnd, kjm::util::formatMessageBySystem(ex).c_str(), TEXT("����DNS�z�X�g���̕ύX�G���["), MB_OK | MB_ICONERROR);
		}
	} else if (wID == IDC_CHG3) {
		try {
			CHANGEDLG_PARAM param;
			param.pszCaption = TEXT("����DNS�T�t�B�b�N�X");
			param.strCurrent = kjm::util::getDlgItemText(hwnd, IDC_ComputerNamePhysicalDnsDomain);

			if (DialogBoxParam(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDD_CHANGE), hwnd, ChangeDlg_DlgProc, (LPARAM)&param) == IDOK) {
				if (kjm::util::__SetComputerNameEx(ComputerNamePhysicalDnsDomain, param.strNext.c_str())) {
					MessageBox(hwnd, TEXT("�ċN����A�L���ɂȂ�܂��B"), TEXT("����DNS�T�t�B�b�N�X�̕ύX"), MB_OK | MB_ICONWARNING);
				} else {
					MessageBox(hwnd, kjm::util::formatMessageBySystem(GetLastError()).c_str(), TEXT("����DNS�T�t�B�b�N�X�̕ύX�G���["), MB_OK | MB_ICONERROR);
				}
			}
		} catch (DWORD ex) {
			MessageBox(hwnd, kjm::util::formatMessageBySystem(ex).c_str(), TEXT("����DNS�T�t�B�b�N�X�̕ύX�G���["), MB_OK | MB_ICONERROR);
		}
	}

	return FALSE;
}

//---------------------------------------------------------------------------------------
// ���C���_�C�A���O�̃v���V�[�W��
//---------------------------------------------------------------------------------------
BOOL CALLBACK MainDlg_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
		return OnInitDialog(hwnd, wParam, lParam);

	case WM_COMMAND:
		return OnCommand(hwnd, wParam, lParam);
	}

	return FALSE;
}


//---------------------------------------------------------------------------------------
// �v���O�����X�^�[�g
//---------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDlg_DlgProc);
}
