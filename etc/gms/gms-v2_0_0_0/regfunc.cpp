/**
 * ���W�X�g���֘A�̊֐�
 */

#include "kjmutil.h"

/**
 * ���W�X�g�����琔�l(DWORD)��ǂݏo��
 */

DWORD kjm::RegGetValue(HKEY hkeyRoot, LPCTSTR lpSubKey, LPTSTR lpValueName, DWORD dwDefault)
{
	DWORD dwRet = dwDefault;
	HKEY hkeySub;

	// �T�u�L�[���J��
	if (RegOpenKeyEx(hkeyRoot, lpSubKey, 0, KEY_QUERY_VALUE, &hkeySub) == ERROR_SUCCESS) {

		// �l���擾����
		DWORD dwType, dwData, cbData = sizeof(dwData);
		if (RegQueryValueEx(hkeySub, lpValueName, NULL, &dwType, (LPBYTE)&dwData, &cbData) == ERROR_SUCCESS) {

			// �擾�����l�̃^�C�v��REG_DWORD�ł���΁A�߂�l���擾�����l�ɐݒ�B
			if (dwType == REG_DWORD) {
				dwRet = dwData;
			}
		}

		RegCloseKey(hkeySub);
	}

	return dwRet;
}

/**
 * ���W�X�g���ɐ��l(DWORD)�������o��
 */

BOOL kjm::RegSetValue(HKEY hkeyRoot, LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD dwData)
{
	BOOL ret = FALSE;
	HKEY hkeySub;
	DWORD dwDisposition;

	// �T�u�L�[�����(����ΊJ�����)
	if (RegCreateKeyEx(hkeyRoot, lpSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hkeySub, &dwDisposition) == ERROR_SUCCESS) {

		// �l�������o��
		if (RegSetValueEx(hkeySub, lpValueName, 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData)) == ERROR_SUCCESS) {
			ret = TRUE;
		}
		
		RegCloseKey(hkeySub);
	}

	return ret;
}

