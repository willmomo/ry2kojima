/*****************************************************************************/
/* 	インクルードファイル													 */
/*****************************************************************************/
#include <windows.h>
#include <windowsx.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>							/* _kbhit関数用 */
#include <time.h>							/* 乱数発生用 */
#include "denbun_struct.h"					/* 電文struct */
#include "pdpctrl.h"						/* 電文送受信 */
#include "emu_test.h"						/* プロトタイプ宣言 */
#include "def.h"							/* 暗号化関数 */


/*****************************************************************************/
/* 	外部変数 宣言															 */
/*****************************************************************************/
/* ini情報 */
char com[8];								/* OPENするCOMポート */
int maker;									/* 払出機メーカー */
int loop_cnt;								/* ループ回数 */
int init_interval;							/* 一定時間受信がなければ初期化する */

/* 全メーカー共通グローバル変数 */
HANDLE	hComm;								/* Comポートの設定 */
unsigned char p_send_buf[512];				/* POS送信手順用ﾊﾞｯﾌｧ */
unsigned char h_send_buf[512];				/* MPC送信手順用ﾊﾞｯﾌｧ */
unsigned char p_send_buf_open[512];				/* POS送信手順用ﾊﾞｯﾌｧ */
unsigned char h_send_buf_open[512];				/* MPC送信手順用ﾊﾞｯﾌｧ */
unsigned char p_send_buf_harai[512];				/* POS送信手順用ﾊﾞｯﾌｧ */
unsigned char h_send_buf_harai[512];				/* MPC送信手順用ﾊﾞｯﾌｧ */
char loop_flag = 0;							/* ループフラグ */
char CsvFilePath[MAX_PATH];					/* メーカー毎のCSVファイル名 */
char CsvFilePathOpen[MAX_PATH];					/* メーカー毎のCSVファイル名 */
int sum_harai[2][4];						/* 初回から現在までの総払出数 */
char esc_flag;								/* 終了フラグ */
char connect_flag;							/* 通信中かどうか */

/* GLORY用 */
char gl_empty[8];
static char gl_emp_flag = FALSE;

/* マース用 */
int mtanka[TOKUSHU_MAX];				/* 景品単価(小を10とする事) */
char mars_lane[TOKUSHU_MAX][LANE_MAX];				/* マースのレーン設定 */
int keihin_put[TOKUSHU_MAX];			/* 入力した分だけ払出数を-1する */
static char m_tanka_check = FALSE;		/* 0:単価チェックを行わない/1:単価チェックを行う */

/* 三共用 */
static char harai_data[256];			/* 払出要求電文データ先頭アドレス */


/*****************************************************************************/
/* 	送信テーブル															 */
/*****************************************************************************/
/* グローリー用送信テーブル */
char check1_glory_old[] = {STX, '0', 0x40, '0', '0', '0', '0', ETX, '0', '\0'};	/* 状態チェック1グローリー(旧) */
char check2_glory_old[] = {STX, 0x33, 0x40, '0', '0', '0', '0', ETX, '0', '\0'};	/* 状態チェック2グローリー(旧) */
char check1_glory_new[] = {STX, '0', 0x60, '0', '0', '0', 0x40, ETX, '0', '\0'};	/* 状態チェック1グローリー(新) */
char check2_glory_new[] = {STX, 0x33, 0x60, '0', '0', '0', 0x40, ETX, '0', '\0'};	/* 状態チェック2グローリー(新) */
/* 三共用送信テーブル */
char harai_sankyo[] = {STX, 
	'0', '0', '0', '0',		/* レーン1払出数[1]～[4] */
	'0', '0', '0', '0',		/* レーン2払出数[5]～[8] */
	'0', '0', '0', '0',		/* レーン3払出数[9]～[12] */
	'0', '0', '0', '0',		/* レーン4払出数[13]～[16] */
	'0', '0', '0', '0',		/* レーン5払出数[17]～[20] */
	'0', '0', '0', '0',		/* レーン6払出数[21]～[24] */
	'0', '0', '0', '0',		/* レーン7払出数[25]～[28] */
	'0', '0', '0', '0',		/* レーン8払出数[29]～[32] */
	'0', '0', '0', '0','0',	/* 払出残玉数[33]～[37] */
	ETX, '0', '\0'};		/* 三共 */

/* マース用送信テーブル */
char check_mars[] = {M_STX,
	'5', '1',									/* CMD */
	'0', '0',									/* 要求コード 00:正常/01:景品単価チェック要求 */
	'0', '0', '0', '0', '0', '0', '0', '0',		/* 予備 */
	M_ETX, '0', '0', CR, LF, '\0'};		/* マース */
char tanka_check_mars[] = {M_STX,
	'6', '0',							/* CMD */
	'0', '0',							/* 応答コード */
	'0', '0', '0', '0', '0', '0', '0', '0',		/* 予備 */
	M_ETX, '0', '0', CR, LF, '\0'};		/* マース */
char harai_mars[] = {M_STX,
	'5', '3',							/* CMD */
	'0', '0',							/* リターンコード */
	'0', '0', '0', '0', '0', '0',		/* 余り玉数 */
	'0', '0', '0', '0', '0', '0',		/* 景品1払出個数 */
	'0', '0', '0', '0', '0', '0',		/* 景品2払出個数 */
	'0', '0', '0', '0', '0', '0',		/* 景品3払出個数 */
	'0', '0', '0', '0', '0', '0',		/* 景品4払出個数 */
	M_ETX, '0', '0', CR, LF, '\0'};		/* マース */
char clr_mars[] = {M_STX,
	'5', '5',							/* CMD */
	'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', /* 予備 */
	M_ETX, '0', '0', CR, LF, '\0'};		/* マース */
char cal_mars[] = {M_STX,
	'6', '1',							/* CMD */
	'0', '0',							/* リターンコード */
	'0', '0', '0', '0', '0', '0', '0', '0', /* 予備 */
	M_ETX, '0', '0', CR, LF, '\0'};		/* マース */


char init_check1_glory_old[256];
char init_check1_glory_new[256];
char init_check2_glory_old[256];
char init_check2_glory_new[256];
char init_check_mars[256];
char init_tanka_check_mars[256];
char init_harai_mars[256];
char init_harai_sankyo[256];

/*****************************************************************************/
/* 	main関数																 */
/* 		Comポートを開いたら設定をmain_loopに渡す							 */
/*		ループを出たら終了したらComポートを閉じて処理を終了する				 */
/* 																			 */
/*****************************************************************************/
void main( void )
{
	int ret;

	/* iniファイルの読込み */
	readini();

	/* COMをOPENする */
	hComm = OpenComm();

	if (hComm != INVALID_HANDLE_VALUE) {
		/* オープン成功 */
		ret = csv_read_func();				/* 送受信手順CSVファイルの読込み */
		memcpy(p_send_buf, p_send_buf_harai, sizeof(p_send_buf));
		memcpy(h_send_buf, h_send_buf_harai, sizeof(h_send_buf));
		if (ret == -1) {
			return;							/* CSVファイル読込み失敗 */
		}
		
		for(;;){
			ret = main_loop();
			if (ret != 0) {
				break;
			}
		}
		printf("終了\n");
		CloseHandle(hComm);		/* Comポートを閉じる */

	} else {
		printf("オープンエラー\n");
	}

}

/*****************************************************************************/
/* 	メインループ															 */
/* 		電文受信					r_data(data, sizeof(data))				 */
/*		CRCﾁｪｯｸ						crc_cal(data, strlen(data))				 */
/*		電文Verﾁｪｯｸ					ver_check(data, ver)					 */
/*		ﾒｰｶｰｺｰﾄﾞﾁｪｯｸ				mcd_check(data, mcd)					 */
/*		暗号化/解除(1:ON, 0:OFF)	cmd_cnvrt(data, strlen(data), dai_no)	 */
/*		電文解析					rdata_anal(data)						 */
/* 																			 */
/*****************************************************************************/
int main_loop()
{
	int rec, send_ret;
	char data[256];				/* 電文データ格納用 */
	int ch;
	static int i = 0;
	FILE *fp;
	static char m_first_flag = TRUE;	/* 初回の状態チェック(MARSのみ単価チェックを行う為必要) */
	static char timeout_check = FALSE;	/* タイマー監視しない */
	static clock_t t1;

/********************************************************************************
	受信処理
********************************************************************************/

	/* 受信電文取得 */
	memset(data, 0x00, sizeof(data));		/* 受信電文用変数の初期化 */
	rec = RecvData(hComm, data, sizeof(data));

	if (rec > 0) {							/* 電文受信 */
		/* TIMER監視スタート */
		t1 = clock();
		timeout_check = TRUE;

		/* マース専用開店動作 */
		if(maker == H_MARS){
			if(connect_flag == FALSE){
				/* 通信中ではない場合は初回通信なのでどのCSVから動作させるか判断させる */
				if(p_send_buf_harai[ptr_count] == data[0]){
					/* 払出動作で確定させる */
					memcpy(p_send_buf, p_send_buf_harai, sizeof(p_send_buf));
					memcpy(h_send_buf, h_send_buf_harai, sizeof(h_send_buf));
				}else if(p_send_buf_open[ptr_count] == data[0]){
					/* 開店動作で確定させる */
					memcpy(p_send_buf, p_send_buf_open, sizeof(p_send_buf));
					memcpy(h_send_buf, h_send_buf_open, sizeof(h_send_buf));
				}else{
					if((p_send_buf_harai[ptr_count] == STATUS1) || (p_send_buf_harai[ptr_count] == STATUS2)){
						/* コマンドチェック */
						if((data[0] == 'S') && (data[1] == '0') && (data[2] == '1')){
							/* 払出要求で確定 */
							memcpy(p_send_buf, p_send_buf_harai, sizeof(p_send_buf));
							memcpy(h_send_buf, h_send_buf_harai, sizeof(h_send_buf));
						}
					}else if(p_send_buf_harai[ptr_count] == HARAI){
						/* コマンドチェック */
						if((data[0] == 'S') && (data[1] == '0') && (data[2] == '3')){
							/* 払出要求で確定 */
							memcpy(p_send_buf, p_send_buf_harai, sizeof(p_send_buf));
							memcpy(h_send_buf, h_send_buf_harai, sizeof(h_send_buf));
						}
					}
					if(p_send_buf_open[ptr_count] == CLR){
						/* コマンドチェック */
						if((data[0] == 'S') && (data[1] == '0') && (data[2] == '5')){
							/* 開店処理で確定 */
							memcpy(p_send_buf, p_send_buf_open, sizeof(p_send_buf));
							memcpy(h_send_buf, h_send_buf_open, sizeof(h_send_buf));
						}
					}else if(p_send_buf_open[ptr_count] == CAL){
						/* コマンドチェック */
						if((data[0] == 'S') && (data[1] == '1') && (data[2] == '1')){
							/* 開店処理で確定 */
							memcpy(p_send_buf, p_send_buf_open, sizeof(p_send_buf));
							memcpy(h_send_buf, h_send_buf_open, sizeof(h_send_buf));
						}
					}
				}
			}
		}else{
			/* その他のメーカー */
			memcpy(p_send_buf, p_send_buf_harai, sizeof(p_send_buf));
			memcpy(h_send_buf, h_send_buf_harai, sizeof(h_send_buf));
		}

		connect_flag = TRUE;
		/* 送信手順通りか？ */
		if(p_send_buf[ptr_count] != data[0]){
			/* 手順通りではないのでreturn */
			if(m_tanka_check == TRUE){
				if((data[0] == M_STX) && (data[1] == '1') && (data[2] == '0')){
					/* 単価設定がきたのでマースのみ特別にここで単価応答を送信する */
					make_tanka_mars(data);
					send_mtanka(data);
					/* 単価要求ONのときは単価チェックしているのでチェックする */
					m_tanka_check = FALSE;
					return 0;
				}else{
					/* 単価設定が来るまでここでリターンする */
					disp_log_cmd(data, RECEIVE, LOG, rec);
					return 0;
				}
			}
			if((p_send_buf[ptr_count] == STATUS1) || (p_send_buf[ptr_count] == STATUS2)){
				/* 状態チェック */
				switch(data[0]){
				case M_STX:
					if((data[0] == M_STX) && (data[1] == '0') && (data[2] == '1')){
						if((maker == H_MARS) && (m_first_flag == TRUE)){
							m_first_flag = FALSE;
							m_tanka_check = TRUE;	/* マース用単価要求フラグ */
						}
					}else{
						rece_err_flag = TRUE;
					}
					break;
				case STX:
					/* STXなので処理なし */
					break;
				default:
					rece_err_flag = TRUE;
					break;
				}
			}else if(p_send_buf[ptr_count] == HARAI){
				/* 払出要求 */
				switch(data[0]){
				case M_STX:
					if((data[0] == M_STX) && (data[1] == '0') && (data[2] == '3')){
						/* 電文正常なので払出要求を保存 */
						memcpy(harai_data, data, sizeof(data));
					}else{
						rece_err_flag = TRUE;
					}
					break;
				case STX:
					/* 電文正常なので払出要求を保存 */
					memcpy(harai_data, data, sizeof(data));
					break;
				default:
					/* 電文異常 */
					rece_err_flag = TRUE;
					break;
				}
			}else if((p_send_buf[ptr_count] == CLR) || (p_send_buf[ptr_count] == CAL)){
				/* クリア要求orカレンダー設定コマンド */
				switch(data[0]){
				case M_STX:
					if(p_send_buf[ptr_count] == CLR){
						if((data[0] != M_STX) || ((data[1] != '0') || (data[2] != '5'))){
							/* 電文異常 */
							rece_err_flag = TRUE;
						}
					}else if(p_send_buf[ptr_count] == CAL){
						if((data[0] != M_STX) || ((data[1] != '1') || (data[2] != '1'))){
							/* 電文異常 */
							rece_err_flag = TRUE;
						}
					}
					break;
				case STX:
					break;
				default:
					/* 電文異常 */
					rece_err_flag = TRUE;
					break;
				}
			}else{
				rece_err_flag = TRUE;
			}
		}

		/* 受信電文をログへ出力 */
		/* disp_log_cmd(data, RECEIVE, DISP, rec); */
		disp_log_cmd(data, RECEIVE, LOG, rec);

		if(rece_err_flag == TRUE){
			/* 電文異常なのでreturn */
			rece_err_flag = FALSE;
			return 0;
		}

		/* 払出要求の場合のみ画面に払出個数の出力を行う */
		if(p_send_buf[ptr_count] == HARAI){
			disp_harai(data);
		}
		/* クリア要求の場合払出個数のクリアを行う */
		if(p_send_buf[ptr_count] == CLR){
			printf("クリア要求受信したので総払出個数をクリアします。\n");
			memset(sum_harai, 0x00, sizeof(sum_harai));
		}
		/* カレンダセット要求の場合のみ画面に日付の出力を行う */
		if(p_send_buf[ptr_count] == CAL){
			disp_cal(data);
		}


/********************************************************************************
		送信処理
********************************************************************************/

		/*
		 *	送信処理
		 *	CSVﾌｧｲﾙに記述されている通りに送信する
		 */
		switch(h_send_buf[ptr_count]){
		case ACK:
			/* ACK送信 */
			disp_log((char)ACK);
			send_ret = send_ack();
			break;
		case NAK:
			/* NAK送信 */
			disp_log((char)NAK);
			send_ret = send_nak();
			break;
		case EOT:
			/* EOT送信 */
			disp_log((char)EOT);
			send_ret = send_eot();
			break;
		case ENQ:
			/* ENQ送信 */
			disp_log((char)ENQ);
			send_ret = send_sa();
			send_ret = send_enq();
			break;
		case SYN:
			/* SYN送信 */
			disp_log((char)SYN);
			send_ret = send_syn();
			break;
		case STATUS1:
			/* ｺﾏﾝﾄﾞ送信(払出要求前状態チェック) */
			disp_log((char)STATUS1);
			send_ret = send_status(0);
			break;
		case STATUS2:
			/* ｺﾏﾝﾄﾞ送信(払出要求後状態チェック) */
			disp_log((char)STATUS2);
			send_ret = send_status(1);
			break;
		case HARAI:
			/* ｺﾏﾝﾄﾞ送信 */
//			disp_log_cmd((char)HARAI);
//			disp_log_cmd(data, SEND, LOG, rec);
			send_ret = send_harai();
			break;
		case CLR:
			/* ｺﾏﾝﾄﾞ送信 */
			disp_log((char)CLR);
			send_ret = send_clr();
			break;
		case CAL:
			/* ｺﾏﾝﾄﾞ送信 */
			disp_log((char)CAL);
			send_ret = send_cal();
			break;
		case NO_REC:
			/* 無応答 */
			disp_log((char)NO_REC);
			send_ret = 0;
			break;
		default:
			/* 記述内容そのまま送信 */
			disp_log((char)h_send_buf[ptr_count]);
			send_ret = send_data(ptr_count);
			break;
		}
		/* エラーが発生した場合は終了 */
		if(send_ret == -1){
			printf("送信エラーで処理を終了します。(0x%08x)\n", GetLastError());
			return -1;
		}else if(send_ret == -2){
			/* 受信待機中にESCｷｰ入力 */
			return -1;
		}
		/* 送受信ﾊﾞｯﾌｧを一つ移動 */
		ptr_count++;
	} else if (rec == -1) {					/* 受信エラー */
		printf("受信エラーで処理を終了します。(0x%08x)\n", GetLastError());
		return -1;
	} else {
		/* 受信なし */
		/* タイマーをチェックして一定時間受信なければ初期化する */
		if(timeout_check == TRUE){
			if((clock() - t1) > init_interval){
				/* 初期化 */
				data_init();
				timeout_check = FALSE;
				m_first_flag = TRUE;
				printf("%dmsec受信がなかったので通信手順を初期化します。\n", init_interval);
				return 0;
			}
		}
	}

	/* 送受信が一通り終了したら最初に戻る */
	if(p_send_buf[ptr_count] == NULL){
		/* 完了 */
		fp = fopen("data.log", "a");
		fputs("\n", fp);
		fclose(fp);
		timeout_check = FALSE;	/* タイマー監視しない */
		data_init();	/* 変数を初期状態へ */
	}



	/*
	 *	キー入力処理を移動
	 */
	if (_kbhit() != 0) {				/* キー入力が発生した場合 */
		ch = getch();
		if (ch == 0x1b) {
			if (esc_flag == TRUE){
				/* 2回連続でESC入力なので終了 */
				return -1;
			}
			/* 1回で通信の初期化 */
			data_init();
			printf("通信手順の初期化を行いました。\n");
			esc_flag = TRUE;
		}
		switch(maker){
		case H_MARS:
			/* マースの時のみ再投出用入力を可能とする */
			if (ch == '1'){
				keihin_put[0]++;
				printf("景品1の払出数を-%dします。\n", keihin_put[0]);
			}
			if (ch == '2'){
				keihin_put[1]++;
				printf("景品2の払出数を-%dします。\n", keihin_put[1]);
			}
			if (ch == '3'){
				keihin_put[2]++;
				printf("景品3の払出数を-%dします。\n", keihin_put[2]);
			}
			if (ch == '4'){
				keihin_put[3]++;
				printf("景品4の払出数を-%dします。\n", keihin_put[3]);
			}
			if ((ch == 'c') || (ch == 'C')){
				/* 入力した情報をクリア */
				memset(keihin_put, 0x00, sizeof(keihin_put));
				printf("景品の-払出数をクリアします。\n");
			}
			if( (ch == 'i') || (ch == 'I') ){
				/* 入力した情報を確認 */
				printf("(合計)景品1の払出数を-%dします。\n", keihin_put[0]);
				printf("(合計)景品2の払出数を-%dします。\n", keihin_put[1]);
				printf("(合計)景品3の払出数を-%dします。\n", keihin_put[2]);
				printf("(合計)景品4の払出数を-%dします。\n", keihin_put[3]);
			}
			break;
		case H_GLORY_OLD:
		case H_GLORY_NEW:
			/* 数字に対応した景品を景品無しとする */
			switch(ch){
			case '1':
				gl_empty[0] = TRUE;
				printf("景品1をENPTYにします。\n");
				break;
			case '2':
				gl_empty[1] = TRUE;
				printf("景品2をENPTYにします。\n");
				break;
			case '3':
				gl_empty[2] = TRUE;
				printf("景品3をENPTYにします。\n");
				break;
			case '4':
				gl_empty[3] = TRUE;
				printf("景品4をENPTYにします。\n");
				break;
			case '5':
				gl_empty[4] = TRUE;
				printf("景品5をENPTYにします。\n");
				break;
			case '6':
				gl_empty[5] = TRUE;
				printf("景品6をENPTYにします。\n");
				break;
			case '7':
				gl_empty[6] = TRUE;
				printf("景品7をENPTYにします。\n");
				break;
			case '8':
				gl_empty[7] = TRUE;
				printf("景品8をENPTYにします。\n");
				break;
			default:
				break;
			}
			
			if ((ch == 'c') || (ch == 'C')){
				/* 入力した情報をクリア */
				memset(gl_empty, 0x00, sizeof(gl_empty));
				printf("景品のEMPTY情報をクリアします。\n");
			}
			if( (ch == 'i') || (ch == 'I') ){
				/* 入力した情報を確認 */
				printf("景品1のEMPTY=%d\n", gl_empty[0]);
				printf("景品2のEMPTY=%d\n", gl_empty[1]);
				printf("景品3のEMPTY=%d\n", gl_empty[2]);
				printf("景品4のEMPTY=%d\n", gl_empty[3]);
				printf("景品5のEMPTY=%d\n", gl_empty[4]);
				printf("景品6のEMPTY=%d\n", gl_empty[5]);
				printf("景品7のEMPTY=%d\n", gl_empty[6]);
				printf("景品8のEMPTY=%d\n", gl_empty[7]);
			}
			break;
		default:
			/* 特別処理無し */
			break;
		}
	}

	return 0;
}

/*****************************************************************************/
/* 	iniファイル読込み用関数													 */
/*****************************************************************************/
void readini(void)
{
	char iniFile[MAX_PATH];
	GetModuleFileName(NULL, iniFile, MAX_PATH);
	memcpy(CsvFilePath, iniFile, sizeof(iniFile));
	memcpy(CsvFilePathOpen, iniFile, sizeof(iniFile));
	PathRemoveFileSpec(CsvFilePath);
	PathRemoveFileSpec(CsvFilePathOpen);
	PathRenameExtension(iniFile, ".ini");

	/* 各グローバル変数の初期化 */
	memset(com, 0x00, sizeof(com));
	memset(sum_harai, 0x00, sizeof(sum_harai));
	memset(harai_data, 0x00, sizeof(harai_data));
	memset(keihin_put, 0x00, sizeof(keihin_put));
	memset(gl_empty, 0x00, sizeof(gl_empty));
	maker = 0;
	memcpy(init_check1_glory_old, check1_glory_old, sizeof(check1_glory_old));
	memcpy(init_check1_glory_new, check1_glory_new, sizeof(check1_glory_new));
	memcpy(init_check2_glory_old, check2_glory_old, sizeof(check2_glory_old));
	memcpy(init_check2_glory_new, check2_glory_new, sizeof(check2_glory_new));
	memcpy(init_check_mars, check_mars, sizeof(check_mars));
	memcpy(init_tanka_check_mars, tanka_check_mars, sizeof(tanka_check_mars));
	memcpy(init_harai_mars, harai_mars, sizeof(harai_mars));
	memcpy(init_harai_sankyo, harai_sankyo, sizeof(harai_sankyo));
	connect_flag = FALSE;



	// iniファイルからOPENするCOMの読込み
	GetPrivateProfileString("HARAI", "COM", "COM1", com, sizeof(com), iniFile);
	printf("%s\n", &com[0]);

	// iniファイルから動作させる払出機メーカーの取得
	maker = GetPrivateProfileInt("HARAI", "harai_maker", 0, iniFile);
	switch(maker){
	case H_GLORY_OLD:
		PathAppend(CsvFilePath, "glory.csv");
//		PathAppend(CsvFilePathOpen, "glory_open.csv");
		printf("払出機メーカー：グローリー(旧)\n");
		break;
	case H_GLORY_NEW:
		PathAppend(CsvFilePath, "glory.csv");
//		PathAppend(CsvFilePathOpen, "glory_open.csv");
		printf("払出機メーカー：グローリー(新)\n");
		break;
	case H_SANKYO:
		PathAppend(CsvFilePath, "sankyo.csv");
//		PathAppend(CsvFilePathOpen, "sankyo_open.csv");
		printf("払出機メーカー：三共\n");
		break;
	case H_MARS:
		PathAppend(CsvFilePath, "mars.csv");
		PathAppend(CsvFilePathOpen, "mars_open.csv");
		printf("払出機メーカー：マース\n");
		break;
	case H_HIROTAC:
		PathAppend(CsvFilePath, "glory.csv");
//		PathAppend(CsvFilePathOpen, "glory_open.csv");
		printf("払出機メーカー：ヒロタック\n");
		break;
	case H_SUBARU:
		PathAppend(CsvFilePath, "glory.csv");
//		PathAppend(CsvFilePathOpen, "glory_open.csv");
		printf("払出機メーカー：スバル\n");
		break;
	case H_TMI:
		PathAppend(CsvFilePath, "glory.csv");
//		PathAppend(CsvFilePathOpen, "glory_open.csv");
		printf("払出機メーカー：TMI\n");
		break;
	default:
		PathAppend(CsvFilePath, "glory.csv");
//		PathAppend(CsvFilePathOpen, "glory_open.csv");
		printf("払出機メーカー：グローリー(デフォルト)\n");
		break;
	}

	loop_cnt = GetPrivateProfileInt("HARAI", "loop", 10, iniFile);
	loop_cnt--;	/* 1回分余分なので・・・ */
	init_interval = GetPrivateProfileInt("HARAI", "timer", 5000, iniFile);
	printf("---------------------------------------------\n");
}


/*****************************************************************************/
/* 	CSVファイル読込み用関数													 */
/*																			 */
/* 		<ret>																 */
/*			(int)			-1: エラー	0: 正常								 */
/*																			 */
/*****************************************************************************/
int csv_read_func( void )
{
	int ret;

	/* 払出用CSVファイルの読込み */
	ret = read_csv_harai();
	if(ret == -1){
		/* ファイルが読み込めない場合 */
		printf("送信手順ファイル読込みエラーのため、処理を終了します。\n");
		return -1;
	}
	/* 開店処理用CSVファイルの読込み */
	if(maker == H_MARS){
		ret = read_csv_open();
		if(ret == -1){
			/* ファイルが読み込めない場合 */
			printf("送信手順ファイル読込みエラーのため、処理を終了します。\n");
			return -1;
		}
	}
	return 0;
}

/*****************************************************************************/
/* 	CSVファイル読込み用関数													 */
/*																			 */
/* 		<ret>																 */
/*			(int)			-1: エラー	0: 正常								 */
/*																			 */
/*****************************************************************************/
int read_csv_harai( void )
{
	FILE *fp;				/* CSVファイル読込み用 */
	char lbuf[1024];		/* データ用ﾊﾞｯﾌｧ（[,]や改行コードを含む） */
	char d_buf[64];
	int len;				/* データ長 */
	int pcnt, hcnt, plcnt, hlcnt;			/* データ数 */
	int i, j, k;
	BOOL harai_send, pos_send, skip_flag;
	int data;				/* ﾊﾞｯﾌｧ格納変数 */
	unsigned char p_send_loop_buf[512];			/* POS送信手順用ﾊﾞｯﾌｧ */
	unsigned char h_send_loop_buf[512];			/* MPC送信手順用ﾊﾞｯﾌｧ */

	/* csvﾌｧｲﾙの読込(メーカー毎に切り替える) */
	if ( (fp = fopen(CsvFilePath, "r")) == NULL){
		/* ファイルが読み込めない場合 */
		return -1;
	}

	/* データの初期化 */
	pcnt = 0;
	hcnt = 0;
	plcnt = 0;
	hlcnt = 0;
	pos_send = FALSE;
	harai_send = FALSE;
	skip_flag = FALSE;
	data = 0;
	memset(lbuf, 0x00, sizeof(lbuf));	/* lbufの初期化 */
	memset(p_send_buf_harai, NULL, sizeof(p_send_buf_harai));	/* p_send_buf_haraiの初期化 */
	memset(h_send_buf_harai, NULL, sizeof(h_send_buf_harai));	/* h_send_buf_haraiの初期化 */

	while(fgets(lbuf, sizeof(lbuf), fp) != NULL){		/* データの読込み */
		if ( (lbuf[0] == 0x0a) || (lbuf[0] == 0x0d) ){	/* 改行コードは読み飛ばす */
			continue;
		}
		len = 0;										/* データ長の初期化 */
		i = strlen(lbuf);								/* サイズの取得 */
		for(j = 0; j < i; j++){							/* サイズ分ループ */
			if(lbuf[j] == '#'){							/* [#]は読み飛ばす */
				if(lbuf[j+1] == 'P'){
					/* POSの送信データ */
					pos_send = TRUE;
					harai_send = FALSE;
				}else if(lbuf[j+1] == 'H'){
					/* 払出機の送信データ */
					pos_send = FALSE;
					harai_send = TRUE;
				}else{
					break;
				}
				skip_flag = TRUE;
			}
			if ( (lbuf[j] == ',') || (lbuf[j] == 0x0a) || (lbuf[j] == 0x0d) ) {	/* [,]と改行コード */
				skip_flag = FALSE;
				if (len != 0) {							/* データ長が0以外 */
					/* 記述したｺﾏﾝﾄﾞﾁｪｯｸ */
					if(len == 3){
						data = get_send_csv(lbuf, j);
						if(data == SKIP){
							len = 0;
							if(pos_send == TRUE){
								if(loop_flag == LOOP_ED){
									/* コピーしたものを挿入 */
									for(k=0;k<loop_cnt;k++){
										memcpy(&(p_send_buf_harai[pcnt]), &(p_send_loop_buf[0]), plcnt);
										pcnt += plcnt;
									}
									plcnt = 0;
									loop_flag = LOOP_NONE;
								}
							}
							if(harai_send == TRUE){
								if(loop_flag == LOOP_ED){
									/* コピーしたものを挿入 */
									for(k=0;k<loop_cnt;k++){
										memcpy(&(h_send_buf_harai[hcnt]), &(h_send_loop_buf[0]), hlcnt);
										hcnt += hlcnt;
									}
									hlcnt = 0;
									loop_flag = LOOP_NONE;
								}
							}
							continue;
						}
					}else if(len == 1){
						if(lbuf[j-1] == '-'){
							/* 無応答 */
							data = NO_REC;
						}
					}else{
						/* 数値入力とtextコマンド入力の２通りある */
						/* 0xで始まる場合は数値入力とし、STXで始まる場合はﾌｧｲﾙ名の入力とする */
						if((lbuf[j-len] == '0') && (lbuf[j-len+1] == 'x')){
							/* 数値入力 */
							for(k = 2; k < len; k++){
								if('0' <= lbuf[j-len+k] && lbuf[j-len+k] <= '9'){
									d_buf[k-2] = lbuf[j-len+k] - '0';
								}else if('a' <= lbuf[j-len+k] && lbuf[j-len+k] <= 'f'){
									d_buf[k-2] = lbuf[j-len+k] - 'a' + 10;
								}
							}
							data = (d_buf[0] << 4) + d_buf[1];
						}
						if((lbuf[j-len] == 'S') && (lbuf[j-len+1] == 'T') && (lbuf[j-len+2] == 'A')){
							/* STATUS(状態チェック) */
							if(lbuf[j-len+6] == '1'){
								data = STATUS1;
							}else{
								data = STATUS2;
							}
						}
						if((lbuf[j-len] == 'H') && (lbuf[j-len+1] == 'A') && (lbuf[j-len+2] == 'R')){
							/* HARAI(払出要求) */
							data = HARAI;
						}
					}
					if(pos_send == TRUE){
						/* POSの送信手順ﾊﾞｯﾌｧへｺﾋﾟｰ */
						memcpy(&(p_send_buf_harai[pcnt]), &data, sizeof(char));			/* p_bufへのデータのコピー */
						if(loop_flag == LOOP_ST){
							/* コピー開始 */
							memcpy(&(p_send_loop_buf[plcnt]), &data, sizeof(char));			/* p_buf_loopへのデータのコピー */
							plcnt++;
						}
						pcnt++;
					}
					if(harai_send == TRUE){
						/* MPCの送信手順ﾊﾞｯﾌｧへｺﾋﾟｰ */
						memcpy(&(h_send_buf_harai[hcnt]), &data, sizeof(char));			/* h_bufへのデータのコピー */
						if(loop_flag == LOOP_ST){
							/* コピー開始 */
							memcpy(&(h_send_loop_buf[hlcnt]), &data, sizeof(char));			/* h_buf_loopへのデータのコピー */
							hlcnt++;
						}
						hcnt++;
					}
					/* lenの初期化 */
					len = 0;
					continue;
				}
			} else {									/*	*/
				if(skip_flag == TRUE){
					continue;
				}
				len++;									/* データ長を+1する */
			}
		}
	}
	fclose(fp);
	return 0;
	
}

/*****************************************************************************/
/* 	CSVファイル読込み用関数													 */
/*																			 */
/* 		<ret>																 */
/*			(int)			-1: エラー	0: 正常								 */
/*																			 */
/*****************************************************************************/
int read_csv_open( void )
{
	FILE *fp;				/* CSVファイル読込み用 */
	char lbuf[1024];		/* データ用ﾊﾞｯﾌｧ（[,]や改行コードを含む） */
	char d_buf[64];
	int len;				/* データ長 */
	int pcnt, hcnt, plcnt, hlcnt;			/* データ数 */
	int i, j, k;
	BOOL harai_send, pos_send, skip_flag;
	int data;				/* ﾊﾞｯﾌｧ格納変数 */
	unsigned char p_send_loop_buf[512];			/* POS送信手順用ﾊﾞｯﾌｧ */
	unsigned char h_send_loop_buf[512];			/* MPC送信手順用ﾊﾞｯﾌｧ */

	/* csvﾌｧｲﾙの読込(メーカー毎に切り替える) */
	if ( (fp = fopen(CsvFilePathOpen, "r")) == NULL){
		/* ファイルが読み込めない場合 */
		return -1;
	}

	/* データの初期化 */
	pcnt = 0;
	hcnt = 0;
	plcnt = 0;
	hlcnt = 0;
	pos_send = FALSE;
	harai_send = FALSE;
	skip_flag = FALSE;
	data = 0;
	memset(lbuf, 0x00, sizeof(lbuf));	/* lbufの初期化 */
	memset(p_send_buf_open, NULL, sizeof(p_send_buf_open));	/* p_send_buf_openの初期化 */
	memset(h_send_buf_open, NULL, sizeof(h_send_buf_open));	/* h_send_buf_openの初期化 */

	while(fgets(lbuf, sizeof(lbuf), fp) != NULL){		/* データの読込み */
		if ( (lbuf[0] == 0x0a) || (lbuf[0] == 0x0d) ){	/* 改行コードは読み飛ばす */
			continue;
		}
		len = 0;										/* データ長の初期化 */
		i = strlen(lbuf);								/* サイズの取得 */
		for(j = 0; j < i; j++){							/* サイズ分ループ */
			if(lbuf[j] == '#'){							/* [#]は読み飛ばす */
				if(lbuf[j+1] == 'P'){
					/* POSの送信データ */
					pos_send = TRUE;
					harai_send = FALSE;
				}else if(lbuf[j+1] == 'H'){
					/* 払出機の送信データ */
					pos_send = FALSE;
					harai_send = TRUE;
				}else{
					break;
				}
				skip_flag = TRUE;
			}
			if ( (lbuf[j] == ',') || (lbuf[j] == 0x0a) || (lbuf[j] == 0x0d) ) {	/* [,]と改行コード */
				skip_flag = FALSE;
				if (len != 0) {							/* データ長が0以外 */
					/* 記述したｺﾏﾝﾄﾞﾁｪｯｸ */
					if(len == 3){
						data = get_send_csv(lbuf, j);
						if(data == SKIP){
							len = 0;
							if(pos_send == TRUE){
								if(loop_flag == LOOP_ED){
									/* コピーしたものを挿入 */
									for(k=0;k<loop_cnt;k++){
										memcpy(&(p_send_buf_open[pcnt]), &(p_send_loop_buf[0]), plcnt);
										pcnt += plcnt;
									}
									plcnt = 0;
									loop_flag = LOOP_NONE;
								}
							}
							if(harai_send == TRUE){
								if(loop_flag == LOOP_ED){
									/* コピーしたものを挿入 */
									for(k=0;k<loop_cnt;k++){
										memcpy(&(h_send_buf_open[hcnt]), &(h_send_loop_buf[0]), hlcnt);
										hcnt += hlcnt;
									}
									hlcnt = 0;
									loop_flag = LOOP_NONE;
								}
							}
							continue;
						}
					}else if(len == 1){
						if(lbuf[j-1] == '-'){
							/* 無応答 */
							data = NO_REC;
						}
					}else{
						/* 数値入力とtextコマンド入力の２通りある */
						/* 0xで始まる場合は数値入力とし、STXで始まる場合はﾌｧｲﾙ名の入力とする */
						if((lbuf[j-len] == '0') && (lbuf[j-len+1] == 'x')){
							/* 数値入力 */
							for(k = 2; k < len; k++){
								if('0' <= lbuf[j-len+k] && lbuf[j-len+k] <= '9'){
									d_buf[k-2] = lbuf[j-len+k] - '0';
								}else if('a' <= lbuf[j-len+k] && lbuf[j-len+k] <= 'f'){
									d_buf[k-2] = lbuf[j-len+k] - 'a' + 10;
								}
							}
							data = (d_buf[0] << 4) + d_buf[1];
						}
						if((lbuf[j-len] == 'S') && (lbuf[j-len+1] == 'T') && (lbuf[j-len+2] == 'A')){
							/* STATUS(状態チェック) */
							if(lbuf[j-len+6] == '1'){
								data = STATUS1;
							}else{
								data = STATUS2;
							}
						}
						if((lbuf[j-len] == 'H') && (lbuf[j-len+1] == 'A') && (lbuf[j-len+2] == 'R')){
							/* HARAI(払出要求) */
							data = HARAI;
						}
					}
					if(pos_send == TRUE){
						/* POSの送信手順ﾊﾞｯﾌｧへｺﾋﾟｰ */
						memcpy(&(p_send_buf_open[pcnt]), &data, sizeof(char));			/* p_bufへのデータのコピー */
						if(loop_flag == LOOP_ST){
							/* コピー開始 */
							memcpy(&(p_send_loop_buf[plcnt]), &data, sizeof(char));			/* p_buf_loopへのデータのコピー */
							plcnt++;
						}
						pcnt++;
					}
					if(harai_send == TRUE){
						/* MPCの送信手順ﾊﾞｯﾌｧへｺﾋﾟｰ */
						memcpy(&(h_send_buf_open[hcnt]), &data, sizeof(char));			/* h_bufへのデータのコピー */
						if(loop_flag == LOOP_ST){
							/* コピー開始 */
							memcpy(&(h_send_loop_buf[hlcnt]), &data, sizeof(char));			/* h_buf_loopへのデータのコピー */
							hlcnt++;
						}
						hcnt++;
					}
					/* lenの初期化 */
					len = 0;
					continue;
				}
			} else {									/*	*/
				if(skip_flag == TRUE){
					continue;
				}
				len++;									/* データ長を+1する */
			}
		}
	}
	fclose(fp);
	return 0;
	
}

/*****************************************************************************/
/* 	CSV記述内容取得関数														 */
/*			buf, cnt			CSV記述データ, 文字番号						 */
/* 		<ret>																 */
/*			データ															 */
/*				0x05:ENQ													 */
/*				0x04:EOT													 */
/*				0x06:ACK													 */
/*				0x15:NAK													 */
/*				0x02:STX													 */
/*				0x99:ERR													 */
/*****************************************************************************/
int get_send_csv(char *buf, int cnt)
{
	char data1, data2, data3;
	int ret;

	/* 初期化 */
	data1 = 0;
	data2 = 0;
	data3 = 0;
	ret = 0;

	/* 値の代入 */
	data1 = buf[cnt-3];
	data2 = buf[cnt-2];
	data3 = buf[cnt-1];

	if((data1 == 'E') && (data2 == 'N') && (data3 == 'Q')){
		/* ENQ */
		ret = ENQ;
	}else if((data1 == 'E') && (data2 == 'O') && (data3 == 'T')){
		/* EOT */
		ret = EOT;
	}else if((data1 == 'A') && (data2 == 'C') && (data3 == 'K')){
		/* ACK */
		ret = ACK;
	}else if((data1 == 'S') && (data2 == 'Y') && (data3 == 'N')){
		/* SYN */
		ret = SYN;
	}else if((data1 == 'N') && (data2 == 'A') && (data3 == 'K')){
		/* NAK */
		ret = NAK;
	}else if((data1 == 'L') && (data2 == 'S') && (data3 == 'T')){
		/* LOOPスタート(LEDまでを指定回数LOOPする) */
		loop_flag = LOOP_ST;
		ret = SKIP;
	}else if((data1 == 'L') && (data2 == 'E') && (data3 == 'D')){
		/* LOOPエンド */
		loop_flag = LOOP_ED;
		ret = SKIP;
	}else if((data1 == 'C') && (data2 == 'L') && (data3 == 'R')){
		/* クリア要求コマンド */
		ret = CLR;
	}else if((data1 == 'C') && (data2 == 'A') && (data3 == 'L')){
		/* カレンダー設定コマンド */
		ret = CAL;
	}else{
		/* 異常な記述 */
		ret = ERR;
	}

	return ret;
}

/*****************************************************************************/
/* 	data_init()																 */
/* 		変数初期化															 */
/*****************************************************************************/
int data_init( void )
{
	/* 送受信の最初へ戻る変数 */
	ptr_count = 0;
	memset(keihin_put, 0x00, sizeof(keihin_put));
	memset(gl_empty, 0x00, sizeof(gl_empty));
	esc_flag = FALSE;
	m_tanka_check = FALSE;
	gl_emp_flag = FALSE;
	memcpy(check1_glory_old, init_check1_glory_old, sizeof(check1_glory_old));
	memcpy(check1_glory_new, init_check1_glory_new, sizeof(check1_glory_new));
	memcpy(check2_glory_old, init_check2_glory_old, sizeof(check2_glory_old));
	memcpy(check2_glory_new, init_check2_glory_new, sizeof(check2_glory_new));
	memcpy(check_mars, init_check_mars, sizeof(check_mars));
	memcpy(tanka_check_mars, init_tanka_check_mars, sizeof(tanka_check_mars));
	memcpy(harai_mars, init_harai_mars, sizeof(harai_mars));
	memcpy(harai_sankyo, init_harai_sankyo, sizeof(harai_sankyo));
	connect_flag = FALSE;
	return 0;
}

/*****************************************************************************/
/* 	send_ack																 */
/* 		ACK電文送信															 */
/*****************************************************************************/
int send_ack( void )
{
	char data[4];
	int rec;

	memset(data, 0x00, sizeof(data));
	data[0] = ACK;
	rec = SendData(hComm, data);				/* 電文送信 */
	if (rec == -1) {
		/* 送信エラー */
		return -1;
	} else {
		/* 送信成功 */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_nak																 */
/* 		NAK電文送信															 */
/*****************************************************************************/
int send_nak( void )
{
	char data[4];
	int rec;

	memset(data, 0x00, sizeof(data));
	data[0] = NAK;
	rec = SendData(hComm, data);				/* 電文送信 */
	if (rec == -1) {
		/* 送信エラー */
		return -1;
	} else {
		/* 送信成功 */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_eot																 */
/* 		EOT電文送信															 */
/*****************************************************************************/
int send_eot( void )
{
	char data[4];
	int rec;

	memset(data, 0x00, sizeof(data));
	data[0] = EOT;
	rec = SendData(hComm, data);				/* 電文送信 */
	if (rec == -1) {
		/* 送信エラー */
		return -1;
	} else {
		/* 送信成功 */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_sa																 */
/* 		SA電文送信															 */
/*****************************************************************************/
int send_sa( void )
{
	char data[4];
	int rec;

	memset(data, 0x00, sizeof(data));
	data[0] = SA;
	rec = SendData(hComm, data);				/* 電文送信 */
	if (rec == -1) {
		/* 送信エラー */
		return -1;
	} else {
		/* 送信成功 */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_syn																 */
/* 		SYN電文送信															 */
/*****************************************************************************/
int send_syn( void )
{
	char data[4];
	int rec;

	memset(data, 0x00, sizeof(data));
	data[0] = SYN;
	rec = SendData(hComm, data);				/* 電文送信 */
	if (rec == -1) {
		/* 送信エラー */
		return -1;
	} else {
		/* 送信成功 */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_enq																 */
/* 		ENQ電文送信															 */
/*****************************************************************************/
int send_enq( void )
{
	char data[4];
	int rec;

	memset(data, 0x00, sizeof(data));
	data[0] = ENQ;
	rec = SendData(hComm, data);				/* 電文送信 */
	if (rec == -1) {
		/* 送信エラー */
		return -1;
	} else {
		/* 送信成功 */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_data																 */
/* 		DATA電文送信														 */
/*****************************************************************************/
int send_data( char n )
{
	char data[32];
	int i = 0, rec;

	
	memset(data, 0x00, sizeof(data));
	data[i] = h_send_buf[n];

	rec = SendData(hComm, data);				/* 電文送信 */
	if (rec == -1) {
		/* 送信エラー */
		return -1;
	} else {
		/* 送信成功 */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_mtanka																 */
/* 		単価応答を作成する(基本的には単価を保存してそのままオウム返し)		 */
/* 			data		ﾃﾞｰﾀ												 */
/* 																			 */
/*****************************************************************************/
int send_mtanka( char *data )
{
	int i = 0, rec;
	int bcc, bcc1, bcc2;

	bcc = 0;
	bcc1 = 0;
	bcc2 = 0;

	/* 受信した単価設定電文をそのままコピー */
	memcpy(tanka_check_mars, data, sizeof(tanka_check_mars) - 6);

	/* CMD番号を変える */
	tanka_check_mars[1] = '6';
	tanka_check_mars[2] = '0';
	/* BCCを計算 */
	for(i = 0; i < sizeof(tanka_check_mars) - 5; i++){
		bcc += tanka_check_mars[i];
	}
	bcc = 0xff - (char)bcc;
	bcc1 = ((bcc & 0xf0) >> 4);		/* 1BYTE分をASCII変換 */
	if((bcc1 >= 0) && (bcc1 <= 9)){
		/* 0～9 */
		bcc1 += '0';
	}else{
		/* A～F */
		bcc1 = (bcc1 - 0x0a) + 'A';
	}
	bcc2 = (bcc & 0x0f);				/* 1BYTE分をASCII変換 */
	if((bcc2 >= 0) && (bcc2 <= 9)){
		/* 0～9 */
		bcc2 += '0';
	}else{
		/* A～F */
		bcc2 = (bcc2 - 0x0a) + 'A';
	}
	tanka_check_mars[14] = bcc1;
	tanka_check_mars[15] = bcc2;

	rec = SendData(hComm, tanka_check_mars);				/* 電文送信 */
	if (rec == -1) {
		/* 送信エラー */
		return -1;
	} else {
		/* 送信成功 */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_clr																 */
/* 		CLR電文送信															 */
/*****************************************************************************/
int send_clr( void )
{
	int i = 0, rec;
	int bcc, bcc1, bcc2;

	bcc = 0;
	bcc1 = 0;
	bcc2 = 0;

	/* BCCを計算 */
	for(i = 0; i < sizeof(clr_mars) - 5; i++){
		bcc += clr_mars[i];
	}
	bcc = 0xff - (char)bcc;
	bcc1 = ((bcc & 0xf0) >> 4);		/* 1BYTE分をASCII変換 */
	if((bcc1 >= 0) && (bcc1 <= 9)){
		/* 0～9 */
		bcc1 += '0';
	}else{
		/* A～F */
		bcc1 = (bcc1 - 0x0a) + 'A';
	}
	bcc2 = (bcc & 0x0f);				/* 1BYTE分をASCII変換 */
	if((bcc2 >= 0) && (bcc2 <= 9)){
		/* 0～9 */
		bcc2 += '0';
	}else{
		/* A～F */
		bcc2 = (bcc2 - 0x0a) + 'A';
	}
	clr_mars[14] = bcc1;
	clr_mars[15] = bcc2;

	rec = SendData(hComm, clr_mars);				/* 電文送信 */
	if (rec == -1) {
		/* 送信エラー */
		return -1;
	} else {
		/* 送信成功 */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_cal																 */
/* 		CLR電文送信															 */
/*****************************************************************************/
int send_cal( void )
{
	int i = 0, rec;
	int bcc, bcc1, bcc2;

	bcc = 0;
	bcc1 = 0;
	bcc2 = 0;

	/* BCCを計算 */
	for(i = 0; i < sizeof(cal_mars) - 5; i++){
		bcc += cal_mars[i];
	}
	bcc = 0xff - (char)bcc;
	bcc1 = ((bcc & 0xf0) >> 4);		/* 1BYTE分をASCII変換 */
	if((bcc1 >= 0) && (bcc1 <= 9)){
		/* 0～9 */
		bcc1 += '0';
	}else{
		/* A～F */
		bcc1 = (bcc1 - 0x0a) + 'A';
	}
	bcc2 = (bcc & 0x0f);				/* 1BYTE分をASCII変換 */
	if((bcc2 >= 0) && (bcc2 <= 9)){
		/* 0～9 */
		bcc2 += '0';
	}else{
		/* A～F */
		bcc2 = (bcc2 - 0x0a) + 'A';
	}
	cal_mars[14] = bcc1;
	cal_mars[15] = bcc2;

	rec = SendData(hComm, cal_mars);				/* 電文送信 */
	if (rec == -1) {
		/* 送信エラー */
		return -1;
	} else {
		/* 送信成功 */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_status																 */
/* 		状態チェック電文送信												 */
/* 			引数															 */
/* 				0:払出要求前												 */
/* 				1:払出要求後												 */
/*****************************************************************************/
int send_status( char mode )
{
	char *send_data;
	int rec, i;
	char bcc, bcc1, bcc2, emp_data;
	static char step = 0;

	bcc = 0;
	bcc1 = 0;
	bcc2 = 0;
	emp_data = 0;
	switch(maker){
	case H_GLORY_OLD:
		if( mode == 0){
			send_data = check1_glory_old;
			send_data[2] = 0x40;
			if(gl_emp_flag == TRUE){
				if(step == 0){
					/* ループ後1回目の状態チェック要求で払出正常終了を返す */
					send_data[1] = '1';
					step++;
				}else{
					/* ループ後2回目の状態チェック要求でEMPTY情報を返す */
					emp_data = gl_empty[0] << 2;
					emp_data += gl_empty[1] << 1;
					emp_data += gl_empty[2];
					emp_data += gl_empty[3] << 3;
					send_data[2] += emp_data;
					gl_emp_flag = FALSE;
					step = 0;
					send_data[1] = '0';
				}
			}
		}else{
			send_data = check2_glory_old;
			gl_emp_flag = TRUE;
		}
		/* STXの次からETXまでの排他的論理和 */
		for(i = 1; i < sizeof(check1_glory_old) - 2; i++){
			bcc ^= send_data[i];
		}
		send_data[i] = bcc;

		//calc_bcc(send_data);
		break;
	case H_GLORY_NEW:
		if( mode == 0){
			send_data = check1_glory_new;
			send_data[2] = 0x60;
			send_data[6] = 0x40;
			if(gl_emp_flag == TRUE){
				if(step == 0){
					/* ループ後1回目の状態チェック要求で払出正常終了を返す */
					send_data[1] = '1';
					step++;
				}else{
					/* ループ後2回目の状態チェック要求でEMPTY情報を返す */
					emp_data = gl_empty[0] << 2;
					emp_data += gl_empty[1] << 1;
					emp_data += gl_empty[2];
					emp_data += gl_empty[3] << 3;
					send_data[2] += emp_data;
					emp_data = gl_empty[4] << 2;
					emp_data += gl_empty[5] << 1;
					emp_data += gl_empty[6];
					emp_data += gl_empty[7] << 3;
					send_data[6] += emp_data;
					gl_emp_flag = FALSE;
					step = 0;
					send_data[1] = '0';
				}
			}
		}else{
			send_data = check2_glory_new;
			gl_emp_flag = TRUE;
		}
		/* STXの次からETXまでの排他的論理和 */
		for(i = 1; i < sizeof(check1_glory_new) - 2; i++){
			bcc ^= send_data[i];
		}
		send_data[i] = bcc;
		//calc_bcc(send_data);
		break;
	case H_SANKYO:
		/* 三共ではステータス送信はない */
		break;
	case H_MARS:
		if(m_tanka_check == TRUE){
			check_mars[4] = '1';
		}else{
			check_mars[4] = '0';
		}

		/* BCCを計算 */
		for(i = 0; i < sizeof(check_mars) - 5; i++){
			bcc += check_mars[i];
		}
		bcc = 0xff - (char)bcc;
		bcc1 = ((bcc & 0xf0) >> 4);		/* 1BYTE分をASCII変換 */
		if((bcc1 >= 0) && (bcc1 <= 9)){
			/* 0～9 */
			bcc1 += '0';
		}else{
			/* A～F */
			bcc1 = (bcc1 - 0x0a) + 'A';
		}
		bcc2 = (bcc & 0x0f);				/* 1BYTE分をASCII変換 */
		if((bcc2 >= 0) && (bcc2 <= 9)){
			/* 0～9 */
			bcc2 += '0';
		}else{
			/* A～F */
			bcc2 = (bcc2 - 0x0a) + 'A';
		}
		check_mars[14] = bcc1;
		check_mars[15] = bcc2;
		send_data = check_mars;
		break;
	case H_HIROTAC:
		break;
	case H_SUBARU:
		break;
	case H_TMI:
		break;
	default:
		if( mode == 0){
			send_data = check1_glory_old;
		}else{
			send_data = check2_glory_old;
		}
		/* STXの次からETXまでの排他的論理和 */
		for(i = 1; i < sizeof(send_data); i++){
			bcc ^= send_data[i];
		}
		send_data[i+1] = bcc;
		break;
	}
	rec = SendData(hComm, send_data);				/* 電文送信 */
	if (rec == -1) {
		/* 送信エラー */
		return -1;
	} else {
		/* 送信成功 */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_harai																 */
/* 		状態チェック電文送信														 */
/*****************************************************************************/
int send_harai( void )
{
	char *send_data;
	char bcc, bcc1, bcc2;
	int rec, i;

	bcc = 0;
	bcc1 = 0;
	bcc2 = 0;
	switch(maker){
	case H_GLORY_OLD:
		/* グローリーでは払出応答はない */
		break;
	case H_GLORY_NEW:
		/* グローリーでは払出応答はない */
		break;
	case H_SANKYO:
		make_harai_sankyo(harai_sankyo);
		send_data = harai_sankyo;
		break;
	case H_MARS:
		make_harai_mars(harai_mars, sizeof(harai_mars));
		/* BCCを計算 */
		for(i = 0; i < sizeof(harai_mars) - 5; i++){
			bcc += harai_mars[i];
		}
		bcc = 0xff - (char)bcc;
		bcc1 = ((bcc & 0xf0) >> 4);		/* 1BYTE分をASCII変換 */
		if((bcc1 >= 0) && (bcc1 <= 9)){
			/* 0～9 */
			bcc1 += '0';
		}else{
			/* A～F */
			bcc1 = (bcc1 - 0x0a) + 'A';
		}
		bcc2 = (bcc & 0x0f);				/* 1BYTE分をASCII変換 */
		if((bcc2 >= 0) && (bcc2 <= 9)){
			/* 0～9 */
			bcc2 += '0';
		}else{
			/* A～F */
			bcc2 = (bcc2 - 0x0a) + 'A';
		}
		harai_mars[36] = bcc1;
		harai_mars[37] = bcc2;
		disp_log_cmd(harai_mars, SEND, LOG, sizeof(harai_mars));
		send_data = harai_mars;
		break;
	case H_HIROTAC:
		break;
	case H_SUBARU:
		break;
	case H_TMI:
		break;
	default:
		send_data = check1_glory_old;
		break;
	}

	rec = SendData(hComm, send_data);				/* 電文送信 */
	if (rec == -1) {
		/* 送信エラー */
		return -1;
	} else {
		/* 送信成功 */
		return 0;
	}
}

/*****************************************************************************/
/* 	disp_log_cmd																 */
/* 		ﾃﾞｰﾀ表示とログ出力													 */
/* 			data, mode, size		ﾃﾞｰﾀ, ﾓｰﾄﾞ(0:DOS表示 1:ﾛｸﾞへ出力), ｻｲｽﾞ	 */
/* 																			 */
/*****************************************************************************/
void disp_log_cmd(char *data, char mode1, char mode2, int size)
{
	return;

	int i;
	char dsp_buf[128], buf[128];
	FILE *fp;

	memset(dsp_buf, 0x00, sizeof(dsp_buf));
	memset(buf, 0x00, sizeof(buf));

	if(mode1 == RECEIVE){
		/* 受信表示 */
		switch(mode2){
		case 0:
			/* DOSへ表示 */
			break;
		case 1:
			/* ﾛｸﾞへ出力 */
			fp = fopen("data.log", "a");
			if(rece_err_flag == TRUE){
				fputs("異常電文:", fp);
			}
			if(size == sizeof(char)){
				switch(data[0]){
				case ACK:
					/* ACK受信表示 */
					fputs("ACK受信\n", fp);
					break;
				case NAK:
					/* NAK受信表示 */
					fputs("NAK受信\n", fp);
					break;
				case ENQ:
					/* ENQ受信表示 */
					fputs("ENQ受信\n", fp);
					break;
				case EOT:
					/* EOT受信表示 */
					fputs("EOT受信\n", fp);
					break;
				default:
					/* 数値受信表示 */
					sprintf(buf, "%02x受信\n", data[0]);
					fputs(buf, fp);
					break;
				}
			}else{
				if(maker == H_MARS){
					if(data[0] == M_STX){
						fputs("CMD受信(", fp);
					}
				}else{
					if(data[0] == STX){
						fputs("CMD受信(", fp);
					}
				}
				for(i = 0; i < size-1; i++){
					if(maker == H_MARS){
						if(data[i] == M_STX){
							fputs("S", fp);
						}else if(data[i] == M_ETX){
							fputs("E", fp);
						}else{
							sprintf(dsp_buf, "%c", data[i]);
							fputs(dsp_buf, fp);
						}
					}else{
						if(data[i] == STX){
							fputs("STX", fp);
						}else if(data[i] == ETX){
							fputs("ETX", fp);
						}else{
							sprintf(dsp_buf, "%c", data[i]);
							fputs(dsp_buf, fp);
						}
					}
				}
				/* BCCのみ16進表示 */
				sprintf(dsp_buf, "%02x", data[size-1]);
				fputs(dsp_buf, fp);
				if(maker == H_MARS){
					if(data[0] == M_STX){
						fputs(")", fp);
					}
				}else{
					if(data[0] == STX){
						fputs(")", fp);
					}
				}
				fputs("\n", fp);
			}
			fclose(fp);
			break;
		}
	}else{
		/* 送信表示 */
		switch(mode2){
		case 0:
			/* DOSへ表示 */
			if(data[0] == STX){
				printf("　CMD送信(");
			}
			for(i = 0; i < size-1; i++){
				if(data[i] == STX){
					printf("STX");
				}else if(data[i] == ETX){
					printf("ETX");
#if 0
				}else if(data[i] == SPACE){
					/* スペースコードなので飛ばす */
					continue;
#endif
				}else{
					sprintf(dsp_buf, "%c", data[i]);
					printf(dsp_buf);
				}
			}
			/* BCCのみ16進表示 */
			sprintf(dsp_buf, "%02x", data[size-1]);
			printf(dsp_buf);
			if(data[0] == STX){
				printf(")");
			}
			printf("\n");
			break;
		case 1:
			/* ﾛｸﾞへ出力 */
			fp = fopen("data.log", "a");
			if(maker == H_MARS){
				if(data[0] == M_STX){
					fputs("　CMD送信(", fp);
				}
			}else{
				if(data[0] == STX){
					fputs("　CMD送信(", fp);
				}
			}
			for(i = 0; i < size-1; i++){
				if(maker == H_MARS){
					if(data[i] == M_STX){
						fputs("S", fp);
					}else if(data[i] == M_ETX){
						fputs("E", fp);
					}else{
						sprintf(dsp_buf, "%c", data[i]);
						fputs(dsp_buf, fp);
					}
				}else{
					if(data[i] == STX){
						fputs("STX", fp);
					}else if(data[i] == ETX){
						fputs("ETX", fp);
					}else{
						sprintf(dsp_buf, "%c", data[i]);
						fputs(dsp_buf, fp);
					}
				}
			}
			/* BCCのみ16進表示 */
			sprintf(dsp_buf, "%02x", data[size-1]);
			fputs(dsp_buf, fp);
			if(maker == H_MARS){
				if(data[0] == M_STX){
					fputs(")", fp);
				}
			}else{
				if(data[0] == STX){
					fputs(")", fp);
				}
			}
			fputs("\n", fp);
			fclose(fp);
			break;
		}
	}
}

/*****************************************************************************/
/* 	disp_log																 */
/* 		ﾃﾞｰﾀ表示とログ出力(1BYTE送信用)										 */
/* 			data, mode, size		ﾃﾞｰﾀ, ﾓｰﾄﾞ(0:DOS表示 1:ﾛｸﾞへ出力), ｻｲｽﾞ	 */
/* 																			 */
/*****************************************************************************/
void disp_log( char data )
{
	int ret;
	FILE *fp;
	char buf[128];
	char log_data[128];
	int i, j;

	j = 0;
	memset(buf, 0x00, sizeof(buf));
	memset(log_data, 0x00, sizeof(log_data));
	fp = fopen("data.log", "a");
	/* 送信表示用 */
	switch(data){
	case ACK:
		/*printf("　ACK送信\n");*/
		fputs("　ACK送信\n", fp);
		break;
	case NAK:
		/*printf("　NAK送信\n");*/
		fputs("　NAK送信\n", fp);
		break;
	case ENQ:
		/*printf("　ENQ送信\n");*/
		fputs("　ENQ送信\n", fp);
		break;
	case SYN:
		/*printf("　SYN送信\n");*/
		fputs("　SYN送信\n", fp);
		break;
	case EOT:
		/*printf("　EOT送信\n");*/
		fputs("　EOT送信\n", fp);
		break;
	case CLR:
		/*printf("　EOT送信\n");*/
		fputs("　CLR送信\n", fp);
		break;
	case CAL:
		/*printf("　EOT送信\n");*/
		fputs("　CAL送信\n", fp);
		break;
	case NO_REC:
		/*printf("　無応答\n");*/
		fputs("　無応答\n", fp);
		break;
	case STATUS1:
		/* 状態チェック */
		switch(maker){
		case H_GLORY_OLD:
		case H_GLORY_NEW:
			for(i=0;i<sizeof(check1_glory_new);i++){
				if(check1_glory_new[i] == STX){
					log_data[j] = 'S';
					log_data[j+1] = 'T';
					log_data[j+2] = 'X';
					j += 3;
				}else if(check1_glory_new[i] == ETX){
					log_data[j] = 'E';
					log_data[j+1] = 'T';
					log_data[j+2] = 'X';
					j += 3;
				}else{
					log_data[j] = check1_glory_new[i];
					j++;
				}
			}
			break;
		case H_MARS:
			for(i=0;i<sizeof(check_mars);i++){
				if(check_mars[i] == M_STX){
					log_data[i] = 'S';
				}else if(check_mars[i] == M_ETX){
					log_data[i] = 'E';
				}else{
					log_data[i] = check1_glory_new[i];
				}
			}
			break;
		case H_SANKYO:
			/* 三共は状態チェックがない */
			break;
		default:
			break;
		}
		ret = _snprintf(buf, sizeof(buf) - 1, "　CMD送信(%s)\n", log_data);
		fputs(buf, fp);
		break;
	case STATUS2:
		/* 状態チェック */
		switch(maker){
		case H_GLORY_OLD:
		case H_GLORY_NEW:
			for(i=0;i<sizeof(check2_glory_new);i++){
				if(check2_glory_new[i] == STX){
					log_data[j] = 'S';
					log_data[j+1] = 'T';
					log_data[j+2] = 'X';
					j += 3;
				}else if(check2_glory_new[i] == ETX){
					log_data[j] = 'E';
					log_data[j+1] = 'T';
					log_data[j+2] = 'X';
					j += 3;
				}else{
					log_data[j] = check2_glory_new[i];
					j++;
				}
			}
			break;
		case H_MARS:
			for(i=0;i<sizeof(check_mars);i++){
				if(check_mars[i] == M_STX){
					log_data[i] = 'S';
				}else if(check_mars[i] == M_ETX){
					log_data[i] = 'E';
				}else{
					log_data[i] = check1_glory_new[i];
				}
			}
			break;
		case H_SANKYO:
			/* 三共は状態チェックがない */
			break;
		default:
			break;
		}
		ret = _snprintf(buf, sizeof(buf) - 1, "　CMD送信(%s)\n", log_data);
		fputs(buf, fp);
		break;
	default:

		ret = _snprintf(buf, sizeof(buf) - 1, "　%02x送信\n", data);
		/*printf(buf);*/
		fputs(buf, fp);
		break;
	}
	fclose(fp);
}

/*****************************************************************************/
/* 	disp_harai																 */
/* 		ﾃﾞｰﾀ表示とログ出力(1BYTE送信用)										 */
/* 			data		ﾃﾞｰﾀ												 */
/* 																			 */
/*****************************************************************************/
void disp_harai( char *data )
{
	int harai_num[2][4];	/* 払出個数 */
	char disp_flag;
	static int call_num = 0;	/* 払出回数 */

	memset(&(harai_num[0][0]), 0x00, sizeof(int)*4);	/* Aグループ用 */
	memset(&(harai_num[1][0]), 0x00, sizeof(int)*4);	/* Bグループ用 */

	/* メーカー毎に内容は違うのでメーカー毎に電文の解析を行う */
	switch(maker){
	case H_GLORY_OLD:
		/* データから払出個数を取得 */
		harai_num[0][0] += (data[2] - 0x30) * 1000;		/* A景品 */
		harai_num[0][0] += (data[3] - 0x30) * 100;		/* A景品 */
		harai_num[0][0] += (data[4] - 0x30) * 10;		/* A景品 */
		harai_num[0][0] += (data[5] - 0x30) * 1;		/* A景品 */
		harai_num[0][1] += (data[6] - 0x30) * 1000;		/* B景品 */
		harai_num[0][1] += (data[7] - 0x30) * 100;		/* B景品 */
		harai_num[0][1] += (data[8] - 0x30) * 10;		/* B景品 */
		harai_num[0][1] += (data[9] - 0x30) * 1;		/* B景品 */
		harai_num[0][2] += (data[10] - 0x30) * 1000;	/* C景品 */
		harai_num[0][2] += (data[11] - 0x30) * 100;		/* C景品 */
		harai_num[0][2] += (data[12] - 0x30) * 10;		/* C景品 */
		harai_num[0][2] += (data[13] - 0x30) * 1;		/* C景品 */
		harai_num[0][3] += (data[14] - 0x30) * 1000;	/* D景品 */
		harai_num[0][3] += (data[15] - 0x30) * 100;		/* D景品 */
		harai_num[0][3] += (data[16] - 0x30) * 10;		/* D景品 */
		harai_num[0][3] += (data[17] - 0x30) * 1;		/* D景品 */
		disp_flag = TRUE;
		break;
	case H_GLORY_NEW:
		harai_num[0][0] += (data[2] - 0x30) * 1000;		/* A景品 */
		harai_num[0][0] += (data[3] - 0x30) * 100;		/* A景品 */
		harai_num[0][0] += (data[4] - 0x30) * 10;		/* A景品 */
		harai_num[0][0] += (data[5] - 0x30) * 1;		/* A景品 */
		harai_num[0][1] += (data[6] - 0x30) * 1000;		/* B景品 */
		harai_num[0][1] += (data[7] - 0x30) * 100;		/* B景品 */
		harai_num[0][1] += (data[8] - 0x30) * 10;		/* B景品 */
		harai_num[0][1] += (data[9] - 0x30) * 1;		/* B景品 */
		harai_num[0][2] += (data[10] - 0x30) * 1000;	/* C景品 */
		harai_num[0][2] += (data[11] - 0x30) * 100;		/* C景品 */
		harai_num[0][2] += (data[12] - 0x30) * 10;		/* C景品 */
		harai_num[0][2] += (data[13] - 0x30) * 1;		/* C景品 */
		harai_num[0][3] += (data[14] - 0x30) * 1000;	/* D景品 */
		harai_num[0][3] += (data[15] - 0x30) * 100;		/* D景品 */
		harai_num[0][3] += (data[16] - 0x30) * 10;		/* D景品 */
		harai_num[0][3] += (data[17] - 0x30) * 1;		/* D景品 */
		harai_num[1][0] += (data[18] - 0x30) * 1000;		/* E景品 */
		harai_num[1][0] += (data[19] - 0x30) * 100;		/* E景品 */
		harai_num[1][0] += (data[20] - 0x30) * 10;		/* E景品 */
		harai_num[1][0] += (data[21] - 0x30) * 1;		/* E景品 */
		harai_num[1][1] += (data[22] - 0x30) * 1000;		/* F景品 */
		harai_num[1][1] += (data[23] - 0x30) * 100;		/* F景品 */
		harai_num[1][1] += (data[24] - 0x30) * 10;		/* F景品 */
		harai_num[1][1] += (data[25] - 0x30) * 1;		/* F景品 */
		harai_num[1][2] += (data[26] - 0x30) * 1000;	/* G景品 */
		harai_num[1][2] += (data[27] - 0x30) * 100;		/* G景品 */
		harai_num[1][2] += (data[28] - 0x30) * 10;		/* G景品 */
		harai_num[1][2] += (data[29] - 0x30) * 1;		/* G景品 */
		harai_num[1][3] += (data[30] - 0x30) * 1000;	/* H景品 */
		harai_num[1][3] += (data[31] - 0x30) * 100;		/* H景品 */
		harai_num[1][3] += (data[32] - 0x30) * 10;		/* H景品 */
		harai_num[1][3] += (data[33] - 0x30) * 1;		/* H景品 */
		disp_flag = TRUE;
		break;
	case H_SANKYO:
		/* 払出要求では払出数が来ない */
		disp_flag = FALSE;
		break;
	case H_MARS:
		break;
	case H_HIROTAC:
		PathAppend(CsvFilePath, "glory.csv");
		printf("払出機メーカー：ヒロタック\n");
		break;
	case H_SUBARU:
		PathAppend(CsvFilePath, "glory.csv");
		printf("払出機メーカー：スバル\n");
		break;
	case H_TMI:
		PathAppend(CsvFilePath, "glory.csv");
		printf("払出機メーカー：TMI\n");
		break;
	default:
		PathAppend(CsvFilePath, "glory.csv");
		printf("払出機メーカー：グローリー(デフォルト)\n");
		break;
	}

	if(disp_flag == TRUE){
		sum_harai[0][0] += harai_num[0][0];
		sum_harai[0][1] += harai_num[0][1];
		sum_harai[0][2] += harai_num[0][2];
		sum_harai[0][3] += harai_num[0][3];
		sum_harai[1][0] += harai_num[1][0];
		sum_harai[1][1] += harai_num[1][1];
		sum_harai[1][2] += harai_num[1][2];
		sum_harai[1][3] += harai_num[1][3];
		call_num++;
		printf("総払出景品　　　　　　今回払出数(交換%3d回目)\n", call_num);
		printf("　払出景品1 =%4d　　　　払出景品1 =%4d\n", sum_harai[0][0], harai_num[0][0]);
		printf("　払出景品2 =%4d　　　　払出景品2 =%4d\n", sum_harai[0][1], harai_num[0][1]);
		printf("　払出景品3 =%4d　　　　払出景品3 =%4d\n", sum_harai[0][2], harai_num[0][2]);
		printf("　払出景品4 =%4d　　　　払出景品4 =%4d\n", sum_harai[0][3], harai_num[0][3]);
		if(maker == H_GLORY_NEW){
			printf("　払出景品5 =%4d　　　　払出景品5 =%4d\n", sum_harai[1][0], harai_num[1][0]);
			printf("　払出景品6 =%4d　　　　払出景品6 =%4d\n", sum_harai[1][1], harai_num[1][1]);
			printf("　払出景品7 =%4d　　　　払出景品7 =%4d\n", sum_harai[1][2], harai_num[1][2]);
			printf("　払出景品8 =%4d　　　　払出景品8 =%4d\n", sum_harai[1][3], harai_num[1][3]);
		}
	}
}

/*****************************************************************************/
/* 	disp_cal																 */
/* 		ﾃﾞｰﾀ表示とログ出力(1BYTE送信用)										 */
/* 			data		ﾃﾞｰﾀ												 */
/* 																			 */
/*****************************************************************************/
void disp_cal( char *data )
{
	char year, mon, day, hour, min, sec;

	/* 初期化 */
	year = 0;	/* 年 */
	mon = 0;	/* 月 */
	day = 0;	/* 日 */
	hour = 0;	/* 時 */
	min = 0;	/* 分 */
	sec = 0;	/* 秒 */

	/* 日付 */
	year += (data[4] - '0') * 10;
	year += (data[5] - '0');
	mon += (data[6] - '0') * 10;
	mon += (data[7] - '0');
	day += (data[8] - '0') * 10;
	day += (data[9] - '0');
	hour += (data[10] - '0') * 10;
	hour += (data[11] - '0');
	min += (data[12] - '0') * 10;
	min += (data[13] - '0');
	sec += (data[14] - '0') * 10;
	sec += (data[15] - '0');
	printf("カレンダセット要求受信(%02d年%02d月%02d日　%02d時%02d分%02d秒：", year, mon, day, hour, min, sec);

	/* 曜日 */
	switch(data[3]){
	case '0':
		/* 日曜日 */
		printf("日曜日)\n");
		break;
	case '1':
		/* 月曜日 */
		printf("月曜日)\n");
		break;
	case '2':
		/* 火曜日 */
		printf("火曜日)\n");
		break;
	case '3':
		/* 水曜日 */
		printf("水曜日)\n");
		break;
	case '4':
		/* 木曜日 */
		printf("木曜日)\n");
		break;
	case '5':
		/* 金曜日 */
		printf("金曜日)\n");
		break;
	case '6':
		/* 土曜日 */
		printf("土曜日)\n");
		break;
	default:
		break;
	}
}

/*****************************************************************************/
/* 	make_tanka_mars															 */
/* 		単価応答を作成する(基本的には単価を保存してそのままオウム返し)		 */
/* 			data		ﾃﾞｰﾀ												 */
/* 																			 */
/*****************************************************************************/
void make_tanka_mars( char *data )
{
	int i;
	char mars_lane_buf[LANE_MAX];
	char cnt1, cnt2, cnt3, cnt4;

	/* 初期化 */
	cnt1 = 0;
	cnt2 = 0;
	cnt3 = 0;
	cnt4 = 0;
	memset(mtanka, 0x00, sizeof(mtanka));
	memset(mars_lane_buf, 0x00, sizeof(mars_lane_buf));
	for(i=0;i<TOKUSHU_MAX;i++){
		memset(&(mars_lane[i][0]), 0x00, sizeof(mars_lane[i]));
	}

	/* 各特殊景品の単価を記憶 */
	for(i = 0; i < TOKUSHU_MAX; i++){
		mtanka[i] += (data[11+i*6] - '0') * 100000;
		mtanka[i] += (data[12+i*6] - '0') * 10000;
		mtanka[i] += (data[13+i*6] - '0') * 1000;
		mtanka[i] += (data[14+i*6] - '0') * 100;
		mtanka[i] += (data[15+i*6] - '0') * 10;
		mtanka[i] += (data[16+i*6] - '0');
	}

	/* 各特殊景品の使用するレーンを記憶 */
	for(i = 0; i < LANE_MAX; i++){
		mars_lane_buf[i] = data[5+30+i] - '0';
	}
	/* 各特殊景品の使用するレーンを元に各景品でまとめてしまう */
	for(i = 0; i < LANE_MAX; i++){
		switch(mars_lane_buf[i]){
		case 0:
			/* 未使用レーン */
			continue;
			break;
		case 1:
			/* 景品1で使用 */
			mars_lane[0][cnt1] = i;
			cnt1++;
			break;
		case 2:
			/* 景品2で使用 */
			mars_lane[1][cnt2] = i;
			cnt2++;
			break;
		case 3:
			/* 景品3で使用 */
			mars_lane[2][cnt3] = i;
			cnt3++;
			break;
		case 4:
			/* 景品4で使用 */
			mars_lane[3][cnt4] = i;
			cnt4++;
			break;
		default:
			continue;
			break;
		}
	}
}

/*****************************************************************************/
/* 	make_harai_sankyo														 */
/* 		払出応答を作成する													 */
/* 			data		ﾃﾞｰﾀ												 */
/* 																			 */
/*****************************************************************************/
void make_harai_sankyo( char *data )
{
	long rest_ball;		/* 余り玉 */
	unsigned long tokushu_num[TOKUSHU_MAX];	/* 特殊景品数 */
	unsigned long tokushu_num_buf;			/* 特殊景品数の計算用変数 */
	int sum_tama;		/* 総玉数 */
	int lane_tama[LANE_MAX];	/* 各レーンの交換玉数 */
	int lane_put[TOKUSHU_MAX][LANE_MAX];	/* 各レーンの交換玉数 */
	char send_tokushu_num[TOKUSHU_MAX][4];	/* 送信払出数格納用バッファ */
	unsigned long disp_lane_harai[LANE_MAX];
	int i, j;
	int cnt;
	char bcc;
	static char disp_first_flag = TRUE;				/* 初回の画面表示 */
	static int cnt1 = 0;
	static int cnt2 = 0;
	static int cnt3 = 0;
	static int cnt4 = 0;

	union {
	  T_EXTRA_OFFER_BALL OfferBall;
	  unsigned long offer_ball[TOKUSHU_MAX];
	} ext;		/* 特殊景品単価 */

	/* 初期化 */
	sum_tama = 0;
	cnt = 0;
	j = 0;
	bcc = 0;
	memset(lane_tama, 0x00, sizeof(lane_tama));
	for(i=0;i<TOKUSHU_MAX;i++){
		memset(&lane_put[i][0], 0x00, sizeof(lane_put[0]));
	}
	memset(&ext.OfferBall, 0x00, sizeof(T_EXTRA_OFFER_BALL));
	memset(disp_lane_harai, 0x00, sizeof(disp_lane_harai));

	/* 総玉数の取得 */
	if((harai_data[1] >= '0') && (harai_data[1] <= '9')){
		/* '0'～'9' */
		sum_tama += (harai_data[1] - '0') << 16;
	}else{
		/* 'A'～'F' */
		sum_tama += (harai_data[1] - 'A' + 0x0a) << 16;
	}
	if((harai_data[2] >= '0') && (harai_data[2] <= '9')){
		sum_tama += (harai_data[2] - '0') << 12;
	}else{
		sum_tama += (harai_data[2] - 'A' + 0x0a) << 12;
	}
	if((harai_data[3] >= '0') && (harai_data[3] <= '9')){
		sum_tama += (harai_data[3] - '0') << 8;
	}else{
		sum_tama += (harai_data[3] - 'A' + 0x0a) << 8;
	}
	if((harai_data[4] >= '0') && (harai_data[4] <= '9')){
		sum_tama += (harai_data[4] - '0') << 4;
	}else{
		sum_tama += (harai_data[4] - 'A' + 0x0a) << 4;
	}
	if((harai_data[5] >= '0') && (harai_data[5] <= '9')){
		sum_tama += (harai_data[5] - '0');
	}else{
		sum_tama += (harai_data[5] - 'A' + 0x0a);
	}

	/* 全8レーンの単価を取得する */
	for(i = 0; i < LANE_MAX; i++){
		if((harai_data[6+i*4] >= '0') && (harai_data[6+i*4] <= '9')){
			lane_tama[i] += (harai_data[6+i*4] - '0') << 12;
		}else{
			lane_tama[i] += (harai_data[6+i*4] - 'A' + 0x0a) << 12;
		}
		if((harai_data[7+i*4] >= '0') && (harai_data[7+i*4] <= '9')){
			lane_tama[i] += (harai_data[7+i*4] - '0') << 8;
		}else{
			lane_tama[i] += (harai_data[7+i*4] - 'A' + 0x0a) << 8;
		}
		if((harai_data[8+i*4] >= '0') && (harai_data[8+i*4] <= '9')){
			lane_tama[i] += (harai_data[8+i*4] - '0') << 4;
		}else{
			lane_tama[i] += (harai_data[8+i*4] - 'A' + 0x0a) << 4;
		}
		if((harai_data[9+i*4] >= '0') && (harai_data[9+i*4] <= '9')){
			lane_tama[i] += (harai_data[9+i*4] - '0');
		}else{
			lane_tama[i] += (harai_data[9+i*4] - 'A' + 0x0a);
		}
	}

	/* 各特殊景品の単価を取得 */
	for(i = 0; i < LANE_MAX; i++){
		if(j == 0){
			/* 無条件で保存 */
			ext.offer_ball[j] = lane_tama[i];
			lane_put[j][0] = i+1;
			j++;
			continue;
		}
		if(lane_tama[i] == 0){
			/* レーンの交換玉数が0の場合は終了 */
			break;
		}
		if(lane_tama[i-1] != lane_tama[i]){
			/* 同じじゃなければ保存する */
			ext.offer_ball[j] = lane_tama[i];
			lane_put[j][0] = i+1;
			j++;
			cnt = 0;
		}else{
			/* 同じの場合はレーン情報のみ記憶 */
			cnt++;
			lane_put[j-1][cnt] = i+1;
		}
	}

	/* 特殊景品自動計算処理 ⇒ 余り玉の算出 */
	rest_ball = AutoOffer(sum_tama, &ext.OfferBall, (T_EXTRA_OFFER *)tokushu_num);


	/* レーンをローテーションさせて引き落とす */
	/* 最大景品 */
	if(lane_put[0][cnt1] == 0){
		/* 最初のレーンに戻る */
		cnt1 = 0;
	}
	/* 2番目景品 */
	if(lane_put[1][cnt2] == 0){
		/* 最初のレーンに戻る */
		cnt2 = 0;
	}
	/* 3番目景品 */
	if(lane_put[2][cnt3] == 0){
		/* 最初のレーンに戻る */
		cnt3 = 0;
	}
	/* 4番目景品 */
	if(lane_put[3][cnt4] == 0){
		/* 最初のレーンに戻る */
		cnt4 = 0;
	}

	disp_lane_harai[(lane_put[0][cnt1])-1] = tokushu_num[0];	/* 特殊景品特大 */
	disp_lane_harai[(lane_put[1][cnt2])-1] = tokushu_num[1];	/* 特殊景品大 */
	disp_lane_harai[(lane_put[2][cnt3])-1] = tokushu_num[2];	/* 特殊景品中 */
	disp_lane_harai[(lane_put[3][cnt4])-1] = tokushu_num[3];	/* 特殊景品小 */
	sum_harai[0][0] += tokushu_num[0];
	sum_harai[0][1] += tokushu_num[1];
	sum_harai[0][2] += tokushu_num[2];
	sum_harai[0][3] += tokushu_num[3];

	for(i = 0; i < TOKUSHU_MAX; i++){
		tokushu_num_buf = tokushu_num[i] & 0x0000f000;	/* short型変換 */
		send_tokushu_num[i][0] = (char)(tokushu_num_buf >> 12);	/* bit操作 */
		if((send_tokushu_num[i][0] >= 0x0a) && (send_tokushu_num[i][0] <= 0x0f)){
			send_tokushu_num[i][0] += 'A';
		}else{
			send_tokushu_num[i][0] += '0';
		}

		tokushu_num_buf = tokushu_num[i] & 0x00000f00;	/* short型変換 */
		send_tokushu_num[i][1] = (char)(tokushu_num_buf >> 8);	/* bit操作 */
		if((send_tokushu_num[i][1] >= 0x0a) && (send_tokushu_num[i][1] <= 0x0f)){
			send_tokushu_num[i][1] += 'A';
		}else{
			send_tokushu_num[i][1] += '0';
		}

		tokushu_num_buf = tokushu_num[i] & 0x000000f0;	/* short型変換 */
		send_tokushu_num[i][2] = (char)(tokushu_num_buf >> 4);	/* bit操作 */
		if((send_tokushu_num[i][2] >= 0x0a) && (send_tokushu_num[i][2] <= 0x0f)){
			send_tokushu_num[i][2] += 'A';
		}else{
			send_tokushu_num[i][2] += '0';
		}

		tokushu_num_buf = tokushu_num[i] & 0x0000000f;	/* short型変換 */
		send_tokushu_num[i][3] = (char)tokushu_num_buf;	/* bit操作 */
		if((send_tokushu_num[i][3] >= 0x0a) && (send_tokushu_num[i][3] <= 0x0f)){
			send_tokushu_num[i][3] += 'A';
		}else{
			send_tokushu_num[i][3] += '0';
		}
	}

	/* 各レーンの払出数にセットする */
	data[1+((lane_put[0][cnt1]-1)*4)] = send_tokushu_num[0][0];
	data[2+((lane_put[0][cnt1]-1)*4)] = send_tokushu_num[0][1];
	data[3+((lane_put[0][cnt1]-1)*4)] = send_tokushu_num[0][2];
	data[4+((lane_put[0][cnt1]-1)*4)] = send_tokushu_num[0][3];
	data[1+((lane_put[1][cnt2]-1)*4)] = send_tokushu_num[1][0];
	data[2+((lane_put[1][cnt2]-1)*4)] = send_tokushu_num[1][1];
	data[3+((lane_put[1][cnt2]-1)*4)] = send_tokushu_num[1][2];
	data[4+((lane_put[1][cnt2]-1)*4)] = send_tokushu_num[1][3];
	data[1+((lane_put[2][cnt3]-1)*4)] = send_tokushu_num[2][0];
	data[2+((lane_put[2][cnt3]-1)*4)] = send_tokushu_num[2][1];
	data[3+((lane_put[2][cnt3]-1)*4)] = send_tokushu_num[2][2];
	data[4+((lane_put[2][cnt3]-1)*4)] = send_tokushu_num[2][3];
	data[1+((lane_put[3][cnt4]-1)*4)] = send_tokushu_num[3][0];
	data[2+((lane_put[3][cnt4]-1)*4)] = send_tokushu_num[3][1];
	data[3+((lane_put[3][cnt4]-1)*4)] = send_tokushu_num[3][2];
	data[4+((lane_put[3][cnt4]-1)*4)] = send_tokushu_num[3][3];

	/* BCC計算 */
	for(i = 1; i < (sizeof(data)) - 3; i++){
		bcc ^= data[i];
	}

	/* レーン単価と総玉数、レーン毎の交換個数、景品毎の交換個数を表示させる */
	if(disp_first_flag == TRUE){
		printf("レーン１:%8d　　レーン５:%8d\n", lane_tama[0], lane_tama[4]);
		printf("レーン２:%8d　　レーン６:%8d\n", lane_tama[1], lane_tama[5]);
		printf("レーン３:%8d　　レーン７:%8d\n", lane_tama[2], lane_tama[6]);
		printf("レーン４:%8d　　レーン８:%8d\n", lane_tama[3], lane_tama[7]);
		printf("--------------------------------------------\n");
		disp_first_flag = FALSE;
	}
	printf("入力玉数　:%8d\n", sum_tama);
	printf("レーン１:%8d　　レーン５:%8d　　(総)景品1払出数%6d\n", disp_lane_harai[0], disp_lane_harai[4], sum_harai[0][0]);
	printf("レーン２:%8d　　レーン６:%8d　　(総)景品2払出数%6d\n", disp_lane_harai[1], disp_lane_harai[5], sum_harai[0][1]);
	printf("レーン３:%8d　　レーン７:%8d　　(総)景品3払出数%6d\n", disp_lane_harai[2], disp_lane_harai[6], sum_harai[0][2]);
	printf("レーン４:%8d　　レーン８:%8d　　(総)景品4払出数%6d\n", disp_lane_harai[3], disp_lane_harai[7], sum_harai[0][3]);
	cnt1++;
	cnt2++;
	cnt3++;
	cnt4++;

}

/*****************************************************************************/
/* 	make_harai_sankyo														 */
/* 		払出応答を作成する													 */
/* 			data		ﾃﾞｰﾀ												 */
/* 																			 */
/*****************************************************************************/
void make_harai_mars( char *data, int cbSize )
{
	int sum_tama;		/* 総玉数 */
	long rest_ball, rest_ball_buf;		/* 余り玉 */
	unsigned long tokushu_num[TOKUSHU_MAX];	/* 特殊景品数 */
	unsigned long tokushu_num_buf;			/* 特殊景品数の計算用変数 */
	int i, j;
	union {
	  T_EXTRA_OFFER_BALL OfferBall;
	  unsigned long offer_ball[TOKUSHU_MAX];
	} ext;		/* 特殊景品単価 */

//	printf("cbSize = %d\n", cbSize);
//printf("HARAI END\n");
	/* 初期化 */
	sum_tama = 0;
	rest_ball = 0;
	rest_ball_buf = 0;
	tokushu_num_buf = 0;
	memset(tokushu_num, 0x00, sizeof(tokushu_num));

//printf("HARAI END\n");
	memcpy(&ext.offer_ball, mtanka, sizeof(ext.offer_ball));
//printf("HARAI END\n");

	/* 総玉数を数値化 */
	sum_tama += (harai_data[3] - '0') * 100000;
	sum_tama += (harai_data[4] - '0') * 10000;
	sum_tama += (harai_data[5] - '0') * 1000;
	sum_tama += (harai_data[6] - '0') * 100;
	sum_tama += (harai_data[7] - '0') * 10;
	sum_tama += (harai_data[8] - '0');

	/* 総玉数と景品単価から景品数を算出、また基本的には余りは0なのでクリアする */
	rest_ball = AutoOffer(sum_tama, &ext.OfferBall, (T_EXTRA_OFFER *)tokushu_num);
	rest_ball = 0;

	/*
	   余りについて再投出できるようにする
	   動作中に以下の入力に沿って動作させる
	    1:景品1を払出数-1払出さない
	    2:景品2を払出数-1払出さない
	    3:景品3を払出数-1払出さない
	    4:景品4を払出数-1払出さない
	    ※景品数が0の場合は再投出しない
	    ※一度に複数の景品を払出さない事も可能
	    ※入力した回数分-1をする
	*/
	/* 入力分減らす払出数の計算 */
	for(i = 0; i < TOKUSHU_MAX; i++){
		if(tokushu_num[i] == 0){
			/* 払出数が0の場合は行わない */
			continue;
		}
		for(j = 0; j < keihin_put[i]; j++){
			if(tokushu_num[i] == 0){
				/* 減らしていくうちに払出数が0になったので行わない */
				continue;
			}
			tokushu_num[i]--;	/* 払出数-1 */
			/* 減らした分は余りにする */
			rest_ball += mtanka[i];
		}
	}

	/* 余りをセット */
	rest_ball_buf = rest_ball;
	data[5] = (char)(tokushu_num_buf % 10) + '0';
	rest_ball_buf = rest_ball / 10;
	data[6] = (char)(tokushu_num_buf % 10) + '0';
	rest_ball_buf = rest_ball / 10;
	data[7] = (char)(tokushu_num_buf % 10) + '0';
	rest_ball_buf = rest_ball / 10;
	data[8] = (char)(tokushu_num_buf % 10) + '0';
	rest_ball_buf = rest_ball / 10;
	data[9] = (char)(tokushu_num_buf % 10) + '0';
	rest_ball_buf = rest_ball / 10;
	data[10] = (char)(tokushu_num_buf % 10) + '0';

	/* マースはレーンが関係ないのでそのまま電文にセット */
	for(i = 0; i < TOKUSHU_MAX; i++){
		tokushu_num_buf = tokushu_num[i];
		data[11+i*6] = (char)(tokushu_num_buf % 10) + '0';
		tokushu_num_buf = tokushu_num[i] / 10;
		data[12+i*6] = (char)(tokushu_num_buf % 10) + '0';
		tokushu_num_buf = tokushu_num[i] / 10;
		data[13+i*6] = (char)(tokushu_num_buf % 10) + '0';
		tokushu_num_buf = tokushu_num[i] / 10;
		data[14+i*6] = (char)(tokushu_num_buf % 10) + '0';
		tokushu_num_buf = tokushu_num[i] / 10;
		data[15+i*6] = (char)(tokushu_num_buf % 10) + '0';
		tokushu_num_buf = tokushu_num[i] / 10;
		data[16+i*6] = (char)(tokushu_num_buf % 10) + '0';
		/* 総払出数に代入 */
		sum_harai[0][i] += tokushu_num[i];
	}

	/* 画面表示 */
	printf("入力玉数%6d\n", sum_tama);
	printf("(総)景品1払出数%6d　　(今)景品1払出数%6d(-%2d)\n", sum_harai[0][0], tokushu_num[0], keihin_put[0]);
	printf("(総)景品1払出数%6d　　(今)景品2払出数%6d(-%2d)\n", sum_harai[0][1], tokushu_num[1], keihin_put[1]);
	printf("(総)景品1払出数%6d　　(今)景品3払出数%6d(-%2d)\n", sum_harai[0][2], tokushu_num[2], keihin_put[2]);
	printf("(総)景品1払出数%6d　　(今)景品4払出数%6d(-%2d)\n", sum_harai[0][3], tokushu_num[3], keihin_put[3]);
	
}

/*MHederS **********************************************************************
*  Function ID		: MP*
*			Name	: AutoOffer ( )
*			Title	: 特殊景品自動計算処理
*			Summary :
*
*  Input Parameter	: unsigned long		 left_ball	自動計算の対象とする玉数
*												=
*					  T_EXTRA_OFFER_BALL *OfferBall	各特殊景品の交換玉数が定義されている領域の
*													ポインタ
*												=
*					  T_EXTRA_OFFER		 *Offer		景品の交換結果を格納する領域のポインタ
*												=
*
*  Output Parameter : unsigned long				余り玉数
*												=
*			Comment : None
*MHederE *********************************************************************/
unsigned long AutoOffer(unsigned long left_ball,
						T_EXTRA_OFFER_BALL *OfferBall,
						T_EXTRA_OFFER *Offer)
{
	/* 1景品交換玉数格納変数ポインタ */
	/* 最大4種類で0を検出する事で特殊景品の種類数を判断する */
	unsigned long *offer_ball_org= (unsigned long*)OfferBall;
	/* 景品交換数格納変数をintのポインタに変換 */
	unsigned long *offer = (unsigned long *)Offer;
	/* 特殊景品数格納変数 */
	long ball;

	unsigned long offer_ball[TOKUSHU_MAX];

	/* 計算終了の値 */
	unsigned long lcm, lcm_tmp;
	long gcd_min, tmp;
	long rmd_tmp;
	int i, j, k, l, cnt;
	unsigned int i_old=0;

	int	tkflag = 0;

	/* 特殊景品個数(a:最大 → d:最小) */
	unsigned long num[TOKUSHU_MAX];
	int chg_a_ball, chg_b_ball, chg_c_ball, chg_d_ball;
	unsigned long total_keihinsuu=0;

	/* 変数初期化 */
	memset(offer, 0x00, sizeof(T_EXTRA_OFFER));
	memset(offer_ball, 0, sizeof(offer_ball));
	gcd_min = 0;
	cnt = 0;

	/* 特殊景品の単価チェック */
	for (i=0; i<TOKUSHU_MAX; i++)
	{
		/* 景品単価が１以上の場合のみ計算対象とする */
		if (0 < offer_ball_org[i])
		{
			offer_ball[cnt] = offer_ball_org[i];
			cnt ++;		/* 有効特殊景品個数のｶｳﾝﾄｱｯﾌﾟ */
		}
	}

	/* 最小景品単価より預り玉数が少ない場合returnする	2004.04.07	kodama */
	if(offer_ball[cnt - 1] > left_ball)
	{
		return left_ball;
	}

	/* 一旦計算 */
	offer[0] = left_ball / offer_ball[0];
	for (i=1; (i<TOKUSHU_MAX) && (0 != offer_ball[i]); i++) {
		lcm = get_lcm(offer_ball[i-1], offer_ball[i]);
		/*
		 *	最小公倍数の１の位Check
		 *	０でないときは０になるように２or１０倍する
		 *
		 */
		lcm_tmp = lcm % 10;
		switch (lcm_tmp)
		{
			case 0:	/* 1位が0→そのまま */
				offer[i] = lcm / offer_ball[i] - 1;
				break;
			case 5:	/* 1位が5→2倍 */
				offer[i] = (lcm * 2) / offer_ball[i] - 1;
				break;
			default:/* その他→10倍 */
				offer[i] = (lcm * 10) / offer_ball[i] - 1;
				break;
		}
	}

	/* 最小景品個数が０の場合は終了 */
	if (0 == offer[cnt-1]) return (left_ball);

	/* 余り玉数を仮計算 */
	ball = left_ball - (offer[0] * offer_ball[0]);
	memset(num, 0, sizeof(num));
	num[0] = offer[0];


	/* (最小の)最大公約数算出 */
	for (i=0; i <(cnt-1); i++)
	{
		tmp = get_gcd(offer_ball[i], offer_ball[cnt-1]);
		if (0 != tmp)
		{
			/* 既決定値と比較し小さい場合は入替える */
			if (0 == gcd_min)
			{
				gcd_min = tmp;
			}
			else if (tmp < gcd_min)
			{
				gcd_min = tmp;
			}
		}
	}

	/* 特殊景品有効個数で処理分岐 */
	switch (cnt)
	{
		/*********************************************************************/
		/* ２景品 */
		case 2:
			for (i=offer[0]; i>=0; i--)
			{
				for (j=offer[1]; j>=0; j--)
				{
					/* あまりに端数（１の位）がでないようにする繰上げ処理 */
					chg_a_ball = i * offer_ball[0];
					chg_b_ball = j * offer_ball[1];

					rmd_tmp = left_ball - (chg_a_ball + chg_b_ball);

					/* 余りが発生するまで続ける */
					if (0 > rmd_tmp) continue;

					/* チェック */
					if (rmd_tmp <= ball)
					{
						ball = rmd_tmp;
						num[0] = i;
						num[1] = j;

						/* 余り０または最大公約数以下なら終了 */
						if ((0 == ball) || (ball < gcd_min)) goto end;
					}
				}
			}
			break;

		/*********************************************************************/
		/* ３景品 */
		case 3:
			for (i=offer[0]; i>=0; i--)
			{
				/* 景品が絞れたので抜ける */
				if(i_old != 0) goto end;
				for (j=offer[1]; j>=0; j--)
				{
					/* 一旦チェック */
					if (0 > (left_ball - i*offer_ball[0] - j*offer_ball[1]))
					{
						/* 最小景品のループまで行く必要なし */
						continue;
					}
					for (k=offer[2]; k>=0; k--)
					{
						chg_a_ball = i * offer_ball[0];
						chg_b_ball = j * offer_ball[1];
						chg_c_ball = k * offer_ball[2];

						rmd_tmp = left_ball - (chg_a_ball + chg_b_ball + chg_c_ball);

						/* 余りが発生するまで続ける */
						if (0 > rmd_tmp) continue;

						/*
						 *	特殊景品最適化不具合対応
						 *	最適化の基準
						 *	1.余りが少ない
						 *　2.景品が少ない(余りが一緒)
						 *　3.大きな景品から(余りと景品数が一緒)
						 *
						 *	2008.08.08	osako
						 */

						/* 余りが最小景品を超えたらループを中止 */
						if (rmd_tmp > (long)offer_ball[2])	break;

						tkflag = FALSE;
						if (rmd_tmp < ball) {
							tkflag = TRUE;
						} else if (rmd_tmp == ball) {
							if ( (total_keihinsuu == 0) || (total_keihinsuu > (unsigned long)(i+j+k)) ) {
								tkflag = TRUE;
							}
						}

						if (tkflag == TRUE)
						{
							ball = rmd_tmp;
							num[0] = i;
							num[1] = j;
							num[2] = k;

							if(total_keihinsuu == 0) (i_old = i);

							total_keihinsuu = i+j+k;
						}
					}
				}
			}
			break;

		/*********************************************************************/
		/* ４景品 */
		case 4:
			for (i=offer[0]; i>=0; i--)
			{
				/* 既に景品が絞れた場合は抜ける */
				if( i_old != 0 ) goto end;
				for (j=offer[1]; j>=0; j--)
				{
					/* 一旦チェック */
					if (0 > (left_ball - i*offer_ball[0] - j*offer_ball[1]))
					{
						/* 次の景品の計算を行う必要なし */
						continue;
					}
					for (k=offer[2]; k>=0; k--)
					{
						/* 一旦チェック */
						if (0 > (left_ball - i*offer_ball[0] - j*offer_ball[1] - k*offer_ball[2]))
						{
							continue;
						}

						for (l=offer[3]; l>=0; l--)
						{
							rmd_tmp = left_ball - i*offer_ball[0] - j*offer_ball[1] - k*offer_ball[2] - l*offer_ball[3];


							/* あまりに端数（１の位）がでないようにする繰上げ処理 */
							chg_a_ball = i * offer_ball[0];
							chg_b_ball = j * offer_ball[1];
							chg_c_ball = k * offer_ball[2];
							chg_d_ball = l * offer_ball[3];

							rmd_tmp = left_ball - (chg_a_ball + chg_b_ball + chg_c_ball + chg_d_ball);

							/* 余りが出るまで続ける */
							if (0 > rmd_tmp) continue;

							/*
							 *	特殊景品最適化不具合対応
							 *	最適化の基準
							 *	1.余りが少ない
							 *　2.景品が少ない(余りが一緒)
							 *　3.大きな景品から(余りと景品数が一緒)
							 *
							 *	2008.08.08	osako
							 */

							/* 余りが最小景品を超えたらループを中止 */
							if (rmd_tmp > (long)offer_ball[3])	break;

							tkflag = FALSE;
							if (rmd_tmp < ball) {
								/* 余りが今までで一番少ない */
								tkflag = TRUE;
							} else if (rmd_tmp == ball) {
								/* 余りが一緒なので景品数が少ないかチェック */
								if ( (total_keihinsuu == 0) || (total_keihinsuu > (unsigned long)(i+j+k+l)) ) {
									/* 景品数が一番少ない場合と初回は通る */
									tkflag = TRUE;
								}
							}

							if (tkflag == TRUE)
							{
								/* 状態を記憶しておく */
								ball = rmd_tmp;
								num[0] = i;
								num[1] = j;
								num[2] = k;
								num[3] = l;

								/* total(各景品個数合計)の景品個数が一番少ないときを求めるために
								   "余り玉"の一番少ない時の特大、大の景品個数を覚えておく。
								   それぞれ１個づつ減らしたときのtotal景品個数と比較
								*/
								if(total_keihinsuu == 0)
								{
									i_old = i;
								}

								total_keihinsuu = i+j+k+l;
							}
						}
					}
				}
			}
			break;
		/*********************************************************************/
	}

end:

	j = 0;
	for (i=0; i<TOKUSHU_MAX; i++)
	{
		/* 単価チェック */
		if (offer_ball_org[i] == offer_ball[j])
		{
			/* 単価同じなら同一景品 */
			offer[i] = num[j];
			j ++;
		}
		else
		{
			/* 単価ことなるので違う景品 */
			offer[i] = 0;
		}
	}

	/* 余り玉数を返す */
	return (ball/10);
}

/*
 *	最小公倍数の算出
 */
int get_lcm(int ball_1, int ball_2)
{
	if ((0 == ball_1) || (0 == ball_2)) {
		/* 引数に０がある場合は0 */
		return 0;
	}

	return ((ball_1 / get_gcd(ball_1, ball_2)) * ball_2);
}

/*
 * 最大公約数の算出
 */
int get_gcd(int ball_1, int ball_2)
{
	if ((0 == ball_1) || (0 == ball_2)) {
		/* 引数に０がある場合は0 */
		return 0;
	}

	/* ユークリッドの互助法を用いて最大公約数を算出 */
	while (ball_1 != ball_2) {
		if (ball_1 > ball_2) {
			ball_1 -= ball_2;
		}
		else {
			ball_2 -= ball_1;
		}
	}

	return ball_1;
}
