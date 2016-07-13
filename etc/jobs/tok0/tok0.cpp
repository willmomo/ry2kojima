
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	unsigned short	dno;				/* 台番号			*/
	unsigned char	time[2];			/* 発生時分			*/
	unsigned char	tokfg;				/* 特賞ﾌﾗｸﾞ			*/
	unsigned char	tcnt;				/* 特賞回数			*/
	unsigned char	t1cnt;				/* 特賞1回数 (T1)		*/
	unsigned char	t2cnt;				/* 特賞2回数 (T2)		*/
	unsigned char	kt1cnt;				/* 確変中T1回数			*/
	unsigned char	infoflg;			/* 情報フラグ			*/
	unsigned short	tout;				/* 特賞ｱｳﾄ			*/
	unsigned short	tsaf;				/* 特賞ｾｰﾌ			*/
	unsigned short	e_bout;				/* 最終特賞間ｱｳﾄ		*/
	unsigned short	e_bsaf;				/* 最終特賞間ｾｰﾌ		*/
	unsigned short	e_bstart;			/* 最終特賞間ｽﾀｰﾄ		*/
	unsigned short	e_bmsa;				/* 最終特賞間最大差(打込)	*/
	unsigned short	e_buri;				/* 最終特賞間売上		*/
	unsigned short	e_motichg;			/* 最終持玉(交換)		*/
	unsigned short	e_motiever;			/* 最終持玉(無定量)		*/
	unsigned char	r_bonus;			/* ﾚｷﾞｭﾗｰ回数(ｽﾛｯﾄのみ)		*/
	unsigned char	kr_bonus;			/* CT中ﾚｷﾞｭﾗｰ回数(ｽﾛｯﾄのみ)	*/
	unsigned short	e_replay;			/* 最終特賞間再ﾌﾟﾚｲ	*/
} TOKREK_KIOKU;

int main(int argc, char** argv)
{
	if (argc != 3) {
		printf("使用法: tok0 元ファイル 保存ファイル\n");
		return 0;
	}

	FILE* fp1 = fopen(argv[1], "rb");
	if (fp1 == NULL) {
		printf("%s がオープンできません。\n", argv[1]);
		return 0;
	}

	FILE* fp2 = fopen(argv[2], "wb");
	if (fp2 == NULL) {
		printf("%s がオープンできません。\n", argv[2]);
		fclose(fp1);
		return 0;
	}

	TOKREK_KIOKU buf;
	while (fread(&buf, sizeof(buf), 1, fp1) == 1) {

		buf.infoflg = 0;

		fwrite(&buf, sizeof(buf), 1, fp2);
	}

	fclose(fp1);
	fclose(fp2);

	return 0;
}
