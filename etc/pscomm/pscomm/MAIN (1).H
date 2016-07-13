/////////////////////////////////////////////////////////////////////////////
//	基本ヘッダ
//	・すべてのソースファイルにインクルードする

#ifndef __MAIN_H
#define __MAIN_H

#include <windows.h>
#include "prestar.h"
#include "resource.h"


enum WindowMessage {
	WM_PRESTAR = WM_USER,
	WM_CLOSESTATUSDLG
};

enum IDButton {
	IDB_CANCEL = 1000,
	IDB_DISPLOG
};

#define DEFWNDWSIZE	640
#define DEFWNDHSIZE	288
#define LOGWNDHSIZE	312

/////////////////////////////////////////////////////////////////////////////
//	プロトタイプ
//	・プロトタイプが必要なときは、ここに書く

BOOL CALLBACK StatusDlgProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL ReadIniDCB(DCB *dcb);
BOOL ReadIniCom(char *Comm);
BOOL WriteIniDCB(DCB *dcb);
BOOL WriteIniCom(char *Comm);
void WriteProfileInt(LPSTR szSection, LPSTR szKey, int i);

#endif
