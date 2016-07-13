
#include <windows.h>

#define MAINFRM_CLASSNAME	"MainFrm_SndPlay"

static void OnDestroy(HWND hwnd)
{
	PostQuitMessage(0);
}

LRESULT CALLBACK MainFrm_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_DESTROY:
		OnDestroy(hwnd);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ウィンドウクラスの登録
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainFrm_WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = MAINFRM_CLASSNAME;
	if (RegisterClass(&wc) == 0) {
		return -1;
	}

	// メインフレームの作成
	HWND hwnd = CreateWindow(
		MAINFRM_CLASSNAME,
		"SndPlay1",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	if (hwnd == NULL) {
		return -1;
	}
	ShowWindow(hwnd, nCmdShow);

	// メッセージループ
	MSG msg;
	while (true) {
		BOOL ret = GetMessage(&msg, NULL, 0, 0);
		if (ret <= 0) {
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
