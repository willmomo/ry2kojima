#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>

typedef unsigned char	uchar;
typedef unsigned short	ushort;
typedef unsigned long	ulong;

#include "include/winrek.h"

//---------------------------------------------------------------------
// [概要]
// eireki.datをCSVに変換
//---------------------------------------------------------------------

int do_main(const char* pszFileName)
{
	TOKREK_KIOKU buf;

	FILE* fp = fopen(pszFileName, "rb");
	if (fp) {
		int rno = 1;

		while (1) {
			if (fread(&buf, sizeof(buf), 1, fp) != 1) {
				break;
			}

			printf("%d,%u,\"%d:%02d\"\n", rno++, ntohs(buf.dno), buf.time[0], buf.time[1]);
		}

		fclose(fp);
	}

	return 0;
}

//---------------------------------------------------------------------
// [概要]
// プログラム・スタート
//---------------------------------------------------------------------

int main(int argc, char** argv)
{
	if (argc != 2) {
		puts("使用法：unpack6102 <mf6102>");
		return -1;
	}

	do_main(argv[1]);

	return 0;
}
