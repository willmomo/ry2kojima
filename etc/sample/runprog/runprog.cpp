// runprog.cpp : アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "resource.h"


#define EXEFILE _T ( "DMCCtrl.exe" )
#define MAX_LOADSTRING 100


// グローバル変数:
TCHAR g_szTitle[ MAX_LOADSTRING ];			// タイトル バー テキスト
TCHAR g_szWindowClass[ MAX_LOADSTRING ];	// タイトル バー テキスト


//---------------------------------------------------------------------
// (改良版)確実にフォアグラウンド化....できるといいな関数
//
// @param hwnd_target:	フォアグラウンド化したいウィンドウへのハンドル
//
// @note WINVER=0x0500 を定義してください。
//---------------------------------------------------------------------
BOOL TrueSetForegroundWindow( HWND hwnd_target ) {
	
	// フォアグラウンドウィンドウのハンドルを取得
	HWND hwnd_fore = GetForegroundWindow();

	// フォアグラウンドウィンドウを作成したスレッドのIDを取得
	DWORD tid_fore = GetWindowThreadProcessId( hwnd_fore, NULL );

	// フォアグラウンドにしたいウィンドウを作成したスレッドのIDを取得
	DWORD tid_target = GetWindowThreadProcessId( hwnd_target, NULL );

	// ターゲットのスレッドの入力をフォアグラウンドのスレッドの入力にアタッチする
	BOOL ret = AttachThreadInput( tid_target, tid_fore, TRUE );

	// フォアグラウンド化禁止時間を保存してから、0に設定。
	DWORD lock_time;
	ret = SystemParametersInfo( SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &lock_time, 0 );
	ret = SystemParametersInfo( SPI_SETFOREGROUNDLOCKTIMEOUT, 0, 0, 0 );

	// ターゲットのウィンドウをフォアグラウンド化
	ret = SetForegroundWindow( hwnd_target );
	
	// フォアグラウンド化禁止時間を元に戻す
	ret = SystemParametersInfo( SPI_SETFOREGROUNDLOCKTIMEOUT, 0, ( PVOID )lock_time, 0 );

	// ターゲットのスレッドの入力をフォアグラウンドのスレッドの入力からデタッチする
	ret = AttachThreadInput( tid_target, tid_fore, FALSE );

	return TRUE;
}


//---------------------------------------------------------------------
// バージョン情報ボックス用メッセージ ハンドラ
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
// メインウィンドウのメッセージハンドラ
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

			// メニュー選択の解析
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
// ウィンドウの作成
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
// ウィンドウクラスの登録
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
// プログラムスタート
//---------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

	// グローバル ストリングを初期化
	LoadString( hInstance, IDS_APP_TITLE, g_szTitle, MAX_LOADSTRING);
	LoadString( hInstance, IDC_RUNPROG, g_szWindowClass, MAX_LOADSTRING);

	// ウィンドウクラスの登録
	MyRegisterClass( hInstance );

	// メインウィンドウの作成
	if ( !InitInstance( hInstance, nCmdShow ) ) {
		return -1;
	}

	HACCEL hAccelTable = LoadAccelerators( hInstance, (LPCTSTR)IDC_RUNPROG );

	// メッセージループ
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
