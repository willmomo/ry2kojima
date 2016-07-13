// noactive.cpp : �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include "resource.h"

#define WS_EX_NOACTIVATE	0x8000000

#define MAX_LOADSTRING 100

// �O���[�o���ϐ�:
HINSTANCE hInst;					// ���݂̃C���X�^���X
TCHAR szTitle[MAX_LOADSTRING];				// �^�C�g�� �o�[ �e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// �^�C�g�� �o�[ �e�L�X�g
RECT g_rcMonitor;

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̑O�錾:
ATOM			MyRegisterClass( HINSTANCE hInstance );
BOOL			InitInstance( HINSTANCE, int );
LRESULT CALLBACK	About( HWND, UINT, WPARAM, LPARAM );

//
//  �֐�: WndProc(HWND, unsigned, WORD, LONG)
//
//  �p�r: ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- �I�����b�Z�[�W�̒ʒm�ƃ��^�[��
//
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static POINT s_points[10];
	static int s_cursor;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) {
	case WM_RBUTTONUP:
		DestroyWindow(hwnd);
		break;

	case WM_LBUTTONUP:
		{
			int xPos = LOWORD(lParam);
			int yPos = HIWORD(lParam);
			s_points[s_cursor].x = xPos;
			s_points[s_cursor].y = yPos;
			if (++s_cursor >= 10) {
				s_cursor = 0;
			}
			InvalidateRect(hwnd, NULL, TRUE);
		}
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// ���j���[�I���̉��:
		switch (wmId) {
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hwnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hwnd);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);

			for (int i = 0; i < 10; i++) {
				if (s_points[i].x != 0 || s_points[i].y != 0) {
					TCHAR buf[128];
					wsprintf(buf, _T("(%d,%d)"), s_points[i].x, s_points[i].y);
					TextOut(hdc, s_points[i].x, s_points[i].y, buf, lstrlen(buf));
				}
			}

			EndPaint(hwnd, &ps);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	static int s_monitorNo = 0;

	++s_monitorNo;

	if (s_monitorNo == 1) {
		g_rcMonitor = *lprcMonitor;
	}

	MONITORINFOEX mi;
	mi.cbSize = sizeof(mi);
	if (GetMonitorInfo(hMonitor, &mi)) {
		if (s_monitorNo == dwData) {
			g_rcMonitor = *lprcMonitor;
			return FALSE;
		}
	}
	return TRUE;
}

//
//   �֐�: InitInstance(HANDLE, int)
//
//   �p�r: �C���X�^���X �n���h���̕ۑ��ƃ��C�� �E�B���h�E�̍쐬
//
//   �R�����g:
//
//        ���̊֐��ł́A�C���X�^���X �n���h�����O���[�o���ϐ��ɕۑ����A�v���O������
//        ���C�� �E�B���h�E���쐬���\�����܂��B
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	HWND hwnd;

	hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X �n���h����ۑ����܂�

	// EnumDisplayMonitors �Ȃǂ��g���ꍇ�́A
	// WINVER=0x0500
	// ���`���Ă����K�v������B
	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 3);

	hwnd = CreateWindowEx(WS_EX_NOACTIVATE, szWindowClass, szTitle, WS_POPUPWINDOW,
		g_rcMonitor.left, g_rcMonitor.top, g_rcMonitor.right - g_rcMonitor.left, g_rcMonitor.bottom - g_rcMonitor.top,
		NULL, NULL, hInstance, NULL);

	if (!hwnd) {
		return FALSE;
	}

	ShowWindow( hwnd, nCmdShow );
	UpdateWindow( hwnd );

	return TRUE;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
 	// TODO: ���̈ʒu�ɃR�[�h���L�q���Ă��������B
	MSG msg;
	HACCEL hAccelTable;

	// �O���[�o�� �X�g�����O�����������܂�
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_NOACTIVE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass( hInstance );

	// �A�v���P�[�V�����̏��������s���܂�:
	if( !InitInstance( hInstance, nCmdShow ) ) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_NOACTIVE);

	// ���C�� ���b�Z�[�W ���[�v:
	while( GetMessage(&msg, NULL, 0, 0) ) 
	{
		if( !TranslateAccelerator (msg.hwnd, hAccelTable, &msg) ) 
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	return msg.wParam;
}



//
//  �֐�: MyRegisterClass()
//
//  �p�r: �E�B���h�E �N���X�̓o�^
//
//  �R�����g:
//
//    ���̊֐�����т��̎g�p�͂��̃R�[�h�� Windows 95 �Ő�ɒǉ����ꂽ
//    'RegisterClassEx' �֐��� Win32 �V�X�e���̌݊�����ێ��������ꍇ��
//    �̂ݕK�v�ƂȂ�܂��B�A�v���P�[�V�������A�A�v���P�[�V�����Ɋ֘A�t��
//    ��ꂽ�X���[�� �A�C�R�����擾�ł���悤�A���̊֐����Ăяo�����Ƃ�
//    �d�v�ł��B
//
ATOM MyRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style		= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon		= LoadIcon(hInstance, (LPCTSTR)IDI_NOACTIVE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx( &wcex );
}

// �o�[�W�������{�b�N�X�p���b�Z�[�W �n���h��
LRESULT CALLBACK About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if( LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL ) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
