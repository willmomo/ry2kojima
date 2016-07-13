/* WAIS START */
/*
 *  2001.03.09 S.A okada
 *	台数に関係なく定時稼動集計を全て30分毎に変更
 */
/*
 * $Header: /usr/external/CVS/as-sh3/ap_svr/aplhed/apl_def.h,v 1.4 2001/03/23 05:01:31 okada Exp $
 * $History: apl_def.h $
 *
 * *****************  Version 4  *****************
 * User: Ohno         Date: 98/03/03   Time: 4:58a
 * Updated in $/NAS9709/AP_SVR/aplhed
 *
 * *****************  Version 3	 *****************
 * User: Sakurada	  Date: 98/02/12   Time: 13:19
 * Updated in $/NAS9709/AP_SVR/aplhed
 * WAIS検索キーワード追加
 *
 * *****************  Version 2	 *****************
 * User: Mizutani	  Date: 98/01/26   Time: 15:45
 * Updated in $/NAS9709/AP_SVR/aplhed
 * ｸﾘｱ履歴､ 時計設定履歴､ 停電履歴を追加
 *
 * *****************  Version 1	 *****************
 * User: Ohno		  Date: 97/12/10   Time: 5:20p
 * Created in $/nas9709/ap_svr/aplhed
 * tokaiより移動
 *
 * *****************  Version 3	 *****************
 * User: Ohno		  Date: 97/12/02   Time: 10:26a
 * Updated in $/NAS9709/Tokai/APLSV/APLHED
 */
/************************************************************************/
/*	アプリケーションサーバ＃ｄｅｆｉｎｅ				*/
/*	Version	Date	 Programmer	Memo				*/
/*	00000.0	97.09.19	y.i		新規作成			*/
/************************************************************************/
#define	L_APL_DEF
/************************************************/
/*						*/
/************************************************/
#define	DOUKI_UNBUFSUU	2		/* 同期情報運用制御ﾊﾞｯﾌｧ数		*/
#define	DOUKI_CTL	2		/* 同期情報運用制御中			*/

/************************************************/
/*						*/
/************************************************/
#define	MAX_KIOKU	9		/* 記憶MAX			*/
#define	MAX_TIMESYU	16		/* 定時集計ﾃﾞｰﾀMAX		*/

/* 2001.03.09 S.A upd ---->	8時から24時まで30分毎に定時稼動集計を実行しﾃﾞｰﾀ保存(締切時間は34)	*/
#define	MAX_TIMEKADO	34		/* 定時稼動ﾃﾞｰﾀMAX	(ｴﾘｱは58存在)	*/
#define	SEC_TIMEKADO	30		/* 定時稼動時間 	(30min)			*/
#if 0
#ifdef _KDAI_MAX_1536		/* 2000/02/22 Ohno */
#define	MAX_TIMEKADO	58		/* 定時稼動ﾃﾞｰﾀMAX		*/
#define	SEC_TIMEKADO	15		/* 定時稼動ﾃﾞｰﾀMAX		*/
#else
#define	MAX_TIMEKADO	96		/* 定時稼動ﾃﾞｰﾀMAX		*/
#define	SEC_TIMEKADO	10		/* 定時稼動ﾃﾞｰﾀMAX		*/
#endif
#endif
/* 2001.03.09 S.A upd <----	*/

#define	MAX_WEEK	7		/* 1週間分の日数		*/

#define	MAX_DFILES	16		/* ﾘｱﾙﾃﾞｰﾀ系ﾌｧｲﾙ数		*/

/************************************************/
/*	ﾌｧｲﾙNoﾃｰﾌﾞﾙ ｵﾌｾｯﾄ			*/
/************************************************/
#define	FTB_KIOKU	1		/* 記憶ﾌｧｲﾙ(重複ﾌｧｲﾙ)		*/
#define	FTB_DAI		2		/* 台ﾃﾞｰﾀﾌｧｲﾙ			*/
#define	FTB_SMCON	3		/* 全店ﾃﾞｰﾀ(島ｺﾝ)		*/
#define	FTB_SIMA	4		/* 島集計ﾌｧｲﾙ			*/
#define	FTB_KISHU	5		/* 機種集計ﾌｧｲﾙ			*/
#define	FTB_SBT		6		/* 種別集計ﾌｧｲﾙ			*/
#define	FTB_ZENTEN	7		/* 全店集計ﾌｧｲﾙ			*/
#define	FTB_KINKO	8		/* 金庫集計ﾌｧｲﾙ			*/
#define	FTB_TIMEINF	9		/* 定時情報ﾌｧｲﾙ			*/
#define	FTB_TIMESBT	10		/* 定時種別集計ﾌｧｲﾙ		*/
#define	FTB_TIMEZENTEN	11		/* 定時全店集計ﾌｧｲﾙ		*/
#define	FTB_KADO_INF	12		/* 定時稼動情報ﾌｧｲﾙ		*/
#define	FTB_KADO_SBT	13		/* 定時種別稼動ﾌｧｲﾙ		*/
#define	FTB_KADO_ZEN	14		/* 定時全店集計ﾌｧｲﾙ		*/
#define	FTB_KADO_KISHU	15		/* 定時機種稼動ﾌｧｲﾙ		*/

#define	KTB_TODAY	1		/* 当日				*/
#define	KTB_KIOKU01	2		/* 1日前			*/
#define	KTB_KIOKU02	3		/* 2日前			*/
#define	KTB_KIOKU03	4		/* 3日前			*/
#define	KTB_KIOKU04	5		/* 4日前			*/
#define	KTB_KIOKU05	6		/* 5日前			*/
#define	KTB_KIOKU06	7		/* 6日前			*/
#define	KTB_KIOKU07	8		/* 7日前			*/
#define	KTB_AVERAGE	9		/* 1週間平均			*/

/************************************************/
/*						*/
/************************************************/
#define	MAX_HANS	8		/* 販売機種類MAX		*/
#define	MAX_KEIS	2		/* 景品機種類MAX		*/

#define	MAX_REKISYS	127		/* ｼｽﾃﾑ異常履歴ﾃﾞｰﾀﾚｺｰﾄﾞMAX	*/
#define MAX_REKICLR	16		/* ｸﾘｱ履歴ﾃﾞｰﾀﾚｺｰﾄﾞMAX		*/
#define MAX_REKITIM	16		/*	時計設定履歴ﾃﾞｰﾀﾚｺｰﾄﾞMAX	*/
#define MAX_REKITEIDEN	16	/*	停電履歴ﾃﾞｰﾀﾚｺｰﾄﾞMAX		*/
#define	MAX_REKIIJO	255		/* 異常履歴ﾃﾞｰﾀﾚｺｰﾄﾞMAX		*/
#define	MAX_REKIHAS	255		/* 発生履歴ﾃﾞｰﾀﾚｺｰﾄﾞMAX		*/
#define	MAX_REKIINJI	63		/* 印字履歴ﾃﾞｰﾀﾚｺｰﾄﾞMAX		*/
#define	MAX_REKIONS	63		/* 音声履歴ﾃﾞｰﾀﾚｺｰﾄﾞMAX		*/
#define	MAX_REKICAME	63		/* ｶﾒﾗ履歴ﾃﾞｰﾀﾚｺｰﾄﾞMAX		*/

#define	MAX_PANELINF	4		/* ﾊﾟﾈﾙ情報ﾊﾞｯﾌｧMAX		*/

/************************************************/
/*	販売機種類				*/
/************************************************/
#define	HANS_KINKO	1		/* 金庫				*/
#define	HANS_GENHAN	2		/* 現金島外			*/
#define	HANS_CARDHAN	3		/* ｶｰﾄﾞ島外			*/
#define	HANS_REPLAY	4		/* ﾘﾌﾟﾚｲ			*/
#define	HANS_PREMIUM	5		/* ﾌﾟﾚﾐｱﾑ			*/
#define	HANS_CDHAKKOU	6		/* ｶｰﾄﾞ発行機			*/
#define	HANS_ZIHAN	7		/* 自販機			*/
#define	HANS_TA		8		/* その他			*/

/************************************************/
/*	景品機種類				*/
/************************************************/
#define	KEIS_KEIHIN	1		/* 計数機			*/
#define	KEIS_POS	2		/* POS				*/

/************************************************/
/*						*/
/************************************************/
#define	DAY_AVERAGE	9		/* 1週間平均 日NO		*/

/************************************************/
/*	ｼｽﾃﾑｴﾗｰｺｰﾄﾞ				*/
/************************************************/
#define	ESYS_SMF_OPEN	100		/* SMFｵｰﾌﾟﾝｴﾗｰ			*/
#define	ESYS_SMF_IO	101		/* SMF I/Oｴﾗｰ			*/
/* WAIS END */

