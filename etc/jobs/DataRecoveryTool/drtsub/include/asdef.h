#ifndef __asdef_h__
#define __asdef_h__
 /* $Id: asdef.h,v 1.5 2002/08/01 04:40:21 nisimura Exp $ */               /* 直接書込データ:  */
                                /* 直接書込データ:  */
 /*  1998/9/18 18:05 osako            */           /* 直接書込データ:  */
 /* WAIS START */               /* 直接書込データ:  */
#define ASDEF
#ifdef _KDAI_MAX_1536
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

#define SETSUZOKU ZENTENSU      /* 全店接続設定件数:  */
#define SHUBETSU 8              /* 種別数:  */
#define ANSHO 8
#define TVCH_SU 12              /* TV ch設定件数:  */
#define REMOTE 32
 /* WAIS END */                 /* 直接書込データ:  */
#endif                          /* __asdef_h__ */
/*
 * $Log: asdef.h,v $
 * Revision 1.5  2002/08/01 04:40:21  nisimura
 * struct 定義が古かったため nas2002(20020731時点) のものをコピーした。
 *
 * Revision 1.4  2002/06/17 06:43:03  nisimura
 * SHUBETSU を３から８へ変更。（２店舗８種別対応）
 *
 * Revision 1.3  2001/02/19 02:31:18  kojima
 * no message
 *
 * Revision 1.6  2000/07/10 15:36:54  ohno-t
 * １５３６台対応操作部正式登録
 *
 * Revision 1.5  2000/07/06 05:24:02  osako
 * merge 1536 sourse files
 *
 * $Log: asdef.h,v $
 * Revision 1.5  2002/08/01 04:40:21  nisimura
 * struct 定義が古かったため nas2002(20020731時点) のものをコピーした。
 *
 * Revision 1.4  2002/06/17 06:43:03  nisimura
 * SHUBETSU を３から８へ変更。（２店舗８種別対応）
 *
 * Revision 1.3  2001/02/19 02:31:18  kojima
 * no message
 *
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
