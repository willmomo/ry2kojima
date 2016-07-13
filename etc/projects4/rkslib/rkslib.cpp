#include "rkslib.h"


//---------------------------------------------------------------------
// ShiftJIS�������UNICODE(UTF16)�ɕϊ�����
//---------------------------------------------------------------------
bool sjis_to_unicode(const char* sjis_string, array<wchar_t>& unicode_string) {
	int sizeNeeded = MultiByteToWideChar(CP_ACP, 0, sjis_string, strlen(sjis_string) + 1, NULL, 0);
	if (sizeNeeded == 0) {
		return false;
	}

	if (!unicode_string.resize(sizeNeeded)) {
		return false;
	}

	if (!MultiByteToWideChar(CP_ACP, 0, sjis_string, strlen(sjis_string) + 1, unicode_string, unicode_string.size())) {
		return false;
	}

	return true;
}


//---------------------------------------------------------------------
// �f�B���N�g�����ċA�I�ɍ폜����(UNICODE)
//---------------------------------------------------------------------
bool remove_folderW(const wchar_t* pszPath) {
	bool bRet = true;
	array<wchar_t> wc(MAX_PATH);

	// �����ɕK�v�ȃ��C���h�J�[�h���쐬
	PathAppendW(wcscpy(wc, pszPath), L"*.*");

	WIN32_FIND_DATAW data;
	HANDLE hFind = FindFirstFileW(wc, &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if ((wcscmp(data.cFileName, L".") != 0) && (wcscmp(data.cFileName, L"..") != 0)) {
					array<wchar_t> child(MAX_PATH);

					// �q���t�H���_���ċA�I�ɍ폜����
					PathAppendW(wcscpy(child, pszPath), data.cFileName);
					if ((bRet = remove_folderW(child)) == false) {
						break;
					}
				}
			} else {
				array<wchar_t> fname(MAX_PATH);
				PathAppendW(wcscpy(fname, pszPath), data.cFileName);

				// �폜����t�@�C���� readonly �����Ă���Ƃ��͗��Ƃ��B
				DWORD dwAttr = GetFileAttributesW(fname);
				if ((dwAttr != 0xffffffff) && ((dwAttr & FILE_ATTRIBUTE_READONLY) != 0)) {
					if (!SetFileAttributesW(fname, dwAttr & ~FILE_ATTRIBUTE_READONLY)) {
						bRet = false;
						break;
					}
				}

				if (!DeleteFileW(fname)) {
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


//---------------------------------------------------------------------
// �f�B���N�g�����ċA�I�ɍ폜����(ANSI)
//---------------------------------------------------------------------
bool remove_folderA(const char* pszPath) {
	array<wchar_t> temp_pszPath;

	if (!sjis_to_unicode(pszPath, temp_pszPath)) {
		return false;
	}

	return remove_folderW(temp_pszPath);
}
