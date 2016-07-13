//---------------------------------------------------------------------
//---------------------------------------------------------------------
#include "default.h"

///#pragma comment(lib, "ComCtl32.lib")

//---------------------------------------------------------------------
//---------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	C_SETLOCALE_JAPAN;

	///InitCommonControls();

	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, Main_DlgProc);
}
