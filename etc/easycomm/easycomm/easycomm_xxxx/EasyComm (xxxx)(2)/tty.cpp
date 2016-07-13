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
	m_fLocalEcho = FALSE;
	m_fContinue = FALSE;
	FillMemory(m_abScreen, sizeof(m_abScreen), ' ');
	ZeroMemory(&m_os, sizeof(m_os));

	ZeroMemory(&m_lf, sizeof(m_lf));
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
	lstrcpy(m_lf.lfFaceName, "ＭＳ ゴシック");

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
	BOOL fRtn;

	if (fConnect) {		// オンラインにする
		if (m_fConnect)		// 既にオンライン？
			return 0;	// 何もせずに終わる

		m_idComDev = CreateFile("COM2", GENERIC_READ|GENERIC_WRITE,
			0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, 0);
		if (m_idComDev == INVALID_HANDLE_VALUE) {	// オープンに失敗？
			OpenComm_Error();
			return -1;
		}

		m_os.hEvent = CreateEvent( NULL,    // no security
			TRUE,    // explicit reset req
			FALSE,   // initial event reset
			NULL ) ; // no name

		DCB dcb;
		GetCommState(m_idComDev, &dcb);
		
//		dcb.BaudRate = 57600;
		dcb.BaudRate = 9600;
		dcb.ByteSize = 8;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;

		SetCommState(m_idComDev, &dcb);
		GetCommState(m_idComDev, &dcb);
		DWORD dwEvMask = EV_RXCHAR;
		SetCommMask(m_idComDev, dwEvMask);
		SetupComm( m_idComDev, 4096, 4096 ) ;
		PurgeComm( m_idComDev, PURGE_TXABORT | PURGE_RXABORT |PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
		EscapeCommFunction(m_idComDev, SETDTR);
		m_fConnect = TRUE;

		COMMTIMEOUTS cto;
		cto.ReadIntervalTimeout = 0xFFFFFFFF ;
		cto.ReadTotalTimeoutMultiplier = 0 ;
		cto.ReadTotalTimeoutConstant = 1000 ;
		cto.WriteTotalTimeoutMultiplier = 0 ;
		cto.WriteTotalTimeoutConstant = 1000 ;
		SetCommTimeouts(m_idComDev, &cto);

		m_fContinue = TRUE;
		m_arg[0] = (DWORD)m_hwnd;
		m_arg[1] = (DWORD)m_idComDev;
		m_arg[2] = (DWORD)&m_fContinue;
		m_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CommEvent, m_arg, 0, &m_threadID);
	} else {		// オフラインにする
		if (!m_fConnect)	// 既にオフライン？
			return 0;	// 何もせずに終わる

		m_fContinue = FALSE;
		SetCommMask( m_idComDev, 0 ) ;
		while (m_fContinue == FALSE);
		m_fContinue = FALSE;

		EscapeCommFunction(m_idComDev, CLRDTR);
		PurgeComm( m_idComDev, PURGE_TXABORT | PURGE_RXABORT |PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
		CloseHandle( m_os.hEvent ) ;
		ZeroMemory(&m_os, sizeof(m_os));
		fRtn = CloseHandle(m_idComDev);
		m_fConnect = FALSE;

		fRtn = CloseHandle(m_thread);
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	スクリーンバッファのスクロール

void XTTY::ScrollScreen()
{
	MoveMemory(&m_abScreen[0*80+0], &m_abScreen[1*80+0], sizeof(BYTE) * (80 * 24));
	FillMemory(&m_abScreen[24*80+0], 80, ' ');
}

////////////////////////////////////////////////////////////////////////////
//	WM_PAINTメッセージ処理

void XTTY::OnPaint()
{
	PAINTSTRUCT ps;
	SIZE size;

	BeginPaint(m_hwnd, &ps);

	HFONT hfontOld = SelectObject(ps.hdc, m_hfont);
	GetTextExtentPoint32(ps.hdc, "□", 2, &size);
	for (int y = 0; y < 25; y++) {
		TextOut(ps.hdc, 0, y * size.cy, (LPCSTR)&m_abScreen[80 * y], 80);
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
	BOOL fScroll = FALSE;
	DWORD dwNumberOfBytesRead;
	COMSTAT cs;
	DWORD dwLength, dwErrorFlags;

	if (!m_fConnect)
		return;

	if (lParam & CN_EVENT) {	// 通信デバイスイベント発生
	}

	if (lParam & CN_RECEIVE) {	// 受信イベント発生
		ClearCommError( m_idComDev, &dwErrorFlags, &cs);
		dwLength = min( (DWORD) sizeof(buf), cs.cbInQue ) ;
		ReadFile(m_idComDev, buf, dwLength, &dwNumberOfBytesRead, NULL);
		WriteLog(buf, dwNumberOfBytesRead);
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
	DWORD dwNumberOfBytesWritten;
	BOOL fRtn;

	if (m_fLocalEcho)
		WriteLog(buf, n);

	fRtn = WriteFile(m_idComDev, buf, n, &dwNumberOfBytesWritten, NULL);

	return (int)dwNumberOfBytesWritten;
}

////////////////////////////////////////////////////////////////////////////
//	OpenCommのエラーメッセージ

void XTTY::OpenComm_Error()
{
	char strBuff[64];
	DWORD dwErrorCode = GetLastError();

	switch (dwErrorCode) {
	case ERROR_ALREADY_EXISTS:
		wsprintf(strBuff, "ｴﾗｰｺｰﾄﾞ: %d(ERROR_ALREADY_EXISTS)", dwErrorCode);
		break;
	case ERROR_ACCESS_DENIED:
		wsprintf(strBuff, "ｴﾗｰｺｰﾄﾞ: %d(ERROR_ACCESS_DENIED)", dwErrorCode);
		break;
	default:
		wsprintf(strBuff, "ｴﾗｰｺｰﾄﾞ: %d", dwErrorCode);
		break;
	}

	MessageBox(m_hwnd, strBuff, "CreateFile error", MB_OK | MB_APPLMODAL);
}

////////////////////////////////////////////////////////////////////////////
//

void XTTY::CommErrorMessage()
{
	DCB dcb;
	char strBuff[64];
	if (GetCommState(m_idComDev, &dcb) == FALSE) {
		DWORD dwErrorCode = GetLastError();
		wsprintf(strBuff, "ｴﾗｰｺｰﾄﾞ: %d", dwErrorCode);
		MessageBox(m_hwnd, strBuff, NULL, MB_OK|MB_APPLMODAL);
	}
}

////////////////////////////////////////////////////////////////////////////
// friend - function

DWORD WINAPI CommEvent(LPDWORD arg)
{
	HWND hwnd = (HWND)arg[0];
	HANDLE idComDev = (HANDLE)arg[1];
	BOOL* pfContinue = (BOOL*)arg[2];

	while (*pfContinue) {
		DWORD dwEvMask = 0;
		WaitCommEvent(idComDev, &dwEvMask, NULL);

		if ((dwEvMask & EV_RXCHAR) == EV_RXCHAR){
			COMMPROP cp;
			GetCommProperties(idComDev, &cp);
			SendMessage(hwnd, WM_COMMNOTIFY, 0, CN_RECEIVE);
		}
	}
	*pfContinue = TRUE;

	return 0;
}
