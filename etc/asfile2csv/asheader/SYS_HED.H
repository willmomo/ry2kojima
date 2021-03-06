/* WAIS START */
/************************************************************************/
/*	�V�X�e���w�b�_							*/
/*	Version	Date	 Programmer	Memo				*/
/*	00000.0	97.01.09	y.i		�V�K�쐬			*/
/************************************************************************/
#ifndef _SYS_HED_H_
#define _SYS_HED_H_

#define	L_SYS_HED
#include "asdef.h"
/************************************************/
/*	�^					*/
/************************************************/
#if 1

#ifndef uchar
typedef	unsigned char	uchar;
#endif
#ifndef ushort
typedef	unsigned short	ushort;
#endif
#ifndef uint
typedef	unsigned int	uint;
#endif
#ifndef ulong
typedef	unsigned long	ulong;
#endif

#endif
/************************************************/
/*	MAX					*/
/************************************************/
#if 1
#define	MAX_DAI		DAISU		/* ��MAX				*/
#else
#define	MAX_DAI		256			/* ��MAX(DEBUG)			*/
#endif
#define	MAX_SIMA	SIMASU		/* ��MAX				*/
#define	MAX_KISHU	KISHU		/* �@��MAX				*/
#define	MAX_KINKO	KINKO		/* ����MAX				*/
#define	MAX_RECSU	RECSU		/* fno1108�̃��R�[�h��MAX*/ //��2002/10/29 (�X�ܕʕϑ��c�ƑΉ�)�ǉ� Fukuyama

#define	MAX_KINKO_SIMA	4		/* ���ɓ���MAX			*/

#define	MAX_PAS		SHUBETSU	/* ���MAX				*/
#define	MAX_SGSBT	5			/* ���O���MAX			*/
#define	MAX_FLOOR	FLOORSU		/* �۱�MAX				*/
#define	MAX_TYPE	TYPE		/* ����MAX				*/
//	2002.05.06 �ǉ� By Fujiki(�Q�X��)-->
#define MAX_TENPO	2			// �X�ܐ�
//	-->2002.05.06 �ǉ� By Fujiki(�Q�X��)
//-> 2002.06.14 �ǉ� kaoru�i�Q�X�܁j
#define MAX_SBTPOCKET	8		// ��ʃ|�P�b�g����
#define MAX_HENDOINS	8		// �ϓ��o�^����
//<- 2002.06.14 �ǉ� kaoru�i�Q�X�܁j
#define	MAX_SMCON_IN ZENTENSU	/* ���O�ڑ�MAX			*/

#define	DSAF_MAX	65535		/* �Z�[�tMAX	2001.10.18	*/

/************************************************/
/*						*/
/************************************************/
#define	LEN_KSHUNAME	28		/* �@�햼��			*/

#endif /* _SYS_HED_H_ */
	/*
/* WAIS END */
