#pragma once

#include <windows.h>
#include <assert.h>

#define ABE_FLOAT	4

class CAppBar
{
public:
	struct APPBARCTRL {
		HWND hwnd;			// AppBar �ɂ��� window �̃n���h��
		UINT uEdge;			// �����|�W�V����
		UINT uMsg;			// AppBar ���b�Z�[�W ID
		SIZE m_sizeFloat;	// ��dock���� window �̕��ƍ���
		SIZE m_sizeDock;	// dock ���� window �̕��ƍ���
	};

private:
	APPBARDATA m_abd;
	SIZE m_sizeFloating;	// ABE_FLOAT���̕��ƍ���
	SIZE m_sizeDocking;		// Dock���̕�/����
	POINT m_ptOffset;		// window drag���A����̃}�E�X���W����̃I�t�Z�b�g
	bool m_fRegisterd;		// �V�X�e���ɓo�^�ς݂̂Ƃ���true�A�܂��̂Ƃ���false
	
	RECT m_rcTarget;		// window �ړ���/��̋�`
	UINT m_uEdgeTarget;		// window �ړ���/���Dock�ʒu

public:
	UINT GetEdge() const {
		return m_abd.uEdge;
	};

public:

	CAppBar();				// �R���X�g���N�^
	virtual ~CAppBar();		// �f�X�g���N�^

	void Attach(const APPBARCTRL* pabc);
	void Detach();

	void MoveWindow(const RECT* lprc, BOOL bRepaint=TRUE);	// ��������Window���ړ�����
	void QueryPos(UINT uNewEdge, RECT* lprc);				// �V�����[�ɓK������`���v�Z����

	void Dock();							// �[�� dock ������
	void Dock(UINT uEdge);					// �w��[�� dock ������
	void Undock();							// �V�X�e������؂藣��
	UINT PtToEdge(LPPOINT lppt);			// �X�N���[�����W����Adock ���ׂ� edge ��Ԃ�

	void OnEnterSizeMove();					// WM_ENTERSIZEMOVE ���b�Z�[�W����
	void OnExitSizeMove();					// WM_EXITSIZEMOVE ���b�Z�[�W����
	void OnSizing(RECT* lprc);				// WM_SIZING ���b�Z�[�W����
	void OnMoving(RECT* lprc);				// WM_MOVING ���b�Z�[�W����
	UINT OnNCHitTest(WPARAM wParam, LPARAM lParam);	// WM_NCHITTEST ���b�Z�[�W����
	void OnWindowPosChanged();				// WM_WINDOWPOSCHANGED ���b�Z�[�W����
	void OnActivate();						// WM_ACTIVATE ���b�Z�[�W����
	void OnAppBarMessage(WPARAM wParam);	// AppBar �ʒm���b�Z�[�W����
};
