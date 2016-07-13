/* WAIS START */
#ifndef __koko_data_h__
#define __koko_data_h__

#include "nassys.h"
//#include "nascom.h"
//#include "dat_file.h"


/*	���삩��̗����ő�v������	*/
//#define		REQ_REKI_MAX		28


/* �����ް�(dat):  */
typedef struct _rstoku {
	uchar	time[2];		/* ��������			*/
	uchar	tokfg;			/* �����׸�			*/
	uchar	tcnt;			/* ���܉�			*/
	uchar	t1cnt;			/* ����1�� (T1)		*/
	uchar	t2cnt;			/* ����2�� (T2)		*/
	uchar	kt1cnt;			/* �m�ϒ�T1��			*/
	uchar	pad1;			/* ....				*/
	ushort	tout;			/* ���ܱ��			*/
	ushort	tsaf;			/* ���ܾ��			*/
	ushort	e_bout;			/* �ŏI���܊Ա��		*/
	ushort	e_bsaf;			/* �ŏI���܊Ծ��		*/
	ushort	e_bstart;		/* �ŏI���܊Խ���		*/
	ushort	e_bmsa;			/* �ŏI���܊ԍő卷(�ō�)	*/
	ushort	e_buri;			/* �ŏI���܊Ԕ���		*/
	ushort	e_motichg;		/* �ŏI����(����)		*/
	ushort	e_motiever;		/* �ŏI����(�����)		*/
	uchar	pad2[6];		/* .....			*/
} rstoku_t;


#endif				/* __vfcom_h__ */

/* WAIS END */
