#ifndef __INIFILEGET_H_INCLUDED__
#define __INIFILEGET_H_INCLUDED__
//------------------------------------------------------------
//概要
//   システムＩｎｉファイル関係
//
//作成者
//   Tokai水谷
//
//備考
//
//   システムＩｎｉファイル関係の定義
//------------------------------------------------------------

#include <windows.h>
//------------------------------------------------------------
//	Define宣言
//------------------------------------------------------------
//	システムＩｎｉファイルＰａｔｈ
//#define	DEF_INIFILENAME ".\\WinSousa.ini"

//	セクション名

//	キー名

//	デフォルト値



//------------------------------------------------------------
//	構造体宣言
//------------------------------------------------------------
typedef struct {						// システム情報テーブル
	long System;						// システム (0:HC-1000,!0:HAL-300i)
	long ModelNo;						// モデル番号
	long EmgTerm;						// 緊急用端末(0=no 1=yes)
	long LineMode;						// ラインモード(0:OFFLINE,1:ONLINE)
	long StopPrint;						// 印字の有無
	long KeyboardType;					// ｷｰﾎﾞｰﾄﾞﾀｲﾌﾟ (0:HC-1000,1:HAL-300i)
	long ConnectPtn;					// 接続パターン（0:自動接続,1:手動接続）
	char DefaultWss[MAX_PATH];			// 主操作のPC名
	long TerminalType;					// 運用端末の使用タイプ	(0:通常の運端として使用 1.リモートターミナルとして使用)
	long KadoCntType;					// 稼動数カウント （0:毎時00分30分はＡＳのファイルから取得 1:操作でカウントする）
	long BestDataPtn;					// ベスト台ファイルの送信 0:ベスト台・おすすめ台ファイルのみ送信	1:コーナーベスト台等のファイルも送信
	long ReadKishuTypeSet;				// MPC機種タイプ設定取込機能 0:なし/1:あり 20051101 add tomida
	long MaskBeltErr;					// 帯エラーマスク 0:マスクしない/1:マスク 20060106 tomida
	long SystemTime;					// システム時刻	0:WSSに合わせる/1:ASに合わせる 20060127 hashimoto
	long SystemTimeSetMinutes;			// SetTime=0(WSS→ASに合わせる)の時、システム時刻合わせを開店n分前までとする
	long ConnectDaiLamp;				// 台ランプ接続	0:無/1:有 20060202 hashimoto
	long ConnectIB;						// ｲﾝﾌｫﾒｰｼｮﾝﾎﾞｰﾄﾞ接続	0:無/1:有 20060202 hashimoto
	long PowerOFFWarningHours;			// 電源切り忘れで警告表示する時間	2007.08.02 add hashimoto(T2X003966)
	long DeadLockTime;					// デッドロック判定時間(分)		※ 処理間隔1分まで
	long DeadLockCnt;					// デッドロック判定回数(これに処理間隔時間をかけて経過時間と比較)	※ 処理間隔2分以上
	long MaskKakudaiKeisu;				// 各台計数マスク設定
	long FtcChkKakudaiKeisuData;		// FTC各台計数データを取得する/しない設定 0:取得しない　1:取得する(デフォルト)
										// ※1:取得する場合でも、主操作且つGetCnctFTC()==1の場合のみ動作対象となります。
	long StartUpGFTP;					// GFTP起動			0:起動しない　1:起動	
	long StartUpSquareTCP;				// SquareTCP起動	0:起動しない　1:起動

} SYSINIFILE_SYSTEM;

typedef struct {
	long StopHasseiInji;				// 発生印字停止
} SYSINIFILE_DEBUG;
/*		** HC2007 定数変更 **
typedef struct {						// 種別表示テーブル
//	char moji[SHUBETSU][8];				// 種別文字 1次バージョンが３で定義してあるので 2002.05.22 koketsu
	char moji[4][8];					// 種別文字	ここは４固定にしておく 2002.05.22 koketsu
} SYSINIFILE_SHUBETSU;
*/
typedef struct {						// パス情報テーブル
	char Base[_MAX_PATH];				// 基本パス
	char Online[_MAX_PATH];				// オンラインパス
	char Tantai[_MAX_PATH];				// 単体パス
	char Kioku[_MAX_PATH];				// 記憶パス
	char RecoverKioku[_MAX_PATH];		// リカバリ記憶パス
	char Log[_MAX_PATH];				// ログパス
	char Kpc[_MAX_PATH];				// ＫＰＣパス
	char Init[_MAX_PATH];				// イニシャパス
	char Unyou[_MAX_PATH];				// 運用パス
	char Arakajime[_MAX_PATH];			// 予めパス
	char UnyouKari[_MAX_PATH];			// 運用仮パス
	char ArakajimeKari[_MAX_PATH];		// 予め仮パス
	char Mpc[_MAX_PATH];				// MPCパス
	char Share[_MAX_PATH];				// shareパス
	char Hn2[_MAX_PATH];				// HN2パス
	char Csv[_MAX_PATH];				// Csvパス
	char Files[_MAX_PATH];				// Filesパス
	char Ftc[_MAX_PATH];				// Ftcパス
	char TempGL01[_MAX_PATH];			// TempGL/DSP/01パス
	char TempGL02[_MAX_PATH];			// TempGL/DSP/02パス
} SYSINIFILE_PATH;

typedef struct {						// ＡＳ通信ＩＰ情報テーブル
	char Ipadr[16];						// IPアドレス
	long Rport;							// リードポートアドレス
	long Wport;							// ライトポートアドレス
} SYSINIFILE_IP;

typedef struct {
	int nLogLevel;						// ログ レベル
} LOG_SECTION;

typedef struct{
	char dsp1copypath1[MAX_PATH];	// DSP1データファイルコピー先1
	char dsp1copypath2[MAX_PATH];	// DSP1データファイルコピー先2
	char dsp1copypath3[MAX_PATH];	// DSP1データファイルコピー先3
	char dsp1copypath4[MAX_PATH];	// DSP1データファイルコピー先4
	char dsp1copypath5[MAX_PATH];	// DSP1データファイルコピー先5
	char dsp2copypath1[MAX_PATH];	// DSP2データファイルコピー先1
	char dsp2copypath2[MAX_PATH];	// DSP2データファイルコピー先2
	char dsp2copypath3[MAX_PATH];	// DSP2データファイルコピー先3
	char dsp2copypath4[MAX_PATH];	// DSP2データファイルコピー先4
	char dsp2copypath5[MAX_PATH];	// DSP2データファイルコピー先5
} HALLINFO_DSP;

typedef struct{
	long MakeTeijiData;				// 定時稼働データ作成
	long TeijiDataKind;				// 定時稼働データ種類
} HALLINFO_KISHU_TEIJI_KADO;

typedef struct {
	SYSINIFILE_SYSTEM sys;				// システム情報
	SYSINIFILE_DEBUG dbg;				// デバッグ用
//	SYSINIFILE_SHUBETSU sbt;			// 種別表示			** HC2007 定数変更 **
	SYSINIFILE_PATH path;				// パス情報
	SYSINIFILE_IP fltAs1;				// ファイル通信　ＡＳ＃１
	SYSINIFILE_IP fltAs2;				// ファイル通信　ＡＳ＃２
	SYSINIFILE_IP fltAs3;				// ファイル通信　ＡＳ＃３
	SYSINIFILE_IP fltAs4;				// ファイル通信　ＡＳ＃４
	SYSINIFILE_IP fltAs5;				// ファイル通信　ＡＳ＃５
	SYSINIFILE_IP opeAs1;				// 運用通信通信　ＡＳ＃１
	LOG_SECTION secLog;					// [Log]セクション
	HALLINFO_DSP dsp;
	HALLINFO_KISHU_TEIJI_KADO KishuTeijiKado;	// hallinfo.ini[機種別定時稼動数]
} SYSINIFILE;
typedef SYSINIFILE SysIniFile;

//------------------------------------------------------------
//	プロトタイプ宣言
//------------------------------------------------------------
extern BOOL GetSystemIniFile(SysIniFile *sysIniFile);

#endif	//	__INIFILEGET_H_INCLUDED__