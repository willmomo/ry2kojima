
#pragma once

#if !defined(__PROCTOOLS_H)
#define __PROCTOOLS_H

#include <windows.h>

HANDLE ProcessIdToProcessHandle(DWORD processId);						// プロセスID -> プロセスハンドル
int ProcessIdToWindowHandle(DWORD processId, HWND* buffer, int count);	// プロセスID -> ウィンドウハンドル

DWORD WindowHandleToProcessId(HWND hwnd);								// ウィンドウハンドル -> プロセスID
HANDLE WindowHandleToProcessHandle(HWND hwnd);							// ウィンドウハンドル -> プロセスハンドル

DWORD ProcessHandleToProcessId(HANDLE hproc);							// プロセスハンドル -> プロセスID
int ProcessHandleToWindowHandle(HANDLE hproc, HWND* buffer, int count);	// プロセスハンドル -> ウィンドウハンドル

#endif
