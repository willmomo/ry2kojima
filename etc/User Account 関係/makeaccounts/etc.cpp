#include <windows.h>
#include "etc.h"
using namespace std;

//---------------------------------------------------------------------
//[�T�v]	string��wstring�ɕϊ�

wstring StringToWString(const string& str)
{
	wstring wstr;	// �߂蕶����
	wstr.empty();	// �܂���ɂ���

	// �ϊ��ɕK�v�ȃo�b�t�@�T�C�Y(������(�܂�\0))���擾
	int nSizeNeeded = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	if (nSizeNeeded == 0)
		return wstr;

	// �o�b�t�@���m��
	wchar_t* pwsz = (wchar_t*)malloc(sizeof(wchar_t) * nSizeNeeded);
	if (pwsz == NULL)
		return wstr;

	// �ϊ�
	int ret = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pwsz, nSizeNeeded);
	if (ret == 0) {
		free(pwsz);
		return wstr;
	}

	wstr.assign(pwsz);
	free(pwsz);

	return wstr;
}

//---------------------------------------------------------------------
//[�T�v]	wstring��string�ɕϊ�

string WStringToString(const wstring& wstr)
{
	string str;		// �߂蕶����
	str.empty();	// �܂���ɂ���

	// �ϊ��ɕK�v�ȃo�b�t�@�T�C�Y(������(�܂�\0))���擾
	int nSizeNeeded = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	if (nSizeNeeded == 0)
		return str;

	// �o�b�t�@���m��
	char* psz = (char*)malloc(sizeof(char) * nSizeNeeded);
	if (psz == NULL)
		return str;

	// �ϊ�
	int ret = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, psz, nSizeNeeded, NULL, NULL);
	if (ret == 0) {
		free(psz);
		return str;
	}

	str.assign(psz);
	free(psz);

	return str;
}

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

//---------------------------------------------------------------------
//[�T�v]	�����̉��s'\n'�̍폜

int chomp(char* psz)
{
	for (int end = strlen(psz) - 1, cnt = 0; end >= 0 && psz[end] == '\n'; --end, ++cnt)
		psz[end] = '\0';

	return cnt;
}

//---------------------------------------------------------------------
//[�T�v]	������̐^�񒆂�؂�o��

char* mid(const char* src, int start, int length, char* dst)
{
	char* p = dst;
	src += start;
	for (int i = 0; i < length; i++) {
		*(p++) = *(src++);
	}
	*p = '\0';

	return dst;
}
