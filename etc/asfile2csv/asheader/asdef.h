#ifndef __asdef_h__
#define __asdef_h__
 /* $Id: asdef.h,v 1.6 2000/07/10 15:36:54 ohno-t Exp $ */               /* 直接書込データ:  */
                                /* 直接書込データ:  */
 /*  1998/9/18 18:05 osako            */           /* 直接書込データ:  */
 /* WAIS START */               /* 直接書込データ:  */
#define ASDEF
//#ifdef _KDAI_MAX_1536
#if 1	// Ｗｉｎ操作は１５３６固定にする 2002.03.26 by koketsu
#define TNSU 1536               /* TN数:  */
#define DAISU 1536              /* 台数:  */
#else
#define TNSU 1024               /* TN数:  */
#define DAISU 1024              /* 台数:  */
#endif
#define ZENTENSU 256            /* 全店数:  */
#define SIMASU 96               /* 島数:  */
#define FLOORSU 8
#define KINKO 64                /* 金庫数:  */
#define HANBAIKISU 256          /* 販売機数:  */
#define TYPE 16                 /* タイプ:  */
#define TYPEMEICHO 46           /* タイプ名長:  */
#define KISHU 96                /* 機種数:  */
#define KISHUMEICHO 28          /* 機種名長:  */
#define PORT_USER 128
#define HENDO_MAX 8				/* 変動～～MAX数 */
#define RECSU 25				/* fno1108のサイズ(1400Byte)をレコード長(56Byte)で割った値:  */

#define SETSUZOKU ZENTENSU      /* 全店接続設定件数:  */

#if	0	/* 2002.06.17(mon) chg.ohnot */
#define SHUBETSU 8              /* 種別数:  */
#else
//	2002.06.11 修正 By Fujiki(２店舗)-->
//#define SHUBETSU 3              /* 種別数:  ＡＳが４種別サポートするまでは３種別とする 2002.04.18 koketsu  */
#define   SHUBETSU 4              /* 種別数:  Ｗｉｎ操作種別はＰＡＳＸの４種類 2002.03.26 Upd. By koketsu*/
//	-->2002.06.11 修正 By Fujiki(２店舗)
#endif

//	2002.06.14 Add By Koketsu(２店舗)-->
//-------------------------------
// KPC 取得ファイルレコード数
// ------------------------------
#define KIOKUINF_RSUU		9			// 記憶情報
#define TEIJIKADO_RSUU		34			// 定時稼動
#define TEIJISHU_RSUU		3264		// 定時機種
#define SYSIJYO_RSUU		256			// システム異常履歴
#define CLEAR_RSUU			16			// クリア履歴
#define	TOKEISET_RSUU		16			// 時計設定
#define	TEIDEN_RSUU			32			// 停電履歴
#define	SETTEI_RSUU			16			// 設定履歴
#define SETTEIVER_RSUU		1000		// 設定ＶＥＲ情報
#define	IJYO_RSUU			768			// 異常履歴
#define	UTI_RSUU			1536		// 打止履歴
#define	HASEI_RSUU			256			// 発生履歴
#define	TNSLUMP_RSUU		512			// ＴＮスランプ
#define	KAIINLOG_RSUU		36863		// 会員ログ
#define	KAIININF_RSUU		1			// 会員ログ管理情報
#define TENPOBETU_RSUU		25			// 店舗別変則営業情報	←2002/10/29 (店舗別変則営業対応)追加 Fukuyama
//<---	2002.06.14 Add By Koketsu(２店舗)

#define ANSHO 8
#define TVCH_SU 12              /* TV ch設定件数:  */
#define REMOTE 32
#define RECSU 25

#define GENKA_GETA		(100 * 1000)		// 原価率データの下駄(百分率 * 小数点以下三桁)
#define GENKA_DEFAULT	(1 * GENKA_GETA)	// 原価率設定==0時のdefault原価率


 /* WAIS END */                 /* 直接書込データ:  */
#endif                          /* __asdef_h__ */
/*
 * $Log: asdef.h,v $
 * Revision 1.6  2000/07/10 15:36:54  ohno-t
 * １５３６台対応操作部正式登録
 *
 * Revision 1.5  2000/07/06 05:24:02  osako
 * merge 1536 sourse files
 *
 * $Log: asdef.h,v $
 * Revision 1.6  2000/07/10 15:36:54  ohno-t
 * １５３６台対応操作部正式登録
 *
 * Revision 1.4  2000/02/23 04:08:35  matsuo
 * no message
 *
 * Revision 1.3  2000/01/11 05:15:08  ohno-t
 * コメント修正
 *
 * Revision 1.2  1999/12/29 15:36:22  sakurada
 * remove hankaku-kana, add keyword, fix indenting
 *
 * Revision 1.1.1.1  1999/12/26 05:57:10  sakurada
 * 1st try to import from VSS
 *
 * 2     99/10/01 9:41 Ohno-t
 *
 * 1     99/09/29 10:15a Sakurada
 * INCLUDE -> include
 * Revision 1.1  1999/09/29 01:13:28  sakurada
 * change INCLUDE -> include
 */
