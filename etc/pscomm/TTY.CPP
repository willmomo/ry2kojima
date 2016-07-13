#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "main.h"
#include "tty.h"
#include "ext.h"

HINSTANCE tty_m_hinst;
HWND tty_m_hwnd;
BOOL tty_m_fConnect;			// 接続中フラグ TRUE:オンライン FALSE:オフライン
int tty_m_idComDev;				// COMデバイスの識別子
static int tty_m_xPos, tty_m_yPos;			// カーソルのX,Y座標
static BYTE tty_m_abScreen[80 * 25];		// 受信データ保存バッファ
static LOGFONT tty_m_lf;				// TTYが使用するフォント情報
static HFONT tty_m_hfont;				// TTYが使用するフォント
BOOL tty_m_fLocalEcho;			// ローカル エコーの有無 TRUE:有り FALSE:無し
static int tty_m_fontHeight;			// フォントの高さ
static DCB tty_m_dcb;
static char tty_m_Comm[16];

////////////////////////////////////////////////////////////////////////////
//	コンストラクタ

void XTTY_XTTY(HINSTANCE hinst, HWND hwnd)
{
	tty_m_hinst = hinst;
	tty_m_hwnd = hwnd;
	tty_m_fConnect = FALSE;
	tty_m_idComDev = 0;
	tty_m_xPos = tty_m_yPos = 0;
	tty_m_fLocalEcho = FALSE;
	tty_m_fontHeight = 18;
	memset(tty_m_abScreen, ' ', sizeof(tty_m_abScreen));

	tty_m_lf.lfHeight = 12;
	tty_m_lf.lfWidth = 0;
	tty_m_lf.lfEscapement = 0;
	tty_m_lf.lfOrientation = 0;
	tty_m_lf.lfWeight = FW_DONTCARE;
	tty_m_lf.lfItalic = 0;
	tty_m_lf.lfUnderline = 0;
	tty_m_lf.lfStrikeOut = 0;
	tty_m_lf.lfCharSet = SHIFTJIS_CHARSET;
	tty_m_lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	tty_m_lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	tty_m_lf.lfQuality = DEFAULT_QUALITY;
	tty_m_lf.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
	strcpy(tty_m_lf.lfFaceName, "ＭＳ ゴシック");

	tty_m_hfont = CreateFontIndirect(&tty_m_lf);

	BuildCommDCB("COM2:9600,n,8,1", &tty_m_dcb);
	sprintf(tty_m_Comm, "COM2");
}

////////////////////////////////////////////////////////////////////////////
//	デストラクタ

void XTTY_deXTTY()
{
	XTTY_Connect(FALSE);		// オフラインにする
	DeleteObject(tty_m_hfont);
}

////////////////////////////////////////////////////////////////////////////
//	オンラインまたは、オフラインにする

int XTTY_Connect(BOOL fConnect)
{
	if (fConnect) {		// オンラインにする
		if (tty_m_fConnect)		// 既にオンライン？
			return 0;	// 何もせずに終わる

		tty_m_idComDev = OpenComm(tty_m_Comm, 1024, 128);
		if (tty_m_idComDev < 0) {	// オープンに失敗？
			XTTY_OpenComm_Error(tty_m_idComDev);
			return -1;
		}

		if(SetCommState(&tty_m_dcb) < 0) {
			CloseComm(tty_m_idComDev);
			MessageBox(tty_m_hwnd, "現在の設定は有効な設定ではありません。\n再度設定をしてください。", "SetComm ｴﾗｰ", MB_OK);
			return -1;
		}

		GetCommState(tty_m_idComDev, &tty_m_dcb);
		SetCommEventMask(tty_m_idComDev, EV_ERR|EV_CTS);
		EnableCommNotification(tty_m_idComDev, tty_m_hwnd, 80, -1);
		EscapeCommFunction(tty_m_idComDev, SETDTR);
		GetCommState(tty_m_idComDev, &tty_m_dcb);
		tty_m_fConnect = TRUE;
	} else {		// オフラインにする
		if (!tty_m_fConnect)	// 既にオフライン？
			return 0;	// 何もせずに終わる

		EscapeCommFunction(tty_m_idComDev, CLRDTR);
		CloseComm(tty_m_idComDev);
		tty_m_fConnect = FALSE;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	スクリーンバッファのスクロール

void XTTY_ScrollScreen()
{
	memmove(&tty_m_abScreen[0*80+0], &tty_m_abScreen[1*80+0], sizeof(BYTE) * (80 * 24));
	memset(&tty_m_abScreen[24*80+0], ' ', 80);
}

////////////////////////////////////////////////////////////////////////////
//	WM_PAINTメッセージ処理

void XTTY_OnPaint(PAINTSTRUCT *ps)
{
	RECT wndrect;

	GetWindowRect(tty_m_hwnd, &wndrect);
	if((wndrect.bottom - wndrect.top) == DEFWNDHSIZE)
		return;

	HFONT hfontOld = SelectObject(ps->hdc, tty_m_hfont);
	DWORD dwSize = GetTextExtent(ps->hdc, "□", 2);
	WORD cy = HIWORD(dwSize);
	for (int y = 0; y < 25; y++) {
		for (int x = 0; x < 80; x++)
			if (tty_m_abScreen[80*y+x] != 0 && iscntrl(tty_m_abScreen[80*y+x]))
				tty_m_abScreen[80*y+x] = '.';

		TextOut(ps->hdc, 0, y * cy + DEFWNDHSIZE - 30, (LPCSTR)&tty_m_abScreen[80 * y], 80);
	}
	SelectObject(ps->hdc, hfontOld);
}

////////////////////////////////////////////////////////////////////////////
//	WM_CHARメッセージ処理

void XTTY_OnChar(WPARAM wParam, LPARAM lParam)
{
	if (!tty_m_fConnect)
		return;

	BYTE bCode = (BYTE)wParam;
	int rtn = XTTY_WriteComm2((const char *)&bCode, 1);
	if (rtn < 0) {
		XTTY_CommErrorMessage();
	}
}

////////////////////////////////////////////////////////////////////////////
//	WM_COMMNOTIFYメッセージ処理

void XTTY_OnCommNotify(WPARAM wParam, LPARAM lParam)
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

	if (lParam & CN_RECEIVE) {	// 受信イベント発生
		rtn = ReadComm(tty_m_idComDev, buf, sizeof(buf));

		if (rtn <= 0) {
			XTTY_CommErrorMessage();
			return;
		}

		XTTY_WriteLog(buf, rtn);
	}
}

////////////////////////////////////////////////////////////////////////////
//	文字列をLogに書き込む

void XTTY_WriteLog(const char * buf, int n)
{
	RECT rc;
	BOOL fScroll;

	rc.left = 0;
	rc.top = tty_m_yPos * tty_m_fontHeight;

	for (int i = 0; i < n; i++) {
		if (buf[i] == '\x0d') {
			tty_m_xPos = 0;
		} else if (buf[i] == '\x0a') {
			if (++tty_m_yPos >= 25) {
				tty_m_yPos = 24;
				XTTY_ScrollScreen();
				fScroll = TRUE;
			}
		} else if (buf[i] == '\x03') {
			tty_m_abScreen[tty_m_yPos * 80 + tty_m_xPos] = buf[i];
			tty_m_xPos = 0;
			if (++tty_m_yPos >= 25) {
				tty_m_yPos = 24;
				XTTY_ScrollScreen();
				fScroll = TRUE;
			}
		} else {
			tty_m_abScreen[tty_m_yPos * 80 + tty_m_xPos] = buf[i];
			if (++tty_m_xPos >= 80) {
				tty_m_xPos = 0;
				if (++tty_m_yPos >= 25) {
					tty_m_yPos = 24;
					XTTY_ScrollScreen();
					fScroll = TRUE;
				}
			}
		}
	}

	rc.right = 80 * 10 + 10;
	rc.bottom = tty_m_yPos * tty_m_fontHeight + tty_m_fontHeight;

	if (fScroll) {
		rc.left = 0;
		rc.right = 640;
		rc.top = DEFWNDHSIZE - 30;
		rc.bottom = DEFWNDHSIZE + LOGWNDHSIZE;
		InvalidateRect(tty_m_hwnd, &rc, FALSE);
	} else
		InvalidateRect(tty_m_hwnd, &rc, FALSE);

	UpdateWindow(tty_m_hwnd);
}

////////////////////////////////////////////////////////////////////////////
//

int XTTY_WriteComm2(const char * buf, int n)
{
	if (tty_m_fLocalEcho)
		XTTY_WriteLog(buf, n);

	return WriteComm(tty_m_idComDev, buf, n);
}

////////////////////////////////////////////////////////////////////////////
//	OpenCommのエラーメッセージ

void XTTY_OpenComm_Error(int errCode)
{
	char * msg;

	switch (errCode) {
	case IE_BADID:
		msg = "デバイス識別子が無効か、サポートされていません。";
		break;
	case IE_BAUDRATE:
		msg = "デバイスのボーレートはサポートされていません。";
		break;
	case IE_BYTESIZE:
		msg = "指定されたバイト サイズが無効です。";
		break;
	case IE_DEFAULT:
		msg = "デフォルトのパラメータが間違っています。";
		break;
	case IE_HARDWARE:
		msg = "ハードウェアが利用可能ではありません (ほかのデバイスがロックしています)。";
		break;
	case IE_MEMORY:
		msg = "関数がキューを割り当てることができません。";
		break;
	case IE_NOPEN:
		msg = "デバイスがオープンされていません。";
		break;
	case IE_OPEN:
		msg = "デバイスはすでにオープンされています。";
		break;
	default:
		msg = "未知のエラーです。";
		break;
	}

	MessageBox(tty_m_hwnd, msg, "OpenComm エラー", MB_OK | MB_APPLMODAL);
}

////////////////////////////////////////////////////////////////////////////
//

void XTTY_CommErrorMessage()
{
	COMSTAT stat;
	int ce = GetCommError(tty_m_idComDev, &stat);
	if (ce & CE_BREAK)
		MessageBox(tty_m_hwnd, "ハードウェアがブレーク状態を検出したことを示します。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_CTSTO)
		MessageBox(tty_m_hwnd, "CTS (clear-to-send) がタイムアウトです。文字を送信している間、COMSTAT構造体のfCtsHoldメンバにより指定された期間だけ、CTSがロー レベルです。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_DNS)
		MessageBox(tty_m_hwnd, "パラレル デバイスが選択されなかったことを示します。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_DSRTO)
		MessageBox(tty_m_hwnd, "DSR (data-set-ready) がタイムアウトです。文字を送信している間、COMSTAT構造体のfDsrHoldメンバにより指定された期間だけ、DSRがロー レベルです。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_FRAME)
		MessageBox(tty_m_hwnd, "ハードウェアがフレーミング エラーを検出したことを示します。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_IOE)
		MessageBox(tty_m_hwnd, "パラレル デバイスとの通信で、I/Oエラーが発生したことを示します。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_MODE)
		MessageBox(tty_m_hwnd, "要求されたモードがサポートされていないか、idComDevパラメータが無効であることを示します。CE_MODEがセットされると、このフラグだけが有効なエラー値になります。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_OOP)
		MessageBox(tty_m_hwnd, "パラレル デバイスの用紙切れを示す信号がオンになったことを示します。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_OVERRUN)
		MessageBox(tty_m_hwnd, "次の文字を受信する前に、ハードウェアから文字が読み取られなかったことを示します。この文字は失われます。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_PTO)
		MessageBox(tty_m_hwnd, "パラレル デバイスとの通信で、タイムアウトが発生したことを示します。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_RLSDTO)
		MessageBox(tty_m_hwnd, "RLSD (receive-line-signal-detect) がタイムアウトです。文字を送信している間、COMSTAT構造体のfRlsdHoldメンバにより指定された期間だけ、RLSDがロー レベルです。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_RXOVER)
		MessageBox(tty_m_hwnd, "受信キューのオーバフローが発生しました。入力キューに空きがないか、エンド オブ ファイル文字を受信した後も文字を受信したことを示します。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_RXPARITY)
		MessageBox(tty_m_hwnd, "ハードウェアがパリティ エラーを検出したことを示します。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_TXFULL)
		MessageBox(tty_m_hwnd, "関数が文字をキューに入れようとしたときに、送信キューに空きがなかったことを示します。", NULL, MB_OK | MB_APPLMODAL);
}

BOOL XTTY_ReadDCB(DCB *pdcb)
{
	memcpy(pdcb, &tty_m_dcb, sizeof(DCB));

	return TRUE;
}

BOOL XTTY_ReadCom(char *pcom)
{
	strcpy(pcom, tty_m_Comm);

	return TRUE;
}

BOOL XTTY_WriteDCB(DCB *pdcb)
{
	memcpy(&tty_m_dcb, pdcb, sizeof(DCB));

	return TRUE;
}

BOOL XTTY_WriteCom(char *pcom)
{
	strcpy(tty_m_Comm, pcom);

	return TRUE;
}
