//---------------------------------------------------------------------
// toolbox: ユーティリティ的機能をまとめたアプリケーション
//---------------------------------------------------------------------


#include "toolbox.h"
#include "resource.h"


//---------------------------------------------------------------------
// 定数
//---------------------------------------------------------------------
#define ID_TIMER	1
#define ID_NOTIFYICON	1
#define MAINWND_CLASSNAME	_T( "ToolBox_MainWnd" )
#define WM_NICALLBACK	( WM_APP + 1 )


//---------------------------------------------------------------------
// 変数
//---------------------------------------------------------------------
static HWND s_hwndWarning = NULL;	// 警告ダイアログのハンドル
toolboxini g_toolboxini;			// INIファイルの設定
bool g_end_flag = false;			// スレッドの終了指示フラグ
std::string g_smart_warning;		// SMART警告文字列
std::string g_evlog_warning;		// イベントログ警告文字列


//---------------------------------------------------------------------
// 警告ダイアログのウィンドウプロシージャ
//---------------------------------------------------------------------
static BOOL CALLBACK Warning_DlgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	
	switch ( msg ) {
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		{
			// 何かアクションされたら、ウィンドウを破棄する。
			WORD wID = LOWORD( wParam );
			if ( wID == IDOK || wID == IDCANCEL ) {
				DestroyWindow( hwnd );
				return TRUE;
			}
		}
		break;

	case WM_DESTROY:
		// ウィンドウハンドルをクリアして終了
		s_hwndWarning = NULL;
		return TRUE;
	}

	return FALSE;
}


//---------------------------------------------------------------------
// メインウィンドウのウィンドウプロシージャ
//---------------------------------------------------------------------
static LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {

	static HANDLE s_handle_smartthrd = NULL;
	static HANDLE s_handle_evlogthrd = NULL;
	static NOTIFYICONDATA s_nid;

	switch ( msg ) {
	case WM_CREATE:
		{
			// タスクトレイにアイコンを登録
			memset( &s_nid, 0, sizeof( s_nid ) );
			s_nid.cbSize = sizeof( s_nid );
			s_nid.hWnd = hwnd;
			s_nid.uID = ID_NOTIFYICON;
			s_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
			s_nid.hIcon = LoadIcon( GetWindowInstance( hwnd ), MAKEINTRESOURCE( IDI_APPL ) );
			s_nid.uCallbackMessage = WM_NICALLBACK;
			strcpy( s_nid.szTip, "Toolbox" );

			if ( Shell_NotifyIcon( NIM_ADD, &s_nid ) == 0 ) {
				MessageBox( NULL, "タスクトレイにアイコンが登録できません。", "Toolbox", MB_ICONSTOP | MB_OK );
				return -1;
			}

			// 設定が有効ならば、SMART監視スレッド起動
			if ( g_toolboxini.get_ToolboxWatchSmart() ) {
				s_handle_smartthrd = ( HANDLE )_beginthreadex( NULL, 0, smart_thread, NULL, 0, NULL );
			}

			// 設定が有効ならば、イベントログ監視スレッド起動
			if ( g_toolboxini.get_ToolboxWatchEventlog() ) {
				s_handle_evlogthrd = ( HANDLE )_beginthreadex( NULL, 0, eventlog_thread, NULL, 0, NULL );
			}

			// 警告ダイアログ表示用のタイマーを起動
			SetTimer( hwnd, ID_TIMER, 60 * 1000, NULL );
		}
		return 0;

	case WM_NICALLBACK:
		{
			if ( lParam == WM_RBUTTONUP ) {
				// タスクトレイのポップアップメニューを表示
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
			// 終了メッセージを処理。
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
			// タイマー停止
			KillTimer( hwnd, ID_TIMER );

			// 警告ダイアログが出ているときは、ダイアログを消す
			if ( s_hwndWarning ) {
				DestroyWindow( s_hwndWarning );
			}

			// スレッドに終了指示
			g_end_flag = true;

			// 各スレッドの終了を待機
			if ( s_handle_smartthrd ) {
				WaitForSingleObject( s_handle_smartthrd, 3000 );
				CloseHandle( s_handle_smartthrd );
			}

			if ( s_handle_evlogthrd ) {
				WaitForSingleObject( s_handle_evlogthrd, 3000 );
				CloseHandle( s_handle_evlogthrd );
			}

			// タスクトレイアイコンの破棄
			Shell_NotifyIcon( NIM_DELETE, &s_nid );

			// プログラム終了
			PostQuitMessage( 0 );
		}
		return 0;

	case WM_TIMER:
		{
			if (( g_toolboxini.get_SmartWarning() && !g_smart_warning.empty() ) ||
				( g_toolboxini.get_EventlogWarning() && !g_evlog_warning.empty() )) {

				// SMARTの警告を出力する and SMARTの警告がある
				// または、
				// イベントログの警告を出力する and イベントログの警告がある
				// とき。

				// 警告ダイアログが存在しないときは、
				// ダイアログを作成して表示する。
				if ( s_hwndWarning == NULL ) {
					s_hwndWarning = CreateDialog( GetWindowInstance( hwnd ), MAKEINTRESOURCE( IDD_WARNING ), hwnd, Warning_DlgProc );
					if ( s_hwndWarning == NULL ) {
						return 0;
					}

					SetWindowPos( s_hwndWarning, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
				}

				// メッセージを現在のメッセージに変更する。
				if ( g_toolboxini.get_SmartWarning() && !g_smart_warning.empty() ) {
					SetDlgItemText( s_hwndWarning, IDC_MSG, g_smart_warning.c_str() );
				} else {
					SetDlgItemText( s_hwndWarning, IDC_MSG, g_evlog_warning.c_str() );
				}
			} else {

				// メッセージが無い。または、出力しないとき
				// 警告ダイアログが存在するときは、警告ダイアログを破棄する。
				// Window ハンドルは、DialogプロシージャでNULLにクリアされる。
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
// プログラムスタート
//---------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

	// INIファイルの読み込み。
	g_toolboxini.Load( kjm::path_string::GetModuleFileName().RenameExtension( ".ini" ).c_str() );

	// ウィンドウクラス登録
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

	// メインウィンドウの作成
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

	// メッセージループ
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
