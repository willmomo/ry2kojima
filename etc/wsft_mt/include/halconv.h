#ifndef __halconv_h__
#define __halconv_h__

//DLL関数のオプション
#define HC_OPT_BODY 0
#define HC_OPT_HEAD 1

//エラー定数
#define HALCONVERR_WRITE 0x1002		//書き込みエラー

//ファイル番号定数
#define R_SYSTEM_IJO	4006
#define R_CLEAR			4007
#define R_TIMESET		4008
#define R_TEIDEN		4009
#define R_SETTEI		4010
#define R_IJO			4022
#define R_UTIDOME		4023
#define R_HASSEI		4024
#define I_KIOKU			4101
#define F_DAI			4102
#define F_ZENTEN		4103
#define F_SHIMA			4104
#define F_KISHU			4105
#define F_SHUBETSU		4106
#define F_TOTAL			4107
#define F_KINKO			4108
#define I_TEIJI			4109
#define F_T_SHUBETSU	4110
#define F_T_ZENTEN		4111
#define I_T_KADO		4112
#define F_T_SBT_KADO	4113
#define F_T_ZEN_KADO	4114
#define F_T_KISHU_KADO	4115
#define F_T_DSP_DATA	4117

//関数名定数
const char FuncNames[2][12] = { "hc_raw2vb()", "hc_vb2raw()" };

//エラーメッセージテーブル
const char MemFiles[23][27] = {
	"システム異常履歴", "クリア履歴", "時計設定履歴", "停電履歴", "設定履歴", "異常履歴",
	"打止履歴", "発生履歴", "当日・記憶情報", "当日・台ファイル", "当日・全店ファイル", "当日・島ファイル",
	"当日・機種ファイル", "当日・種別集計", "当日・全店集計ファイル", "当日・金庫集計", "定時情報", "当日・定時種別集計",
	"当日・定時全店集計", "当日・定時稼動情報", "当日・定時種別稼動ファイル", "当日・定時全店稼動ファイル", "当日・定時機種稼動ファイル"
};



/*	AS用構造体	*/

/****************************************/
/*	ｼｽﾃﾑ異常履歴(4006)					*/
/****************************************/
typedef struct {					/* ﾍｯﾀﾞﾚｺｰﾄﾞ			*/
	unsigned short	cnt;			/* ｼｽﾃﾑ異常表示更新回数		*/
	unsigned short	dmy[3];
} REKI_SYSIJO_HED;

typedef struct {					/* ﾃﾞｰﾀﾚｺｰﾄﾞ			*/
	unsigned long	sysijyo_pw_tim;		/* 年月日曜日時分		*/
	unsigned char 	code1;			/* 発生ｺｰﾄﾞ1			*/
	unsigned char 	code2;			/* 発生ｺｰﾄﾞ2			*/
	unsigned short	no;				/* 番号(CH)			*/
} REKI_SYSIJO;


/****************************************/
/*	ｸﾘｱ履歴(4007)						*/
/****************************************/
typedef struct {					/* ﾍｯﾀﾞﾚｺｰﾄﾞ			*/
	unsigned long	clr_pw_tim;		/* ｸﾘｱ時刻			*/
	unsigned char 	code1;			/* 発生ｺｰﾄﾞ1			*/
	unsigned char 	code2;			/* 発生ｺｰﾄﾞ2			*/
	unsigned short	dmy[3];			/* yobi */
	unsigned long	urikin;			/* ｸﾘｱ時の売上金額(100円単位	*/
} REKI_CLEAR;

/****************************************/
/*	時計設定履歴(4008)					*/
/****************************************/
typedef struct {					/* ﾃﾞｰﾀﾚｺｰﾄﾞ			*/
	unsigned long	mae_pw_tim;		/* 前  年月日曜日時分		*/
	unsigned char 	code1;			/* 発生ｺｰﾄﾞ1			*/
	unsigned char 	code2;			/* 発生ｺｰﾄﾞ2			*/
	unsigned short	dmy[3];			/* yobi */
	unsigned long	set_pw_tim;		/* 設定年月日曜日時分		*/
} REKI_TIMSET;

/****************************************/
/*	停電履歴(4009)						*/
/****************************************/
typedef struct {					/* ﾃﾞｰﾀﾚｺｰﾄﾞ			*/
	unsigned long	pwon_pw_tim;	/* ＯＮ年月日曜日時分	*/
	unsigned char 	code1;			/* 発生ｺｰﾄﾞ1			*/
	unsigned char 	code2;			/* 発生ｺｰﾄﾞ2			*/
	unsigned short	dmy[2];
	unsigned char 	eflg;			/* 停電ﾌﾗｸﾞ (1)営業中 (0)閉店中	*/
	unsigned char 	dmy1;			/*				*/
	unsigned long	pwoff_pw_tim;	/* 停電年月日曜日時分		*/
} REKI_TEIDEN;

/****************************************/
/*	設定履歴(4010)						*/
/****************************************/
typedef struct {					/* ﾃﾞｰﾀﾚｺｰﾄﾞ			*/
	unsigned long	pw_tim;			/* 設定年月日曜日時分	*/
	unsigned char 	hcd1;			/* 発生ｺｰﾄﾞ1			*/
	unsigned char 	hcd2;			/* 発生ｺｰﾄﾞ2			*/
	unsigned short	set_fno[5];		/* 設定ﾌｧｲﾙ番号			*/
} REKI_SETTEI;


/****************************************/
/*	異常履歴(4022)						*/
/****************************************/
typedef struct {					/* ﾍｯﾀﾞﾚｺｰﾄﾞ			*/
	unsigned short	cnt;			/* 異常表示更新回数		*/
	unsigned short	hasei;			/* 異常発生回数			*/
	unsigned short	suu;			/* 異常中件数			*/
	unsigned short	dmy[5];			/* yobi	*/
} REKI_IJO_HED;

typedef struct {					/* ﾃﾞｰﾀﾚｺｰﾄﾞ			*/
	unsigned char 	code1;			/* 発生ｺｰﾄﾞ1			*/
	unsigned char 	code2;			/* 発生ｺｰﾄﾞ2			*/
	unsigned char 	htime[2];		/* 発生時刻			*/
	unsigned short	no;				/* 番号(CH)	(0:ｴﾝﾄﾞﾚｺｰﾄﾞ)	*/
	unsigned char 	jflg[2];		/* 状態ﾌﾗｸﾞ			*/
	unsigned short	tflg;			/* 停止ﾌﾗｸﾞ			*/
	unsigned short	cnt;			/* 回数･値			*/
	unsigned char 	dmy[2];
	unsigned char 	ftime[2];		/* 復旧時刻			*/
} REKI_IJO;

/****************************************/
/*	打止履歴(4023)						*/
/****************************************/
typedef struct {					/* ﾍｯﾀﾞﾚｺｰﾄﾞ			*/
	unsigned short	cnt;			/* 打止表示更新回数		*/
	unsigned short	hasei;			/* 打止発生回数			*/
	unsigned short	suu;			/* 打止中台数			*/
	unsigned short	dmy[3];
} REKI_UTI_HED;

typedef struct {					/* ﾃﾞｰﾀﾚｺｰﾄﾞ			*/
	unsigned char 	code1;			/* 発生ｺｰﾄﾞ1			*/
	unsigned char 	code2;			/* 発生ｺｰﾄﾞ2			*/
	unsigned char 	time[2];		/* 発生時刻			*/
	unsigned short	no;				/* 番号		(0:ｴﾝﾄﾞﾚｺｰﾄﾞ)	*/
	unsigned char 	jflg[2];		/* 状態ﾌﾗｸﾞ			*/
	unsigned short	tflg;			/* 停止ﾌﾗｸﾞ			*/
	unsigned short	cnt;			/* 回数･値			*/
} REKI_UTI;

/****************************************/
/*	発生履歴(4024)			*/
/****************************************/
typedef struct {			/* ﾍｯﾀﾞﾚｺｰﾄﾞ			*/
	unsigned short	cnt;			/* 発生履歴表示更新回数		*/
	unsigned short	wpos;			/* 発生履歴書込みﾎﾟｲﾝﾀ		*/
	unsigned short	dmy[4];
} REKI_HASSEI_HED;

typedef struct {			/* ﾃﾞｰﾀﾚｺｰﾄﾞ			*/
	unsigned char 	code1;			/* 発生ｺｰﾄﾞ1			*/
	unsigned char 	code2;			/* 発生ｺｰﾄﾞ2			*/
	unsigned char 	time[2];		/* 発生時刻			*/
	unsigned short	no;			/* 番号		(0:ｴﾝﾄﾞﾚｺｰﾄﾞ)	*/
	unsigned char 	jflg[2];		/* 状態ﾌﾗｸﾞ			*/
	unsigned short	tflg;			/* 停止ﾌﾗｸﾞ			*/
	unsigned short	cnt;			/* 回数･値			*/
} REKI_HASSEI;

/*#**************************************/
/*	記憶情報(4101)			*/
/****************************************/
typedef struct {
	USHORT	sync;			/* ｼﾝｸｺｰﾄﾞ	0x87cd			*/
	UCHAR	kioku_index;	/* 記憶ﾌﾞﾛｯｸ.no				*/
	UCHAR	kioku_fg;		/* 記憶ﾌﾗｸﾞ	(b7=1)使用中	*/
							/*		(b6=1)ｸﾘｱ		*/
							/*		(b1=1)ﾒﾝﾃ		*/
							/*		(b0=1)CRCｴﾗｰ		*/
	UCHAR	emark;			/* 営業ﾏｰｸ	(b7)ｸﾘｱ済		*/
							/*		(b6)開店済		*/
							/*		(b5)閉店済		*/
							/*		(b4)xxx			*/
							/*		(b3)記憶済		*/
							/*		ｸﾘｱで=0x80		*/
	UCHAR	dmy1[3];
	ULONG	down_pw_tim;		/* 停電時刻				*/
	ULONG	pwon_pw_tim;		/* 電源ON時刻				*/
	ULONG	clear_pw_tim;		/* ｸﾘｱ時刻				*/
	ULONG	open_pw_tim;		/* 開店時刻	(平均時:開始日付)	*/
	ULONG	close_pw_tim;		/* 閉店時刻	(平均時:最終日付)	*/
	ULONG	sime_pw_tim;		/* 締切時刻				*/
	ULONG	kioku_pw_tim;		/* 記憶時刻				*/
	USHORT	eigyou_min_cnt;		/* 営業時間		ｸﾘｱ･開店で=0	*/
	UCHAR	clear_cnt;			/* ｸﾘｱ回数		記憶後の最初のｸﾘｱで =1	*/
	UCHAR	clear_cnt_0;		/* ｸﾘｱ回数(開店前)	記憶後の最初のｸﾘｱで =1	*/
	UCHAR	clear_cnt_1;		/* ｸﾘｱ回数(開店-記憶)	記憶後の最初のｸﾘｱで =1	*/
	UCHAR	down_cnt;			/* 停電回数		ｸﾘｱで=0		*/
	UCHAR	down_cnt_1;			/* 停電回数(ｸﾘｱ-記憶)	ｸﾘｱで=0		*/
	UCHAR	down_cnt_2;			/* 停電回数(記憶-)	ｸﾘｱで=0		*/
	UCHAR	kioku_cnt;			/* 記憶回数		ｸﾘｱ後の最初の記憶で =1	*/
	UCHAR	dmy2[3];
	ULONG	settei_ps_tim;		/* 設定更新時刻				*/
	ULONG	now_pw_tim;			/* 現在時刻 (定時処理時刻) 停電時刻作成	*/
	UCHAR	dmy3[8];
	struct TENPO_KIOKU_JYOHO {			/* AS(SH3)店舗記憶情報	*/
		unsigned char	t_mark;			/* 00:店舗:(=$41)A店,(=$42)B店	*/
		unsigned char	t_holiday;		/* 01:営業日FLAG (bit7=1)開店日,(bit7=0)休店日	*/
		unsigned short	t_eday_md;		/* 02:営業日付：bit15-8=月,bit7-0=日	*/
		unsigned short	t_clear_hm;		/* 04:クリア時刻：bit15-8=時,bit7-0=分	*/
		unsigned short	t_open_hm;		/* 06:開店時刻：bit15-8=時,bit7-0=分	*/
		unsigned short	t_clos_hm;		/* 08:閉店時刻：bit15-8=時,bit7-0=分	*/ 
		unsigned short	t_sime_hm;		/* 0a:締切時刻：bit15-8=時,bit7-0=分	*/
		unsigned short	t_eigyo_m;		/* 0c:営業時間：累積分					*/
		unsigned char	t_dmy2[2];		/* 0e:	*/
	} tenpo_kioku_jyoho[2];				/* 10:	*/
	unsigned char	dmy4[160];			/* 160: */

} INF_KIOKU;

/*#**************************************/
/*	台ﾃﾞｰﾀ(累計)(4102)			*/
/****************************************/
typedef	struct {
	USHORT	jflg;			/* 台情報ﾌﾗｸﾞ			*/
							/*    0x0001 : 特賞ﾍﾞｰｽ		*/
							/*    0x0002 : ﾃﾞｰﾀ設定		*/
							/*    0x0004 : 集計停止		*/
							/*    0x0008 : 稼動中		*/
							/*    0x0010 : 会員稼動中	*/
	USHORT	out;			/* ｱｳﾄ				*/
	USHORT	saf;			/* ｾｰﾌ				*/
	USHORT	start;			/* ｽﾀｰﾄ				*/
	USHORT	tout;			/* 特賞ｱｳﾄ			*/
	USHORT	tsaf;			/* 特賞ｾｰﾌ			*/
	USHORT	t2out;			/* 特賞2ｱｳﾄ			*/
	USHORT	t2saf;			/* 特賞2ｾｰﾌ			*/
	USHORT	t2start;		/* 特賞2ｽﾀｰﾄ			*/
	USHORT	bcnt;			/* ﾍﾞｰｽ回数(特賞回数/打止回数)	*/
	USHORT	ucnt;			/* 打止回数			*/
	USHORT	tcnt;			/* 特賞回数  (T)		*/
	USHORT	t1cnt;			/* 特賞1回数 (T1)		*/
	USHORT	t2cnt;			/* 特賞2回数 (T2)		*/
	USHORT	kcnt;			/* 確変回数((T2)>T1>T2>T1･･･)	*/
	USHORT	kt1cnt;			/* 確変中T1回数			*/

	USHORT	lcnt;			/* ﾗｯｷｰ回数			*/
	USHORT	lt1cnt;			/* ﾗｯｷｰ中特賞(T1)回数		*/
	USHORT	lt1max;			/* ﾗｯｷｰ中MAX特賞(T1)回数	*/
	USHORT	lout;			/* ﾗｯｷｰ中ｱｳﾄ			*/
	USHORT	lsaf;			/* ﾗｯｷｰ中ｾｰﾌ			*/
	USHORT	lbout;			/* ﾗｯｷｰ特賞(T1)間ｱｳﾄ		*/
	USHORT	lmoti;			/* ﾗｯｷｰ中持玉			*/

	USHORT	start1;			/* ｽﾀｰﾄ1			*/
	USHORT	start2;			/* ｽﾀｰﾄ2			*/
	USHORT	start3;			/* ｽﾀｰﾄ3			*/
	USHORT	t2start1;		/* 特賞2(T2)中ｽﾀｰﾄ1		*/
	USHORT	t2start2;		/* 特賞2(T2)中ｽﾀｰﾄ2		*/
	USHORT	t2start3;		/* 特賞2(T2)中ｽﾀｰﾄ3		*/
	USHORT	t1start2;		/* 特賞ｽﾀｰﾄ2回数		*/

	USHORT	ukomi;			/* 打込玉			*/
	USHORT	t1ukomi;		/* 特賞打込			*/
	short	minsa;			/* 最小差			*/
	short	maxsa;			/* 最大差			*/
	USHORT	minout;			/* 最小差ｱｳﾄ			*/
	USHORT	maxout;			/* 最大差ｱｳﾄ			*/
	USHORT	maxmoti;		/* 最大持玉 MY			*/
	USHORT	maxukomi;		/* 最大打込 M◇			*/

	USHORT	kadofg;			/* 定時稼動ﾌﾗｸﾞ(8-23)		*/
	UCHAR	kado;			/* 積稼動数			*/
	UCHAR	kyaku;			/* 客交代			*/

	USHORT	hasfg;			/* 発生済ﾌﾗｸﾞ			*/
						/*    0x0001 : 台不正		*/
						/*    0x0002 : 台断線		*/
						/*    0x0004 : 台異常		*/
						/*    0x0008 : 台点検		*/
						/*    0x0100 : 売上不正		*/
						/*    0x0200 : 売上断線		*/
						/*    0x0400 : 売上異常		*/
						/*    0x0800 : 売上点検		*/
	UCHAR	door1_2;	/* ﾄﾞｱ1回数		0xf0	*/
						/* ﾄﾞｱ2回数		0x0f	*/
	UCHAR	call1_2;	/* 呼出1回数		0xf0	*/
						/* 呼出2回数		0x0f	*/
	UCHAR	fus_ddan;	/* 不正回数		0xf0	*/
						/* 台断線回数		0x0f	*/
	UCHAR	ijo_ten;	/* 異常回数		0xf0	*/
						/* 点検回数		0x0f	*/
	UCHAR	g1_g2dan;	/* 現金1断線回数	0xf0	*/
						/* 現金2断線回数	0x0f	*/
	UCHAR	cddan_uijo;		/* ｶｰﾄﾞ断線回数		0xf0	*/
							/* 売上異常回数		0x0f	*/

	UCHAR	ukind;			/* 売上情報ﾌﾗｸﾞ			*/
						/*     0x01 : ｻﾝﾄﾞ現金1有り	*/
						/*     0x02 : ｻﾝﾄﾞ現金2有り	*/
						/*     0x04 : ｶｰﾄﾞ売上有り	*/
						/*     0x10 : ｻﾝﾄﾞ現金1金庫	*/
						/*     0x20 : ｻﾝﾄﾞ現金2金庫	*/
	UCHAR	dmy1;			/* ....				*/
	USHORT	gen1uri;		/* ｻﾝﾄﾞ現金1売上(100円)		*/
	USHORT	gen2uri;		/* ｻﾝﾄﾞ現金2売上(100円)		*/
	USHORT	carduri;		/* ｻﾝﾄﾞｶｰﾄﾞ売上(100円)		*/
	USHORT	daiuri;			/* 台売上(100円)		*/
	USHORT	hosei;			/* 売上補正値(円)		*/

	UCHAR	k_suu;			/* 会員数			*/
	UCHAR	k_kado;			/* 会員稼動数			*/
	USHORT	k_kadofg;		/* 会員定時稼動ﾌﾗｸﾞ(8-23)	*/
	UCHAR	k_t1cnt;		/* 会員特賞1回数 (T1)		*/
	UCHAR	k_tcnt;			/* 会員特賞回数	 (T)		*/
	UCHAR	k_kcnt;			/* 会員確変回数			*/
	UCHAR	k_lcnt;			/* 会員ﾗｯｷｰ回数			*/
	USHORT	k_out;			/* 会員ｱｳﾄ			*/
	USHORT	k_saf;			/* 会員ｾｰﾌ			*/
	USHORT	k_tout;			/* 会員特賞ｱｳﾄ			*/
	USHORT	k_tsaf;			/* 会員特賞ｾｰﾌ			*/
	USHORT	k_start;		/* 会員ｽﾀｰﾄ			*/
	USHORT	k_uri;			/* 会員売上(100円)		*/
	USHORT	k_repkos;		/* 台ﾘﾌﾟﾚｲ個数			*/
	USHORT	k_reptesu;		/* 台ﾘﾌﾟﾚｲ手数料(個数)		*/

	USHORT	r_bonus;		/* ﾚｷﾞｭﾗｰ回数(ｽﾛｯﾄのみ)		*/

	USHORT	toutmax;		/* 特賞中ｱｳﾄ(最大値)		*/
	USHORT	toutmin;		/* 特賞中ｱｳﾄ(最小値)		*/
	short	tsamax;			/* 特賞中差(最大値)		*/
	short	tsamin;			/* 特賞中差(最小値)		*/
	USHORT	boutmax;		/* 特賞間ｱｳﾄ(最大値)		*/
	USHORT	boutmin;		/* 特賞間ｱｳﾄ(最小値)		*/
	short	bsamax;			/* 特賞間差(最大値)		*/
	short	bsamin;			/* 特賞間差(最小値)		*/
	USHORT	bstartmax;		/* 特賞間ｽﾀｰﾄ(最大値)		*/
	USHORT	bstartmin;		/* 特賞間ｽﾀｰﾄ(最小値)		*/
	short	bmsamax;		/* 特賞間最大差(最大値)		*/
	short	bmsamin;		/* 特賞間最大差(最小値)		*/
	short	tmotimax;		/* 特賞持玉(最大値)		*/
	short	tmotimin;		/* 特賞持玉(最小値)		*/

	USHORT	kr_bonus;		/* CT中ﾚｷﾞｭﾗｰ回数(ｽﾛｯﾄのみ)		*/

	UCHAR	tflg;			/* 特賞ﾌﾗｸﾞ			*/
							/*	0x01 : T1(ﾋﾞｯｸﾞ)		*/
							/*	0x02 : T2(CT) 			*/
							/*	0x04 : ﾗｯｷ-				*/
							/*	0x08 : ﾚｷﾞｭﾗｰ(Sのみ)	*/
	UCHAR	dmy2;			/* ....				*/
	UCHAR	e_hour;			/* 最終時刻(時)			*/
	UCHAR	e_min;			/* 最終時刻(分)			*/
	USHORT	e_t1out;		/* 最終特賞１中ｱｳﾄ		*/
	USHORT	e_t1saf;		/* 最終特賞１中ｾｰﾌ		*/
	USHORT	e_bout;			/* 最終特賞１間ｱｳﾄ		*/
	USHORT	e_bsaf;			/* 最終特賞１間ｾｰﾌ		*/
	USHORT	e_bstart;		/* 最終特賞１間ｽﾀｰﾄ		*/
	USHORT	e_bmsa;			/* 最終特賞１間最大差(打込)	*/
	USHORT	e_buri;			/* 最終特賞１間売上		*/
	USHORT	e_motichg;		/* 最終持玉(交換)		*/
	USHORT	e_motiever;		/* 最終持玉(無定量)		*/
	UCHAR	yobi2[2];		/* ....				*/
	USHORT	san_ukos;		/* ｻﾝﾄﾞ売上個数			*/
	USHORT	chk_dat;		/* CHK ﾃﾞｰﾀ			*/
	ULONG	k_keisu;		/* 台景品個数		*/
	ULONG	k_cyokos;		/* 台貯玉個数			*/
} FDATA_DAI;

/*#**************************************/
/*	全店ﾃﾞｰﾀ(島ｺﾝ)(4103)			*/
/****************************************/
typedef	struct {
	long	indata;			/* 計数値			*/
	char	dancnt;			/* 断線回数			*/
	char	err_door;		/* ｴﾗｰ回数  0xf0		*/
							/* ﾄﾞｱ回数  0x0f		*/
	char	othercnt;		/* その他回数			*/
	char	flg;			/* ﾌﾗｸﾞ				*/
} FDATA_SMCON;

/*#**************************************/
/*	島･機種ﾃﾞｰﾀ(4104,4105)			*/
/****************************************/
typedef	struct {
	USHORT	daisu;			/* 台数				*/
	USHORT	jflg;			/* 台情報ﾌﾗｸﾞ(台ﾃﾞｰﾀ参照)	*/
	ULONG	out;			/* ｱｳﾄ				*/
	ULONG	saf;			/* ｾｰﾌ				*/
	ULONG	start;			/* ｽﾀｰﾄ				*/
	ULONG	tout;			/* 特賞ｱｳﾄ			*/
	ULONG	tsaf;			/* 特賞ｾｰﾌ			*/
	ULONG	t2out;			/* 特賞2ｱｳﾄ			*/
	ULONG	t2saf;			/* 特賞2ｾｰﾌ			*/
	ULONG	t2start;		/* 特賞2ｽﾀｰﾄ			*/
	USHORT	bcnt;			/* ﾍﾞｰｽ回数(特賞回数/打止回数)	*/
	USHORT	ucnt;			/* 打止回数			*/
	USHORT	tcnt;			/* 特賞回数  (T)		*/
	USHORT	t1cnt;			/* 特賞1回数 (T1)		*/
	USHORT	t2cnt;			/* 特賞2回数 (T2)		*/
	USHORT	kcnt;			/* 確変回数((T2)>T1>T2>T1･･)	*/
	USHORT	kt1cnt;			/* 確変中T1回数			*/

	USHORT	lcnt;			/* ﾗｯｷｰ回数			*/
	USHORT	lt1cnt;			/* ﾗｯｷｰ中T1回数			*/
	USHORT	lt1max;			/* ﾗｯｷｰ中MAXT1回数		*/
	ULONG	lout;			/* ﾗｯｷｰ中ｱｳﾄ			*/
	ULONG	lsaf;			/* ﾗｯｷｰ中ｾｰﾌ			*/
	ULONG	lbout;			/* ﾗｯｷｰT1間ｱｳﾄ			*/
	ULONG	lmoti;			/* ﾗｯｷｰ中持玉			*/

	ULONG	start1;			/* ｽﾀｰﾄ1			*/
	ULONG	start2;			/* ｽﾀｰﾄ2			*/
	ULONG	start3;			/* ｽﾀｰﾄ3			*/
	ULONG	t2start1;		/* 特賞2(T2)中ｽﾀｰﾄ1		*/
	ULONG	t2start2;		/* 特賞2(T2)中ｽﾀｰﾄ2		*/
	ULONG	t2start3;		/* 特賞2(T2)中ｽﾀｰﾄ3		*/
	ULONG	t1start2;		/* 特賞ｽﾀｰﾄ2回数		*/

	ULONG	ukomi;			/* 打込玉			*/
	ULONG	t1ukomi;		/* 特賞打込			*/
	ULONG	maxmoti;		/* 最大持玉 MY			*/
	ULONG	maxukomi;		/* 最大打込 M◇			*/

	USHORT	kado;			/* 積稼動数			*/
	USHORT	kyaku;			/* 客交代			*/

	USHORT	hasfg;			/* 発生済ﾌﾗｸﾞ			*/
	USHORT	door1;			/* ﾄﾞｱ1回数			*/
	USHORT	door2;			/* ﾄﾞｱ2回数			*/
	USHORT	call1;			/* 呼出1回数			*/
	USHORT	call2;			/* 呼出2回数			*/
	UCHAR	fuscnt;			/* 不正回数			*/
	UCHAR	ddancnt;		/* 台断線回数			*/
	UCHAR	ijocnt;			/* 異常回数			*/
	UCHAR	tencnt;			/* 点検回数			*/
	UCHAR	g1dancnt;		/* 現金1断線回数		*/
	UCHAR	g2dancnt;		/* 現金2断線回数		*/
	UCHAR	cddancnt;		/* ｶｰﾄﾞ断線回数			*/
	UCHAR	uijocnt;		/* 売上異常回数			*/

	UCHAR	ukind;			/* 売上情報ﾌﾗｸﾞ(台ﾃﾞｰﾀ参照)	*/
	UCHAR	dmy1;			/* ....				*/
	ULONG	gen1uri;		/* ｻﾝﾄﾞ現金1売上(100円)		*/
	ULONG	gen2uri;		/* ｻﾝﾄﾞ現金2売上(100円)		*/
	ULONG	carduri;		/* ｻﾝﾄﾞｶｰﾄﾞ売上(100円)		*/
	ULONG	daiuri;			/* 台売上(100円)		*/
	ULONG	hosei;			/* 売上補正値(円)		*/

	USHORT	k_suu;			/* 会員数			*/
	USHORT	k_kado;			/* 会員稼動数			*/
	USHORT	k_t1cnt;		/* 会員特賞1回数 (T1)		*/
	USHORT	k_tcnt;			/* 会員特賞回数	 (T)		*/
	USHORT	k_kcnt;			/* 会員確変回数			*/
	USHORT	k_lcnt;			/* 会員ﾗｯｷｰ回数			*/
	ULONG	k_out;			/* 会員ｱｳﾄ			*/
	ULONG	k_saf;			/* 会員ｾｰﾌ			*/
	ULONG	k_tout;			/* 会員特賞ｱｳﾄ			*/
	ULONG	k_tsaf;			/* 会員特賞ｾｰﾌ			*/
	ULONG	k_start;		/* 会員ｽﾀｰﾄ			*/
	ULONG	k_uri;			/* 会員売上(100円)		*/
	ULONG	k_repkos;		/* 台ﾘﾌﾟﾚｲ個数			*/
	ULONG	k_repkin;		/* 台ﾘﾌﾟﾚｲ金額(円)		*/
	ULONG	k_reptesu;		/* 台ﾘﾌﾟﾚｲ手数料(個数)		*/
	ULONG	k_reptesukin;		/* 台ﾘﾌﾟﾚｲ手数料(金額)(円)	*/
	ULONG	k_cyokos;		/* 台貯玉個数			*/
	ULONG	k_cyokin;		/* 台貯玉金額			*/
	ULONG	k_keisu;		/* 台景品個数		*/

	USHORT	r_bonus;		/* ﾚｷﾞｭﾗｰ回数(ｽﾛｯﾄのみ)		*/
	USHORT	kr_bonus;		/* CT中ﾚｷﾞｭﾗｰ回数(ｽﾛｯﾄのみ)	*/

	USHORT	g_kado;			/* 現稼動台数		*/
	USHORT	g_k_kado;		/* 現会員稼働台数	*/

	UCHAR	yobi[24];		/* ....				*/
} FDATA_SK;

/*#**************************************/
/*	種別集計･ﾌﾛｱﾃﾞｰﾀ(4106,4110)		*/
/****************************************/
typedef	struct {
	USHORT	daisu;			/* 台数				*/
	USHORT	jflg;			/* 台情報ﾌﾗｸﾞ(台ﾃﾞｰﾀ参照)	*/
	ULONG	out;			/* ｱｳﾄ				*/
	ULONG	saf;			/* ｾｰﾌ				*/
	ULONG	start;			/* ｽﾀｰﾄ				*/
	ULONG	tout;			/* 特賞ｱｳﾄ			*/
	ULONG	tsaf;			/* 特賞ｾｰﾌ			*/
	ULONG	t2out;			/* 特賞2ｱｳﾄ			*/
	ULONG	t2saf;			/* 特賞2ｾｰﾌ			*/
	ULONG	t2start;		/* 特賞2ｽﾀｰﾄ			*/
	USHORT	bcnt;			/* ﾍﾞｰｽ回数(特賞回数/打止回数)	*/
	USHORT	ucnt;			/* 打止回数			*/
	USHORT	tcnt;			/* 特賞回数  (T)		*/
	USHORT	t1cnt;			/* 特賞1回数 (T1)		*/
	USHORT	t2cnt;			/* 特賞2回数 (T2)		*/
	USHORT	kcnt;			/* 確変回数((T2)>T1>T2>T1･･)	*/
	USHORT	kt1cnt;			/* 確変中T1回数			*/

	USHORT	lcnt;			/* ﾗｯｷｰ回数			*/
	USHORT	lt1cnt;			/* ﾗｯｷｰ中T1回数			*/
	USHORT	lt1max;			/* ﾗｯｷｰ中MAXT1回数		*/
	ULONG	lout;			/* ﾗｯｷｰ中ｱｳﾄ			*/
	ULONG	lsaf;			/* ﾗｯｷｰ中ｾｰﾌ			*/
	ULONG	lbout;			/* ﾗｯｷｰT1間ｱｳﾄ			*/
	ULONG	lmoti;			/* ﾗｯｷｰ中持玉			*/

	ULONG	ukomi;			/* 打込玉			*/
	ULONG	t1ukomi;		/* 特賞打込			*/

	USHORT	kado;			/* 積稼動数			*/
	USHORT	kyaku;			/* 客交代			*/

	USHORT	hasfg;			/* 発生済ﾌﾗｸﾞ			*/
	USHORT	door1;			/* ﾄﾞｱ1回数			*/
	USHORT	door2;			/* ﾄﾞｱ2回数			*/
	USHORT	call1;			/* 呼出1回数			*/
	USHORT	call2;			/* 呼出2回数			*/
	UCHAR	fuscnt;			/* 不正回数			*/
	UCHAR	ddancnt;		/* 台断線回数			*/
	UCHAR	ijocnt;			/* 異常回数			*/
	UCHAR	tencnt;			/* 点検回数			*/
	UCHAR	g1dancnt;		/* 現金1断線回数		*/
	UCHAR	g2dancnt;		/* 現金2断線回数		*/
	UCHAR	cddancnt;		/* ｶｰﾄﾞ断線回数			*/
	UCHAR	uijocnt;		/* 売上異常回数			*/

	UCHAR	ukind;			/* 売上情報ﾌﾗｸﾞ(台ﾃﾞｰﾀ参照)	*/
	UCHAR	dmy1;			/* ....				*/
	ULONG	gen1uri;		/* ｻﾝﾄﾞ現金1売上(100円)		*/
	ULONG	gen2uri;		/* ｻﾝﾄﾞ現金2売上(100円)		*/
	ULONG	carduri;		/* ｻﾝﾄﾞｶｰﾄﾞ売上(100円)		*/
	ULONG	daiuri;			/* 台売上(100円)		*/
	ULONG	hosei;			/* 売上補正値(円)		*/

	USHORT	k_suu;			/* 会員数			*/
	USHORT	k_kado;			/* 会員稼動数			*/
	USHORT	k_t1cnt;		/* 会員特賞1回数 (T1)		*/
	USHORT	k_tcnt;			/* 会員特賞回数	 (T)		*/
	USHORT	k_kcnt;			/* 会員確変回数			*/
	USHORT	k_lcnt;			/* 会員ﾗｯｷｰ回数			*/
	ULONG	k_out;			/* 会員ｱｳﾄ			*/
	ULONG	k_saf;			/* 会員ｾｰﾌ			*/
	ULONG	k_tout;			/* 会員特賞ｱｳﾄ			*/
	ULONG	k_tsaf;			/* 会員特賞ｾｰﾌ			*/
	ULONG	k_start;		/* 会員ｽﾀｰﾄ			*/
	ULONG	k_uri;			/* 会員売上(100円)		*/
	ULONG	k_repkos;		/* 台ﾘﾌﾟﾚｲ個数			*/
	ULONG	k_reptesu;		/* 台ﾘﾌﾟﾚｲ手数料(個数)		*/
	ULONG	k_cyokos;		/* 台貯玉個数			*/

	ULONG	sg_danflg;		/* 島外断線済ﾌﾗｸﾞ		*/
							/*  0x00000001 : 金庫断線	*/
							/*  0x00000002 : 景品断線	*/
							/*  0x00000004 : 現金売上断線	*/
							/*  0x00000008 : ｶｰﾄﾞ売上断線	*/
							/*  0x00000010 : ﾘﾌﾟﾚｲ断線	*/
							/*  0x00000020 : ﾌﾟﾚﾐｱﾑ断線	*/
							/*  0x00000040 : 貯玉断線	*/
							/*  0x00000080 : ｶｰﾄﾞ発行断線	*/
							/*  0x00000200 : POS断線	*/
							/*  0x00000400 : 自販機		*/
							/*  0x00000800 : その他断線	*/
	ULONG	urikin;			/* 売上金額(合計)(100円)	*/
	ULONG	t_keikos;		/* 景品個数(合計)		*/
	ULONG	t_g_keikos;		/* 原価景品個数			*/
	ULONG	sg_gen1uri;		/* 島外現金1売上(100円)		*/
	ULONG	sg_gen2uri;		/* 島外現金2売上(100円)		*/
	ULONG	sg_carduri;		/* 島外ｶｰﾄﾞ売上(100円)		*/
	ULONG	sg_repkos;		/* 島外ﾘﾌﾟﾚｲ個数		*/
	ULONG	sg_reptesu;		/* 島外ﾘﾌﾟﾚｲ手数料(個数)	*/
	ULONG	sg_premkos;		/* 島外ﾌﾟﾚﾐｱﾑ個数		*/
	ULONG	sg_cyokos;		/* 島外貯玉個数			*/
	ULONG	kinko;			/* 金庫合計			*/
	ULONG	sand;			/* ｻﾝﾄﾞ現金計			*/
	ULONG	sandkinko;		/* ｻﾝﾄﾞ金庫計			*/
	ULONG	poskos;			/* POS個数			*/
	ULONG	g_poskos;		/* 原価POS個数			*/
	ULONG	k_keisu;		/* 台景品個数		*/

	USHORT	r_bonus;		/* ﾚｷﾞｭﾗｰ回数(ｽﾛｯﾄのみ)		*/
	USHORT	kr_bonus;		/* CT中ﾚｷﾞｭﾗｰ回数(ｽﾛｯﾄのみ)	*/

	USHORT	g_kado;			/* 現稼動台数		*/
	USHORT	g_k_kado;		/* 現会員稼働台数	*/

	UCHAR	yobi[8];		/* ....				*/
} FDATA_SBT;

/*#**************************************/
/*	全店集計(4107,4111)			*/
/****************************************/
typedef	struct {
	USHORT	daisu;			/* 台数				*/
	USHORT	jflg;			/* 台情報ﾌﾗｸﾞ(台ﾃﾞｰﾀ参照)	*/

	USHORT	kado;			/* 積稼動数			*/

	USHORT	hasfg;			/* 発生済ﾌﾗｸﾞ			*/

	UCHAR	g1dancnt;		/* 現金1断線回数		*/
	UCHAR	g2dancnt;		/* 現金2断線回数		*/
	UCHAR	cddancnt;		/* ｶｰﾄﾞ断線回数			*/
	UCHAR	uijocnt;		/* 売上異常回数			*/

	UCHAR	ukind;			/* 売上情報ﾌﾗｸﾞ(台ﾃﾞｰﾀ参照)	*/
	UCHAR	dmy1[3];		/* ....				*/
	ULONG	gen1uri;		/* ｻﾝﾄﾞ現金1売上(100円)		*/
	ULONG	gen2uri;		/* ｻﾝﾄﾞ現金2売上(100円)		*/
	ULONG	carduri;		/* ｻﾝﾄﾞｶｰﾄﾞ売上(100円)		*/
	ULONG	daiuri;			/* 台売上(100円)		*/
	ULONG	hosei;			/* 売上補正値(円)		*/

	USHORT	k_suu;			/* 会員数			*/
	USHORT	k_kado;			/* 会員稼動数			*/
	ULONG	k_uri;			/* 会員売上(100円)		*/
	ULONG	k_repkin;		/* 台ﾘﾌﾟﾚｲ金額(円)		*/
	ULONG	k_reptesukin;		/* 台ﾘﾌﾟﾚｲ手数料(金額)(円)	*/
	ULONG	k_cyokin;		/* 台貯玉金額(円)		*/

	ULONG	sg_danflg;		/* 島外断線済ﾌﾗｸﾞ(種別集計参照)	*/
	ULONG	urikin;			/* 売上金額(合計)(100円)	*/
	ULONG	t_keikin;		/* 景品金額(合計)(円)		*/
	ULONG	t_g_keikin;		/* 原価景品金額(円)		*/
	ULONG	sg_gen1uri;		/* 島外現金1売上(100円)		*/
	ULONG	sg_gen2uri;		/* 島外現金2売上(100円)		*/
	ULONG	sg_carduri;		/* 島外ｶｰﾄﾞ売上(100円)		*/
	ULONG	sg_repkin;		/* 島外ﾘﾌﾟﾚｲ金額(円)		*/
	ULONG	sg_reptesukin;		/* 島外ﾘﾌﾟﾚｲ手数料(金額)(円)	*/
	ULONG	sg_premkin;		/* 島外ﾌﾟﾚﾐｱﾑ金額(円)		*/
	ULONG	sg_cyokin;		/* 島外貯玉金額(円)		*/
	ULONG	kinko;			/* 金庫合計			*/
	ULONG	sand;			/* ｻﾝﾄﾞ現金計			*/
	ULONG	sandkinko;		/* ｻﾝﾄﾞ金庫計			*/
	ULONG	poskin;			/* POS金額(円)			*/
	ULONG	g_poskin;		/* 原価POS金額(円)		*/

	USHORT	cd_hfg;			/* ｶｰﾄﾞ発行ﾌﾗｸﾞ			*/
	USHORT	cd_hdanfg;		/* ｶｰﾄﾞ発行断線ﾌﾗｸﾞ		*/
	ULONG	cd_htotal;		/* ｶｰﾄﾞ発行合計金額(100円)	*/
	ULONG	cd_h1kei;		/* ｶｰﾄﾞ発行1000円計(100円)	*/
	ULONG	cd_h2kei;		/* ｶｰﾄﾞ発行2000円計(100円)	*/
	ULONG	cd_h3kei;		/* ｶｰﾄﾞ発行3000円計(100円)	*/
	ULONG	cd_h5kei;		/* ｶｰﾄﾞ発行5000円計(100円)	*/
	ULONG	cd_h10kei;		/* ｶｰﾄﾞ発行10000円計(100円)	*/

	ULONG	zihan;			/* 自販機計(100円)		*/
	ULONG	ta;				/* その他計			*/

	long	d_keikin;		/* 台景品金額(100円)		*/
	long	g_d_keikin;		/* 原価台景品金額(100円)	*/
	long	hd_keikin;		/* 補正台景品金額(100円)	*/
	long	g_hd_keikin;		/* 補正原価台景品金額(100円)	*/
	long	k_keikin;		/* 台景品金額(貯玉･リプレイ)		*/
#if 1	/*	台売上での粗利益計算の為、台売上からの景品金額追加	*/
//	long	sa_kin;			/* 台売上からの景品金額(円)		*/	
	long	g_duri_keikin;	/* 台売上からの原価台景品金額(円)	*/

	USHORT	g_kado;			/* 現稼動台数		*/
	USHORT	g_k_kado;		/* 現会員稼働台数	*/

	UCHAR	yobi[72];		/* ....				*/
#else
	UCHAR	yobi[80];		/* ....				*/
#endif
} FDATA_TOTAL;

/*#**************************************/
/*	金庫集計(4108)						*/
/****************************************/
typedef	struct {
	UCHAR	sima[4];		/* 島番号 [0]:1 - [3]:4		*/
	ULONG	gen1kei;		/* 現金1計(100円)		*/
	ULONG	gen2kei;		/* 現金2計(100円)		*/
	ULONG	sandkinko;		/* ｻﾝﾄﾞ金庫計			*/
	ULONG	cardkei;		/* ｶｰﾄﾞ計(100円)		*/
	ULONG	kinko;			/* 金庫計			*/
	UCHAR	g1dancnt;		/* 現金1断線回数		*/
	UCHAR	g2dancnt;		/* 現金2断線回数		*/
	UCHAR	skkdancnt;		/* ｻﾝﾄﾞ金庫断線回数		*/
	UCHAR	cddancnt;		/* ｶｰﾄﾞ断線回数			*/
	UCHAR	kkdancnt;		/* 金庫断線回数			*/
	UCHAR	errcnt;			/* ｴﾗｰ回数			*/
	UCHAR	doorcnt;		/* ﾄﾞｱ回数			*/
	UCHAR	kktencnt;		/* 金庫点検回数			*/
#if 0
	UCHAR	flg;			/* ﾌﾗｸﾞ				*/
	UCHAR	yobi;			/* ....				*/
#endif
} FDATA_KINKO;

/*#**************************************/
/*	定時情報(4109)						*/
/****************************************/
typedef	struct {
	UCHAR	kiotim[2];		/* 記憶時刻 (時:分)	*/
	USHORT	egyo;			/* 営業時間			*/
} FDATA_TIME_INF;


/*#**************************************/
/*	定時稼動情報(4112)					*/
/****************************************/
typedef	FDATA_TIME_INF	FDATA_KADO_INF;


/*#**************************************/
/*	定時種別稼動(4113)					*/
/****************************************/
typedef	struct {
	USHORT	daisu;			/* 台数				*/
	USHORT	tcnt;			/* 特賞回数			*/
	ULONG	out;			/* ｱｳﾄ				*/
	ULONG	saf;			/* ｾｰﾌ				*/
	ULONG	urikin;			/* 売上(100円)			*/
	ULONG	hosei;			/* 売上補正値(円)		*/
	USHORT	kado;			/* 現稼動数			*/
	USHORT	k_kado;			/* 現会員稼動数			*/
	ULONG	keikos;			/* 景品個数			*/
	UCHAR	yobi[4];		/* ....				*/
} FDATA_KADO_SBT;


/*#**************************************/
/*	定時全店稼動(4114)			*/
/****************************************/
typedef	struct {
	USHORT	daisu;			/* 台数				*/
	USHORT	tcnt;			/* 特賞回数			*/
	long	yobi[2];		/* 予備				*/
	ULONG	urikin;			/* 売上(100円)			*/
	ULONG	hosei;			/* 売上補正値(円)		*/
	USHORT	kado;			/* 現稼動数			*/
	USHORT	k_kado;			/* 会員稼動数			*/
	ULONG	keikin;			/* 景品金額(100円)		*/
	long	g_keikin;		/* 原価台景品金額(100円)	*/
	long	d_keikin;		/* 台景品金額(100円)		*/
	long	g_d_keikin;		/* 原価台景品金額(100円)	*/
	long	hd_keikin;		/* 補正台景品金額(100円)	*/
	long	g_hd_keikin;		/* 補正原価台景品金額(100円)	*/
} FDATA_KADO_ZEN;

/*#**************************************/
/*	定時機種稼動(4115)			*/			// 機種別定時稼動 2003/04/09 nakatani　コメント
/****************************************/
//typedef	struct {
//	USHORT	daisu;			/* 台数				*/
//	USHORT	tcnt;			/* 特賞回数			*/
//	ULONG	out;			/* ｱｳﾄ				*/
//	ULONG	saf;			/* ｾｰﾌ				*/
//	ULONG	urikin;			/* 売上(100円)			*/
//	ULONG	hosei;			/* 売上補正値(円)		*/
//	USHORT	kado;			/* 現稼動数			*/
//	USHORT	k_kado;			/* 現会員稼動数			*/
//} FDATA_KADO_KISHU;

#endif