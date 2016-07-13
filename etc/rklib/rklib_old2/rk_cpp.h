#pragma once

// 標準ヘッダ

#include <string.h>

// 便利マクロ

#define	countof(I)	(sizeof(I)/sizeof(I[0]))

// 関数プロトタイプ

int rkChomp(char *str);
