//---------------------------------------------------------------------
// AS�Ŏg�p���Ă���\����(�̃R�s�[)
// VB�Ŏg�p���邽�߂�vb_utype.h�Ƃ�����AC���x���Ŏg�p���邽�߂�
// vb_struct.h������܂��B

#if !defined(__AS_STRUCT_H)
#define __AS_STRUCT_H

// fno1000: �V�X�e���ݒ�
typedef struct {
	u_short vn;				// �o�[�W����
	u_short model_code;		// ���f���R�[�h
	u_short tn_line;		// TN�ڑ�
	u_short replay_line;	// �䃊�v���C�@�ڑ�
	u_short keisuu_line;	// ��v���@�ڑ�
	u_short pointsys;		// �|�C���g�V�X�e���ڑ�
	u_short kaisvr;			// ����T�[�o�ڑ�
	u_short kaipc;			// ���PC�ڑ�
	u_short cashsvr;		// �L���b�V���T�[�o�ڑ�
	u_short jarnal;			// �W���[�i���ڑ�
	u_short hnsuu;			// �z�[���i�r�ڑ�
	u_short kugicho;		// �B���L��
	u_short chodama;		// ���ʗL��
	u_short futureland;		// FL�L��
	u_short rs;				// ����RS����
	u_char yobi2[34];		// �\��
	u_short mode;			// ��ԃ��[�h: 0=49���� 1=�A��
	u_short dai;			// �䐔
	u_short tn;				// TN��(���g�p)
	u_short sima;			// ����(���g�p)
	u_short floor;			// �t���A��(���g�p)
	u_short kinko;			// ���ɐ�(���g�p)
	u_short g_genkin;		// ���O�����̔��@��(���g�p)
	u_short g_card;			// ���O�J�[�h�̔��@��(���g�p)
	u_short g_keihin;		// ���O�i�i�@��(���g�p)
	u_short g_replay;		// ���O���v���C�@��(���g�p)
	u_short g_premium;		// ���O�v���~�A���@��(���g�p)
	u_short g_cardh;		// ���O�J�[�h���s�@(���g�p)
	u_short pos;			// POS��(���g�p)
	u_short jihan;			// ���̋@��(���g�p)
	u_short ta;				// ���̑�(���g�p)
	u_short pnl_tab_mask1;	// �p�l���^�u�}�X�N(ON�̎��^�u���\���ɂ���)
							// bit0:�S��(�Ƃ������O�̃^�u) bit1:�V�Y bit2:��V�Y
							// bit3:�哖�� bit4:�ُ� bit5:���
	u_short option1;		// �I�v�V����1(�\��)
	u_short option2;		// �I�v�V����2: �_�E�����[�h�ݒ�
							// 0:�ʏ�(Lv1->Lv2)
							// 1:�ʏ�+�����_�E�����[�hON
							// 2:Lv1->Lv2->Lv0
							// 3:Lv0 only(IPL1)
							// 4:Lv1 only(IPL2)
							// 5:Lv2 only(Appli)
	u_short option3;		// �I�v�V����3: �@��ُ팟�n�}�X�N�ڍ�(ON���}�X�N)
							// bit0:���v���C�ُ�
							// bit1:���v���C�ʐM�ُ�
							// bit2:Tn I/O�ʐM�ُ�
							// bit3:Tn �ʐM�ُ�
							// bit4:Hn �ʐM�ُ�
							// bit5:D2 �ʐM�ُ�
							// bit6:Dn �ʐM�ُ�
	u_short option4;		// �I�v�V����4(�\��)
	u_short option5;		// �I�v�V����5(�\��)
	u_short option6;		// �I�v�V����6(�\��)
	u_short option7;		// �I�v�V����7(�\��)
	u_short option8;		// �I�v�V����8(�\��)
	u_char yobi[16];		// �\��
} fno1000_t;

typedef struct {
	int hoge;
} fno1001_t;

#endif
