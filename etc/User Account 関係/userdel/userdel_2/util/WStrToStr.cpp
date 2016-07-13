#include "util.h"

string WStrToStr(const wchar_t* pwsz)
{
	string str;

	// �ϊ��ɕK�v�ȃo�b�t�@�T�C�Y(������(\0�܂�))���擾
	int nSizeNeeded = WideCharToMultiByte(CP_ACP, 0, pwsz, -1, NULL, 0, NULL, NULL);
	if (nSizeNeeded == 0)
		return str;

	// �o�b�t�@���m��
	char* psz = new char[nSizeNeeded];
	if (psz == NULL)
		return str;

	// �ϊ�
	int ret = WideCharToMultiByte(CP_ACP, 0, pwsz, -1, psz, nSizeNeeded, NULL, NULL);
	if (ret == 0) {
		delete[] psz;
		return str;
	}

	str.assign(psz);
	delete[] psz;

	return str;
}

