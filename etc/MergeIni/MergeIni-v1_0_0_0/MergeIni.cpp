//-----------------------------------------------------------------------------
// MergeIni ���ini�t�@�C�����}�[�W����v���O����
//-----------------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>
#include "kjmsub.h"

#include "resource.h"


kjm::log g_log;	// ���O�p�̃O���[�o���ϐ�


//-----------------------------------------------------------------------------
// �v���O�����X�^�[�g
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	g_log.set_Ident(_T("MRGINI"));
	g_log.writeInfo(_T("----[ %s ]---- start."), __argv[0]);
	g_log.deleteOldLog();
	kjm::util::set_logObj(&g_log);

	if (__argc != 3) {
		g_log.writeError(_T("MergeIni <�}�[�W��t�@�C����> <�}�[�W���t�@�C����>"));
	}

	g_log.writeInfo(_T("'%s' �̓��e�� '%s' �Ƀ}�[�W���܂��B"), __argv[2], __argv[1]);

	kjm::iniFile iniOrg(__argv[1]);
	iniOrg.mergeIniFile(kjm::iniFile(__argv[2]));

	g_log.writeInfo(_T("----[ %s ]---- end."), __argv[0]);
	return 0;
}
