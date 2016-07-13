#include <windows.h>
#include <sstream>
#include "../kjmlib3_v1/kjmlib3.h"

using namespace std;

#define LOGC_NAME	"BGUP"

#define SRC_FILE	"c:\\bgup\\BackupTool.exe"
#define DST_FILE	"c:\\mpc2\\BackupTool\\BackupTool.exe"
#define DST_INIFILE	"c:\\mpc2\\BackupTool\\BackupTool.ini"

kl3::log g_log(LOGC_NAME);

string g_strMyFileName = kl3::util::get_module_file_name();		// �������g�̃t���p�X����ێ�����

//---------------------------------------------------------------------
// bgup.ini [option] BackupPath ���擾
// �f�t�H���g�́A���s�t�@�C���̃p�X�B
//---------------------------------------------------------------------
string GetIni_Option_BackupPath() {
	char szTemp[MAX_PATH];

	GetPrivateProfileString(
		"option",
		"BackupPath",
		kl3::path::remove_file_spec(g_strMyFileName).c_str(),
		szTemp,
		sizeof(szTemp),
		kl3::path::rename_extension(g_strMyFileName, ".ini").c_str());

	return szTemp;
}

//---------------------------------------------------------------------
// BackupTool.exe�̋N����҂�
//---------------------------------------------------------------------
DWORD WaitProcess() {
	g_log.write("[ BackupTool.exe ] ���N������܂ő҂��܂�(�ő� 1 ��)�B");

	kl3::tool_help th;
	bool bFind = false;
	DWORD dwProcessId = 0;

	DWORD dwStart = GetTickCount();
	while (!bFind) {
		if (th.create_snapshot()) {
			bool bRet = th.process_first();
			while (bRet) {
				if (stricmp("backuptool.exe", th.get_exe_file().c_str()) == 0) {
					bFind = true;
					dwProcessId = th.get_process_id();
					break;
				}

				bRet = th.process_next();
			}
			th.close_handle();
		}

		if ((GetTickCount() - dwStart) > (60 * 1000)) {
			break;
		}

		Sleep(1 * 1000);
	}

	if (dwProcessId != 0) {
		g_log.write("[ BackupTool.exe ] ���N�����܂����B�v���Z�XID�� %d �ł��B", dwProcessId);
	} else {
		g_log.write("[ BackupTool.exe ] ���N�����Ă��܂���ł����B");
	}
		
	return dwProcessId;
}

//---------------------------------------------------------------------
// BackupTool.exe���E��
//---------------------------------------------------------------------
void KillProcess(DWORD dwProcessId) {
	g_log.write("�N������ BackupTool.exe (�v���Z�XID %d) �������I�����܂��B", dwProcessId);

	kl3::process::terminate_process(dwProcessId, 0);
}

//---------------------------------------------------------------------
// BackupTool.exe ���o�b�N�A�b�v����
//---------------------------------------------------------------------
bool BackupFile() {

	// �w�肳�ꂽ�o�b�N�A�b�v������炩���ߍ쐬���Ă���

	string strBackupPath = GetIni_Option_BackupPath();
	kl3::util::create_directory(strBackupPath);

	// DST_FILE �Ŏw�肳�ꂽ�t�@�C�����A����̏ꏊ�Ƀo�b�N�A�b�v���܂��B
	string strBackupFilename = kl3::util::create_backup_filename(
		kl3::path::append(strBackupPath, kl3::path::find_file_name(DST_FILE))
	);

	g_log.write("[ %s ] �� [ %s ] �Ƀo�b�N�A�b�v���܂��B", DST_FILE, strBackupFilename.c_str());

	if (CopyFile(DST_FILE, strBackupFilename.c_str(), FALSE) == FALSE) {
		DWORD dwErr = GetLastError();

		g_log.write("�R�s�[�Ɏ��s���܂����B");
		g_log.write("�G���[�ڍ� : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}

	// DST_INIFILE �Ŏw�肳�ꂽ�t�@�C�����A����̏ꏊ�Ƀo�b�N�A�b�v���܂��B
	strBackupFilename = kl3::util::create_backup_filename(
		kl3::path::append(strBackupPath, kl3::path::find_file_name(DST_INIFILE))
	);

	g_log.write("[ %s ] �� [ %s ] �Ƀo�b�N�A�b�v���܂��B", DST_INIFILE, strBackupFilename.c_str());

	if (CopyFile(DST_INIFILE, strBackupFilename.c_str(), FALSE) == FALSE) {
		DWORD dwErr = GetLastError();

		g_log.write("�R�s�[�Ɏ��s���܂����B");
		g_log.write("�G���[�ڍ� : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}

	return true;
}

//---------------------------------------------------------------------
// BackupTool.exe���R�s�[����
//---------------------------------------------------------------------
bool UpdateFile() {
	g_log.write("[ %s ] �� [ %s ] �Ɉړ����܂��B", SRC_FILE, DST_FILE);

	// �폜�O�ɓǂݎ���p�������O���Ă���
	DWORD dwAttr = GetFileAttributes(DST_FILE);
	if (dwAttr == 0xffffffff) {
		DWORD dwErr = GetLastError();

		g_log.write("[ %s ] �̑����擾�Ɏ��s���܂����B", DST_FILE);
		g_log.write("�G���[�ڍ� : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}
	if (SetFileAttributes(DST_FILE, dwAttr & ~FILE_ATTRIBUTE_READONLY) == FALSE) {
		DWORD dwErr = GetLastError();

		g_log.write("[ %s ] �̑����ݒ�Ɏ��s���܂����B", DST_FILE);
		g_log.write("�G���[�ڍ� : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}

	if (DeleteFile(DST_FILE) == FALSE) {
		DWORD dwErr = GetLastError();

		g_log.write("[ %s ] �̍폜�Ɏ��s���܂����B", DST_FILE);
		g_log.write("�G���[�ڍ� : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}

	if (MoveFile(SRC_FILE, DST_FILE) == FALSE) {
		DWORD dwErr = GetLastError();

		g_log.write("[ %s ] �̈ړ��Ɏ��s���܂����B", SRC_FILE);
		g_log.write("�G���[�ڍ� : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}

	return true;
}

//---------------------------------------------------------------------
// BackupTool.ini���X�V����
//---------------------------------------------------------------------
bool UpdateIni() {
	g_log.write("INI�t�@�C�� [ %s ] ���X�V���܂��B", DST_INIFILE);

	// �폜�O�ɓǂݎ���p�������O���Ă���
	DWORD dwAttr = GetFileAttributes(DST_INIFILE);
	if (dwAttr == 0xffffffff) {
		DWORD dwErr = GetLastError();

		g_log.write("[ %s ] �̑����擾�Ɏ��s���܂����B", DST_INIFILE);
		g_log.write("�G���[�ڍ� : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}
	if (SetFileAttributes(DST_INIFILE, dwAttr & ~FILE_ATTRIBUTE_READONLY) == FALSE) {
		DWORD dwErr = GetLastError();

		g_log.write("[ %s ] �̑����ݒ�Ɏ��s���܂����B", DST_INIFILE);
		g_log.write("�G���[�ڍ� : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}

	if (WritePrivateProfileString("option", "DstFile", "mpc2", DST_INIFILE) == 0) {
		DWORD dwErr = GetLastError();

		g_log.write("[ %s ] �̍X�V�Ɏ��s���܂���(�X�V1)�B", DST_INIFILE);
		g_log.write("�G���[�ڍ� : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}

	if (WritePrivateProfileString("option", "DstFile2", "mpc", DST_INIFILE) == 0) {
		DWORD dwErr = GetLastError();

		g_log.write("[ %s ] �̍X�V�Ɏ��s���܂���(�X�V2)�B", DST_INIFILE);
		g_log.write("�G���[�ڍ� : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}

	if (WritePrivateProfileString("option", "DstDump", "mpc2_dump", DST_INIFILE) == 0) {
		DWORD dwErr = GetLastError();

		g_log.write("[ %s ] �̍X�V�Ɏ��s���܂���(�X�V3)�B", DST_INIFILE);
		g_log.write("�G���[�ڍ� : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}

	return true;
}

//---------------------------------------------------------------------
// BackupTool.exe���N������
//---------------------------------------------------------------------
bool StartFile() {
	bool bRet;

	g_log.write("�o�b�N�A�b�v�c�[�� [ %s ] ���ċN�����܂��B", DST_FILE);

	kl3::process proc;
	if (proc.create_process(DST_FILE)) {
		proc.close_handle();

		bRet = true;
	} else {
		DWORD dwErr = GetLastError();

		g_log.write("[ %s ] �̋N���Ɏ��s���܂����B", DST_FILE);
		g_log.write("�G���[�ڍ� : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		bRet = false;
	}

	return bRet;
}

//---------------------------------------------------------------------
// �v���O�����E�X�^�[�g
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	g_log.write("�������� [ %s ] �J�n ��������", g_strMyFileName.c_str());

	// �N������ 60 �b�҂�������
	g_log.write("�V�X�e�������肷��܂� 60 �b�ҋ@���܂��B");
	Sleep(60 * 1000);

	// c:\bgup\BackupTool.exe �����݂��邩�ǂ������ׂ�
	if (kl3::path::file_exists(SRC_FILE)) {

		g_log.write("�R�s�[���̃t�@�C�� [ %s ] ���A����܂��B", SRC_FILE);

		// BackupTool.exe���N������̂�҂�
		DWORD dwProcId = WaitProcess();

		// BackupTool.exe���I��������
		if (dwProcId != 0) {
			KillProcess(dwProcId);
		}

		// BackupTool.exe ���o�b�N�A�b�v����
		if (BackupFile() == false) goto end_p;

		// BackupTool.exe���X�V����
		if (UpdateFile() == false) goto end_p;

		// BackupTool.ini���X�V����
		if (UpdateIni() == false) goto end_p;

		// �I�J���g�I�� 3 �b�̑҂����Ԃ�����
		Sleep(3 * 1000);

		// BackupTool.exe���N������
		if (dwProcId != 0) {
			StartFile();
		}
	} else {
		g_log.write("�R�s�[���̃t�@�C�� [ %s ] ���A����܂���B�A�v���P�[�V�������I�����܂��B", SRC_FILE);
	}

end_p:

	g_log.write("�������� [ %s ] �I�� ��������", g_strMyFileName.c_str());

	return 0;
}
