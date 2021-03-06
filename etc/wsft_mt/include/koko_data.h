/* WAIS START */
#ifndef __koko_data_h__
#define __koko_data_h__

#include "nassys.h"
//#include "nascom.h"
//#include "dat_file.h"


/*	μ©ηΜπΕεv	*/
//#define		REQ_REKI_MAX		28


/* ΑάΓή°ΐ(dat):  */
typedef struct _rstoku {
	uchar	time[2];		/* ­Άͺ			*/
	uchar	tokfg;			/* ΑάΜΧΈή			*/
	uchar	tcnt;			/* Αάρ			*/
	uchar	t1cnt;			/* Αά1ρ (T1)		*/
	uchar	t2cnt;			/* Αά2ρ (T2)		*/
	uchar	kt1cnt;			/* mΟT1ρ			*/
	uchar	pad1;			/* ....				*/
	ushort	tout;			/* Αά±³Δ			*/
	ushort	tsaf;			/* ΑάΎ°Μ			*/
	ushort	e_bout;			/* ΕIΑάΤ±³Δ		*/
	ushort	e_bsaf;			/* ΕIΑάΤΎ°Μ		*/
	ushort	e_bstart;		/* ΕIΑάΤ½ΐ°Δ		*/
	ushort	e_bmsa;			/* ΕIΑάΤΕε·(Ε)	*/
	ushort	e_buri;			/* ΕIΑάΤγ		*/
	ushort	e_motichg;		/* ΕIΚ(π·)		*/
	ushort	e_motiever;		/* ΕIΚ(³θΚ)		*/
	uchar	pad2[6];		/* .....			*/
} rstoku_t;


#endif				/* __vfcom_h__ */

/* WAIS END */
