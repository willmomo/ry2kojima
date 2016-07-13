#include "tty.h"

////////////////////////////////////////////////////////////////////////////
//	コンストラクタ

XTTY::XTTY(HINSTANCE hinst, HWND hwnd)
{
	m_hinst = hinst;
	m_hwnd = hwnd;
	m_fConnect = FALSE;
	m_idComDev = 0;
	m_xPos = m_yPos = 0;
	m_fLocalEcho = TRUE;
	memset(m_abScreen, ' ', sizeof(m_abScreen));

	m_lf.lfHeight = 20;
	m_lf.lfWidth = 10;
	m_lf.lfEscapement = 0;
	m_lf.lfOrientation = 0;
	m_lf.lfWeight = FW_DONTCARE;
	m_lf.lfItalic = 0;
	m_lf.lfUnderline = 0;
	m_lf.lfStrikeOut = 0;
	m_lf.lfCharSet = SHIFTJIS_CHARSET;
	m_lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_lf.lfQuality = DEFAULT_QUALITY;
	m_lf.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
	m_lf.lfFaceName[0] = NULL;

	m_hfont = CreateFontIndirect(&m_lf);
}

////////////////////////////////////////////////////////////////////////////
//	デストラクタ

XTTY::~XTTY()
{
	Connect(FALSE);		// オフラインにする
	DeleteObject(m_hfont);
}

////////////////////////////////////////////////////////////////////////////
//	オンラインまたは、オフラインにする

int XTTY::Connect(BOOL fConnect)
{
	if (fConnect) {		// オンラインにする
		if (m_fConnect)		// 既にオンライン？
			return 0;	// 何もせずに終わる

		m_idComDev = OpenComm("COM2", 1024, 128);
		if (m_idComDev < 0) {	// オープンに失敗？
			OpenComm_Error(m_idComDev);
			return -1;
		}

		DCB dcb;
		GetCommState(m_idComDev, &dcb);
		
		dcb.BaudRate = 57600;
		dcb.ByteSize = 8;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;

		SetCommState(&dcb);
		GetCommState(m_idComDev, &dcb);
		EnableCommNotification(m_idComDev, m_hwnd, 80, -1);
		EscapeCommFunction(m_idComDev, SETDTR);
		m_fConnect = TRUE;
	} else {		// オフラインにする
		if (!m_fConnect)	// 既にオフライン？
			return 0;	// 何もせずに終わる

		EscapeCommFunction(m_idComDev, CLRDTR);
		CloseComm(m_idComDev);
		m_fConnect = FALSE;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	スクリーンバッファのスクロール

void XTTY::ScrollScreen()
{
	memmove(&m_abScreen[0*80+0], &m_abScreen[1*80+0], sizeof(BYTE) * (80 * 24));
	memset(&m_abScreen[24*80+0], ' ', 80);
}

////////////////////////////////////////////////////////////////////////////
//	WM_PAINTメッセージ処理

void XTTY::OnPaint()
{
	PAINTSTRUCT ps;

	BeginPaint(m_hwnd, &ps);

	HFONT hfontOld = SelectObject(ps.hdc, m_hfont);
	for (int y = 0; y < 25; y++) {
		TextOut(ps.hdc, 0, y * 20, (LPCSTR)&m_abScreen[80 * y], 80);
	}
	SelectObject(ps.hdc, hfontOld);

	EndPaint(m_hwnd, &ps);
}

////////////////////////////////////////////////////////////////////////////
//	WM_CHARメッセージ処理

void XTTY::OnChar(WPARAM wParam, LPARAM lParam)
{
	if (!m_fConnect)
		return;

	BYTE bCode = (BYTE)wParam;
	WriteComm2((const char *)&bCode, 1);
}

////////////////////////////////////////////////////////////////////////////
//	WM_COMMNOTIFYメッセージ処理

void XTTY::OnCommNotify(WPARAM wParam, LPARAM lParam)
{
	char buf[128];
	int rtn;
	RECT rc;
	BOOL fScroll = FALSE;

	if (!m_fConnect)
		return;

	if (lParam & CN_EVENT) {	// 通信デバイスイベント発生
	}

	if (lParam & CN_RECEIVE) {	// 受信イベント発生
		rtn = ReadComm(m_idComDev, buf, sizeof(buf));

		if (rtn <= 0) {
			CommErrorMessage();
			return;
		}

		WriteLog(buf, rtn);
	}
}

////////////////////////////////////////////////////////////////////////////
//	文字列をLogに書き込む

void XTTY::WriteLog(const char * buf, int n)
{
	RECT rc;
	BOOL fScroll;

	rc.left = 0;
	rc.top = m_yPos * 20;
	
	for (int i = 0; i < n; i++) {
		if (buf[i] == '\x0d') {
			m_xPos = 0;
		} else if (buf[i] == '\x0a') {
			if (++m_yPos >= 25) {
				m_yPos = 24;
				ScrollScreen();
				fScroll = TRUE;
			}
		} else {
			m_abScreen[m_yPos * 80 + m_xPos] = buf[i];
			if (++m_xPos >= 80) {
				m_xPos = 0;
				if (++m_yPos >= 25) {
					m_yPos = 24;
					ScrollScreen();
					fScroll = TRUE;
				}
			}
		}
	}

	rc.right = 80 * 10 + 10;
	rc.bottom = m_yPos * 20 + 20;

	if (fScroll)
		InvalidateRect(m_hwnd, NULL, FALSE);
	else
		InvalidateRect(m_hwnd, &rc, FALSE);

	UpdateWindow(m_hwnd);
}

////////////////////////////////////////////////////////////////////////////
//

int XTTY::WriteComm2(const char * buf, int n)
{
	if (m_fLocalEcho)
		WriteLog(buf, n);

	return WriteComm(m_idComDev, buf, n);
}

////////////////////////////////////////////////////////////////////////////
//	OpenCommのエラーメッセージ

void XTTY::OpenComm_Error(int errCode)
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

	MessageBox(m_hwnd, msg, "OpenComm エラー", MB_OK | MB_APPLMODAL);
}

////////////////////////////////////////////////////////////////////////////
//

void XTTY::CommErrorMessage()
{
	COMSTAT stat;
	int ce = GetCommError(m_idComDev, &stat);
	if (ce & CE_BREAK)
		MessageBox(m_hwnd, "ハードウェアがブレーク状態を検出したことを示します。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_CTSTO)
		MessageBox(m_hwnd, "CTS (clear-to-send) がタイムアウトです。文字を送信している間、COMSTAT構造体のfCtsHoldメンバにより指定された期間だけ、CTSがロー レベルです。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_DNS)
		MessageBox(m_hwnd, "パラレル デバイスが選択されなかったことを示します。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_DSRTO)
		MessageBox(m_hwnd, "DSR (data-set-ready) がタイムアウトです。文字を送信している間、COMSTAT構造体のfDsrHoldメンバにより指定された期間だけ、DSRがロー レベルです。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_FRAME)
		MessageBox(m_hwnd, "ハードウェアがフレーミング エラーを検出したことを示します。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_IOE)
		MessageBox(m_hwnd, "パラレル デバイスとの通信で、I/Oエラーが発生したことを示します。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_MODE)
		MessageBox(m_hwnd, "要求されたモードがサポートされていないか、idComDevパラメータが無効であることを示します。CE_MODEがセットされると、このフラグだけが有効なエラー値になります。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_OOP)
		MessageBox(m_hwnd, "パラレル デバイスの用紙切れを示す信号がオンになったことを示します。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_OVERRUN)
		MessageBox(m_hwnd, "次の文字を受信する前に、ハードウェアから文字が読み取られなかったことを示します。この文字は失われます。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_PTO)
		MessageBox(m_hwnd, "パラレル デバイスとの通信で、タイムアウトが発生したことを示します。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_RLSDTO)
		MessageBox(m_hwnd, "RLSD (receive-line-signal-detect) がタイムアウトです。文字を送信している間、COMSTAT構造体のfRlsdHoldメンバにより指定された期間だけ、RLSDがロー レベルです。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_RXOVER)
		MessageBox(m_hwnd, "受信キューのオーバフローが発生しました。入力キューに空きがないか、エンド オブ ファイル文字を受信した後も文字を受信したことを示します。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_RXPARITY)
		MessageBox(m_hwnd, "ハードウェアがパリティ エラーを検出したことを示します。", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_TXFULL)
		MessageBox(m_hwnd, "関数が文字をキューに入れようとしたときに、送信キューに空きがなかったことを示します。", NULL, MB_OK | MB_APPLMODAL);
}
