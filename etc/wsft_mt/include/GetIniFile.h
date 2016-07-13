#pragma once


//---------------------------------------------------------------------
//	GetIniFile���ʃw�b�_(VB�p��GetIniFile.bas)
//
//	[�T�v]
//	GetIniFile.dll���g�p����l�́A������C���N���[�h


#include <windows.h>


// Embedded ���l�������A���b�p�[
DWORD APIENTRY GIFGetString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize, LPCTSTR lpFileName);
UINT APIENTRY GIFGetInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nDefault, LPCTSTR lpFileName);
BOOL APIENTRY GIFWriteString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString, LPCTSTR lpFileName);

void APIENTRY GIFGetIniPath(char* path, int cbSize);
void APIENTRY GIFSetIniPath(const char* path);
void APIENTRY GIFSetIniPathByExe();
void APIENTRY GIFSetIniPathByPC(const char* base_path);

// WinSousa.ini �p
BOOL APIENTRY GetWinSousaStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpReturnedString, DWORD nSize);
BOOL APIENTRY GetWinSousaInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long* plData);
BOOL APIENTRY WriteWinSousaStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpString);
BOOL APIENTRY WriteWinSousaInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long lData);

// WssCom.ini �p
BOOL APIENTRY GetWssComStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpReturnedString, DWORD nSize);
BOOL APIENTRY GetWssComInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long* plData);
BOOL APIENTRY GetWssComInt2(LPCTSTR lpAppName, LPCTSTR lpKeyName,long* plData, BOOL* fUseDef);
BOOL APIENTRY WriteWssComStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpString);
BOOL APIENTRY WriteWssComInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long lData);

// WssAppCtrl.ini �p
BOOL APIENTRY GetWssAppCtrlStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpReturnedString, DWORD nSize);
BOOL APIENTRY GetWssAppCtrlInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long* plData);
BOOL APIENTRY WriteWssAppCtrlStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpString);
BOOL APIENTRY WriteWssAppCtrlInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long lData);

// HallInfo.ini �p
BOOL APIENTRY GetHallInfoStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpReturnedString, DWORD nSize);
BOOL APIENTRY GetHallInfoInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long* plData);
BOOL APIENTRY WriteHallInfoStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpString);
BOOL APIENTRY WriteHallInfoInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long lData);

// tnio.ini �p
BOOL APIENTRY GetTnioStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpReturnedString, DWORD nSize);
BOOL APIENTRY GetTnioInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long* plData);
BOOL APIENTRY WriteTnioStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpString);
BOOL APIENTRY WriteTnioInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long lData);

// version.ini �p
BOOL APIENTRY GetVersionStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpReturnedString, DWORD nSize);
BOOL APIENTRY GetVersionInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long* plData);
BOOL APIENTRY WriteVersionStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpString);
BOOL APIENTRY WriteVersionInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long lData);

// active.ini �p
BOOL APIENTRY GetActiveStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpReturnedString, DWORD nSize);
BOOL APIENTRY GetActiveInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long* plData);
BOOL APIENTRY WriteActiveStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpString);
BOOL APIENTRY WriteActiveInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long lData);

// mpcproc.ini �p
BOOL APIENTRY GetMpcProcStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpReturnedString, DWORD nSize);
BOOL APIENTRY GetMpcProcInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long* plData);
BOOL APIENTRY WriteMpcProcStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpString);
BOOL APIENTRY WriteMpcProcInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long lData);

// GtageCSV.ini �p
BOOL APIENTRY GetGtageCSVStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpReturnedString, DWORD nSize);
BOOL APIENTRY GetGtageCSVInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long* plData);
BOOL APIENTRY WriteGtageCSVStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpString);
BOOL APIENTRY WriteGtageCSVInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long lData);

// ITA.ini �p
BOOL APIENTRY GetITAStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpReturnedString, DWORD nSize);
BOOL APIENTRY GetITAInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long* plData);
BOOL APIENTRY WriteITAStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpString);
BOOL APIENTRY WriteITAInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long lData);

// SquareComm.ini �p
BOOL APIENTRY GetSquareCommStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpReturnedString, DWORD nSize);
BOOL APIENTRY GetSquareCommInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long* plData);
BOOL APIENTRY WriteSquareCommStr(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpString);
BOOL APIENTRY WriteSquareCommInt(LPCTSTR lpAppName,	LPCTSTR lpKeyName,long lData);

// ���̑�
BOOL WINAPI CalcTimeInf_EigyoDayChangeTime(DWORD days, LPCTSTR lpDate, long *ans);

