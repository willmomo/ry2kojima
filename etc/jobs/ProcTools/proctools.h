
#pragma once

#if !defined(__PROCTOOLS_H)
#define __PROCTOOLS_H

#include <windows.h>

HANDLE ProcessIdToProcessHandle(DWORD processId);						// �v���Z�XID -> �v���Z�X�n���h��
int ProcessIdToWindowHandle(DWORD processId, HWND* buffer, int count);	// �v���Z�XID -> �E�B���h�E�n���h��

DWORD WindowHandleToProcessId(HWND hwnd);								// �E�B���h�E�n���h�� -> �v���Z�XID
HANDLE WindowHandleToProcessHandle(HWND hwnd);							// �E�B���h�E�n���h�� -> �v���Z�X�n���h��

DWORD ProcessHandleToProcessId(HANDLE hproc);							// �v���Z�X�n���h�� -> �v���Z�XID
int ProcessHandleToWindowHandle(HANDLE hproc, HWND* buffer, int count);	// �v���Z�X�n���h�� -> �E�B���h�E�n���h��

#endif
