/////////////////////////////////////////////////////////////////////////////
// すべてのファイルにインクルードするデフォルトインクルードファイル

#ifndef __DEFINC_H
#define __DEFINC_H

/////////////////////////////////////////////////////////////////////////////
// 定数

#define MAIN_WINDOW_WIDTH	(310 + 2)
#define MAIN_WINDOW_HEIGHT	(72 + 2)

#define	WMU_TIMEUP	(WM_USER+0)
#define WMU_FROMCONTROL	(WM_USER+1)
#define WMU_NOTIFYICON	(WM_USER+2)

extern char gszAppName[64];	// アプリケーション名
extern DWORD gdwLeft;
extern DWORD gdwTop;
extern BOOL gfTopMost;
extern LPCTSTR glpSerifu;

/////////////////////////////////////////////////////////////////////////////
// プロトタイプ宣言

// Main.cpp
BOOL CALLBACK PropertyDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

// MsgProc.cpp
void OnActionTopmost(HWND hwnd);
void OnPaint(HWND hwnd);
void OnCreate(HWND hwnd);
void OnDestroy(HWND hwnd);
void OnTimer(HWND hwnd);
void OnDrawItem(HWND hwnd, UINT idCtl, LPDRAWITEMSTRUCT lpdis);
void START_OnBnClicked(HWND hwnd, HWND hwndCtl);
void SETTEI_OnBnClicked(HWND hwnd, HWND hwndCtl);
void OnUTimeUp(HWND hwnd);
void OnLButtonDown(HWND hwnd, UINT fwKeys, WORD xPos, WORD yPos);
void OnLButtonUp(HWND hwnd, UINT fwKeys, WORD xPos, WORD yPos);
void OnMouseMove(HWND hwnd, UINT fwKeys, WORD xPos, WORD yPos);
void OnRButtonDown(HWND hwnd, UINT fwKeys, WORD xPos, WORD yPos);
void OnClock(HWND hwnd);
void PROP_OnInitDialog(HWND hwnd);
void PROP_OnOk(HWND hwnd);
void GetRegistory();
void OnPowerBroadcast(HWND hwnd, DWORD wParam, DWORD lParam);
void OnUNotifyIcon(HWND hwnd, WPARAM wParam, LPARAM lParam);
void OnActionStart(HWND hwnd);
void OnActionProperty(HWND hwnd);

// FromProc.cpp
void FROM_OnCreate(HWND hwnd);
void FROM_OnDestroy(HWND hwnd);
void FROM_OnUFromControl(HWND hwnd, DWORD dw1, DWORD dw2);
void FROM_OnTimer(HWND hwnd);
void FROM_OnPaint(HWND hwnd);

// FukiProc.cpp
void FUKI_OnPaint(HWND hwnd);
void FUKI_OnDestroy(HWND hwnd);
void FUKI_OnCreate(HWND hwnd);

#endif
