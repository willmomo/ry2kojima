#pragma once


#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "resource.h"


// MainWnd.cpp

BOOL MainWnd_CreateWindow(HINSTANCE hInstance);
BOOL MainWnd_RegisterClass(HINSTANCE hInstance);

// AboutDlg.cpp

int AboutDlg_DialogBox(HINSTANCE hInstance, HWND hwndParent);

// cpuusage.cpp

BOOL StartPerf();
INT GetCPUUsage();
BOOL StopPerf();
