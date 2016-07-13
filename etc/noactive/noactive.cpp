#include <windows.h>
#include <tchar.h>

#define WS_EX_NOACTIVATE	0x8000000

// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch( msg )
	{
//	case WM_MOUSEACTIVATE:
//		return MA_NOACTIVATE;
//		return MA_NOACTIVATEANDEAT;
	case WM_LBUTTONDOWN:
		{
			static unsigned char r = 0;
			static unsigned char g = 0;
			static unsigned char b = 0;
			RECT rc;
			HDC hdc = GetDC(hWnd);
			GetClientRect(hWnd, &rc);
			HBRUSH br = CreateSolidBrush(RGB(r, g, b));
			FillRect(hdc, &rc, br);
			DeleteObject(br);
			ReleaseDC(hWnd, hdc);

			r += 20;
			g += 20;
			b += 20;
		}
		return 0;

	case WM_DESTROY:  // ウィンドウを破棄するとき
		PostQuitMessage( 0 );
		return 0;
	}

	// 他のメッセージは、デフォルトの処理を行う
	return DefWindowProc( hWnd, msg, wp, lp );
}


// 開始位置
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int showCmd)
{
	WNDCLASSEX wc;
	HWND hWnd;
	MSG msg;

	// ウィンドウクラスの情報を設定
	wc.cbSize = sizeof(wc);               // 構造体サイズ
	wc.style = CS_HREDRAW | CS_VREDRAW;   // スタイル
	wc.lpfnWndProc = WndProc;             // ウィンドウプロシージャ
	wc.cbClsExtra = 0;                    // 拡張情報１
	wc.cbWndExtra = 0;                    // 拡張情報２
	wc.hInstance = hInst;                 // インスタンスハンドル
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//	wc.hIcon = (HICON)LoadImage(          // アイコン
//		NULL, MAKEINTRESOURCE(IDI_APPLICATION), IMAGE_ICON,
//		0, 0, LR_DEFAULTSIZE | LR_SHARED
//	);
	wc.hIconSm = wc.hIcon;                // 子アイコン
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wc.hCursor = (HCURSOR)LoadImage(      // マウスカーソル
//		NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR,
//		0, 0, LR_DEFAULTSIZE | LR_SHARED
//	);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ウィンドウ背景
	wc.lpszMenuName = NULL;                     // メニュー名
	wc.lpszClassName = _T("NOACTIVE");// ウィンドウクラス名
	
	// ウィンドウクラスを登録する
	if( RegisterClassEx( &wc ) == 0 ){ return 1; }

	// ウィンドウを作成する
	hWnd = CreateWindowEx(
		WS_EX_NOACTIVATE,
		wc.lpszClassName,      // ウィンドウクラス名
		_T("Sample Program"),  // タイトルバーに表示する文字列
		WS_OVERLAPPEDWINDOW,   // ウィンドウの種類
		CW_USEDEFAULT,         // ウィンドウを表示する位置（X座標）
		CW_USEDEFAULT,         // ウィンドウを表示する位置（Y座標）
		CW_USEDEFAULT,         // ウィンドウの幅
		CW_USEDEFAULT,         // ウィンドウの高さ
		NULL,                  // 親ウィンドウのウィンドウハンドル
		NULL,                  // メニューハンドル
		hInst,                 // インスタンスハンドル
		NULL                   // その他の作成データ
	);
	if( hWnd == NULL ){ return 1; }

	// ウィンドウを表示する
	ShowWindow( hWnd, SW_SHOW );
	UpdateWindow( hWnd );

	// メッセージループ
	while( 1 )
	{
		BOOL ret = GetMessage( &msg, NULL, 0, 0 );  // メッセージを取得する
		if( ret == 0 || ret == -1 )
		{
			// アプリケーションを終了させるメッセージが来ていたら、
			// あるいは GetMessage() が失敗したら( -1 が返されたら ）、ループを抜ける
			break;
		}
		else
		{
			// メッセージを処理する
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	return 0;
}
