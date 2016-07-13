
////////////////////////////////////////////////////////////////////////////

/*		** HC2007 定数変更 **

	HC2007 2店舗8種別250機種50タイプ　で種別を4→8に変更

	POS有のシステムで8種別対応する場合はこのファイルを修正してください。
*/

////////////////////////////////////////////////////////////////////////////



/*
 *	NAS POS
 *	アプリケーション定数、ワーク、構造体の定義
 *
 *	$Header: /usr/external/CVS/nas2002/Pos/include/work.h,v 1.8 2003/10/01 04:40:04 sugiura Exp $
 *	$Log: work.h,v $
 *	Revision 1.8  2003/10/01 04:40:04  sugiura
 *	no message
 *	
 *	Revision 1.7  2003/09/25 01:34:30  sugiura
 *	エラーチケット情報対応(POS)
 *	
 *	Revision 1.6  2003/07/14 05:55:06  sugiura
 *	SNAP_R_030701のソフトに対して
 *	責任者管理機能の不具合を修正
 *	
 *	Revision 1.5  2003/03/18 05:40:50  sugiura
 *	POS貯玉処理リトライ対応
 *	
 *	Revision 1.4  2003/02/27 04:27:02  sugiura
 *	SNAP_R_021119のソフトに対して
 *	POS種別選択画面変更の機能を実装
 *	
 *	Revision 1.3  2002/11/20 02:12:51  sugiura
 *	POS-CDS通信処理変更
 *	
 *	Revision 1.2  2002/10/07 04:52:49  sugiura
 *	SNAP_R_020808のソフトに対して
 *	２店舗４種別(完全版)の機能を実装
 *	
 *	Revision 1.1  2002/06/25 04:22:00  sugiura
 *	2店舗4種別対応(なります)
 *	
 *	Revision 1.37  2002/04/03 07:46:06  sugiura
 *	SNAP_R_020227のソフトに対して
 *	新ＴＰ(太巻き対応)に対応した機能を追加
 *	
 *	Revision 1.36  2002/02/25 02:07:00  sugiura
 *	SNAP_R_020213のソフトに対して
 *	・POS再起動後にCDSにカードが挿入されていた時のカードOUT
 *	を送信する条件を追加
 *	
 *	・myreconが発生したらエラー履歴ファイルに書き込む処理追加
 *	
 *	Revision 1.35  2002/02/01 06:56:11  sugiura
 *	SNAP_R_020122のソフトに対して
 *	マルハン特殊景品特別パターンに対応する機能追加
 *	３種別対応(A種別の貯メダル処理はSとAの単価を考慮して行う)
 *	貯玉日報のA種別対応
 *	
 *	Revision 1.34  2001/12/12 10:50:00  sugiura
 *	同一アークネット２店舗構成実装
 *	交換画面のfont14 → 16に変更
 *	
 *	Revision 1.33  2001/11/30 10:33:53  sugiura
 *	会員カード制御(POS)
 *	前日分閉店処理
 *	
 *	Revision 1.32  2001/11/05 03:55:30  sugiura
 *	POS交換単価小数点対応
 *	
 *	Revision 1.31  2001/10/02 04:46:25  sugiura
 *	中締め機能拡張、返品(戻し)制限追加
 *	
 *	Revision 1.30  2001/08/24 08:29:12  sugiura
 *	責任者別操作制限機能
 *	処理履歴をCAに送信
 *	POS主動マスタ受信機能
 *	
 *	Revision 1.29  2001/06/29 01:38:42  sugiura
 *	POS在庫管理拡張、新閉店処理対応
 *	
 *	Revision 1.28  2001/04/16 06:06:33  fukuoka
 *	POS第１四半期
 *	
 *	Revision 1.27  2001/02/26 05:11:38  fukuoka
 *	不具合対応
 *	１　画面操作不具合
 *	２　エースチケットのコード入力不具合
 *	３　会員情報端末-会員情報変更の改良
 *	４　総合履歴の変更（貯玉・サービス玉）
 *	５　エラー内容の充実
 *	６　1回目のチケット読込で連続キーを要求される不具合
 *	
 *	Revision 1.26  2001/01/12 09:55:53  fukuoka
 *	機能改良
 *	１　記憶処理、クリア処理の見直し。
 *	　　OPEN前クリアの新規追加
 *	
 *	２　景品情報参照画面の充実
 *	
 *	３　処理結果を明確に印字させる
 *	
 *	Revision 1.24  2000/11/01 07:48:39  shimizu
 *	no message
 *	
 *	Revision 1.23  2000/08/03 12:24:15  shimizu
 *	no message
 *	
 *	Revision 1.21  2000/07/12 09:53:23  hoshiyam
 *	 １．レシート自動印字で「景品戻し」「拾い玉」「保留券発券」の区別が付ける
 *	 ２．接続機器設定のＫＳとＣＡの状態を見て処理を切替え(フィラー)
 *	 ３．景品戻しの状態から抜けない不具合の修正
 *	 ４．在庫集計処理のタイムアウト値変更(５秒→２秒)
 *	
 *	Revision 1.20  2000/07/11 10:55:21  fukuoka
 *	レシート変更（拾い玉・景品戻し・保留券発券）、閉店処理変更等
 *	
 *	Revision 1.16  2000/06/19 23:50:37  hoshiyam
 *	中締め印字機能追加に伴う修正
 *	
 *	Revision 1.15  2000/06/01 05:23:33  hoshiyam
 *	チケットパスワードエラーコードの追加
 *	
 *	Revision 1.14  2000/05/18 03:55:01  hoshiyam
 *	no message
 *	
 *	Revision 1.13  2000/04/21 08:00:09  hoshiyam
 *	POS基本設定にフラグの追加
 *	
 *	Revision 1.12  2000/04/21 02:00:36  hoshiyam
 *	POS基本設定構造体にフラグ情報メンバーの追加
 *	
 *	Revision 1.10  2000/04/07 01:03:32  hoshiyam
 *	no message
 *	
 *	Revision 1.9  2000/03/29 09:33:11  hoshiyam
 *	no message
 *	
 *	Revision 1.8  2000/03/23 07:11:04  hoshiyam
 *	no message
 *	
 *	Revision 1.7  2000/03/21 06:50:19  hoshiyam
 *	no message
 *	
 *	Revision 1.5  2000/02/23 01:47:55  hoshiyam
 *	no message
 *	
 *	Revision 1.2  2000/02/04 07:11:12  hoshiyam
 *	no message
 *	
 *	Revision 1.5  2000/01/31 12:23:24  hoshiyam
 *	no message
 *	
 * 
 * 3     00/01/31 16:48 Hoshiyam
 * 
 * 2     00/01/24 14:00 Shimizu
 * 
 * 1     00/01/21 20:42 Hoshiyam
 * 
 * 1     00/01/21 20:04 Hoshiyam
 * 
 * 93    99/12/13 11:35 Hoshiyam
 * 特殊景品自動計算方法選択情報を基本設定に追加。
 * 
 * 92    99/11/26 20:22 Hoshiyam
 * 
 * 91    99/11/10 13:56 Hoshiyam
 * マルハン 新小岩対応に関する修正
 * 
 * 90    99/10/20 14:32 Hoshiyam
 * 連続キーのエラーメッセージコードを追加
 * 
 * 89    99/10/20 13:12 Hoshiyam
 * 自販機データリンク不具合対応
 * 
 * 88    99/10/18 12:19 Hoshiyam
 * 自販機データリンク、連続キー対応、カードイン状態LCD表示、TP通信リトライ
 * 時間の仕様変更に伴う修正。
 * 
 * 87    99/09/30 13:04 Hoshiyam
 * フィーバー 不具合対応 Version 1.1
 * 
 * 86    99/09/10 17:13 Hoshiyam
 * カード照合機間通信の改造に関する修正
 * 
 * 85    99/09/07 16:13 Hoshiyam
 * 連続キーフラグエリアの追加
 * 
 * 84    99/09/02 18:01 Hoshiyam
 * 払出し機のエラーコード追加
 * 
 * 83    99/08/30 14:41 Hoshiyam
 * 払出し機通信に関する通信
 * 
 * 82    99/08/10 9:53 Hoshiyam
 * 
 * 81    99/08/09 13:34 Hoshiyam
 * 
 * 80    99/08/03 14:18 Nakata
 * 払い出し機メーカー定義追加
 * 
 * 79    99/08/03 13:37 Tanaka
 * ca_syukei_flagを追加
 * 
 * 78    99/08/03 13:13 Nakata
 * 
 * 77    99/07/27 12:16 Tanaka
 * JIHANA_ARCMIN定数を追加。今後VENDING_ARCIDは廃止の方向
 * 
 * 76    99/07/26 10:31 Tanaka
 * sizeof(pos_basic_t)==72となるよう調整
 * 
 * 75    99/07/24 12:17 Hoshiyam
 * 
 * 74    99/07/24 10:17 Hoshiyam
 * 
 * 73    99/07/24 10:04 Hoshiyam
 * 
 * 73	 99/07/23 17:40 Hoshiyama
 * 接続機器設定で８台まで自販機と通信できるように変更した。
 *
 * 72    99/07/07 9:39 Tanaka
 * 貯玉強制フラグを追加
 * pmf_basicファイルのサイズを72バイトになるよう調整
 * 
 * 71    99/07/02 16:49 Tanaka
 * 総合履歴で、貯玉預入の玉数をWORDからDWORDへ拡張
 * 
 * 70    99/07/02 10:34 Fukuoka
 * ＰＯＳ基本設定に玉換算値を追加
 * 
 * 69    99/06/29 18:34 Tanaka
 * POS基本設定にlcd_clear_flagを追加
 * 
 * 68    99/06/22 15:00 Tanaka
 * 手入力抑制フラグを基本設定に追加
 * 
 * 67    99/06/21 15:14 Tanaka
 * 自販機アダプタのARC IDを定義
 * 
 * 66    99/06/18 17:24 Tanaka
 * 貯玉履歴定数の間違いを修正
 * 
 * 65    99/06/18 16:24 Tanaka
 * 貯玉履歴構造体を追加
 * 
 * 64    99/06/18 13:59 Tanaka
 * チケットプリンタ集計ファイルを32×48バイトに拡張
 * 
 * 63    99/06/18 10:55 Tanaka
 * 
 * 62    99/06/18 10:39 Tanaka
 * 貯玉関連の履歴定数を追加
 * 
 * 61    99/06/18 0:25 Tanaka
 * 99/6/17作業終了分
 * 
 * 60    99/06/14 9:04 Fukuoka
 * 貯玉日報、自販機印字用ボタン対応
 * 
 * 59    99/06/14 3:20 Tanaka
 * 貯玉対応(デバッグは不十分)
 * 
 * 58    99/06/11 9:44 Tanaka
 * change_level, tokushu_select[]を追加
 * 
 * 57    99/06/10 16:32 Nao
 * 
 * 55    99/06/10 13:36 Tanaka
 * seg_levelワークを追加
 * 
 * 54    99/06/10 11:49 Nao
 * dmy1 -> dmy1[12]
 * 
 * 49    99/06/08 13:15 Fukuoka
 * 貯玉日報構造体追加
 * 
 * 48    99/06/07 18:52 Tanaka
 * PMF_SNIPPOUファイル追加
 * 
 * 47    99/06/07 18:31 Tanaka
 * 貯玉中間版
 * 
 * 46    99/06/07 15:12 Fukuoka
 * ７セグ設定ファイル構造体追加
 * 
 * 45    99/05/14 18:34 Tanaka
 * 交換POS存在エラーコードを追加
 * 
 * 44    99/04/16 23:00 Tanaka
 * 統計データの保留券部の意味づけを変更
 * 
 * 43    99/04/16 21:55 Tanaka
 * 在庫構造体pos_zaiko_tは、signed longとした
 * 
 * 42    99/04/16 21:00 Tanaka
 * pos_ticket_tにエース電研用のフィールド(パスワード、発券日付)格納
 * 
 * 41    99/04/16 14:57 Tanaka
 * 払出機グローバル変数pos_haraiを追加
 * 
 * 40    99/04/16 11:35 Tanaka
 * POS-PC(CA)のARCIDを正式決定(0x9e)
 * 
 * 39    99/04/16 1:14 Tanaka
 * pos_total_flagの追加
 * 
 * 38    99/04/13 23:44 Tanaka
 * エラーコード ERR_INT_FILE を追加
 * 
 * 37    99/04/09 11:44 Tanaka
 * 交換仕様の変更にあわせ、追加・削除
 * 
 * 36    99/04/02 19:20 Tanaka
 * tokushu_itemsの追加
 * 
 * 35    99/04/02 17:53 Tanaka
 * ERR_HORYU_PAS定数を追加
 * 
 * 34    99/04/02 14:51 Tanaka
 * エラーコードを新規追加
 * 
 * 33    99/03/31 23:44 Tanaka
 * 
 * 32    99/03/30 21:01 Tanaka
 * 一般景品10000件
 * 
 * 31    99/03/29 9:58 Tanaka
 * メーカーコードの追加(HALSYSTEM,ACEDENKEN)
 * 
 * 30    99/03/25 17:43 Tanaka
 * change_ronly_flagの導入
 * 
 * 29    99/03/25 10:50 Tanaka
 * pos_item_tその他のマスターインデックス対応、SUM_MAX定数追加
 * 
 * 28    99/03/15 20:46 Tanaka
 * 余り玉区分定数を追加
 * 
 * 27    99/03/12 19:59 Tanaka
 * 構造体のアラインメントを２バイトに調整
 * 
 * 26    99/03/12 15:30 Tanaka
 * PMF_ZAIKOSUMファイルを追加
 * 
 * 25    99/03/11 19:17 Tanaka
 * 日計印字定数が不正に定義されていたので修正
 * 
 * 24    99/03/11 18:50 Tanaka
 * 不要な旧形式ワークの削除
 * 
 * 23    99/03/11 14:07 Tanaka
 * 旧定数削除
 * 
 * 21    99/03/08 2:13 Tanaka
 * PRN_JOB_NIKKEI定数を追加
 * 
 * 20    99/03/08 1:51 Tanaka
 * 履歴の一部定数を変更(GRIREKI_DEPOSIT→GRIREKI_GET)
 * 
 * 19    99/03/06 23:11 Tanaka
 * 3/4版xlsファイルに対応、大幅書き換え
 * 
 * 18    99/03/04 19:21 Tanaka
 * 保留券履歴を変更(3/3版xlsファイルに基づく)
 * 
 * 17    99/03/04 11:25 Tanaka
 * 印字定数PRN_JOB_HORYU追加
 * 
 * 16    99/03/03 14:49 Tanaka
 * 印字ジョブ定数追加
 * 
 * 15    99/03/02 22:07 Tanaka
 * 履歴定数、その他追加
 * 
 * 14    99/02/24 13:58 Tanaka
 * pos_tool.hに含まれていたローカル構造体を移した
 * 
 * 13    99/02/22 19:15 Matsuura
 * 追加画面に併せてワーク追加
 * 
 * 12    99/02/21 23:35 Tanaka
 * 交換関連の定数追加
 * 
 * 11    99/02/19 20:14 Tanaka
 * pos_group_dat_tに、親POSの状態管理用ワークを持たせた
 * 
 * 10    99/02/19 14:26 Tanaka
 * ネットワークグループ管理で、各機器ステータス管理テーブルを追加
 * (親POSが使用する)
 * 
 * 9     99/02/16 20:40 Tanaka
 * pos_offline_flagを追加
 * 
 * 8     99/02/16 17:14 Tanaka
 * フィラー通信に合致したバージョン情報、フラグを追加
 * 
 * 7     99/02/16 14:51 Tanaka
 * pos状態フラグ(pos_???_flag)を追加
 * 
 * 6     99/02/09 18:45 Tanaka
 * 外部機器の商品登録、pos_boot_flagを追加
 * 
 * 5     99/02/08 14:29 Tanaka
 * ファイルクリア要求フラグを削除
 * 
 * 4     99/02/05 22:55 Tanaka
 * 990203版xlsファイルにあわせて大幅変更
 */

#ifndef _pos_work_h_
#define _pos_work_h_

//#include "typedef.h"		yumipos del
//#include "time_68k.h"		yumipos del

/*---[ 定数定義 ]------------------------------------------------------------*/

/* 接続台数など */
#define POS_MAX			8				/* POSの個数 */
#define TP_MAX			32				/* チケットプリンタの個数 */
#define HARAI_MAX		8				/* 払出機の個数 */
#define EXTDEV_MAX		8				/* 外部接続機器の個数 */
#define CARDINQ_MAX		8				/* カード照合機の個数 */
#define JIHANA_MAX		8				/* 自販機アダプターの個数 */
#define V_CDS_MAX		8				/* 自販機カード照合機 */
#define SRIF_MAX        8				/* 島端リプレイ機 */

/* 品数など */
#define TOKUSHU_MAX		4				/* 特殊景品個数 */
#define IPPAN_MAX		10000			/* 一般景品個数 */
#define ITEM_MAX		IPPAN_MAX+TOKUSHU_MAX	/* 商品マスタの最大数 */
#define EXT_ITEM_MAX	36				/* 外部機器商品個数 */
#define BUMON_MAX		256				/* 部門数 */
#define ETC_MAX			10				/* その他数 */
#define CONST_MAX		5				/* 定数設定の最大数 */
#define CONSTSET_MAX	5				/* 定数組み合わせ設定の最大数 */
#define CHANGE_MAX		40				/* 一度の交換処理で交換可能な品数 */
#define SUM_MAX			2000			/* 集計処理で扱う最大有効商品数 */
#define CLOSE_ITEM_MAX	128				/* 中締め設定登録最大数 */
#define BACK_ITEM_MAX		40			/* 最大景品選択数(返品、戻し制限) */
#define ITEM_RECERVE_MAX		9			/* １度の通信の最大取得景品データ */

/* 操作 */
#define ADMIN_MAX		30				/* 責任者最大人数 */
#define ADMIN_LEVEL_MAX	30				/* 責任者レベル分け最大数 */
#define KEY_ITEM_MAX	64				/* 商品ダイレクトキーの個数 */
#define ADMIN_REC_MAX	5				/* 責任者別統計管理・ループ数 */
#define REC_FILE_MAX	9				/* 記憶情報ファイルの記憶日数 */
#define TICKET_NUM_MAX	9				/* ticket_num最大数 */
#define MEMBER_HISTORY_MAX 512			/* 会員履歴ファイル最大レコード数 */

/* 文字列長さ */
#define HALL_NAME_LEN	14				/* ホールの名称長さ(\0なし) */
#define TYPE_NAME_LEN	2					/* 各種別の表示文字列(\0なし) */
#define ITEM_NAME_LEN	24				/* 一般・特殊景品の名称長さ(\0なし) */
#define BUMON_NAME_LEN	24				/* 部門の名称長さ(\0なし) */
#define EXTDEV_NAME_LEN	8				/* 外部接続機器の名称長さ(\0なし) */
#define PAGE_NAME_LEN	20				/* タッチパネルページ名(\0なし) */
#define ADMIN_NAME_LEN	24				/* 責任者氏名長さ(\0なし) */

/* バーコード長さ */
#define BC_BIN_LEN		8				/* バーコード長(圧縮形式、\0なし) */
#define BC_DEC_LEN		16				/* バーコード長(通常形式、\0なし) */
#define BC_TOTAL_LEN	64				/* バーコード長(最大受信長、\0あり */
#define BC_HEAD_LEN		16				/* バーコード長(ヘッダ部分、\0あり */
#define BC_CODE_LEN		25				/* バーコード長(コード部分、\0あり */

/* PAS種別 */
#define PAS_P			0x01			/* 1種別目選択 */
#define PAS_A			0x02			/* 2種別目選択 */
#define PAS_S			0x04			/* 3種別目選択 */
#define PAS_EX		0x08			/* 4種別目選択 */

#define JRL_A_PAS_P			0x01			/* A店のJRL、レシートで使用する1種別目 */
#define JRL_A_PAS_A			0x02			/* A店のJRL、レシートで使用する2種別目 */
#define JRL_A_PAS_S			0x03			/* A店のJRL、レシートで使用する3種別目 */
#define JRL_A_PAS_EX		0x04			/* A店のJRL、レシートで使用する4種別目 */
#define JRL_B_PAS_P			0x05			/* B店のJRL、レシートで使用する1種別目 */
#define JRL_B_PAS_A			0x06			/* B店のJRL、レシートで使用する2種別目 */
#define JRL_B_PAS_S			0x07			/* B店のJRL、レシートで使用する3種別目 */
#define JRL_B_PAS_EX		0x08			/* B店のJRL、レシートで使用する4種別目 */

#define	PAS_MAX			3				/* 全部で３種類(現在は4種別) */

/* 三共払い出し機レーン設定 */
#define	LANE_MAX		8				/* 最大レーン設定数 */

/* 余り玉区分 */
#define REST_NONE		0				/* 余り玉処理:終了不可能 */
#define REST_GET		1				/* 余り玉処理:取り込み */
#define REST_SERVICE	2				/* 余り玉処理:サービス */
#define REST_HORYU		3				/* 余り玉処理:保留券 */
#define REST_SAVINGS	4				/* 余り玉処理:貯玉 */
#define REST_RETURN		5				/* 余り玉処理:戻し玉 */
#define REST_PICKUP		6				/* 余り玉処理:拾い玉 */

/* 税 */
#define TAX_INT			1				/* 消費税 内税 */
#define TAX_EXT			2				/* 消費税 外税 */

/* 集計区分 */
#define SUMAREA_NONE	0				/* 無し */
#define SUMAREA_POS		1				/* POS別集計 */
#define SUMAREA_COUNTER	2				/* カウンタ別集計 */
#define SUMAREA_FLOOR	3				/* フロア別集計 */
#define SUMAREA_ALL		4				/* 全店集計 */

/* 集計印刷区分 */
#define SUMTYPE_NONE	0				/* 無し */
#define SUMTYPE_TOUKEI	1				/* 統計情報の印刷 */
#define SUMTYPE_BUMON	2				/* 部門別景品の印刷 */
#define SUMTYPE_TOP30	3				/* 景品交換TOP30品目の印刷 */
#define SUMTYPE_TODAY	4				/* 当日景品交換全印刷 */
#define SUMTYPE_NAKA	5				/* 指定景品中締め印刷 */
#define SUMTYPE_BUMON_APP 6				/* 指定部門集計印刷 */
#define SUMTYPE_ALLITEMS  7				/* 指定部門集計印刷 */
#define SUMTYPE_SKIP_BUMON  8				/* 指定部門集計印刷 */

/* 印字ジョブ */
#define PRN_JOB_RECEIPT	0x00			/* レシート印字 */
#define PRN_JOB_HORYU	0x01			/* 保留券印字 */
#define PRN_JOB_NIKKEI	0x02			/* 日計印字 */
#define PRN_JOB_HRIREKI	0x03			/* 保留券履歴印字 */
#define PRN_JOB_CYDMNIP 0x04			/* 貯玉日報印字 */
#define PRN_JOB_JIHANKI 0x05			/* 自販機印字 */
#define PRN_JOB_KTERM	0x06			/* 会員端末印字 */
#define PRN_JOB_ZAIKO	0x07			/* 在庫情報印字 */
#define PRN_JOB_ZAIKO_SHORI	0x08		/* 在庫処理情報印字 */
#define PRN_JOB_ZAIKO_TITLE	0x09		/* 在庫処理ﾀｲﾄﾙ印字 */
#define PRN_JOB_ERROR_LOG	0x0a		/* エラー処理履歴印字 */
#define PRN_JOB_MARS_ERROR	0x0b		/* エラー処理履歴印字 */
#define PRN_JOB_CONFIG	0x0c			/* ＰＯＳ設定印字 */
#define PRN_JOB_SAVINGS 0x0d			/* 貯玉処理エラー印字 */

/* ネットワーク */
#define POS_ARCMIN		0x90			/* POSの最小ARC ID */
#define TP_ARCMIN		0x70			/* TPの最小ARC ID */
#define ACE_ARCID		200				/* エース電研集中管理アダプタARC ID */
#define CA_ARCID		158				/* CAのARC ID */
#define KS_ARCID		0xf9			/* KSのARC ID */
#define VENDING_ARCID	0x40			/* 自販機アダプタARC ID(↓へ移行) */
#define JIHANA_ARCMIN	0x40			/* 自販機アダプタの最小ARC ID */
#define SRIF_ARCMIN		0x20			/* SRIFの最小ARC ID */
#define VCD_ARCMIN		0xb0			/* VCDの最小ARC ID */
#define CDS_ARCMIN		0xa0			/* CDSの最小ARC ID */

/* メーカーコード */
#define HALSYSTEM_ID	1				/* ハルシステム */
#define ACEDENKEN_ID	2				/* エース電研 */


/* 払い出し機メーカーコード */
#define	HARAI_NONE_ID		0				/* 払い出し機未使用 */
#define	HARAI_GLORY_ID		1				/* グローリー製 */
#define	HARAI_SANKYO_ID		2				/* 三共製 */
#define	HARAI_TMI_ID		3				/* TMI製 */
#define	HARAI_SUBARU_ID		4				/* スバル製 */
#define HARAI_MARS_ID		5				/* マース製 */
#define HARAI_HIROTAC_ID	6				/* ヒロタック製 */
#define	HARAI_NO_USE_ID		HARAI_NONE_ID	/* alias */

#define PRICE_DEC_BAS	10				/* 景品単価小数点対応基準値
											1:対応なし(通常どおり)
											10:小数点以下１桁まで対応
											100:小数点以下２桁まで対応
										 */

/* 総合履歴 種別(type1, type2, 詳細区分) */
#define GRIREKI_SERIAL_MAX	3			/* 閉店時の履歴のシリアル番号を保存する期間 */
#define GRIREKI_CHANGE	0x0001			/* 通常交換 */
	#define GRIREKI_GET		0x0001			/* 預かり */
		#define GRIREKI_GTICKET	0x0001			/* チケット預かり */
		#define GRIREKI_GTENYU	0x0002			/* 手入力預かり */
		#define GRIREKI_GHORYU	0x0004			/* 保留券預かり */
		#define GRIREKI_GSAVINGS 0x0008			/* 貯玉預かり */
	#define GRIREKI_IPPAN	0x0002			/* 一般景品払い出し */
	#define GRIREKI_TOKUSHU	0x0004			/* 特殊景品払い出し */
	#define GRIREKI_REST	0x0008			/* 余り玉 */
		#define GRIREKI_ROVER	0x0001			/* 余り玉を小口景品 */
		#define GRIREKI_RGET	0x0002			/* 余り玉を取り込み */
		#define GRIREKI_RSERVICE 0x0004			/* サービス玉 */
		#define GRIREKI_RRETURN	 0x0008			/* 戻し(景品) */
		#define GRIREKI_RPICKUP	 0x0010			/* 拾い玉 */
		#define GRIREKI_RHORYU	 0x0020			/* 保留券発券 */
	#define GRIREKI_MIPPAN	0x0010			/* 一般景品返品 */
		#define GRIREKI_MHENPIN	 0x0001			/* 返品 */
		#define GRIREKI_MRETURN	 0x0002			/* 戻し(景品) */
	#define GRIREKI_MTOKUSHU 0x0020			/* 特殊景品返品 */
		#define GRIREKI_THENPIN	 0x0001			/* 返品 */
		#define GRIREKI_TRETURN	 0x0002			/* 戻し(景品) */
	#define GRIREKI_SAVINGS	0x0040			/* 貯玉払い出し */
	#define GRIREKI_SAVINGSDAT	0x0060		/* 貯玉遷移 */
	#define GRIREKI_MARSOK	0x0080			/* マース払出し */
	#define GRIREKI_MARSNG	0x0081			/* マース未払い */
	#define GRIREKI_CHGERR	0x8000			/* エラー */
		#define GRIREKI_ETCK	0x0001			/* チケット処理エラー */
		#define GRIREKI_ETYO	0x0002			/* 貯玉処理エラー */
		#define GRIREKI_EHARAI	0x0004			/* 払出し処理エラー */
		#define GRIREKI_EOTHER	0x0008			/* その他エラー */

#define GRIREKI_UNYOU	0x0002			/* 運用処理 */
	#define GRIREKI_OPEN	0x0001			/* 営業開始処理（開店処理） */
	#define GRIREKI_START	0x0002			/* 営業開始処理（開店処理なし） */
	#define GRIREKI_HSTART	0x0003			/* 発券開始処理 */
	#define GRIREKI_HSTOP	0x0004			/* 発券停止処理 */
	#define GRIREKI_CLOSE	0x0005			/* 閉店処理 */
	#define GRIREKI_RECORD	0x0006			/* 手動データ記憶 */
	#define GRIREKI_CLEAR	0x0007			/* 手動データクリア */
	#define GRIREKI_OPENCLR	0x0008			/* OPEN前クリア処理 */
	#define GRIREKI_TESTCHG	0x0009			/* テスト交換開始操作 */
	#define GRIREKI_MSTPC	0x000a			/* POSPC景品マスタ受信(POSPC主動) */
	#define GRIREKI_MSTPOS	0x000b			/* POSPC景品マスタ受信(POS主動) */
	#define GRIREKI_AMSTPC	0x000c			/* POSPC責任者マスタ受信(POSPC主動) */
	#define GRIREKI_AMSTPOS	0x000d			/* POSPC責任者マスタ受信(POS主動) */
	#define GRIREKI_UNYERR	0x8000			/* エラー処理 */
		#define GRIREKI_EDATE	0x0001			/* 営業日付取得処理エラー */
		#define GRIREKI_EJIHANA 0x0002			/* 自販機データ取得エラー */
		#define GRIREKI_UPZAIK	0x0003			/* 在庫データ更新エラー */
		#define GRIREKI_FCLEAR	0x0004			/* ファイルクリアエラー */
		#define GRIREKI_JCLEAR	0x0005			/* 自販機アダプタへのデータクリア指示エラー */
		#define GRIREKI_MARSCL	0x0006			/* マースクリア処理エラー */
		#define GRIREKI_MARSCA	0x0007			/* マースカレンダ設定エラー */
		#define GRIREKI_TPCLEAR 0x0008			/* TPクリア処理エラー */
		#define GRIREKI_TPSTOP	0x004c			/* TP発券状態エラー */
		#define GRIREKI_TSUMINT	0x0040			/* PMF_TOUKEISUM初期化エラー */
		#define GRIREKI_TZIKINT	0x0041			/* PMF_TZAIKOSUM初期化エラー */
		#define GRIREKI_PTOUKEI	0x0042			/* POS統計データ集計エラー */
		#define GRIREKI_PZAIKO	0x0043			/* POS在庫データ集計エラー */
		#define GRIREKI_TPFILE	0x0044			/* TPファイル集計エラー */
		#define GRIREKI_CHONIP	0x0045			/* 貯玉日報データ取得エラー */
		#define GRIREKI_JIHFILE	0x0046			/* 自販機ファイル集計エラー */
		#define GRIREKI_CACOMM 	0x0047			/* CA通信エラー */
		#define GRIREKI_CA10MIN	0x0048			/* CA10分タイムアウト */
		#define GRIREKI_PCCOMM	0x0049			/* POSPC通信エラー */
		#define GRIREKI_PC10MIN	0x004a			/* POSPC10分タイムアウト */
		#define GRIREKI_PCERR	0x004b			/* POSPCエラー情報取得処理エラー */
		#define GRIREKI_HIROTAC	0x004d			/* ヒロタック集計エラー */
		#define GRIREKI_MSTCOMM	0x0080			/* POSPC通信エラー */
		#define GRIREKI_MST2S	0x0081			/* POSPC応答待ち(2秒)タイムアウトエラー */
		#define GRIREKI_MST10M	0x0082			/* POSPC応答待ち(10分)タイムアウトエラー */
		#define GRIREKI_MSTNAK	0x0083			/* POSPCNAK受信エラー */
		#define GRIREKI_MSTNG	0x0084			/* 処理結果NG */
		#define GRIREKI_MSTRECV	0x0085			/* POSPC主動のマスタ受信処理エラー */

#define GRIREKI_ZAIKO	0x0003			/* 在庫管理処理 */
	#define GRIREKI_STORE	0x0001			/* 景品入庫処理 */
	#define GRIREKI_MOVE	0x0002			/* 在庫移動処理 */
	#define GRIREKI_STOCK	0x0003			/* 景品棚卸処理(各POS) */
	#define GRIREKI_STCKALL	0x0004			/* 景品棚卸処理(全店) */
	#define GRIREKI_ADJUST	0x0005			/* 在庫調整処理 */
	#define GRIREKI_ZAIKERR	0x8000			/* エラー処理 */
		#define GRIREKI_NOTITEM	0x0001			/* 商品未選択 */
		#define GRIREKI_NOTNUM	0x0002			/* 処理個数未入力 */
		#define GRIREKI_MINUS	0x0003			/* マイナス入庫 */
		#define GRIREKI_SRCPOS	0x0004			/* 移動元POSエラー */
		#define GRIREKI_SRCNUM	0x0005			/* 移動元個数範囲エラー */
		#define GRIREKI_ZCOMM	0x0006			/* 通信エラー */
		#define GRIREKI_DATA	0x0007			/* データ未照会エラー */
		#define GRIREKI_CHGING	0x0008			/* 移動元POS交換処理中エラー */
		#define GRIREKI_ZWDAT	0x0009			/* 在庫データ書込み失敗 */
		#define GRIREKI_TANAD	0x000a			/* 棚卸画面表示中エラー */
		#define GRIREKI_ATANAD	0x000b			/* 全店棚卸画面表示中エラー */
		#define GRIREKI_ZGDAT	0x000c			/* 全店在庫データ取得失敗(全店棚卸集計通信エラー) */
		#define GRIREKI_MASTERR	0x000d			/* 全店在庫マスタエラー(全店棚卸集計マスタエラー) */

#define GRIREKI_KAIIN	0x0004			/* 会員端末処理 */
	#define GRIREKI_KADD	0x0001			/* 会員追加 */
	#define GRIREKI_KDEL	0x0002			/* 会員削除 */
	#define GRIREKI_PASSCLR	0x0003			/* 暗証クリア */
	#define GRIREKI_KMOVE	0x0004			/* 会員移行 */
	#define GRIREKI_KCHG	0x0005			/* 会員状態変更 */
	#define GRIREKI_KERR	0x8000			/* 会員処理エラー */

#define GRIREKI_CONFIG	0x0005			/* 設定処理 */
	#define GRIREKI_EXCHG	0x0001			/* 交換定数設定 */
	#define GRIREKI_TIMEDEF	0x0002			/* 時刻設定 */
	#define GRIREKI_SRIF	0x0003			/* 島端I/F設定 */
	#define GRIREKI_CFGERR	0x8000			/* エラー処理 */
		#define GRIREKI_WDAT	0x0001				/* ファイルへのデータ書込み処理失敗 */
		#define GRIREKI_SRIFCOM	0x0002				/* SRIF通信エラー */
		#define GRIREKI_SRIFTO	0x0003				/* SRIF応答待ちタイムアウト */

#define GRIREKI_CARD	0x0006			/* カード処理 */
	#define GRIREKI_CARDERR	0x8000			/* カード処理エラー */
		#define GRIREKI_CALIB	0x0001				/* 運用画面以外でのキャリブレーションカード挿入 */
		#define GRIREKI_TEST	0x0002				/* 営業中のテスト交換カード挿入 */
		#define GRIREKI_CANNOT	0x0003				/* その他の使用出来ない設定カード */
		#define GRIREKI_NENTRY	0x0004				/* 未登録責任者カード */
		#define GRIREKI_COMCODE	0x0005				/* 会社コードエラー */
		#define GRIREKI_ADCARD	0x0006				/* 異なる責任者カード */

/* 在庫管理パターン */
#define NON_ZAIK_CTRL	0x00				/* 在庫管理なし */
#define ALL_ZAIK_CTRL	0x01				/* 全店在庫管理 */
#define IND_ZAIK_CTRL	0x02				/* 各POS在庫管理 */
#define TDY_ZAIK_CTRL	0x03				/* 当日在庫管理 */


/* その他 */

/* エラーコード */
#define ERR_HORYU_INFO		-10				/* 保留券情報エラー(※未使用) */
#define ERR_HORYU_NFND		-11				/* 保留券該当なしエラー */
#define ERR_HORYU_ALRDY		-12				/* 保留券使用済みエラー */
#define ERR_HORYU_COMM		-13				/* 保留券通信エラー */
#define ERR_HORYU_PAS		-14				/* 保留券PAS違いエラー */
#define	ERR_HORYU_PRINT		-15				/* 保留券印刷エラー(※未使用) */
#define ERR_HORYU_DATA		-16				/* 保留券該当なしエラー(内部異常) */
#define ERR_HDAT_SERIAL		-121			/* 保留券シリアル番号不一致 */
#define ERR_HDAT_REC		-122			/* 保留券発券履歴なし */
#define ERR_HDAT_PAS		-123			/* 保留券PASエラー */
#define ERR_HDAT_NUM		-124			/* 保留券預かり玉数エラー */
#define ERR_HDAT_CODE		-125			/* 保留券データ異常 */
#define ERR_HORYU_TYPE		-126			/* 保留券種別データ異常 */
#define ERR_HORYU_POSNO		-127			/* 保留券ＰＯＳ番号異常 */
#define ERR_HREC_PAS		-128			/* 保留券発券履歴なし */
#define ERR_HREC_NUM		-129			/* 保留券PASエラー */
#define ERR_HORYU_PASTYPE	-130			/* 保留券PASエラー */
#define ERR_HORYU_SERIAL	-131			/* 保留券シリアル番号が０以下 */
#define ERR_INT_FILE		-20				/* ファイルアクセスエラー */
#define ERR_INT_FULL		-21				/* バッファフルエラー(※未使用) */
#define ERR_INT_NFND		-22				/* バッファ該当なしエラー(※未使用) */
#define ERR_INT_COMM		-23				/* 通信失敗エラー */
#define ERR_CHG_9OVER		-30				/* 預かり９枚越えエラー */
#define ERR_CHG_NOINPUT		-31				/* 預かりなし選択エラー */
#define ERR_CHG_NORIREKI	-32				/* 履歴なしレシートエラー */
#define ERR_CHG_RESTNONE	-33				/* 余り玉終了エラー */
#define ERR_CHG_NOHORYU		-34				/* 余り玉保留エラー */
#define ERR_CHG_CHANGING	-35				/* 交換中エラー */
#define ERR_CHG_CHGMAX		-36				/* スロットフルエラー */
#define ERR_CHG_ITEMNFND	-37				/* 該当景品なしエラー */
#define ERR_CHG_NOTENOUGH	-38				/* 玉不足エラー */
#define ERR_CHG_RENUM		-39				/* 数量変更エラー */
#define ERR_CHG_NOPAS		-40				/* PAS未確定エラー */
#define ERR_CHG_DELETE		-41				/* 削除スロットなしエラー */
#define ERR_CHG_RECEIPT		-42				/* レシート範囲オーバーエラー */
#define ERR_CHG_CANTALL		-43				/* 直前操作!=選択エラー */
#define ERR_CHG_KAKUTEI		-44				/* 確定後の預玉入力エラー */
#define ERR_CHG_KAKUCAN		-45				/* 貯玉操作後の確定取消エラー */
#define ERR_CHG_USERCAN		-46				/* お客様によるキャンセル */
#define ERR_CHG_KSERR		-47				/* ＫＳとの通信エラー */
#define ERR_CHG_KAKUERR		-48				/* 確定エラー */
#define ERR_CHG_TENYU		-49				/* 手入力操作制限エラー(※未使用) */
#define	ERR_CHG_SAVING		-50				/* 照合機書込みエラー */
#define ERR_CHG_CARD		-51				/* カード照合機通信エラー */
#define ERR_CHG_LICENSE		-87				/* 操作制限許可カードエラー */
#define ERR_CHG_RETURN		-88				/* 戻し玉処理操作制限エラー */
#define ERR_CHG_PICKUP		-89				/* 拾い玉処理操作制限エラー */
#define ERR_CHG_MODOSHI		-94				/* 戻し処理操作制限エラー */
#define ERR_CHG_ITEMPRICE	-95				/* 景品単価エラー */
#define ERR_CHG_ITEMPRICE2	-103		/* 景品単価エラー(端数エラー) */
#define ERR_CHG_ITEMDATE	-96				/* 景品日付エラー */
#define ERR_CHG_LIM_BACK	-98				/* 返品制限エラー */
#define ERR_CHG_LIM_RETURN	-99			/* 戻し制限エラー */
#define ERR_TICKET_ILLE		-60				/* チケット情報エラー */
#define ERR_TICKET_NFND		-61				/* チケット問い合わせ情報なしエラー */
#define ERR_TICKET_ALRDY	-62				/* チケット使用済みエラー */
#define ERR_TICKET_COMM		-63				/* チケット通信エラー */
#define ERR_TICKET_PAS		-64				/* チケットPAS違いエラー */
#define ERR_TICKET_OFFLINE	-65				/* チケットオフライン警告 */
#define ERR_TICKET_ONLINE   -109			/* チケットオンライン区分異常(エース) */
#define ERR_TICKET_PAPER	-66				/* チケット紙切れエラー */
#define ERR_TICKET_READ		-67				/* 連続キー未押下エラー */
#define ERR_INPUT_CODE		-68				/* 手入力によるチケットコードエラー */
#define ERR_TICKET_DATA		-69				/* チケット バーコードデータ異常 */
#define ERR_TDAT_PAS		-111			/* チケットバーコードPAS違い */
#define ERR_TDAT_CODE		-112			/* チケットバーコードデータ異常 */
#define ERR_TDAT_TP			-113			/* チケットバーコードTP番号異常 */
#define ERR_TDAT_DATE		-114			/* チケットバーコード日付異常 */
#define ERR_TICKET_TYPE		-115			/* チケット種別データ異常 */
#define ERR_TDAT_NUM		-116			/* チケット玉数異常 */
#define ERR_TICKET_PASS		-110			/* パスワード異常 */
#define ERR_TICKET_BARHED	-117			/* バーコードヘッダ異常 */
#define ERR_TICKET_PASTYPE	-118			/* バーコードヘッダ異常 */
#define ERR_TICKET_ERROR	-119			/* 発券機種異常 */
#define ERR_TICKET_SERIAL	-120			/* チケットシリアル番号が範囲外 */
#define ERR_HORYU_NONPOS	-140			/* POS未接続 */
#define ERR_TICKET_NONTP	-141			/* TP未接続 */
#define ERR_HARAI_COMM		-70				/* 払出機通信エラー */
#define ERR_HARAI_ERROR		-71				/* 払出機払出エラー */
#define ERR_HARAI_CHECK		-72				/* 払出機払出エラー(点検) */
#define ERR_HARAI_END		-73				/* 払出機払出エラー(17H-18H通信中断) */
#define ERR_NET_CHANGE		-80				/* ネットワーク内で交換業務中 */
#define ERR_CHG_CANCELN		-81				/* 通信エラーなどによる貯玉処理の中断 */
#define ERR_CHG_CANCEL2		-82				/* 通信エラーなどによる貯玉処理の中断 */
#define ERR_CHG_CANCEL3		-83				/* 通信エラーなどによる貯玉処理の中断 */
#define ERR_CHG_CANCEL4		-84				/* 通信エラーなどによる貯玉処理の中断 */
#define ERR_CHG_CANCEL5		-85				/* 通信エラーなどによる貯玉処理の中断 */
#define ERR_CHG_CANCEL		-86
#define ERR_MEMBER_KSERR	-101			/* 会員処理ＫＳとの通信エラー */
#define ERR_MEMBER_CAERR	-102			/* 会員処理ＣＡとの通信エラー */
#define ERR_CD_NONE			-90				/* カード未挿入 */
#define ERR_CD_PASSIN		-91				/* 照合機パスワード入力中 */
#define ERR_CD_PASS_NO		-92				/* 照合機パスワード未入力 */
#define ERR_CD_PASS_OK		-93				/* 照合機パスワード入力済み */
#define ERR_FAIL_TRANS		-150			/* 送信エラー */
#define ERR_CHG_DENY		-151			/* 送信エラー */
#define ERR_RETRY_OVER		-97				/* 送信エラー */
#define ERR_CHANGE_CLOSE	-100			/* 全ての接続機器の停止が行えませんでした */

/* 貯玉処理関連(01/09～) */
#define E_CARDIN_TIMEOUT	-160			/* KSｶｰﾄﾞｲﾝ通信：ﾀｲﾑｱｳﾄ */
#define E_CARDIN_INVALID	-161			/* KSｶｰﾄﾞｲﾝ通信：無効会員 */
#define E_CARDIN_NOEXIST	-162			/* KSｶｰﾄﾞｲﾝ通信：会員存在しない */
#define E_CARDIN_LOCK		-163			/* KSｶｰﾄﾞｲﾝ通信：ﾛｯｸ中 */
#define E_CARDIN_REPUB		-164			/* KSｶｰﾄﾞｲﾝ通信：再発行ｶｰﾄﾞ */
#define E_POINT_TIMEOUT		-165			/* KSﾎﾟｲﾝﾄｱｯﾌﾟ通信：ﾀｲﾑｱｳﾄ */
#define E_POINT_OVER		-166			/* KSﾎﾟｲﾝﾄｱｯﾌﾟ通信：預かりﾎﾟｲﾝﾄ数超過 */
#define E_PASS_TIMEOUT		-167			/* KS暗証登録通信：ﾀｲﾑｱｳﾄ */
#define E_PASS_KSERR		-168			/* KS暗証登録通信：KSからｴﾗｰｺｰﾄﾞ */
#define E_PASS_TIMEOUT_CA	-189			/* CA暗証登録通信：ﾀｲﾑｱｳﾄ */
#define E_PASS_KSERR_CA		-190			/* CA暗証登録通信：KSからｴﾗｰｺｰﾄﾞ */
#define E_TYOIN_TIMEOUT		-169			/* KS貯玉(預入)通信：ﾀｲﾑｱｳﾄ */
#define E_TYOIN_OVER		-170			/* KS貯玉(預入)通信：最大値ｵｰﾊﾞｰ */
#define E_TYOIN_KSERR		-171			/* KS貯玉(預入)通信：KSからｴﾗｰｺｰﾄﾞ */
#define E_TYOOUT_TIMEOUT	-172			/* KS貯玉(引出)通信：ﾀｲﾑｱｳﾄ */
#define E_TYOOUT_KSERR		-173			/* KS貯玉(引出)通信：KSからｴﾗｰｺｰﾄﾞ */
#define E_CARDOUT_TIMEOUT	-174			/* KSｶｰﾄﾞｱｳﾄ通信：ﾀｲﾑｱｳﾄ */
#define E_FORCE_BUTTON_Y	-175			/* CDS会員ｶｰﾄﾞ強制排出：ｶｰﾄﾞ排出ﾎﾞﾀﾝ */
#define E_FORCE_BUTTON_N	-185
#define E_FORCE_POFF		-176			/* CDS会員ｶｰﾄﾞ強制排出：CDS電源断 */
#define E_ERR_CARDIN		-177			/* CDSに会員ｶｰﾄﾞ以外のｶｰﾄﾞ挿入 */
#define E_READ_DATA			-178			/* CDS会員ｶｰﾄﾞ読取り失敗 */
#define E_CARD_EJECT		-179			/* CDSｶｰﾄﾞ詰まり */
#define E_POLLING_REPLY		-180			/* CDSﾎﾟｰﾘﾝｸﾞ通信無応答ｴﾗｰ */
#define E_POLLING_REPLY2	-186			/* CDSﾎﾟｰﾘﾝｸﾞ通信無応答ｴﾗｰ */
#define E_POLLING_REPLY3	-187			/* CDSﾎﾟｰﾘﾝｸﾞ通信無応答ｴﾗｰ */
#define E_PULL_OUT			-181			/* CDS会員ｶｰﾄﾞ引抜き */
#define E_PULL_OUT2			-188			/* CDS会員ｶｰﾄﾞ引抜き(貯玉読込・景品選択あり) */
#define E_CARD_LEFT			-182			/* CDS会員ｶｰﾄﾞ取り忘れ */
#define E_CDS_POWEROFF		-183			/* CDSが突然ｶｰﾄﾞｱｳﾄ状態になった */
#define E_CONNECTCFG_CDS	-184			/* 接続機器設定でCDS未接続状態 */

/* 中締め印字範囲、スキップあり/なし */
#define PRINT_SELF	0x00				/* 印字範囲：自POS */
#define PRINT_ALL 	0x01				/* 印字範囲：全店 */
#define SKIP_OFF	 	0x00				/* スキップ：なし */
#define SKIP_ON 		0x01				/* スキップ：あり */

/* 端末エラー履歴　エラーコード */
#define ARCNET_DOWN_ERR		1000	/* アークネットダウン */

/*---[ 構造体設定(ファイルと直結しているもの) ]------------------------------*/

/*
 *
 *	構造体定義
 *	動作ファイル
 *
 */

/*
 *	ファイル情報更新
 */
typedef struct {
	DWORD update_date;				/* 更新日付(内部形式) */
	DWORD update_time;				/* 更新時間(内部形式) */
	BYTE update_flag;				/* 更新フラグ(>=0x80で更新あり) */
    BYTE wp_record;					/* 記憶情報前回書き込み位置 */
	BYTE reserve[2];				/* 予備 */
} pos_update_t;

/*
 *	記憶情報
 */
typedef struct {
	DWORD record_date;				/* 記憶日付(内部形式) */
	DWORD record_time;				/* 記憶時間(内部形式) */
	DWORD open_date;				/* 開店日付(内部形式) */
	DWORD open_time;				/* 開店時間(内部形式) */
	DWORD close_date;				/* 閉店日付(内部形式) */
	DWORD close_time;				/* 閉店時間(内部形式) */
    DWORD daily_rireki_serial;		/* 閉店時の履歴数を記憶 */
  /*	BYTE record_C4;					 C4インデックス(未使用) */
  /*	BYTE reserve_c4;				 C4予備 */
  /*	BYTE reserve[32];				 予備 */
} pos_record_t;

/*
 *
 *	構造体定義
 *	設定ファイル
 *
 */

/*
 *	POS基本設定
 */
typedef struct {
	BYTE hall_name[HALL_NAME_LEN];	/* ホール名称(\0なし) */
	BYTE hall_name2[HALL_NAME_LEN];	/* ホール名称(\0なし) */
	BYTE disp_type_a1[TYPE_NAME_LEN];	/* A店種別1の表示文字列(\0なし) */
	BYTE disp_type_a2[TYPE_NAME_LEN];	/* A店種別2の表示文字列(\0なし) */
	BYTE disp_type_a3[TYPE_NAME_LEN];	/* A店種別3の表示文字列(\0なし) */
	BYTE disp_type_a4[TYPE_NAME_LEN];	/* A店種別4の表示文字列(\0なし) */
	BYTE disp_type_b1[TYPE_NAME_LEN];	/* B店種別1の表示文字列(\0なし) */
	BYTE disp_type_b2[TYPE_NAME_LEN];	/* B店種別2の表示文字列(\0なし) */
	BYTE disp_type_b3[TYPE_NAME_LEN];	/* B店種別3の表示文字列(\0なし) */
	BYTE disp_type_b4[TYPE_NAME_LEN];	/* B店種別4の表示文字列(\0なし) */
	WORD disp_left_flag;							/* 客側残り玉表示有無(TRUEで表示) */
	BYTE tenyu_card_flag;							/* カードによる手入力許可(TRUE:有効) */
	BYTE lcd_clear_flag;							/* 交換終了時のLCD消去フラグ(TRUE:消去) */
	WORD tama_kan;										/* 玉換算値（コイン一枚あたりの玉数） */
	BYTE savings_force_flag;					/* 貯玉問い合わせなしフラグ */
	BYTE tokushu_auto_select;					/* 特殊景品自動換算設定(TRUE:有効) */
	BYTE ticket_continue_flag;				/* チケット読み出し連続読み込み確認フラグ(TRUE:有効) */
	BYTE ext_offer_flag;							/* 特殊景品最適化フラグ（TRUE:有効） */
	BYTE card_interlock_flag;					/* CDS カードイン インターロックフラグ(TRUE:有効) */
	BYTE err_ticket_read;							/* エラーチケット強制読み込み */
	BYTE barcode_buzzer;							/* バーコードスキャン音（TRUE:有効） */
	BYTE yesterday_zaiko;							/* 在庫管理設定 0:在庫管理なし 1:全店在庫管理
																			 2:各POS在庫管理 3:本日在庫管理 */
  BYTE change_receipt;							/* 交換時レシート無条件印字（TRUE:有効） */
	BYTE pos_member_ca;								/* CAへの会員処理通知(TRUE:あり、FALSE:なし) */
	WORD pos_member_ca_retry;					/* CAへの会員処理通知リトライ回数 */
	WORD pos_member_ca_timeout;				/* CAへの会員処理通知タイムアウト */
	BYTE horyu_card_flag;							/* カードによる保留券処理許可(TRUE:有効) */
	BYTE return_card_flag;						/* カードによる返品処理許可(TRUE:有効) */
	BYTE codein_card_flag;						/* カードによるコード入力許可(TRUE:有効) */
	BYTE all_nakajime_flag;						/* 全景品中締め印字 0:未使用 1:使用 */
	BYTE admin_operation;							/* 責任者管理機能有効フラグ(TRUE:有効) */
	BYTE return_trans_flag;						/* 返品、戻し制限フラグ */
	WORD com_code;										/* 会社コード(0000～9998) */
} pos_basic_t;


/*
 *	交換定数設定
 */
typedef struct {
	WORD tanka_a1;					/* A店種別1単価(1玉/円) */
	WORD canend_a1;					/* A店種別1終了可能玉 */
	WORD horyu_a1;					/* A店種別1保留券発行可能玉 */
	WORD tanka_a2;					/* A店種別2単価(1玉/円) */
	WORD canend_a2;					/* A店種別2終了可能玉 */
	WORD horyu_a2;					/* A店種別2保留券発行可能玉 */
	WORD tanka_a3;					/* A店種別3単価(1玉/円) */
	WORD canend_a3;					/* A店種別3終了可能玉 */
	WORD horyu_a3;					/* A店種別3保留券発行可能玉 */
	WORD tanka_a4;					/* A店種別4単価(1玉/円) */
	WORD canend_a4;					/* A店種別4終了可能玉 */
	WORD horyu_a4;					/* A店種別4保留券発行可能玉 */
	WORD tanka_b1;					/* B店種別1単価(1玉/円) */
	WORD canend_b1;					/* B店種別1終了可能玉 */
	WORD horyu_b1;					/* B店種別1保留券発行可能玉 */
	WORD tanka_b2;					/* B店種別2単価(1玉/円) */
	WORD canend_b2;					/* B店種別2終了可能玉 */
	WORD horyu_b2;					/* B店種別2保留券発行可能玉 */
	WORD tanka_b3;					/* B店種別3単価(1玉/円) */
	WORD canend_b3;					/* B店種別3終了可能玉 */
	WORD horyu_b3;					/* B店種別3保留券発行可能玉 */
	WORD tanka_b4;					/* B店種別4単価(1玉/円) */
	WORD canend_b4;					/* B店種別4終了可能玉 */
	WORD horyu_b4;					/* B店種別4保留券発行可能玉 */
	DWORD reserve1[3];			/* 予備 */
} pos_const_t;

/*
 *	定数組み合わせ設定
 */
typedef struct {
	DWORD start_time;				/* 開始時間(内部形式) */
	DWORD end_time;					/* 終了時間(内部形式) */
	WORD pattern;					/* パターン(0～CONST_MAX-1) */
	WORD reserve;					/* 予備 */
} pos_constset_t;

/*
 *	計数機(チケットプリンタ)設定
 */
typedef struct {
	BYTE no;						/* 計数機の番号(0～KEISUU_MAX-1) */
	BYTE counter;					/* 設置カウンタ番号(0～) */
	BYTE floor;						/* 設置フロア番号(0～) */
	BYTE tanka;						/* 交換単価 */
	WORD shop;						/* 設置店舗番号(0～) */
	WORD extdev;					/* その他、外付け機器の有無設定 */
	BYTE waku_char;					/* 預かり券の枠文字 */
	BYTE waku_flag;					/* 預かり券の枠有無(TRUEで有り) */
	BYTE name_flag;					/* 預かり券の店名有無(TRUEで有り) */
	BYTE tokushu_flag;				/* 預かり券の特殊景品有無(TRUEで有り) */
	WORD tokushu_num;				/* 預かり券の特殊景品換算個数 */
	BYTE pas_flag;					/* 計数機のPASフラグ(PAS_x定数) */
	BYTE maker;						/* 計数機のメーカー(0～) */
	BYTE type;						/* 計数機の型番(0～) */
	BYTE pulse;						/* 計数機のパルス幅 */
	BYTE genka;						/* 原価コード */
	BYTE online;					/* オンライン区分 TRUE:オンライン */
	BYTE password[3];				/* パスワード(エース電研ADで使用) */
	BYTE reserve;					/* 予備 */
} tp_setting_t;

/*
 *	払出機設定
 */
typedef struct {
	WORD no;						/* 払出機の番号(0～HARAI_MAX-1) */
	BYTE online;					/* オンライン区分 TRUE:オンライン */
	BYTE maker;						/* 払出機のメーカー(0～) */
	BYTE type;						/* 払出機のタイプ(0～) */
	BYTE pulse;						/* 払出機のパルス幅 */
	WORD reserve2;					/* 予備２ */
} harai_setting_t;

/*
 *	POS設定
 */
typedef struct {
	BYTE no;						/* POSの番号(0～POS_MAX-1) */
	BYTE counter;					/* 設置カウンタ番号(0～) */
	BYTE floor;						/* 設置フロア番号(0～) */
	BYTE pos_syubetu;			/* POSの種別(４種別対応) */
	BYTE shop;						/* 設置店舗番号(0～) */
	BYTE saving_join;			/* 貯玉店舗合算 0:なし 1:あり */
	BYTE point_access;		/* ポイント合算 0:合算エリア 1:来店ポイントエリア */
	BYTE point_join;			/* 貯玉店舗合算 0:なし 1:あり */
	BYTE b_start_pos_no;	/* 貯玉エリアをB店にするPOSの開始番号(0～7) */
	BYTE physics_syubetu;		/* 各種別の物理種別(0:P 1:S) */
	BYTE waku_char;					/* 保留券の枠文字 */
	BYTE waku_flag;					/* 保留券の枠有無(TRUEで有り) */
	BYTE name_flag;					/* 保留券の店名有無(TRUEで有り) */
	BYTE tokushu_flag;				/* 保留券の特殊景品有無(TRUEで有り) */
	WORD tokushu_num;				/* 保留券の特殊景品換算個数 */
	WORD lane[PAS_MAX][LANE_MAX];	/* 三共払い出し機レーン設定(P,A,Sの順) */
	BYTE mars_tokushu_num;			/* マース払出機景品数 */
	BYTE tokushu_wari[3]; 			/* マース払出機景品割数 */
	BYTE mars1_tanka[3]; 			/* マース払出機景品１玉単価 */
	BYTE mars2_tanka[3];			/* マース払出機景品２玉単価 */
	BYTE mars3_tanka[3];			/* マース払出機景品３玉単価 */
	BYTE mars4_tanka[3];		 	/* マース払出機景品４玉単価 */
	BYTE s_select_popup_str[4]; 	/* 種別選択POPUP文字列1～4   2003.02.17 sugiura */
  BYTE glory_status_flag;		/* グローリ状態チェックフラグ 0:行う 1:行わない */
	BYTE reserve2[3];				/* 予備 */
#if 0
	BYTE reserve5[12];				/* 予備 */
	BYTE reserve2[4];				/* 予備 */
	BYTE reserve3[4];				/* 予備 */
	BYTE reserve4[4];				/* 予備 */
#endif
} pos_setting_t;

/*
 *	ネットワークグループ補助
 */
typedef struct {
	BYTE arc_id;					/* ARCNET ID $00:未接続 */
	BYTE type;						/* 機器種別 $00:親 $01:子 $02:TP $03:CA $04:KS */
	BYTE no;						/* POSまたはTPの番号(0～) */
	BYTE alive_flag;				/* 起動状態でTRUE */
	BYTE open_flag;					/* 営業状態でTRUE */
	BYTE change_flag;				/* 交換可能状態でTRUE */
	BYTE hakken_flag;				/* 発券可能状態でTRUE */
	BYTE clear_flag;				/* クリア状態でTRUE */
	BYTE rec_flag;					/* 記憶状態でTRUE */
	BYTE htpsts_flag;				/* ファイル転送中でTRUE */
	BYTE p_alive_flag;				/* 親POSからの指示を記憶するため */
	BYTE p_open_flag;				/* 親POSからの指示を記憶するため */
	BYTE p_change_flag;				/* 親POSからの指示を記憶するため */
	BYTE p_hakken_flag;				/* 親POSからの指示を記憶するため */
	BYTE p_clear_flag;				/* 親POSからの指示を記憶するため */
	BYTE p_rec_flag;				/* 親POSからの指示を記憶するため */
	BYTE p_htpsts_flag;				/* 親POSからの指示を記憶するため */
    BYTE err_status;				/* エラーステータス記憶 */
} pos_group_dat_t;

/*
 *	ネットワークグループ管理
 */
typedef struct {
	pos_group_dat_t pos[POS_MAX];	/* 各POS詳細ステータス */
	pos_group_dat_t tp[TP_MAX];		/* 各TP詳細ステータス */
  BYTE			tp_err[TP_MAX];	/*  */
	pos_group_dat_t ks;				/* KS詳細ステータス */
 	pos_group_dat_t ca;				/* CA詳細ステータス */
 	BYTE srif[SRIF_MAX];			/* SRIF接続有無 */
 	BYTE jihana[JIHANA_MAX];	/* 自販機接続有無 */
 	BYTE vcd[V_CDS_MAX];			/* VCD接続有無 */
 	BYTE cds[CARDINQ_MAX];		/* CDS接続有無 */
 	BYTE jrl_dest_id;					/* JRL送信先ARC_ID */
	BYTE has_dest_id;					/* 発券照会ADP ARC_ID */
	BYTE parent_pos_id;				/* 親POS ARC_ID */
	BYTE reserve;							/* 予備 */
} pos_group_t;

/*
 *	日計１印字選択設定
 */
typedef struct {
	WORD prn_code1;					/* 印字条件コード１ */
	WORD prn_code2;					/* 印字条件コード２ */
	WORD prn_code3;					/* 印字条件コード３ */
	WORD prn_code4;					/* 印字条件コード４ */
	WORD prn_code5;					/* 印字条件コード５ */
	WORD reserved;					/* 予備 */
} pos_nikkei1_print_t;

/*
 *	日計２印字選択設定
 */
typedef struct {
	WORD prn_code1;					/* 印字条件コード１ */
	WORD prn_code2;					/* 印字条件コード２ */
	WORD prn_code3;					/* 印字条件コード３ */
	WORD prn_code4;					/* 印字条件コード４ */
	WORD prn_code5;					/* 印字条件コード５ */
	WORD reserved;					/* 予備 */
} pos_nikkei2_print_t;

/*
 *	日計３印字選択設定
 */
typedef struct {
	WORD prn_code1;					/* 印字条件コード１ */
	WORD prn_code2;					/* 印字条件コード２ */
	WORD prn_code3;					/* 印字条件コード３ */
	WORD prn_code4;					/* 印字条件コード４ */
	WORD prn_code5;					/* 印字条件コード５ */
	WORD reserved;					/* 予備 */
} pos_nikkei3_print_t;

/*
 *	日計４印字選択設定
 */
typedef struct {
	WORD prn_code1;					/* 印字条件コード１ */
	WORD prn_code2;					/* 印字条件コード２ */
	WORD prn_code3;					/* 印字条件コード３ */
	WORD prn_code4;					/* 印字条件コード４ */
	WORD prn_code5;					/* 印字条件コード５ */
	WORD reserved;					/* 予備 */
} pos_nikkei4_print_t;

/*
 *	責任者別操作制限設定
 */
typedef struct {
	BYTE seigen_flag[64];			/* それぞれTRUE,FALSEで操作制限を設ける */
} pos_seigen_t;

/*
 *	責任者登録設定(PMF_ADMIN 構造体):28+2byte/1record
 */
typedef struct {
	BYTE 	gcode;					/* グループコード */
	BYTE 	reserve[3];				/* 予備 */
	DWORD	unyou_limit;			/* 運用処理制限フラグ */
	DWORD	change_limit;			/* 交換処理制限フラグ */
	DWORD	zaiko_limit;			/* 在庫処理制限フラグ */
	DWORD	kaiin_limit;			/* 会員処理制限フラグ */
	DWORD	config_limit;			/* 設定処理制限フラグ */
	DWORD	reserve2;				/* 予備2 */
} pos_admin_t;

/*
 *	キー商品接続設定
 */
typedef struct {
	BYTE name[EXTDEV_NAME_LEN];		/* 外部接続機器名(\0なし) */
	BYTE type[3];					/* 入力種類 'ARC' */
	BYTE id;						/* ARC ID */
} pos_extdev_t;

/*
 *	接続機器設定
 */
typedef struct {
	BYTE pos[POS_MAX];				/* 接続有無(TRUEで接続) */
	BYTE tp[TP_MAX];				/* 接続有無(TRUEで接続) */
	BYTE cardinq[CARDINQ_MAX];		/* 接続有無(TRUEで接続) */
	BYTE jihana[JIHANA_MAX];		/* 接続有無(TRUEで接続) */
	BYTE vending_cds[V_CDS_MAX];	/* 接続有無(TRUEで接続) */
	BYTE ca;						/* 接続有無(TRUEで接続) */
	BYTE ks;						/* 接続有無(TRUEで接続) */
  BYTE srif[SRIF_MAX];			/* 接続有無(TRUEで接続) */
	BYTE resreve1[16];				/* 予備1 */
	BYTE group1_parent_pos_id;	/* グループ1 親POS ID */
	BYTE group1_ca_id;	/* グループ1 CA ID */
	BYTE group1_ks_id;	/* グループ1 KS ID */
	BYTE group1_jrl_id;	/* グループ1 JRL送信先開始ID */
	BYTE group1_tp_id;	/* グループ1 TP(JCR)開始ID */
	BYTE group1_srif_id;	/* グループ1 SRIF開始 ID */
	BYTE group1_vcd_id;	/* グループ1 VCD開始 ID */
	BYTE group1_hsa_id;	/* グループ1 HSA開始 ID */
	BYTE group1_jihana_id;	/* グループ1 自販機開始 ID */
	BYTE resreve2[2];		/* 予備2 */
	BYTE group2_parent_pos_id;	/* グループ2 親POS ID */
	BYTE group2_ca_id;	/* グループ2 CA ID */
	BYTE group2_ks_id;	/* グループ2 KS ID */
	BYTE group2_jrl_id;	/* グループ2 JRL送信先開始ID */
	BYTE group2_tp_id;	/* グループ2 TP(JCR)開始ID */
	BYTE group2_srif_id;	/* グループ2 SRIF開始 ID */
	BYTE group2_vcd_id;	/* グループ2 VCD開始 ID */
	BYTE group2_hsa_id;	/* グループ2 HSA開始 ID */
	BYTE group2_jihana_id;	/* グループ2 自販機開始 ID */
	BYTE resreve3[2];		/* 予備3 */
	BYTE resreve4[16];			/* 予備4 */
} pos_connect_t;

/*
 *
 *	構造体定義
 *	マスタファイル
 *
 */

/*
 *	部門マスタ
 */
typedef struct {
	WORD nID;						/* 部門コード(1～BUMON_MAX) */
	BYTE name[BUMON_NAME_LEN];		/* 部門の名称(\0なし) */
	BYTE reserve[2];
} pos_bumon_t;

/*
 *	商品マスタ
 */
typedef struct {
	BYTE barcode[BC_BIN_LEN];		/* バーコード(BIN形式) */
	DWORD numcode;					/* ユーザ商品コード */
	WORD bumon;						/* 属する部門(1～BUMON_MAX) */
	WORD nID;						/* 内部商品コード(1～ITEM_MAX) */
	BYTE name[ITEM_NAME_LEN];		/* 商品名(\0なし) */
	BYTE reserve1[2];				/* 予備 */
	WORD tax;						/* 課税タイプ(TAX_x定数) */
	DWORD tax_rate;					/* 消費税率(0～100) */
	DWORD a_1_price;				/* 1種別交換単価(A店) */
	DWORD a_2_price;				/* 2種別交換単価(A店) */
	DWORD a_3_price;				/* 3種別交換単価(A店) */
	DWORD a_4_price;				/* 4種別交換単価(A店) */
	DWORD last_date;				/* 最後に使用した日付(内部形式) */
	DWORD last_time;				/* 最後に使用した時間(内部形式) */
	DWORD en_date;					/* データを有効にする年月日(内部形式) */
	DWORD en_time;					/* データを有効にする時間(内部形式) */
	DWORD b_1_price;				/* 1種別交換単価(B店) */
	DWORD b_2_price;				/* 2種別交換単価(B店) */
	DWORD b_3_price;				/* 3種別交換単価(B店) */
	DWORD b_4_price;				/* 4種別交換単価(B店) */
} pos_item_t;

/*
 *	在庫マスタ
 */
typedef struct {
	DWORD numcode;					/* ユーザ商品コード */
	long zaiko;							/* 在庫数 */
	long import;						/* 入庫数 */
	long export_4;					/* 種別4出庫数 */
	long chosei;						/* 調整数 */
	long tana;							/* 棚卸数 */
	long export_1;					/* 種別1出庫数 */
	long export_2;					/* 種別2出庫数 */
	long export_3;					/* 種別3出庫数 */
	WORD nID;								/* 内部商品コード(1～ITEM_MAX) */
	short gosa;							/* 誤差数 */
	long zaiko_all;					/* 全店在庫数(親POSのみ) */
	short move;							/* 移動数 */
	BYTE syori_flag;				/* 処理フラグ(下位ビットから入庫・棚卸・調整・移動・集計) */
	BYTE reserve1;					/* 予備１ */
} pos_zaiko_t;


/*
 *	外部機器商品設定
 */
typedef struct {
	BYTE barcode[BC_BIN_LEN];		/* バーコード(BIN形式) */
	BYTE search_mode;				/* 商品コード検索モード */
	BYTE dumy1;						/* 予備１ */
	BYTE dumy2[2];					/* 予備２ */
} pos_extitem_t;

/*
*  返品、戻し処理制限
*/
typedef struct {
	DWORD nID;							/* 商品コード、シリアル番号 */
	DWORD num;							/* 個数 */
} back_rireki_t;


/*
 *	指定中締め設定
 */
typedef struct {
	WORD nID;						/* 内部商品コード(1～ITEM_MAX) */
	BYTE dmy1[2];					/* 予備 */
} T_CLOSE_PRINT;

/*
 *
 */
typedef struct tagDmyPmf {
	BYTE dmy[2];
} T_DMY_PMF;

/*
 *	自販機アダプタ(富士冷気製)売上データ
 */
typedef struct {
	DWORD rece_date;				/* 受信日付(未使用で0) */
	DWORD rece_time;				/* 受信時間 */
	DWORD code;						/* 商品コード */
	BYTE name[16];					/* 商品名(スペース詰め、\0なし) */
	BYTE reserve1[4];				/* 予備 */
	DWORD cash_price;				/* 現金価格 */
	DWORD p_price;					/* 玉単価 */
	DWORD s_price;					/* メダル単価 */
	DWORD p_item_price;				/* 景品玉単価 */
	DWORD s_item_price;				/* 景品メダル単価 */
	DWORD p_member_price;			/* 会員玉単価 */
	DWORD s_member_price;			/* 会員メダル単価 */
	DWORD cash_out;					/* 現金売上 */
	DWORD p_out;					/* 玉売上 */
	DWORD s_out;					/* メダル売上 */
	DWORD p_item_out;				/* 景品玉売上 */
	DWORD s_item_out;				/* 景品メダル売上 */
	DWORD p_member_out;				/* 会員玉売上 */
	DWORD s_member_out;				/* 会員メダル売上 */
	BYTE reserve2[8];				/* 予備 */
} pos_vending_t;

/*
 *	キーアイテム設定
 */
typedef struct {
	BYTE barcode[BC_BIN_LEN];		/* バーコード(BIN形式) */
} pos_keyitem_t;

/*
 *
 *	構造体定義
 *	履歴関連
 *
 */

/*---[ ジャーナル構造体設定 ]------------------------------------------------*/
/*
 *	総合履歴共通（必要ならこれにキャスト）
 */
typedef struct {
	BYTE dummy1[16];				/* 履歴データ */
	BYTE dummy2[14];				/* 30バイトアラインメント */
} pos_j_common_t;

/*
 *	総合履歴ヘッダ
 */
typedef struct {
	WORD 	type1;					/* 処理区分１(GRIREKI_x定数) */
	WORD 	serial;					/* 取引シリアル */
	DWORD 	date;					/* 日付(内部形式) */
	DWORD 	time;					/* 時間(内部形式) */
	WORD 	pos_no;					/* POSの番号(0～POS_MAX-1) */
	BYTE 	number;					/* 以降の履歴個数(0～) */
	BYTE 	admin;					/* 責任者番号(0～ADMIN_MAX-1) */
	BYTE 	error;					/* 0x00:エラーなし、0x01:エラーあり */
	BYTE 	dummy;					/* 予備 */
	WORD	regi_sheet;				/* レシート番号(※交換処理以外は0を格納) */
	BYTE	dummy2[8];				/* 予備２ */
	WORD	pmf_header;				/* PMFヘッダ(0xEEFF) */
} pos_j_header_t;

/*
 *	総合履歴・一般景品
 */
typedef struct {
	WORD	type2;					/* 処理区分２(GRIREKI_IPPAN) */
	WORD 	pas_flag;				/* PASフラグ(PAS_x定数) */
	DWORD 	nID;					/* 商品コード(1～ITEM_MAX) */
	WORD 	num;					/* 景品数量(1～) */
	BYTE 	reserve[2];				/* 予備 */
	DWORD 	price;					/* 景品玉数(単価×数量) */
} pos_j_ippan_t;

/*
 *	総合履歴・特殊景品
 */
typedef struct {
	WORD type2;						/* 処理区分２(GRIREKI_TOKUSHU) */
	WORD num1;						/* 特殊景品特大 個数 */
	WORD num2;						/* 特殊景品大 個数 */
	WORD num3;						/* 特殊景品中 個数 */
	WORD num4;						/* 特殊景品小 個数 */
	WORD pas_flag;					/* PASフラグ(PAS_x定数) */
	DWORD price;					/* 景品玉数(単価×数量の４合計) */
} pos_j_tokushu_t;

/*
 *	総合履歴・貯玉指定
 */
typedef struct {
	WORD type2;						/* 処理区分２(GRIREKI_SAVINGS) */
	WORD pas_flag;					/* PASフラグ(PAS_x定数) */
	DWORD code;						/* 会員番号 */
	DWORD num;						/* 貯玉玉数(1～) */
	DWORD arng_num;					/* (POS3種別対応 2002/01/30 DDS.Shimizu) */
} pos_j_savings_t;

/*
 *	総合履歴・貯玉遷移
 */
typedef struct {
	WORD		type2;				/* 処理区分２(GRIREKI_SAVINGSDAT) */
	WORD		pas_flag;			/* PASフラグ */
	DWORD		num;				/* 実際にKSに預入れ、引き出した玉数 */
	DWORD		chot1;				/* カードイン時の貯玉数 */
	DWORD		chot2;				/* 処理後の貯玉数 */
	BYTE		type;				/* 0x01:預入れ、0x02:引出し */
	BYTE		dmy[13];			/* 予備 */
} pos_j_savingsdat_t;

/*
 *	総合履歴・交換処理エラー
 */
typedef struct {
	WORD		type2;				/* 処理区分２(GRIREKI_CHGERR) */
	WORD		type;				/* エラー区分 */
	WORD		code;				/* エラーコード */
	BYTE		dat1;				/* データ１ */
	BYTE		dat2;				/* データ２ */
	BYTE		dat3;				/* データ３ */
	BYTE		dat4;				/* データ４ */
	WORD		dat5;				/* データ５ */
	DWORD		dat6;				/* データ６ */
	DWORD		dat7;				/* データ７ */
	DWORD		dat8;				/* データ８ */
	DWORD		dat9;				/* データ９ */
	WORD		dat10;				/* データ１０ */
} pos_j_chgerr_t;

/*
 *	総合履歴・預かり
 */
typedef struct {
	WORD type2;						/* 処理区分２(GRIREKI_DEPOSIT) */
	WORD dep_type;					/* 預かり区分(GRIREKI_x定数) */
	DWORD nID;						/* 会員番号,保留券番号 */
	DWORD num;						/* 景品玉数 */
	WORD pas_flag;					/* PASフラグ(PAS_x定数) */
	WORD machine_type;				/* 機器番号 */
	WORD mashine_id;				/* 号機番号 */
	DWORD slot_org;					/* 貯メダル処理前数(POS3種別対応 2002/01/30 DDS.Shimizu) */
	WORD reserve[10];				/* 予備 */
} pos_j_deposit_t;

/*
 *	総合履歴・余り玉
 */
typedef struct {
	WORD type2;						/* 処理区分２(GRIREKI_REST) */
	WORD rest_type;					/* 余り玉処理区分 */
	DWORD nID;						/* 会員番号,保留券番号 */
	DWORD num;						/* 景品玉数 */
	DWORD total;					/* 貯玉数（更新後）*/
	WORD  pas_flag;					/* PASフラグ */
	BYTE reserve[12];				/* 予備 */
} pos_j_rest_t;

/*
 *	総合履歴・一般景品返品／戻し
 */
typedef struct {
	WORD type2;						/* 処理区分２(GIREKI_MIPPAN) */
	WORD pas_flag;					/* PASフラグ(PAS_x定数) */
	DWORD nID;						/* 商品コード */
	WORD num;						/* 商品数量 */
	WORD reserve;					/* 予備 */
	DWORD price;					/* 玉数 */
	WORD dep_type;					/* 預かり区分(GRIREKI_x定数) */
} pos_j_mippan_t;

/*
 *	総合履歴・特殊景品返品／戻し
 */
typedef struct {
	WORD type2;						/* 処理区分２(GIREKI_MTOKUSHU) */
	WORD num1;						/* 特殊景品特大 個数 */
	WORD num2;						/* 特殊景品大 個数 */
	WORD num3;						/* 特殊景品中 個数 */
	WORD num4;						/* 特殊景品小 個数 */
	WORD pas_flag;					/* PASフラグ(PAS_x定数) */
	DWORD price;					/* 景品玉数(単価×数量の４合計) */
	WORD dep_type;					/* 預かり区分(GRIREKI_x定数) */
} pos_j_mtokushu_t;

/*
 *	保留券履歴
 */
typedef struct {
	DWORD date;						/* 発行日付(内部形式) */
	DWORD time;						/* 発行時間(内部形式) */
	BYTE pas_flag;					/* PASフラグ(PAS_x定数) */
	BYTE admin;						/* 責任者番号(0～ADMIN_MAX-1) */
	WORD nID;						/* 保留券通し番号(1～) */
	DWORD num;						/* 保留玉数 */
	BYTE re_issue;					/* 再発行回数(0～9) */
	BYTE flag;						/* 処理済みフラグ(>=0x80で処理済) */
} pos_j_horyu_t;

/*
 *	端末エラー履歴
 */
typedef struct {
	DWORD date;						/* 発生日付(内部形式) */
	DWORD time;						/* 発生時間(内部形式) */
	WORD pos_no;					/* POS番号 */
	WORD error;						/* エラーコード */
	BYTE machine_type;				/* 機器番号 */
	BYTE machine_id;				/* 号機番号 */
	WORD ticket_no;					/* チケットシリアル番号 */
	BYTE reserve3[14];				/* 予備３ */
} pos_j_error_t;

/*
 *	総合履歴・運用処理
 */
typedef struct {
	WORD 	type2;					/* 処理区分２ */
	BYTE 	reserve[28];			/* 予備 */
} pos_j_unyou_t;

/*
 *	総合履歴・運用エラー処理
 */
typedef struct {
	WORD	type2;					/* 処理区分２(GRIREKI_UNYERR) */
	WORD	code;					/* エラーコード */
	DWORD	tp;						/* TP1～TP32(bit0～bit31) */
	BYTE	pos;					/* POS1～POS8(bit0～bit7) */
	BYTE	cds;					/* CDS1～CDS8(bit0～bit7) */
	BYTE	reserve[20];			/* 予備 */
} pos_j_unyerr_t;

/*
 *	総合履歴・在庫管理処理
 */
typedef struct {
	WORD 	type2;					/* 処理区分２ */
	WORD	reserve;				/* 予備１ */
	DWORD	nID;					/* 景品コード */
	long	num;					/* 処理景品個数 */
	long	zaiko1;					/* 理論在庫数(処理前) */
	long	zaiko2;					/* 理論在庫数(処理後) */
	short	gosa_all;				/* 誤差数(全店) */
	short	gosa;					/* 誤差数 */
	BYTE	pos_no;					/* 移動元POS番号(移動のみ) */
	BYTE 	reserve2[5];			/* 予備 */
} pos_j_zaiko_t;

/*
 *	総合履歴・在庫管理エラー処理
 */
typedef struct {
	WORD	type2;					/* 処理区分２(GRIREKI_ZAIKERR) */
	WORD	code;					/* エラーコード */
	BYTE	pos_no;					/* POS1～POS8(bit0～bit7) */
	BYTE	reserve[25];			/* 予備 */
} pos_j_zaikoerr_t;

/*
 *	総合履歴・会員端末処理
 */
typedef struct {
	WORD 	type2;					/* 処理区分２ */
	BYTE	reserve;				/* 予備 */
	BYTE	chg_type;				/* 状態変更種類 */
	DWORD	code1;					/* 会員番号1 */
	DWORD	code2;					/* 会員番号2 */
	DWORD	chot1;					/* 移行前貯玉数 */
	DWORD	chom1;					/* 移行前貯メダル数 */
	DWORD	chot2;					/* 移行後貯玉数 */
	DWORD	chom2;					/* 移行後貯メダル数 */
	WORD 	password;				/* クリア前暗証番号 */
} pos_j_kaiin_t;

/*
 *	総合履歴・会員端末エラー処理
 */
typedef struct {
	WORD	type2;					/* 処理区分２(GRIRKEI_KERR) */
	WORD	ecode;					/* エラーコード */
	DWORD	kcode1;					/* 処理エラー会員番号１ */
	DWORD	kcode2;					/* 処理エラー会員番号２ */
	BYTE	reserve[18];			/* 予備 */
} pos_j_kaiinerr_t;

/*
 *	総合履歴・交換定数設定処理(1)
 */
typedef struct {
	WORD 	type2;					/* 処理区分2(GRIREKI_EXCHG) */
	WORD	p_tanka;				/* Ｐ単価 */
	WORD	p_servece;				/* Ｐサービス玉 */
	WORD	p_canservice;			/* Ｐサービス有効玉 */
	WORD	p_canend;				/* Ｐ終了可能玉数 */
	WORD	p_horyu;				/* Ｐ保留券発行玉数 */
	WORD	p_tesuu;				/* Ｐ手数料玉数(貯玉) */
	WORD	p_limit;				/* Ｐ制限値(貯玉) */
	WORD	p_limitout;				/* Ｐ制限外手数料玉数 */
	WORD	a_tanka;				/* Ａ単価 */
	WORD	a_service;				/* Ａサービス玉 */
	WORD	a_cansevice;			/* Ａサービス有効玉 */
	WORD	a_canend;				/* Ａ終了可能玉数 */
	WORD	a_horyu;				/* Ａ保留券発行玉数 */
	WORD	a_tesuu;				/* Ａ手数料玉数(貯玉) */
} pos_j_chgcfg1_t;

/*
 *	交換定数設定処理履歴(2)
 */
typedef struct {
	WORD 	type2;					/* 処理区分2(GRIREKI_EXCHG) */
	WORD	a_limit;				/* Ａ制限値(貯玉) */
	WORD	a_limitout;				/* Ａ制限外手数料玉数 */
	WORD	s_tanka;				/* Ｓ単価 */
	WORD	s_service;				/* Ｓサービス玉 */
	WORD	s_cansevice;			/* Ｓサービス有効玉 */
	WORD	s_canend;				/* Ｓ終了可能玉数 */
	WORD	s_horyu;				/* Ｓ保留券発行玉数 */
	WORD	s_tesuu;				/* Ｓ手数料玉数(貯玉) */
	WORD	s_limit;				/* Ｓ制限値(貯玉) */
	WORD	s_limitout;				/* Ｓ制限外手数料玉数 */
	BYTE	reserve[8];				/* 予備 */
} pos_j_chgcfg2_t;

/*
 *	総合履歴・時刻設定処理
 */
typedef struct {
	WORD	type2;					/* 処理区分2(GRIREKI_TIMEDEF) */
	BYTE	reserve[2];				/* 予備 */
	DWORD	p_date;					/* 設定前日時 */
	DWORD	p_time;					/* 設定前時刻 */
	DWORD	date;					/* 設定日時 */
	DWORD	time;					/* 設定時刻 */
	BYTE	reserve2[10];			/* 予備２ */
} pos_j_timedef_t;

/*
 *	総合履歴・島端I/F設定処理１
 */
typedef struct {
	WORD	type2;					/* 処理区分2(GRIREKI_SRIF) */
	WORD	srif_no;				/* 設定SRI/F番号(0～SRIF_MAX-1) */
	BYTE	reserve2[2];			/* 予備２ */
	BYTE	syu;					/* 種別(0x00:玉、0x01;メダル) */
	BYTE	pt;						/* ポイント表示(0x00:なし、0x01:あり) */
	DWORD	max_tama;				/* 払出上限値(玉) */
	DWORD	max_med;				/* 払出上限値(メダル) */
	WORD	b1_tesu_t;				/* ボタン1手数料(玉) */
	WORD	b1_harai_t;				/* ボタン1払出数(玉) */
	WORD	b2_tesu_t;				/* ボタン2手数料(玉) */
	WORD	b2_harai_t;				/* ボタン2払出数(玉) */
	WORD	b3_tesu_t;				/* ボタン3手数料(玉) */
	WORD	b3_harai_t;				/* ボタン3払出数(玉) */
	WORD	b4_tesu_t;				/* ボタン4手数料(玉) */
} pos_j_srif_t;

/*
 *	総合履歴・島端I/F設定処理２
 */
typedef struct {
	WORD	type2;					/* 処理区分2(GRIREKI_SRIF) */
	BYTE	reserve[2];				/* 予備 */
	BYTE	reserve2[2];			/* 予備２ */
	WORD	b4_harai_t;				/* ボタン4払出数(玉) */
	WORD	b5_tesu_t;				/* ボタン5手数料(玉) */
	WORD	b5_harai_t;				/* ボタン5払出数(玉) */
	WORD	b6_tesu_t;				/* ボタン6手数料(玉) */
	WORD	b6_harai_t;				/* ボタン6払出数(玉) */
	WORD	b1_tesu_m;				/* ボタン1手数料(メダル) */
	WORD	b1_harai_m;				/* ボタン1払出数(メダル) */
	WORD	b2_tesu_m;				/* ボタン2手数料(メダル) */
	WORD	b2_harai_m;				/* ボタン2払出数(メダル) */
	WORD	b3_tesu_m;				/* ボタン3手数料(メダル) */
	WORD	b3_harai_m;				/* ボタン3払出数(メダル) */
	WORD	b4_tesu_m;				/* ボタン4手数料(メダル) */
} pos_j_srif2_t;

/*
 *	総合履歴・島端I/F設定処理３
 */
typedef struct {
	WORD	type2;					/* 処理区分2(GRIREKI_SRIF) */
	BYTE	reserve[2];				/* 予備 */
	BYTE	reserve2[2];			/* 予備２ */
	WORD	b4_harai_m;				/* ボタン4払出数(メダル) */
	WORD	b5_tesu_m;				/* ボタン5手数料(メダル) */
	WORD	b5_harai_m;				/* ボタン5払出数(メダル) */
	WORD	b6_tesu_m;				/* ボタン6手数料(メダル) */
	WORD	b6_harai_m;				/* ボタン6払出数(メダル) */
	BYTE	reserve3[14];			/* 予備３ */
} pos_j_srif3_t;

/*
 *	総合履歴・設定エラー処理
 */
typedef struct {
	WORD	type2;					/* 処理区分２(GRIREKI_CFGERR) */
	WORD	code;					/* エラーコード */
	DWORD	tp;						/* TP1～TP32(bit0～bit31) */
	BYTE	pos;					/* POS1～POS8(bit0～bit7) */
	BYTE	cds;					/* CDS1～CDS8(bit0～bit7) */
	BYTE	srif;					/* SRIF1～SRIF(bit0～bit7) */
	BYTE	reserve[19];			/* 予備 */
} pos_j_configerr_t;

/*
 *	総合履歴・カード処理
 */
typedef struct {
	WORD	type2;					/* 処理区分２(0x0000) */
	WORD	reserve;				/* 予備 */
	DWORD	card_no;				/* カード番号 */
	BYTE	type;					/* 処理タイプ(カードイン／アウト) */
	BYTE	reserve2[21];			/* 予備2 */
} pos_j_card_t;

/*
 *	総合履歴・カードエラー処理
 */
typedef struct {
	WORD	type2;					/* 処理区分２(0x0000) */
	WORD	code;					/* エラーコード */
	BYTE	reserve[26];			/* 予備 */
} pos_j_carderr_t;

/*
 *	エラー処理
 */
typedef struct {
	WORD	type1;					/* 処理区分1(GRIREKI_ERROR) */
	WORD	type2;					/* 処理区分2 */
	BYTE	reserve[2];				/* 予備 */
	WORD	error;					/* エラー情報 */
	BYTE	harai_type;				/* 払出機種類(払出機エラーのみ) */
	BYTE	reserve2[21];
} pos_j_err_t;


/*---[ 構造体設定(統計ファイル) ]--------------------------------------------*/
/*
 *	責任者別コードファイル
 */
typedef struct {
	WORD code;						/* 設定コード */
} pos_admin_code_t;

/*
 *	兼用ファイル
 *		・POS統計
 *		・POS責任者別統計
 *		・各POS集計統合
 */
#if 0		// pos_toukei_t　を配列型に変更しました。　中谷
typedef struct  {
	DWORD	guest;						/* 客数 */
	DWORD	n_change_1;				/* 種別1交換回数 */
	DWORD	n_change_2;				/* 種別2交換回数 */
	DWORD	n_change_3;				/* 種別3交換回数 */
	DWORD	n_change_4;				/* 種別4交換回数 */
	DWORD	change_1;					/* 種別1交換玉数 */
	DWORD	change_2;					/* 種別2交換玉数 */
	DWORD	change_3;					/* 種別3交換玉数 */
	DWORD	change_4;					/* 種別4交換玉数 */

	long	tokushu_1[TOKUSHU_MAX];	/* 種別1特殊景品玉 */
	long	tokushu_2[TOKUSHU_MAX];	/* 種別2特殊景品玉 */
	long	tokushu_3[TOKUSHU_MAX];	/* 種別3特殊景品玉 */
	long	tokushu_4[TOKUSHU_MAX];	/* 種別4特殊景品玉 */

	DWORD	mharai_true_1[4];			/* 種別1マース、ヒロタック製払出機 景品1.2.3.4 */
	DWORD	mharai_true_2[4];			/* 種別2マース、ヒロタック製払出機 景品1.2.3.4 */
	DWORD	mharai_true_3[4];			/* 種別3マース、ヒロタック製払出機 景品1.2.3.4 */
	DWORD	mharai_true_4[4];			/* 種別4マース、ヒロタック製払出機 景品1.2.3.4 */
	DWORD	mharai_false_1[4];		/* 種別1マース製払出機 未払い分景品1.2.3.4 */
	DWORD	mharai_false_2[4];		/* 種別2マース製払出機 未払い分景品1.2.3.4 */
	DWORD	mharai_false_3[4];		/* 種別3マース製払出機 未払い分景品1.2.3.4 */
	DWORD	mharai_false_4[4];		/* 種別4マース製払出機 未払い分景品1.2.3.4 */

	DWORD	n_return_1;				/* 種別1戻し玉処理回数 */
	DWORD	n_return_2;				/* 種別2戻し玉処理回数 */
	DWORD	n_return_3;				/* 種別3戻し玉処理回数 */
	DWORD	n_return_4;				/* 種別4戻し玉処理回数 */
	DWORD	return_1;					/* 種別1戻し玉玉数 */
	DWORD	return_2;					/* 種別2戻し玉玉数 */
	DWORD	return_3;					/* 種別3戻し玉玉数 */
	DWORD	return_4;					/* 種別4戻し玉玉数 */

	DWORD	n_pickup_1;				/* 種別1拾い玉処理回数 */
	DWORD	n_pickup_2;				/* 種別2拾い玉処理回数 */
	DWORD	n_pickup_3;				/* 種別3拾い玉処理回数 */
	DWORD	n_pickup_4;				/* 種別4拾い玉処理回数 */
	DWORD	pickup_1;					/* 種別1拾い玉玉数 */
	DWORD	pickup_2;					/* 種別2拾い玉玉数 */
	DWORD	pickup_3;					/* 種別3拾い玉玉数 */
	DWORD	pickup_4;					/* 種別4拾い玉玉数 */

	DWORD	g_withdraw_1;			/* 種別1貯玉払出客数 */
	DWORD	g_withdraw_2;			/* 種別2貯玉払出客数 */
	DWORD	g_withdraw_3;			/* 種別3貯玉払出客数 */
	DWORD	g_withdraw_4;			/* 種別4貯玉払出客数 */
	DWORD	withdraw_1;				/* 種別1貯玉払出玉数 */
	DWORD	withdraw_2;				/* 種別2貯玉払出玉数 */
	DWORD	withdraw_3;				/* 種別3貯玉払出玉数 */
	DWORD	withdraw_4;				/* 種別4貯玉払出玉数 */
	DWORD	g_deposit_1;			/* 種別1貯玉預入客数 */
	DWORD	g_deposit_2;			/* 種別2貯玉預入客数 */
	DWORD	g_deposit_3;			/* 種別3貯玉預入客数 */
	DWORD	g_deposit_4;			/* 種別4貯玉預入客数 */
	DWORD	deposit_1;				/* 種別1貯玉預入玉数 */
	DWORD	deposit_2;				/* 種別2貯玉預入玉数 */
	DWORD	deposit_3;				/* 種別3貯玉預入玉数 */
	DWORD	deposit_4;				/* 種別4貯玉預入玉数 */

	DWORD	i_ha_1;					/* 種別1端玉景品数 */
	DWORD	i_ha_2;					/* 種別2端玉景品数 */
	DWORD	i_ha_3;					/* 種別3端玉景品数 */
	DWORD	i_ha_4;					/* 種別4端玉景品数 */
	DWORD	ha_1;						/* 種別1端玉玉数 */
	DWORD	ha_2;						/* 種別2端玉玉数 */
	DWORD	ha_3;						/* 種別3端玉玉数 */
	DWORD	ha_4;						/* 種別4端玉玉数 */

	DWORD	n_over_1;				/* 種別1余り玉処理回数 */
	DWORD	n_over_2;				/* 種別2余り玉処理回数 */
	DWORD	n_over_3;				/* 種別3余り玉処理回数 */
	DWORD	n_over_4;				/* 種別4余り玉処理回数 */
	DWORD	over_1;					/* 種別1余り玉玉数 */
	DWORD	over_2;					/* 種別2余り玉玉数 */
	DWORD	over_3;					/* 種別3余り玉玉数 */
	DWORD	over_4;					/* 種別4余り玉玉数 */
	DWORD	back_ha;				/* 端玉景品戻し数 */

	DWORD	n_get_1;					/* 種別1取り込み処理回数 */
	DWORD	n_get_2;					/* 種別2取り込み処理回数 */
	DWORD	n_get_3;					/* 種別3取り込み処理回数 */
	DWORD	n_get_4;					/* 種別4取り込み処理回数 */
	DWORD	get_1;						/* 種別1取り込み玉数 */
	DWORD	get_2;						/* 種別2取り込み玉数 */
	DWORD	get_3;						/* 種別3取り込み玉数 */
	DWORD	get_4;						/* 種別4取り込み玉数 */

	DWORD	n_service_1;				/* 種別1サービス処理回数 */
	DWORD	n_service_2;				/* 種別2サービス処理回数 */
	DWORD	n_service_3;				/* 種別3サービス処理回数 */
	DWORD	n_service_4;				/* 種別4サービス処理回数 */
	DWORD	service_1;					/* 種別1サービス玉数 */
	DWORD	service_2;					/* 種別2サービス玉数 */
	DWORD	service_3;					/* 種別3サービス玉数 */
	DWORD	service_4;					/* 種別4サービス玉数 */

	WORD	n_tenyu_1;				/* 種別1手入力回数 */
	WORD	c_tenyu_1;				/* 種別1手入力交換回数 */
	WORD	n_tenyu_2;				/* 種別2手入力回数 */
	WORD	c_tenyu_2;				/* 種別2手入力交換回数 */
	WORD	n_tenyu_3;				/* 種別3手入力回数 */
	WORD	c_tenyu_3;				/* 種別3手入力交換回数 */
	WORD	n_tenyu_4;				/* 種別4手入力回数 */
	WORD	c_tenyu_4;				/* 種別4手入力交換回数 */
	DWORD	tenyu_1;					/* 種別1手入力玉数 */
	DWORD	tenyu_2;					/* 種別2手入力玉数 */
	DWORD	tenyu_3;					/* 種別3手入力玉数 */
	DWORD	tenyu_4;					/* 種別4手入力玉数 */

	WORD	n_henpin_1;				/* 種別1返品回数 */
	WORD	c_henpin_1;				/* 種別1返品交換回数 */
	WORD	n_henpin_2;				/* 種別2返品回数 */
	WORD	c_henpin_2;				/* 種別2返品交換回数 */
	WORD	n_henpin_3;				/* 種別3返品回数 */
	WORD	c_henpin_3;				/* 種別3返品交換回数 */
	WORD	n_henpin_4;				/* 種別4返品回数 */
	WORD	c_henpin_4;				/* 種別4返品交換回数 */
	DWORD	henpin_1;					/* 種別1返品玉数 */
	DWORD	henpin_2;					/* 種別2返品玉数 */
	DWORD	henpin_3;					/* 種別3返品玉数 */
	DWORD	henpin_4;					/* 種別4返品玉数 */

	DWORD	n_horyu_1;				/* 種別1保留券発行件数 */
	DWORD	n_horyu_2;				/* 種別2保留券発行件数 */
	DWORD	n_horyu_3;				/* 種別3保留券発行件数 */
	DWORD	n_horyu_4;				/* 種別4保留券発行件数 */
	DWORD	horyu_1;					/* 種別1保留券発行玉数 */
	DWORD	horyu_2;					/* 種別2保留券発行玉数 */
	DWORD	horyu_3;					/* 種別3保留券発行玉数 */
	DWORD	horyu_4;					/* 種別4保留券発行玉数 */
	
	DWORD	n_horyue_1;				/* 種別1保留券読み取り成功件数 */
	DWORD	n_horyue_2;				/* 種別2保留券読み取り成功件数 */
	DWORD	n_horyue_3;				/* 種別3保留券読み取り成功件数 */
	DWORD	n_horyue_4;				/* 種別4保留券読み取り成功件数 */
	DWORD	horyue_1;					/* 種別1保留券読み取り成功玉数 */
	DWORD	horyue_2;					/* 種別2保留券読み取り成功玉数 */
	DWORD	horyue_3;					/* 種別3保留券読み取り成功玉数 */
	DWORD	horyue_4;					/* 種別4保留券読み取り成功玉数 */

	DWORD	n_hold_1;				/* 種別1預かり券読み取り件数 */
	DWORD	n_hold_2;				/* 種別2預かり券読み取り件数 */
	DWORD	n_hold_3;				/* 種別3預かり券読み取り件数 */
	DWORD	n_hold_4;				/* 種別4預かり券読み取り件数 */
	DWORD	hold_1;					/* 種別1預かり券読み取り玉数 */
	DWORD	hold_2;					/* 種別2預かり券読み取り玉数 */
	DWORD	hold_3;					/* 種別3預かり券読み取り玉数 */
	DWORD	hold_4;					/* 種別4預かり券読み取り玉数 */

	DWORD	n_holde_1;				/* 種別1預かり券処理済件数 */
	DWORD	n_holde_2;				/* 種別2預かり券処理済件数 */
	DWORD	n_holde_3;				/* 種別3預かり券処理済件数 */
	DWORD	n_holde_4;				/* 種別4預かり券処理済件数 */
	DWORD	holde_1;					/* 種別1預かり券処理済玉数 */
	DWORD	holde_2;					/* 種別2預かり券処理済玉数 */
	DWORD	holde_3;					/* 種別3預かり券処理済玉数 */
	DWORD	holde_4;					/* 種別4預かり券処理済玉数 */

	DWORD	clear_num;				/* クリア回数 */
	DWORD	power_num;				/* 電源投入回数 */
	DWORD	admchg_num;				/* 責任者入れ替わり回数 */
	DWORD	chosei_num;				/* 調整数 */
	DWORD	reserve5;				/* 予備 */
	DWORD	reserve6;				/* 予備 */
	DWORD	reserve7;				/* 予備 */
	DWORD	reserve8;				/* 予備 */
	DWORD	reserve9;				/* 予備 */
} pos_toukei_t;
#endif

typedef struct  {
	DWORD	guest;						/* 客数 */
	DWORD	n_change[4];				/* 交換回数 [種別(１～４)]*/
	DWORD	change[4];					/* 交換玉数 [種別(１～４)]*/

	long	tokushu[4][TOKUSHU_MAX];	/* 特殊景品玉 [種別(１～４)][TOKUSHU_MAX]*/

	DWORD	mharai_true[4][4];			/* マース、ヒロタック製払出機 [種別(１～４)][景品1.2.3.4] */
	DWORD	mharai_false[4][4];			/* マース製払出機 未払い分 [種別(１～４)][景品1.2.3.4] */

	DWORD	n_return[4];				/* 戻し玉処理回数 [種別(１～４)] */
	DWORD	return_[4];					/* 戻し玉玉数 [種別(１～４)] */

	DWORD	n_pickup[4];				/* 拾い玉処理回数 [種別(１～４)] */
	DWORD	pickup[4];					/* 拾い玉玉数 [種別(１～４)] */

	DWORD	g_withdraw[4];				/* 貯玉払出客数 [種別(１～４)] */
	DWORD	withdraw[4];				/* 貯玉払出玉数 [種別(１～４)] */
	DWORD	g_deposit[4];				/* 貯玉預入客数 [種別(１～４)] */
	DWORD	deposit[4];					/* 貯玉預入玉数 [種別(１～４)] */

	DWORD	i_ha[4];					/* 端玉景品数 [種別(１～４)] */
	DWORD	ha[4];						/* 端玉玉数 [種別(１～４)] */

	DWORD	n_over[4];					/* 余り玉処理回数 [種別(１～４)] */
	DWORD	over[4];					/* 余り玉玉数 [種別(１～４)] */
	DWORD	back_ha;					/* 端玉景品戻し数 */

	DWORD	n_get[4];					/* 取り込み処理回数 [種別(１～４)] */
	DWORD	get[4];						/* 取り込み玉数 [種別(１～４)] */

	DWORD	n_service[4];				/* サービス処理回数 [種別(１～４)] */
	DWORD	service[4];					/* サービス玉数 [種別(１～４)] */

	WORD	n_tenyu[4][2];				/* 手入力回数 [種別(１～４)] [手入力交換回数 種別(１～４)] */
	DWORD	tenyu[4];					/* 手入力玉数 [種別(１～４)] */

	WORD	n_henpin[4][2];				/* 返品回数 [種別(１～４)] [ 返品交換回数  種別(１～４)] */
	DWORD	henpin[4];					/* 返品玉数 [種別(１～４)] */

	DWORD	n_horyu[4];					/* 保留券発行件数 [種別(１～４)] */
	DWORD	horyu[4];					/* 保留券発行玉数 [種別(１～４)] */

	DWORD	n_horyue[4];				/* 保留券読み取り成功件数 [種別(１～４)] */
	DWORD	horyue[4];					/* 保留券読み取り成功玉数 [種別(１～４)] */

	DWORD	n_hold[4];					/* 預かり券読み取り件数 [種別(１～４)] */
	DWORD	hold[4];					/* 預かり券読み取り玉数 [種別(１～４)] */

	DWORD	n_holde[4];					/* 預かり券処理済件数 [種別(１～４)] */
	DWORD	holde[4];					/* 預かり券処理済玉数 [種別(１～４)] */

	DWORD	clear_num;					/* クリア回数 */
	DWORD	power_num;					/* 電源投入回数 */
	DWORD	admchg_num;					/* 責任者入れ替わり回数 */
	DWORD	chosei_num;					/* 調整数 */
	DWORD	reserve5;					/* 予備 */
	DWORD	reserve6;					/* 予備 */
	DWORD	reserve7;					/* 予備 */
	DWORD	reserve8;					/* 予備 */
	DWORD	reserve9;					/* 予備 */

} pos_toukei_t;
					


/*
 *	チケットプリンタ集計(00/11/01 追加)
 */
typedef struct {
	DWORD	sum_num;			/* 入力総数 */
	DWORD	print_num;				/* 預り券発券玉数(通常) */
	DWORD	print_err_num;			/* 預り券発券玉数(エラー) */
	DWORD	noinq_num;				/* 未照会玉数(通常) */
	DWORD	noinq_err_num;		/* 未照会玉数(エラー) */
	DWORD	savings_num;			/* 貯玉数 */
	DWORD	rtn_num;				/* 余り玉返却玉数 */
	WORD	sum_ticket;			/* 入力総件数 */
	WORD	print_ticket;				/* 預り券発券件数(通常) */
	WORD	print_err_ticket;			/* 預り券発券件数(エラー) */
	WORD	noinq_ticket;				/* 未照会件数(通常) */
	WORD	noinq_err_ticket;		/* 未照会件数(エラー) */
	WORD	savings_ticket;			/* 貯玉件数 */
	WORD	rtn_ticket;				/* 余り玉返却件数 */
	BYTE	pas_ticket;				/* 種別 */
	BYTE	reserve[5];				/* 予備１ */
} tp_sum_t;

/*---[ 構造体設定(内部管理用) ]----------------------------------------------*/

/*
 *	交換部テンポラリ構造体
 */
typedef struct {
	WORD flag;						/* 0:未使用 1:交換 2:返品 */
	WORD bumon;						/* 部門コード */
	BYTE name[ITEM_NAME_LEN];		/* 景品の名前(\0なし) */
	BYTE name_term;					/* '\0' */
	WORD nID;						/* 商品コード */
	DWORD num;						/* 商品個数 */
	DWORD price;					/* 単価(P,A,Sはpas_typeに基づく) */
	DWORD date;						/* 選択した年月日 */
	DWORD time;						/* 選択した時間 */
} change_tmp_t;

/*
 *	特殊景品計算用構造体
 */
typedef struct tagExtraOffer {
	unsigned long 	total,
					dai,
					chu,
					syo;
} T_EXTRA_OFFER;

/*
 *	商品編集用構造体
 */
typedef struct {
	BYTE bc_head[BC_HEAD_LEN];		/* バーコード ヘッド */
	BYTE bc_code[BC_CODE_LEN];		/* バーコード コード */
	DWORD nID;						/* 商品コード */
	WORD bumon;						/* 属する部門 */
	BYTE name[ITEM_NAME_LEN];		/* 商品名 */
	BYTE reserve[2];				/* 予備\0 */
	WORD tax;						/* 課税タイプ */
	WORD key;						/* アイテムキー */
	DWORD tax_rate;					/* 消費税率 */
	DWORD p_price;					/* P交換単価 */
	DWORD a_price;					/* A交換単価 */
	DWORD s_price;					/* S交換単価 */
	int en_year;					/* 有効年 */
	int en_month;					/* 有効月 */
	int en_day;						/* 有効日 */
} pos_in_item_t;

/*
 *	チケット構造体
 *	※保留券構造体としても使用
 */
typedef struct {
	BYTE type;						/* 機器種別(TPは2) */
	BYTE tp_no;						/* TP/POSの番号(1～TP_MAX/1～POS_MAX) */
	DWORD serial_no;				/* チケット通し番号 */
	BYTE ken_type;					/* 券種別(預かり券は0) */
	BYTE pas_type;					/* PAS種別(PAS_P or PAS_A or PAS_S or PAS_EX) */
	DWORD num;						/* 玉(メダル)の数 */
	WORD maker;						/* メーカーコード */
	WORD online;					/* オンライン区分(エース電研で使用) */
	BYTE year;						/* 発券日付 年 */
	BYTE month;						/* 発券日付 月 */
	BYTE day;						/* 発券日付 日 */
	BYTE password[3];				/* パスワード */
} pos_ticket_t;

/*
 *	７セグ表示設定サブ構造体
 */
typedef struct {
	BYTE offset;					/* ｵﾌｾｯﾄ（表示位置) */
	BYTE itemno;					/* 項目ＮＯ */
	BYTE size;						/* ｻｲｽﾞ（項目ｻｲｽﾞ) */
	BYTE color;						/* 色（表示色） */
} pos_segconfig_dat_t;

/*
 *	７セグ表示設定構造体
 */
typedef struct {
	pos_segconfig_dat_t segu[10];	/* 設定可能項目数分 */
} pos_segconfig_t;

/*
 *	貯玉日報印字用構造体
 */
typedef struct {
	unsigned long	nippou_make_ps_tm;		/*	日報作成日付(開店日付)	*/
	unsigned char	dmy1[12];					/*	*/
	unsigned long	master_kaiinsu;			/*	会員総数		*/
	unsigned long	del_kaiinsu;			/*	削除件数		*/
	unsigned short	today_kaiinsu;			/*	本日来店会員数	*/
	unsigned short	dumy2;					/*	ﾀﾞﾐｰ			*/
	unsigned long	pt;						/* 	ﾎﾟｲﾝﾄ数 (累計): 今現在までの累計ﾎﾟｲﾝﾄ  */
	unsigned long	pt_ari_kaiinsu;			/*	累計ﾎﾟｲﾝﾄあり会員総数		*/
	unsigned long	chotama;				/* 	貯玉数  玉数	*/
	unsigned long 	chomed;					/* 	貯玉数  ﾒﾀﾞﾙ数	*/
	unsigned long	chotama_ari_kaiinsu;	/*	貯玉保有会員総数	*/
	unsigned long 	chomeda_ari_kaiinsu;		/*	貯ﾒﾀﾞﾙ保有会員総数	*/
	unsigned long	chotama_yen;			/* 	貯玉数  玉数(円換算)	*/
	unsigned long 	chomed_yen;				/* 	貯玉数  ﾒﾀﾞﾙ数(円換算)	*/
	unsigned short 	today_ptup;    			/* 	本日ﾎﾟｲﾝﾄ数 増分(+) 	*/
    unsigned short 	today_ptdown;    		/* 	本日ﾎﾟｲﾝﾄ数 減分(-)  */
    long	today_ptupdown;			/*	本日ﾎﾟｲﾝﾄ数	増減分	 */
    unsigned long 	today_chotamacnt;		/* 	(本日)貯玉利用回数   */
    unsigned long	today_chomedacnt;			/*	(本日)貯ﾒﾀﾞﾙ利用回数   */
    unsigned long 	today_reptamacnt;		/* 	(本日)再ﾌﾟﾚｰ(玉)回数        */
    unsigned long 	today_repmedacnt;		/* 	(本日)再ﾌﾟﾚｰ(ﾒﾀﾞﾙ)回数        */
    unsigned long 	today_reptamasu;		/* 	(本日)再ﾌﾟﾚｰ玉数        */
    unsigned long 	today_repmedasu;		/* 	(本日)再ﾌﾟﾚｰﾒﾀﾞﾙ数        */
    unsigned long 	today_reptama_yen;		/* 	(本日)再ﾌﾟﾚｰ玉数(円換算) */
    unsigned long 	today_repmeda_yen;		/* 	(本日)再ﾌﾟﾚｰﾒﾀﾞﾙ数(円換算) */
    unsigned long	today_reptamatesu;		/* 	(本日)再ﾌﾟﾚｰ手数玉料     */
    unsigned long	today_repmedatesu;		/* 	(本日)再ﾌﾟﾚｰ手数ﾒﾀﾞﾙ料     */
    unsigned long	today_reptamatesu_yen;	/* 	(本日)再ﾌﾟﾚｰ手数玉料(円換算)        */
    unsigned long	today_repmedatesu_yen;	/* 	(本日)再ﾌﾟﾚｰ手数ﾒﾀﾞﾙ料(円換算)        */
    unsigned long 	today_chotamain;		/* 	(本日)貯玉受入数   */
    unsigned long 	today_chomedain;		/* 	(本日)貯ﾒﾀﾞﾙ受入数   */
    unsigned long 	today_chotamain_yen;	/* 	(本日)貯玉受入数(円換算)    */
    unsigned long 	today_chomedain_yen;	/* 	(本日)貯ﾒﾀﾞﾙ受入数(円換算)    */
    unsigned long 	today_chotamaout;		/* 	(本日)景品で貯玉引落数     */
    unsigned long 	today_chomedaout;		/* 	(本日)景品で貯ﾒﾀﾞﾙ引落数     */
    unsigned long 	today_chotamaout_yen;	/* 	(本日)景品で貯玉引落数(円換算)    */
    unsigned long 	today_chomedaout_yen;	/* 	(本日)景品で貯ﾒﾀﾞﾙ引落数(円換算)    */
    long	today_chotamaupdown;	/*	(本日)貯玉増減数	*/
    long	today_chomedaupdown;	/*	(本日)貯ﾒﾀﾞﾙ増減数	*/
    unsigned short	today_ptup_kaiinsu;		/*	(本日)ﾎﾟｲﾝﾄ増加人数	*/
    unsigned short	today_ptdown_kaiinsu;	/*	(本日)ﾎﾟｲﾝﾄ引出人数	*/
    unsigned short	today_ptupdown_kaiinsu;		/*	(本日)ﾎﾟｲﾝﾄ増減人数	*/
    unsigned short	today_chotamain_kaiinsu;	/*	(本日)貯玉預入人数	*/
    unsigned short	today_reptama_kaiinsu;		/*	(本日)再ﾌﾟﾚｲ人数	*/
    unsigned short	today_reputesutama_kaiinsu;	/*	(本日)再ﾌﾟﾚｲ手数料引出人数	*/
    unsigned short	today_chotamaout_kaiinsu;	/*	(本日)貯玉引出人数	*/
    unsigned short	today_chotamaupdown_kaiinsu;	/*	(本日)貯玉増減人数	*/
    unsigned short	today_chomedain_kaiinsu;	/*	(本日)貯ﾒﾀﾞﾙ預入人数	*/
    unsigned short	today_repmeda_kaiinsu;		/*	(本日)再ﾒﾀﾞﾙ人数	*/
    unsigned short	today_reputesumeda_kaiinsu;	/*	(本日)再ﾒﾀﾞﾙ手数料引出人数	*/
    unsigned short	today_chomedaout_kaiinsu;	/*	(本日)貯ﾒﾀﾞﾙ引出人数	*/
    unsigned short	today_chomedaupdown_kaiinsu;/*	(本日)貯ﾒﾀﾞﾙ増減人数	*/
    unsigned short 	dumy3;
    
    unsigned long	pt_ave;						/* 	ﾎﾟｲﾝﾄ数 (累計平均)  */
    unsigned short 	today_ptup_ave;    			/* 	本日ﾎﾟｲﾝﾄ数 増分(+)(平均) 	*/
    unsigned short 	today_ptdown_ave;    		/* 	本日ﾎﾟｲﾝﾄ数 減分(-)(平均)  */
    long	today_ptupdown_ave;			/*	本日ﾎﾟｲﾝﾄ数	増減分(平均)	 */
    
    unsigned long	chotama_ave;				/* 	貯玉数(平均)	*/
    unsigned long 	today_chotamain_ave;		/* 	(本日)貯玉受入数(平均)   */
    unsigned long 	today_reptamasu_ave;		/* 	(本日)再ﾌﾟﾚｰ玉数(平均)  */
    unsigned long	today_reptamatesu_ave;		/* 	(本日)再ﾌﾟﾚｰ手数玉料(平均)     */
    unsigned long 	today_chotamaout_ave;		/* 	(本日)景品で貯玉引落数(平均)      */
    unsigned long	today_chotamaupdown_ave;	/*	(本日)貯玉増減数(平均)		*/
    
    unsigned long	chomeda_ave;				/* 	貯ﾒﾀﾞﾙ数(平均)	*/
    unsigned long 	today_chomedain_ave;		/* 	(本日)貯ﾒﾀﾞﾙ受入数(平均)   */
    unsigned long 	today_repmedasu_ave;		/* 	(本日)再ﾒﾀﾞﾙ数(平均)  */
    unsigned long	today_repmedatesu_ave;		/* 	(本日)再ﾒﾀﾞﾙ手数料(平均)     */
    unsigned long 	today_chomedaout_ave;		/* 	(本日)景品で貯ﾒﾀﾞﾙ引落数(平均)      */
    unsigned long	today_chomedaupdown_ave;	/*	(本日)貯ﾒﾀﾞﾙ増減数(平均)		*/
    
    
    unsigned long	chotama_ave_yen;				/* 	貯玉数(平均)(円換算)	*/
    unsigned long 	today_chotamain_ave_yen;		/* 	(本日)貯玉受入数(平均)(円換算)   */
    unsigned long 	today_reptamasu_ave_yen;		/* 	(本日)再ﾌﾟﾚｰ玉数(平均)(円換算)  */
    unsigned long	today_reptamatesu_ave_yen;		/* 	(本日)再ﾌﾟﾚｰ手数玉料(平均)(円換算)     */
    unsigned long 	today_chotamaout_ave_yen;		/* 	(本日)景品で貯玉引落数(平均)(円換算)      */
    unsigned long	today_chotamaupdown_ave_yen;	/*	(本日)貯玉増減数(円換算)		*/
    
    unsigned long	chomeda_ave_yen;				/* 	貯ﾒﾀﾞﾙ数(平均)(円換算)	*/
    unsigned long 	today_chomedain_ave_yen;		/* 	(本日)貯ﾒﾀﾞﾙ受入数(平均)(円換算)   */
    unsigned long 	today_repmedasu_ave_yen;		/* 	(本日)再ﾒﾀﾞﾙ数(平均)(円換算)  */
    unsigned long	today_repmedatesu_ave_yen;		/* 	(本日)再ﾒﾀﾞﾙ手数料(平均)(円換算)     */
    unsigned long 	today_chomedaout_ave_yen;		/* 	(本日)景品で貯ﾒﾀﾞﾙ引落数(平均)(円換算)      */
    unsigned long	today_chomedaupdown_ave_yen;	/*	(本日)貯ﾒﾀﾞﾙ増減数(円換算)		*/
    
    unsigned char	dumy4[92];				/*	ﾀﾞﾐｰ	*/
} pos_cydmnip_t;


typedef struct {
	unsigned long date;								/* 年月日 */
	unsigned short time;							/* 時間 */
	unsigned char stype;							/* 処理区分 */
	unsigned char dmy1;								/* 予備１ */
	unsigned long kkaiin_no1;						/* 会員番号１ */
	unsigned long kkaiin_no2;						/* 会員番号２ */
	unsigned char dmy2[8];							/* 予備２  */
} T_MEMBER_HISTORY;


/* 島端I/F設定ﾃﾞｰﾀ構造体 */
typedef struct {
	WORD tesu;
	WORD harai;
} T_SRIF_BUTTON;


/* 島端I/F設定ﾃﾞｰﾀ構造体 */
typedef struct {
	BYTE syubetu;
	BYTE pt;
	WORD max_tama;
	WORD max_med;
	T_SRIF_BUTTON tama[6];
	T_SRIF_BUTTON med[6];
} T_SRIF_CONF;


/*---[ アプリケーションワーク]-----------------------------------------------*/

/* 自POS バージョン情報 */
extern DWORD pos_prog_ver;						/* POSプログラムバージョン */
extern DWORD pos_prog_sum;						/* POSプログラムCRC */
extern DWORD tp_prog_ver;						/* TPプログラムバージョン */
extern DWORD tp_prog_sum;						/* TPプログラムCRC */
extern DWORD pos_setfile_sum;					/* 設定ファイルSUM */
extern DWORD pos_mstfile_date;					/* マスターファイル日付 */
extern DWORD pos_mstfile_time;					/* マスターファイル時間 */

/* 自POS ステータスフラグ */
extern BYTE pos_boot_flag;						/* POS正常起動後にTRUE */
extern BYTE pos_parent_flag;					/* 親POSの場合TRUE */
extern BYTE pos_offline_flag;					/* オフラインモードでTRUE */
extern BYTE pos_total_flag;						/* 集計完了時TRUE */
extern BYTE pos_recvok_flag;					/* HALNET電文受信OK状態でTRUE */

extern BYTE pos_open_flag;						/* 営業状態でTRUE */
extern BYTE pos_change_flag;					/* 交換可能状態でTRUE */
extern BYTE pos_hakken_flag;					/* 発券可能状態でTRUE */
extern BYTE pos_clear_flag;						/* クリア状態でTRUE */
extern BYTE pos_rec_flag;						/* 記憶状態でTRUE */
extern BYTE pos_htpsts_flag;					/* ファイル転送中でTRUE */
extern BYTE pos_sys_rec_flag;					/* システム－記憶処理状態でTRUE */
extern BYTE pos_sys_clr_flag;					/* システム－クリア処理状態でTRUE */
extern BYTE pos_pclr_flag;						/* OPEN前クリア処理状態でTRUE */
extern BYTE pos_testmode_flag;					/* テスト交換モードでTRUE */
extern BYTE pos_stock_flag;						/* 棚卸処理区分フラグ
													0:自POS
													1:全店
													2:景品棚卸画面を開いていない
													*/
extern BYTE pos_sansho_flag;					/* 在庫参照、印字範囲(0:自POS、1:全店) */
extern BYTE pos_skip_flag;					  /* スキップあり/なしフラグ(0:なし、1:あり) */
extern BYTE prn_noskip_all_flag;			/* 全景品中締め(ぜんぶ)印字フラグ */
extern WORD	all_item_nid[ITEM_MAX];			/* スキップあり時の全景品のnID格納 */
extern WORD	skip_item_count;				/* スキップあり時の景品カウント数 */
extern WORD all_noskip_bumonID; 		/* 全景品中締めスキップなし時使用 */
extern WORD bumon_prn_title_flag; 		/* 部門別印字のタイトル印字する/しないに使用 */
extern BYTE pos_group_no; 						/* POSのグループNoに使用 */
extern BYTE force_stop_flag;					/* TP強制停止状態でTRUE */
extern BYTE saving_shop_no;						/* 貯玉店舗No 0:A店 1:B店 */
extern BYTE saving_join_flag;					/* 貯玉店舗合算フラグ 0:合算なし 1:合算あり*/
extern BYTE point_erea_flag;					/* ポイントアクセスフラグ 0:合算エリア 1:来店ポイントエリア*/
extern BYTE point_join_flag;					/* ポイント店舗合算フラグ 0:合算なし 1:合算あり*/
extern BYTE welcome_point;						/* 来店ポイント値*/
extern BYTE other_no_move;						/* 交換画面へ自動移行時に、安定するまで他画面へ移行できなくするフラグ 1：移行不可 */
extern BYTE exe_all_nakajime_flag;		/* (親POS)全店全景品中締め印字を行ったかのフラグ 0:閉店処理 1：全景品中締め */
extern BYTE rec_all_nakajime_flag;		/* (子POS)全店全景品中締め電文を受信したかのフラグ 0:閉店処理 1：全景品中締め */
extern BYTE kind_syubetu;							/* 使用する種別フラグ(4種別) */
extern BYTE physic_syubetu;						/* 各種別の物理種別(4種別) */
extern BYTE all_kind_syubetu;					/* 全店分の使用する種別フラグ(4種別) */
extern BYTE all_physic_syubetu;				/* 全店分の各種別の物理種別(4種別) */
extern BYTE disp_syubetu_char[4][TYPE_NAME_LEN + 1];		/* 各種別の文字列(A,B店各4種別) */
extern BYTE select_popup_str[4];			/* 種別選択POPUP文字列1～4(予約語の値がセットされる) */


/* 親POS バージョン情報 */
extern DWORD ppos_prog_ver;						/* POSプログラムバージョン */
extern DWORD ppos_prog_sum;						/* POSプログラムCRC */
extern DWORD ptp_prog_ver;						/* TPプログラムバージョン */
extern DWORD ptp_prog_sum;						/* TPプログラムCRC */
extern DWORD ppos_setfile_sum;					/* 設定ファイルSUM */
extern DWORD ppos_mstfile_date;					/* マスターファイル日付 */
extern DWORD ppos_mstfile_time;					/* マスターファイル時間 */

/* 親POS ステータスフラグ */
extern BYTE ppos_open_flag;						/* 営業状態でTRUE */
extern BYTE ppos_change_flag;					/* 交換可能状態でTRUE */
extern BYTE ppos_hakken_flag;					/* 発券可能状態でTRUE */
extern BYTE ppos_clear_flag;					/* クリア状態でTRUE */
extern BYTE ppos_rec_flag;						/* 記憶状態でTRUE */
extern BYTE ppos_htpsts_flag;					/* ファイル転送中でTRUE */

/* CAステータスフラグ */
extern DWORD ca_syukei_flag;					/* CA集計中にTRUE */

/* POSPCステータスフラグ */
extern WORD pospc_syukei_state;				/* POSPC集計処理状態 */
extern BYTE	 pospc_filler_flag;					/* POSPCへのフィラー送信フラグ */
extern WORD pos_close_state;					/* 親POS閉店処理状態 */

/*
 *
 *	システムワーク
 *
 */
extern char* update_time;						/* プログラム日付 */
extern BYTE admin_no;							/* 責任者番号(0～ADMIN_MAX-1)*/
extern BYTE pos_no;								/* POS番号(0～POS_MAX-1) */
extern BYTE close_error_id;						/* 閉店処理エラー機器ARC_ID */
extern DWORD err_machine;						/* 閉店処理エラー番号
													POS:bit0～bit7(POS1～POS8) 統計データエラー
													    bit8～bit15(POS1～POS8) 在庫データエラー
													TP :bit0～bit31(TP1～TP32) TPデータエラー
													CDS:bit0～bit7(CDS1～CDS8) CDSデータエラー
													*/
extern BYTE master_ok_flag;						/* TRUE:ﾏｽﾀ受信処理OK bit0=景品、bit1=責任者 */




/*
 *
 *	交換部
 *	グローバルワーク
 *
 */
extern ulong total_ball;						/* 累計入力玉(貯玉含む) */
extern ulong totalex_ball;						/* 累計入力玉(貯玉含まず) */
extern ulong left_ball;							/* 残玉(貯玉含む) */
extern ulong leftex_ball;						/* 残玉(貯玉含まず) */
extern ulong rest_ball;							/* 余玉(貯玉含む) */
extern ulong saveorg_ball;						/* 貯玉(玉)交換開始数(1種別目) */
extern ulong saveorg_ar;							/* 貯玉(玉)交換開始数(2種別目) */
extern ulong saveorg_slot;						/* 貯玉(スロット)交換開始数(3種別目) */
extern ulong saveorg_ex;							/* 貯玉(スロット)交換開始数(4種別目) */
extern ulong arng_num;
extern ulong saveorg;							/* 貯玉(玉・スロット)開始数 */
extern ulong savein;							/* 貯玉(玉・スロット)預入数 */
extern ulong saveout;							/* 貯玉(玉・スロット)引出数 */
extern ulong savesim;							/* 貯玉(玉・スロット)結果数 */
extern uint ticket_num;							/* チケット数 */
extern int pas_type;							/* 処理中PAS種別 */
extern int change_cursor;						/* 交換部カーソル */
extern int change_top;							/* 交換部表示トップ番号 */
extern int change_level;						/* 交換レベル */
extern int seg_level;							/* ７セグレベル */
extern BYTE change_auto_flag;					/* 自動再計算フラグ */
extern BYTE change_ronly_flag;					/* Read Onlyフラグ */
extern BYTE change_card_flag;					/* 会員カード挿入フラグ */
extern BYTE card_on;							/* 貯玉処理済み 	*/
extern BYTE change_savings_flag;				/* 貯玉指定フラグ */
extern int tokushu_items;						/* 特殊景品数(0～TOKUSHU_MAX)*/
extern pos_item_t tokushu_item[TOKUSHU_MAX];	/* 特殊景品データ */
extern ulong tokushu_num[TOKUSHU_MAX];			/* 特殊景品自動計算個数 */
extern ulong tokushu_select[TOKUSHU_MAX];		/* 特殊景品確定個数 */
extern change_tmp_t change_tmp[CHANGE_MAX];		/* 交換用テンポラリ */
extern WORD	mars_harai_num[2][TOKUSHU_MAX];		/* 景品1,2,3,4の払出し数 */
extern back_rireki_t back_rireki[BACK_ITEM_MAX];	/* 返品、戻し制限構造体 */
extern uchar tokushu_dec_flag;					/* 交換単価小数点対応 */
extern uchar ks_errcode;						/* KSから受信したｴﾗｰｺｰﾄﾞ */
extern uchar cds_cardout_flag;					/* TRUE:CDSｶｰﾄﾞ排出ｴﾗｰ中 */
extern uchar yesterday_close_flag;				/* 前日閉店処理確認フラグ(TRUE:確認済み) */
extern uchar g_glory_status_flag;					/* グローリ状態チェックフラグ 0:行う 1:行わない */
extern uchar chg_horyuu_admin_flag;				/* 交換(保留券発券)中に責任者カードが抜かれた? 1:抜かれた */


/*
 *
 *	構造体関連(常時ワークに保持するもの)
 *
 */
extern pos_basic_t pos_basic;					/* POS基本設定 */
extern pos_const_t pos_const;					/* POS定数設定 */
extern pos_toukei_t pos_toukei;					/* この機器の統計データ */
extern harai_setting_t pos_harai;				/* 接続払出機の情報 */
extern pos_group_t pos_group;					/* ネットワークグループ */
extern pos_setting_t pos_setting[POS_MAX];		/* 各POSの情報 */
extern tp_setting_t tp_setting[TP_MAX];			/* 各TPの情報 */
extern pos_zaiko_t b_zaiko_blk[9];


/*----------------------------------------------------------------------------
 *
 * 99.2.15 以降追加分
 *
 *--------------------------------------------------------------------------*/

#define ADMINISTRATOR_MAX	ADMIN_MAX			/* 責任者最大数 */
#define ADMIN_LIMIT_LEVEL_MAX	6				/* 操作制限レベル最大数 */
#define ADMIN_LIMIT_HEAD_MAX	64				/* 操作制限項目最大数 */

#if 0
/*
 * 責任者情報登録設定
 */
typedef struct {
	short num;				/* 番号 */
	short level;			/* 操作レベル */
	unsigned char name[16];	/* 責任者名		*/
} admin_info_t;

extern admin_info_t admin_info_work[ADMINISTRATOR_MAX];
extern char admin_limit[ADMIN_LIMIT_LEVEL_MAX][ADMIN_LIMIT_HEAD_MAX];
extern admin_info_t administrator;
#endif

#if 0
/*
 * 読込みチケット情報(01/04/05)
 */
typedef struct {
	BYTE 	barcode[BC_CODE_LEN];		/* バーコードデータ */
	BYTE 	req_flag;					/* 照会処理フラグ(TRUE:照会中) */
	clock_t	timer;						/* 照会結果判明後のタイマ */
} ticket_data_t;

extern ticket_data_t	ticket_work;
#endif

/*
 *	責任者制限情報内部ワーク構造体(22byte)
 */
typedef struct {
	unsigned char gcode;		/* グループコード */
	unsigned char code;			/* 責任者コード */
	unsigned long unyou;		/* 運用処理 */
	unsigned long change;		/* 交換処理 */
	unsigned long zaiko;		/* 在庫管理処理 */
	unsigned long kaiin;		/* 会員情報処理 */
	unsigned long config;		/* 設定変更処理 */
} admin_operation_t;

extern admin_operation_t admin_limit;		/* 責任者制限情報 */
extern uchar poscs_flag;					/* != 0:KSカードイン済み通知 */
extern uchar transaction_no;			/* トランザクション番号 */

#endif	/* _pos_work_h_ */
