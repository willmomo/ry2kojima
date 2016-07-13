// rkStatIcon.cpp : �X�e�[�^�X��Ԃ��^�X�N�g���C�A�C�R���ŕ\������N���X

#include "rkStatIcon.h"

// �X�e�[�^�X��Ԃ̕ύX

BOOL rkStatIcon::ModifyState(int state, HICON hicon, const char *tip)
{
	int i;

	// ���łɓo�^����Ă�����̂�u��������
	for (i = 0; i < RK_MAX_STATINFO; i++) {
		if (m_useInfo[i] && m_statInfo[i].state == state) {
			if (hicon)	m_statInfo[i].hicon = hicon;
			if (tip)	lstrcpy(m_statInfo[i].tip, tip);
			return TRUE;
		}
	}

	return FALSE;	// ���o�^�Ȃ�G���[�I��
}

// �X�e�[�^�X�̓o�^

BOOL rkStatIcon::AddState(int state, HICON hicon, const char *tip)
{
	int i;

	// ���łɓo�^����Ă���X�e�[�^�X�Ȃ�u��������
	if (ModifyState(state, hicon, tip))
		return TRUE;

	// �Ȃ���΁A�����Ă���Ƃ���ɓo�^����
	for (i = 0; i < RK_MAX_STATINFO; i++) {
		if (m_useInfo[i] == false) {
			m_useInfo[i] = true;
			m_statInfo[i].state = state;
			m_statInfo[i].hicon = hicon;
			lstrcpy(m_statInfo[i].tip, tip);

			return TRUE;
		}
	}

	return FALSE;	// �o�^�ł��Ȃ���΁A�G���[
}

// �X�e�[�^�X�̍폜

BOOL rkStatIcon::DeleteState(int state)
{
	int i;

	// ������΁A�폜
	for (i = 0; i < RK_MAX_STATINFO; i++) {
		if (m_useInfo[i] && m_statInfo[i].state == state) {
			m_useInfo[i] = false;
			memset(&m_statInfo[i], 0, sizeof(RKSTATINFO));
			
			return TRUE;
		}
	}

	return FALSE;
}

// �^�X�N�g���C�A�C�R���ɕK�v�ȏ���ݒ肷��

VOID rkStatIcon::SetNotifyIconInfo(HWND hwnd, UINT uid, UINT uMsg)
{
	m_hwnd = hwnd;
	m_uid = uid;
	m_uMsg = uMsg;
}

// �X�e�[�^�X��ݒ肷��

BOOL rkStatIcon::SetState(int state)
{
	RKSTATINFO info;

	if (!m_hwnd || !m_uid)	return FALSE;	// SetNotifyIconInfo���g�p���̓G���[

	if (state == -1 && m_state != -1) {		// �^�X�N�g���C�A�C�R������菜��
		NOTIFYICONDATA nid;
		memset(&nid, 0, sizeof(nid));
		nid.cbSize = sizeof(nid);
		nid.hWnd = m_hwnd;
		nid.uID = m_uid;
		
		m_state = state;
		return Shell_NotifyIcon(NIM_DELETE, &nid);
	} else if (state >= 0 && m_state == -1 && GetStateInfo(state, &info)) {	// �^�X�N�g���C�ɃA�C�R����o�^
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
	} else if (state >= 0 && m_state != -1 && GetStateInfo(state, &info)) {	// �^�X�N�g���C�̃A�C�R����ύX
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

// �X�e�[�^�X�ɑΉ����������擾

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

