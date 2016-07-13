#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(__WIN32)
#include <io.h>
#endif
#include <fcntl.h>

/* getopt系関数が__STDC__未define時にリンクエラーになるので */
#if !defined(__STDC__)			/* __STDC__が未定義のときは */
#define __STDC__	1			/* __STDC__を無理やり定義して */
#define __MIMIC_STDC__			/* 無理やり定義したことを覚える */
#endif
#include "getopt.h"
#if defined(__MIMIC_STDC__)		/* __STDC__を無理やり定義した時は */
#undef __STDC__					/* __STDC__の定義を取り消して */
#undef __MIMIC_STDC__			/* 無理やり定義したことも忘れる */
#endif

/*************************************************************************
 * 定数
 */

#define MODE_SYSTEM	0
#define MODE_CR		1
#define MODE_LF		2
#define MODE_CRLF	3

/*************************************************************************
 * グローバル変数
 */

int g_nCrLf = MODE_SYSTEM;	/* 改行モード */
char g_szOutput[256];		/* 出力ファイル */

/*************************************************************************
 * 使用方法
 */

void usage()
{
	puts("使用法: clf [オプション] [ファイル [...]]");
	puts("");
	puts("  --cr                 出力改行をCRにする");
	puts("  --lf                 出力改行をLFにする");
	puts("  --crlf               出力改行をCR/LFにする");
	puts("                       出力改行を指定しない場合は、OS標準の改行を使用");
	puts("  --output[=outfile]   出力ファイルを指定.ファイル名を省略した場合は、clf.out");
	puts("  --help               このヘルプメッセージを表示して終了する");
	puts("  --version            バージョン情報を表示して終了する");
	puts("");
	puts("ファイルまたは標準入力からの入力を,改行コードを変換してファイルまたは、標準出力に書き出す.");

	exit(0);
}

/*************************************************************************
 * バージョンを表示して終了
 */

void version()
{
	puts("clf (Cr/Lf Filter) 1.01");
	exit(0);
}

/*************************************************************************
 * 改行コードフィルタ
 */

void CrLfFilter(FILE* in, FILE* out)
{
	char c, c2;

	while ((c = fgetc(in)) != EOF) {
		switch (c) {
		case '\x0d':
			if ((c2 = fgetc(in)) != '\x0a')
				ungetc(c2, in);
		case '\x0a':
			if (g_nCrLf == MODE_CRLF)
				fputs("\x0d\x0a", out);
			else
				fputc("\n\x0d\x0a"[g_nCrLf], out);
			break;
		default:
			fputc(c, out);
			break;
		}
	}
}

/*************************************************************************
 * オプションを確認する
 */

void chkarg(int argc, char** argv)
{
	int c;
	int option_index = 0;
	struct option long_options[] = {
		{"help", no_argument, 0, 0},
		{"version", no_argument, 0, 0},
		{"cr", no_argument, 0, 0},
		{"lf", no_argument, 0, 0},
		{"crlf", no_argument, 0, 0},
		{"output", optional_argument, 0, 0},
		{NULL, 0, 0, 0}
	};

	while ((c = getopt_long(argc, argv, "", long_options, &option_index)) != EOF) {
		switch (c) {
		case 0:
			switch (option_index) {
			case 0:	usage();
			case 1:	version();
			case 2:	g_nCrLf = MODE_CR;	break;
			case 3:	g_nCrLf = MODE_LF;	break;
			case 4:	g_nCrLf = MODE_CRLF;	break;
			case 5:
				strcpy(g_szOutput, (optarg) ? optarg : "clf.out");
				break;
			}
			break;
		default:
			usage();
		}
	}
}

/*************************************************************************
 * プログラム スタート
 */

int main(int argc, char** argv)
{
	int i, fArg = 0;
	FILE* in = NULL;
	FILE* out = NULL;

	chkarg(argc, argv);	/* 引数のチェック */

	/* 出力ファイルを準備する */
	if (g_szOutput[0] != '\0') {
		if ((out = fopen(g_szOutput, "w")) == NULL) {
			fprintf(stderr, "%sがオープンできません.\n", g_szOutput);
			return 1;
		}
	} else {
		out = stdout;
	}

#if defined(_WIN32)
	if (g_nCrLf != MODE_SYSTEM)
		_setmode(_fileno(out), _O_BINARY);
#endif

	/* 改行コードを変換する */
	fArg = 0;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-") == 0 || argv[i][0] != '-') {
			fArg = 1;	/* 入力ファイルの指定があった */

			/* 入力ファイルを準備する */
			if (in)
				fclose(in);

			in = (argv[i][0] == '-') ? stdin : fopen(argv[i], "r");
			if (in == NULL) {
				fprintf(stderr, "%sがオープンできません.\n", argv[i]);
				return 1;
			}

#if defined(_WIN32)
			_setmode(_fileno(in), _O_BINARY);
#endif
			CrLfFilter(in, out);
		}
	}

	if (fArg == 0) {	/* 入力ファイルの指定が無かった? */
		in = stdin;
#if defined(_WIN32)
		_setmode(_fileno(in), _O_BINARY);
#endif
		CrLfFilter(in, out);
	}

	return 0;
}
