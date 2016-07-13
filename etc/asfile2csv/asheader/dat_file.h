/* WAIS START */
/* $Header: /usr/external/CVS/as-sh3/include/dat_file.h,v 1.8 2002/04/24 09:40:54 ohno Exp $ */
/********************************************************************************************************/
/*	�f�[�^�t�@�C��											*/
/*	Version	Date		Programmer	Memo							*/
/*	?????.? 2001.02.28	S.A		�@��A��ʁA�S�X�Ɍ��ғ��䐔�A������ғ��䐔�ǉ�	*/
/*	?????.? 2000.12.06	mizu		�䔄�ォ��̌�����i�i���z(�~)�Ɩ��̕ύX		*/
/*	?????.? 2000.12.05	mizu		�䔄��ł̑e���v�v�Z�ׁ̈A�䔄�ォ��̌i�i���z�ǉ�	*/
/*	00000.0	97.07.30	����		�ŏI���ܒ�->�ŏI���܂P���ɕύX				*/
/*	00000.0	97.04.07	y.i		�V�K�쐬						*/
/********************************************************************************************************/
#ifndef __dat_file_h__
#define __dat_file_h__
#define	L_DAT_FILE
/****************************************/
/*	���O�f�����׸�			*/
/****************************************/
#define	DANFG_KINKO	0x00000001
#define	DANFG_KEIHIN	0x00000002
#define	DANFG_GENHAN	0x00000004
#define	DANFG_CARDHAN	0x00000008
#define	DANFG_REPLAY	0x00000010
#define	DANFG_PREMIUM	0x00000020
#define	DANFG_CYOTAMA	0x00000040
#define	DANFG_CDHAKKOU	0x00000080
#define	DANFG_POS	0x00000200
#define	DANFG_ZIHAN	0x00000400
#define	DANFG_TA	0x00000800

/****************************************/
/*	���ޔ��s�@�f�����׸�		*/
/****************************************/
#define	CDH_DANFG01K	0x0001
#define	CDH_DANFG02K	0x0002
#define	CDH_DANFG03K	0x0004
#define	CDH_DANFG05K	0x0008
#define	CDH_DANFG10K	0x0010

/****************************************/
/*	���ޔ��s�@�L���׸�		*/
/****************************************/
#define	CDHFG_01K	0x0001
#define	CDHFG_02K	0x0002
#define	CDHFG_03K	0x0004
#define	CDHFG_05K	0x0008
#define	CDHFG_10K	0x0010

/****************************************/
/*	�݌v��f�[�^			*/
/****************************************/

/****************************************/
/*	�����׸�			*/
/****************************************/

#define		FG_JFLG_TBASE		0x0001	/* �����ް�	*/
#define		FG_JFLG_DSET		0x0002	/* �ް��ݒ�	*/
#define		FG_JFLG_SHUTEI		0x0004	/* �W�v��~	*/
#define		FG_JFLG_KADO		0x0008	/* �ғ���	*/
#define		FG_JFLG_KAIIN		0x0010	/* �����	*/
#define		FG_JFLG_TOKTEI		0x2000	/* ���ܒ�~	*/
#define		FG_JFLG_IJOTEI		0x4000	/* �ُ��~	*/
#define		FG_JFLG_INTEI		0x8000	/* �捞��~	*/

#define		FG_JFLG_TEIALL		0xe004	/* �S��~�׸�	*/

/****************************************/
/*	�������׸�			*/
/****************************************/
#define		FG_HASFG_DFUS		0x0001	/* ��s��	*/
#define		FG_HASFG_DDAN		0x0002	/* ��f��	*/
#define		FG_HASFG_DIJYO		0x0004	/* ��ُ�	*/
#define		FG_HASFG_DTEN		0x0008	/* ��_��	*/
#define		FG_HASFG_URIFUS		0x0100	/* ����s��	*/
#define		FG_HASFG_URIDAN		0x0200	/* ����f��	*/
#define		FG_HASFG_URIIJYO	0x0400	/* ����ُ�	*/
#define		FG_HASFG_URITEN		0x0800	/* ����_��	*/

/****************************************/
/*	�������׸�			*/
/****************************************/

#define		FG_UKIND_GEN1		0x01	/* ���ތ���1�L��	*/
#define		FG_UKIND_GEN2		0x02	/* ���ތ���2�L��	*/
#define		FG_UKIND_CARD		0x04	/* ���ޔ���L��		*/
#define		FG_UKIND_GEN1KIN	0x10	/* ���ތ���1����	*/
#define		FG_UKIND_GEN2KIN	0x20	/* ���ތ���2����	*/

/*#**************************************/
/*	���ް�(�݌v)			*/
/****************************************/
typedef	struct {
	ushort	jflg;			/* �����׸�			*/
					/*    0x0001 : �����ް�		*/
					/*    0x0002 : �ް��ݒ�		*/
					/*    0x0004 : �W�v��~		*/
					/*    0x0008 : �ғ���		*/
					/*    0x0010 : ����ғ���	*/
	ushort	out;			/* ���				*/
	ushort	saf;			/* ���				*/
	ushort	start;			/* ����				*/
	ushort	tout;			/* ���ܱ��			*/
	ushort	tsaf;			/* ���ܾ��			*/
	ushort	t2out;			/* ����2���			*/
	ushort	t2saf;			/* ����2���			*/
	ushort	t2start;		/* ����2����			*/
	ushort	bcnt;			/* �ް���(���܉�/�Ŏ~��)	*/
	ushort	ucnt;			/* �Ŏ~��			*/
	ushort	tcnt;			/* ���܉�  (T)		*/
	ushort	t1cnt;			/* ����1�� (T1)		*/
	ushort	t2cnt;			/* ����2�� (T2)		*/
	ushort	kcnt;			/* �m�ω�((T2)>T1>T2>T1���)	*/
	ushort	kt1cnt;			/* �m�ϒ�T1��			*/

	ushort	lcnt;			/* ׯ����			*/
	ushort	lt1cnt;			/* ׯ��������(T1)��		*/
	ushort	lt1max;			/* ׯ����MAX����(T1)��	*/
	ushort	lout;			/* ׯ�������			*/
	ushort	lsaf;			/* ׯ�������			*/
	ushort	lbout;			/* ׯ������(T1)�Ա��		*/
	ushort	lmoti;			/* ׯ��������			*/

	ushort	start1;			/* ����1			*/
	ushort	start2;			/* ����2			*/
	ushort	start3;			/* ����3			*/
	ushort	t2start1;		/* ����2(T2)������1		*/
	ushort	t2start2;		/* ����2(T2)������2		*/
	ushort	t2start3;		/* ����2(T2)������3		*/
	ushort	t1start2;		/* ���ܽ���2��	2002.4.24 OTO�̓s���ɂ��ғ��f�[�^�͌��ɖ߂� */

	ushort	ukomi;			/* �ō���			*/
	ushort	t1ukomi;		/* ���ܑō�			*/
	short	minsa;			/* �ŏ���			*/
	short	maxsa;			/* �ő卷			*/
	ushort	minout;			/* �ŏ������			*/
	ushort	maxout;			/* �ő卷���			*/
	ushort	maxmoti;		/* �ő厝�� MY			*/
	ushort	maxukomi;		/* �ő�ō� M��			*/

	ushort	kadofg;			/* �莞�ғ��׸�(8-23)		*/
	uchar	kado;			/* �ωғ���			*/
	uchar	kyaku;			/* �q���			*/

	ushort	hasfg;			/* �������׸�			*/
					/*    0x0001 : ��s��		*/
					/*    0x0002 : ��f��		*/
					/*    0x0004 : ��ُ�		*/
					/*    0x0008 : ��_��		*/
					/*    0x0100 : ����s��		*/
					/*    0x0200 : ����f��		*/
					/*    0x0400 : ����ُ�		*/
					/*    0x0800 : ����_��		*/
	uchar	door1_2;		/* �ޱ1��		0xf0	*/
					/* �ޱ2��		0x0f	*/
	uchar	call1_2;		/* �ďo1��		0xf0	*/
					/* �ďo2��		0x0f	*/
	uchar	fus_ddan;		/* �s����		0xf0	*/
					/* ��f����		0x0f	*/
	uchar	ijo_ten;		/* �ُ��		0xf0	*/
					/* �_����		0x0f	*/
	uchar	g1_g2dan;		/* ����1�f����	0xf0	*/
					/* ����2�f����	0x0f	*/
	uchar	cddan_uijo;		/* ���ޒf����		0xf0	*/
					/* ����ُ��		0x0f	*/

	uchar	ukind;			/* �������׸�			*/
					/*     0x01 : ���ތ���1�L��	*/
					/*     0x02 : ���ތ���2�L��	*/
					/*     0x04 : ���ޔ���L��	*/
					/*     0x10 : ���ތ���1����	*/
					/*     0x20 : ���ތ���2����	*/
	uchar	dmy1;			/* ....				*/
	ushort	gen1uri;		/* ���ތ���1����(100�~)		*/
	ushort	gen2uri;		/* ���ތ���2����(100�~)		*/
	ushort	carduri;		/* ���޶��ޔ���(100�~)		*/
	ushort	daiuri;			/* �䔄��(100�~)		*/
	ushort	hosei;			/* ����␳�l(100�~) 2001.09.13 chg */

	uchar	k_suu;			/* �����			*/
	uchar	k_kado;			/* ����ғ���			*/
	ushort	k_kadofg;		/* ����莞�ғ��׸�(8-23)	*/
	uchar	k_t1cnt;		/* �������1�� (T1)		*/
	uchar	k_tcnt;			/* ������܉�	 (T)		*/
	uchar	k_kcnt;			/* ����m�ω�			*/
	uchar	k_lcnt;			/* ���ׯ����			*/
	ushort	k_out;			/* ������			*/
	ushort	k_saf;			/* ������			*/
	ushort	k_tout;			/* ������ܱ��			*/
	ushort	k_tsaf;			/* ������ܾ��			*/
	ushort	k_start;		/* �������			*/
	ushort	k_uri;			/* �������(100�~)		*/
	ushort	k_repkos;		/* �����ڲ��			*/
	ushort	k_reptesu;		/* �����ڲ�萔��(��)		*/

	ushort	r_bonus;		/* ڷޭװ��(�ۯĂ̂�)	1999.02.03 osako */

	ushort	toutmax;		/* ���ܒ����(�ő�l)		*/
	ushort	toutmin;		/* ���ܒ����(�ŏ��l)		*/
	short	tsamax;			/* ���ܒ���(�ő�l)		*/
	short	tsamin;			/* ���ܒ���(�ŏ��l)		*/
	ushort	boutmax;		/* ���܊Ա��(�ő�l)		*/
	ushort	boutmin;		/* ���܊Ա��(�ŏ��l)		*/
	short	bsamax;			/* ���܊ԍ�(�ő�l)		*/
	short	bsamin;			/* ���܊ԍ�(�ŏ��l)		*/
	ushort	bstartmax;		/* ���܊Խ���(�ő�l)		*/
	ushort	bstartmin;		/* ���܊Խ���(�ŏ��l)		*/
	short	bmsamax;		/* ���܊ԍő卷(�ő�l)		*/
	short	bmsamin;		/* ���܊ԍő卷(�ŏ��l)		*/
	short	tmotimax;		/* ���܎���(�ő�l)		*/
	short	tmotimin;		/* ���܎���(�ŏ��l)		*/

	ushort	kr_bonus;		/* CT��ڷޭװ��(�ۯĂ̂�)	1999.02.03 osako */

	uchar	tflg;			/* �����׸�			*/
					/*	0x01 : T1(�ޯ��)		*/
					/*	0x02 : T2(CT) 			*/
					/*	0x04 : ׯ�-				*/
					/*	0x08 : ڷޭװ(S�̂�)	*/
	uchar	dmy2;			/* ....				*/
	uchar	e_hour;			/* �ŏI����(��)			*/
	uchar	e_min;			/* �ŏI����(��)			*/
	ushort	e_t1out;		/* �ŏI���܂P�����		*/
	ushort	e_t1saf;		/* �ŏI���܂P�����		*/
	ushort	e_bout;			/* �ŏI���܂P�Ա��		*/
	ushort	e_bsaf;			/* �ŏI���܂P�Ծ��		*/
	ushort	e_bstart;		/* �ŏI���܂P�Խ���		*/
	ushort	e_bmsa;			/* �ŏI���܂P�ԍő卷(�ō�)	*/
	ushort	e_buri;			/* �ŏI���܂P�Ԕ���		*/
	ushort	e_motichg;		/* �ŏI����(����)		*/
	ushort	e_motiever;		/* �ŏI����(�����)		*/
#if	1					/* LEC.ICCARD 01/06/26 */
	ushort	hakkenuri;		/* ���ޔ�������(100�~)	*/
#else
	uchar	yobi2[2];		/* ....				*/
#endif
	ushort	san_ukos;		/* ���ޔ����			*/
	ushort	chk_dat;		/* CHK �ް�			*/
	ulong	k_keisu;		/* ��i�i��		*/
	ulong	k_cyokos;		/* �䒙�ʌ�			*/
} FDATA_DAI;

/*#**************************************/
/*	���ް�2(�݌v)			*/
/****************************************/
typedef	struct {
	/* ���Ľ����/���ĕ��z �ݒ� */
/*000*/	uchar	staslp_item;		/* ����ߍ���			*/
/*001*/	uchar	staslp_so;		/* SO�l				*/
/*002*/	short	stabun_bo;		/* BO�l				*/
/*004*/	short	stabun_kai;		/* �J�n�l			*/
/*006*/	uchar	stabun_ken;		/* �����l			*/
/*007*/	uchar	dmy1;			/* ��а				*/

	/* ���Ľ����/���ĕ��z �ް� */
/*008*/	uchar	staslp_kisyu;		/* �@��No.			*/
/*009*/	uchar	staslp_sima;		/* ��No.			*/
/*010*/	uchar	dmy2[2];		/* ��а				*/
/*012*/	ushort	staslp_s[5];		/* S1����ߓx 1�`5		*/
/*022*/	ushort	staslp_stotal;		/* SL�����İ�ٌ���		*/
/*024*/	ushort	stabun_b[6];		/* B1���z�x 1�`6		*/
/*036*/	ushort	stabun_btotal;		/* SB���zİ�ٌ���		*/
/*038*/	uchar	yobi1[90];		/* �\��				*/
} FDATA_DAI2;

/*#**************************************/
/*	����@���ް�			*/
/****************************************/
typedef	struct {
	ushort	daisu;			/* �䐔				*/
	ushort	jflg;			/* �����׸�(���ް��Q��)	*/
	ulong	out;			/* ���				*/
	ulong	saf;			/* ���				*/
	ulong	start;			/* ����				*/
	ulong	tout;			/* ���ܱ��			*/
	ulong	tsaf;			/* ���ܾ��			*/
	ulong	t2out;			/* ����2���			*/
	ulong	t2saf;			/* ����2���			*/
	ulong	t2start;		/* ����2����			*/
	ushort	bcnt;			/* �ް���(���܉�/�Ŏ~��)	*/
	ushort	ucnt;			/* �Ŏ~��			*/
	ushort	tcnt;			/* ���܉�  (T)		*/
	ushort	t1cnt;			/* ����1�� (T1)		*/
	ushort	t2cnt;			/* ����2�� (T2)		*/
	ushort	kcnt;			/* �m�ω�((T2)>T1>T2>T1��)	*/
	ushort	kt1cnt;			/* �m�ϒ�T1��			*/

	ushort	lcnt;			/* ׯ����			*/
	ushort	lt1cnt;			/* ׯ����T1��			*/
	ushort	lt1max;			/* ׯ����MAXT1��		*/
	ulong	lout;			/* ׯ�������			*/
	ulong	lsaf;			/* ׯ�������			*/
	ulong	lbout;			/* ׯ��T1�Ա��			*/
	ulong	lmoti;			/* ׯ��������			*/

	ulong	start1;			/* ����1			*/
	ulong	start2;			/* ����2			*/
	ulong	start3;			/* ����3			*/
	ulong	t2start1;		/* ����2(T2)������1		*/
	ulong	t2start2;		/* ����2(T2)������2		*/
	ulong	t2start3;		/* ����2(T2)������3		*/
	ulong	t1start2;		/* ���ܽ���2��		*/

	ulong	ukomi;			/* �ō���			*/
	ulong	t1ukomi;		/* ���ܑō�			*/
	ulong	maxmoti;		/* �ő厝�� MY			*/
	ulong	maxukomi;		/* �ő�ō� M��			*/

	ushort	kado;			/* �ωғ���			*/
	ushort	kyaku;			/* �q���			*/

	ushort	hasfg;			/* �������׸�			*/
	ushort	door1;			/* �ޱ1��			*/
	ushort	door2;			/* �ޱ2��			*/
	ushort	call1;			/* �ďo1��			*/
	ushort	call2;			/* �ďo2��			*/
	uchar	fuscnt;			/* �s����			*/
	uchar	ddancnt;		/* ��f����			*/
	uchar	ijocnt;			/* �ُ��			*/
	uchar	tencnt;			/* �_����			*/
	uchar	g1dancnt;		/* ����1�f����		*/
	uchar	g2dancnt;		/* ����2�f����		*/
	uchar	cddancnt;		/* ���ޒf����			*/
	uchar	uijocnt;		/* ����ُ��			*/

	uchar	ukind;			/* �������׸�(���ް��Q��)	*/
	uchar	dmy1;			/* ....				*/
	ulong	gen1uri;		/* ���ތ���1����(100�~)		*/
	ulong	gen2uri;		/* ���ތ���2����(100�~)		*/
	ulong	carduri;		/* ���޶��ޔ���(100�~)		*/
	ulong	daiuri;			/* �䔄��(100�~)		*/
	ulong	hosei;			/* ����␳�l(�~)		*/

	ushort	k_suu;			/* �����			*/
	ushort	k_kado;			/* ����ғ���			*/
	ushort	k_t1cnt;		/* �������1�� (T1)		*/
	ushort	k_tcnt;			/* ������܉�	 (T)		*/
	ushort	k_kcnt;			/* ����m�ω�			*/
	ushort	k_lcnt;			/* ���ׯ����			*/
	ulong	k_out;			/* ������			*/
	ulong	k_saf;			/* ������			*/
	ulong	k_tout;			/* ������ܱ��			*/
	ulong	k_tsaf;			/* ������ܾ��			*/
	ulong	k_start;		/* �������			*/
	ulong	k_uri;			/* �������(100�~)		*/
	ulong	k_repkos;		/* �����ڲ��			*/
	ulong	k_repkin;		/* �����ڲ���z(�~)		*/
	ulong	k_reptesu;		/* �����ڲ�萔��(��)		*/
	ulong	k_reptesukin;		/* �����ڲ�萔��(���z)(�~)	*/
	ulong	k_cyokos;		/* �䒙�ʌ�			*/
	ulong	k_cyokin;		/* �䒙�ʋ��z			*/
	ulong	k_keisu;		/* ��i�i��		*/

	ushort	r_bonus;		/* ڷޭװ��(�ۯĂ̂�)	1999.02.03 osako */
	ushort	kr_bonus;		/* CT��ڷޭװ��(�ۯĂ̂�)	1999.02.03 osako */

	ushort	g_kado;			/* ���ғ��䐔		2001.02.28 S.A add	*/
	ushort	g_k_kado;		/* ������ғ��䐔	2001.02.28 S.A add	*/

#if	1					/* LEC.ICCARD 01/06/26 */
	ulong	hakkenuri;		/* ���ޔ�������(100�~)	*/
	uchar	yobi[20];		/* ....				*/
#else
	uchar	yobi[24];		/* ....				*/
#endif

} FDATA_SK;

/*#**************************************/
/*	��ʏW�v��۱�ް�		*/
/****************************************/
typedef	struct {
	ushort	daisu;			/* �䐔				*/
	ushort	jflg;			/* �����׸�(���ް��Q��)	*/
	ulong	out;			/* ���				*/
	ulong	saf;			/* ���				*/
	ulong	start;			/* ����				*/
	ulong	tout;			/* ���ܱ��			*/
	ulong	tsaf;			/* ���ܾ��			*/
	ulong	t2out;			/* ����2���			*/
	ulong	t2saf;			/* ����2���			*/
	ulong	t2start;		/* ����2����			*/
	ushort	bcnt;			/* �ް���(���܉�/�Ŏ~��)	*/
	ushort	ucnt;			/* �Ŏ~��			*/
	ushort	tcnt;			/* ���܉�  (T)		*/
	ushort	t1cnt;			/* ����1�� (T1)		*/
	ushort	t2cnt;			/* ����2�� (T2)		*/
	ushort	kcnt;			/* �m�ω�((T2)>T1>T2>T1��)	*/
	ushort	kt1cnt;			/* �m�ϒ�T1��			*/

	ushort	lcnt;			/* ׯ����			*/
	ushort	lt1cnt;			/* ׯ����T1��			*/
	ushort	lt1max;			/* ׯ����MAXT1��		*/
	ulong	lout;			/* ׯ�������			*/
	ulong	lsaf;			/* ׯ�������			*/
	ulong	lbout;			/* ׯ��T1�Ա��			*/
	ulong	lmoti;			/* ׯ��������			*/

	ulong	ukomi;			/* �ō���			*/
	ulong	t1ukomi;		/* ���ܑō�			*/

	ushort	kado;			/* �ωғ���			*/
	ushort	kyaku;			/* �q���			*/

	ushort	hasfg;			/* �������׸�			*/
	ushort	door1;			/* �ޱ1��			*/
	ushort	door2;			/* �ޱ2��			*/
	ushort	call1;			/* �ďo1��			*/
	ushort	call2;			/* �ďo2��			*/
	uchar	fuscnt;			/* �s����			*/
	uchar	ddancnt;		/* ��f����			*/
	uchar	ijocnt;			/* �ُ��			*/
	uchar	tencnt;			/* �_����			*/
	uchar	g1dancnt;		/* ����1�f����		*/
	uchar	g2dancnt;		/* ����2�f����		*/
	uchar	cddancnt;		/* ���ޒf����			*/
	uchar	uijocnt;		/* ����ُ��			*/

	uchar	ukind;			/* �������׸�(���ް��Q��)	*/
	uchar	dmy1;			/* ....				*/
	ulong	gen1uri;		/* ���ތ���1����(100�~)		*/
	ulong	gen2uri;		/* ���ތ���2����(100�~)		*/
	ulong	carduri;		/* ���޶��ޔ���(100�~)		*/
	ulong	daiuri;			/* �䔄��(100�~)		*/
	ulong	hosei;			/* ����␳�l(�~)		*/

	ushort	k_suu;			/* �����			*/
	ushort	k_kado;			/* ����ғ���			*/
	ushort	k_t1cnt;		/* �������1�� (T1)		*/
	ushort	k_tcnt;			/* ������܉�	 (T)		*/
	ushort	k_kcnt;			/* ����m�ω�			*/
	ushort	k_lcnt;			/* ���ׯ����			*/
	ulong	k_out;			/* ������			*/
	ulong	k_saf;			/* ������			*/
	ulong	k_tout;			/* ������ܱ��			*/
	ulong	k_tsaf;			/* ������ܾ��			*/
	ulong	k_start;		/* �������			*/
	ulong	k_uri;			/* �������(100�~)		*/
	ulong	k_repkos;		/* �����ڲ��			*/
	ulong	k_reptesu;		/* �����ڲ�萔��(��)		*/
	ulong	k_cyokos;		/* �䒙�ʌ�			*/

	ulong	sg_danflg;		/* ���O�f�����׸�		*/
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
	ulong	urikin;			/* ������z(���v)(100�~)	*/
	ulong	t_keikos;		/* �i�i��(���v)		*/
	ulong	t_g_keikos;		/* �����i�i��			*/
	ulong	sg_gen1uri;		/* ���O����1����(100�~)		*/
	ulong	sg_gen2uri;		/* ���O����2����(100�~)		*/
	ulong	sg_carduri;		/* ���O���ޔ���(100�~)		*/
	ulong	sg_repkos;		/* ���O���ڲ��		*/
	ulong	sg_reptesu;		/* ���O���ڲ�萔��(��)	*/
	ulong	sg_premkos;		/* ���O���бь�		*/
	ulong	sg_cyokos;		/* ���O���ʌ�			*/
	ulong	kinko;			/* ���ɍ��v			01/06/26  ���ɔ����@���܂܂�  */
	ulong	sand;			/* ���ތ����v			*/
	ulong	sandkinko;		/* ���ދ��Ɍv			*/
	ulong	poskos;			/* POS��			*/
	ulong	g_poskos;		/* ����POS��			*/
	ulong	k_keisu;		/* ��i�i��		*/

	ushort	r_bonus;		/* ڷޭװ��(�ۯĂ̂�)	1999.02.03 osako */
	ushort	kr_bonus;		/* CT��ڷޭװ��(�ۯĂ̂�)	1999.02.03 osako */

	ushort	g_kado;			/* ���ғ��䐔		2001.02.28 S.A add	*/
	ushort	g_k_kado;		/* ������ғ��䐔	2001.02.28 S.A add	*/

#if	1					/* LEC.ICCARD 01/06/26 */
	ulong	hakkenuri;			/* ���ޔ�������(100�~)	��:����3=�T���h��������̌v  */
	ulong	kinko_hakkenuri;	/* ���ɔ�������(100�~)  ����:�\���ԍ�2=1xxx�̍��v	*/
#else
	uchar	yobi[8];		/* ....				*/
#endif

} FDATA_SBT;

/*#**************************************/
/*	�S�X�W�v			*/
/****************************************/
typedef	struct {
	ushort	daisu;			/* �䐔				*/
	ushort	jflg;			/* �����׸�(���ް��Q��)	*/

	ushort	kado;			/* �ωғ���			*/

	ushort	hasfg;			/* �������׸�			*/

	uchar	g1dancnt;		/* ����1�f����		*/
	uchar	g2dancnt;		/* ����2�f����		*/
	uchar	cddancnt;		/* ���ޒf����			*/
	uchar	uijocnt;		/* ����ُ��			*/

	uchar	ukind;			/* �������׸�(���ް��Q��)	*/
	uchar	dmy1[3];		/* ....				*/
	ulong	gen1uri;		/* ���ތ���1����(100�~)		*/
	ulong	gen2uri;		/* ���ތ���2����(100�~)		*/
	ulong	carduri;		/* ���޶��ޔ���(100�~)		*/
	ulong	daiuri;			/* �䔄��(100�~)		*/
	ulong	hosei;			/* ����␳�l(�~)		*/

	ushort	k_suu;			/* �����			*/
	ushort	k_kado;			/* ����ғ���			*/
	ulong	k_uri;			/* �������(100�~)		*/
	ulong	k_repkin;		/* �����ڲ���z(�~)		*/
	ulong	k_reptesukin;		/* �����ڲ�萔��(���z)(�~)	*/
	ulong	k_cyokin;		/* �䒙�ʋ��z(�~)		*/

	ulong	sg_danflg;		/* ���O�f�����׸�(��ʏW�v�Q��)	*/
	ulong	urikin;			/* ������z(���v)(100�~)	*/
	ulong	t_keikin;		/* �i�i���z(���v)(�~)		*/
	ulong	t_g_keikin;		/* �����i�i���z(�~)		*/
	ulong	sg_gen1uri;		/* ���O����1����(100�~)		*/
	ulong	sg_gen2uri;		/* ���O����2����(100�~)		*/
	ulong	sg_carduri;		/* ���O���ޔ���(100�~)		*/
	ulong	sg_repkin;		/* ���O���ڲ���z(�~)		*/
	ulong	sg_reptesukin;		/* ���O���ڲ�萔��(���z)(�~)	*/
	ulong	sg_premkin;		/* ���O���бы��z(�~)		*/
	ulong	sg_cyokin;		/* ���O���ʋ��z(�~)		*/
	ulong	kinko;			/* ���ɍ��v			01/06/26  ���ɔ����@���܂܂�  */
	ulong	sand;			/* ���ތ����v			*/
	ulong	sandkinko;		/* ���ދ��Ɍv			*/
	ulong	poskin;			/* POS���z(�~)			*/
	ulong	g_poskin;		/* ����POS���z(�~)		*/

	ushort	cd_hfg;			/* ���ޔ��s�׸�			*/
	ushort	cd_hdanfg;		/* ���ޔ��s�f���׸�		*/
	ulong	cd_htotal;		/* ���ޔ��s���v���z(100�~)	*/
	ulong	cd_h1kei;		/* ���ޔ��s1000�~�v(100�~)	*/
	ulong	cd_h2kei;		/* ���ޔ��s2000�~�v(100�~)	*/
	ulong	cd_h3kei;		/* ���ޔ��s3000�~�v(100�~)	*/
	ulong	cd_h5kei;		/* ���ޔ��s5000�~�v(100�~)	*/
	ulong	cd_h10kei;		/* ���ޔ��s10000�~�v(100�~)	*/

	ulong	zihan;			/* ���̋@�v(100�~)		*/
	ulong	ta;			/* ���̑��v			*/

	long	d_keikin;		/* ��i�i���z(100�~)		*/
	long	g_d_keikin;		/* ������i�i���z(100�~)	*/
	long	hd_keikin;		/* �␳��i�i���z(100�~)	*/
	long	g_hd_keikin;		/* �␳������i�i���z(100�~)	*/
	long	k_keikin;		/* ��i�i���z(���ʥ���v���C)		*/
#if 1	/* 2000.12.05 �䔄��ł̑e���v�v�Z�ׁ̈A�䔄�ォ��̌i�i���z�ǉ�	*/
/*	long	sa_kin;	*/		/* �䔄�ォ��̌i�i���z(�~)			*//* 2000.12.05 �䔄�ォ��̌i�i���z�ǉ� */
	long	g_duri_keikin;	/* �䔄�ォ��̌�����i�i���z(�~)	*//* 2000.12.06 ���̕ύX */

	ushort	g_kado;			/* ���ғ��䐔		2001.02.28 S.A add	*/
	ushort	g_k_kado;		/* ������ғ��䐔	2001.02.28 S.A add	*/

#if	1					/* LEC.ICCARD 01/06/26 */
	ulong	hakkenuri;			/* ���ޔ�������(100�~)	��:����3=�T���h��������̌v  */
	ulong	kinko_hakkenuri;	/* ���ɔ�������(100�~)  ����:�\���ԍ�2=1xxx�̍��v	*/

						/* LEC.ICCARD 01/09/06 */
	ulong	cd_seisanki;	/* �J�[�h���Z�@���v(100�~)  �J�[�h���s�@�̃}�C�i�X�X�P�[������	*/
	uchar	yobi[60];		/* ....				*/
#else
	uchar	yobi[72];		/* ....				*/
#endif

#else
	uchar	yobi[80];		/* ....				*/
#endif
} FDATA_TOTAL;

/*#**************************************/
/*	�莞���			*/
/****************************************/
typedef	struct {
	uchar	kiotim[2];		/* �L������ (��:��)		*/
	ushort	egyo;			/* �c�Ǝ���			*/
} FDATA_TIME_INF;

/*#**************************************/
/*	�莞�ғ����			*/
/****************************************/
typedef	FDATA_TIME_INF	FDATA_KADO_INF;

/*#**************************************/
/*	�莞�@��ғ�			*/
/****************************************/
typedef	struct {
	ushort	daisu;			/* �䐔				*/
	ushort	tcnt;			/* ���܉�			*/
	ulong	out;			/* ���				*/
	ulong	saf;			/* ���				*/
	ulong	urikin;			/* ����(100�~)			*/
	ulong	repkos;			/* ���ڲ��			*/
	ushort	kado;			/* ���ғ���			*/
	ushort	k_kado;			/* ������ғ���			*/
	ulong	startreg;		/* P:���ĉ�(T2�����Ċ�)	*/
					/* S:ڷޭװ��			*/
	ulong	startgame;		/* P:���ĉ�(�ް���)		*/
					/* S:�ްщ�(�ް����/3)	*/
	ushort	t2cnt;			/* P:�m�ω�			*/
					/* S:CT��			*/
	ushort	kcnt;			/* P:�m�ϓ˓���		*/
					/* S:CT�˓���			*/
	ushort	t1cnt;			/* P:����1��			*/
					/* S:�ޯ�މ�			*/
	uchar	yobi[2];		/* ....				*/
	ulong	reptesu;		/* ���ڲ�萔��(��)		*/
} FDATA_KADO_KISHU;

/*#**************************************/
/*	�莞��ʉғ�			*/
/****************************************/
typedef	struct {
	ushort	daisu;			/* �䐔				*/
	ushort	tcnt;			/* ���܉�			*/
	ulong	out;			/* ���				*/
	ulong	saf;			/* ���				*/
	ulong	urikin;			/* ����(100�~)			*/
	ulong	repkos;			/* ���ڲ��			*/
	ushort	kado;			/* ���ғ���			*/
	ushort	k_kado;			/* ������ғ���			*/
	ulong	keikos;			/* �i�i��			*/
	ulong	startreg;		/* P:���ĉ�(T2�����Ċ�)	*/
					/* S:ڷޭװ��			*/
	ulong	startgame;		/* P:���ĉ�(�ް���)		*/
					/* S:�ްщ�(�ް����/3)	*/
	ushort	t2cnt;			/* P:�m�ω�			*/
					/* S:CT��			*/
	ushort	kcnt;			/* P:�m�ϓ˓���		*/
					/* S:CT�˓���			*/
	ushort	t1cnt;			/* P:����1��			*/
					/* S:�ޯ�މ�			*/
	uchar	yobi[2];		/* ....				*/
	ulong	reptesu;		/* ���ڲ�萔��(��)		*/
} FDATA_KADO_SBT;

/*#**************************************/
/*	�莞�S�X�ғ�			*/
/****************************************/
typedef	struct {
	ushort	daisu;			/* �䐔				*/
	ushort	tcnt;			/* ���܉�			*/
	long	yobi[2];		/* �\��				*/
	ulong	urikin;			/* ����(100�~)			*/
	ulong	hosei;			/* ����␳�l(�~)		*/
	ushort	kado;			/* ���ғ���			*/
	ushort	k_kado;			/* ����ғ���			*/
	ulong	keikin;			/* �i�i���z(100�~)		*/
	long	g_keikin;		/* ������i�i���z(100�~)	*/
	long	d_keikin;		/* ��i�i���z(100�~)		*/
	long	g_d_keikin;		/* ������i�i���z(100�~)	*/
	long	hd_keikin;		/* �␳��i�i���z(100�~)	*/
	long	g_hd_keikin;		/* �␳������i�i���z(100�~)	*/
} FDATA_KADO_ZEN;

/*#**************************************/
/*	���ɏW�v			*/
/****************************************/
typedef	struct {
	uchar	sima[4];		/* ���ԍ� [0]:1 - [3]:4		*/
	ulong	gen1kei;		/* ����1�v(100�~)		*/
	ulong	gen2kei;		/* ����2�v(100�~)		*/

#if	1				/* LEC.ICCARD 01/06/26 */
	ulong	hakkenuri;		/* ���ޔ�������(100�~)	��:����3=�T���h��������̌v  */
	ulong	kinko_hakkenuri;	/* ���ɔ�������(100�~)  ����:�\���ԍ�2=1xxx�̍��v	*/
#else
	ulong	sandkinko;		/* ���ދ��Ɍv			*/
	ulong	cardkei;		/* ���ތv(100�~)		*/
#endif

	ulong	kinko;			/* ���Ɍv			01/06/25 ���ɔ�������܂�  */
	uchar	g1dancnt;		/* ����1�f����		*/
	uchar	g2dancnt;		/* ����2�f����		*/
	uchar	skkdancnt;		/* ���ދ��ɒf����		*/
	uchar	cddancnt;		/* ���ޒf����			*/
	uchar	kkdancnt;		/* ���ɒf����			*/
	uchar	errcnt;			/* �װ��			*/
	uchar	doorcnt;		/* �ޱ��			*/
	uchar	kktencnt;		/* ���ɓ_����			*/
#if 0
	uchar	flg;			/* �׸�				*/
	uchar	yobi;			/* ....				*/
#endif
} FDATA_KINKO;

/*#**************************************/
/*	�S�X�ް�(����)			*/
/****************************************/
typedef	struct {
	long	indata;			/* �v���l			*/
	char	dancnt;			/* �f����			*/
	char	err_door;		/* �װ��  0xf0		*/
					/* �ޱ��  0x0f		*/
	char	othercnt;		/* ���̑���			*/
	char	flg;			/* �׸�				*/
} FDATA_SMCON;

#endif



/* WAIS END */
