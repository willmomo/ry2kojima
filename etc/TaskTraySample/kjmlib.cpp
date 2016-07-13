//-----------------------------------------------------------------------------
// 2012/03/14 r.kojima
//-----------------------------------------------------------------------------
#include "kjmlib.h"

//=============================================================================
// タスクトレイアイコンを扱うクラス
//=============================================================================
//-----------------------------------------------------------------------------
// アイコンの登録
//
// hIcon, uCallback, pszTip は、それぞれ、NULL or 0 が可能。
// 二重に taskTrayIcon::addIcon を実行した場合、削除してから登録する。
//-----------------------------------------------------------------------------
BOOL kjm::taskTrayIcon::addIcon(HWND hwnd, UINT uID, HICON hIcon, UINT uCallback, LPCTSTR pszTip) {
	// すでに登録済みかもしれないので削除する。
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
		// アイコンの追加に失敗したときは、未登録状態にする。
		memset(&m_nid, 0, sizeof(m_nid));
	}

	return rc;
}

//-----------------------------------------------------------------------------
// タスクトレイからアイコンを取り除く
//
// taskTrayIcon::addIcon していなくても正しく動作する。
//-----------------------------------------------------------------------------
BOOL kjm::taskTrayIcon::deleteIcon() {
	BOOL rc = TRUE;

	if (m_nid.cbSize != 0) {
		rc = Shell_NotifyIcon(NIM_DELETE, &m_nid);

		// 成功失敗にかかわらず、未登録状態にする。
		memset(&m_nid, 0, sizeof(m_nid));
	}

	return rc;
}
