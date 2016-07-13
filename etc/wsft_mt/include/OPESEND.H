#ifndef __OPESEND_H__
#define __OPESEND_H__

#include "nascom.h"

/************************************************************/
/*	設定用定数												*/
/************************************************************/

//iniファイル用文字列
#define INISEC_OPESEND	"Opesend"
#define INISEC_LOG		"LOG"
#define INIKEY_PORT		"Port"
#define INIKEY_TIMEOUT	"timeoutsec"
#define INIKEY_RETRY	"retrycount"
#define INIKEY_CLIENT	"clientID"
#define INIKEY_IP		"addr"
#define INIKEY_PATH		"path"
#define INIKEY_SIZE		"tracemaxsize"
#define INIKEY_LEVEL	"tracelevel"
#define INIKEY_SAVED	"tracesaved"
#define INIKEY_VER		"tracever"


//デフォルト値
#define CLIENT_ID_MIN	96

//VBからのオプション指定文字列
#define TIMEOUT		"OSOPT_TIMEOUT"
#define RETRY		"OSOPT_RETRY"
#define CLIENT		"OSOPT_CLIENT"
#define PORT		"OSOPT_PORT"
#define IPADDRESS	"OSOPT_IP"

/************************************************************/
/*	エラー定数												*/
/************************************************************/

#define OPEERR_BUSY			0x0001		//排他書込みによりブロックされた
#define OPEERR_TIMEOUT		0x2010		//通信時のタイムアウトエラー
#define OPEERR_CAST			0x2004		//型変換時に上限・下限値を超える等、正しく変換できなかった
#define OPEERR_SEND			0x2005		//リクエスト電文が送れない
#define OPEERR_COMINFO		0x2006		//cominfo_t構造体のエラーコードが0でない
#define OPEERR_ANSWERSIZE	0x2007		//受け取った電文のサイズ異常
#define OPEERR_WSSTARTUP	0x3001		//Winsock初期化時のエラー
#define OPEERR_NOTYETREADY	0x3002		//DLLを初期化する前に、他のDLL関数を呼び出した
#define OPEERR_DOUBLESTART	0x3003		//二重に初期化しようとした
#define OPEERR_LOGPATH		0x3004		//ログファイルのパスが指定されていない
#define OPEERR_MAXSIZE		0x3005		//分割サイズが不正、または指定されていない
#define OPEERR_TRACELEVEL	0x3006		//トレースレベルが不正、または指定されていない
#define OPEERR_SAVEDDATE	0x3007		//保持日数が不正、または指定されていない
#define OPEERR_TRACEVER		0x3008		//ログ出力DLLのバージョンが不正、または指定されていない
#define OPEERR_TRACESTART	0x3009		//TraceLogStart関数が失敗した
#define OPEERR_INIPORT		0x3010		//ポート番号が不正、または指定されていない
#define OPEERR_INITIMEOUT	0x3011		//タイムアウトが不正、または指定されていない
#define OPEERR_INIRETRY		0x3012		//リトライ回数が不正、または指定されていない
#define OPEERR_CLIENTID		0x3013		//通信元号機番号が不正、または指定されていない
#define OPEERR_IPADDRESS	0x3014		//IPアドレスが指定されていない
#define OPEERR_DOUBLEEND	0x4001		//二重に解放しようとした
#define OPEERR_SETPORT		0x5001		//不正なポート番号を設定しようとした
#define OPEERR_SETWAIT		0x5002		//不正な受信タイムアウト時間を設定しようとした
#define OPEERR_SETRETRY		0x5003		//不正なリトライ回数を設定しようとした
#define OPEERR_SETCLIENT	0x5004		//不正な通信元号機番号を設定しようとした
#define OPEERR_NOTEXISTOPT	0x5005		//存在しないオプションを設定しようとした
#define OPEERR_NOTGETHC		0x5006		//ASのホスト情報が取得できなかった
#define OPEERR_CREATESOCK	0x5007		//socketが作成できなかった
#define OPEERR_SETIP		0x5008		//IPアドレスに-1を設定しようとした
#define OPEERR_OPENMEM		0x5009		//共有メモリのオープンエラー
#define OPEERR_MAPVIEW		0x5010		//共有メモリのマップビューエラー
#define OPEERR_UNMAPVIEW	0x5011		//共有メモリのアンマップビューエラー
#define OPEERR_STOP			0x5012		//オペレータによって処理が中止されました。
#define OPEERR_ARG			0x5013		//データ異常
#define OPEERR_BIND			0x5014		//ｿｹｯﾄに名前を付けられなかった **02.03.18 misako**
#define OPEERR_INVALID_ARGUMENT	0x5015	//引数が不正

/* 0500~0503 データ部（発生コード、情報)(dat):  */
typedef struct{
	unsigned char h_code1;			/* 発生コード１ */
	unsigned char h_code2;			/* 発生コード２ */
	unsigned short info1;			/* 情報１ */
	unsigned short info2;			/* 情報２ */
	unsigned short info3;			/* 情報３ */
	unsigned short info4;			/* 情報４ */
	unsigned short info5;			/* 情報５ */
	unsigned short yobi1;			/* 情報１ */
	unsigned short yobi2;			/* 情報２ */
	unsigned short yobi3;			/* 情報３ */
	unsigned short yobi4;			/* 情報４ */
	unsigned short yobi5;			/* 情報５ */
	unsigned short yobi6;			/* 情報６ */
} dat05XX_t;

/* 0500 運用動作要求通信(req):  */
typedef struct{
	comhed_t	ch;					/* 共通ﾍｯﾀﾞ:  */
	dat05XX_t	dat0500;			/* データ部（発生、情報）*/
} req0500_t;

/* 0500~0503 データ部２（動作データ)(dat)  */
typedef struct{
	unsigned char mode;				/* 動作モード */
	unsigned char claim;			/* 動作要求 */
	unsigned char d_Flg1;			/* 動作フラグ１ */
	unsigned char d_Flg2;			/* 動作フラグ２ */
	unsigned long Yobi;				/* 予備  */
} dat05XX_t2;


/* 0500 運用動作要求通信(ans):  */
typedef struct{
	comhed_t	ch;					/* 共通ﾍｯﾀﾞ:  */
	dat05XX_t	dat0500;			/* データ部（発生、情報）*/			
	dat05XX_t2	dat0500T;			/* データ部２（動作データ） */
} ans0500_t;


/* 0501 音声制御要求通信(req):  */
typedef struct{
	comhed_t ch;					/* 共通ﾍｯﾀﾞ:  */
	dat05XX_t	dat0501;			/* データ部（発生、情報）*/	
} req0501_t;


/* 0501 音声制御要求通信(ans):  */
typedef struct{
	comhed_t ch;					/* 共通ﾍｯﾀﾞ:  */
	dat05XX_t	dat0501;			/* データ部（発生、情報）*/	
	dat05XX_t2	dat0501T;			/* データ部２（動作データ）*/
} ans0501_t;


/* 0503 ダウンロード指示通信(req):  */
typedef struct{
	comhed_t ch;					/* 共通ﾍｯﾀﾞ:  */
	dat05XX_t	dat0503;			/* データ部（発生、情報）*/	
} req0503_t;


/* 0503 ダウンロード指示通信(ans):  */
typedef struct{
	comhed_t ch;					/* 共通ﾍｯﾀﾞ:  */
	dat05XX_t	dat0503;			/* データ部（発生、情報）*/	
	dat05XX_t2	dat0503T;			/* データ部２（動作データ） */
} ans0503_t;


/* 0508 データ部(dat):  */
typedef struct{
	unsigned char sound_kind;
	unsigned char dmy;
	unsigned short chime;
	unsigned char simano[160];
} dat0508_t;

/* 0508 ホール演出音声通信(req):  */
typedef struct{
	comhed_t ch;
	dat0508_t dat;
} req0508_t;

/* 0508 ホール演出音声通信(ans):  */
typedef struct{
	comhed_t ch;
	char result;
	char dmy[3];
} ans0508_t;


/* 0509 データ部(dat) */
typedef struct{
	unsigned char pat;		// 制御パターン	0:全店 1:論理種別 2:島 3:機種 4:範囲台
	unsigned char dmy[11];
	unsigned char flg[1536];	// 制御フラグ	0:切替なし 1:切替あり (未接続の台は0)
} dat0509_t;

/* 0509 台間TV CH切替要求(req) */
typedef struct{
	comhed_t ch;
	dat0509_t dat;
} req0509_t;

/* 0509 台間TV CH切替要求(ans) */
typedef struct{
	comhed_t ch;
	unsigned char result;
	unsigned char dmy[11];
} ans0509_t;

/* 050A データ部（発生コード、情報)(dat):  */
typedef struct{
	unsigned char result;			/* 結果 0x00: 正常（固定） */
	unsigned char dmy;				/* 予備 */
	unsigned short info1;			/* 補給機設定手動取得フラグ 
									　 bit0: 補給機1 設定取得結果 [0]OK [1]NG
									　 bit1: 補給機1 設定取得結果 [0]OK [1]NG
									　 bit2: 補給機1 設定取得結果 [0]OK [1]NG
									　 bit3: 補給機1 設定取得結果 [0]OK [1]NG
									　 bit4: 補給機1 設定取得結果 [0]OK [1]NG
									　 bit5: 補給機1 設定取得結果 [0]OK [1]NG
									　 bit6: 補給機1 設定取得結果 [0]OK [1]NG
									　 bit7: 補給機1 設定取得結果 [0]OK [1]NG */
	unsigned long info2;			/* 補給機設定取得時刻: 年・月・日・時・分・曜日: bit7・4・5・5・6・5 1910+年・0=日曜 */
	unsigned short info3;			/* 情報３ */
	unsigned short info4;			/* 情報４ */
	unsigned short info5;			/* 情報５ */
	unsigned short yobi1;			/* 予備１ */
	unsigned short yobi2;			/* 予備２ */
	unsigned short yobi3;			/* 予備３ */
	unsigned short yobi4;			/* 予備４ */
	unsigned short yobi5;			/* 予備５ */
} dat050A_t;

typedef struct{
	comhed_t	ch;					/* 共通ﾍｯﾀﾞ:  */
	dat050A_t	dat050A;			/* データ部（発生、情報）*/			
} ans050A_t;

//---------------------------------------------------------------------
//	VB用通信構造体

//共通ヘッダ
typedef comhed_t COMHEAD;

/* 0500~0503 情報予備データ(dat):	 */
typedef struct{
	unsigned char h_code1;			/* 発生コード１ */
	unsigned char h_code2;			/* 発生コード２ */
	long info1;						/* 情報１ */
	long info2;						/* 情報２ */
	long info3;						/* 情報３ */
	long info4;						/* 情報４ */
	long info5;						/* 情報５ */
	long yobi1;						/* 予備１ */
	long yobi2;						/* 予備２ */
	long yobi3;						/* 予備３ */
	long yobi4;						/* 予備４ */
	long yobi5;						/* 予備５ */
	long yobi6;						/* 予備６ */
} DAT05XX_T;

/* 0500 運用動作要求通信(req):  */
typedef struct{
	COMHEAD		ch;					/* 共通ﾍｯﾀﾞ:  */
	DAT05XX_T	dat0500;			/* データ部(発生コード、情報) */	
} REQ0500;

/* 0500~0503 動作データ(dat):):  */
typedef struct{
	unsigned char mode;				/* 動作モード */
	unsigned char claim;			/* 動作要求 */
	unsigned char d_Flg1;			/* 動作フラグ１ */
	unsigned char d_Flg2;			/* 動作フラグ２ */
	long Yobi;						/* 予備  */	
} DAT05XX_T2;

/* 0500 運用動作要求通信(ans):  */
typedef struct{
	COMHEAD	 ch;					/* 共通ﾍｯﾀﾞ:  */
	DAT05XX_T	dat0500;			/* データ部(発生コード、情報) */
	DAT05XX_T2	dat0500T;			/* データ部２（動作データ）*/
} ANS0500;

/* 050A 運用動作要求通信(ans):  */
typedef struct{
	COMHEAD		ch;			/* 共通ﾍｯﾀﾞ:  */
	dat050A_t	dat050A;	/* 050A データ部 */
} ANS050A;

//---------------------------------------------------------------------
//	関数プロトタイプ

#if defined(OPESEND_EXPORTS)
#define OPESEND_API	__declspec(dllexport) __stdcall
#else
#define OPESEND_API	__declspec(dllimport) __stdcall
#endif

int OPESEND_API ope_start(HWND hwnd);
int OPESEND_API ope_setserv(unsigned long addr, int report);
int OPESEND_API ope_setopt(char *opt, int value);
int OPESEND_API ope_getopt(char *opt);
int OPESEND_API ope_end();
int OPESEND_API ope_getver();
int OPESEND_API ope_setserv_str(const char* addr, int nRport);
int OPESEND_API ope_setsockopt(int timeout);
int OPESEND_API ope_0500_vb(REQ0500 *req, ANS0500 *ans);
int OPESEND_API ope_050A_vb(REQ0500 *req, ANS050A *ans, int retry_count, int timeout_second);
int OPESEND_API ope_0500(req0500_t *req, ans0500_t *ans);
int OPESEND_API ope_0501(req0501_t *req, ans0501_t *ans);
int OPESEND_API ope_0503(req0503_t *req, ans0503_t *ans);
int OPESEND_API ope_0508(const req0508_t *req, ans0508_t *ans);
int OPESEND_API ope_ff01(reqff01_t *req, ansff01_t *ans);
int OPESEND_API ope_ff02(reqff02_t *req, ansff02_t *ans);
int OPESEND_API ope_ff03(reqff03_t *req, ansff03_t *ans);

#endif
