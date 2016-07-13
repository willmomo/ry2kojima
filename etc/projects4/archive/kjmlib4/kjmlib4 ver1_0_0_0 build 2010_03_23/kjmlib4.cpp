//=====================================================================
// kjmlib4.cpp (ver.1.0.0.0 build 2010.03.23)
//=====================================================================
#include "kjmlib4.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

//=====================================================================
// ���[�e�B���e�B�N���X(UNICODE�o�[�W����)
//=====================================================================

//---------------------------------------------------------------------
// �f�B���N�g�����ċA�I�ɍ쐬����
//---------------------------------------------------------------------
bool kjm4::util::create_directory(const wchar_t* strPathName) {
	if (PathIsRootW(strPathName))		return true;
	if (PathFileExistsW(strPathName))	return true;

	std::vector<wchar_t> parent(MAX_PATH);
	PathRemoveFileSpecW(lstrcpyW(parent.begin(), strPathName));

	if (kjm4::util::create_directory(parent.begin())) {
		if (CreateDirectoryW(strPathName, NULL)) {
			return true;
		}
	}

	return false;
}


//---------------------------------------------------------------------
// �f�B���N�g�����ċA�I�ɍ폜����
//---------------------------------------------------------------------
bool kjm4::util::remove_folder(const wchar_t* pszPath) {
	bool bRet = true;
	std::vector<wchar_t> wc(MAX_PATH);

	// �����ɕK�v�ȃ��C���h�J�[�h���쐬
	PathAppendW(lstrcpynW(wc.begin(), pszPath, wc.size()), L"*.*");

	WIN32_FIND_DATAW data;
	HANDLE hFind = FindFirstFileW(wc.begin(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if ((lstrcmpW(data.cFileName, L".") != 0) && (lstrcmpW(data.cFileName, L"..") != 0)) {
					std::vector<wchar_t> child(MAX_PATH);

					// �q���t�H���_���ċA�I�ɍ폜����
					PathAppendW(lstrcpynW(child.begin(), pszPath, child.size()), data.cFileName);
					if ((bRet = kjm4::util::remove_folder(child.begin())) == false) {
						break;
					}
				}
			} else {
				std::vector<wchar_t> fname(MAX_PATH);
				PathAppendW(lstrcpynW(fname.begin(), pszPath, fname.size()), data.cFileName);

				// �폜����t�@�C���� readonly �����Ă���Ƃ��͗��Ƃ��B
				DWORD dwAttr = GetFileAttributesW(fname.begin());
				if ((dwAttr != 0xffffffff) && ((dwAttr & FILE_ATTRIBUTE_READONLY) != 0)) {
					if (!SetFileAttributesW(fname.begin(), dwAttr & ~FILE_ATTRIBUTE_READONLY)) {
						bRet = false;
						break;
					}
				}

				if (!DeleteFileW(fname.begin())) {
					bRet = false;
					break;
				}
			}
		} while (FindNextFileW(hFind, &data));

		FindClose(hFind);
	} else {
		bRet = false;
	}

	if (bRet) {
		if (!RemoveDirectoryW(pszPath)) {
			bRet = false;
		}
	}

	return bRet;
}

//=====================================================================
// ���[�e�B���e�B�N���X(ASCII�o�[�W����)
//=====================================================================

//---------------------------------------------------------------------
// ShiftJIS�������UNICODE(UTF16)�ɕϊ�����
//---------------------------------------------------------------------
bool kjm4::util::sjis_to_unicode(const char* sjis_string, std::vector<wchar_t>& unicode_string) {
	int sizeNeeded = MultiByteToWideChar(CP_ACP, 0, sjis_string, lstrlenA(sjis_string) + 1, NULL, 0);
	if (sizeNeeded == 0) {
		return false;
	}

	unicode_string.resize(sizeNeeded);

	if (!MultiByteToWideChar(CP_ACP, 0, sjis_string, lstrlenA(sjis_string) + 1, unicode_string.begin(), unicode_string.size())) {
		return false;
	}

	return true;
}

//---------------------------------------------------------------------
// �f�B���N�g�����ċA�I�ɍ쐬����
//---------------------------------------------------------------------
bool kjm4::util::create_directory(const char* strPathName) {
	std::vector<wchar_t> temp_pszPath;

	if (!kjm4::util::sjis_to_unicode(strPathName, temp_pszPath)) {
		return false;
	}

	return kjm4::util::create_directory(temp_pszPath.begin());
}

//---------------------------------------------------------------------
// �f�B���N�g�����ċA�I�ɍ폜����
//---------------------------------------------------------------------
bool kjm4::util::remove_folder(const char* pszPath) {
	std::vector<wchar_t> temp_pszPath;

	if (!kjm4::util::sjis_to_unicode(pszPath, temp_pszPath)) {
		return false;
	}

	return kjm4::util::remove_folder(temp_pszPath.begin());
}
