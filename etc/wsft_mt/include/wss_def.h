/* WAIS START */
//----------------------------------------------
//概要
//	ＷＩｎ操作共通各種定義
//
//作成者
//	Tokai 纐纈
//
//備考
//	Ｗｉｎ操作各種定義を宣言する
//----------------------------------------------

#if !defined(_WSS_DEF__H)
#define _WSS_DEF__H

//-------------------------------------------------
//	ウィンドウタイトル名(FindWindow()で指定するなまえ）
//-------------------------------------------------
#define	WTL_APCTRL		"無題 - WssAppCtrl"				// アプリケーション管理プロセス
#define	WTL_COM			"無題 - WssCom"					// 通信管理プロセス
#define	WTL_SYUKEI		"Syukei"						// 集計プロセス
#define	WTL_SYSSETTEI	"システム設定メニュー"			// システム設定プロセス
#define	WTL_GYOSETTEI	"業務設定トップメニュー"		// 業務設定プロセス
#define	WTL_UNYOU		"Unyou"							// 運用画面プロセス
#define WTL_COMPROC		"ComProc"						// ComProcプロセス	(MsgView.iniの[System]ComProcTitleでこの通り設定して下さい)
#define WTL_WINKT		"Win会員端末"					// WinKTのタイトル
#define	WTL_TNIO		"無題 - Tnio"					// ＴＮＩＯプロセス
#define	WTL_UPS			"無題 - Ups"					// ＵＰＳプロセス
#define WTL_TOUINJ		"無題 - Touinji"				// 透過印字プロセス
#define WTL_SWOFF		"カギSWOFF画面"					// カギＳＷＯＦＦ画面プロセス
#define WTL_NOP			"空白画面"						// 空白画面プロセス
#define	WTL_START		"立上り画面"					// 立ち上がり画面プロセス
#define	WTL_INFO		"インフォメーション画面"		// インフォメーション画面プロセス
#define	WTL_DEMIREAL	"demireal"						// demirealプロセス
#define WTL_MPCPROC		"mpcproc"						// mpcprocプロセス
#define WTL_MAILPROC	"メールお知らせ"				// mailprocプロセス	
#define WTL_GFTP		"GFTP"							// GFTPプロセス	
#define WTL_FANPROC		"fanproc"						// fanprocプロセス	
// ---> 2006/02/10 Y.Okajima
#define	WTL_DLG			"WssDialog Notification Window"	// ダイアログプロセスウィンドウタイトル
// <--- 2006/02/10 Y.Okajima
#define	WTL_SIMAKANRI	"SimaKanri"						// 島管理プロセスウィンドウタイトル
#define	WTL_HALLMAP		"HallMap"						// ホールマップレイアウタープロセスウィンドウタイトル
#define WTL_TCPCOMM		"SquareTcpComm Main Window"		// SquareTcpCommプロセスウィンドウタイトル
#define	WTL_UDPCOMM		"SquareUdpComm Main Window"		// SquareUdpCommプロセスウィンドウタイトル
#define WTL_WSSHUT		"Win操作 シャッター"

//------------------------------------------------------------
//	画面プロセスNO（画面切り替えで指定するＮＯ）
//------------------------------------------------------------
#define GAMNO_UNYOU				1		// 運用画面ＮＯ
#define GAMNO_GYOUSET			2		// 業務設定画面ＮＯ
#define GAMNO_SYSSET			3		// システム設定画面ＮＯ
#define GAMNO_SWOFF				4		// 電源ＯＦＦ画面ＮＯ
#define GAMNO_NOP				5		// 空白画面ＮＯ
#define GAMNO_START				6		// 立ち上がり画面ＮＯ
#define GAMNO_CARIV				7		// キャリブレーション画面ＮＯ
#define GAMNO_INFO				8		// インフォーメーション画面ＮＯ
#define GAMNO_WINKT				9		// Win会員端末ＮＯ
#define GAMNO_Mail			   10		// メールお知らせダイアログＮＯ
#define GAMNO_SIMAKANRI		   11		// 島管理端末ダイアログＮＯ

//------------------------------------------------------------
//	プロセスウォッチドッグタイムウトエラー中フラグ
//------------------------------------------------------------
#define	WATCH_COM			0x0001		// 通信プロセス
#define	WATCH_TNIO			0x0002		// ＴＮＩＯプロセス
#define	WATCH_SYUKEI		0x0004		// 集計プロセス
#define	WATCH_UPS			0x0008		// ＵＰＳプロセス

//-------------------------------------------------
//	メッセージイベント
//-------------------------------------------------

// TNIOプロセス－－＞TNIO　RS－232C制御スレッドメッセージイベント
#define	WM_RS232CEND	WM_USER + 1000		// RS－232C制御スレッド終了

// アプリケーション管理－－＞画面プロセスメッセージイベント
#define	WM_APC_CHANGE	WM_USER + 1001		// 画面切り替え
#define	WM_APC_DIALOG	WM_USER + 1002		// ダイアログ表示（モーダル指定）

// アプリケーション管理－－＞プリンタ紙送り要求メッセージイベント
#define	WM_APC_PRFEED	WM_USER + 1003		// プリンタ紙送り
// アプリケーション管理－－＞通信プロセスメッセージイベント
#define	WM_APC_ONSEISTOP	WM_USER + 1004		// 音声ストップ要求


// アプリケーション管理－－＞全プロセスメッセージイベント
#define	WM_APC_SYURYOU	WM_USER + 1009		// 終了

//  通信プロセス－－＞集計処理メッセージイベント
#define WM_COM_START	WM_USER + 1010		// 集計スタート
#define	WM_COM_KIOKU	WM_USER + 1011		// 記憶
#define	WM_COM_RESET	WM_USER + 1012		// 設定更新
#define	WM_COM_CLEAR	WM_USER + 1013		// クリア
//  通信プロセス－－＞画面プロセスメッセージイベント
#define	WM_COM_DIALOG	WM_USER + 1014		// ダイアログ表示（モーダル指定）
//  通信プロセス－－＞印字要求メッセージイベント
#define	WM_COM_PRINT	WM_USER + 1015		// 印字要求
//  通信プロセス－－＞アプリケーション管理プロセスメッセージイベント
#define	WM_COM_NOTKIOKU	WM_USER + 1016		// 未記憶通知
// WssCom → WssAppCtrl にMPCのPOSデータ取込忘れ警告のON/OFFメッセージ
// wParamは常に0
// lParam == 0 なら OFF lParam == 1 ならON
#define WM_COM_MPCPOSWARN	(WM_USER + 1017)
#define WM_COM_TOKUREKI	WM_USER + 1018		// 特賞履歴ファイル取得		T2X002549	2005.07.20	nao
#define WM_COM_MMP_UPDATE	(WM_USER + 1019)	// メモリマップドファイル即時更新

//  集計処理－－＞アプリケーション管理プロセス & 通信プロセス
#define	WM_SYU_KIOKUEND		WM_USER + 1020		// 記憶完了
//  集計処理－－＞通信プロセス
#define	WM_SYU_SYUKEIEND	WM_USER + 1021		// 集計完了
#define	WM_SYU_CLEAREND		WM_USER + 1022		// クリア完了

// ＴＮＩＯ－－＞アプリケーション管理
#define	WM_TNIO_KAGI	WM_USER + 1030		// カギ変化通知
// ＴＮＩＯ－－＞画面プロセス，アプリケーションプロセス
#define	WM_TNIO_CARD	WM_USER + 1031		// カードＩＮ／ＯＵＴ通知

// ＵＰＳ－－＞アプリケーション管理
#define WM_UPS_TEIDEN_ON	WM_USER + 1040		// 停電発生
#define WM_UPS_TEIDEN_OFF	WM_USER + 1041		// 停電回復
#define WM_UPS_SYSDOWN		WM_USER + 1042		// システムシャットダウン命令
#define WM_UPS_BATERY_OLD	WM_USER + 1043		// バッテリー老朽化通知

// システム設定画面－－＞通信プロセス
#define WM_SYS_SET		WM_USER + 1050		 	// 設定更新通知
#define WM_SYS_OPE		WM_USER + 1055			// 運用操作通知（設定クリア、記憶クリアetc)	2002.06.21 修正 by Koketu

// 業務設定画面－－＞通信プロセス
#define WM_GYOU_SET		WM_USER + 1060			// 設定更新通知
// 業務設定画面－－＞アプリケーション管理
#define WM_GYOU_START	WM_USER + 1061			// 起動完了通知
#define WM_GYOU_GAMEN	WM_USER + 1062			// 画面切り替え要求
#define WM_GYOU_SEIGOU	WM_USER + 1063		 	// 整合性エラー発生復旧通知
#define WM_GYOU_ERR		WM_USER + 1064		 	// その他エラー発生通知

// システム設定画面－－＞アプリケーション管理
#define WM_SYS_GAMEN		WM_USER + 1070		 // 画面切り替え要求
#define WM_SYS_START		WM_USER + 1071		 // 起動完了通知
#define WM_SYS_SEIGOU		WM_USER + 1072	 	// 整合性エラー発生復旧通知
#define WM_SYS_ERR			WM_USER + 1073	 	// その他エラー発生通知
#define WM_SYS_ARAEND		WM_USER + 1074	 	// 予め設定終了通知

// 運用画面－－＞アプリケーション管理
#define WM_UNYOU_START		WM_USER + 1080		 // 起動完了通知
#define WM_WINKT_START		WM_USER + 1085		 // 起動完了通知(WinKT)
#define WM_SIMAKANRI_START	WM_USER + 1086		 // 起動完了通知(島管理端末)

// カギＳＷＯＦＦ画面－－＞アプリケーション管理
#define WM_SWOFF_YES		WM_USER + 1090		 // 「はい」「いいえ」で「はい」が選択された通知
#define WM_SWOFF_ERR		WM_USER + 1091		 //  エラー発生通知

// キーボードフック通知DLL
#define WM_KEYHOOK_KEY	WM_USER + 1100

// 透過印字 -->メッセージ ハンドラ印字要求
#define WM_TOU_TOUINJI WM_USER + 1200
// 立ち上がり画面－－＞アプリケーション管理
#define WM_TATI_ERR		WM_USER + 1300			 //  エラー発生通知
// 空白画面－－＞アプリケーション管理
#define WM_NOP_ERR		WM_USER + 1310			 //  エラー発生通知
// インフォメーション画面－－＞アプリケーション管理
#define WM_INFO_ERR		WM_USER + 1320			 //  エラー発生通知

#define WM_USER_TEIJIRELOAD	(WM_USER+1321)	// 定時集計のディスクから再読込み

#define WM_USER_PRE_PT_SEIRI (WM_USER + 1322)  //ポイント整理準備中	add 2004.11.01 hashimoto
#define WM_ENSHUTSU_KOCHO_PREVIEW		 (WM_USER + 1330)	//	好調島・機種プレビューイベント起動
#define WM_ENSHUTSU_KOCHO_ENDPREVIEW	 (WM_USER + 1331)	//	好調島・機種プレビュー停止イベント起動
#define WM_ENSHUTSU_MANUAL_DOEVENT		 (WM_USER + 1332)	//	手動イベント起動
#define WM_ENSHUTSU_AUTO_DOEVENT	  	 (WM_USER + 1333)	//	自動イベント起動
#define WM_ENSHUTSU_COMMON_KILL			 (WM_USER + 1334)	//	全停止
#define WM_GYOU_LAMP_PREVIEW             (WM_USER + 1335)	//  ホール演出(台ランププレビュー)
#define WM_ENSHUTSU_MONITOR_CLEARLOG	 (WM_USER + 1336)	//	好調島・機種モニタのログ

#define WSSM_GETINJISW	(WM_APP + 0)		 // 自動印字SWの状態取得		//yumi
#define WSSM_SETINJISW	(WM_APP + 1)		 // 自動印字SWの状態設定		//yumi
#define WM_APP_SWKHOOK	(WM_APP + 3)		//yumi
#define WSSM_GETREMOTESTATE	(WM_APP + 4)	// リモート端末の通信状態取得	2003.9.29 nao
#define WSSM_SETREMOTESTATE	(WM_APP + 5)	// リモート端末の通信状態設定	2003.9.29 nao
#define WM_KMWATCH_KEYBOARD	(WM_APP + 6)	// キーボード操作通知	2003.10.15 nao
#define WM_KMWATCH_MOUSE	(WM_APP + 7)	// マウス操作通知		2003.10.15 nao
#define WSSM_AUT_REMOTECHG	(WM_APP + 8)	// 自動切断／自動接続		2003.10.20 nao
#define WM_LEFTBTN_DOWN		(WM_APP + 9)	// マウス左ボタンクリック 2004.03.22 nakatani
#define WM_SHUTDOWN			(WM_APP + 10)	// シャットダウン or 再起動通知 2004.03.22 nakatani
#define WM_MAILLIGHTON		(WM_APP + 11)	// パトライト点灯通知 2005.07.31 Fukuyama

#define		WM_USER_RECV_931A			1100
#define		WM_USER_SEND_931A_ACK		1101
#define		WM_USER_SEND_9315			1102
#define		WM_USER_RECV_9315_ACK		1103
#define		WM_USER_NOTICE_LIGTH_OFF	1104

// ---> 2006/02/10 Y.Okajima P-BANK通信エラー通知
#define	WM_PBANK_COMM_ERROR			(WM_USER + 1400)	// P-BANK通信エラー通知
#define	WM_PBANK_GETDATA_ERROR		(WM_USER + 1401)	// P-BANK確定データＣＲＣエラー通知
#define	WM_PBANK_GETPASTDATA_ERROR	(WM_USER + 1402)	// P-BANK過去データＣＲＣエラー通知
// <--- 2006/02/10 Y.Okajima P-BANK通信エラー通知
#define	WM_PBANK_DATAMODE_CHANGE	(WM_USER + 1403)	// P-BANKデータモード変更通知

//-------------------------------------------------
//	記憶済みファイル名
//-------------------------------------------------
#define FLN_KIOKUZUMI		"Kiokuzumi.dat"

//-------------------------------------------------
//	営業履歴ファイル件数(128 * 1024 + 1)
//-------------------------------------------------
#define EIREKI_SUU			131073
//-------------------------------------------------
//	MPC営業履歴ファイル件数
//-------------------------------------------------
#define EIREKI_MPC_SUU			1000000
//-------------------------------------------------
//	夜間履歴ファイル件数()
//-------------------------------------------------
#define YAKANREKI_SUU			1024

#define TOKUREKI_SUU		28670				// AS1枚あたりの特賞履歴ファイルのレコード数
//#define TOKUREKI_SUUx3		(TOKUREKI_SUU * 3)	// AS3枚分の特賞履歴ファイルのレコード数
#define TOKUREKI_SUUx3		(TOKUREKI_SUU * MAX_AS)	// AS3枚分の特賞履歴ファイルのレコード数

#define MOCHIREKI_SUU		3000
#define MOCHIREKI_SUUx3		(MOCHIREKI_SUU * MAX_AS)	// AS3枚分の持玉照合機履歴ファイルのレコード数

#define SOFTSW 2

#endif

