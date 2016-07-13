//---------------------------------------------------------------------
// GetSystemPowerStatus ���g���āA�o�b�e���̕ω���\���E�L�^����B
//
// <���l>
// �A�C�R���́A http://www.iconfinder.net/ ����A�E���Ă��܂����B
//---------------------------------------------------------------------
#include "SystemStatus.h"

#include <stdio.h>

#define RGB_BLACK	(RGB(0, 0, 0))


SYSTEM_POWER_STATUS g_sps;
static HWND s_hwnd;


//---------------------------------------------------------------------
// TextColor ���󂯎��A�����񒷂��ȗ��ł��� TextOut �֐�
//---------------------------------------------------------------------
void TextOutEx(HDC hdc, int nXStart, int nYStart, LPCTSTR lpString, COLORREF crTextColor) {
	SetTextColor(hdc, crTextColor);
	TextOut(hdc, nXStart, nYStart, lpString, lstrlen(lpString));
}


//---------------------------------------------------------------------
// SYSTEM_POWER_STATUS�̕ۑ�
//---------------------------------------------------------------------
void SaveSystemPowerStatus(DWORD dwPowerEvent, DWORD dwData) {
#if 0
	FILE* fp = _tfopen(_T("SystemPowerStatus.csv"), _T("a+"));
	if (fp) {
		SYSTEM_POWER_STATUS sps;
		GetSystemPowerStatus(&sps);

		SYSTEMTIME stNow;
		GetLocalTime(&stNow);
		_ftprintf(fp, _T("%d/%d/%d %d:%02d:%02d,%d,%d,%d,%d,%d,%d,%d\n"),
			stNow.wYear, stNow.wMonth, stNow.wDay,
			stNow.wHour, stNow.wMinute, stNow.wSecond,
			dwPowerEvent, dwData,
			sps.ACLineStatus,
			sps.BatteryFlag,
			sps.BatteryLifePercent,
			sps.BatteryLifeTime,
			sps.BatteryFullLifeTime
		);

		fclose(fp);
	}
#endif
}


//---------------------------------------------------------------------
// �ėp�I�`�揈��
//---------------------------------------------------------------------
static void Draw(HWND hwnd, HDC hdc) {
	COLORREF color1;
	TCHAR txtBuf[512];
	POINT pt = {4, 4};
	SIZE size;

	// �\���p�t�H���g�̍쐬�B
	HFONT hFontSys = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	GetObject(hFontSys, sizeof(lf), &lf);
	lf.lfHeight = -MulDiv(11, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	lf.lfWeight = FW_BOLD;
	//lstrcpy(lf.lfFaceName, _T("�l�r �o�S�V�b�N"));
	//lstrcpy(lf.lfFaceName, _T("���C���I"));
	//lf.lfCharSet = ANSI_CHARSET;
	//lstrcpy(lf.lfFaceName, _T("Arial"));
	//lstrcpy(lf.lfFaceName, _T("GauFontRoot"));

	HFONT hFont = CreateFontIndirect(&lf);
	HFONT hFontOld;

	hFontOld = (HFONT)SelectObject(hdc, hFont);

	// ��Ԓ����e�L�X�g����ɁA���W�̕ω��ʂ����߂�
	GetTextExtentPoint32(hdc, _T(" Charge status"), lstrlen(_T(" Charge status")), &size);
	size.cy += 1;

	// �o�b�e���[�������Ƃ��́A��ʒ�����NO SYSTEM BATTERY�ƕ`��
	if (g_sps.BatteryFlag & BATTERY_FLAG_NO_BATTERY) {
		RECT rcClient;
		GetClientRect(hwnd, &rcClient);
		
		SetTextColor(hdc, RGB(220, 0, 0));
		DrawText(hdc, _T("NO SYSTEM BATTERY"), -1, &rcClient, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		SelectObject(hdc, hFontOld);
		DeleteObject(hFont);

		return;
	}

	// AC status �̕`��

	switch (g_sps.ACLineStatus) {
	case AC_LINE_OFFLINE:
		lstrcpy(txtBuf, _T("Offline"));
		color1 = RGB(220, 0, 0);
		break;
	case AC_LINE_ONLINE:
		lstrcpy(txtBuf, _T("Online"));
		color1 = RGB(0, 180, 0);
		break;
	case AC_LINE_BACKUP_POWER:
		lstrcpy(txtBuf, _T("Backuped"));
		color1 = RGB(200, 150, 0);
		break;
	case AC_LINE_UNKNOWN:
		lstrcpy(txtBuf, _T("Unknown"));
		color1 = RGB(200, 150, 0);
		break;
	default:
		wsprintf(txtBuf, _T("(raw value : %d)"), g_sps.ACLineStatus);
		color1 = RGB_BLACK;
		break;
	}
	TextOutEx(hdc, pt.x, pt.y, _T("AC status"), RGB_BLACK);
	TextOutEx(hdc, pt.x + size.cx + 12, pt.y, txtBuf, color1);

	pt.y += size.cy;

	TextOutEx(hdc, pt.x, pt.y, _T("Battery"), RGB_BLACK);
	
	pt.y += size.cy;

	// Charge status �̕`��

	TextOutEx(hdc, pt.x, pt.y, _T(" Charge status"), RGB_BLACK);
	TextOutEx(hdc, pt.x + size.cx + 12, pt.y, _T("Charging"), (g_sps.BatteryFlag & BATTERY_FLAG_CHARGING) ? RGB(220, 170, 0) : RGB(210, 210, 210));

	pt.y += size.cy;

	// BatteryLifePercent �̕`��

	if (g_sps.BatteryFlag & BATTERY_FLAG_HIGH) {
		wsprintf(txtBuf, _T("%d%%%s"), g_sps.BatteryLifePercent, _T(" [High]"));
		color1 = RGB(0, 180, 0);
	} else if (g_sps.BatteryFlag & BATTERY_FLAG_LOW) {
		wsprintf(txtBuf, _T("%d%%%s"), g_sps.BatteryLifePercent, _T(" [Low]"));
		color1 = RGB(220, 170, 0);
	} else if (g_sps.BatteryFlag & BATTERY_FLAG_CRITICAL) {
		wsprintf(txtBuf, _T("%d%%%s"), g_sps.BatteryLifePercent, _T(" [Critical]"));
		color1 = RGB(220, 0, 0);
	} else {
		wsprintf(txtBuf, _T("%d%%%s"), g_sps.BatteryLifePercent, _T(" [Normal]"));
		color1 = RGB_BLACK;
	}

	TextOutEx(hdc, pt.x, pt.y, _T(" Life Percent"), RGB_BLACK);
	TextOutEx(hdc, pt.x + size.cx + 12, pt.y, txtBuf, color1);
	
	pt.y += size.cy;

	// BatteryLifeTime �̕`��

	if (g_sps.BatteryLifeTime == BATTERY_LIFE_UNKNOWN) {
		wsprintf(txtBuf, _T("unknown"));
	} else if (g_sps.BatteryLifeTime >= 3600) {
		wsprintf(txtBuf, _T("%dh %dm %ds"), g_sps.BatteryLifeTime / 3600, (g_sps.BatteryLifeTime % 3600) / 60, (g_sps.BatteryLifeTime % 3600) % 60);
	} else if (g_sps.BatteryLifeTime >= 60) {
		wsprintf(txtBuf, _T("%dm %ds"), g_sps.BatteryLifeTime / 60, g_sps.BatteryLifeTime % 60);
	} else {
		wsprintf(txtBuf, _T("%d sec"), g_sps.BatteryLifeTime);
	}

	TextOutEx(hdc, pt.x, pt.y, _T(" Life Time"), RGB_BLACK);
	TextOutEx(hdc, pt.x + size.cx + 12, pt.y, txtBuf, RGB_BLACK);

	pt.y += size.cy;

	// BatteryFullLifeTime �̕`��

	DWORD dwflt;
	if (g_sps.BatteryFullLifeTime == BATTERY_LIFE_UNKNOWN && g_sps.BatteryLifeTime != BATTERY_LIFE_UNKNOWN) {
		dwflt = (g_sps.BatteryLifeTime * 100) / g_sps.BatteryLifePercent;
	} else {
		dwflt = g_sps.BatteryFullLifeTime;
	}

	if (dwflt == BATTERY_LIFE_UNKNOWN) {
		wsprintf(txtBuf, _T("unknown"));
	} else if (dwflt >= 3600) {
		wsprintf(txtBuf, _T("%dh %dm %ds"), dwflt / 3600, (dwflt % 3600) / 60, (dwflt % 3600) % 60);
	} else if (dwflt >= 60) {
		wsprintf(txtBuf, _T("%dm %ds"), dwflt / 60, dwflt % 60);
	} else {
		wsprintf(txtBuf, _T("%d sec"), dwflt);
	}

	TextOutEx(hdc, pt.x, pt.y, _T(" Full Life Time"), RGB_BLACK);
	TextOutEx(hdc, pt.x + size.cx + 12, pt.y, txtBuf, RGB_BLACK);

	pt.y += size.cy;

	// BatteryLifeTime ����̐����l�̂Ƃ��́A[estimate time]�̕\����ǉ��B

	if (g_sps.BatteryFullLifeTime == BATTERY_LIFE_UNKNOWN && g_sps.BatteryLifeTime != BATTERY_LIFE_UNKNOWN) {
		TextOutEx(hdc, pt.x + size.cx + 12, pt.y, _T("[estimate time]"), RGB_BLACK);
	}

	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
}


//---------------------------------------------------------------------
// WM_CREATE���b�Z�[�W����
//---------------------------------------------------------------------
static LRESULT OnCreate(HWND hwnd, WPARAM wParam, LPARAM lParam) {

	// �d����ԕϐ���������
	GetSystemPowerStatus(&g_sps);

	// �N�����̓d����Ԃ��L�^
	SaveSystemPowerStatus(0, 0);

	return 0;
}


//---------------------------------------------------------------------
// WM_PAINT���b�Z�[�W����
//---------------------------------------------------------------------
LRESULT OnPaint(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;

	hdc = BeginPaint(hwnd, &ps);

	Draw(hwnd, hdc);

	EndPaint(hwnd, &ps);
	return 0;
}


//---------------------------------------------------------------------
// WM_POWERBROADCAST���b�Z�[�W����
//---------------------------------------------------------------------
LRESULT OnPowerBroadcast(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	DWORD dwPowerEvent = (DWORD)wParam;
	DWORD dwData = (DWORD)lParam;

	// �d����ԕϐ��̍X�V
	//
	// �͂��߂́A�`�掞�Ɏ擾���ĂȂ�ׂ��V������Ԃ�\������悤�ɂ��Ă������A
	// GetSystemPowerStatus �֐��̏��v���Ԃ��A250ms ���x�����邱�Ƃ������A
	// ��ʂ���������߁A���̃^�C�~���O�ŏ����X�V����悤�ɏC���B

	GetSystemPowerStatus(&g_sps);

	SaveSystemPowerStatus(dwPowerEvent, dwData);

	// �E�B���h�E�̍ĕ`��𑣂�
	InvalidateRect(hwnd, NULL, TRUE);
	return TRUE;
}


//-----------------------------------------------------------------------------
// System Power Status �p�E�B���h�E�v���V�[�W��
//-----------------------------------------------------------------------------
static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT ret;

	switch (uMsg) {
	case WM_CREATE:
		return OnCreate(hwnd, wParam, lParam);

	case WM_PAINT:
		return OnPaint(hwnd, wParam, lParam);

	case WM_POWERBROADCAST:
		return OnPowerBroadcast(hwnd, wParam, lParam);

	case WM_NCHITTEST:
		// �V�X�e����Hit Test�����āA�N���C�A���g�̒���������A������悤�Ɉړ�����B
		ret = DefWindowProc(hwnd, uMsg, wParam, lParam);
		if (ret == HTCLIENT) {
			MainFrm_MovePosition();
		}
		return ret;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



BOOL SystemPowerStatusWnd_PostMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	return PostMessage(s_hwnd, uMsg, wParam, lParam);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
BOOL SystemPowerStatus_CreateWindow(HINSTANCE hInstance, HWND hwndParent) {
	s_hwnd = CreateWindow(CLASSNAME_SYSTEMPOWERSTATUS, 
		_T(""),
		WS_BORDER | WS_VISIBLE | WS_CHILD,
		50, 0, 243, 122,
		hwndParent, NULL, hInstance, NULL);
	if (s_hwnd == NULL) {
		return FALSE;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// System Power Status �\���p�q�E�B���h�E��o�^
//-----------------------------------------------------------------------------
BOOL SystemPowerStatus_RegistClass(HINSTANCE hInstance) {
	// �E�B���h�E�N���X�̓o�^
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASSNAME_SYSTEMPOWERSTATUS;
	
	return RegisterClassEx(&wc);
}
