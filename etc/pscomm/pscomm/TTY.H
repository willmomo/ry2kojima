#ifndef __TTY_H
#define __TTY_H

////////////////////////////////////////////////////////////////////////////
//	�w�b�_

#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <ctype.h>

extern HINSTANCE tty_m_hinst;
extern HWND tty_m_hwnd;
extern BOOL tty_m_fConnect;			// �ڑ����t���O TRUE:�I�����C�� FALSE:�I�t���C��
extern int tty_m_idComDev;				// COM�f�o�C�X�̎��ʎq
extern BOOL tty_m_fLocalEcho;			// ���[�J�� �G�R�[�̗L�� TRUE:�L�� FALSE:����

// �v���g�^�C�v�錾
void XTTY_XTTY(HINSTANCE hinst, HWND hwnd);	// �R���X�g���N�^
void XTTY_deXTTY();				// �f�X�g���N�^

int XTTY_Connect(BOOL fConnect);		// �ڑ�
void XTTY_OpenComm_Error(int errCode);	// OpenComm�̃G���[���b�Z�[�W
void XTTY_ScrollScreen();			// �X�N���[���o�b�t�@�̃X�N���[��
void XTTY_CommErrorMessage();		// GetCommError���g�p���ẴG���[���b�Z�[�W �_�C�A���O
void XTTY_WriteLog(const char * buf, int n);	// ���O�o�b�t�@�ւ̕����񏑂�����
int XTTY_WriteComm2(const char * buf, int n);	// WriteLog�ɑΉ�����WriteComm�֐�
BOOL XTTY_ReadDCB(DCB *pdcb);
BOOL XTTY_ReadCom(char *pcom);
BOOL XTTY_WriteDCB(DCB *pdcb);
BOOL XTTY_WriteCom(char *pcom);

void XTTY_OnChar(WPARAM wParam, LPARAM lParam);		// WM_CHAR���b�Z�[�W����
void XTTY_OnCommNotify(WPARAM wParam, LPARAM lParam);	// WM_COMMNOTIFY���b�Z�[�W����
void XTTY_OnPaint(PAINTSTRUCT *ps);						// WM_PAINT���b�Z�[�W����


#endif