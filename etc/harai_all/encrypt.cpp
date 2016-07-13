/* インクルード */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "encrypt.h"
#include "entable.h"		/* 暗号化テーブルの定義 */

/* 定数宣言 */
#define DATA_SEQUENCE			0			/* シーケンス番号 */
#define DATA_COMMAND_EX			6			/* コマンド文字列 */

/*************************************************************/
/* 	暗号化/暗号化解除										 */
/* 		main関数より受取った値data[]にXORで暗号化する。		 */
/* 		暗号化ﾃｰﾌﾞﾙのセルの値をかける。						 */
/* 															 */
/* 	<入力>	data, n, dai_no		通信電文, 電文長, 台番号	 */
/* 															 */
/* 	<結果>	暗号化/暗号化解除								 */
/* 															 */
/*************************************************************/
void cmd_cnvrt(char *data, int n, unsigned short dai_no)
{
	int offset;		/* オフセット値 */
	char seq, cmd;	/* シーケンス番号、コマンド文字列（２文字目） */
	int i;

	seq = data[DATA_SEQUENCE];			/* シーケンス番号(受信電文ではSTXの分だけ+1する） */
	cmd = data[DATA_COMMAND_EX];			/* コマンド文字列(受信電文ではSTXの分だけ+1する） */
	offset = offset_cal(dai_no, seq, cmd);			/* offset値の算出 */
	
	for(i = 7; i < n - 2; i++){
		data[i] = data[i]^encry(offset + i - 7);	/* 暗号化 */
	}

	return;
}

/*************************************************************/
/* 	台番号、シーケンス番号、コマンド1文字目からoffset値を	 */
/* 	算出する												 */
/* 															 */
/* 			dai_no, seq, cmd	台番号, SEQ, コマンド2文字目 */
/* 		<RET>												 */
/* 			offset				計算結果（offset値）		 */
/* 															 */
/*************************************************************/
int offset_cal(unsigned short dai_no, char seq, char cmd)
{
	int X[2];					/* コマンド(16進）格納用 */
	int Y[4];					/* 台番号格納用（位ごと） */
	int offset;					/* オフセット値 */

	X[0] = (cmd&0xF0)>>4;		/* コマンド2文字目（16進(0xYZ)のY, Zそれぞれを10進変換） */
	X[1] = cmd&0x0F;

	Y[0] = dai_no/1000;				/* 台番号　位ごとの分解　 1000の位 */
	Y[1] = (dai_no/100)%10;									/* 100の位 */
	Y[2] = (dai_no/10)%100%10;								/*  10の位 */
	Y[3] = dai_no%1000%100%10;								/*   1の位 */
	
	offset = atoi(&seq) * X[0] + X[1] + Y[0] + Y[1] + Y[2] + Y[3];		/* オフセット値の算出 */

	if(offset < 96){				/* offsetがテーブルの最後を越えない場合 */
		return(offset);
	}else{							/* テーブルの最後を越える場合 */
		return(offset - 96);		/* 96を差し引く */
	}
}

/*************************************************************/
/* 	暗号化テーブル											 */
/* 		オフセット値からテーブルの暗号化に使用するセル		 */
/* 		の値を返す。										 */
/* 															 */
/* 			offset				オフセット値				 */
/* 		<RET>												 */
/* 			encrypt_tbl			暗号化テーブルのセルの値	 */
/*								（encrypt.h内で定義）		 */
/*************************************************************/
int encry(int offset)
{

	if(offset> 95){						/* 一番最後のセル以上のとき */
		return(encrypt_tbl[offset-96]);	/* 最初のセルに戻り、値を返却 */
	}else{								/* それ以外 */
		return(encrypt_tbl[offset]);	/* セルの値を返却 */
	}
}
