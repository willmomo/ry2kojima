/* WAIS START */
/************************************************************************/
/*	RS記憶								*/
/*	Version	Date	  Programmer	Memo				*/
/*	00000.0	01.09.06  t.kaneko	新規作成			*/
/************************************************************************/
#ifndef	__RS_KIOKUH__

#define	__RS_KIOKUH__

#include "dat_file.h"
#include "kailog.h"
/********************************************************************************/
/*	define									*/
/********************************************************************************/
#define	FNO_RSKIOKU	6307			/* RS記憶情報ﾌｧｲﾙ番号		*/
#define	FNO_TTOKREK	6102			/* 当日特賞履歴ﾌｧｲﾙ番号		*/

#define	FNO_KRSTOP	6110			/* 記憶用RS記憶重複ﾌｧｲﾙ番号	*/
#define	FNO_KRSKIOKU	6111			/* 記憶用RS記憶情報ﾌｧｲﾙ番号	*/
#define	FNO_KTOKREK	6112			/* 記憶用特賞履歴ﾌｧｲﾙ番号	*/
#define	FNO_KRUIDAI	6113			/* 記憶用累計台2ﾌｧｲﾙ番号	*/

#define	FNO_TTKDTOP	5200			/* 当日TKD重複ﾌｧｲﾙ番号		*/
#define	FNO_TTKDLOG	5201			/* 当日TKD LOGﾌｧｲﾙ番号		*/

#define	FNO_KTKDTOP	5210			/* 記憶用TKD記憶重複ﾌｧｲﾙ番号	*/
#define	FNO_KTKDKIOKU	5212			/* 記憶用TKD記憶情報ﾌｧｲﾙ番号	*/
#define	FNO_KKAILOG	5211			/* 記憶用TKD LOGﾌｧｲﾙ番号	*/

#define	RSTREK_MAX	16383			/* 特賞履歴最大件数		*/

#define	RSKIOKU_SYNC	0x87cd			/* ｼﾝｸｺｰﾄﾞ			*/

#define	RSKIOKU_MAX	7			/* RS記憶日数			*/
#define	TKDKIOKU_MAX	3			/* TKD記憶日数			*/


/********************************************************************************/
/*	RS記憶情報								*/
/********************************************************************************/
typedef	struct {
	ulong	today_t_ymd;			/* 開店日付			*/
	ushort	ken;				/* FROMﾗｲﾄ件数			*/
	ushort	max;				/* FROMﾗｲﾄMAX件数		*/

	ulong	kioku_pw_tim;			/* 記憶日付			*/
	ushort	kioku_no;			/* 記憶No.(1～7)		*/
	ushort	sync;				/* ｼﾝｸｺｰﾄﾞ 0x87cd		*/
	ushort	yobi1[8];			/* 予備				*/
} RS_KIOKUINF;


/*#**************************************/
/*	特賞履歴記憶データ		*/
/****************************************/
typedef struct {
	ushort	dno;				/* 台番号			*/
	uchar	time[2];			/* 発生時分			*/
	uchar	tokfg;				/* 特賞ﾌﾗｸﾞ			*/
	uchar	tcnt;				/* 特賞回数			*/
	uchar	t1cnt;				/* 特賞1回数 (T1)		*/
	uchar	t2cnt;				/* 特賞2回数 (T2)		*/
	uchar	kt1cnt;				/* 確変中T1回数			*/
	uchar	pad1;				/* ....				*/
	ushort	tout;				/* 特賞ｱｳﾄ			*/
	ushort	tsaf;				/* 特賞ｾｰﾌ			*/
	ushort	e_bout;				/* 最終特賞間ｱｳﾄ		*/
	ushort	e_bsaf;				/* 最終特賞間ｾｰﾌ		*/
	ushort	e_bstart;			/* 最終特賞間ｽﾀｰﾄ		*/
	ushort	e_bmsa;				/* 最終特賞間最大差(打込)	*/
	ushort	e_buri;				/* 最終特賞間売上		*/
	ushort	e_motichg;			/* 最終持玉(交換)		*/
	ushort	e_motiever;			/* 最終持玉(無定量)		*/
	uchar	r_bonus;			/* ﾚｷﾞｭﾗｰ回数(ｽﾛｯﾄのみ)		*/
	uchar	kr_bonus;			/* CT中ﾚｷﾞｭﾗｰ回数(ｽﾛｯﾄのみ)	*/
	ushort	yobi2;				/* .....			*/
} TOKREK_KIOKU;


/*#**************************************/
/*	RS記憶ﾃﾞｰﾀ			*/
/****************************************/
typedef struct {
	uchar			*jp;		/* 重複ﾌｧｲﾙﾎﾟｲﾝﾀ		*/
	RS_KIOKUINF		*kinf;		/* RS記憶情報			*/
	TOKREK_KIOKU		*trek;		/* 特賞履歴記憶ﾌｧｲﾙ		*/
	FDATA_DAI2		*rdai;		/* 累計台ﾌｧｲﾙ2			*/
} RSKIOKU_DATA;

/*#**************************************/
/*	TKD記憶ﾃﾞｰﾀ			*/
/****************************************/
typedef struct {
	uchar			*jp;		/* 重複ﾌｧｲﾙﾎﾟｲﾝﾀ		*/
	RS_KIOKUINF		*kinf;		/* RS記憶情報			*/
	RDATA_KAIIN		*klog;		/* 会員ﾃﾞｰﾀ			*/
} TKDLOG_DATA;

#endif
/* WAIS END */
