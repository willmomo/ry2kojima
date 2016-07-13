#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_WIN32)
#include <io.h>
#endif
#include <fcntl.h>
#include "_getopt.h"	// getopt.hのインクルード

const char* VERSION = "clf (Cr/Lf Filter) 1.02";
const char* USAGE = "Usage: clf [OPTION] [FILE]...\n"
 "\n"
 " -r, --cr       CR only\n"
 " -n, --lf       LF only\n"
 " -d, --crlf     CR/LF\n"
 " -o FILE, --output=FILE\n"
 "                write result on FILE instead of standard output\n"
 "     --help     display this help and exit\n"
 "     --version  output version information and exit\n"
 "\n"
 "Convert CR/LF code of FILE(s), or standard input, to standard output.\n"
 "With no FILE read standard input.\n"
 "\n"
 "Report bugs to will@iris.dti.ne.jp";

char* g_pszAppName;	/* アプリケーション名 */

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
 * プログラム スタート
 */

int main(int argc, char** argv)
{
  int f_version = 0, f_help = 0;
  int c;
  int option_index = 0;
  struct option long_options[] = {
    {"cr", no_argument, NULL, 'r'},
    {"lf", no_argument, NULL, 'n'},
    {"crlf", no_argument, NULL, 'd'},
    {"output", required_argument, NULL, 'o'},
    {"version", no_argument, (int*)&f_version, 1},
    {"help", no_argument, (int*)&f_help, 1},
    {NULL, 0, 0, 0}
  };
  int i, fArg = 0;
  FILE* in = NULL;
  FILE* out = NULL;

  g_pszAppName = argv[0];	/* アプリケーションの名前をどこでも使えるようにする */

  while ((c = getopt_long(argc, argv, "rndo:", long_options, &option_index)) != EOF) {
    switch (c) {
    case 0:
      break;
    case 'r':
      g_nCrLf = MODE_CR;
      break;
    case 'n':
      g_nCrLf = MODE_LF;
      break;
    case 'd':
      g_nCrLf = MODE_CRLF;
      break;
    case 'o':
      strcpy(g_szOutput, optarg);
      break;
    default:
      puts(USAGE);
      exit(1);
    }
  }

  if (f_version) {	/* バージョンを表示して終了 */
    puts(VERSION);
    exit(0);
  }
  
  if (f_help) {		/* 使用法を表示して終了 */
    puts(USAGE);
    exit(0);
  }

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
    setmode(fileno(out), O_BINARY);
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
      setmode(fileno(in), O_BINARY);
#endif
      CrLfFilter(in, out);
    }
  }

  if (fArg == 0) {	/* 入力ファイルの指定が無かった? */
    in = stdin;
#if defined(_WIN32)
    setmode(fileno(in), O_BINARY);
#endif
    CrLfFilter(in, out);
  }

  return 0;
}
