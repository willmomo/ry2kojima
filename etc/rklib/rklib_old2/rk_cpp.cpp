// rk_cpp.cpp : cppで使える関数など
//
// 関数、クラスの先頭は必ずrkで始まる。
// 基本的に、Unicodeは無視

#include "rk_cpp.h"

// 文字列終端の改行(\r, \n)を取り除く
//
// [戻り値]
// 0 : 取り除いた文字なし
// 1 : \x0d or \x0a を取り除いた
// 2 : \x0d \x0a を取り除いた

int rkChomp(char *str)
{
	int chomp_count = 0;	// 取り除いた数
	int len;

	// 行末の\n(\x0a)を取り除く
	len = strlen(str);
	if (len > 0 && str[len - 1] == '\n') {
		str[len - 1] = '\0';
		++chomp_count;
	}

	// 行末の\r(\x0d)を取り除く
	len = strlen(str);
	if (len > 0 && str[len - 1] == '\r') {
		str[len - 1] = '\0';
		++chomp_count;
	}

	return chomp_count;
}

