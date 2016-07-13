/* WAIS START */
//----------------------------------------------
//概要
//	共有メモリ関係各種定義
//
//作成者
//	Tokai 纐纈
//
//備考
//	共有メモリ関係各種定義を宣言する
//----------------------------------------------

#if !defined(_SHAREDOBJS__H)
#define _SHAREDOBJS__H


// マッピングオブジェクト名
// Ｗｉｎ形式データメモリファイル（インテル形式）
#define MMP_F4006		"f4006.bin"	// システム異常履歴
#define MMP_F4007		"f4007.bin"	// クリア履歴
#define MMP_F4008		"f4008.bin"	// 時計設定
#define MMP_F4009		"f4009.bin"	// 停電履歴
#define MMP_F4010		"f4010.bin"	// 設定履歴
//	2003/11/10	yasui
#define MMP_F4012		"f4012.bin"	// 夜間履歴
#define MMP_F4013		"f4013.bin"	// 記憶履歴	2004.03.23 nao
#define MMP_F4015		"f4015.bin"	// システム履歴	2004.10.15 hashimoto
#define	MMP_F4021		"f4021.bin"	// パネル情報ファイル
#define	MMP_F4028		"f4028.bin"	// パネル情報2ファイル

#define	MMP_F4101		"f4101.bin"			// 記憶情報ファイル
#define	MMP_F4102		"f4102.bin"			// 台ファイル
#define	MMP_F4103		"f4103.bin"			// 全店ファイル
#define	MMP_F4112		"f4112.bin"			// 定時情報  nakatani
#define	MMP_F4113		"f4113.bin"			// 定時種別稼動  nakatani
#define	MMP_F4114		"f4114.bin"			// 定時全店稼動  nakatani
#define	MMP_F4115		"f4115.bin"			// 機種別定時稼動 2003/04/09 nakatani
#define	MMP_F4117		"f4117.bin"			// DSPデータファイル	2006.06.29 add hashimoto(T2X002907)
#define	MMP_F4550		"f4550.bin"			// ピークデータファイル
#define	MMP_F4601		"f4601.bin"			// 営業履歴ファイル
#define	MMP_F4848		"f4848.bin"			// 島制御発生履歴ファイル	2006.05.12 add hashimoto(T2X002883)
#define	MMP_F5201		"f5201.bin"			// ＡＳ会員履歴データファイル	2002.11.06  Add by koketsu (wss1.5)
#define	MMP_F5201_KEN	"f5201_ken.bin"		// ＡＳ会員履歴データ件数ファイル	2002.11.11  Add by koketsu (wss1.5)
#define MMP_RPLY_LOG	"Rply_Log.bin"		// 遊技履歴(累計ﾃﾞｰﾀ)		2002.11.06  Add by koketsu (wss1.5)
#define MMP_KPLY_LOG	"Kply_Log.bin"		// 遊技履歴(個々ﾃﾞｰﾀ)		2002.11.06  Add by koketsu (wss1.5)
#define MMP_KAIIN_LOG	"Kaiin_Log.bin"		// 会員履歴					2002.11.06  Add by koketsu (wss1.5)
#define MMP_KAIPLY_LOG	"Kaiply_Log.bin"	// 会員遊技履歴				2002.11.06  Add by koketsu (wss1.5)
#define MMP_F6001_1		"f6001_1.bin"		// ラージ素データ(台) (AS#1用)
#define MMP_F6001_2		"f6001_2.bin"		// ラージ素データ(台) (AS#2用)
#define MMP_F6001_3		"f6001_3.bin"		// ラージ素データ(台) (AS#3用)
#define MMP_F6001_4		"f6001_4.bin"		// ラージ素データ(台) (AS#4用)
#define MMP_F6001		"f6001.bin"			// ラージ素データ(台) (AS#1-MAX_AS用)
#define MMP_F6002_1		"f6002_1.bin"		// ラージ素データ(全店) (AS#1用)
#define MMP_F6002_2		"f6002_2.bin"		// ラージ素データ(全店) (AS#2用)
#define MMP_F6002_3		"f6002_3.bin"		// ラージ素データ(全店) (AS#3用)
#define MMP_F6002_4		"f6002_4.bin"		// ラージ素データ(全店) (AS#4用)
#define MMP_F6002		"f6002.bin"			// ラージ素データ(全店) (AS#1-MAX_AS用)
#define MMP_F6003_1		"f6003_1.bin"		// 各台定時稼動情報ファイル (AS#1用)
#define MMP_F6003_2		"f6003_2.bin"		// 各台定時稼動情報ファイル (AS#2用)
#define MMP_F6003_3		"f6003_3.bin"		// 各台定時稼動情報ファイル (AS#3用)
#define MMP_F6003_4		"f6003_4.bin"		// 各台定時稼動情報ファイル (AS#4用)
#define MMP_F6003		"f6003.bin"			// 各台定時稼動情報ファイル (AS#1-MAX_AS用)
#define MMP_F6004		"f6004.bin"			// 持玉照合機履歴ファイル (AS#1-MAX_AS用)
#define MMP_F6004_1		"f6004_1.bin"		// 持玉照合機履歴ファイル (AS#1用)
#define MMP_F6004_2		"f6004_2.bin"		// 持玉照合機履歴ファイル (AS#2用)
#define MMP_F6004_3		"f6004_3.bin"		// 持玉照合機履歴ファイル (AS#3用)
#define MMP_F6004_4		"f6004_4.bin"		// 持玉照合機履歴ファイル (AS#4用)
#define MMP_F6005		"f6005.bin"			// 持玉照合機履歴管理ファイル (AS#1-MAX_AS用)
#define MMP_F6005_1		"f6005_1.bin"		// 持玉照合機履歴管理ファイル (AS#1用)
#define MMP_F6005_2		"f6005_2.bin"		// 持玉照合機履歴管理ファイル (AS#2用)
#define MMP_F6005_3		"f6005_3.bin"		// 持玉照合機履歴管理ファイル (AS#3用)
#define MMP_F6005_4		"f6005_4.bin"		// 持玉照合機履歴管理ファイル (AS#4用)
#define	MMP_F6101_1		"f6101_1.bin"		// 特賞履歴記憶情報ファイル１（ＡＳ＃１用）
#define	MMP_F6101_2		"f6101_2.bin"		// 特賞履歴記憶情報ファイル２（ＡＳ＃２用）
#define	MMP_F6101_3		"f6101_3.bin"		// 特賞履歴記憶情報ファイル３（ＡＳ＃３用）
#define	MMP_F6101_4		"f6101_4.bin"		// 特賞履歴記憶情報ファイル４（ＡＳ＃４用）
#define	MMP_F6102		"f6102.bin"			// 特賞履歴ファイル（ＡＳ＃ 1～MAX_AS 用）
#define	MMP_F6102_1		"f6102_1.bin"		// 特賞履歴ファイル１（ＡＳ＃１用）
#define	MMP_F6102_2		"f6102_2.bin"		// 特賞履歴ファイル２（ＡＳ＃２用）
#define	MMP_F6102_3		"f6102_3.bin"		// 特賞履歴ファイル３（ＡＳ＃３用）
#define	MMP_F6102_4		"f6102_4.bin"		// 特賞履歴ファイル４（ＡＳ＃４用）
//***↓　追加　yasui　2002/10/09
#define	MMP_F6103		"f6103.bin"			// スタートスランプファイル（ＡＳ＃ 1～MAX_AS 用）
#define	MMP_F6103_1		"f6103_1.bin"		// スタートスランプファイル1（ＡＳ＃１用）
#define	MMP_F6103_2		"f6103_2.bin"		// スタートスランプファイル2（ＡＳ＃２用）
#define	MMP_F6103_3		"f6103_3.bin"		// スタートスランプファイル3（ＡＳ＃３用）
#define	MMP_F6103_4		"f6103_4.bin"		// スタートスランプファイル4（ＡＳ＃４用）
//***↑　追加　yasui　2002/10/09
#define	MMP_F6105		"f6105.bin"			// 特賞履歴2ファイル（ＡＳ＃ 1～MAX_AS 用）
#define	MMP_F6105_1		"f6105_1.bin"		// 特賞履歴2ファイル１（ＡＳ＃１用）
#define	MMP_F6105_2		"f6105_2.bin"		// 特賞履歴2ファイル２（ＡＳ＃２用）
#define	MMP_F6105_3		"f6105_3.bin"		// 特賞履歴2ファイル３（ＡＳ＃３用）
#define	MMP_F6105_4		"f6105_4.bin"		// 特賞履歴2ファイル４（ＡＳ＃４用）
#define	MMP_F6408		"f6408.bin"			// 個々台ファイル（ DAISU 分）
#define	MMP_F6408_1		"f6408_1.bin"		// 個々台ファイル１（ＡＳ＃１用５１２台分）
#define	MMP_F6408_2		"f6408_2.bin"		// 個々台ファイル２（ＡＳ＃２用５１２台分）
#define	MMP_F6408_3		"f6408_3.bin"		// 個々台ファイル３（ＡＳ＃３用５１２台分）
#define	MMP_F6408_4		"f6408_4.bin"		// 個々台ファイル４（ＡＳ＃４用５１２台分）
#define	MMP_F6407		"f6407.bin"			// 累計台ファイル（ DAISU 分）			2002.06.12 koketsu Add
#define	MMP_F6407_1		"f6407_1.bin"		// 累計台ファイル１（ＡＳ＃１用５１２台分）2002.06.12 koketsu Add
#define	MMP_F6407_2		"f6407_2.bin"		// 累計台ファイル２（ＡＳ＃２用５１２台分）2002.06.12 koketsu Add
#define	MMP_F6407_3		"f6407_3.bin"		// 累計台ファイル３（ＡＳ＃３用５１２台分）2002.06.12 koketsu Add
#define	MMP_F6407_4		"f6407_4.bin"		// 累計台ファイル４（ＡＳ＃４用５１２台分）2002.06.12 koketsu Add
#define	MMP_F6409		"f6409.bin"			// TNスランプファイル（ DAISU 分）		2002.11.20 koketsu Add
#define	MMP_F6409_1		"f6409_1.bin"		// TNスランプファイル１（ＡＳ＃１用５１２台分）2002.11.20 koketsu Add
#define	MMP_F6409_2		"f6409_2.bin"		// TNスランプファイル２（ＡＳ＃２用５１２台分）2002.11.20 koketsu Add
#define	MMP_F6409_3		"f6409_3.bin"		// TNスランプファイル３（ＡＳ＃３用５１２台分）2002.11.20 koketsu Add
#define	MMP_F6409_4		"f6409_4.bin"		// TNスランプファイル４（ＡＳ＃４用５１２台分）2002.11.20 koketsu Add
#define MMP_F6409_0		"f6409_0.bin"		// 正しい配列のTNスランプファイル

#define	MMP_F6411_1		"f6411_1.bin"		// 全店ファイル１（ＡＳ＃１用）
#define	MMP_F6411_2		"f6411_2.bin"		// 全店ファイル２（ＡＳ＃２用）
#define	MMP_F6411_3		"f6411_3.bin"		// 全店ファイル３（ＡＳ＃３用）
#define	MMP_F6411_4		"f6411_4.bin"		// 全店ファイル４（ＡＳ＃４用）

#define	MMP_F6419		"f6419.bin"			// TNスランプ２ファイル（ DAISU 分）
#define	MMP_F6419_1		"f6419_1.bin"		// TNスランプ２ファイル１（ＡＳ＃１用５１２台分）
#define	MMP_F6419_2		"f6419_2.bin"		// TNスランプ２ファイル２（ＡＳ＃２用５１２台分）
#define	MMP_F6419_3		"f6419_3.bin"		// TNスランプ２ファイル３（ＡＳ＃３用５１２台分）
#define	MMP_F6419_4		"f6419_4.bin"		// TNスランプ２ファイル４（ＡＳ＃４用５１２台分）

#define	MMP_F9000		"possum_a.bin"		// POS統計データファイル（Ａ店用）
#define	MMP_F9001		"possum_b.bin"		// POS統計データファイル（Ｂ店用）
#define	MMP_F9010		"tptokei_a.bin"		// TP統計データファイル（Ａ店用）
#define	MMP_F9011		"tptokei_b.bin"		// TP統計データファイル（Ｂ店用）

#define	MMP_F5120		"f5120.bin"			// 貯玉日報ファイル

#define	MMP_KIJUNKI_DAI		"kijunki_dai_syu.bin"	// 新基準機 台集計ファイル		T2X002549	2005.07.19	nao
#define	MMP_KIJUNKI_KISHU	"kijunki_kis_syu.bin"	// 新基準機 機種集計ファイル	T2X002549	2005.07.19	nao
#define	MMP_KIJUNKI_SIMA	"kijunki_sim_syu.bin"	// 新基準機 島集計ファイル		T2X002549	2005.07.19	nao
#define	MMP_KIJUNKI_SBT		"kijunki_sbt_syu.bin"	// 新基準機 種別集計ファイル	T2X002549	2005.07.26	nao

/*//↓モトローラ形式MMP廃止　2003/12/16　yasui
// 生データメモリファイル（モトローラ形式）
#define	MMP_MF4101		"mf4101.bin"		// 記憶情報ファイル
#define	MMP_MF4102		"mf4102.bin"		// 台ファイル
#define	MMP_MF4103		"mf4103.bin"		// 全店ファイル
#define	MMP_MF4601		"mf4601.bin"		// 営業履歴ファイル
#define	MMP_MF6101_1	"mf6101_1.bin"		// 特賞履歴記憶情報ファイル１（ＡＳ＃１用）
#define	MMP_MF6101_2	"mf6101_2.bin"		// 特賞履歴記憶情報ファイル２（ＡＳ＃２用）
#define	MMP_MF6101_3	"mf6101_3.bin"		// 特賞履歴記憶情報ファイル３（ＡＳ＃３用）
#define	MMP_MF6102		"mf6102.bin"		// 特賞履歴ファイル（ＡＳ＃１２３用）
#define	MMP_MF6102_1	"mf6102_1.bin"		// 特賞履歴ファイル１（ＡＳ＃１用）
#define	MMP_MF6102_2	"mf6102_2.bin"		// 特賞履歴ファイル２（ＡＳ＃２用）
#define	MMP_MF6102_3	"mf6102_3.bin"		// 特賞履歴ファイル３（ＡＳ＃３用）
//***↓　追加　yasui　2002/10/09
#define	MMP_MF6103  	"mf6103.bin"		// スタートスランプファイル
#define	MMP_MF6103_1	"mf6103_1.bin"		// スタートスランプファイル1（ＡＳ＃１用）
#define	MMP_MF6103_2	"mf6103_2.bin"		// スタートスランプファイル2（ＡＳ＃２用）
#define	MMP_MF6103_3	"mf6103_3.bin"		// スタートスランプファイル3（ＡＳ＃３用）
//***↑　追加　yasui　2002/10/09
#define	MMP_MF6408		"mf6408.bin"		// 個々台ファイル（１５３３台分）
#define	MMP_MF6408_1	"mf6408_1.bin"		// 個々台ファイル１（ＡＳ＃１用５１２台分）
#define	MMP_MF6408_2	"mf6408_2.bin"		// 個々台ファイル２（ＡＳ＃２用５１２台分）
#define	MMP_MF6408_3	"mf6408_3.bin"		// 個々台ファイル３（ＡＳ＃３用５１２台分）
#define	MMP_MF6407		"mf6407.bin"		// 累計台ファイル（１５３３台分）			2002.06.12 koketsu Add
#define	MMP_MF6407_1	"mf6407_1.bin"		// 累計台ファイル１（ＡＳ＃１用５１２台分）	2002.06.12 koketsu Add
#define	MMP_MF6407_2	"mf6407_2.bin"		// 累計台ファイル２（ＡＳ＃２用５１２台分）	2002.06.12 koketsu Add
#define	MMP_MF6407_3	"mf6407_3.bin"		// 累計台ファイル３（ＡＳ＃３用５１２台分）	2002.06.12 koketsu Add
#define	MMP_MF6409		"mf6409.bin"		// TNスランプファイル（１５３３台分）		2002.11.20 koketsu Add
#define	MMP_MF6409_1	"mf6409_1.bin"		// TNスランプファイル１（ＡＳ＃１用５１２台分）2002.11.20 koketsu Add
#define	MMP_MF6409_2	"mf6409_2.bin"		// TNスランプファイル２（ＡＳ＃２用５１２台分）2002.11.20 koketsu Add
#define	MMP_MF6409_3	"mf6409_3.bin"		// TNスランプファイル３（ＡＳ＃３用５１２台分）2002.11.20 koketsu Add

#define	MMP_MF9000		"possum_a.bin"		// POS統計データファイル（Ａ店用）
#define	MMP_MF9001		"possum_b.bin"		// POS統計データファイル（Ｂ店用）
#define	MMP_MF9010		"tptokei_a.bin"		// TP統計データファイル（Ａ店用）
#define	MMP_MF9011		"tptokei_b.bin"		// TP統計データファイル（Ｂ店用）
//↑モトローラ形式MMP廃止　2003/12/16　yasui*/

// グローバルテーブル
#define	MMP_COMNTBL		"comntbl.bin"		// 共有テーブル
#define	MMP_SYUFILE		"syufile.bin"		// 集計データ
#define MMP_KOKODAI		"kokodai.bin"		// 個々台データ
#define	MMP_IDXSIMA		"idxsima.bin"		// 島ＩＤＸテーブル
#define	MMP_IDXKISYU	"idxkisyu.bin"		// 機種ＩＤＸテーブル
#define	MMP_IDXKINKO	"idxkinko.bin"		// 金庫ＩＤＸテーブル
#define	MMP_TEIKI		"teiki.bin"			// 定期通信データ（ＦＦ０１伝文データ）
//	2002.05.20 追加 By Fujiki(２店舗)-->
#define MMP_ZEN_TENPO	"Zen_Tenpo.bin"	// Ａ・Ｂ店舗全店集計データ（２店舗）
#define MMP_ZEN_FLOOR	"Zen_Floor.bin"		// フロア全店集計データ（２店舗）
//	-->2002.05.20 追加 By Fujiki(２店舗)

//設定データメモリファイル
#define	MMP_F1000		"f1000.bin"			// システム設定
#define	MMP_F1001		"f1001.bin"			// 台接続設定
#define	MMP_F1002		"f1002.bin"			// 全店接続設定
#define	MMP_F1003		"f1003.bin"			// 島端ランプ設定
#define	MMP_F1004		"f1004.bin"			// タイプ設定
//	2002.05.17 追加 By Fujiki(２店舗)-->
#define	MMP_F1006		"f1006.bin"			// 店舗接続設定
//	-->2002.05.17 追加 By Fujiki(２店舗)
#define	MMP_F1007		"f1007.bin"			// 種別ポケット設定 2002.06.14 追加 kaoru（２店舗）
#define	MMP_F1008		"f1008.bin"			// 店舗･種別名称設定 ←2002/11/25 (店舗数対応)追加 Fukuyama
#define	MMP_F1010		"f1010.bin"			// 台接続設定2
#define	MMP_F1011		"f1011.bin"			// 音声自動出力設定
#define	MMP_F1012		"f1012.bin"			// 持玉照合機ＩＤ設定
#define	MMP_F1050		"f1050.bin"			// 売上定数設定
#define	MMP_F1051		"f1051.bin"			// 機種名設定
#define	MMP_F1052		"f1052.bin"			// タイプ設定
#define	MMP_F1053		"f1053.bin"			// Ｔｎスランプ設定
#define	MMP_F1056		"f1056.bin"			// 貯玉上限値設定 // <-- 2010/09/10 add 新会員管理Step1 kanda
#define	MMP_F1100		"f1100.bin"			// 暗証番号設定
#define	MMP_F1101		"f1101.bin"			// 開閉店時刻設定
#define	MMP_F1102		"f1102.bin"			// 金庫点検設定
#define	MMP_F1103		"f1103.bin"			// 全店停止設定
#define	MMP_F1104		"f1104.bin"			// 機種停止設定
#define	MMP_F1105		"f1105.bin"			// 全店運用設定
#define	MMP_F1106		"f1106.bin"			// 機種運用設定
#define	MMP_F1107		"f1107.bin"			// D2設定
#define	MMP_F1108		"f1108.bin"			// 店舗別変則営業設定 ←2002/10/29 (店舗別変則営業対応)追加 Fukuyama
#define	MMP_F1109		"f1109.bin"			// 特賞振分設定		　←2005/07/19 (新基準機対応)追加 hashimoto
#define	MMP_F1110		"f1110.bin"			// D2拡張設定		　←2006/02/06 (DU4拡張入力対応)追加 hashimoto
#define	MMP_F1111		"f1111.bin"			// DSP設定						←2006/06/28 (DSP-100接続対応)追加 hashimoto
#define	MMP_F1112		"f1112.bin"			// DSPサンド金庫回収ルート設定	←2006/06/28 (DSP-100接続対応)追加 hashimoto
#define	MMP_F1113		"f1113.bin"			// 各台計数機設定
#define	MMP_F1114		"f1114.bin"			// D2設定(台別)
#define	MMP_F1115		"f1115.bin"			// D2拡張設定(台別)
#define	MMP_F1118		"f1118.bin"			// セーフメーター異常検知設定
#define	MMP_F1150		"f1150.bin"			// ＴＮ設定
#define	MMP_F1151		"f1151.bin"			// TN設定２
#define	MMP_F1152		"f1152.bin"			// リプレイ手数料共通設定 2002.06.14 追加 kaoru（２店舗）
#define	MMP_F1153		"f1153.bin"			// ＴＮ時間変動リプレイ手数料設定 2002.06.14 追加 kaoru（２店舗）
#define	MMP_F1200		"f1200.bin"			// ポイント全店設定
#define	MMP_F1201		"f1201.bin"			// ポイント機種別設定
//	2002.06.29 追加 By Fujiki-->
#define	MMP_F1202		"f1202.bin"			// 時間変動来店ポイント設定
//	-->2002.06.29 追加 By Fujiki
#define	MMP_F1203		"f1203.bin"			// 変動稼動ポイント設定 2002.06.14 追加 kaoru（２店舗）
//	2002.06.29 追加 By Fujiki-->
#define	MMP_F1204		"f1204.bin"			// 時間変動売上ポイント設定
//	-->2002.06.29 追加 By Fujiki
#define	MMP_F1401		"f1401.bin"			// 台ランプ全店動作設定			2005/3/8 (台ランプ対応 T2X002355対応)追加 Fukuyama
#define	MMP_F1402		"f1402.bin"			// 台ランプ機種別動作設定		2005/3/8 (台ランプ対応 T2X002355対応)追加 Fukuyama
#define	MMP_F1403		"f1403.bin"			// 台ランプリモコンＩＤ設定（T2X002355）2005/03/08 tomida
#define	MMP_F1404		"f1404.bin"			// 台ランプランプパターン設定	2005/3/8 (台ランプ対応 T2X002355対応)追加 Fukuyama
#define	MMP_F1405		"f1405.bin"			// 台ランプランプ設定			2005/3/8 (台ランプ対応 T2X002355対応)追加 Fukuyama
#define MMP_F1406		"f1406.bin"			// 台ランプ光フロー設定			20050629 add tomida 台ランプ次期開発（T2X002500）
#define MMP_F1407		"f1407.bin"			// 台ランプ演出フロー設定		20060106 add hashimoto Ｇシステム対応（T2X002759）
#define MMP_F1408		"f1408.bin"			// 台ランプランプパターン設定2	20061208 add hashimoto 80型台ランプ対応（T2X003029）

#define MMP_F1500		"f1500.bin"			// 島制御島全体設定		20060512 add hashimoto 島管理システム開発（T2X002883）
#define MMP_F1501		"f1501.bin"			// 島制御島別設定		20060512 add hashimoto 島管理システム開発（T2X002883）

// ↓モトローラ形式MMP廃止に伴い移動　2003/12/16　yasui
#define MMP_F4104		"f4104.bin"		// 島集計
#define MMP_F4105		"f4105.bin"		// 機種集計
#define MMP_F4106		"f4106.bin"		// 種別集計

#define MMP_F4112			"f4112.bin"	// 定時稼動
#define MMP_F4113			"f4113.bin"	// 定時種別稼動
#define MMP_F4114			"f4114.bin"	// 定時全店稼動
#define MMP_F4115			"f4115.bin"	// 定時機種
#define MMP_F4117			"f4117.bin"	// DSPデータファイル
#define MMP_F4550			"f4550.bin"	// ピークデータファイル
#define MMP_F4005			"f4005.bin"	// 記憶情報
#define MMP_F4006			"f4006.bin"	// システム異常履歴
#define MMP_F4007			"f4007.bin"	// クリア履歴
#define MMP_F4008			"f4008.bin"	// 時計設定
#define MMP_F4009			"f4009.bin"	// 停電履歴
#define MMP_F4010			"f4010.bin"	// 設定履歴
#define MMP_F4011			"f4011.bin"	// 設定VER情報
#define	MMP_F4116			"f4116.bin"	// DSPデータファイル		←2006/06/28 (DSP-100接続対応)追加 hashimoto
#define MMP_F4022			"f4022.bin"	// 異常履歴
#define MMP_F4023			"f4023.bin"	// 打止履歴
#define MMP_F4024			"f4024.bin"	// 発生履歴

#define MMP_F4848			"f4848.bin"	// 島制御発生履歴		20060512 add hashimoto 島管理システム開発（T2X002883）

//#define MMP_MF6409			"mf6409.bin"	// ＴＮスランプ2002.11.20 koketsu Del （上で定義）
#define MMP_F5201			"f5201.bin"	// 会員ログ
#define MMP_F6301			"f6301.bin"	// 会員ログ管理情報

#if	1	/* 2004.06.07(mon) add.AS交換時復旧用 by ohnot */

#define MMP_F4001			"f4001.bin"
#define MMP_F6303_1			"f6303_1.bin"	//ＴＮ情報テーブル１（ＡＳ＃１用）
#define MMP_F6303_2			"f6303_2.bin"	//ＴＮ情報テーブル２（ＡＳ＃２用）
#define MMP_F6303_3			"f6303_3.bin"	//ＴＮ情報テーブル３（ＡＳ＃３用）
#define MMP_F6303_4			"f6303_4.bin"	//ＴＮ情報テーブル４（ＡＳ＃４用）
#define MMP_F6304_1			"f6304_1.bin"	//ＴＮスランプ記憶前日分１（ＡＳ＃１用５１２台分）
#define MMP_F6304_2			"f6304_2.bin"	//ＴＮスランプ記憶前日分２（ＡＳ＃２用５１２台分）
#define MMP_F6304_3			"f6304_3.bin"	//ＴＮスランプ記憶前日分３（ＡＳ＃３用５１２台分）
#define MMP_F6304_4			"f6304_4.bin"	//ＴＮスランプ記憶前日分４（ＡＳ＃４用５１２台分）
#define MMP_F6305_1			"f6305_1.bin"	//ＴＮスランプ記憶前々日分１（ＡＳ＃１用５１２台分）
#define MMP_F6305_2			"f6305_2.bin"	//ＴＮスランプ記憶前々日分２（ＡＳ＃２用５１２台分）
#define MMP_F6305_3			"f6305_3.bin"	//ＴＮスランプ記憶前々日分３（ＡＳ＃３用５１２台分）
#define MMP_F6305_4			"f6305_4.bin"	//ＴＮスランプ記憶前々日分４（ＡＳ＃４用５１２台分）

#define MMP_F6410_1			"f6410_1.bin"	// 個々ＴＮデータ１（ＡＳ＃１用５１２台分）
#define MMP_F6410_2			"f6410_2.bin"	// 個々ＴＮデータ２（ＡＳ＃２用５１２台分）
#define MMP_F6410_3			"f6410_3.bin"	// 個々ＴＮデータ３（ＡＳ＃３用５１２台分）
#define MMP_F6410_4			"f6410_4.bin"	// 個々ＴＮデータ４（ＡＳ＃４用５１２台分）

#define MMP_F6501_1			"f6501_1.bin"	// 台ランプリカバリ用バックアップデータ１（ＡＳ＃１用５１２台分）
#define MMP_F6501_2			"f6501_2.bin"	// 台ランプリカバリ用バックアップデータ２（ＡＳ＃２用５１２台分）
#define MMP_F6501_3			"f6501_3.bin"	// 台ランプリカバリ用バックアップデータ３（ＡＳ＃３用５１２台分）
#define MMP_F6501_4			"f6501_4.bin"	// 台ランプリカバリ用バックアップデータ４（ＡＳ＃４用５１２台分）

#endif	/* 2004.06.07(mon) add.AS交換時復旧用 by ohnot */
// ↑モトローラ形式MMP廃止に伴い移動　2003/12/16　yasui

// 排他アクセス用
#define MTX_F4006		"mtx_f4006"			// システム異常履歴
#define MTX_F4007		"mtx_f4007"			// クリア履歴
#define MTX_F4008		"mtx_f4008"			// 時計設定
#define MTX_F4009		"mtx_f4009"			// 停電履歴
#define MTX_F4010		"mtx_f4010"			// 設定履歴
#define MTX_F4012		"mtx_f4012"			// 夜間履歴ファイル　2003/11/10yasui
#define MTX_F4013		"mtx_f4013"			// 記憶履歴	2004.03.23 nao
#define MTX_F4015		"mtx_f4015"			// システム履歴	2004.10.15 hashimoto
#define	MTX_F4021		"mtx_f4021"			// パネル情報ファイル
#define	MTX_F4028		"mtx_f4028"			// パネル情報2ファイル

#define	MTX_F4101		"mtx_f4101"			// 排他記憶情報データ
#define	MTX_F4102		"mtx_f4102"			// 排他台ファイル
#define	MTX_F4103		"mtx_f4103"			// 排他全店ファイル
#define	MTX_F4112		"mtx_f4112"			// 定時情報  nakatani
#define	MTX_F4113		"mtx_f4113"			// 定時種別稼動  nakatani
#define	MTX_F4114		"mtx_f4114"			// 定時全店稼動  nakatani
#define	MTX_F4115		"mtx_f4115"			// 機種別定時稼動 2003/04/09 nakatani
#define	MTX_F4117		"mtx_f4117"			// DSPデータファイル	2006.06.29 add hashimoto(T2X002907)
#define	MTX_F4550		"mtx_f4550"			// ピークデータファイル
#define	MTX_F4601		"mtx_f4601"			// 排他営業履歴ファイル
#define	MTX_F4848		"mtx_f4848"			// 排他島制御発生履歴ファイル	2006.05.12 add hashimoto(T2X002883)

#define	MTX_F5201		"mtx_f5201"			// ＡＳ会員履歴ファイル（Win形式用） 2002.11.06  Add by koketsu
#define	MTX_MF5201		"mtx_mf5201"		// ＡＳ会員履歴ファイル（ﾓﾄﾛｰﾗ形式用） 2002.11.06  Add by koketsu
#define MTX_RPLY_LOG	"mtx_Rply_Log"		// 遊技履歴(累計ﾃﾞｰﾀ)		2002.11.06  Add by koketsu (wss1.5)
#define MTX_KPLY_LOG	"mtx_Kply_Log"		// 遊技履歴(個々ﾃﾞｰﾀ)		2002.11.06  Add by koketsu (wss1.5)
#define MTX_KAIIN_LOG	"mtx_Kaiin_Log"		// 会員履歴					2002.11.06  Add by koketsu (wss1.5)
#define MTX_KAIPLY_LOG	"mtx_Kaiply_Log"	// 会員遊技履歴				2002.11.06  Add by koketsu (wss1.5)

#define	MTX_F5120		"mtx_f5120"			// ＫＳ貯玉日報ファイル（Win形式用） 2004.1.15  Add by nao

#define MTX_F6001		"mtx_f6001"			// 排他ラージ素データ(台)
#define MTX_F6002		"mtx_f6002"			// 排他ラージ素データ(全店)
#define MTX_F6003		"mtx_f6003"			// 各台定時稼動情報ファイル
#define	MTX_F6004		"mtx_f6004"			// 排他持玉照合機履歴ファイル（ＡＳ＃ 1～MAX_AS ４用）
#define	MTX_F6004_1		"mtx_f6004_1"		// 排他持玉照合機履歴ファイル１（ＡＳ＃１用）
#define	MTX_F6004_2		"mtx_f6004_2"		// 排他持玉照合機履歴ファイル２（ＡＳ＃２用）
#define	MTX_F6004_3		"mtx_f6004_3"		// 排他持玉照合機履歴ファイル３（ＡＳ＃３用）
#define	MTX_F6004_4		"mtx_f6004_4"		// 排他持玉照合機履歴ファイル４（ＡＳ＃４用）
#define	MTX_F6005		"mtx_f6005"			// 排他持玉照合機履歴管理ファイル（ＡＳ＃ 1～MAX_AS ４用）
#define	MTX_F6005_1		"mtx_f6005_1"		// 排他持玉照合機履歴管理ファイル１（ＡＳ＃１用）
#define	MTX_F6005_2		"mtx_f6005_2"		// 排他持玉照合機履歴管理ファイル２（ＡＳ＃２用）
#define	MTX_F6005_3		"mtx_f6005_3"		// 排他持玉照合機履歴管理ファイル３（ＡＳ＃３用）
#define	MTX_F6005_4		"mtx_f6005_4"		// 排他持玉照合機履歴管理ファイル４（ＡＳ＃４用）

#define	MTX_F6101_1		"mtx_f6101_1"		// 排他特賞履歴記憶情報ファイル１（ＡＳ＃１用）
#define	MTX_F6101_2		"mtx_f6101_2"		// 排他特賞履歴記憶情報ファイル２（ＡＳ＃２用）
#define	MTX_F6101_3		"mtx_f6101_3"		// 排他特賞履歴記憶情報ファイル３（ＡＳ＃３用）
#define	MTX_F6101_4		"mtx_f6101_4"		// 排他特賞履歴記憶情報ファイル４（ＡＳ＃４用）
#define	MTX_F6102		"mtx_f6102"			// 排他特賞履歴ファイル（ＡＳ＃ 1～MAX_AS ４用）
#define	MTX_F6102_1		"mtx_f6102_1"		// 排他特賞履歴ファイル１（ＡＳ＃１用）
#define	MTX_F6102_2		"mtx_f6102_2"		// 排他特賞履歴ファイル２（ＡＳ＃２用）
#define	MTX_F6102_3		"mtx_f6102_3"		// 排他特賞履歴ファイル３（ＡＳ＃３用）
#define	MTX_F6102_4		"mtx_f6102_4"		// 排他特賞履歴ファイル４（ＡＳ＃４用）
//***↓　追加　yasui　2002/10/09
#define	MTX_F6103		"mtx_f6103"			// スタートスランプファイル（ＡＳ＃ 1～MAX_AS 用）
#define	MTX_F6103_1		"mtx_f6103_1"		// スタートスランプファイル1（ＡＳ＃１用）
#define	MTX_F6103_2		"mtx_f6103_2"		// スタートスランプファイル2（ＡＳ＃２用）
#define	MTX_F6103_3		"mtx_f6103_3"		// スタートスランプファイル3（ＡＳ＃３用）
#define	MTX_F6103_4		"mtx_f6103_4"		// スタートスランプファイル4（ＡＳ＃４用）
//***↑　追加　yasui　2002/10/09
#define	MTX_F6105		"mtx_f6105"			// 排他特賞履歴2ファイル（ＡＳ＃ 1～MAX_AS ４用）
#define	MTX_F6105_1		"mtx_f6105_1"		// 排他特賞履歴2ファイル１（ＡＳ＃１用）
#define	MTX_F6105_2		"mtx_f6105_2"		// 排他特賞履歴2ファイル２（ＡＳ＃２用）
#define	MTX_F6105_3		"mtx_f6105_3"		// 排他特賞履歴2ファイル３（ＡＳ＃３用）
#define	MTX_F6105_4		"mtx_f6105_4"		// 排他特賞履歴2ファイル４（ＡＳ＃４用）
#define	MTX_F6408		"mtx_f6408"			// 排他個々台データ
#define	MTX_F6408_1		"mtx_f6408_1"		// 排他個々台ファイル１（ＡＳ＃１用）
#define	MTX_F6408_2		"mtx_f6408_2"		// 排他個々台ファイル２（ＡＳ＃２用）
#define	MTX_F6408_3		"mtx_f6408_3"		// 排他個々台ファイル３（ＡＳ＃３用）
#define	MTX_F6408_4		"mtx_f6408_4"		// 排他個々台ファイル４（ＡＳ＃４用）
#define	MTX_F6407		"mtx_f6407"			// 排他累計台データ						2002.06.12 koketsu Add
#define	MTX_F6407_1		"mtx_f6407_1"		// 排他累計台ファイル１（ＡＳ＃１用）	2002.06.12 koketsu Add
#define	MTX_F6407_2		"mtx_f6407_2"		// 排他累計台ファイル２（ＡＳ＃２用）	2002.06.12 koketsu Add
#define	MTX_F6407_3		"mtx_f6407_3"		// 排他累計台ファイル３（ＡＳ＃３用）	2002.06.12 koketsu Add
#define	MTX_F6407_4		"mtx_f6407_4"		// 排他累計台ファイル４（ＡＳ＃４用）	2002.06.12 koketsu Add
#define	MTX_F6409		"mtx_f6409"			// TNスランプファイル（５１２ * MAX_AS 台分）		2002.11.20 koketsu Add
#define	MTX_F6409_1		"mtx_f6409_1"		// TNスランプファイル１（ＡＳ＃１用５１２台分）2002.11.20 koketsu Add
#define	MTX_F6409_2		"mtx_f6409_2"		// TNスランプファイル２（ＡＳ＃２用５１２台分）2002.11.20 koketsu Add
#define	MTX_F6409_3		"mtx_f6409_3"		// TNスランプファイル３（ＡＳ＃３用５１２台分）2002.11.20 koketsu Add
#define	MTX_F6409_4		"mtx_f6409_4"		// TNスランプファイル４（ＡＳ＃４用５１２台分）2002.11.20 koketsu Add
#define MTX_F6409_0		"mtx_f6409_0"

#define	MTX_F6411_1		"mtx_f6411_1"		// 排他全店ファイル１（ＡＳ＃１用）
#define	MTX_F6411_2		"mtx_f6411_2"		// 排他全店ファイル２（ＡＳ＃２用）
#define	MTX_F6411_3		"mtx_f6411_3"		// 排他全店ファイル３（ＡＳ＃３用）
#define	MTX_F6411_4		"mtx_f6411_4"		// 排他全店ファイル４（ＡＳ＃４用）

#define	MTX_F6419		"mtx_f6419"			// TNスランプ２ファイル（５１２ * MAX_AS 台分）
#define	MTX_F6419_1		"mtx_f6419_1"		// TNスランプ２ファイル１（ＡＳ＃１用５１２台分）
#define	MTX_F6419_2		"mtx_f6419_2"		// TNスランプ２ファイル２（ＡＳ＃２用５１２台分）
#define	MTX_F6419_3		"mtx_f6419_3"		// TNスランプ２ファイル３（ＡＳ＃３用５１２台分）
#define	MTX_F6419_4		"mtx_f6419_4"		// TNスランプ２ファイル４（ＡＳ＃４用５１２台分）

#define	MTX_COMNTBL		"mtx_comntbl"		// 排他共有テーブル
#define	MTX_SYUFILE		"mtx_syufile"		// 排他集計データ
#define MTX_KOKODAI		"mtx_kokodai"		// 排他個々台データ
#define	MTX_IDXSIMA		"mtx_idxsima"		// 排他島ＩＤＸテーブル
#define	MTX_IDXKISYU	"mtx_idxkisyu"		// 排他機種ＩＤＸテーブル
#define	MTX_IDXKINKO	"mtx_idxkinko"		// 排他金庫ＩＤＸテーブル
#define	MTX_TEIKI		"mtx_teiki"			// 排他定期通信データ（ＦＦ０１伝文データ）
//	2002.05.20 追加 By Fujiki(２店舗)-->
#define MTX_ZEN_TENPO	"mtx_Zen_Tenpo"		// Ａ・Ｂ店舗全店集計データ	（２店舗）
#define MTX_ZEN_FLOOR	"mtx_Zen_Floor"		// フロア全店集計データ		（２店舗）
//	-->2002.05.20 追加 By Fujiki(２店舗)

#define MTX_KIJUNKI_ALL		"mtx_Kijunki_all"		// 新基準機 集計ファイル		2005.07.11	nao

#define	MTX_F1000		"mtx_f1000"			// 排他システム設定
#define	MTX_F1001		"mtx_f1001"			// 排他台接続設定
#define	MTX_F1002		"mtx_f1002"			// 排他全店接続設定
#define	MTX_F1003		"mtx_f1003"			// 排他島端ランプ設定
#define	MTX_F1004		"mtx_f1004"			// 排他タイプ設定
//	2002.05.20 追加 By Fujiki(２店舗)-->
#define	MTX_F1006		"mtx_f1006"			// 排他店舗接続設定
//	-->2002.05.20 追加 By Fujiki(２店舗)
#define	MTX_F1007		"mtx_f1007"			// 排他種別ポケット設定 2002.06.14 追加 kaoru（２店舗）
#define	MTX_F1008		"mtx_f1008"			// 排他店舗･種別名称設定 ←2002/11/25 (店舗数対応)追加 Fukuyama
#define	MTX_F1010		"mtx_f1010"			// 排他台接続設定2
#define	MTX_F1011		"mtx_f1011"			// 排他音声自動出力設定
#define	MTX_F1012		"mtx_f1012"			// 持玉照合機ＩＤ設定
#define	MTX_F1050		"mtx_f1050"			// 排他売上定数設定
#define	MTX_F1051		"mtx_f1051"			// 排他機種名設定
#define	MTX_F1052		"mtx_f1052"			// 排他タイプ設定
#define	MTX_F1053		"mtx_f1053"			// 排他Ｔｎスランプ設定
#define	MTX_F1056		"mtx_f1056"			// 排他貯玉上限値設定 // <-- 2010/09/10 add 新会員管理Step1 kanda
#define	MTX_F1100		"mtx_f1100"			// 排他暗証番号設定
#define	MTX_F1101		"mtx_f1101"			// 排他開閉店時刻設定
#define	MTX_F1102		"mtx_f1102"			// 排他金庫点検設定
#define	MTX_F1103		"mtx_f1103"			// 排他全店停止設定
#define	MTX_F1104		"mtx_f1104"			// 排他機種停止設定
#define	MTX_F1105		"mtx_f1105"			// 排他全店運用設定
#define	MTX_F1106		"mtx_f1106"			// 排他機種運用設定
#define	MTX_F1107		"mtx_f1107"			// 排他D2設定
#define	MTX_F1108		"mtx_f1108"			// 排他店舗別変則営業設定 ←2002/10/29 (店舗別変則営業対応)追加 Fukuyama
#define	MTX_F1109		"mtx_f1109"			// 排他特賞振分設定 　　　←2005/07/19 (新基準機対応)追加 hashimoto
#define	MTX_F1110		"mtx_f1110"			// 排他D2拡張設定		　←2006/02/06 (DU4拡張入力対応)追加 hashimoto
#define	MTX_F1111		"mtx_f1111"			// DSP設定						←2006/06/28 (DSP-100接続対応)追加 hashimoto
#define	MTX_F1112		"mtx_f1112"			// DSPサンド金庫回収ルート設定	←2006/06/28 (DSP-100接続対応)追加 hashimoto
#define	MTX_F1113		"mtx_f1113"			// 排他各台計数機設定
#define	MTX_F1114		"mtx_f1114"			// 排他D2設定(台別)
#define	MTX_F1115		"mtx_f1115"			// 排他D2拡張設定(台別)
#define	MTX_F1118		"mtx_f1118"			// 排他セーフメーター異常検知設定
#define	MTX_F1150		"mtx_f1150"			// 排他ＴＮ設定
#define	MTX_F1151		"mtx_f1151"			// 排他TN設定２
#define	MTX_F1152		"mtx_f1152"			// リプレイ手数料共通設定 2002.06.14 追加 kaoru（２店舗）
#define	MTX_F1153		"mtx_f1153"			// ＴＮ時間変動リプレイ手数料設定 2002.06.14 追加 kaoru（２店舗）
#define	MTX_F1200		"mtx_f1200"			// 排他ポイント全店設定
#define	MTX_F1201		"mtx_f1201"			// 排他ポイント機種別設定
//	2002.06.29 追加 By Fujiki-->
#define	MTX_F1202		"mtx_f1202"			// 変動稼動ポイント設定 2002.06.14 追加 kaoru（２店舗）
//	-->2002.06.29 追加 By Fujiki
#define	MTX_F1203		"mtx_f1203"			// 変動稼動ポイント設定 2002.06.14 追加 kaoru（２店舗）
//	2002.06.29 追加 By Fujiki-->
#define	MTX_F1204		"mtx_f1204"			// 変動稼動ポイント設定 2002.06.14 追加 kaoru（２店舗）
//	-->2002.06.29 追加 By Fujiki
#define	MTX_F1401		"mtx_f1401"			// 排他台ランプ全店動作設定			2005/3/8 (台ランプ対応 T2X002355対応)追加 Fukuyama
#define	MTX_F1402		"mtx_f1402"			// 排他台ランプ機種別動作設定		2005/3/8 (台ランプ対応 T2X002355対応)追加 Fukuyama
#define	MTX_F1403		"mtx_f1403"			// 排他台ランプリモコンＩＤ設定（T2X002355）2005/03/08 tomida
#define	MTX_F1404		"mtx_f1404"			// 排他台ランプランプパターン設定	2005/3/8 (台ランプ対応 T2X002355対応)追加 Fukuyama
#define	MTX_F1405		"mtx_f1405"			// 排他台ランプランプ設定			2005/3/8 (台ランプ対応 T2X002355対応)追加 Fukuyama
#define MTX_F1406		"mtx_f1406"			// 20050629 add tomida 台ランプ次期開発（T2X002500）
#define MTX_F1407		"mtx_f1407"			// 20060106 add hashimoto Ｇシステム対応（T2X002759）
#define MTX_F1408		"mtx_f1408"			// 20061208 add hashimoto 80型台ランプ対応（T2X003029）

#define MTX_F1500		"mtx_f1500"			// 20060512 add hashimoto 島管理システム開発（T2X002883）
#define MTX_F1501		"mtx_f1501"			// 20060512 add hashimoto 島管理システム開発（T2X002883）
#define	MTX_F4116		"mtx_f4116"			// DSPデータファイル		←2006/06/28 (DSP-100接続対応)追加 hashimoto
#define MTX_F4848		"mtx_f4848"			// 20060512 add hashimoto 島管理システム開発（T2X002883）

#define MTX_F9000		"mtx_f9000"			// POS統計データファイル（Ａ店用）
#define MTX_F9001		"mtx_f9001"			// POS統計データファイル（Ｂ店用）
#define MTX_F9010		"mtx_f9010"			// TP統計データファイル（Ａ店用）
#define MTX_F9011		"mtx_f9011"			// TP統計データファイル（Ｂ店用）

/*↓モトローラ形式MMP廃止　2003/12/16　yasui
//--------------------------------------------
// ＫＰＣに必要なファイル名（モトローラ形式）
// これらはＷｉｎ操作が使用するわけではありせん
// -------------------------------------------
#define	MMP_MF1000		"mf1000.bin"		// システム設定
#define	MMP_MF1001		"mf1001.bin"		// 台接続設定
#define	MMP_MF1002		"mf1002.bin"		// 全店接続設定
#define	MMP_MF1003		"mf1003.bin"		// 島端ランプ設定
#define	MMP_MF1004		"mf1004.bin"		// タイプ設定
#define	MMP_MF1005		"mf1005.bin"		// 音声基板接続設定
#define	MMP_MF1006		"mf1006.bin"		// 店舗接続設定
#define	MMP_MF1007		"mf1007.bin"		// 種別ポケット設定
#define	MMP_MF1008		"mf1008.bin"		// 店舗･種別名称設定	←2002/11/25 (店舗数対応)追加 Fukuyama
#define	MMP_MF1050		"mf1050.bin"		// 売上定数設定
#define	MMP_MF1051		"mf1051.bin"		// 機種名設定
#define	MMP_MF1052		"mf1052.bin"		// タイプ設定
#define	MMP_MF1053		"mf1053.bin"		// Ｔｎスランプ設定
#define	MMP_MF1100		"mf1100.bin"		// 暗証番号設定
#define	MMP_MF1101		"mf1101.bin"		// 開閉店時刻設定
#define	MMP_MF1102		"mf1102.bin"		// 金庫点検設定
#define	MMP_MF1103		"mf1103.bin"		// 全店停止設定
#define	MMP_MF1104		"mf1104.bin"		// 機種停止設定
#define	MMP_MF1105		"mf1105.bin"		// 全店運用設定
#define	MMP_MF1106		"mf1106.bin"		// 機種運用設定			← 2002/09/18 追加 Fukuyama
#define	MMP_MF1107		"mf1107.bin"		// D2設定
#define	MMP_MF1108		"mf1108.bin"		// 店舗別変則営業設定   ← 2002/10/29 (店舗別変則営業対応)追加 Fukuyama
#define	MMP_MF1150		"mf1150.bin"		// ＴＮ設定
#define	MMP_MF1151		"mf1151.bin"		// TN設定２
#define	MMP_MF1152		"mf1152.bin"		// リプレイ手数料共通設定
#define	MMP_MF1153		"mf1153.bin"		// ＴＮ時間変動リプレイ手数料設定
#define	MMP_MF1200		"mf1200.bin"		// ポイント全店設定
#define	MMP_MF1201		"mf1201.bin"		// ポイント機種別設定
#define	MMP_MF1202		"mf1202.bin"		// 時間変動来店ポイント設定
#define	MMP_MF1203		"mf1203.bin"		// 変動稼動ポイント設定
#define	MMP_MF1204		"mf1204.bin"		// 時間変動売上ポイント設定

#define MMP_MF4104		"mf4104.bin"		// 島集計
#define MMP_MF4105		"mf4105.bin"		// 機種集計
#define MMP_MF4106		"mf4106.bin"		// 種別集計


#define MMP_MF4112			"mf4112.bin"	// 定時稼動
#define MMP_MF4113			"mf4113.bin"	// 定時種別稼動
#define MMP_MF4114			"mf4114.bin"	// 定時全店稼動
#define MMP_MF4115			"mf4115.bin"	// 定時機種
#define MMP_MF4005			"mf4005.bin"	// 記憶情報
#define MMP_MF4006			"mf4006.bin"	// システム異常履歴
#define MMP_MF4007			"mf4007.bin"	// クリア履歴
#define MMP_MF4008			"mf4008.bin"	// 時計設定
#define MMP_MF4009			"mf4009.bin"	// 停電履歴
#define MMP_MF4010			"mf4010.bin"	// 設定履歴
#define MMP_MF4011			"mf4011.bin"	// 設定VER情報
#define MMP_MF4022			"mf4022.bin"	// 異常履歴
#define MMP_MF4023			"mf4023.bin"	// 打止履歴
#define MMP_MF4024			"mf4024.bin"	// 発生履歴
//#define MMP_MF6409			"mf6409.bin"	// ＴＮスランプ2002.11.20 koketsu Del （上で定義）
#define MMP_MF5201			"mf5201.bin"	// 会員ログ
#define MMP_MF6301			"mf6301.bin"	// 会員ログ管理情報
//↑モトローラ形式MMP廃止　2003/12/12　yasui*/

// Kpc出力用ファイル名
#define KPC_F1051			"f1051_org.bin"	// 機種名設定	

#endif
