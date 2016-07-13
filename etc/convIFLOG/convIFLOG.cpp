#include <windows.h>
#include <stdio.h>

//---------------------------------------------------------------------
// �O���[�o���ϐ�

char g_szFname[MAX_PATH];	// �ϊ�����t�@�C����

//---------------------------------------------------------------------
// [�T�v]
// �g�p�@�̕\���ƏI��
//
// [����]
// int status	: �I���X�e�[�^�X
//
// [�߂�l]
// �Ȃ�
//
// [����]
// �w���v��\�����ďI�����܂��B������exit�֐��ւ̈���
//
//---------------------------------------------------------------------

void usage(int status)
{
	puts("convIFLOG ver.2003.12.05 (r.kojima)");
	puts("");
	puts("�g�p�@�FconvIFLOG <�t�@�C����>");
	puts("");
	puts("���p��̍쐬���� 'IFLOG??.LOG'�A�T�C�Y <<<128>>> �̓d����");
	puts("CSV�`���ŏo�͂��܂��B");
	puts("�t�@�C���ɕۑ�����ꍇ�́A���_�C���N�g���Ă��������B");

	exit(status);
}

//---------------------------------------------------------------------
// [�T�v]
// �����`�F�b�N
//
// [����]
// int argc		: �����̐�
// char** argv	: �����̎���
//
// [�߂�l]
// �Ȃ�
//
// [����]
// �������s�K�؂ȏꍇ�́A�w���v��\�����ďI��
//
//---------------------------------------------------------------------

void chkarg(int argc, char** argv)
{
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '/' || argv[i][0] == '-') {
			if (argv[i][1] == '?' && argv[i][2] == '\0') {
				usage(EXIT_SUCCESS);	// -?�̎��́A�w���v��\�����Đ���I��
			} else {
				usage(EXIT_FAILURE);	// -?�ȊO�́A�w���v��\�����Ĉُ�I��
			}
		} else {
			if (g_szFname[0] != '\0') {
				usage(EXIT_FAILURE);	// ���łɃt�@�C���͎w�肳��Ă���?
			} else {
				strcpy(g_szFname, argv[i]);
			}
		}
	}

	if (g_szFname[0] == '\0') {
		usage(EXIT_FAILURE);	// �Ō�܂Ńt�@�C���������w��?
	}
}

//---------------------------------------------------------------------
// [�T�v]
// �s���̉��s���폜
//
// [����]
// char* buf	: ������
//
// [�߂�l]
// �Ȃ�
//
// [����]
// �s���̉��s '\x0d' or '\x0a' ���Ȃ��Ȃ�܂ō폜����B
//
//---------------------------------------------------------------------

void chomp(char* buf)
{
	for (int n = strlen(buf) - 1; n >= 0; n--) {
		if (buf[n] == '\x0d' || buf[n] == '\x0a') {
			buf[n] = '\0';
		}
	}
}

//---------------------------------------------------------------------
// [�T�v]
// �v���O�����X�^�[�g
//
// [����]
// main�֐��̃w���v���Q��
//
// [�߂�l]
// main�֐��̃w���v���Q��
//
// [����]
// �Ȃ�
//
//---------------------------------------------------------------------

int main(int argc, char** argv)
{
	char line[1024];
	int no = 1;

	chkarg(argc, argv);	// �p�����[�^���

	printf("no,msg1,msg2,jhd_no,code3,choin0,choin1,choin2,cdhikisu,cdsu,update\n");
	FILE* fp = fopen(g_szFname, "r");
	if (fp) {
		while (fgets(line, sizeof(line), fp) != NULL) {
			chomp(line);	// ���s����菜��
			if (strcmp("<<<128>>>", line) == 0) {

				// 128�o�C�g�̓d�����O�̂ݏ�������B
				// �˂ɁA92 05 �d���炵���B

				unsigned char buf[128];
				int idx = 0;

				// ����8�s������������(1�s16�o�C�g * 8�s = 128�o�C�g)
				for (int i = 0; i < 8; i++) {
					fgets(line, sizeof(line), fp);

					for (int c = 0; c < 16; c++) {
						buf[idx++] = (unsigned char)strtoul(&line[c*3], NULL, 16);
					}
				}

				printf("%d,%02X,%02X,%u,%d,%d,%d,%d,%d,%d,%c%c%c%c/%c%c/%c%c %c%c:%c%c:%c%c\n",
					no++,
					buf[0] & 0xff,
					buf[1] & 0xff,
					ntohl(*(long*)&buf[20]),
					
					buf[50] & 0xff,

					ntohl(*(long*)&buf[64]),
					ntohl(*(long*)&buf[68]),
					ntohl(*(long*)&buf[72]),

					ntohl(*(long*)&buf[88]),
					ntohl(*(long*)&buf[92]),

					buf[96],buf[97],buf[98],buf[99],
					buf[100],buf[101],buf[102],buf[103],
					
					buf[104],buf[105],buf[106],buf[107],buf[108],buf[109]);
			}
		}

		fclose(fp);
	}

	return 0;
}
