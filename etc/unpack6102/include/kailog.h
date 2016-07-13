/* WAIS START */
#ifndef __kailog_h__
#define __kailog_h__


/*#**************************************/
/*	����f�[�^			*/
/****************************************/
typedef	struct {
	unsigned char	dcode1;			/* �d������1 0x22�Œ�	*/
	unsigned char	dcode2;			/* �d������2 ox03�Œ�	*/
	unsigned char	subcode;		/* ��޺���		*/
					/*	3 = �ғ����	*/
					/*	4 = ���ޱ��	*/

	unsigned char	yobi1;			/* ......		*/
	unsigned char	yobi2[2];		/* ......		*/
	unsigned char	format;			/* ̫�ϯ� 0x02�Œ�	*/
	unsigned char	jflg;			/* ����׸� 0x00�Œ�	*/
	unsigned long	kaiid;			/* ���ID		*/
	unsigned char	subno;			/* ��ޔԍ�		*/
	unsigned char	pas;			/* ���			*/
	unsigned short	daino;			/* ��			*/
	unsigned char	intim[2];		/* ����IN ����(BCD)	*/
	unsigned char	outtim[2];		/* ����OUT ����(BCD)	*/
	unsigned char	t1cnt;			/* T1��		*/
	unsigned char	k1cnt;			/* �m�ςP��		*/
	unsigned short	out;			/* �݌v���		*/
	unsigned short	saf;			/* �݌v���		*/
	unsigned short	genuri;			/* ���ތ���1+����2����	*/
	unsigned short	carduri;		/* ���޶��ޔ���		*/
#if 1								/* 2001.10.12	*/
	unsigned short	saf_ov;			/* ��̂̂��ӂꕪ add 2001.10.12	*/
#else
	unsigned char	yobi3[2];		/* .....		*/
#endif

#if 1	/* 99/11/02 */
	unsigned long	cyokos;			/* ����			*/
#else
	unsigned long	saverno;		/* ���ފǗ��ԍ�		*/
#endif
	unsigned short	start;			/* �݌v����		*/
	unsigned short	ukomi;			/* �ō���		*/
	unsigned char	tcnt;			/* ����			*/
	unsigned char	t2cnt;			/* ���܂Q��		*/
	unsigned char	kcnt;			/* �m�ω�		*/
	unsigned char	kt1cnt;			/* �m�ϒ�T1��		*/
	unsigned short	t1out;			/* ���ܒ���� - ���܂Q�����	*/
	unsigned short	t1saf;			/* ���ܒ���� - ���܂Q�����	*/
	unsigned short	t2out;			/* ����2���			*/
	unsigned short	t2saf;			/* ����2���			*/
	unsigned short	t2start;		/* ����2����			*/
	unsigned short	point;			/* �߲�Đ�		*/
	unsigned short	cdin_errcode;		/* ����IN �ʐM�װ����	*/
	unsigned short	cdout_errcode;		/* ����OUT �ʐM�װ����	*/
#if 1	/* 99/11/02 */
	unsigned short	repkos;			/* ����ڲ��		*/
	unsigned short	reptesu;		/* ����ڲ�萔��	*/
#else
	unsigned char	yobi4[4];		/* .....		*/
#endif
} RDATA_KAIIN;


/*#**************************************/
/* ����Ǘ����ð���			*/
/*#**************************************/
typedef	struct {
					/* �������			*/
	unsigned long	today_t_ymd;			/* �J�X���̔N����	*/
	unsigned short	ken;				/* FROMײČ���		*/
	unsigned short	max;				/* FROMײ�MAX����	*/

/* 2001.09.06 t.kaneko */
#if 0
					/* �O�����			*/
	unsigned long	save_t_ymd;			/* �J�X���̔N����	*/
	unsigned short	save_ken;			/* FROMײČ���		*/
	unsigned short	save_max;			/* FROMײ�MAX����	*/
#else
	ulong	kioku_pw_tim;			/* �L�����t			*/
	ushort	kioku_no;			/* �L��No.(1�`7)		*/
	ushort	sync;				/* �ݸ���� 0x87cd		*/
	ushort	yobi1[8];			/* �\��				*/
#endif
/* 2001.09.06 t.kaneko */

} RDATA_KAIININF;


#endif		/* __kailog_h__ */

/* WAIS END */
