#ifndef __asdef_h__
#define __asdef_h__
 /* $Id: asdef.h,v 1.5 2002/08/01 04:40:21 nisimura Exp $ */               /* ���ڏ����f�[�^:  */
                                /* ���ڏ����f�[�^:  */
 /*  1998/9/18 18:05 osako            */           /* ���ڏ����f�[�^:  */
 /* WAIS START */               /* ���ڏ����f�[�^:  */
#define ASDEF
#ifdef _KDAI_MAX_1536
#define TNSU 1536               /* TN��:  */
#define DAISU 1536              /* �䐔:  */
#else
#define TNSU 1024               /* TN��:  */
#define DAISU 1024              /* �䐔:  */
#endif
#define ZENTENSU 256            /* �S�X��:  */
#define SIMASU 96               /* ����:  */
#define FLOORSU 8
#define KINKO 64                /* ���ɐ�:  */
#define HANBAIKISU 256          /* �̔��@��:  */
#define TYPE 16                 /* �^�C�v:  */
#define TYPEMEICHO 46           /* �^�C�v����:  */
#define KISHU 96                /* �@�퐔:  */
#define KISHUMEICHO 28          /* �@�햼��:  */
#define PORT_USER 128

#define SETSUZOKU ZENTENSU      /* �S�X�ڑ��ݒ茏��:  */
#define SHUBETSU 8              /* ��ʐ�:  */
#define ANSHO 8
#define TVCH_SU 12              /* TV ch�ݒ茏��:  */
#define REMOTE 32
 /* WAIS END */                 /* ���ڏ����f�[�^:  */
#endif                          /* __asdef_h__ */
/*
 * $Log: asdef.h,v $
 * Revision 1.5  2002/08/01 04:40:21  nisimura
 * struct ��`���Â��������� nas2002(20020731���_) �̂��̂��R�s�[�����B
 *
 * Revision 1.4  2002/06/17 06:43:03  nisimura
 * SHUBETSU ���R����W�֕ύX�B�i�Q�X�܂W��ʑΉ��j
 *
 * Revision 1.3  2001/02/19 02:31:18  kojima
 * no message
 *
 * Revision 1.6  2000/07/10 15:36:54  ohno-t
 * �P�T�R�U��Ή����암�����o�^
 *
 * Revision 1.5  2000/07/06 05:24:02  osako
 * merge 1536 sourse files
 *
 * $Log: asdef.h,v $
 * Revision 1.5  2002/08/01 04:40:21  nisimura
 * struct ��`���Â��������� nas2002(20020731���_) �̂��̂��R�s�[�����B
 *
 * Revision 1.4  2002/06/17 06:43:03  nisimura
 * SHUBETSU ���R����W�֕ύX�B�i�Q�X�܂W��ʑΉ��j
 *
 * Revision 1.3  2001/02/19 02:31:18  kojima
 * no message
 *
 * Revision 1.6  2000/07/10 15:36:54  ohno-t
 * �P�T�R�U��Ή����암�����o�^
 *
 * Revision 1.4  2000/02/23 04:08:35  matsuo
 * no message
 *
 * Revision 1.3  2000/01/11 05:15:08  ohno-t
 * �R�����g�C��
 *
 * Revision 1.2  1999/12/29 15:36:22  sakurada
 * remove hankaku-kana, add keyword, fix indenting
 *
 * Revision 1.1.1.1  1999/12/26 05:57:10  sakurada
 * 1st try to import from VSS
 *
 * 2     99/10/01 9:41 Ohno-t
 *
 * 1     99/09/29 10:15a Sakurada
 * INCLUDE -> include
 * Revision 1.1  1999/09/29 01:13:28  sakurada
 * change INCLUDE -> include
 */
