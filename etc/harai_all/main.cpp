/*****************************************************************************/
/* 	�C���N���[�h�t�@�C��													 */
/*****************************************************************************/
#include <windows.h>
#include <windowsx.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>							/* _kbhit�֐��p */
#include <time.h>							/* ���������p */
#include "denbun_struct.h"					/* �d��struct */
#include "pdpctrl.h"						/* �d������M */
#include "emu_test.h"						/* �v���g�^�C�v�錾 */
#include "def.h"							/* �Í����֐� */


/*****************************************************************************/
/* 	�O���ϐ� �錾															 */
/*****************************************************************************/
/* ini��� */
char com[8];								/* OPEN����COM�|�[�g */
int maker;									/* ���o�@���[�J�[ */
int loop_cnt;								/* ���[�v�� */
int init_interval;							/* ��莞�Ԏ�M���Ȃ���Ώ��������� */

/* �S���[�J�[���ʃO���[�o���ϐ� */
HANDLE	hComm;								/* Com�|�[�g�̐ݒ� */
unsigned char p_send_buf[512];				/* POS���M�菇�p�ޯ̧ */
unsigned char h_send_buf[512];				/* MPC���M�菇�p�ޯ̧ */
unsigned char p_send_buf_open[512];				/* POS���M�菇�p�ޯ̧ */
unsigned char h_send_buf_open[512];				/* MPC���M�菇�p�ޯ̧ */
unsigned char p_send_buf_harai[512];				/* POS���M�菇�p�ޯ̧ */
unsigned char h_send_buf_harai[512];				/* MPC���M�菇�p�ޯ̧ */
char loop_flag = 0;							/* ���[�v�t���O */
char CsvFilePath[MAX_PATH];					/* ���[�J�[����CSV�t�@�C���� */
char CsvFilePathOpen[MAX_PATH];					/* ���[�J�[����CSV�t�@�C���� */
int sum_harai[2][4];						/* ���񂩂猻�݂܂ł̑����o�� */
char esc_flag;								/* �I���t���O */
char connect_flag;							/* �ʐM�����ǂ��� */

/* GLORY�p */
char gl_empty[8];
static char gl_emp_flag = FALSE;

/* �}�[�X�p */
int mtanka[TOKUSHU_MAX];				/* �i�i�P��(����10�Ƃ��鎖) */
char mars_lane[TOKUSHU_MAX][LANE_MAX];				/* �}�[�X�̃��[���ݒ� */
int keihin_put[TOKUSHU_MAX];			/* ���͂������������o����-1���� */
static char m_tanka_check = FALSE;		/* 0:�P���`�F�b�N���s��Ȃ�/1:�P���`�F�b�N���s�� */

/* �O���p */
static char harai_data[256];			/* ���o�v���d���f�[�^�擪�A�h���X */


/*****************************************************************************/
/* 	���M�e�[�u��															 */
/*****************************************************************************/
/* �O���[���[�p���M�e�[�u�� */
char check1_glory_old[] = {STX, '0', 0x40, '0', '0', '0', '0', ETX, '0', '\0'};	/* ��ԃ`�F�b�N1�O���[���[(��) */
char check2_glory_old[] = {STX, 0x33, 0x40, '0', '0', '0', '0', ETX, '0', '\0'};	/* ��ԃ`�F�b�N2�O���[���[(��) */
char check1_glory_new[] = {STX, '0', 0x60, '0', '0', '0', 0x40, ETX, '0', '\0'};	/* ��ԃ`�F�b�N1�O���[���[(�V) */
char check2_glory_new[] = {STX, 0x33, 0x60, '0', '0', '0', 0x40, ETX, '0', '\0'};	/* ��ԃ`�F�b�N2�O���[���[(�V) */
/* �O���p���M�e�[�u�� */
char harai_sankyo[] = {STX, 
	'0', '0', '0', '0',		/* ���[��1���o��[1]�`[4] */
	'0', '0', '0', '0',		/* ���[��2���o��[5]�`[8] */
	'0', '0', '0', '0',		/* ���[��3���o��[9]�`[12] */
	'0', '0', '0', '0',		/* ���[��4���o��[13]�`[16] */
	'0', '0', '0', '0',		/* ���[��5���o��[17]�`[20] */
	'0', '0', '0', '0',		/* ���[��6���o��[21]�`[24] */
	'0', '0', '0', '0',		/* ���[��7���o��[25]�`[28] */
	'0', '0', '0', '0',		/* ���[��8���o��[29]�`[32] */
	'0', '0', '0', '0','0',	/* ���o�c�ʐ�[33]�`[37] */
	ETX, '0', '\0'};		/* �O�� */

/* �}�[�X�p���M�e�[�u�� */
char check_mars[] = {M_STX,
	'5', '1',									/* CMD */
	'0', '0',									/* �v���R�[�h 00:����/01:�i�i�P���`�F�b�N�v�� */
	'0', '0', '0', '0', '0', '0', '0', '0',		/* �\�� */
	M_ETX, '0', '0', CR, LF, '\0'};		/* �}�[�X */
char tanka_check_mars[] = {M_STX,
	'6', '0',							/* CMD */
	'0', '0',							/* �����R�[�h */
	'0', '0', '0', '0', '0', '0', '0', '0',		/* �\�� */
	M_ETX, '0', '0', CR, LF, '\0'};		/* �}�[�X */
char harai_mars[] = {M_STX,
	'5', '3',							/* CMD */
	'0', '0',							/* ���^�[���R�[�h */
	'0', '0', '0', '0', '0', '0',		/* �]��ʐ� */
	'0', '0', '0', '0', '0', '0',		/* �i�i1���o�� */
	'0', '0', '0', '0', '0', '0',		/* �i�i2���o�� */
	'0', '0', '0', '0', '0', '0',		/* �i�i3���o�� */
	'0', '0', '0', '0', '0', '0',		/* �i�i4���o�� */
	M_ETX, '0', '0', CR, LF, '\0'};		/* �}�[�X */
char clr_mars[] = {M_STX,
	'5', '5',							/* CMD */
	'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', /* �\�� */
	M_ETX, '0', '0', CR, LF, '\0'};		/* �}�[�X */
char cal_mars[] = {M_STX,
	'6', '1',							/* CMD */
	'0', '0',							/* ���^�[���R�[�h */
	'0', '0', '0', '0', '0', '0', '0', '0', /* �\�� */
	M_ETX, '0', '0', CR, LF, '\0'};		/* �}�[�X */


char init_check1_glory_old[256];
char init_check1_glory_new[256];
char init_check2_glory_old[256];
char init_check2_glory_new[256];
char init_check_mars[256];
char init_tanka_check_mars[256];
char init_harai_mars[256];
char init_harai_sankyo[256];

/*****************************************************************************/
/* 	main�֐�																 */
/* 		Com�|�[�g���J������ݒ��main_loop�ɓn��							 */
/*		���[�v���o����I��������Com�|�[�g����ď������I������				 */
/* 																			 */
/*****************************************************************************/
void main( void )
{
	int ret;

	/* ini�t�@�C���̓Ǎ��� */
	readini();

	/* COM��OPEN���� */
	hComm = OpenComm();

	if (hComm != INVALID_HANDLE_VALUE) {
		/* �I�[�v������ */
		ret = csv_read_func();				/* ����M�菇CSV�t�@�C���̓Ǎ��� */
		memcpy(p_send_buf, p_send_buf_harai, sizeof(p_send_buf));
		memcpy(h_send_buf, h_send_buf_harai, sizeof(h_send_buf));
		if (ret == -1) {
			return;							/* CSV�t�@�C���Ǎ��ݎ��s */
		}
		
		for(;;){
			ret = main_loop();
			if (ret != 0) {
				break;
			}
		}
		printf("�I��\n");
		CloseHandle(hComm);		/* Com�|�[�g����� */

	} else {
		printf("�I�[�v���G���[\n");
	}

}

/*****************************************************************************/
/* 	���C�����[�v															 */
/* 		�d����M					r_data(data, sizeof(data))				 */
/*		CRC����						crc_cal(data, strlen(data))				 */
/*		�d��Ver����					ver_check(data, ver)					 */
/*		Ұ����������				mcd_check(data, mcd)					 */
/*		�Í���/����(1:ON, 0:OFF)	cmd_cnvrt(data, strlen(data), dai_no)	 */
/*		�d�����					rdata_anal(data)						 */
/* 																			 */
/*****************************************************************************/
int main_loop()
{
	int rec, send_ret;
	char data[256];				/* �d���f�[�^�i�[�p */
	int ch;
	static int i = 0;
	FILE *fp;
	static char m_first_flag = TRUE;	/* ����̏�ԃ`�F�b�N(MARS�̂ݒP���`�F�b�N���s���וK�v) */
	static char timeout_check = FALSE;	/* �^�C�}�[�Ď����Ȃ� */
	static clock_t t1;

/********************************************************************************
	��M����
********************************************************************************/

	/* ��M�d���擾 */
	memset(data, 0x00, sizeof(data));		/* ��M�d���p�ϐ��̏����� */
	rec = RecvData(hComm, data, sizeof(data));

	if (rec > 0) {							/* �d����M */
		/* TIMER�Ď��X�^�[�g */
		t1 = clock();
		timeout_check = TRUE;

		/* �}�[�X��p�J�X���� */
		if(maker == H_MARS){
			if(connect_flag == FALSE){
				/* �ʐM���ł͂Ȃ��ꍇ�͏���ʐM�Ȃ̂łǂ�CSV���瓮�삳���邩���f������ */
				if(p_send_buf_harai[ptr_count] == data[0]){
					/* ���o����Ŋm�肳���� */
					memcpy(p_send_buf, p_send_buf_harai, sizeof(p_send_buf));
					memcpy(h_send_buf, h_send_buf_harai, sizeof(h_send_buf));
				}else if(p_send_buf_open[ptr_count] == data[0]){
					/* �J�X����Ŋm�肳���� */
					memcpy(p_send_buf, p_send_buf_open, sizeof(p_send_buf));
					memcpy(h_send_buf, h_send_buf_open, sizeof(h_send_buf));
				}else{
					if((p_send_buf_harai[ptr_count] == STATUS1) || (p_send_buf_harai[ptr_count] == STATUS2)){
						/* �R�}���h�`�F�b�N */
						if((data[0] == 'S') && (data[1] == '0') && (data[2] == '1')){
							/* ���o�v���Ŋm�� */
							memcpy(p_send_buf, p_send_buf_harai, sizeof(p_send_buf));
							memcpy(h_send_buf, h_send_buf_harai, sizeof(h_send_buf));
						}
					}else if(p_send_buf_harai[ptr_count] == HARAI){
						/* �R�}���h�`�F�b�N */
						if((data[0] == 'S') && (data[1] == '0') && (data[2] == '3')){
							/* ���o�v���Ŋm�� */
							memcpy(p_send_buf, p_send_buf_harai, sizeof(p_send_buf));
							memcpy(h_send_buf, h_send_buf_harai, sizeof(h_send_buf));
						}
					}
					if(p_send_buf_open[ptr_count] == CLR){
						/* �R�}���h�`�F�b�N */
						if((data[0] == 'S') && (data[1] == '0') && (data[2] == '5')){
							/* �J�X�����Ŋm�� */
							memcpy(p_send_buf, p_send_buf_open, sizeof(p_send_buf));
							memcpy(h_send_buf, h_send_buf_open, sizeof(h_send_buf));
						}
					}else if(p_send_buf_open[ptr_count] == CAL){
						/* �R�}���h�`�F�b�N */
						if((data[0] == 'S') && (data[1] == '1') && (data[2] == '1')){
							/* �J�X�����Ŋm�� */
							memcpy(p_send_buf, p_send_buf_open, sizeof(p_send_buf));
							memcpy(h_send_buf, h_send_buf_open, sizeof(h_send_buf));
						}
					}
				}
			}
		}else{
			/* ���̑��̃��[�J�[ */
			memcpy(p_send_buf, p_send_buf_harai, sizeof(p_send_buf));
			memcpy(h_send_buf, h_send_buf_harai, sizeof(h_send_buf));
		}

		connect_flag = TRUE;
		/* ���M�菇�ʂ肩�H */
		if(p_send_buf[ptr_count] != data[0]){
			/* �菇�ʂ�ł͂Ȃ��̂�return */
			if(m_tanka_check == TRUE){
				if((data[0] == M_STX) && (data[1] == '1') && (data[2] == '0')){
					/* �P���ݒ肪�����̂Ń}�[�X�̂ݓ��ʂɂ����ŒP�������𑗐M���� */
					make_tanka_mars(data);
					send_mtanka(data);
					/* �P���v��ON�̂Ƃ��͒P���`�F�b�N���Ă���̂Ń`�F�b�N���� */
					m_tanka_check = FALSE;
					return 0;
				}else{
					/* �P���ݒ肪����܂ł����Ń��^�[������ */
					disp_log_cmd(data, RECEIVE, LOG, rec);
					return 0;
				}
			}
			if((p_send_buf[ptr_count] == STATUS1) || (p_send_buf[ptr_count] == STATUS2)){
				/* ��ԃ`�F�b�N */
				switch(data[0]){
				case M_STX:
					if((data[0] == M_STX) && (data[1] == '0') && (data[2] == '1')){
						if((maker == H_MARS) && (m_first_flag == TRUE)){
							m_first_flag = FALSE;
							m_tanka_check = TRUE;	/* �}�[�X�p�P���v���t���O */
						}
					}else{
						rece_err_flag = TRUE;
					}
					break;
				case STX:
					/* STX�Ȃ̂ŏ����Ȃ� */
					break;
				default:
					rece_err_flag = TRUE;
					break;
				}
			}else if(p_send_buf[ptr_count] == HARAI){
				/* ���o�v�� */
				switch(data[0]){
				case M_STX:
					if((data[0] == M_STX) && (data[1] == '0') && (data[2] == '3')){
						/* �d������Ȃ̂ŕ��o�v����ۑ� */
						memcpy(harai_data, data, sizeof(data));
					}else{
						rece_err_flag = TRUE;
					}
					break;
				case STX:
					/* �d������Ȃ̂ŕ��o�v����ۑ� */
					memcpy(harai_data, data, sizeof(data));
					break;
				default:
					/* �d���ُ� */
					rece_err_flag = TRUE;
					break;
				}
			}else if((p_send_buf[ptr_count] == CLR) || (p_send_buf[ptr_count] == CAL)){
				/* �N���A�v��or�J�����_�[�ݒ�R�}���h */
				switch(data[0]){
				case M_STX:
					if(p_send_buf[ptr_count] == CLR){
						if((data[0] != M_STX) || ((data[1] != '0') || (data[2] != '5'))){
							/* �d���ُ� */
							rece_err_flag = TRUE;
						}
					}else if(p_send_buf[ptr_count] == CAL){
						if((data[0] != M_STX) || ((data[1] != '1') || (data[2] != '1'))){
							/* �d���ُ� */
							rece_err_flag = TRUE;
						}
					}
					break;
				case STX:
					break;
				default:
					/* �d���ُ� */
					rece_err_flag = TRUE;
					break;
				}
			}else{
				rece_err_flag = TRUE;
			}
		}

		/* ��M�d�������O�֏o�� */
		/* disp_log_cmd(data, RECEIVE, DISP, rec); */
		disp_log_cmd(data, RECEIVE, LOG, rec);

		if(rece_err_flag == TRUE){
			/* �d���ُ�Ȃ̂�return */
			rece_err_flag = FALSE;
			return 0;
		}

		/* ���o�v���̏ꍇ�̂݉�ʂɕ��o���̏o�͂��s�� */
		if(p_send_buf[ptr_count] == HARAI){
			disp_harai(data);
		}
		/* �N���A�v���̏ꍇ���o���̃N���A���s�� */
		if(p_send_buf[ptr_count] == CLR){
			printf("�N���A�v����M�����̂ő����o�����N���A���܂��B\n");
			memset(sum_harai, 0x00, sizeof(sum_harai));
		}
		/* �J�����_�Z�b�g�v���̏ꍇ�̂݉�ʂɓ��t�̏o�͂��s�� */
		if(p_send_buf[ptr_count] == CAL){
			disp_cal(data);
		}


/********************************************************************************
		���M����
********************************************************************************/

		/*
		 *	���M����
		 *	CSV̧�قɋL�q����Ă���ʂ�ɑ��M����
		 */
		switch(h_send_buf[ptr_count]){
		case ACK:
			/* ACK���M */
			disp_log((char)ACK);
			send_ret = send_ack();
			break;
		case NAK:
			/* NAK���M */
			disp_log((char)NAK);
			send_ret = send_nak();
			break;
		case EOT:
			/* EOT���M */
			disp_log((char)EOT);
			send_ret = send_eot();
			break;
		case ENQ:
			/* ENQ���M */
			disp_log((char)ENQ);
			send_ret = send_sa();
			send_ret = send_enq();
			break;
		case SYN:
			/* SYN���M */
			disp_log((char)SYN);
			send_ret = send_syn();
			break;
		case STATUS1:
			/* ����ޑ��M(���o�v���O��ԃ`�F�b�N) */
			disp_log((char)STATUS1);
			send_ret = send_status(0);
			break;
		case STATUS2:
			/* ����ޑ��M(���o�v�����ԃ`�F�b�N) */
			disp_log((char)STATUS2);
			send_ret = send_status(1);
			break;
		case HARAI:
			/* ����ޑ��M */
//			disp_log_cmd((char)HARAI);
//			disp_log_cmd(data, SEND, LOG, rec);
			send_ret = send_harai();
			break;
		case CLR:
			/* ����ޑ��M */
			disp_log((char)CLR);
			send_ret = send_clr();
			break;
		case CAL:
			/* ����ޑ��M */
			disp_log((char)CAL);
			send_ret = send_cal();
			break;
		case NO_REC:
			/* ������ */
			disp_log((char)NO_REC);
			send_ret = 0;
			break;
		default:
			/* �L�q���e���̂܂ܑ��M */
			disp_log((char)h_send_buf[ptr_count]);
			send_ret = send_data(ptr_count);
			break;
		}
		/* �G���[�����������ꍇ�͏I�� */
		if(send_ret == -1){
			printf("���M�G���[�ŏ������I�����܂��B(0x%08x)\n", GetLastError());
			return -1;
		}else if(send_ret == -2){
			/* ��M�ҋ@����ESC������ */
			return -1;
		}
		/* ����M�ޯ̧����ړ� */
		ptr_count++;
	} else if (rec == -1) {					/* ��M�G���[ */
		printf("��M�G���[�ŏ������I�����܂��B(0x%08x)\n", GetLastError());
		return -1;
	} else {
		/* ��M�Ȃ� */
		/* �^�C�}�[���`�F�b�N���Ĉ�莞�Ԏ�M�Ȃ���Ώ��������� */
		if(timeout_check == TRUE){
			if((clock() - t1) > init_interval){
				/* ������ */
				data_init();
				timeout_check = FALSE;
				m_first_flag = TRUE;
				printf("%dmsec��M���Ȃ������̂ŒʐM�菇�����������܂��B\n", init_interval);
				return 0;
			}
		}
	}

	/* ����M����ʂ�I��������ŏ��ɖ߂� */
	if(p_send_buf[ptr_count] == NULL){
		/* ���� */
		fp = fopen("data.log", "a");
		fputs("\n", fp);
		fclose(fp);
		timeout_check = FALSE;	/* �^�C�}�[�Ď����Ȃ� */
		data_init();	/* �ϐ���������Ԃ� */
	}



	/*
	 *	�L�[���͏������ړ�
	 */
	if (_kbhit() != 0) {				/* �L�[���͂����������ꍇ */
		ch = getch();
		if (ch == 0x1b) {
			if (esc_flag == TRUE){
				/* 2��A����ESC���͂Ȃ̂ŏI�� */
				return -1;
			}
			/* 1��ŒʐM�̏����� */
			data_init();
			printf("�ʐM�菇�̏��������s���܂����B\n");
			esc_flag = TRUE;
		}
		switch(maker){
		case H_MARS:
			/* �}�[�X�̎��̂ݍē��o�p���͂��\�Ƃ��� */
			if (ch == '1'){
				keihin_put[0]++;
				printf("�i�i1�̕��o����-%d���܂��B\n", keihin_put[0]);
			}
			if (ch == '2'){
				keihin_put[1]++;
				printf("�i�i2�̕��o����-%d���܂��B\n", keihin_put[1]);
			}
			if (ch == '3'){
				keihin_put[2]++;
				printf("�i�i3�̕��o����-%d���܂��B\n", keihin_put[2]);
			}
			if (ch == '4'){
				keihin_put[3]++;
				printf("�i�i4�̕��o����-%d���܂��B\n", keihin_put[3]);
			}
			if ((ch == 'c') || (ch == 'C')){
				/* ���͂��������N���A */
				memset(keihin_put, 0x00, sizeof(keihin_put));
				printf("�i�i��-���o�����N���A���܂��B\n");
			}
			if( (ch == 'i') || (ch == 'I') ){
				/* ���͂��������m�F */
				printf("(���v)�i�i1�̕��o����-%d���܂��B\n", keihin_put[0]);
				printf("(���v)�i�i2�̕��o����-%d���܂��B\n", keihin_put[1]);
				printf("(���v)�i�i3�̕��o����-%d���܂��B\n", keihin_put[2]);
				printf("(���v)�i�i4�̕��o����-%d���܂��B\n", keihin_put[3]);
			}
			break;
		case H_GLORY_OLD:
		case H_GLORY_NEW:
			/* �����ɑΉ������i�i���i�i�����Ƃ��� */
			switch(ch){
			case '1':
				gl_empty[0] = TRUE;
				printf("�i�i1��ENPTY�ɂ��܂��B\n");
				break;
			case '2':
				gl_empty[1] = TRUE;
				printf("�i�i2��ENPTY�ɂ��܂��B\n");
				break;
			case '3':
				gl_empty[2] = TRUE;
				printf("�i�i3��ENPTY�ɂ��܂��B\n");
				break;
			case '4':
				gl_empty[3] = TRUE;
				printf("�i�i4��ENPTY�ɂ��܂��B\n");
				break;
			case '5':
				gl_empty[4] = TRUE;
				printf("�i�i5��ENPTY�ɂ��܂��B\n");
				break;
			case '6':
				gl_empty[5] = TRUE;
				printf("�i�i6��ENPTY�ɂ��܂��B\n");
				break;
			case '7':
				gl_empty[6] = TRUE;
				printf("�i�i7��ENPTY�ɂ��܂��B\n");
				break;
			case '8':
				gl_empty[7] = TRUE;
				printf("�i�i8��ENPTY�ɂ��܂��B\n");
				break;
			default:
				break;
			}
			
			if ((ch == 'c') || (ch == 'C')){
				/* ���͂��������N���A */
				memset(gl_empty, 0x00, sizeof(gl_empty));
				printf("�i�i��EMPTY�����N���A���܂��B\n");
			}
			if( (ch == 'i') || (ch == 'I') ){
				/* ���͂��������m�F */
				printf("�i�i1��EMPTY=%d\n", gl_empty[0]);
				printf("�i�i2��EMPTY=%d\n", gl_empty[1]);
				printf("�i�i3��EMPTY=%d\n", gl_empty[2]);
				printf("�i�i4��EMPTY=%d\n", gl_empty[3]);
				printf("�i�i5��EMPTY=%d\n", gl_empty[4]);
				printf("�i�i6��EMPTY=%d\n", gl_empty[5]);
				printf("�i�i7��EMPTY=%d\n", gl_empty[6]);
				printf("�i�i8��EMPTY=%d\n", gl_empty[7]);
			}
			break;
		default:
			/* ���ʏ������� */
			break;
		}
	}

	return 0;
}

/*****************************************************************************/
/* 	ini�t�@�C���Ǎ��ݗp�֐�													 */
/*****************************************************************************/
void readini(void)
{
	char iniFile[MAX_PATH];
	GetModuleFileName(NULL, iniFile, MAX_PATH);
	memcpy(CsvFilePath, iniFile, sizeof(iniFile));
	memcpy(CsvFilePathOpen, iniFile, sizeof(iniFile));
	PathRemoveFileSpec(CsvFilePath);
	PathRemoveFileSpec(CsvFilePathOpen);
	PathRenameExtension(iniFile, ".ini");

	/* �e�O���[�o���ϐ��̏����� */
	memset(com, 0x00, sizeof(com));
	memset(sum_harai, 0x00, sizeof(sum_harai));
	memset(harai_data, 0x00, sizeof(harai_data));
	memset(keihin_put, 0x00, sizeof(keihin_put));
	memset(gl_empty, 0x00, sizeof(gl_empty));
	maker = 0;
	memcpy(init_check1_glory_old, check1_glory_old, sizeof(check1_glory_old));
	memcpy(init_check1_glory_new, check1_glory_new, sizeof(check1_glory_new));
	memcpy(init_check2_glory_old, check2_glory_old, sizeof(check2_glory_old));
	memcpy(init_check2_glory_new, check2_glory_new, sizeof(check2_glory_new));
	memcpy(init_check_mars, check_mars, sizeof(check_mars));
	memcpy(init_tanka_check_mars, tanka_check_mars, sizeof(tanka_check_mars));
	memcpy(init_harai_mars, harai_mars, sizeof(harai_mars));
	memcpy(init_harai_sankyo, harai_sankyo, sizeof(harai_sankyo));
	connect_flag = FALSE;



	// ini�t�@�C������OPEN����COM�̓Ǎ���
	GetPrivateProfileString("HARAI", "COM", "COM1", com, sizeof(com), iniFile);
	printf("%s\n", &com[0]);

	// ini�t�@�C�����瓮�삳���镥�o�@���[�J�[�̎擾
	maker = GetPrivateProfileInt("HARAI", "harai_maker", 0, iniFile);
	switch(maker){
	case H_GLORY_OLD:
		PathAppend(CsvFilePath, "glory.csv");
//		PathAppend(CsvFilePathOpen, "glory_open.csv");
		printf("���o�@���[�J�[�F�O���[���[(��)\n");
		break;
	case H_GLORY_NEW:
		PathAppend(CsvFilePath, "glory.csv");
//		PathAppend(CsvFilePathOpen, "glory_open.csv");
		printf("���o�@���[�J�[�F�O���[���[(�V)\n");
		break;
	case H_SANKYO:
		PathAppend(CsvFilePath, "sankyo.csv");
//		PathAppend(CsvFilePathOpen, "sankyo_open.csv");
		printf("���o�@���[�J�[�F�O��\n");
		break;
	case H_MARS:
		PathAppend(CsvFilePath, "mars.csv");
		PathAppend(CsvFilePathOpen, "mars_open.csv");
		printf("���o�@���[�J�[�F�}�[�X\n");
		break;
	case H_HIROTAC:
		PathAppend(CsvFilePath, "glory.csv");
//		PathAppend(CsvFilePathOpen, "glory_open.csv");
		printf("���o�@���[�J�[�F�q���^�b�N\n");
		break;
	case H_SUBARU:
		PathAppend(CsvFilePath, "glory.csv");
//		PathAppend(CsvFilePathOpen, "glory_open.csv");
		printf("���o�@���[�J�[�F�X�o��\n");
		break;
	case H_TMI:
		PathAppend(CsvFilePath, "glory.csv");
//		PathAppend(CsvFilePathOpen, "glory_open.csv");
		printf("���o�@���[�J�[�FTMI\n");
		break;
	default:
		PathAppend(CsvFilePath, "glory.csv");
//		PathAppend(CsvFilePathOpen, "glory_open.csv");
		printf("���o�@���[�J�[�F�O���[���[(�f�t�H���g)\n");
		break;
	}

	loop_cnt = GetPrivateProfileInt("HARAI", "loop", 10, iniFile);
	loop_cnt--;	/* 1�񕪗]���Ȃ̂ŁE�E�E */
	init_interval = GetPrivateProfileInt("HARAI", "timer", 5000, iniFile);
	printf("---------------------------------------------\n");
}


/*****************************************************************************/
/* 	CSV�t�@�C���Ǎ��ݗp�֐�													 */
/*																			 */
/* 		<ret>																 */
/*			(int)			-1: �G���[	0: ����								 */
/*																			 */
/*****************************************************************************/
int csv_read_func( void )
{
	int ret;

	/* ���o�pCSV�t�@�C���̓Ǎ��� */
	ret = read_csv_harai();
	if(ret == -1){
		/* �t�@�C�����ǂݍ��߂Ȃ��ꍇ */
		printf("���M�菇�t�@�C���Ǎ��݃G���[�̂��߁A�������I�����܂��B\n");
		return -1;
	}
	/* �J�X�����pCSV�t�@�C���̓Ǎ��� */
	if(maker == H_MARS){
		ret = read_csv_open();
		if(ret == -1){
			/* �t�@�C�����ǂݍ��߂Ȃ��ꍇ */
			printf("���M�菇�t�@�C���Ǎ��݃G���[�̂��߁A�������I�����܂��B\n");
			return -1;
		}
	}
	return 0;
}

/*****************************************************************************/
/* 	CSV�t�@�C���Ǎ��ݗp�֐�													 */
/*																			 */
/* 		<ret>																 */
/*			(int)			-1: �G���[	0: ����								 */
/*																			 */
/*****************************************************************************/
int read_csv_harai( void )
{
	FILE *fp;				/* CSV�t�@�C���Ǎ��ݗp */
	char lbuf[1024];		/* �f�[�^�p�ޯ̧�i[,]����s�R�[�h���܂ށj */
	char d_buf[64];
	int len;				/* �f�[�^�� */
	int pcnt, hcnt, plcnt, hlcnt;			/* �f�[�^�� */
	int i, j, k;
	BOOL harai_send, pos_send, skip_flag;
	int data;				/* �ޯ̧�i�[�ϐ� */
	unsigned char p_send_loop_buf[512];			/* POS���M�菇�p�ޯ̧ */
	unsigned char h_send_loop_buf[512];			/* MPC���M�菇�p�ޯ̧ */

	/* csv̧�ق̓Ǎ�(���[�J�[���ɐ؂�ւ���) */
	if ( (fp = fopen(CsvFilePath, "r")) == NULL){
		/* �t�@�C�����ǂݍ��߂Ȃ��ꍇ */
		return -1;
	}

	/* �f�[�^�̏����� */
	pcnt = 0;
	hcnt = 0;
	plcnt = 0;
	hlcnt = 0;
	pos_send = FALSE;
	harai_send = FALSE;
	skip_flag = FALSE;
	data = 0;
	memset(lbuf, 0x00, sizeof(lbuf));	/* lbuf�̏����� */
	memset(p_send_buf_harai, NULL, sizeof(p_send_buf_harai));	/* p_send_buf_harai�̏����� */
	memset(h_send_buf_harai, NULL, sizeof(h_send_buf_harai));	/* h_send_buf_harai�̏����� */

	while(fgets(lbuf, sizeof(lbuf), fp) != NULL){		/* �f�[�^�̓Ǎ��� */
		if ( (lbuf[0] == 0x0a) || (lbuf[0] == 0x0d) ){	/* ���s�R�[�h�͓ǂݔ�΂� */
			continue;
		}
		len = 0;										/* �f�[�^���̏����� */
		i = strlen(lbuf);								/* �T�C�Y�̎擾 */
		for(j = 0; j < i; j++){							/* �T�C�Y�����[�v */
			if(lbuf[j] == '#'){							/* [#]�͓ǂݔ�΂� */
				if(lbuf[j+1] == 'P'){
					/* POS�̑��M�f�[�^ */
					pos_send = TRUE;
					harai_send = FALSE;
				}else if(lbuf[j+1] == 'H'){
					/* ���o�@�̑��M�f�[�^ */
					pos_send = FALSE;
					harai_send = TRUE;
				}else{
					break;
				}
				skip_flag = TRUE;
			}
			if ( (lbuf[j] == ',') || (lbuf[j] == 0x0a) || (lbuf[j] == 0x0d) ) {	/* [,]�Ɖ��s�R�[�h */
				skip_flag = FALSE;
				if (len != 0) {							/* �f�[�^����0�ȊO */
					/* �L�q������������� */
					if(len == 3){
						data = get_send_csv(lbuf, j);
						if(data == SKIP){
							len = 0;
							if(pos_send == TRUE){
								if(loop_flag == LOOP_ED){
									/* �R�s�[�������̂�}�� */
									for(k=0;k<loop_cnt;k++){
										memcpy(&(p_send_buf_harai[pcnt]), &(p_send_loop_buf[0]), plcnt);
										pcnt += plcnt;
									}
									plcnt = 0;
									loop_flag = LOOP_NONE;
								}
							}
							if(harai_send == TRUE){
								if(loop_flag == LOOP_ED){
									/* �R�s�[�������̂�}�� */
									for(k=0;k<loop_cnt;k++){
										memcpy(&(h_send_buf_harai[hcnt]), &(h_send_loop_buf[0]), hlcnt);
										hcnt += hlcnt;
									}
									hlcnt = 0;
									loop_flag = LOOP_NONE;
								}
							}
							continue;
						}
					}else if(len == 1){
						if(lbuf[j-1] == '-'){
							/* ������ */
							data = NO_REC;
						}
					}else{
						/* ���l���͂�text�R�}���h���͂̂Q�ʂ肠�� */
						/* 0x�Ŏn�܂�ꍇ�͐��l���͂Ƃ��ASTX�Ŏn�܂�ꍇ��̧�ٖ��̓��͂Ƃ��� */
						if((lbuf[j-len] == '0') && (lbuf[j-len+1] == 'x')){
							/* ���l���� */
							for(k = 2; k < len; k++){
								if('0' <= lbuf[j-len+k] && lbuf[j-len+k] <= '9'){
									d_buf[k-2] = lbuf[j-len+k] - '0';
								}else if('a' <= lbuf[j-len+k] && lbuf[j-len+k] <= 'f'){
									d_buf[k-2] = lbuf[j-len+k] - 'a' + 10;
								}
							}
							data = (d_buf[0] << 4) + d_buf[1];
						}
						if((lbuf[j-len] == 'S') && (lbuf[j-len+1] == 'T') && (lbuf[j-len+2] == 'A')){
							/* STATUS(��ԃ`�F�b�N) */
							if(lbuf[j-len+6] == '1'){
								data = STATUS1;
							}else{
								data = STATUS2;
							}
						}
						if((lbuf[j-len] == 'H') && (lbuf[j-len+1] == 'A') && (lbuf[j-len+2] == 'R')){
							/* HARAI(���o�v��) */
							data = HARAI;
						}
					}
					if(pos_send == TRUE){
						/* POS�̑��M�菇�ޯ̧�ֺ�߰ */
						memcpy(&(p_send_buf_harai[pcnt]), &data, sizeof(char));			/* p_buf�ւ̃f�[�^�̃R�s�[ */
						if(loop_flag == LOOP_ST){
							/* �R�s�[�J�n */
							memcpy(&(p_send_loop_buf[plcnt]), &data, sizeof(char));			/* p_buf_loop�ւ̃f�[�^�̃R�s�[ */
							plcnt++;
						}
						pcnt++;
					}
					if(harai_send == TRUE){
						/* MPC�̑��M�菇�ޯ̧�ֺ�߰ */
						memcpy(&(h_send_buf_harai[hcnt]), &data, sizeof(char));			/* h_buf�ւ̃f�[�^�̃R�s�[ */
						if(loop_flag == LOOP_ST){
							/* �R�s�[�J�n */
							memcpy(&(h_send_loop_buf[hlcnt]), &data, sizeof(char));			/* h_buf_loop�ւ̃f�[�^�̃R�s�[ */
							hlcnt++;
						}
						hcnt++;
					}
					/* len�̏����� */
					len = 0;
					continue;
				}
			} else {									/*	*/
				if(skip_flag == TRUE){
					continue;
				}
				len++;									/* �f�[�^����+1���� */
			}
		}
	}
	fclose(fp);
	return 0;
	
}

/*****************************************************************************/
/* 	CSV�t�@�C���Ǎ��ݗp�֐�													 */
/*																			 */
/* 		<ret>																 */
/*			(int)			-1: �G���[	0: ����								 */
/*																			 */
/*****************************************************************************/
int read_csv_open( void )
{
	FILE *fp;				/* CSV�t�@�C���Ǎ��ݗp */
	char lbuf[1024];		/* �f�[�^�p�ޯ̧�i[,]����s�R�[�h���܂ށj */
	char d_buf[64];
	int len;				/* �f�[�^�� */
	int pcnt, hcnt, plcnt, hlcnt;			/* �f�[�^�� */
	int i, j, k;
	BOOL harai_send, pos_send, skip_flag;
	int data;				/* �ޯ̧�i�[�ϐ� */
	unsigned char p_send_loop_buf[512];			/* POS���M�菇�p�ޯ̧ */
	unsigned char h_send_loop_buf[512];			/* MPC���M�菇�p�ޯ̧ */

	/* csv̧�ق̓Ǎ�(���[�J�[���ɐ؂�ւ���) */
	if ( (fp = fopen(CsvFilePathOpen, "r")) == NULL){
		/* �t�@�C�����ǂݍ��߂Ȃ��ꍇ */
		return -1;
	}

	/* �f�[�^�̏����� */
	pcnt = 0;
	hcnt = 0;
	plcnt = 0;
	hlcnt = 0;
	pos_send = FALSE;
	harai_send = FALSE;
	skip_flag = FALSE;
	data = 0;
	memset(lbuf, 0x00, sizeof(lbuf));	/* lbuf�̏����� */
	memset(p_send_buf_open, NULL, sizeof(p_send_buf_open));	/* p_send_buf_open�̏����� */
	memset(h_send_buf_open, NULL, sizeof(h_send_buf_open));	/* h_send_buf_open�̏����� */

	while(fgets(lbuf, sizeof(lbuf), fp) != NULL){		/* �f�[�^�̓Ǎ��� */
		if ( (lbuf[0] == 0x0a) || (lbuf[0] == 0x0d) ){	/* ���s�R�[�h�͓ǂݔ�΂� */
			continue;
		}
		len = 0;										/* �f�[�^���̏����� */
		i = strlen(lbuf);								/* �T�C�Y�̎擾 */
		for(j = 0; j < i; j++){							/* �T�C�Y�����[�v */
			if(lbuf[j] == '#'){							/* [#]�͓ǂݔ�΂� */
				if(lbuf[j+1] == 'P'){
					/* POS�̑��M�f�[�^ */
					pos_send = TRUE;
					harai_send = FALSE;
				}else if(lbuf[j+1] == 'H'){
					/* ���o�@�̑��M�f�[�^ */
					pos_send = FALSE;
					harai_send = TRUE;
				}else{
					break;
				}
				skip_flag = TRUE;
			}
			if ( (lbuf[j] == ',') || (lbuf[j] == 0x0a) || (lbuf[j] == 0x0d) ) {	/* [,]�Ɖ��s�R�[�h */
				skip_flag = FALSE;
				if (len != 0) {							/* �f�[�^����0�ȊO */
					/* �L�q������������� */
					if(len == 3){
						data = get_send_csv(lbuf, j);
						if(data == SKIP){
							len = 0;
							if(pos_send == TRUE){
								if(loop_flag == LOOP_ED){
									/* �R�s�[�������̂�}�� */
									for(k=0;k<loop_cnt;k++){
										memcpy(&(p_send_buf_open[pcnt]), &(p_send_loop_buf[0]), plcnt);
										pcnt += plcnt;
									}
									plcnt = 0;
									loop_flag = LOOP_NONE;
								}
							}
							if(harai_send == TRUE){
								if(loop_flag == LOOP_ED){
									/* �R�s�[�������̂�}�� */
									for(k=0;k<loop_cnt;k++){
										memcpy(&(h_send_buf_open[hcnt]), &(h_send_loop_buf[0]), hlcnt);
										hcnt += hlcnt;
									}
									hlcnt = 0;
									loop_flag = LOOP_NONE;
								}
							}
							continue;
						}
					}else if(len == 1){
						if(lbuf[j-1] == '-'){
							/* ������ */
							data = NO_REC;
						}
					}else{
						/* ���l���͂�text�R�}���h���͂̂Q�ʂ肠�� */
						/* 0x�Ŏn�܂�ꍇ�͐��l���͂Ƃ��ASTX�Ŏn�܂�ꍇ��̧�ٖ��̓��͂Ƃ��� */
						if((lbuf[j-len] == '0') && (lbuf[j-len+1] == 'x')){
							/* ���l���� */
							for(k = 2; k < len; k++){
								if('0' <= lbuf[j-len+k] && lbuf[j-len+k] <= '9'){
									d_buf[k-2] = lbuf[j-len+k] - '0';
								}else if('a' <= lbuf[j-len+k] && lbuf[j-len+k] <= 'f'){
									d_buf[k-2] = lbuf[j-len+k] - 'a' + 10;
								}
							}
							data = (d_buf[0] << 4) + d_buf[1];
						}
						if((lbuf[j-len] == 'S') && (lbuf[j-len+1] == 'T') && (lbuf[j-len+2] == 'A')){
							/* STATUS(��ԃ`�F�b�N) */
							if(lbuf[j-len+6] == '1'){
								data = STATUS1;
							}else{
								data = STATUS2;
							}
						}
						if((lbuf[j-len] == 'H') && (lbuf[j-len+1] == 'A') && (lbuf[j-len+2] == 'R')){
							/* HARAI(���o�v��) */
							data = HARAI;
						}
					}
					if(pos_send == TRUE){
						/* POS�̑��M�菇�ޯ̧�ֺ�߰ */
						memcpy(&(p_send_buf_open[pcnt]), &data, sizeof(char));			/* p_buf�ւ̃f�[�^�̃R�s�[ */
						if(loop_flag == LOOP_ST){
							/* �R�s�[�J�n */
							memcpy(&(p_send_loop_buf[plcnt]), &data, sizeof(char));			/* p_buf_loop�ւ̃f�[�^�̃R�s�[ */
							plcnt++;
						}
						pcnt++;
					}
					if(harai_send == TRUE){
						/* MPC�̑��M�菇�ޯ̧�ֺ�߰ */
						memcpy(&(h_send_buf_open[hcnt]), &data, sizeof(char));			/* h_buf�ւ̃f�[�^�̃R�s�[ */
						if(loop_flag == LOOP_ST){
							/* �R�s�[�J�n */
							memcpy(&(h_send_loop_buf[hlcnt]), &data, sizeof(char));			/* h_buf_loop�ւ̃f�[�^�̃R�s�[ */
							hlcnt++;
						}
						hcnt++;
					}
					/* len�̏����� */
					len = 0;
					continue;
				}
			} else {									/*	*/
				if(skip_flag == TRUE){
					continue;
				}
				len++;									/* �f�[�^����+1���� */
			}
		}
	}
	fclose(fp);
	return 0;
	
}

/*****************************************************************************/
/* 	CSV�L�q���e�擾�֐�														 */
/*			buf, cnt			CSV�L�q�f�[�^, �����ԍ�						 */
/* 		<ret>																 */
/*			�f�[�^															 */
/*				0x05:ENQ													 */
/*				0x04:EOT													 */
/*				0x06:ACK													 */
/*				0x15:NAK													 */
/*				0x02:STX													 */
/*				0x99:ERR													 */
/*****************************************************************************/
int get_send_csv(char *buf, int cnt)
{
	char data1, data2, data3;
	int ret;

	/* ������ */
	data1 = 0;
	data2 = 0;
	data3 = 0;
	ret = 0;

	/* �l�̑�� */
	data1 = buf[cnt-3];
	data2 = buf[cnt-2];
	data3 = buf[cnt-1];

	if((data1 == 'E') && (data2 == 'N') && (data3 == 'Q')){
		/* ENQ */
		ret = ENQ;
	}else if((data1 == 'E') && (data2 == 'O') && (data3 == 'T')){
		/* EOT */
		ret = EOT;
	}else if((data1 == 'A') && (data2 == 'C') && (data3 == 'K')){
		/* ACK */
		ret = ACK;
	}else if((data1 == 'S') && (data2 == 'Y') && (data3 == 'N')){
		/* SYN */
		ret = SYN;
	}else if((data1 == 'N') && (data2 == 'A') && (data3 == 'K')){
		/* NAK */
		ret = NAK;
	}else if((data1 == 'L') && (data2 == 'S') && (data3 == 'T')){
		/* LOOP�X�^�[�g(LED�܂ł��w���LOOP����) */
		loop_flag = LOOP_ST;
		ret = SKIP;
	}else if((data1 == 'L') && (data2 == 'E') && (data3 == 'D')){
		/* LOOP�G���h */
		loop_flag = LOOP_ED;
		ret = SKIP;
	}else if((data1 == 'C') && (data2 == 'L') && (data3 == 'R')){
		/* �N���A�v���R�}���h */
		ret = CLR;
	}else if((data1 == 'C') && (data2 == 'A') && (data3 == 'L')){
		/* �J�����_�[�ݒ�R�}���h */
		ret = CAL;
	}else{
		/* �ُ�ȋL�q */
		ret = ERR;
	}

	return ret;
}

/*****************************************************************************/
/* 	data_init()																 */
/* 		�ϐ�������															 */
/*****************************************************************************/
int data_init( void )
{
	/* ����M�̍ŏ��֖߂�ϐ� */
	ptr_count = 0;
	memset(keihin_put, 0x00, sizeof(keihin_put));
	memset(gl_empty, 0x00, sizeof(gl_empty));
	esc_flag = FALSE;
	m_tanka_check = FALSE;
	gl_emp_flag = FALSE;
	memcpy(check1_glory_old, init_check1_glory_old, sizeof(check1_glory_old));
	memcpy(check1_glory_new, init_check1_glory_new, sizeof(check1_glory_new));
	memcpy(check2_glory_old, init_check2_glory_old, sizeof(check2_glory_old));
	memcpy(check2_glory_new, init_check2_glory_new, sizeof(check2_glory_new));
	memcpy(check_mars, init_check_mars, sizeof(check_mars));
	memcpy(tanka_check_mars, init_tanka_check_mars, sizeof(tanka_check_mars));
	memcpy(harai_mars, init_harai_mars, sizeof(harai_mars));
	memcpy(harai_sankyo, init_harai_sankyo, sizeof(harai_sankyo));
	connect_flag = FALSE;
	return 0;
}

/*****************************************************************************/
/* 	send_ack																 */
/* 		ACK�d�����M															 */
/*****************************************************************************/
int send_ack( void )
{
	char data[4];
	int rec;

	memset(data, 0x00, sizeof(data));
	data[0] = ACK;
	rec = SendData(hComm, data);				/* �d�����M */
	if (rec == -1) {
		/* ���M�G���[ */
		return -1;
	} else {
		/* ���M���� */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_nak																 */
/* 		NAK�d�����M															 */
/*****************************************************************************/
int send_nak( void )
{
	char data[4];
	int rec;

	memset(data, 0x00, sizeof(data));
	data[0] = NAK;
	rec = SendData(hComm, data);				/* �d�����M */
	if (rec == -1) {
		/* ���M�G���[ */
		return -1;
	} else {
		/* ���M���� */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_eot																 */
/* 		EOT�d�����M															 */
/*****************************************************************************/
int send_eot( void )
{
	char data[4];
	int rec;

	memset(data, 0x00, sizeof(data));
	data[0] = EOT;
	rec = SendData(hComm, data);				/* �d�����M */
	if (rec == -1) {
		/* ���M�G���[ */
		return -1;
	} else {
		/* ���M���� */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_sa																 */
/* 		SA�d�����M															 */
/*****************************************************************************/
int send_sa( void )
{
	char data[4];
	int rec;

	memset(data, 0x00, sizeof(data));
	data[0] = SA;
	rec = SendData(hComm, data);				/* �d�����M */
	if (rec == -1) {
		/* ���M�G���[ */
		return -1;
	} else {
		/* ���M���� */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_syn																 */
/* 		SYN�d�����M															 */
/*****************************************************************************/
int send_syn( void )
{
	char data[4];
	int rec;

	memset(data, 0x00, sizeof(data));
	data[0] = SYN;
	rec = SendData(hComm, data);				/* �d�����M */
	if (rec == -1) {
		/* ���M�G���[ */
		return -1;
	} else {
		/* ���M���� */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_enq																 */
/* 		ENQ�d�����M															 */
/*****************************************************************************/
int send_enq( void )
{
	char data[4];
	int rec;

	memset(data, 0x00, sizeof(data));
	data[0] = ENQ;
	rec = SendData(hComm, data);				/* �d�����M */
	if (rec == -1) {
		/* ���M�G���[ */
		return -1;
	} else {
		/* ���M���� */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_data																 */
/* 		DATA�d�����M														 */
/*****************************************************************************/
int send_data( char n )
{
	char data[32];
	int i = 0, rec;

	
	memset(data, 0x00, sizeof(data));
	data[i] = h_send_buf[n];

	rec = SendData(hComm, data);				/* �d�����M */
	if (rec == -1) {
		/* ���M�G���[ */
		return -1;
	} else {
		/* ���M���� */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_mtanka																 */
/* 		�P���������쐬����(��{�I�ɂ͒P����ۑ����Ă��̂܂܃I�E���Ԃ�)		 */
/* 			data		�ް�												 */
/* 																			 */
/*****************************************************************************/
int send_mtanka( char *data )
{
	int i = 0, rec;
	int bcc, bcc1, bcc2;

	bcc = 0;
	bcc1 = 0;
	bcc2 = 0;

	/* ��M�����P���ݒ�d�������̂܂܃R�s�[ */
	memcpy(tanka_check_mars, data, sizeof(tanka_check_mars) - 6);

	/* CMD�ԍ���ς��� */
	tanka_check_mars[1] = '6';
	tanka_check_mars[2] = '0';
	/* BCC���v�Z */
	for(i = 0; i < sizeof(tanka_check_mars) - 5; i++){
		bcc += tanka_check_mars[i];
	}
	bcc = 0xff - (char)bcc;
	bcc1 = ((bcc & 0xf0) >> 4);		/* 1BYTE����ASCII�ϊ� */
	if((bcc1 >= 0) && (bcc1 <= 9)){
		/* 0�`9 */
		bcc1 += '0';
	}else{
		/* A�`F */
		bcc1 = (bcc1 - 0x0a) + 'A';
	}
	bcc2 = (bcc & 0x0f);				/* 1BYTE����ASCII�ϊ� */
	if((bcc2 >= 0) && (bcc2 <= 9)){
		/* 0�`9 */
		bcc2 += '0';
	}else{
		/* A�`F */
		bcc2 = (bcc2 - 0x0a) + 'A';
	}
	tanka_check_mars[14] = bcc1;
	tanka_check_mars[15] = bcc2;

	rec = SendData(hComm, tanka_check_mars);				/* �d�����M */
	if (rec == -1) {
		/* ���M�G���[ */
		return -1;
	} else {
		/* ���M���� */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_clr																 */
/* 		CLR�d�����M															 */
/*****************************************************************************/
int send_clr( void )
{
	int i = 0, rec;
	int bcc, bcc1, bcc2;

	bcc = 0;
	bcc1 = 0;
	bcc2 = 0;

	/* BCC���v�Z */
	for(i = 0; i < sizeof(clr_mars) - 5; i++){
		bcc += clr_mars[i];
	}
	bcc = 0xff - (char)bcc;
	bcc1 = ((bcc & 0xf0) >> 4);		/* 1BYTE����ASCII�ϊ� */
	if((bcc1 >= 0) && (bcc1 <= 9)){
		/* 0�`9 */
		bcc1 += '0';
	}else{
		/* A�`F */
		bcc1 = (bcc1 - 0x0a) + 'A';
	}
	bcc2 = (bcc & 0x0f);				/* 1BYTE����ASCII�ϊ� */
	if((bcc2 >= 0) && (bcc2 <= 9)){
		/* 0�`9 */
		bcc2 += '0';
	}else{
		/* A�`F */
		bcc2 = (bcc2 - 0x0a) + 'A';
	}
	clr_mars[14] = bcc1;
	clr_mars[15] = bcc2;

	rec = SendData(hComm, clr_mars);				/* �d�����M */
	if (rec == -1) {
		/* ���M�G���[ */
		return -1;
	} else {
		/* ���M���� */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_cal																 */
/* 		CLR�d�����M															 */
/*****************************************************************************/
int send_cal( void )
{
	int i = 0, rec;
	int bcc, bcc1, bcc2;

	bcc = 0;
	bcc1 = 0;
	bcc2 = 0;

	/* BCC���v�Z */
	for(i = 0; i < sizeof(cal_mars) - 5; i++){
		bcc += cal_mars[i];
	}
	bcc = 0xff - (char)bcc;
	bcc1 = ((bcc & 0xf0) >> 4);		/* 1BYTE����ASCII�ϊ� */
	if((bcc1 >= 0) && (bcc1 <= 9)){
		/* 0�`9 */
		bcc1 += '0';
	}else{
		/* A�`F */
		bcc1 = (bcc1 - 0x0a) + 'A';
	}
	bcc2 = (bcc & 0x0f);				/* 1BYTE����ASCII�ϊ� */
	if((bcc2 >= 0) && (bcc2 <= 9)){
		/* 0�`9 */
		bcc2 += '0';
	}else{
		/* A�`F */
		bcc2 = (bcc2 - 0x0a) + 'A';
	}
	cal_mars[14] = bcc1;
	cal_mars[15] = bcc2;

	rec = SendData(hComm, cal_mars);				/* �d�����M */
	if (rec == -1) {
		/* ���M�G���[ */
		return -1;
	} else {
		/* ���M���� */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_status																 */
/* 		��ԃ`�F�b�N�d�����M												 */
/* 			����															 */
/* 				0:���o�v���O												 */
/* 				1:���o�v����												 */
/*****************************************************************************/
int send_status( char mode )
{
	char *send_data;
	int rec, i;
	char bcc, bcc1, bcc2, emp_data;
	static char step = 0;

	bcc = 0;
	bcc1 = 0;
	bcc2 = 0;
	emp_data = 0;
	switch(maker){
	case H_GLORY_OLD:
		if( mode == 0){
			send_data = check1_glory_old;
			send_data[2] = 0x40;
			if(gl_emp_flag == TRUE){
				if(step == 0){
					/* ���[�v��1��ڂ̏�ԃ`�F�b�N�v���ŕ��o����I����Ԃ� */
					send_data[1] = '1';
					step++;
				}else{
					/* ���[�v��2��ڂ̏�ԃ`�F�b�N�v����EMPTY����Ԃ� */
					emp_data = gl_empty[0] << 2;
					emp_data += gl_empty[1] << 1;
					emp_data += gl_empty[2];
					emp_data += gl_empty[3] << 3;
					send_data[2] += emp_data;
					gl_emp_flag = FALSE;
					step = 0;
					send_data[1] = '0';
				}
			}
		}else{
			send_data = check2_glory_old;
			gl_emp_flag = TRUE;
		}
		/* STX�̎�����ETX�܂ł̔r���I�_���a */
		for(i = 1; i < sizeof(check1_glory_old) - 2; i++){
			bcc ^= send_data[i];
		}
		send_data[i] = bcc;

		//calc_bcc(send_data);
		break;
	case H_GLORY_NEW:
		if( mode == 0){
			send_data = check1_glory_new;
			send_data[2] = 0x60;
			send_data[6] = 0x40;
			if(gl_emp_flag == TRUE){
				if(step == 0){
					/* ���[�v��1��ڂ̏�ԃ`�F�b�N�v���ŕ��o����I����Ԃ� */
					send_data[1] = '1';
					step++;
				}else{
					/* ���[�v��2��ڂ̏�ԃ`�F�b�N�v����EMPTY����Ԃ� */
					emp_data = gl_empty[0] << 2;
					emp_data += gl_empty[1] << 1;
					emp_data += gl_empty[2];
					emp_data += gl_empty[3] << 3;
					send_data[2] += emp_data;
					emp_data = gl_empty[4] << 2;
					emp_data += gl_empty[5] << 1;
					emp_data += gl_empty[6];
					emp_data += gl_empty[7] << 3;
					send_data[6] += emp_data;
					gl_emp_flag = FALSE;
					step = 0;
					send_data[1] = '0';
				}
			}
		}else{
			send_data = check2_glory_new;
			gl_emp_flag = TRUE;
		}
		/* STX�̎�����ETX�܂ł̔r���I�_���a */
		for(i = 1; i < sizeof(check1_glory_new) - 2; i++){
			bcc ^= send_data[i];
		}
		send_data[i] = bcc;
		//calc_bcc(send_data);
		break;
	case H_SANKYO:
		/* �O���ł̓X�e�[�^�X���M�͂Ȃ� */
		break;
	case H_MARS:
		if(m_tanka_check == TRUE){
			check_mars[4] = '1';
		}else{
			check_mars[4] = '0';
		}

		/* BCC���v�Z */
		for(i = 0; i < sizeof(check_mars) - 5; i++){
			bcc += check_mars[i];
		}
		bcc = 0xff - (char)bcc;
		bcc1 = ((bcc & 0xf0) >> 4);		/* 1BYTE����ASCII�ϊ� */
		if((bcc1 >= 0) && (bcc1 <= 9)){
			/* 0�`9 */
			bcc1 += '0';
		}else{
			/* A�`F */
			bcc1 = (bcc1 - 0x0a) + 'A';
		}
		bcc2 = (bcc & 0x0f);				/* 1BYTE����ASCII�ϊ� */
		if((bcc2 >= 0) && (bcc2 <= 9)){
			/* 0�`9 */
			bcc2 += '0';
		}else{
			/* A�`F */
			bcc2 = (bcc2 - 0x0a) + 'A';
		}
		check_mars[14] = bcc1;
		check_mars[15] = bcc2;
		send_data = check_mars;
		break;
	case H_HIROTAC:
		break;
	case H_SUBARU:
		break;
	case H_TMI:
		break;
	default:
		if( mode == 0){
			send_data = check1_glory_old;
		}else{
			send_data = check2_glory_old;
		}
		/* STX�̎�����ETX�܂ł̔r���I�_���a */
		for(i = 1; i < sizeof(send_data); i++){
			bcc ^= send_data[i];
		}
		send_data[i+1] = bcc;
		break;
	}
	rec = SendData(hComm, send_data);				/* �d�����M */
	if (rec == -1) {
		/* ���M�G���[ */
		return -1;
	} else {
		/* ���M���� */
		return 0;
	}
}

/*****************************************************************************/
/* 	send_harai																 */
/* 		��ԃ`�F�b�N�d�����M														 */
/*****************************************************************************/
int send_harai( void )
{
	char *send_data;
	char bcc, bcc1, bcc2;
	int rec, i;

	bcc = 0;
	bcc1 = 0;
	bcc2 = 0;
	switch(maker){
	case H_GLORY_OLD:
		/* �O���[���[�ł͕��o�����͂Ȃ� */
		break;
	case H_GLORY_NEW:
		/* �O���[���[�ł͕��o�����͂Ȃ� */
		break;
	case H_SANKYO:
		make_harai_sankyo(harai_sankyo);
		send_data = harai_sankyo;
		break;
	case H_MARS:
		make_harai_mars(harai_mars, sizeof(harai_mars));
		/* BCC���v�Z */
		for(i = 0; i < sizeof(harai_mars) - 5; i++){
			bcc += harai_mars[i];
		}
		bcc = 0xff - (char)bcc;
		bcc1 = ((bcc & 0xf0) >> 4);		/* 1BYTE����ASCII�ϊ� */
		if((bcc1 >= 0) && (bcc1 <= 9)){
			/* 0�`9 */
			bcc1 += '0';
		}else{
			/* A�`F */
			bcc1 = (bcc1 - 0x0a) + 'A';
		}
		bcc2 = (bcc & 0x0f);				/* 1BYTE����ASCII�ϊ� */
		if((bcc2 >= 0) && (bcc2 <= 9)){
			/* 0�`9 */
			bcc2 += '0';
		}else{
			/* A�`F */
			bcc2 = (bcc2 - 0x0a) + 'A';
		}
		harai_mars[36] = bcc1;
		harai_mars[37] = bcc2;
		disp_log_cmd(harai_mars, SEND, LOG, sizeof(harai_mars));
		send_data = harai_mars;
		break;
	case H_HIROTAC:
		break;
	case H_SUBARU:
		break;
	case H_TMI:
		break;
	default:
		send_data = check1_glory_old;
		break;
	}

	rec = SendData(hComm, send_data);				/* �d�����M */
	if (rec == -1) {
		/* ���M�G���[ */
		return -1;
	} else {
		/* ���M���� */
		return 0;
	}
}

/*****************************************************************************/
/* 	disp_log_cmd																 */
/* 		�ް��\���ƃ��O�o��													 */
/* 			data, mode, size		�ް�, Ӱ��(0:DOS�\�� 1:۸ނ֏o��), ����	 */
/* 																			 */
/*****************************************************************************/
void disp_log_cmd(char *data, char mode1, char mode2, int size)
{
	return;

	int i;
	char dsp_buf[128], buf[128];
	FILE *fp;

	memset(dsp_buf, 0x00, sizeof(dsp_buf));
	memset(buf, 0x00, sizeof(buf));

	if(mode1 == RECEIVE){
		/* ��M�\�� */
		switch(mode2){
		case 0:
			/* DOS�֕\�� */
			break;
		case 1:
			/* ۸ނ֏o�� */
			fp = fopen("data.log", "a");
			if(rece_err_flag == TRUE){
				fputs("�ُ�d��:", fp);
			}
			if(size == sizeof(char)){
				switch(data[0]){
				case ACK:
					/* ACK��M�\�� */
					fputs("ACK��M\n", fp);
					break;
				case NAK:
					/* NAK��M�\�� */
					fputs("NAK��M\n", fp);
					break;
				case ENQ:
					/* ENQ��M�\�� */
					fputs("ENQ��M\n", fp);
					break;
				case EOT:
					/* EOT��M�\�� */
					fputs("EOT��M\n", fp);
					break;
				default:
					/* ���l��M�\�� */
					sprintf(buf, "%02x��M\n", data[0]);
					fputs(buf, fp);
					break;
				}
			}else{
				if(maker == H_MARS){
					if(data[0] == M_STX){
						fputs("CMD��M(", fp);
					}
				}else{
					if(data[0] == STX){
						fputs("CMD��M(", fp);
					}
				}
				for(i = 0; i < size-1; i++){
					if(maker == H_MARS){
						if(data[i] == M_STX){
							fputs("S", fp);
						}else if(data[i] == M_ETX){
							fputs("E", fp);
						}else{
							sprintf(dsp_buf, "%c", data[i]);
							fputs(dsp_buf, fp);
						}
					}else{
						if(data[i] == STX){
							fputs("STX", fp);
						}else if(data[i] == ETX){
							fputs("ETX", fp);
						}else{
							sprintf(dsp_buf, "%c", data[i]);
							fputs(dsp_buf, fp);
						}
					}
				}
				/* BCC�̂�16�i�\�� */
				sprintf(dsp_buf, "%02x", data[size-1]);
				fputs(dsp_buf, fp);
				if(maker == H_MARS){
					if(data[0] == M_STX){
						fputs(")", fp);
					}
				}else{
					if(data[0] == STX){
						fputs(")", fp);
					}
				}
				fputs("\n", fp);
			}
			fclose(fp);
			break;
		}
	}else{
		/* ���M�\�� */
		switch(mode2){
		case 0:
			/* DOS�֕\�� */
			if(data[0] == STX){
				printf("�@CMD���M(");
			}
			for(i = 0; i < size-1; i++){
				if(data[i] == STX){
					printf("STX");
				}else if(data[i] == ETX){
					printf("ETX");
#if 0
				}else if(data[i] == SPACE){
					/* �X�y�[�X�R�[�h�Ȃ̂Ŕ�΂� */
					continue;
#endif
				}else{
					sprintf(dsp_buf, "%c", data[i]);
					printf(dsp_buf);
				}
			}
			/* BCC�̂�16�i�\�� */
			sprintf(dsp_buf, "%02x", data[size-1]);
			printf(dsp_buf);
			if(data[0] == STX){
				printf(")");
			}
			printf("\n");
			break;
		case 1:
			/* ۸ނ֏o�� */
			fp = fopen("data.log", "a");
			if(maker == H_MARS){
				if(data[0] == M_STX){
					fputs("�@CMD���M(", fp);
				}
			}else{
				if(data[0] == STX){
					fputs("�@CMD���M(", fp);
				}
			}
			for(i = 0; i < size-1; i++){
				if(maker == H_MARS){
					if(data[i] == M_STX){
						fputs("S", fp);
					}else if(data[i] == M_ETX){
						fputs("E", fp);
					}else{
						sprintf(dsp_buf, "%c", data[i]);
						fputs(dsp_buf, fp);
					}
				}else{
					if(data[i] == STX){
						fputs("STX", fp);
					}else if(data[i] == ETX){
						fputs("ETX", fp);
					}else{
						sprintf(dsp_buf, "%c", data[i]);
						fputs(dsp_buf, fp);
					}
				}
			}
			/* BCC�̂�16�i�\�� */
			sprintf(dsp_buf, "%02x", data[size-1]);
			fputs(dsp_buf, fp);
			if(maker == H_MARS){
				if(data[0] == M_STX){
					fputs(")", fp);
				}
			}else{
				if(data[0] == STX){
					fputs(")", fp);
				}
			}
			fputs("\n", fp);
			fclose(fp);
			break;
		}
	}
}

/*****************************************************************************/
/* 	disp_log																 */
/* 		�ް��\���ƃ��O�o��(1BYTE���M�p)										 */
/* 			data, mode, size		�ް�, Ӱ��(0:DOS�\�� 1:۸ނ֏o��), ����	 */
/* 																			 */
/*****************************************************************************/
void disp_log( char data )
{
	int ret;
	FILE *fp;
	char buf[128];
	char log_data[128];
	int i, j;

	j = 0;
	memset(buf, 0x00, sizeof(buf));
	memset(log_data, 0x00, sizeof(log_data));
	fp = fopen("data.log", "a");
	/* ���M�\���p */
	switch(data){
	case ACK:
		/*printf("�@ACK���M\n");*/
		fputs("�@ACK���M\n", fp);
		break;
	case NAK:
		/*printf("�@NAK���M\n");*/
		fputs("�@NAK���M\n", fp);
		break;
	case ENQ:
		/*printf("�@ENQ���M\n");*/
		fputs("�@ENQ���M\n", fp);
		break;
	case SYN:
		/*printf("�@SYN���M\n");*/
		fputs("�@SYN���M\n", fp);
		break;
	case EOT:
		/*printf("�@EOT���M\n");*/
		fputs("�@EOT���M\n", fp);
		break;
	case CLR:
		/*printf("�@EOT���M\n");*/
		fputs("�@CLR���M\n", fp);
		break;
	case CAL:
		/*printf("�@EOT���M\n");*/
		fputs("�@CAL���M\n", fp);
		break;
	case NO_REC:
		/*printf("�@������\n");*/
		fputs("�@������\n", fp);
		break;
	case STATUS1:
		/* ��ԃ`�F�b�N */
		switch(maker){
		case H_GLORY_OLD:
		case H_GLORY_NEW:
			for(i=0;i<sizeof(check1_glory_new);i++){
				if(check1_glory_new[i] == STX){
					log_data[j] = 'S';
					log_data[j+1] = 'T';
					log_data[j+2] = 'X';
					j += 3;
				}else if(check1_glory_new[i] == ETX){
					log_data[j] = 'E';
					log_data[j+1] = 'T';
					log_data[j+2] = 'X';
					j += 3;
				}else{
					log_data[j] = check1_glory_new[i];
					j++;
				}
			}
			break;
		case H_MARS:
			for(i=0;i<sizeof(check_mars);i++){
				if(check_mars[i] == M_STX){
					log_data[i] = 'S';
				}else if(check_mars[i] == M_ETX){
					log_data[i] = 'E';
				}else{
					log_data[i] = check1_glory_new[i];
				}
			}
			break;
		case H_SANKYO:
			/* �O���͏�ԃ`�F�b�N���Ȃ� */
			break;
		default:
			break;
		}
		ret = _snprintf(buf, sizeof(buf) - 1, "�@CMD���M(%s)\n", log_data);
		fputs(buf, fp);
		break;
	case STATUS2:
		/* ��ԃ`�F�b�N */
		switch(maker){
		case H_GLORY_OLD:
		case H_GLORY_NEW:
			for(i=0;i<sizeof(check2_glory_new);i++){
				if(check2_glory_new[i] == STX){
					log_data[j] = 'S';
					log_data[j+1] = 'T';
					log_data[j+2] = 'X';
					j += 3;
				}else if(check2_glory_new[i] == ETX){
					log_data[j] = 'E';
					log_data[j+1] = 'T';
					log_data[j+2] = 'X';
					j += 3;
				}else{
					log_data[j] = check2_glory_new[i];
					j++;
				}
			}
			break;
		case H_MARS:
			for(i=0;i<sizeof(check_mars);i++){
				if(check_mars[i] == M_STX){
					log_data[i] = 'S';
				}else if(check_mars[i] == M_ETX){
					log_data[i] = 'E';
				}else{
					log_data[i] = check1_glory_new[i];
				}
			}
			break;
		case H_SANKYO:
			/* �O���͏�ԃ`�F�b�N���Ȃ� */
			break;
		default:
			break;
		}
		ret = _snprintf(buf, sizeof(buf) - 1, "�@CMD���M(%s)\n", log_data);
		fputs(buf, fp);
		break;
	default:

		ret = _snprintf(buf, sizeof(buf) - 1, "�@%02x���M\n", data);
		/*printf(buf);*/
		fputs(buf, fp);
		break;
	}
	fclose(fp);
}

/*****************************************************************************/
/* 	disp_harai																 */
/* 		�ް��\���ƃ��O�o��(1BYTE���M�p)										 */
/* 			data		�ް�												 */
/* 																			 */
/*****************************************************************************/
void disp_harai( char *data )
{
	int harai_num[2][4];	/* ���o�� */
	char disp_flag;
	static int call_num = 0;	/* ���o�� */

	memset(&(harai_num[0][0]), 0x00, sizeof(int)*4);	/* A�O���[�v�p */
	memset(&(harai_num[1][0]), 0x00, sizeof(int)*4);	/* B�O���[�v�p */

	/* ���[�J�[���ɓ��e�͈Ⴄ�̂Ń��[�J�[���ɓd���̉�͂��s�� */
	switch(maker){
	case H_GLORY_OLD:
		/* �f�[�^���略�o�����擾 */
		harai_num[0][0] += (data[2] - 0x30) * 1000;		/* A�i�i */
		harai_num[0][0] += (data[3] - 0x30) * 100;		/* A�i�i */
		harai_num[0][0] += (data[4] - 0x30) * 10;		/* A�i�i */
		harai_num[0][0] += (data[5] - 0x30) * 1;		/* A�i�i */
		harai_num[0][1] += (data[6] - 0x30) * 1000;		/* B�i�i */
		harai_num[0][1] += (data[7] - 0x30) * 100;		/* B�i�i */
		harai_num[0][1] += (data[8] - 0x30) * 10;		/* B�i�i */
		harai_num[0][1] += (data[9] - 0x30) * 1;		/* B�i�i */
		harai_num[0][2] += (data[10] - 0x30) * 1000;	/* C�i�i */
		harai_num[0][2] += (data[11] - 0x30) * 100;		/* C�i�i */
		harai_num[0][2] += (data[12] - 0x30) * 10;		/* C�i�i */
		harai_num[0][2] += (data[13] - 0x30) * 1;		/* C�i�i */
		harai_num[0][3] += (data[14] - 0x30) * 1000;	/* D�i�i */
		harai_num[0][3] += (data[15] - 0x30) * 100;		/* D�i�i */
		harai_num[0][3] += (data[16] - 0x30) * 10;		/* D�i�i */
		harai_num[0][3] += (data[17] - 0x30) * 1;		/* D�i�i */
		disp_flag = TRUE;
		break;
	case H_GLORY_NEW:
		harai_num[0][0] += (data[2] - 0x30) * 1000;		/* A�i�i */
		harai_num[0][0] += (data[3] - 0x30) * 100;		/* A�i�i */
		harai_num[0][0] += (data[4] - 0x30) * 10;		/* A�i�i */
		harai_num[0][0] += (data[5] - 0x30) * 1;		/* A�i�i */
		harai_num[0][1] += (data[6] - 0x30) * 1000;		/* B�i�i */
		harai_num[0][1] += (data[7] - 0x30) * 100;		/* B�i�i */
		harai_num[0][1] += (data[8] - 0x30) * 10;		/* B�i�i */
		harai_num[0][1] += (data[9] - 0x30) * 1;		/* B�i�i */
		harai_num[0][2] += (data[10] - 0x30) * 1000;	/* C�i�i */
		harai_num[0][2] += (data[11] - 0x30) * 100;		/* C�i�i */
		harai_num[0][2] += (data[12] - 0x30) * 10;		/* C�i�i */
		harai_num[0][2] += (data[13] - 0x30) * 1;		/* C�i�i */
		harai_num[0][3] += (data[14] - 0x30) * 1000;	/* D�i�i */
		harai_num[0][3] += (data[15] - 0x30) * 100;		/* D�i�i */
		harai_num[0][3] += (data[16] - 0x30) * 10;		/* D�i�i */
		harai_num[0][3] += (data[17] - 0x30) * 1;		/* D�i�i */
		harai_num[1][0] += (data[18] - 0x30) * 1000;		/* E�i�i */
		harai_num[1][0] += (data[19] - 0x30) * 100;		/* E�i�i */
		harai_num[1][0] += (data[20] - 0x30) * 10;		/* E�i�i */
		harai_num[1][0] += (data[21] - 0x30) * 1;		/* E�i�i */
		harai_num[1][1] += (data[22] - 0x30) * 1000;		/* F�i�i */
		harai_num[1][1] += (data[23] - 0x30) * 100;		/* F�i�i */
		harai_num[1][1] += (data[24] - 0x30) * 10;		/* F�i�i */
		harai_num[1][1] += (data[25] - 0x30) * 1;		/* F�i�i */
		harai_num[1][2] += (data[26] - 0x30) * 1000;	/* G�i�i */
		harai_num[1][2] += (data[27] - 0x30) * 100;		/* G�i�i */
		harai_num[1][2] += (data[28] - 0x30) * 10;		/* G�i�i */
		harai_num[1][2] += (data[29] - 0x30) * 1;		/* G�i�i */
		harai_num[1][3] += (data[30] - 0x30) * 1000;	/* H�i�i */
		harai_num[1][3] += (data[31] - 0x30) * 100;		/* H�i�i */
		harai_num[1][3] += (data[32] - 0x30) * 10;		/* H�i�i */
		harai_num[1][3] += (data[33] - 0x30) * 1;		/* H�i�i */
		disp_flag = TRUE;
		break;
	case H_SANKYO:
		/* ���o�v���ł͕��o�������Ȃ� */
		disp_flag = FALSE;
		break;
	case H_MARS:
		break;
	case H_HIROTAC:
		PathAppend(CsvFilePath, "glory.csv");
		printf("���o�@���[�J�[�F�q���^�b�N\n");
		break;
	case H_SUBARU:
		PathAppend(CsvFilePath, "glory.csv");
		printf("���o�@���[�J�[�F�X�o��\n");
		break;
	case H_TMI:
		PathAppend(CsvFilePath, "glory.csv");
		printf("���o�@���[�J�[�FTMI\n");
		break;
	default:
		PathAppend(CsvFilePath, "glory.csv");
		printf("���o�@���[�J�[�F�O���[���[(�f�t�H���g)\n");
		break;
	}

	if(disp_flag == TRUE){
		sum_harai[0][0] += harai_num[0][0];
		sum_harai[0][1] += harai_num[0][1];
		sum_harai[0][2] += harai_num[0][2];
		sum_harai[0][3] += harai_num[0][3];
		sum_harai[1][0] += harai_num[1][0];
		sum_harai[1][1] += harai_num[1][1];
		sum_harai[1][2] += harai_num[1][2];
		sum_harai[1][3] += harai_num[1][3];
		call_num++;
		printf("�����o�i�i�@�@�@�@�@�@���񕥏o��(����%3d���)\n", call_num);
		printf("�@���o�i�i1 =%4d�@�@�@�@���o�i�i1 =%4d\n", sum_harai[0][0], harai_num[0][0]);
		printf("�@���o�i�i2 =%4d�@�@�@�@���o�i�i2 =%4d\n", sum_harai[0][1], harai_num[0][1]);
		printf("�@���o�i�i3 =%4d�@�@�@�@���o�i�i3 =%4d\n", sum_harai[0][2], harai_num[0][2]);
		printf("�@���o�i�i4 =%4d�@�@�@�@���o�i�i4 =%4d\n", sum_harai[0][3], harai_num[0][3]);
		if(maker == H_GLORY_NEW){
			printf("�@���o�i�i5 =%4d�@�@�@�@���o�i�i5 =%4d\n", sum_harai[1][0], harai_num[1][0]);
			printf("�@���o�i�i6 =%4d�@�@�@�@���o�i�i6 =%4d\n", sum_harai[1][1], harai_num[1][1]);
			printf("�@���o�i�i7 =%4d�@�@�@�@���o�i�i7 =%4d\n", sum_harai[1][2], harai_num[1][2]);
			printf("�@���o�i�i8 =%4d�@�@�@�@���o�i�i8 =%4d\n", sum_harai[1][3], harai_num[1][3]);
		}
	}
}

/*****************************************************************************/
/* 	disp_cal																 */
/* 		�ް��\���ƃ��O�o��(1BYTE���M�p)										 */
/* 			data		�ް�												 */
/* 																			 */
/*****************************************************************************/
void disp_cal( char *data )
{
	char year, mon, day, hour, min, sec;

	/* ������ */
	year = 0;	/* �N */
	mon = 0;	/* �� */
	day = 0;	/* �� */
	hour = 0;	/* �� */
	min = 0;	/* �� */
	sec = 0;	/* �b */

	/* ���t */
	year += (data[4] - '0') * 10;
	year += (data[5] - '0');
	mon += (data[6] - '0') * 10;
	mon += (data[7] - '0');
	day += (data[8] - '0') * 10;
	day += (data[9] - '0');
	hour += (data[10] - '0') * 10;
	hour += (data[11] - '0');
	min += (data[12] - '0') * 10;
	min += (data[13] - '0');
	sec += (data[14] - '0') * 10;
	sec += (data[15] - '0');
	printf("�J�����_�Z�b�g�v����M(%02d�N%02d��%02d���@%02d��%02d��%02d�b�F", year, mon, day, hour, min, sec);

	/* �j�� */
	switch(data[3]){
	case '0':
		/* ���j�� */
		printf("���j��)\n");
		break;
	case '1':
		/* ���j�� */
		printf("���j��)\n");
		break;
	case '2':
		/* �Ηj�� */
		printf("�Ηj��)\n");
		break;
	case '3':
		/* ���j�� */
		printf("���j��)\n");
		break;
	case '4':
		/* �ؗj�� */
		printf("�ؗj��)\n");
		break;
	case '5':
		/* ���j�� */
		printf("���j��)\n");
		break;
	case '6':
		/* �y�j�� */
		printf("�y�j��)\n");
		break;
	default:
		break;
	}
}

/*****************************************************************************/
/* 	make_tanka_mars															 */
/* 		�P���������쐬����(��{�I�ɂ͒P����ۑ����Ă��̂܂܃I�E���Ԃ�)		 */
/* 			data		�ް�												 */
/* 																			 */
/*****************************************************************************/
void make_tanka_mars( char *data )
{
	int i;
	char mars_lane_buf[LANE_MAX];
	char cnt1, cnt2, cnt3, cnt4;

	/* ������ */
	cnt1 = 0;
	cnt2 = 0;
	cnt3 = 0;
	cnt4 = 0;
	memset(mtanka, 0x00, sizeof(mtanka));
	memset(mars_lane_buf, 0x00, sizeof(mars_lane_buf));
	for(i=0;i<TOKUSHU_MAX;i++){
		memset(&(mars_lane[i][0]), 0x00, sizeof(mars_lane[i]));
	}

	/* �e����i�i�̒P�����L�� */
	for(i = 0; i < TOKUSHU_MAX; i++){
		mtanka[i] += (data[11+i*6] - '0') * 100000;
		mtanka[i] += (data[12+i*6] - '0') * 10000;
		mtanka[i] += (data[13+i*6] - '0') * 1000;
		mtanka[i] += (data[14+i*6] - '0') * 100;
		mtanka[i] += (data[15+i*6] - '0') * 10;
		mtanka[i] += (data[16+i*6] - '0');
	}

	/* �e����i�i�̎g�p���郌�[�����L�� */
	for(i = 0; i < LANE_MAX; i++){
		mars_lane_buf[i] = data[5+30+i] - '0';
	}
	/* �e����i�i�̎g�p���郌�[�������Ɋe�i�i�ł܂Ƃ߂Ă��܂� */
	for(i = 0; i < LANE_MAX; i++){
		switch(mars_lane_buf[i]){
		case 0:
			/* ���g�p���[�� */
			continue;
			break;
		case 1:
			/* �i�i1�Ŏg�p */
			mars_lane[0][cnt1] = i;
			cnt1++;
			break;
		case 2:
			/* �i�i2�Ŏg�p */
			mars_lane[1][cnt2] = i;
			cnt2++;
			break;
		case 3:
			/* �i�i3�Ŏg�p */
			mars_lane[2][cnt3] = i;
			cnt3++;
			break;
		case 4:
			/* �i�i4�Ŏg�p */
			mars_lane[3][cnt4] = i;
			cnt4++;
			break;
		default:
			continue;
			break;
		}
	}
}

/*****************************************************************************/
/* 	make_harai_sankyo														 */
/* 		���o�������쐬����													 */
/* 			data		�ް�												 */
/* 																			 */
/*****************************************************************************/
void make_harai_sankyo( char *data )
{
	long rest_ball;		/* �]��� */
	unsigned long tokushu_num[TOKUSHU_MAX];	/* ����i�i�� */
	unsigned long tokushu_num_buf;			/* ����i�i���̌v�Z�p�ϐ� */
	int sum_tama;		/* ���ʐ� */
	int lane_tama[LANE_MAX];	/* �e���[���̌����ʐ� */
	int lane_put[TOKUSHU_MAX][LANE_MAX];	/* �e���[���̌����ʐ� */
	char send_tokushu_num[TOKUSHU_MAX][4];	/* ���M���o���i�[�p�o�b�t�@ */
	unsigned long disp_lane_harai[LANE_MAX];
	int i, j;
	int cnt;
	char bcc;
	static char disp_first_flag = TRUE;				/* ����̉�ʕ\�� */
	static int cnt1 = 0;
	static int cnt2 = 0;
	static int cnt3 = 0;
	static int cnt4 = 0;

	union {
	  T_EXTRA_OFFER_BALL OfferBall;
	  unsigned long offer_ball[TOKUSHU_MAX];
	} ext;		/* ����i�i�P�� */

	/* ������ */
	sum_tama = 0;
	cnt = 0;
	j = 0;
	bcc = 0;
	memset(lane_tama, 0x00, sizeof(lane_tama));
	for(i=0;i<TOKUSHU_MAX;i++){
		memset(&lane_put[i][0], 0x00, sizeof(lane_put[0]));
	}
	memset(&ext.OfferBall, 0x00, sizeof(T_EXTRA_OFFER_BALL));
	memset(disp_lane_harai, 0x00, sizeof(disp_lane_harai));

	/* ���ʐ��̎擾 */
	if((harai_data[1] >= '0') && (harai_data[1] <= '9')){
		/* '0'�`'9' */
		sum_tama += (harai_data[1] - '0') << 16;
	}else{
		/* 'A'�`'F' */
		sum_tama += (harai_data[1] - 'A' + 0x0a) << 16;
	}
	if((harai_data[2] >= '0') && (harai_data[2] <= '9')){
		sum_tama += (harai_data[2] - '0') << 12;
	}else{
		sum_tama += (harai_data[2] - 'A' + 0x0a) << 12;
	}
	if((harai_data[3] >= '0') && (harai_data[3] <= '9')){
		sum_tama += (harai_data[3] - '0') << 8;
	}else{
		sum_tama += (harai_data[3] - 'A' + 0x0a) << 8;
	}
	if((harai_data[4] >= '0') && (harai_data[4] <= '9')){
		sum_tama += (harai_data[4] - '0') << 4;
	}else{
		sum_tama += (harai_data[4] - 'A' + 0x0a) << 4;
	}
	if((harai_data[5] >= '0') && (harai_data[5] <= '9')){
		sum_tama += (harai_data[5] - '0');
	}else{
		sum_tama += (harai_data[5] - 'A' + 0x0a);
	}

	/* �S8���[���̒P�����擾���� */
	for(i = 0; i < LANE_MAX; i++){
		if((harai_data[6+i*4] >= '0') && (harai_data[6+i*4] <= '9')){
			lane_tama[i] += (harai_data[6+i*4] - '0') << 12;
		}else{
			lane_tama[i] += (harai_data[6+i*4] - 'A' + 0x0a) << 12;
		}
		if((harai_data[7+i*4] >= '0') && (harai_data[7+i*4] <= '9')){
			lane_tama[i] += (harai_data[7+i*4] - '0') << 8;
		}else{
			lane_tama[i] += (harai_data[7+i*4] - 'A' + 0x0a) << 8;
		}
		if((harai_data[8+i*4] >= '0') && (harai_data[8+i*4] <= '9')){
			lane_tama[i] += (harai_data[8+i*4] - '0') << 4;
		}else{
			lane_tama[i] += (harai_data[8+i*4] - 'A' + 0x0a) << 4;
		}
		if((harai_data[9+i*4] >= '0') && (harai_data[9+i*4] <= '9')){
			lane_tama[i] += (harai_data[9+i*4] - '0');
		}else{
			lane_tama[i] += (harai_data[9+i*4] - 'A' + 0x0a);
		}
	}

	/* �e����i�i�̒P�����擾 */
	for(i = 0; i < LANE_MAX; i++){
		if(j == 0){
			/* �������ŕۑ� */
			ext.offer_ball[j] = lane_tama[i];
			lane_put[j][0] = i+1;
			j++;
			continue;
		}
		if(lane_tama[i] == 0){
			/* ���[���̌����ʐ���0�̏ꍇ�͏I�� */
			break;
		}
		if(lane_tama[i-1] != lane_tama[i]){
			/* ��������Ȃ���Εۑ����� */
			ext.offer_ball[j] = lane_tama[i];
			lane_put[j][0] = i+1;
			j++;
			cnt = 0;
		}else{
			/* �����̏ꍇ�̓��[�����̂݋L�� */
			cnt++;
			lane_put[j-1][cnt] = i+1;
		}
	}

	/* ����i�i�����v�Z���� �� �]��ʂ̎Z�o */
	rest_ball = AutoOffer(sum_tama, &ext.OfferBall, (T_EXTRA_OFFER *)tokushu_num);


	/* ���[�������[�e�[�V���������Ĉ������Ƃ� */
	/* �ő�i�i */
	if(lane_put[0][cnt1] == 0){
		/* �ŏ��̃��[���ɖ߂� */
		cnt1 = 0;
	}
	/* 2�Ԗڌi�i */
	if(lane_put[1][cnt2] == 0){
		/* �ŏ��̃��[���ɖ߂� */
		cnt2 = 0;
	}
	/* 3�Ԗڌi�i */
	if(lane_put[2][cnt3] == 0){
		/* �ŏ��̃��[���ɖ߂� */
		cnt3 = 0;
	}
	/* 4�Ԗڌi�i */
	if(lane_put[3][cnt4] == 0){
		/* �ŏ��̃��[���ɖ߂� */
		cnt4 = 0;
	}

	disp_lane_harai[(lane_put[0][cnt1])-1] = tokushu_num[0];	/* ����i�i���� */
	disp_lane_harai[(lane_put[1][cnt2])-1] = tokushu_num[1];	/* ����i�i�� */
	disp_lane_harai[(lane_put[2][cnt3])-1] = tokushu_num[2];	/* ����i�i�� */
	disp_lane_harai[(lane_put[3][cnt4])-1] = tokushu_num[3];	/* ����i�i�� */
	sum_harai[0][0] += tokushu_num[0];
	sum_harai[0][1] += tokushu_num[1];
	sum_harai[0][2] += tokushu_num[2];
	sum_harai[0][3] += tokushu_num[3];

	for(i = 0; i < TOKUSHU_MAX; i++){
		tokushu_num_buf = tokushu_num[i] & 0x0000f000;	/* short�^�ϊ� */
		send_tokushu_num[i][0] = (char)(tokushu_num_buf >> 12);	/* bit���� */
		if((send_tokushu_num[i][0] >= 0x0a) && (send_tokushu_num[i][0] <= 0x0f)){
			send_tokushu_num[i][0] += 'A';
		}else{
			send_tokushu_num[i][0] += '0';
		}

		tokushu_num_buf = tokushu_num[i] & 0x00000f00;	/* short�^�ϊ� */
		send_tokushu_num[i][1] = (char)(tokushu_num_buf >> 8);	/* bit���� */
		if((send_tokushu_num[i][1] >= 0x0a) && (send_tokushu_num[i][1] <= 0x0f)){
			send_tokushu_num[i][1] += 'A';
		}else{
			send_tokushu_num[i][1] += '0';
		}

		tokushu_num_buf = tokushu_num[i] & 0x000000f0;	/* short�^�ϊ� */
		send_tokushu_num[i][2] = (char)(tokushu_num_buf >> 4);	/* bit���� */
		if((send_tokushu_num[i][2] >= 0x0a) && (send_tokushu_num[i][2] <= 0x0f)){
			send_tokushu_num[i][2] += 'A';
		}else{
			send_tokushu_num[i][2] += '0';
		}

		tokushu_num_buf = tokushu_num[i] & 0x0000000f;	/* short�^�ϊ� */
		send_tokushu_num[i][3] = (char)tokushu_num_buf;	/* bit���� */
		if((send_tokushu_num[i][3] >= 0x0a) && (send_tokushu_num[i][3] <= 0x0f)){
			send_tokushu_num[i][3] += 'A';
		}else{
			send_tokushu_num[i][3] += '0';
		}
	}

	/* �e���[���̕��o���ɃZ�b�g���� */
	data[1+((lane_put[0][cnt1]-1)*4)] = send_tokushu_num[0][0];
	data[2+((lane_put[0][cnt1]-1)*4)] = send_tokushu_num[0][1];
	data[3+((lane_put[0][cnt1]-1)*4)] = send_tokushu_num[0][2];
	data[4+((lane_put[0][cnt1]-1)*4)] = send_tokushu_num[0][3];
	data[1+((lane_put[1][cnt2]-1)*4)] = send_tokushu_num[1][0];
	data[2+((lane_put[1][cnt2]-1)*4)] = send_tokushu_num[1][1];
	data[3+((lane_put[1][cnt2]-1)*4)] = send_tokushu_num[1][2];
	data[4+((lane_put[1][cnt2]-1)*4)] = send_tokushu_num[1][3];
	data[1+((lane_put[2][cnt3]-1)*4)] = send_tokushu_num[2][0];
	data[2+((lane_put[2][cnt3]-1)*4)] = send_tokushu_num[2][1];
	data[3+((lane_put[2][cnt3]-1)*4)] = send_tokushu_num[2][2];
	data[4+((lane_put[2][cnt3]-1)*4)] = send_tokushu_num[2][3];
	data[1+((lane_put[3][cnt4]-1)*4)] = send_tokushu_num[3][0];
	data[2+((lane_put[3][cnt4]-1)*4)] = send_tokushu_num[3][1];
	data[3+((lane_put[3][cnt4]-1)*4)] = send_tokushu_num[3][2];
	data[4+((lane_put[3][cnt4]-1)*4)] = send_tokushu_num[3][3];

	/* BCC�v�Z */
	for(i = 1; i < (sizeof(data)) - 3; i++){
		bcc ^= data[i];
	}

	/* ���[���P���Ƒ��ʐ��A���[�����̌������A�i�i���̌�������\�������� */
	if(disp_first_flag == TRUE){
		printf("���[���P:%8d�@�@���[���T:%8d\n", lane_tama[0], lane_tama[4]);
		printf("���[���Q:%8d�@�@���[���U:%8d\n", lane_tama[1], lane_tama[5]);
		printf("���[���R:%8d�@�@���[���V:%8d\n", lane_tama[2], lane_tama[6]);
		printf("���[���S:%8d�@�@���[���W:%8d\n", lane_tama[3], lane_tama[7]);
		printf("--------------------------------------------\n");
		disp_first_flag = FALSE;
	}
	printf("���͋ʐ��@:%8d\n", sum_tama);
	printf("���[���P:%8d�@�@���[���T:%8d�@�@(��)�i�i1���o��%6d\n", disp_lane_harai[0], disp_lane_harai[4], sum_harai[0][0]);
	printf("���[���Q:%8d�@�@���[���U:%8d�@�@(��)�i�i2���o��%6d\n", disp_lane_harai[1], disp_lane_harai[5], sum_harai[0][1]);
	printf("���[���R:%8d�@�@���[���V:%8d�@�@(��)�i�i3���o��%6d\n", disp_lane_harai[2], disp_lane_harai[6], sum_harai[0][2]);
	printf("���[���S:%8d�@�@���[���W:%8d�@�@(��)�i�i4���o��%6d\n", disp_lane_harai[3], disp_lane_harai[7], sum_harai[0][3]);
	cnt1++;
	cnt2++;
	cnt3++;
	cnt4++;

}

/*****************************************************************************/
/* 	make_harai_sankyo														 */
/* 		���o�������쐬����													 */
/* 			data		�ް�												 */
/* 																			 */
/*****************************************************************************/
void make_harai_mars( char *data, int cbSize )
{
	int sum_tama;		/* ���ʐ� */
	long rest_ball, rest_ball_buf;		/* �]��� */
	unsigned long tokushu_num[TOKUSHU_MAX];	/* ����i�i�� */
	unsigned long tokushu_num_buf;			/* ����i�i���̌v�Z�p�ϐ� */
	int i, j;
	union {
	  T_EXTRA_OFFER_BALL OfferBall;
	  unsigned long offer_ball[TOKUSHU_MAX];
	} ext;		/* ����i�i�P�� */

//	printf("cbSize = %d\n", cbSize);
//printf("HARAI END\n");
	/* ������ */
	sum_tama = 0;
	rest_ball = 0;
	rest_ball_buf = 0;
	tokushu_num_buf = 0;
	memset(tokushu_num, 0x00, sizeof(tokushu_num));

//printf("HARAI END\n");
	memcpy(&ext.offer_ball, mtanka, sizeof(ext.offer_ball));
//printf("HARAI END\n");

	/* ���ʐ��𐔒l�� */
	sum_tama += (harai_data[3] - '0') * 100000;
	sum_tama += (harai_data[4] - '0') * 10000;
	sum_tama += (harai_data[5] - '0') * 1000;
	sum_tama += (harai_data[6] - '0') * 100;
	sum_tama += (harai_data[7] - '0') * 10;
	sum_tama += (harai_data[8] - '0');

	/* ���ʐ��ƌi�i�P������i�i�����Z�o�A�܂���{�I�ɂ͗]���0�Ȃ̂ŃN���A���� */
	rest_ball = AutoOffer(sum_tama, &ext.OfferBall, (T_EXTRA_OFFER *)tokushu_num);
	rest_ball = 0;

	/*
	   �]��ɂ��čē��o�ł���悤�ɂ���
	   ���쒆�Ɉȉ��̓��͂ɉ����ē��삳����
	    1:�i�i1�𕥏o��-1���o���Ȃ�
	    2:�i�i2�𕥏o��-1���o���Ȃ�
	    3:�i�i3�𕥏o��-1���o���Ȃ�
	    4:�i�i4�𕥏o��-1���o���Ȃ�
	    ���i�i����0�̏ꍇ�͍ē��o���Ȃ�
	    ����x�ɕ����̌i�i�𕥏o���Ȃ������\
	    �����͂����񐔕�-1������
	*/
	/* ���͕����炷���o���̌v�Z */
	for(i = 0; i < TOKUSHU_MAX; i++){
		if(tokushu_num[i] == 0){
			/* ���o����0�̏ꍇ�͍s��Ȃ� */
			continue;
		}
		for(j = 0; j < keihin_put[i]; j++){
			if(tokushu_num[i] == 0){
				/* ���炵�Ă��������ɕ��o����0�ɂȂ����̂ōs��Ȃ� */
				continue;
			}
			tokushu_num[i]--;	/* ���o��-1 */
			/* ���炵�����͗]��ɂ��� */
			rest_ball += mtanka[i];
		}
	}

	/* �]����Z�b�g */
	rest_ball_buf = rest_ball;
	data[5] = (char)(tokushu_num_buf % 10) + '0';
	rest_ball_buf = rest_ball / 10;
	data[6] = (char)(tokushu_num_buf % 10) + '0';
	rest_ball_buf = rest_ball / 10;
	data[7] = (char)(tokushu_num_buf % 10) + '0';
	rest_ball_buf = rest_ball / 10;
	data[8] = (char)(tokushu_num_buf % 10) + '0';
	rest_ball_buf = rest_ball / 10;
	data[9] = (char)(tokushu_num_buf % 10) + '0';
	rest_ball_buf = rest_ball / 10;
	data[10] = (char)(tokushu_num_buf % 10) + '0';

	/* �}�[�X�̓��[�����֌W�Ȃ��̂ł��̂܂ܓd���ɃZ�b�g */
	for(i = 0; i < TOKUSHU_MAX; i++){
		tokushu_num_buf = tokushu_num[i];
		data[11+i*6] = (char)(tokushu_num_buf % 10) + '0';
		tokushu_num_buf = tokushu_num[i] / 10;
		data[12+i*6] = (char)(tokushu_num_buf % 10) + '0';
		tokushu_num_buf = tokushu_num[i] / 10;
		data[13+i*6] = (char)(tokushu_num_buf % 10) + '0';
		tokushu_num_buf = tokushu_num[i] / 10;
		data[14+i*6] = (char)(tokushu_num_buf % 10) + '0';
		tokushu_num_buf = tokushu_num[i] / 10;
		data[15+i*6] = (char)(tokushu_num_buf % 10) + '0';
		tokushu_num_buf = tokushu_num[i] / 10;
		data[16+i*6] = (char)(tokushu_num_buf % 10) + '0';
		/* �����o���ɑ�� */
		sum_harai[0][i] += tokushu_num[i];
	}

	/* ��ʕ\�� */
	printf("���͋ʐ�%6d\n", sum_tama);
	printf("(��)�i�i1���o��%6d�@�@(��)�i�i1���o��%6d(-%2d)\n", sum_harai[0][0], tokushu_num[0], keihin_put[0]);
	printf("(��)�i�i1���o��%6d�@�@(��)�i�i2���o��%6d(-%2d)\n", sum_harai[0][1], tokushu_num[1], keihin_put[1]);
	printf("(��)�i�i1���o��%6d�@�@(��)�i�i3���o��%6d(-%2d)\n", sum_harai[0][2], tokushu_num[2], keihin_put[2]);
	printf("(��)�i�i1���o��%6d�@�@(��)�i�i4���o��%6d(-%2d)\n", sum_harai[0][3], tokushu_num[3], keihin_put[3]);
	
}

/*MHederS **********************************************************************
*  Function ID		: MP*
*			Name	: AutoOffer ( )
*			Title	: ����i�i�����v�Z����
*			Summary :
*
*  Input Parameter	: unsigned long		 left_ball	�����v�Z�̑ΏۂƂ���ʐ�
*												=
*					  T_EXTRA_OFFER_BALL *OfferBall	�e����i�i�̌����ʐ�����`����Ă���̈��
*													�|�C���^
*												=
*					  T_EXTRA_OFFER		 *Offer		�i�i�̌������ʂ��i�[����̈�̃|�C���^
*												=
*
*  Output Parameter : unsigned long				�]��ʐ�
*												=
*			Comment : None
*MHederE *********************************************************************/
unsigned long AutoOffer(unsigned long left_ball,
						T_EXTRA_OFFER_BALL *OfferBall,
						T_EXTRA_OFFER *Offer)
{
	/* 1�i�i�����ʐ��i�[�ϐ��|�C���^ */
	/* �ő�4��ނ�0�����o���鎖�œ���i�i�̎�ސ��𔻒f���� */
	unsigned long *offer_ball_org= (unsigned long*)OfferBall;
	/* �i�i�������i�[�ϐ���int�̃|�C���^�ɕϊ� */
	unsigned long *offer = (unsigned long *)Offer;
	/* ����i�i���i�[�ϐ� */
	long ball;

	unsigned long offer_ball[TOKUSHU_MAX];

	/* �v�Z�I���̒l */
	unsigned long lcm, lcm_tmp;
	long gcd_min, tmp;
	long rmd_tmp;
	int i, j, k, l, cnt;
	unsigned int i_old=0;

	int	tkflag = 0;

	/* ����i�i��(a:�ő� �� d:�ŏ�) */
	unsigned long num[TOKUSHU_MAX];
	int chg_a_ball, chg_b_ball, chg_c_ball, chg_d_ball;
	unsigned long total_keihinsuu=0;

	/* �ϐ������� */
	memset(offer, 0x00, sizeof(T_EXTRA_OFFER));
	memset(offer_ball, 0, sizeof(offer_ball));
	gcd_min = 0;
	cnt = 0;

	/* ����i�i�̒P���`�F�b�N */
	for (i=0; i<TOKUSHU_MAX; i++)
	{
		/* �i�i�P�����P�ȏ�̏ꍇ�̂݌v�Z�ΏۂƂ��� */
		if (0 < offer_ball_org[i])
		{
			offer_ball[cnt] = offer_ball_org[i];
			cnt ++;		/* �L������i�i���̶��ı��� */
		}
	}

	/* �ŏ��i�i�P�����a��ʐ������Ȃ��ꍇreturn����	2004.04.07	kodama */
	if(offer_ball[cnt - 1] > left_ball)
	{
		return left_ball;
	}

	/* ��U�v�Z */
	offer[0] = left_ball / offer_ball[0];
	for (i=1; (i<TOKUSHU_MAX) && (0 != offer_ball[i]); i++) {
		lcm = get_lcm(offer_ball[i-1], offer_ball[i]);
		/*
		 *	�ŏ����{���̂P�̈�Check
		 *	�O�łȂ��Ƃ��͂O�ɂȂ�悤�ɂQor�P�O�{����
		 *
		 */
		lcm_tmp = lcm % 10;
		switch (lcm_tmp)
		{
			case 0:	/* 1�ʂ�0�����̂܂� */
				offer[i] = lcm / offer_ball[i] - 1;
				break;
			case 5:	/* 1�ʂ�5��2�{ */
				offer[i] = (lcm * 2) / offer_ball[i] - 1;
				break;
			default:/* ���̑���10�{ */
				offer[i] = (lcm * 10) / offer_ball[i] - 1;
				break;
		}
	}

	/* �ŏ��i�i�����O�̏ꍇ�͏I�� */
	if (0 == offer[cnt-1]) return (left_ball);

	/* �]��ʐ������v�Z */
	ball = left_ball - (offer[0] * offer_ball[0]);
	memset(num, 0, sizeof(num));
	num[0] = offer[0];


	/* (�ŏ���)�ő���񐔎Z�o */
	for (i=0; i <(cnt-1); i++)
	{
		tmp = get_gcd(offer_ball[i], offer_ball[cnt-1]);
		if (0 != tmp)
		{
			/* ������l�Ɣ�r���������ꍇ�͓��ւ��� */
			if (0 == gcd_min)
			{
				gcd_min = tmp;
			}
			else if (tmp < gcd_min)
			{
				gcd_min = tmp;
			}
		}
	}

	/* ����i�i�L�����ŏ������� */
	switch (cnt)
	{
		/*********************************************************************/
		/* �Q�i�i */
		case 2:
			for (i=offer[0]; i>=0; i--)
			{
				for (j=offer[1]; j>=0; j--)
				{
					/* ���܂�ɒ[���i�P�̈ʁj���łȂ��悤�ɂ���J�グ���� */
					chg_a_ball = i * offer_ball[0];
					chg_b_ball = j * offer_ball[1];

					rmd_tmp = left_ball - (chg_a_ball + chg_b_ball);

					/* �]�肪��������܂ő����� */
					if (0 > rmd_tmp) continue;

					/* �`�F�b�N */
					if (rmd_tmp <= ball)
					{
						ball = rmd_tmp;
						num[0] = i;
						num[1] = j;

						/* �]��O�܂��͍ő���񐔈ȉ��Ȃ�I�� */
						if ((0 == ball) || (ball < gcd_min)) goto end;
					}
				}
			}
			break;

		/*********************************************************************/
		/* �R�i�i */
		case 3:
			for (i=offer[0]; i>=0; i--)
			{
				/* �i�i���i�ꂽ�̂Ŕ����� */
				if(i_old != 0) goto end;
				for (j=offer[1]; j>=0; j--)
				{
					/* ��U�`�F�b�N */
					if (0 > (left_ball - i*offer_ball[0] - j*offer_ball[1]))
					{
						/* �ŏ��i�i�̃��[�v�܂ōs���K�v�Ȃ� */
						continue;
					}
					for (k=offer[2]; k>=0; k--)
					{
						chg_a_ball = i * offer_ball[0];
						chg_b_ball = j * offer_ball[1];
						chg_c_ball = k * offer_ball[2];

						rmd_tmp = left_ball - (chg_a_ball + chg_b_ball + chg_c_ball);

						/* �]�肪��������܂ő����� */
						if (0 > rmd_tmp) continue;

						/*
						 *	����i�i�œK���s��Ή�
						 *	�œK���̊
						 *	1.�]�肪���Ȃ�
						 *�@2.�i�i�����Ȃ�(�]�肪�ꏏ)
						 *�@3.�傫�Ȍi�i����(�]��ƌi�i�����ꏏ)
						 *
						 *	2008.08.08	osako
						 */

						/* �]�肪�ŏ��i�i�𒴂����烋�[�v�𒆎~ */
						if (rmd_tmp > (long)offer_ball[2])	break;

						tkflag = FALSE;
						if (rmd_tmp < ball) {
							tkflag = TRUE;
						} else if (rmd_tmp == ball) {
							if ( (total_keihinsuu == 0) || (total_keihinsuu > (unsigned long)(i+j+k)) ) {
								tkflag = TRUE;
							}
						}

						if (tkflag == TRUE)
						{
							ball = rmd_tmp;
							num[0] = i;
							num[1] = j;
							num[2] = k;

							if(total_keihinsuu == 0) (i_old = i);

							total_keihinsuu = i+j+k;
						}
					}
				}
			}
			break;

		/*********************************************************************/
		/* �S�i�i */
		case 4:
			for (i=offer[0]; i>=0; i--)
			{
				/* ���Ɍi�i���i�ꂽ�ꍇ�͔����� */
				if( i_old != 0 ) goto end;
				for (j=offer[1]; j>=0; j--)
				{
					/* ��U�`�F�b�N */
					if (0 > (left_ball - i*offer_ball[0] - j*offer_ball[1]))
					{
						/* ���̌i�i�̌v�Z���s���K�v�Ȃ� */
						continue;
					}
					for (k=offer[2]; k>=0; k--)
					{
						/* ��U�`�F�b�N */
						if (0 > (left_ball - i*offer_ball[0] - j*offer_ball[1] - k*offer_ball[2]))
						{
							continue;
						}

						for (l=offer[3]; l>=0; l--)
						{
							rmd_tmp = left_ball - i*offer_ball[0] - j*offer_ball[1] - k*offer_ball[2] - l*offer_ball[3];


							/* ���܂�ɒ[���i�P�̈ʁj���łȂ��悤�ɂ���J�グ���� */
							chg_a_ball = i * offer_ball[0];
							chg_b_ball = j * offer_ball[1];
							chg_c_ball = k * offer_ball[2];
							chg_d_ball = l * offer_ball[3];

							rmd_tmp = left_ball - (chg_a_ball + chg_b_ball + chg_c_ball + chg_d_ball);

							/* �]�肪�o��܂ő����� */
							if (0 > rmd_tmp) continue;

							/*
							 *	����i�i�œK���s��Ή�
							 *	�œK���̊
							 *	1.�]�肪���Ȃ�
							 *�@2.�i�i�����Ȃ�(�]�肪�ꏏ)
							 *�@3.�傫�Ȍi�i����(�]��ƌi�i�����ꏏ)
							 *
							 *	2008.08.08	osako
							 */

							/* �]�肪�ŏ��i�i�𒴂����烋�[�v�𒆎~ */
							if (rmd_tmp > (long)offer_ball[3])	break;

							tkflag = FALSE;
							if (rmd_tmp < ball) {
								/* �]�肪���܂łň�ԏ��Ȃ� */
								tkflag = TRUE;
							} else if (rmd_tmp == ball) {
								/* �]�肪�ꏏ�Ȃ̂Ōi�i�������Ȃ����`�F�b�N */
								if ( (total_keihinsuu == 0) || (total_keihinsuu > (unsigned long)(i+j+k+l)) ) {
									/* �i�i������ԏ��Ȃ��ꍇ�Ə���͒ʂ� */
									tkflag = TRUE;
								}
							}

							if (tkflag == TRUE)
							{
								/* ��Ԃ��L�����Ă��� */
								ball = rmd_tmp;
								num[0] = i;
								num[1] = j;
								num[2] = k;
								num[3] = l;

								/* total(�e�i�i�����v)�̌i�i������ԏ��Ȃ��Ƃ������߂邽�߂�
								   "�]���"�̈�ԏ��Ȃ����̓���A��̌i�i�����o���Ă����B
								   ���ꂼ��P�Â��炵���Ƃ���total�i�i���Ɣ�r
								*/
								if(total_keihinsuu == 0)
								{
									i_old = i;
								}

								total_keihinsuu = i+j+k+l;
							}
						}
					}
				}
			}
			break;
		/*********************************************************************/
	}

end:

	j = 0;
	for (i=0; i<TOKUSHU_MAX; i++)
	{
		/* �P���`�F�b�N */
		if (offer_ball_org[i] == offer_ball[j])
		{
			/* �P�������Ȃ瓯��i�i */
			offer[i] = num[j];
			j ++;
		}
		else
		{
			/* �P�����ƂȂ�̂ňႤ�i�i */
			offer[i] = 0;
		}
	}

	/* �]��ʐ���Ԃ� */
	return (ball/10);
}

/*
 *	�ŏ����{���̎Z�o
 */
int get_lcm(int ball_1, int ball_2)
{
	if ((0 == ball_1) || (0 == ball_2)) {
		/* �����ɂO������ꍇ��0 */
		return 0;
	}

	return ((ball_1 / get_gcd(ball_1, ball_2)) * ball_2);
}

/*
 * �ő���񐔂̎Z�o
 */
int get_gcd(int ball_1, int ball_2)
{
	if ((0 == ball_1) || (0 == ball_2)) {
		/* �����ɂO������ꍇ��0 */
		return 0;
	}

	/* ���[�N���b�h�̌ݏ��@��p���čő���񐔂��Z�o */
	while (ball_1 != ball_2) {
		if (ball_1 > ball_2) {
			ball_1 -= ball_2;
		}
		else {
			ball_2 -= ball_1;
		}
	}

	return ball_1;
}
