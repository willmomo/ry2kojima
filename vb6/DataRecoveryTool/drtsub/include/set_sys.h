#ifndef __set_sys_h__
#define __set_sys_h__
/* WAIS START */

/*
 *	システム設定ファイル
 *		2002/05/31 osako		2店舗4種別8フロア対応
 */

#include "nassys.h"
#include "asdef.h"
#define L_DAT_FILE
#define L_SET_SYS
 /****************************************/
 /*	島ｺﾝ入力種類	*/
 /****************************************/
#define SCON_KINKO 1
#define SCON_KEIHIN 2
#define SCON_GENHAN 3
#define SCON_CARDHAN 4
#define SCON_REPLAY 5
#define SCON_PREMIUM 6
#define SCON_CDHAKKOU 7
#define SCON_POS 8
#define SCON_ZIHAN 9
#define SCON_TA 10
 /****************************************/
 /* ｶｰﾄﾞ発行機種類ｽｹｰﾗ   */
 /****************************************/
#define SCL_CDH01K 10
#define SCL_CDH02K 20
#define SCL_CDH03K 30
#define SCL_CDH05K 50
#define SCL_CDH10K 100

/* ｼｽﾃﾑ設定:  */
typedef struct _fno1000 {
    unsigned short vn;          /* ﾊﾞｰｼﾞｮﾝ:  */
    unsigned short model_code;  /* ﾓﾃﾞﾙｺｰﾄﾞ:  */
    unsigned short tn_line;     /* TN接続:  */
    unsigned short replay_line; /* 台ﾘﾌﾟﾚｲ機接続:  */
    unsigned short keisuu_line; /* 台計数機接続:  */
    unsigned short pointsys;    /* ﾎﾟｲﾝﾄｼｽﾃﾑ接続:  */
    unsigned short kaisvr;      /* 会員サーバ接続:  */
    unsigned short kaipc;       /* 会員ＰＣ接続:  */
    unsigned short cashsvr;     /* キャッシュサーバ接続:  */
    unsigned short jarnal;      /* ジャーナル接続:  */
    unsigned short hnsuu;       /* ホールナビ接続:  */
    unsigned short kugicho;     /* 釘調有無:  */
    unsigned short chodama;     /* 貯玉有無:  */
    unsigned short futureland;  /* ＦＬ有無:  */
	unsigned short rs;			/* 増設RS枚数 */
    unsigned short ss;          /* ＳＳ接続:  */
    unsigned short card_tan;    /* 台毎カード端末接続:  */
    unsigned short lecsvr;      /* LECサーバー接続:  */
    unsigned char yobi2[26];    /* 予備:  */
    unsigned short tenpo;       /* 店舗数:  */
    unsigned short mode;        /* 台番ﾓｰﾄﾞ: (0)49抜き･(1)連番  */
    unsigned short dai;         /* 台数:  */
    unsigned short tn;          /* TN数: 使わない  */
    unsigned short sima;        /* 島数: 使わない  */
    unsigned short floor;       /* ﾌﾛｱ数: 使わない  */
    unsigned short kinko;       /* 金庫数: 使わない  */
    unsigned short g_genkin;    /* 島外現金販売機数: 使わない  */
    unsigned short g_card;      /* 島外ｶｰﾄﾞ販売機数: 使わない  */
    unsigned short g_keihin;    /* 島外景品機数: 使わない  */
    unsigned short g_replay;    /* 島外ﾘﾌﾟﾚｲ機数: 使わない  */
    unsigned short g_premium;   /* 島外ﾌﾟﾚﾐｱﾑ機数: 使わない  */
    unsigned short g_cardh;     /* 島外ｶｰﾄﾞ発行機数: 使わない  */
    unsigned short pos;         /* POS数: 使わない  */
    unsigned short jihan;       /* 自販機数: 使わない  */
    unsigned short ta;          /* その他: 使わない  */
    unsigned short pnl_tab_mask1;/* パネルタブマスクbit: 各ビットがONの時タブを非表示にする(bit0:全部 bit1:遊戯 bit2:非遊戯 bit3:大当り bit4:異常 bit5:会員)  */
    unsigned short option1;     /* オプション１: dwtsk5が使用                       */
                                /*  ＝０  何も起こらない                            */
                                /*  ≠０  リカバリ処理が走る(が不具合ありで設定禁止)*/
    unsigned short option2;     /* オプション２: ダウンロード設定                   */
                                /*      0：通常( Lv1 -> Lv2 )                       */
                                /*      1：通常 + 立上りダウンロード ON             */
                                /*      2：Lv1 -> Lv2 -> Lv0                        */
                                /*      3：Lv0 only ( IPL1 )                        */
                                /*      4：Lv1 only ( IPL2 )                        */
                                /*      5：Lv2 only ( Appli )                       */
    unsigned short option3;     /* オプション３: 機器異常検知マスク詳細(ON時マスク) */
                                /*      bit 0：リプレイ異常                         */
                                /*      bit 1：リプレイ通信異常                     */
                                /*      bit 2：Tn I/O 通信異常                      */
                                /*      bit 3：Tn 通信異常                          */
                                /*      bit 4：Hn 通信異常                          */
                                /*      bit 5：D2 通信異常                          */
                                /*      bit 6：Dn 通信異常                          */
    unsigned short option4;     /* オプション４: カメラ接続設定                     */
                                /*  ＝０  ＦＴ仕様                                  */
                                /*  ＝１  松下仕様                                  */
                                /*  ＞１  ＦＴ仕様                                  */
    unsigned short option5;     /* オプション５: ＡＳ（ＳＨ３）会員ＬＯＧ設定       */
                                /*  ＝０  ＡＳ＃１に集中記憶                        */
                                /*  ＝１  ＡＳ＃１，２，３で個別記憶                */
    unsigned short option6;     /* オプション６: 音声２店舗管理                     */
                                /*  ＝０  １店舗管理                                */
                                /*  ≠０  ２店舗管理：２店舗目先頭台ＣＨ番号        */
    unsigned short option7;     /* オプション７: 音声２店舗管理                     */
                                /*  ＝０  １店舗管理                                */
                                /*  ≠０  ２店舗管理：２店舗目先頭全店ＣＨ番号      */
    unsigned short option8;     /* オプション８: 台ファイルの売上補正値の単位設定   */
                                /*  ＝０  １円単位                                  */
                                /*  ≠０  １００円単位                              */
#if	1	/* 2002.06.17(mon) chg.ohnot */
    unsigned short option9;     /* オプション９: reserve  */
    unsigned short option10;     /* オプション１０: reserve  */
    unsigned short option11;     /* オプション１１: reserve  */
    unsigned short option12;     /* オプション１２: reserve  */
    unsigned short option13;     /* オプション１３: reserve  */
    unsigned short option14;     /* オプション１４: reserve  */
    unsigned short option15;     /* オプション１５: reserve  */
    unsigned short option16;     /* オプション１６: reserve  */
#else
    unsigned char yobi[16];     /* 予備:  */
#endif
} fno1000_t;

#define		F1001_V1	0		/* ＤＮ３１台モデル    2000/02/18 Ohno */
#define		F1001_V2	1		/* ＤＮ２２２台モデル  2000/02/18 Ohno */

/* 台接続設定:  */
typedef struct _fno1001 {
    unsigned char rsdn_nid;/* Rs･Dn.NID: (b765=1-7)Rs.NID･(b43210=1-31)Dn.NID  */
    unsigned char tn_nid;       /* TN NID:  */
    unsigned char d2_nid;  /* D2 NID: (b765=1-4)ﾎﾟｰﾄ番号･(b43210=1-31)D2.NID  */
    unsigned char d2_subid;     /* D2 SUBID:  */
    unsigned short dno;         /* 台番号:  */
    unsigned char pas;          /* 種別:  */
    unsigned char kishu;        /* 機種番号:  */
    unsigned char sima;         /* 島番号:  */
    unsigned char type;         /* ﾀｲﾌﾟ番号:  */
    unsigned char floor;        /* ﾌﾛｱ番号:  */
    unsigned char sh_lamp;/* 島端ランプ出力: (bit0/4)ﾎﾟｰﾄ1 A/B・(bit1/5)ﾎﾟｰﾄ2 A/B・(bit2/6)ﾎﾟｰﾄ3 A/B・(bit3/7)ﾎﾟｰﾄ4 A/B  */
    unsigned char form;         /* 売上形態: (1)1:1･(2)2:1  */
    unsigned char kinko;        /* 金庫番号: (>0)金庫番号  */
    unsigned char urisyu1; /* 売上1種類: (1)現金･(2)現金金庫･(3)ｶｰﾄﾞ･(4)個数  */
        /*  1 = 現金         */          
        /*  2 = 現金金庫     */          
        /*  3 = ｶｰﾄﾞ         */          
        /*  4 = 個数         */          
    char scale1;                /* 売上1ｽｹｰﾗ:  */
    unsigned char urisyu2; /* 売上2種類: (1)現金･(2)現金金庫･(3)ｶｰﾄﾞ･(4)個数  */
    char scale2;                /* 売上2ｽｹｰﾗ:  */
    unsigned char urisyu3; /* 売上3種類: (1)現金･(2)現金金庫･(3)ｶｰﾄﾞ･(4)個数  */
    char scale3;                /* 売上3ｽｹｰﾗ:  */
} fno1001_t;


/* 全店接続設定:  */
typedef struct _fno1002 {
    unsigned char rsdn_nid;/* Rs･Dn.NID: (b765=1-7)Rs.NID･(b43210=1-31)Dn.NID  */
    unsigned char d2_nid;     /* D2 NID: (b765=1-4)ﾎﾟｰﾄ番号･(b43210=0)D2.NID  */
    unsigned char d2_pi;        /* D2 Pi入力Chno.:  */
    unsigned char d2_err;       /* D2 ｴﾗｰ入力Chno.:  */
    unsigned short disp1;       /* 表示番号1:  */
    unsigned short disp2;       /* 表示番号2:  */
    unsigned char floor;        /* ﾌﾛｱ番号:  */
    unsigned char kind;         /* 種類:  */
        /*   1 = 金庫                  */          
        /*   2 = 景品機                */          
        /*   3 = 現金販売機            */          
        /*   4 = ｶｰﾄﾞ販売機            */          
        /*   5 = ﾘﾌﾟﾚｲ玉               */          
        /*   6 = ﾌﾟﾚﾐｱﾑ玉              */          
        /*   7 = ｶｰﾄﾞ発行機            */          
        /*   8 = POS                   */          
        /*   9 = 自販機                */          
        /*  >9 = その他                */          
    unsigned char inp;          /* 入力:  */
        /*   0x01 = 計数 Pi            */          
        /*   0x02 = 断線 Di            */          
        /*   0x04 = ｴﾗｰ Pi            */          
        /*   0x08 = ﾄﾞｱ Di            */          
    unsigned char pas;          /* 種別:  */
    char scale;                 /* ｽｹｰﾗ:  */
    char yobi[3];               /* 予備:  */
} fno1002_t;


/* 島端ﾗﾝﾌﾟ設定:  */
typedef struct _fno1003 {
    unsigned char lamp1;/* 1番ﾗﾝﾌﾟ: (0)無･(1)呼出･(2)特賞１･(3)打止･(4)不正･(5)不正/異常･(6)ﾜｺﾞﾝ･(7)確変･(8)特賞２  */
    unsigned char lamp2;/* 2番ﾗﾝﾌﾟ: (0)無･(1)呼出･(2)特賞１･(3)打止･(4)不正･(5)不正/異常･(6)ﾜｺﾞﾝ･(7)確変･(8)特賞２  */
    unsigned char lamp3;/* 3番ﾗﾝﾌﾟ: (0)無･(1)呼出･(2)特賞１･(3)打止･(4)不正･(5)不正/異常･(6)ﾜｺﾞﾝ･(7)確変･(8)特賞２  */
    unsigned char yobi[5];      /* 予備:  */
} fno1003_t;


/* ﾀｲﾌﾟ設定:  */
typedef struct _fno1004 {
    unsigned char sbt;          /* 種別: */
    unsigned char sru;          /* ﾀｲﾌﾟ番号: 1～16  */
    unsigned char name[TYPEMEICHO];     /* ﾀｲﾌﾟ名:  */
} fno1004_t;


/* 音声基盤接続設定:  */
typedef struct _fno1005 {
    unsigned char onsei1;	/* フロア番号１・ホール用	(0)未接続 (1~16)送信先音声基板番号 */
    unsigned char onsei2;	/* フロア番号１・店員用		(0)未接続 (1~16)送信先音声基板番号 */
    unsigned char onsei3;	/* フロア番号２・ホール用	(0)未接続 (1~16)送信先音声基板番号 */
    unsigned char onsei4;	/* フロア番号２・店員用		(0)未接続 (1~16)送信先音声基板番号 */
    unsigned char onsei5;	/* フロア番号３・ホール用	(0)未接続 (1~16)送信先音声基板番号 */
    unsigned char onsei6;	/* フロア番号３・店員用		(0)未接続 (1~16)送信先音声基板番号 */
    unsigned char onsei7;	/* フロア番号４・ホール用	(0)未接続 (1~16)送信先音声基板番号 */
    unsigned char onsei8;	/* フロア番号４・店員用		(0)未接続 (1~16)送信先音声基板番号 */
    unsigned char onsei9;	/* フロア番号５・ホール用	(0)未接続 (1~16)送信先音声基板番号 */
    unsigned char onsei10;	/* フロア番号５・店員用		(0)未接続 (1~16)送信先音声基板番号 */
    unsigned char onsei11;	/* フロア番号６・ホール用	(0)未接続 (1~16)送信先音声基板番号 */
    unsigned char onsei12;	/* フロア番号６・店員用		(0)未接続 (1~16)送信先音声基板番号 */
    unsigned char onsei13;	/* フロア番号７・ホール用	(0)未接続 (1~16)送信先音声基板番号 */
    unsigned char onsei14;	/* フロア番号７・店員用		(0)未接続 (1~16)送信先音声基板番号 */
    unsigned char onsei15;	/* フロア番号８・ホール用	(0)未接続 (1~16)送信先音声基板番号 */
    unsigned char onsei16;	/* フロア番号８・店員用		(0)未接続 (1~16)送信先音声基板番号 */
} fno1005_t;


/* 店舗接続設定 */
typedef struct _fno1006 {
	unsigned char floor[8];				/* フロア店舗接続(８フロア) (0)未接続 (1)A店舗 (2)B店舗 */
	unsigned char pmode_store;			/* ポイント店舗合算 (0)店舗合算しない (1)店舗合算する */
	unsigned char pmode_type;			/* ポイント種別合算			*/
										/*		bit0=1:来店ポイント	*/
										/*		bit1=1:稼動ポイント	*/
										/*		bit2=1:売上ポイント	*/
	unsigned char dmy1[22];
} fno1006_t;


/* 種別ポケット設定 */
typedef struct _fno1007 {
	unsigned char	btsbt;				/* 物理種別 (0)未接続 (1)玉 (2)メダル */
	unsigned char	kmpkt;				/* KMポケット位置 (0)未使用 (1~8)位置 */
	unsigned char	absout;				/* アウト定数 */
	unsigned char	inpscl;				/* 入力比(1パルスに対する個数) */
	unsigned char	urikos;				/* 売上単位個数(個) */
	unsigned char	urikin;				/* 売上単位金額(100円単位) */
	unsigned char	rnsbt;				/* 論理種別 */
	unsigned char	dmy[9];
} fno1007_t;


/* 店舗・種別名称設定 */				/* 2002.11.29 add */
typedef struct _fno1008 {
	unsigned char	uni_name[12];			/* 合算名 */
	unsigned char	tenpo1[12];			/* 店舗名１ */
	unsigned char	tenpo2[12];			/* 店舗名２ */
	unsigned char	dmy1[36];			/* 予備 */
	unsigned char	sbt1[12];			/* 種別名１ */
	unsigned char	sbt2[12];			/* 種別名２ */
	unsigned char	sbt3[12];			/* 種別名３ */
	unsigned char	sbt4[12];			/* 種別名４ */
	unsigned char	sbt5[12];			/* 種別名５ */
	unsigned char	sbt6[12];			/* 種別名６ */
	unsigned char	sbt7[12];			/* 種別名７ */
	unsigned char	sbt8[12];			/* 種別名８ */
	unsigned char	dmy2[24];			/* 予備 */
} fno1008_t;


typedef fno1000_t FSET_SYS_SYS;
typedef fno1001_t FSET_SYS_DAI;
typedef fno1002_t FSET_SYS_ZEN;
typedef fno1003_t FSET_SYS_LAMP;
typedef fno1004_t FSET_SYS_TYPE;
typedef fno1005_t FSET_SYS_ONS;			/* 音線基板接続設定 */
typedef fno1006_t FSET_SYS_TENPO;		/* F1006 店舗接続設定 */
typedef fno1007_t FSET_SYS_POCKET;		/* F1007 種別ポケット設定 */

/* システム設定 オプション３ */
#define SYSOPT3_RPY_IJO		0x0001
#define SYSOPT3_RPYT_IJO	0x0002
#define SYSOPT3_TNIO_IJO	0x0004
#define SYSOPT3_TN_IJO		0x0008
#define SYSOPT3_HN_IJO		0x0010
#define SYSOPT3_D2_IJO		0x0020
#define SYSOPT3_DN_IJO		0x0040


/* ポイント種別合算フラグ	*/
#define	P_RAI_GBIT 0x01
#define P_KAD_GBIT 0x02
#define P_URI_GBIT 0x04




/* WAIS END */
#endif                          /* __set_sys_h__ */
