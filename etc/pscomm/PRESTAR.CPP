#include <stdlib.h>
#include <sys\stat.h>
#include <sys\types.h>
#include <errno.h>
#include <io.h>
#include <mbstring.h>
#include "main.h"
#include "tty.h"
#include "prestar.h"
#include "resource.h"
#include "ext.h"
#include "dib.h"
#include "errors.h"

static enum pre_Mode {
	MODE_IDLE = 0,
	MODE_GRP00,
	MODE_GRP01,
	MODE_GRP02,
	MODE_RCVGRP
};

static enum pre_Stat {
	STAT_IDLE = 0,
	STAT_LNK,
	STAT_REP_GS00,
	STAT_REP_FS11,
	STAT_DAT11,
	STAT_REP_FE11,
	STAT_REP_GE00,
	STAT_REP_GS01,
	STAT_REP_FS00,
	STAT_DAT00,
	STAT_REP_FE00,
	STAT_REP_FS01,
	STAT_DAT01,
	STAT_REP_FE01,
	STAT_REP_FS04,
	STAT_DAT04,
	STAT_REP_FE04,
	STAT_REP_GE01,
	STAT_REP_GS02,
	STAT_REP_FS16,
	STAT_DAT16,
	STAT_REP_FE16,
	STAT_REP_FS17,
	STAT_DAT17,
	STAT_REP_FE17,
	STAT_REP_GE02,

	STAT_REP_GS10,
	STAT_REP_FS13,
	STAT_REP_FE13,
	STAT_REP_FS14,
	STAT_REP_FE14,
	STAT_REP_FS15,
	STAT_REP_FE15,
	STAT_REP_FS19,
	STAT_REP_FE19,

	STAT_REP_GS20,
	STAT_REP_FS18,
	STAT_REP_FE18
};

static enum pre_WParam {
	WP_TIMEOUT = 0,
	WP_COMMAND
};

static UINT pre_m_idTimer;
static int pre_m_ModeOld;
static int pre_m_StatOld;
static int pre_m_TOutOld;
static int pre_m_RetryOld;
static int pre_m_Mode;
static int pre_m_Stat;
static int pre_m_NextStat;
static int pre_m_TOut;
static int pre_m_Retry;
static char pre_m_SendBuf[512];
static char pre_m_Command[512];
static int pre_m_CmdPt;
static BOOL pre_m_fCmdStart;
static FILE *pre_m_fp;
static char pre_m_FileName[16];
static short pre_m_Shop;
static short pre_m_Group;
static short pre_m_File;
static FILE *pre_m_fpLog;
static BOOL pre_m_Loging;
static UINT pre_m_idTimer2;
static short pre_m_Anime;
static HBITMAP pre_m_hbmparent;					// イメージのハンドル
static HBITMAP pre_m_hbmchild;
static HBITMAP pre_m_hbmtrance1r;
static HBITMAP pre_m_hbmtrance2r;
static HBITMAP pre_m_hbmtrance3r;
static HBITMAP pre_m_hbmtrance4r;
static HBITMAP pre_m_hbmtrance5r;
static HBITMAP pre_m_hbmtrance1l;
static HBITMAP pre_m_hbmtrance2l;
static HBITMAP pre_m_hbmtrance3l;
static HBITMAP pre_m_hbmtrance4l;
static HBITMAP pre_m_hbmtrance5l;

/////////////////////////////////////////////////////////////////////////////
//	コンストラクタ

void XPrestar_XPrestar(HINSTANCE hinst, HWND hwnd)
{
	pre_m_ModeOld = -1;
	pre_m_StatOld = -1;
	pre_m_TOut = -1;
	pre_m_Retry = -1;

	pre_m_Mode = MODE_IDLE;
	pre_m_Stat = STAT_IDLE;
	pre_m_TOut = 0;
	pre_m_Retry = 0;
	pre_m_CmdPt = 0;
	pre_m_fCmdStart = FALSE;
	pre_m_idTimer = SetTimer(hwnd, ID_TIMER, 1000, NULL);	// タイムアウト用のタイマ作成
	pre_m_idTimer2 = SetTimer(hwnd, ID_ANIME, 200, NULL);	// タイムアウト用のタイマ作成
	pre_m_fp = 0;
	pre_m_fpLog = fopen("prestar.log", "w");
	pre_m_Loging = FALSE;

	pre_m_Group = pre_m_File = -1;
	pre_m_Anime = 1;

	pre_m_hbmparent = XPrestar_LoadBitmap("PARENT");
	pre_m_hbmchild = XPrestar_LoadBitmap("CHILD");
	pre_m_hbmtrance1r = XPrestar_LoadBitmap("TRANCE1");
	pre_m_hbmtrance2r = XPrestar_LoadBitmap("TRANCE2");
	pre_m_hbmtrance3r = XPrestar_LoadBitmap("TRANCE3");
	pre_m_hbmtrance4r = XPrestar_LoadBitmap("TRANCE4");
	pre_m_hbmtrance5r = XPrestar_LoadBitmap("TRANCE5");
	pre_m_hbmtrance1l = XPrestar_LoadBitmap("TRANCE6");
#if 1	// 小島
	pre_m_hbmtrance2l = XPrestar_LoadBitmap("TRANCE7");
	pre_m_hbmtrance3l = XPrestar_LoadBitmap("TRANCE8");
	pre_m_hbmtrance4l = XPrestar_LoadBitmap("TRANCE9");
	pre_m_hbmtrance5l = XPrestar_LoadBitmap("TRANCE10");
#endif

	tty_m_fLocalEcho = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	デストラクタ

void XPrestar_deXPrestar()
{
	if (pre_m_idTimer)
		KillTimer(tty_m_hwnd, pre_m_idTimer);
	if (pre_m_idTimer2)
		KillTimer(tty_m_hwnd, pre_m_idTimer2);
	if(pre_m_fp)
		fclose(pre_m_fp);
	if(pre_m_fpLog)
		fclose(pre_m_fpLog);

	if(pre_m_hbmparent)
		DeleteObject(pre_m_hbmparent);
	if(pre_m_hbmchild)
		DeleteObject(pre_m_hbmchild);
	if(pre_m_hbmtrance1r)
		DeleteObject(pre_m_hbmtrance1r);
	if(pre_m_hbmtrance2r)
		DeleteObject(pre_m_hbmtrance2r);
	if(pre_m_hbmtrance3r)
		DeleteObject(pre_m_hbmtrance3r);
	if(pre_m_hbmtrance4r)
		DeleteObject(pre_m_hbmtrance4r);
	if(pre_m_hbmtrance5r)
		DeleteObject(pre_m_hbmtrance5r);
	if(pre_m_hbmtrance1l)
		DeleteObject(pre_m_hbmtrance1l);
	if(pre_m_hbmtrance2l)
		DeleteObject(pre_m_hbmtrance2l);
	if(pre_m_hbmtrance3l)
		DeleteObject(pre_m_hbmtrance3l);
	if(pre_m_hbmtrance4l)
		DeleteObject(pre_m_hbmtrance4l);
	if(pre_m_hbmtrance5l)
		DeleteObject(pre_m_hbmtrance5l);

}

/////////////////////////////////////////////////////////////////////////////
//	モードにあわせた文字列取得

LPCSTR XPrestar_GetModeText(int nMode)
{
	char * pszText;

	switch (nMode) {
	case MODE_IDLE:
		pszText = "ｱｲﾄﾞﾙ";	break;
	case MODE_RCVGRP:
		pszText = "ｺﾏﾝﾄﾞ受信中";	break;
	case MODE_GRP00:
		pszText = "ｸﾞﾙｰﾌﾟ00店舗初期設定通信中";	break;
	case MODE_GRP01:
		pszText = "ｸﾞﾙｰﾌﾟ01商品関連マスター通信中";	break;
	case MODE_GRP02:
		pszText = "ｸﾞﾙｰﾌﾟ02発注関連マスター通信中";	break;
	default:
		pszText = "(ﾓｰﾄﾞが異常です)";	break;
	}

	return (LPCSTR)pszText;
}

/////////////////////////////////////////////////////////////////////////////
//	ステータスにあわせた文字列取得

LPCSTR XPrestar_GetStatText(int nMode, int nStat)
{
	char * pszText;

	switch (nMode) {
	case MODE_IDLE:
		switch (nStat) {
		case STAT_IDLE:
			pszText = "ｱｲﾄﾞﾙ";	break;
		default:
			pszText = "(ｽﾃｰﾀｽが異常です)";	break;
		}
		break;
	case MODE_RCVGRP:
		switch(nStat) {
		case STAT_LNK:
			pszText = "ｺﾏﾝﾄﾞ受信待ち";	break;
		case STAT_REP_GS10:
			pszText = "REP,G,S,10受信後ACK送信､ｺﾏﾝﾄﾞ待ち";	break;
		case STAT_REP_FS13:
			pszText = "REP,F,S,13受信後ACK送信､ｺﾏﾝﾄﾞ待ち";	break;
		case STAT_REP_FE13:
			pszText = "REP,F,E,13受信後ACK送信､ｺﾏﾝﾄﾞ待ち";	break;
		case STAT_REP_FS14:
			pszText = "REP,F,S,14受信後ACK送信､ｺﾏﾝﾄﾞ待ち";	break;
		case STAT_REP_FE14:
			pszText = "REP,F,E,14受信後ACK送信､ｺﾏﾝﾄﾞ待ち";	break;
		case STAT_REP_FS15:
			pszText = "REP,F,S,15受信後ACK送信､ｺﾏﾝﾄﾞ待ち";	break;
		case STAT_REP_FE15:
			pszText = "REP,F,E,15受信後ACK送信､ｺﾏﾝﾄﾞ待ち";	break;
		case STAT_REP_FS19:
			pszText = "REP,F,S,19受信後ACK送信､ｺﾏﾝﾄﾞ待ち";	break;
		case STAT_REP_FE19:
			pszText = "REP,F,E,19受信後ACK送信､ｺﾏﾝﾄﾞ待ち";	break;

		case STAT_REP_GS20:
			pszText = "REP,G,S,20受信後ACK送信､ｺﾏﾝﾄﾞ待ち";	break;
		case STAT_REP_FS18:
			pszText = "REP,F,S,18受信後ACK送信､ｺﾏﾝﾄﾞ待ち";	break;
		case STAT_REP_FE18:
			pszText = "REP,F,E,18受信後ACK送信､ｺﾏﾝﾄﾞ待ち";	break;
		default:
			pszText = "(ｽﾃｰﾀｽが異常です)";	break;
		}
		break;
	case MODE_GRP00:
		switch (nStat) {
		case STAT_LNK:
			pszText = "LNK,99送信後ACK待ち";	break;
		case STAT_REP_GS00:
			pszText = "REP,G,S,00送信後ACK待ち";	break;
		case STAT_REP_FS11:
			pszText = "REP,F,S,11送信後ACK待ち";	break;
		case STAT_DAT11:
			pszText = "DAT送信後ACK待ち";	break;
		case STAT_REP_FE11:
			pszText = "REP,F,E,11送信後ACK待ち";	break;
		case STAT_REP_GE00:
			pszText = "REP,G,E,00送信後ACK待ち";	break;
		default:
			pszText = "(ｽﾃｰﾀｽが異常です)";	break;
		}
		break;
	case MODE_GRP01:
		switch (nStat) {
		case STAT_LNK:
			pszText = "LNK,99送信後ACK待ち";	break;
		case STAT_REP_GS01:
			pszText = "REP,G,S,00送信後ACK待ち";	break;
		case STAT_REP_FS00:
			pszText = "REP,F,S,00送信後ACK待ち";	break;
		case STAT_DAT00:
			pszText = "品目マスターDAT送信後ACK待ち";	break;
		case STAT_REP_FE00:
			pszText = "REP,F,E,00送信後ACK待ち";	break;
		case STAT_REP_FS01:
			pszText = "REP,F,S,01送信後ACK待ち";	break;
		case STAT_DAT01:
			pszText = "分類マスターDAT送信後ACK待ち";	break;
		case STAT_REP_FE01:
			pszText = "REP,F,E,01送信後ACK待ち";	break;
		case STAT_REP_FS04:
			pszText = "REP,F,S,04送信後ACK待ち";	break;
		case STAT_DAT04:
			pszText = "税率マスターDAT送信後ACK待ち";	break;
		case STAT_REP_FE04:
			pszText = "REP,F,E,04送信後ACK待ち";	break;
		case STAT_REP_GE01:
			pszText = "REP,G,E,00送信後ACK待ち";	break;
		default:
			pszText = "(ｽﾃｰﾀｽが異常です)";	break;
		}
		break;
	case MODE_GRP02:
		switch (nStat) {
		case STAT_LNK:
			pszText = "LNK,99送信後ACK待ち";	break;
		case STAT_REP_GS02:
			pszText = "REP,G,S,02送信後ACK待ち";	break;
		case STAT_REP_FS16:
			pszText = "REP,F,S,16送信後ACK待ち";	break;
		case STAT_DAT16:
			pszText = "材料マスターDAT送信後ACK待ち";	break;
		case STAT_REP_FE16:
			pszText = "REP,F,E,16送信後ACK待ち";	break;
		case STAT_REP_FS17:
			pszText = "REP,F,S,17送信後ACK待ち";	break;
		case STAT_DAT17:
			pszText = "材料分類マスターDAT送信後ACK待ち";	break;
		case STAT_REP_FE17:
			pszText = "REP,F,E,17送信後ACK待ち";	break;
		case STAT_REP_GE02:
			pszText = "REP,G,E,02送信後ACK待ち";	break;
		default:
			pszText = "(ｽﾃｰﾀｽが異常です)";	break;
		}
		break;
	default:
		pszText = "(ﾓｰﾄﾞが異常です)";	break;
	}

	return (LPCSTR)pszText;
}

/////////////////////////////////////////////////////////////////////////////
//	エラー テキスト取り出し

LPCSTR XPrestar_GetErrorText(char * psz, char * pszBuf)
{
	for (int i = 5; psz[i] != '\0' && psz[i] != ','; i++) {
		pszBuf[i-5] = psz[i];
	}
	pszBuf[i-5] = '\0';

	return (LPCSTR)pszBuf;
}

/////////////////////////////////////////////////////////////////////////////
//	ステータスダイアログの変更

void XPrestar_ChangeStatus()
{
	if (ghwndStatusDlg) {
		if (pre_m_ModeOld != pre_m_Mode ||
		    pre_m_StatOld != pre_m_Stat ||
		    pre_m_TOutOld != pre_m_TOut ||
		    pre_m_RetryOld != pre_m_Retry) {
			pre_m_ModeOld = pre_m_Mode;
			pre_m_StatOld = pre_m_Stat;
			pre_m_TOutOld = pre_m_TOut;
			pre_m_RetryOld = pre_m_Retry;

			SetDlgItemText(ghwndStatusDlg, IDC_EDIT_MODE, XPrestar_GetModeText(pre_m_ModeOld));
			SetDlgItemText(ghwndStatusDlg, IDC_EDIT_STAT, XPrestar_GetStatText(pre_m_ModeOld, pre_m_StatOld));
			SetDlgItemInt(ghwndStatusDlg, IDC_EDIT_TIMEOUT, pre_m_TOutOld, TRUE);
			SetDlgItemInt(ghwndStatusDlg, IDC_EDIT_RETRY, pre_m_Retry, TRUE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//	WM_TIMER処理

void XPrestar_OnTimer(WPARAM wParam)
{
	switch(wParam) {
	case ID_TIMER:
		XPrestar_ChangeStatus();
		if (pre_m_Mode != MODE_IDLE) {
			if (++pre_m_TOut >= 10) {
				pre_m_TOut = 0;
				SendMessage(tty_m_hwnd, WM_PRESTAR, WP_TIMEOUT, 0L);
			}
		}
		break;
	case ID_ANIME:
		if(++pre_m_Anime > 5)
			pre_m_Anime = 1;

		RECT UpdateRect = {216,64,424,128};
		InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	WM_PRESTAR処理

void XPrestar_OnPrestar(WPARAM wParam)
{
	char szTitleBuf[256];
	char szMessageBuf[256];

	XPrestar_ChangeStatus();
	if (wParam == WP_TIMEOUT) {
		if (++pre_m_Retry < 3) {
			XPrestar_WriteComm2(pre_m_SendBuf, lstrlen((LPSTR)pre_m_SendBuf));	// 前回送った伝聞を再送する
		} else {
			wsprintf(szTitleBuf, "%s ｴﾗｰ", XPrestar_GetModeText(pre_m_Mode));
			wsprintf(szMessageBuf, "ｽﾃｰﾀｽ \"%s\" でﾀｲﾑｱｳﾄしました。", XPrestar_GetStatText(pre_m_Mode, pre_m_Stat));

			XPrestar_InitStatus();	// 初期状態に戻る

			MessageBox(tty_m_hwnd, szMessageBuf, szTitleBuf, MB_OK|MB_APPLMODAL);
		}
	} else if (wParam == WP_COMMAND) {
		switch (pre_m_Mode) {
		case MODE_IDLE:		XPrestar_OnIdle();		break;
		case MODE_RCVGRP:	XPrestar_RcvAction();	break;
		case MODE_GRP00:	XPrestar_Grp00Action();	break;
		case MODE_GRP01:	XPrestar_Grp01Action();	break;
		case MODE_GRP02:	XPrestar_Grp02Action();	break;
		default:	break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////
//	WM_COMMNOTIFYメッセージ処理

void XPrestar_OnCommNotify(WPARAM wParam, LPARAM lParam)
{
	char buf[128];
	int rtn;
	BOOL fScroll = FALSE;

	if (!tty_m_fConnect)
		return;

	if (lParam & CN_EVENT) {	// 通信デバイスイベント発生
		GetCommEventMask(tty_m_idComDev, EV_ERR|EV_CTS);
		XTTY_CommErrorMessage();
	}

	memset(buf, 0, sizeof(buf));
	if (lParam & CN_RECEIVE) {	// 受信イベント発生
		rtn = ReadComm(tty_m_idComDev, buf, sizeof(buf));
		XPrestar_WriteLog(buf, rtn);

		if (rtn <= 0) {
			XTTY_CommErrorMessage();
			return;
		}

		for (int i = 0; i < rtn; i++) {
			if (pre_m_fCmdStart)
				pre_m_Command[pre_m_CmdPt++] = buf[i];

			if (buf[i] == ETX) {
				pre_m_Command[pre_m_CmdPt] = '\0';
				SendMessage(tty_m_hwnd, WM_PRESTAR, WP_COMMAND, 0L);
				pre_m_CmdPt = 0;
				pre_m_fCmdStart = FALSE;
			} else if (buf[i] == STX) {
				pre_m_CmdPt = 0;
				pre_m_fCmdStart = TRUE;
				pre_m_Command[pre_m_CmdPt++] = buf[i];
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//	ステータスを初期状態に戻す

void XPrestar_InitStatus()
{
	pre_m_Mode = MODE_IDLE;
	pre_m_Stat = STAT_IDLE;
	pre_m_TOut = 0;
	pre_m_Retry = 0;

	RECT UpdateRect = {0,0,640,DEFWNDHSIZE};
	InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
//	グループ00の処理

void XPrestar_Grp00Action()
{
	int len;
	char buf[256];
	char buf2[256];
	char szTitleBuf[256];
	char szMessageBuf[256];
	RECT UpdateRect = {0,0,640,DEFWNDHSIZE};

	if (XPrestar_SumCheck(pre_m_Command) == FALSE)	return;
	if (XPrestar_FunctionCheck(pre_m_Command) == FALSE)	return;

	if (_strnicmp("NAK", &pre_m_Command[1], 3) == 0) {
		wsprintf(szTitleBuf, "%s ｴﾗｰ", XPrestar_GetModeText(pre_m_Mode));
		wsprintf(szMessageBuf, "ｽﾃｰﾀｽ \"%s\" で、NAKによる %s です。",
			XPrestar_GetStatText(pre_m_Mode, pre_m_Stat), XPrestar_GetErrorText(pre_m_Command, buf2));
		XPrestar_InitStatus();
		MessageBox(tty_m_hwnd, szMessageBuf, szTitleBuf, MB_OK|MB_APPLMODAL);
	} else if (_strnicmp("ACK", &pre_m_Command[1], 3) == 0) {
		switch (pre_m_Stat) {
		case STAT_LNK:		// LNK送信後のACK待ち
			// ACKが来たので次の伝聞を出力して、状態を変化させる
			len = XPrestar_MakeCommand(pre_m_SendBuf, "REP", 'G', 'S', 0);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = STAT_REP_GS00;
			pre_m_TOut = 0;
			pre_m_Retry = 0;
			pre_m_Group = 0;
			InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);
			break;
		case STAT_REP_GS00:	// REP,G,S,00送信後のACK待ち
			// ACKが来たので次の伝聞を出力して、状態を変化させる
			len = XPrestar_MakeCommand(pre_m_SendBuf, "REP", 'F', 'S',  11);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = STAT_REP_FS11;
			pre_m_TOut = pre_m_Retry = 0;
			pre_m_File = 11;
			InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);
			break;
		case STAT_REP_FS11:	// REP,F,S,11送信後のACK待ち
			// ACKが来たので次の伝聞を出力して、状態を変化させる
			if(pre_m_fp == NULL)
				pre_m_fp = fopen("G00F11.CSV", "r");
			if(fgets(buf, 255, pre_m_fp))
				pre_m_NextStat = STAT_REP_FS11;
			else {
				if(pre_m_fp) {if(fclose(pre_m_fp))	PostQuitMessage(255);	pre_m_fp = NULL;}

				// ACKが来たので伝聞を出力して、状態を変化させる
				len = XPrestar_MakeCommand(pre_m_SendBuf, "REP", 'F', 'E', 11);
				XPrestar_WriteComm2(pre_m_SendBuf, len);
				pre_m_Stat = STAT_REP_FE11;
				pre_m_TOut = pre_m_Retry = 0;
				pre_m_File = -1;
				InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);
				break;
			}
			buf[strlen(buf) - 1] = '\0';

			len = XPrestar_MakeCommand(pre_m_SendBuf, "DAT", buf);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = STAT_REP_FS11;
			pre_m_TOut = pre_m_Retry = 0;
			break;
		case STAT_REP_FE11:	// REP,F,E送信後のACK待ち
			// ACKが来たので伝聞を出力して、状態を変化させる
			len = XPrestar_MakeCommand(pre_m_SendBuf, "REP", 'G', 'E', 0);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = STAT_REP_GE00;
			pre_m_TOut = pre_m_Retry = 0;
			pre_m_Group = -1;
			InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);
			break;
		case STAT_REP_GE00:	// REP,G,E,送信後のACK待ち
			XPrestar_InitStatus();	// 後は、終わるだけなので、初期状態に移行する
			break;
		default:
			XPrestar_InitStatus();
			break;
		}
	} else {
		XPrestar_InitStatus();
	}
}

/////////////////////////////////////////////////////////////////////////////
//	グループ01の処理

void XPrestar_Grp01Action()
{
	int len;
	char buf[256];
	char buf2[256];
	char szTitleBuf[256];
	char szMessageBuf[256];
	RECT UpdateRect = {0,0,640,DEFWNDHSIZE};

	if (XPrestar_SumCheck(pre_m_Command) == FALSE)	return;
	if (XPrestar_FunctionCheck(pre_m_Command) == FALSE)	return;

	if (_strnicmp("NAK", &pre_m_Command[1], 3) == 0) {
		wsprintf(szTitleBuf, "%s ｴﾗｰ", XPrestar_GetModeText(pre_m_Mode));
		wsprintf(szMessageBuf, "ｽﾃｰﾀｽ \"%s\" で、NAKによる %s です。",
			XPrestar_GetStatText(pre_m_Mode, pre_m_Stat), XPrestar_GetErrorText(pre_m_Command, buf2));
		XPrestar_InitStatus();
		if(pre_m_fp) {if(fclose(pre_m_fp))	PostQuitMessage(255);	pre_m_fp = NULL;}

		MessageBox(tty_m_hwnd, szMessageBuf, szTitleBuf, MB_OK|MB_APPLMODAL);
	} else if (_strnicmp("ACK", &pre_m_Command[1], 3) == 0) {
		switch (pre_m_Stat) {
		case STAT_LNK:		// LNK送信後のACK待ち
			// ACKが来たので次の伝聞を出力して、状態を変化させる
			len = XPrestar_MakeCommand(pre_m_SendBuf, "REP", 'G', 'S', 1);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = STAT_REP_GS01;
			pre_m_TOut = pre_m_Retry = 0;
			pre_m_Group = 1;
			InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);
			break;
		case STAT_REP_GS01:	// REP,G,S,01送信後のACK待ち
			// ACKが来たので次の伝聞を出力して、状態を変化させる
			len = XPrestar_MakeCommand(pre_m_SendBuf, "REP", 'F', 'S',  0);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = STAT_REP_FS00;
			pre_m_TOut = pre_m_Retry = 0;
			pre_m_File = 0;
			InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);
			break;
		case STAT_REP_FS00:	// REP,F,S,00送信後のACK待ち
			// ACKが来たので次の伝聞を出力して、状態を変化させる
			if(pre_m_fp == NULL)
				pre_m_fp = fopen("G01F00.CSV", "r");
			if(fgets(buf, 255, pre_m_fp))
				pre_m_NextStat = STAT_REP_FS00;
			else {
				if(pre_m_fp) {if(fclose(pre_m_fp))	PostQuitMessage(255);	pre_m_fp = NULL;}

				// ACKが来たので伝聞を出力して、状態を変化させる
				len = XPrestar_MakeCommand(pre_m_SendBuf, "REP", 'F', 'E', 0);
				XPrestar_WriteComm2(pre_m_SendBuf, len);
				pre_m_Stat = STAT_REP_FE00;
				pre_m_TOut = pre_m_Retry = 0;
				pre_m_File = -1;
				InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);
				break;
			}
			buf[strlen(buf) - 1] = '\0';

			len = XPrestar_MakeCommand(pre_m_SendBuf, "DAT", buf);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = pre_m_NextStat;
			pre_m_TOut = pre_m_Retry = 0;
			break;
		case STAT_REP_FE00:	// REP,F,E送信後のACK待ち
			// ACKが来たので伝聞を出力して、状態を変化させる
			len = XPrestar_MakeCommand(pre_m_SendBuf, "REP", 'F', 'S', 1);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = STAT_REP_FS01;
			pre_m_TOut = pre_m_Retry = 0;
			pre_m_File = 1;
			InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);
			break;
		case STAT_REP_FS01:	// REP,F,S,00送信後のACK待ち
			// ACKが来たので次の伝聞を出力して、状態を変化させる
			if(pre_m_fp == NULL)
				pre_m_fp = fopen("G01F01.CSV", "r");
			if(fgets(buf, 255, pre_m_fp))
				pre_m_NextStat = STAT_REP_FS01;
			else {
				pre_m_NextStat = STAT_DAT01;
				if(pre_m_fp) {if(fclose(pre_m_fp))	PostQuitMessage(255);	pre_m_fp = NULL;}

				// ACKが来たので伝聞を出力して、状態を変化させる
				len = XPrestar_MakeCommand(pre_m_SendBuf, "REP", 'F', 'E', 1);
				XPrestar_WriteComm2(pre_m_SendBuf, len);
				pre_m_Stat = STAT_REP_FE01;
				pre_m_TOut = pre_m_Retry = 0;
				pre_m_File = -1;
				InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);
				break;
			}
			buf[strlen(buf) - 1] = '\0';

			len = XPrestar_MakeCommand(pre_m_SendBuf, "DAT", buf);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = pre_m_NextStat;
			pre_m_TOut = pre_m_Retry = 0;
			break;
		case STAT_REP_FE01:	// REP,F,E送信後のACK待ち
			// ACKが来たので伝聞を出力して、状態を変化させる
			len = XPrestar_MakeCommand(pre_m_SendBuf, "REP", 'F', 'S', 4);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = STAT_REP_FS04;
			pre_m_TOut = pre_m_Retry = 0;
			pre_m_File = 4;
			InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);
			break;
		case STAT_REP_FS04:	// REP,F,S,00送信後のACK待ち
			// ACKが来たので次の伝聞を出力して、状態を変化させる
			if(pre_m_fp == NULL)
				pre_m_fp = fopen("G01F04.CSV", "r");
			if(fgets(buf, 255, pre_m_fp))
				pre_m_NextStat = STAT_REP_FS04;
			else {
				pre_m_NextStat = STAT_DAT04;
				if(pre_m_fp) {if(fclose(pre_m_fp))	PostQuitMessage(255);	pre_m_fp = NULL;}

				// ACKが来たので伝聞を出力して、状態を変化させる
				len = XPrestar_MakeCommand(pre_m_SendBuf, "REP", 'F', 'E', 4);
				XPrestar_WriteComm2(pre_m_SendBuf, len);
				pre_m_Stat = STAT_REP_FE04;
				pre_m_TOut = pre_m_Retry = 0;
				pre_m_File = -1;
				InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);
				break;
			}
			buf[strlen(buf) - 1] = '\0';

			len = XPrestar_MakeCommand(pre_m_SendBuf, "DAT", buf);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = pre_m_NextStat;
			pre_m_TOut = pre_m_Retry = 0;
			break;
		case STAT_REP_FE04:	// REP,F,E送信後のACK待ち
			// ACKが来たので伝聞を出力して、状態を変化させる
			len = XPrestar_MakeCommand(pre_m_SendBuf, "REP", 'G', 'E', 1);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = STAT_REP_GE01;
			pre_m_TOut = pre_m_Retry = 0;
			pre_m_Group = -1;
			InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);
			break;
		case STAT_REP_GE01:	// REP,G,E,送信後のACK待ち
			XPrestar_InitStatus();	// 後は、終わるだけなので、初期状態に移行する
			break;
		default:
			XPrestar_InitStatus();
			break;
		}
	} else {
		XPrestar_InitStatus();
	}
}

/////////////////////////////////////////////////////////////////////////////
//	グループ02の処理

void XPrestar_Grp02Action()
{
	int len;
	char buf[256];
	char buf2[256];
	char szTitleBuf[256];
	char szMessageBuf[256];
	RECT UpdateRect = {0,0,640,DEFWNDHSIZE};

	if (XPrestar_SumCheck(pre_m_Command) == FALSE)	return;
	if (XPrestar_FunctionCheck(pre_m_Command) == FALSE)	return;

	if (_strnicmp("NAK", &pre_m_Command[1], 3) == 0) {
		wsprintf(szTitleBuf, "%s ｴﾗｰ", XPrestar_GetModeText(pre_m_Mode));
		wsprintf(szMessageBuf, "ｽﾃｰﾀｽ \"%s\" で、NAKによる %s です。",
			XPrestar_GetStatText(pre_m_Mode, pre_m_Stat), XPrestar_GetErrorText(pre_m_Command, buf2));
		XPrestar_InitStatus();
		MessageBox(tty_m_hwnd, szMessageBuf, szTitleBuf, MB_OK|MB_APPLMODAL);
	} else if (_strnicmp("ACK", &pre_m_Command[1], 3) == 0) {
		switch (pre_m_Stat) {
		case STAT_LNK:		// LNK送信後のACK待ち
			// ACKが来たので次の伝聞を出力して、状態を変化させる
			len = XPrestar_MakeCommand(pre_m_SendBuf, "REP", 'G', 'S', 2);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = STAT_REP_GS02;
			pre_m_TOut = 0;
			pre_m_Retry = 0;
			pre_m_Group = 2;
			InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);
			break;
		case STAT_REP_GS02:	// REP,G,S,01送信後のACK待ち
			// ACKが来たので次の伝聞を出力して、状態を変化させる
			len = XPrestar_MakeCommand(pre_m_SendBuf, "REP", 'F', 'S', 16);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = STAT_REP_FS16;
			pre_m_TOut = pre_m_Retry = 0;
			pre_m_File = 16;
			InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);
			break;
		case STAT_REP_FS16:	// REP,F,S,00送信後のACK待ち
			// ACKが来たので次の伝聞を出力して、状態を変化させる
			if(pre_m_fp == NULL)
				pre_m_fp = fopen("G02F16.CSV", "r");
			if(fgets(buf, 255, pre_m_fp))
				pre_m_NextStat = STAT_REP_FS16;
			else {
				pre_m_NextStat = STAT_DAT16;
				if(pre_m_fp) {if(fclose(pre_m_fp))	PostQuitMessage(255);	pre_m_fp = NULL;}

				// ACKが来たので伝聞を出力して、状態を変化させる
				len = XPrestar_MakeCommand(pre_m_SendBuf, "REP", 'F', 'E', 16);
				XPrestar_WriteComm2(pre_m_SendBuf, len);
				pre_m_Stat = STAT_REP_FE16;
				pre_m_TOut = pre_m_Retry = 0;
				pre_m_File = -1;
				InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);
				break;
			}
			buf[strlen(buf) - 1] = '\0';

			len = XPrestar_MakeCommand(pre_m_SendBuf, "DAT", buf);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = pre_m_NextStat;
			pre_m_TOut = pre_m_Retry = 0;
			break;
		case STAT_REP_FE16:	// REP,F,E送信後のACK待ち
			// ACKが来たので伝聞を出力して、状態を変化させる
			len = XPrestar_MakeCommand(pre_m_SendBuf, "REP", 'F', 'S', 17);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = STAT_REP_FS17;
			pre_m_TOut = pre_m_Retry = 0;
			pre_m_File = 17;
			InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);
			break;
		case STAT_REP_FS17:	// REP,F,S,00送信後のACK待ち
			// ACKが来たので次の伝聞を出力して、状態を変化させる
			if(pre_m_fp == NULL)
				pre_m_fp = fopen("G02F17.CSV", "r");
			if(fgets(buf, 255, pre_m_fp))
				pre_m_NextStat = STAT_REP_FS17;
			else {
				pre_m_NextStat = STAT_DAT17;
				if(pre_m_fp) {if(fclose(pre_m_fp))	PostQuitMessage(255);	pre_m_fp = NULL;}

				// ACKが来たので伝聞を出力して、状態を変化させる
				len = XPrestar_MakeCommand(pre_m_SendBuf, "REP", 'F', 'E', 17);
				XPrestar_WriteComm2(pre_m_SendBuf, len);
				pre_m_Stat = STAT_REP_FE17;
				pre_m_TOut = pre_m_Retry = 0;
				pre_m_File = -1;
				InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);
				break;
			}
			buf[strlen(buf) - 1] = '\0';

			len = XPrestar_MakeCommand(pre_m_SendBuf, "DAT", buf);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = pre_m_NextStat;
			pre_m_TOut = pre_m_Retry = 0;
			break;
		case STAT_REP_FE17:	// REP,F,E送信後のACK待ち
			// ACKが来たので伝聞を出力して、状態を変化させる
			len = XPrestar_MakeCommand(pre_m_SendBuf, "REP", 'G', 'E', 2);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = STAT_REP_GE02;
			pre_m_TOut = pre_m_Retry = 0;
			pre_m_Group = -1;
			InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);
			break;
		case STAT_REP_GE02:	// REP,G,E,送信後のACK待ち
			XPrestar_InitStatus();	// 後は、終わるだけなので、初期状態に移行する
			break;
		default:
			XPrestar_InitStatus();
			break;
		}
	} else {
		XPrestar_InitStatus();
	}
}

/////////////////////////////////////////////////////////////////////////////
//	コマンドの作成

int XPrestar_MakeCommand(char * buf, char * fnc, int p1, int p2, int p3)
{
	char * msg[] = {
		"ﾊﾟﾗﾒｰﾀｲﾘｰｶﾞﾙ",		// p1 == 0
		"SUMｴﾗｰ",		// p1 == 1
		"ﾌｧｲﾙｸﾞﾙｰﾌﾟｴﾗｰ",	// p1 == 2
		"ﾌｧｲﾙNOｴﾗｰ",		// p1 == 3
		"ﾌｧﾝｸｼｮﾝｲﾘｰｶﾞﾙ",	// p1 == 4
		"ｽﾃｰﾀｽｴﾗｰ"		// p1 == 5
	};

	if (lstrcmp("LNK", fnc) == 0) {
		wsprintf((LPSTR)buf, "%c%s,%02d,@@%c", STX, (LPSTR)fnc, p1, ETX);
	} else if (lstrcmp("REP", fnc) == 0) {
		wsprintf((LPSTR)buf, "%c%s,%c,%c,%02d,@@%c", STX, (LPSTR)fnc, p1, p2, p3, ETX);
	} else if (lstrcmp("ACK", fnc) == 0) {
		wsprintf((LPSTR)buf, "%c%s,@@%c", STX, (LPSTR)fnc, ETX);
	} else if (lstrcmp("NAK", fnc) == 0) {
		wsprintf((LPSTR)buf, "%c%s,ERROR:%s,@@%c", STX, (LPSTR)fnc, (LPSTR)msg[p1], ETX);
	} else {
		return 0;
	}

	int sum = 0;
	int len = lstrlen((LPSTR)buf);
	for (int i = 0; i < len - 3; i++)
		sum += buf[i];

	char temp[8];
	wsprintf((LPSTR)temp, "%02X", sum & 0xff);
	buf[len-3] = temp[0];
	buf[len-2] = temp[1];

	return len;
}

/////////////////////////////////////////////////////////////////////////////
//	コマンドの作成

int XPrestar_MakeCommand(char * buf, char * fnc, char * pszDataString)
{
	char * msg[] = {
		"ﾊﾟﾗﾒｰﾀｲﾘｰｶﾞﾙ",		// p1 == 0
		"SUMｴﾗｰ",		// p1 == 1
		"ﾌｧｲﾙｸﾞﾙｰﾌﾟｴﾗｰ",	// p1 == 2
		"ﾌｧｲﾙNOｴﾗｰ",		// p1 == 3
		"ﾌｧﾝｸｼｮﾝｲﾘｰｶﾞﾙ",	// p1 == 4
		"ｽﾃｰﾀｽｴﾗｰ"		// p1 == 5
	};

	if (lstrcmp("DAT", fnc) == 0) {
		wsprintf((LPSTR)buf, "%c%s,%s,@@%c", STX, (LPSTR)fnc, (LPSTR)pszDataString, ETX);
	} else {
		return 0;
	}

	int sum = 0;
	int len = lstrlen((LPSTR)buf);
	for (int i = 0; i < len - 3; i++)
		sum += buf[i];

	char temp[8];
	wsprintf((LPSTR)temp, "%02X", sum & 0xff);
	buf[len-3] = temp[0];
	buf[len-2] = temp[1];

	return len;
}

/////////////////////////////////////////////////////////////////////////////
//	サムチェック
BOOL XPrestar_SumCheck(char *buf, char flg)
{
	char str[32];
	unsigned int i;
	int sum;
	char szMessageBuf[256];

	for(i = sum = 0; i < (strlen(buf) - 3); i++) {
		sum += buf[i];
	}

	strcpy(str, "0x");
	strcat(str, &buf[strlen(buf) - 3]);
	if((sum & 0x00ff) == strtol(str, 0, 16))
		return TRUE;

	wsprintf(szMessageBuf, "ｽﾃｰﾀｽ \"%s\" で、SUMｴﾗｰです。",
		XPrestar_GetStatText(pre_m_Mode, pre_m_Stat));
	if(flg) {
		int len = XPrestar_MakeCommand(pre_m_SendBuf, "NAK", 1);
		XPrestar_WriteComm2(pre_m_SendBuf, len);
	}
	XPrestar_InitStatus();
	MessageBox(tty_m_hwnd, szMessageBuf, "SUMｴﾗｰ", MB_OK|MB_APPLMODAL);

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	ファンクションチェック
BOOL XPrestar_FunctionCheck(char *buf, char flg)
{
	char szMessageBuf[256];

	if (_strnicmp("LNK", &buf[1], 3) == 0)
		return TRUE;
	if (_strnicmp("REP", &buf[1], 3) == 0)
		return TRUE;
	if (_strnicmp("DAT", &buf[1], 3) == 0)
		return TRUE;
	if (_strnicmp("ACK", &buf[1], 3) == 0)
		return TRUE;
	if (_strnicmp("NAK", &buf[1], 3) == 0)
		return TRUE;
	if (_strnicmp("TST", &buf[1], 3) == 0)
		return TRUE;

	wsprintf(szMessageBuf, "ｽﾃｰﾀｽ \"%s\" で、ﾌｧﾝｸｼｮﾝ ｲﾘｰｶﾞﾙです。",
		XPrestar_GetStatText(pre_m_Mode, pre_m_Stat));
	if(flg) {
		int len = XPrestar_MakeCommand(pre_m_SendBuf, "NAK", 4);
		XPrestar_WriteComm2(pre_m_SendBuf, len);
	}
	XPrestar_InitStatus();
	MessageBox(tty_m_hwnd, szMessageBuf, "ﾌｧﾝｸｼｮﾝｲﾘｰｶﾞﾙ", MB_OK|MB_APPLMODAL);
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	ステータスチェック
BOOL XPrestar_StatusCheck(char *buf, char *cmp, char flg, char dialog)
{
	char szMessageBuf[256];

	if (_strnicmp("NAK", &buf[1], 3) == 0)
		flg = 0;

	if (_strnicmp(cmp, &buf[1], strlen(cmp)) == 0)
		return TRUE;

	if (dialog) {
		wsprintf(szMessageBuf, "ｽﾃｰﾀｽ \"%s\" で、ｽﾃｰﾀｽｴﾗｰです。",
			XPrestar_GetStatText(pre_m_Mode, pre_m_Stat));
		if(flg) {
			int len = XPrestar_MakeCommand(pre_m_SendBuf, "NAK", 5);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
		}
		XPrestar_InitStatus();
		MessageBox(tty_m_hwnd, szMessageBuf, "ｽﾃｰﾀｽｴﾗｰ", MB_OK|MB_APPLMODAL);
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	パラメータチェック
BOOL XPrestar_ParamCheck(char *buf, char flg)
{
	char szMessageBuf[256];

	if (_strnicmp("LNK", &buf[1], 3) == 0)
//		if(strlen(buf) == 11)
			return TRUE;
	if (_strnicmp("REP", &buf[1], 3) == 0)
		if(strlen(buf) == 15)
			return TRUE;
	if (_strnicmp("DAT", &buf[1], 3) == 0)
			return TRUE;
	if (_strnicmp("ACK", &buf[1], 3) == 0)
		if(strlen(buf) == 8)
			return TRUE;
	if (_strnicmp("NAK", &buf[1], 3) == 0)
			return TRUE;
	if (_strnicmp("TST", &buf[1], 3) == 0)
			return TRUE;

	wsprintf(szMessageBuf, "ｽﾃｰﾀｽ \"%s\" で、ﾊﾟﾗﾒｰﾀｲﾘｰｶﾞﾙです。",
		XPrestar_GetStatText(pre_m_Mode, pre_m_Stat));
	if(flg) {
		int len = XPrestar_MakeCommand(pre_m_SendBuf, "NAK", 0);
		XPrestar_WriteComm2(pre_m_SendBuf, len);
	}
	XPrestar_InitStatus();
	MessageBox(tty_m_hwnd, szMessageBuf, "ﾊﾟﾗﾒｰﾀｲﾘｰｶﾞﾙ", MB_OK|MB_APPLMODAL);
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	ﾌｧｲﾙ番号、ｸﾞﾙｰﾌﾟ番号チェック
BOOL XPrestar_FileGroupCheck(char *buf, char flg)
{
	char szMessageBuf[256];
	char str[16];

	if (_strnicmp("REP,G", &buf[1], 5) == 0) {
		strcpy(str, &buf[9]);
		switch(strtol(str, 0, 10)) {
		case 10:
		case 20:
			return TRUE;
		default:
			wsprintf(szMessageBuf, "ｽﾃｰﾀｽ \"%s\" で、ﾌｧｲﾙｸﾞﾙｰﾌﾟｴﾗｰです。",
				XPrestar_GetStatText(pre_m_Mode, pre_m_Stat));
			if(flg) {
				int len = XPrestar_MakeCommand(pre_m_SendBuf, "NAK", 2);
				XPrestar_WriteComm2(pre_m_SendBuf, len);
			}
			XPrestar_InitStatus();
			MessageBox(tty_m_hwnd, szMessageBuf, "ﾌｧｲﾙｸﾞﾙｰﾌﾟ､ﾌｧｲﾙﾌｧｲﾙNo.ｴﾗｰ", MB_OK|MB_APPLMODAL);
			return FALSE;
		}
	}
	if (_strnicmp("REP,F", &buf[1], 5) == 0) {
		strcpy(str, &buf[9]);
		switch(strtol(str, 0, 10)) {
		case 11:
		case 0:
		case 1:
		case 4:
		case 16:
		case 17:
		case 13:
		case 14:
		case 15:
		case 19:
		case 18:
			return TRUE;
		default:
			wsprintf(szMessageBuf, "ｽﾃｰﾀｽ \"%s\" で、ﾌｧｲﾙ ﾌｧｲﾙNo.ｴﾗｰです。",
				XPrestar_GetStatText(pre_m_Mode, pre_m_Stat));
			if(flg) {
				int len = XPrestar_MakeCommand(pre_m_SendBuf, "NAK", 3);
				XPrestar_WriteComm2(pre_m_SendBuf, len);
			}
			XPrestar_InitStatus();
			MessageBox(tty_m_hwnd, szMessageBuf, "ﾌｧｲﾙｸﾞﾙｰﾌﾟ､ﾌｧｲﾙﾌｧｲﾙNo.ｴﾗｰ", MB_OK|MB_APPLMODAL);
			return FALSE;
		}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	店ＩＤ番号切り出し
int XPrestar_SplitShopNum(char *buf)
{
	int num;
	char str[8];

	memset(str, 0, sizeof(str));
	memcpy(str, &buf[5], 2);
	for (num = 0; num < 8; num++)
		if(str[num] < 0x30 && 0x39 < str[num])
			str[num] = 0;
	num = (int)strtol(str, 0, 10);

	return num;
}

/////////////////////////////////////////////////////////////////////////////
//	グループ番号切り出し
int XPrestar_SplitGroupNum(char *buf)
{
	int num;
	char str[8];

	memcpy(str, &buf[9], 2);
	num = (int)strtol(str, 0, 10);

	return num;
}

/////////////////////////////////////////////////////////////////////////////
//	ファイル番号切り出し
int XPrestar_SplitFileNum(char *buf)
{
	int num;
	char str[8];

	memcpy(str, &buf[9], 2);
	num = (int)strtol(str, 0, 10);

	return num;
}

/////////////////////////////////////////////////////////////////////////////
//	通信データの書き込み
BOOL XPrestar_WriteData(char *buf)
{
	int len;
	char str[1024];

	len = strlen(buf);
	memset(str, 0, len);
	memcpy(str, &buf[5], len - 9);
	strcat(str, "\n");

	return (fputs(str, pre_m_fp) == EOF) ?FALSE :TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	グループ00通信開始

void XPrestar_OnGrp00()
{
	if (pre_m_Mode != MODE_IDLE)
		return;

	pre_m_Mode = MODE_GRP00;
	pre_m_Stat = STAT_LNK;
	pre_m_TOut = 0;

	if(pre_m_fpLog) {
		fclose(pre_m_fpLog);
		pre_m_fpLog = 0;
	}
	pre_m_fpLog = fopen("prestar.log", "w");

	RECT UpdateRect = {0,0,640,DEFWNDHSIZE};
	InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);

	int len = XPrestar_MakeCommand(pre_m_SendBuf, "LNK", 99);
	XPrestar_WriteComm2(pre_m_SendBuf, len);
}

/////////////////////////////////////////////////////////////////////////////
//	グループ01通信開始

void XPrestar_OnGrp01()
{
	if (pre_m_Mode != MODE_IDLE)
		return;

	pre_m_Mode = MODE_GRP01;
	pre_m_Stat = STAT_LNK;
	pre_m_TOut = 0;

	if(pre_m_fpLog) {
		fclose(pre_m_fpLog);
		pre_m_fpLog = 0;
	}
	pre_m_fpLog = fopen("prestar.log", "w");

	RECT UpdateRect = {0,0,640,DEFWNDHSIZE};
	InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);

	int len = XPrestar_MakeCommand(pre_m_SendBuf, "LNK", 99);
	XPrestar_WriteComm2(pre_m_SendBuf, len);
}

/////////////////////////////////////////////////////////////////////////////
//	グループ02通信開始

void XPrestar_OnGrp02()
{
	if (pre_m_Mode != MODE_IDLE)
		return;

	pre_m_Mode = MODE_GRP02;
	pre_m_Stat = STAT_LNK;
	pre_m_TOut = 0;

	if(pre_m_fpLog) {
		fclose(pre_m_fpLog);
		pre_m_fpLog = 0;
	}
	pre_m_fpLog = fopen("prestar.log", "w");

	RECT UpdateRect = {0,0,640,DEFWNDHSIZE};
	InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);

	int len = XPrestar_MakeCommand(pre_m_SendBuf, "LNK", 99);
	XPrestar_WriteComm2(pre_m_SendBuf, len);
}

/////////////////////////////////////////////////////////////////////////////
//	アイドル中データ受信の処理

void XPrestar_OnIdle()
{
	if (XPrestar_SumCheck(pre_m_Command, 1) == FALSE)	return;
	if (XPrestar_FunctionCheck(pre_m_Command, 1) == FALSE)	return;
	if (XPrestar_StatusCheck(pre_m_Command, "LNK,", 1) == FALSE)	return;
	if (XPrestar_ParamCheck(pre_m_Command, 1) == FALSE)	return;

	pre_m_Mode = MODE_RCVGRP;
	pre_m_Stat = STAT_LNK;
	pre_m_TOut = 0;
	pre_m_Retry = 0;

	if(pre_m_fpLog) {
		fclose(pre_m_fpLog);
		pre_m_fpLog = 0;
	}
	pre_m_fpLog = fopen("prestar.log", "w");

	pre_m_Shop = (short)XPrestar_SplitShopNum(pre_m_Command);

	RECT UpdateRect = {0,0,640,DEFWNDHSIZE};
	InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);

    int len = XPrestar_MakeCommand(pre_m_SendBuf, "ACK");
	XPrestar_WriteComm2(pre_m_SendBuf, len);
}

/////////////////////////////////////////////////////////////////////////////
//	受信グループの処理
void XPrestar_RcvAction()
{
	int len;
	struct _stat filestat;
	RECT UpdateRect = {0,0,640,DEFWNDHSIZE};

	if (XPrestar_SumCheck(pre_m_Command, 1) == FALSE)	return;
	if (XPrestar_FunctionCheck(pre_m_Command, 1) == FALSE)	return;

	switch (pre_m_Stat) {
	case STAT_LNK:
		if (XPrestar_StatusCheck(pre_m_Command, "REP,G,S", 1) == FALSE)	return;
		if (XPrestar_ParamCheck(pre_m_Command, 1) == FALSE)	return;
		if (XPrestar_FileGroupCheck(pre_m_Command, 1) == FALSE)	return;
		pre_m_Group = (short)XPrestar_SplitGroupNum(pre_m_Command);
		InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);

		if (_strnicmp("REP,G,S,10", &pre_m_Command[1], 10) == 0) {
			len = XPrestar_MakeCommand(pre_m_SendBuf, "ACK");
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = STAT_REP_GS10;
			pre_m_TOut = pre_m_Retry = 0;
			break;
		}
		if (_strnicmp("REP,G,S,20", &pre_m_Command[1], 10) == 0) {
			len = XPrestar_MakeCommand(pre_m_SendBuf, "ACK");
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = STAT_REP_GS20;
			pre_m_TOut = pre_m_Retry = 0;
			break;
		}

		len = XPrestar_MakeCommand(pre_m_SendBuf, "NAK", 5);
		XPrestar_WriteComm2(pre_m_SendBuf, len);
		XPrestar_InitStatus();
		break;
	case STAT_REP_GS10:
		if (XPrestar_StatusCheck(pre_m_Command, "REP,F,S", 1) == FALSE)	return;
		if (XPrestar_ParamCheck(pre_m_Command, 1) == FALSE)	return;
		if (XPrestar_FileGroupCheck(pre_m_Command, 1) == FALSE)	return;
		pre_m_File = (short)XPrestar_SplitFileNum(pre_m_Command);
		InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);

		if (_strnicmp("REP,F,S,13", &pre_m_Command[1], 10) != 0) {
			len = XPrestar_MakeCommand(pre_m_SendBuf, "NAK", 5);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			XPrestar_InitStatus();
		}

		sprintf(pre_m_FileName, "G%02dF%02d.CSV", pre_m_Group, pre_m_File);
		if(_stat(pre_m_FileName, &filestat))
			_creat(pre_m_FileName, (_S_IREAD | _S_IWRITE));
		pre_m_fp = fopen(pre_m_FileName, "w");

		len = XPrestar_MakeCommand(pre_m_SendBuf, "ACK");
		XPrestar_WriteComm2(pre_m_SendBuf, len);
		pre_m_Stat = STAT_REP_FS13;
		pre_m_TOut = pre_m_Retry = 0;
		break;
	case STAT_REP_FS13:
		if (XPrestar_StatusCheck(pre_m_Command, "DAT,", 1, 0) == FALSE)
			if(XPrestar_StatusCheck(pre_m_Command, "REP,F,E", 1) == FALSE)	return;
		if (XPrestar_ParamCheck(pre_m_Command, 1) == FALSE)	return;
		if (XPrestar_FileGroupCheck(pre_m_Command, 1) == FALSE)	return;
		if (_strnicmp("REP,F,E,13", &pre_m_Command[1], 10) == 0) {
			len = XPrestar_MakeCommand(pre_m_SendBuf, "ACK");
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = STAT_REP_FE13;
			pre_m_TOut = pre_m_Retry = 0;
			pre_m_File = -1;
			InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);

			if(pre_m_fp) {fclose(pre_m_fp);	pre_m_fp = 0;}
			break;
		}

		XPrestar_WriteData(pre_m_Command);

		len = XPrestar_MakeCommand(pre_m_SendBuf, "ACK");
		XPrestar_WriteComm2(pre_m_SendBuf, len);
		pre_m_Stat = STAT_REP_FS13;
		pre_m_TOut = pre_m_Retry = 0;
		break;
	case STAT_REP_FE13:
		if (XPrestar_StatusCheck(pre_m_Command, "REP,F,S", 1) == FALSE)	return;
		if (XPrestar_ParamCheck(pre_m_Command, 1) == FALSE)	return;
		if (XPrestar_FileGroupCheck(pre_m_Command, 1) == FALSE)	return;
		pre_m_File = (short)XPrestar_SplitFileNum(pre_m_Command);
		InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);

		if (_strnicmp("REP,F,S,14", &pre_m_Command[1], 10) != 0) {
			len = XPrestar_MakeCommand(pre_m_SendBuf, "NAK", 5);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			XPrestar_InitStatus();
		}

		sprintf(pre_m_FileName, "G%02dF%02d.CSV", pre_m_Group, pre_m_File);
		if(_stat(pre_m_FileName, &filestat))
			_creat(pre_m_FileName, (_S_IREAD | _S_IWRITE));
		pre_m_fp = fopen(pre_m_FileName, "w");

		len = XPrestar_MakeCommand(pre_m_SendBuf, "ACK");
		XPrestar_WriteComm2(pre_m_SendBuf, len);
		pre_m_Stat = STAT_REP_FS14;
		pre_m_TOut = pre_m_Retry = 0;
		break;
	case STAT_REP_FS14:
		if (XPrestar_StatusCheck(pre_m_Command, "DAT,", 1, 0) == FALSE)
			if(XPrestar_StatusCheck(pre_m_Command, "REP,F,E", 1) == FALSE)	return;
		if (XPrestar_ParamCheck(pre_m_Command, 1) == FALSE)	return;
		if (XPrestar_FileGroupCheck(pre_m_Command, 1) == FALSE)	return;
		if (_strnicmp("REP,F,E,14", &pre_m_Command[1], 10) == 0) {
			len = XPrestar_MakeCommand(pre_m_SendBuf, "ACK");
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = STAT_REP_FE14;
			pre_m_TOut = pre_m_Retry = 0;
			pre_m_File = -1;
			InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);

			if(pre_m_fp) {fclose(pre_m_fp);	pre_m_fp = 0;}
			break;
		}

		XPrestar_WriteData(pre_m_Command);

		len = XPrestar_MakeCommand(pre_m_SendBuf, "ACK");
		XPrestar_WriteComm2(pre_m_SendBuf, len);
		pre_m_Stat = STAT_REP_FS14;
		pre_m_TOut = pre_m_Retry = 0;
		break;
	case STAT_REP_FE14:
		if (XPrestar_StatusCheck(pre_m_Command, "REP,F,S", 1) == FALSE)	return;
		if (XPrestar_ParamCheck(pre_m_Command, 1) == FALSE)	return;
		if (XPrestar_FileGroupCheck(pre_m_Command, 1) == FALSE)	return;
		pre_m_File = (short)XPrestar_SplitFileNum(pre_m_Command);
		InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);

		if (_strnicmp("REP,F,S,15", &pre_m_Command[1], 10) != 0) {
			len = XPrestar_MakeCommand(pre_m_SendBuf, "NAK", 5);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			XPrestar_InitStatus();
		}

		sprintf(pre_m_FileName, "G%02dF%02d.CSV", pre_m_Group, pre_m_File);
		if(_stat(pre_m_FileName, &filestat))
			_creat(pre_m_FileName, (_S_IREAD | _S_IWRITE));
		pre_m_fp = fopen(pre_m_FileName, "w");

		len = XPrestar_MakeCommand(pre_m_SendBuf, "ACK");
		XPrestar_WriteComm2(pre_m_SendBuf, len);
		pre_m_Stat = STAT_REP_FS15;
		pre_m_TOut = pre_m_Retry = 0;
		break;
	case STAT_REP_FS15:
		if (XPrestar_StatusCheck(pre_m_Command, "DAT,", 1, 0) == FALSE)
			if(XPrestar_StatusCheck(pre_m_Command, "REP,F,E", 1) == FALSE)	return;
		if (XPrestar_ParamCheck(pre_m_Command, 1) == FALSE)	return;
		if (XPrestar_FileGroupCheck(pre_m_Command, 1) == FALSE)	return;
		if (_strnicmp("REP,F,E,15", &pre_m_Command[1], 10) == 0) {
			len = XPrestar_MakeCommand(pre_m_SendBuf, "ACK");
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = STAT_REP_FE15;
			pre_m_TOut = pre_m_Retry = 0;
			pre_m_File = -1;
			InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);

			if(pre_m_fp) {fclose(pre_m_fp);	pre_m_fp = 0;}
			break;
		}

		XPrestar_WriteData(pre_m_Command);

		len = XPrestar_MakeCommand(pre_m_SendBuf, "ACK");
		XPrestar_WriteComm2(pre_m_SendBuf, len);
		pre_m_Stat = STAT_REP_FS15;
		pre_m_TOut = pre_m_Retry = 0;
		break;
	case STAT_REP_FE15:
		if (XPrestar_StatusCheck(pre_m_Command, "REP,F,S", 1) == FALSE)	return;
		if (XPrestar_ParamCheck(pre_m_Command, 1) == FALSE)	return;
		if (XPrestar_FileGroupCheck(pre_m_Command, 1) == FALSE)	return;
		pre_m_File = (short)XPrestar_SplitFileNum(pre_m_Command);
		InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);

		if (_strnicmp("REP,F,S,19", &pre_m_Command[1], 10) != 0) {
			len = XPrestar_MakeCommand(pre_m_SendBuf, "NAK", 5);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			XPrestar_InitStatus();
		}

		sprintf(pre_m_FileName, "G%02dF%02d.CSV", pre_m_Group, pre_m_File);
		if(_stat(pre_m_FileName, &filestat))
			_creat(pre_m_FileName, (_S_IREAD | _S_IWRITE));
		pre_m_fp = fopen(pre_m_FileName, "w");

		len = XPrestar_MakeCommand(pre_m_SendBuf, "ACK");
		XPrestar_WriteComm2(pre_m_SendBuf, len);
		pre_m_Stat = STAT_REP_FS19;
		pre_m_TOut = pre_m_Retry = 0;
		break;
	case STAT_REP_FS19:
		if (XPrestar_StatusCheck(pre_m_Command, "DAT,", 1, 0) == FALSE)
			if(XPrestar_StatusCheck(pre_m_Command, "REP,F,E", 1) == FALSE)	return;
		if (XPrestar_ParamCheck(pre_m_Command, 1) == FALSE)	return;
		if (XPrestar_FileGroupCheck(pre_m_Command, 1) == FALSE)	return;
		if (_strnicmp("REP,F,E,19", &pre_m_Command[1], 10) == 0) {
			len = XPrestar_MakeCommand(pre_m_SendBuf, "ACK");
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = STAT_REP_FE19;
			pre_m_TOut = pre_m_Retry = 0;
			pre_m_File = -1;
			InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);

			if(pre_m_fp) {fclose(pre_m_fp);	pre_m_fp = 0;}
			break;
		}

		XPrestar_WriteData(pre_m_Command);

		len = XPrestar_MakeCommand(pre_m_SendBuf, "ACK");
		XPrestar_WriteComm2(pre_m_SendBuf, len);
		pre_m_Stat = STAT_REP_FS19;
		pre_m_TOut = pre_m_Retry = 0;
		break;
	case STAT_REP_FE19:
		if (XPrestar_StatusCheck(pre_m_Command, "REP,G,E", 1) == FALSE)	return;
		if (XPrestar_ParamCheck(pre_m_Command, 1) == FALSE)	return;
		if (XPrestar_FileGroupCheck(pre_m_Command, 1) == FALSE)	return;
		if (_strnicmp("REP,G,E,10", &pre_m_Command[1], 10) != 0) {
			len = XPrestar_MakeCommand(pre_m_SendBuf, "NAK", 5);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			XPrestar_InitStatus();
		}

		len = XPrestar_MakeCommand(pre_m_SendBuf, "ACK");
		XPrestar_WriteComm2(pre_m_SendBuf, len);
		pre_m_TOut = pre_m_Retry = pre_m_Shop = 0;
		pre_m_Group = -1;
		InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);

		XPrestar_InitStatus();
		break;



	case STAT_REP_GS20:
		if (XPrestar_StatusCheck(pre_m_Command, "REP,F,S", 1) == FALSE)	return;
		if (XPrestar_ParamCheck(pre_m_Command, 1) == FALSE)	return;
		if (XPrestar_FileGroupCheck(pre_m_Command, 1) == FALSE)	return;
		pre_m_File = (short)XPrestar_SplitFileNum(pre_m_Command);
		InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);

		if (_strnicmp("REP,F,S,18", &pre_m_Command[1], 10) != 0) {
			len = XPrestar_MakeCommand(pre_m_SendBuf, "NAK", 5);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			XPrestar_InitStatus();
		}

		sprintf(pre_m_FileName, "G%02dF%02d.CSV", pre_m_Group, pre_m_File);
		if(_stat(pre_m_FileName, &filestat))
			_creat(pre_m_FileName, (_S_IREAD | _S_IWRITE));
		pre_m_fp = fopen(pre_m_FileName, "w");

		len = XPrestar_MakeCommand(pre_m_SendBuf, "ACK");
		XPrestar_WriteComm2(pre_m_SendBuf, len);
		pre_m_Stat = STAT_REP_FS18;
		pre_m_TOut = pre_m_Retry = 0;
		break;
	case STAT_REP_FS18:
		if (XPrestar_StatusCheck(pre_m_Command, "DAT,", 1, 0) == FALSE)
			if(XPrestar_StatusCheck(pre_m_Command, "REP,F,E", 1) == FALSE)	return;
		if (XPrestar_ParamCheck(pre_m_Command, 1) == FALSE)	return;
		if (XPrestar_FileGroupCheck(pre_m_Command, 1) == FALSE)	return;
		if (_strnicmp("REP,F,E,18", &pre_m_Command[1], 10) == 0) {
			len = XPrestar_MakeCommand(pre_m_SendBuf, "ACK");
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			pre_m_Stat = STAT_REP_FE18;
			pre_m_TOut = pre_m_Retry = 0;
			pre_m_File = -1;
			InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);

			if(pre_m_fp) {fclose(pre_m_fp);	pre_m_fp = 0;}
			break;
		}

		XPrestar_WriteData(pre_m_Command);

		len = XPrestar_MakeCommand(pre_m_SendBuf, "ACK");
		XPrestar_WriteComm2(pre_m_SendBuf, len);
		pre_m_Stat = STAT_REP_FS18;
		pre_m_TOut = pre_m_Retry = 0;
		break;
	case STAT_REP_FE18:
		if (XPrestar_StatusCheck(pre_m_Command, "REP,G,E", 1) == FALSE)	return;
		if (XPrestar_ParamCheck(pre_m_Command, 1) == FALSE)	return;
		if (XPrestar_FileGroupCheck(pre_m_Command, 1) == FALSE)	return;
		if (_strnicmp("REP,G,E,20", &pre_m_Command[1], 10) != 0) {
			len = XPrestar_MakeCommand(pre_m_SendBuf, "NAK", 5);
			XPrestar_WriteComm2(pre_m_SendBuf, len);
			XPrestar_InitStatus();
		}

		len = XPrestar_MakeCommand(pre_m_SendBuf, "ACK");
		XPrestar_WriteComm2(pre_m_SendBuf, len);
		pre_m_TOut = pre_m_Retry = pre_m_Shop = 0;
		pre_m_Group = -1;
		InvalidateRect(tty_m_hwnd, &UpdateRect, FALSE);

		XPrestar_InitStatus();
		break;
	}
}

void XPrestar_WriteLog(const char * buf, int n) {
	if(pre_m_Loging == TRUE) {
		fputs(buf, pre_m_fpLog);
		fputc('\n', pre_m_fpLog);
	}
	XTTY_WriteLog(buf, n);
}

int XPrestar_WriteComm2(const char * buf, int n) {
	if(pre_m_Loging == TRUE) {
		fputs(buf, pre_m_fpLog);
		fputc('\n', pre_m_fpLog);
	}
	return XTTY_WriteComm2(buf, n);
}

void XPrestar_Log() {
	pre_m_Loging = (pre_m_Loging == TRUE) ?FALSE :TRUE;
}

void XPrestar_OnPaint()
{
	PAINTSTRUCT ps;
	char str[256];

	BeginPaint(tty_m_hwnd, &ps);

	strcpy(str, "現在状態：");
	if(pre_m_Mode == MODE_IDLE && pre_m_Stat == STAT_IDLE)
		strcat(str, "待機中");
	else
		strcat(str, "通信中");
	TextOut(ps.hdc, 12, 12, str, strlen(str));

	if(pre_m_Shop)
		sprintf(str, "店ＩＤ：%02d ", pre_m_Shop);
	else
		sprintf(str, "店ＩＤ：--   ", pre_m_Shop);
	TextOut(ps.hdc, 182, 12, str, strlen(str));

	XPrestar_GetGroupName(str);
	TextOut(ps.hdc, 322, 12, str, strlen(str));

	XPrestar_GetFileName(str);
	TextOut(ps.hdc, 222, DEFWNDHSIZE - 118, str, strlen(str));

	XTTY_OnPaint(&ps);
//	EndPaint(tty_m_hwnd, &ps);


//	HDC	hDC = GetDC(tty_m_hwnd);
//	HDC hMemDC = CreateCompatibleDC(hDC);
	HDC hMemDC = CreateCompatibleDC(ps.hdc);
	BITMAP bitmap;
	HBITMAP hbmOld;

	HBITMAP trance[5] = { pre_m_hbmtrance1r, pre_m_hbmtrance2r, pre_m_hbmtrance3r, pre_m_hbmtrance4r, pre_m_hbmtrance5r };
	HBITMAP trance1[5] = { pre_m_hbmtrance1l, pre_m_hbmtrance2l, pre_m_hbmtrance3l, pre_m_hbmtrance4l, pre_m_hbmtrance5l };

	hbmOld = SelectObject(hMemDC, pre_m_hbmparent);
	GetObject(pre_m_hbmparent, sizeof(bitmap), (LPSTR)&bitmap);
//	BitBlt(hDC, 130, 64, bitmap.bmWidth, bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);
	BitBlt(ps.hdc, 130, 64, bitmap.bmWidth, bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);
	SelectObject(hMemDC, hbmOld);


	hbmOld = SelectObject(hMemDC, pre_m_hbmchild);
	GetObject(pre_m_hbmchild, sizeof(bitmap), (LPSTR)&bitmap);
//	BitBlt(hDC, 430, 72, bitmap.bmWidth, bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);
	BitBlt(ps.hdc, 430, 72, bitmap.bmWidth, bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);
	SelectObject(hMemDC, hbmOld);


	if(pre_m_Mode != MODE_IDLE && pre_m_Stat != STAT_IDLE) {
		if (trance[pre_m_Anime-1] && trance1[pre_m_Anime-1]) {
			hbmOld = SelectObject(hMemDC, (pre_m_Mode != MODE_RCVGRP) ?trance[pre_m_Anime - 1] :trance1[pre_m_Anime - 1]);
			GetObject((pre_m_Mode != MODE_RCVGRP) ?trance[pre_m_Anime - 1] :trance1[pre_m_Anime - 1], sizeof(bitmap), (LPSTR)&bitmap);
//			BitBlt(hDC, 216, 64, bitmap.bmWidth, bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);
			BitBlt(ps.hdc, 216, 64, bitmap.bmWidth, bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);
			SelectObject(hMemDC, hbmOld);
		}
	} else {
		RECT fill = {216,64,424,128};
//		FillRect(hDC, &fill, GetStockObject(WHITE_BRUSH));
		FillRect(ps.hdc, &fill, GetStockObject(WHITE_BRUSH));
	}

	if(hbmOld) DeleteObject(hbmOld);
	if(hMemDC) DeleteDC(hMemDC);
//	if(hDC) ReleaseDC(tty_m_hwnd, hDC);
	EndPaint(tty_m_hwnd, &ps);
}

void XPrestar_GetGroupName(char *str)
{
	strcpy(str, "グループ：");
	switch(pre_m_Group) {
	case 0:
		strcat(str, "店舗設定　　　　　　");break;
	case 1:
		strcat(str, "商品マスター　　　　");break;
	case 2:
		strcat(str, "発注マスター　　　　");break;
	case 10:
		strcat(str, "履歴　　　　　　　　");break;
	case 20:
		strcat(str, "発注トランザクション");break;
	default:
		strcat(str, "－－－－－－－－－－");break;
	}
}

void XPrestar_GetFileName(char *str)
{
	strcpy(str, "ファイル：");
	switch(pre_m_File) {
	case 11:
		strcat(str, "店舗マスター　　　　");break;
	case 00:
		strcat(str, "品目マスター　　　　");break;
	case 01:
		strcat(str, "分類マスター　　　　");break;
	case 04:
		strcat(str, "税率マスター　　　　");break;
	case 16:
		strcat(str, "材料マスター　　　　");break;
	case 17:
		strcat(str, "材料分類マスター　　");break;
	case 13:
		strcat(str, "オーダー履歴　　　　");break;
	case 14:
		strcat(str, "売上げ履歴　　　　　");break;
	case 15:
		strcat(str, "入金履歴　　　　　　");break;
	case 19:
		strcat(str, "日計　　　　　　　　");break;
	case 18:
		strcat(str, "発注データ　　　　　");break;
	default:
		strcat(str, "－－－－－－－－－－");break;
	}
}


HBITMAP XPrestar_LoadBitmap(char *name)
{
	HRSRC hrsrc;
	LPSTR lpbitmap;
	HGLOBAL hglb;
	HPALETTE hpal;
	HBITMAP hbitmap;

	hrsrc = FindResource(tty_m_hinst, (LPCSTR)name, (LPCSTR)RT_BITMAP);
	hglb = LoadResource(tty_m_hinst, hrsrc);
	lpbitmap = LockResource(hglb);
	if(lpbitmap) UnlockResource(hglb);

	hpal = CreateDIBPalette(hglb);
	hbitmap = DIBToBitmap(hglb, hpal);

	if(hpal) DeleteObject(hpal);
	if(hglb) FreeResource(hglb);
//	if(hrsrc) FreeResource(hrsrc);	delete kojima

	return hbitmap;
}


