/*
 *	設定
 *		プライベートヘッダ
 *
 */



/*
 *	設定ファイル(2)
 *
 *
 */
#if defined(_WIN32)
#include <pshpack1.h>
#endif

#if 0
/* tn_set.h で定義済み 2002.03.04 koketsu */
typedef	struct	{
	short	repeat;			/* リピート回数（負の値でランプ点燈無し）*/
	unsigned char	dat[16];		/* 2秒分のデータ */
} lamp_t;
#endif


typedef struct {
	char	company_id[4];
	char	hall_id[2];
	BYTE	reserve[2];
} card_id_t;


/* tn_set.h で定義済み 2002.03.04 koketsu */
#if 0
typedef struct {
	DWORD top;
	DWORD end;
} card_range_t;
#endif	/* tn_set.h で定義済み 2002.03.04 koketsu */

typedef struct {
	DWORD company;
	DWORD hall;
} card_company_hall_t;

typedef struct {
	BYTE	ticket_shopname[40];		/* 0:   asciz */
	BYTE	ticket_message[40];			/* 40:  asciz */
	BYTE	robocard_use;				/* 80:	0/1  ロボカード使わない/使う */
	BYTE	robocard_open_use;			/* 81:	0/1  オープンカード使わない/使う */
	BYTE	sankyocard_use;				/* 82:	0/1  三共カード使わない/使う */
	BYTE	halcard_use;				/* 83:	0/1  ハルカード使わない/使う */
	BYTE	pbankcard_use;
	BYTE	reserve_1_4[1];
	WORD	printer_existence;			/* HNでプリンタがあるかどうか */
	BYTE	fl_rest_disp;				/* FLで0/1=「離席中」/「休憩中」 */
	BYTE	reserve_1_5[7];
	BYTE	coinsel_payout_100;			/* 96:	 100円当たり払い出し数 */
	BYTE	coinsel_test_enable;		/* 97:	 0/1 コインセレクタテストng/ok */
	BYTE	reserve2[2];				/* 99: */
	BYTE	remocon_code[40];			/* 100:  リモコンコード */
	DWORD	ticket_points[8];			/* 140:  チケットのポイント */
	lamp_t	lamp_data[33];				/* 148:  ランプ点灯パターン */
	BYTE	point_mode;					/* 0/1/2 ポイントなし/あり1/あり2 */
	BYTE	point_caption_mode;
	BYTE	reserve3[2];
	BYTE	replay_device_p;
	BYTE	replay_device_as;
	BYTE	pm842_timer;				/* pm842に設定するタイマー値(0~9) */
	BYTE	reserve4[3];
	DWORD	fl_payback_max;				/* 752:  クレジット玉払い出し上限 */
	DWORD	fl_deposit_max;				/* 756:  貯玉払い出し上限 */
	BYTE	cs_use;						/* 760:  0/1 CS使わない/使う */
	BYTE	reserve5[3];				/* 761:*/
	BYTE	tyodama_use;				/* 764:  貯玉 利用フラグ */
	BYTE	tyomedal_use;				/* 765:  貯メダル 利用フラグ */
	BYTE	replay_confirm;				/* リプレイ 状態確認 */
	BYTE	reserve6[1];
	BYTE	ball_100;
	BYTE	medal_100;
	BYTE	reserve12[2];
	WORD	replay_ball_yen;
	WORD	replay_medal_yen;
	BYTE	order_mode;					/* 0/1/2 オーダーしない/するx2 */
	BYTE	reserve7[3];
	BYTE	message_confirm[6];			/* メッセージ表示時の確認 0/1 しない/する */
	BYTE	reserve_7_0[10];
#if 0
	BYTE	carddata[4*2*8];			/* カード会社/ホール ID */
#else
	card_company_hall_t card_company_hall[8];
#endif
	BYTE	reserve_7_5[12*8+4];
	BYTE	call_mode;					/* 0/1/2 呼び出し0/1/2 なし/ボタン/画面外*/
	BYTE	call_cancel_corner_enable;	/* 0/1   画面隅呼出cancel 0/1 なし/あり */
	BYTE	reserve8[2];				/* 986:*/
	DWORD	replay_cs_shortage_time;	/* 988:  コインセレクタ玉不足検出時間sec */
	BYTE	lamp_remocon_feedback[16];	/* 992:  リモコン受付ランプ */
	BYTE	point_cardin_retry;			/* ポイントカードイン電文リトライ回数 */
	BYTE	point_cardout_retry;		/* ポイントカードアウト電文リトライ回数 */
	BYTE	point_retry;				/* ポイント電文リトライ回数 */
	BYTE	point_timeout;				/* ポイント電文タイムアウト */
	BYTE	tyodama_retry;				/* 貯玉電文リトライ回数 */
	BYTE	tyodama_timeout;			/* 貯玉電文タイムアウト */
	BYTE	unreliable_server_point;	/* サーバーを信頼しない ポイント */
	BYTE	unreliable_server_tyodama;	/* サーバーを信頼しない 貯だま */
	BYTE	reserve9[8];
	card_id_t card_id[32];				/* カードID 32個 */
	char	set2_string_1[14];			/* 設定2 文字列1 */
	char	set2_string_2[14];			/* 設定2 文字列2 */
	BYTE	reserve10[4];
	BYTE	ansyo_retry;				/* 暗証番号リトライ回数 */
	BYTE	disp_mode;				/* 貯玉数メイン表示切替 (0:度数表示　1:玉数表示) */
	BYTE	reserve11[2];
	DWORD	replay_fee_100_coin[2];		/* リプレイ手数料(100円当たり) */
	DWORD	replay_fee_100_medal[2];
	DWORD	replay_fee_coin_limit;		/* リプレイ手数料切り替え値 */
	DWORD	replay_fee_medal_limit;
	DWORD	replay_ball_limit[2];		/* リプレイ玉数上限 0/1=p/p以外 0で無効 */
	DWORD	replay_payout_timeout;
	BYTE	reserve13[8];
	card_range_t tyodama_disable_card[16];	/* 貯玉不可カード */
	BYTE	intercalary_shift_type;		/* 閏日(2/29)シフト方法  0=2/28,1=3/1 */
	BYTE	birthday_range_before;		/* 誕生日範囲前(日) */
	BYTE	birthday_range_after;		/* 誕生日範囲後(日) */
	BYTE	ansyo_change;				/* 0/1 暗証番号変更不可/許可 */
	BYTE	reserve14[4];
	BYTE	appli_data_tn[1080];		/* アプリケーションデータ */
	DWORD	replay_fee_1_coin[2];		/* リプレイ手数料(1回払い出し値段あたり) */
	DWORD	replay_fee_1_medal[2];
	BYTE	hn_setting[16][10];			/* HN端末毎設定 */
	BYTE	disppoint_when_cardin;		/* 照会 カードイン時 */
	BYTE	auth_when_disppoint;		/* 認証 照会時 */
	BYTE	hn_setting_available;		/* HN端末設定有効 */
	BYTE	reserve16[109];
	BYTE	slump_bold;					/* 0/1 スランプ太くない/太い */
	BYTE	birthday_message_time;		/* 誕生日メッセージ表示時間(sec) */
	BYTE	return_service;				/* 0/1 サービス画面に戻らない/戻る */
	BYTE	slot_disp_mode;				/* 0/1 スロット表示しない/する */
	BYTE	slot_disp_nowty_ct;			/* 0/1 CT中現在獲得枚数表示しない/する */
	BYTE	slot_disp_nowty_tokusyo;	/* 0/1 大当り中現在獲得枚数表示しない/する*/
	BYTE	slot_disp_hn_only;			/* 0/1 HN固有表示する/しない */
	BYTE	reserve_15[1];
	card_range_t disable_card[16];		/* 使用不可カード */
	BYTE	pass_set_when_cardin;		/* 0/1 cardin時暗証設定しない/する */
	BYTE	birth_set_when_cardin;		/* 0/1 cardin時誕生日電話設定しない/する */
	BYTE	mail_use;					/* 0/1 メール使用しないする */
	BYTE	medal_pulse_num;			/* メダル払い出し時のパルス数 */
	BYTE	ss_type;					/* スクリーンセーバーのタイプ */
	BYTE	order_type;					/* オーダ方式 0:旧(max 16)  1:新(max 64) */
	BYTE	reserve_16[2];
	BYTE	appli_data_hn[1080];		/* アプリケーションデータ */
	BYTE	mail_real_use;				/* 0/1 メールリアルメッセージ使わない/う */
	BYTE	pass_set_when_ticket;		/* 0/1 チケット発行時暗証不要/必要 */
	BYTE	uri_type[2];				/* idx=0/1 P/P以外  0/1 売上/打込玉 */
	BYTE	replay_ball_per_pulse;		/* 0/1 １パルス １玉／１００円（２５玉） */
	BYTE	reserve_end[3];
} setting_3_t;


#if defined(_WIN32)
#include <poppack.h>
#endif

//extern setting_t2 setting_n;			/* 設定データ(コピー) */
//extern setting_3_t setting3;			/* 設定データ(2) */
