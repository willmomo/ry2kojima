#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include "dcp.h"
#include "resource.h"

char g_szFilename[MAX_PATH];

class SbRect : public RECT
{
public:
	SbRect() { ::SetRectEmpty(this); };
	virtual ~SbRect() {};

	int Width() { return right - left; };
	int Height() { return bottom - top; };
};

void OnInitDialog(HWND hwnd)
{
	SbRect rcWorkArea, rcWindow;

	SystemParametersInfo(SPI_GETWORKAREA, 0, (LPRECT)&rcWorkArea, 0);
	GetWindowRect(hwnd, &rcWindow);
	MoveWindow(hwnd,
		(rcWorkArea.Width() - rcWindow.Width()) / 2,
		(rcWorkArea.Height() - rcWindow.Height()) / 2,
		rcWindow.Width(), rcWindow.Height() / 2, TRUE);
}

void CopyFD(HWND hwnd, char* pszFname)
{
	char szDst[MAX_PATH];

	wsprintf(szDst, "A:\\%s", pszFname);
	HANDLE hf = CreateFile(szDst, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!hf) {
		char szMsgBuf[256];
		FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM, NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
			szMsgBuf, sizeof(szMsgBuf), NULL);
		OutputDebugString(szMsgBuf);
		OutputDebugString("\n");
	}
	if (hf) CloseHandle(hf);
}

void CopyABoxFile(HWND hwnd, char* pszSrc, char* pszDst)
{
	HANDLE hfSrc, hfDst;

	// コピー元ファイルを開く
	hfSrc = CreateFile(pszSrc, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	// コピー先ファイルを開く
	hfDst = CreateFile(pszDst, GENERIC_WRITE, 0, NULL, CREATE_NEW, 
		FILE_ATTRIBUTE_NORMAL, NULL);
}

void RestoreLastDate(char* pszFilename)
{
	char szLastDate[64];

	// [profile]セクションのlastdate_oldキーの値を取得する
	GetPrivateProfileString("profile", "lastdate_old", "", 
		szLastDate, sizeof(szLastDate), pszFilename);

	// [profile]セクションのlastdateキーの値にする
	WritePrivateProfileString("profile", "lastdate", 
		szLastDate, pszFilename);
}

char* GetErrMsg(HWND hwnd, int exitCode)
{
	static char szErrMsg[256];
	HINSTANCE hinst = GetWindowInstance(hwnd);

	switch (exitCode) {
	case -1:
		LoadString(hinst, IDS_E001, szErrMsg, sizeof(szErrMsg));
		break;
	case -2:
		LoadString(hinst, IDS_E002, szErrMsg, sizeof(szErrMsg));
		break;
	case -3:
		LoadString(hinst, IDS_E003, szErrMsg, sizeof(szErrMsg));
		break;
	case -4:
		LoadString(hinst, IDS_E004, szErrMsg, sizeof(szErrMsg));
		break;
	case -5:
		LoadString(hinst, IDS_E005, szErrMsg, sizeof(szErrMsg));
		break;
	case -6:
		LoadString(hinst, IDS_E006, szErrMsg, sizeof(szErrMsg));
		break;
	case -7:
		LoadString(hinst, IDS_E007, szErrMsg, sizeof(szErrMsg));
		break;
	case -8:
		LoadString(hinst, IDS_E008, szErrMsg, sizeof(szErrMsg));
		break;
	case -20:
		LoadString(hinst, IDS_E020, szErrMsg, sizeof(szErrMsg));
		break;
	case -21:
		LoadString(hinst, IDS_E021, szErrMsg, sizeof(szErrMsg));
		break;
	case -22:
		LoadString(hinst, IDS_E022, szErrMsg, sizeof(szErrMsg));
		break;
	case -23:
		LoadString(hinst, IDS_E023, szErrMsg, sizeof(szErrMsg));
		break;
	case -24:
		LoadString(hinst, IDS_E024, szErrMsg, sizeof(szErrMsg));
		break;
	case -40:
		LoadString(hinst, IDS_E040, szErrMsg, sizeof(szErrMsg));
		break;
	case -41:
		LoadString(hinst, IDS_E041, szErrMsg, sizeof(szErrMsg));
		break;
	case -42:
		LoadString(hinst, IDS_E042, szErrMsg, sizeof(szErrMsg));
		break;
	case -43:
		LoadString(hinst, IDS_E043, szErrMsg, sizeof(szErrMsg));
		break;
	case -44:
		LoadString(hinst, IDS_E044, szErrMsg, sizeof(szErrMsg));
		break;
	case -60:
		LoadString(hinst, IDS_E060, szErrMsg, sizeof(szErrMsg));
		break;
	case -61:
		LoadString(hinst, IDS_E061, szErrMsg, sizeof(szErrMsg));
		break;
	case -80:
		LoadString(hinst, IDS_E080, szErrMsg, sizeof(szErrMsg));
		break;
	case -81:
		LoadString(hinst, IDS_E081, szErrMsg, sizeof(szErrMsg));
		break;
	case -82:
		LoadString(hinst, IDS_E082, szErrMsg, sizeof(szErrMsg));
		break;
	case -83:
		LoadString(hinst, IDS_E083, szErrMsg, sizeof(szErrMsg));
		break;
	case -84:
		LoadString(hinst, IDS_E084, szErrMsg, sizeof(szErrMsg));
		break;
	case -100:
		LoadString(hinst, IDS_E100, szErrMsg, sizeof(szErrMsg));
		break;
	case -101:
		LoadString(hinst, IDS_E101, szErrMsg, sizeof(szErrMsg));
		break;
	case -102:
		LoadString(hinst, IDS_E102, szErrMsg, sizeof(szErrMsg));
		break;
	case -120:
		LoadString(hinst, IDS_E120, szErrMsg, sizeof(szErrMsg));
		break;
	case -130:
		LoadString(hinst, IDS_E130, szErrMsg, sizeof(szErrMsg));
		break;
	case -131:
		LoadString(hinst, IDS_E131, szErrMsg, sizeof(szErrMsg));
		break;
	default:
		szErrMsg[0] = '\0';
		break;
	}

	return szErrMsg;
}

void OnAboxCreate(HWND hwnd)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	char szCmdLine[1024];

	EnableWindow(GetDlgItem(hwnd, IDC_CREATE), FALSE);
	HCURSOR hcurOld = SetCursor(LoadCursor(NULL, IDC_WAIT));

	wsprintf(szCmdLine, "mkabox2.exe ");

	// -sオプションを付けるか否か
	if (IsWindowEnabled(GetDlgItem(hwnd, IDC_START)) &&
		GetWindowTextLength(GetDlgItem(hwnd, IDC_START)) != 0) {
		char szText[256];
		GetDlgItemText(hwnd, IDC_START, szText, sizeof(szText));
		wsprintf(&szCmdLine[lstrlen(szCmdLine)], "-s=%s ", szText);
	}

	// -eオプションを付けるか否か
	if (IsWindowEnabled(GetDlgItem(hwnd, IDC_END)) &&
		GetWindowTextLength(GetDlgItem(hwnd, IDC_END)) != 0) {
		char szText[256];
		GetDlgItemText(hwnd, IDC_END, szText, sizeof(szText));
		wsprintf(&szCmdLine[lstrlen(szCmdLine)], "-e=%s ", szText);
	}

	// -oオプションを付けるか否か
	char szOutput[256];
	if (IsWindowEnabled(GetDlgItem(hwnd, IDC_OUTPUT)) &&
		GetWindowTextLength(GetDlgItem(hwnd, IDC_OUTPUT)) != 0) {
		GetDlgItemText(hwnd, IDC_OUTPUT, szOutput, sizeof(szOutput));
		wsprintf(&szCmdLine[lstrlen(szCmdLine)], "-o=%s ", szOutput);
	} else {
		lstrcpy(szOutput, "abox.dat");
		wsprintf(&szCmdLine[lstrlen(szCmdLine)], "-o=%s ", szOutput);
	}

	// CreateProcess()の引数を設定します
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	if(!CreateProcess(NULL, 
		szCmdLine,
		NULL,NULL,FALSE,0,NULL,NULL,&si,&pi)){ 

		char szMsgBuf[256];
		FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM, NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
			szMsgBuf, sizeof(szMsgBuf), NULL);
		OutputDebugString(szMsgBuf);
		OutputDebugString("\n");
		SetCursor(hcurOld);
		return;
	} 

	WaitForSingleObject(pi.hProcess, INFINITE);

	DWORD dwExitCode;
	GetExitCodeProcess(pi.hProcess, &dwExitCode);

	if (dwExitCode != 0) {
		char szBuffer[512];
		wsprintf(szBuffer, "%s (エラーコード=%d)", 
			GetErrMsg(hwnd, (int)dwExitCode), dwExitCode);
		MessageBox(hwnd, szBuffer, "異常終了", MB_OK);
	} else {
		if (ABoxCopyDisk(hwnd, szOutput)) {	// ファイルをFDにコピー
			MessageBox(hwnd, "フロッピーディスクにコピー終了しました",
				"正常終了", MB_OK);
		} else {
			RestoreLastDate(g_szFilename);	// lastdateキーを元に戻す
			MessageBox(hwnd, "異常終了 (フロッピーにコピーされませんでした)",
				"異常終了", MB_OK);
		}
	}

	SetCursor(hcurOld);
	EnableWindow(GetDlgItem(hwnd, IDC_CREATE), TRUE);

	EndDialog(hwnd, 0);		// 終了する
}

void OnOption(HWND hwnd)
{
	SbRect rc;

	EnableWindow(GetDlgItem(hwnd, IDC_OPTION), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_START), TRUE);
	EnableWindow(GetDlgItem(hwnd, IDC_END), TRUE);
	EnableWindow(GetDlgItem(hwnd, IDC_OUTPUT), TRUE);
	SetFocus(GetDlgItem(hwnd, IDC_START));

	GetWindowRect(hwnd, &rc);
	MoveWindow(hwnd,
		rc.left, rc.top,
		rc.Width(), rc.Height() * 2, TRUE);
}

BOOL CALLBACK MainDlg_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_INITDIALOG:
		OnInitDialog(hwnd);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			EndDialog(hwnd, 0);
			return TRUE;
		case IDC_CREATE:
			OnAboxCreate(hwnd);
			return TRUE;
		case IDC_OPTION:
			OnOption(hwnd);
			return TRUE;
		}
		return FALSE;
	}

	return FALSE;
}

void CopyLastDate(char* pszFilename)
{
	char szLastDate[64];

	// [profile]セクションのlastdateキーの値を取得する
	GetPrivateProfileString("profile", "lastdate", "", 
		szLastDate, sizeof(szLastDate), pszFilename);

	// [profile]セクションのlastdate_oldキーの値にする
	WritePrivateProfileString("profile", "lastdate_old", 
		szLastDate, pszFilename);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HWND hwnd = FindWindow(NULL, "Wmkabox2");
	if (hwnd) {
		SetForegroundWindow(hwnd);
		return 0;
	}

	// 実行ファイルのフルパスを取得する
	GetModuleFileName(hInstance, g_szFilename, sizeof(g_szFilename));

	// iniファイルのフルパスを作成する
	char szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
	_splitpath(g_szFilename, szDrive, szDir, NULL, NULL);
	_makepath(g_szFilename, szDrive, szDir, "mkabox2", ".ini");

	CopyLastDate(g_szFilename);	// lastdateキーをlastdate_bakキーにコピーする

	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, (DLGPROC)MainDlg_DlgProc);
}
