#include "kjmlib.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

//---------------------------------------------------------------------
// ASCII�������UNICODE������ɕϊ�����
//---------------------------------------------------------------------
std::wstring kjm::util::strA2strW(const char* src) {
	// �K�v�ȕ����������߂�
	int nRet = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
	if (nRet == 0) {
		return std::wstring();	// �G���[�̎��́A�󕶎����Ԃ�
	}

	std::vector<wchar_t> result(nRet);	// �K�v�ȃo�b�t�@���m��

	MultiByteToWideChar(CP_ACP, 0, src, -1, &result[0], result.size());
	return &result[0];
}

//---------------------------------------------------------------------
// UNICODE�������ASCII������ɕϊ�����
//---------------------------------------------------------------------
std::string kjm::util::strW2strA(const wchar_t* src) {
	// �K�v�ȕ����������߂�
	int nRet = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0, NULL, NULL);
	if (nRet == 0) {
		return std::string();	// �G���[�̎��́A�󕶎����Ԃ�
	}

	std::vector<char> result(nRet);	// �K�v�ȃo�b�t�@���m��
	
	WideCharToMultiByte(CP_ACP, 0, src, -1, &result[0], result.size(), NULL, NULL);
	return &result[0];
}

//---------------------------------------------------------------------
// pszPath�Ŏw�肵���f�B���N�g�����́A���ݓ�������nDay�ȏ�Â��t�@�C��
// ���폜���܂��B
//
// �폜�Ώۂ́ApszPath�����̃t�@�C�������ł��B
// �폜�̊�́A(���ݓ��� - �ŏI�X�V����)�̒l�Ȃ̂ŁA���Ԃ܂ōl�����āA
// nDay�Ɣ�r���܂��B
//---------------------------------------------------------------------
void kjm::util::cleanup_dir(const _tstring& pszPath, int nDay) {
	_tstring findPath = kjm::util::path_append(pszPath, _T("*.*"));

	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(findPath.c_str(), &data);
	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}

	do {
		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			__int64 diff = (kjm::time::get_system_time() - kjm::time::time(data.ftLastWriteTime)) / FT_1DAY;
			if (diff >= nDay) {
				_tstring delFile = kjm::util::path_append(pszPath, data.cFileName);
				DeleteFile(delFile.c_str());
			}
		}
	} while (FindNextFile(hFind, &data));

	FindClose(hFind);
}

//---------------------------------------------------------------------
// GetEnvironmentVariable�֐��̃��b�p�[
//---------------------------------------------------------------------
_tstring kjm::util::get_environment_variable(const _tstring& lpName) {
	// �K�v�ȕ����������߂�
	DWORD dwRet = GetEnvironmentVariable(lpName.c_str(), NULL, 0);
	if (dwRet == 0) {
		return _tstring();	// ���ϐ��Ȃ��̎��́A�󕶎����Ԃ�
	}

	std::vector<TCHAR> buffer(dwRet);	// �K�v�ȃo�b�t�@���m��

	GetEnvironmentVariable(lpName.c_str(), &buffer[0], buffer.size());
	return &buffer[0];
}

//---------------------------------------------------------------------
// GetModuleFileName�֐��̃��b�p�[
//---------------------------------------------------------------------
_tstring kjm::util::get_module_filename(HMODULE hModule) {
	TCHAR buffer[MAX_PATH];
	GetModuleFileName(hModule, buffer, countof(buffer));
	return buffer;
}

//---------------------------------------------------------------------
// GetPrivateProfileString�֐��̃��b�p�[
//---------------------------------------------------------------------
_tstring kjm::util::get_private_profile_string(const _tstring& lpSectionName, const _tstring& lpKeyName, const _tstring& lpDefault, const _tstring& lpFileName) {
	std::vector<TCHAR> buffer(512);
	while (1) {
		int nRet = GetPrivateProfileString(lpSectionName.c_str(), lpKeyName.c_str(), lpDefault.c_str(), &buffer[0], buffer.size(), lpFileName.c_str());
		if (buffer.size() - 1 > nRet) {
			// �o�b�t�@�̃T�C�Y - 1���A�߂�l�����傫���Ƃ��́A
			// ���ׂĂ̕�����ǂݍ��񂾂��Ƃ��m���Ȃ̂ŁA���[�v�𔲂���B
			break;
		}
		buffer.resize(buffer.size() * 2);
	}

	return &buffer[0];
}

//---------------------------------------------------------------------
// LoadString�֐��̃��b�p�[
//---------------------------------------------------------------------
_tstring kjm::util::load_string(UINT uID, HINSTANCE hInstance) {
	std::vector<TCHAR> buffer(512);
	while (1) {
		int nRet = LoadString(hInstance, uID, &buffer[0], buffer.size());
		if (nRet == 0) {
			return _tstring();	// �����񃊃\�[�X���������́A�󕶎����Ԃ�
		} else if (buffer.size() - 1 > nRet) {
			// �o�b�t�@�̃T�C�Y - 1���A�߂�l�����傫���Ƃ��́A
			// ���ׂĂ̕�����ǂݍ��񂾂��Ƃ��m���Ȃ̂ŁA���[�v�𔲂���B
			break;
		}
		buffer.resize(buffer.size() * 2);
	}

	return &buffer[0];
}

//---------------------------------------------------------------------
// PathAppend�֐��̃��b�p�[
//---------------------------------------------------------------------
_tstring kjm::util::path_append(const _tstring& pPath, const _tstring& pMore) {
	TCHAR buffer[MAX_PATH];
	if (PathAppend(lstrcpy(buffer, pPath.c_str()), pMore.c_str()) == FALSE) {
		return _tstring();	// �֐����s�̎��́A�󕶎����Ԃ�
	}

	return buffer;
}

//---------------------------------------------------------------------
// PathRemoveFileSpec�̃��b�p�[
//---------------------------------------------------------------------
_tstring kjm::util::path_remove_filespec(const _tstring& path) {
	TCHAR buffer[MAX_PATH];
	if (PathRemoveFileSpec(lstrcpy(buffer, path.c_str())) == FALSE) {
		return _tstring();	// �֐����s�̂Ƃ��́A�󕶎����Ԃ�
	}

	return buffer;
}


//---------------------------------------------------------------------
// PathRenameExtension�̃��b�p�[
//---------------------------------------------------------------------
_tstring kjm::util::path_rename_extension(const _tstring& pszPath, const _tstring& pszExt) {
	TCHAR buffer[MAX_PATH];
	if (PathRenameExtension(lstrcpy(buffer, pszPath.c_str()), pszExt.c_str()) == FALSE) {
		return _tstring();	// �֐����s�̎��́A�󕶎����Ԃ�
	}

	return buffer;
}

//---------------------------------------------------------------------
// �ċA�I�Ƀf�B���N�g�����쐬����CreateDirectory�֐�
//---------------------------------------------------------------------
bool kjm::util::create_directory(const _tstring& path) {
	// �w��t�H���_�����[�g���w���Ă���Ƃ��́A����I��
	if (PathIsRoot(path.c_str())) {
		return true;
	}

	// ���łɃt�H���_�����݂���̂ŁA����I��
	if (PathFileExists(path.c_str())) {
		return true;
	}

	// �e�t�H���_�̍쐬�����݂�
	TCHAR work[MAX_PATH];
	PathAppend(lstrcpy(work, path.c_str()), _T(".."));
	if (kjm::util::create_directory(work)) {
		if (CreateDirectory(path.c_str(), NULL)) {
			return true;
		}
	}

	return false;
}
