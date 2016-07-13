/////////////////////////////////////////////////////////////////////////////
// メッセージ処理関数

#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <pbt.h>
#include "x.h"
#include "resource.h"
#include "definc.h"

#pragma comment(lib, "winmm.lib")

#define HMSMS(H,M,S,MS)	((H)* 3600000+(M)* 60000+(S)*1000+(MS))
#define GetMSec(MS)	((MS)%1000)
#define GetSec(MS)	(((MS)/1000)%60)
#define GetMin(MS)	(((MS)/60000)%60)
#define GetMin2(MS)	((MS)/60000)
#define GetHour(MS)	((MS)/3600000)

UINT guIDTimer;
DWORD gdwSave = HMSMS(0, 4, 0, 0);
DWORD gdwSave2 = HMSMS(0, 4, 0, 0);
DWORD gdwCnt = HMSMS(0, 4, 0, 0);
DWORD gdwStart;
HBITMAP ghbm7SegBig;
HBITMAP ghbm7SegSmall;
HBITMAP ghbmBkGround;
HBITMAP ghbmStartStop;
HBITMAP ghbmSettei;
HWND ghwndButton[2];
HWND ghwndFrom;
BOOL gfRed = FALSE;
POINT gpt;
BOOL gfClock = FALSE;
BOOL gfEndWindows = FALSE;
BOOL gfTopMost = FALSE;
DWORD gdwLeft;
DWORD gdwTop;
HGLOBAL ghresWave;
LPBYTE glpWave;
HGLOBAL ghresWave2;
LPBYTE glpWave2;
HGLOBAL ghresWave3;
LPBYTE glpWave3;
BOOL gfPlay = FALSE;
BOOL gfPlay2 = FALSE;
BOOL gfPlay3 = FALSE;
NOTIFYICONDATA gnid;
TCHAR gszSerifu[128];
SYSTEM_POWER_STATUS gsps;
BOOL gfPowSys;
BOOL gfClockStart;
BOOL gfEndTime;

/////////////////////////////////////////////////////////////////////////////
// WM_COMMAND::BN_CLICKED処理

void START_OnBnClicked(HWND hwnd, HWND hwndCtl)
{
	OnActionStart(hwnd);
}

/////////////////////////////////////////////////////////////////////////////
// WM_COMMAND::BN_CLICKED処理

void SETTEI_OnBnClicked(HWND hwnd, HWND hwndCtl)
{
	OnActionProperty(hwnd);
}

/////////////////////////////////////////////////////////////////////////////
// WM_CREATE処理

void OnCreate(HWND hwnd)
{
	if (GetSystemPowerStatus(&gsps) == TRUE) {
		gfPowSys = TRUE;			// 電源管理をサポート
	} else {
		gfPowSys = FALSE;			// 電源管理をサポートしていない
	}

	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);

	ghbm7SegBig = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_7SEG_BIG));
	ghbm7SegSmall = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_7SEG_SMALL));
	ghbmBkGround = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BKGROUND));
	ghbmStartStop = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_START));
	ghbmSettei = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SETTEI));

	// サウンドリソースをロードし、ロックする
	HRSRC hResInfo = FindResource(hInstance, MAKEINTRESOURCE(IDR_WAVE3), "WAVE");
	if (hResInfo) {
		ghresWave = LoadResource(hInstance, hResInfo);
		if (ghresWave) {
			glpWave = (LPBYTE)LockResource(ghresWave);
		}
	}
	hResInfo = FindResource(hInstance, MAKEINTRESOURCE(IDR_3HUNWAVE), "WAVE");
	if (hResInfo) {
		ghresWave2 = LoadResource(hInstance, hResInfo);
		if (ghresWave2) {
			glpWave2 = (LPBYTE)LockResource(ghresWave2);
		}
	}
	hResInfo = FindResource(hInstance, MAKEINTRESOURCE(IDR_ENDWAVE), "WAVE");
	if (hResInfo) {
		ghresWave3 = LoadResource(hInstance, hResInfo);
		if (ghresWave3) {
			glpWave3 = (LPBYTE)LockResource(ghresWave3);
		}
	}

	ghwndButton[0] = CreateWindow("BUTTON", "",
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_PUSHBUTTON | BS_NOTIFY,
		263, 16, 27, 25, hwnd, (HMENU)IDW_SETTEI, hInstance, NULL);

	ghwndButton[1] = CreateWindow("BUTTON", "START",
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_PUSHBUTTON | BS_NOTIFY,
		263, 43, 27, 25, hwnd, (HMENU)IDW_START, hInstance, NULL);

	ghwndFrom = CreateWindow("FROM_CONTROL", "",
		WS_POPUP | WS_BORDER,
		0, 0, 138+2, 148+2, hwnd, NULL, hInstance, NULL);

	// タスクトレイにアイコンを登録する
	ZeroMemory(&gnid, sizeof(gnid));
	gnid.cbSize = sizeof(gnid);
	gnid.hWnd = hwnd;
	gnid.uID = 1;
	gnid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	gnid.uCallbackMessage = WMU_NOTIFYICON;
	gnid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPL));
	lstrcpy(gnid.szTip, "Eva Timer Mini");
	Shell_NotifyIcon(NIM_ADD, &gnid);

	if (gfClockStart)
		OnClock(hwnd);
}

/////////////////////////////////////////////////////////////////////////////
// WMU_NOTIFYICON処理

void OnUNotifyIcon(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	switch (lParam) {
	case WM_LBUTTONDOWN:
		if (wParam == 1) {
			SetForegroundWindow(hwnd);
		}
		break;
	case WM_RBUTTONDOWN:
		if (wParam == 1) {
			POINT pt;
			GetCursorPos(&pt);
			HMENU hmenu, hmenuSub;
			hmenu = LoadMenu((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDR_RMENU));
			hmenuSub = GetSubMenu(hmenu, 0);
			TrackPopupMenu(hmenuSub, TPM_LEFTALIGN | TPM_TOPALIGN, pt.x, pt.y, 0, hwnd, NULL);
			DestroyMenu(hmenu);
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// レジストリデータ取得処理

void GetRegistory()
{
	LONG lRtn;
	HKEY hkResult;
	lRtn = RegOpenKeyEx(
		HKEY_CURRENT_USER,
		"Software\\WillSoft\\EvaTimeMini",
		0,
		KEY_ALL_ACCESS,
		&hkResult);

	if (lRtn != ERROR_SUCCESS) {
		// すべての値にデフォルトを設定
		gdwLeft = 0;
		gdwTop = 0;
		gdwSave = gdwCnt = HMSMS(1, 0, 0, 0);
		gfEndWindows = FALSE;
		gfClockStart = FALSE;
		return;
	}

	DWORD dwType;
	DWORD dwcbData = sizeof(gdwLeft);
	lRtn = RegQueryValueEx(
		hkResult,
		"WindowLeft",
		NULL,
		&dwType,
		(LPBYTE)&gdwLeft,
		&dwcbData);
	if (dwType != REG_DWORD || lRtn != ERROR_SUCCESS)
		gdwLeft = 0;

	dwcbData = sizeof(gdwTop);
	lRtn = RegQueryValueEx(
		hkResult,
		"WindowTop",
		NULL,
		&dwType,
		(LPBYTE)&gdwTop,
		&dwcbData);
	if (dwType != REG_DWORD || lRtn != ERROR_SUCCESS)
		gdwTop = 0;

	dwcbData = sizeof(gdwSave);
	lRtn = RegQueryValueEx(
		hkResult,
		"Count",
		NULL,
		&dwType,
		(LPBYTE)&gdwSave,
		&dwcbData);
	if (dwType != REG_DWORD || lRtn != ERROR_SUCCESS)
		gdwSave = HMSMS(1, 0, 0, 0);
	gdwCnt = gdwSave;

	dwcbData = sizeof(gfEndWindows);
	lRtn = RegQueryValueEx(
		hkResult,
		"EndWindows",
		NULL,
		&dwType,
		(LPBYTE)&gfEndWindows,
		&dwcbData);
	if (dwType != REG_DWORD || lRtn != ERROR_SUCCESS)
		gfEndWindows = FALSE;

	dwcbData = sizeof(gfTopMost);
	lRtn = RegQueryValueEx(
		hkResult,
		"TopMost",
		NULL,
		&dwType,
		(LPBYTE)&gfTopMost,
		&dwcbData);
	if (dwType != REG_DWORD || lRtn != ERROR_SUCCESS)
		gfTopMost = FALSE;

	dwcbData = sizeof(gfClockStart);
	lRtn = RegQueryValueEx(
		hkResult,
		"ClockStart",
		NULL,
		&dwType,
		(LPBYTE)&gfClockStart,
		&dwcbData);
	if (dwType != REG_DWORD || lRtn != ERROR_SUCCESS)
		gfClockStart = FALSE;

	dwcbData = sizeof(gfEndTime);
	lRtn = RegQueryValueEx(
		hkResult,
		"EndTime",
		NULL,
		&dwType,
		(LPBYTE)&gfEndTime,
		&dwcbData);
	if (dwType != REG_DWORD || lRtn != ERROR_SUCCESS)
		gfEndTime = FALSE;

	lRtn = RegCloseKey(hkResult);
}

/////////////////////////////////////////////////////////////////////////////
// WM_DRAWITEM処理

void START_OnDrawItem(HWND hwnd, UINT idCtl, LPDRAWITEMSTRUCT lpdis)
{
	INT ofsX, ofsY;
	TCHAR szTitle[32];
	GetWindowText(lpdis->hwndItem, szTitle, sizeof(szTitle));

	ofsY = (lstrcmp("START", szTitle) == 0) ? 0 : 25;
	if (gfRed)	ofsY += 50;

	ofsX = 0;
	if (lpdis->itemState & ODS_DISABLED)	ofsX = 27;
	if (lpdis->itemState & ODS_SELECTED)	ofsX = 54;

	HDC hdcMem = CreateCompatibleDC(lpdis->hDC);
	SelectObject(hdcMem, ghbmStartStop);

	BitBlt(lpdis->hDC, 0, 0, 27, 25, hdcMem, ofsX, ofsY, SRCCOPY);

	DeleteDC(hdcMem);
}

/////////////////////////////////////////////////////////////////////////////
// WM_DRAWITEM処理

void SETTEI_OnDrawItem(HWND hwnd, UINT idCtl, LPDRAWITEMSTRUCT lpdis)
{
	INT ofsX, ofsY;

	ofsY = (gfRed) ? 25 : 0;
	ofsX = 0;
	if (lpdis->itemState & ODS_DISABLED)	ofsX = 27;
	if (lpdis->itemState & ODS_SELECTED)	ofsX = 54;

	HDC hdcMem = CreateCompatibleDC(lpdis->hDC);
	SelectObject(hdcMem, ghbmSettei);

	BitBlt(lpdis->hDC, 0, 0, 27, 25, hdcMem, ofsX, ofsY, SRCCOPY);

	DeleteDC(hdcMem);
}

/////////////////////////////////////////////////////////////////////////////
// WM_DRAWITEM処理

void OnDrawItem(HWND hwnd, UINT idCtl, LPDRAWITEMSTRUCT lpdis)
{
	switch (idCtl) {
	case IDW_START:
		START_OnDrawItem(hwnd, idCtl, lpdis);
		break;
	case IDW_SETTEI:
		SETTEI_OnDrawItem(hwnd, idCtl, lpdis);
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// レジストリの保存処理

void SaveRegistory(HWND hwnd)
{
	XRect rc;
	GetWindowRect(hwnd, &rc);

	LONG lRtn;
	HKEY hkResult;
	lRtn = RegOpenKeyEx(
		HKEY_CURRENT_USER,
		"Software\\WillSoft\\EvaTimeMini",
		0,
		KEY_ALL_ACCESS,
		&hkResult);

	if (lRtn != ERROR_SUCCESS) {
		DWORD dwDisposition;
		lRtn = RegCreateKeyEx(
			HKEY_CURRENT_USER,
			"Software\\WillSoft\\EvaTimeMini",
			0,
			"",
			REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS,
			NULL,
			&hkResult,
			&dwDisposition);

		if (lRtn != ERROR_SUCCESS) {
			OutputDebugString("ﾚｼﾞｽﾄﾘｷｰが作成できないのであきらめる\n");
			return;
		}
	}

	DWORD dwData = rc.left;
	lRtn = RegSetValueEx(
		hkResult,
		"WindowLeft",
		0,
		REG_DWORD,
		(CONST BYTE*)&dwData,
		sizeof(dwData));

	dwData = rc.top;
	lRtn = RegSetValueEx(
		hkResult,
		"WindowTop",
		0,
		REG_DWORD,
		(CONST BYTE*)&dwData,
		sizeof(dwData));

	dwData = gdwSave;
	lRtn = RegSetValueEx(
		hkResult,
		"Count",
		0,
		REG_DWORD,
		(CONST BYTE*)&dwData,
		sizeof(dwData));

	dwData = gfEndWindows;
	lRtn = RegSetValueEx(
		hkResult,
		"EndWindows",
		0,
		REG_DWORD,
		(CONST BYTE*)&dwData,
		sizeof(dwData));

	dwData = gfTopMost;
	lRtn = RegSetValueEx(
		hkResult,
		"TopMost",
		0, REG_DWORD,
		(CONST BYTE*)&dwData,
		sizeof(dwData));

	dwData = gfClockStart;
	lRtn = RegSetValueEx(
		hkResult,
		"ClockStart",
		0, REG_DWORD,
		(CONST BYTE*)&dwData,
		sizeof(dwData));

	dwData = gfEndTime;
	lRtn = RegSetValueEx(
		hkResult,
		"EndTime",
		0, REG_DWORD,
		(CONST BYTE*)&dwData,
		sizeof(dwData));

	lRtn = RegCloseKey(hkResult);
}

/////////////////////////////////////////////////////////////////////////////
// WM_DESTROY処理

void OnDestroy(HWND hwnd)
{
	if (guIDTimer)
		KillTimer(hwnd, guIDTimer);

	DeleteObject(ghbm7SegBig);
	DeleteObject(ghbm7SegSmall);
	DeleteObject(ghbmBkGround);
	DeleteObject(ghbmStartStop);
	DeleteObject(ghbmSettei);

	if (glpWave)	UnlockResource(glpWave);
	if (ghresWave)	FreeResource(ghresWave);

	if (glpWave2)	UnlockResource(glpWave2);
	if (ghresWave2)	FreeResource(ghresWave2);

	if (glpWave3)	UnlockResource(glpWave3);
	if (ghresWave3)	FreeResource(ghresWave3);

	SaveRegistory(hwnd);

	Shell_NotifyIcon(NIM_DELETE, &gnid);

	PostQuitMessage(0);
}

/////////////////////////////////////////////////////////////////////////////
// CLOCK処理

void OnClock(HWND hwnd)
{
	if (gfClock == FALSE) {		// 時計は表示されていない
		if (!guIDTimer) {	// タイマも使われていない
			gfClock = TRUE;
			gfRed = FALSE;
			guIDTimer = SetTimer(hwnd, ID_TIMER, 10, NULL);
			InvalidateRect(hwnd, NULL, FALSE);
		}
	} else {
		gfClock = FALSE;
		KillTimer(hwnd, ID_TIMER);
		InvalidateRect(hwnd, NULL, FALSE);
		guIDTimer = 0;
	}
}

void OnPowerBroadcast(HWND hwnd, DWORD wParam, DWORD lParam)
{
	SYSTEM_POWER_STATUS sps;

	if (gfPowSys) {
		if (wParam == PBT_APMPOWERSTATUSCHANGE) {
			GetSystemPowerStatus(&sps);
			if (sps.ACLineStatus != gsps.ACLineStatus) {	// AC電源の状態が変わった
				if (sps.ACLineStatus == 0) {	// AC切断(カウントダウンを開始)
					if (guIDTimer == 0) {	// タイマが未設定
						OnActionStart(hwnd);
					}
				} else {			// AC接続(カウントダウン中止)
					if (guIDTimer != 0 && gfClock == 0) {
						OnActionStart(hwnd);
					}
				}
				gsps = sps;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// WM_RBUTTONDOWN処理

void OnRButtonDown(HWND hwnd, UINT fwKeys, WORD xPos, WORD yPos)
{
	POINT pt;
	GetCursorPos(&pt);
	HMENU hmenu, hmenuSub;
	hmenu = LoadMenu((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDR_RMENU));
	hmenuSub = GetSubMenu(hmenu, 0);

	if (gfClock == 0 && guIDTimer != 0) {
		EnableMenuItem(hmenuSub, IDM_ACTION_PROPERTY, MF_BYCOMMAND|MFS_GRAYED);
	}

	if (gfTopMost) {	// 常に手前に表示している？
		CheckMenuItem(hmenuSub, IDM_ACTION_TOPMOST, MF_BYCOMMAND | MF_CHECKED);
	} else {
		CheckMenuItem(hmenuSub, IDM_ACTION_TOPMOST, MF_BYCOMMAND | MF_UNCHECKED);
	}

	TrackPopupMenu(hmenuSub, TPM_LEFTALIGN | TPM_TOPALIGN, pt.x, pt.y, 0, hwnd, NULL);
	DestroyMenu(hmenu);
}

/////////////////////////////////////////////////////////////////////////////
// WM_LBUTTONDOWN処理

void OnLButtonDown(HWND hwnd, UINT fwKeys, WORD xPos, WORD yPos)
{
	SetCapture(hwnd);
	GetCursorPos(&gpt);
}

/////////////////////////////////////////////////////////////////////////////
// WM_LBUTTONUP処理

void OnLButtonUp(HWND hwnd, UINT fwKeys, WORD xPos, WORD yPos)
{
	ReleaseCapture();
}

/////////////////////////////////////////////////////////////////////////////
// WM_MOUSEMOVE処理

void OnMouseMove(HWND hwnd, UINT fwKeys, WORD xPos, WORD yPos)
{
	XRect rc;
	POINT pt;

	if (GetCapture() != hwnd)		// マウスをキャプチャしていない？
		return;				// 終了

	GetWindowRect(hwnd, &rc);		// 現在のウィンドウ位置(グローバル)
	GetCursorPos(&pt);			// 現在のカーソル位置(グローバル)

	// ウィンドウ位置を変化分だけずらして
	// 移動する
	rc.OffsetRect(pt.x - gpt.x, pt.y - gpt.y);
	SetWindowPos(hwnd, NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	gpt = pt;				// 座標を保存
}

/////////////////////////////////////////////////////////////////////////////
// ExitWindowsExをプラットフォーム別に実行
// Windowsを終了させる手順がNTと95/98では、違うためこの関数で分岐させる

BOOL ExitWindowsExPlatform(UINT uFlags)
{
	// OSのバージョンを取得
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	if (!GetVersionEx(&osvi)) {
		char szBuffer[128];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
			GetLastError(),		// ここがエラーの値
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&szBuffer, sizeof(szBuffer), NULL);
		MessageBox(NULL, szBuffer, "GetVersionEx error.", MB_OK);
		return FALSE;
	}

	// Windows NT ?
	if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) {
		HANDLE hToken;
		TOKEN_PRIVILEGES tkp;  // Get a token for this process.  

		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
		    MessageBox(NULL, "OpenProcessToken error.", NULL, MB_OK);  // Get the LUID for the shutdown privilege.  

		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);  
		tkp.PrivilegeCount = 1;  // one privilege to set    
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;  

		// Get the shutdown privilege for this process.  
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);  

		// Cannot test the return value of AdjustTokenPrivileges.  
		if (GetLastError() != ERROR_SUCCESS)     
			MessageBox(NULL, "AdjustTokenPrivileges error.", NULL, MB_OK);  
	}

	// Shut down the system and force all applications to close.  
	if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0))     
		MessageBox(NULL, "ExitWindowsEx error.", NULL, MB_OK); 

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// WMU_TIMEUP処理

void OnUTimeUp(HWND hwnd)
{
	EnableWindow(ghwndButton[0], TRUE);
	SetWindowText(ghwndButton[1], "START");
	SetForegroundWindow(hwnd);
	MessageBeep(MB_OK);
	lstrcpy(gszSerifu, "エヴァ初号機、活動を停止");
	if (!gfEndWindows) {
		if (gfEndTime)		// 終了時分から活動限界を求める？
			gdwSave = gdwSave2;
		MessageBox(hwnd, "カウントダウン終了", "警告", MB_OK);

		PostMessage(ghwndFrom, WMU_FROMCONTROL, 5, (LPARAM)gszSerifu);
		PlaySound((LPCSTR)glpWave3, NULL, SND_ASYNC | SND_MEMORY | SND_NODEFAULT);
		gfPlay3 = TRUE;
	} else {
		if (gfEndTime)		// 終了時分から活動限界を求める？
			gdwSave = gdwSave2;
		PostMessage(ghwndFrom, WMU_FROMCONTROL, 5, (LPARAM)gszSerifu);
		PlaySound((LPCSTR)glpWave3, NULL, SND_ASYNC | SND_MEMORY | SND_NODEFAULT);
		gfPlay3 = TRUE;

		SendMessage(ghwndFrom, WMU_FROMCONTROL, 5, (LPARAM)gszSerifu);
		ExitWindowsExPlatform(EWX_SHUTDOWN);
	}
}

/////////////////////////////////////////////////////////////////////////////
// WM_TIMER処理

void OnTimer(HWND hwnd)
{
	XRect rc(18, 19, 258, 69);

	if (gfClock != FALSE) {
		InvalidateRect(hwnd, &rc, FALSE);
		return;
	}

	DWORD dwMS = gdwSave - (GetTickCount() - gdwStart);
	if ((INT)dwMS <= 0) {
		KillTimer(hwnd, guIDTimer);
		guIDTimer = 0;
		PostMessage(hwnd, WMU_TIMEUP, 0, 0);
	}

	if (gfPlay == FALSE && (HMSMS(0, 4, 53, 0) <= dwMS && dwMS <= HMSMS(0, 4, 53, 999))) {
		lstrcpy(gszSerifu, "活動限界まであと、4分53秒!!");
		PostMessage(ghwndFrom, WMU_FROMCONTROL, 5, (LPARAM)gszSerifu);
		PlaySound((LPCSTR)glpWave, NULL, SND_ASYNC | SND_MEMORY | SND_NODEFAULT);
		gfPlay = TRUE;
	}

	if (gfPlay2 == FALSE && (HMSMS(0, 3, 0, 0) <= dwMS && dwMS <= HMSMS(0, 3, 0, 999))) {
		lstrcpy(gszSerifu, "初号機、活動限界まであと、3分");
		PostMessage(ghwndFrom, WMU_FROMCONTROL, 5, (LPARAM)gszSerifu);
		PlaySound((LPCSTR)glpWave2, NULL, SND_ASYNC | SND_MEMORY | SND_NODEFAULT);
		gfPlay2 = TRUE;
	}

	if (gfRed == FALSE && dwMS < HMSMS(0, 3, 0, 0)) {
		gfRed = TRUE;
		InvalidateRect(hwnd, NULL, FALSE);
	} else {
		InvalidateRect(hwnd, &rc, FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// WM_COMMAND::IDM_ACTION_PROPERTY処理

void OnActionProperty(HWND hwnd)
{
	if (IsWindowEnabled(ghwndButton[0])) {
		DialogBox((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			MAKEINTRESOURCE(IDD_PROPERTY), hwnd, (DLGPROC)PropertyDialogProc);
	}
}

/////////////////////////////////////////////////////////////////////////////
// WM_COMMAND::IDM_ACTION_START処理

void OnActionStart(HWND hwnd)
{
	if (gfClock) {				// 時刻表示中？
		KillTimer(hwnd, ID_TIMER);	// タイマを破棄する
		guIDTimer = 0;			// タイマIDの初期化
		gfClock = FALSE;		// 時計表示中フラグOFF
	}

	if (guIDTimer) {			// タイマ使用中？
		gdwSave = gdwSave - (GetTickCount() - gdwStart);
		if (gfEndTime)		// 終了時分から活動限界を求める？
			gdwSave = gdwSave2;

		EnableWindow(ghwndButton[0], TRUE);
		SetWindowText(ghwndButton[1], "START");
		KillTimer(hwnd, ID_TIMER);
		guIDTimer = 0;
		InvalidateRect(hwnd, NULL, FALSE);

		if (gfClockStart)
			OnClock(hwnd);
	} else {
		if (gfEndTime) {		// 終了時分から活動限界を求める？
			SYSTEMTIME si;
			GetLocalTime(&si);
			DWORD dwNow = HMSMS(si.wHour, si.wMinute, si.wSecond, si.wMilliseconds);
			gdwSave2 = gdwSave;
			gdwSave = gdwSave - dwNow;
			if ((INT)gdwSave <= 0) {
				gdwSave = 0;
				return;
			}
		}

		EnableWindow(ghwndButton[0], FALSE);
		SetWindowText(ghwndButton[1], "STOP");
		guIDTimer = SetTimer(hwnd, ID_TIMER, 10, NULL);
		gdwStart = GetTickCount();
		gfRed = FALSE;
		gfPlay = FALSE;
		gfPlay2 = FALSE;
		gfPlay3 = FALSE;
		InvalidateRect(hwnd, NULL, FALSE);

		lstrcpy(gszSerifu, "アンビリカルケーブル断線!!");
		PostMessage(ghwndFrom, WMU_FROMCONTROL, 5, (LPARAM)gszSerifu);
	}
}

/////////////////////////////////////////////////////////////////////////////

void OnActionTopmost(HWND hwnd)
{
	XRect rc;
	GetWindowRect(hwnd, &rc);
	if (gfTopMost) {	// 常に手前に表示している？
		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		gfTopMost = FALSE;
	} else {
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		gfTopMost = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// WM_PAINT処理

void OnPaint(HWND hwnd)
{
	INT ofsX;
	INT ofsY;
	HDC hdcMem;
	DWORD dwMS;
	DWORD dw;
	PAINTSTRUCT ps;

	if (gfClock != FALSE) {
		SYSTEMTIME si;
		GetLocalTime(&si);
		dwMS = HMSMS(si.wHour, si.wMinute, si.wSecond, si.wMilliseconds);
	} else {
		dwMS = gdwSave - (GetTickCount() - gdwStart);
		if ((INT)dwMS < 0)
			dwMS = 0;
	}

	if (guIDTimer == 0)
		dwMS = 0;

	BeginPaint(hwnd, &ps);

	hdcMem = CreateCompatibleDC(ps.hdc);

	SelectObject(hdcMem, ghbmBkGround);
	ofsY = (gfRed) ? 72 : 0;

	BitBlt(ps.hdc,   0,  0,  18, 72, hdcMem,   0, ofsY, SRCCOPY);	// 左ブロック
	BitBlt(ps.hdc, 292,  0,  18, 72, hdcMem, 292, ofsY, SRCCOPY);	// 右ブロック
	BitBlt(ps.hdc,  18,  0, 274, 14, hdcMem,  18, ofsY, SRCCOPY);	// 上ブロック1
	BitBlt(ps.hdc, 211, 14,  50, 17, hdcMem, 211, 14 + ofsY, SRCCOPY);	// 上ブロック2
	BitBlt(ps.hdc,  18, 70, 274,  2, hdcMem,  18, 70 + ofsY, SRCCOPY);	// 下ブロック

	BitBlt(ps.hdc,  74, 19,  12, 51, hdcMem,  74, 19 + ofsY, SRCCOPY);
	BitBlt(ps.hdc, 142, 19,  12, 51, hdcMem, 142, 19 + ofsY, SRCCOPY);
	BitBlt(ps.hdc, 210, 33,   7, 37, hdcMem, 210, 33 + ofsY, SRCCOPY);

	SelectObject(hdcMem, ghbm7SegBig);
	ofsY = (gfRed) ? 51 : 0;

	dw = GetHour(dwMS);
	ofsX = 28 * ((dw / 10) % 10);		// 時の2桁目
	BitBlt(ps.hdc, 18, 19, 28, 51, hdcMem, ofsX, ofsY, SRCCOPY);
	ofsX = 28 * (dw % 10);
	BitBlt(ps.hdc, 46, 19, 28, 51, hdcMem, ofsX, ofsY, SRCCOPY);

	dw = GetMin(dwMS);
	ofsX = 28 * ((dw / 10) % 10);		// 分の2桁目
	BitBlt(ps.hdc,  86, 19, 28, 51, hdcMem, ofsX, ofsY, SRCCOPY);
	ofsX = 28 * (dw % 10);		// 分の1桁目
	BitBlt(ps.hdc, 114, 19, 28, 51, hdcMem, ofsX, ofsY, SRCCOPY);

	dw = GetSec(dwMS);
	ofsX = 28 * ((dw / 10) % 10);		// 分の10の位
	BitBlt(ps.hdc, 154, 19, 28, 51, hdcMem, ofsX, ofsY, SRCCOPY);
	ofsX = 28 * ((dw / 1) % 10);		// 分の1の位
	BitBlt(ps.hdc, 182, 19, 28, 51, hdcMem, ofsX, ofsY, SRCCOPY);

	SelectObject(hdcMem, ghbm7SegSmall);
	ofsY = (gfRed) ? 37 : 0;

	dw = GetMSec(dwMS);
	ofsX = 21 * ((dw / 100) % 10);		// 分の100の位
	BitBlt(ps.hdc, 217, 33, 21, 37, hdcMem, ofsX, ofsY, SRCCOPY);
	ofsX = 21 * ((dw / 10) % 10);		// 分の10の位
	BitBlt(ps.hdc, 238, 33, 21, 37, hdcMem, ofsX, ofsY, SRCCOPY);

	DeleteDC(hdcMem);

	EndPaint(hwnd, &ps);
}

/////////////////////////////////////////////////////////////////////////////
// PropDialog WM_INITDIALOG処理

void PROP_OnInitDialog(HWND hwnd)
{
	TCHAR szBuf[64];
	wsprintf(szBuf, "%d:%d:%d:%d",
		GetHour(gdwSave), GetMin(gdwSave), GetSec(gdwSave), GetMSec(gdwSave));
	SetDlgItemText(hwnd, IDC_EDIT1, szBuf);

	Button_SetCheck(GetDlgItem(hwnd, IDC_CHECK2), (gfEndTime == FALSE) ? 0 : 1);
	Button_SetCheck(GetDlgItem(hwnd, IDC_CHECK1), (gfEndWindows == FALSE) ? 0 : 1);
	Button_SetCheck(GetDlgItem(hwnd, IDC_CHECK3), (gfClockStart == FALSE) ? 0 : 1);
}

/////////////////////////////////////////////////////////////////////////////
// PropDialog WM_COMMAND::IDOK処理

void PROP_OnOk(HWND hwnd)
{
	INT h = 0, m = 0, s = 0, ms = 0;
	TCHAR szBuf[64];
	GetDlgItemText(hwnd, IDC_EDIT1, szBuf, sizeof(szBuf));
	LPSTR pt = strtok(szBuf, ": ./");
	if (pt)	{
		h = atoi(pt);
		pt = strtok(NULL, ": ./");
		if (pt)	{
			m = atoi(pt);
			pt = strtok(NULL, ": /.");
			if (pt)	{
				s = atoi(pt);
				pt = strtok(NULL, ": ./");
				if (pt)	{
					ms = atoi(pt);
				}
			}
		}
	}

	gdwSave = HMSMS(h, m, s, ms);

	LRESULT lr;

	lr = Button_GetCheck(GetDlgItem(hwnd, IDC_CHECK2));
	gfEndTime = (lr == 0) ? FALSE : TRUE;

	lr = Button_GetCheck(GetDlgItem(hwnd, IDC_CHECK1));
	gfEndWindows = (lr == 0) ? FALSE : TRUE;

	lr = Button_GetCheck(GetDlgItem(hwnd, IDC_CHECK3));
	gfClockStart = (lr == 0) ? FALSE : TRUE;
}
