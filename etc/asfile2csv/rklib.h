#ifndef __RKLIB_H__
#define __RKLIB_H__

// 必要なヘッダをインクルード
#include <windows.h>

// マクロ
#define	countof(I)	(sizeof(I)/sizeof(I[0]))

// プロトタイプ
int chomp(char *pStr);
DWORD FormatMessageFromSystem(DWORD dwError, LPTSTR lpBuffer, DWORD nSize);

#endif
