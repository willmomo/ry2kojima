/* WAIS START */
#ifndef __ordercom_h__
#define __ordercom_h__

#define	OD_SHIMA_SUU		128
#define	OD_HINMOKU_SUU		64
#define	OD_SHINAGIRE_SUU	128

/* �I�[�_�[�E���O���[�v�ݒ� */
typedef struct _fno5016 {
	unsigned short	sima;		/* ���ԍ� */
	unsigned char	grp_num;	/* �O���[�v�ԍ� */
	unsigned char	pad;
} fno5016_t;	/* GRP_SIMA */

/* �I�[�_�[�E�i�ڐݒ� */
typedef struct _fno5018 {
	unsigned char	namae[24];		/* ���j���[�̕i��: �S�p12����  */
	unsigned short	tanka_genkin;	/* �P��(�������z): 0�`65535  */
	unsigned short	tanka_tama;		/* �P��(�ʐ�): 0�`65535  */
	unsigned short	tanka_coin;		/* �P��(�R�C����): 0�`65535  */
	unsigned char	reserve[2];		/* �\��:  */
} fno5018_t;	/* d_2450_hinmoku_t */

/* �I�[�_�[�E�i�؂�ݒ� */
typedef struct _fno5019 {
	unsigned char	sinagire;		/* �i�؂�t���O: ($00)�݌ɂ��� ($10)�i�؂�*/
	unsigned char	reserve;		/* �\��:  */
	unsigned short	zaiko_suu;		/* �݌ɐ�: 0�`65535  */
} fno5019_t;	/* d_2430_hinmoku_t */

#endif	/*	__ordercom_h__	*/
/* WAIS END */
