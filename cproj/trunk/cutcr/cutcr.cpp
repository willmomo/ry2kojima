/*
	�W�����͂������ CR �����ׂĎ�菜���B
	�W�����͂������ LF �����ׂ� CR LF �Ƃ��ĕW���o�͂ɓf���o���B
	���s�R�[�h��

	CR CR LF

	�ɁA�Ȃ��Ă��܂����t�@�C���� CR LF �ɒ������߂̃v���O�����B
*/

// �V���O���X���b�h����ŁAIO�����������ɂ���}�N��
#define _CRT_DISABLE_PERFCRIT_LOCKS

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include "resource.h"

//---------------------------------------------------------------------------
// �v���O�����X�^�[�g
//---------------------------------------------------------------------------
int main(int argc, char** argv) {

	if (argc != 1) {
		puts("cutcr < ���̓t�@�C�� > �o�̓t�@�C��");
		puts("���_�C���N�g�����A�󂯕t���܂���B");
		return -1;
	}

	_setmode(_fileno(stdin), _O_BINARY);

	int c;
	while ((c = getchar()) != EOF) {
		if (c != '\x0d') {
			putchar(c);
		}
	}

	return 0;
}
