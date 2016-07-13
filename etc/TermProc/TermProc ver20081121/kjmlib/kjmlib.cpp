//---------------------------------------------------------------------
// �����p���C�u����
//
// ���ׂẴN���X�́Anamespace kjm�Ŏ��������B
//---------------------------------------------------------------------
#include "kjmlib.h"


//---------------------------------------------------------------------
// �ȈՃ��O�N���X
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// ���O������
//
// [����]
// format  �������䕶����
//---------------------------------------------------------------------
void kjm::log::writeLog(const char* format, ...) {
	std::vector<char> buf(8192);
	va_list marker;

	// ���O�t�@�C���̖��O
	SYSTEMTIME st;
	GetLocalTime(&st);
	std::vector<char> work2(MAX_PATH);
	sprintf(work2.begin(), "%s%04d%02d%02d.log", 
		kjm::win32::pathFindFileName(kjm::win32::pathRemoveExtension(kjm::win32::getModuleFileName())).c_str(),
		st.wYear, st.wMonth, st.wDay);

	// ���O�t�@�C���̍쐬�������
	std::string fname = kjm::win32::pathAppend(kjm::win32::getEnvironmentVariable(_T("TEMP")), work2.begin());

	// �t�@�C�����J����܂ŁA500ms�҂�
	DWORD dw = GetTickCount();
	FILE* fp = NULL;
	while ((fp = _fsopen(fname.begin(), "a+", _SH_DENYWR)) == NULL) {
		if ((GetTickCount() - dw) > 500) break;
		Sleep(0);
	}

	if (fp) {
		fprintf(fp, "[%04d/%02d/%02d %02d:%02d:%02d.%03d][%04X] ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, GetCurrentProcessId());
		va_start(marker, format);
		vfprintf(fp, format, marker);
		va_end(marker);
		fputs("\n", fp);

		fclose(fp);
	}
}


//---------------------------------------------------------------------
// WIN32�֐��̊ȈՌĂяo���N���X
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// GetEnvironmentVariable�̃��b�p�[
//
// [����]
// lpName  ���ϐ��̖��O�ւ̃|�C���^
//
// [�߂�l]
// �w�肳�ꂽ���ϐ������l��Ԃ��B
//---------------------------------------------------------------------
std::string kjm::win32::getEnvironmentVariable(LPCTSTR lpName) {
	// �K�v�ȃo�b�t�@�̃T�C�Y���擾
	DWORD dwSizeNeeded = ::GetEnvironmentVariable(lpName, NULL, 0);
	if (dwSizeNeeded == 0) {
		return "";
	}

	std::vector<TCHAR> buf(dwSizeNeeded);
	::GetEnvironmentVariable(lpName, buf.begin(), buf.size());

	return buf.begin();
}


//---------------------------------------------------------------------
// GetModuleFileName�̃��b�p�[
//
// [����]
// hModule  ���W���[���̃n���h��
//
// [�߂�l]
// ���s�t�@�C���̖��O��Ԃ��B
//---------------------------------------------------------------------
std::string kjm::win32::getModuleFileName(HMODULE hModule) {
	std::vector<TCHAR> buf(MAX_PATH);
	while (::GetModuleFileName(hModule, buf.begin(), buf.size()) == buf.size()) {
		buf.resize(buf.size() * 2);
	}

	return buf.begin();
}


//---------------------------------------------------------------------
// PathAppend �̃��b�p�[
//
// [����]
// strPath  ���ɂȂ�t���p�X�̃t�@�C����
// strMore  ��������p�X�̖��O
//
// [�߂�l]
// �������ꂽ�V�����t�@�C����������
//---------------------------------------------------------------------
std::string kjm::win32::pathAppend(const std::string& strPath, const std::string& strMore) {
	// �K�v�\���ȃo�b�t�@������
	std::vector<TCHAR> work(MAX_PATH + strPath.length() + strMore.length());

	// ���ɂȂ�t�@�C���������[�N�ɃR�s�[
	lstrcpy(work.begin(), strPath.c_str());

	// �p�X����������
	PathAppend(work.begin(), strMore.c_str());

	return work.begin();
}

//---------------------------------------------------------------------
// PathFindFileName �̃��b�p�[
//
// [����]
// strPath  �t�@�C��������������t���p�X�̃t�@�C����
//
// [�߂�l]
// �t�@�C���������̕�����
//---------------------------------------------------------------------
std::string kjm::win32::pathFindFileName(const std::string& strPath) {
	return PathFindFileName(strPath.c_str());
}


//---------------------------------------------------------------------
// PathRemoveExtension �̃��b�p�[
//
// [����]
// strPath  �g���q����菜�������t�@�C����
//
// [�߂�l]
// �g���q����菜�����V����������
//---------------------------------------------------------------------
std::string kjm::win32::pathRemoveExtension(const std::string& strPath) {
	// �K�v�\���ȃo�b�t�@������
	std::vector<TCHAR> work(MAX_PATH + strPath.length());

	// ���ɂȂ�t�@�C���������[�N�ɃR�s�[
	lstrcpy(work.begin(), strPath.c_str());

	// �g���q����菜��
	PathRemoveExtension(work.begin());

	return work.begin();
}


//---------------------------------------------------------------------
// PathRenameExtension �̃��b�p�[
//
// [����]
// strPath  ���ɂӂ���t���p�X�̃t�@�C����
// strExt   �u��������g���q
//
// [�߂�l]
// �g���q��u���������V�����t�@�C����������
//---------------------------------------------------------------------
std::string kjm::win32::pathRenameExtension(const std::string& strPath, const std::string& strExt) {
	// �K�v�\���ȃo�b�t�@������
	std::vector<TCHAR> work(MAX_PATH + strPath.length() + strExt.length());

	// ���ɂȂ�t�@�C���������[�N�ɃR�s�[
	lstrcpy(work.begin(), strPath.c_str());

	// �g���q�u������
	PathRenameExtension(work.begin(), strExt.c_str());

	return work.begin();
}


//---------------------------------------------------------------------
// �t�@�C������������菜���ĐV�����t�@�C��������������
// PathRemoveFileSpec �� PathAppend �����������悤�Ȋ֐�
//
// [����]
// strPath  ���ɂȂ�t���p�X�̃t�@�C����
// strMore  �ύX���� FileSpec
//
// [�߂�l]
// �V�����t���p�X�̃t�@�C�����B
//---------------------------------------------------------------------
std::string kjm::win32::pathRenameFileSpec(const std::string& strPath, const std::string& strMore) {
	// �K�v�\���ȃo�b�t�@������
	std::vector<TCHAR> work(MAX_PATH + strPath.length() + strMore.length());

	// ���ɂȂ�t���p�X�����[�N�ɃR�s�[
	lstrcpy(work.begin(), strPath.c_str());

	// �t�@�C������������菜��
	PathRemoveFileSpec(work.begin());

	// �V�����t�@�C��������������
	PathAppend(work.begin(), strMore.c_str());

	return work.begin();
}


//---------------------------------------------------------------------
// ini�t�@�C���A�N�Z�X�N���X
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//
// [����]
// ���s�t�@�C����.ini ���f�t�H���g�ŃA�N�Z�X����悤�ɒ��������B
//---------------------------------------------------------------------
kjm::iniFile::iniFile() {
	this->m_strIniFile = kjm::win32::pathRenameExtension(kjm::win32::getModuleFileName(), _T(".ini"));
}


//---------------------------------------------------------------------
// ������Z�q
//---------------------------------------------------------------------
kjm::iniFile& kjm::iniFile::operator =(const kjm::iniFile& rhs) {
	if (this == &rhs)	return *this;	// �����I�u�W�F�N�g���w���Ă���

	this->m_strIniFile = rhs.m_strIniFile;
	return *this;
}
