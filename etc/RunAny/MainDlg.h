/**
 */
#pragma once


#include <windows.h>
#include "kjmlib.h"
#include "unlha32.h"


/**
 */
class MainDlg : public kjm::Dialog {
public:
	// �f�t�H���g �R���X�g���N�^
	MainDlg() {};

	// �f�X�g���N�^
	virtual ~MainDlg() {};

	// WM_INITDIALOG ���b�Z�[�W
	virtual BOOL OnInitDialog(HWND hwndFocus, LPARAM lInitParam);

	// WM_COMMAND ���b�Z�[�W
	virtual BOOL OnCommand(WORD wNotifyCode, WORD wID, HWND hwndCtl);

	// WM_TIMER ���b�Z�[�W
	virtual BOOL OnTimer(WPARAM wTimerID, TIMERPROC* tmprc);

private:
	kjm::Process m_procExp;	// exp �R�}���h���s�p
	DWORD m_dwStart;		// �R�}���h�����s��������
};
