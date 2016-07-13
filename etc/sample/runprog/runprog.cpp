// runprog.cpp : �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include "resource.h"


#define EXEFILE _T ( "DMCCtrl.exe" )
#define MAX_LOADSTRING 100


// �O���[�o���ϐ�:
TCHAR g_szTitle[ MAX_LOADSTRING ];			// �^�C�g�� �o�[ �e�L�X�g
TCHAR g_szWindowClass[ MAX_LOADSTRING ];	// �^�C�g�� �o�[ �e�L�X�g


//---------------------------------------------------------------------
// (���ǔ�)�m���Ƀt�H�A�O���E���h��....�ł���Ƃ����Ȋ֐�
//
// @param hwnd_target:	�t�H�A�O���E���h���������E�B���h�E�ւ̃n���h��
//
// @note WINVER=0x0500 ���`���Ă��������B
//---------------------------------------------------------------------
BOOL TrueSetForegroundWindow( HWND hwnd_target ) {
	
	// �t�H�A�O���E���h�E�B���h�E�̃n���h�����擾
	HWND hwnd_fore = GetForegroundWindow();

	// �t�H�A�O���E���h�E�B���h�E���쐬�����X���b�h��ID���擾
	DWORD tid_fore = GetWindowThreadProcessId( hwnd_fore, NULL );

	// �t�H�A�O���E���h�ɂ������E�B���h�E���쐬�����X���b�h��ID���擾
	DWORD tid_target = GetWindowThreadProcessId( hwnd_target, NULL );

	// �^�[�Q�b�g�̃X���b�h�̓��͂��t�H�A�O���E���h�̃X���b�h�̓��͂ɃA�^�b�`����
	BOOL ret = AttachThreadInput( tid_target, tid_fore, TRUE );

	// �t�H�A�O���E���h���֎~���Ԃ�ۑ����Ă���A0�ɐݒ�B
	DWORD lock_time;
	ret = SystemParametersInfo( SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &lock_time, 0 );
	ret = SystemParametersInfo( SPI_SETFOREGROUNDLOCKTIMEOUT, 0, 0, 0 );

	// �^�[�Q�b�g�̃E�B���h�E���t�H�A�O���E���h��
	ret = SetForegroundWindow( hwnd_target );
	
	// �t�H�A�O���E���h���֎~���Ԃ����ɖ߂�
	ret = SystemParametersInfo( SPI_SETFOREGROUNDLOCKTIMEOUT, 0, ( PVOID )lock_time, 0 );

	// �^�[�Q�b�g�̃X���b�h�̓��͂��t�H�A�O���E���h�̃X���b�h�̓��͂���f�^�b�`����
	ret = AttachThreadInput( tid_target, tid_fore, FALSE );

	return TRUE;
}


//---------------------------------------------------------------------
// �o�[�W�������{�b�N�X�p���b�Z�[�W �n���h��
//---------------------------------------------------------------------
BOOL CALLBACK About( HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam ) {

	switch ( msg ) {
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if ( LOWORD( wParam ) == IDOK || LOWORD( wParam ) == IDCANCEL ) {

			EndDialog( hdlg, LOWORD( wParam ) );
			return TRUE;
		}
		break;
	}

    return FALSE;
}


//---------------------------------------------------------------------
// ���C���E�B���h�E�̃��b�Z�[�W�n���h��
//---------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {

	switch ( msg ) {
	case WM_CREATE:
		RegisterHotKey( hwnd, 'Q', MOD_CONTROL | MOD_SHIFT, 'Q' );
		RegisterHotKey( hwnd, 'W', MOD_CONTROL | MOD_SHIFT, 'W' );
		RegisterHotKey( hwnd, 'E', MOD_CONTROL | MOD_SHIFT, 'E' );
		break;

	case WM_HOTKEY:
		{
			switch ( wParam ) {
			case 'Q':
				DestroyWindow(hwnd);
				break;

			case 'W':
				{
					rkProcess proc;
					proc.SetSIShowWindow( SW_SHOWNORMAL );
					if ( proc.Create( EXEFILE ) ) {
						proc.WaitForInputIdle();

						HWND hwnd_proc = proc.GetProcessWindow();
						if ( hwnd_proc ) {
							TCHAR buf[ 256 ];
							GetWindowText( hwnd_proc, buf, countof( buf ) );
							MessageBox( hwnd_proc, buf, NULL, MB_OK | MB_SETFOREGROUND );
						}
					}
				}
				break;

			case 'E':
				{
					rkProcess proc;
					//proc.SetSIShowWindow( SW_SHOWNORMAL );
					if ( proc.Create( EXEFILE ) ) {
						proc.WaitForInputIdle();

						HWND hwnd_proc = proc.GetProcessWindow();
						if ( hwnd_proc ) {

							TrueSetForegroundWindow( hwnd_proc );
						}
					}
				}
				break;
			}
		}
		break;

	case WM_COMMAND:
		{
			WORD wmId = LOWORD( wParam );
			WORD wmEvent = HIWORD( wParam ); 

			// ���j���[�I���̉��
			switch( wmId ) {
			case IDM_ABOUT:
				DialogBox( GetWindowInstance( hwnd ), MAKEINTRESOURCE( IDD_ABOUTBOX ), hwnd, About );
				break;

			case IDM_EXIT:
				DestroyWindow( hwnd );
				break;

			default:
				return DefWindowProc( hwnd, msg, wParam, lParam );
			}
		}
		break;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint( hwnd, &ps );

			TCHAR szHello[ MAX_LOADSTRING ];
			LoadString( GetWindowInstance( hwnd ), IDS_HELLO, szHello, MAX_LOADSTRING );

			RECT rt;
			GetClientRect( hwnd, &rt );
			DrawText( hdc, szHello, strlen( szHello ), &rt, DT_CENTER );

			EndPaint( hwnd, &ps );
		}
		break;

	case WM_DESTROY:
		UnregisterHotKey( hwnd, 'Q' );
		UnregisterHotKey( hwnd, 'W' );
		UnregisterHotKey( hwnd, 'E' );
		PostQuitMessage( 0 );
		break;

	default:
		return DefWindowProc( hwnd, msg, wParam, lParam );

   }
   return 0;
}


//---------------------------------------------------------------------
// �E�B���h�E�̍쐬
//---------------------------------------------------------------------
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow ) {

	HWND hwnd = CreateWindow( g_szWindowClass, g_szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL );

	if ( !hwnd ) {
		return FALSE;
	}

	//ShowWindow( hwnd, nCmdShow );
	//UpdateWindow( hwnd );

	return TRUE;
}


//---------------------------------------------------------------------
// �E�B���h�E�N���X�̓o�^
//---------------------------------------------------------------------
ATOM MyRegisterClass( HINSTANCE hInstance ) {

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof( wcex );

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_RUNPROG ) );
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	wcex.lpszMenuName = MAKEINTRESOURCE( IDC_RUNPROG );
	wcex.lpszClassName = g_szWindowClass;
	wcex.hIconSm = LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_SMALL ) );

	return RegisterClassEx( &wcex );
}


//---------------------------------------------------------------------
// �v���O�����X�^�[�g
//---------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

	// �O���[�o�� �X�g�����O��������
	LoadString( hInstance, IDS_APP_TITLE, g_szTitle, MAX_LOADSTRING);
	LoadString( hInstance, IDC_RUNPROG, g_szWindowClass, MAX_LOADSTRING);

	// �E�B���h�E�N���X�̓o�^
	MyRegisterClass( hInstance );

	// ���C���E�B���h�E�̍쐬
	if ( !InitInstance( hInstance, nCmdShow ) ) {
		return -1;
	}

	HACCEL hAccelTable = LoadAccelerators( hInstance, (LPCTSTR)IDC_RUNPROG );

	// ���b�Z�[�W���[�v
	MSG msg;
	while ( true ) {
		BOOL ret = GetMessage( &msg, NULL, 0, 0 );
		if ( ret == 0 || ret == -1 ) {
			break;
		}

		if ( !TranslateAccelerator( msg.hwnd, hAccelTable, &msg ) ) {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	return msg.wParam;
}
