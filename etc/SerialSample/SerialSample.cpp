#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "resource.h"


HANDLE g_hCom = INVALID_HANDLE_VALUE;


void UpdateControl(HWND hwnd, BOOL fOpen) {
	EnableWindow(GetDlgItem(hwnd, IDC_OPEN), !fOpen);
	EnableWindow(GetDlgItem(hwnd, IDC_CLOSE), fOpen);
	EnableWindow(GetDlgItem(hwnd, IDC_GOTO00), fOpen);
	EnableWindow(GetDlgItem(hwnd, IDC_GOTO01), fOpen);
	EnableWindow(GetDlgItem(hwnd, IDC_GOTO02), fOpen);
	EnableWindow(GetDlgItem(hwnd, IDC_GOTO03), fOpen);
	EnableWindow(GetDlgItem(hwnd, IDC_GOTO04), fOpen);
	EnableWindow(GetDlgItem(hwnd, IDC_GOTO05), fOpen);
	EnableWindow(GetDlgItem(hwnd, IDC_GOTO06), fOpen);
	EnableWindow(GetDlgItem(hwnd, IDC_GOTO07), fOpen);
	EnableWindow(GetDlgItem(hwnd, IDC_GOTO08), fOpen);
	EnableWindow(GetDlgItem(hwnd, IDC_GOTO09), fOpen);
	EnableWindow(GetDlgItem(hwnd, IDC_GOTO10), fOpen);
}


BOOL OnInitDialog(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	TCHAR buf[32];

	// コンボボックスに COM1 ～ COM9 を追加する
	HWND hwndComPort = GetDlgItem(hwnd, IDC_COMPORT);
	for (int i = 1; i <= 9; i++) {
		wsprintf(buf, _T("COM%d"), i);
		SendMessage(hwndComPort, CB_ADDSTRING, 0, (LPARAM)buf);
	}

	// COM1を選択状態にする。
	SendMessage(hwndComPort, CB_SETCURSEL, 0, 0);

	// 開くボタンを有効に、閉じるボタンを無効にしておく
	// 遷移ボタンは、無効にしておく
	UpdateControl(hwnd, FALSE);

	return FALSE;
}


BOOL OnOpen(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	TCHAR szComPort[32];
	GetDlgItemText(hwnd, IDC_COMPORT, szComPort, sizeof(szComPort));

	g_hCom = CreateFile(szComPort, GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	if (g_hCom == INVALID_HANDLE_VALUE) {	// オープンに失敗？
		TCHAR msg[512];
		wsprintf(msg, _T("'%s' がオープンできません(%u)"), szComPort, GetLastError());
		MessageBox(hwnd, msg, _T("開くエラー"), MB_OK);
		return TRUE;
	}

	DCB dcb;
	GetCommState(g_hCom, &dcb);

	dcb.BaudRate = CBR_19200;
	dcb.ByteSize = 8;
	dcb.Parity = EVENPARITY;
	dcb.StopBits = ONESTOPBIT;
	
	SetCommState(g_hCom, &dcb);

	GetCommState(g_hCom, &dcb);

	DWORD dwEvMask = EV_RXCHAR;
	SetCommMask(g_hCom, dwEvMask);
	SetupComm( g_hCom, 4096, 4096 ) ;
	PurgeComm( g_hCom, PURGE_TXABORT | PURGE_RXABORT |PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
	EscapeCommFunction(g_hCom, SETDTR);

	COMMTIMEOUTS cto;
	cto.ReadIntervalTimeout = 0xFFFFFFFF ;
	cto.ReadTotalTimeoutMultiplier = 0 ;
	cto.ReadTotalTimeoutConstant = 1000 ;
	cto.WriteTotalTimeoutMultiplier = 0 ;
	cto.WriteTotalTimeoutConstant = 1000 ;
	SetCommTimeouts(g_hCom, &cto);

	// 開くボタンを無効に、閉じるボタンを有効に、
	// 遷移ボタンも有効にしておく
	UpdateControl(hwnd, TRUE);

	return TRUE;
}


BOOL OnClose(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	CloseHandle(g_hCom);
	g_hCom = INVALID_HANDLE_VALUE;

	// 開くボタンを無効に、閉じるボタンを有効に、
	// 遷移ボタンも有効にしておく
	UpdateControl(hwnd, FALSE);

	return TRUE;
}


BOOL OnGoTo00(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	DWORD dwNumberOfBytesWritten = 0;
	BOOL fRtn;
	char buf[512] = {0xfe, 0x0c, 0x08, 0x45, 0x77, 0x00, 0x31, 0x32, 0x33, 0x34, 0x32, 0xff};

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO01), FALSE);

	WriteFile(g_hCom, buf, 12, &dwNumberOfBytesWritten, NULL);

	DWORD bytesRead = 0;
	char readBuf[32];
	if (ReadFile(g_hCom, readBuf, sizeof(readBuf), &bytesRead, NULL) == FALSE) {
		MessageBox(hwnd, _T("読み込みエラー"), _T("エラー"), MB_OK);
	}

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO01), TRUE);

	return TRUE;
}


BOOL OnGoTo01(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	DWORD dwNumberOfBytesWritten = 0;
	BOOL fRtn;
	char buf[512] = {0xfe, 0x0c, 0x08, 0x45, 0x77, 0x01, 0x31, 0x32, 0x33, 0x34, 0x33, 0xff};

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO01), FALSE);

	WriteFile(g_hCom, buf, 12, &dwNumberOfBytesWritten, NULL);

	DWORD bytesRead = 0;
	char readBuf[32];
	if (ReadFile(g_hCom, readBuf, sizeof(readBuf), &bytesRead, NULL) == FALSE) {
		MessageBox(hwnd, _T("読み込みエラー"), _T("エラー"), MB_OK);
	}

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO01), TRUE);

	return TRUE;
}


BOOL OnGoTo02(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	DWORD dwNumberOfBytesWritten = 0;
	BOOL fRtn;
	char buf[512] = {0xfe, 0x0c, 0x08, 0x45, 0x77, 0x02, 0x31, 0x32, 0x33, 0x34, 0x30, 0xff};

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO02), FALSE);

	WriteFile(g_hCom, buf, 12, &dwNumberOfBytesWritten, NULL);

	DWORD bytesRead = 0;
	char readBuf[32];
	if (ReadFile(g_hCom, readBuf, sizeof(readBuf), &bytesRead, NULL) == FALSE) {
		MessageBox(hwnd, _T("読み込みエラー"), _T("エラー"), MB_OK);
	}

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO02), TRUE);

	return TRUE;
}


BOOL OnGoTo03(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	DWORD dwNumberOfBytesWritten = 0;
	BOOL fRtn;
	char buf[512] = {0xfe, 0x0c, 0x08, 0x45, 0x77, 0x03, 0x31, 0x32, 0x33, 0x34, 0x31, 0xff};

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO03), FALSE);

	WriteFile(g_hCom, buf, 12, &dwNumberOfBytesWritten, NULL);

	DWORD bytesRead = 0;
	char readBuf[32];
	if (ReadFile(g_hCom, readBuf, sizeof(readBuf), &bytesRead, NULL) == FALSE) {
		MessageBox(hwnd, _T("読み込みエラー"), _T("エラー"), MB_OK);
	}

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO03), TRUE);

	return TRUE;
}


BOOL OnGoTo04(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	DWORD dwNumberOfBytesWritten = 0;
	BOOL fRtn;
	char buf[512] = {0xfe, 0x0c, 0x08, 0x45, 0x77, 0x04, 0x31, 0x32, 0x33, 0x34, 0x36, 0xff};

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO04), FALSE);

	WriteFile(g_hCom, buf, 12, &dwNumberOfBytesWritten, NULL);

	DWORD bytesRead = 0;
	char readBuf[32];
	if (ReadFile(g_hCom, readBuf, sizeof(readBuf), &bytesRead, NULL) == FALSE) {
		MessageBox(hwnd, _T("読み込みエラー"), _T("エラー"), MB_OK);
	}

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO04), TRUE);

	return TRUE;
}


BOOL OnGoTo05(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	DWORD dwNumberOfBytesWritten = 0;
	BOOL fRtn;
	char buf[512] = {0xfe, 0x0c, 0x08, 0x45, 0x77, 0x05, 0x31, 0x32, 0x33, 0x34, 0x37, 0xff};

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO05), FALSE);

	WriteFile(g_hCom, buf, 12, &dwNumberOfBytesWritten, NULL);

	DWORD bytesRead = 0;
	char readBuf[32];
	if (ReadFile(g_hCom, readBuf, sizeof(readBuf), &bytesRead, NULL) == FALSE) {
		MessageBox(hwnd, _T("読み込みエラー"), _T("エラー"), MB_OK);
	}

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO05), TRUE);

	return TRUE;
}


BOOL OnGoTo06(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	DWORD dwNumberOfBytesWritten = 0;
	BOOL fRtn;
	char buf[512] = {0xfe, 0x0c, 0x08, 0x45, 0x77, 0x06, 0x31, 0x32, 0x33, 0x34, 0x34, 0xff};

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO06), FALSE);

	WriteFile(g_hCom, buf, 12, &dwNumberOfBytesWritten, NULL);

	DWORD bytesRead = 0;
	char readBuf[32];
	if (ReadFile(g_hCom, readBuf, sizeof(readBuf), &bytesRead, NULL) == FALSE) {
		MessageBox(hwnd, _T("読み込みエラー"), _T("エラー"), MB_OK);
	}

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO06), TRUE);

	return TRUE;
}


BOOL OnGoTo07(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	DWORD dwNumberOfBytesWritten = 0;
	BOOL fRtn;
	char buf[512] = {0xfe, 0x0c, 0x08, 0x45, 0x77, 0x07, 0x31, 0x32, 0x33, 0x34, 0x35, 0xff};

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO07), FALSE);

	WriteFile(g_hCom, buf, 12, &dwNumberOfBytesWritten, NULL);

	DWORD bytesRead = 0;
	char readBuf[32];
	if (ReadFile(g_hCom, readBuf, sizeof(readBuf), &bytesRead, NULL) == FALSE) {
		MessageBox(hwnd, _T("読み込みエラー"), _T("エラー"), MB_OK);
	}

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO07), TRUE);

	return TRUE;
}


BOOL OnGoTo08(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	DWORD dwNumberOfBytesWritten = 0;
	BOOL fRtn;
	char buf[512] = {0xfe, 0x0c, 0x08, 0x45, 0x77, 0x08, 0x31, 0x32, 0x33, 0x34, 0x3a, 0xff};

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO08), FALSE);

	WriteFile(g_hCom, buf, 12, &dwNumberOfBytesWritten, NULL);

	DWORD bytesRead = 0;
	char readBuf[32];
	if (ReadFile(g_hCom, readBuf, sizeof(readBuf), &bytesRead, NULL) == FALSE) {
		MessageBox(hwnd, _T("読み込みエラー"), _T("エラー"), MB_OK);
	}

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO08), TRUE);

	return TRUE;
}


BOOL OnGoTo09(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	DWORD dwNumberOfBytesWritten = 0;
	BOOL fRtn;
	char buf[512] = {0xfe, 0x0c, 0x08, 0x45, 0x77, 0x09, 0x31, 0x32, 0x33, 0x34, 0x3b, 0xff};

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO09), FALSE);

	WriteFile(g_hCom, buf, 12, &dwNumberOfBytesWritten, NULL);

	DWORD bytesRead = 0;
	char readBuf[32];
	if (ReadFile(g_hCom, readBuf, sizeof(readBuf), &bytesRead, NULL) == FALSE) {
		MessageBox(hwnd, _T("読み込みエラー"), _T("エラー"), MB_OK);
	}

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO09), TRUE);

	return TRUE;
}


BOOL OnGoTo10(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	DWORD dwNumberOfBytesWritten = 0;
	BOOL fRtn;
	char buf[512] = {0xfe, 0x0c, 0x08, 0x45, 0x77, 0x0a, 0x31, 0x32, 0x33, 0x34, 0x38, 0xff};

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO10), FALSE);

	WriteFile(g_hCom, buf, 12, &dwNumberOfBytesWritten, NULL);

	DWORD bytesRead = 0;
	char readBuf[32];
	if (ReadFile(g_hCom, readBuf, sizeof(readBuf), &bytesRead, NULL) == FALSE) {
		MessageBox(hwnd, _T("読み込みエラー"), _T("エラー"), MB_OK);
	}

	EnableWindow(GetDlgItem(hwnd, IDC_GOTO10), TRUE);

	return TRUE;
}


BOOL CALLBACK Main_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
		return OnInitDialog(hwnd, wParam, lParam);

	case WM_COMMAND:
		{
			WORD wID = LOWORD(wParam);
			if (wID == IDOK || wID == IDCANCEL) {
				EndDialog(hwnd, 0);
				return TRUE;
			} else if (wID == IDC_OPEN) {
				return OnOpen(hwnd, wParam, lParam);
			} else if (wID == IDC_CLOSE) {
				return OnClose(hwnd, wParam, lParam);
			} else if (wID == IDC_GOTO00) {
				return OnGoTo00(hwnd, wParam, lParam);
			} else if (wID == IDC_GOTO01) {
				return OnGoTo01(hwnd, wParam, lParam);
			} else if (wID == IDC_GOTO02) {
				return OnGoTo02(hwnd, wParam, lParam);
			} else if (wID == IDC_GOTO03) {
				return OnGoTo03(hwnd, wParam, lParam);
			} else if (wID == IDC_GOTO04) {
				return OnGoTo04(hwnd, wParam, lParam);
			} else if (wID == IDC_GOTO05) {
				return OnGoTo05(hwnd, wParam, lParam);
			} else if (wID == IDC_GOTO06) {
				return OnGoTo06(hwnd, wParam, lParam);
			} else if (wID == IDC_GOTO07) {
				return OnGoTo07(hwnd, wParam, lParam);
			} else if (wID == IDC_GOTO08) {
				return OnGoTo08(hwnd, wParam, lParam);
			} else if (wID == IDC_GOTO09) {
				return OnGoTo09(hwnd, wParam, lParam);
			} else if (wID == IDC_GOTO10) {
				return OnGoTo10(hwnd, wParam, lParam);
			}
		}
		break;
	}

	return FALSE;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, Main_DlgProc);
}
