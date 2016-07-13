#ifndef __halconv_h__
#define __halconv_h__

//DLL�֐��̃I�v�V����
#define HC_OPT_BODY 0
#define HC_OPT_HEAD 1

//�G���[�萔
#define HALCONVERR_WRITE 0x1002		//�������݃G���[

//�t�@�C���ԍ��萔
#define R_SYSTEM_IJO	4006
#define R_CLEAR			4007
#define R_TIMESET		4008
#define R_TEIDEN		4009
#define R_SETTEI		4010
#define R_IJO			4022
#define R_UTIDOME		4023
#define R_HASSEI		4024
#define I_KIOKU			4101
#define F_DAI			4102
#define F_ZENTEN		4103
#define F_SHIMA			4104
#define F_KISHU			4105
#define F_SHUBETSU		4106
#define F_TOTAL			4107
#define F_KINKO			4108
#define I_TEIJI			4109
#define F_T_SHUBETSU	4110
#define F_T_ZENTEN		4111
#define I_T_KADO		4112
#define F_T_SBT_KADO	4113
#define F_T_ZEN_KADO	4114
#define F_T_KISHU_KADO	4115
#define F_T_DSP_DATA	4117

//�֐����萔
const char FuncNames[2][12] = { "hc_raw2vb()", "hc_vb2raw()" };

//�G���[���b�Z�[�W�e�[�u��
const char MemFiles[23][27] = {
	"�V�X�e���ُ헚��", "�N���A����", "���v�ݒ藚��", "��d����", "�ݒ藚��", "�ُ헚��",
	"�Ŏ~����", "��������", "�����E�L�����", "�����E��t�@�C��", "�����E�S�X�t�@�C��", "�����E���t�@�C��",
	"�����E�@��t�@�C��", "�����E��ʏW�v", "�����E�S�X�W�v�t�@�C��", "�����E���ɏW�v", "�莞���", "�����E�莞��ʏW�v",
	"�����E�莞�S�X�W�v", "�����E�莞�ғ����", "�����E�莞��ʉғ��t�@�C��", "�����E�莞�S�X�ғ��t�@�C��", "�����E�莞�@��ғ��t�@�C��"
};



/*	AS�p�\����	*/

/****************************************/
/*	���шُ헚��(4006)					*/
/****************************************/
typedef struct {					/* ͯ��ں���			*/
	unsigned short	cnt;			/* ���шُ�\���X�V��		*/
	unsigned short	dmy[3];
} REKI_SYSIJO_HED;

typedef struct {					/* �ް�ں���			*/
	unsigned long	sysijyo_pw_tim;		/* �N�����j������		*/
	unsigned char 	code1;			/* ��������1			*/
	unsigned char 	code2;			/* ��������2			*/
	unsigned short	no;				/* �ԍ�(CH)			*/
} REKI_SYSIJO;


/****************************************/
/*	�ر����(4007)						*/
/****************************************/
typedef struct {					/* ͯ��ں���			*/
	unsigned long	clr_pw_tim;		/* �ر����			*/
	unsigned char 	code1;			/* ��������1			*/
	unsigned char 	code2;			/* ��������2			*/
	unsigned short	dmy[3];			/* yobi */
	unsigned long	urikin;			/* �ر���̔�����z(100�~�P��	*/
} REKI_CLEAR;

/****************************************/
/*	���v�ݒ藚��(4008)					*/
/****************************************/
typedef struct {					/* �ް�ں���			*/
	unsigned long	mae_pw_tim;		/* �O  �N�����j������		*/
	unsigned char 	code1;			/* ��������1			*/
	unsigned char 	code2;			/* ��������2			*/
	unsigned short	dmy[3];			/* yobi */
	unsigned long	set_pw_tim;		/* �ݒ�N�����j������		*/
} REKI_TIMSET;

/****************************************/
/*	��d����(4009)						*/
/****************************************/
typedef struct {					/* �ް�ں���			*/
	unsigned long	pwon_pw_tim;	/* �n�m�N�����j������	*/
	unsigned char 	code1;			/* ��������1			*/
	unsigned char 	code2;			/* ��������2			*/
	unsigned short	dmy[2];
	unsigned char 	eflg;			/* ��d�׸� (1)�c�ƒ� (0)�X��	*/
	unsigned char 	dmy1;			/*				*/
	unsigned long	pwoff_pw_tim;	/* ��d�N�����j������		*/
} REKI_TEIDEN;

/****************************************/
/*	�ݒ藚��(4010)						*/
/****************************************/
typedef struct {					/* �ް�ں���			*/
	unsigned long	pw_tim;			/* �ݒ�N�����j������	*/
	unsigned char 	hcd1;			/* ��������1			*/
	unsigned char 	hcd2;			/* ��������2			*/
	unsigned short	set_fno[5];		/* �ݒ�̧�ٔԍ�			*/
} REKI_SETTEI;


/****************************************/
/*	�ُ헚��(4022)						*/
/****************************************/
typedef struct {					/* ͯ��ں���			*/
	unsigned short	cnt;			/* �ُ�\���X�V��		*/
	unsigned short	hasei;			/* �ُ픭����			*/
	unsigned short	suu;			/* �ُ풆����			*/
	unsigned short	dmy[5];			/* yobi	*/
} REKI_IJO_HED;

typedef struct {					/* �ް�ں���			*/
	unsigned char 	code1;			/* ��������1			*/
	unsigned char 	code2;			/* ��������2			*/
	unsigned char 	htime[2];		/* ��������			*/
	unsigned short	no;				/* �ԍ�(CH)	(0:����ں���)	*/
	unsigned char 	jflg[2];		/* ����׸�			*/
	unsigned short	tflg;			/* ��~�׸�			*/
	unsigned short	cnt;			/* �񐔥�l			*/
	unsigned char 	dmy[2];
	unsigned char 	ftime[2];		/* ��������			*/
} REKI_IJO;

/****************************************/
/*	�Ŏ~����(4023)						*/
/****************************************/
typedef struct {					/* ͯ��ں���			*/
	unsigned short	cnt;			/* �Ŏ~�\���X�V��		*/
	unsigned short	hasei;			/* �Ŏ~������			*/
	unsigned short	suu;			/* �Ŏ~���䐔			*/
	unsigned short	dmy[3];
} REKI_UTI_HED;

typedef struct {					/* �ް�ں���			*/
	unsigned char 	code1;			/* ��������1			*/
	unsigned char 	code2;			/* ��������2			*/
	unsigned char 	time[2];		/* ��������			*/
	unsigned short	no;				/* �ԍ�		(0:����ں���)	*/
	unsigned char 	jflg[2];		/* ����׸�			*/
	unsigned short	tflg;			/* ��~�׸�			*/
	unsigned short	cnt;			/* �񐔥�l			*/
} REKI_UTI;

/****************************************/
/*	��������(4024)			*/
/****************************************/
typedef struct {			/* ͯ��ں���			*/
	unsigned short	cnt;			/* ��������\���X�V��		*/
	unsigned short	wpos;			/* ���������������߲��		*/
	unsigned short	dmy[4];
} REKI_HASSEI_HED;

typedef struct {			/* �ް�ں���			*/
	unsigned char 	code1;			/* ��������1			*/
	unsigned char 	code2;			/* ��������2			*/
	unsigned char 	time[2];		/* ��������			*/
	unsigned short	no;			/* �ԍ�		(0:����ں���)	*/
	unsigned char 	jflg[2];		/* ����׸�			*/
	unsigned short	tflg;			/* ��~�׸�			*/
	unsigned short	cnt;			/* �񐔥�l			*/
} REKI_HASSEI;

/*#**************************************/
/*	�L�����(4101)			*/
/****************************************/
typedef struct {
	USHORT	sync;			/* �ݸ����	0x87cd			*/
	UCHAR	kioku_index;	/* �L����ۯ�.no				*/
	UCHAR	kioku_fg;		/* �L���׸�	(b7=1)�g�p��	*/
							/*		(b6=1)�ر		*/
							/*		(b1=1)���		*/
							/*		(b0=1)CRC�װ		*/
	UCHAR	emark;			/* �c��ϰ�	(b7)�ر��		*/
							/*		(b6)�J�X��		*/
							/*		(b5)�X��		*/
							/*		(b4)xxx			*/
							/*		(b3)�L����		*/
							/*		�ر��=0x80		*/
	UCHAR	dmy1[3];
	ULONG	down_pw_tim;		/* ��d����				*/
	ULONG	pwon_pw_tim;		/* �d��ON����				*/
	ULONG	clear_pw_tim;		/* �ر����				*/
	ULONG	open_pw_tim;		/* �J�X����	(���ώ�:�J�n���t)	*/
	ULONG	close_pw_tim;		/* �X����	(���ώ�:�ŏI���t)	*/
	ULONG	sime_pw_tim;		/* ���؎���				*/
	ULONG	kioku_pw_tim;		/* �L������				*/
	USHORT	eigyou_min_cnt;		/* �c�Ǝ���		�ر��J�X��=0	*/
	UCHAR	clear_cnt;			/* �ر��		�L����̍ŏ��̸ر�� =1	*/
	UCHAR	clear_cnt_0;		/* �ر��(�J�X�O)	�L����̍ŏ��̸ر�� =1	*/
	UCHAR	clear_cnt_1;		/* �ر��(�J�X-�L��)	�L����̍ŏ��̸ر�� =1	*/
	UCHAR	down_cnt;			/* ��d��		�ر��=0		*/
	UCHAR	down_cnt_1;			/* ��d��(�ر-�L��)	�ر��=0		*/
	UCHAR	down_cnt_2;			/* ��d��(�L��-)	�ر��=0		*/
	UCHAR	kioku_cnt;			/* �L����		�ر��̍ŏ��̋L���� =1	*/
	UCHAR	dmy2[3];
	ULONG	settei_ps_tim;		/* �ݒ�X�V����				*/
	ULONG	now_pw_tim;			/* ���ݎ��� (�莞��������) ��d�����쐬	*/
	UCHAR	dmy3[8];
	struct TENPO_KIOKU_JYOHO {			/* AS(SH3)�X�܋L�����	*/
		unsigned char	t_mark;			/* 00:�X��:(=$41)A�X,(=$42)B�X	*/
		unsigned char	t_holiday;		/* 01:�c�Ɠ�FLAG (bit7=1)�J�X��,(bit7=0)�x�X��	*/
		unsigned short	t_eday_md;		/* 02:�c�Ɠ��t�Fbit15-8=��,bit7-0=��	*/
		unsigned short	t_clear_hm;		/* 04:�N���A�����Fbit15-8=��,bit7-0=��	*/
		unsigned short	t_open_hm;		/* 06:�J�X�����Fbit15-8=��,bit7-0=��	*/
		unsigned short	t_clos_hm;		/* 08:�X�����Fbit15-8=��,bit7-0=��	*/ 
		unsigned short	t_sime_hm;		/* 0a:���؎����Fbit15-8=��,bit7-0=��	*/
		unsigned short	t_eigyo_m;		/* 0c:�c�Ǝ��ԁF�ݐϕ�					*/
		unsigned char	t_dmy2[2];		/* 0e:	*/
	} tenpo_kioku_jyoho[2];				/* 10:	*/
	unsigned char	dmy4[160];			/* 160: */

} INF_KIOKU;

/*#**************************************/
/*	���ް�(�݌v)(4102)			*/
/****************************************/
typedef	struct {
	USHORT	jflg;			/* �����׸�			*/
							/*    0x0001 : �����ް�		*/
							/*    0x0002 : �ް��ݒ�		*/
							/*    0x0004 : �W�v��~		*/
							/*    0x0008 : �ғ���		*/
							/*    0x0010 : ����ғ���	*/
	USHORT	out;			/* ���				*/
	USHORT	saf;			/* ���				*/
	USHORT	start;			/* ����				*/
	USHORT	tout;			/* ���ܱ��			*/
	USHORT	tsaf;			/* ���ܾ��			*/
	USHORT	t2out;			/* ����2���			*/
	USHORT	t2saf;			/* ����2���			*/
	USHORT	t2start;		/* ����2����			*/
	USHORT	bcnt;			/* �ް���(���܉�/�Ŏ~��)	*/
	USHORT	ucnt;			/* �Ŏ~��			*/
	USHORT	tcnt;			/* ���܉�  (T)		*/
	USHORT	t1cnt;			/* ����1�� (T1)		*/
	USHORT	t2cnt;			/* ����2�� (T2)		*/
	USHORT	kcnt;			/* �m�ω�((T2)>T1>T2>T1���)	*/
	USHORT	kt1cnt;			/* �m�ϒ�T1��			*/

	USHORT	lcnt;			/* ׯ����			*/
	USHORT	lt1cnt;			/* ׯ��������(T1)��		*/
	USHORT	lt1max;			/* ׯ����MAX����(T1)��	*/
	USHORT	lout;			/* ׯ�������			*/
	USHORT	lsaf;			/* ׯ�������			*/
	USHORT	lbout;			/* ׯ������(T1)�Ա��		*/
	USHORT	lmoti;			/* ׯ��������			*/

	USHORT	start1;			/* ����1			*/
	USHORT	start2;			/* ����2			*/
	USHORT	start3;			/* ����3			*/
	USHORT	t2start1;		/* ����2(T2)������1		*/
	USHORT	t2start2;		/* ����2(T2)������2		*/
	USHORT	t2start3;		/* ����2(T2)������3		*/
	USHORT	t1start2;		/* ���ܽ���2��		*/

	USHORT	ukomi;			/* �ō���			*/
	USHORT	t1ukomi;		/* ���ܑō�			*/
	short	minsa;			/* �ŏ���			*/
	short	maxsa;			/* �ő卷			*/
	USHORT	minout;			/* �ŏ������			*/
	USHORT	maxout;			/* �ő卷���			*/
	USHORT	maxmoti;		/* �ő厝�� MY			*/
	USHORT	maxukomi;		/* �ő�ō� M��			*/

	USHORT	kadofg;			/* �莞�ғ��׸�(8-23)		*/
	UCHAR	kado;			/* �ωғ���			*/
	UCHAR	kyaku;			/* �q���			*/

	USHORT	hasfg;			/* �������׸�			*/
						/*    0x0001 : ��s��		*/
						/*    0x0002 : ��f��		*/
						/*    0x0004 : ��ُ�		*/
						/*    0x0008 : ��_��		*/
						/*    0x0100 : ����s��		*/
						/*    0x0200 : ����f��		*/
						/*    0x0400 : ����ُ�		*/
						/*    0x0800 : ����_��		*/
	UCHAR	door1_2;	/* �ޱ1��		0xf0	*/
						/* �ޱ2��		0x0f	*/
	UCHAR	call1_2;	/* �ďo1��		0xf0	*/
						/* �ďo2��		0x0f	*/
	UCHAR	fus_ddan;	/* �s����		0xf0	*/
						/* ��f����		0x0f	*/
	UCHAR	ijo_ten;	/* �ُ��		0xf0	*/
						/* �_����		0x0f	*/
	UCHAR	g1_g2dan;	/* ����1�f����	0xf0	*/
						/* ����2�f����	0x0f	*/
	UCHAR	cddan_uijo;		/* ���ޒf����		0xf0	*/
							/* ����ُ��		0x0f	*/

	UCHAR	ukind;			/* �������׸�			*/
						/*     0x01 : ���ތ���1�L��	*/
						/*     0x02 : ���ތ���2�L��	*/
						/*     0x04 : ���ޔ���L��	*/
						/*     0x10 : ���ތ���1����	*/
						/*     0x20 : ���ތ���2����	*/
	UCHAR	dmy1;			/* ....				*/
	USHORT	gen1uri;		/* ���ތ���1����(100�~)		*/
	USHORT	gen2uri;		/* ���ތ���2����(100�~)		*/
	USHORT	carduri;		/* ���޶��ޔ���(100�~)		*/
	USHORT	daiuri;			/* �䔄��(100�~)		*/
	USHORT	hosei;			/* ����␳�l(�~)		*/

	UCHAR	k_suu;			/* �����			*/
	UCHAR	k_kado;			/* ����ғ���			*/
	USHORT	k_kadofg;		/* ����莞�ғ��׸�(8-23)	*/
	UCHAR	k_t1cnt;		/* �������1�� (T1)		*/
	UCHAR	k_tcnt;			/* ������܉�	 (T)		*/
	UCHAR	k_kcnt;			/* ����m�ω�			*/
	UCHAR	k_lcnt;			/* ���ׯ����			*/
	USHORT	k_out;			/* ������			*/
	USHORT	k_saf;			/* ������			*/
	USHORT	k_tout;			/* ������ܱ��			*/
	USHORT	k_tsaf;			/* ������ܾ��			*/
	USHORT	k_start;		/* �������			*/
	USHORT	k_uri;			/* �������(100�~)		*/
	USHORT	k_repkos;		/* �����ڲ��			*/
	USHORT	k_reptesu;		/* �����ڲ�萔��(��)		*/

	USHORT	r_bonus;		/* ڷޭװ��(�ۯĂ̂�)		*/

	USHORT	toutmax;		/* ���ܒ����(�ő�l)		*/
	USHORT	toutmin;		/* ���ܒ����(�ŏ��l)		*/
	short	tsamax;			/* ���ܒ���(�ő�l)		*/
	short	tsamin;			/* ���ܒ���(�ŏ��l)		*/
	USHORT	boutmax;		/* ���܊Ա��(�ő�l)		*/
	USHORT	boutmin;		/* ���܊Ա��(�ŏ��l)		*/
	short	bsamax;			/* ���܊ԍ�(�ő�l)		*/
	short	bsamin;			/* ���܊ԍ�(�ŏ��l)		*/
	USHORT	bstartmax;		/* ���܊Խ���(�ő�l)		*/
	USHORT	bstartmin;		/* ���܊Խ���(�ŏ��l)		*/
	short	bmsamax;		/* ���܊ԍő卷(�ő�l)		*/
	short	bmsamin;		/* ���܊ԍő卷(�ŏ��l)		*/
	short	tmotimax;		/* ���܎���(�ő�l)		*/
	short	tmotimin;		/* ���܎���(�ŏ��l)		*/

	USHORT	kr_bonus;		/* CT��ڷޭװ��(�ۯĂ̂�)		*/

	UCHAR	tflg;			/* �����׸�			*/
							/*	0x01 : T1(�ޯ��)		*/
							/*	0x02 : T2(CT) 			*/
							/*	0x04 : ׯ�-				*/
							/*	0x08 : ڷޭװ(S�̂�)	*/
	UCHAR	dmy2;			/* ....				*/
	UCHAR	e_hour;			/* �ŏI����(��)			*/
	UCHAR	e_min;			/* �ŏI����(��)			*/
	USHORT	e_t1out;		/* �ŏI���܂P�����		*/
	USHORT	e_t1saf;		/* �ŏI���܂P�����		*/
	USHORT	e_bout;			/* �ŏI���܂P�Ա��		*/
	USHORT	e_bsaf;			/* �ŏI���܂P�Ծ��		*/
	USHORT	e_bstart;		/* �ŏI���܂P�Խ���		*/
	USHORT	e_bmsa;			/* �ŏI���܂P�ԍő卷(�ō�)	*/
	USHORT	e_buri;			/* �ŏI���܂P�Ԕ���		*/
	USHORT	e_motichg;		/* �ŏI����(����)		*/
	USHORT	e_motiever;		/* �ŏI����(�����)		*/
	UCHAR	yobi2[2];		/* ....				*/
	USHORT	san_ukos;		/* ���ޔ����			*/
	USHORT	chk_dat;		/* CHK �ް�			*/
	ULONG	k_keisu;		/* ��i�i��		*/
	ULONG	k_cyokos;		/* �䒙�ʌ�			*/
} FDATA_DAI;

/*#**************************************/
/*	�S�X�ް�(����)(4103)			*/
/****************************************/
typedef	struct {
	long	indata;			/* �v���l			*/
	char	dancnt;			/* �f����			*/
	char	err_door;		/* �װ��  0xf0		*/
							/* �ޱ��  0x0f		*/
	char	othercnt;		/* ���̑���			*/
	char	flg;			/* �׸�				*/
} FDATA_SMCON;

/*#**************************************/
/*	����@���ް�(4104,4105)			*/
/****************************************/
typedef	struct {
	USHORT	daisu;			/* �䐔				*/
	USHORT	jflg;			/* �����׸�(���ް��Q��)	*/
	ULONG	out;			/* ���				*/
	ULONG	saf;			/* ���				*/
	ULONG	start;			/* ����				*/
	ULONG	tout;			/* ���ܱ��			*/
	ULONG	tsaf;			/* ���ܾ��			*/
	ULONG	t2out;			/* ����2���			*/
	ULONG	t2saf;			/* ����2���			*/
	ULONG	t2start;		/* ����2����			*/
	USHORT	bcnt;			/* �ް���(���܉�/�Ŏ~��)	*/
	USHORT	ucnt;			/* �Ŏ~��			*/
	USHORT	tcnt;			/* ���܉�  (T)		*/
	USHORT	t1cnt;			/* ����1�� (T1)		*/
	USHORT	t2cnt;			/* ����2�� (T2)		*/
	USHORT	kcnt;			/* �m�ω�((T2)>T1>T2>T1��)	*/
	USHORT	kt1cnt;			/* �m�ϒ�T1��			*/

	USHORT	lcnt;			/* ׯ����			*/
	USHORT	lt1cnt;			/* ׯ����T1��			*/
	USHORT	lt1max;			/* ׯ����MAXT1��		*/
	ULONG	lout;			/* ׯ�������			*/
	ULONG	lsaf;			/* ׯ�������			*/
	ULONG	lbout;			/* ׯ��T1�Ա��			*/
	ULONG	lmoti;			/* ׯ��������			*/

	ULONG	start1;			/* ����1			*/
	ULONG	start2;			/* ����2			*/
	ULONG	start3;			/* ����3			*/
	ULONG	t2start1;		/* ����2(T2)������1		*/
	ULONG	t2start2;		/* ����2(T2)������2		*/
	ULONG	t2start3;		/* ����2(T2)������3		*/
	ULONG	t1start2;		/* ���ܽ���2��		*/

	ULONG	ukomi;			/* �ō���			*/
	ULONG	t1ukomi;		/* ���ܑō�			*/
	ULONG	maxmoti;		/* �ő厝�� MY			*/
	ULONG	maxukomi;		/* �ő�ō� M��			*/

	USHORT	kado;			/* �ωғ���			*/
	USHORT	kyaku;			/* �q���			*/

	USHORT	hasfg;			/* �������׸�			*/
	USHORT	door1;			/* �ޱ1��			*/
	USHORT	door2;			/* �ޱ2��			*/
	USHORT	call1;			/* �ďo1��			*/
	USHORT	call2;			/* �ďo2��			*/
	UCHAR	fuscnt;			/* �s����			*/
	UCHAR	ddancnt;		/* ��f����			*/
	UCHAR	ijocnt;			/* �ُ��			*/
	UCHAR	tencnt;			/* �_����			*/
	UCHAR	g1dancnt;		/* ����1�f����		*/
	UCHAR	g2dancnt;		/* ����2�f����		*/
	UCHAR	cddancnt;		/* ���ޒf����			*/
	UCHAR	uijocnt;		/* ����ُ��			*/

	UCHAR	ukind;			/* �������׸�(���ް��Q��)	*/
	UCHAR	dmy1;			/* ....				*/
	ULONG	gen1uri;		/* ���ތ���1����(100�~)		*/
	ULONG	gen2uri;		/* ���ތ���2����(100�~)		*/
	ULONG	carduri;		/* ���޶��ޔ���(100�~)		*/
	ULONG	daiuri;			/* �䔄��(100�~)		*/
	ULONG	hosei;			/* ����␳�l(�~)		*/

	USHORT	k_suu;			/* �����			*/
	USHORT	k_kado;			/* ����ғ���			*/
	USHORT	k_t1cnt;		/* �������1�� (T1)		*/
	USHORT	k_tcnt;			/* ������܉�	 (T)		*/
	USHORT	k_kcnt;			/* ����m�ω�			*/
	USHORT	k_lcnt;			/* ���ׯ����			*/
	ULONG	k_out;			/* ������			*/
	ULONG	k_saf;			/* ������			*/
	ULONG	k_tout;			/* ������ܱ��			*/
	ULONG	k_tsaf;			/* ������ܾ��			*/
	ULONG	k_start;		/* �������			*/
	ULONG	k_uri;			/* �������(100�~)		*/
	ULONG	k_repkos;		/* �����ڲ��			*/
	ULONG	k_repkin;		/* �����ڲ���z(�~)		*/
	ULONG	k_reptesu;		/* �����ڲ�萔��(��)		*/
	ULONG	k_reptesukin;		/* �����ڲ�萔��(���z)(�~)	*/
	ULONG	k_cyokos;		/* �䒙�ʌ�			*/
	ULONG	k_cyokin;		/* �䒙�ʋ��z			*/
	ULONG	k_keisu;		/* ��i�i��		*/

	USHORT	r_bonus;		/* ڷޭװ��(�ۯĂ̂�)		*/
	USHORT	kr_bonus;		/* CT��ڷޭװ��(�ۯĂ̂�)	*/

	USHORT	g_kado;			/* ���ғ��䐔		*/
	USHORT	g_k_kado;		/* ������ғ��䐔	*/

	UCHAR	yobi[24];		/* ....				*/
} FDATA_SK;

/*#**************************************/
/*	��ʏW�v��۱�ް�(4106,4110)		*/
/****************************************/
typedef	struct {
	USHORT	daisu;			/* �䐔				*/
	USHORT	jflg;			/* �����׸�(���ް��Q��)	*/
	ULONG	out;			/* ���				*/
	ULONG	saf;			/* ���				*/
	ULONG	start;			/* ����				*/
	ULONG	tout;			/* ���ܱ��			*/
	ULONG	tsaf;			/* ���ܾ��			*/
	ULONG	t2out;			/* ����2���			*/
	ULONG	t2saf;			/* ����2���			*/
	ULONG	t2start;		/* ����2����			*/
	USHORT	bcnt;			/* �ް���(���܉�/�Ŏ~��)	*/
	USHORT	ucnt;			/* �Ŏ~��			*/
	USHORT	tcnt;			/* ���܉�  (T)		*/
	USHORT	t1cnt;			/* ����1�� (T1)		*/
	USHORT	t2cnt;			/* ����2�� (T2)		*/
	USHORT	kcnt;			/* �m�ω�((T2)>T1>T2>T1��)	*/
	USHORT	kt1cnt;			/* �m�ϒ�T1��			*/

	USHORT	lcnt;			/* ׯ����			*/
	USHORT	lt1cnt;			/* ׯ����T1��			*/
	USHORT	lt1max;			/* ׯ����MAXT1��		*/
	ULONG	lout;			/* ׯ�������			*/
	ULONG	lsaf;			/* ׯ�������			*/
	ULONG	lbout;			/* ׯ��T1�Ա��			*/
	ULONG	lmoti;			/* ׯ��������			*/

	ULONG	ukomi;			/* �ō���			*/
	ULONG	t1ukomi;		/* ���ܑō�			*/

	USHORT	kado;			/* �ωғ���			*/
	USHORT	kyaku;			/* �q���			*/

	USHORT	hasfg;			/* �������׸�			*/
	USHORT	door1;			/* �ޱ1��			*/
	USHORT	door2;			/* �ޱ2��			*/
	USHORT	call1;			/* �ďo1��			*/
	USHORT	call2;			/* �ďo2��			*/
	UCHAR	fuscnt;			/* �s����			*/
	UCHAR	ddancnt;		/* ��f����			*/
	UCHAR	ijocnt;			/* �ُ��			*/
	UCHAR	tencnt;			/* �_����			*/
	UCHAR	g1dancnt;		/* ����1�f����		*/
	UCHAR	g2dancnt;		/* ����2�f����		*/
	UCHAR	cddancnt;		/* ���ޒf����			*/
	UCHAR	uijocnt;		/* ����ُ��			*/

	UCHAR	ukind;			/* �������׸�(���ް��Q��)	*/
	UCHAR	dmy1;			/* ....				*/
	ULONG	gen1uri;		/* ���ތ���1����(100�~)		*/
	ULONG	gen2uri;		/* ���ތ���2����(100�~)		*/
	ULONG	carduri;		/* ���޶��ޔ���(100�~)		*/
	ULONG	daiuri;			/* �䔄��(100�~)		*/
	ULONG	hosei;			/* ����␳�l(�~)		*/

	USHORT	k_suu;			/* �����			*/
	USHORT	k_kado;			/* ����ғ���			*/
	USHORT	k_t1cnt;		/* �������1�� (T1)		*/
	USHORT	k_tcnt;			/* ������܉�	 (T)		*/
	USHORT	k_kcnt;			/* ����m�ω�			*/
	USHORT	k_lcnt;			/* ���ׯ����			*/
	ULONG	k_out;			/* ������			*/
	ULONG	k_saf;			/* ������			*/
	ULONG	k_tout;			/* ������ܱ��			*/
	ULONG	k_tsaf;			/* ������ܾ��			*/
	ULONG	k_start;		/* �������			*/
	ULONG	k_uri;			/* �������(100�~)		*/
	ULONG	k_repkos;		/* �����ڲ��			*/
	ULONG	k_reptesu;		/* �����ڲ�萔��(��)		*/
	ULONG	k_cyokos;		/* �䒙�ʌ�			*/

	ULONG	sg_danflg;		/* ���O�f�����׸�		*/
							/*  0x00000001 : ���ɒf��	*/
							/*  0x00000002 : �i�i�f��	*/
							/*  0x00000004 : ��������f��	*/
							/*  0x00000008 : ���ޔ���f��	*/
							/*  0x00000010 : ���ڲ�f��	*/
							/*  0x00000020 : ���бђf��	*/
							/*  0x00000040 : ���ʒf��	*/
							/*  0x00000080 : ���ޔ��s�f��	*/
							/*  0x00000200 : POS�f��	*/
							/*  0x00000400 : ���̋@		*/
							/*  0x00000800 : ���̑��f��	*/
	ULONG	urikin;			/* ������z(���v)(100�~)	*/
	ULONG	t_keikos;		/* �i�i��(���v)		*/
	ULONG	t_g_keikos;		/* �����i�i��			*/
	ULONG	sg_gen1uri;		/* ���O����1����(100�~)		*/
	ULONG	sg_gen2uri;		/* ���O����2����(100�~)		*/
	ULONG	sg_carduri;		/* ���O���ޔ���(100�~)		*/
	ULONG	sg_repkos;		/* ���O���ڲ��		*/
	ULONG	sg_reptesu;		/* ���O���ڲ�萔��(��)	*/
	ULONG	sg_premkos;		/* ���O���бь�		*/
	ULONG	sg_cyokos;		/* ���O���ʌ�			*/
	ULONG	kinko;			/* ���ɍ��v			*/
	ULONG	sand;			/* ���ތ����v			*/
	ULONG	sandkinko;		/* ���ދ��Ɍv			*/
	ULONG	poskos;			/* POS��			*/
	ULONG	g_poskos;		/* ����POS��			*/
	ULONG	k_keisu;		/* ��i�i��		*/

	USHORT	r_bonus;		/* ڷޭװ��(�ۯĂ̂�)		*/
	USHORT	kr_bonus;		/* CT��ڷޭװ��(�ۯĂ̂�)	*/

	USHORT	g_kado;			/* ���ғ��䐔		*/
	USHORT	g_k_kado;		/* ������ғ��䐔	*/

	UCHAR	yobi[8];		/* ....				*/
} FDATA_SBT;

/*#**************************************/
/*	�S�X�W�v(4107,4111)			*/
/****************************************/
typedef	struct {
	USHORT	daisu;			/* �䐔				*/
	USHORT	jflg;			/* �����׸�(���ް��Q��)	*/

	USHORT	kado;			/* �ωғ���			*/

	USHORT	hasfg;			/* �������׸�			*/

	UCHAR	g1dancnt;		/* ����1�f����		*/
	UCHAR	g2dancnt;		/* ����2�f����		*/
	UCHAR	cddancnt;		/* ���ޒf����			*/
	UCHAR	uijocnt;		/* ����ُ��			*/

	UCHAR	ukind;			/* �������׸�(���ް��Q��)	*/
	UCHAR	dmy1[3];		/* ....				*/
	ULONG	gen1uri;		/* ���ތ���1����(100�~)		*/
	ULONG	gen2uri;		/* ���ތ���2����(100�~)		*/
	ULONG	carduri;		/* ���޶��ޔ���(100�~)		*/
	ULONG	daiuri;			/* �䔄��(100�~)		*/
	ULONG	hosei;			/* ����␳�l(�~)		*/

	USHORT	k_suu;			/* �����			*/
	USHORT	k_kado;			/* ����ғ���			*/
	ULONG	k_uri;			/* �������(100�~)		*/
	ULONG	k_repkin;		/* �����ڲ���z(�~)		*/
	ULONG	k_reptesukin;		/* �����ڲ�萔��(���z)(�~)	*/
	ULONG	k_cyokin;		/* �䒙�ʋ��z(�~)		*/

	ULONG	sg_danflg;		/* ���O�f�����׸�(��ʏW�v�Q��)	*/
	ULONG	urikin;			/* ������z(���v)(100�~)	*/
	ULONG	t_keikin;		/* �i�i���z(���v)(�~)		*/
	ULONG	t_g_keikin;		/* �����i�i���z(�~)		*/
	ULONG	sg_gen1uri;		/* ���O����1����(100�~)		*/
	ULONG	sg_gen2uri;		/* ���O����2����(100�~)		*/
	ULONG	sg_carduri;		/* ���O���ޔ���(100�~)		*/
	ULONG	sg_repkin;		/* ���O���ڲ���z(�~)		*/
	ULONG	sg_reptesukin;		/* ���O���ڲ�萔��(���z)(�~)	*/
	ULONG	sg_premkin;		/* ���O���бы��z(�~)		*/
	ULONG	sg_cyokin;		/* ���O���ʋ��z(�~)		*/
	ULONG	kinko;			/* ���ɍ��v			*/
	ULONG	sand;			/* ���ތ����v			*/
	ULONG	sandkinko;		/* ���ދ��Ɍv			*/
	ULONG	poskin;			/* POS���z(�~)			*/
	ULONG	g_poskin;		/* ����POS���z(�~)		*/

	USHORT	cd_hfg;			/* ���ޔ��s�׸�			*/
	USHORT	cd_hdanfg;		/* ���ޔ��s�f���׸�		*/
	ULONG	cd_htotal;		/* ���ޔ��s���v���z(100�~)	*/
	ULONG	cd_h1kei;		/* ���ޔ��s1000�~�v(100�~)	*/
	ULONG	cd_h2kei;		/* ���ޔ��s2000�~�v(100�~)	*/
	ULONG	cd_h3kei;		/* ���ޔ��s3000�~�v(100�~)	*/
	ULONG	cd_h5kei;		/* ���ޔ��s5000�~�v(100�~)	*/
	ULONG	cd_h10kei;		/* ���ޔ��s10000�~�v(100�~)	*/

	ULONG	zihan;			/* ���̋@�v(100�~)		*/
	ULONG	ta;				/* ���̑��v			*/

	long	d_keikin;		/* ��i�i���z(100�~)		*/
	long	g_d_keikin;		/* ������i�i���z(100�~)	*/
	long	hd_keikin;		/* �␳��i�i���z(100�~)	*/
	long	g_hd_keikin;		/* �␳������i�i���z(100�~)	*/
	long	k_keikin;		/* ��i�i���z(���ʥ���v���C)		*/
#if 1	/*	�䔄��ł̑e���v�v�Z�ׁ̈A�䔄�ォ��̌i�i���z�ǉ�	*/
//	long	sa_kin;			/* �䔄�ォ��̌i�i���z(�~)		*/	
	long	g_duri_keikin;	/* �䔄�ォ��̌�����i�i���z(�~)	*/

	USHORT	g_kado;			/* ���ғ��䐔		*/
	USHORT	g_k_kado;		/* ������ғ��䐔	*/

	UCHAR	yobi[72];		/* ....				*/
#else
	UCHAR	yobi[80];		/* ....				*/
#endif
} FDATA_TOTAL;

/*#**************************************/
/*	���ɏW�v(4108)						*/
/****************************************/
typedef	struct {
	UCHAR	sima[4];		/* ���ԍ� [0]:1 - [3]:4		*/
	ULONG	gen1kei;		/* ����1�v(100�~)		*/
	ULONG	gen2kei;		/* ����2�v(100�~)		*/
	ULONG	sandkinko;		/* ���ދ��Ɍv			*/
	ULONG	cardkei;		/* ���ތv(100�~)		*/
	ULONG	kinko;			/* ���Ɍv			*/
	UCHAR	g1dancnt;		/* ����1�f����		*/
	UCHAR	g2dancnt;		/* ����2�f����		*/
	UCHAR	skkdancnt;		/* ���ދ��ɒf����		*/
	UCHAR	cddancnt;		/* ���ޒf����			*/
	UCHAR	kkdancnt;		/* ���ɒf����			*/
	UCHAR	errcnt;			/* �װ��			*/
	UCHAR	doorcnt;		/* �ޱ��			*/
	UCHAR	kktencnt;		/* ���ɓ_����			*/
#if 0
	UCHAR	flg;			/* �׸�				*/
	UCHAR	yobi;			/* ....				*/
#endif
} FDATA_KINKO;

/*#**************************************/
/*	�莞���(4109)						*/
/****************************************/
typedef	struct {
	UCHAR	kiotim[2];		/* �L������ (��:��)	*/
	USHORT	egyo;			/* �c�Ǝ���			*/
} FDATA_TIME_INF;


/*#**************************************/
/*	�莞�ғ����(4112)					*/
/****************************************/
typedef	FDATA_TIME_INF	FDATA_KADO_INF;


/*#**************************************/
/*	�莞��ʉғ�(4113)					*/
/****************************************/
typedef	struct {
	USHORT	daisu;			/* �䐔				*/
	USHORT	tcnt;			/* ���܉�			*/
	ULONG	out;			/* ���				*/
	ULONG	saf;			/* ���				*/
	ULONG	urikin;			/* ����(100�~)			*/
	ULONG	hosei;			/* ����␳�l(�~)		*/
	USHORT	kado;			/* ���ғ���			*/
	USHORT	k_kado;			/* ������ғ���			*/
	ULONG	keikos;			/* �i�i��			*/
	UCHAR	yobi[4];		/* ....				*/
} FDATA_KADO_SBT;


/*#**************************************/
/*	�莞�S�X�ғ�(4114)			*/
/****************************************/
typedef	struct {
	USHORT	daisu;			/* �䐔				*/
	USHORT	tcnt;			/* ���܉�			*/
	long	yobi[2];		/* �\��				*/
	ULONG	urikin;			/* ����(100�~)			*/
	ULONG	hosei;			/* ����␳�l(�~)		*/
	USHORT	kado;			/* ���ғ���			*/
	USHORT	k_kado;			/* ����ғ���			*/
	ULONG	keikin;			/* �i�i���z(100�~)		*/
	long	g_keikin;		/* ������i�i���z(100�~)	*/
	long	d_keikin;		/* ��i�i���z(100�~)		*/
	long	g_d_keikin;		/* ������i�i���z(100�~)	*/
	long	hd_keikin;		/* �␳��i�i���z(100�~)	*/
	long	g_hd_keikin;		/* �␳������i�i���z(100�~)	*/
} FDATA_KADO_ZEN;

/*#**************************************/
/*	�莞�@��ғ�(4115)			*/			// �@��ʒ莞�ғ� 2003/04/09 nakatani�@�R�����g
/****************************************/
//typedef	struct {
//	USHORT	daisu;			/* �䐔				*/
//	USHORT	tcnt;			/* ���܉�			*/
//	ULONG	out;			/* ���				*/
//	ULONG	saf;			/* ���				*/
//	ULONG	urikin;			/* ����(100�~)			*/
//	ULONG	hosei;			/* ����␳�l(�~)		*/
//	USHORT	kado;			/* ���ғ���			*/
//	USHORT	k_kado;			/* ������ғ���			*/
//} FDATA_KADO_KISHU;

#endif