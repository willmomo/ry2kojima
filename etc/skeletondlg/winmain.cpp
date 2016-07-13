#include <windows.h>
#include <windowsx.h>
#include "resource.h"

BOOL CALLBACK Main_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_INITDIALOG:
		//SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
		//SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_HIDEWINDOW);
		return TRUE;

	case WM_CLOSE:
		EndDialog(hwnd, 0);
		return TRUE;
	}

	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, Main_DlgProc);
}
