//---------------------------------------------------------------------
// syslogd.exe���C���X�g�[�����邽�߂̒��ȒP�v���O����
//---------------------------------------------------------------------
#include "setup.h"

//---------------------------------------------------------------------
// �v���O�����E�X�^�[�g
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	TCHAR cmd[MAX_PATH];
	kjm::process proc;

	// �C���X�g�[���ς݂� SyslogdService ���~����
	if (proc.run(lstrcpy(cmd, _T("net stop SyslogdService")))) {
		proc.wait();
		proc.close();
	}

	// �R�s�[��t�@�C����������
	_tstring dstFile = kjm::util::path_append(
		kjm::util::get_environment_variable(_T("windir")).c_str(), _T("system32\\syslogd.exe"));

	// syslogd.exe ���R�s�[����
	if (!CopyFile(_T("syslogd.exe"), dstFile.c_str(), FALSE)) {
		MessageBox(NULL, _T("�t�@�C���̃R�s�[�Ɏ��s���܂����B"), _T("syslogd setup"), MB_OK | MB_ICONERROR);
		return -1;
	}

	// syslogd.exe ���T�[�r�X�ɓo�^����
	if (proc.run(lstrcat(lstrcpy(cmd, dstFile.c_str()), _T(" install")))) {
		proc.wait();
		proc.close();
	} else {
		MessageBox(NULL, _T("�T�[�r�X�̓o�^�Ɏ��s���܂����B"), _T("syslogd setup"), MB_OK | MB_ICONERROR);
		return -1;
	}

	// syslogd.exe ���X�^�[�g������
	if (proc.run(lstrcpy(cmd, _T("net start SyslogdService")))) {
		proc.wait();
		proc.close();
	} else {
		MessageBox(NULL, _T("�T�[�r�X�̊J�n�Ɏ��s���܂����B"), _T("syslogd setup"), MB_OK | MB_ICONERROR);
		return -1;
	}

	return 0;
}
