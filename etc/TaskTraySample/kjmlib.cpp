//-----------------------------------------------------------------------------
// 2012/03/14 r.kojima
//-----------------------------------------------------------------------------
#include "kjmlib.h"

//=============================================================================
// �^�X�N�g���C�A�C�R���������N���X
//=============================================================================
//-----------------------------------------------------------------------------
// �A�C�R���̓o�^
//
// hIcon, uCallback, pszTip �́A���ꂼ��ANULL or 0 ���\�B
// ��d�� taskTrayIcon::addIcon �����s�����ꍇ�A�폜���Ă���o�^����B
//-----------------------------------------------------------------------------
BOOL kjm::taskTrayIcon::addIcon(HWND hwnd, UINT uID, HICON hIcon, UINT uCallback, LPCTSTR pszTip) {
	// ���łɓo�^�ς݂�������Ȃ��̂ō폜����B
	deleteIcon();

	memset(&m_nid, 0, sizeof(m_nid));
	
	m_nid.cbSize = sizeof(m_nid);
	m_nid.hWnd = hwnd;
	m_nid.uID = uID;
	m_nid.uFlags = 0;

	if (hIcon != NULL) {
		m_nid.hIcon = hIcon;
		m_nid.uFlags |= NIF_ICON;
	}

	if (uCallback != 0) {
		m_nid.uCallbackMessage = uCallback;
		m_nid.uFlags |= NIF_MESSAGE;
	}

	if (pszTip != NULL) {
		lstrcpy(m_nid.szTip, pszTip);
		m_nid.uFlags |= NIF_TIP;
	}

	BOOL rc = Shell_NotifyIcon(NIM_ADD, &m_nid);
	if (rc == FALSE) {
		// �A�C�R���̒ǉ��Ɏ��s�����Ƃ��́A���o�^��Ԃɂ���B
		memset(&m_nid, 0, sizeof(m_nid));
	}

	return rc;
}

//-----------------------------------------------------------------------------
// �^�X�N�g���C����A�C�R������菜��
//
// taskTrayIcon::addIcon ���Ă��Ȃ��Ă����������삷��B
//-----------------------------------------------------------------------------
BOOL kjm::taskTrayIcon::deleteIcon() {
	BOOL rc = TRUE;

	if (m_nid.cbSize != 0) {
		rc = Shell_NotifyIcon(NIM_DELETE, &m_nid);

		// �������s�ɂ�����炸�A���o�^��Ԃɂ���B
		memset(&m_nid, 0, sizeof(m_nid));
	}

	return rc;
}
