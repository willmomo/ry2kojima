#pragma once


#include <windows.h>
#include <tchar.h>

#include "kjmsub.h"

#include "resource.h"

#define CLASSNAME_MAINFRM				_T("MainFrm_SystemStatus")
#define CLASSNAME_CPUUSAGE				_T("CpuUsage_SystemStatus")
#define CLASSNAME_SYSTEMPOWERSTATUS		_T("SystemPowerStatus_SystemStatus")


void MainFrm_MovePosition();

BOOL CpuUsage_RegistClass(HINSTANCE hInstance);
BOOL CpuUsage_CreateWindow(HINSTANCE hInstance, HWND hwndParent);

BOOL SystemPowerStatus_CreateWindow(HINSTANCE hInstance, HWND hwndParent);
BOOL SystemPowerStatusWnd_PostMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL SystemPowerStatus_RegistClass(HINSTANCE hInstance);
