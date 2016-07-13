/*
	ver:20151018-1203
*/

#include "kjm_util.h"

// �e���|�����t�H���_���擾(GetTempPath)

std::basic_string<TCHAR> kjm::util::get_TempPath() {
	DWORD ret;
	std::vector<TCHAR> buf;
	do {
		buf.resize(buf.size() + MAX_PATH);
		ret = GetTempPath(buf.size(), &buf[0]);
	} while (ret > buf.size());
	return &buf[0];
}

// ���s���W���[���̃t�@�C�������擾(GetModuleFileName)

std::basic_string<TCHAR> kjm::util::get_ModuleFileName(HMODULE hModule) {
	DWORD ret;
	std::vector<TCHAR> buf;
	do {
		buf.resize(buf.size() + MAX_PATH);
		ret = ::GetModuleFileName(NULL, &buf[0], buf.size());
	} while (ret == buf.size());
	return &buf[0];
}
