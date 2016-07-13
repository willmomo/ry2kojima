/* WAIS START */
#ifndef __set_gyou_h__
#define __set_gyou_h__          /* �Ɩ��ݒ�:  */

 /*    �����ݒ�̧��                                   */              /* ���ڏ����f�[�^:  */
 /*    Version   Date      Programmer    Memo         */              /* ���ڏ����f�[�^:  */
 /*    00000.0   97.04.07      y.I       �V�K�쐬     */              /* ���ڏ����f�[�^:  */
 /*    �\����°قɂčč쐬                            */              /* ���ڏ����f�[�^:  */
 /*  1997/10/6 11:29 osako            */           /* ���ڏ����f�[�^:  */
#include "nassys.h"
#define L_SET_GYOU

/* �Ïؔԍ�:  */
typedef struct _fno1100 {
    unsigned long ansyou_no;    /* �Ïؔԍ�:  */
    unsigned long dmy1;         /* �\��:  */
} fno1100_t;


/* �x���ݒ��۱�ԍ�:  */
typedef struct _holidayfloor {
    unsigned char flag;         /* �g�p����g�p:  */
    unsigned char floor;        /* �۱No:  */
    unsigned char date[2];      /* ����:  */
} holidayfloor_t;


/* �J�X�����ݒ�:  */
typedef struct _fno1101 {
// 2002/10/25 (�X�ܕʋx���Ή�)�ǉ��C�� Fukuyama -->
    unsigned char open_time[2]; /* �J�X����:  */
    unsigned char close_time[2];/* �X����:  */
    unsigned char shut_time[2]; /* ���؎���:  */
    unsigned char kioku_time[2];/* �L������:  */
    unsigned char clear_time[2];/* (�ر����):  */
    unsigned char getstop_time[2];/* (�捞��~����):  */
    unsigned char revcany_time[2];/* ��\�����\������:  */
    unsigned char revcan_time[2];/* ��\��������:  */
    unsigned char open_time2[2]; /* �J�X����(2�X�ܖ�):  */
    unsigned char close_time2[2];/* �X����(2�X�ܖ�):  */
    unsigned char shut_time2[2]; /* ���؎���(2�X�ܖ�):  */
    unsigned char kioku_time2[2];/* �L������(2�X�ܖ�):  */
    unsigned char clear_time2[2];/* (�ر����)(2�X�ܖ�):  */
    unsigned char getstop_time2[2];/* (�捞��~����)(2�X�ܖ�):  */
    unsigned char revcany_time2[2];/* ��\�����\������(2�X�ܖ�):  */
    unsigned char revcan_time2[2];/* ��\��������(2�X�ܖ�):  */
//    unsigned char yobi1[16];    /* *:  */
//--> 2002/10/25 (�X�ܕʋx���Ή�)�ǉ��C�� Fukuyama
    holidayfloor_t holiday[8];/* �x���ݒ��۱�ԍ�: (�۱��[0]�`[7]) �۱=0:�S�۱  */
} fno1101_t;


/* ���ɓ_���ݒ�:  */
typedef struct _fno1102 {
    unsigned short kinko_tenken;/* ���ɓ_���l: (���ތv - ���Ɍv)  */
    unsigned short kinko_tentimer;/* ���ɓ_�����:  */
} fno1102_t;


/* �S�X��~�ݒ�:  */
typedef struct _fno1103 {
    unsigned short sanddan_stopfg;/* ���ޒf��: ��~�׸�  */
        /*  (=1)���¥(=0)��~       */
        /*  (b0)���m                */
        /*  (b1)�q����              */
        /*  (b2)�X����              */
        /*  (b3)�\��                */
        /*  (b4)��                */
        /*  (b5)���                 */
        /*  (b6)��חD��             */
        /*  ��c                     */
        /*  (b15)�f��               */
    unsigned short sandijo_stopfg;/* ���ވُ�: ��~�׸�  */
    unsigned short fusdan_stopfg;/* �s���f��: ��~�׸�  */
    unsigned short daifus_stopfg;/* ��s��: ��~�׸�  */
    unsigned short daiijo_stopfg;/* ��ُ�: ��~�׸�  */
    unsigned short daiten_stopfg;/* ��_��: ��~�׸�  */
    unsigned short kinkoten_stopfg;/* ���ɓ_��: ��~�׸�  */
    unsigned short tanijo_stopfg;/* �[���ُ�: ��~�׸�  */
    unsigned short revten_stopfg;/* ��\��_��: ��~�׸�  */
    unsigned short risekiten_stopfg;/* ���ȓ_��: ��~�׸�  */
    unsigned short shokuten_stopfg;/* �H���_��: ��~�׸�  */
    unsigned short zkeidan_stopfg;/* �S�X�v���f��: ��~�׸�  */
    unsigned short zerror_stopfg;/* �S�X�װ����: ��~�׸�  */
    unsigned short zdoor_stopfg;/* �S�X�ޱ����: ��~�׸�  */
    unsigned short zdoorten_stopfg;/* �S�X�ޱ�_��: ��~�׸�  */
#if 1		//2002.04.05 koketsu for WinSousa
    unsigned short daicall_stopfg;/* ��ďo: ��~�׸�  */
    unsigned short daidoor_stopfg;/* ��h�A: ��~�׸�  */
    unsigned short yobi[15];     /* �\��:  */
#else
    unsigned char yobi[34];     /* �\��:  */
#endif
} fno1103_t;


/* �@���~�ݒ�:  */
typedef struct _fno1104 {
    unsigned short fus1_stopfg; /* �s������: ��~�׸�  */
        /*  (=1)���¥(=0)��~       */
        /*  (b0)���m                */
        /*  (b1)�q����              */
        /*  (b2)�X����              */
        /*  (b3)�\��                */
        /*  (b4)��                */
        /*  (b5)���                 */
        /*  (b6)��חD��             */
        /*  ��c                     */
        /*  (b15)�f��               */
    unsigned short fus2_stopfg; /* �s��2����: ��~�׸�  */
    unsigned short door1_stopfg;/* �ޱ����: ��~�׸�  */
    unsigned short door2_stopfg;/* �ޱ2����: ��~�׸�  */
    unsigned short call_stopfg; /* �ďo: ��~�׸�  */
    unsigned short uchi_stopfg; /* �Ŏ~: ��~�׸�  */
    unsigned short kaihou_stopfg;/* �J��: ��~�׸�  */
    unsigned short toku_stopfg; /* ����: ��~�׸�  */
    unsigned short ijo_stopfg;  /* �ُ��~: ��~�׸�  */
    unsigned short safijo_stopfg;/* ��ُ̈�: ��~�׸�  */
    unsigned short doorijo_stopfg;/* �ޱ�ُ�: ��~�׸�  */
    unsigned short tokuijo_stopfg;/* ���܈ُ�: ��~�׸�  */
    unsigned short staijo_stopfg;/* ���Ĉُ�: ��~�׸�  */
    unsigned short outten_stopfg;/* ��ē_��: ��~�׸�  */
    unsigned short saften_stopfg;/* ��̥�⋋�_��: ��~�׸�  */
    unsigned short staten_stopfg;/* ���ē_��: ��~�׸�  */
#if 1			// 2002.04.05 Upd koketsu for WinSousa
    unsigned short genkin_stopfg;	/* ������~�׸�  */
    unsigned short card_stopfg;		/* �J�[�h��~�׸�  */
    unsigned short replay_stopfg;	/* ���v���C��~�׸�  */
    unsigned short kenbai_stopfg;	/* �����@��~�׸�  */
    unsigned char yobi[24];     	/* �\��:  */
#else
    unsigned short dmy1;        /* �\��:  */
    unsigned short dmy2;        /* �\��:  */
    unsigned char yobi[28];     /* �\��:  */
#endif
} fno1104_t;


/* �S�X�^�p�ݒ�:  */
typedef struct _fno1105 {
    unsigned short kado_timersec;/* �ғ���� (*�b):  */
    unsigned short kado_timermin;/* �ғ���� (*�b):  */
    unsigned short shoku_timer;/* �H����� (�b): (0)���m�����(1-9999�b)�ݒ�͕b�P��  */
    unsigned short riseki_timer;/* ������� (�b): (0)���m�����(1-9999�b)�ݒ�͕b�P��  */
    unsigned short revcan_timer;/* �\�������: (0)���m�����(1-9999�b)�ݒ�͕b�P��  */
    unsigned short fuscan_timer;/* �s���������: (0)���m�����(1-9999�b)�ݒ�͕b�P��  */
    unsigned short callten_timer;/* �ďo�_�����: (0)���m�����(1-9999�b)  */
    unsigned short kai_hikado_timer;/* �����ғ��_�����: (0)���m�����(1-9999�b)  */
    unsigned short uriijo_sagaku;/* ������ُ�: ���z�l (���z-��)  */
    unsigned short uriijo_kingaku;/* ����ُ�  ���z�l: ���z�l  */
    unsigned short uriijo_timer;/* ����ُ�  ��ϒl: ��ϒl  */
    unsigned short t_uriijo_kingaku;/* ����1������ُ�: ���z�l  */
    unsigned short ukomi;       /* �ō���: �ō���  */
    unsigned short sg_doorten_timer;/* �S�X�ޱ�_�����: (0-255)�b�P��  */
    unsigned short sg_errten_timer;/* ���O�װ�������: (0)���m�����(1-9999�b)�ݒ�͕b�P��  */
    unsigned short kai_kado_timer;/* ����ғ����: (0:60�b)(1-999�b)  */
    unsigned short kai_card_timer;/* �������OUT���: (0:60�b)(1-999�b)  */
    unsigned char yobi2[94];    /* �\��:  */
} fno1105_t;


/* �@��^�p�ݒ�:  */
typedef struct _fno1106 {
    unsigned char pas;          /* ���:  */
    unsigned char type;         /* ����:  */
    unsigned short uchi;        /* �Ŏ~: �Ŏ~�l  */
    unsigned short uchifg;      /* �Ŏ~: �����׸�  */
        /*  (b0=1)�����Ŏ~��        */
        /*  (b1=1)T2���Ŏ~��~      */
    unsigned short kaihou_timer;/* �J��: �J����� (�b)  */
        /*  (=0)�����J�����Ȃ�      */
        /*  (=1)���J��              */
        /*  (2-999�b)�J�����        */
    unsigned short dmy1;        /* �\��:  */
    unsigned short tohosei;     /* ����: T0�␳�l  */
    unsigned short tokufg;      /* ����: �����׸�  */
        /*  (b0=1)�����ް�          */
        /*  (b1=1)T2��������~      */
        /*  (b2=1)T2�����܏o��      */
    unsigned short safijo_saf;  /* ��ُ̈�: ��̒l  */
    unsigned short safijo_out;  /* ��ُ̈�: ��Ēl  */
    unsigned short safijo_timer;/* ��ُ̈�: ��ϒl  */
    unsigned short tcntijo_bo;  /* ���܉񐔈ُ�: BO  */
    unsigned short tcntijo_t1;  /* ���܉񐔈ُ�: T1��  */
    unsigned short tdamaijo_tymax;/* ���܏o�ʈُ�: Tymax�l  */
    unsigned short staijo_start;/* ���Ĉُ�: ���Ēl  */
    unsigned short staijo_out;  /* ���Ĉُ�: ��Ēl  */
    unsigned short staijo_timer;/* ���Ĉُ�: ��ϒl  */
    unsigned short outten_saf;  /* ��ē_��: ��̒l  */
    unsigned short outten_start;/* ��ē_��: ���Ēl  */
    unsigned short saften_out;  /* ��̓_��: ��Ēl  */
    unsigned short saften_start;/* ��̓_��: ���Ēl  */
    unsigned short saften_t1out;/* ��̓_��(T1��): T1��Ēl  */
    unsigned short tdamaten_tymin;/* ���܏o�ʓ_��: Tymin�l  */
    unsigned short staten_saf;  /* ���ē_��: ��̒l  */
    unsigned short staten_out;  /* ���ē_��: ��Ēl  */
#if 0				// 2002.04.05 Upd. koketsu for Win����
    unsigned short dmy2;        /* �\��:  */
    unsigned short dmy3;        /* �\��:  */
    unsigned char yobi1[12];    /* �\��:  */
#else
    unsigned char  stslump_ss1; /* ���Ľ���� s/s1�I�� */
    unsigned char  stslump_so; 	/* ���Ľ���� so�l 	*/
    unsigned short stslump_bo; /* ���Ľ����BO�����l */
    unsigned short stslump_sbst; /* ���Ľ���߽��ĕ��z�͈͊J�n�l*/
    unsigned char  stslump_sbkn; /* ���Ľ���߽��ĕ��z�͈͌����l*/
    unsigned char  stslump_sbbn; /* ���Ľ���߽��ĕ��z�͈͕����l*/
    unsigned short next_uti; 	/* ����Ŏ~�l*/
// 2002/09/18 �ǉ��C�� Fukuyama -->
	unsigned char  outstart_out; /* ��Ē萔���Ċ֘A��Ē萔*/
	unsigned char  stslump_sharai; /* ��Ē萔���Ċ֘A���ĕ��o��*/// �o�E���_���C�� (����)
    unsigned short outstart_start; /* ��Ē萔���Ċ֘A���Ē萔*/
    unsigned char yobi1[2];    /* �\��:  */
// --> 2002/09/18 �ǉ��C�� Fukuyama
#endif
    unsigned char yobi2[64];    /* �\��:  */
} fno1106_t;


/* D2�ݒ�:  */
typedef struct _fno1107 {
    unsigned char setei1;       /* �ݒ��ް�1:  */
        /*  ���         b7   (0)P��(1)AS��                       */
        /*  �ďoSW����   b6   (0)���Ȱ�SW�(1)Ӱ����SW              */
        /*  ���Č��m     b5   (0)ON���m�(1)OFF���m                 */
        /*  S2�S3�쐬    b4   (0)��:�ʏ���ē��ͥ(1)�L:��������    */
    unsigned char setei2;       /* �ݒ��ް�2:  */
        /*  �������(�b)     0.2�b�P��  (����OFF���m��1/10�\��)    */
    unsigned char setei3;       /* �ݒ��ް�3:  */
        /*  �������(�b)     0.02�b�P��    */
    unsigned char setei4;       /* �ݒ��ް�4:  */
        /*  T1���        b5-0     (0-31)�b�P��     */
    unsigned char setei5;       /* �ݒ��ް�5:  */
        /*  T1�x�����    b0-7     (0-255)�b�P��    */
    unsigned char setei6;       /* �ݒ��ް�6:  */
        /*  T2���        b5-0     (0-31)�b�P��     */
    unsigned char setei7;       /* �ݒ��ް�7:  */
        /*  T2�x�����    b0-7     (0-255)�b�P��    */
    unsigned char setei8;       /* �ݒ��ް�8:  */
        /*  �ޱ�ُ����   b0123:�u���ޱ�����    (0)���m�Ȃ��(1-15)0.1�b�P��     */
        /*  �ޱ�������   b4567:�ޱ����ݓ���    (0)���m�Ȃ��(1-15)1�b�P��       */
    unsigned char setei9;       /* �ݒ��ް�9:  */
        /*  �ޱ�_�����   b0-7     (0)���m�Ȃ��(1-225)1�b�P��       */
    unsigned char setei10;      /* �ݒ��ް�10:  */
        /*  �⋋�_�����  b0-7     (0)���m�Ȃ��(1-225)1�b�P��       */
    unsigned char setei11;      /* �ݒ��ް�11:  */
    unsigned char setei12;      /* �ݒ��ް�12:  */
    unsigned char setei13;      /* �ݒ��ް�13:  */
    unsigned char setei14;      /* �ݒ��ް�14:  */
        /*  ���͑I��1    b0123    (0)�ޱ1�(1)�ޱ1�(2)�s��1�(3)�ďo1�(4)�ޱ2�(5)�s��2�(6)�ďo2�(7)�⋋     */
        /*  ���͑I��2    b4567    (0)�s��1�(1)�ޱ1�(2)�s��1�(3)�ďo1�(4)�ޱ2�(5)�s��2�(6)�ďo2�(7)�⋋    */
    unsigned char setei15;      /* �ݒ��ް�15:  */
        /*  ���͑I��3    b0123    (0)�ďo1�(1)�ޱ1�(2)�s��1�(3)�ďo1�(4)�ޱ2�(5)�s��2�(6)�ďo2�(7)�⋋    */
        /*  *                                                                                             */
    unsigned char setei16;      /* �ݒ��ް�16:  */
        /*  �o��1�ݒ�    b0123    (=0)�Ŏ~���(=1)T1���(=2)T1+T2���(3)T2��    */
        /*  �o��2�ݒ�    b4567    (=0)�Ŏ~���(=1)T1���(=2)T1+T2���(3)T2��    */
} fno1107_t;

typedef struct _fno1108_tenpo_t {
	unsigned char	t_holiday;			/* 00:�c�Ɠ�FLAG (bit0=1)�J�X��,(bit0=0)�x�X�� */
	unsigned char	dmy[3];				/* *:  */
	unsigned char	open_time[2];		/* �J�X����:  */
	unsigned char	close_time[2];		/* �X����:  */
	unsigned char	shut_time[2];		/* ���؎���:  */
	unsigned char	kioku_time[2];		/* �L������:  */
	unsigned char	clear_time[2];		/* (�ر����):  */
	unsigned char	getstop_time[2];	/* (�捞��~����):  */
	unsigned char	revcany_time[2];	/* ��\�����\������:  */
	unsigned char	revcan_time[2];		/* ��\��������:  */
} fno1108_tenpo_t;

/* �X�ܕʕϑ��c�Ɛݒ�:  */
typedef struct _fno1108 {
	unsigned long	eigyo_pw_tim;		/* �c�Ɠ��t: �c�Ǝ��̌������� �N��������������b = bit7�4�5�5�6�5  1910+�N 0=���j  */
	unsigned char	dmy[12];				/* *:  */
	fno1108_tenpo_t tenpo[2];
} fno1108_t;


/* �J�X����e�[�u�� */
typedef fno1108_tenpo_t fno4824_t;

#if 0
/* �X�ܖ������ݒ� */
typedef struct _fno1108_d1 {
	unsigned char	t_holiday;		/* �c�Ɠ��t���O (bit7=1)�J�X��,(bit7=0)�x�X�� */
	unsigned char	t_dmy2;
	unsigned char	t_clear_hm[2];		/* (����)�N���A����([0]�� [1]��) */
	unsigned char	t_open_hm[2];		/* �J�X����([0]�� [1]��) */
	unsigned char	t_close_hm[2];		/* �X����([0]�� [1]��) */
	unsigned char	t_sime_hm[2];		/* ���؎���([0]�� [1]��) */
	unsigned char	t_kioku_hm[2];		/* �����L������([0]�� [1]��) */
	unsigned char	t_tstop_hm[2];		/* (����)�捞��~����([0]�� [1]��) */
	unsigned char	t_opmae_hm[2];		/* �J�X�O�x������([0]�� [1]��) */
	unsigned char	t_dmy3[8];
} fno1108_d1_t;

typedef struct _fno1108_d2 {
	unsigned char	t_holiday;		/* �c�Ɠ��t���O (bit7=1)�J�X��,(bit7=0)�x�X�� */
	unsigned char	t_dmy2;		
	unsigned short	t_clear_hm;		/* (����)�N���A����(bit15-8=�� bit7-0=��) */
	unsigned short	t_open_hm;		/* �J�X����(bit15-8=�� bit7-0=��) */
	unsigned short	t_close_hm;		/* �X����(bit15-8=�� bit7-0=��) */
	unsigned short	t_sime_hm;		/* ���؎���(bit15-8=�� bit7-0=��) */
	unsigned short	t_kioku_hm;		/* �����L������(bit15-8=�� bit7-0=��) */
	unsigned short	t_tstop_hm;		/* (����)�捞��~����(bit15-8=�� bit7-0=��) */
	unsigned short	t_opmae_hm;		/* �J�X�O�x������(bit15-8=�� bit7-0=��) */
	unsigned char	t_dmy3[8];
} fno1108_d2_t;

typedef union _fno1108_tenpo {
	fno1108_d1_t a;
	fno1108_d2_t b;
} fno1108_tenpo_t;

/* �J�����_�[�ݒ� */
typedef struct _fno1108 {
	unsigned char	month;			/* �� */
	unsigned char	day;			/* �� */
	unsigned char	dmy[6];
	fno1108_tenpo_t	f1108_tenpo[8];		/* �X�ܕʎ����W�O���[�v */
} fno1108_t;
#endif

typedef fno1100_t FSET_GYOU_ANSYOU;
typedef fno1101_t FSET_GYOU_OPCL;
typedef fno1102_t FSET_GYOU_KINKO;
typedef fno1103_t FSET_GYOU_ZENTEI;
typedef fno1104_t FSET_GYOU_KSHUTEI;
typedef fno1105_t FSET_GYOU_ZEN;
typedef fno1106_t FSET_GYOU_KSHU;
typedef fno1107_t FSET_GYOU_D2;
typedef fno1108_t FSET_GYOU_HENSOKU; //��2002/10/29 (�X�ܕʕϑ��c�ƑΉ�)�ǉ� Fukuyama
#endif                          /* __set_gyou_h__ */
/* WAIS END */
