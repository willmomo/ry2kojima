//---------------------------------------------------------------------
// c:\mpc3\mpc2.ini内の、すべてのtenpo_??セクションに
// Send_Directory=\mpc2\from_honbuを書き出すだけのプログラム。
//---------------------------------------------------------------------
#pragma warning(disable : 4786)

#include <windows.h>
#include <tchar.h>
#include <vector>
#include <string>
#include "resource.h"


//---------------------------------------------------------------------
// INIファイルからすべてのセクション名を取得する。
//---------------------------------------------------------------------
int GetPrivateProfileSectionNames(std::vector<std::basic_string<TCHAR> >& names, LPCTSTR lpFileName) {
	std::vector<TCHAR> buf(512);
	do {
		if (GetPrivateProfileSectionNames(buf.begin(), buf.size(), lpFileName) != (buf.size() - 2)) {
			break;
		}
		buf.resize(buf.size() * 2);
	} while (1);


	// \0 で区切りながら、names に追加していく。
	names.clear();
	for (int i = 0; buf[i] != '\0'; i += (lstrlen(&buf[i]) + 1)) {
		names.push_back(&buf[i]);
	}

	return names.size();
}


//---------------------------------------------------------------------
// INIファイルの値を取得する。
// デフォルト値を採用したかどうかの情報も取得できる。
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
// プログラム・スタート
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	LPCTSTR pszIniFile = _T("C:\\mpc3\\mpc2.ini");
	std::vector<std::basic_string<TCHAR> > names;

	// 全セクションを読込み
	GetPrivateProfileSectionNames(names, pszIniFile);

	// TENPO_で始まるセクション内の
	// Send_Directoryキーの値が空文字列のものに
	// \mpc2\from_honbuを書き込む

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
