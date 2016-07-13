/**
 */
#include "RunAny.h"
#include "MainDlg.h"


/**
	プログラム・スタート
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	MainDlg mainDlg;

	return mainDlg.ShowModalDialog(hInstance, IDD_MAIN, NULL);
}
