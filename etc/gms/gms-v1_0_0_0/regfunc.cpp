/**
 * レジストリ関連の関数
 */

#include "kjmutil.h"

/**
 * レジストリから数値(DWORD)を読み出す
 */

DWORD kjm::RegGetValue(HKEY hkeyRoot, LPCTSTR lpSubKey, LPTSTR lpValueName, DWORD dwDefault)
{
	DWORD dwRet = dwDefault;
	HKEY hkeySub;

	// サブキーを開く
	if (RegOpenKeyEx(hkeyRoot, lpSubKey, 0, KEY_QUERY_VALUE, &hkeySub) == ERROR_SUCCESS) {

		// 値を取得する
		DWORD dwType, dwData, cbData = sizeof(dwData);
		if (RegQueryValueEx(hkeySub, lpValueName, NULL, &dwType, (LPBYTE)&dwData, &cbData) == ERROR_SUCCESS) {

			// 取得した値のタイプがREG_DWORDであれば、戻り値を取得した値に設定。
			if (dwType == REG_DWORD) {
				dwRet = dwData;
			}
		}

		RegCloseKey(hkeySub);
	}

	return dwRet;
}

/**
 * レジストリに数値(DWORD)を書き出す
 */

BOOL kjm::RegSetValue(HKEY hkeyRoot, LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD dwData)
{
	BOOL ret = FALSE;
	HKEY hkeySub;
	DWORD dwDisposition;

	// サブキーを作る(あれば開かれる)
	if (RegCreateKeyEx(hkeyRoot, lpSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hkeySub, &dwDisposition) == ERROR_SUCCESS) {

		// 値を書き出す
		if (RegSetValueEx(hkeySub, lpValueName, 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData)) == ERROR_SUCCESS) {
			ret = TRUE;
		}
		
		RegCloseKey(hkeySub);
	}

	return ret;
}

