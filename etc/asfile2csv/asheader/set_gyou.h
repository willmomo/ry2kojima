/* WAIS START */
#ifndef __set_gyou_h__
#define __set_gyou_h__          /* 業務設定:  */

 /*    初期設定ﾌｧｲﾙ                                   */              /* 直接書込データ:  */
 /*    Version   Date      Programmer    Memo         */              /* 直接書込データ:  */
 /*    00000.0   97.04.07      y.I       新規作成     */              /* 直接書込データ:  */
 /*    構造体ﾂｰﾙにて再作成                            */              /* 直接書込データ:  */
 /*  1997/10/6 11:29 osako            */           /* 直接書込データ:  */
#include "nassys.h"
#define L_SET_GYOU

/* 暗証番号:  */
typedef struct _fno1100 {
    unsigned long ansyou_no;    /* 暗証番号:  */
    unsigned long dmy1;         /* 予備:  */
} fno1100_t;


/* 休日設定･ﾌﾛｱ番号:  */
typedef struct _holidayfloor {
    unsigned char flag;         /* 使用･未使用:  */
    unsigned char floor;        /* ﾌﾛｱNo:  */
    unsigned char date[2];      /* 月日:  */
} holidayfloor_t;


/* 開閉店時刻設定:  */
typedef struct _fno1101 {
// 2002/10/25 (店舗別休日対応)追加修正 Fukuyama -->
    unsigned char open_time[2]; /* 開店時刻:  */
    unsigned char close_time[2];/* 閉店時刻:  */
    unsigned char shut_time[2]; /* 締切時刻:  */
    unsigned char kioku_time[2];/* 記憶時刻:  */
    unsigned char clear_time[2];/* (ｸﾘｱ時刻):  */
    unsigned char getstop_time[2];/* (取込停止時刻):  */
    unsigned char revcany_time[2];/* 台予約取消予告時刻:  */
    unsigned char revcan_time[2];/* 台予約取消時刻:  */
    unsigned char open_time2[2]; /* 開店時刻(2店舗目):  */
    unsigned char close_time2[2];/* 閉店時刻(2店舗目):  */
    unsigned char shut_time2[2]; /* 締切時刻(2店舗目):  */
    unsigned char kioku_time2[2];/* 記憶時刻(2店舗目):  */
    unsigned char clear_time2[2];/* (ｸﾘｱ時刻)(2店舗目):  */
    unsigned char getstop_time2[2];/* (取込停止時刻)(2店舗目):  */
    unsigned char revcany_time2[2];/* 台予約取消予告時刻(2店舗目):  */
    unsigned char revcan_time2[2];/* 台予約取消時刻(2店舗目):  */
//    unsigned char yobi1[16];    /* *:  */
//--> 2002/10/25 (店舗別休日対応)追加修正 Fukuyama
    holidayfloor_t holiday[8];/* 休日設定･ﾌﾛｱ番号: (ﾌﾛｱ毎[0]〜[7]) ﾌﾛｱ=0:全ﾌﾛｱ  */
} fno1101_t;


/* 金庫点検設定:  */
typedef struct _fno1102 {
    unsigned short kinko_tenken;/* 金庫点検値: (ｻﾝﾄﾞ計 - 金庫計)  */
    unsigned short kinko_tentimer;/* 金庫点検ﾀｲﾏ:  */
} fno1102_t;


/* 全店停止設定:  */
typedef struct _fno1103 {
    unsigned short sanddan_stopfg;/* ｻﾝﾄﾞ断線: 停止ﾌﾗｸﾞ  */
        /*  (=1)許可･(=0)停止       */
        /*  (b0)検知                */
        /*  (b1)客音声              */
        /*  (b2)店音声              */
        /*  (b3)表示                */
        /*  (b4)印字                */
        /*  (b5)ｶﾒﾗ                 */
        /*  (b6)ｶﾒﾗ優先             */
        /*  ･…                     */
        /*  (b15)断線               */
    unsigned short sandijo_stopfg;/* ｻﾝﾄﾞ異常: 停止ﾌﾗｸﾞ  */
    unsigned short fusdan_stopfg;/* 不正断線: 停止ﾌﾗｸﾞ  */
    unsigned short daifus_stopfg;/* 台不正: 停止ﾌﾗｸﾞ  */
    unsigned short daiijo_stopfg;/* 台異常: 停止ﾌﾗｸﾞ  */
    unsigned short daiten_stopfg;/* 台点検: 停止ﾌﾗｸﾞ  */
    unsigned short kinkoten_stopfg;/* 金庫点検: 停止ﾌﾗｸﾞ  */
    unsigned short tanijo_stopfg;/* 端末異常: 停止ﾌﾗｸﾞ  */
    unsigned short revten_stopfg;/* 台予約点検: 停止ﾌﾗｸﾞ  */
    unsigned short risekiten_stopfg;/* 離席点検: 停止ﾌﾗｸﾞ  */
    unsigned short shokuten_stopfg;/* 食事点検: 停止ﾌﾗｸﾞ  */
    unsigned short zkeidan_stopfg;/* 全店計数断線: 停止ﾌﾗｸﾞ  */
    unsigned short zerror_stopfg;/* 全店ｴﾗｰ入力: 停止ﾌﾗｸﾞ  */
    unsigned short zdoor_stopfg;/* 全店ﾄﾞｱ入力: 停止ﾌﾗｸﾞ  */
    unsigned short zdoorten_stopfg;/* 全店ﾄﾞｱ点検: 停止ﾌﾗｸﾞ  */
#if 1		//2002.04.05 koketsu for WinSousa
    unsigned short daicall_stopfg;/* 台呼出: 停止ﾌﾗｸﾞ  */
    unsigned short daidoor_stopfg;/* 台ドア: 停止ﾌﾗｸﾞ  */
    unsigned short yobi[15];     /* 予備:  */
#else
    unsigned char yobi[34];     /* 予備:  */
#endif
} fno1103_t;


/* 機種停止設定:  */
typedef struct _fno1104 {
    unsigned short fus1_stopfg; /* 不正入力: 停止ﾌﾗｸﾞ  */
        /*  (=1)許可･(=0)停止       */
        /*  (b0)検知                */
        /*  (b1)客音声              */
        /*  (b2)店音声              */
        /*  (b3)表示                */
        /*  (b4)印字                */
        /*  (b5)ｶﾒﾗ                 */
        /*  (b6)ｶﾒﾗ優先             */
        /*  ･…                     */
        /*  (b15)断線               */
    unsigned short fus2_stopfg; /* 不正2入力: 停止ﾌﾗｸﾞ  */
    unsigned short door1_stopfg;/* ﾄﾞｱ入力: 停止ﾌﾗｸﾞ  */
    unsigned short door2_stopfg;/* ﾄﾞｱ2入力: 停止ﾌﾗｸﾞ  */
    unsigned short call_stopfg; /* 呼出: 停止ﾌﾗｸﾞ  */
    unsigned short uchi_stopfg; /* 打止: 停止ﾌﾗｸﾞ  */
    unsigned short kaihou_stopfg;/* 開放: 停止ﾌﾗｸﾞ  */
    unsigned short toku_stopfg; /* 特賞: 停止ﾌﾗｸﾞ  */
    unsigned short ijo_stopfg;  /* 異常停止: 停止ﾌﾗｸﾞ  */
    unsigned short safijo_stopfg;/* ｾｰﾌ異常: 停止ﾌﾗｸﾞ  */
    unsigned short doorijo_stopfg;/* ﾄﾞｱ異常: 停止ﾌﾗｸﾞ  */
    unsigned short tokuijo_stopfg;/* 特賞異常: 停止ﾌﾗｸﾞ  */
    unsigned short staijo_stopfg;/* ｽﾀｰﾄ異常: 停止ﾌﾗｸﾞ  */
    unsigned short outten_stopfg;/* ｱｳﾄ点検: 停止ﾌﾗｸﾞ  */
    unsigned short saften_stopfg;/* ｾｰﾌ･補給点検: 停止ﾌﾗｸﾞ  */
    unsigned short staten_stopfg;/* ｽﾀｰﾄ点検: 停止ﾌﾗｸﾞ  */
#if 1			// 2002.04.05 Upd koketsu for WinSousa
    unsigned short genkin_stopfg;	/* 現金停止ﾌﾗｸﾞ  */
    unsigned short card_stopfg;		/* カード停止ﾌﾗｸﾞ  */
    unsigned short replay_stopfg;	/* リプレイ停止ﾌﾗｸﾞ  */
    unsigned short kenbai_stopfg;	/* 券売機停止ﾌﾗｸﾞ  */
    unsigned char yobi[24];     	/* 予備:  */
#else
    unsigned short dmy1;        /* 予備:  */
    unsigned short dmy2;        /* 予備:  */
    unsigned char yobi[28];     /* 予備:  */
#endif
} fno1104_t;


/* 全店運用設定:  */
typedef struct _fno1105 {
    unsigned short kado_timersec;/* 稼動ﾀｲﾏ (*秒):  */
    unsigned short kado_timermin;/* 稼動ﾀｲﾏ (*秒):  */
    unsigned short shoku_timer;/* 食事ﾀｲﾏ (秒): (0)検知無し･(1-9999秒)設定は秒単位  */
    unsigned short riseki_timer;/* 離席ﾀｲﾏ (秒): (0)検知無し･(1-9999秒)設定は秒単位  */
    unsigned short revcan_timer;/* 予約取消ﾀｲﾏ: (0)検知無し･(1-9999秒)設定は秒単位  */
    unsigned short fuscan_timer;/* 不正解除ﾀｲﾏ: (0)検知無し･(1-9999秒)設定は秒単位  */
    unsigned short callten_timer;/* 呼出点検ﾀｲﾏ: (0)検知無し･(1-9999秒)  */
    unsigned short kai_hikado_timer;/* 会員非稼動点検ﾀｲﾏ: (0)検知無し･(1-9999秒)  */
    unsigned short uriijo_sagaku;/* 売上個数異常: 差額値 (金額-個数)  */
    unsigned short uriijo_kingaku;/* 売上異常  金額値: 金額値  */
    unsigned short uriijo_timer;/* 売上異常  ﾀｲﾏ値: ﾀｲﾏ値  */
    unsigned short t_uriijo_kingaku;/* 特賞1中売上異常: 金額値  */
    unsigned short ukomi;       /* 打込玉: 打込玉  */
    unsigned short sg_doorten_timer;/* 全店ﾄﾞｱ点検ﾀｲﾏ: (0-255)秒単位  */
    unsigned short sg_errten_timer;/* 島外ｴﾗｰ解除ﾀｲﾏ: (0)検知無し･(1-9999秒)設定は秒単位  */
    unsigned short kai_kado_timer;/* 会員稼動ﾀｲﾏ: (0:60秒)(1-999秒)  */
    unsigned short kai_card_timer;/* 会員ｶｰﾄﾞOUTﾀｲﾏ: (0:60秒)(1-999秒)  */
    unsigned char yobi2[94];    /* 予備:  */
} fno1105_t;


/* 機種運用設定:  */
typedef struct _fno1106 {
    unsigned char pas;          /* 種別:  */
    unsigned char type;         /* ﾀｲﾌﾟ:  */
    unsigned short uchi;        /* 打止: 打止値  */
    unsigned short uchifg;      /* 打止: 条件ﾌﾗｸﾞ  */
        /*  (b0=1)自動打止台        */
        /*  (b1=1)T2中打止停止      */
    unsigned short kaihou_timer;/* 開放: 開放ﾀｲﾏ (秒)  */
        /*  (=0)自動開放しない      */
        /*  (=1)即開放              */
        /*  (2-999秒)開放ﾀｲﾏ        */
    unsigned short dmy1;        /* 予備:  */
    unsigned short tohosei;     /* 特賞: T0補正値  */
    unsigned short tokufg;      /* 特賞: 条件ﾌﾗｸﾞ  */
        /*  (b0=1)特賞ﾍﾞｰｽ          */
        /*  (b1=1)T2中音声停止      */
        /*  (b2=1)T2中特賞出力      */
    unsigned short safijo_saf;  /* ｾｰﾌ異常: ｾｰﾌ値  */
    unsigned short safijo_out;  /* ｾｰﾌ異常: ｱｳﾄ値  */
    unsigned short safijo_timer;/* ｾｰﾌ異常: ﾀｲﾏ値  */
    unsigned short tcntijo_bo;  /* 特賞回数異常: BO  */
    unsigned short tcntijo_t1;  /* 特賞回数異常: T1回数  */
    unsigned short tdamaijo_tymax;/* 特賞出玉異常: Tymax値  */
    unsigned short staijo_start;/* ｽﾀｰﾄ異常: ｽﾀｰﾄ値  */
    unsigned short staijo_out;  /* ｽﾀｰﾄ異常: ｱｳﾄ値  */
    unsigned short staijo_timer;/* ｽﾀｰﾄ異常: ﾀｲﾏ値  */
    unsigned short outten_saf;  /* ｱｳﾄ点検: ｾｰﾌ値  */
    unsigned short outten_start;/* ｱｳﾄ点検: ｽﾀｰﾄ値  */
    unsigned short saften_out;  /* ｾｰﾌ点検: ｱｳﾄ値  */
    unsigned short saften_start;/* ｾｰﾌ点検: ｽﾀｰﾄ値  */
    unsigned short saften_t1out;/* ｾｰﾌ点検(T1中): T1ｱｳﾄ値  */
    unsigned short tdamaten_tymin;/* 特賞出玉点検: Tymin値  */
    unsigned short staten_saf;  /* ｽﾀｰﾄ点検: ｾｰﾌ値  */
    unsigned short staten_out;  /* ｽﾀｰﾄ点検: ｱｳﾄ値  */
#if 0				// 2002.04.05 Upd. koketsu for Win操作
    unsigned short dmy2;        /* 予備:  */
    unsigned short dmy3;        /* 予備:  */
    unsigned char yobi1[12];    /* 予備:  */
#else
    unsigned char  stslump_ss1; /* ｽﾀｰﾄｽﾗﾝﾌﾟ s/s1選択 */
    unsigned char  stslump_so; 	/* ｽﾀｰﾄｽﾗﾝﾌﾟ so値 	*/
    unsigned short stslump_bo; /* ｽﾀｰﾄｽﾗﾝﾌﾟBO検索値 */
    unsigned short stslump_sbst; /* ｽﾀｰﾄｽﾗﾝﾌﾟｽﾀｰﾄ分布範囲開始値*/
    unsigned char  stslump_sbkn; /* ｽﾀｰﾄｽﾗﾝﾌﾟｽﾀｰﾄ分布範囲検索値*/
    unsigned char  stslump_sbbn; /* ｽﾀｰﾄｽﾗﾝﾌﾟｽﾀｰﾄ分布範囲分割値*/
    unsigned short next_uti; 	/* 次回打止値*/
// 2002/09/18 追加修正 Fukuyama -->
	unsigned char  outstart_out; /* ｱｳﾄ定数ｽﾀｰﾄ関連ｱｳﾄ定数*/
	unsigned char  stslump_sharai; /* ｱｳﾄ定数ｽﾀｰﾄ関連ｽﾀｰﾄ払出数*/// バウンダリ修正 (小島)
    unsigned short outstart_start; /* ｱｳﾄ定数ｽﾀｰﾄ関連ｽﾀｰﾄ定数*/
    unsigned char yobi1[2];    /* 予備:  */
// --> 2002/09/18 追加修正 Fukuyama
#endif
    unsigned char yobi2[64];    /* 予備:  */
} fno1106_t;


/* D2設定:  */
typedef struct _fno1107 {
    unsigned char setei1;       /* 設定ﾃﾞｰﾀ1:  */
        /*  種別         b7   (0)P台･(1)AS台                       */
        /*  呼出SW入力   b6   (0)ｵﾙﾀﾈｰﾄSW･(1)ﾓｰﾒﾝﾀﾘSW              */
        /*  ｽﾀｰﾄ検知     b5   (0)ON検知･(1)OFF検知                 */
        /*  S2･S3作成    b4   (0)無:通常ｽﾀｰﾄ入力･(1)有:自動判別    */
    unsigned char setei2;       /* 設定ﾃﾞｰﾀ2:  */
        /*  ｽﾀｰﾄﾀｲﾏ(秒)     0.2秒単位  (ｽﾀｰﾄOFF検知時1/10表示)    */
    unsigned char setei3;       /* 設定ﾃﾞｰﾀ3:  */
        /*  判別ﾀｲﾏ(秒)     0.02秒単位    */
    unsigned char setei4;       /* 設定ﾃﾞｰﾀ4:  */
        /*  T1ﾀｲﾏ        b5-0     (0-31)秒単位     */
    unsigned char setei5;       /* 設定ﾃﾞｰﾀ5:  */
        /*  T1遅延ﾀｲﾏ    b0-7     (0-255)秒単位    */
    unsigned char setei6;       /* 設定ﾃﾞｰﾀ6:  */
        /*  T2ﾀｲﾏ        b5-0     (0-31)秒単位     */
    unsigned char setei7;       /* 設定ﾃﾞｰﾀ7:  */
        /*  T2遅延ﾀｲﾏ    b0-7     (0-255)秒単位    */
    unsigned char setei8;       /* 設定ﾃﾞｰﾀ8:  */
        /*  ﾄﾞｱ異常ﾀｲﾏ   b0123:瞬間ﾄﾞｱｵｰﾌﾟﾝ    (0)検知なし･(1-15)0.1秒単位     */
        /*  ﾄﾞｱ特賞ﾀｲﾏ   b4567:ﾄﾞｱｵｰﾌﾟﾝ特賞    (0)検知なし･(1-15)1秒単位       */
    unsigned char setei9;       /* 設定ﾃﾞｰﾀ9:  */
        /*  ﾄﾞｱ点検ﾀｲﾏ   b0-7     (0)検知なし･(1-225)1秒単位       */
    unsigned char setei10;      /* 設定ﾃﾞｰﾀ10:  */
        /*  補給点検ﾀｲﾏ  b0-7     (0)検知なし･(1-225)1秒単位       */
    unsigned char setei11;      /* 設定ﾃﾞｰﾀ11:  */
    unsigned char setei12;      /* 設定ﾃﾞｰﾀ12:  */
    unsigned char setei13;      /* 設定ﾃﾞｰﾀ13:  */
    unsigned char setei14;      /* 設定ﾃﾞｰﾀ14:  */
        /*  入力選択1    b0123    (0)ﾄﾞｱ1･(1)ﾄﾞｱ1･(2)不正1･(3)呼出1･(4)ﾄﾞｱ2･(5)不正2･(6)呼出2･(7)補給     */
        /*  入力選択2    b4567    (0)不正1･(1)ﾄﾞｱ1･(2)不正1･(3)呼出1･(4)ﾄﾞｱ2･(5)不正2･(6)呼出2･(7)補給    */
    unsigned char setei15;      /* 設定ﾃﾞｰﾀ15:  */
        /*  入力選択3    b0123    (0)呼出1･(1)ﾄﾞｱ1･(2)不正1･(3)呼出1･(4)ﾄﾞｱ2･(5)不正2･(6)呼出2･(7)補給    */
        /*  *                                                                                             */
    unsigned char setei16;      /* 設定ﾃﾞｰﾀ16:  */
        /*  出力1設定    b0123    (=0)打止中･(=1)T1中･(=2)T1+T2中･(3)T2中    */
        /*  出力2設定    b4567    (=0)打止中･(=1)T1中･(=2)T1+T2中･(3)T2中    */
} fno1107_t;

typedef struct _fno1108_tenpo_t {
	unsigned char	t_holiday;			/* 00:営業日FLAG (bit0=1)開店日,(bit0=0)休店日 */
	unsigned char	dmy[3];				/* *:  */
	unsigned char	open_time[2];		/* 開店時刻:  */
	unsigned char	close_time[2];		/* 閉店時刻:  */
	unsigned char	shut_time[2];		/* 締切時刻:  */
	unsigned char	kioku_time[2];		/* 記憶時刻:  */
	unsigned char	clear_time[2];		/* (ｸﾘｱ時刻):  */
	unsigned char	getstop_time[2];	/* (取込停止時刻):  */
	unsigned char	revcany_time[2];	/* 台予約取消予告時刻:  */
	unsigned char	revcan_time[2];		/* 台予約取消時刻:  */
} fno1108_tenpo_t;

/* 店舗別変則営業設定:  */
typedef struct _fno1108 {
	unsigned long	eigyo_pw_tim;		/* 営業日付: 営業時の月日時刻 年･月･日･時･分･秒 = bit7･4･5･5･6･5  1910+年 0=日曜  */
	unsigned char	dmy[12];				/* *:  */
	fno1108_tenpo_t tenpo[2];
} fno1108_t;


/* 開閉店動作テーブル */
typedef fno1108_tenpo_t fno4824_t;

#if 0
/* 店舗毎時刻設定 */
typedef struct _fno1108_d1 {
	unsigned char	t_holiday;		/* 営業日フラグ (bit7=1)開店日,(bit7=0)休店日 */
	unsigned char	t_dmy2;
	unsigned char	t_clear_hm[2];		/* (将来)クリア時刻([0]時 [1]分) */
	unsigned char	t_open_hm[2];		/* 開店時刻([0]時 [1]分) */
	unsigned char	t_close_hm[2];		/* 閉店時刻([0]時 [1]分) */
	unsigned char	t_sime_hm[2];		/* 締切時刻([0]時 [1]分) */
	unsigned char	t_kioku_hm[2];		/* 自動記憶時刻([0]時 [1]分) */
	unsigned char	t_tstop_hm[2];		/* (将来)取込停止時刻([0]時 [1]分) */
	unsigned char	t_opmae_hm[2];		/* 開店前警告時刻([0]時 [1]分) */
	unsigned char	t_dmy3[8];
} fno1108_d1_t;

typedef struct _fno1108_d2 {
	unsigned char	t_holiday;		/* 営業日フラグ (bit7=1)開店日,(bit7=0)休店日 */
	unsigned char	t_dmy2;		
	unsigned short	t_clear_hm;		/* (将来)クリア時刻(bit15-8=時 bit7-0=分) */
	unsigned short	t_open_hm;		/* 開店時刻(bit15-8=時 bit7-0=分) */
	unsigned short	t_close_hm;		/* 閉店時刻(bit15-8=時 bit7-0=分) */
	unsigned short	t_sime_hm;		/* 締切時刻(bit15-8=時 bit7-0=分) */
	unsigned short	t_kioku_hm;		/* 自動記憶時刻(bit15-8=時 bit7-0=分) */
	unsigned short	t_tstop_hm;		/* (将来)取込停止時刻(bit15-8=時 bit7-0=分) */
	unsigned short	t_opmae_hm;		/* 開店前警告時刻(bit15-8=時 bit7-0=分) */
	unsigned char	t_dmy3[8];
} fno1108_d2_t;

typedef union _fno1108_tenpo {
	fno1108_d1_t a;
	fno1108_d2_t b;
} fno1108_tenpo_t;

/* カレンダー設定 */
typedef struct _fno1108 {
	unsigned char	month;			/* 月 */
	unsigned char	day;			/* 日 */
	unsigned char	dmy[6];
	fno1108_tenpo_t	f1108_tenpo[8];		/* 店舗別時刻８グループ */
} fno1108_t;
#endif

typedef fno1100_t FSET_GYOU_ANSYOU;
typedef fno1101_t FSET_GYOU_OPCL;
typedef fno1102_t FSET_GYOU_KINKO;
typedef fno1103_t FSET_GYOU_ZENTEI;
typedef fno1104_t FSET_GYOU_KSHUTEI;
typedef fno1105_t FSET_GYOU_ZEN;
typedef fno1106_t FSET_GYOU_KSHU;
typedef fno1107_t FSET_GYOU_D2;
typedef fno1108_t FSET_GYOU_HENSOKU; //←2002/10/29 (店舗別変則営業対応)追加 Fukuyama
#endif                          /* __set_gyou_h__ */
/* WAIS END */
