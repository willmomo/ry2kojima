// hhwin.cpp : �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include "resource.h"

#include <htmlhelp.h>

#define MAX_LOADSTRING 100

// �O���[�o���ϐ�:
HINSTANCE hInst;					// ���݂̃C���X�^���X
TCHAR szTitle[MAX_LOADSTRING];				// �^�C�g�� �o�[ �e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// �^�C�g�� �o�[ �e�L�X�g

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̑O�錾:
ATOM			MyRegisterClass( HINSTANCE hInstance );
BOOL			InitInstance( HINSTANCE, int );
LRESULT CALLBACK	WndProc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK	About( HWND, UINT, WPARAM, LPARAM );

void Test(HWND hwnd)
{
	HWND hwndHelp = HtmlHelp(hwnd, "D:\\work\\hhwin\\�[���ύX�NXX.chm", HH_DISPLAY_TOPIC, 0);

	HH_WINTYPE *buf = NULL;
	HtmlHelp(hwnd, "D:\\work\\hhwin\\�[���ύX�NXX.chm", HH_GET_WIN_TYPE, (DWORD)&buf);

	{
		char temp[512];
		wsprintf(temp, "0x%08X\n", buf->fsToolBarFlags);
		OutputDebugString(temp);
	}
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
	LoadString(hInstance, IDC_HHWIN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass( hInstance );

	// �A�v���P�[�V�����̏��������s���܂�:
	if( !InitInstance( hInstance, nCmdShow ) ) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_HHWIN);

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
	wcex.hIcon		= LoadIcon(hInstance, (LPCTSTR)IDI_HHWIN);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_HHWIN;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx( &wcex );
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
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
   HWND hWnd;

   hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X �n���h����ۑ����܂�

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if( !hWnd ) 
   {
      return FALSE;
   }

   ShowWindow( hWnd, nCmdShow );
   UpdateWindow( hWnd );

   return TRUE;
}

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch( message ) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// ���j���[�I���̉��:
			switch( wmId ) 
			{
				case IDM_ABOUT:
				   //DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   Test(hWnd);
				   break;
				case IDM_EXIT:
					HtmlHelp(hWnd, "D:\\work\\hhwin\\�[���ύX�NXX.chm", HH_CLOSE_ALL, 0);
				   DestroyWindow( hWnd );
				   break;
				default:
				   return DefWindowProc( hWnd, message, wParam, lParam );
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint (hWnd, &ps);
			// TODO: ���̈ʒu�ɕ`��p�̃R�[�h��ǉ����Ă�������...
			RECT rt;
			GetClientRect( hWnd, &rt );
			DrawText( hdc, szHello, strlen(szHello), &rt, DT_CENTER );
			EndPaint( hWnd, &ps );
			break;
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
   }
   return 0;
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
