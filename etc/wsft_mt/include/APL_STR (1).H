/* WAIS START */
/************************************************************************/
/*	アプリケーションサーバシステムテーブル				*/
/*	Version	Date	 Programmer	Memo				*/
/*	00000.0	97.05.21    y.i		新規作成			*/
/************************************************************************/
#define	L_APL_STR

//	2002.04.16 追加 By Fujiki-->
#include "APL_DEF.H"
//	-->2002.04.16 追加 By Fujiki
#include "rek_file.h"


#ifdef L_INF_FILE
/*#**************************************/
/*	INDEXｴﾘｱﾎﾟｲﾝﾀﾃｰﾌﾞﾙ		*/
/****************************************/
typedef struct {
	IDX_SIMA *sima;			/* 島ｲﾝﾃﾞｯｸｽ			*/
	IDX_KISHU *kishu;		/* 機種ｲﾝﾃﾞｯｸｽ			*/
	IDX_KINKO *kinko;		/* 金庫ｲﾝﾃﾞｯｸｽ			*/
					/*   [0] ～ [MAX_KINKO-1]	*/
} IDX_TABLE;

/*#**************************************/
/*	現在情報ﾎﾟｲﾝﾀﾃｰﾌﾞﾙ		*/
/****************************************/
typedef struct {
	INF_KIOKU *kioku;		/* 記憶情報			*/
					/*   [0]:当日			*/
					/*   [1]:1日前～[7]:7日前	*/
					/*   [8]:1週間平均		*/
} INF_TABLE;
#endif

#ifdef __un_as_h__
/*#**************************************/
/*	運用情報ﾎﾟｲﾝﾀﾃｰﾌﾞﾙ		*/
/****************************************/
typedef struct {
	fno4001_t *dousa;		/* 動作ﾃｰﾌﾞﾙ			*/
	fno4003_t *douki;		/* 同期情報ﾃｰﾌﾞﾙ		*/
} UNYOU_TABLE;
#endif

#ifdef L_DAT_FILE
/*#**************************************/
/*	ﾃﾞｰﾀｴﾘｱﾎﾟｲﾝﾀﾃｰﾌﾞﾙ		*/
/****************************************/
typedef struct {
	FDATA_DAI *dai;			/* 台ﾃﾞｰﾀ(島ｺﾝ)			*/
					/*   [0]～[MAX_DAI-1]		*/
	FDATA_SMCON *smcon;		/* 全店ﾃﾞｰﾀ(島ｺﾝ)		*/
					/*   [0]～[MAX_SMCON_IN-1]	*/
	FDATA_SK *sima;			/* 島集計ﾃﾞｰﾀ			*/
	FDATA_SK *kishu;		/* 機種集計ﾃﾞｰﾀ			*/
	FDATA_SBT *sbt;			/* 種別集計ﾃﾞｰﾀ			*/
					/*   [0]～[MAX_PAS-1]		*/
	FDATA_TOTAL *total;		/* 全店集計ﾃﾞｰﾀ			*/
	FDATA_KINKO *kinko;		/* 金庫集計ﾃﾞｰﾀ			*/
					/*   [0]～[MAX_KINKO-1]		*/
//	FDATA_TIME_INF *timeinf;	/* 定時情報ﾃｰﾌﾞﾙ		*/		
//	FDATA_KADO_INF *kadoinf;	/* 定時稼動情報ﾃｰﾌﾞﾙ		*/
	FDATA_KADO_KISHU *kiskado;	/* 定時機種稼動ﾃﾞｰﾀ		*/		// 機種別定時稼動 2003/04/09 nakatani
																// 有効にしました		
	FDATA_KADO_SBT *sbtkado;	/* 定時種別稼動ﾃﾞｰﾀ		*/		// nakatani 有効にしました
	FDATA_KADO_ZEN *zenkado;	/* 定時全店稼動ﾃﾞｰﾀ		*/		// nakatani 有効にしました
	fno4117_t *dspdata;			/* DSPデータファイル	*/

	// 2002.04.15 追加 By Fujiki-->
	FDATA_TIME_INF *timeinf;	// 定時情報ﾃｰﾌﾞﾙ Max16件
	FDATA_SBT *sbtShu_Seki;		// 定時種別集計ﾃﾞｰﾀ（積）
	FDATA_SBT *sbtShu_Sa;		// 定時種別集計ﾃﾞｰﾀ（差）
//	FDATA_SBT *sbtShu_Seki[MAX_PAS];		// 定時種別集計ﾃﾞｰﾀ（積）
//	FDATA_SBT *sbtShu_Sa[MAX_PAS];		// 定時種別集計ﾃﾞｰﾀ（差）
	FDATA_TOTAL *zenShu_Seki;	// 定時全店集計ﾃﾞｰﾀ（積）
	FDATA_TOTAL *zenShu_Sa;		// 定時全店集計ﾃﾞｰﾀ（差）
	// -->2002.04.15 追加 By Fujiki

	fno4021_t *panel_info;		// パネル情報
	fno4028_t *panel_info2;		// パネル情報2
} FDATA_TABLE;
#endif

#ifdef	L_SET_SYS
/*#**************************************/
/*	ｼｽﾃﾑ設定ｴﾘｱﾎﾟｲﾝﾀﾃｰﾌﾞﾙ		*/
/****************************************/
typedef struct {
	FSET_SYS_SYS	*sys;		/* ｼｽﾃﾑ設定			*/
	FSET_SYS_DAI	*dai;		/* 台接続設定			*/
	FSET_SYS_ZEN	*zen;		/* 全店接続設定			*/
					/*   [0]～[MAX_SMCON_IN-1]	*/
} FSET_SYS_TABLE;
#endif

#ifdef	L_SET_INIT
/*#**************************************/
/*	初期設定ｴﾘｱﾎﾟｲﾝﾀﾃｰﾌﾞﾙ		*/
/****************************************/
typedef struct {
	FSET_INIT_URI	*uri;		/* 売上定数設定			*/
	FSET_INIT_KSHU	*kshu;		/* 機種名設定			*/
} FSET_INIT_TABLE;
#endif

#ifdef	L_REK_FILE
/*#**************************************/
/*	履歴ｴﾘｱﾎﾟｲﾝﾀﾃｰﾌﾞﾙ		*/
/****************************************/
typedef struct {
	REKI_INF *inf;			/* 発生情報ﾃｰﾌﾞﾙ		*/

	REKI_SYSIJO_HED *sys_hed;	/* ｼｽﾃﾑ異常履歴ﾍｯﾀﾞ		*/
	REKI_SYSIJO *sys;		/* ｼｽﾃﾑ異常履歴ﾃﾞｰﾀ		*/
					/*   [0] ～ [MAX_REKISYS-1]	*/
	REKI_CLEAR	*clear;	/* ｸﾘｱ履歴	*/

	REKI_TIMSET	*timer;	/* 時計設定履歴		*/

	REKI_TEIDEN *teiden;	/* 停電履歴	*/

	REKI_IJO_HED *ijo_hed;		/* 異常履歴ﾍｯﾀﾞ			*/
	REKI_IJO *ijo;			/* 異常履歴ﾃﾞｰﾀ			*/
					/*   [0] ～ [MAX_REKIIJO-1]	*/
	REKI_UTI_HED *uti_hed;		/* 打止履歴ﾍｯﾀﾞ			*/
	REKI_UTI *uti;			/* 打止履歴ﾃﾞｰﾀ			*/
					/*   [0] ～ [MAX_DAI-1]		*/
	REKI_HASSEI_HED *has_hed;	/* 発生履歴ﾍｯﾀﾞ			*/
	REKI_HASSEI *has;		/* 発生履歴ﾃﾞｰﾀ			*/
					/*   [0] ～ [MAX_REKIHAS-1]	*/
	REKI_INJI_HED *inji_hed;	/* 印字履歴ﾍｯﾀﾞ			*/
	REKI_INJI *inji;		/* 印字履歴ﾃﾞｰﾀ			*/
					/*   [0] ～ [MAX_REKIINJI-1]	*/
	REKI_ONSEI_HED *ons_hed;	/* 音声履歴ﾍｯﾀﾞ			*/
	REKI_ONSEI *ons;		/* 音声履歴ﾃﾞｰﾀ			*/
					/*   [0] ～ [MAX_REKIONS-1]	*/
	REKI_CAMERA_HED *came_hed;	/* ｶﾒﾗ履歴ﾍｯﾀﾞ			*/
	REKI_CAMERA *came;		/* ｶﾒﾗ履歴ﾃﾞｰﾀ			*/
					/*   [0] ～ [MAX_REKICAME-1]	*/
	char (*panel)[4];		/* ﾊﾟﾈﾙ情報 panel[MAX_DAI][4]	*/
	/*char (*panel)[MAX_DAI];*/	/* ﾊﾟﾈﾙ情報			*/
					/*   [0][] ～ [MAX_PANELINF-1][]*/
					/*   [][0] ～ [][MAX_DAI-1]	*/
} REKI_TABLE;
#endif

/****************************************/
/*	ﾃﾞｰﾀﾌｧｲﾙ番号格納ｴﾘｱﾎﾟｲﾝﾀﾃｰﾌﾞﾙ	*/
/****************************************/
typedef	struct {
	ushort	*dai;			/* 台ﾃﾞｰﾀﾌｧｲﾙ			*/
	ushort	*smcon;			/* 全店ﾃﾞｰﾀﾌｧｲﾙ			*/
	ushort	*sima;			/* 島集計ﾌｧｲﾙ			*/
	ushort	*kishu;			/* 機種集計ﾌｧｲﾙ			*/
	ushort	*sbt;			/* 種別集計ﾌｧｲﾙ			*/
	ushort	*total;			/* 全店集計ﾌｧｲﾙ			*/
	ushort	*kinko;			/* 金庫集計ﾌｧｲﾙ			*/
	ushort	*timesbt;		/* 定時種別集計ﾌｧｲﾙ		*/
	ushort	*timezen;		/* 定時全店集計ﾌｧｲﾙ		*/
	ushort	*kiskado;		/* 定時機種稼動ﾌｧｲﾙ		*/
	ushort	*sbtkado;		/* 定時種別稼動ﾌｧｲﾙ		*/
	ushort	*zenkado;		/* 定時全店稼動ﾌｧｲﾙ		*/
} FILENO_TABLE;

/* WAIS END */

