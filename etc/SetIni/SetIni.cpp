//---------------------------------------------------------------------
// c:\mpc3\mpc2.ini���́A���ׂĂ�tenpo_??�Z�N�V������
// Send_Directory=\mpc2\from_honbu�������o�������̃v���O�����B
//---------------------------------------------------------------------
#pragma warning(disable : 4786)

#include <windows.h>
#include <tchar.h>
#include <vector>
#include <string>
#include "resource.h"


//---------------------------------------------------------------------
// INI�t�@�C�����炷�ׂẴZ�N�V���������擾����B
//---------------------------------------------------------------------
int GetPrivateProfileSectionNames(std::vector<std::basic_string<TCHAR> >& names, LPCTSTR lpFileName) {
	std::vector<TCHAR> buf(512);
	do {
		if (GetPrivateProfileSectionNames(buf.begin(), buf.size(), lpFileName) != (buf.size() - 2)) {
			break;
		}
		buf.resize(buf.size() * 2);
	} while (1);


	// \0 �ŋ�؂�Ȃ���Anames �ɒǉ����Ă����B
	names.clear();
	for (int i = 0; buf[i] != '\0'; i += (lstrlen(&buf[i]) + 1)) {
		names.push_back(&buf[i]);
	}

	return names.size();
}


//---------------------------------------------------------------------
// INI�t�@�C���̒l���擾����B
// �f�t�H���g�l���̗p�������ǂ����̏����擾�ł���B
//---------------------------------------------------------------------
std::basic_string<TCHAR> GetPrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPCTSTR lpFileName, bool *pfUseDefault=NULL) {
	std::basic_string<TCHAR> pre_default = std::basic_string<TCHAR>(_T("pre_")) + lpDefault;

	std::vector<TCHAR> pre_buf(max(32, pre_default.length() * 2));
	do {
		if (GetPrivateProfileString(lpAppName, lpKeyName, pre_default.c_str(), pre_buf.begin(), pre_buf.size(), lpFileName) != (pre_buf.size() - 1)) {
			break;
		}
		pre_buf.resize(pre_buf.size() * 2);
	} while (1);

	std::vector<TCHAR> buf(pre_buf.size());
	do {
		if (GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, buf.begin(), buf.size(), lpFileName) != (buf.size() - 1)) {
			break;
		}
		buf.resize(buf.size() * 2);
	} while (1);

	if (pfUseDefault) {
		*pfUseDefault = (lstrcmp(pre_buf.begin(), buf.begin()) != 0);
	}

	return buf.begin();
}


//---------------------------------------------------------------------
// �v���O�����E�X�^�[�g
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	LPCTSTR pszIniFile = _T("C:\\mpc3\\mpc2.ini");
	std::vector<std::basic_string<TCHAR> > names;

	// �S�Z�N�V������Ǎ���
	GetPrivateProfileSectionNames(names, pszIniFile);

	// TENPO_�Ŏn�܂�Z�N�V��������
	// Send_Directory�L�[�̒l���󕶎���̂��̂�
	// \mpc2\from_honbu����������

	int len1 = lstrlen(_T("TENPO_"));
	for (int i = 0; i < names.size(); i++) {
		if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, _T("TENPO_"), len1, names[i].c_str(), min(names[i].length(), len1)) == CSTR_EQUAL) {
			std::basic_string<TCHAR> keyValue = GetPrivateProfileString(names[i].c_str(), _T("Send_Directory"), _T(""), pszIniFile);
			if (keyValue.empty()) {
				WritePrivateProfileString(names[i].c_str(), _T("Send_Directory"), _T("\\mpc2\\from_honbu"), pszIniFile);
			}
		}
	}

	return 0;
}
