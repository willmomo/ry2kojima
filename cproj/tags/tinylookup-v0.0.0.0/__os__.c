/*-----------------------------------------------------------------------------
	Visual Studio ���g���Ƃ��́A���̃\�[�X�t�@�C���ɁA�v�����R���p�C���ς�
	�w�b�_���g��Ȃ��悤�ɐݒ肵�Ă��������B
-----------------------------------------------------------------------------*/
/* version ����� GUID {E51B0ADA-ADCA-4a04-947A-8ED2E34C390C} */
/* --------------------------------------------------------------------
 * OS�Ɉˑ������R�[�h���u�����邽�߂̃\�[�X
 *
 * Windows��OS9000�����Ŏg�p�ł���R�[�h���������߂ɁA�ˑ��R�[�h���u��
 * ���邽�߂̃\�[�X�t�@�C���ł��B
 *
 * ��{�I�ɁA__os__�Ŏn�܂�֐����`���A����#if�ŏ��������܂��B
 *
 * Windows�ł��ɃR�[�f�B���O���邱�Ƃ������֌W��A�֐����́AWindows
 * �̂���ł��邱�Ƃ������Ȃ��Ă��܂��B
 *
 * -----------------------------------------------------------------
 * bmark , ftpclient
 * �Ŏg�p���Ă��܂��B
 * ----------------------------------------------------------------- */

#if defined(_WIN32)
#pragma warning(disable : 4996)
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#endif

#if defined(_OS9000)
#include <stddef.h>
#include <sysglob.h>
#include <types.h>
#endif

#include "__os__.h"

/* --------------------------------------------------------------------
 * OS�̃`�b�N�J�E���g��1ms�P�ʂŕԂ��B
 * ----------------------------------------------------------------- */
#if defined(_OS9000)
unsigned long GetTickCount() {
	unsigned long d_ticks;
	_os_getsys(offsetof(sysglobs, d_ticks), 4, (glob_buff *)&d_ticks);

	return(d_ticks * 10);
}
#elif defined(__GNUC__)
unsigned long GetTickCount() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
#endif

/* --------------------------------------------------------------------
 * syslog �ɏ��𑗐M����֐�
 * _OS9000�p�̃R�[�h�͂���܂���B
 * ----------------------------------------------------------------- */
#if defined(_WIN32) || defined(__GNUC__)
void syslog(const char *fmt, ...) {
	/* �������Ȃ��X�^�u */
}
#endif

/* --------------------------------------------------------------------
 * Debug�pprintf�֐�
 * _OS9000�p�̃R�[�h�͂���܂���B
 * ----------------------------------------------------------------- */
#if defined(_WIN32) || defined(__GNUC__)
void DebugPrintf(int level, const char *fmt, ...) {
	va_list arg;

	va_start(arg, fmt);
	vprintf(fmt, arg);
	va_end(arg);
}
#endif

/* --------------------------------------------------------------------
 * ��M�ς݃f�[�^����Ԃ��֐�
 * _OS9000�p�̃R�[�h�͂���܂���B
 * ----------------------------------------------------------------- */
#if defined(_WIN32) || defined(__GNUC__)
int _gs_rdy(int s) {
	int ret = 0;
	char buf[8192];

	ret = recv(s, &buf[0], 8192, MSG_PEEK);
	return ret;
}
#endif
