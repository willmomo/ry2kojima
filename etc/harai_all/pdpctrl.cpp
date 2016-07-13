//---------------------------------------------------------------------
// [概要]
// PDPをコントロールする関数群
//---------------------------------------------------------------------

#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include "def.h"

//---------------------------------------------------------------------
// [概要]
// COMポートを開く
//
// [備考]
// 通信条件の設定は、PDP固有の設定に固定。
//---------------------------------------------------------------------

HANDLE OpenComm()
{
	// iniで指定したCOMポートを開く
	HANDLE hComm = CreateFile(com, GENERIC_READ|GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	// 正常にオープンできたら通信パラメータを設定
	if (hComm != INVALID_HANDLE_VALUE) {
		DCB dcb;

		GetCommState(hComm, &dcb);

		/* メーカーによりOPENしたCOMの情報を変化させる */
		switch(maker){
		case H_GLORY_OLD:
		case H_GLORY_NEW:
			/* グローリー払出機 */
			dcb.BaudRate = CBR_9600;	// ボーレート: 9600bps
			dcb.Parity = ODDPARITY;		// パリティ: 1
			dcb.ByteSize = 8;			// キャラクター長: 8bit
			dcb.StopBits = TWOSTOPBITS;	// ストップビット: 2bit
			break;
		case H_SANKYO:
			/* 三共 */
			dcb.BaudRate = CBR_2400;	// ボーレート: 2400bps
			dcb.Parity = EVENPARITY;		// パリティ: なし
			dcb.ByteSize = 7;			// キャラクター長: 8bit
			dcb.StopBits = ONESTOPBIT;	// ストップビット: 1bit
			break;
		case H_MARS:
			/* マース */
			dcb.BaudRate = CBR_4800;	// ボーレート: 4800bps
			dcb.Parity = NOPARITY;		// パリティ: なし
			dcb.ByteSize = 8;			// キャラクター長: 8bit
			dcb.StopBits = ONESTOPBIT;	// ストップビット: 1bit
			break;
		case H_HIROTAC:
			/* ヒロタック */
		case H_SUBARU:
			/* スバル */
		case H_TMI:
			/* TMI */
		default:
			/* グローリー払出機 */
			dcb.BaudRate = CBR_9600;	// ボーレート: 9600bps
			dcb.Parity = ODDPARITY;		// パリティ: なし
			dcb.ByteSize = 8;			// キャラクター長: 8bit
			dcb.StopBits = TWOSTOPBITS;	// ストップビット: 1bit
			break;
		}

		
		// フロー制御: なし
		dcb.fDtrControl = DTR_CONTROL_DISABLE;
		dcb.fRtsControl = RTS_CONTROL_DISABLE;

		SetCommState(hComm, &dcb);

		// タイムアウト時間の設定
		COMMTIMEOUTS ct;
		GetCommTimeouts(hComm, &ct);

		// 受信インターバルタイムアウト: 0ms(なし)
		ct.ReadIntervalTimeout = 0;

		// 受信タイムアウト: 500ms + (10ms * 受信バイト数)
		ct.ReadTotalTimeoutMultiplier = 10;
		ct.ReadTotalTimeoutConstant = 500;

		// 送信タイムアウト: 3000ms + (10ms * 送信バイト数)
		ct.WriteTotalTimeoutMultiplier = 10;
		ct.WriteTotalTimeoutConstant = 3000;

		SetCommTimeouts(hComm, &ct);
	} else {
		return INVALID_HANDLE_VALUE;
	}

	return hComm;
}

//---------------------------------------------------------------------
// [概要]
// 電文送信
//
// flag = (0/1/2/3)=(STX.ETX有/STXなし/ETXなし/両方とも無し)
//---------------------------------------------------------------------

int SendData(HANDLE hComm, const char* pszCmd)
{
	BOOL ret;
	DWORD dwWritten = 0;

	char work[256];

	wsprintf(work, "%s", pszCmd);
	ret = WriteFile(hComm, work, lstrlen(work), &dwWritten, NULL);

	return (ret == FALSE || (int)dwWritten != lstrlen(work)) ? -1 : 0;
}

//---------------------------------------------------------------------
// [概要]
// 受信電文解析
//
// [備考]
// なし
//---------------------------------------------------------------------

int RecvData(HANDLE hComm, char* pszAns, size_t bufSize)
{
	int wp = 0;
	BOOL ret;
	DWORD dwRead;
	char work[256];
	char stx_rec_flag, etx_rec_flag;

	stx_rec_flag = 0;
	etx_rec_flag = 0;

	// まずは1BYTEのみ解析
	for (;;) {
		ret = ReadFile(hComm, work, 1, &dwRead, NULL);	// 1バイト読み込み
		if (ret == FALSE){
			/* 受信エラー１ */
			return -1;	// エラー発生。エラー終了
		}
		if (dwRead == 0) {
			/* 受信なし */
			return 0;	// 受信なし。次へ進む
		}
		/* 電文受信 */
		if((work[0] == '\x53') && (maker == H_MARS)){
			/* STX */
			stx_rec_flag = TRUE;
		}
		if(work[0] == '\x02') {
			/* STX */
			stx_rec_flag = TRUE;
		}
		/* ﾊﾞｯﾌｧへ受信電文を保存 */
		pszAns[wp++] = work[0];
		break;
	}

	if(stx_rec_flag == FALSE){
		/* STX以外はここでreturn */
		return wp;
	}

	// ETXまで、読み込む
	for (;;) {
		ret = ReadFile(hComm, work, 1, &dwRead, NULL);	// 1バイト読み込み
		if (ret == FALSE) {
			/* 受信エラー２ */
			return -1;	// エラー発生。エラー終了
		} else if (dwRead == 0) {
			/* printf("受信タイムアウト２\n"); */
			return -1;	// タイムアウト発生。エラー終了
		}else if((work[0] == '\x45') && (maker == H_MARS)){
			/* printf("受信成功２\n");*/
			pszAns[wp++] = work[0];
			etx_rec_flag = TRUE;
			break;	// ETX読み込み。次へ進む
		} else if (work[0] == '\x03') {
			/* printf("受信成功２\n");*/
			pszAns[wp++] = work[0];
			etx_rec_flag = TRUE;
			break;	// ETX読み込み。次へ進む
		} else {
			pszAns[wp++] = work[0];
		}
	}

	/* BCCを読込む */
	if(etx_rec_flag == TRUE){
		ret = ReadFile(hComm, work, 1, &dwRead, NULL);	// 1バイト読み込み
		if (ret == FALSE) {
			/* 受信エラー２ */
			return -1;	// エラー発生。エラー終了
		} else if (dwRead == 0) {
			return -1;	// タイムアウト発生。エラー終了
		} else {
			/* BCC受信 */
			pszAns[wp++] = work[0];
		}
	}

	/* MARSの場合はCR+LFがあるのでそれも読み込む */
	if(maker == H_MARS){
		ret = ReadFile(hComm, work, 1, &dwRead, NULL);	// 1バイト読み込み
		if (ret == FALSE) {
			/* 受信エラー２ */
			return -1;	// エラー発生。エラー終了
		} else if (dwRead == 0) {
			return -1;	// タイムアウト発生。エラー終了
		} else {
			/* BCC受信 */
			pszAns[wp++] = work[0];
		}
		ret = ReadFile(hComm, work, 1, &dwRead, NULL);	// 1バイト読み込み
		if (ret == FALSE) {
			/* 受信エラー２ */
			return -1;	// エラー発生。エラー終了
		} else if (dwRead == 0) {
			return -1;	// タイムアウト発生。エラー終了
		} else {
			/* BCC受信 */
			pszAns[wp++] = work[0];
		}
		ret = ReadFile(hComm, work, 1, &dwRead, NULL);	// 1バイト読み込み
		if (ret == FALSE) {
			/* 受信エラー２ */
			return -1;	// エラー発生。エラー終了
		} else if (dwRead == 0) {
			return -1;	// タイムアウト発生。エラー終了
		} else {
			/* BCC受信 */
			pszAns[wp++] = work[0];
		}
	}

	return wp;
}

