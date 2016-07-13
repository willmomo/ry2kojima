//---------------------------------------------------------------------
// ASで使用している構造体(のコピー)
// VBで使用するためのvb_utype.hとそれを、Cレベルで使用するための
// vb_struct.hがあります。

#if !defined(__AS_STRUCT_H)
#define __AS_STRUCT_H

// fno1000: システム設定
typedef struct {
	u_short vn;				// バージョン
	u_short model_code;		// モデルコード
	u_short tn_line;		// TN接続
	u_short replay_line;	// 台リプレイ機接続
	u_short keisuu_line;	// 台計数機接続
	u_short pointsys;		// ポイントシステム接続
	u_short kaisvr;			// 会員サーバ接続
	u_short kaipc;			// 会員PC接続
	u_short cashsvr;		// キャッシュサーバ接続
	u_short jarnal;			// ジャーナル接続
	u_short hnsuu;			// ホールナビ接続
	u_short kugicho;		// 釘調有無
	u_short chodama;		// 貯玉有無
	u_short futureland;		// FL有無
	u_short rs;				// 増設RS枚数
	u_char yobi2[34];		// 予備
	u_short mode;			// 台番モード: 0=49抜き 1=連番
	u_short dai;			// 台数
	u_short tn;				// TN数(未使用)
	u_short sima;			// 島数(未使用)
	u_short floor;			// フロア数(未使用)
	u_short kinko;			// 金庫数(未使用)
	u_short g_genkin;		// 島外現金販売機数(未使用)
	u_short g_card;			// 島外カード販売機数(未使用)
	u_short g_keihin;		// 島外景品機数(未使用)
	u_short g_replay;		// 島外リプレイ機数(未使用)
	u_short g_premium;		// 島外プレミアム機数(未使用)
	u_short g_cardh;		// 島外カード発行機(未使用)
	u_short pos;			// POS数(未使用)
	u_short jihan;			// 自販機数(未使用)
	u_short ta;				// その他(未使用)
	u_short pnl_tab_mask1;	// パネルタブマスク(ONの時タブを非表示にする)
							// bit0:全部(という名前のタブ) bit1:遊戯 bit2:非遊戯
							// bit3:大当り bit4:異常 bit5:会員
	u_short option1;		// オプション1(予備)
	u_short option2;		// オプション2: ダウンロード設定
							// 0:通常(Lv1->Lv2)
							// 1:通常+立上りダウンロードON
							// 2:Lv1->Lv2->Lv0
							// 3:Lv0 only(IPL1)
							// 4:Lv1 only(IPL2)
							// 5:Lv2 only(Appli)
	u_short option3;		// オプション3: 機器異常検地マスク詳細(ON時マスク)
							// bit0:リプレイ異常
							// bit1:リプレイ通信異常
							// bit2:Tn I/O通信異常
							// bit3:Tn 通信異常
							// bit4:Hn 通信異常
							// bit5:D2 通信異常
							// bit6:Dn 通信異常
	u_short option4;		// オプション4(予備)
	u_short option5;		// オプション5(予備)
	u_short option6;		// オプション6(予備)
	u_short option7;		// オプション7(予備)
	u_short option8;		// オプション8(予備)
	u_char yobi[16];		// 予備
} fno1000_t;

typedef struct {
	int hoge;
} fno1001_t;

#endif
