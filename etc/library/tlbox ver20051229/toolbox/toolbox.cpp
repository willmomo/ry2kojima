//---------------------------------------------------------------------
// toolbox: ���[�e�B���e�B�I�@�\���܂Ƃ߂��A�v���P�[�V����
//---------------------------------------------------------------------


#include "toolbox.h"
#include "resource.h"


//---------------------------------------------------------------------
// �萔
//---------------------------------------------------------------------
#define ID_TIMER	1
#define ID_NOTIFYICON	1
#define MAINWND_CLASSNAME	_T( "ToolBox_MainWnd" )
#define WM_NICALLBACK	( WM_APP + 1 )


//---------------------------------------------------------------------
// �ϐ�
//---------------------------------------------------------------------
static HWND s_hwndWarning = NULL;	// �x���_�C�A���O�̃n���h��
toolboxini g_toolboxini;			// INI�t�@�C���̐ݒ�
bool g_end_flag = false;			// �X���b�h�̏I���w���t���O
std::string g_smart_warning;		// SMART�x��������
std::string g_evlog_warning;		// �C�x���g���O�x��������


//---------------------------------------------------------------------
// �x���_�C�A���O�̃E�B���h�E�v���V�[�W��
//---------------------------------------------------------------------
static BOOL CALLBACK Warning_DlgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	
	switch ( msg ) {
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		{
			// �����A�N�V�������ꂽ��A�E�B���h�E��j������B
			WORD wID = LOWORD( wParam );
			if ( wID == IDOK || wID == IDCANCEL ) {
				DestroyWindow( hwnd );
				return TRUE;
			}
		}
		break;

	case WM_DESTROY:
		// �E�B���h�E�n���h�����N���A���ďI��
		s_hwndWarning = NULL;
		return TRUE;
	}

	return FALSE;
}


//---------------------------------------------------------------------
// ���C���E�B���h�E�̃E�B���h�E�v���V�[�W��
//---------------------------------------------------------------------
static LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {

	static HANDLE s_handle_smartthrd = NULL;
	static HANDLE s_handle_evlogthrd = NULL;
	static NOTIFYICONDATA s_nid;

	switch ( msg ) {
	case WM_CREATE:
		{
			// �^�X�N�g���C�ɃA�C�R����o�^
			memset( &s_nid, 0, sizeof( s_nid ) );
			s_nid.cbSize = sizeof( s_nid );
			s_nid.hWnd = hwnd;
			s_nid.uID = ID_NOTIFYICON;
			s_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
			s_nid.hIcon = LoadIcon( GetWindowInstance( hwnd ), MAKEINTRESOURCE( IDI_APPL ) );
			s_nid.uCallbackMessage = WM_NICALLBACK;
			strcpy( s_nid.szTip, "Toolbox" );

			if ( Shell_NotifyIcon( NIM_ADD, &s_nid ) == 0 ) {
				MessageBox( NULL, "�^�X�N�g���C�ɃA�C�R�����o�^�ł��܂���B", "Toolbox", MB_ICONSTOP | MB_OK );
				return -1;
			}

			// �ݒ肪�L���Ȃ�΁ASMART�Ď��X���b�h�N��
			if ( g_toolboxini.get_ToolboxWatchSmart() ) {
				s_handle_smartthrd = ( HANDLE )_beginthreadex( NULL, 0, smart_thread, NULL, 0, NULL );
			}

			// �ݒ肪�L���Ȃ�΁A�C�x���g���O�Ď��X���b�h�N��
			if ( g_toolboxini.get_ToolboxWatchEventlog() ) {
				s_handle_evlogthrd = ( HANDLE )_beginthreadex( NULL, 0, eventlog_thread, NULL, 0, NULL );
			}

			// �x���_�C�A���O�\���p�̃^�C�}�[���N��
			SetTimer( hwnd, ID_TIMER, 60 * 1000, NULL );
		}
		return 0;

	case WM_NICALLBACK:
		{
			if ( lParam == WM_RBUTTONUP ) {
				// �^�X�N�g���C�̃|�b�v�A�b�v���j���[��\��
				POINT pt;
				GetCursorPos(&pt);

				HMENU hmenu = LoadMenu( GetWindowInstance( hwnd ), MAKEINTRESOURCE( IDR_POPUPMENU ) );
				if ( hmenu ) {
					HMENU hmenuSub = GetSubMenu( hmenu, 0 );
					if ( hmenuSub ) {
						SetForegroundWindow( hwnd );
						TrackPopupMenu( hmenuSub, TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL );
						PostMessage( hwnd, WM_NULL, 0, 0 );
					}

					DestroyMenu( hmenu );
				}
			}
		}
		return 0;

	case WM_COMMAND:
		{
			// �I�����b�Z�[�W�������B
			WORD wID = LOWORD( wParam );

			switch ( wID ) {
			case IDM_FILE_EXIT:
				DestroyWindow( hwnd );
				return 0;

			case IDM_TOOL_RELOAD:
				g_toolboxini.Reload();
				return 0;
			}
		}
		break;

	case WM_ENDSESSION:
		DestroyWindow( hwnd );
		return 0;

	case WM_DESTROY:
		{
			// �^�C�}�[��~
			KillTimer( hwnd, ID_TIMER );

			// �x���_�C�A���O���o�Ă���Ƃ��́A�_�C�A���O������
			if ( s_hwndWarning ) {
				DestroyWindow( s_hwndWarning );
			}

			// �X���b�h�ɏI���w��
			g_end_flag = true;

			// �e�X���b�h�̏I����ҋ@
			if ( s_handle_smartthrd ) {
				WaitForSingleObject( s_handle_smartthrd, 3000 );
				CloseHandle( s_handle_smartthrd );
			}

			if ( s_handle_evlogthrd ) {
				WaitForSingleObject( s_handle_evlogthrd, 3000 );
				CloseHandle( s_handle_evlogthrd );
			}

			// �^�X�N�g���C�A�C�R���̔j��
			Shell_NotifyIcon( NIM_DELETE, &s_nid );

			// �v���O�����I��
			PostQuitMessage( 0 );
		}
		return 0;

	case WM_TIMER:
		{
			if (( g_toolboxini.get_SmartWarning() && !g_smart_warning.empty() ) ||
				( g_toolboxini.get_EventlogWarning() && !g_evlog_warning.empty() )) {

				// SMART�̌x�����o�͂��� and SMART�̌x��������
				// �܂��́A
				// �C�x���g���O�̌x�����o�͂��� and �C�x���g���O�̌x��������
				// �Ƃ��B

				// �x���_�C�A���O�����݂��Ȃ��Ƃ��́A
				// �_�C�A���O���쐬���ĕ\������B
				if ( s_hwndWarning == NULL ) {
					s_hwndWarning = CreateDialog( GetWindowInstance( hwnd ), MAKEINTRESOURCE( IDD_WARNING ), hwnd, Warning_DlgProc );
					if ( s_hwndWarning == NULL ) {
						return 0;
					}

					SetWindowPos( s_hwndWarning, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
				}

				// ���b�Z�[�W�����݂̃��b�Z�[�W�ɕύX����B
				if ( g_toolboxini.get_SmartWarning() && !g_smart_warning.empty() ) {
					SetDlgItemText( s_hwndWarning, IDC_MSG, g_smart_warning.c_str() );
				} else {
					SetDlgItemText( s_hwndWarning, IDC_MSG, g_evlog_warning.c_str() );
				}
			} else {

				// ���b�Z�[�W�������B�܂��́A�o�͂��Ȃ��Ƃ�
				// �x���_�C�A���O�����݂���Ƃ��́A�x���_�C�A���O��j������B
				// Window �n���h���́ADialog�v���V�[�W����NULL�ɃN���A�����B
				if ( s_hwndWarning ) {
					DestroyWindow( s_hwndWarning );
				}
			}
		}
		return 0;
	}

	return DefWindowProc( hwnd, msg, wParam, lParam );
}


//---------------------------------------------------------------------
// �v���O�����X�^�[�g
//---------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

	// INI�t�@�C���̓ǂݍ��݁B
	g_toolboxini.Load( kjm::path_string::GetModuleFileName().RenameExtension( ".ini" ).c_str() );

	// �E�B���h�E�N���X�o�^
	WNDCLASS wc;
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_APPL ) );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	wc.lpszMenuName = MAKEINTRESOURCE( IDR_MAINMENU );
	wc.lpszClassName = MAINWND_CLASSNAME;

	if ( !RegisterClass( &wc ) ) {
		return -1;
	}

	// ���C���E�B���h�E�̍쐬
	HWND hwnd = CreateWindow(
		MAINWND_CLASSNAME, _T( "Toolbox" ),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL );

	if ( !hwnd ) {
		return -1;
	}

	///ShowWindow( hwnd, nCmdShow );

	// ���b�Z�[�W���[�v
	MSG msg;
	while ( true ) {

		BOOL ret = GetMessage( &msg, NULL, 0, 0 );
		if ( ret == 0 || ret == -1 ) {
			break;
		}

		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	return msg.wParam;
}
