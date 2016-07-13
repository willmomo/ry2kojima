#if !defined(__WSS_CMNTBL_H)
#define __WSS_CMNTBL_H

#include "asdef.h"


// 共有メモリ
typedef struct {
	long	now_date[4];		// 現在年月日曜		[0] =年　[1] = 月　[2]=日　[3]= 曜
	long	now_time[2];		// 現在時分			[0] =時　[1] = 分
	long	eigyo_date[4];		// 営業年月日曜		[0] =年　[1] = 月　[2]=日　[3]= 曜
	long	eigyo_time[2];		// 営業時分   [0] =時　[1] = 分
	long	as_mode;			// ＡＳ接続有無		1 = ＡＳ通信有り　　0 = ＡＳ通信なし（単体）
	long	settei_mode;		// 設定モード		0 = 運用設定    1= 予め設定モード
	long	sys_setsw;		// システム設定ＳＷ	0 = OFF  1 = ON
	long	now_keysw;		// 今回カギＳＷ状態	0 = OFF 1 = 営業１　2 = 設定　3 = 営業２
	long	old_keysw;		// 前回カギＳＷ状態	0 = OFF 1 = 営業１　2 = 設定　3 = 営業２
	long	now_card;		// 今回カード状態	0 = 抜取り中    1 = 挿入中
	long	old_card;		// 前回カード状態	0 = 抜取り中    1 = 挿入中
	long	card_syu;		// 挿入中カード状態	1 = ｷｬﾘﾌﾞﾚｶｰﾄﾞ  2 = 設定ﾓｰﾄﾞ変更ｶｰﾄﾞ
							//			3 = 会員ﾏｽﾀ読込み・書込みｶｰﾄﾞ4 = ﾃﾞｰﾀ変更ｶｰﾄﾞ
	char	cdata[64];		// ｶｰﾄﾞ読み取りﾃﾞｰﾀ
	long	now_proc;		// 今回画面プロセス	0 = NOP 1 = 運用ﾌﾟﾛｾｽ  2 = 業務設定ﾌﾟﾛｾｽ
							//							3 = ｼｽﾃﾑ設定ﾌﾟﾛｾｽ
	long	old_proc;		// 前回画面プロセス	0 = NOP 1 = 運用ﾌﾟﾛｾｽ  2 = 業務設定ﾌﾟﾛｾｽ
							//							3 = ｼｽﾃﾑ設定ﾌﾟﾛｾｽ
	long	inji_sw	;		// 印字ＳＷ状態		0 = OFF   1 = ON
	long	ups_ijyo;		// ＵＰＳ電源異常	0 = 正常   1 = 異常
	long	touka_recv;		// 透過印字受信継続フラグ	1 = 継続   0 = 打ちきり
	long	wsscom_watch;	// 通信プロセスウォッチドッグ監視カウンタ
	long	tnio_watch;		// ＴＮＩＯプロセスウォッチドッグ監視カウンタ
	long	syukei_watch;	// 集計プロセスウォッチドッグ監視カウンタ
	long	touka_watch;	// 透過印字プロセスウォッチドッグ監視カウンタ
	long	ups_watch;		// ＵＰＳプロセスウォッチドッグ監視カウンタ
	long	unyou_watch;	// 運用画面プロセスウォッチドッグ監視カウンタ
	long	sys_watch;		// システム設定画面プロセスウォッチドッグ監視カウンタ
	long	gyou_watch;		// 業務設定画面プロセスウォッチドッグ監視カウンタ
	unsigned char	dflag;	// 動作ﾌﾗｸﾞ
							//	0x01	 クリア中:  */
                            //  0x02	 記憶中:	*/
                            //  0x04	 自動記憶中:	*/
                            //  0x08	 日次処理中:	*/
                            //  0x10	 ＣＳフラッシュ処理中:  (追加:99/01/18:ohno) */
							//	0x20	 設定クリア中:  */
                            //  0x40	 記憶クリア中:  */
                            //  0x80	 システムクリア中:  */
	unsigned char	dflag2;	// 動作ﾌﾗｸﾞ2
							//	0x01	 Rsダウンロード中:  */
                            //  0x02	 Dnダウンロード中:  */
                            //  0x04	 Tnダウンロード中:  */
                            //  0x08	 Hnダウンロード中:  */
                            //  0x80	 全体一斉ダウンロード中:
	unsigned char	eflag;	// 営業ﾌﾗｸﾞ
							//	0x80	 開店中:	*/
                            //  0x40	 閉店業務中:	*/
                            //  0x00	 閉店中:	*/
	unsigned char	emark;	// 営業ﾏｰｸ
							//	0x80	 クリア済:  */
                            //  0x40	 開店済:	*/
                            //  0x20	 閉店済:	*/
                            //  0x10	 XXX:	 */
                            //  0x08	記憶済:	*/
	unsigned char	ksdflag;// ＫＳ動作ﾌﾗｸﾞ
							//	0x01	 LOGｸﾘｱ中:  */
                            //  0x02	 LOG記憶中:  */
                            //  0x04	 ﾏｽﾀ記憶中:  */
                            //  0x40	 KSSYR動作中:  */
                            //  0x80	 KS動作中:  */
	unsigned char	kpdflag;// 会員ＰＣ動作ﾌﾗｸﾞ
							//	0x01	 TKD取得中:  */
                            //  0x02	 会員ﾏｽﾀ取得中:  */
                            //  0x80	 会員PC動作中:  */
	unsigned char	ks_pt_seiri_fg;	//  0x01	KMポイント整理中			2004.09.30 add hashimoto
									//  0x02	KM書戻中
									//  0x04	キャッシュ件数チェック中
									//  0x08	キャッシュバックアップ中
									//  0x10	キャッシュポイント整理中
									//  0x80	チェック中
	unsigned char	wss_kioku_flag;	//	操作部記憶フラグ	0:OFF, 1:ON(操作部記憶中)
	unsigned char	pad1;			//	2009.09.03
	long	kioku_zumi;		// 0=未記憶　１＝記憶済み
	unsigned short eigyou_min_cnt;		// 営業時間（分）クリア・開店で０
	short	SeigouErr;					// 島サンド設定と機種名設定で整合性エラー中 0=ｴﾗｰなし 1=ｴﾗｰ中
	short	Proc_Watch;		// プロセスウォッチドッグタイムアウト状態
							// 0x0001	通信プロセス	ウォッチドッグタイムアウト中
							// 0x0002	ＴＮＩＯプロセスウォッチドッグタイムアウト中
							// 0x0004	集計プロセス	ウォッチドッグタイムアウト中
							// 0x0008	UPSプロセス		ウォッチドッグタイムアウト中
	short	pad2;
	unsigned long	ks_pt_seiri_plan_ps_tim;	// 前回ポイント整理予定日付	2004.10.29 add fukuyama
	unsigned char	mpcdflag;					// MPC動作フラグ	2006.02.16 add hashimoto(T2X002759)
	unsigned char	pbnkdflag[2];				// PBANK動作フラグ	2006.02.16 add hashimoto(T2X002759)
	unsigned char	ms1kdflag;					// MS-1000動作フラグ	2006.06.05 add hashimoto(T2X002867)
	unsigned char	masdflag;					// ﾏｽﾀｰｻｰﾊﾞ動作フラグ	2006.06.05 add hashimoto(T2X002867)
	unsigned char	dspdflag[2];				// DSP動作フラグ		2006.06.28 add hashimoto(T2X002907)
	unsigned char	lecdflag;					// GWｻｰﾊﾞ動作フラグ	2006.06.05 add hashimoto(T2X003143)
	unsigned long	clear_time[5];				// クリア時刻	[0] =年　[1] = 月　[2]=日　[3]= 時　[4]= 分
	unsigned char	today_clear_zumi;			// 本日クリア済みフラグ（営業日付が変わってから、クリアされたら1、それ以外は0）

	unsigned char	hokyu_setflag;				// 補給機設定手動取得結果 (bit0-7)ｸﾞﾙｰﾌﾟA～H [0]OK [1]NG
	unsigned char	estat[2];					// 営業状態 [0]A店[1]B店
												// (0x01)営業日付変更 (0x02)開店 (0x03)閉店 (0x04)記憶[開店後] (0x05)記憶[開店前] (0xFE)店休日 (0xFF)店舗なし
	unsigned long	hokyuki_set_time[5];		// 補給機設定取得時刻	[0] =年　[1] = 月　[2]=日　[3]= 時　[4]= 分
	unsigned char	hokyu_dflag[8];				// 補給機A～H動作フラグ
												// 0x80: 0=ｵﾌﾗｲﾝ中 1=ｵﾝﾗｲﾝ中
												// 0x40: 0=補給機設定手動取得中でない 1=手動取得中
												// 0x20: 0=補給機設定自動取得中でない 1=自動取得中
	unsigned long	as_cpu_time;				// ASCPU時間(AS起動からの通算秒)
	unsigned char	dflag3;						// 動作フラグ３
	unsigned char	zc_dflag;					// 自動クリア動作フラグ
	unsigned char	zc_set_tim[2];				// 自動クリア設定時間(本日)　zc_set_tim[1]:時,zc_set_tim[2]:分

	unsigned char	gtage_dflg;					// GTage動作フラグ: (bit0)CSV出力PC動作フラグ=0/1:オフライン/動作中
	unsigned char	as_dflag[2];				// AS動作フラグ: (bit0)AS#2接続状態 0/1 = 動作中(未接続)/オフライン
												//               (bit1)AS#3接続状態 0/1 = 動作中(未接続)/オフライン

	unsigned char	choflag;					// 貯玉処理フラグ(bit0=1)預入不可
	unsigned char	dmy[3];						// 

	//pasxは一番最後のいちでないとＶＢが都合が悪いため @koketsu
	//char	pasx[4][2];		// 種別文字		[0]=種別Pの文字 [1]=種別Aの文字		// 未使用のためマスク		** HC2007 定数変更 **
	char	pasx_vb[SHUBETSU];		//				[2]=種別Sの文字 [3]=種別Xの文字									** HC2007 定数変更 **
	unsigned char	autoprn_cyu;				// 1:主操作自動印字中(自動印字機能があるのは主操作のみ)

} wss_cmntbl_t;
#endif






