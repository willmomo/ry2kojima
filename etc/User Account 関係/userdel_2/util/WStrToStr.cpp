#include "util.h"

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

