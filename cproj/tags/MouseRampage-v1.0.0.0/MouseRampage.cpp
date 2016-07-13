// MouseRampage.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "MouseRampage.h"

#define MAX_LOADSTRING 100

// �O���[�o���ϐ�:
HINSTANCE hInst;								// ���݂̃C���^�[�t�F�C�X
TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �����ɃR�[�h��}�����Ă��������B
	MSG msg;
	HACCEL hAccelTable;

	// �O���[�o������������������Ă��܂��B
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MOUSERAMPAGE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// �A�v���P�[�V�����̏����������s���܂�:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MOUSERAMPAGE));

	// ���C�� ���b�Z�[�W ���[�v:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
//  �R�����g:
//
//    ���̊֐�����юg�����́A'RegisterClassEx' �֐����ǉ����ꂽ
//    Windows 95 ���O�� Win32 �V�X�e���ƌ݊�������ꍇ�ɂ̂ݕK�v�ł��B
//    �A�v���P�[�V�������A�֘A�t����ꂽ
//    �������`���̏������A�C�R�����擾�ł���悤�ɂ���ɂ́A
//    ���̊֐����Ăяo���Ă��������B
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MOUSERAMPAGE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MOUSERAMPAGE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:  ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- ���~���b�Z�[�W��\�����Ė߂�
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool s_moving = false;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		RegisterHotKey(hWnd, 1, MOD_ALT|MOD_CONTROL, 'M');
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �I�����ꂽ���j���[�̉��:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �`��R�[�h�������ɒǉ����Ă�������...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		UnregisterHotKey(hWnd, 1);
		PostQuitMessage(0);
		break;
	case WM_HOTKEY:
		if (s_moving == false) {
			s_moving = true;

			TCHAR fname[MAX_PATH];
			GetModuleFileName(NULL, fname, _countof(fname));
			PathRenameExtension(fname, _T(".ini"));
			DWORD timer = GetPrivateProfileInt(_T("option"), _T("timer"), 20, fname);
			//s_speed = GetPrivateProfileInt(_T("option"), _T("speed"), 2, fname);

			SetTimer(hWnd, 1, timer, NULL);
		} else {
			s_moving = false;
			KillTimer(hWnd, 1);
		}
		break;
	case WM_TIMER:
		{
			std::vector<INPUT> inputs;

			POINT pt;
			GetCursorPos(&pt);
			int x2 = rand() % GetSystemMetrics(SM_CXSCREEN);
			int y2 = rand() % GetSystemMetrics(SM_CYSCREEN);

			int dx, dy;
			if (abs(x2 - pt.x) > abs(y2 - pt.y)) {
				// x������Ɉړ�
				dx = (x2 > pt.x) ? 1 : -1;
				dy = (y2 > pt.y) ? 1 : -1;

				for (int xx = pt.x; xx != x2; xx += dx) {
					int yy = abs(y2 - pt.y) * abs(xx - pt.x) / abs(x2 - pt.x);
					yy = pt.y + (yy * dy);
					INPUT inp;
					inp.type = INPUT_MOUSE;
					inp.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
					inp.mi.dx = xx * 65535 / GetSystemMetrics(SM_CXSCREEN);
					inp.mi.dy = yy * 65535 / GetSystemMetrics(SM_CYSCREEN);
					inp.mi.mouseData = 0;
					inp.mi.dwExtraInfo = 0;
					inp.mi.time = 0;
					inputs.push_back(inp);
				}
			} else {
				// y������Ɉړ�
				dx = (x2 > pt.x) ? 1 : -1;
				dy = (y2 > pt.y) ? 1 : -1;

				for (int yy = pt.y; yy != y2; yy += dy) {
					int xx = abs(x2 - pt.x) * abs(yy - pt.y) / abs(y2 - pt.y);
					xx = pt.x + (xx * dx);
					INPUT inp;
					inp.type = INPUT_MOUSE;
					inp.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
					inp.mi.dx = xx * 65535 / GetSystemMetrics(SM_CXSCREEN);
					inp.mi.dy = yy * 65535 / GetSystemMetrics(SM_CYSCREEN);
					inp.mi.mouseData = 0;
					inp.mi.dwExtraInfo = 0;
					inp.mi.time = 0;
					inputs.push_back(inp);
				}
			}

			//for (int j = 0; j < inputs.size(); j++) {
			//	SetCursorPos(inputs[j].mi.dx * GetSystemMetrics(SM_CXSCREEN) / 65535, inputs[j].mi.dy * GetSystemMetrics(SM_CYSCREEN) / 65535);
			//	Sleep(1);
			//}

			SendInput(inputs.size(), &inputs[0], sizeof(INPUT));

			//SetCursorPos(s_x, s_y);
/*
			while (1) {
				INPUT inp;
				inp.type = INPUT_MOUSE;
				inp.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
				inp.mi.dx = s_x * 65535 / GetSystemMetrics(SM_CXSCREEN);
				inp.mi.dy = s_y * 65535 / GetSystemMetrics(SM_CYSCREEN);
				inp.mi.mouseData = 0;
				inp.mi.dwExtraInfo = 0;
				inp.mi.time = 0;
				inputs.push_back(inp);

				if (s_edge == 0) {
					s_x += s_speed;
					if (s_x >= s_x2) {
						s_x = s_x2;
						s_edge = 1;
						break;
					}
				} else if (s_edge == 1) {
					s_y += s_speed;
					if (s_y >= s_y2) {
						s_y = s_y2;
						s_edge = 2;
						break;
					}
				} else if (s_edge == 2) {
					s_x -= s_speed;
					if (s_x <= s_x1) {
						s_x = s_x1;
						s_edge = 3;
						break;
					}
				} else if (s_edge == 3) {
					s_y -= s_speed;
					if (s_y <= s_y1) {
						s_y = s_y1;
						s_edge = 0;
						break;
					}
				}
			}

			SendInput(inputs.size(), &inputs[0], sizeof(INPUT));
			*/
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���ł��B
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
