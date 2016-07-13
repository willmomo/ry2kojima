/* WAIS START */
//------------------------------------------------------------
//概要
//   物理領域構造体宣言
//
//作成者
//   Tokai水谷
//
//備考
//   物理領域の構造体を宣言する。
//------------------------------------------------------------
#if !defined(L_WSS_STR)
#define	L_WSS_STR

//	2002.04.16 追加 By Fujiki-->
#include "rs_kioku.h"
#include "APL_DEF.H"
//	-->2002.04.16 追加 By Fujiki

#ifdef L_INF_FILE
//------------------------------------------------------------
//	INDEXｴﾘｱ物理ﾃｰﾌﾞﾙ
//------------------------------------------------------------
//typedef struct {
//	IDX_SIMA sima;						// 島ｲﾝﾃﾞｯｸｽ
//	IDX_KISHU kishu;					// 機種ｲﾝﾃﾞｯｸｽ
//	IDX_KINKO kinko[MAX_KINKO];			// 金庫ｲﾝﾃﾞｯｸｽ
//} IDX_REALTABLE;

/*#**************************************/
/*	現在情報ﾎﾟｲﾝﾀﾃｰﾌﾞﾙ		*/
/****************************************/
//typedef struct {
//	INF_KIOKU *kioku;		/* 記憶情報			*/
					/*   [0]:当日			*/
					/*   [1]:1日前～[7]:7日前	*/
					/*   [8]:1週間平均		*/
//} INF_TABLE;
#endif

#ifdef L_DAT_FILE
//------------------------------------------------------------
//	ﾃﾞｰﾀｴﾘｱ物理ﾃｰﾌﾞﾙ
//------------------------------------------------------------
typedef struct {
	FDATA_DAI dai[MAX_DAI];				// 台ﾃﾞｰﾀ(島ｺﾝ)
	FDATA_SMCON smcon[MAX_SMCON_IN];	// 全店ﾃﾞｰﾀ(島ｺﾝ)
	FDATA_SK sima[MAX_SIMA];			// 島集計ﾃﾞｰﾀ
	FDATA_SK kishu[MAX_KISHU];			// 機種集計ﾃﾞｰﾀ
	FDATA_SBT sbt[MAX_PAS];				// 種別集計ﾃﾞｰﾀ
	FDATA_TOTAL total;					// 全店集計ﾃﾞｰﾀ
	FDATA_KINKO kinko[MAX_KINKO];		// 金庫集計ﾃﾞｰﾀ

	// 2002.04.15 追加 By Fujiki-->
	FDATA_TIME_INF timeinf[MAX_TIMESYU];// 定時情報ﾃｰﾌﾞﾙ Max16件
	FDATA_SBT sbtShu_Seki[MAX_TIMESYU][MAX_PAS];	// 定時種別集計ﾃﾞｰﾀ（積）
	FDATA_SBT sbtShu_Sa[MAX_TIMESYU][MAX_PAS];		// 定時種別集計ﾃﾞｰﾀ（差）
	FDATA_TOTAL zenShu_Seki[MAX_TIMESYU];			// 定時全店集計ﾃﾞｰﾀ（積）
	FDATA_TOTAL zenShu_Sa[MAX_TIMESYU];				// 定時全店集計ﾃﾞｰﾀ（差）
	// -->2002.04.15 追加 By Fujiki
} FDATA_REALTABLE;
#endif

#define ARPOS_ZENTEN		0
#define ARPOS_TENPO_A		1
#define ARPOS_TENPO_B		2
#define ARPOS_SYUBET_TOP	3

typedef struct {
	long daisu;						// 台数
	char name[32];					// 名前
	long out;						// アウト
	long safe;						// セーフ
	long sa;						// 差玉
	double dedama;					// 出玉率
	long tcnt;						// 特賞回数
	long ucnt;						// 打止回数
	double kado_par;				// 稼働率
	long gen_kado;					// 現稼動数
	long seki_kado;					// 積稼動数
	long gen_member_kado;			// 現会員数
	long member_kado;				// 積会員数
	long uriage;					// 売上金額
	double hd_keihin_kin;			// 機械景品金額
	double keihin_kei;				// 景品金額
	double hosei_arari2;			// 機械粗利益
	double hosei_arari;				// 粗利益
	double hosei_ewari;				// 機械割数
	double hosei_keihin_wari;		// 景品割数
	double hosei_kyaku_tanka;		// 客単価
	double hosei_kyaku_tai_par;		// 客滞率
	long hosei_tama_tanka_dai_uri;	// 玉単価
	long hosei_tama_arari_dai_uri;	// 玉粗利
	double hosei_gosa_tama;			// 誤差玉数
	long gen_syukei_sand;			// 現金(サンド)
	long gen_syukei_kinko;			// 現金(金庫)
	long card_uriage;				// カード売上
	double replay;					// リプレイ売上
	long hosei_uriage;				// 補正売上
} SUMDATA_ZENTEN_SYUKEI_SYUBET;

typedef struct {
	long daisu;						// 台数
	char name[32];					// 名前
	long out;						// アウト
	long safe;						// セーフ
	long sa;						// 差玉
	long dedama;					// 出玉率
	double tcnt;					// 特賞回数
	double ucnt;					// 打止回数
	long kado_par;					// 稼働率
	long gen_kado;					// 現稼動数
	double seki_kado;				// 積稼動数
	long gen_member_kado;			// 現会員数
	double member_kado;				// 積会員数
	long uriage;					// 売上金額
	double hd_keihin_kin;			// 機械景品金額
	double keihin_kei;				// 景品金額
	double hosei_arari2;			// 機械粗利益
	double hosei_arari;				// 粗利益
	long hosei_ewari;				// 機械割数
	long hosei_keihin_wari;			// 景品割数
	long hosei_kyaku_tanka;			// 客単価
	long hosei_kyaku_tai_par;		// 客滞率
	double hosei_tama_tanka_dai_uri;	// 玉単価
	double hosei_tama_arari_dai_uri;	// 玉粗利
	double hosei_gosa_tama;			// 誤差玉数
	long gen_syukei_sand;			// 現金(サンド)
	long gen_syukei_kinko;			// 現金(金庫)
	long card_uriage;				// カード売上
	double replay;					// リプレイ売上
	long hosei_uriage;				// 補正売上
} AVGDATA_ZENTEN_SYUKEI_SYUBET;

typedef struct {
	long sand_hakken;		// サンド発券
	long sima_gai_hakken;	// 島外発券
	long card_seisanki;		// カード精算機
	long card_hakken;		// カード発券
	long card_hakken_total;	// カード発券計
} KENBAI_DATA;

typedef struct {
	SUMDATA_ZENTEN_SYUKEI_SYUBET sumData[MAX_PAS + 3];
	AVGDATA_ZENTEN_SYUKEI_SYUBET avgData[MAX_PAS + 3];
	KENBAI_DATA kenbaiData;
} ZENTEN_SYUKEI_SYUBET;

//---------------------------------------------------------------------
// [概要]
// 機種別定時稼動数 を表現するための構造体
//---------------------------------------------------------------------

typedef struct {
	long kno;					// 機種番号
	char ksh_name[32];			// 機種名
	long sbt;					// 論理種別番号
	char sbt_name[32];			// 種別名
	long daisu;					// 台数
	long kado_data[34+4];		// 稼動データ
} KISHU_TEIJI_KADO;

typedef struct {
	long kno;					// (未使用)
	char ksh_name[32];			// '小店舗名'+'種別名'を格納
	long sbt;
	char sbt_name[32];
	long daisu;
	long kado_data[34+4];
} SBT_TEIJI_KADO;

typedef struct {
	long kno;					// (未使用)
	char ksh_name[32];			// '大店舗名'を格納
	long sbt;					// (未使用)
	char sbt_name[32];			// (未使用)
	long daisu;
	long kado_data[34+4];
} ZEN_TEIJI_KADO;

typedef struct {
	KISHU_TEIJI_KADO kishu_teiji_kado[MAX_KISHU];	// 機種別データ
	SBT_TEIJI_KADO sbt_teiji_kado[MAX_PAS];
	ZEN_TEIJI_KADO zen_teiji_kado;
} TEIJI_KADO;

//---------------------------------------------------------------------
// [概要]
// 特賞履歴に連続台番号を格納するための構造体
// ホールナビII用の特殊型
//---------------------------------------------------------------------

typedef struct {
	unsigned short rdno;	// 連続台番号
	unsigned short kno;		// 機種番号
	TOKREK_KIOKU tokrek;	// 特賞履歴1レコード分
} fno9023_t;

//-- HN2多店舗対応 --//
//---------------------------------------------------------------------
// [概要]
// 多店舗対応版 全店集計(9030) を表現するための構造体
//---------------------------------------------------------------------

typedef struct {
	long daisu;						// 台数
	char name[32];					// 名前
	long out;						// アウト
	long safe;						// セーフ
	long sa;						// 差玉
	double dedama;					// 出玉率
	long tcnt;						// 特賞回数
	long ucnt;						// 打止回数
	double kado_par;				// 稼働率
	long gen_kado;					// 現稼動数
	long seki_kado;					// 積稼動数
	long gen_member_kado;			// 現会員数
	long member_kado;				// 積会員数
	long uriage;					// 売上金額
	double hd_keihin_kin;			// 機械景品金額
	double keihin_kei;				// 景品金額
	double hosei_arari2;			// 機械粗利益
	double hosei_arari;				// 粗利益
	double hosei_arari_inc_rplay;	// [NEW] 補正粗利益(再プレイ含)
	double hosei_ewari;				// 機械割数
	double hosei_keihin_wari;		// 景品割数
	double hosei_kyaku_tanka;		// 客単価
	double hosei_kyaku_tai_par;		// 客滞率
	double hosei_tama_tanka_dai_uri;	// 玉単価
	double hosei_tama_arari_dai_uri;	// 玉粗利
	double hosei_gosa_tama;			// 誤差玉数
	long gen_syukei_sand;			// 現金(サンド)
	long gen_syukei_kinko;			// 現金(金庫)
	long card_uriage;				// カード売上
	double replay;					// リプレイ売上
	long hosei_uriage;				// 補正売上
	long sg_genkin;					// [NEW] 島外貸機[現金]
	long sand_card;					// [NEW] サンド計[カード]
	long sand_kenbai;				// [NEW] サンド計[券売]
	long sg_kenbai;					// [NEW] 島外貸機[券売]
	long seisan_kenbai;				// [NEW] 精算機計[券売]
	long total_kenbai;				// [NEW] 券売合計
	long kenbai_nyukin;				// [NEW] 券売入金計
	long genkin_aridaka;			// [NEW] 現金有高
	double replay_tamasu;				// [NEW] 再プレイ玉数
	long replay_tesuryo_tamasu;		// [NEW] 再プレイ手数料数
	long chodama_pm_su;				// [NEW] 貯玉増減数
	long ruikei_chodama_su;			// [NEW] 累計貯玉数
	long pos_keihin_kingaku;		// [NEW] 景品金額(POS)
	long pos_koukan_kingaku;		// [NEW] 交換金額
	long pos_keihin_rieki;			// [NEW] 景品利益
	long pos_jitsu_keihin_kingaku;	// [NEW] 実景品金額(POS)
	long pos_jitsu_koukan_kingaku;	// [NEW] 実交換金額
	long pos_azukariken_yomitori_tamasu;	// [NEW] POS預かり券読取玉数
	long pos_chodama_hikidasi_tamasu;	// [NEW] POS貯玉引出数
	long pos_keihin_koukan_tamasu;		// [NEW] POS景品交換玉数
	long pos_chodama_azukeire_tamasu;	// [NEW] POS貯玉預入玉数
	long pos_amari_tamasu;			// [NEW] POS余り玉数(取込玉数)
	long tp_azukaritama_hakken_tamasu;	// [NEW] TP預かり玉発券玉数
} SUMDATA_ZENTEN_SYUKEI_SYUBET_9030;

typedef struct {
	long daisu;						// 台数
	char name[32];					// 名前
	long out;						// アウト
	long safe;						// セーフ
	long sa;						// 差玉
	long dedama;					// 出玉率
	double tcnt;					// 特賞回数
	double ucnt;					// 打止回数
	long kado_par;					// 稼働率
	long gen_kado;					// 現稼動数
	double seki_kado;				// 積稼動数
	long gen_member_kado;			// 現会員数
	double member_kado;				// 積会員数
	long uriage;					// 売上金額
	double hd_keihin_kin;			// 機械景品金額
	double keihin_kei;				// 景品金額
	double hosei_arari2;			// 機械粗利益
	double hosei_arari;				// 粗利益
	double hosei_arari_inc_rplay;	// [NEW] 補正粗利益(再プレイ含)
	long hosei_ewari;				// 機械割数
	long hosei_keihin_wari;			// 景品割数
	long hosei_kyaku_tanka;			// 客単価
	long hosei_kyaku_tai_par;		// 客滞率
	double hosei_tama_tanka_dai_uri;	// 玉単価
	double hosei_tama_arari_dai_uri;	// 玉粗利
	double hosei_gosa_tama;			// 誤差玉数
	long gen_syukei_sand;			// 現金(サンド)
	long gen_syukei_kinko;			// 現金(金庫)
	long card_uriage;				// カード売上
	double replay;					// リプレイ売上
	long hosei_uriage;				// 補正売上
	long sg_genkin;					// [NEW] 島外貸機[現金]
	long sand_card;					// [NEW] サンド計[カード]
	long sand_kenbai;				// [NEW] サンド計[券売]
	long sg_kenbai;					// [NEW] 島外貸機[券売]
	long seisan_kenbai;				// [NEW] 精算機計[券売]
	long total_kenbai;				// [NEW] 券売合計
	long kenbai_nyukin;				// [NEW] 券売入金計
	long genkin_aridaka;			// [NEW] 現金有高
	double replay_tamasu;				// [NEW] 再プレイ玉数
	long replay_tesuryo_tamasu;		// [NEW] 再プレイ手数料数
	long chodama_pm_su;				// [NEW] 貯玉増減数
	long ruikei_chodama_su;			// [NEW] 累計貯玉数
	long pos_keihin_kingaku;		// [NEW] 景品金額(POS)
	long pos_koukan_kingaku;		// [NEW] 交換金額
	long pos_keihin_rieki;			// [NEW] 景品利益
	long pos_jitsu_keihin_kingaku;	// [NEW] 実景品金額(POS)
	long pos_jitsu_koukan_kingaku;	// [NEW] 実交換金額
	long pos_azukariken_yomitori_tamasu;	// [NEW] POS預かり券読取玉数
	long pos_chodama_hikidasi_tamasu;	// [NEW] POS貯玉引出数
	long pos_keihin_koukan_tamasu;		// [NEW] POS景品交換玉数
	long pos_chodama_azukeire_tamasu;	// [NEW] POS貯玉預入玉数
	long pos_amari_tamasu;			// [NEW] POS余り玉数(取込玉数)
	long tp_azukaritama_hakken_tamasu;	// [NEW] TP預かり玉発券玉数
} AVGDATA_ZENTEN_SYUKEI_SYUBET_9030;

typedef struct {
	long sand_hakken;		// サンド発券
	long sima_gai_hakken;	// 島外発券
	long card_seisanki;		// カード精算機
	long card_hakken;		// カード発券
	long card_hakken_total;	// カード発券計
} KENBAI_DATA_9030;

typedef struct {
	SUMDATA_ZENTEN_SYUKEI_SYUBET_9030 sumData[MAX_PAS + 3];
	AVGDATA_ZENTEN_SYUKEI_SYUBET_9030 avgData[MAX_PAS + 3];
	KENBAI_DATA_9030 kenbaiData;
} ZENTEN_SYUKEI_SYUBET_9030;

//-- HN2多店舗対応 --//
//---------------------------------------------------------------------
// [概要]
// 機種別定時稼動数(9032) を表現するための構造体
//---------------------------------------------------------------------

typedef struct {
	long kno;					// 機種番号
	char ksh_name[32];			// 機種名
	long sbt;					// 論理種別番号
	char sbt_name[32];			// 種別名
	long daisu;					// 台数
	long kado_data[33*2];		// 稼動データ[偶数:定時, 奇数:ピーク]
} KISHU_TEIJI_KADO_9032;

#define	MISETTEI	0
#define TYPE_XS		1
#define TYPE_S		2
#define TYPE_M		3
#define TYPE_L		4
#define TYPE_XL		5

#define USE_XS		1
#define USE_S		2
#define USE_M		3
#define USE_L		4
#define USE_XL		5

// 特賞履歴 新基準機対応のフラグ
#define FLG_T1_XS	0x01
#define FLG_T1_S	0x02
#define FLG_T1_M	0x03
#define FLG_T1_L	0x04
#define FLG_T1_XL	0x05

//---------------------------------------------------------------------
// [概要]
// 新基準機対応の集計構造体
//---------------------------------------------------------------------
typedef struct {
	unsigned long cnt;
	unsigned long out;
	unsigned long safe;
} KIJUNKI_DATA;

typedef struct {
	KIJUNKI_DATA t1_xs;
	KIJUNKI_DATA t1_s;
	KIJUNKI_DATA t1_m;
	KIJUNKI_DATA t1_l;
	KIJUNKI_DATA t1_xl;
} KIJUNKI_SYUKEI;

#endif
/* WAIS END */

