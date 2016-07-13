// rk_cpp.cpp : cpp�Ŏg����֐��Ȃ�
//
// �֐��A�N���X�̐擪�͕K��rk�Ŏn�܂�B
// ��{�I�ɁAUnicode�͖���

#include "rk_cpp.h"

// ������I�[�̉��s(\r, \n)����菜��
//
// [�߂�l]
// 0 : ��菜���������Ȃ�
// 1 : \x0d or \x0a ����菜����
// 2 : \x0d \x0a ����菜����

int rkChomp(char *str)
{
	int chomp_count = 0;	// ��菜������
	int len;

	// �s����\n(\x0a)����菜��
	len = strlen(str);
	if (len > 0 && str[len - 1] == '\n') {
		str[len - 1] = '\0';
		++chomp_count;
	}

	// �s����\r(\x0d)����菜��
	len = strlen(str);
	if (len > 0 && str[len - 1] == '\r') {
		str[len - 1] = '\0';
		++chomp_count;
	}

	return chomp_count;
}

