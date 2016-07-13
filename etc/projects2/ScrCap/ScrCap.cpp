//---------------------------------------------------------------------
// �ȒP��ʃL���v�`��
//---------------------------------------------------------------------
#include "ScrCap.h"

//---------------------------------------------------------------------
// �萔
//---------------------------------------------------------------------
#define ID_TASKTRAYICON	100				// �^�X�N�g���C�A�C�R����ID
#define MYWM_NOTIFYICON	(WM_USER + 1)	// �^�X�N�g���C�A�C�R������̃��b�Z�[�W

#define ID_HK_END		100				// �A�v���P�[�V�����I���̃z�b�g�L�[
#define ID_HK_CAPTURE	101				// ��ʃL���v�`���̃z�b�g�L�[

//---------------------------------------------------------------------
// �O���[�o���ϐ�
//---------------------------------------------------------------------
NOTIFYICONDATA g_nid;

//---------------------------------------------------------------------
// �f�X�N�g�b�v���L���v�`������
//---------------------------------------------------------------------
void DoDesktopCapture(HWND hwnd) {
	// ��ʑS�̂̋�`���擾
	RECT rc;
	SetRect(&rc, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

	// ��ʑS�̂� DIB �ɂ���
	HDIB hdib = CopyScreenToDIB(&rc);
	if (hdib) {
		static DWORD s_dwSerial = 0;
		SYSTEMTIME st;
		GetLocalTime(&st);

		// ���ݓ��� + �ʂ��ԍ��Ńt�@�C���������B
		TCHAR fname[MAX_PATH];
		wsprintf(fname, _T("%04d%02d%02d_%02d%02d%02d_%03d.bmp"),
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, s_dwSerial);
		s_dwSerial++;

		SaveDIB(hdib, kjm::path::rename_file_spec(kjm::util::get_module_file_name(), fname).c_str());
		DestroyDIB(hdib);
	}
}

//---------------------------------------------------------------------
// WM_COMMAND ���b�Z�[�W����
//---------------------------------------------------------------------
BOOL OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	WORD wNotifyCode = HIWORD(wParam);	// notification code 
	WORD wID = LOWORD(wParam);			// item, control, or accelerator identifier 
	HWND hwndCtl = (HWND) lParam;		// handle of control 

	switch (wID) {
	case IDOK:
	case IDCANCEL:
	case IDM_POPUP_EXIT:
		DestroyWindow(hwnd);
		return TRUE;
	}

	return FALSE;
}

//---------------------------------------------------------------------
// WM_DESTROY ���b�Z�[�W����
//---------------------------------------------------------------------
BOOL OnDestroy(HWND hwnd) {
	// �z�b�g�L�[����菜��
	UnregisterHotKey(hwnd, ID_HK_END);
	UnregisterHotKey(hwnd, ID_HK_CAPTURE);

	// �^�X�N�g���C�A�C�R�����폜
	Shell_NotifyIcon(NIM_DELETE, &g_nid);

	// ���b�Z�[�W���[�v�̏I�����w��
	PostQuitMessage(0);

	return TRUE;
}

//---------------------------------------------------------------------
// WM_HOTKEY ���b�Z�[�W����
//---------------------------------------------------------------------
BOOL OnHotKey(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	int idHotKey = (int) wParam;				// identifier of hot key 
	UINT fuModifiers = (UINT) LOWORD(lParam);	// key-modifier flags 
	UINT uVirtKey = (UINT) HIWORD(lParam);		// virtual-key code 

	switch (idHotKey) {
	case ID_HK_END:
		PostMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDM_POPUP_EXIT, 0), 0);
		break;

	case ID_HK_CAPTURE:
		DoDesktopCapture(hwnd);
		break;
	}

	return TRUE;
}

//---------------------------------------------------------------------
// WM_INITDIALOG ���b�Z�[�W����
//---------------------------------------------------------------------
BOOL OnInitDialog(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	HWND hwndFocus = (HWND) wParam;	// handle of control to receive focus 
	LONG lInitParam = lParam;		// initialization parameter 

	// �^�X�N�g���C�A�C�R����o�^
	ZeroMemory(&g_nid, sizeof(g_nid));
	g_nid.cbSize = sizeof(g_nid);
	g_nid.hWnd = hwnd;
	g_nid.uID = ID_TASKTRAYICON;
	g_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	g_nid.uCallbackMessage = MYWM_NOTIFYICON;
	g_nid.hIcon = LoadIcon(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDI_MAIN));
	lstrcpyn(g_nid.szTip, _T("�X�N���[���E�L���v�`��"), countof(g_nid.szTip));
	
	Shell_NotifyIcon(NIM_ADD, &g_nid);

	// �z�b�g�L�[��o�^
	RegisterHotKey(hwnd, ID_HK_END, MOD_CONTROL, VK_END);			// Ctrl + End
	RegisterHotKey(hwnd, ID_HK_CAPTURE, MOD_CONTROL, VK_SNAPSHOT);	// Ctrl + PrtSc

	return FALSE;	// �t�H�[�J�X�̓V�X�e���ɔC����
}

//---------------------------------------------------------------------
// MYWM_NOTIFYICON ���b�Z�[�W����
//---------------------------------------------------------------------
BOOL OnMyNotifyIcon(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	UINT uID = wParam;
	UINT uMouseMsg = lParam;

	if (uMouseMsg == WM_RBUTTONUP) {
		HMENU hmenu = LoadMenu(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDR_POPUPMENU));
		if (hmenu != NULL) {
			HMENU hmenuSub = GetSubMenu(hmenu, 0);
			if (hmenuSub) {
				POINT pt;
				GetCursorPos(&pt);
				TrackPopupMenu(hmenuSub, TPM_LEFTALIGN | TPM_BOTTOMALIGN, pt.x, pt.y, 0, hwnd, NULL);
				PostMessage(hwnd, WM_NULL, 0, 0);
			}
			DestroyMenu(hmenu);
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------
// �_�C�A���O�v���V�[�W��
//---------------------------------------------------------------------
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
		return OnCommand(hwnd, wParam, lParam);

	case WM_DESTROY:
		return OnDestroy(hwnd);

	case WM_HOTKEY:
		return OnHotKey(hwnd, wParam, lParam);

	case WM_INITDIALOG:
		return OnInitDialog(hwnd, wParam, lParam);

	case MYWM_NOTIFYICON:
		return OnMyNotifyIcon(hwnd, wParam, lParam);
	}

	return FALSE;
}

//---------------------------------------------------------------------
// �v���O�����X�^�[�g
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// �N�����ɔ�\���̃_�C�A���O����肽���̂ŁA���[�h���X�_�C�A���O���g�p
	HWND hdlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DlgProc);
	if (hdlg == NULL) {
		return -1;
	}

	MSG msg;
	while (1) {
		BOOL bRet = GetMessage(&msg, NULL, 0, 0);
		if (bRet == 0 || bRet == -1) {
			break;
		}

		if (!IsDialogMessage(hdlg, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}
