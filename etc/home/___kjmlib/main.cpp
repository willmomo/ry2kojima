//---------------------------------------------------------------------
// �e�X�g�p�̃��C���֐�
//
// 2009/01/24 ����  �V�K�쐬
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
