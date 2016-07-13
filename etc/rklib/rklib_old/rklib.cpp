#include "rklib.h"

///////////////////////////////////////////////////////////////////////
//	[�T�v]
//	������I�[�̉��s(\r, \n)����菜��
//
//	[����]
//	pStr	���s����菜������������
//
//	[�߂�l]
//	���s����菜������
//	0	:�s���ɉ��s����
//	1	:\x0d or \x0a 1�����̉��s����菜����
//	2	:\x0d\x0a 2�����΂̉��s����菜����

int chomp(char *pStr)
{
	int nChomp = 0;	// ��菜������

	// �s����\n(\x0a)����菜��
	int nLen = strlen(pStr);
	if (nLen > 0 && pStr[nLen - 1] == '\n') {
		pStr[nLen - 1] = '\0';
		++nChomp;
	}

	// �s����\r(\x0d)����菜��
	nLen = strlen(pStr);
	if (nLen > 0 && pStr[nLen - 1] == '\r') {
		pStr[nLen - 1] = '\0';
		++nChomp;
	}

	return nChomp;
}

///////////////////////////////////////////////////////////////////////
//	[�T�v]
//	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, ...)�ȈՌďo
//
//	[����]
//	dwError		GetLastError()�̖߂�l
//	lpBuffer	FormatMessage�֐��̂���ɓ���
//	nSize		����
//
//	[�߂�l]
//	FormatMessage�ɓ���
//
//	[���l]
//	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, lpBuffer, nSize, NULL);
//	����邾���B�s���̉��s�͎�菜����܂��B

DWORD FormatMessageFromSystem(DWORD dwError, LPTSTR lpBuffer, DWORD nSize)
{
	DWORD dwRet = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, lpBuffer, nSize, NULL);
	while (chomp(lpBuffer)) ;

	return dwRet;
}
