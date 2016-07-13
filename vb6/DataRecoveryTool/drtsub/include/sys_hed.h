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
#define	MAX_DAI		DAISU		/* ��MAX			*/
#else
#define	MAX_DAI		256			/* ��MAX(DEBUG)			*/
#endif
#define	MAX_SIMA	SIMASU		/* ��MAX			*/
#define	MAX_KISHU	KISHU		/* �@��MAX			*/
#define	MAX_KINKO	KINKO		/* ����MAX			*/

#define	MAX_KINKO_SIMA	4		/* ���ɓ���MAX			*/

#if 0	/* del 2002/03/25 */
#define	MAX_PAS		SHUBETSU	/* ���MAX  :�S��ʑΉ� RUNTIME�� del 2002.03.25 */
#endif

#define	MAX_SGSBT	4			/* ���O���MAX			*/
#define	MAX_FLOOR	FLOORSU		/* �۱�MAX			*/
#define	MAX_TYPE	TYPE		/* ����MAX			*/

#define	MAX_SMCON_IN ZENTENSU	/* ���O�ڑ�MAX			*/

#define	DSAF_MAX	65535		/* �Z�[�tMAX	2001.10.18	*/

/************************************************/
/*						*/
/************************************************/
#define	LEN_KSHUNAME	28		/* �@�햼��			*/

#endif /* _SYS_HED_H_ */
/* WAIS END */
