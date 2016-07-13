/*
	標準入力から入る CR をすべて取り除く。
	標準入力から入る LF をすべて CR LF として標準出力に吐き出す。
	改行コードが

	CR CR LF

	に、なってしまったファイルを CR LF に直すためのプログラム。
*/

// シングルスレッド限定で、IO処理を高速にするマクロ
#define _CRT_DISABLE_PERFCRIT_LOCKS

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include "resource.h"

//---------------------------------------------------------------------------
// プログラムスタート
//---------------------------------------------------------------------------
int main(int argc, char** argv) {

	if (argc != 1) {
		puts("cutcr < 入力ファイル > 出力ファイル");
		puts("リダイレクトしか、受け付けません。");
		return -1;
	}

	_setmode(_fileno(stdin), _O_BINARY);

	int c;
	while ((c = getchar()) != EOF) {
		if (c != '\x0d') {
			putchar(c);
		}
	}

	return 0;
}
