#ifndef __asdef_h__
#define __asdef_h__
 /* $Id: asdef.h,v 1.6 2000/07/10 15:36:54 ohno-t Exp $ */               /* ���ڏ����f�[�^:  */
                                /* ���ڏ����f�[�^:  */
 /*  1998/9/18 18:05 osako            */           /* ���ڏ����f�[�^:  */
 /* WAIS START */               /* ���ڏ����f�[�^:  */
#define ASDEF
//#ifdef _KDAI_MAX_1536
#if 1	// �v��������͂P�T�R�U�Œ�ɂ��� 2002.03.26 by koketsu
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
#define HENDO_MAX 8				/* �ϓ��`�`MAX�� */
#define RECSU 25				/* fno1108�̃T�C�Y(1400Byte)�����R�[�h��(56Byte)�Ŋ������l:  */

#define SETSUZOKU ZENTENSU      /* �S�X�ڑ��ݒ茏��:  */

#if	0	/* 2002.06.17(mon) chg.ohnot */
#define SHUBETSU 8              /* ��ʐ�:  */
#else
//	2002.06.11 �C�� By Fujiki(�Q�X��)-->
//#define SHUBETSU 3              /* ��ʐ�:  �`�r���S��ʃT�|�[�g����܂ł͂R��ʂƂ��� 2002.04.18 koketsu  */
#define   SHUBETSU 4              /* ��ʐ�:  �v���������ʂ͂o�`�r�w�̂S��� 2002.03.26 Upd. By koketsu*/
//	-->2002.06.11 �C�� By Fujiki(�Q�X��)
#endif

//	2002.06.14 Add By Koketsu(�Q�X��)-->
//-------------------------------
// KPC �擾�t�@�C�����R�[�h��
// ------------------------------
#define KIOKUINF_RSUU		9			// �L�����
#define TEIJIKADO_RSUU		34			// �莞�ғ�
#define TEIJISHU_RSUU		3264		// �莞�@��
#define SYSIJYO_RSUU		256			// �V�X�e���ُ헚��
#define CLEAR_RSUU			16			// �N���A����
#define	TOKEISET_RSUU		16			// ���v�ݒ�
#define	TEIDEN_RSUU			32			// ��d����
#define	SETTEI_RSUU			16			// �ݒ藚��
#define SETTEIVER_RSUU		1000		// �ݒ�u�d�q���
#define	IJYO_RSUU			768			// �ُ헚��
#define	UTI_RSUU			1536		// �Ŏ~����
#define	HASEI_RSUU			256			// ��������
#define	TNSLUMP_RSUU		512			// �s�m�X�����v
#define	KAIINLOG_RSUU		36863		// ������O
#define	KAIININF_RSUU		1			// ������O�Ǘ����
#define TENPOBETU_RSUU		25			// �X�ܕʕϑ��c�Ə��	��2002/10/29 (�X�ܕʕϑ��c�ƑΉ�)�ǉ� Fukuyama
//<---	2002.06.14 Add By Koketsu(�Q�X��)

#define ANSHO 8
#define TVCH_SU 12              /* TV ch�ݒ茏��:  */
#define REMOTE 32
#define RECSU 25

#define GENKA_GETA		(100 * 1000)		// �������f�[�^�̉���(�S���� * �����_�ȉ��O��)
#define GENKA_DEFAULT	(1 * GENKA_GETA)	// �������ݒ�==0����default������


 /* WAIS END */                 /* ���ڏ����f�[�^:  */
#endif                          /* __asdef_h__ */
/*
 * $Log: asdef.h,v $
 * Revision 1.6  2000/07/10 15:36:54  ohno-t
 * �P�T�R�U��Ή����암�����o�^
 *
 * Revision 1.5  2000/07/06 05:24:02  osako
 * merge 1536 sourse files
 *
 * $Log: asdef.h,v $
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
