// noactive.cpp : アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "resource.h"

#define WS_EX_NOACTIVATE	0x8000000

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;					// 現在のインスタンス
TCHAR szTitle[MAX_LOADSTRING];				// タイトル バー テキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// タイトル バー テキスト
RECT g_rcMonitor;

// このコード モジュールに含まれる関数の前宣言:
ATOM			MyRegisterClass( HINSTANCE hInstance );
BOOL			InitInstance( HINSTANCE, int );
LRESULT CALLBACK	About( HWND, UINT, WPARAM, LPARAM );

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
		// メニュー選択の解析:
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
//   関数: InitInstance(HANDLE, int)
//
//   用途: インスタンス ハンドルの保存とメイン ウィンドウの作成
//
//   コメント:
//
//        この関数では、インスタンス ハンドルをグローバル変数に保存し、プログラムの
//        メイン ウィンドウを作成し表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	HWND hwnd;

	hInst = hInstance; // グローバル変数にインスタンス ハンドルを保存します

	// EnumDisplayMonitors などを使う場合は、
	// WINVER=0x0500
	// を定義しておく必要がある。
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
 	// TODO: この位置にコードを記述してください。
	MSG msg;
	HACCEL hAccelTable;

	// グローバル ストリングを初期化します
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_NOACTIVE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass( hInstance );

	// アプリケーションの初期化を行います:
	if( !InitInstance( hInstance, nCmdShow ) ) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_NOACTIVE);

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
	wcex.hIcon		= LoadIcon(hInstance, (LPCTSTR)IDI_NOACTIVE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx( &wcex );
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
