//---------------------------------------------------------------------
// 画面キャプチャ
//---------------------------------------------------------------------

#include <windows.h>
#include <windowsx.h>
#include "dibapi.h"


#define CLSNAME_MAINFRM		"scrcap_mainfrm"


// アプリケーションの状態変数
struct app_status {
	// 必要になったら追加。
} g_app_stat;


char g_ini_file[ MAX_PATH ];

void make_ini_filename() {

	char buf[ MAX_PATH ];
	char drive[ MAX_PATH ];
	char dir[ MAX_PATH ];
	char fname[ MAX_PATH ];
	GetModuleFileName( NULL, buf, MAX_PATH );

	_splitpath( buf, drive, dir, fname, NULL );

	_makepath( buf, drive, dir, fname, "ini" );

}


//---------------------------------------------------------------------
// capture screen
//---------------------------------------------------------------------
static void CaptureScreen( HWND hwnd ) {

	RECT rc = { 0, 0, GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ) };
	HDIB hdib = CopyScreenToDIB( &rc );

	HDC hdc = GetDC( hwnd );
	PaintDIB( hdc, &rc, hdib, &rc, NULL );
	ReleaseDC( hwnd, hdc );

	char buf[ MAX_PATH ];
	char drive[ MAX_PATH ];
	char dir[ MAX_PATH ];
	char fname[ MAX_PATH ];
	GetModuleFileName( NULL, buf, MAX_PATH );

	_splitpath( buf, drive, dir, NULL, NULL );

	SYSTEMTIME st;
	GetLocalTime( &st );

	wsprintf( fname, "%04d%02d%02d %02d%02d%02d %03d",
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds );

	_makepath( buf, drive, dir, fname, "bmp" );

	SaveDIB( hdib, buf );

	DestroyDIB( hdib );
}


//---------------------------------------------------------------------
// WM_LBUTTONDOWNメッセージ処理
//---------------------------------------------------------------------
static void OnLButtonDown( HWND hwnd, WPARAM wParam, LPARAM lParam ) {

	CaptureScreen( hwnd );
}


//---------------------------------------------------------------------
// ウィンドウプロシージャ
//---------------------------------------------------------------------
LRESULT CALLBACK MainFrm_WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {

	switch ( msg ) {
	case WM_CREATE:
		if ( !RegisterHotKey( hwnd, 1, 
			GetPrivateProfileInt( "option", "mod", MOD_CONTROL | MOD_SHIFT, g_ini_file ),
			GetPrivateProfileInt( "option", "vk", VK_SNAPSHOT, g_ini_file ) ) ) {


			char buf[ 256 ];
			wsprintf( buf, "mod = %d , vk = %d は、使用されています。",
				GetPrivateProfileInt( "option", "mod", MOD_CONTROL | MOD_SHIFT, g_ini_file ),
				GetPrivateProfileInt( "option", "vk", VK_SNAPSHOT, g_ini_file ) );
			MessageBox( hwnd, buf, "ok", MB_OK );
		}
		return 0;

	case WM_HOTKEY:
		CaptureScreen( hwnd );
		return 0;

	case WM_LBUTTONDOWN:
		OnLButtonDown( hwnd, wParam, lParam );
		return 0;

	case WM_DESTROY:
		UnregisterHotKey( hwnd, 1 );
		PostQuitMessage( 0 );
		return 0;
	}

	return DefWindowProc( hwnd, msg, wParam, lParam );
}


//---------------------------------------------------------------------
// プログラムスタート
//---------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

	// ウィンドウクラスの登録
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainFrm_WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLSNAME_MAINFRM;

	if ( !RegisterClass( &wc )) {
		return -1;
	}

	// メインウィンドウの作成
	HWND hwnd = CreateWindow( 
		CLSNAME_MAINFRM,
		"ScrCap",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL );
	if ( hwnd == NULL ) {
		return -1;
	}

	ShowWindow( hwnd, nCmdShow );

	// メッセージループ
	MSG msg;
	while ( GetMessage( &msg, NULL, 0, 0 )) {
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	return msg.wParam;
}
