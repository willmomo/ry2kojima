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
#define	MAX_DAI		DAISU		/* 台MAX				*/
#else
#define	MAX_DAI		256			/* 台MAX(DEBUG)			*/
#endif
#define	MAX_SIMA	SIMASU		/* 島MAX				*/
#define	MAX_KISHU	KISHU		/* 機種MAX				*/
#define	MAX_KINKO	KINKO		/* 金庫MAX				*/
#define	MAX_RECSU	RECSU		/* fno1108のレコード数MAX*/ //←2002/10/29 (店舗別変則営業対応)追加 Fukuyama

#define	MAX_KINKO_SIMA	4		/* 金庫内島MAX			*/

#define	MAX_PAS		SHUBETSU	/* 種別MAX				*/
#define	MAX_SGSBT	5			/* 島外種別MAX			*/
#define	MAX_FLOOR	FLOORSU		/* ﾌﾛｱｰMAX				*/
#define	MAX_TYPE	TYPE		/* ﾀｲﾌﾟMAX				*/
//	2002.05.06 追加 By Fujiki(２店舗)-->
#define MAX_TENPO	2			// 店舗数
//	-->2002.05.06 追加 By Fujiki(２店舗)
//-> 2002.06.14 追加 kaoru（２店舗）
#define MAX_SBTPOCKET	8		// 種別ポケット件数
#define MAX_HENDOINS	8		// 変動登録件数
//<- 2002.06.14 追加 kaoru（２店舗）
#define	MAX_SMCON_IN ZENTENSU	/* 島外接続MAX			*/

#define	DSAF_MAX	65535		/* セーフMAX	2001.10.18	*/

/************************************************/
/*						*/
/************************************************/
#define	LEN_KSHUNAME	28		/* 機種名長			*/

#endif /* _SYS_HED_H_ */
	/*
/* WAIS END */
