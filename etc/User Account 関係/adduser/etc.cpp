#include "stdafx.h"
#include <windows.h>
#include "etc.h"

//---------------------------------------------------------------------
//[�T�v]	C�������Unicode������ɕϊ�
//[����]	�߂�l�́Afree()����K�v�L��

wchar_t* AsciiToUnicode(const char* psz)
{
	// �ϊ��ɕK�v�ȃo�b�t�@�T�C�Y(������(�܂�\0))���擾
	int nSizeNeeded = MultiByteToWideChar(CP_ACP, 0, psz, -1, NULL, 0);
	if (nSizeNeeded == 0)
		return NULL;

	// �o�b�t�@���m��
	wchar_t* pwsz = (wchar_t*)malloc(sizeof(wchar_t) * nSizeNeeded);
	if (pwsz == NULL)
		return NULL;

	// �ϊ�
	int ret = MultiByteToWideChar(CP_ACP, 0, psz, -1, pwsz, nSizeNeeded);
	if (ret == 0) {
		free(pwsz);
		return NULL;
	}

	return pwsz;
}

//---------------------------------------------------------------------
//[�T�v]	Unicode�������C������ɕϊ�
//[����]	�߂�l�́Afree()����K�v�L��

char* UnicodeToAscii(const wchar_t* pwsz)
{
	// �ϊ��ɕK�v�ȃo�b�t�@�T�C�Y(������(�܂�\0))���擾
	int nSizeNeeded = WideCharToMultiByte(CP_ACP, 0, pwsz, -1, NULL, 0, NULL, NULL);
	if (nSizeNeeded == 0)
		return NULL;

	// �o�b�t�@���m��
	char* psz = (char*)malloc(sizeof(char) * nSizeNeeded);
	if (psz == NULL)
		return NULL;

	// �ϊ�
	int ret = WideCharToMultiByte(CP_ACP, 0, pwsz, -1, psz, nSizeNeeded, NULL, NULL);
	if (ret == 0) {
		free(psz);
		return NULL;
	}

	return psz;
}
