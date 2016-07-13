// rkStatIcon.h : �X�e�[�^�X��Ԃ��^�X�N�g���C�A�C�R���ŕ\������N���X

#pragma once

#include <windows.h>

// �X�e�[�^�X�ɑΉ�����A�C�R���A��������i�[����

typedef struct {
	int state;		// �X�e�[�^�X�̔ԍ�
	HICON hicon;	// �X�e�[�^�X�ɑΉ�����A�C�R��
	char tip[64];	// �X�e�[�^�X�ɑΉ�����c�[���`�b�v�̕�����
} RKSTATINFO;

// �X�e�[�^�X�Ǘ��N���X

#define RK_MAX_STATINFO	32	// �o�^�\�ȍő�X�e�[�^�X��

class rkStatIcon
{
private:
	HWND m_hwnd;	// NOTIFYICONDATA::hWnd�p�f�[�^
	UINT m_uid;		// NOTIFYICONDATA::uID�p�f�[�^
	UINT m_uMsg;		// NOTIFYICONDATA::uCallbackMessage�p�f�[�^

	int m_state;							// ���݂̃X�e�[�^�X
	RKSTATINFO m_statInfo[RK_MAX_STATINFO];	// �X�e�[�^�X�ɑΉ�������
	bool m_useInfo[RK_MAX_STATINFO];		// m_statInfo[n]�̎g�p��������

public:
	rkStatIcon()
	{
		m_hwnd = NULL;
		m_uid = m_uMsg = 0;
		m_state = -1;	// �������(�^�X�N�g���C�ɃA�C�R�����o�^)
		memset(m_statInfo, 0, sizeof(m_statInfo));
		memset(m_useInfo, false, sizeof(m_useInfo));
	};
	virtual ~rkStatIcon()
	{
		if (m_state != -1)
			SetState(-1);	// �A�C�R���̓o�^������
	};

public:
	BOOL AddState(int state, HICON hicon, const char* tip);		// �X�e�[�^�X�̓o�^
	BOOL DeleteState(int state);								// �X�e�[�^�X�̍폜
	BOOL ModifyState(int state, HICON hicon, const char* tip);	// �X�e�[�^�X��Ԃ̕ύX

	VOID SetNotifyIconInfo(HWND hwnd, UINT uid, UINT uMsg);		// �^�X�N�g���C�A�C�R���p�̃f�[�^�o�^
	BOOL GetStateInfo(int state, RKSTATINFO* info);

	BOOL SetState(int state);	// �X�e�[�^�X��ݒ肷��
	int GetState()				// �X�e�[�^�X��Ԃ�
	{
		return m_state;
	}
};
