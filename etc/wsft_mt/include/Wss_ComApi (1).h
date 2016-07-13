/* WAIS START */
//------------------------------------------------------------
//概要
//   Ｗｉｎ操作共通ルーチン
//
//作成者
//   Tokai水谷
//
//備考
//	 Ｗｉｎ操作で使用する共通な関数を定義する
//------------------------------------------------------------
#if !defined(_Wss_ComApi__H)
#define _Wss_ComApi__H

#include "wss_str.h"
#include "set_gyou.h"

//------------------------------------------------------------
//	apl_sub.c
//------------------------------------------------------------
extern	ulong apl_getpw_tm(void );
extern	void apl_kaihei(int day,ulong *topen,ulong *tclose);
extern	int	 apl_getegyo(int day);
extern	int	 apl_gettden(int day);
extern	int	 apl_get_kblk(int day);

//------------------------------------------------------------
//	csb1_sub.c
//------------------------------------------------------------

int	csb1_idx_make(void);		/* INDEX 更新	*/

extern	int csb1_simaidx(void );
extern	int csb1_simaidx_qsort(int spos,int epos);
extern	int csb1_kishuidx(void );
extern	int csb1_kishuidx_qsort(int spos,int epos);
extern	int csb1_kinkoidx(void );
extern	int csb1_kiokufno(int kday,int dfno);
extern	int csb1_ftnopos(int dfno);
extern	int csb1_kiokusearch(uint dr,int kday,int dfno);
extern	int csb1_daichk(int dno);
extern	int csb1_daifloor(int dno);
extern	int csb1_daikinko(int dno);
extern	int csb1_dai_sima_no(int dno);
extern	int csb1_dai_kishu_no(int dno);
extern	int csb1_dai_ksname(int dno,char *ksname);
extern	int csb1_daisearch(uint dr,int dno);
extern	int csb1_sbtchk(int pas);
extern	int csb1_sbtsearch(uint dr,int sbt);
extern	int csb1_simachk(int smno);
extern	int csb1_simasearch(uint dr,int smno);
extern	int csb1_sima_dai(int smno,int pos);
extern	int csb1_sima_daisearch(uint dr,int smno,int dno);
extern	int csb1_kishuchk(int ksno);
extern	int csb1_kishusearch(uint dr,int ksno);
extern	int csb1_kishu_dai(int ksno,int pos);
extern	int csb1_kishu_daisearch(uint dr,int ksno,int dno);
extern	int	csb1_gyo1109_settei(FSET_GYOU_HURIWAKE* ans);	/* 新基準機対応		2005.07.29	nao */
extern	int	csb1_kinko_sima_no(int kinko, int pos);	/* 2000.11.24 mizu 金庫集計実装の為追加 */
extern	int	csb1_kinkosearch(uint dr, int kinko);	/* 2000.11.24 mizu 金庫集計実装の為追加 */

//------------------------------------------------------------
//	csb2_sub.c
//------------------------------------------------------------
extern	int csb2_getsyszen(int kind,int no1,int no2);
extern	int csb2_zensearch(uint dr,int kind,int chno);
extern	int csb2_zenchk(int kind,int chno);
extern	int csb2_hans_search(uint dr,int kno);
extern	int csb2_keis_search(uint dr,int kno);
extern	int csb2_hans_zens(int hans);
extern	int csb2_keis_zens(int keis);
extern	unsigned short rZentoRs(unsigned short no);
extern	int csb2_ureki_search(uint dr,int dno);
extern	int csb2_hrekipos(int no);
#ifdef L_HAS_HED
extern	int csb2_get_hcodetbl(uchar code1,uchar code2,HAS_CODE_TBL **tbl);
extern	int csb2_hsnochg(ushort chno,uchar *jflg,ushort tflg,HAS_CODE_TBL *tbl,int *no1,int *no2,int *pas);
#endif

//------------------------------------------------------------
//	err_sub.c
//------------------------------------------------------------
extern	void err_system(int ecode,short sinf,long linf);

//------------------------------------------------------------
//	cal_sub.cpp
//------------------------------------------------------------
extern long cal_div(long ch, long mt);
extern long cal_div45(double ch, double mt);
extern long cal_start_ave(short dscl, short skeisu, long start, long out);
extern long cal_start_pro(long start, long out);
extern long cal_kingaku_100yen(short dscl, short urikos, short urikin, long kos);
extern long cal_kingaku45_100yen(short dscl, short urikos, short urikin, long kos);
extern long cal_kingaku45_1yen(short dscl, short urikos, short urikin, long kos);
extern long cal_kingaku_1yen(short dscl, short urikos, short urikin, long kos);
extern long cal_kosuu(short dscl, short urikos, short urikin, long uri);
extern long cal_genkakingaku(long lkin, long lgenka);
extern long cal_genkakosuu(long lkos, long lgenka);
extern long cal_genka3kingaku(long lkin, long lgenka3);
extern long cal_genka3kosuu(long lkos, long lgenka3);
extern long cal_daikeikin(long uri, short warisu, long sa);
extern long cal_keisuu(short dscl, short urikos, short urikin, long uri, long sa);
extern long cal_ewari(short dscl, short urikos, short urikin, short warisu, long uri, long sa);
extern long cal_gen_ewari(short dscl, short urikos, short urikin, short warisu, long lgenka, long uri, long	sa);
extern long cal_gen3_ewari(short dscl, short urikos, short urikin, short warisu, long lgenka3, long uri, long	sa);
extern ulong cal_tama(ulong uri, ulong out);
extern long cal_hosei(long sg_premkin, long sg_repkin, long ukomi, long sbt_ukomi, long k_repkin);
extern long cal_hoseikin(short tanka, long hkin, long prem, long cho, long replay, long replaytesu, long uri);
extern long cal_sk_hoseikin(short tanka, long hkin, long prem, long cho, long replay, long replaytesu, long uri);
extern long cal_pas_hoseikin(short tanka, long prem, long cho, long replay, long replaytesu, long uri);

//------------------------------------------------------------
//	cal_sub2.cpp
//------------------------------------------------------------
#if 0
extern double cal_dblKingaku100Yen(long scl, long urikos, long urikin, double dblKos);
#endif
extern double cal_dblKingaku1Yen(long scl, long urikos, long urikin, double dblKos);
extern double cal_dblKosuu(long scl, long urikos, long urikin, long uri);
extern double cal_dblGenkaKingaku(double dblKin, long genka);
extern double cal_dblGenkaKosuu(double dblKos, long genka);
extern double cal_dblDaiKeikin(double dblUri, long warisu, double dblSa);
extern double cal_dblKeisuu(long dscl, long urikos, long urikin, long uri, long sa);
extern double cal_dblEWari(long scl, long urikos, long urikin, long warisu, double uri, long sa);
extern double cal_dblGenEWari(long scl, long urikos, long urikin, long warisu, long genka, double uri, long sa);

extern double fl_dblArarieki(double duri, double dkei, long genka);
extern double cal_dblKyakuTaiPar(long scl, long urikos, long urikin, double uri, long bsa);


//------------------------------------------------------------
//	RIREK_S.C
//------------------------------------------------------------
#ifdef L_REK_FILE
void	hinf_init(void );

int	has_sysijo_clr	(void);
int	has_sysijo_set	(INF_HASSEI	*inf);
int	has_sysijo_get	(REKI_SYSIJO	*gsys, ulong *hno);

int	has_inji_clr	();
int	has_inji_set	(INF_HASSEI	*inf);
int	has_inji_get	(REKI_INJI	*ginj, ulong *hno);
int	has_inji_pget	(ulong		*hno);

int	has_onsei_clr	();
int	has_onsei_set	(INF_HASSEI	*inf);
int	has_onsei_get	(REKI_ONSEI	*gons, ulong *hno);
int	has_onsei_pget	(ulong		*hno);

int	has_camera_clr	();
int	has_camera_set	(INF_HASSEI	*inf);
int	has_camera_get	(REKI_CAMERA	*gcam, ulong *hno);
int	has_camera_pget	(ulong		*hno);
#endif

extern BOOL	GetTokusyoHuriwake( FSET_GYOU_HURIWAKE	*Kio_F1109 );

#endif
/* WAIS END */
