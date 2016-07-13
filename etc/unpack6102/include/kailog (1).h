/* WAIS START */
#ifndef __kailog_h__
#define __kailog_h__


/*#**************************************/
/*	会員データ			*/
/****************************************/
typedef	struct {
	unsigned char	dcode1;			/* 電文ｺｰﾄﾞ1 0x22固定	*/
	unsigned char	dcode2;			/* 電文ｺｰﾄﾞ2 ox03固定	*/
	unsigned char	subcode;		/* ｻﾌﾞｺｰﾄﾞ		*/
					/*	3 = 稼動情報	*/
					/*	4 = ｶｰﾄﾞｱｳﾄ	*/

	unsigned char	yobi1;			/* ......		*/
	unsigned char	yobi2[2];		/* ......		*/
	unsigned char	format;			/* ﾌｫｰﾏｯﾄ 0x02固定	*/
	unsigned char	jflg;			/* 状態ﾌﾗｸﾞ 0x00固定	*/
	unsigned long	kaiid;			/* 会員ID		*/
	unsigned char	subno;			/* ｻﾌﾞ番号		*/
	unsigned char	pas;			/* 種別			*/
	unsigned short	daino;			/* 台			*/
	unsigned char	intim[2];		/* ｶｰﾄﾞIN 時刻(BCD)	*/
	unsigned char	outtim[2];		/* ｶｰﾄﾞOUT 時刻(BCD)	*/
	unsigned char	t1cnt;			/* T1回数		*/
	unsigned char	k1cnt;			/* 確変１回数		*/
	unsigned short	out;			/* 累計ｱｳﾄ		*/
	unsigned short	saf;			/* 累計ｾｰﾌ		*/
	unsigned short	genuri;			/* ｻﾝﾄﾞ現金1+現金2売上	*/
	unsigned short	carduri;		/* ｻﾝﾄﾞｶｰﾄﾞ売上		*/
#if 1								/* 2001.10.12	*/
	unsigned short	saf_ov;			/* ｾｰﾌのあふれ分 add 2001.10.12	*/
#else
	unsigned char	yobi3[2];		/* .....		*/
#endif

#if 1	/* 99/11/02 */
	unsigned long	cyokos;			/* 貯玉			*/
#else
	unsigned long	saverno;		/* ｻｰﾊﾞ管理番号		*/
#endif
	unsigned short	start;			/* 累計ｽﾀｰﾄ		*/
	unsigned short	ukomi;			/* 打込玉		*/
	unsigned char	tcnt;			/* 特賞			*/
	unsigned char	t2cnt;			/* 特賞２回数		*/
	unsigned char	kcnt;			/* 確変回数		*/
	unsigned char	kt1cnt;			/* 確変中T1回数		*/
	unsigned short	t1out;			/* 特賞中ｱｳﾄ - 特賞２中ｱｳﾄ	*/
	unsigned short	t1saf;			/* 特賞中ｾｰﾌ - 特賞２中ｾｰﾌ	*/
	unsigned short	t2out;			/* 特賞2ｱｳﾄ			*/
	unsigned short	t2saf;			/* 特賞2ｾｰﾌ			*/
	unsigned short	t2start;		/* 特賞2ｽﾀｰﾄ			*/
	unsigned short	point;			/* ﾎﾟｲﾝﾄ数		*/
	unsigned short	cdin_errcode;		/* ｶｰﾄﾞIN 通信ｴﾗｰｺｰﾄﾞ	*/
	unsigned short	cdout_errcode;		/* ｶｰﾄﾞOUT 通信ｴﾗｰｺｰﾄﾞ	*/
#if 1	/* 99/11/02 */
	unsigned short	repkos;			/* 再ﾌﾟﾚｲ数		*/
	unsigned short	reptesu;		/* 再ﾌﾟﾚｲ手数料	*/
#else
	unsigned char	yobi4[4];		/* .....		*/
#endif
} RDATA_KAIIN;


/*#**************************************/
/* 会員管理情報ﾃｰﾌﾞﾙ			*/
/*#**************************************/
typedef	struct {
					/* 当日情報			*/
	unsigned long	today_t_ymd;			/* 開店時の年月日	*/
	unsigned short	ken;				/* FROMﾗｲﾄ件数		*/
	unsigned short	max;				/* FROMﾗｲﾄMAX件数	*/

/* 2001.09.06 t.kaneko */
#if 0
					/* 前日情報			*/
	unsigned long	save_t_ymd;			/* 開店時の年月日	*/
	unsigned short	save_ken;			/* FROMﾗｲﾄ件数		*/
	unsigned short	save_max;			/* FROMﾗｲﾄMAX件数	*/
#else
	ulong	kioku_pw_tim;			/* 記憶日付			*/
	ushort	kioku_no;			/* 記憶No.(1～7)		*/
	ushort	sync;				/* ｼﾝｸｺｰﾄﾞ 0x87cd		*/
	ushort	yobi1[8];			/* 予備				*/
#endif
/* 2001.09.06 t.kaneko */

} RDATA_KAIININF;


#endif		/* __kailog_h__ */

/* WAIS END */
