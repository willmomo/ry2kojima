#ifndef __set_sys_h__
#define __set_sys_h__
/* WAIS START */

/*
 *	�V�X�e���ݒ�t�@�C��
 *		2002/05/31 osako		2�X��4���8�t���A�Ή�
 */

#include "nassys.h"
#include "asdef.h"
#define L_DAT_FILE
#define L_SET_SYS
 /****************************************/
 /*	���ݓ��͎��	*/
 /****************************************/
#define SCON_KINKO 1
#define SCON_KEIHIN 2
#define SCON_GENHAN 3
#define SCON_CARDHAN 4
#define SCON_REPLAY 5
#define SCON_PREMIUM 6
#define SCON_CDHAKKOU 7
#define SCON_POS 8
#define SCON_ZIHAN 9
#define SCON_TA 10
 /****************************************/
 /* ���ޔ��s�@��޽���   */
 /****************************************/
#define SCL_CDH01K 10
#define SCL_CDH02K 20
#define SCL_CDH03K 30
#define SCL_CDH05K 50
#define SCL_CDH10K 100

/* ���ѐݒ�:  */
typedef struct _fno1000 {
    unsigned short vn;          /* �ް�ޮ�:  */
    unsigned short model_code;  /* ���ٺ���:  */
    unsigned short tn_line;     /* TN�ڑ�:  */
    unsigned short replay_line; /* �����ڲ�@�ڑ�:  */
    unsigned short keisuu_line; /* ��v���@�ڑ�:  */
    unsigned short pointsys;    /* �߲�ļ��ѐڑ�:  */
    unsigned short kaisvr;      /* ����T�[�o�ڑ�:  */
    unsigned short kaipc;       /* ����o�b�ڑ�:  */
    unsigned short cashsvr;     /* �L���b�V���T�[�o�ڑ�:  */
    unsigned short jarnal;      /* �W���[�i���ڑ�:  */
    unsigned short hnsuu;       /* �z�[���i�r�ڑ�:  */
    unsigned short kugicho;     /* �B���L��:  */
    unsigned short chodama;     /* ���ʗL��:  */
    unsigned short futureland;  /* �e�k�L��:  */
	unsigned short rs;			/* ����RS���� */
    unsigned short ss;          /* �r�r�ڑ�:  */
    unsigned short card_tan;    /* �䖈�J�[�h�[���ڑ�:  */
    unsigned short lecsvr;      /* LEC�T�[�o�[�ڑ�:  */
    unsigned char yobi2[26];    /* �\��:  */
    unsigned short tenpo;       /* �X�ܐ�:  */
    unsigned short mode;        /* ���Ӱ��: (0)49�����(1)�A��  */
    unsigned short dai;         /* �䐔:  */
    unsigned short tn;          /* TN��: �g��Ȃ�  */
    unsigned short sima;        /* ����: �g��Ȃ�  */
    unsigned short floor;       /* �۱��: �g��Ȃ�  */
    unsigned short kinko;       /* ���ɐ�: �g��Ȃ�  */
    unsigned short g_genkin;    /* ���O�����̔��@��: �g��Ȃ�  */
    unsigned short g_card;      /* ���O���ޔ̔��@��: �g��Ȃ�  */
    unsigned short g_keihin;    /* ���O�i�i�@��: �g��Ȃ�  */
    unsigned short g_replay;    /* ���O���ڲ�@��: �g��Ȃ�  */
    unsigned short g_premium;   /* ���O���бы@��: �g��Ȃ�  */
    unsigned short g_cardh;     /* ���O���ޔ��s�@��: �g��Ȃ�  */
    unsigned short pos;         /* POS��: �g��Ȃ�  */
    unsigned short jihan;       /* ���̋@��: �g��Ȃ�  */
    unsigned short ta;          /* ���̑�: �g��Ȃ�  */
    unsigned short pnl_tab_mask1;/* �p�l���^�u�}�X�Nbit: �e�r�b�g��ON�̎��^�u���\���ɂ���(bit0:�S�� bit1:�V�Y bit2:��V�Y bit3:�哖�� bit4:�ُ� bit5:���)  */
    unsigned short option1;     /* �I�v�V�����P: dwtsk5���g�p                       */
                                /*  ���O  �����N����Ȃ�                            */
                                /*  ���O  ���J�o������������(���s�����Őݒ�֎~)*/
    unsigned short option2;     /* �I�v�V�����Q: �_�E�����[�h�ݒ�                   */
                                /*      0�F�ʏ�( Lv1 -> Lv2 )                       */
                                /*      1�F�ʏ� + �����_�E�����[�h ON             */
                                /*      2�FLv1 -> Lv2 -> Lv0                        */
                                /*      3�FLv0 only ( IPL1 )                        */
                                /*      4�FLv1 only ( IPL2 )                        */
                                /*      5�FLv2 only ( Appli )                       */
    unsigned short option3;     /* �I�v�V�����R: �@��ُ팟�m�}�X�N�ڍ�(ON���}�X�N) */
                                /*      bit 0�F���v���C�ُ�                         */
                                /*      bit 1�F���v���C�ʐM�ُ�                     */
                                /*      bit 2�FTn I/O �ʐM�ُ�                      */
                                /*      bit 3�FTn �ʐM�ُ�                          */
                                /*      bit 4�FHn �ʐM�ُ�                          */
                                /*      bit 5�FD2 �ʐM�ُ�                          */
                                /*      bit 6�FDn �ʐM�ُ�                          */
    unsigned short option4;     /* �I�v�V�����S: �J�����ڑ��ݒ�                     */
                                /*  ���O  �e�s�d�l                                  */
                                /*  ���P  �����d�l                                  */
                                /*  ���P  �e�s�d�l                                  */
    unsigned short option5;     /* �I�v�V�����T: �`�r�i�r�g�R�j����k�n�f�ݒ�       */
                                /*  ���O  �`�r���P�ɏW���L��                        */
                                /*  ���P  �`�r���P�C�Q�C�R�ŌʋL��                */
    unsigned short option6;     /* �I�v�V�����U: �����Q�X�܊Ǘ�                     */
                                /*  ���O  �P�X�܊Ǘ�                                */
                                /*  ���O  �Q�X�܊Ǘ��F�Q�X�ܖڐ擪��b�g�ԍ�        */
    unsigned short option7;     /* �I�v�V�����V: �����Q�X�܊Ǘ�                     */
                                /*  ���O  �P�X�܊Ǘ�                                */
                                /*  ���O  �Q�X�܊Ǘ��F�Q�X�ܖڐ擪�S�X�b�g�ԍ�      */
    unsigned short option8;     /* �I�v�V�����W: ��t�@�C���̔���␳�l�̒P�ʐݒ�   */
                                /*  ���O  �P�~�P��                                  */
                                /*  ���O  �P�O�O�~�P��                              */
#if	1	/* 2002.06.17(mon) chg.ohnot */
    unsigned short option9;     /* �I�v�V�����X: reserve  */
    unsigned short option10;     /* �I�v�V�����P�O: reserve  */
    unsigned short option11;     /* �I�v�V�����P�P: reserve  */
    unsigned short option12;     /* �I�v�V�����P�Q: reserve  */
    unsigned short option13;     /* �I�v�V�����P�R: reserve  */
    unsigned short option14;     /* �I�v�V�����P�S: reserve  */
    unsigned short option15;     /* �I�v�V�����P�T: reserve  */
    unsigned short option16;     /* �I�v�V�����P�U: reserve  */
#else
    unsigned char yobi[16];     /* �\��:  */
#endif
} fno1000_t;

#define		F1001_V1	0		/* �c�m�R�P�䃂�f��    2000/02/18 Ohno */
#define		F1001_V2	1		/* �c�m�Q�Q�Q�䃂�f��  2000/02/18 Ohno */

/* ��ڑ��ݒ�:  */
typedef struct _fno1001 {
    unsigned char rsdn_nid;/* Rs�Dn.NID: (b765=1-7)Rs.NID�(b43210=1-31)Dn.NID  */
    unsigned char tn_nid;       /* TN NID:  */
    unsigned char d2_nid;  /* D2 NID: (b765=1-4)�߰Ĕԍ��(b43210=1-31)D2.NID  */
    unsigned char d2_subid;     /* D2 SUBID:  */
    unsigned short dno;         /* ��ԍ�:  */
    unsigned char pas;          /* ���:  */
    unsigned char kishu;        /* �@��ԍ�:  */
    unsigned char sima;         /* ���ԍ�:  */
    unsigned char type;         /* ���ߔԍ�:  */
    unsigned char floor;        /* �۱�ԍ�:  */
    unsigned char sh_lamp;/* ���[�����v�o��: (bit0/4)�߰�1 A/B�E(bit1/5)�߰�2 A/B�E(bit2/6)�߰�3 A/B�E(bit3/7)�߰�4 A/B  */
    unsigned char form;         /* ����`��: (1)1:1�(2)2:1  */
    unsigned char kinko;        /* ���ɔԍ�: (>0)���ɔԍ�  */
    unsigned char urisyu1; /* ����1���: (1)����(2)�������ɥ(3)���ޥ(4)��  */
        /*  1 = ����         */          
        /*  2 = ��������     */          
        /*  3 = ����         */          
        /*  4 = ��         */          
    char scale1;                /* ����1����:  */
    unsigned char urisyu2; /* ����2���: (1)����(2)�������ɥ(3)���ޥ(4)��  */
    char scale2;                /* ����2����:  */
    unsigned char urisyu3; /* ����3���: (1)����(2)�������ɥ(3)���ޥ(4)��  */
    char scale3;                /* ����3����:  */
} fno1001_t;


/* �S�X�ڑ��ݒ�:  */
typedef struct _fno1002 {
    unsigned char rsdn_nid;/* Rs�Dn.NID: (b765=1-7)Rs.NID�(b43210=1-31)Dn.NID  */
    unsigned char d2_nid;     /* D2 NID: (b765=1-4)�߰Ĕԍ��(b43210=0)D2.NID  */
    unsigned char d2_pi;        /* D2 Pi����Chno.:  */
    unsigned char d2_err;       /* D2 �װ����Chno.:  */
    unsigned short disp1;       /* �\���ԍ�1:  */
    unsigned short disp2;       /* �\���ԍ�2:  */
    unsigned char floor;        /* �۱�ԍ�:  */
    unsigned char kind;         /* ���:  */
        /*   1 = ����                  */          
        /*   2 = �i�i�@                */          
        /*   3 = �����̔��@            */          
        /*   4 = ���ޔ̔��@            */          
        /*   5 = ���ڲ��               */          
        /*   6 = ���бы�              */          
        /*   7 = ���ޔ��s�@            */          
        /*   8 = POS                   */          
        /*   9 = ���̋@                */          
        /*  >9 = ���̑�                */          
    unsigned char inp;          /* ����:  */
        /*   0x01 = �v�� Pi            */          
        /*   0x02 = �f�� Di            */          
        /*   0x04 = �װ Pi            */          
        /*   0x08 = �ޱ Di            */          
    unsigned char pas;          /* ���:  */
    char scale;                 /* ����:  */
    char yobi[3];               /* �\��:  */
} fno1002_t;


/* ���[���ߐݒ�:  */
typedef struct _fno1003 {
    unsigned char lamp1;/* 1������: (0)���(1)�ďo�(2)���܂P�(3)�Ŏ~�(4)�s���(5)�s��/�ُ�(6)ܺ�ݥ(7)�m�ϥ(8)���܂Q  */
    unsigned char lamp2;/* 2������: (0)���(1)�ďo�(2)���܂P�(3)�Ŏ~�(4)�s���(5)�s��/�ُ�(6)ܺ�ݥ(7)�m�ϥ(8)���܂Q  */
    unsigned char lamp3;/* 3������: (0)���(1)�ďo�(2)���܂P�(3)�Ŏ~�(4)�s���(5)�s��/�ُ�(6)ܺ�ݥ(7)�m�ϥ(8)���܂Q  */
    unsigned char yobi[5];      /* �\��:  */
} fno1003_t;


/* ���ߐݒ�:  */
typedef struct _fno1004 {
    unsigned char sbt;          /* ���: */
    unsigned char sru;          /* ���ߔԍ�: 1�`16  */
    unsigned char name[TYPEMEICHO];     /* ���ߖ�:  */
} fno1004_t;


/* ������Րڑ��ݒ�:  */
typedef struct _fno1005 {
    unsigned char onsei1;	/* �t���A�ԍ��P�E�z�[���p	(0)���ڑ� (1~16)���M�批����ԍ� */
    unsigned char onsei2;	/* �t���A�ԍ��P�E�X���p		(0)���ڑ� (1~16)���M�批����ԍ� */
    unsigned char onsei3;	/* �t���A�ԍ��Q�E�z�[���p	(0)���ڑ� (1~16)���M�批����ԍ� */
    unsigned char onsei4;	/* �t���A�ԍ��Q�E�X���p		(0)���ڑ� (1~16)���M�批����ԍ� */
    unsigned char onsei5;	/* �t���A�ԍ��R�E�z�[���p	(0)���ڑ� (1~16)���M�批����ԍ� */
    unsigned char onsei6;	/* �t���A�ԍ��R�E�X���p		(0)���ڑ� (1~16)���M�批����ԍ� */
    unsigned char onsei7;	/* �t���A�ԍ��S�E�z�[���p	(0)���ڑ� (1~16)���M�批����ԍ� */
    unsigned char onsei8;	/* �t���A�ԍ��S�E�X���p		(0)���ڑ� (1~16)���M�批����ԍ� */
    unsigned char onsei9;	/* �t���A�ԍ��T�E�z�[���p	(0)���ڑ� (1~16)���M�批����ԍ� */
    unsigned char onsei10;	/* �t���A�ԍ��T�E�X���p		(0)���ڑ� (1~16)���M�批����ԍ� */
    unsigned char onsei11;	/* �t���A�ԍ��U�E�z�[���p	(0)���ڑ� (1~16)���M�批����ԍ� */
    unsigned char onsei12;	/* �t���A�ԍ��U�E�X���p		(0)���ڑ� (1~16)���M�批����ԍ� */
    unsigned char onsei13;	/* �t���A�ԍ��V�E�z�[���p	(0)���ڑ� (1~16)���M�批����ԍ� */
    unsigned char onsei14;	/* �t���A�ԍ��V�E�X���p		(0)���ڑ� (1~16)���M�批����ԍ� */
    unsigned char onsei15;	/* �t���A�ԍ��W�E�z�[���p	(0)���ڑ� (1~16)���M�批����ԍ� */
    unsigned char onsei16;	/* �t���A�ԍ��W�E�X���p		(0)���ڑ� (1~16)���M�批����ԍ� */
} fno1005_t;


/* �X�ܐڑ��ݒ� */
typedef struct _fno1006 {
	unsigned char floor[8];				/* �t���A�X�ܐڑ�(�W�t���A) (0)���ڑ� (1)A�X�� (2)B�X�� */
	unsigned char pmode_store;			/* �|�C���g�X�܍��Z (0)�X�܍��Z���Ȃ� (1)�X�܍��Z���� */
	unsigned char pmode_type;			/* �|�C���g��ʍ��Z			*/
										/*		bit0=1:���X�|�C���g	*/
										/*		bit1=1:�ғ��|�C���g	*/
										/*		bit2=1:����|�C���g	*/
	unsigned char dmy1[22];
} fno1006_t;


/* ��ʃ|�P�b�g�ݒ� */
typedef struct _fno1007 {
	unsigned char	btsbt;				/* ������� (0)���ڑ� (1)�� (2)���_�� */
	unsigned char	kmpkt;				/* KM�|�P�b�g�ʒu (0)���g�p (1~8)�ʒu */
	unsigned char	absout;				/* �A�E�g�萔 */
	unsigned char	inpscl;				/* ���͔�(1�p���X�ɑ΂����) */
	unsigned char	urikos;				/* ����P�ʌ�(��) */
	unsigned char	urikin;				/* ����P�ʋ��z(100�~�P��) */
	unsigned char	rnsbt;				/* �_����� */
	unsigned char	dmy[9];
} fno1007_t;


/* �X�܁E��ʖ��̐ݒ� */				/* 2002.11.29 add */
typedef struct _fno1008 {
	unsigned char	uni_name[12];			/* ���Z�� */
	unsigned char	tenpo1[12];			/* �X�ܖ��P */
	unsigned char	tenpo2[12];			/* �X�ܖ��Q */
	unsigned char	dmy1[36];			/* �\�� */
	unsigned char	sbt1[12];			/* ��ʖ��P */
	unsigned char	sbt2[12];			/* ��ʖ��Q */
	unsigned char	sbt3[12];			/* ��ʖ��R */
	unsigned char	sbt4[12];			/* ��ʖ��S */
	unsigned char	sbt5[12];			/* ��ʖ��T */
	unsigned char	sbt6[12];			/* ��ʖ��U */
	unsigned char	sbt7[12];			/* ��ʖ��V */
	unsigned char	sbt8[12];			/* ��ʖ��W */
	unsigned char	dmy2[24];			/* �\�� */
} fno1008_t;


typedef fno1000_t FSET_SYS_SYS;
typedef fno1001_t FSET_SYS_DAI;
typedef fno1002_t FSET_SYS_ZEN;
typedef fno1003_t FSET_SYS_LAMP;
typedef fno1004_t FSET_SYS_TYPE;
typedef fno1005_t FSET_SYS_ONS;			/* ������ڑ��ݒ� */
typedef fno1006_t FSET_SYS_TENPO;		/* F1006 �X�ܐڑ��ݒ� */
typedef fno1007_t FSET_SYS_POCKET;		/* F1007 ��ʃ|�P�b�g�ݒ� */

/* �V�X�e���ݒ� �I�v�V�����R */
#define SYSOPT3_RPY_IJO		0x0001
#define SYSOPT3_RPYT_IJO	0x0002
#define SYSOPT3_TNIO_IJO	0x0004
#define SYSOPT3_TN_IJO		0x0008
#define SYSOPT3_HN_IJO		0x0010
#define SYSOPT3_D2_IJO		0x0020
#define SYSOPT3_DN_IJO		0x0040


/* �|�C���g��ʍ��Z�t���O	*/
#define	P_RAI_GBIT 0x01
#define P_KAD_GBIT 0x02
#define P_URI_GBIT 0x04




/* WAIS END */
#endif                          /* __set_sys_h__ */
