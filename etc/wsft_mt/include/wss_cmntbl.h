#if !defined(__WSS_CMNTBL_H)
#define __WSS_CMNTBL_H

#include "asdef.h"


// ���L������
typedef struct {
	long	now_date[4];		// ���ݔN�����j		[0] =�N�@[1] = ���@[2]=���@[3]= �j
	long	now_time[2];		// ���ݎ���			[0] =���@[1] = ��
	long	eigyo_date[4];		// �c�ƔN�����j		[0] =�N�@[1] = ���@[2]=���@[3]= �j
	long	eigyo_time[2];		// �c�Ǝ���   [0] =���@[1] = ��
	long	as_mode;			// �`�r�ڑ��L��		1 = �`�r�ʐM�L��@�@0 = �`�r�ʐM�Ȃ��i�P�́j
	long	settei_mode;		// �ݒ胂�[�h		0 = �^�p�ݒ�    1= �\�ߐݒ胂�[�h
	long	sys_setsw;		// �V�X�e���ݒ�r�v	0 = OFF  1 = ON
	long	now_keysw;		// ����J�M�r�v���	0 = OFF 1 = �c�ƂP�@2 = �ݒ�@3 = �c�ƂQ
	long	old_keysw;		// �O��J�M�r�v���	0 = OFF 1 = �c�ƂP�@2 = �ݒ�@3 = �c�ƂQ
	long	now_card;		// ����J�[�h���	0 = ����蒆    1 = �}����
	long	old_card;		// �O��J�[�h���	0 = ����蒆    1 = �}����
	long	card_syu;		// �}�����J�[�h���	1 = �����ڶ���  2 = �ݒ�Ӱ�ޕύX����
							//			3 = ���Ͻ��Ǎ��݁E�����ݶ���4 = �ް��ύX����
	char	cdata[64];		// ���ޓǂݎ���ް�
	long	now_proc;		// �����ʃv���Z�X	0 = NOP 1 = �^�p��۾�  2 = �Ɩ��ݒ���۾�
							//							3 = ���ѐݒ���۾�
	long	old_proc;		// �O���ʃv���Z�X	0 = NOP 1 = �^�p��۾�  2 = �Ɩ��ݒ���۾�
							//							3 = ���ѐݒ���۾�
	long	inji_sw	;		// �󎚂r�v���		0 = OFF   1 = ON
	long	ups_ijyo;		// �t�o�r�d���ُ�	0 = ����   1 = �ُ�
	long	touka_recv;		// ���߈󎚎�M�p���t���O	1 = �p��   0 = �ł�����
	long	wsscom_watch;	// �ʐM�v���Z�X�E�H�b�`�h�b�O�Ď��J�E���^
	long	tnio_watch;		// �s�m�h�n�v���Z�X�E�H�b�`�h�b�O�Ď��J�E���^
	long	syukei_watch;	// �W�v�v���Z�X�E�H�b�`�h�b�O�Ď��J�E���^
	long	touka_watch;	// ���߈󎚃v���Z�X�E�H�b�`�h�b�O�Ď��J�E���^
	long	ups_watch;		// �t�o�r�v���Z�X�E�H�b�`�h�b�O�Ď��J�E���^
	long	unyou_watch;	// �^�p��ʃv���Z�X�E�H�b�`�h�b�O�Ď��J�E���^
	long	sys_watch;		// �V�X�e���ݒ��ʃv���Z�X�E�H�b�`�h�b�O�Ď��J�E���^
	long	gyou_watch;		// �Ɩ��ݒ��ʃv���Z�X�E�H�b�`�h�b�O�Ď��J�E���^
	unsigned char	dflag;	// �����׸�
							//	0x01	 �N���A��:  */
                            //  0x02	 �L����:	*/
                            //  0x04	 �����L����:	*/
                            //  0x08	 ����������:	*/
                            //  0x10	 �b�r�t���b�V��������:  (�ǉ�:99/01/18:ohno) */
							//	0x20	 �ݒ�N���A��:  */
                            //  0x40	 �L���N���A��:  */
                            //  0x80	 �V�X�e���N���A��:  */
	unsigned char	dflag2;	// �����׸�2
							//	0x01	 Rs�_�E�����[�h��:  */
                            //  0x02	 Dn�_�E�����[�h��:  */
                            //  0x04	 Tn�_�E�����[�h��:  */
                            //  0x08	 Hn�_�E�����[�h��:  */
                            //  0x80	 �S�̈�ă_�E�����[�h��:
	unsigned char	eflag;	// �c���׸�
							//	0x80	 �J�X��:	*/
                            //  0x40	 �X�Ɩ���:	*/
                            //  0x00	 �X��:	*/
	unsigned char	emark;	// �c��ϰ�
							//	0x80	 �N���A��:  */
                            //  0x40	 �J�X��:	*/
                            //  0x20	 �X��:	*/
                            //  0x10	 XXX:	 */
                            //  0x08	�L����:	*/
	unsigned char	ksdflag;// �j�r�����׸�
							//	0x01	 LOG�ر��:  */
                            //  0x02	 LOG�L����:  */
                            //  0x04	 Ͻ��L����:  */
                            //  0x40	 KSSYR���쒆:  */
                            //  0x80	 KS���쒆:  */
	unsigned char	kpdflag;// ����o�b�����׸�
							//	0x01	 TKD�擾��:  */
                            //  0x02	 ���Ͻ��擾��:  */
                            //  0x80	 ���PC���쒆:  */
	unsigned char	ks_pt_seiri_fg;	//  0x01	KM�|�C���g������			2004.09.30 add hashimoto
									//  0x02	KM���ߒ�
									//  0x04	�L���b�V�������`�F�b�N��
									//  0x08	�L���b�V���o�b�N�A�b�v��
									//  0x10	�L���b�V���|�C���g������
									//  0x80	�`�F�b�N��
	unsigned char	wss_kioku_flag;	//	���암�L���t���O	0:OFF, 1:ON(���암�L����)
	unsigned char	pad1;			//	2009.09.03
	long	kioku_zumi;		// 0=���L���@�P���L���ς�
	unsigned short eigyou_min_cnt;		// �c�Ǝ��ԁi���j�N���A�E�J�X�łO
	short	SeigouErr;					// ���T���h�ݒ�Ƌ@�햼�ݒ�Ő������G���[�� 0=�װ�Ȃ� 1=�װ��
	short	Proc_Watch;		// �v���Z�X�E�H�b�`�h�b�O�^�C���A�E�g���
							// 0x0001	�ʐM�v���Z�X	�E�H�b�`�h�b�O�^�C���A�E�g��
							// 0x0002	�s�m�h�n�v���Z�X�E�H�b�`�h�b�O�^�C���A�E�g��
							// 0x0004	�W�v�v���Z�X	�E�H�b�`�h�b�O�^�C���A�E�g��
							// 0x0008	UPS�v���Z�X		�E�H�b�`�h�b�O�^�C���A�E�g��
	short	pad2;
	unsigned long	ks_pt_seiri_plan_ps_tim;	// �O��|�C���g�����\����t	2004.10.29 add fukuyama
	unsigned char	mpcdflag;					// MPC����t���O	2006.02.16 add hashimoto(T2X002759)
	unsigned char	pbnkdflag[2];				// PBANK����t���O	2006.02.16 add hashimoto(T2X002759)
	unsigned char	ms1kdflag;					// MS-1000����t���O	2006.06.05 add hashimoto(T2X002867)
	unsigned char	masdflag;					// Ͻ�����ޓ���t���O	2006.06.05 add hashimoto(T2X002867)
	unsigned char	dspdflag[2];				// DSP����t���O		2006.06.28 add hashimoto(T2X002907)
	unsigned char	lecdflag;					// GW���ޓ���t���O	2006.06.05 add hashimoto(T2X003143)
	unsigned long	clear_time[5];				// �N���A����	[0] =�N�@[1] = ���@[2]=���@[3]= ���@[4]= ��
	unsigned char	today_clear_zumi;			// �{���N���A�ς݃t���O�i�c�Ɠ��t���ς���Ă���A�N���A���ꂽ��1�A����ȊO��0�j

	unsigned char	hokyu_setflag;				// �⋋�@�ݒ�蓮�擾���� (bit0-7)��ٰ��A�`H [0]OK [1]NG
	unsigned char	estat[2];					// �c�Ə�� [0]A�X[1]B�X
												// (0x01)�c�Ɠ��t�ύX (0x02)�J�X (0x03)�X (0x04)�L��[�J�X��] (0x05)�L��[�J�X�O] (0xFE)�X�x�� (0xFF)�X�܂Ȃ�
	unsigned long	hokyuki_set_time[5];		// �⋋�@�ݒ�擾����	[0] =�N�@[1] = ���@[2]=���@[3]= ���@[4]= ��
	unsigned char	hokyu_dflag[8];				// �⋋�@A�`H����t���O
												// 0x80: 0=��ײݒ� 1=��ײݒ�
												// 0x40: 0=�⋋�@�ݒ�蓮�擾���łȂ� 1=�蓮�擾��
												// 0x20: 0=�⋋�@�ݒ莩���擾���łȂ� 1=�����擾��
	unsigned long	as_cpu_time;				// ASCPU����(AS�N������̒ʎZ�b)
	unsigned char	dflag3;						// ����t���O�R
	unsigned char	zc_dflag;					// �����N���A����t���O
	unsigned char	zc_set_tim[2];				// �����N���A�ݒ莞��(�{��)�@zc_set_tim[1]:��,zc_set_tim[2]:��

	unsigned char	gtage_dflg;					// GTage����t���O: (bit0)CSV�o��PC����t���O=0/1:�I�t���C��/���쒆
	unsigned char	as_dflag[2];				// AS����t���O: (bit0)AS#2�ڑ���� 0/1 = ���쒆(���ڑ�)/�I�t���C��
												//               (bit1)AS#3�ڑ���� 0/1 = ���쒆(���ڑ�)/�I�t���C��

	unsigned char	choflag;					// ���ʏ����t���O(bit0=1)�a���s��
	unsigned char	dmy[3];						// 

	//pasx�͈�ԍŌ�̂����łȂ��Ƃu�a���s������������ @koketsu
	//char	pasx[4][2];		// ��ʕ���		[0]=���P�̕��� [1]=���A�̕���		// ���g�p�̂��߃}�X�N		** HC2007 �萔�ύX **
	char	pasx_vb[SHUBETSU];		//				[2]=���S�̕��� [3]=���X�̕���									** HC2007 �萔�ύX **
	unsigned char	autoprn_cyu;				// 1:�呀�쎩���󎚒�(�����󎚋@�\������͎̂呀��̂�)

} wss_cmntbl_t;
#endif






