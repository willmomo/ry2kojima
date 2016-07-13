// rk_w32.cpp : Win32API�Ŏg����֐��Ȃ�
//
// �֐��A�N���X�̐擪�͕K��rk�Ŏn�܂�B
// ��{�I�ɁAUnicode�͖���

#include "rk_cpp.h"
#include "rk_w32.h"

// FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM ... �̊ȈՌĂяo��
//
// [����]
// DWORD dwError     : GetLastError()�̖߂�l
// LPTSTR *lppBuffer : ������o�b�t�@�̃A�h���X���󂯎��o�b�t�@�ւ̃|�C���^
//
// [�߂�l]
// FormatMessage()�Ɠ���
//
// [���l]
// �s���̉��s�͎�菜����܂��B
// lppBuffer�Ŏ󂯎�����o�b�t�@�́ALocalFree()�ŊJ�����Ă��������B

DWORD rkFormatMessageFromSystem(DWORD dwError, LPTSTR *lppBuffer)
{
	DWORD dwRet = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, dwError, 0, (LPTSTR)lppBuffer, 256, NULL);
	if (dwRet != 0) {
		while (rkChomp(*lppBuffer)) ;
	} else {
		DWORD dwErr = GetLastError();	// FormatMessage�ŃG���[���o��
		
		if (dwErr == ERROR_MR_MID_NOT_FOUND) {
			*lppBuffer = (LPTSTR)LocalAlloc(LPTR, 256);
			wsprintf(*lppBuffer, "The system cannot find message text for message number 0x%08X.", dwError);
		} else if (dwErr == ERROR_INVALID_PARAMETER) {
			*lppBuffer = (LPTSTR)LocalAlloc(LPTR, 256);
			wsprintf(*lppBuffer, "The parameter is incorrect.");
		} else {
			*lppBuffer = (LPTSTR)LocalAlloc(LPTR, 256);
			wsprintf(*lppBuffer, "FormatMessage function error: 0x%08X.", dwErr);
		}
	}

	return dwRet;
}
