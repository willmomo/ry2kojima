// getopt.h�����ɍ��킹�ăC���N���[�h����

#if !defined(___GETOPT_H)
#define ___GETOPT_H

// VC++�̎�
// ���[�J���ŗp�ӂ���getopt.h���C���N���[�h����
// ���̍ہA__STDC__������`�̏ꍇ�͈ꎞ�I�ɒ�`���Ă���C���N���[�h����
#if defined(_MSC_VER)
#if defined(__STDC__)
#include "getopt.h"
#else
#define __STDC__	1
#include "getopt.h"
#undef __STDC__
#endif

// ���̑�(CYGWIN, LINUX)
// �����n�W����getopt.h���C���N���[�h����
#else
#include <getopt.h>

#endif

#endif
