// PapimoPassword.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	//00031607201106242330
	char yyyymmdd[] = "20110624";
	for (int i = 0, v = 40; i < 8; i++, v += 10) {
		printf("%c", (yyyymmdd[i] - '0') + v);
	}
	puts("00031607");

	return 0;
}

