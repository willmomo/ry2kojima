/*-----------------------------------------------------------------------------
	Visual Studio ���g���Ƃ��́A���̃\�[�X�t�@�C���ɁA�v�����R���p�C���ς�
	�w�b�_���g��Ȃ��悤�ɐݒ肵�Ă��������B
-----------------------------------------------------------------------------*/

#if defined(_WIN32)
#pragma warning(disable : 4996)		/* OS9�Ōx���ɂȂ�̂ł����� */
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <mbstring.h>				/* OS9�ŃG���[�ɂȂ�̂ł����� */
#include "resource.h"
#endif

#if defined(_OS9000)
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <modes.h>
#include <UNIX\os9def.h>

#define _mbstok	strtok				/* OS9�ł́A�����������ƈ����Ȃ��\�����o�܂� */

#define MAX_PATH	MAXPATHLEN
#endif

#include <stdio.h>
#include <assert.h>
#include "__os__.h"
#include "ftp_cmd.h"

#define FTP_MAX_RETRY	3
#define FTP_RETRY_WAIT	0

/*-----------------------------------------------------------------------------
// put �R�}���h����
// @return  0 ����I��
// @return -1 �ڑ��G���[
// @return -2 TYPE �R�}���h�G���[
// @return -3 PORT �R�}���h�G���[
// @return -4 STOR �R�}���h�G���[
// @return -5 �f�[�^�|�[�g�̐ڑ��G���[
// @return -6 PASV �R�}���h�G���[
// @return -7 �f�[�^�|�[�g�ւ̐ڑ��G���[
// @return -8 �f�[�^���M�G���[
// @return -9 �t�@�C���I�[�v���G���[
-----------------------------------------------------------------------------*/
int ftp_put_file_one(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char *local_name) {
	FTP_CTRL ftpCtrl;
	FILE* fp;
	int result = 0;

	/* FTP�p�[���N���X�̍\�z */
	ftp_construct(&ftpCtrl);

	printf("ftp_put_file_one(%s, %s, %s, %c, %d, %s, %s)\n", serv, user, pass, mode, pasv, retr, local_name);

	/* FTP �̐ڑ��V�[�P���X�����{ */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* TYPE command �̑���M */
	if (ftp_command_TYPE(&ftpCtrl, mode) != 0) {
		result = -2;
		goto quit_command;
	}

	if (pasv == FTP_USE_ACTIVE) {
		/* ACTIVE ���[�h�̓��� */
		/* �f�[�^�R�l�N�V�����p�̃\�P�b�g���쐬���� bind , listen ���Ă��� */
		ftp_listen_data_waiting_port(&ftpCtrl);

		/* PORT command �̑���M */
		if (ftp_command_PORT(&ftpCtrl) != 0) {
			result = -3;
			goto quit_command;
		}

		/* STOR command �̑���M */
		if (ftp_command_STOR(&ftpCtrl, retr) != 0) {
			result = -4;
			goto quit_command;
		}

		/* data waiting socket �Őڑ���҂� */
		if (ftp_data_accept(&ftpCtrl) != 0) {
			result = -5;
			goto quit_command;
		}
	} else {
		/* PASSIVE���[�h�̓��� */
		/* PASIV command �̑���M */
		if (ftp_command_PASV(&ftpCtrl) != 0) {
			result = -6;
			goto quit_command;
		}

		if (ftp_data_connect(&ftpCtrl) != 0) {
			result = -7;
			goto quit_command;
		}

		/* STOR command �̑���M */
		if (ftp_command_STOR(&ftpCtrl, retr) != 0) {
			/* ���̏�ԂŁAgracefully close ���s���ƁA
			   �������߂��Ă��Ȃ��̂ŁAshutdown ���āA�������ƃN���[�Y���Ă��܂� */
			ftp_close_data_socket(&ftpCtrl);

			result = -4;
			goto quit_command;
		}
	}

	/* command socket �Ŏ�M��҂��Adata socket �Ńt�@�C���̓��e�𑗐M���� */
	fp = fopen(local_name, "rb");
	if (fp) {
		if (ftp_data_send(&ftpCtrl, fp) != 0) {
			result = -8;
		}
		fclose(fp);
	} else {
		printf("�t�@�C�� '%s' ���I�[�v���ł��܂���B\n", local_name);
		result = -9;
	}


quit_command:

	/* QUIT command �̑���M(���X�|���X�ɑ΂��ẮA�������Ȃ�) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp�p�[���N���X�̔j�� */
	ftp_destroy(&ftpCtrl);

	return result;
}

/*-----------------------------------------------------------------------------
// put �R�}���h����
// @return  0 ����I��
// @return -1 �ڑ��G���[
// @return -2 TYPE �R�}���h�G���[
// @return -3 PORT �R�}���h�G���[
// @return -4 STOR �R�}���h�G���[
// @return -5 �f�[�^�|�[�g�̐ڑ��G���[
// @return -6 PASV �R�}���h�G���[
// @return -7 �f�[�^�|�[�g�ւ̐ڑ��G���[
// @return -8 �f�[�^���M�G���[
// @return -9 �t�@�C���I�[�v���G���[
-----------------------------------------------------------------------------*/
int ftp_put_buffer_one(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char *pBuf, size_t cbSize) {
	FTP_CTRL ftpCtrl;
	int result = 0;

	/* FTP�p�[���N���X�̍\�z */
	ftp_construct(&ftpCtrl);

	printf("ftp_put_buffer_one(%s, %s, %s, %c, %d, %s, %p, %u)\n", serv, user, pass, mode, pasv, retr, pBuf, cbSize);

	/* FTP �̐ڑ��V�[�P���X�����{ */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* TYPE command �̑���M */
	if (ftp_command_TYPE(&ftpCtrl, mode) != 0) {
		result = -2;
		goto quit_command;
	}

	if (pasv == FTP_USE_ACTIVE) {
		/* ACTIVE ���[�h�̓��� */
		/* �f�[�^�R�l�N�V�����p�̃\�P�b�g���쐬���� bind , listen ���Ă��� */
		ftp_listen_data_waiting_port(&ftpCtrl);

		/* PORT command �̑���M */
		if (ftp_command_PORT(&ftpCtrl) != 0) {
			result = -3;
			goto quit_command;
		}

		/* STOR command �̑���M */
		if (ftp_command_STOR(&ftpCtrl, retr) != 0) {
			result = -4;
			goto quit_command;
		}

		/* data waiting socket �Őڑ���҂� */
		if (ftp_data_accept(&ftpCtrl) != 0) {
			result = -5;
			goto quit_command;
		}
	} else {
		/* PASSIVE���[�h�̓��� */
		/* PASIV command �̑���M */
		if (ftp_command_PASV(&ftpCtrl) != 0) {
			result = -6;
			goto quit_command;
		}

		if (ftp_data_connect(&ftpCtrl) != 0) {
			result = -7;
			goto quit_command;
		}

		/* STOR command �̑���M */
		if (ftp_command_STOR(&ftpCtrl, retr) != 0) {
			/* ���̏�ԂŁAgracefully close ���s���ƁA
			   �������߂��Ă��Ȃ��̂ŁAshutdown ���āA�������ƃN���[�Y���Ă��܂� */
			ftp_close_data_socket(&ftpCtrl);

			result = -4;
			goto quit_command;
		}
	}

	/* command socket �Ŏ�M��҂��Adata socket �Ńt�@�C���̓��e�𑗐M���� */
	if (ftp_data_send_buffer(&ftpCtrl, pBuf, cbSize) != 0) {
		result = -8;
	}

quit_command:

	/* QUIT command �̑���M(���X�|���X�ɑ΂��ẮA�������Ȃ�) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp�p�[���N���X�̔j�� */
	ftp_destroy(&ftpCtrl);

	return result;
}

/*-----------------------------------------------------------------------------
// put �R�}���h����
// @returns         �Ō�Ɏ��s���� ftp_get_file �֐��̖߂�l�B
-----------------------------------------------------------------------------*/
int ftp_put_file(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char *local_name) {
	int i;
	int ret;

	for (i = 0; i <= FTP_MAX_RETRY; i++) {
		ret = ftp_put_file_one(serv, user, pass, mode, pasv, retr, local_name);
		if (ret == 0) {
			break;
		}

		syslog("ftp_put_file(%s, %s, %s, %d, %d, %s, %s) try %d error.",
			serv, user, pass, mode, pasv, retr, local_name, (i + 1));
		DebugPrintf(5, "ftp_put_file(%s, %s, %s, %d, %d, %s, %s) try %d error.\n",
			serv, user, pass, mode, pasv, retr, local_name, (i + 1));

		if (FTP_RETRY_WAIT >= 10)	Sleep(FTP_RETRY_WAIT);
	}

	return ret;
}

/*-----------------------------------------------------------------------------
// put �R�}���h����
// @returns         �Ō�Ɏ��s���� ftp_get_file �֐��̖߂�l�B
-----------------------------------------------------------------------------*/
int ftp_put_buffer(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char *pBuf, size_t cbSize) {
	int i;
	int ret;

	for (i = 0; i <= FTP_MAX_RETRY; i++) {
		ret = ftp_put_buffer_one(serv, user, pass, mode, pasv, retr, pBuf, cbSize);
		if (ret == 0) {
			break;
		}

		if (FTP_RETRY_WAIT >= 10)	Sleep(FTP_RETRY_WAIT);
	}

	return ret;
}


/*-----------------------------------------------------------------------------
// get �R�}���h����
// @return -1 �ڑ��G���[
// @return -2 TYPE �R�}���h�G���[
// @return -3 PORT �R�}���h�G���[
// @return -4 RETR �R�}���h�G���[
// @return -5 �f�[�^�|�[�g�ڑ��҂��G���[
// @return -6 PASV �R�}���h�G���[
// @return -7 �f�[�^�|�[�g�ւ̐ڑ��G���[
// @return -8 �f�[�^��M�G���[
// @return -9 �t�@�C���I�[�v���G���[
-----------------------------------------------------------------------------*/
int ftp_get_file_one(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char *local_name) {
	FTP_CTRL ftpCtrl;
	FILE *fp = NULL;
	int result = 0;

	/* FTP�p�[���N���X�̍\�z */
	ftp_construct(&ftpCtrl);

	printf("ftp_get_file_one(%s, %s, %s, %c, %d, %s, %s)\n", serv, user, pass, mode, pasv, retr, local_name);

	/* FTP �̐ڑ��V�[�P���X�����{ */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* TYPE command �̑���M */
	if (ftp_command_TYPE(&ftpCtrl, mode) != 0) {
		result = -2;
		goto quit_command;
	}

	/* ���[�J���̃t�@�C������������B
	   �t�@�C���̏������ł��Ȃ��Ƃ��́A���X��QUIT�ŏI�� */
	if ((fp = fopen(local_name, "wb")) == NULL) {
		printf("�t�@�C�� '%s' ���I�[�v���ł��܂���B\n", local_name);
		result = -9;
		goto quit_command;
	}

	if (pasv == FTP_USE_ACTIVE) {
		/* ACTIVE ���[�h�̓��� */
		/* �f�[�^�R�l�N�V�����p�̃\�P�b�g���쐬���� bind , listen ���Ă��� */
		ftp_listen_data_waiting_port(&ftpCtrl);

		/* PORT command �̑���M */
		if (ftp_command_PORT(&ftpCtrl) != 0) {
			result = -3;
			goto quit_command;
		}

		/* RETR command �̑���M */
		if (ftp_command_RETR(&ftpCtrl, retr) != 0) {
			result = -4;
			goto quit_command;
		}

		/* data waiting socket �Őڑ���҂� */
		if (ftp_data_accept(&ftpCtrl) != 0) {
			result = -5;
			goto quit_command;
		}
	} else {
		/* PASSIVE���[�h�̓��� */
		/* PASIV command �̑���M */
		if (ftp_command_PASV(&ftpCtrl) != 0) {
			result = -6;
			goto quit_command;
		}

		if (ftp_data_connect(&ftpCtrl) != 0) {
			result = -7;
			goto quit_command;
		}

		/* RETR command �̑���M */
		if (ftp_command_RETR(&ftpCtrl, retr) != 0) {
			/* ���̏�ԂŁAgracefully close ���s���ƁA
			   �������߂��Ă��Ȃ��̂ŁAshutdown ���āA�������ƃN���[�Y���Ă��܂� */
			ftp_close_data_socket(&ftpCtrl);

			result = -4;
			goto quit_command;
		}
	}

	/* command socket �Ŏ�M��҂��Adata socket �Ŏ�M�f�[�^���t�@�C���ɕۑ� */
	if (ftp_data_recv(&ftpCtrl, fp) != 0) {
		result = -8;
	}

quit_command:
	if (fp) fclose(fp);

	/* QUIT command �̑���M(���X�|���X�ɑ΂��ẮA�������Ȃ�) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp�p�[���N���X�̔j�� */
	ftp_destroy(&ftpCtrl);

	/* ����I���ȊO�́A�쐬���Ă��܂�����������Ȃ��t�@�C���̍폜�����݂� */
	if (result != 0) {
		remove(local_name);
	}

	return result;
}

/*-----------------------------------------------------------------------------
// get �R�}���h����
// @returns         �Ō�Ɏ��s���� ftp_get_file �֐��̖߂�l�B
-----------------------------------------------------------------------------*/
int ftp_get_file(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char *local_name) {
	int i;
	int ret;

	for (i = 0; i <= FTP_MAX_RETRY; i++) {
		ret = ftp_get_file_one(serv, user, pass, mode, pasv, retr, local_name);
		if (ret == 0) {
			break;
		}

		syslog("ftp_get_file(%s, %s, %s, %d, %d, %s, %s) try %d error.",
			serv, user, pass, mode, pasv, retr, local_name, (i + 1));
		DebugPrintf(5, "ftp_get_file(%s, %s, %s, %d, %d, %s, %s) try %d error.\n",
			serv, user, pass, mode, pasv, retr, local_name, (i + 1));

		if (FTP_RETRY_WAIT >= 10)	Sleep(FTP_RETRY_WAIT);
	}

	return ret;
}

/*-----------------------------------------------------------------------------
// get �R�}���h����
// @return >=0 ��M�T�C�Y�BbufSize�Ɠ����Ƃ��́A�o�b�t�@�̃T�C�Y������Ȃ��\������B
// @return -1 �ڑ��G���[
// @return -2 TYPE �R�}���h�G���[
// @return -3 PORT �R�}���h�G���[
// @return -4 RETR �R�}���h�G���[
// @return -5 �f�[�^�|�[�g�ڑ��҂��G���[
// @return -6 PASV �R�}���h�G���[
// @return -7 �f�[�^�|�[�g�ւ̐ڑ��G���[
// @return -8 �f�[�^��M�G���[
-----------------------------------------------------------------------------*/
int ftp_get_file2(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, char* pBuf, int bufSize) {
	FTP_CTRL ftpCtrl;
	int result = 0;

	/* FTP�p�[���N���X�̍\�z */
	ftp_construct(&ftpCtrl);

	printf("ftp_get_file2(%s, %s, %s, %c, %d, %s, %p, %d)\n", serv, user, pass, mode, pasv, retr, pBuf, bufSize);

	/* FTP �̐ڑ��V�[�P���X�����{ */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* TYPE command �̑���M */
	if (ftp_command_TYPE(&ftpCtrl, mode) != 0) {
		result = -2;
		goto quit_command;
	}

	if (pasv == FTP_USE_ACTIVE) {
		/* ACTIVE ���[�h�̓��� */
		/* �f�[�^�R�l�N�V�����p�̃\�P�b�g���쐬���� bind , listen ���Ă��� */
		ftp_listen_data_waiting_port(&ftpCtrl);

		/* PORT command �̑���M */
		if (ftp_command_PORT(&ftpCtrl) != 0) {
			result = -3;
			goto quit_command;
		}

		/* RETR command �̑���M */
		if (ftp_command_RETR(&ftpCtrl, retr) != 0) {
			result = -4;
			goto quit_command;
		}

		/* data waiting socket �Őڑ���҂� */
		if (ftp_data_accept(&ftpCtrl) != 0) {
			result = -5;
			goto quit_command;
		}
	} else {
		/* PASSIVE���[�h�̓��� */
		/* PASIV command �̑���M */
		if (ftp_command_PASV(&ftpCtrl) != 0) {
			result = -6;
			goto quit_command;
		}

		if (ftp_data_connect(&ftpCtrl) != 0) {
			result = -7;
			goto quit_command;
		}

		/* RETR command �̑���M */
		if (ftp_command_RETR(&ftpCtrl, retr) != 0) {
			/* ���̏�ԂŁAgracefully close ���s���ƁA
			   �������߂��Ă��Ȃ��̂ŁAshutdown ���āA�������ƃN���[�Y���Ă��܂� */
			ftp_close_data_socket(&ftpCtrl);

			result = -4;
			goto quit_command;
		}
	}

	/* command socket �Ŏ�M��҂��Adata socket �Ŏ�M�f�[�^���t�@�C���ɕۑ� */
	result = ftp_data_recv2(&ftpCtrl, pBuf, bufSize);
	if (result < 0) {
		result = -8;
	}

quit_command:

	/* QUIT command �̑���M(���X�|���X�ɑ΂��ẮA�������Ȃ�) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp�p�[���N���X�̔j�� */
	ftp_destroy(&ftpCtrl);

	return result;
}

/*---------------------------------------------------------------------
	dir ����
---------------------------------------------------------------------*/
int ftp_list_file(const char *serv, const char *user, const char *pass, int pasv, const char* retr) {
	FTP_CTRL ftpCtrl;

	/* FTP�p�[���N���X�̍\�z */
	ftp_construct(&ftpCtrl);

	printf("ftp_list_file(%s, %s, %s, %d, %s)\n", serv, user, pass, pasv, retr);

	/* FTP �̐ڑ��V�[�P���X�����{ */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* TYPE command �̑���M(ASCII�Œ�) */
	if (ftp_command_TYPE(&ftpCtrl, FTP_MODE_ASCII) != 0) {
		goto quit_command;
	}

	if (pasv == FTP_USE_ACTIVE) {
		/* ACTIVE ���[�h�̓��� */
		/* �f�[�^�R�l�N�V�����p�̃\�P�b�g���쐬���� bind , listen ���Ă��� */
		ftp_listen_data_waiting_port(&ftpCtrl);

		/* PORT command �̑���M */
		if (ftp_command_PORT(&ftpCtrl) != 0) {
			goto quit_command;
		}

		/* LIST command �̑���M */
		if (ftp_command_LIST(&ftpCtrl, retr) != 0) {
			goto quit_command;
		}

		/* data waiting socket �Őڑ���҂� */
		if (ftp_data_accept(&ftpCtrl) != 0) {
			goto quit_command;
		}
	} else {
		/* PASSIVE���[�h�̓��� */
		/* PASIV command �̑���M */
		if (ftp_command_PASV(&ftpCtrl) != 0) {
			goto quit_command;
		}

		if (ftp_data_connect(&ftpCtrl) != 0) {
			goto quit_command;
		}

		/* LIST command �̑���M */
		if (ftp_command_LIST(&ftpCtrl, retr) != 0) {
			/* ���̏�ԂŁAgracefully close ���s���ƁA
			   �������߂��Ă��Ȃ��̂ŁAshutdown ���āA�������ƃN���[�Y���Ă��܂� */
			ftp_close_data_socket(&ftpCtrl);

			goto quit_command;
		}
	}

	/* command socket �Ŏ�M��҂��Adata socket �Ŏ�M�f�[�^����ʂɕ\�� */
	ftp_data_recv(&ftpCtrl, stdout);

quit_command:

	/* QUIT command �̑���M(���X�|���X�ɑ΂��ẮA�������Ȃ�) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp�p�[���N���X�̔j�� */
	ftp_destroy(&ftpCtrl);

	return 0;
}

/*---------------------------------------------------------------------
	�f�B���N�g�����X�g���擾����(�Ή�����R�}���h�Ȃ�)
---------------------------------------------------------------------*/
int ftp_nlst_file(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr) {
	FTP_CTRL ftpCtrl;

	/* FTP�p�[���N���X�̍\�z */
	ftp_construct(&ftpCtrl);

	printf("ftp_nlst_file(%s, %s, %s, %c, %d, %s)\n", serv, user, pass, mode, pasv, retr);

	/* FTP �̐ڑ��V�[�P���X�����{ */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* TYPE command �̑���M */
	if (ftp_command_TYPE(&ftpCtrl, mode) != 0) {
		goto quit_command;
	}

	if (pasv == FTP_USE_ACTIVE) {
		/* ACTIVE ���[�h�̓��� */
		/* �f�[�^�R�l�N�V�����p�̃\�P�b�g���쐬���� bind , listen ���Ă��� */
		ftp_listen_data_waiting_port(&ftpCtrl);

		/* PORT command �̑���M */
		if (ftp_command_PORT(&ftpCtrl) != 0) {
			goto quit_command;
		}

		/* NLST command �̑���M */
		if (ftp_command_NLST(&ftpCtrl, retr) != 0) {
			goto quit_command;
		}

		/* data waiting socket �Őڑ���҂� */
		if (ftp_data_accept(&ftpCtrl) != 0) {
			goto quit_command;
		}
	} else {
		/* PASSIVE���[�h�̓��� */
		/* PASIV command �̑���M */
		if (ftp_command_PASV(&ftpCtrl) != 0) {
			goto quit_command;
		}

		if (ftp_data_connect(&ftpCtrl) != 0) {
			goto quit_command;
		}

		/* NLST command �̑���M */
		if (ftp_command_NLST(&ftpCtrl, retr) != 0) {
			/* ���̏�ԂŁAgracefully close ���s���ƁA
			   �������߂��Ă��Ȃ��̂ŁAshutdown ���āA�������ƃN���[�Y���Ă��܂� */
			ftp_close_data_socket(&ftpCtrl);

			goto quit_command;
		}
	}

	/* command socket �Ŏ�M��҂��Adata socket �Ŏ�M�f�[�^����ʂɕ\�� */
	ftp_data_recv(&ftpCtrl, stdout);

quit_command:

	/* QUIT command �̑���M(���X�|���X�ɑ΂��ẮA�������Ȃ�) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp�p�[���N���X�̔j�� */
	ftp_destroy(&ftpCtrl);

	return 0;
}


/*-----------------------------------------------------------------------------
// �f�B���N�g�����X�g���擾����(�Ή�����R�}���h�Ȃ�)
// @return  0 ����I��
// @return -1 �ڑ��G���[
// @return -2 TYPE �R�}���h�G���[
// @return -3 PORT �R�}���h�G���[
// @return -4 NLST �R�}���h�G���[
// @return -5 �f�[�^�|�[�g�ڑ��҂��G���[
// @return -6 PASV �R�}���h�G���[
// @return -7 �f�[�^�|�[�g�ւ̐ڑ��G���[
// @return -8 �f�[�^��M�G���[
// @return -9 �t�@�C���I�[�v���G���[
-----------------------------------------------------------------------------*/
int ftp_nlst_file2_one(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char* outputFile) {
	FTP_CTRL ftpCtrl;
	FILE* fp;
	int result = 0;

	/* FTP�p�[���N���X�̍\�z */
	ftp_construct(&ftpCtrl);

	printf("ftp_nlst_file2_one(%s, %s, %s, %c, %d, %s, %s)\n", serv, user, pass, mode, pasv, retr, outputFile);

	/* FTP �̐ڑ��V�[�P���X�����{ */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* TYPE command �̑���M */
	if (ftp_command_TYPE(&ftpCtrl, mode) != 0) {
		result = -2;
		goto quit_command;
	}

	if (pasv == FTP_USE_ACTIVE) {
		/* ACTIVE ���[�h�̓��� */
		/* �f�[�^�R�l�N�V�����p�̃\�P�b�g���쐬���� bind , listen ���Ă��� */
		ftp_listen_data_waiting_port(&ftpCtrl);

		/* PORT command �̑���M */
		if (ftp_command_PORT(&ftpCtrl) != 0) {
			result = -3;
			goto quit_command;
		}

		/* NLST command �̑���M */
		if (ftp_command_NLST(&ftpCtrl, retr) != 0) {
			result = -4;
			goto quit_command;
		}

		/* data waiting socket �Őڑ���҂� */
		if (ftp_data_accept(&ftpCtrl) != 0) {
			result = -5;
			goto quit_command;
		}
	} else {
		/* PASSIVE���[�h�̓��� */
		/* PASIV command �̑���M */
		if (ftp_command_PASV(&ftpCtrl) != 0) {
			result = -6;
			goto quit_command;
		}

		if (ftp_data_connect(&ftpCtrl) != 0) {
			result = -7;
			goto quit_command;
		}

		/* NLST command �̑���M */
		if (ftp_command_NLST(&ftpCtrl, retr) != 0) {
			/* ���̏�ԂŁAgracefully close ���s���ƁA
			   �������߂��Ă��Ȃ��̂ŁAshutdown ���āA�������ƃN���[�Y���Ă��܂� */
			ftp_close_data_socket(&ftpCtrl);

			result = -4;
			goto quit_command;
		}
	}

	/* command socket �Ŏ�M��҂��Adata socket �Ŏ�M�f�[�^����ʂɕ\�� */
	fp = fopen(outputFile, "wb");
	if (fp != NULL) {
		if (ftp_data_recv(&ftpCtrl, fp) != 0) {
			result = -8;
		}
		fclose(fp);
	} else {
		printf("%s ���I�[�v���ł��܂���B\n", outputFile);
		result = -9;
	}

quit_command:

	/* QUIT command �̑���M(���X�|���X�ɑ΂��ẮA�������Ȃ�) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp�p�[���N���X�̔j�� */
	ftp_destroy(&ftpCtrl);

	/* ����I���ȊO�́A�쐬���Ă��܂�����������Ȃ��t�@�C���̍폜�����݂� */
	if (result != 0) {
		remove(outputFile);
	}

	return result;
}


/*-----------------------------------------------------------------------------
// �f�B���N�g�����X�g���擾����(�Ή�����R�}���h�Ȃ�)
// @returns         �Ō�Ɏ��s���� ftp_nlst_file2 �֐��̖߂�l�B
-----------------------------------------------------------------------------*/
int ftp_nlst_file2(const char *serv, const char *user, const char *pass, int mode, int pasv, const char* retr, const char* outputFile) {
	int i;
	int ret;

	for (i = 0; i <= FTP_MAX_RETRY; i++) {
		ret = ftp_nlst_file2_one(serv, user, pass, mode, pasv, retr, outputFile);
		if (ret == 0) {
			break;
		}

		syslog("ftp_nlst_file2(%s, %s, %s, %d, %d, %s, %s) try %d error.",
			serv, user, pass, mode, pasv, retr, outputFile, (i + 1));
		DebugPrintf(5, "ftp_nlst_file2(%s, %s, %s, %d, %d, %s, %s) try %d error.\n",
			serv, user, pass, mode, pasv, retr, outputFile, (i + 1));

		if (FTP_RETRY_WAIT >= 10)	Sleep(FTP_RETRY_WAIT);
	}

	return ret;
}

/*-----------------------------------------------------------------------------
// del �R�}���h����
// @return  0 ����I��
// @return -1 �ڑ��G���[
// @return -2 dele �R�}���h�G���[
-----------------------------------------------------------------------------*/
int ftp_dele_file(const char *serv, const char *user, const char *pass, const char* remote_filename) {
	FTP_CTRL ftpCtrl;
	int result = 0;

	/* ftp�p�[���N���X�̍\�z */
	ftp_construct(&ftpCtrl);

	printf("ftp_dele_file(%s, %s, %s, %s)\n", serv, user, pass, remote_filename);

	/* FTP �T�[�o�[�ɐڑ� */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* DELE command �̑���M(���ʂɂ�����炸QUIT�����Ɉڍs) */
	if (ftp_command_DELE(&ftpCtrl, remote_filename) != 0) {
		result = -2;
	}

	/* QUIT command �̑���M(���X�|���X�ɑ΂��ẮA�������Ȃ�) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp�p�[���N���X�̔j�� */
	ftp_destroy(&ftpCtrl);

	return result;
}


/*-----------------------------------------------------------------------------
// �t�@�C���̍X�V���t�擾
// @return  0 ����I��
// @return -1 �ڑ��G���[
// @return -2 dele �R�}���h�G���[
-----------------------------------------------------------------------------*/
int ftp_mdtm_file(const char *serv, const char *user, const char *pass, const char* remote_filename, char* response) {
	FTP_CTRL ftpCtrl;
	int result = 0;

	/* ftp�p�[���N���X�̍\�z */
	ftp_construct(&ftpCtrl);

	printf("ftp_mdtm_file(%s, %s, %s, %s, %s)\n", serv, user, pass, remote_filename, response);

	/* FTP �T�[�o�[�ɐڑ� */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* MDTM command �̑���M(���ʂɂ�����炸QUIT�����Ɉڍs) */
	if (ftp_command_MDTM(&ftpCtrl, remote_filename, response) != 0) {
		result = -2;
	}

	/* QUIT command �̑���M(���X�|���X�ɑ΂��ẮA�������Ȃ�) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp�p�[���N���X�̔j�� */
	ftp_destroy(&ftpCtrl);

	return result;
}


/*-----------------------------------------------------------------------------
// xmkd �R�}���h����
// @return  0 ����I��
// @return -1 �ڑ��G���[
// @return -2 xmkd �R�}���h�G���[
//
// �K�v�ɉ����ăp�X�̒��ԃf�B���N�g�����쐬���܂��B
-----------------------------------------------------------------------------*/
int ftp_xmkd_file_one(const char *serv, const char *user, const char *pass, const char* remote_filename) {
	FTP_CTRL ftpCtrl;
	int result = 0;
	char make_path[MAX_PATH];	/* ���ۂɍ쐬����f�B���N�g�� */
	char work[MAX_PATH];
	char* p = NULL;				/* �t�H���_���������邽�߂̃p�X */

	/* ftp�p�[���N���X�̍\�z */
	ftp_construct(&ftpCtrl);

	printf("ftp_xmkd_file_one(%s, %s, %s, %s)\n", serv, user, pass, remote_filename);

	/* FTP �T�[�o�[�ɐڑ� */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* ��ƃo�b�t�@�ɃR�s�[ */
	strcpy(work, remote_filename);

	/* �w��p�X����΃p�X�̏ꍇ�́A�擪�� / ����ꂽ��Ԃŏ������J�n */
	if (work[0] == '/') {
		strcpy(make_path, "/");
		p = &work[1];
	} else {
		strcpy(make_path, "");
		p = &work[0];
	}

	p = _mbstok(p, "/");
	while (p) {
		strcat(make_path, p);

		/* �Ō�� XMKD command �̐��ۂ�Ԃ��悤�ɂ��� */
		result = 0;
		if (ftp_command_XMKD(&ftpCtrl, make_path) != 0) {
			result = -2;
		}

		p = _mbstok(NULL, "/");
		strcat(make_path, "/");
	}

	/* QUIT command �̑���M(���X�|���X�ɑ΂��ẮA�������Ȃ�) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp�p�[���N���X�̔j�� */
	ftp_destroy(&ftpCtrl);

	return result;
}

/*-----------------------------------------------------------------------------
// xmkd �R�}���h����
// @returns         �Ō�Ɏ��s���� ftp_get_file �֐��̖߂�l�B
-----------------------------------------------------------------------------*/
int ftp_xmkd_file(const char *serv, const char *user, const char *pass, const char* remote_filename) {
	int i;
	int ret;

	for (i = 0; i <= FTP_MAX_RETRY; i++) {
		ret = ftp_xmkd_file_one(serv, user, pass, remote_filename);

		printf("ftp_xmkd_file: ret(%d) recv_status(%d)\n", ret, ftp_get_recv_status());

		if (ret == 0 || (ret != 0 && ftp_get_recv_status() != -2)) {
			break;
		}

		syslog("ftp_xmkd_file(%s, %s, %s, %s) try %d error.",
			serv, user, pass, remote_filename, (i + 1));
		DebugPrintf(5, "ftp_xmkd_file(%s, %s, %s, %s) try %d error.\n",
			serv, user, pass, remote_filename, (i + 1));

		if (FTP_RETRY_WAIT >= 10)	Sleep(FTP_RETRY_WAIT);
	}

	return ret;
}

/*-----------------------------------------------------------------------------
// xrmd �R�}���h����
// @return  0 ����I��
// @return -1 �ڑ��G���[
// @return -2 xmkd �R�}���h�G���[
-----------------------------------------------------------------------------*/
int ftp_xrmd_file(const char *serv, const char *user, const char *pass, const char* remote_filename) {
	FTP_CTRL ftpCtrl;
	int result = 0;

	/* ftp�p�[���N���X�̍\�z */
	ftp_construct(&ftpCtrl);

	printf("ftp_xrmd_file(%s, %s, %s, %s)\n", serv, user, pass, remote_filename);

	/* FTP �T�[�o�[�ɐڑ� */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* XRMD command �̑���M(���ʂɂ�����炸QUIT�����Ɉڍs) */
	if (ftp_command_XRMD(&ftpCtrl, remote_filename) != 0) {
		result = -2;
	}

	/* QUIT command �̑���M(���X�|���X�ɑ΂��ẮA�������Ȃ�) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp�p�[���N���X�̔j�� */
	ftp_destroy(&ftpCtrl);

	return result;
}

/*-----------------------------------------------------------------------------
// RNFR + RNTO �R�}���h����
// @return  0 ����I��
// @return -1 �ڑ��G���[
// @return -2 xmkd �R�}���h�G���[
-----------------------------------------------------------------------------*/
int ftp_rename_file(const char *serv, const char *user, const char *pass, const char* from_filename, const char* to_filename) {
	FTP_CTRL ftpCtrl;
	int result = 0;

	/* ftp�p�[���N���X�̍\�z */
	ftp_construct(&ftpCtrl);

	printf("ftp_rename_file(%s, %s, %s, %s, %s)\n", serv, user, pass, from_filename, to_filename);

	/* FTP �T�[�o�[�ɐڑ� */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* RNFR �𑗎�M */
	if (ftp_command_RNFR(&ftpCtrl, from_filename) != 0) {
		result = -2;
	}

	/* RNTO �𑗎�M */
	if (result == 0 && ftp_command_RNTO(&ftpCtrl, to_filename) != 0) {
		result = -3;
	}

	/* QUIT command �̑���M(���X�|���X�ɑ΂��ẮA�������Ȃ�) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp�p�[���N���X�̔j�� */
	ftp_destroy(&ftpCtrl);

	return result;
}

/*---------------------------------------------------------------------
	SIZE�R�}���h�Ńt�@�C���̃T�C�Y���擾
---------------------------------------------------------------------*/
int ftp_size_file(const char *serv, const char *user, const char *pass, const char* remote_filename) {
	FTP_CTRL ftpCtrl;
	int result = 0;

	/* ftp�p�[���N���X�̍\�z */
	ftp_construct(&ftpCtrl);

	printf("ftp_size_file(%s, %s, %s, %s)\n", serv, user, pass, remote_filename);

	/* FTP �T�[�o�[�ɐڑ� */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* SIZE command �̑���M(���ʂɂ�����炸QUIT�����Ɉڍs) */
	result = ftp_command_SIZE(&ftpCtrl, remote_filename);

	/* QUIT command �̑���M(���X�|���X�ɑ΂��ẮA�������Ȃ�) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp�p�[���N���X�̔j�� */
	ftp_destroy(&ftpCtrl);

	/* return 0; */
	return result;
}

/*---------------------------------------------------------------------
	HELP�R�}���h�ŃR�}���h���X�g���擾
---------------------------------------------------------------------*/
int ftp_help(const char *serv, const char *user, const char *pass, const char *cmd) {
	FTP_CTRL ftpCtrl;

	/* ftp�p�[���N���X�̍\�z */
	ftp_construct(&ftpCtrl);

	printf("ftp_help(%s, %s, %s, %s)\n", serv, user, pass, cmd);

	/* FTP �T�[�o�[�ɐڑ� */
	if (ftp_connect(&ftpCtrl, serv, user, pass) != 0) {
		return -1;
	}

	/* HELP command �̑���M(���ʂɂ�����炸QUIT�����Ɉڍs) */
	ftp_command_HELP(&ftpCtrl, cmd);

	/* QUIT command �̑���M(���X�|���X�ɑ΂��ẮA�������Ȃ�) */
	ftp_command_QUIT(&ftpCtrl);

	/* ftp�p�[���N���X�̔j�� */
	ftp_destroy(&ftpCtrl);

	return 0;
}

/*---------------------------------------------------------------------
	�v���O�����X�^�[�g
---------------------------------------------------------------------*/
#if defined(TEST)
int main(int argc, char** argv) {
	char* p = NULL;
	int size = 0;

#if defined(_WIN32)
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif

#if 1
	printf("�e�X�g�X�^�[�g\n");

	//ftp_nlst_file2("192.168.120.23", "sankoftp", "UvGyVM", FTP_MODE_ASCII, FTP_USE_ACTIVE, "/mnt/array1", "out.txt");
#if 0
	{
		FILE* fp;
		fp = fopen("out.txt", "r");
		if (fp) {
			char buf[512];

			while (fgets(buf, sizeof(buf), fp) != NULL) {
				int i;
				char remote[512];
				char local[512];

				buf[strlen(buf) - 1] = '\0';

				strcpy(remote, buf);
				strcat(strcpy(local, "."), buf);
				for (i = 0; local[i] != '\0'; i++) {
					if (local[i] == '/') {
						local[i] = '\\';
					}
				}

				ftp_get_file("192.168.150.2", "wssadmin", "wssadmin", FTP_MODE_ASCII, FTP_USE_ACTIVE, remote, local);
			}

			fclose(fp);
		}
	}
#endif
#endif
	if (argc != 9) {
		printf("%s <mdtm|get|get2|put|list|nlst|nlst2|dele|xmkd|xrmd|size|help>\n"
			"<�T�[�o�[> <���[�U�[> <�p�X���[�h> <A or I> <A or P> <�����[�g�t�@�C��> <���[�J���t�@�C��>\n", argv[0]);
		return 0;
	}

	/*
	*/

	if (strcmp(argv[1], "get") == 0) {
		ftp_get_file(
			argv[2],	/* �T�[�o�[ */
			argv[3],	/* ���[�U�[ */
			argv[4],	/* �p�X���[�h */
			argv[5][0],	/* 'A' or 'I' */
			(argv[6][0] == 'A' ? FTP_USE_ACTIVE : FTP_USE_PASSIVE),	/* 'A' or 'P' */
			argv[7],	/* �����[�g�t�@�C�� */
			argv[8]		/* ���[�J���t�@�C�� */
		);
	} else if (strcmp(argv[1], "get2") == 0) {
		char buf[8192];

		ftp_get_file2(
			argv[2],	/* �T�[�o�[ */
			argv[3],	/* ���[�U�[ */
			argv[4],	/* �p�X���[�h */
			argv[5][0],	/* 'A' or 'I' */
			(argv[6][0] == 'A' ? FTP_USE_ACTIVE : FTP_USE_PASSIVE),	/* 'A' or 'P' */
			argv[7],	/* �����[�g�t�@�C�� */
			buf,		/* �o�b�t�@�ւ̃|�C���^ */
			sizeof(buf)	/* �o�b�t�@�̃T�C�Y */
		);
	} else if (strcmp(argv[1], "put") == 0) {
		ftp_put_file(
			argv[2],	/* �T�[�o�[ */
			argv[3],	/* ���[�U�[ */
			argv[4],	/* �p�X���[�h */
			argv[5][0],	/* 'A' or 'I' */
			(argv[6][0] == 'A' ? FTP_USE_ACTIVE : FTP_USE_PASSIVE),	/* 'A' or 'P' */
			argv[7],	/* �����[�g�t�@�C�� */
			argv[8]		/* ���[�J���t�@�C�� */
		);
	} else if (strcmp(argv[1], "list") == 0) {
		ftp_list_file(
			argv[2],	/* �T�[�o�[ */
			argv[3],	/* ���[�U�[ */
			argv[4],	/* �p�X���[�h */
			(argv[6][0] == 'A' ? FTP_USE_ACTIVE : FTP_USE_PASSIVE),	/* 'A' or 'P' */
			argv[7]		/* �����[�g�t�@�C�� */
		);
	} else if (strcmp(argv[1], "nlst") == 0) {
		ftp_nlst_file(
			argv[2],	/* �T�[�o�[ */
			argv[3],	/* ���[�U�[ */
			argv[4],	/* �p�X���[�h */
			argv[5][0],	/* 'A' or 'I' */
			(argv[6][0] == 'A' ? FTP_USE_ACTIVE : FTP_USE_PASSIVE),	/* 'A' or 'P' */
			argv[7]		/* �����[�g�t�@�C�� */
		);
	} else if (strcmp(argv[1], "nlst2") == 0) {
		ftp_nlst_file2(
			argv[2],	/* �T�[�o�[ */
			argv[3],	/* ���[�U�[ */
			argv[4],	/* �p�X���[�h */
			argv[5][0],	/* 'A' or 'I' */
			(argv[6][0] == 'A' ? FTP_USE_ACTIVE : FTP_USE_PASSIVE),	/* 'A' or 'P' */
			argv[7],	/* �����[�g�t�@�C�� */
			argv[8]		/* ���[�J���t�@�C�� */
		);
	} else if (strcmp(argv[1], "dele") == 0) {
		ftp_dele_file(
			argv[2],	/* �T�[�o�[ */
			argv[3],	/* ���[�U�[ */
			argv[4],	/* �p�X���[�h */
			argv[7]		/* �����[�g�t�@�C�� */
		);
	} else if (strcmp(argv[1], "mdtm") == 0) {
		char response[80];
		ftp_mdtm_file(
			argv[2],	/* �T�[�o�[ */
			argv[3],	/* ���[�U�[ */
			argv[4],	/* �p�X���[�h */
			argv[7],		/* �����[�g�t�@�C�� */
			response
		);
	} else if (strcmp(argv[1], "xmkd") == 0) {
		ftp_xmkd_file(
			argv[2],	/* �T�[�o�[ */
			argv[3],	/* ���[�U�[ */
			argv[4],	/* �p�X���[�h */
			argv[7]		/* �����[�g�t�@�C�� */
		);
	} else if (strcmp(argv[1], "xrmd") == 0) {
		ftp_xrmd_file(
			argv[2],	/* �T�[�o�[ */
			argv[3],	/* ���[�U�[ */
			argv[4],	/* �p�X���[�h */
			argv[7]		/* �����[�g�t�@�C�� */
		);
	} else if (strcmp(argv[1], "size") == 0) {
		ftp_size_file(
			argv[2],	/* �T�[�o�[ */
			argv[3],	/* ���[�U�[ */
			argv[4],	/* �p�X���[�h */
			argv[7]		/* �����[�g�t�@�C�� */
		);
	} else if (strcmp(argv[1], "help") == 0) {
		ftp_help(
			argv[2],	/* �T�[�o�[ */
			argv[3],	/* ���[�U�[ */
			argv[4],	/* �p�X���[�h */
			argv[7]		/* �R�}���h�� */
		);
	}

#if defined(_WIN32)
	WSACleanup();
#endif
	return 0;
}
#endif

#if 0	/*toshi*/

#define CPL 16

void
hexdump(char *ptr, int len)
{
	int		col;
	long	i;
	char	c, chr[CPL+1], *buf = ptr;
	
	chr[CPL] = '\0';
	printf("ptr=0x%08X len=0x%08X(%d)\n", (unsigned long)ptr, len, len);
	for (i = 0; i < len; i++) {
		col = i % CPL;
		c = buf[i];
		
		if (col == 0)
			printf("%08X: ", i);
		printf("%02X ", (unsigned char)c);
		if (c >= 0x7f || c <= 0x1f)
			chr[col] = '.';
		else
			chr[col] = c;
		if (col == CPL - 1) {
			printf("[%s]\n", chr);
		}
	}
	printf("\n");
}

int main(int argc, char** argv)
{
	char buf[10];

	memset(buf, 0, sizeof(buf));

/*		ftp_put_file("192.168.130.113", "super", "user", 'I', FTP_USE_ACTIVE, "/r0/cc.bin", "/mhc1/cc.bin"); */
/*		ftp_get_file("192.168.130.113", "super", "user", 'I', FTP_USE_ACTIVE, "/r0/cc.bin", "/mhc1/dd.bin"); */
/*		ftp_get_file2("192.168.130.113", "super", "user", 'I', FTP_USE_ACTIVE, "/mhc1/aa.bin", buf, sizeof(buf));*/
/*		ftp_nlst_file2("192.168.130.113", "super", "user", 'I', FTP_USE_ACTIVE, "/r0", "/mhc1/dir.txt");*/
/*		ftp_dele_file("192.168.130.113", "super", "user", "/mhc1/dd.bin");*/
/*		ftp_xmkd_file("172.17.101.10", "wssadmin", "wssadmin", "aaa");*/
/*		ftp_xmkd_file("172.17.101.19", "admin", "password", "array1/backup/bbb");*/
/*		ftp_xrmd_file("172.17.101.10", "wssadmin", "wssadmin", "aaa");*/
/*		ftp_xrmd_file("192.168.130.113", "super", "user", "aaa");*/

/*	hexdump(buf, sizeof(buf));*/
}
#endif	/*toshi*/

