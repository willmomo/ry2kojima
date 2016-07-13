// MouseRampage.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "MouseRampage.h"

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;								// 現在のインターフェイス
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ここにコードを挿入してください。
	MSG msg;
	HACCEL hAccelTable;

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MOUSERAMPAGE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーションの初期化を実行します:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MOUSERAMPAGE));

	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
//  コメント:
//
//    この関数および使い方は、'RegisterClassEx' 関数が追加された
//    Windows 95 より前の Win32 システムと互換させる場合にのみ必要です。
//    アプリケーションが、関連付けられた
//    正しい形式の小さいアイコンを取得できるようにするには、
//    この関数を呼び出してください。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MOUSERAMPAGE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MOUSERAMPAGE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:  メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool s_moving = false;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		RegisterHotKey(hWnd, 1, MOD_ALT|MOD_CONTROL, 'M');
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 描画コードをここに追加してください...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		UnregisterHotKey(hWnd, 1);
		PostQuitMessage(0);
		break;
	case WM_HOTKEY:
		if (s_moving == false) {
			s_moving = true;

			TCHAR fname[MAX_PATH];
			GetModuleFileName(NULL, fname, _countof(fname));
			PathRenameExtension(fname, _T(".ini"));
			DWORD timer = GetPrivateProfileInt(_T("option"), _T("timer"), 20, fname);
			//s_speed = GetPrivateProfileInt(_T("option"), _T("speed"), 2, fname);

			SetTimer(hWnd, 1, timer, NULL);
		} else {
			s_moving = false;
			KillTimer(hWnd, 1);
		}
		break;
	case WM_TIMER:
		{
			std::vector<INPUT> inputs;

			POINT pt;
			GetCursorPos(&pt);
			int x2 = rand() % GetSystemMetrics(SM_CXSCREEN);
			int y2 = rand() % GetSystemMetrics(SM_CYSCREEN);

			int dx, dy;
			if (abs(x2 - pt.x) > abs(y2 - pt.y)) {
				// x軸を基準に移動
				dx = (x2 > pt.x) ? 1 : -1;
				dy = (y2 > pt.y) ? 1 : -1;

				for (int xx = pt.x; xx != x2; xx += dx) {
					int yy = abs(y2 - pt.y) * abs(xx - pt.x) / abs(x2 - pt.x);
					yy = pt.y + (yy * dy);
					INPUT inp;
					inp.type = INPUT_MOUSE;
					inp.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
					inp.mi.dx = xx * 65535 / GetSystemMetrics(SM_CXSCREEN);
					inp.mi.dy = yy * 65535 / GetSystemMetrics(SM_CYSCREEN);
					inp.mi.mouseData = 0;
					inp.mi.dwExtraInfo = 0;
					inp.mi.time = 0;
					inputs.push_back(inp);
				}
			} else {
				// y軸を基準に移動
				dx = (x2 > pt.x) ? 1 : -1;
				dy = (y2 > pt.y) ? 1 : -1;

				for (int yy = pt.y; yy != y2; yy += dy) {
					int xx = abs(x2 - pt.x) * abs(yy - pt.y) / abs(y2 - pt.y);
					xx = pt.x + (xx * dx);
					INPUT inp;
					inp.type = INPUT_MOUSE;
					inp.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
					inp.mi.dx = xx * 65535 / GetSystemMetrics(SM_CXSCREEN);
					inp.mi.dy = yy * 65535 / GetSystemMetrics(SM_CYSCREEN);
					inp.mi.mouseData = 0;
					inp.mi.dwExtraInfo = 0;
					inp.mi.time = 0;
					inputs.push_back(inp);
				}
			}

			//for (int j = 0; j < inputs.size(); j++) {
			//	SetCursorPos(inputs[j].mi.dx * GetSystemMetrics(SM_CXSCREEN) / 65535, inputs[j].mi.dy * GetSystemMetrics(SM_CYSCREEN) / 65535);
			//	Sleep(1);
			//}

			SendInput(inputs.size(), &inputs[0], sizeof(INPUT));

			//SetCursorPos(s_x, s_y);
/*
			while (1) {
				INPUT inp;
				inp.type = INPUT_MOUSE;
				inp.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
				inp.mi.dx = s_x * 65535 / GetSystemMetrics(SM_CXSCREEN);
				inp.mi.dy = s_y * 65535 / GetSystemMetrics(SM_CYSCREEN);
				inp.mi.mouseData = 0;
				inp.mi.dwExtraInfo = 0;
				inp.mi.time = 0;
				inputs.push_back(inp);

				if (s_edge == 0) {
					s_x += s_speed;
					if (s_x >= s_x2) {
						s_x = s_x2;
						s_edge = 1;
						break;
					}
				} else if (s_edge == 1) {
					s_y += s_speed;
					if (s_y >= s_y2) {
						s_y = s_y2;
						s_edge = 2;
						break;
					}
				} else if (s_edge == 2) {
					s_x -= s_speed;
					if (s_x <= s_x1) {
						s_x = s_x1;
						s_edge = 3;
						break;
					}
				} else if (s_edge == 3) {
					s_y -= s_speed;
					if (s_y <= s_y1) {
						s_y = s_y1;
						s_edge = 0;
						break;
					}
				}
			}

			SendInput(inputs.size(), &inputs[0], sizeof(INPUT));
			*/
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
