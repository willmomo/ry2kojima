/* WAIS START */
/************************************************************************/
/*	システムヘッダ							*/
/*	Version	Date	 Programmer	Memo				*/
/*	00000.0	97.01.09	y.i		新規作成			*/
/************************************************************************/
#ifndef _SYS_HED_H_
#define _SYS_HED_H_

#define	L_SYS_HED
#include "asdef.h"
/************************************************/
/*	型					*/
/************************************************/
#if 1

#ifndef uchar
typedef	unsigned char	uchar;
#endif
#ifndef ushort
typedef	unsigned short	ushort;
#endif
#ifndef uint
typedef	unsigned int	uint;
#endif
#ifndef ulong
typedef	unsigned long	ulong;
#endif

#endif
/************************************************/
/*	MAX					*/
/************************************************/
#if 1
#define	MAX_DAI		DAISU		/* 台MAX			*/
#else
#define	MAX_DAI		256			/* 台MAX(DEBUG)			*/
#endif
#define	MAX_SIMA	SIMASU		/* 島MAX			*/
#define	MAX_KISHU	KISHU		/* 機種MAX			*/
#define	MAX_KINKO	KINKO		/* 金庫MAX			*/

#define	MAX_KINKO_SIMA	4		/* 金庫内島MAX			*/

#if 0	/* del 2002/03/25 */
#define	MAX_PAS		SHUBETSU	/* 種別MAX  :４種別対応 RUNTIME化 del 2002.03.25 */
#endif

#define	MAX_SGSBT	4			/* 島外種別MAX			*/
#define	MAX_FLOOR	FLOORSU		/* ﾌﾛｱｰMAX			*/
#define	MAX_TYPE	TYPE		/* ﾀｲﾌﾟMAX			*/

#define	MAX_SMCON_IN ZENTENSU	/* 島外接続MAX			*/

#define	DSAF_MAX	65535		/* セーフMAX	2001.10.18	*/

/************************************************/
/*						*/
/************************************************/
#define	LEN_KSHUNAME	28		/* 機種名長			*/

#endif /* _SYS_HED_H_ */
/* WAIS END */
