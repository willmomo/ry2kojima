//-----------------------------------------------------------------------------
// ���C���t���[���̂��߂̃\�[�X�R�[�h
//
// 2012/03/14 r.kojima
//-----------------------------------------------------------------------------
#include "mainFrm.h"
#include "kjmlib.h"

#include <process.h>
#include "resource.h"

#define MAINFRM_CLASSNAME	_T("TasktraySampleMainFrm")

#define ID_TASKTRAYICON	1

#define MYWM_TASKTRAY	(WM_APP + 0)	// �^�X�N�g���C����̃��b�Z�[�W

static kjm::taskTrayIcon s_taskTrayIcon;
static HANDLE s_hThreadWorker = NULL;
static bool s_loop = true;

//-----------------------------------------------------------------------------
// ���[�J�[�X���b�h
//-----------------------------------------------------------------------------
static unsigned __stdcall worker(void* parg) {
	while (s_loop) {
		Sleep(100);
	}
	return 0;
}

//-----------------------------------------------------------------------------
// WM_CREATE ���b�Z�[�W
//-----------------------------------------------------------------------------
static LRESULT OnCreate(HWND hwnd) {
	// �^�X�N�g���C�ɃA�C�R����ǉ�
	s_taskTrayIcon.addIcon(hwnd, ID_TASKTRAYICON, 
		(HICON)LoadImage(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDI_MAINFRM), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR),
		MYWM_TASKTRAY, _T("Task Tray Sample"));

	// ���[�J�[�X���b�h�N��
	s_loop = true;
	s_hThreadWorker = (HANDLE)_beginthreadex(NULL, 0, worker, NULL, 0, NULL);

	return 0;
}

//-----------------------------------------------------------------------------
// WM_DESTROY ���b�Z�[�W
//-----------------------------------------------------------------------------
static LRESULT OnDestroy(HWND hwnd) {
	// ���[�J�[�X���b�h�I��
	if (s_hThreadWorker != NULL) {
		s_loop = false;
		WaitForSingleObject(s_hThreadWorker, INFINITE);
	}

	// �^�X�N�g���C����A�C�R�����폜
	s_taskTrayIcon.deleteIcon();

	PostQuitMessage(0);
	return 0;
}

//-----------------------------------------------------------------------------
// [�I��]���j���[����
//-----------------------------------------------------------------------------
static LRESULT OnPopupExit(HWND hwnd) {
	DestroyWindow(hwnd);
	return 0;
}

//-----------------------------------------------------------------------------
// WM_COMMAND ���b�Z�[�W
//-----------------------------------------------------------------------------
static LRESULT OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	WORD wID = LOWORD(wParam);

	if (wID == IDM_POPUP_EXIT) {
		return OnPopupExit(hwnd);
	}

	return DefWindowProc(hwnd, WM_COMMAND, wParam, lParam);
}

//-----------------------------------------------------------------------------
// MYWM_TASKTRAY ���b�Z�[�W
//-----------------------------------------------------------------------------
static LRESULT OnMyTaskTray(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	if (lParam == WM_RBUTTONUP) {
		HMENU hmenu = LoadMenu(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDR_POPUP_MENU));
		if (hmenu) {
			HMENU hmenuSub = GetSubMenu(hmenu, 0);
			if (hmenuSub) {
				POINT point;
				GetCursorPos(&point);

				SetForegroundWindow(hwnd);
				TrackPopupMenu(hmenuSub, TPM_LEFTBUTTON | TPM_LEFTALIGN, point.x, point.y, 0, hwnd, NULL);
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
		case WM_CREATE:
			return OnCreate(hwnd);

		case WM_DESTROY:
			return OnDestroy(hwnd);

		case WM_COMMAND:
			return OnCommand(hwnd, wParam, lParam);

		case MYWM_TASKTRAY:
			return OnMyTaskTray(hwnd, wParam, lParam);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// �E�B���h�E�̍쐬
//-----------------------------------------------------------------------------
BOOL MainFrm_CreateWindow(HINSTANCE hInstance, int nCmdShow) {
	HWND hwnd = CreateWindow(
		MAINFRM_CLASSNAME, _T("Task Tray Sample"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	if (hwnd != NULL) {
		/// ShowWindow(hwnd, nCmdShow);
	}

	return (hwnd != NULL) ? TRUE : FALSE;
}

//-----------------------------------------------------------------------------
// �E�B���h�E�N���X�̍쐬
//-----------------------------------------------------------------------------
BOOL MainFrm_RegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX wc = {sizeof(wc)};
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_MAINFRM), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	wc.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_MAINFRM), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = MAINFRM_CLASSNAME;

	return RegisterClassEx(&wc);
}
