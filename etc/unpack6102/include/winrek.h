#ifndef __winrek_h__
#define __winrek_h__

#include "asdef.h"
#include "Rs_kioku.h"


//------------------------------------------------------------
// システムクリア履歴構造体
//------------------------------------------------------------
typedef struct {
	short	syubetu;				// ０＝システムクリア　１＝設定クリア　２＝記憶クリア
	short	yobi;					// 予備
	long	Date[6];				//	システムクリア年月日		[0] = 年   [1] = 月 [2]=日
									//								[3] = 曜日 [4] = 時 [5]=分
} SysClear_Rek_t;

//------------------------------------------------------------
// 設定更新履歴構造体
//------------------------------------------------------------
typedef struct {
	long	upd_date[6];		//	更新年月日		[0] = 年　 [1] = 月　[2] = 日
								//					[3] = 曜日 [4] = 時  [5] = 分
	short	upd_fno;			// 更新ファイルＮＯ
	short	yobi;
} settei_upd_t;

//------------------------------------------------------------
// 稼動数・会員稼動数履歴構造体
//------------------------------------------------------------
typedef union{
	char size[512];
	struct{
		long flag;			// =1　記録済みレコード
		char hm[2];			// 履歴時／分
		short kado;			// 稼動数
		short kaiKado;		// 会員稼動数
		short reserved;		// 予備
	} d;
} KadoRek_t;



//------------------------------------------------------------
// 営業履歴構造体
//------------------------------------------------------------
typedef struct _EIREKI_KEN{
	unsigned long kensuu;
	unsigned long dmy1;
	unsigned long dmy2;
	unsigned long dmy3;
} eireki_KEN;

typedef struct _EIGYO_RIREKI_A{
	unsigned char	code1;
	unsigned char	code2;
	unsigned short	hhmms0;
	unsigned short	data1;
	unsigned short	data2;
	unsigned short	data3;
	unsigned short	data4;
	long			ldata5;
} eigyou_rireki_A;

typedef struct _EIGYO_RIREKI_B{
	unsigned char	code1;
	unsigned char	code2;
	unsigned short	hhmms0;
	unsigned short	data1;
	unsigned short	data2;
	unsigned short	data3;
	unsigned short	data4;
	unsigned short	data5;
	unsigned short	data6;
} eigyou_rireki_B;

typedef struct _EIGYO_RIREKI_C{
	unsigned char	code1;
	unsigned char	code2;
	unsigned short	hhmms0;
	unsigned char	data1;
	unsigned char	data2;
	unsigned char	data3;
	unsigned char	data4;
	unsigned char	data5;
	unsigned char	data6;
	unsigned char	data7;
	unsigned char	data8;
	unsigned char	data9;
	unsigned char	data10;
	unsigned char	data11;
	unsigned char	data12;

} eigyou_rireki_C;

//------------------------------------------------------------
// 営業履歴共用体
//------------------------------------------------------------

typedef union _EIGYO_RIREKI {

	eireki_KEN			eireki_KEN;
	eigyou_rireki_A		e_rireki_A;
	eigyou_rireki_B		e_rireki_B;
	eigyou_rireki_C		e_rireki_C;
} EIGYO_RIREKI;

//------------------------------------------------------------
// 夜間履歴構造体
//------------------------------------------------------------
typedef struct _YAKAN_RIREKI {
	
	unsigned char	code1;
	unsigned char	code2;
	unsigned short	hhmms0;
	unsigned short	no;
	unsigned short	state;
	unsigned short	stop;
	unsigned short	data;
	unsigned short	yobi;
	unsigned short	mmdd;
} YAKAN_RIREKI;

//------------------------------------------------------------
// 特賞履歴共用体
//------------------------------------------------------------
typedef struct _TOKUSYOU_RIREKI_KEN{
	unsigned long kensuu;
	unsigned long dmy1[7];
} tokureki_KEN;

typedef union _TOKUSYOU_RIREKI {
	tokureki_KEN	toku_ken;
	TOKREK_KIOKU	toku_rek;
} TOKU_RIREKI;


//------------------------------------------------------------
// パネルデータ格納用構造体
//------------------------------------------------------------
typedef struct _dai_status {
unsigned long dai_status[DAISU];
} dai_status_t;

//------------------------------------------------------------
// 割数履歴構造体
//------------------------------------------------------------
typedef union{
	char size[2048];
	struct{
		long flag;						// =1　記録済みレコード
		char hm[2];						// 履歴時／分
		short reserved;					// 予備
		double kikai[SHUBETSU+1];		// 機械割数(P,A,S,X,全店)
		double keihin[SHUBETSU+1];		// 景品割数(P,A,S,X,全店)
		double hos_kikai[SHUBETSU+1];	// 補正機械割数(P,A,S,X,全店)
		double hos_keihin[SHUBETSU+1];	// 補正景品割数(P,A,S,X,全店)
		double kis_kikai[KISHU];			// 機種別機械割数
		double kis_hoskikai[KISHU];		// 機種別補正機械割数
	} d;
} WarisuuRek_t;

// 2002.11.07 会員遊技履歴 追加->
#define MAX_KAILOG	(KAIINLOG_RSUU + DAISU)
//------------------------------------------------------------
// 会員遊技履歴構造体
//------------------------------------------------------------
typedef struct _wssfno5201_00 {
	unsigned long	qs_idx;			/* ﾚｺｰﾄﾞｲﾝﾃﾞｯｸｽ(qsort用)*/// 2002.12.11 kaoru 対策(会員履歴が台番順になる)
#if 0
	unsigned char	dcode1;			/* 電文ｺｰﾄﾞ1 0x22固定	*/
	unsigned char	dcode2;			/* 電文ｺｰﾄﾞ2 ox03固定	*/
	unsigned char	subcode;		/* ｻﾌﾞｺｰﾄﾞ				*/
									/*	3 = 稼動情報		*/
									/*	4 = ｶｰﾄﾞｱｳﾄ			*/

	unsigned char	yobi1;			/* ......				*/
	unsigned char	yobi2[2];		/* ......				*/
	unsigned char	format;			/* ﾌｫｰﾏｯﾄ 0x02固定		*/
	unsigned char	jflg;			/* 状態ﾌﾗｸﾞ 0x00固定	*/
#endif
	unsigned long	kaiid;			/* 会員ID				*/
	unsigned char	subno;			/* ｻﾌﾞ番号				*/
	unsigned char	pas;			/* 種別					*/
	unsigned short	daino;			/* 台表示番号			*/
	unsigned char	intim[2];		/* ｶｰﾄﾞIN 時刻(BCD)		*/
	unsigned char	outtim[2];		/* ｶｰﾄﾞOUT 時刻(BCD)	*/
	unsigned char	t1cnt;			/* T1回数				*/
	unsigned char	k1cnt;			/* 確変１回数			*/
	unsigned short	out;			/* 累計ｱｳﾄ				*/
	unsigned short	saf;			/* 累計ｾｰﾌ				*/
	unsigned short	genuri;			/* ｻﾝﾄﾞ現金1+現金2売上	*/
	unsigned short	carduri;		/* ｻﾝﾄﾞｶｰﾄﾞ売上			*/
	unsigned short	saf_ov;			/* ｾｰﾌのあふれ分		*/
	unsigned long	cyokos;			/* 貯玉					*/
	unsigned short	start;			/* 累計ｽﾀｰﾄ				*/
	unsigned short	ukomi;			/* 打込玉				*/
	unsigned char	tcnt;			/* 特賞					*/
	unsigned char	t2cnt;			/* 特賞2回数			*/
	unsigned char	kcnt;			/* 確変回数				*/
	unsigned char	kt1cnt;			/* 確変中T1回数			*/
	unsigned short	t1out;			/* 特賞中ｱｳﾄ-特賞2中ｱｳﾄ	*/
	unsigned short	t1saf;			/* 特賞中ｾｰﾌ-特賞2中ｾｰﾌ	*/
	unsigned short	t2out;			/* 特賞2ｱｳﾄ				*/
	unsigned short	t2saf;			/* 特賞2ｾｰﾌ				*/
	unsigned short	t2start;		/* 特賞2ｽﾀｰﾄ			*/
	unsigned short	point;			/* ﾎﾟｲﾝﾄ数				*/
	unsigned short	cdin_errcode;	/* ｶｰﾄﾞIN 通信ｴﾗｰｺｰﾄﾞ	*/
	unsigned short	cdout_errcode;	/* ｶｰﾄﾞOUT 通信ｴﾗｰｺｰﾄﾞ	*/
	unsigned short	repkos;			/* 再ﾌﾟﾚｲ数				*/
	unsigned short	reptesu;		/* 再ﾌﾟﾚｲ手数料			*/
} wssfno5201_00_t;

typedef wssfno5201_00_t RPLY_LOG;	/* 遊技履歴(累計ﾃﾞｰﾀ)	*/
typedef wssfno5201_00_t KPLY_LOG;	/* 遊技履歴(個々ﾃﾞｰﾀ)	*/
typedef wssfno5201_00_t KAIIN_LOG;	/* 会員履歴				*/
typedef wssfno5201_00_t KAIPLY_LOG;	/* 会員遊技履歴			*/
// 2002.11.07 会員遊技履歴 追加<-

#endif

