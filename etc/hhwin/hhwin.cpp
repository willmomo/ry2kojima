// hhwin.cpp : アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "resource.h"

#include <htmlhelp.h>

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;					// 現在のインスタンス
TCHAR szTitle[MAX_LOADSTRING];				// タイトル バー テキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// タイトル バー テキスト

// このコード モジュールに含まれる関数の前宣言:
ATOM			MyRegisterClass( HINSTANCE hInstance );
BOOL			InitInstance( HINSTANCE, int );
LRESULT CALLBACK	WndProc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK	About( HWND, UINT, WPARAM, LPARAM );

void Test(HWND hwnd)
{
	HWND hwndHelp = HtmlHelp(hwnd, "D:\\work\\hhwin\\端末変更君XX.chm", HH_DISPLAY_TOPIC, 0);

	HH_WINTYPE *buf = NULL;
	HtmlHelp(hwnd, "D:\\work\\hhwin\\端末変更君XX.chm", HH_GET_WIN_TYPE, (DWORD)&buf);

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
 	// TODO: この位置にコードを記述してください。
	MSG msg;
	HACCEL hAccelTable;

	// グローバル ストリングを初期化します
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_HHWIN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass( hInstance );

	// アプリケーションの初期化を行います:
	if( !InitInstance( hInstance, nCmdShow ) ) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_HHWIN);

	// メイン メッセージ ループ:
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
//  関数: MyRegisterClass()
//
//  用途: ウィンドウ クラスの登録
//
//  コメント:
//
//    この関数およびその使用はこのコードを Windows 95 で先に追加された
//    'RegisterClassEx' 関数と Win32 システムの互換性を保持したい場合に
//    のみ必要となります。アプリケーションが、アプリケーションに関連付け
//    られたスモール アイコンを取得できるよう、この関数を呼び出すことは
//    重要です。
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
//   関数: InitInstance(HANDLE, int)
//
//   用途: インスタンス ハンドルの保存とメイン ウィンドウの作成
//
//   コメント:
//
//        この関数では、インスタンス ハンドルをグローバル変数に保存し、プログラムの
//        メイン ウィンドウを作成し表示します。
//
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
   HWND hWnd;

   hInst = hInstance; // グローバル変数にインスタンス ハンドルを保存します

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
//  関数: WndProc(HWND, unsigned, WORD, LONG)
//
//  用途: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 終了メッセージの通知とリターン
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
			// メニュー選択の解析:
			switch( wmId ) 
			{
				case IDM_ABOUT:
				   //DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   Test(hWnd);
				   break;
				case IDM_EXIT:
					HtmlHelp(hWnd, "D:\\work\\hhwin\\端末変更君XX.chm", HH_CLOSE_ALL, 0);
				   DestroyWindow( hWnd );
				   break;
				default:
				   return DefWindowProc( hWnd, message, wParam, lParam );
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint (hWnd, &ps);
			// TODO: この位置に描画用のコードを追加してください...
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

// バージョン情報ボックス用メッセージ ハンドラ
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
