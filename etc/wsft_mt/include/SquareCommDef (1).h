/****************************************************************************/
/*							共通定義 ヘッダファイル							*/
/****************************************************************************/
/*	2006/06/30													Y.Okajima	*/
/****************************************************************************/
//----------------------------------------------------------------------------
/*
 *	$Id$
 */
//----------------------------------------------------------------------------
#if !defined( __SQUARECOMMDEF_H__ )
#define	__SQUARECOMMDEF_H__

//----------------------------------------------------------------------------
/****************************************************************************/
/*	ログ用識別名															*/
/****************************************************************************/
#define		SQUARE_TCP_COMM_APP_TITLE				"SQTCPP"
#define		SQUARE_UDP_COMM_APP_TITLE				"SQUDPP"

//----------------------------------------------------------------------------
/****************************************************************************/
/*	ＴＣＰ送信リクエストメッセージ											*/
/****************************************************************************/
#define		UWM_SQUARE_COMM_TCP_SEND				"UWM_SQUARE_COMM_TCP_SEND"
#define		UWM_SQUARE_COMM_TCP_SET_DATE			"UWM_SQUARE_COMM_TCP_SET_DATE"

#define		UWM_SQUARE_COMM_TCP_SEND_COMPLETE		"UWM_SQUARE_COMM_TCP_SEND_COMPLETE"
#define		UWM_SQUARE_COMM_TCP_SEND_COMPLETE_ERROR	"UWM_SQUARE_COMM_TCP_SEND_COMPLETE_ERROR"

//----------------------------------------------------------------------------
/****************************************************************************/
/*	ＵＤＰ送信リクエストメッセージ											*/
/****************************************************************************/
#define		UWM_SQUARE_COMM_UDP_SEND				"UWM_SQUARE_COMM_UDP_SEND"

//----------------------------------------------------------------------------
/****************************************************************************/
/*	サーバー同時接続数														*/
/****************************************************************************/
#define	SQUARE_COMM_TCP_MAX_CONN			4
#define	SQUARE_COMM_TCP_PORT_START			10601
#define	SQUARE_COMM_TCP_PORT_END			10605
#define	SQUARE_COMM_UDP_PORT_START			20601
#define	SQUARE_COMM_UDP_PORT_END			20602

//----------------------------------------------------------------------------
/****************************************************************************/
/*	終了コード																*/
/****************************************************************************/
enum ENUM_SQUARE_COMM_RESULT {
	SQUARE_COMM_RESULT_CODE_NG = -1,			// エラー終了
	SQUARE_COMM_RESULT_CODE_OK = 0,				// 正常終了
	SQUARE_COMM_RESULT_CODE_ENDSESSION = 9998,	// 強制終了
	SQUARE_COMM_RESULT_CODE_ABORT = 9999		// 強制終了
};

//----------------------------------------------------------------------------
/****************************************************************************/
/*	電文種類																*/
/****************************************************************************/
enum	ENUM_SQUARE_MSG_ID {
	SQUARE_MSG_ID_UNKNOWN = 0,					// 不明

	SQUARE_MSG_ID_TM001 = 1001,					// TM001
	SQUARE_MSG_ID_TM002 = 1002,					// TM002

	SQUARE_MSG_ID_SU001 = 2001,					// SU001
	SQUARE_MSG_ID_SU005 = 2005,					// SU005
	SQUARE_MSG_ID_SU101 = 2101,					// SU101
	SQUARE_MSG_ID_SU102 = 2102,					// SU102

	SQUARE_MSG_ID_EA001 = (-1001),				// EA001
	SQUARE_MSG_ID_EA002 = (-1002),				// EA002

	SQUARE_MSG_ID_EE001 = (-5001),				// EE001
	SQUARE_MSG_ID_EE101 = (-5101),				// EE101
	SQUARE_MSG_ID_EE201 = (-5201),				// EE201
	SQUARE_MSG_ID_EE301 = (-5301),				// EE301
	SQUARE_MSG_ID_EE302 = (-5007),				// EE302
	SQUARE_MSG_ID_EE303 = (-5303),				// EE303
	SQUARE_MSG_ID_EE304 = (-5304),				// EE304
	SQUARE_MSG_ID_EE305 = (-5305),				// EE305
	SQUARE_MSG_ID_EE401 = (-5401),				// EE401
	SQUARE_MSG_ID_EE402 = (-5402),				// EE402
	SQUARE_MSG_ID_EE403 = (-5403),				// EE403
	SQUARE_MSG_ID_EE404 = (-5404),				// EE404
	SQUARE_MSG_ID_EE410 = (-5410),				// EE410
	SQUARE_MSG_ID_EE501 = (-5501),				// EE501
	SQUARE_MSG_ID_EE502 = (-5502),				// EE502
	SQUARE_MSG_ID_EF001 = (-6001),				// EF001
	SQUARE_MSG_ID_EF002 = (-6002),				// EF002
	SQUARE_MSG_ID_EH001 = (-8001),				// EH001
	SQUARE_MSG_ID_EH002 = (-8002),				// EH002
	SQUARE_MSG_ID_EH003 = (-8003),				// EH003
	SQUARE_MSG_ID_EH101 = (-8101),				// EH101
	SQUARE_MSG_ID_EH102 = (-8102),				// EH102
	SQUARE_MSG_ID_EH201 = (-8201),				// EH201
	SQUARE_MSG_ID_EH301 = (-8301),				// EH301
	SQUARE_MSG_ID_EL001 = (-12001),				// EL001
	SQUARE_MSG_ID_EL002 = (-12002),				// EL002
	SQUARE_MSG_ID_EL003 = (-12003),				// EL003
	SQUARE_MSG_ID_EL004 = (-12004),				// EL004
	SQUARE_MSG_ID_EL005 = (-12005),				// EL005
	SQUARE_MSG_ID_EL006 = (-12006),				// EL006
	SQUARE_MSG_ID_EL101 = (-12101),				// EL101
	SQUARE_MSG_ID_EL201 = (-12201),				// EL201
	SQUARE_MSG_ID_EL202 = (-12202),				// EL202
	SQUARE_MSG_ID_EL203 = (-12203),				// EL203
	SQUARE_MSG_ID_EL204 = (-12204),				// EL204
	SQUARE_MSG_ID_EL205 = (-12205),				// EL205
	SQUARE_MSG_ID_EL301 = (-12301)				// EL301
};

//----------------------------------------------------------------------------
/****************************************************************************/
/*	電文ＩＤ																*/
/****************************************************************************/
#define		SQUARE_HEADER_MSG_ID_TM001			"TM001"			// TM001
#define		SQUARE_HEADER_MSG_ID_TM002			"TM002"			// TM002

#define		SQUARE_HEADER_MSG_ID_SU001			"SU001"			// SU001
#define		SQUARE_HEADER_MSG_ID_SU005			"SU005"			// SU005
#define		SQUARE_HEADER_MSG_ID_SU101			"SU101"			// SU101
#define		SQUARE_HEADER_MSG_ID_SU102			"SU102"			// SU102

#define		SQUARE_HEADER_MSG_ID_EA001			"EA001"			// EA001
#define		SQUARE_HEADER_MSG_ID_EA002			"EA002"			// EA002
#define		SQUARE_HEADER_MSG_ID_EE001			"EE001"			// EE001
#define		SQUARE_HEADER_MSG_ID_EE101			"EE101"			// EE101
#define		SQUARE_HEADER_MSG_ID_EE201			"EE201"			// EE201
#define		SQUARE_HEADER_MSG_ID_EE301			"EE301"			// EE301
#define		SQUARE_HEADER_MSG_ID_EE302			"EE302"			// EE302
#define		SQUARE_HEADER_MSG_ID_EE303			"EE303"			// EE303
#define		SQUARE_HEADER_MSG_ID_EE304			"EE304"			// EE304
#define		SQUARE_HEADER_MSG_ID_EE305			"EE305"			// EE305
#define		SQUARE_HEADER_MSG_ID_EE401			"EE401"			// EE401
#define		SQUARE_HEADER_MSG_ID_EE402			"EE402"			// EE402
#define		SQUARE_HEADER_MSG_ID_EE403			"EE403"			// EE403
#define		SQUARE_HEADER_MSG_ID_EE404			"EE404"			// EE404
#define		SQUARE_HEADER_MSG_ID_EE410			"EE410"			// EE410
#define		SQUARE_HEADER_MSG_ID_EE501			"EE501"			// EE501
#define		SQUARE_HEADER_MSG_ID_EE502			"EE502"			// EE502
#define		SQUARE_HEADER_MSG_ID_EF001			"EF001"			// EF001
#define		SQUARE_HEADER_MSG_ID_EF002			"EF002"			// EF002
#define		SQUARE_HEADER_MSG_ID_EH001			"EH001"			// EH001
#define		SQUARE_HEADER_MSG_ID_EH002			"EH002"			// EH002
#define		SQUARE_HEADER_MSG_ID_EH003			"EH003"			// EH003
#define		SQUARE_HEADER_MSG_ID_EH101			"EH101"			// EH101
#define		SQUARE_HEADER_MSG_ID_EH102			"EH102"			// EH102
#define		SQUARE_HEADER_MSG_ID_EH201			"EH201"			// EH201
#define		SQUARE_HEADER_MSG_ID_EH301			"EH301"			// EH301
#define		SQUARE_HEADER_MSG_ID_EL001			"EL001"			// EL001
#define		SQUARE_HEADER_MSG_ID_EL002			"EL002"			// EL002
#define		SQUARE_HEADER_MSG_ID_EL003			"EL003"			// EL003
#define		SQUARE_HEADER_MSG_ID_EL004			"EL004"			// EL004
#define		SQUARE_HEADER_MSG_ID_EL005			"EL005"			// EL005
#define		SQUARE_HEADER_MSG_ID_EL006			"EL006"			// EL006
#define		SQUARE_HEADER_MSG_ID_EL101			"EL101"			// EL101
#define		SQUARE_HEADER_MSG_ID_EL201			"EL201"			// EL201
#define		SQUARE_HEADER_MSG_ID_EL202			"EL202"			// EL202
#define		SQUARE_HEADER_MSG_ID_EL203			"EL203"			// EL203
#define		SQUARE_HEADER_MSG_ID_EL204			"EL204"			// EL204
#define		SQUARE_HEADER_MSG_ID_EL205			"EL205"			// EL205
#define		SQUARE_HEADER_MSG_ID_EL301			"EL301"			// EL301

//----------------------------------------------------------------------------
/****************************************************************************/
/*	端末略号																*/
/****************************************************************************/
#define		SQUARE_HEADER_TERM_NAME_TCAN		"TCAN"			// 送信元端末略号
#define		SQUARE_HEADER_TERM_NAME_CC			"CC  "			// 送信元端末略号
#define		SQUARE_HEADER_TERM_NAME_TC			"TC  "			// 送信元端末略号
#define		SQUARE_HEADER_TERM_NAME_UNKNOWN		"    "			// 送信元端末略号

//----------------------------------------------------------------------------
/****************************************************************************/
/*	ネットワーク番号														*/
/****************************************************************************/
#define		SQUARE_HEADER_SRC_NETADDRESS		8				// 送信元ネットワーク番号

//----------------------------------------------------------------------------
/****************************************************************************/
/*	端末番号																*/
/****************************************************************************/
#define		SQUARE_HEADER_SRC_STATION			0				// 送信元端末番号
#define		SQUARE_HEADER_DEST_STATION			201				// 送信先端末番号

//----------------------------------------------------------------------------
/****************************************************************************/
/*	台番号																	*/
/****************************************************************************/
//#define		SQUARE_HEADER_TERMNO_BASE			8000			// 台番号（ベース番号）
//					⇒　HALLINFO_INI_DEFAULT_SUHEADER_TERMNO_BASE　T2X004967_PBANKVなし対応

//----------------------------------------------------------------------------
/****************************************************************************/
/*	曜日(Header)															*/
/****************************************************************************/
#define		SQUARE_HEADER_WEEK_SUN				0x00			// 日曜日
#define		SQUARE_HEADER_WEEK_MON				0x01			// 月曜日
#define		SQUARE_HEADER_WEEK_TUE				0x02			// 火曜日
#define		SQUARE_HEADER_WEEK_WED				0x03			// 水曜日
#define		SQUARE_HEADER_WEEK_THU				0x04			// 木曜日
#define		SQUARE_HEADER_WEEK_FRI				0x05			// 金曜日
#define		SQUARE_HEADER_WEEK_SAT				0x06			// 土曜日

//----------------------------------------------------------------------------
/****************************************************************************/
/*	台情報電文区分(SU001)													*/
/****************************************************************************/
#define		SQUARE_SU001_USE_SU005				5				// SU005使用

//----------------------------------------------------------------------------
/****************************************************************************/
/*	台情報電文送信間隔(SU001)												*/
/****************************************************************************/
#define		SQUARE_SU001_DEFAULT_INTERVAL_SU005	2*60*1000		// ２分

//----------------------------------------------------------------------------
/****************************************************************************/
/*	システムフラグ(SU001)													*/
/****************************************************************************/
#define		SQUARE_SU001_TYPE_SQUEARE			0				// SQUARE
#define		SQUARE_SU001_TYPE_SQUEARE2			1				// SQUARE2

//----------------------------------------------------------------------------
/****************************************************************************/
/*	アラーム通知（正常系：特賞、正常系：一般、異常系）(SU001)				*/
/****************************************************************************/
#define		SQUARE_SU001_IGNORE_ALARM			0				// 通知しない
#define		SQUARE_SU001_NOTIFY_ALARM			1				// 通知する


//----------------------------------------------------------------------------
/****************************************************************************/
/*	最大送信件数(SU005)														*/
/****************************************************************************/
#define		SU005_SEND_DATA_MAX					6

//----------------------------------------------------------------------------
/****************************************************************************/
/*	区分(SU005)																*/
/****************************************************************************/
#define		SQUARE_SU005_TYPE_DAI				1				// 台
#define		SQUARE_SU005_TYPE_KEISUKI			2				// 計数機
#define		SQUARE_SU005_TYPE_KINKO				3				// 金庫
#define		SQUARE_SU005_TYPE_RYOGAEKI			4				// 両替機
#define		SQUARE_SU005_TYPE_KENBAIKI			5				// 券売機
#define		SQUARE_SU005_TYPE_GAIKASHIKI		6				// 外貸機
#define		SQUARE_SU005_TYPE_SEISANKI			7				// 精算機
#define		SQUARE_SU005_TYPE_HAKKENKINKO		8				// 発券金庫

//----------------------------------------------------------------------------
/****************************************************************************/
/*	台番号オフセット(SU005)													*/
/****************************************************************************/
#define		SQUARE_SU005_DAI_ID_OFFSET			   0			// 台　　　：＋０
#define		SQUARE_SU005_KEISUKI_ID_OFFSET		4000			// 計数機　：＋４０００
#define		SQUARE_SU005_KINKO_ID_OFFSET		4300			// 金庫　　：＋４３００
#define		SQUARE_SU005_RYOGAEKI_ID_OFFSET		4500			// 両替機　：＋４５００
#define		SQUARE_SU005_KENBAIKI_ID_OFFSET		4600			// 券売機　：＋４６００
#define		SQUARE_SU005_GAIKASHIKI_ID_OFFSET	4800			// 外貸機　：＋４８００
#define		SQUARE_SU005_SEISANKI_ID_OFFSET		4900			// 精算機　：＋４９００
#define		SQUARE_SU005_HAKKENKINKO_ID_OFFSET	4700			// 発券金庫：＋４７００

//----------------------------------------------------------------------------
/****************************************************************************/
/*	遊技台状態情報(SU005)													*/
/****************************************************************************/
#define		SQUARE_SU005_DAI_KAD_CND			0x00000001		//稼動中
#define		SQUARE_SU005_DAI_UTI_CND			0x00000002		//打止中
#define		SQUARE_SU005_DAI_TOK_CND			0x00000004		//特賞中
#define		SQUARE_SU005_DAI_HIB_CND			0x00000010		//高確中
#define		SQUARE_SU005_DAI_RTK_CND			0x00000080		//連続特賞
#define		SQUARE_SU005_DAI_YOK_CND			0x00000100		//予告台
#define		SQUARE_SU005_DAI_KAR_CND			0x00000200		//空皿台
#define		SQUARE_SU005_DAI_HOU_CND			0x00000400		//放置台
#define		SQUARE_SU005_DAI_CHK_CND			0x00000800		//注意台
#define		SQUARE_SU005_DAI_FRM_CND			0x00004000		//木枠開
#define		SQUARE_SU005_DAI_PNL_CND			0x00008000		//幕板開
#define		SQUARE_SU005_DAI_FTK_CND			0x00010000		//初当たり中
#define		SQUARE_SU005_DAI_OP1_CND			0x00020000		//ＯＰ１
#define		SQUARE_SU005_DAI_OP2_CND			0x00040000		//ＯＰ２
#define		SQUARE_SU005_DAI_OP3_CND			0x00080000		//ＯＰ３
#define		SQUARE_SU005_DAI_MAD_CND			0x00100000		//窓枠開
#define		SQUARE_SU005_DAI_SN2_CND			0x00200000		//不正２
#define		SQUARE_SU005_DAI_SN1_CND			0x00400000		//不正１
#define		SQUARE_SU005_DAI_CAL_CND			0x00800000		//台呼出
#define		SQUARE_SU005_DAI_KTR_CND			0x01000000		//貸機トラブル
#define		SQUARE_SU005_DAI_KCD_CND			0x02000000		//貸機状態
#define		SQUARE_SU005_DAI_D01_CND			0x04000000		//100断線
#define		SQUARE_SU005_DAI_D05_CND			0x08000000		//500断線
#define		SQUARE_SU005_DAI_D10_CND			0x10000000		//1000断線
#define		SQUARE_SU005_DAI_DCD_CND			0x20000000		//カード断線
#define		SQUARE_SU005_DAI_MTK_CND			0x40000000		//窓枠特賞
#define		SQUARE_SU005_DAI_AUT_CND			0x80000000		//自動打止

//----------------------------------------------------------------------------
/****************************************************************************/
/*	遊技台状態情報(SU005)													*/
/****************************************************************************/
#define		SQUARE_SU005_NASCA_REPLAY_FLG_OFF	0x00			// ＯＦＦ
#define		SQUARE_SU005_NASCA_REPLAY_FLG_ON	0x01			// ＯＮ

//----------------------------------------------------------------------------
/****************************************************************************/
/*	断線(SU005)																*/
/****************************************************************************/
#define		SQUARE_SU005_LINE_NORMAL			0x00			// ＯＦＦ
#define		SQUARE_SU005_LINE_BREAK				0x01			// ＯＮ

//----------------------------------------------------------------------------
/****************************************************************************/
/*	最大データ件数(SU101)													*/
/****************************************************************************/
#define		SQUARE_SU101_DATA_MAX				14

//----------------------------------------------------------------------------
/****************************************************************************/
/*	状態フラグ(SU101)														*/
/****************************************************************************/
#define		SQUARE_SU101_STATUS_FLG_KOUKAKU		0x01			// 高確
#define		SQUARE_SU101_STATUS_FLG_RENZOKU		0x02			// 連続
#define		SQUARE_SU101_STATUS_FLG_MADOWAKU	0x04			// 窓枠
#define		SQUARE_SU101_STATUS_FLG_SENSOR1		0x08			// センサー１
#define		SQUARE_SU101_STATUS_FLG_SENSOR2		0x10			// センサー２
#define		SQUARE_SU101_STATUS_FLG_MOCHIDAMA	0x20			// 持玉
#define		SQUARE_SU101_STATUS_FLG_JITAN		0x40			// 時短
#define		SQUARE_SU101_STATUS_FLG_REG			0x80			// ＲＥＧ

//----------------------------------------------------------------------------
/****************************************************************************/
/*	種別区分(SU102)															*/
/****************************************************************************/
// 遊技台系
#define		SQUARE_SU102_KIND_YUGI						  1			// 遊技台
#define		SQUARE_SU102_KIND_CALL_BUTTON				  2			// 呼出ボタン

// 貸機系
#define		SQUARE_SU102_KIND_SAND						 11			// サンド
#define		SQUARE_SU102_KIND_KINKO						 12			// 金庫
#define		SQUARE_SU102_KIND_GAIKASHIKI				 13			// 外貸機
#define		SQUARE_SU102_KIND_CARD_GAIKASHIKI			 14			// カード外貸機
#define		SQUARE_SU102_KIND_CARD_HAKKENKI				 15			// カード発券機
#define		SQUARE_SU102_KIND_HIKIOTOSHIKI				 16			// 貯玉引落機
#define		SQUARE_SU102_KIND_HAKKEN_PREMIUM			 17			// 発券プレミアム
#define		SQUARE_SU102_KIND_RYOGAEKI					 18			// 両替機
#define		SQUARE_SU102_KIND_DAI_REPLAYKI				 19			// 各台再プレイ機
#define		SQUARE_SU102_KIND_CARD_SEISANKI				 20			// カード精算機
#define		SQUARE_SU102_KIND_HAKKEN_KINKO				 21			// 発券金庫
#define		SQUARE_SU102_KIND_DAI_CARD_HAKKENKI			 22			// 各台カード発券機

// 景品機器系
#define		SQUARE_SU102_KIND_KEIHIN_KEISU				 41			// 景品計数機
#define		SQUARE_SU102_KIND_KEIHIN_RESET				 42			// 景品リセット
#define		SQUARE_SU102_KIND_KEIHIN_HENKYAKU			 43			// 景品返却機
#define		SQUARE_SU102_KIND_KEIHIN_DAI_KEISU			 44			// 各台景品計数機

// 外部機器系
#define		SQUARE_SU102_KIND_SIMA_LAMP					101			// 島ランプ
#define		SQUARE_SU102_KIND_LIFT_TANK_EMPTY			102			// リフト点検（タンク空）
#define		SQUARE_SU102_KIND_LIFT_TANK_FILL			103			// リフト点検（タンク満）
#define		SQUARE_SU102_KIND_SAND_TROUBLE				104			// サンド点検（トラブル）
#define		SQUARE_SU102_KIND_SAND_STATUS				105			// サンド点検（状態）
#define		SQUARE_SU102_KIND_KINKO_TROUBLE				106			// 金庫点検（トラブル）
#define		SQUARE_SU102_KIND_KINKO_STATUS				107			// 金庫点検（状態）
#define		SQUARE_SU102_KIND_GAIKASHIKI_TROUBLE		108			// 外貸機点検（トラブル）
#define		SQUARE_SU102_KIND_GAIKASHIKI_STATUS			109			// 外貸機点検（状態）
#define		SQUARE_SU102_KIND_HAKKENKI_TROUBLE			110			// カード発券機点検（トラブル）
#define		SQUARE_SU102_KIND_HAKKENKI_STATUS			111			// カード発券機点検（状態）
#define		SQUARE_SU102_KIND_HIKIOTOSHIKI_TROUBLE		112			// 貯玉引落機点検（トラブル）
#define		SQUARE_SU102_KIND_HIKIOTOSHIKI_STATUS		113			// 貯玉引落機点検（状態）
#define		SQUARE_SU102_KIND_KEIHIN_KEISUKI_TROUBLE	114			// 景品計数機点検（トラブル）
#define		SQUARE_SU102_KIND_KEIHIN_KEISUKI_STATUS		115			// 景品計数機点検（状態）
#define		SQUARE_SU102_KIND_KEIHIN_HENKYAKUKI_TROUBLE	116			// 景品返却機点検（トラブル）
#define		SQUARE_SU102_KIND_KEIHIN_HENKYAKUKI_STATUS	117			// 景品返却機点検（状態）
#define		SQUARE_SU102_KIND_ETC						118			// その他点検

//----------------------------------------------------------------------------
/****************************************************************************/
/*	事象(SU102)																*/
/****************************************************************************/
#define		SQUARE_SU102_EVENT_NORMAL_UCHIDOME_AUTO		  1		// 自動打止発生				正常系(一般)
#define		SQUARE_SU102_EVENT_NORMAL_UCHIDOME_MANUAL	  2		// 手動打止発生				正常系(一般)
#define		SQUARE_SU102_EVENT_NORMAL_UCHIDOME_NOTIFY	  3		// 打止予告通知				正常系(一般)
#define		SQUARE_SU102_EVENT_NORMAL_UCHIDOME_CANCEL	  4		// 打止予告取消				正常系(一般)
#define		SQUARE_SU102_EVENT_NORMA_TOKUSYO_START		  5		// 特賞検出通知				正常系(特賞)
#define		SQUARE_SU102_EVENT_NORMAL_TOKUSYO_END		  6		// 特賞終了通知				正常系(特賞)
#define		SQUARE_SU102_EVENT_TOKUSYO_FIRST_HIT_START	  7		// 初当り検出通知			正常系(特賞)
#define		SQUARE_SU102_EVENT_TOKUSYO_FIRST_HIT_END	  8		// 初当り終了通知			正常系(特賞)
#define		SQUARE_SU102_EVENT_ERROR_NOTICE_START		  9		// 注意台検出通知			異常系
#define		SQUARE_SU102_EVENT_ERROR_NOTICE_END			 10		// 注意台終了通知			異常系
#define		SQUARE_SU102_EVENT_NORMAL_AUTO_CLEAR		 11		// 自動クリア開放通知		正常系(一般)
#define		SQUARE_SU102_EVENT_NORMAL_MANUAL_CLEAR		 12		// 手動クリア開放通知		正常系(一般)
#define		SQUARE_SU102_EVENT_NORMAL_LEAVE_SET			 13		// 放置台セット通知			正常系(一般)
#define		SQUARE_SU102_EVENT_NORMAL_LEAVE_PASSED		 14		// 放置台時間通過通知		正常系(一般)
#define		SQUARE_SU102_EVENT_ERROR_EMPTY				 15		// 空皿異常発生通知			異常系
#define		SQUARE_SU102_EVENT_ERROR_EMPTY_RECOVERY		 16		// 空皿異常復帰通知			異常系
#define		SQUARE_SU102_EVENT_ERROR_OUT				 17		// アウト異常通知			異常系
#define		SQUARE_SU102_EVENT_ERROR_OUT_RECOVERY		 18		// アウト異常解除			異常系
#define		SQUARE_SU102_EVENT_ERROR_SAFE				 19		// セーフ異常通知			異常系
#define		SQUARE_SU102_EVENT_ERROR_SAFE_RECOVERY		 20		// セーフ異常解除			異常系
#define		SQUARE_SU102_EVENT_ERROR_WINDOW_OPEN		 21		// 窓枠オープン				異常系
#define		SQUARE_SU102_EVENT_ERROR_WINDOW_CLOSE		 22		// 窓枠クローズ				異常系
#define		SQUARE_SU102_EVENT_ERROR_FRAME_OPEN			 23		// 木枠オープン				異常系
#define		SQUARE_SU102_EVENT_ERROR_FRAME_CLOSE		 24		// 木枠クローズ				異常系
#define		SQUARE_SU102_EVENT_ERROR_COVER_OPEN			 25		// 幕板オープン				異常系
#define		SQUARE_SU102_EVENT_ERROR_COVER_CLOSE		 26		// 幕板クローズ				異常系
#define		SQUARE_SU102_EVENT_ERROR_SENSOR1_DETECT		 27		// センサー１検出			異常系
#define		SQUARE_SU102_EVENT_ERROR_SENSOR1_CLEAR		 28		// センサー１消滅			異常系
#define		SQUARE_SU102_EVENT_ERROR_SENSOR2_DETECT		 29		// センサー２検出			異常系
#define		SQUARE_SU102_EVENT_ERROR_SENSOR2_CLEAR		 30		// センサー２消滅			異常系
#define		SQUARE_SU102_EVENT_NORMAL_LEAVE_CLEAR		 31		// 放置台解除通知			正常系(一般)
#define		SQUARE_SU102_EVENT_ERROR_WINDOW_TOKUSYO		 32		// 窓枠特賞発生				異常系
#define		SQUARE_SU102_EVENT_NORMAL_ADD_BALL			 33		// 玉数追加通知				正常系(一般)
#define		SQUARE_SU102_EVENT_NORMAL_ADD_BALL_OPEN		 34		// 玉数追加開放通知			正常系(一般)
#define		SQUARE_SU102_EVENT_NORMAL_MODIFY_BALL		 35		// 持ち玉修正通知			正常系(一般)
#define		SQUARE_SU102_EVENT_NORMAL_MODIFY_BALL_OPEN	 36		// 持ち玉修正開放通知		正常系(一般)
#define		SQUARE_SU102_EVENT_ERROR_KASHIKI_BREAKE_LINE 37		// 貸機断線検出通知			異常系
#define		SQUARE_SU102_EVENT_ERROR_KASHIKI_RECOVERY	 38		// 貸機断線復帰通知			異常系
#define		SQUARE_SU102_EVENT_ERROR_KEIHIN_BREAK_LINE	 39		// 景品断線検出通知			異常系
#define		SQUARE_SU102_EVENT_ERROR_KEIHIN_RECOVERY	 40		// 景品断線復帰通知			異常系
#define		SQUARE_SU102_EVENT_ERROR_KINKO_ERROR		 41		// 金庫サンド誤差異常		異常系
#define		SQUARE_SU102_EVENT_NORMAL_DAI_CALL_START	 42		// 台呼出検出				正常系(一般)
#define		SQUARE_SU102_EVENT_NORMAL_DAI_CALL_END		 43		// 台呼出終了				正常系(一般)
#define		SQUARE_SU102_EVENT_ERROR_KASHIKI_DATA_ERROR	 44		// 貸機データ異常通知		異常系
#define		SQUARE_SU102_EVENT_ERROR_KEIHIN_DATA_ERROR	 45		// 景品データ異常通知		異常系
#define		SQUARE_SU102_EVENT_ERROR_OUTSIDE			 46		// 外部異常発生通知			異常系
#define		SQUARE_SU102_EVENT_ERROR_OUTSIDE_RECOVERY	 47		// 外部異常復帰通知			異常系
#define		SQUARE_SU102_EVENT_ERROR_TOKUSYO_START_CNT	 48		// 特賞始動数異常検出		異常系
#define		SQUARE_SU102_EVENT_ERROR_TOKUSYU_CONTINUE_A	 49		// 特賞連続異常Ａ検出		異常系
#define		SQUARE_SU102_EVENT_ERROR_TOKUSYU_CONTINUE_B	 50		// 特賞連続異常Ｂ検出		異常系
#define		SQUARE_SU102_EVENT_ERROR_TOKUSYU_DEDAMA_A	 51		// 特賞出玉異常Ａ検出		異常系
#define		SQUARE_SU102_EVENT_ERROR_TOKUSYO_DEDAMA_B	 52		// 特賞出玉異常Ｂ検出		異常系
#define		SQUARE_SU102_EVENT_ERROR_START_CNT_A		 53		// 始動数異常Ａ検出			異常系
#define		SQUARE_SU102_EVENT_ERROR_START_CNT_B		 54		// 始動数異常Ｂ検出			異常系
#define		SQUARE_SU102_EVENT_ERROR_IRREGULAR_PLAY_A	 55		// 変則プレーＡ検出			異常系
#define		SQUARE_SU102_EVENT_ERROR_IRREGULAR_PLAY_B	 56		// 変則プレーＢ検出			異常系
#define		SQUARE_SU102_EVENT_ERROR_NORMAL_SAFE		 57		// 通常セーフ異常検出		異常系
#define		SQUARE_SU102_EVENT_ERROR_TOKUSYO_OUT		 58		// 特賞アウト異常検出		異常系
#define		SQUARE_SU102_EVENT_ERROR_NORMAL_DEDAMA		 59		// 通常出玉異常検出			異常系
#define		SQUARE_SU102_EVENT_ERROR_HIGH_DEDAMA		 60		// 高確率時出玉率異常検出	異常系
#define		SQUARE_SU102_EVENT_ERROR_OP1_DEDAMA			 61		// ＯＰ１中出玉率異常検出	異常系
#define		SQUARE_SU102_EVENT_ERROR_TOKUSYO			 62		// 特賞異常					異常系
#define		SQUARE_SU102_EVENT_ERROR_HIGH				 63		// 高確率異常				異常系
#define		SQUARE_SU102_EVENT_ERROR_DEDAMA				 64		// 出玉異常					異常系


//----------------------------------------------------------------------------
/****************************************************************************/
/*	通信エラー判定回数														*/
/****************************************************************************/
#define		SQUARE_COMM_COMM_ERROR_COUNT_MAX			3


#endif
//----------------------------------------------------------------------------
/*
 * $Log: /GNw3.2.0.x/include/SquareCommDef.h $
 * 
 * 9     09/01/19 11:10 Nakatani
 * T2X004967_PBANKVなし対応
 */
//----------------------------------------------------------------------------
