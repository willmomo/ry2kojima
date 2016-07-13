// rkStatIcon.cpp : ステータス状態をタスクトレイアイコンで表示するクラス

#include "rkStatIcon.h"

// ステータス状態の変更

BOOL rkStatIcon::ModifyState(int state, HICON hicon, const char *tip)
{
	int i;

	// すでに登録されているものを置き換える
	for (i = 0; i < RK_MAX_STATINFO; i++) {
		if (m_useInfo[i] && m_statInfo[i].state == state) {
			if (hicon)	m_statInfo[i].hicon = hicon;
			if (tip)	lstrcpy(m_statInfo[i].tip, tip);
			return TRUE;
		}
	}

	return FALSE;	// 未登録ならエラー終了
}

// ステータスの登録

BOOL rkStatIcon::AddState(int state, HICON hicon, const char *tip)
{
	int i;

	// すでに登録されているステータスなら置き換える
	if (ModifyState(state, hicon, tip))
		return TRUE;

	// なければ、あいているところに登録する
	for (i = 0; i < RK_MAX_STATINFO; i++) {
		if (m_useInfo[i] == false) {
			m_useInfo[i] = true;
			m_statInfo[i].state = state;
			m_statInfo[i].hicon = hicon;
			lstrcpy(m_statInfo[i].tip, tip);

			return TRUE;
		}
	}

	return FALSE;	// 登録できなければ、エラー
}

// ステータスの削除

BOOL rkStatIcon::DeleteState(int state)
{
	int i;

	// 見つかれば、削除
	for (i = 0; i < RK_MAX_STATINFO; i++) {
		if (m_useInfo[i] && m_statInfo[i].state == state) {
			m_useInfo[i] = false;
			memset(&m_statInfo[i], 0, sizeof(RKSTATINFO));
			
			return TRUE;
		}
	}

	return FALSE;
}

// タスクトレイアイコンに必要な情報を設定する

VOID rkStatIcon::SetNotifyIconInfo(HWND hwnd, UINT uid, UINT uMsg)
{
	m_hwnd = hwnd;
	m_uid = uid;
	m_uMsg = uMsg;
}

// ステータスを設定する

BOOL rkStatIcon::SetState(int state)
{
	RKSTATINFO info;

	if (!m_hwnd || !m_uid)	return FALSE;	// SetNotifyIconInfo未使用時はエラー

	if (state == -1 && m_state != -1) {		// タスクトレイアイコンを取り除く
		NOTIFYICONDATA nid;
		memset(&nid, 0, sizeof(nid));
		nid.cbSize = sizeof(nid);
		nid.hWnd = m_hwnd;
		nid.uID = m_uid;
		
		m_state = state;
		return Shell_NotifyIcon(NIM_DELETE, &nid);
	} else if (state >= 0 && m_state == -1 && GetStateInfo(state, &info)) {	// タスクトレイにアイコンを登録
		NOTIFYICONDATA nid;
		memset(&nid, 0, sizeof(nid));
		nid.cbSize = sizeof(nid);
		nid.hWnd = m_hwnd;
		nid.uID = m_uid;
		nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		nid.uCallbackMessage = m_uMsg;
		nid.hIcon = info.hicon;
		lstrcpy(nid.szTip, info.tip);

		m_state = state;
		return Shell_NotifyIcon(NIM_ADD, &nid);
	} else if (state >= 0 && m_state != -1 && GetStateInfo(state, &info)) {	// タスクトレイのアイコンを変更
		NOTIFYICONDATA nid;
		memset(&nid, 0, sizeof(nid));
		nid.cbSize = sizeof(nid);
		nid.hWnd = m_hwnd;
		nid.uID = m_uid;
		nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		nid.uCallbackMessage = m_uMsg;
		nid.hIcon = info.hicon;
		lstrcpy(nid.szTip, info.tip);

		m_state = state;
		return Shell_NotifyIcon(NIM_MODIFY, &nid);
	}

	return FALSE;
}

// ステータスに対応した情報を取得

BOOL rkStatIcon::GetStateInfo(int state, RKSTATINFO *info)
{
	int i;

	for (i = 0; i < RK_MAX_STATINFO; i++) {
		if (m_useInfo[i] && m_statInfo[i].state == state) {
			memmove(info, &m_statInfo[i], sizeof(RKSTATINFO));

			return TRUE;
		}
	}

	return FALSE;
}

