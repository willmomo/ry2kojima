/* WAIS START */
/************************************************************************/
/*	RS�L��								*/
/*	Version	Date	  Programmer	Memo				*/
/*	00000.0	01.09.06  t.kaneko	�V�K�쐬			*/
/************************************************************************/
#ifndef	__RS_KIOKUH__

#define	__RS_KIOKUH__

#include "dat_file.h"
#include "kailog.h"
/********************************************************************************/
/*	define									*/
/********************************************************************************/
#define	FNO_RSKIOKU	6307			/* RS�L�����̧�ٔԍ�		*/
#define	FNO_TTOKREK	6102			/* �������ܗ���̧�ٔԍ�		*/

#define	FNO_KRSTOP	6110			/* �L���pRS�L���d��̧�ٔԍ�	*/
#define	FNO_KRSKIOKU	6111			/* �L���pRS�L�����̧�ٔԍ�	*/
#define	FNO_KTOKREK	6112			/* �L���p���ܗ���̧�ٔԍ�	*/
#define	FNO_KRUIDAI	6113			/* �L���p�݌v��2̧�ٔԍ�	*/

#define	FNO_TTKDTOP	5200			/* ����TKD�d��̧�ٔԍ�		*/
#define	FNO_TTKDLOG	5201			/* ����TKD LOĢ�ٔԍ�		*/

#define	FNO_KTKDTOP	5210			/* �L���pTKD�L���d��̧�ٔԍ�	*/
#define	FNO_KTKDKIOKU	5212			/* �L���pTKD�L�����̧�ٔԍ�	*/
#define	FNO_KKAILOG	5211			/* �L���pTKD LOĢ�ٔԍ�	*/

#define	RSTREK_MAX	16383			/* ���ܗ����ő匏��		*/

#define	RSKIOKU_SYNC	0x87cd			/* �ݸ����			*/

#define	RSKIOKU_MAX	7			/* RS�L������			*/
#define	TKDKIOKU_MAX	3			/* TKD�L������			*/


/********************************************************************************/
/*	RS�L�����								*/
/********************************************************************************/
typedef	struct {
	ulong	today_t_ymd;			/* �J�X���t			*/
	ushort	ken;				/* FROMײČ���			*/
	ushort	max;				/* FROMײ�MAX����		*/

	ulong	kioku_pw_tim;			/* �L�����t			*/
	ushort	kioku_no;			/* �L��No.(1�`7)		*/
	ushort	sync;				/* �ݸ���� 0x87cd		*/
	ushort	yobi1[8];			/* �\��				*/
} RS_KIOKUINF;


/*#**************************************/
/*	���ܗ����L���f�[�^		*/
/****************************************/
typedef struct {
	ushort	dno;				/* ��ԍ�			*/
	uchar	time[2];			/* ��������			*/
	uchar	tokfg;				/* �����׸�			*/
	uchar	tcnt;				/* ���܉�			*/
	uchar	t1cnt;				/* ����1�� (T1)		*/
	uchar	t2cnt;				/* ����2�� (T2)		*/
	uchar	kt1cnt;				/* �m�ϒ�T1��			*/
	uchar	pad1;				/* ....				*/
	ushort	tout;				/* ���ܱ��			*/
	ushort	tsaf;				/* ���ܾ��			*/
	ushort	e_bout;				/* �ŏI���܊Ա��		*/
	ushort	e_bsaf;				/* �ŏI���܊Ծ��		*/
	ushort	e_bstart;			/* �ŏI���܊Խ���		*/
	ushort	e_bmsa;				/* �ŏI���܊ԍő卷(�ō�)	*/
	ushort	e_buri;				/* �ŏI���܊Ԕ���		*/
	ushort	e_motichg;			/* �ŏI����(����)		*/
	ushort	e_motiever;			/* �ŏI����(�����)		*/
	uchar	r_bonus;			/* ڷޭװ��(�ۯĂ̂�)		*/
	uchar	kr_bonus;			/* CT��ڷޭװ��(�ۯĂ̂�)	*/
	ushort	yobi2;				/* .....			*/
} TOKREK_KIOKU;


/*#**************************************/
/*	RS�L���ް�			*/
/****************************************/
typedef struct {
	uchar			*jp;		/* �d��̧���߲��		*/
	RS_KIOKUINF		*kinf;		/* RS�L�����			*/
	TOKREK_KIOKU		*trek;		/* ���ܗ����L��̧��		*/
	FDATA_DAI2		*rdai;		/* �݌v��̧��2			*/
} RSKIOKU_DATA;

/*#**************************************/
/*	TKD�L���ް�			*/
/****************************************/
typedef struct {
	uchar			*jp;		/* �d��̧���߲��		*/
	RS_KIOKUINF		*kinf;		/* RS�L�����			*/
	RDATA_KAIIN		*klog;		/* ����ް�			*/
} TKDLOG_DATA;

#endif
/* WAIS END */
