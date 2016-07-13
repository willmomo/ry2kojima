#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(__WIN32)
#include <io.h>
#endif
#include <fcntl.h>

/* getopt�ϴؿ���__STDC__̤define���˥�󥯥��顼�ˤʤ�Τ� */
#if !defined(__STDC__)			/* __STDC__��̤����ΤȤ��� */
#define __STDC__	1			/* __STDC__��̵������������ */
#define __MIMIC_STDC__			/* ̵���������������Ȥ�Ф��� */
#endif
#include "getopt.h"
#if defined(__MIMIC_STDC__)		/* __STDC__��̵���������������� */
#undef __STDC__					/* __STDC__���������ä��� */
#undef __MIMIC_STDC__			/* ̵���������������Ȥ�˺��� */
#endif

/*************************************************************************
 * ���
 */

#define MODE_SYSTEM	0
#define MODE_CR		1
#define MODE_LF		2
#define MODE_CRLF	3

/*************************************************************************
 * �����Х��ѿ�
 */

int g_nCrLf = MODE_SYSTEM;	/* ���ԥ⡼�� */
char g_szOutput[256];		/* ���ϥե����� */

/*************************************************************************
 * ������ˡ
 */

void usage()
{
	puts("����ˡ: clf [���ץ����] [�ե����� [...]]");
	puts("");
	puts("  --cr                 ���ϲ��Ԥ�CR�ˤ���");
	puts("  --lf                 ���ϲ��Ԥ�LF�ˤ���");
	puts("  --crlf               ���ϲ��Ԥ�CR/LF�ˤ���");
	puts("                       ���ϲ��Ԥ���ꤷ�ʤ����ϡ�OSɸ��β��Ԥ����");
	puts("  --output[=outfile]   ���ϥե���������.�ե�����̾���ά�������ϡ�clf.out");
	puts("  --help               ���Υإ�ץ�å�������ɽ�����ƽ�λ����");
	puts("  --version            �С����������ɽ�����ƽ�λ����");
	puts("");
	puts("�ե�����ޤ���ɸ�����Ϥ�������Ϥ�,���ԥ����ɤ��Ѵ����ƥե�����ޤ��ϡ�ɸ����Ϥ˽񤭽Ф�.");

	exit(0);
}

/*************************************************************************
 * �С�������ɽ�����ƽ�λ
 */

void version()
{
	puts("clf (Cr/Lf Filter) 1.01");
	exit(0);
}

/*************************************************************************
 * ���ԥ����ɥե��륿
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
 * ���ץ������ǧ����
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
 * �ץ���� ��������
 */

int main(int argc, char** argv)
{
	int i, fArg = 0;
	FILE* in = NULL;
	FILE* out = NULL;

	chkarg(argc, argv);	/* �����Υ����å� */

	/* ���ϥե������������� */
	if (g_szOutput[0] != '\0') {
		if ((out = fopen(g_szOutput, "w")) == NULL) {
			fprintf(stderr, "%s�������ץ�Ǥ��ޤ���.\n", g_szOutput);
			return 1;
		}
	} else {
		out = stdout;
	}

#if defined(_WIN32)
	if (g_nCrLf != MODE_SYSTEM)
		_setmode(_fileno(out), _O_BINARY);
#endif

	/* ���ԥ����ɤ��Ѵ����� */
	fArg = 0;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-") == 0 || argv[i][0] != '-') {
			fArg = 1;	/* ���ϥե�����λ��꤬���ä� */

			/* ���ϥե������������� */
			if (in)
				fclose(in);

			in = (argv[i][0] == '-') ? stdin : fopen(argv[i], "r");
			if (in == NULL) {
				fprintf(stderr, "%s�������ץ�Ǥ��ޤ���.\n", argv[i]);
				return 1;
			}

#if defined(_WIN32)
			_setmode(_fileno(in), _O_BINARY);
#endif
			CrLfFilter(in, out);
		}
	}

	if (fArg == 0) {	/* ���ϥե�����λ��̵꤬���ä�? */
		in = stdin;
#if defined(_WIN32)
		_setmode(_fileno(in), _O_BINARY);
#endif
		CrLfFilter(in, out);
	}

	return 0;
}
