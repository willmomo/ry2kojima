//---------------------------------------------------------------------
// テスト用のメイン関数
//
// 2009/01/24 小島  新規作成
//---------------------------------------------------------------------
#include <tchar.h>
#include "kjmProcess.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	kjm::Process proc;
	if (proc.Run(_T("notepad"))) {
		while (proc.IsStillActive()) {
			Sleep(0);
		}
	}

	return 0;
}
