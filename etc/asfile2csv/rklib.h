#ifndef __RKLIB_H__
#define __RKLIB_H__

// �K�v�ȃw�b�_���C���N���[�h
#include <windows.h>

// �}�N��
#define	countof(I)	(sizeof(I)/sizeof(I[0]))

// �v���g�^�C�v
int chomp(char *pStr);
DWORD FormatMessageFromSystem(DWORD dwError, LPTSTR lpBuffer, DWORD nSize);

#endif
