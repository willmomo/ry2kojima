/////////////////////////////////////////////////////////////////////////////
//	SDI Generic

#include "main.h"
#include "ext.h"

//XPrestar * pps;

/////////////////////////////////////////////////////////////////////////////
//	About・ダイアログ・プロシージャ

BOOL CALLBACK AboutDialogProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (wParam == IDOK) {
			EndDialog(hwndDlg, 0);
			return TRUE;
		} else if (wParam == IDCANCEL) {
			EndDialog(hwndDlg, -1);
			return TRUE;
		}
		break;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	通信設定・ダイアログ・プロシージャ

BOOL CALLBACK ComsetDialogProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int id;
	DCB dcb;
	char Comm[16];

	switch (msg) {
	case WM_INITDIALOG:
		XTTY_ReadDCB(&dcb);
		XTTY_ReadCom(Comm);

		switch(dcb.BaudRate){
		case CBR_2400:	id = IDC_2400;	break;
		case CBR_4800:	id = IDC_4800;	break;
		case CBR_19200:	id = IDC_19200;	break;
		case CBR_38400:	id = IDC_38400;	break;
		case CBR_9600:
		default:		id = IDC_9600;	break;
		}
		CheckDlgButton(hwndDlg, id, 1);

		switch(dcb.ByteSize){
		case 5:	id = IDC_DAT5;	break;
		case 6:	id = IDC_DAT6;	break;
		case 7:	id = IDC_DAT7;	break;
		case 8:
		default:id = IDC_DAT8;	break;
		}
		CheckDlgButton(hwndDlg, id, 1);

		switch(dcb.Parity){
		case EVENPARITY:id = IDC_EVEN;	break;
		case ODDPARITY: id = IDC_ODD;	break;
		case NOPARITY:
		default:		id = IDC_NONE;	break;
		}
		CheckDlgButton(hwndDlg, id, 1);

		switch(dcb.StopBits){
		case ONE5STOPBITS:	id = IDC_STOP15;	break;
		case TWOSTOPBITS:	id = IDC_STOP2;	break;
		case ONESTOPBIT:
		default:			id = IDC_STOP1;	break;
		}
		CheckDlgButton(hwndDlg, id, 1);

		if(strcmp(Comm, "COM1") == 0) id = IDC_COM1;
		else if(strcmp(Comm, "COM3") == 0) id = IDC_COM3;
		else if(strcmp(Comm, "COM4") == 0) id = IDC_COM4;
		else id = IDC_COM2;
		CheckDlgButton(hwndDlg, id, 1);

		return TRUE;

	case WM_COMMAND:
		switch(wParam) {
		case IDOK:
			XTTY_ReadDCB(&dcb);
			XTTY_ReadCom(Comm);

			if(IsDlgButtonChecked(hwndDlg, IDC_2400))	dcb.BaudRate = CBR_2400;
			else if(IsDlgButtonChecked(hwndDlg, IDC_4800))	dcb.BaudRate = CBR_4800;
			else if(IsDlgButtonChecked(hwndDlg, IDC_19200))	dcb.BaudRate = CBR_19200;
			else if(IsDlgButtonChecked(hwndDlg, IDC_38400))	dcb.BaudRate = CBR_38400;
			else dcb.BaudRate = CBR_9600;

			if(IsDlgButtonChecked(hwndDlg, IDC_DAT5))	dcb.ByteSize = 5;
			else if(IsDlgButtonChecked(hwndDlg, IDC_DAT6))	dcb.ByteSize = 6;
			else if(IsDlgButtonChecked(hwndDlg, IDC_DAT7))	dcb.ByteSize = 7;
			else dcb.ByteSize = 8;

			if(IsDlgButtonChecked(hwndDlg, IDC_EVEN))	dcb.Parity = EVENPARITY;
			else if(IsDlgButtonChecked(hwndDlg, IDC_ODD))	dcb.Parity = ODDPARITY;
			else dcb.Parity = NOPARITY;

			if(IsDlgButtonChecked(hwndDlg, IDC_STOP15))	dcb.StopBits = ONE5STOPBITS;
			else if(IsDlgButtonChecked(hwndDlg, IDC_STOP2))	dcb.StopBits = TWOSTOPBITS;
			else dcb.StopBits = ONESTOPBIT;

			if(IsDlgButtonChecked(hwndDlg, IDC_COM1))	sprintf(Comm, "COM1");
			else if(IsDlgButtonChecked(hwndDlg, IDC_COM3))	sprintf(Comm, "COM3");
			else if(IsDlgButtonChecked(hwndDlg, IDC_COM4))	sprintf(Comm, "COM4");
			else sprintf(Comm, "COM2");

			XTTY_WriteDCB(&dcb);
			XTTY_WriteCom(Comm);

	    EndDialog(hwndDlg, 0);
			return TRUE;
		case IDCANCEL:
			EndDialog(hwndDlg, -1);
			return TRUE;
		}
		break;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// メイン・ウィンドウ・プロシージャ

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HMENU hmenu;
	HWND hbtn1, hbtn2;
	DCB dcb;
	char Comm[16];

	switch (msg) {
	case WM_CREATE:
		XTTY_XTTY(ghinst, hwnd);
		XPrestar_XPrestar(ghinst, hwnd);

		hbtn1 = CreateWindow(
			"BUTTON",
			"キャンセル",
			BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
			DEFWNDWSIZE - 225,
			DEFWNDHSIZE - 96,
			100,
			38,
			hwnd,
	        IDB_CANCEL,
			ghinst,
			0);
		if (!hbtn1)
			return FALSE;

		hbtn2 = CreateWindow(
			"BUTTON",
			"ログ表示",
			BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
			DEFWNDWSIZE - 110,
			DEFWNDHSIZE - 96,
			90,
			38,
			hwnd,
	        IDB_DISPLOG,
			ghinst,
			0);
		if (!hbtn2)
			return FALSE;

		BuildCommDCB("COM2:9600,n,8,1", &dcb);
		sprintf(Comm, "COM2");
		ReadIniDCB(&dcb);
		XTTY_WriteDCB(&dcb);
		ReadIniCom(Comm);
		XTTY_WriteCom(Comm);

		XTTY_Connect(TRUE);
		return 0;

	case WM_COMMAND:
		if (wParam == ID_HELP_ABOUT) {
			DialogBox(ghinst, "About", hwnd, (DLGPROC)AboutDialogProc);
		} else if (wParam == IDM_COMSET) {
			if(DialogBox(ghinst, "COMSET", hwnd, (DLGPROC)ComsetDialogProc) == 0) {
				XTTY_Connect(FALSE);
				XTTY_Connect(TRUE);
			}
		} else if (wParam == IDM_FILE_EXIT) {
			DestroyWindow(hwnd);
		} else if (wParam == IDM_ONLINE) {
			XTTY_Connect(TRUE);
		} else if (wParam == IDM_OFFLINE) {
			XTTY_Connect(FALSE);
		} else if (wParam == IDM_GRP00) {
			XPrestar_OnGrp00();
		} else if (wParam == IDM_GRP01) {
			XPrestar_OnGrp01();
		} else if (wParam == IDM_GRP02) {
			XPrestar_OnGrp02();
		} else if (wParam == IDM_VIEW_STATUS) {
			hmenu = GetMenu(hwnd);
			if (!(GetMenuState(hmenu, IDM_VIEW_STATUS, MF_BYCOMMAND) & MF_CHECKED)) {
				CheckMenuItem(hmenu, IDM_VIEW_STATUS, MF_BYCOMMAND | MF_CHECKED);
				ghwndStatusDlg = CreateDialog(ghinst, "status", hwnd, (DLGPROC)StatusDlgProc);
			}
		} else if (wParam == IDM_LOG) {
			hmenu = GetMenu(hwnd);
			if (!(GetMenuState(hmenu, IDM_LOG, MF_BYCOMMAND) & MF_CHECKED)) {
				CheckMenuItem(hmenu, IDM_LOG, MF_BYCOMMAND | MF_CHECKED);
			} else {
				CheckMenuItem(hmenu, IDM_LOG, MF_BYCOMMAND | MF_UNCHECKED);
			}
			XPrestar_Log();
		} else if (wParam == IDB_CANCEL) {
			XPrestar_InitStatus();
		} else if (wParam == IDB_DISPLOG) {
			RECT wndrect;
			GetWindowRect(hwnd, &wndrect);
			if ((wndrect.bottom - wndrect.top) == DEFWNDHSIZE) {
				wndrect.bottom += LOGWNDHSIZE;
			} else {
				wndrect.bottom -= LOGWNDHSIZE;
			}
			MoveWindow(hwnd, wndrect.left, wndrect.top, (wndrect.right - wndrect.left), (wndrect.bottom - wndrect.top), TRUE);
		} else if (wParam == IDM_PLOG) {
			DestroyWindow(hwnd);
		} else {
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		break;
	case WM_PRESTAR:
		XPrestar_OnPrestar(wParam);
		break;
	case WM_CLOSESTATUSDLG:
		hmenu = GetMenu(hwnd);
		CheckMenuItem(hmenu, IDM_VIEW_STATUS, MF_BYCOMMAND | MF_UNCHECKED);
		break;
	case WM_TIMER:
		XPrestar_OnTimer(wParam);
		break;

	case WM_PAINT:
		XPrestar_OnPaint();
		break;

	case WM_CHAR:
		XTTY_OnChar(wParam, lParam);		//	ｷｰｲﾍﾞﾝﾄ
		break;

	case WM_COMMNOTIFY:
		XPrestar_OnCommNotify(wParam, lParam);	//	COMｲﾍﾞﾝﾄ
		break;

	case WM_DESTROY:
		XTTY_ReadDCB(&dcb);
		WriteIniDCB(&dcb);
		XTTY_ReadCom(Comm);
		WriteIniCom(Comm);

		XTTY_deXTTY();
		XPrestar_deXPrestar();
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//	インスタンスの初期化

BOOL InitInstance(int nCmdShow)
{
	HWND hwnd = CreateWindow(
		gszAppName,
		gszWindowName,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
		0,
		0,
		DEFWNDWSIZE,
		DEFWNDHSIZE,
		NULL,
		NULL,
		ghinst,
		0);

	if (!hwnd)
		return FALSE;

	ShowWindow(hwnd, nCmdShow);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	アプリケーションの初期化

BOOL InitApplication()
{
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = ghinst;
	wc.hIcon = LoadIcon(ghinst, MAKEINTRESOURCE(PRESTAR));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = "DEFAULT";
	wc.lpszClassName = gszAppName;

	return RegisterClass(&wc);
}

/////////////////////////////////////////////////////////////////////////////
//	グローバル変数の初期化

void InitGlobal(HINSTANCE hinst)
{
	ghinst = hinst;
	LoadString(ghinst, IDS_APPNAME, gszAppName, sizeof(gszAppName));
	LoadString(ghinst, IDS_WINDOWNAME, gszWindowName, sizeof(gszWindowName));
	LoadString(ghinst, IDS_BAUDRATE, gszBaudRate, sizeof(gszBaudRate));
	LoadString(ghinst, IDS_DATABIT, gszDataBit, sizeof(gszDataBit));
	LoadString(ghinst, IDS_PARITYBIT, gszParityBit, sizeof(gszParityBit));
	LoadString(ghinst, IDS_STOPBIT, gszStopBit, sizeof(gszStopBit));
	LoadString(ghinst, IDS_COMPORT, gszComPort, sizeof(gszComPort));
}

/////////////////////////////////////////////////////////////////////////////
//	プログラム・スタート

int PASCAL WinMain(HINSTANCE hinstCurrent, HINSTANCE hinstPrevious, LPSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;

	InitGlobal(hinstCurrent);		// グローバル変数の初期化

	if (hinstPrevious == NULL)		// ほかのインスタンス?
		if (!InitApplication())		// 共有の項目
			return FALSE;		// 初期化に失敗

	if (!InitInstance(nCmdShow))		// このインスタンスの初期化
		return FALSE;

	// WM_QUITメッセージが送られるまで
	// メッセージの取得とディスパッチを繰り返す

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);		// 仮想キー コードを変換
		DispatchMessage(&msg);		// メッセージをウィンドウにディスパッチ
	}

	return (int)msg.wParam;			// PostQuitMessageの戻り値
}

void WriteProfileInt(LPSTR szSection, LPSTR szKey, int i)
{
	char achBuf[40];

	/*
	 * GetPrivateProfileInt()は符号付きの値を処理できないので
	 * 符号なしの値として書き込む
	 */
	wsprintf(achBuf, "%u", i);
	WritePrivateProfileString(szSection, szKey, achBuf, "prestar.ini");
}

BOOL ReadIniDCB(DCB *dcb)
{
	dcb->BaudRate = (UINT)GetPrivateProfileInt(gszAppName, gszBaudRate, 9600, "prestar.ini");
	dcb->ByteSize = (BYTE)GetPrivateProfileInt(gszAppName, gszDataBit, 8, "prestar.ini");
	dcb->Parity = (BYTE)GetPrivateProfileInt(gszAppName, gszParityBit, NOPARITY, "prestar.ini");
	dcb->StopBits = (BYTE)GetPrivateProfileInt(gszAppName, gszStopBit, ONESTOPBIT, "prestar.ini");;

	return TRUE;
}

BOOL ReadIniCom(char *Comm)
{
	switch(GetPrivateProfileInt(gszAppName, gszComPort, 2, "prestar.ini")) {
	case 1:
		sprintf(Comm, "COM1");
		break;
	case 3:
		sprintf(Comm, "COM3");
		break;
	case 4:
		sprintf(Comm, "COM4");
		break;
	case 2:
	default:
		sprintf(Comm, "COM2");
		break;
	}

	return TRUE;
}

BOOL WriteIniDCB(DCB *dcb)
{
	WriteProfileInt(gszAppName, gszBaudRate, dcb->BaudRate);
	WriteProfileInt(gszAppName, gszDataBit, dcb->ByteSize);
	WriteProfileInt(gszAppName, gszParityBit, dcb->Parity);
	WriteProfileInt(gszAppName, gszStopBit, dcb->StopBits);

	return TRUE;
}

BOOL WriteIniCom(char *Comm)
{
	char comnum;

	if(strcmp(Comm, "COM1") == 0) comnum = 1;
	else if(strcmp(Comm, "COM3") == 0) comnum = 3;
	else if(strcmp(Comm, "COM4") == 0) comnum = 4;
	else comnum = 2;

	WriteProfileInt(gszAppName, gszComPort, comnum);

	return TRUE;
}
