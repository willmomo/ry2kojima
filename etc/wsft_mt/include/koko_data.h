/* WAIS START */
#ifndef __koko_data_h__
#define __koko_data_h__

#include "nassys.h"
//#include "nascom.h"
//#include "dat_file.h"


/*	操作からの履歴最大要求件数	*/
//#define		REQ_REKI_MAX		28


/* 特賞ﾃﾞｰﾀ(dat):  */
typedef struct _rstoku {
	uchar	time[2];		/* 発生時分			*/
	uchar	tokfg;			/* 特賞ﾌﾗｸﾞ			*/
	uchar	tcnt;			/* 特賞回数			*/
	uchar	t1cnt;			/* 特賞1回数 (T1)		*/
	uchar	t2cnt;			/* 特賞2回数 (T2)		*/
	uchar	kt1cnt;			/* 確変中T1回数			*/
	uchar	pad1;			/* ....				*/
	ushort	tout;			/* 特賞ｱｳﾄ			*/
	ushort	tsaf;			/* 特賞ｾｰﾌ			*/
	ushort	e_bout;			/* 最終特賞間ｱｳﾄ		*/
	ushort	e_bsaf;			/* 最終特賞間ｾｰﾌ		*/
	ushort	e_bstart;		/* 最終特賞間ｽﾀｰﾄ		*/
	ushort	e_bmsa;			/* 最終特賞間最大差(打込)	*/
	ushort	e_buri;			/* 最終特賞間売上		*/
	ushort	e_motichg;		/* 最終持玉(交換)		*/
	ushort	e_motiever;		/* 最終持玉(無定量)		*/
	uchar	pad2[6];		/* .....			*/
} rstoku_t;


#endif				/* __vfcom_h__ */

/* WAIS END */
