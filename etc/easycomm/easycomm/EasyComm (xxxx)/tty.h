#ifndef __TTY_H
#define __TTY_H

////////////////////////////////////////////////////////////////////////////
//	�w�b�_

#include <windows.h>
#include <windowsx.h>
// #include <string.h>
// #include <process.h>

////////////////////////////////////////////////////////////////////////////
//	TTY�N���X

#define CN_EVENT	0x01
#define CN_RECEIVE	0x02

class XTTY
{
public:
	OVERLAPPED  m_os ;
	BOOL m_fContinue;
	DWORD m_arg[3];
	HANDLE m_thread;
	DWORD m_threadID;
	HINSTANCE m_hinst;
	HWND m_hwnd;
	BOOL m_fConnect;			// �ڑ����t���O TRUE:�I�����C�� FALSE:�I�t���C��
	HANDLE m_idComDev;			// COM�f�o�C�X�̎��ʎq
	int m_xPos, m_yPos;			// �J�[�\����X,Y���W
	BYTE m_abScreen[80 * 25];		// ��M�f�[�^�ۑ��o�b�t�@
	LOGFONT m_lf;				// TTY���g�p����t�H���g���
	HFONT m_hfont;				// TTY���g�p����t�H���g
	BOOL m_fLocalEcho;			// ���[�J�� �G�R�[�̗L�� TRUE:�L�� FALSE:����

	XTTY(HINSTANCE hinst, HWND hwnd);	// �R���X�g���N�^
	~XTTY();				// �f�X�g���N�^

	int Connect(BOOL fConnect);		// �ڑ�
	void OpenComm_Error();			// OpenComm�̃G���[���b�Z�[�W
	void ScrollScreen();			// �X�N���[���o�b�t�@�̃X�N���[��
	void CommErrorMessage();		// GetCommError���g�p���ẴG���[���b�Z�[�W �_�C�A���O
	void WriteLog(const char * buf, int n);	// ���O�o�b�t�@�ւ̕����񏑂�����
	int WriteComm2(const char * buf, int n);	// WriteLog�ɑΉ�����WriteComm�֐�
	friend DWORD WINAPI CommEvent(LPDWORD arg);		// WaitCommEvent�̂��߂̃X���b�h

	void OnChar(WPARAM wParam, LPARAM lParam);		// WM_CHAR���b�Z�[�W����
	void OnCommNotify(WPARAM wParam, LPARAM lParam);	// WM_COMMNOTIFY���b�Z�[�W����
	void OnPaint();						// WM_PAINT���b�Z�[�W����
};

#endif
