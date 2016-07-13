//-----------------------------------------------------------------------------
// ���C���t���[��
//-----------------------------------------------------------------------------
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "./kjmlib-20120604/kjmlib.h"

#include "mainFrm.h"
#include "worker.h"
#include <process.h>
#include "resource.h"

#define CLASS_MAINFRM	_T("SAMPLETASKTRAY_MAINFRM")

#define WM_MYNOTIFYICON	(WM_APP + 1)

static HANDLE s_hThread = NULL;

//-----------------------------------------------------------------------------
// SampleTaskTray �̃X���b�h���N������
//-----------------------------------------------------------------------------
static void start_SampleTaskTray_thread() {
	unsigned thrdid = 0;
	s_hThread = (HANDLE)_beginthreadex(NULL, 0, worker_thread, NULL, 0, &thrdid);
}

//-----------------------------------------------------------------------------
// sntp�̃X���b�h���I������
//-----------------------------------------------------------------------------
static void stop_SampleTaskTray_thread() {
	if (s_hThread) {
		worker_thread_end();
		WaitForSingleObject(s_hThread, 10 * 1000);

		CloseHandle(s_hThread);
		s_hThread = NULL;
	}
}

//-----------------------------------------------------------------------------
// WM_COMMAND ���b�Z�[�W����
//-----------------------------------------------------------------------------
static LRESULT OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	WORD wNotifyCode = HIWORD(wParam);
	WORD wID = LOWORD(wParam);
	HWND hwndCtl = (HWND)lParam;

	LRESULT nRet = 0;

	if (wID == IDM_POPUP_EXIT) {
		DestroyWindow(hwnd);
	} else {
		nRet = DefWindowProc(hwnd, WM_COMMAND, wParam, lParam);
	}

	return nRet;
}

//-----------------------------------------------------------------------------
// WM_CREATE ���b�Z�[�W����
//-----------------------------------------------------------------------------
static LRESULT OnCreate(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	// �^�X�N�g���C�A�C�R���̒ǉ�
	NOTIFYICONDATA nid = {sizeof(nid), hwnd, 1};
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.hIcon = (HICON)LoadImage(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDI_APPL), IMAGE_ICON, 16, 16, 0);
	nid.uCallbackMessage = WM_MYNOTIFYICON;
	strcpy(nid.szTip, "SampleTaskTray");
	Shell_NotifyIcon(NIM_ADD, &nid);

	// SampleTaskTray �X���b�h�����s
	start_SampleTaskTray_thread();

	return 0;
}

//-----------------------------------------------------------------------------
// WM_DESTROY ���b�Z�[�W����
//-----------------------------------------------------------------------------
static LRESULT OnDestroy(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	// �X���b�h�I��
	stop_SampleTaskTray_thread();

	// �^�X�N�g���C�A�C�R���̍폜
	NOTIFYICONDATA nid = {sizeof(nid), hwnd, 1};
	Shell_NotifyIcon(NIM_DELETE, &nid);

	PostQuitMessage(0);
	return 0;
}

//-----------------------------------------------------------------------------
// WM_MYNOTIFYICON ���b�Z�[�W����
//-----------------------------------------------------------------------------
static LRESULT OnMyNotifyIcon(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	UINT uID = (UINT)wParam;
	UINT uMouseMsg = (UINT)lParam;

	if (uMouseMsg == WM_RBUTTONUP) {
		HMENU hMenu = LoadMenu(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDR_POPUPMENU));
		if (hMenu) {
			HMENU hMenuPopup = GetSubMenu(hMenu, 0);
			if (hMenuPopup) {
				POINT pt;
				GetCursorPos(&pt);
				SetForegroundWindow(hwnd);
				TrackPopupMenu(hMenuPopup, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
				PostMessage(hwnd, WM_NULL, 0, 0);
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// �E�B���h�E�v���V�[�W��
//-----------------------------------------------------------------------------
static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
		return OnCommand(hwnd, wParam, lParam);

	case WM_CREATE:
		return OnCreate(hwnd, wParam, lParam);

	case WM_DESTROY:
		return OnDestroy(hwnd, wParam, lParam);

	case WM_MYNOTIFYICON:
		return OnMyNotifyIcon(hwnd, wParam, lParam);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// ���C���t���[���̍쐬
//-----------------------------------------------------------------------------
BOOL MainFrm_CreateWindow(HINSTANCE hInstance, int nCmdShow) {
	HWND hwnd = CreateWindow(CLASS_MAINFRM, "SampleTaskTray", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		return FALSE;
	}

	/// ShowWindow(hwnd, nCmdShow);

	return TRUE;
}

//-----------------------------------------------------------------------------
// ���C���t���[���̓o�^
//-----------------------------------------------------------------------------
BOOL MainFrm_RegisterClass(HINSTANCE hInstance) {
	WNDCLASS wc;
	wc.style = 0;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPL));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpfnWndProc = WndProc;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASS_MAINFRM;

	return RegisterClass(&wc);
}
