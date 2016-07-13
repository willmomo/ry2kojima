//---------------------------------------------------------------------------------------
// GetComputerNameEx/SetComputerNameEx関数のサンプル
//---------------------------------------------------------------------------------------
#include "mycompinfo.h"


//---------------------------------------------------------------------------------------
// 変更ダイアログへの引数型
//---------------------------------------------------------------------------------------
typedef struct {
	LPCTSTR pszCaption;		// ウィンドウタイトル
	std::basic_string<TCHAR> strCurrent;	// 現在の値
	std::basic_string<TCHAR> strNext;	// 新しい値
} CHANGEDLG_PARAM;


//---------------------------------------------------------------------------------------
// 変更ダイアログのプロシージャ
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
// メインダイアログ: WM_INITDIALOG メッセージ処理
//---------------------------------------------------------------------------------------
BOOL OnInitDialog(HWND hwnd, WPARAM wParam, LPARAM lParam) {

	// ウィンドウをワークエリアの中心に移動させる
	kjm::util::moveCenterWorkarea(hwnd);

	// コンピュータ名に関する情報を取得して、ラベルを更新する。
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
		MessageBox(hwnd, kjm::util::formatMessageBySystem(ex).c_str(), TEXT("ダイアログ初期化エラー"), MB_OK | MB_ICONERROR);
		EndDialog(hwnd, -1);
	}

	return FALSE;
}


//---------------------------------------------------------------------------------------
// メインダイアログ: WM_COMMAND メッセージ処理
//---------------------------------------------------------------------------------------
BOOL OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	WORD wID = LOWORD(wParam);

	if (wID == IDOK || wID == IDCANCEL) {
		EndDialog(hwnd, 0);
		return TRUE;
	} else if (wID == IDC_CHG1) {
		try {
			CHANGEDLG_PARAM param;
			param.pszCaption = TEXT("物理NetBIOS名の変更");
			param.strCurrent = kjm::util::getDlgItemText(hwnd, IDC_ComputerNamePhysicalNetBIOS);

			if (DialogBoxParam(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDD_CHANGE), hwnd, ChangeDlg_DlgProc, (LPARAM)&param) == IDOK) {
				if (kjm::util::__SetComputerNameEx(ComputerNamePhysicalNetBIOS, param.strNext.c_str())) {
					MessageBox(hwnd, TEXT("再起動後、有効になります。"), TEXT("物理NetBIOS名の変更"), MB_OK | MB_ICONWARNING);
				} else {
					MessageBox(hwnd, kjm::util::formatMessageBySystem(GetLastError()).c_str(), TEXT("物理NetBIOS名の変更エラー"), MB_OK | MB_ICONERROR);
				}
			}
		} catch (DWORD ex) {
			MessageBox(hwnd, kjm::util::formatMessageBySystem(ex).c_str(), TEXT("物理NetBIOS名の変更エラー"), MB_OK | MB_ICONERROR);
		}
	} else if (wID == IDC_CHG2) {
		try {
			CHANGEDLG_PARAM param;
			param.pszCaption = TEXT("物理DNSホスト名");
			param.strCurrent = kjm::util::getDlgItemText(hwnd, IDC_ComputerNamePhysicalDnsHostname);

			if (DialogBoxParam(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDD_CHANGE), hwnd, ChangeDlg_DlgProc, (LPARAM)&param) == IDOK) {
				if (kjm::util::__SetComputerNameEx(ComputerNamePhysicalDnsHostname, param.strNext.c_str())) {
					MessageBox(hwnd, TEXT("再起動後、有効になります。"), TEXT("物理DNSホスト名の変更"), MB_OK | MB_ICONWARNING);
				} else {
					MessageBox(hwnd, kjm::util::formatMessageBySystem(GetLastError()).c_str(), TEXT("物理DNSホスト名の変更エラー"), MB_OK | MB_ICONERROR);
				}
			}
		} catch (DWORD ex) {
			MessageBox(hwnd, kjm::util::formatMessageBySystem(ex).c_str(), TEXT("物理DNSホスト名の変更エラー"), MB_OK | MB_ICONERROR);
		}
	} else if (wID == IDC_CHG3) {
		try {
			CHANGEDLG_PARAM param;
			param.pszCaption = TEXT("物理DNSサフィックス");
			param.strCurrent = kjm::util::getDlgItemText(hwnd, IDC_ComputerNamePhysicalDnsDomain);

			if (DialogBoxParam(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDD_CHANGE), hwnd, ChangeDlg_DlgProc, (LPARAM)&param) == IDOK) {
				if (kjm::util::__SetComputerNameEx(ComputerNamePhysicalDnsDomain, param.strNext.c_str())) {
					MessageBox(hwnd, TEXT("再起動後、有効になります。"), TEXT("物理DNSサフィックスの変更"), MB_OK | MB_ICONWARNING);
				} else {
					MessageBox(hwnd, kjm::util::formatMessageBySystem(GetLastError()).c_str(), TEXT("物理DNSサフィックスの変更エラー"), MB_OK | MB_ICONERROR);
				}
			}
		} catch (DWORD ex) {
			MessageBox(hwnd, kjm::util::formatMessageBySystem(ex).c_str(), TEXT("物理DNSサフィックスの変更エラー"), MB_OK | MB_ICONERROR);
		}
	}

	return FALSE;
}

//---------------------------------------------------------------------------------------
// メインダイアログのプロシージャ
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
// プログラムスタート
//---------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDlg_DlgProc);
}
