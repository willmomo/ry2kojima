#include "rklib.h"

///////////////////////////////////////////////////////////////////////
//	[概要]
//	文字列終端の改行(\r, \n)を取り除く
//
//	[引数]
//	pStr	改行を取り除きたい文字列
//
//	[戻り値]
//	改行を取り除いた数
//	0	:行末に改行無し
//	1	:\x0d or \x0a 1文字の改行を取り除いた
//	2	:\x0d\x0a 2文字対の改行を取り除いた

int chomp(char *pStr)
{
	int nChomp = 0;	// 取り除いた数

	// 行末の\n(\x0a)を取り除く
	int nLen = strlen(pStr);
	if (nLen > 0 && pStr[nLen - 1] == '\n') {
		pStr[nLen - 1] = '\0';
		++nChomp;
	}

	// 行末の\r(\x0d)を取り除く
	nLen = strlen(pStr);
	if (nLen > 0 && pStr[nLen - 1] == '\r') {
		pStr[nLen - 1] = '\0';
		++nChomp;
	}

	return nChomp;
}

///////////////////////////////////////////////////////////////////////
//	[概要]
//	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, ...)簡易呼出
//
//	[引数]
//	dwError		GetLastError()の戻り値
//	lpBuffer	FormatMessage関数のそれに同じ
//	nSize		同上
//
//	[戻り値]
//	FormatMessageに同じ
//
//	[備考]
//	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, lpBuffer, nSize, NULL);
//	をやるだけ。行末の改行は取り除かれます。

DWORD FormatMessageFromSystem(DWORD dwError, LPTSTR lpBuffer, DWORD nSize)
{
	DWORD dwRet = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, lpBuffer, nSize, NULL);
	while (chomp(lpBuffer)) ;

	return dwRet;
}
