
#if !defined(__BV_STRUCT_H)
#define __VB_STRUCT_H

// fno1000: システム設定
typedef struct {
	long vn;				// バージョン
	long model_code;		// モデルコード
	long tn_line;		// TN接続
	long replay_line;	// 台リプレイ機接続
	long keisuu_line;	// 台計数機接続
	long pointsys;		// ポイントシステム接続
	long kaisvr;			// 会員サーバ接続
	long kaipc;			// 会員PC接続
	long cashsvr;		// キャッシュサーバ接続
	long jarnal;			// ジャーナル接続
	long hnsuu;			// ホールナビ接続
	long kugicho;		// 釘調有無
	long chodama;		// 貯玉有無
	long futureland;		// FL有無
	long rs;				// 増設RS枚数
	u_char yobi2[34];		// 予備
	long mode;			// 台番モード: 0=49抜き 1=連番
	long dai;			// 台数
	long tn;				// TN数(未使用)
	long sima;			// 島数(未使用)
	long floor;			// フロア数(未使用)
	long kinko;			// 金庫数(未使用)
	long g_genkin;		// 島外現金販売機数(未使用)
	long g_card;			// 島外カード販売機数(未使用)
	long g_keihin;		// 島外景品機数(未使用)
	long g_replay;		// 島外リプレイ機数(未使用)
	long g_premium;		// 島外プレミアム機数(未使用)
	long g_cardh;		// 島外カード発行機(未使用)
	long pos;			// POS数(未使用)
	long jihan;			// 自販機数(未使用)
	long ta;				// その他(未使用)
	long pnl_tab_mask1;	// パネルタブマスク(ONの時タブを非表示にする)
							// bit0:全部(という名前のタブ) bit1:遊戯 bit2:非遊戯
							// bit3:大当り bit4:異常 bit5:会員
	long option1;		// オプション1(予備)
	long option2;		// オプション2: ダウンロード設定
							// 0:通常(Lv1->Lv2)
							// 1:通常+立上りダウンロードON
							// 2:Lv1->Lv2->Lv0
							// 3:Lv0 only(IPL1)
							// 4:Lv1 only(IPL2)
							// 5:Lv2 only(Appli)
	long option3;		// オプション3: 機器異常検地マスク詳細(ON時マスク)
							// bit0:リプレイ異常
							// bit1:リプレイ通信異常
							// bit2:Tn I/O通信異常
							// bit3:Tn 通信異常
							// bit4:Hn 通信異常
							// bit5:D2 通信異常
							// bit6:Dn 通信異常
	long option4;		// オプション4(予備)
	long option5;		// オプション5(予備)
	long option6;		// オプション6(予備)
	long option7;		// オプション7(予備)
	long option8;		// オプション8(予備)
	u_char yobi[16];		// 予備
} vb_fno1000_t;

typedef struct {
	int hoge;
} vb_fno1001_t;

#endif
