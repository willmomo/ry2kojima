

#include <windows.h>
#include <windowsx.h>
#include "kjm.h"
#include "resource.h"


//---------------------------------------------------------------------
// ダイアログ・ウィンドウ・プロシージャ
//---------------------------------------------------------------------
BOOL CALLBACK Main_DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_COMMAND:
		{
			switch (LOWORD(wParam)) {
			case IDOK:
				kjm::SystemShutdown::Shutdown();
				return TRUE;

			case IDCANCEL:
				DestroyWindow(hwnd);
				return TRUE;
			}
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	}

	return FALSE;
}


//---------------------------------------------------------------------
// プログラム・スタート
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	HWND hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, Main_DlgProc);
	ShowWindow(hwnd, SW_SHOW);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!IsDialogMessage(hwnd, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}
