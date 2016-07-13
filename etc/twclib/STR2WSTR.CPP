#include "twclib.h"

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
