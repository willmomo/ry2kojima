#include "sub.h"

wstring StrToWStr(const char* psz)
{
	wstring wstr;

	// 変換に必要なバッファサイズ(文字数(\0含む))を取得
	int nSizeNeeded = MultiByteToWideChar(CP_ACP, 0, psz, -1, NULL, 0);
	if (nSizeNeeded == 0)
		return wstr;

	// バッファを確保
	wchar_t* pwsz = new wchar_t[nSizeNeeded];
	if (pwsz == NULL)
		return wstr;

	// 変換
	int ret = MultiByteToWideChar(CP_ACP, 0, psz, -1, pwsz, nSizeNeeded);
	if (ret == 0) {
		delete[] pwsz;
		return wstr;
	}

	wstr.assign(pwsz);
	delete[] pwsz;

	return wstr;
}

string WStrToStr(const wchar_t* pwsz)
{
	string str;

	// 変換に必要なバッファサイズ(文字数(\0含む))を取得
	int nSizeNeeded = WideCharToMultiByte(CP_ACP, 0, pwsz, -1, NULL, 0, NULL, NULL);
	if (nSizeNeeded == 0)
		return str;

	// バッファを確保
	char* psz = new char[nSizeNeeded];
	if (psz == NULL)
		return str;

	// 変換
	int ret = WideCharToMultiByte(CP_ACP, 0, pwsz, -1, psz, nSizeNeeded, NULL, NULL);
	if (ret == 0) {
		delete[] psz;
		return str;
	}

	str.assign(psz);
	delete[] psz;

	return str;
}

string WildCardToRegExp(const char* psz)
{
	string str;

	str += '^';
	for (int k = 0; psz[k] != '\0';) {
		if (psz[k] == '*') {
			str += '.';
			str += psz[k++];
		} else if (psz[k] == '?') {
			str += '?';
			k++;
		} else {
			str += psz[k++];
		}
	}
	str += '$';

	return str;
}

string GetSystemMessage(DWORD code)
{
	string s;
	LPTSTR p;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, NULL, code, 0, (LPTSTR)&p, 256, NULL);
	s.assign(p);
	LocalFree(p);

	return s;
}
