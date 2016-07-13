#include <windows.h>
#include "etc.h"
using namespace std;

//---------------------------------------------------------------------
//[概要]	stringをwstringに変換

wstring StringToWString(const string& str)
{
	wstring wstr;	// 戻り文字列
	wstr.empty();	// まず空にする

	// 変換に必要なバッファサイズ(文字数(含む\0))を取得
	int nSizeNeeded = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	if (nSizeNeeded == 0)
		return wstr;

	// バッファを確保
	wchar_t* pwsz = (wchar_t*)malloc(sizeof(wchar_t) * nSizeNeeded);
	if (pwsz == NULL)
		return wstr;

	// 変換
	int ret = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pwsz, nSizeNeeded);
	if (ret == 0) {
		free(pwsz);
		return wstr;
	}

	wstr.assign(pwsz);
	free(pwsz);

	return wstr;
}

//---------------------------------------------------------------------
//[概要]	wstringをstringに変換

string WStringToString(const wstring& wstr)
{
	string str;		// 戻り文字列
	str.empty();	// まず空にする

	// 変換に必要なバッファサイズ(文字数(含む\0))を取得
	int nSizeNeeded = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	if (nSizeNeeded == 0)
		return str;

	// バッファを確保
	char* psz = (char*)malloc(sizeof(char) * nSizeNeeded);
	if (psz == NULL)
		return str;

	// 変換
	int ret = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, psz, nSizeNeeded, NULL, NULL);
	if (ret == 0) {
		free(psz);
		return str;
	}

	str.assign(psz);
	free(psz);

	return str;
}

//---------------------------------------------------------------------
//[概要]	C文字列をUnicode文字列に変換
//[説明]	戻り値は、free()する必要有り

wchar_t* AsciiToUnicode(const char* psz)
{
	// 変換に必要なバッファサイズ(文字数(含む\0))を取得
	int nSizeNeeded = MultiByteToWideChar(CP_ACP, 0, psz, -1, NULL, 0);
	if (nSizeNeeded == 0)
		return NULL;

	// バッファを確保
	wchar_t* pwsz = (wchar_t*)malloc(sizeof(wchar_t) * nSizeNeeded);
	if (pwsz == NULL)
		return NULL;

	// 変換
	int ret = MultiByteToWideChar(CP_ACP, 0, psz, -1, pwsz, nSizeNeeded);
	if (ret == 0) {
		free(pwsz);
		return NULL;
	}

	return pwsz;
}

//---------------------------------------------------------------------
//[概要]	Unicode文字列をC文字列に変換
//[説明]	戻り値は、free()する必要有り

char* UnicodeToAscii(const wchar_t* pwsz)
{
	// 変換に必要なバッファサイズ(文字数(含む\0))を取得
	int nSizeNeeded = WideCharToMultiByte(CP_ACP, 0, pwsz, -1, NULL, 0, NULL, NULL);
	if (nSizeNeeded == 0)
		return NULL;

	// バッファを確保
	char* psz = (char*)malloc(sizeof(char) * nSizeNeeded);
	if (psz == NULL)
		return NULL;

	// 変換
	int ret = WideCharToMultiByte(CP_ACP, 0, pwsz, -1, psz, nSizeNeeded, NULL, NULL);
	if (ret == 0) {
		free(psz);
		return NULL;
	}

	return psz;
}

//---------------------------------------------------------------------
//[概要]	末尾の改行'\n'の削除

int chomp(char* psz)
{
	for (int end = strlen(psz) - 1, cnt = 0; end >= 0 && psz[end] == '\n'; --end, ++cnt)
		psz[end] = '\0';

	return cnt;
}

//---------------------------------------------------------------------
//[概要]	文字列の真ん中を切り出す

char* mid(const char* src, int start, int length, char* dst)
{
	char* p = dst;
	src += start;
	for (int i = 0; i < length; i++) {
		*(p++) = *(src++);
	}
	*p = '\0';

	return dst;
}
