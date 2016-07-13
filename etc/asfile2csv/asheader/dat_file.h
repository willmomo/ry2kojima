/* WAIS START */
/* $Header: /usr/external/CVS/as-sh3/include/dat_file.h,v 1.8 2002/04/24 09:40:54 ohno Exp $ */
/********************************************************************************************************/
/*	データファイル											*/
/*	Version	Date		Programmer	Memo							*/
/*	?????.? 2001.02.28	S.A		機種、種別、全店に現稼動台数、現会員稼動台数追加	*/
/*	?????.? 2000.12.06	mizu		台売上からの原価台景品金額(円)と名称変更		*/
/*	?????.? 2000.12.05	mizu		台売上での粗利益計算の為、台売上からの景品金額追加	*/
/*	00000.0	97.07.30	藤木		最終特賞中->最終特賞１中に変更				*/
/*	00000.0	97.04.07	y.i		新規作成						*/
/********************************************************************************************************/
#ifndef __dat_file_h__
#define __dat_file_h__
#define	L_DAT_FILE
/****************************************/
/*	島外断線済ﾌﾗｸﾞ			*/
/****************************************/
#define	DANFG_KINKO	0x00000001
#define	DANFG_KEIHIN	0x00000002
#define	DANFG_GENHAN	0x00000004
#define	DANFG_CARDHAN	0x00000008
#define	DANFG_REPLAY	0x00000010
#define	DANFG_PREMIUM	0x00000020
#define	DANFG_CYOTAMA	0x00000040
#define	DANFG_CDHAKKOU	0x00000080
#define	DANFG_POS	0x00000200
#define	DANFG_ZIHAN	0x00000400
#define	DANFG_TA	0x00000800

/****************************************/
/*	ｶｰﾄﾞ発行機断線済ﾌﾗｸﾞ		*/
/****************************************/
#define	CDH_DANFG01K	0x0001
#define	CDH_DANFG02K	0x0002
#define	CDH_DANFG03K	0x0004
#define	CDH_DANFG05K	0x0008
#define	CDH_DANFG10K	0x0010

/****************************************/
/*	ｶｰﾄﾞ発行機有無ﾌﾗｸﾞ		*/
/****************************************/
#define	CDHFG_01K	0x0001
#define	CDHFG_02K	0x0002
#define	CDHFG_03K	0x0004
#define	CDHFG_05K	0x0008
#define	CDHFG_10K	0x0010

/****************************************/
/*	累計台データ			*/
/****************************************/

/****************************************/
/*	台情報ﾌﾗｸﾞ			*/
/****************************************/

#define		FG_JFLG_TBASE		0x0001	/* 特賞ﾍﾞｰｽ	*/
#define		FG_JFLG_DSET		0x0002	/* ﾃﾞｰﾀ設定	*/
#define		FG_JFLG_SHUTEI		0x0004	/* 集計停止	*/
#define		FG_JFLG_KADO		0x0008	/* 稼動中	*/
#define		FG_JFLG_KAIIN		0x0010	/* 会員中	*/
#define		FG_JFLG_TOKTEI		0x2000	/* 特賞停止	*/
#define		FG_JFLG_IJOTEI		0x4000	/* 異常停止	*/
#define		FG_JFLG_INTEI		0x8000	/* 取込停止	*/

#define		FG_JFLG_TEIALL		0xe004	/* 全停止ﾌﾗｸﾞ	*/

/****************************************/
/*	発生済ﾌﾗｸﾞ			*/
/****************************************/
#define		FG_HASFG_DFUS		0x0001	/* 台不正	*/
#define		FG_HASFG_DDAN		0x0002	/* 台断線	*/
#define		FG_HASFG_DIJYO		0x0004	/* 台異常	*/
#define		FG_HASFG_DTEN		0x0008	/* 台点検	*/
#define		FG_HASFG_URIFUS		0x0100	/* 売上不正	*/
#define		FG_HASFG_URIDAN		0x0200	/* 売上断線	*/
#define		FG_HASFG_URIIJYO	0x0400	/* 売上異常	*/
#define		FG_HASFG_URITEN		0x0800	/* 売上点検	*/

/****************************************/
/*	売上情報ﾌﾗｸﾞ			*/
/****************************************/

#define		FG_UKIND_GEN1		0x01	/* ｻﾝﾄﾞ現金1有り	*/
#define		FG_UKIND_GEN2		0x02	/* ｻﾝﾄﾞ現金2有り	*/
#define		FG_UKIND_CARD		0x04	/* ｶｰﾄﾞ売上有り		*/
#define		FG_UKIND_GEN1KIN	0x10	/* ｻﾝﾄﾞ現金1金庫	*/
#define		FG_UKIND_GEN2KIN	0x20	/* ｻﾝﾄﾞ現金2金庫	*/

/*#**************************************/
/*	台ﾃﾞｰﾀ(累計)			*/
/****************************************/
typedef	struct {
	ushort	jflg;			/* 台情報ﾌﾗｸﾞ			*/
					/*    0x0001 : 特賞ﾍﾞｰｽ		*/
					/*    0x0002 : ﾃﾞｰﾀ設定		*/
					/*    0x0004 : 集計停止		*/
					/*    0x0008 : 稼動中		*/
					/*    0x0010 : 会員稼動中	*/
	ushort	out;			/* ｱｳﾄ				*/
	ushort	saf;			/* ｾｰﾌ				*/
	ushort	start;			/* ｽﾀｰﾄ				*/
	ushort	tout;			/* 特賞ｱｳﾄ			*/
	ushort	tsaf;			/* 特賞ｾｰﾌ			*/
	ushort	t2out;			/* 特賞2ｱｳﾄ			*/
	ushort	t2saf;			/* 特賞2ｾｰﾌ			*/
	ushort	t2start;		/* 特賞2ｽﾀｰﾄ			*/
	ushort	bcnt;			/* ﾍﾞｰｽ回数(特賞回数/打止回数)	*/
	ushort	ucnt;			/* 打止回数			*/
	ushort	tcnt;			/* 特賞回数  (T)		*/
	ushort	t1cnt;			/* 特賞1回数 (T1)		*/
	ushort	t2cnt;			/* 特賞2回数 (T2)		*/
	ushort	kcnt;			/* 確変回数((T2)>T1>T2>T1･･･)	*/
	ushort	kt1cnt;			/* 確変中T1回数			*/

	ushort	lcnt;			/* ﾗｯｷｰ回数			*/
	ushort	lt1cnt;			/* ﾗｯｷｰ中特賞(T1)回数		*/
	ushort	lt1max;			/* ﾗｯｷｰ中MAX特賞(T1)回数	*/
	ushort	lout;			/* ﾗｯｷｰ中ｱｳﾄ			*/
	ushort	lsaf;			/* ﾗｯｷｰ中ｾｰﾌ			*/
	ushort	lbout;			/* ﾗｯｷｰ特賞(T1)間ｱｳﾄ		*/
	ushort	lmoti;			/* ﾗｯｷｰ中持玉			*/

	ushort	start1;			/* ｽﾀｰﾄ1			*/
	ushort	start2;			/* ｽﾀｰﾄ2			*/
	ushort	start3;			/* ｽﾀｰﾄ3			*/
	ushort	t2start1;		/* 特賞2(T2)中ｽﾀｰﾄ1		*/
	ushort	t2start2;		/* 特賞2(T2)中ｽﾀｰﾄ2		*/
	ushort	t2start3;		/* 特賞2(T2)中ｽﾀｰﾄ3		*/
	ushort	t1start2;		/* 特賞ｽﾀｰﾄ2回数	2002.4.24 OTOの都合により稼動データは元に戻す */

	ushort	ukomi;			/* 打込玉			*/
	ushort	t1ukomi;		/* 特賞打込			*/
	short	minsa;			/* 最小差			*/
	short	maxsa;			/* 最大差			*/
	ushort	minout;			/* 最小差ｱｳﾄ			*/
	ushort	maxout;			/* 最大差ｱｳﾄ			*/
	ushort	maxmoti;		/* 最大持玉 MY			*/
	ushort	maxukomi;		/* 最大打込 M◇			*/

	ushort	kadofg;			/* 定時稼動ﾌﾗｸﾞ(8-23)		*/
	uchar	kado;			/* 積稼動数			*/
	uchar	kyaku;			/* 客交代			*/

	ushort	hasfg;			/* 発生済ﾌﾗｸﾞ			*/
					/*    0x0001 : 台不正		*/
					/*    0x0002 : 台断線		*/
					/*    0x0004 : 台異常		*/
					/*    0x0008 : 台点検		*/
					/*    0x0100 : 売上不正		*/
					/*    0x0200 : 売上断線		*/
					/*    0x0400 : 売上異常		*/
					/*    0x0800 : 売上点検		*/
	uchar	door1_2;		/* ﾄﾞｱ1回数		0xf0	*/
					/* ﾄﾞｱ2回数		0x0f	*/
	uchar	call1_2;		/* 呼出1回数		0xf0	*/
					/* 呼出2回数		0x0f	*/
	uchar	fus_ddan;		/* 不正回数		0xf0	*/
					/* 台断線回数		0x0f	*/
	uchar	ijo_ten;		/* 異常回数		0xf0	*/
					/* 点検回数		0x0f	*/
	uchar	g1_g2dan;		/* 現金1断線回数	0xf0	*/
					/* 現金2断線回数	0x0f	*/
	uchar	cddan_uijo;		/* ｶｰﾄﾞ断線回数		0xf0	*/
					/* 売上異常回数		0x0f	*/

	uchar	ukind;			/* 売上情報ﾌﾗｸﾞ			*/
					/*     0x01 : ｻﾝﾄﾞ現金1有り	*/
					/*     0x02 : ｻﾝﾄﾞ現金2有り	*/
					/*     0x04 : ｶｰﾄﾞ売上有り	*/
					/*     0x10 : ｻﾝﾄﾞ現金1金庫	*/
					/*     0x20 : ｻﾝﾄﾞ現金2金庫	*/
	uchar	dmy1;			/* ....				*/
	ushort	gen1uri;		/* ｻﾝﾄﾞ現金1売上(100円)		*/
	ushort	gen2uri;		/* ｻﾝﾄﾞ現金2売上(100円)		*/
	ushort	carduri;		/* ｻﾝﾄﾞｶｰﾄﾞ売上(100円)		*/
	ushort	daiuri;			/* 台売上(100円)		*/
	ushort	hosei;			/* 売上補正値(100円) 2001.09.13 chg */

	uchar	k_suu;			/* 会員数			*/
	uchar	k_kado;			/* 会員稼動数			*/
	ushort	k_kadofg;		/* 会員定時稼動ﾌﾗｸﾞ(8-23)	*/
	uchar	k_t1cnt;		/* 会員特賞1回数 (T1)		*/
	uchar	k_tcnt;			/* 会員特賞回数	 (T)		*/
	uchar	k_kcnt;			/* 会員確変回数			*/
	uchar	k_lcnt;			/* 会員ﾗｯｷｰ回数			*/
	ushort	k_out;			/* 会員ｱｳﾄ			*/
	ushort	k_saf;			/* 会員ｾｰﾌ			*/
	ushort	k_tout;			/* 会員特賞ｱｳﾄ			*/
	ushort	k_tsaf;			/* 会員特賞ｾｰﾌ			*/
	ushort	k_start;		/* 会員ｽﾀｰﾄ			*/
	ushort	k_uri;			/* 会員売上(100円)		*/
	ushort	k_repkos;		/* 台ﾘﾌﾟﾚｲ個数			*/
	ushort	k_reptesu;		/* 台ﾘﾌﾟﾚｲ手数料(個数)		*/

	ushort	r_bonus;		/* ﾚｷﾞｭﾗｰ回数(ｽﾛｯﾄのみ)	1999.02.03 osako */

	ushort	toutmax;		/* 特賞中ｱｳﾄ(最大値)		*/
	ushort	toutmin;		/* 特賞中ｱｳﾄ(最小値)		*/
	short	tsamax;			/* 特賞中差(最大値)		*/
	short	tsamin;			/* 特賞中差(最小値)		*/
	ushort	boutmax;		/* 特賞間ｱｳﾄ(最大値)		*/
	ushort	boutmin;		/* 特賞間ｱｳﾄ(最小値)		*/
	short	bsamax;			/* 特賞間差(最大値)		*/
	short	bsamin;			/* 特賞間差(最小値)		*/
	ushort	bstartmax;		/* 特賞間ｽﾀｰﾄ(最大値)		*/
	ushort	bstartmin;		/* 特賞間ｽﾀｰﾄ(最小値)		*/
	short	bmsamax;		/* 特賞間最大差(最大値)		*/
	short	bmsamin;		/* 特賞間最大差(最小値)		*/
	short	tmotimax;		/* 特賞持玉(最大値)		*/
	short	tmotimin;		/* 特賞持玉(最小値)		*/

	ushort	kr_bonus;		/* CT中ﾚｷﾞｭﾗｰ回数(ｽﾛｯﾄのみ)	1999.02.03 osako */

	uchar	tflg;			/* 特賞ﾌﾗｸﾞ			*/
					/*	0x01 : T1(ﾋﾞｯｸﾞ)		*/
					/*	0x02 : T2(CT) 			*/
					/*	0x04 : ﾗｯｷ-				*/
					/*	0x08 : ﾚｷﾞｭﾗｰ(Sのみ)	*/
	uchar	dmy2;			/* ....				*/
	uchar	e_hour;			/* 最終時刻(時)			*/
	uchar	e_min;			/* 最終時刻(分)			*/
	ushort	e_t1out;		/* 最終特賞１中ｱｳﾄ		*/
	ushort	e_t1saf;		/* 最終特賞１中ｾｰﾌ		*/
	ushort	e_bout;			/* 最終特賞１間ｱｳﾄ		*/
	ushort	e_bsaf;			/* 最終特賞１間ｾｰﾌ		*/
	ushort	e_bstart;		/* 最終特賞１間ｽﾀｰﾄ		*/
	ushort	e_bmsa;			/* 最終特賞１間最大差(打込)	*/
	ushort	e_buri;			/* 最終特賞１間売上		*/
	ushort	e_motichg;		/* 最終持玉(交換)		*/
	ushort	e_motiever;		/* 最終持玉(無定量)		*/
#if	1					/* LEC.ICCARD 01/06/26 */
	ushort	hakkenuri;		/* ｻﾝﾄﾞ発券売上(100円)	*/
#else
	uchar	yobi2[2];		/* ....				*/
#endif
	ushort	san_ukos;		/* ｻﾝﾄﾞ売上個数			*/
	ushort	chk_dat;		/* CHK ﾃﾞｰﾀ			*/
	ulong	k_keisu;		/* 台景品個数		*/
	ulong	k_cyokos;		/* 台貯玉個数			*/
} FDATA_DAI;

/*#**************************************/
/*	台ﾃﾞｰﾀ2(累計)			*/
/****************************************/
typedef	struct {
	/* ｽﾀｰﾄｽﾗﾝﾌﾟ/ｽﾀｰﾄ分布 設定 */
/*000*/	uchar	staslp_item;		/* ｽﾗﾝﾌﾟ項目			*/
/*001*/	uchar	staslp_so;		/* SO値				*/
/*002*/	short	stabun_bo;		/* BO値				*/
/*004*/	short	stabun_kai;		/* 開始値			*/
/*006*/	uchar	stabun_ken;		/* 検索値			*/
/*007*/	uchar	dmy1;			/* ﾀﾞﾐｰ				*/

	/* ｽﾀｰﾄｽﾗﾝﾌﾟ/ｽﾀｰﾄ分布 ﾃﾞｰﾀ */
/*008*/	uchar	staslp_kisyu;		/* 機種No.			*/
/*009*/	uchar	staslp_sima;		/* 島No.			*/
/*010*/	uchar	dmy2[2];		/* ﾀﾞﾐｰ				*/
/*012*/	ushort	staslp_s[5];		/* S1ｽﾗﾝﾌﾟ度 1～5		*/
/*022*/	ushort	staslp_stotal;		/* SLｽﾗﾝﾌﾟﾄｰﾀﾙ件数		*/
/*024*/	ushort	stabun_b[6];		/* B1分布度 1～6		*/
/*036*/	ushort	stabun_btotal;		/* SB分布ﾄｰﾀﾙ件数		*/
/*038*/	uchar	yobi1[90];		/* 予備				*/
} FDATA_DAI2;

/*#**************************************/
/*	島･機種ﾃﾞｰﾀ			*/
/****************************************/
typedef	struct {
	ushort	daisu;			/* 台数				*/
	ushort	jflg;			/* 台情報ﾌﾗｸﾞ(台ﾃﾞｰﾀ参照)	*/
	ulong	out;			/* ｱｳﾄ				*/
	ulong	saf;			/* ｾｰﾌ				*/
	ulong	start;			/* ｽﾀｰﾄ				*/
	ulong	tout;			/* 特賞ｱｳﾄ			*/
	ulong	tsaf;			/* 特賞ｾｰﾌ			*/
	ulong	t2out;			/* 特賞2ｱｳﾄ			*/
	ulong	t2saf;			/* 特賞2ｾｰﾌ			*/
	ulong	t2start;		/* 特賞2ｽﾀｰﾄ			*/
	ushort	bcnt;			/* ﾍﾞｰｽ回数(特賞回数/打止回数)	*/
	ushort	ucnt;			/* 打止回数			*/
	ushort	tcnt;			/* 特賞回数  (T)		*/
	ushort	t1cnt;			/* 特賞1回数 (T1)		*/
	ushort	t2cnt;			/* 特賞2回数 (T2)		*/
	ushort	kcnt;			/* 確変回数((T2)>T1>T2>T1･･)	*/
	ushort	kt1cnt;			/* 確変中T1回数			*/

	ushort	lcnt;			/* ﾗｯｷｰ回数			*/
	ushort	lt1cnt;			/* ﾗｯｷｰ中T1回数			*/
	ushort	lt1max;			/* ﾗｯｷｰ中MAXT1回数		*/
	ulong	lout;			/* ﾗｯｷｰ中ｱｳﾄ			*/
	ulong	lsaf;			/* ﾗｯｷｰ中ｾｰﾌ			*/
	ulong	lbout;			/* ﾗｯｷｰT1間ｱｳﾄ			*/
	ulong	lmoti;			/* ﾗｯｷｰ中持玉			*/

	ulong	start1;			/* ｽﾀｰﾄ1			*/
	ulong	start2;			/* ｽﾀｰﾄ2			*/
	ulong	start3;			/* ｽﾀｰﾄ3			*/
	ulong	t2start1;		/* 特賞2(T2)中ｽﾀｰﾄ1		*/
	ulong	t2start2;		/* 特賞2(T2)中ｽﾀｰﾄ2		*/
	ulong	t2start3;		/* 特賞2(T2)中ｽﾀｰﾄ3		*/
	ulong	t1start2;		/* 特賞ｽﾀｰﾄ2回数		*/

	ulong	ukomi;			/* 打込玉			*/
	ulong	t1ukomi;		/* 特賞打込			*/
	ulong	maxmoti;		/* 最大持玉 MY			*/
	ulong	maxukomi;		/* 最大打込 M◇			*/

	ushort	kado;			/* 積稼動数			*/
	ushort	kyaku;			/* 客交代			*/

	ushort	hasfg;			/* 発生済ﾌﾗｸﾞ			*/
	ushort	door1;			/* ﾄﾞｱ1回数			*/
	ushort	door2;			/* ﾄﾞｱ2回数			*/
	ushort	call1;			/* 呼出1回数			*/
	ushort	call2;			/* 呼出2回数			*/
	uchar	fuscnt;			/* 不正回数			*/
	uchar	ddancnt;		/* 台断線回数			*/
	uchar	ijocnt;			/* 異常回数			*/
	uchar	tencnt;			/* 点検回数			*/
	uchar	g1dancnt;		/* 現金1断線回数		*/
	uchar	g2dancnt;		/* 現金2断線回数		*/
	uchar	cddancnt;		/* ｶｰﾄﾞ断線回数			*/
	uchar	uijocnt;		/* 売上異常回数			*/

	uchar	ukind;			/* 売上情報ﾌﾗｸﾞ(台ﾃﾞｰﾀ参照)	*/
	uchar	dmy1;			/* ....				*/
	ulong	gen1uri;		/* ｻﾝﾄﾞ現金1売上(100円)		*/
	ulong	gen2uri;		/* ｻﾝﾄﾞ現金2売上(100円)		*/
	ulong	carduri;		/* ｻﾝﾄﾞｶｰﾄﾞ売上(100円)		*/
	ulong	daiuri;			/* 台売上(100円)		*/
	ulong	hosei;			/* 売上補正値(円)		*/

	ushort	k_suu;			/* 会員数			*/
	ushort	k_kado;			/* 会員稼動数			*/
	ushort	k_t1cnt;		/* 会員特賞1回数 (T1)		*/
	ushort	k_tcnt;			/* 会員特賞回数	 (T)		*/
	ushort	k_kcnt;			/* 会員確変回数			*/
	ushort	k_lcnt;			/* 会員ﾗｯｷｰ回数			*/
	ulong	k_out;			/* 会員ｱｳﾄ			*/
	ulong	k_saf;			/* 会員ｾｰﾌ			*/
	ulong	k_tout;			/* 会員特賞ｱｳﾄ			*/
	ulong	k_tsaf;			/* 会員特賞ｾｰﾌ			*/
	ulong	k_start;		/* 会員ｽﾀｰﾄ			*/
	ulong	k_uri;			/* 会員売上(100円)		*/
	ulong	k_repkos;		/* 台ﾘﾌﾟﾚｲ個数			*/
	ulong	k_repkin;		/* 台ﾘﾌﾟﾚｲ金額(円)		*/
	ulong	k_reptesu;		/* 台ﾘﾌﾟﾚｲ手数料(個数)		*/
	ulong	k_reptesukin;		/* 台ﾘﾌﾟﾚｲ手数料(金額)(円)	*/
	ulong	k_cyokos;		/* 台貯玉個数			*/
	ulong	k_cyokin;		/* 台貯玉金額			*/
	ulong	k_keisu;		/* 台景品個数		*/

	ushort	r_bonus;		/* ﾚｷﾞｭﾗｰ回数(ｽﾛｯﾄのみ)	1999.02.03 osako */
	ushort	kr_bonus;		/* CT中ﾚｷﾞｭﾗｰ回数(ｽﾛｯﾄのみ)	1999.02.03 osako */

	ushort	g_kado;			/* 現稼動台数		2001.02.28 S.A add	*/
	ushort	g_k_kado;		/* 現会員稼働台数	2001.02.28 S.A add	*/

#if	1					/* LEC.ICCARD 01/06/26 */
	ulong	hakkenuri;		/* ｻﾝﾄﾞ発券売上(100円)	*/
	uchar	yobi[20];		/* ....				*/
#else
	uchar	yobi[24];		/* ....				*/
#endif

} FDATA_SK;

/*#**************************************/
/*	種別集計･ﾌﾛｱﾃﾞｰﾀ		*/
/****************************************/
typedef	struct {
	ushort	daisu;			/* 台数				*/
	ushort	jflg;			/* 台情報ﾌﾗｸﾞ(台ﾃﾞｰﾀ参照)	*/
	ulong	out;			/* ｱｳﾄ				*/
	ulong	saf;			/* ｾｰﾌ				*/
	ulong	start;			/* ｽﾀｰﾄ				*/
	ulong	tout;			/* 特賞ｱｳﾄ			*/
	ulong	tsaf;			/* 特賞ｾｰﾌ			*/
	ulong	t2out;			/* 特賞2ｱｳﾄ			*/
	ulong	t2saf;			/* 特賞2ｾｰﾌ			*/
	ulong	t2start;		/* 特賞2ｽﾀｰﾄ			*/
	ushort	bcnt;			/* ﾍﾞｰｽ回数(特賞回数/打止回数)	*/
	ushort	ucnt;			/* 打止回数			*/
	ushort	tcnt;			/* 特賞回数  (T)		*/
	ushort	t1cnt;			/* 特賞1回数 (T1)		*/
	ushort	t2cnt;			/* 特賞2回数 (T2)		*/
	ushort	kcnt;			/* 確変回数((T2)>T1>T2>T1･･)	*/
	ushort	kt1cnt;			/* 確変中T1回数			*/

	ushort	lcnt;			/* ﾗｯｷｰ回数			*/
	ushort	lt1cnt;			/* ﾗｯｷｰ中T1回数			*/
	ushort	lt1max;			/* ﾗｯｷｰ中MAXT1回数		*/
	ulong	lout;			/* ﾗｯｷｰ中ｱｳﾄ			*/
	ulong	lsaf;			/* ﾗｯｷｰ中ｾｰﾌ			*/
	ulong	lbout;			/* ﾗｯｷｰT1間ｱｳﾄ			*/
	ulong	lmoti;			/* ﾗｯｷｰ中持玉			*/

	ulong	ukomi;			/* 打込玉			*/
	ulong	t1ukomi;		/* 特賞打込			*/

	ushort	kado;			/* 積稼動数			*/
	ushort	kyaku;			/* 客交代			*/

	ushort	hasfg;			/* 発生済ﾌﾗｸﾞ			*/
	ushort	door1;			/* ﾄﾞｱ1回数			*/
	ushort	door2;			/* ﾄﾞｱ2回数			*/
	ushort	call1;			/* 呼出1回数			*/
	ushort	call2;			/* 呼出2回数			*/
	uchar	fuscnt;			/* 不正回数			*/
	uchar	ddancnt;		/* 台断線回数			*/
	uchar	ijocnt;			/* 異常回数			*/
	uchar	tencnt;			/* 点検回数			*/
	uchar	g1dancnt;		/* 現金1断線回数		*/
	uchar	g2dancnt;		/* 現金2断線回数		*/
	uchar	cddancnt;		/* ｶｰﾄﾞ断線回数			*/
	uchar	uijocnt;		/* 売上異常回数			*/

	uchar	ukind;			/* 売上情報ﾌﾗｸﾞ(台ﾃﾞｰﾀ参照)	*/
	uchar	dmy1;			/* ....				*/
	ulong	gen1uri;		/* ｻﾝﾄﾞ現金1売上(100円)		*/
	ulong	gen2uri;		/* ｻﾝﾄﾞ現金2売上(100円)		*/
	ulong	carduri;		/* ｻﾝﾄﾞｶｰﾄﾞ売上(100円)		*/
	ulong	daiuri;			/* 台売上(100円)		*/
	ulong	hosei;			/* 売上補正値(円)		*/

	ushort	k_suu;			/* 会員数			*/
	ushort	k_kado;			/* 会員稼動数			*/
	ushort	k_t1cnt;		/* 会員特賞1回数 (T1)		*/
	ushort	k_tcnt;			/* 会員特賞回数	 (T)		*/
	ushort	k_kcnt;			/* 会員確変回数			*/
	ushort	k_lcnt;			/* 会員ﾗｯｷｰ回数			*/
	ulong	k_out;			/* 会員ｱｳﾄ			*/
	ulong	k_saf;			/* 会員ｾｰﾌ			*/
	ulong	k_tout;			/* 会員特賞ｱｳﾄ			*/
	ulong	k_tsaf;			/* 会員特賞ｾｰﾌ			*/
	ulong	k_start;		/* 会員ｽﾀｰﾄ			*/
	ulong	k_uri;			/* 会員売上(100円)		*/
	ulong	k_repkos;		/* 台ﾘﾌﾟﾚｲ個数			*/
	ulong	k_reptesu;		/* 台ﾘﾌﾟﾚｲ手数料(個数)		*/
	ulong	k_cyokos;		/* 台貯玉個数			*/

	ulong	sg_danflg;		/* 島外断線済ﾌﾗｸﾞ		*/
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
	ulong	urikin;			/* 売上金額(合計)(100円)	*/
	ulong	t_keikos;		/* 景品個数(合計)		*/
	ulong	t_g_keikos;		/* 原価景品個数			*/
	ulong	sg_gen1uri;		/* 島外現金1売上(100円)		*/
	ulong	sg_gen2uri;		/* 島外現金2売上(100円)		*/
	ulong	sg_carduri;		/* 島外ｶｰﾄﾞ売上(100円)		*/
	ulong	sg_repkos;		/* 島外ﾘﾌﾟﾚｲ個数		*/
	ulong	sg_reptesu;		/* 島外ﾘﾌﾟﾚｲ手数料(個数)	*/
	ulong	sg_premkos;		/* 島外ﾌﾟﾚﾐｱﾑ個数		*/
	ulong	sg_cyokos;		/* 島外貯玉個数			*/
	ulong	kinko;			/* 金庫合計			01/06/26  金庫発券機分含まず  */
	ulong	sand;			/* ｻﾝﾄﾞ現金計			*/
	ulong	sandkinko;		/* ｻﾝﾄﾞ金庫計			*/
	ulong	poskos;			/* POS個数			*/
	ulong	g_poskos;		/* 原価POS個数			*/
	ulong	k_keisu;		/* 台景品個数		*/

	ushort	r_bonus;		/* ﾚｷﾞｭﾗｰ回数(ｽﾛｯﾄのみ)	1999.02.03 osako */
	ushort	kr_bonus;		/* CT中ﾚｷﾞｭﾗｰ回数(ｽﾛｯﾄのみ)	1999.02.03 osako */

	ushort	g_kado;			/* 現稼動台数		2001.02.28 S.A add	*/
	ushort	g_k_kado;		/* 現会員稼働台数	2001.02.28 S.A add	*/

#if	1					/* LEC.ICCARD 01/06/26 */
	ulong	hakkenuri;			/* ｻﾝﾄﾞ発券売上(100円)	台:売上3=サンド発券売上の計  */
	ulong	kinko_hakkenuri;	/* 金庫発券売上(100円)  金庫:表示番号2=1xxxの合計	*/
#else
	uchar	yobi[8];		/* ....				*/
#endif

} FDATA_SBT;

/*#**************************************/
/*	全店集計			*/
/****************************************/
typedef	struct {
	ushort	daisu;			/* 台数				*/
	ushort	jflg;			/* 台情報ﾌﾗｸﾞ(台ﾃﾞｰﾀ参照)	*/

	ushort	kado;			/* 積稼動数			*/

	ushort	hasfg;			/* 発生済ﾌﾗｸﾞ			*/

	uchar	g1dancnt;		/* 現金1断線回数		*/
	uchar	g2dancnt;		/* 現金2断線回数		*/
	uchar	cddancnt;		/* ｶｰﾄﾞ断線回数			*/
	uchar	uijocnt;		/* 売上異常回数			*/

	uchar	ukind;			/* 売上情報ﾌﾗｸﾞ(台ﾃﾞｰﾀ参照)	*/
	uchar	dmy1[3];		/* ....				*/
	ulong	gen1uri;		/* ｻﾝﾄﾞ現金1売上(100円)		*/
	ulong	gen2uri;		/* ｻﾝﾄﾞ現金2売上(100円)		*/
	ulong	carduri;		/* ｻﾝﾄﾞｶｰﾄﾞ売上(100円)		*/
	ulong	daiuri;			/* 台売上(100円)		*/
	ulong	hosei;			/* 売上補正値(円)		*/

	ushort	k_suu;			/* 会員数			*/
	ushort	k_kado;			/* 会員稼動数			*/
	ulong	k_uri;			/* 会員売上(100円)		*/
	ulong	k_repkin;		/* 台ﾘﾌﾟﾚｲ金額(円)		*/
	ulong	k_reptesukin;		/* 台ﾘﾌﾟﾚｲ手数料(金額)(円)	*/
	ulong	k_cyokin;		/* 台貯玉金額(円)		*/

	ulong	sg_danflg;		/* 島外断線済ﾌﾗｸﾞ(種別集計参照)	*/
	ulong	urikin;			/* 売上金額(合計)(100円)	*/
	ulong	t_keikin;		/* 景品金額(合計)(円)		*/
	ulong	t_g_keikin;		/* 原価景品金額(円)		*/
	ulong	sg_gen1uri;		/* 島外現金1売上(100円)		*/
	ulong	sg_gen2uri;		/* 島外現金2売上(100円)		*/
	ulong	sg_carduri;		/* 島外ｶｰﾄﾞ売上(100円)		*/
	ulong	sg_repkin;		/* 島外ﾘﾌﾟﾚｲ金額(円)		*/
	ulong	sg_reptesukin;		/* 島外ﾘﾌﾟﾚｲ手数料(金額)(円)	*/
	ulong	sg_premkin;		/* 島外ﾌﾟﾚﾐｱﾑ金額(円)		*/
	ulong	sg_cyokin;		/* 島外貯玉金額(円)		*/
	ulong	kinko;			/* 金庫合計			01/06/26  金庫発券機分含まず  */
	ulong	sand;			/* ｻﾝﾄﾞ現金計			*/
	ulong	sandkinko;		/* ｻﾝﾄﾞ金庫計			*/
	ulong	poskin;			/* POS金額(円)			*/
	ulong	g_poskin;		/* 原価POS金額(円)		*/

	ushort	cd_hfg;			/* ｶｰﾄﾞ発行ﾌﾗｸﾞ			*/
	ushort	cd_hdanfg;		/* ｶｰﾄﾞ発行断線ﾌﾗｸﾞ		*/
	ulong	cd_htotal;		/* ｶｰﾄﾞ発行合計金額(100円)	*/
	ulong	cd_h1kei;		/* ｶｰﾄﾞ発行1000円計(100円)	*/
	ulong	cd_h2kei;		/* ｶｰﾄﾞ発行2000円計(100円)	*/
	ulong	cd_h3kei;		/* ｶｰﾄﾞ発行3000円計(100円)	*/
	ulong	cd_h5kei;		/* ｶｰﾄﾞ発行5000円計(100円)	*/
	ulong	cd_h10kei;		/* ｶｰﾄﾞ発行10000円計(100円)	*/

	ulong	zihan;			/* 自販機計(100円)		*/
	ulong	ta;			/* その他計			*/

	long	d_keikin;		/* 台景品金額(100円)		*/
	long	g_d_keikin;		/* 原価台景品金額(100円)	*/
	long	hd_keikin;		/* 補正台景品金額(100円)	*/
	long	g_hd_keikin;		/* 補正原価台景品金額(100円)	*/
	long	k_keikin;		/* 台景品金額(貯玉･リプレイ)		*/
#if 1	/* 2000.12.05 台売上での粗利益計算の為、台売上からの景品金額追加	*/
/*	long	sa_kin;	*/		/* 台売上からの景品金額(円)			*//* 2000.12.05 台売上からの景品金額追加 */
	long	g_duri_keikin;	/* 台売上からの原価台景品金額(円)	*//* 2000.12.06 名称変更 */

	ushort	g_kado;			/* 現稼動台数		2001.02.28 S.A add	*/
	ushort	g_k_kado;		/* 現会員稼働台数	2001.02.28 S.A add	*/

#if	1					/* LEC.ICCARD 01/06/26 */
	ulong	hakkenuri;			/* ｻﾝﾄﾞ発券売上(100円)	台:売上3=サンド発券売上の計  */
	ulong	kinko_hakkenuri;	/* 金庫発券売上(100円)  金庫:表示番号2=1xxxの合計	*/

						/* LEC.ICCARD 01/09/06 */
	ulong	cd_seisanki;	/* カード精算機合計(100円)  カード発行機のマイナススケーラ入力	*/
	uchar	yobi[60];		/* ....				*/
#else
	uchar	yobi[72];		/* ....				*/
#endif

#else
	uchar	yobi[80];		/* ....				*/
#endif
} FDATA_TOTAL;

/*#**************************************/
/*	定時情報			*/
/****************************************/
typedef	struct {
	uchar	kiotim[2];		/* 記憶時刻 (時:分)		*/
	ushort	egyo;			/* 営業時間			*/
} FDATA_TIME_INF;

/*#**************************************/
/*	定時稼動情報			*/
/****************************************/
typedef	FDATA_TIME_INF	FDATA_KADO_INF;

/*#**************************************/
/*	定時機種稼動			*/
/****************************************/
typedef	struct {
	ushort	daisu;			/* 台数				*/
	ushort	tcnt;			/* 特賞回数			*/
	ulong	out;			/* ｱｳﾄ				*/
	ulong	saf;			/* ｾｰﾌ				*/
	ulong	urikin;			/* 売上(100円)			*/
	ulong	repkos;			/* ﾘﾌﾟﾚｲ個数			*/
	ushort	kado;			/* 現稼動数			*/
	ushort	k_kado;			/* 現会員稼動数			*/
	ulong	startreg;		/* P:ｽﾀｰﾄ回数(T2中ｽﾀｰﾄ含)	*/
					/* S:ﾚｷﾞｭﾗｰ回数			*/
	ulong	startgame;		/* P:ｽﾀｰﾄ回数(ﾍﾞｰｽ中)		*/
					/* S:ｹﾞｰﾑ回数(ﾍﾞｰｽｱｳﾄ/3)	*/
	ushort	t2cnt;			/* P:確変回数			*/
					/* S:CT回数			*/
	ushort	kcnt;			/* P:確変突入回数		*/
					/* S:CT突入回数			*/
	ushort	t1cnt;			/* P:特賞1回数			*/
					/* S:ﾋﾞｯｸﾞ回数			*/
	uchar	yobi[2];		/* ....				*/
	ulong	reptesu;		/* ﾘﾌﾟﾚｲ手数料(個数)		*/
} FDATA_KADO_KISHU;

/*#**************************************/
/*	定時種別稼動			*/
/****************************************/
typedef	struct {
	ushort	daisu;			/* 台数				*/
	ushort	tcnt;			/* 特賞回数			*/
	ulong	out;			/* ｱｳﾄ				*/
	ulong	saf;			/* ｾｰﾌ				*/
	ulong	urikin;			/* 売上(100円)			*/
	ulong	repkos;			/* ﾘﾌﾟﾚｲ個数			*/
	ushort	kado;			/* 現稼動数			*/
	ushort	k_kado;			/* 現会員稼動数			*/
	ulong	keikos;			/* 景品個数			*/
	ulong	startreg;		/* P:ｽﾀｰﾄ回数(T2中ｽﾀｰﾄ含)	*/
					/* S:ﾚｷﾞｭﾗｰ回数			*/
	ulong	startgame;		/* P:ｽﾀｰﾄ回数(ﾍﾞｰｽ中)		*/
					/* S:ｹﾞｰﾑ回数(ﾍﾞｰｽｱｳﾄ/3)	*/
	ushort	t2cnt;			/* P:確変回数			*/
					/* S:CT回数			*/
	ushort	kcnt;			/* P:確変突入回数		*/
					/* S:CT突入回数			*/
	ushort	t1cnt;			/* P:特賞1回数			*/
					/* S:ﾋﾞｯｸﾞ回数			*/
	uchar	yobi[2];		/* ....				*/
	ulong	reptesu;		/* ﾘﾌﾟﾚｲ手数料(個数)		*/
} FDATA_KADO_SBT;

/*#**************************************/
/*	定時全店稼動			*/
/****************************************/
typedef	struct {
	ushort	daisu;			/* 台数				*/
	ushort	tcnt;			/* 特賞回数			*/
	long	yobi[2];		/* 予備				*/
	ulong	urikin;			/* 売上(100円)			*/
	ulong	hosei;			/* 売上補正値(円)		*/
	ushort	kado;			/* 現稼動数			*/
	ushort	k_kado;			/* 会員稼動数			*/
	ulong	keikin;			/* 景品金額(100円)		*/
	long	g_keikin;		/* 原価台景品金額(100円)	*/
	long	d_keikin;		/* 台景品金額(100円)		*/
	long	g_d_keikin;		/* 原価台景品金額(100円)	*/
	long	hd_keikin;		/* 補正台景品金額(100円)	*/
	long	g_hd_keikin;		/* 補正原価台景品金額(100円)	*/
} FDATA_KADO_ZEN;

/*#**************************************/
/*	金庫集計			*/
/****************************************/
typedef	struct {
	uchar	sima[4];		/* 島番号 [0]:1 - [3]:4		*/
	ulong	gen1kei;		/* 現金1計(100円)		*/
	ulong	gen2kei;		/* 現金2計(100円)		*/

#if	1				/* LEC.ICCARD 01/06/26 */
	ulong	hakkenuri;		/* ｻﾝﾄﾞ発券売上(100円)	台:売上3=サンド発券売上の計  */
	ulong	kinko_hakkenuri;	/* 金庫発券売上(100円)  金庫:表示番号2=1xxxの合計	*/
#else
	ulong	sandkinko;		/* ｻﾝﾄﾞ金庫計			*/
	ulong	cardkei;		/* ｶｰﾄﾞ計(100円)		*/
#endif

	ulong	kinko;			/* 金庫計			01/06/25 金庫発券売上含む  */
	uchar	g1dancnt;		/* 現金1断線回数		*/
	uchar	g2dancnt;		/* 現金2断線回数		*/
	uchar	skkdancnt;		/* ｻﾝﾄﾞ金庫断線回数		*/
	uchar	cddancnt;		/* ｶｰﾄﾞ断線回数			*/
	uchar	kkdancnt;		/* 金庫断線回数			*/
	uchar	errcnt;			/* ｴﾗｰ回数			*/
	uchar	doorcnt;		/* ﾄﾞｱ回数			*/
	uchar	kktencnt;		/* 金庫点検回数			*/
#if 0
	uchar	flg;			/* ﾌﾗｸﾞ				*/
	uchar	yobi;			/* ....				*/
#endif
} FDATA_KINKO;

/*#**************************************/
/*	全店ﾃﾞｰﾀ(島ｺﾝ)			*/
/****************************************/
typedef	struct {
	long	indata;			/* 計数値			*/
	char	dancnt;			/* 断線回数			*/
	char	err_door;		/* ｴﾗｰ回数  0xf0		*/
					/* ﾄﾞｱ回数  0x0f		*/
	char	othercnt;		/* その他回数			*/
	char	flg;			/* ﾌﾗｸﾞ				*/
} FDATA_SMCON;

#endif



/* WAIS END */
