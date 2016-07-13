typedef struct _csv_read{
	char			psend[48];		/* POS���M�o�b�t�@ */
	char			hsend[48];		/* ���o�@���M�o�b�t�@ */
}csv_read;			/* CSV�t�@�C������ǂݍ��ރf�[�^ */

typedef struct _cr_recv {
	char			seq;				/* �V�[�P���X�ԍ� */
	char			cmd;				/* �R�}���h */
	char			flag;				/* �t���O */
	unsigned long	kaiin_no[2];		/* ����ԍ� */
	unsigned short	ansyo;				/* �Ïؔԍ� */
	unsigned long	chotama;			/* �����ʐ� */
	char			harai[6];				/* ���o�\�x�� */
	unsigned long	point;				/* �|�C���g */
	int				event;				/* �C�x���g */
} cr_recv;			/* �󂯓n���p */

typedef struct _cr2tn_n {
	char			seq;			/* 1:	�V�[�P���X�ԍ� */
	char			ver[2];			/* 2:	�d���o�[�W���� */
	char			maker_code[2];	/* 4:	���[�J�[�R�[�h */
	char			cmd;			/* 6:	�R�}���h'N' */
	char			cmd_ex;			/* 7:	�R�}���h�g�� */
	char			cr_status[4];	/* 8:	CR�T���h��ԃX�e�[�^�X */
	char			req_cmd;		/* 12:	�v���R�}���h */
	char			card_kind;		/* 13:	�J�[�h��� */
	char			kai_no[16];		/* 14:	����ԍ� */
	char			s_zankin[4];	/* 30:	�i�X�e�B�b�N�j�c����z��� */
	char			s_riyoukin[4];	/* 34:	�i�X�e�B�b�N�j���p���z��� */
	char			d_nyukin[4];	/* 38:	�i��ώZ�j������� */
	char			d_riyokin[4];	/* 42:	�i��ώZ�j���p���z��� */
	char			crc[2];			/* 46:	CRC */
} cr2tn_n;		/* N�d���iCR�T���h��TV�i�r�j */

typedef struct _cr2tn_i {
	char	seq;			/* 1:	�V�[�P���X�ԍ� */
	char	ver[2];			/* 2:	�d���o�[�W���� */
	char	maker_code[2];	/* 4:	���[�J�[�R�[�h */
	char	cmd;			/* 6:	�R�}���h'I' */
	char	cmd_ex;			/* 7:	�R�}���h�g�� */
	char	cr_status[4];	/* 8:	CR�T���h��ԃX�e�[�^�X */
	char	req_cmd;		/* 12:	�v���R�}���h */
	char	card_kind;		/* 13:	�J�[�h��� */
	char	kai_no[16];		/* 14:	����ԍ� */
	char	nyukin[4];		/* 30:	������� */
	char	riyoukin[4];	/* 34:	���p���z��� */
	char	d_nyukin[4];	/* 38:	�i��ώZ�j������� */
	char	d_riyokin[4];	/* 42:	�i��ώZ�j���p���z��� */
	char	crc[2];			/* 46:	CRC */
} cr2tn_i;		/* I�d���iCR�T���h��TV�i�r�j */

typedef struct _cr2tn_r {
	char	seq;			/* 1:	�V�[�P���X�ԍ� */
	char	ver[2];			/* 2:	�d���o�[�W���� */
	char	maker_code[2];	/* 4:	���[�J�[�R�[�h */
	char	cmd;			/* 6:	�R�}���h'N' */
	char	cmd_ex;			/* 7:	�R�}���h�g�� */
	char	cr_status[4];	/* 8:	CR�T���h��ԃX�e�[�^�X */
	char	kai_no[16];		/* 12:	����ԍ� */
	char	harai_dosu[6];	/* 28:	���o�\�x�� */
	char	crc[2];			/* 34:	CRC */
} cr2tn_r;		/* R�d���iCR�T���h��TV�i�r�j */

typedef struct _cr2tn_o {
	char	seq;			/* 1:	�V�[�P���X�ԍ� */
	char	ver[2];			/* 2:	�d���o�[�W���� */
	char	maker_code[2];	/* 4:	���[�J�[�R�[�h */
	char	cmd;			/* 6:	�R�}���h'N' */
	char	cmd_ex;			/* 7:	�R�}���h�g�� */
	char	cr_status[4];	/* 8:	CR�T���h��ԃX�e�[�^�X */
	char	req_cmd;		/* 12:	�v���R�}���h */
	char	card_kind;		/* 13:	�J�[�h��� */
	char	kai_no[16];		/* 14:	����ԍ� */
	char	nyukin[4];		/* 30:	������� */
	char	riyoukin[4];	/* 34:	���p���z��� */
	char	d_nyukin[4];	/* 38:	�i��ώZ�j������� */
	char	d_riyokin[4];	/* 42:	�i��ώZ�j���p���z��� */
	char	crc[2];			/* 46:	CRC */
} cr2tn_o;		/* O�d���iTN��CR�T���h�j */

