
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	unsigned short	dno;				/* ��ԍ�			*/
	unsigned char	time[2];			/* ��������			*/
	unsigned char	tokfg;				/* �����׸�			*/
	unsigned char	tcnt;				/* ���܉�			*/
	unsigned char	t1cnt;				/* ����1�� (T1)		*/
	unsigned char	t2cnt;				/* ����2�� (T2)		*/
	unsigned char	kt1cnt;				/* �m�ϒ�T1��			*/
	unsigned char	infoflg;			/* ���t���O			*/
	unsigned short	tout;				/* ���ܱ��			*/
	unsigned short	tsaf;				/* ���ܾ��			*/
	unsigned short	e_bout;				/* �ŏI���܊Ա��		*/
	unsigned short	e_bsaf;				/* �ŏI���܊Ծ��		*/
	unsigned short	e_bstart;			/* �ŏI���܊Խ���		*/
	unsigned short	e_bmsa;				/* �ŏI���܊ԍő卷(�ō�)	*/
	unsigned short	e_buri;				/* �ŏI���܊Ԕ���		*/
	unsigned short	e_motichg;			/* �ŏI����(����)		*/
	unsigned short	e_motiever;			/* �ŏI����(�����)		*/
	unsigned char	r_bonus;			/* ڷޭװ��(�ۯĂ̂�)		*/
	unsigned char	kr_bonus;			/* CT��ڷޭװ��(�ۯĂ̂�)	*/
	unsigned short	e_replay;			/* �ŏI���܊ԍ���ڲ	*/
} TOKREK_KIOKU;

int main(int argc, char** argv)
{
	if (argc != 3) {
		printf("�g�p�@: tok0 ���t�@�C�� �ۑ��t�@�C��\n");
		return 0;
	}

	FILE* fp1 = fopen(argv[1], "rb");
	if (fp1 == NULL) {
		printf("%s ���I�[�v���ł��܂���B\n", argv[1]);
		return 0;
	}

	FILE* fp2 = fopen(argv[2], "wb");
	if (fp2 == NULL) {
		printf("%s ���I�[�v���ł��܂���B\n", argv[2]);
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
