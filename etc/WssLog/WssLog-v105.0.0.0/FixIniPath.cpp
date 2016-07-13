//---------------------------------------------------------------------
// $Header: /GLw3.0.0.x/GetIniFIle/FixIniPath.cpp 2     07/09/07 16:11 Kojima $ 
//
// �A�v���P�[�V��������INI�t�@�C���̓Ǎ�������肷��֐��̒񋟁B
// GetIniFile.dll �� wsslog.dll ���Aprocess attach �̃^�C�~���O�ŁA�g�p����B
//
// INI�t�@�C���̓Ǎ���́A���s�t�@�C�������� .exe ����菜�������́B
//
// INI�t�@�C���̓Ǎ���́A���s�t�@�C���̂���ꏊ�B
// �������A�A�v���P�[�V�������� 'vb6' �̂Ƃ��́A�J�����g�f�B���N�g����INI�t�@�C���̓Ǎ���ɂ���B
// ����ɁA���W�X�g�� HKLM\Software\Sanko\Wss\IniPath ���A����Ƃ��́A�������INI�t�@�C���̓Ǎ���ɂ���B
//---------------------------------------------------------------------
#include "FixIniPath.h"
#include "kjmlib.h"


//---------------------------------------------------------------------
// �A�v���P�[�V��������INI�t�@�C���̓Ǎ�������肷��
//
// [����]
// app_name  �A�v���P�[�V������������o�b�t�@�BMAX_PATH�o�C�g�K�v�B
// app_path  INI�t�@�C���̓Ǎ��悪����o�b�t�@�BMAX_PATH�o�C�g�K�v�B
//---------------------------------------------------------------------
void FixIniPath(char* app_name, char* app_path) {

	char work[MAX_PATH];

	// ���W���[���̃t���p�X���擾����B 
	GetModuleFileName (NULL, work, MAX_PATH);

	// ���s�t�@�C�������擾����
	PathRemoveExtension(strcpy(app_name, PathFindFileName(work)));

	// ���s�t�@�C���̃p�X�����擾����
	PathRemoveFileSpec(strcpy(app_path, work));

	// VB�̏ꍇ�AAppPath���J�����g�f�B���N�g���Ƃ���B
	// "C:\Program Files\Microsoft Visual Studio\VB98\VB6.EXE"
	// "C:\Program Files\Microsoft Visual Studio\"�̃t�@�C����ǂ݂ɂ����̂�h���B
	if (_stricmp(app_name, "vb6") == 0) {

		// �J�����g�f�B���N�g�����Z�b�g����B
		GetCurrentDirectory(MAX_PATH, app_path);
	}

	// !! Embedded �Ή� !!
	// ���W�X�g���ɐݒ肪����Ƃ��́A�������INI�t�@�C���̓Ǎ���Ƃ���
	//
	kjm::registry reg;
	if (reg.reg_open_key_ex(HKEY_LOCAL_MACHINE, _T("Software\\Sanko\\Wss"), KEY_READ) == ERROR_SUCCESS) {
		std::string path = reg.get_string(_T("IniPath"), _T(""));
		if (!path.empty()) {
			strcpy(app_path, path.c_str());
		}
	}
}

/**

$History: FixIniPath.cpp $ 
 * 
 * *****************  Version 2  *****************
 * User: Kojima       Date: 07/09/07   Time: 16:11
 * Updated in $/GLw3.0.0.x/GetIniFIle
 * GetIniFile, WssLog���ʂ̏����p�X�擾�֐� FixIniPath �ǉ��B

**/
