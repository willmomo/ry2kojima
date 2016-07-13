#include <windows.h>
#include <windowsx.h>
#include <cpl.h>
#include "resource.h"
#include "CPlApplet.h"

// アプリケーション内で使用するインスタンスハンドル
HINSTANCE hInst = NULL;

BOOL DlgOnInit(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	HICON hIcon;

	// アイコンを設定します
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
		// ダイアログボックスを破棄します
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
	MessageBox(hwnd, "CPlSampleのテスト", "CPlApplet sample", MB_OK);
}

LONG CALLBACK CPlApplet(HWND hwnd, UINT uMsg, LONG lParam1, LONG lParam2)
{
	LPCPLINFO lpCPlInfo;

	switch (uMsg) {
	case CPL_INIT:
		// インスタンスを取得します
		hInst = GetModuleHandle("CPlApplet.cpl");
		return TRUE;
	case CPL_GETCOUNT:
		// アプレットは1種類のため、1を返します
		return 1;
		break;
	case CPL_INQUIRE:
		{
			// ダイアログボックスの設定を行います。
			lpCPlInfo = (LPCPLINFO)lParam2;
			lpCPlInfo->lData = 0;
			lpCPlInfo->idIcon = IDI_GENERIC;
			lpCPlInfo->idName = IDS_NAME;
			lpCPlInfo->idInfo = IDS_INFO;
		}
		break;
	case CPL_DBLCLK:
		// ダイアログボックスを表示します
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)MainDlgProc);
		break;
	case CPL_STOP:
		// ダイアログボックス終了後の処理を行います。
		break;
	case CPL_EXIT:
		// FreeLibrary()関数が呼ばれる前の処理を行います
		break;
	default:
		break;
	}

	return 0;
}
