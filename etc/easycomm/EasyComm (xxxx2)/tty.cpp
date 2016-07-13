#include "tty.h"

////////////////////////////////////////////////////////////////////////////
//	�R���X�g���N�^

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
//	�f�X�g���N�^

XTTY::~XTTY()
{
	Connect(FALSE);		// �I�t���C���ɂ���
	DeleteObject(m_hfont);
}

////////////////////////////////////////////////////////////////////////////
//	�I�����C���܂��́A�I�t���C���ɂ���

int XTTY::Connect(BOOL fConnect)
{
	if (fConnect) {		// �I�����C���ɂ���
		if (m_fConnect)		// ���ɃI�����C���H
			return 0;	// ���������ɏI���

		m_idComDev = OpenComm("COM2", 1024, 128);
		if (m_idComDev < 0) {	// �I�[�v���Ɏ��s�H
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
	} else {		// �I�t���C���ɂ���
		if (!m_fConnect)	// ���ɃI�t���C���H
			return 0;	// ���������ɏI���

		EscapeCommFunction(m_idComDev, CLRDTR);
		CloseComm(m_idComDev);
		m_fConnect = FALSE;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	�X�N���[���o�b�t�@�̃X�N���[��

void XTTY::ScrollScreen()
{
	memmove(&m_abScreen[0*80+0], &m_abScreen[1*80+0], sizeof(BYTE) * (80 * 24));
	memset(&m_abScreen[24*80+0], ' ', 80);
}

////////////////////////////////////////////////////////////////////////////
//	WM_PAINT���b�Z�[�W����

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
//	WM_CHAR���b�Z�[�W����

void XTTY::OnChar(WPARAM wParam, LPARAM lParam)
{
	if (!m_fConnect)
		return;

	BYTE bCode = (BYTE)wParam;
	WriteComm2((const char *)&bCode, 1);
}

////////////////////////////////////////////////////////////////////////////
//	WM_COMMNOTIFY���b�Z�[�W����

void XTTY::OnCommNotify(WPARAM wParam, LPARAM lParam)
{
	char buf[128];
	int rtn;
	RECT rc;
	BOOL fScroll = FALSE;

	if (!m_fConnect)
		return;

	if (lParam & CN_EVENT) {	// �ʐM�f�o�C�X�C�x���g����
	}

	if (lParam & CN_RECEIVE) {	// ��M�C�x���g����
		rtn = ReadComm(m_idComDev, buf, sizeof(buf));

		if (rtn <= 0) {
			CommErrorMessage();
			return;
		}

		WriteLog(buf, rtn);
	}
}

////////////////////////////////////////////////////////////////////////////
//	�������Log�ɏ�������

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
//	OpenComm�̃G���[���b�Z�[�W

void XTTY::OpenComm_Error(int errCode)
{
	char * msg;

	switch (errCode) {
	case IE_BADID:
		msg = "�f�o�C�X���ʎq���������A�T�|�[�g����Ă��܂���B";
		break;
	case IE_BAUDRATE:
		msg = "�f�o�C�X�̃{�[���[�g�̓T�|�[�g����Ă��܂���B";
		break;
	case IE_BYTESIZE:
		msg = "�w�肳�ꂽ�o�C�g �T�C�Y�������ł��B";
		break;
	case IE_DEFAULT:
		msg = "�f�t�H���g�̃p�����[�^���Ԉ���Ă��܂��B";
		break;
	case IE_HARDWARE:
		msg = "�n�[�h�E�F�A�����p�\�ł͂���܂��� (�ق��̃f�o�C�X�����b�N���Ă��܂�)�B";
		break;
	case IE_MEMORY:
		msg = "�֐����L���[�����蓖�Ă邱�Ƃ��ł��܂���B";
		break;
	case IE_NOPEN:
		msg = "�f�o�C�X���I�[�v������Ă��܂���B";
		break;
	case IE_OPEN:
		msg = "�f�o�C�X�͂��łɃI�[�v������Ă��܂��B";
		break;
	default:
		msg = "���m�̃G���[�ł��B";
		break;
	}

	MessageBox(m_hwnd, msg, "OpenComm �G���[", MB_OK | MB_APPLMODAL);
}

////////////////////////////////////////////////////////////////////////////
//

void XTTY::CommErrorMessage()
{
	COMSTAT stat;
	int ce = GetCommError(m_idComDev, &stat);
	if (ce & CE_BREAK)
		MessageBox(m_hwnd, "�n�[�h�E�F�A���u���[�N��Ԃ����o�������Ƃ������܂��B", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_CTSTO)
		MessageBox(m_hwnd, "CTS (clear-to-send) ���^�C���A�E�g�ł��B�����𑗐M���Ă���ԁACOMSTAT�\���̂�fCtsHold�����o�ɂ��w�肳�ꂽ���Ԃ����ACTS�����[ ���x���ł��B", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_DNS)
		MessageBox(m_hwnd, "�p������ �f�o�C�X���I������Ȃ��������Ƃ������܂��B", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_DSRTO)
		MessageBox(m_hwnd, "DSR (data-set-ready) ���^�C���A�E�g�ł��B�����𑗐M���Ă���ԁACOMSTAT�\���̂�fDsrHold�����o�ɂ��w�肳�ꂽ���Ԃ����ADSR�����[ ���x���ł��B", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_FRAME)
		MessageBox(m_hwnd, "�n�[�h�E�F�A���t���[�~���O �G���[�����o�������Ƃ������܂��B", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_IOE)
		MessageBox(m_hwnd, "�p������ �f�o�C�X�Ƃ̒ʐM�ŁAI/O�G���[�������������Ƃ������܂��B", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_MODE)
		MessageBox(m_hwnd, "�v�����ꂽ���[�h���T�|�[�g����Ă��Ȃ����AidComDev�p�����[�^�������ł��邱�Ƃ������܂��BCE_MODE���Z�b�g�����ƁA���̃t���O�������L���ȃG���[�l�ɂȂ�܂��B", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_OOP)
		MessageBox(m_hwnd, "�p������ �f�o�C�X�̗p���؂�������M�����I���ɂȂ������Ƃ������܂��B", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_OVERRUN)
		MessageBox(m_hwnd, "���̕�������M����O�ɁA�n�[�h�E�F�A���當�����ǂݎ���Ȃ��������Ƃ������܂��B���̕����͎����܂��B", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_PTO)
		MessageBox(m_hwnd, "�p������ �f�o�C�X�Ƃ̒ʐM�ŁA�^�C���A�E�g�������������Ƃ������܂��B", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_RLSDTO)
		MessageBox(m_hwnd, "RLSD (receive-line-signal-detect) ���^�C���A�E�g�ł��B�����𑗐M���Ă���ԁACOMSTAT�\���̂�fRlsdHold�����o�ɂ��w�肳�ꂽ���Ԃ����ARLSD�����[ ���x���ł��B", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_RXOVER)
		MessageBox(m_hwnd, "��M�L���[�̃I�[�o�t���[���������܂����B���̓L���[�ɋ󂫂��Ȃ����A�G���h �I�u �t�@�C����������M���������������M�������Ƃ������܂��B", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_RXPARITY)
		MessageBox(m_hwnd, "�n�[�h�E�F�A���p���e�B �G���[�����o�������Ƃ������܂��B", NULL, MB_OK | MB_APPLMODAL);
	if (ce & CE_TXFULL)
		MessageBox(m_hwnd, "�֐����������L���[�ɓ���悤�Ƃ����Ƃ��ɁA���M�L���[�ɋ󂫂��Ȃ��������Ƃ������܂��B", NULL, MB_OK | MB_APPLMODAL);
}
