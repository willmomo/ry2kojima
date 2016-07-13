/* �C���N���[�h */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "encrypt.h"
#include "entable.h"		/* �Í����e�[�u���̒�` */

/* �萔�錾 */
#define DATA_SEQUENCE			0			/* �V�[�P���X�ԍ� */
#define DATA_COMMAND_EX			6			/* �R�}���h������ */

/*************************************************************/
/* 	�Í���/�Í�������										 */
/* 		main�֐����������ldata[]��XOR�ňÍ�������B		 */
/* 		�Í���ð��ق̃Z���̒l��������B						 */
/* 															 */
/* 	<����>	data, n, dai_no		�ʐM�d��, �d����, ��ԍ�	 */
/* 															 */
/* 	<����>	�Í���/�Í�������								 */
/* 															 */
/*************************************************************/
void cmd_cnvrt(char *data, int n, unsigned short dai_no)
{
	int offset;		/* �I�t�Z�b�g�l */
	char seq, cmd;	/* �V�[�P���X�ԍ��A�R�}���h������i�Q�����ځj */
	int i;

	seq = data[DATA_SEQUENCE];			/* �V�[�P���X�ԍ�(��M�d���ł�STX�̕�����+1����j */
	cmd = data[DATA_COMMAND_EX];			/* �R�}���h������(��M�d���ł�STX�̕�����+1����j */
	offset = offset_cal(dai_no, seq, cmd);			/* offset�l�̎Z�o */
	
	for(i = 7; i < n - 2; i++){
		data[i] = data[i]^encry(offset + i - 7);	/* �Í��� */
	}

	return;
}

/*************************************************************/
/* 	��ԍ��A�V�[�P���X�ԍ��A�R�}���h1�����ڂ���offset�l��	 */
/* 	�Z�o����												 */
/* 															 */
/* 			dai_no, seq, cmd	��ԍ�, SEQ, �R�}���h2������ */
/* 		<RET>												 */
/* 			offset				�v�Z���ʁioffset�l�j		 */
/* 															 */
/*************************************************************/
int offset_cal(unsigned short dai_no, char seq, char cmd)
{
	int X[2];					/* �R�}���h(16�i�j�i�[�p */
	int Y[4];					/* ��ԍ��i�[�p�i�ʂ��Ɓj */
	int offset;					/* �I�t�Z�b�g�l */

	X[0] = (cmd&0xF0)>>4;		/* �R�}���h2�����ځi16�i(0xYZ)��Y, Z���ꂼ���10�i�ϊ��j */
	X[1] = cmd&0x0F;

	Y[0] = dai_no/1000;				/* ��ԍ��@�ʂ��Ƃ̕����@ 1000�̈� */
	Y[1] = (dai_no/100)%10;									/* 100�̈� */
	Y[2] = (dai_no/10)%100%10;								/*  10�̈� */
	Y[3] = dai_no%1000%100%10;								/*   1�̈� */
	
	offset = atoi(&seq) * X[0] + X[1] + Y[0] + Y[1] + Y[2] + Y[3];		/* �I�t�Z�b�g�l�̎Z�o */

	if(offset < 96){				/* offset���e�[�u���̍Ō���z���Ȃ��ꍇ */
		return(offset);
	}else{							/* �e�[�u���̍Ō���z����ꍇ */
		return(offset - 96);		/* 96���������� */
	}
}

/*************************************************************/
/* 	�Í����e�[�u��											 */
/* 		�I�t�Z�b�g�l����e�[�u���̈Í����Ɏg�p����Z��		 */
/* 		�̒l��Ԃ��B										 */
/* 															 */
/* 			offset				�I�t�Z�b�g�l				 */
/* 		<RET>												 */
/* 			encrypt_tbl			�Í����e�[�u���̃Z���̒l	 */
/*								�iencrypt.h���Œ�`�j		 */
/*************************************************************/
int encry(int offset)
{

	if(offset> 95){						/* ��ԍŌ�̃Z���ȏ�̂Ƃ� */
		return(encrypt_tbl[offset-96]);	/* �ŏ��̃Z���ɖ߂�A�l��ԋp */
	}else{								/* ����ȊO */
		return(encrypt_tbl[offset]);	/* �Z���̒l��ԋp */
	}
}
