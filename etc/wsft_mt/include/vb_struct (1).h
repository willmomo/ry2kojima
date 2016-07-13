
#if !defined(__BV_STRUCT_H)
#define __VB_STRUCT_H

// fno1000: �V�X�e���ݒ�
typedef struct {
	long vn;				// �o�[�W����
	long model_code;		// ���f���R�[�h
	long tn_line;		// TN�ڑ�
	long replay_line;	// �䃊�v���C�@�ڑ�
	long keisuu_line;	// ��v���@�ڑ�
	long pointsys;		// �|�C���g�V�X�e���ڑ�
	long kaisvr;			// ����T�[�o�ڑ�
	long kaipc;			// ���PC�ڑ�
	long cashsvr;		// �L���b�V���T�[�o�ڑ�
	long jarnal;			// �W���[�i���ڑ�
	long hnsuu;			// �z�[���i�r�ڑ�
	long kugicho;		// �B���L��
	long chodama;		// ���ʗL��
	long futureland;		// FL�L��
	long rs;				// ����RS����
	u_char yobi2[34];		// �\��
	long mode;			// ��ԃ��[�h: 0=49���� 1=�A��
	long dai;			// �䐔
	long tn;				// TN��(���g�p)
	long sima;			// ����(���g�p)
	long floor;			// �t���A��(���g�p)
	long kinko;			// ���ɐ�(���g�p)
	long g_genkin;		// ���O�����̔��@��(���g�p)
	long g_card;			// ���O�J�[�h�̔��@��(���g�p)
	long g_keihin;		// ���O�i�i�@��(���g�p)
	long g_replay;		// ���O���v���C�@��(���g�p)
	long g_premium;		// ���O�v���~�A���@��(���g�p)
	long g_cardh;		// ���O�J�[�h���s�@(���g�p)
	long pos;			// POS��(���g�p)
	long jihan;			// ���̋@��(���g�p)
	long ta;				// ���̑�(���g�p)
	long pnl_tab_mask1;	// �p�l���^�u�}�X�N(ON�̎��^�u���\���ɂ���)
							// bit0:�S��(�Ƃ������O�̃^�u) bit1:�V�Y bit2:��V�Y
							// bit3:�哖�� bit4:�ُ� bit5:���
	long option1;		// �I�v�V����1(�\��)
	long option2;		// �I�v�V����2: �_�E�����[�h�ݒ�
							// 0:�ʏ�(Lv1->Lv2)
							// 1:�ʏ�+�����_�E�����[�hON
							// 2:Lv1->Lv2->Lv0
							// 3:Lv0 only(IPL1)
							// 4:Lv1 only(IPL2)
							// 5:Lv2 only(Appli)
	long option3;		// �I�v�V����3: �@��ُ팟�n�}�X�N�ڍ�(ON���}�X�N)
							// bit0:���v���C�ُ�
							// bit1:���v���C�ʐM�ُ�
							// bit2:Tn I/O�ʐM�ُ�
							// bit3:Tn �ʐM�ُ�
							// bit4:Hn �ʐM�ُ�
							// bit5:D2 �ʐM�ُ�
							// bit6:Dn �ʐM�ُ�
	long option4;		// �I�v�V����4(�\��)
	long option5;		// �I�v�V����5(�\��)
	long option6;		// �I�v�V����6(�\��)
	long option7;		// �I�v�V����7(�\��)
	long option8;		// �I�v�V����8(�\��)
	u_char yobi[16];		// �\��
} vb_fno1000_t;

typedef struct {
	int hoge;
} vb_fno1001_t;

#endif
