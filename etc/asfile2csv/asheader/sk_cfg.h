/*
 * $Id: sk_cfg.h,v 1.10 2000/03/28 05:16:41 sakurada Exp $
 */

/*  �R���p�C�������� */

/*=========================================================================*/

#if !defined(_SK_CFG_H_)
#define	_SK_CFG_H_


/*----------------------------------------------------------------------------
  ����
	_SK_target_ ���`����
----------------------------------------------------------------------------*/

#if defined(_MRI)

#define	_SK_SRM68K_
#define _SK_EMBED_

#elif defined(_WIN32)

#define _SK_WINDOWS_

#elif defined(MSDOS)

#define _SK_MSDOS_
#if defined(__WATCOMC__) && defined(__386__)
#define	_SK_WCC_		/* dos-32bit / watcom c */
#endif

#elif defined(_OS9)
#define _SK_OS9_
#define _SK_EMBED_

#elif defined(_OS9000)
#define _SK_OS9000_

#elif defined(linux)
#define _SK_LINUX_

#elif defined(__FreeBSD__)	/* 1999/12/27 sakurada */
#define _SK_FREEBSD_

#else

#error undefined environment

#endif


/*----------------------------------------------------------------------------
  ��`
	��ɍs�����`
----------------------------------------------------------------------------*/
#ifndef _SK_LINUX_		/* 2000/03/16 sakurada */
#define	LITTLE_ENDIAN	1234	/* LSB first: i386, vax */
#define	BIG_ENDIAN	4321	/* MSB first: 68000, ibm, net */
#define	PDP_ENDIAN	3412	/* LSB first in word, MSW first in long */
#else  /* in case LINUX */
#include <endian.h>
#endif /* _SK_LINUX_ */

/*----------------------------------------------------------------------------
  �^�[�Q�b�g���̒�`�A�Ē�`
	_SK_target_ �𗘗p���ĕK�v�Ȓ�`���s��

	BYTE_ORDER		BIG_ENDIAN ���� LITTLE_ENDIAN ����`�����
	_SK_EMBED_		�g�ݍ��݊��̏ꍇ�ɒ�`�����
----------------------------------------------------------------------------*/
/*	SRM68K �̏ꍇ	*/
#if defined(_SK_SRM68K_)
/*#undef	_DEBUG*/
#undef	_WINDOWS
#undef	_MSDOS
#define	_SRM68K
#define	BYTE_ORDER	BIG_ENDIAN
/* #define	_SK_EMBED_	1*/

/*	MS-DOS �̏ꍇ	*/
#elif defined(_SK_MSDOS_)
/*#define	_MSDOS*/
/*	#define	_DEBUG	*/
#define	BYTE_ORDER	LITTLE_ENDIAN

/*	WINDOWS(95) �̏ꍇ	*/
#elif defined(_SK_WINDOWS_)
/*#define	_WINDOWS*/
#define	BYTE_ORDER	LITTLE_ENDIAN

/*	OS-9 �̏ꍇ	*/
#elif defined(_SK_OS9_)
#define BYTE_ORDER	BIG_ENDIAN

/*	���@��OS9�ł�sscanf���G���[���N�����̂ŁA����ւ���	*/
#define sscanf sk_sscanf

/* xcc��memcmp()�̕s��΍� 98/03/05 sakurada */
#define memcmp sk_memcmp

/*	OS9000�̏ꍇ	*/
#elif defined(_SK_OS9000_)
#define BYTE_ORDER	LITTLE_ENDIAN

#elif defined(_SK_LINUX_)
				/* 2000/03/28 removed sakurada */
#endif


#endif	/* _SK_CFG_H_ */

/*
 * $Log: sk_cfg.h,v $
 * Revision 1.10  2000/03/28 05:16:41  sakurada
 * changed define in endian.h and remove some stuff from sk_cfg.h
 * for Linux.
 *
 * Revision 1.9  2000/03/17 07:20:34  sakurada
 * define BYTE_ORDER in sk_cfg.h for linux
 * remove stdio.h in endian.h for linux
 *
 * Revision 1.8  2000/03/15 09:16:53  sakurada
 * fix work arround BYTE_ORDER for linux
 *
 * Revision 1.7  2000/03/14 00:25:58  nisimura
 * no message
 *
 * Revision 1.6  2000/03/14 00:20:04  sakurada
 * fix for double definition
 *
 * Revision 1.5  2000/01/05 04:34:24  nisimura
 * �R�����g�̗��[�Ɂ^���@���^�������Ă��Ȃ������̂��C���B
 *
 * Revision 1.4  1999/12/28 06:28:55  sakurada
 * VC���commit�����ƒ�ߒ�뒡�FreeBSD�����钡�
 *
 * Revision 1.3  1999/12/27 08:54:33  sakurada
 * remove junks
 *
 * Revision 1.2  1999/12/27 07:15:34  sakurada
 * add _SK_FREEBSD_
 *
 * Revision 1.1.1.1  1999/12/26 05:57:11  sakurada
 * 1st try to import from VSS
 *
 * 
 * 1     99/09/29 10:17a Sakurada
 * INCLUDE -> include
 * Revision 1.1  1999/09/29 01:13:32  sakurada
 * change INCLUDE -> include
 *
 * 
 * 2     99/07/16 2:53p Sakurada
 * LINUX�ǉ�
 *
 * 13    98/08/03 12:01p Sakurada
 * OS9000�̏ꍇ��dBYTE_ORDER�ǉ�
 * 
 * 12    98/08/03 11:51a Sakurada
 * OS9000��make�ʂ�悤�ɕύX
 * 
 * 11    98/03/05 20:43 Sakurada
 * _SK_OS9_�� memcmp -> sk_memcmp
 * 
 * WAIS�����L�[���[�h�ǉ�
 * 
 * User: Matsuo       Date: 97/09/30   Time: 14:31
 * _SK_SRM68K_�̂Ƃ��A�K��_DEBUG��undef���Ă����̂���߂�
 * 
 * User: Nakata       Date: 97/09/01   Time: 13:29
 * _SK_WCC_���������ݒ肳���悤�ɂ����B
 * 
 * User: Sakurada     Date: 97/08/12   Time: 9:42
 * OS9�̏ꍇ sscanf -> sk_sscanf �� define
 * 
 * User: Matsuo       Date: 97/07/04   Time: 13:42
 * _SK_ENBED_��ǉ�����
 * 
 * User: Matsuo       Date: 97/07/04   Time: 10:45
 * _SK_WINDOWS_�̂Ƃ���_WINDOWS�̍Ē�`����߂�
 * 
 * User: Sakurada     Date: 97/06/20   Time: 16:19
 * _sk_OS9_�ł�BYTE_ORDER�ǉ�
 * 
 * User: Sakurada     Date: 97/05/17   Time: 14:59
 * Created in $/NAS9709/INCLUDE
 */
/* WAIS END */

