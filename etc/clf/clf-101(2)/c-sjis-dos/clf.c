#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(__WIN32)
#include <io.h>
#endif
#include <fcntl.h>

/* getopt�n�֐���__STDC__��define���Ƀ����N�G���[�ɂȂ�̂� */
#if !defined(__STDC__)			/* __STDC__������`�̂Ƃ��� */
#define __STDC__	1			/* __STDC__�𖳗�����`���� */
#define __MIMIC_STDC__			/* ��������`�������Ƃ��o���� */
#endif
#include "getopt.h"
#if defined(__MIMIC_STDC__)		/* __STDC__�𖳗�����`�������� */
#undef __STDC__					/* __STDC__�̒�`���������� */
#undef __MIMIC_STDC__			/* ��������`�������Ƃ��Y��� */
#endif

/*************************************************************************
 * �萔
 */

#define MODE_SYSTEM	0
#define MODE_CR		1
#define MODE_LF		2
#define MODE_CRLF	3

/*************************************************************************
 * �O���[�o���ϐ�
 */

int g_nCrLf = MODE_SYSTEM;	/* ���s���[�h */
char g_szOutput[256];		/* �o�̓t�@�C�� */

/*************************************************************************
 * �g�p���@
 */

void usage()
{
	puts("�g�p�@: clf [�I�v�V����] [�t�@�C�� [...]]");
	puts("");
	puts("  --cr                 �o�͉��s��CR�ɂ���");
	puts("  --lf                 �o�͉��s��LF�ɂ���");
	puts("  --crlf               �o�͉��s��CR/LF�ɂ���");
	puts("                       �o�͉��s���w�肵�Ȃ��ꍇ�́AOS�W���̉��s���g�p");
	puts("  --output[=outfile]   �o�̓t�@�C�����w��.�t�@�C�������ȗ������ꍇ�́Aclf.out");
	puts("  --help               ���̃w���v���b�Z�[�W��\�����ďI������");
	puts("  --version            �o�[�W��������\�����ďI������");
	puts("");
	puts("�t�@�C���܂��͕W�����͂���̓��͂�,���s�R�[�h��ϊ����ăt�@�C���܂��́A�W���o�͂ɏ����o��.");

	exit(0);
}

/*************************************************************************
 * �o�[�W������\�����ďI��
 */

void version()
{
	puts("clf (Cr/Lf Filter) 1.01");
	exit(0);
}

/*************************************************************************
 * ���s�R�[�h�t�B���^
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
 * �I�v�V�������m�F����
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
 * �v���O���� �X�^�[�g
 */

int main(int argc, char** argv)
{
	int i, fArg = 0;
	FILE* in = NULL;
	FILE* out = NULL;

	chkarg(argc, argv);	/* �����̃`�F�b�N */

	/* �o�̓t�@�C������������ */
	if (g_szOutput[0] != '\0') {
		if ((out = fopen(g_szOutput, "w")) == NULL) {
			fprintf(stderr, "%s���I�[�v���ł��܂���.\n", g_szOutput);
			return 1;
		}
	} else {
		out = stdout;
	}

#if defined(_WIN32)
	if (g_nCrLf != MODE_SYSTEM)
		_setmode(_fileno(out), _O_BINARY);
#endif

	/* ���s�R�[�h��ϊ����� */
	fArg = 0;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-") == 0 || argv[i][0] != '-') {
			fArg = 1;	/* ���̓t�@�C���̎w�肪������ */

			/* ���̓t�@�C������������ */
			if (in)
				fclose(in);

			in = (argv[i][0] == '-') ? stdin : fopen(argv[i], "r");
			if (in == NULL) {
				fprintf(stderr, "%s���I�[�v���ł��܂���.\n", argv[i]);
				return 1;
			}

#if defined(_WIN32)
			_setmode(_fileno(in), _O_BINARY);
#endif
			CrLfFilter(in, out);
		}
	}

	if (fArg == 0) {	/* ���̓t�@�C���̎w�肪��������? */
		in = stdin;
#if defined(_WIN32)
		_setmode(_fileno(in), _O_BINARY);
#endif
		CrLfFilter(in, out);
	}

	return 0;
}
