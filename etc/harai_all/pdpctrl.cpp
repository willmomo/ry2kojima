//---------------------------------------------------------------------
// [�T�v]
// PDP���R���g���[������֐��Q
//---------------------------------------------------------------------

#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include "def.h"

//---------------------------------------------------------------------
// [�T�v]
// COM�|�[�g���J��
//
// [���l]
// �ʐM�����̐ݒ�́APDP�ŗL�̐ݒ�ɌŒ�B
//---------------------------------------------------------------------

HANDLE OpenComm()
{
	// ini�Ŏw�肵��COM�|�[�g���J��
	HANDLE hComm = CreateFile(com, GENERIC_READ|GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	// ����ɃI�[�v���ł�����ʐM�p�����[�^��ݒ�
	if (hComm != INVALID_HANDLE_VALUE) {
		DCB dcb;

		GetCommState(hComm, &dcb);

		/* ���[�J�[�ɂ��OPEN����COM�̏���ω������� */
		switch(maker){
		case H_GLORY_OLD:
		case H_GLORY_NEW:
			/* �O���[���[���o�@ */
			dcb.BaudRate = CBR_9600;	// �{�[���[�g: 9600bps
			dcb.Parity = ODDPARITY;		// �p���e�B: 1
			dcb.ByteSize = 8;			// �L�����N�^�[��: 8bit
			dcb.StopBits = TWOSTOPBITS;	// �X�g�b�v�r�b�g: 2bit
			break;
		case H_SANKYO:
			/* �O�� */
			dcb.BaudRate = CBR_2400;	// �{�[���[�g: 2400bps
			dcb.Parity = EVENPARITY;		// �p���e�B: �Ȃ�
			dcb.ByteSize = 7;			// �L�����N�^�[��: 8bit
			dcb.StopBits = ONESTOPBIT;	// �X�g�b�v�r�b�g: 1bit
			break;
		case H_MARS:
			/* �}�[�X */
			dcb.BaudRate = CBR_4800;	// �{�[���[�g: 4800bps
			dcb.Parity = NOPARITY;		// �p���e�B: �Ȃ�
			dcb.ByteSize = 8;			// �L�����N�^�[��: 8bit
			dcb.StopBits = ONESTOPBIT;	// �X�g�b�v�r�b�g: 1bit
			break;
		case H_HIROTAC:
			/* �q���^�b�N */
		case H_SUBARU:
			/* �X�o�� */
		case H_TMI:
			/* TMI */
		default:
			/* �O���[���[���o�@ */
			dcb.BaudRate = CBR_9600;	// �{�[���[�g: 9600bps
			dcb.Parity = ODDPARITY;		// �p���e�B: �Ȃ�
			dcb.ByteSize = 8;			// �L�����N�^�[��: 8bit
			dcb.StopBits = TWOSTOPBITS;	// �X�g�b�v�r�b�g: 1bit
			break;
		}

		
		// �t���[����: �Ȃ�
		dcb.fDtrControl = DTR_CONTROL_DISABLE;
		dcb.fRtsControl = RTS_CONTROL_DISABLE;

		SetCommState(hComm, &dcb);

		// �^�C���A�E�g���Ԃ̐ݒ�
		COMMTIMEOUTS ct;
		GetCommTimeouts(hComm, &ct);

		// ��M�C���^�[�o���^�C���A�E�g: 0ms(�Ȃ�)
		ct.ReadIntervalTimeout = 0;

		// ��M�^�C���A�E�g: 500ms + (10ms * ��M�o�C�g��)
		ct.ReadTotalTimeoutMultiplier = 10;
		ct.ReadTotalTimeoutConstant = 500;

		// ���M�^�C���A�E�g: 3000ms + (10ms * ���M�o�C�g��)
		ct.WriteTotalTimeoutMultiplier = 10;
		ct.WriteTotalTimeoutConstant = 3000;

		SetCommTimeouts(hComm, &ct);
	} else {
		return INVALID_HANDLE_VALUE;
	}

	return hComm;
}

//---------------------------------------------------------------------
// [�T�v]
// �d�����M
//
// flag = (0/1/2/3)=(STX.ETX�L/STX�Ȃ�/ETX�Ȃ�/�����Ƃ�����)
//---------------------------------------------------------------------

int SendData(HANDLE hComm, const char* pszCmd)
{
	BOOL ret;
	DWORD dwWritten = 0;

	char work[256];

	wsprintf(work, "%s", pszCmd);
	ret = WriteFile(hComm, work, lstrlen(work), &dwWritten, NULL);

	return (ret == FALSE || (int)dwWritten != lstrlen(work)) ? -1 : 0;
}

//---------------------------------------------------------------------
// [�T�v]
// ��M�d�����
//
// [���l]
// �Ȃ�
//---------------------------------------------------------------------

int RecvData(HANDLE hComm, char* pszAns, size_t bufSize)
{
	int wp = 0;
	BOOL ret;
	DWORD dwRead;
	char work[256];
	char stx_rec_flag, etx_rec_flag;

	stx_rec_flag = 0;
	etx_rec_flag = 0;

	// �܂���1BYTE�̂݉��
	for (;;) {
		ret = ReadFile(hComm, work, 1, &dwRead, NULL);	// 1�o�C�g�ǂݍ���
		if (ret == FALSE){
			/* ��M�G���[�P */
			return -1;	// �G���[�����B�G���[�I��
		}
		if (dwRead == 0) {
			/* ��M�Ȃ� */
			return 0;	// ��M�Ȃ��B���֐i��
		}
		/* �d����M */
		if((work[0] == '\x53') && (maker == H_MARS)){
			/* STX */
			stx_rec_flag = TRUE;
		}
		if(work[0] == '\x02') {
			/* STX */
			stx_rec_flag = TRUE;
		}
		/* �ޯ̧�֎�M�d����ۑ� */
		pszAns[wp++] = work[0];
		break;
	}

	if(stx_rec_flag == FALSE){
		/* STX�ȊO�͂�����return */
		return wp;
	}

	// ETX�܂ŁA�ǂݍ���
	for (;;) {
		ret = ReadFile(hComm, work, 1, &dwRead, NULL);	// 1�o�C�g�ǂݍ���
		if (ret == FALSE) {
			/* ��M�G���[�Q */
			return -1;	// �G���[�����B�G���[�I��
		} else if (dwRead == 0) {
			/* printf("��M�^�C���A�E�g�Q\n"); */
			return -1;	// �^�C���A�E�g�����B�G���[�I��
		}else if((work[0] == '\x45') && (maker == H_MARS)){
			/* printf("��M�����Q\n");*/
			pszAns[wp++] = work[0];
			etx_rec_flag = TRUE;
			break;	// ETX�ǂݍ��݁B���֐i��
		} else if (work[0] == '\x03') {
			/* printf("��M�����Q\n");*/
			pszAns[wp++] = work[0];
			etx_rec_flag = TRUE;
			break;	// ETX�ǂݍ��݁B���֐i��
		} else {
			pszAns[wp++] = work[0];
		}
	}

	/* BCC��Ǎ��� */
	if(etx_rec_flag == TRUE){
		ret = ReadFile(hComm, work, 1, &dwRead, NULL);	// 1�o�C�g�ǂݍ���
		if (ret == FALSE) {
			/* ��M�G���[�Q */
			return -1;	// �G���[�����B�G���[�I��
		} else if (dwRead == 0) {
			return -1;	// �^�C���A�E�g�����B�G���[�I��
		} else {
			/* BCC��M */
			pszAns[wp++] = work[0];
		}
	}

	/* MARS�̏ꍇ��CR+LF������̂ł�����ǂݍ��� */
	if(maker == H_MARS){
		ret = ReadFile(hComm, work, 1, &dwRead, NULL);	// 1�o�C�g�ǂݍ���
		if (ret == FALSE) {
			/* ��M�G���[�Q */
			return -1;	// �G���[�����B�G���[�I��
		} else if (dwRead == 0) {
			return -1;	// �^�C���A�E�g�����B�G���[�I��
		} else {
			/* BCC��M */
			pszAns[wp++] = work[0];
		}
		ret = ReadFile(hComm, work, 1, &dwRead, NULL);	// 1�o�C�g�ǂݍ���
		if (ret == FALSE) {
			/* ��M�G���[�Q */
			return -1;	// �G���[�����B�G���[�I��
		} else if (dwRead == 0) {
			return -1;	// �^�C���A�E�g�����B�G���[�I��
		} else {
			/* BCC��M */
			pszAns[wp++] = work[0];
		}
		ret = ReadFile(hComm, work, 1, &dwRead, NULL);	// 1�o�C�g�ǂݍ���
		if (ret == FALSE) {
			/* ��M�G���[�Q */
			return -1;	// �G���[�����B�G���[�I��
		} else if (dwRead == 0) {
			return -1;	// �^�C���A�E�g�����B�G���[�I��
		} else {
			/* BCC��M */
			pszAns[wp++] = work[0];
		}
	}

	return wp;
}

