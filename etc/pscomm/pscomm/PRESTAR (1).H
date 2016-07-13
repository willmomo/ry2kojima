#ifndef __PRESTAR_H
#define __PRESTAR_H

#include <stdio.h>
#include "tty.h"

#define ID_TIMER	1
#define ID_ANIME	2
#define STX		0x02
#define ETX		0x03

void XPrestar_XPrestar(HINSTANCE, HWND);
void XPrestar_deXPrestar();

void XPrestar_ChangeStatus();
LPCSTR XPrestar_GetModeText(int nMode);
LPCSTR XPrestar_GetStatText(int nMode, int nStat);
LPCSTR XPrestar_GetErrorText(char * psz, char * pszBuf);
int XPrestar_MakeCommand(char * buf, char * fnc, int p1 = 0, int p2 = 0, int p3 = 0);
int XPrestar_MakeCommand(char * buf, char * fnc, char * pszDataString);
BOOL XPrestar_SumCheck(char *buf, char flg = 0);
BOOL XPrestar_FunctionCheck(char *buf, char flg = 0);
BOOL XPrestar_StatusCheck(char *buf, char *cmp, char flg = 0, char dialog = 1);
BOOL XPrestar_ParamCheck(char *buf, char flg = 0);
BOOL XPrestar_FileGroupCheck(char *buf, char flg = 0);
int XPrestar_SplitShopNum(char *buf);
int XPrestar_SplitGroupNum(char *buf);
int XPrestar_SplitFileNum(char *buf);
BOOL XPrestar_WriteData(char *buf);
void XPrestar_WriteLog(const char * buf, int n);
int XPrestar_WriteComm2(const char * buf, int n);
void XPrestar_Log();

void XPrestar_InitStatus();
void XPrestar_Grp00Action();
void XPrestar_Grp01Action();
void XPrestar_Grp02Action();
void XPrestar_RcvAction();

void XPrestar_OnIdle();
void XPrestar_OnGrp00();
void XPrestar_OnGrp01();
void XPrestar_OnGrp02();
void XPrestar_OnTimer(WPARAM);
void XPrestar_OnPrestar(WPARAM);
void XPrestar_OnCommNotify(WPARAM, LPARAM);
void XPrestar_OnPaint();

void XPrestar_GetGroupName(char *str);
void XPrestar_GetFileName(char *str);

HBITMAP XPrestar_LoadBitmap(char *name);
void XPrestar_DrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, short xStart,
                           short yStart, COLORREF cTransparentColor, int xx, int yy);
#endif
