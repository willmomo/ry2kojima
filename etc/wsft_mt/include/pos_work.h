
////////////////////////////////////////////////////////////////////////////

/*		** HC2007 �萔�ύX **

	HC2007 2�X��8���250�@��50�^�C�v�@�Ŏ�ʂ�4��8�ɕύX

	POS�L�̃V�X�e����8��ʑΉ�����ꍇ�͂��̃t�@�C�����C�����Ă��������B
*/

////////////////////////////////////////////////////////////////////////////



/*
 *	NAS POS
 *	�A�v���P�[�V�����萔�A���[�N�A�\���̂̒�`
 *
 *	$Header: /usr/external/CVS/nas2002/Pos/include/work.h,v 1.8 2003/10/01 04:40:04 sugiura Exp $
 *	$Log: work.h,v $
 *	Revision 1.8  2003/10/01 04:40:04  sugiura
 *	no message
 *	
 *	Revision 1.7  2003/09/25 01:34:30  sugiura
 *	�G���[�`�P�b�g���Ή�(POS)
 *	
 *	Revision 1.6  2003/07/14 05:55:06  sugiura
 *	SNAP_R_030701�̃\�t�g�ɑ΂���
 *	�ӔC�ҊǗ��@�\�̕s����C��
 *	
 *	Revision 1.5  2003/03/18 05:40:50  sugiura
 *	POS���ʏ������g���C�Ή�
 *	
 *	Revision 1.4  2003/02/27 04:27:02  sugiura
 *	SNAP_R_021119�̃\�t�g�ɑ΂���
 *	POS��ʑI����ʕύX�̋@�\������
 *	
 *	Revision 1.3  2002/11/20 02:12:51  sugiura
 *	POS-CDS�ʐM�����ύX
 *	
 *	Revision 1.2  2002/10/07 04:52:49  sugiura
 *	SNAP_R_020808�̃\�t�g�ɑ΂���
 *	�Q�X�܂S���(���S��)�̋@�\������
 *	
 *	Revision 1.1  2002/06/25 04:22:00  sugiura
 *	2�X��4��ʑΉ�(�Ȃ�܂�)
 *	
 *	Revision 1.37  2002/04/03 07:46:06  sugiura
 *	SNAP_R_020227�̃\�t�g�ɑ΂���
 *	�V�s�o(�������Ή�)�ɑΉ������@�\��ǉ�
 *	
 *	Revision 1.36  2002/02/25 02:07:00  sugiura
 *	SNAP_R_020213�̃\�t�g�ɑ΂���
 *	�EPOS�ċN�����CDS�ɃJ�[�h���}������Ă������̃J�[�hOUT
 *	�𑗐M���������ǉ�
 *	
 *	�Emyrecon������������G���[�����t�@�C���ɏ������ޏ����ǉ�
 *	
 *	Revision 1.35  2002/02/01 06:56:11  sugiura
 *	SNAP_R_020122�̃\�t�g�ɑ΂���
 *	�}���n������i�i���ʃp�^�[���ɑΉ�����@�\�ǉ�
 *	�R��ʑΉ�(A��ʂ̒����_��������S��A�̒P�����l�����čs��)
 *	���ʓ����A��ʑΉ�
 *	
 *	Revision 1.34  2001/12/12 10:50:00  sugiura
 *	����A�[�N�l�b�g�Q�X�܍\������
 *	������ʂ�font14 �� 16�ɕύX
 *	
 *	Revision 1.33  2001/11/30 10:33:53  sugiura
 *	����J�[�h����(POS)
 *	�O�����X����
 *	
 *	Revision 1.32  2001/11/05 03:55:30  sugiura
 *	POS�����P�������_�Ή�
 *	
 *	Revision 1.31  2001/10/02 04:46:25  sugiura
 *	�����ߋ@�\�g���A�ԕi(�߂�)�����ǉ�
 *	
 *	Revision 1.30  2001/08/24 08:29:12  sugiura
 *	�ӔC�ҕʑ��쐧���@�\
 *	����������CA�ɑ��M
 *	POS�哮�}�X�^��M�@�\
 *	
 *	Revision 1.29  2001/06/29 01:38:42  sugiura
 *	POS�݌ɊǗ��g���A�V�X�����Ή�
 *	
 *	Revision 1.28  2001/04/16 06:06:33  fukuoka
 *	POS��P�l����
 *	
 *	Revision 1.27  2001/02/26 05:11:38  fukuoka
 *	�s��Ή�
 *	�P�@��ʑ���s�
 *	�Q�@�G�[�X�`�P�b�g�̃R�[�h���͕s�
 *	�R�@������[��-������ύX�̉���
 *	�S�@���������̕ύX�i���ʁE�T�[�r�X�ʁj
 *	�T�@�G���[���e�̏[��
 *	�U�@1��ڂ̃`�P�b�g�Ǎ��ŘA���L�[��v�������s�
 *	
 *	Revision 1.26  2001/01/12 09:55:53  fukuoka
 *	�@�\����
 *	�P�@�L�������A�N���A�����̌������B
 *	�@�@OPEN�O�N���A�̐V�K�ǉ�
 *	
 *	�Q�@�i�i���Q�Ɖ�ʂ̏[��
 *	
 *	�R�@�������ʂ𖾊m�Ɉ󎚂�����
 *	
 *	Revision 1.24  2000/11/01 07:48:39  shimizu
 *	no message
 *	
 *	Revision 1.23  2000/08/03 12:24:15  shimizu
 *	no message
 *	
 *	Revision 1.21  2000/07/12 09:53:23  hoshiyam
 *	 �P�D���V�[�g�����󎚂Łu�i�i�߂��v�u�E���ʁv�u�ۗ��������v�̋�ʂ��t����
 *	 �Q�D�ڑ��@��ݒ�̂j�r�Ƃb�`�̏�Ԃ����ď�����ؑւ�(�t�B���[)
 *	 �R�D�i�i�߂��̏�Ԃ��甲���Ȃ��s��̏C��
 *	 �S�D�݌ɏW�v�����̃^�C���A�E�g�l�ύX(�T�b���Q�b)
 *	
 *	Revision 1.20  2000/07/11 10:55:21  fukuoka
 *	���V�[�g�ύX�i�E���ʁE�i�i�߂��E�ۗ��������j�A�X�����ύX��
 *	
 *	Revision 1.16  2000/06/19 23:50:37  hoshiyam
 *	�����߈󎚋@�\�ǉ��ɔ����C��
 *	
 *	Revision 1.15  2000/06/01 05:23:33  hoshiyam
 *	�`�P�b�g�p�X���[�h�G���[�R�[�h�̒ǉ�
 *	
 *	Revision 1.14  2000/05/18 03:55:01  hoshiyam
 *	no message
 *	
 *	Revision 1.13  2000/04/21 08:00:09  hoshiyam
 *	POS��{�ݒ�Ƀt���O�̒ǉ�
 *	
 *	Revision 1.12  2000/04/21 02:00:36  hoshiyam
 *	POS��{�ݒ�\���̂Ƀt���O��񃁃��o�[�̒ǉ�
 *	
 *	Revision 1.10  2000/04/07 01:03:32  hoshiyam
 *	no message
 *	
 *	Revision 1.9  2000/03/29 09:33:11  hoshiyam
 *	no message
 *	
 *	Revision 1.8  2000/03/23 07:11:04  hoshiyam
 *	no message
 *	
 *	Revision 1.7  2000/03/21 06:50:19  hoshiyam
 *	no message
 *	
 *	Revision 1.5  2000/02/23 01:47:55  hoshiyam
 *	no message
 *	
 *	Revision 1.2  2000/02/04 07:11:12  hoshiyam
 *	no message
 *	
 *	Revision 1.5  2000/01/31 12:23:24  hoshiyam
 *	no message
 *	
 * 
 * 3     00/01/31 16:48 Hoshiyam
 * 
 * 2     00/01/24 14:00 Shimizu
 * 
 * 1     00/01/21 20:42 Hoshiyam
 * 
 * 1     00/01/21 20:04 Hoshiyam
 * 
 * 93    99/12/13 11:35 Hoshiyam
 * ����i�i�����v�Z���@�I��������{�ݒ�ɒǉ��B
 * 
 * 92    99/11/26 20:22 Hoshiyam
 * 
 * 91    99/11/10 13:56 Hoshiyam
 * �}���n�� �V����Ή��Ɋւ���C��
 * 
 * 90    99/10/20 14:32 Hoshiyam
 * �A���L�[�̃G���[���b�Z�[�W�R�[�h��ǉ�
 * 
 * 89    99/10/20 13:12 Hoshiyam
 * ���̋@�f�[�^�����N�s��Ή�
 * 
 * 88    99/10/18 12:19 Hoshiyam
 * ���̋@�f�[�^�����N�A�A���L�[�Ή��A�J�[�h�C�����LCD�\���ATP�ʐM���g���C
 * ���Ԃ̎d�l�ύX�ɔ����C���B
 * 
 * 87    99/09/30 13:04 Hoshiyam
 * �t�B�[�o�[ �s��Ή� Version 1.1
 * 
 * 86    99/09/10 17:13 Hoshiyam
 * �J�[�h�ƍ��@�ԒʐM�̉����Ɋւ���C��
 * 
 * 85    99/09/07 16:13 Hoshiyam
 * �A���L�[�t���O�G���A�̒ǉ�
 * 
 * 84    99/09/02 18:01 Hoshiyam
 * ���o���@�̃G���[�R�[�h�ǉ�
 * 
 * 83    99/08/30 14:41 Hoshiyam
 * ���o���@�ʐM�Ɋւ���ʐM
 * 
 * 82    99/08/10 9:53 Hoshiyam
 * 
 * 81    99/08/09 13:34 Hoshiyam
 * 
 * 80    99/08/03 14:18 Nakata
 * �����o���@���[�J�[��`�ǉ�
 * 
 * 79    99/08/03 13:37 Tanaka
 * ca_syukei_flag��ǉ�
 * 
 * 78    99/08/03 13:13 Nakata
 * 
 * 77    99/07/27 12:16 Tanaka
 * JIHANA_ARCMIN�萔��ǉ��B����VENDING_ARCID�͔p�~�̕���
 * 
 * 76    99/07/26 10:31 Tanaka
 * sizeof(pos_basic_t)==72�ƂȂ�悤����
 * 
 * 75    99/07/24 12:17 Hoshiyam
 * 
 * 74    99/07/24 10:17 Hoshiyam
 * 
 * 73    99/07/24 10:04 Hoshiyam
 * 
 * 73	 99/07/23 17:40 Hoshiyama
 * �ڑ��@��ݒ�łW��܂Ŏ��̋@�ƒʐM�ł���悤�ɕύX�����B
 *
 * 72    99/07/07 9:39 Tanaka
 * ���ʋ����t���O��ǉ�
 * pmf_basic�t�@�C���̃T�C�Y��72�o�C�g�ɂȂ�悤����
 * 
 * 71    99/07/02 16:49 Tanaka
 * ���������ŁA���ʗa���̋ʐ���WORD����DWORD�֊g��
 * 
 * 70    99/07/02 10:34 Fukuoka
 * �o�n�r��{�ݒ�ɋʊ��Z�l��ǉ�
 * 
 * 69    99/06/29 18:34 Tanaka
 * POS��{�ݒ��lcd_clear_flag��ǉ�
 * 
 * 68    99/06/22 15:00 Tanaka
 * ����͗}���t���O����{�ݒ�ɒǉ�
 * 
 * 67    99/06/21 15:14 Tanaka
 * ���̋@�A�_�v�^��ARC ID���`
 * 
 * 66    99/06/18 17:24 Tanaka
 * ���ʗ���萔�̊ԈႢ���C��
 * 
 * 65    99/06/18 16:24 Tanaka
 * ���ʗ����\���̂�ǉ�
 * 
 * 64    99/06/18 13:59 Tanaka
 * �`�P�b�g�v�����^�W�v�t�@�C����32�~48�o�C�g�Ɋg��
 * 
 * 63    99/06/18 10:55 Tanaka
 * 
 * 62    99/06/18 10:39 Tanaka
 * ���ʊ֘A�̗���萔��ǉ�
 * 
 * 61    99/06/18 0:25 Tanaka
 * 99/6/17��ƏI����
 * 
 * 60    99/06/14 9:04 Fukuoka
 * ���ʓ���A���̋@�󎚗p�{�^���Ή�
 * 
 * 59    99/06/14 3:20 Tanaka
 * ���ʑΉ�(�f�o�b�O�͕s�\��)
 * 
 * 58    99/06/11 9:44 Tanaka
 * change_level, tokushu_select[]��ǉ�
 * 
 * 57    99/06/10 16:32 Nao
 * 
 * 55    99/06/10 13:36 Tanaka
 * seg_level���[�N��ǉ�
 * 
 * 54    99/06/10 11:49 Nao
 * dmy1 -> dmy1[12]
 * 
 * 49    99/06/08 13:15 Fukuoka
 * ���ʓ���\���̒ǉ�
 * 
 * 48    99/06/07 18:52 Tanaka
 * PMF_SNIPPOU�t�@�C���ǉ�
 * 
 * 47    99/06/07 18:31 Tanaka
 * ���ʒ��Ԕ�
 * 
 * 46    99/06/07 15:12 Fukuoka
 * �V�Z�O�ݒ�t�@�C���\���̒ǉ�
 * 
 * 45    99/05/14 18:34 Tanaka
 * ����POS���݃G���[�R�[�h��ǉ�
 * 
 * 44    99/04/16 23:00 Tanaka
 * ���v�f�[�^�ۗ̕������̈Ӗ��Â���ύX
 * 
 * 43    99/04/16 21:55 Tanaka
 * �݌ɍ\����pos_zaiko_t�́Asigned long�Ƃ���
 * 
 * 42    99/04/16 21:00 Tanaka
 * pos_ticket_t�ɃG�[�X�d���p�̃t�B�[���h(�p�X���[�h�A�������t)�i�[
 * 
 * 41    99/04/16 14:57 Tanaka
 * ���o�@�O���[�o���ϐ�pos_harai��ǉ�
 * 
 * 40    99/04/16 11:35 Tanaka
 * POS-PC(CA)��ARCID�𐳎�����(0x9e)
 * 
 * 39    99/04/16 1:14 Tanaka
 * pos_total_flag�̒ǉ�
 * 
 * 38    99/04/13 23:44 Tanaka
 * �G���[�R�[�h ERR_INT_FILE ��ǉ�
 * 
 * 37    99/04/09 11:44 Tanaka
 * �����d�l�̕ύX�ɂ��킹�A�ǉ��E�폜
 * 
 * 36    99/04/02 19:20 Tanaka
 * tokushu_items�̒ǉ�
 * 
 * 35    99/04/02 17:53 Tanaka
 * ERR_HORYU_PAS�萔��ǉ�
 * 
 * 34    99/04/02 14:51 Tanaka
 * �G���[�R�[�h��V�K�ǉ�
 * 
 * 33    99/03/31 23:44 Tanaka
 * 
 * 32    99/03/30 21:01 Tanaka
 * ��ʌi�i10000��
 * 
 * 31    99/03/29 9:58 Tanaka
 * ���[�J�[�R�[�h�̒ǉ�(HALSYSTEM,ACEDENKEN)
 * 
 * 30    99/03/25 17:43 Tanaka
 * change_ronly_flag�̓���
 * 
 * 29    99/03/25 10:50 Tanaka
 * pos_item_t���̑��̃}�X�^�[�C���f�b�N�X�Ή��ASUM_MAX�萔�ǉ�
 * 
 * 28    99/03/15 20:46 Tanaka
 * �]��ʋ敪�萔��ǉ�
 * 
 * 27    99/03/12 19:59 Tanaka
 * �\���̂̃A���C�������g���Q�o�C�g�ɒ���
 * 
 * 26    99/03/12 15:30 Tanaka
 * PMF_ZAIKOSUM�t�@�C����ǉ�
 * 
 * 25    99/03/11 19:17 Tanaka
 * ���v�󎚒萔���s���ɒ�`����Ă����̂ŏC��
 * 
 * 24    99/03/11 18:50 Tanaka
 * �s�v�ȋ��`�����[�N�̍폜
 * 
 * 23    99/03/11 14:07 Tanaka
 * ���萔�폜
 * 
 * 21    99/03/08 2:13 Tanaka
 * PRN_JOB_NIKKEI�萔��ǉ�
 * 
 * 20    99/03/08 1:51 Tanaka
 * �����̈ꕔ�萔��ύX(GRIREKI_DEPOSIT��GRIREKI_GET)
 * 
 * 19    99/03/06 23:11 Tanaka
 * 3/4��xls�t�@�C���ɑΉ��A�啝��������
 * 
 * 18    99/03/04 19:21 Tanaka
 * �ۗ���������ύX(3/3��xls�t�@�C���Ɋ�Â�)
 * 
 * 17    99/03/04 11:25 Tanaka
 * �󎚒萔PRN_JOB_HORYU�ǉ�
 * 
 * 16    99/03/03 14:49 Tanaka
 * �󎚃W���u�萔�ǉ�
 * 
 * 15    99/03/02 22:07 Tanaka
 * ����萔�A���̑��ǉ�
 * 
 * 14    99/02/24 13:58 Tanaka
 * pos_tool.h�Ɋ܂܂�Ă������[�J���\���̂��ڂ���
 * 
 * 13    99/02/22 19:15 Matsuura
 * �ǉ���ʂɕ����ă��[�N�ǉ�
 * 
 * 12    99/02/21 23:35 Tanaka
 * �����֘A�̒萔�ǉ�
 * 
 * 11    99/02/19 20:14 Tanaka
 * pos_group_dat_t�ɁA�ePOS�̏�ԊǗ��p���[�N����������
 * 
 * 10    99/02/19 14:26 Tanaka
 * �l�b�g���[�N�O���[�v�Ǘ��ŁA�e�@��X�e�[�^�X�Ǘ��e�[�u����ǉ�
 * (�ePOS���g�p����)
 * 
 * 9     99/02/16 20:40 Tanaka
 * pos_offline_flag��ǉ�
 * 
 * 8     99/02/16 17:14 Tanaka
 * �t�B���[�ʐM�ɍ��v�����o�[�W�������A�t���O��ǉ�
 * 
 * 7     99/02/16 14:51 Tanaka
 * pos��ԃt���O(pos_???_flag)��ǉ�
 * 
 * 6     99/02/09 18:45 Tanaka
 * �O���@��̏��i�o�^�Apos_boot_flag��ǉ�
 * 
 * 5     99/02/08 14:29 Tanaka
 * �t�@�C���N���A�v���t���O���폜
 * 
 * 4     99/02/05 22:55 Tanaka
 * 990203��xls�t�@�C���ɂ��킹�đ啝�ύX
 */

#ifndef _pos_work_h_
#define _pos_work_h_

//#include "typedef.h"		yumipos del
//#include "time_68k.h"		yumipos del

/*---[ �萔��` ]------------------------------------------------------------*/

/* �ڑ��䐔�Ȃ� */
#define POS_MAX			8				/* POS�̌� */
#define TP_MAX			32				/* �`�P�b�g�v�����^�̌� */
#define HARAI_MAX		8				/* ���o�@�̌� */
#define EXTDEV_MAX		8				/* �O���ڑ��@��̌� */
#define CARDINQ_MAX		8				/* �J�[�h�ƍ��@�̌� */
#define JIHANA_MAX		8				/* ���̋@�A�_�v�^�[�̌� */
#define V_CDS_MAX		8				/* ���̋@�J�[�h�ƍ��@ */
#define SRIF_MAX        8				/* ���[���v���C�@ */

/* �i���Ȃ� */
#define TOKUSHU_MAX		4				/* ����i�i�� */
#define IPPAN_MAX		10000			/* ��ʌi�i�� */
#define ITEM_MAX		IPPAN_MAX+TOKUSHU_MAX	/* ���i�}�X�^�̍ő吔 */
#define EXT_ITEM_MAX	36				/* �O���@�폤�i�� */
#define BUMON_MAX		256				/* ���吔 */
#define ETC_MAX			10				/* ���̑��� */
#define CONST_MAX		5				/* �萔�ݒ�̍ő吔 */
#define CONSTSET_MAX	5				/* �萔�g�ݍ��킹�ݒ�̍ő吔 */
#define CHANGE_MAX		40				/* ��x�̌��������Ō����\�ȕi�� */
#define SUM_MAX			2000			/* �W�v�����ň����ő�L�����i�� */
#define CLOSE_ITEM_MAX	128				/* �����ߐݒ�o�^�ő吔 */
#define BACK_ITEM_MAX		40			/* �ő�i�i�I��(�ԕi�A�߂�����) */
#define ITEM_RECERVE_MAX		9			/* �P�x�̒ʐM�̍ő�擾�i�i�f�[�^ */

/* ���� */
#define ADMIN_MAX		30				/* �ӔC�ҍő�l�� */
#define ADMIN_LEVEL_MAX	30				/* �ӔC�҃��x�������ő吔 */
#define KEY_ITEM_MAX	64				/* ���i�_�C���N�g�L�[�̌� */
#define ADMIN_REC_MAX	5				/* �ӔC�ҕʓ��v�Ǘ��E���[�v�� */
#define REC_FILE_MAX	9				/* �L�����t�@�C���̋L������ */
#define TICKET_NUM_MAX	9				/* ticket_num�ő吔 */
#define MEMBER_HISTORY_MAX 512			/* ��������t�@�C���ő僌�R�[�h�� */

/* �����񒷂� */
#define HALL_NAME_LEN	14				/* �z�[���̖��̒���(\0�Ȃ�) */
#define TYPE_NAME_LEN	2					/* �e��ʂ̕\��������(\0�Ȃ�) */
#define ITEM_NAME_LEN	24				/* ��ʁE����i�i�̖��̒���(\0�Ȃ�) */
#define BUMON_NAME_LEN	24				/* ����̖��̒���(\0�Ȃ�) */
#define EXTDEV_NAME_LEN	8				/* �O���ڑ��@��̖��̒���(\0�Ȃ�) */
#define PAGE_NAME_LEN	20				/* �^�b�`�p�l���y�[�W��(\0�Ȃ�) */
#define ADMIN_NAME_LEN	24				/* �ӔC�Ҏ�������(\0�Ȃ�) */

/* �o�[�R�[�h���� */
#define BC_BIN_LEN		8				/* �o�[�R�[�h��(���k�`���A\0�Ȃ�) */
#define BC_DEC_LEN		16				/* �o�[�R�[�h��(�ʏ�`���A\0�Ȃ�) */
#define BC_TOTAL_LEN	64				/* �o�[�R�[�h��(�ő��M���A\0���� */
#define BC_HEAD_LEN		16				/* �o�[�R�[�h��(�w�b�_�����A\0���� */
#define BC_CODE_LEN		25				/* �o�[�R�[�h��(�R�[�h�����A\0���� */

/* PAS��� */
#define PAS_P			0x01			/* 1��ʖڑI�� */
#define PAS_A			0x02			/* 2��ʖڑI�� */
#define PAS_S			0x04			/* 3��ʖڑI�� */
#define PAS_EX		0x08			/* 4��ʖڑI�� */

#define JRL_A_PAS_P			0x01			/* A�X��JRL�A���V�[�g�Ŏg�p����1��ʖ� */
#define JRL_A_PAS_A			0x02			/* A�X��JRL�A���V�[�g�Ŏg�p����2��ʖ� */
#define JRL_A_PAS_S			0x03			/* A�X��JRL�A���V�[�g�Ŏg�p����3��ʖ� */
#define JRL_A_PAS_EX		0x04			/* A�X��JRL�A���V�[�g�Ŏg�p����4��ʖ� */
#define JRL_B_PAS_P			0x05			/* B�X��JRL�A���V�[�g�Ŏg�p����1��ʖ� */
#define JRL_B_PAS_A			0x06			/* B�X��JRL�A���V�[�g�Ŏg�p����2��ʖ� */
#define JRL_B_PAS_S			0x07			/* B�X��JRL�A���V�[�g�Ŏg�p����3��ʖ� */
#define JRL_B_PAS_EX		0x08			/* B�X��JRL�A���V�[�g�Ŏg�p����4��ʖ� */

#define	PAS_MAX			3				/* �S���łR���(���݂�4���) */

/* �O�������o���@���[���ݒ� */
#define	LANE_MAX		8				/* �ő僌�[���ݒ萔 */

/* �]��ʋ敪 */
#define REST_NONE		0				/* �]��ʏ���:�I���s�\ */
#define REST_GET		1				/* �]��ʏ���:��荞�� */
#define REST_SERVICE	2				/* �]��ʏ���:�T�[�r�X */
#define REST_HORYU		3				/* �]��ʏ���:�ۗ��� */
#define REST_SAVINGS	4				/* �]��ʏ���:���� */
#define REST_RETURN		5				/* �]��ʏ���:�߂��� */
#define REST_PICKUP		6				/* �]��ʏ���:�E���� */

/* �� */
#define TAX_INT			1				/* ����� ���� */
#define TAX_EXT			2				/* ����� �O�� */

/* �W�v�敪 */
#define SUMAREA_NONE	0				/* ���� */
#define SUMAREA_POS		1				/* POS�ʏW�v */
#define SUMAREA_COUNTER	2				/* �J�E���^�ʏW�v */
#define SUMAREA_FLOOR	3				/* �t���A�ʏW�v */
#define SUMAREA_ALL		4				/* �S�X�W�v */

/* �W�v����敪 */
#define SUMTYPE_NONE	0				/* ���� */
#define SUMTYPE_TOUKEI	1				/* ���v���̈�� */
#define SUMTYPE_BUMON	2				/* ����ʌi�i�̈�� */
#define SUMTYPE_TOP30	3				/* �i�i����TOP30�i�ڂ̈�� */
#define SUMTYPE_TODAY	4				/* �����i�i�����S��� */
#define SUMTYPE_NAKA	5				/* �w��i�i�����߈�� */
#define SUMTYPE_BUMON_APP 6				/* �w�蕔��W�v��� */
#define SUMTYPE_ALLITEMS  7				/* �w�蕔��W�v��� */
#define SUMTYPE_SKIP_BUMON  8				/* �w�蕔��W�v��� */

/* �󎚃W���u */
#define PRN_JOB_RECEIPT	0x00			/* ���V�[�g�� */
#define PRN_JOB_HORYU	0x01			/* �ۗ����� */
#define PRN_JOB_NIKKEI	0x02			/* ���v�� */
#define PRN_JOB_HRIREKI	0x03			/* �ۗ��������� */
#define PRN_JOB_CYDMNIP 0x04			/* ���ʓ���� */
#define PRN_JOB_JIHANKI 0x05			/* ���̋@�� */
#define PRN_JOB_KTERM	0x06			/* ����[���� */
#define PRN_JOB_ZAIKO	0x07			/* �݌ɏ��� */
#define PRN_JOB_ZAIKO_SHORI	0x08		/* �݌ɏ������� */
#define PRN_JOB_ZAIKO_TITLE	0x09		/* �݌ɏ������و� */
#define PRN_JOB_ERROR_LOG	0x0a		/* �G���[���������� */
#define PRN_JOB_MARS_ERROR	0x0b		/* �G���[���������� */
#define PRN_JOB_CONFIG	0x0c			/* �o�n�r�ݒ�� */
#define PRN_JOB_SAVINGS 0x0d			/* ���ʏ����G���[�� */

/* �l�b�g���[�N */
#define POS_ARCMIN		0x90			/* POS�̍ŏ�ARC ID */
#define TP_ARCMIN		0x70			/* TP�̍ŏ�ARC ID */
#define ACE_ARCID		200				/* �G�[�X�d���W���Ǘ��A�_�v�^ARC ID */
#define CA_ARCID		158				/* CA��ARC ID */
#define KS_ARCID		0xf9			/* KS��ARC ID */
#define VENDING_ARCID	0x40			/* ���̋@�A�_�v�^ARC ID(���ֈڍs) */
#define JIHANA_ARCMIN	0x40			/* ���̋@�A�_�v�^�̍ŏ�ARC ID */
#define SRIF_ARCMIN		0x20			/* SRIF�̍ŏ�ARC ID */
#define VCD_ARCMIN		0xb0			/* VCD�̍ŏ�ARC ID */
#define CDS_ARCMIN		0xa0			/* CDS�̍ŏ�ARC ID */

/* ���[�J�[�R�[�h */
#define HALSYSTEM_ID	1				/* �n���V�X�e�� */
#define ACEDENKEN_ID	2				/* �G�[�X�d�� */


/* �����o���@���[�J�[�R�[�h */
#define	HARAI_NONE_ID		0				/* �����o���@���g�p */
#define	HARAI_GLORY_ID		1				/* �O���[���[�� */
#define	HARAI_SANKYO_ID		2				/* �O���� */
#define	HARAI_TMI_ID		3				/* TMI�� */
#define	HARAI_SUBARU_ID		4				/* �X�o���� */
#define HARAI_MARS_ID		5				/* �}�[�X�� */
#define HARAI_HIROTAC_ID	6				/* �q���^�b�N�� */
#define	HARAI_NO_USE_ID		HARAI_NONE_ID	/* alias */

#define PRICE_DEC_BAS	10				/* �i�i�P�������_�Ή���l
											1:�Ή��Ȃ�(�ʏ�ǂ���)
											10:�����_�ȉ��P���܂őΉ�
											100:�����_�ȉ��Q���܂őΉ�
										 */

/* �������� ���(type1, type2, �ڍ׋敪) */
#define GRIREKI_SERIAL_MAX	3			/* �X���̗����̃V���A���ԍ���ۑ�������� */
#define GRIREKI_CHANGE	0x0001			/* �ʏ���� */
	#define GRIREKI_GET		0x0001			/* �a���� */
		#define GRIREKI_GTICKET	0x0001			/* �`�P�b�g�a���� */
		#define GRIREKI_GTENYU	0x0002			/* ����͗a���� */
		#define GRIREKI_GHORYU	0x0004			/* �ۗ����a���� */
		#define GRIREKI_GSAVINGS 0x0008			/* ���ʗa���� */
	#define GRIREKI_IPPAN	0x0002			/* ��ʌi�i�����o�� */
	#define GRIREKI_TOKUSHU	0x0004			/* ����i�i�����o�� */
	#define GRIREKI_REST	0x0008			/* �]��� */
		#define GRIREKI_ROVER	0x0001			/* �]��ʂ������i�i */
		#define GRIREKI_RGET	0x0002			/* �]��ʂ���荞�� */
		#define GRIREKI_RSERVICE 0x0004			/* �T�[�r�X�� */
		#define GRIREKI_RRETURN	 0x0008			/* �߂�(�i�i) */
		#define GRIREKI_RPICKUP	 0x0010			/* �E���� */
		#define GRIREKI_RHORYU	 0x0020			/* �ۗ������� */
	#define GRIREKI_MIPPAN	0x0010			/* ��ʌi�i�ԕi */
		#define GRIREKI_MHENPIN	 0x0001			/* �ԕi */
		#define GRIREKI_MRETURN	 0x0002			/* �߂�(�i�i) */
	#define GRIREKI_MTOKUSHU 0x0020			/* ����i�i�ԕi */
		#define GRIREKI_THENPIN	 0x0001			/* �ԕi */
		#define GRIREKI_TRETURN	 0x0002			/* �߂�(�i�i) */
	#define GRIREKI_SAVINGS	0x0040			/* ���ʕ����o�� */
	#define GRIREKI_SAVINGSDAT	0x0060		/* ���ʑJ�� */
	#define GRIREKI_MARSOK	0x0080			/* �}�[�X���o�� */
	#define GRIREKI_MARSNG	0x0081			/* �}�[�X������ */
	#define GRIREKI_CHGERR	0x8000			/* �G���[ */
		#define GRIREKI_ETCK	0x0001			/* �`�P�b�g�����G���[ */
		#define GRIREKI_ETYO	0x0002			/* ���ʏ����G���[ */
		#define GRIREKI_EHARAI	0x0004			/* ���o�������G���[ */
		#define GRIREKI_EOTHER	0x0008			/* ���̑��G���[ */

#define GRIREKI_UNYOU	0x0002			/* �^�p���� */
	#define GRIREKI_OPEN	0x0001			/* �c�ƊJ�n�����i�J�X�����j */
	#define GRIREKI_START	0x0002			/* �c�ƊJ�n�����i�J�X�����Ȃ��j */
	#define GRIREKI_HSTART	0x0003			/* �����J�n���� */
	#define GRIREKI_HSTOP	0x0004			/* ������~���� */
	#define GRIREKI_CLOSE	0x0005			/* �X���� */
	#define GRIREKI_RECORD	0x0006			/* �蓮�f�[�^�L�� */
	#define GRIREKI_CLEAR	0x0007			/* �蓮�f�[�^�N���A */
	#define GRIREKI_OPENCLR	0x0008			/* OPEN�O�N���A���� */
	#define GRIREKI_TESTCHG	0x0009			/* �e�X�g�����J�n���� */
	#define GRIREKI_MSTPC	0x000a			/* POSPC�i�i�}�X�^��M(POSPC�哮) */
	#define GRIREKI_MSTPOS	0x000b			/* POSPC�i�i�}�X�^��M(POS�哮) */
	#define GRIREKI_AMSTPC	0x000c			/* POSPC�ӔC�҃}�X�^��M(POSPC�哮) */
	#define GRIREKI_AMSTPOS	0x000d			/* POSPC�ӔC�҃}�X�^��M(POS�哮) */
	#define GRIREKI_UNYERR	0x8000			/* �G���[���� */
		#define GRIREKI_EDATE	0x0001			/* �c�Ɠ��t�擾�����G���[ */
		#define GRIREKI_EJIHANA 0x0002			/* ���̋@�f�[�^�擾�G���[ */
		#define GRIREKI_UPZAIK	0x0003			/* �݌Ƀf�[�^�X�V�G���[ */
		#define GRIREKI_FCLEAR	0x0004			/* �t�@�C���N���A�G���[ */
		#define GRIREKI_JCLEAR	0x0005			/* ���̋@�A�_�v�^�ւ̃f�[�^�N���A�w���G���[ */
		#define GRIREKI_MARSCL	0x0006			/* �}�[�X�N���A�����G���[ */
		#define GRIREKI_MARSCA	0x0007			/* �}�[�X�J�����_�ݒ�G���[ */
		#define GRIREKI_TPCLEAR 0x0008			/* TP�N���A�����G���[ */
		#define GRIREKI_TPSTOP	0x004c			/* TP������ԃG���[ */
		#define GRIREKI_TSUMINT	0x0040			/* PMF_TOUKEISUM�������G���[ */
		#define GRIREKI_TZIKINT	0x0041			/* PMF_TZAIKOSUM�������G���[ */
		#define GRIREKI_PTOUKEI	0x0042			/* POS���v�f�[�^�W�v�G���[ */
		#define GRIREKI_PZAIKO	0x0043			/* POS�݌Ƀf�[�^�W�v�G���[ */
		#define GRIREKI_TPFILE	0x0044			/* TP�t�@�C���W�v�G���[ */
		#define GRIREKI_CHONIP	0x0045			/* ���ʓ���f�[�^�擾�G���[ */
		#define GRIREKI_JIHFILE	0x0046			/* ���̋@�t�@�C���W�v�G���[ */
		#define GRIREKI_CACOMM 	0x0047			/* CA�ʐM�G���[ */
		#define GRIREKI_CA10MIN	0x0048			/* CA10���^�C���A�E�g */
		#define GRIREKI_PCCOMM	0x0049			/* POSPC�ʐM�G���[ */
		#define GRIREKI_PC10MIN	0x004a			/* POSPC10���^�C���A�E�g */
		#define GRIREKI_PCERR	0x004b			/* POSPC�G���[���擾�����G���[ */
		#define GRIREKI_HIROTAC	0x004d			/* �q���^�b�N�W�v�G���[ */
		#define GRIREKI_MSTCOMM	0x0080			/* POSPC�ʐM�G���[ */
		#define GRIREKI_MST2S	0x0081			/* POSPC�����҂�(2�b)�^�C���A�E�g�G���[ */
		#define GRIREKI_MST10M	0x0082			/* POSPC�����҂�(10��)�^�C���A�E�g�G���[ */
		#define GRIREKI_MSTNAK	0x0083			/* POSPCNAK��M�G���[ */
		#define GRIREKI_MSTNG	0x0084			/* ��������NG */
		#define GRIREKI_MSTRECV	0x0085			/* POSPC�哮�̃}�X�^��M�����G���[ */

#define GRIREKI_ZAIKO	0x0003			/* �݌ɊǗ����� */
	#define GRIREKI_STORE	0x0001			/* �i�i���ɏ��� */
	#define GRIREKI_MOVE	0x0002			/* �݌Ɉړ����� */
	#define GRIREKI_STOCK	0x0003			/* �i�i�I������(�ePOS) */
	#define GRIREKI_STCKALL	0x0004			/* �i�i�I������(�S�X) */
	#define GRIREKI_ADJUST	0x0005			/* �݌ɒ������� */
	#define GRIREKI_ZAIKERR	0x8000			/* �G���[���� */
		#define GRIREKI_NOTITEM	0x0001			/* ���i���I�� */
		#define GRIREKI_NOTNUM	0x0002			/* ������������ */
		#define GRIREKI_MINUS	0x0003			/* �}�C�i�X���� */
		#define GRIREKI_SRCPOS	0x0004			/* �ړ���POS�G���[ */
		#define GRIREKI_SRCNUM	0x0005			/* �ړ������͈̓G���[ */
		#define GRIREKI_ZCOMM	0x0006			/* �ʐM�G���[ */
		#define GRIREKI_DATA	0x0007			/* �f�[�^���Ɖ�G���[ */
		#define GRIREKI_CHGING	0x0008			/* �ړ���POS�����������G���[ */
		#define GRIREKI_ZWDAT	0x0009			/* �݌Ƀf�[�^�����ݎ��s */
		#define GRIREKI_TANAD	0x000a			/* �I����ʕ\�����G���[ */
		#define GRIREKI_ATANAD	0x000b			/* �S�X�I����ʕ\�����G���[ */
		#define GRIREKI_ZGDAT	0x000c			/* �S�X�݌Ƀf�[�^�擾���s(�S�X�I���W�v�ʐM�G���[) */
		#define GRIREKI_MASTERR	0x000d			/* �S�X�݌Ƀ}�X�^�G���[(�S�X�I���W�v�}�X�^�G���[) */

#define GRIREKI_KAIIN	0x0004			/* ����[������ */
	#define GRIREKI_KADD	0x0001			/* ����ǉ� */
	#define GRIREKI_KDEL	0x0002			/* ����폜 */
	#define GRIREKI_PASSCLR	0x0003			/* �Ï؃N���A */
	#define GRIREKI_KMOVE	0x0004			/* ����ڍs */
	#define GRIREKI_KCHG	0x0005			/* �����ԕύX */
	#define GRIREKI_KERR	0x8000			/* ��������G���[ */

#define GRIREKI_CONFIG	0x0005			/* �ݒ菈�� */
	#define GRIREKI_EXCHG	0x0001			/* �����萔�ݒ� */
	#define GRIREKI_TIMEDEF	0x0002			/* �����ݒ� */
	#define GRIREKI_SRIF	0x0003			/* ���[I/F�ݒ� */
	#define GRIREKI_CFGERR	0x8000			/* �G���[���� */
		#define GRIREKI_WDAT	0x0001				/* �t�@�C���ւ̃f�[�^�����ݏ������s */
		#define GRIREKI_SRIFCOM	0x0002				/* SRIF�ʐM�G���[ */
		#define GRIREKI_SRIFTO	0x0003				/* SRIF�����҂��^�C���A�E�g */

#define GRIREKI_CARD	0x0006			/* �J�[�h���� */
	#define GRIREKI_CARDERR	0x8000			/* �J�[�h�����G���[ */
		#define GRIREKI_CALIB	0x0001				/* �^�p��ʈȊO�ł̃L�����u���[�V�����J�[�h�}�� */
		#define GRIREKI_TEST	0x0002				/* �c�ƒ��̃e�X�g�����J�[�h�}�� */
		#define GRIREKI_CANNOT	0x0003				/* ���̑��̎g�p�o���Ȃ��ݒ�J�[�h */
		#define GRIREKI_NENTRY	0x0004				/* ���o�^�ӔC�҃J�[�h */
		#define GRIREKI_COMCODE	0x0005				/* ��ЃR�[�h�G���[ */
		#define GRIREKI_ADCARD	0x0006				/* �قȂ�ӔC�҃J�[�h */

/* �݌ɊǗ��p�^�[�� */
#define NON_ZAIK_CTRL	0x00				/* �݌ɊǗ��Ȃ� */
#define ALL_ZAIK_CTRL	0x01				/* �S�X�݌ɊǗ� */
#define IND_ZAIK_CTRL	0x02				/* �ePOS�݌ɊǗ� */
#define TDY_ZAIK_CTRL	0x03				/* �����݌ɊǗ� */


/* ���̑� */

/* �G���[�R�[�h */
#define ERR_HORYU_INFO		-10				/* �ۗ������G���[(�����g�p) */
#define ERR_HORYU_NFND		-11				/* �ۗ����Y���Ȃ��G���[ */
#define ERR_HORYU_ALRDY		-12				/* �ۗ����g�p�ς݃G���[ */
#define ERR_HORYU_COMM		-13				/* �ۗ����ʐM�G���[ */
#define ERR_HORYU_PAS		-14				/* �ۗ���PAS�Ⴂ�G���[ */
#define	ERR_HORYU_PRINT		-15				/* �ۗ�������G���[(�����g�p) */
#define ERR_HORYU_DATA		-16				/* �ۗ����Y���Ȃ��G���[(�����ُ�) */
#define ERR_HDAT_SERIAL		-121			/* �ۗ����V���A���ԍ��s��v */
#define ERR_HDAT_REC		-122			/* �ۗ������������Ȃ� */
#define ERR_HDAT_PAS		-123			/* �ۗ���PAS�G���[ */
#define ERR_HDAT_NUM		-124			/* �ۗ����a����ʐ��G���[ */
#define ERR_HDAT_CODE		-125			/* �ۗ����f�[�^�ُ� */
#define ERR_HORYU_TYPE		-126			/* �ۗ�����ʃf�[�^�ُ� */
#define ERR_HORYU_POSNO		-127			/* �ۗ����o�n�r�ԍ��ُ� */
#define ERR_HREC_PAS		-128			/* �ۗ������������Ȃ� */
#define ERR_HREC_NUM		-129			/* �ۗ���PAS�G���[ */
#define ERR_HORYU_PASTYPE	-130			/* �ۗ���PAS�G���[ */
#define ERR_HORYU_SERIAL	-131			/* �ۗ����V���A���ԍ����O�ȉ� */
#define ERR_INT_FILE		-20				/* �t�@�C���A�N�Z�X�G���[ */
#define ERR_INT_FULL		-21				/* �o�b�t�@�t���G���[(�����g�p) */
#define ERR_INT_NFND		-22				/* �o�b�t�@�Y���Ȃ��G���[(�����g�p) */
#define ERR_INT_COMM		-23				/* �ʐM���s�G���[ */
#define ERR_CHG_9OVER		-30				/* �a����X���z���G���[ */
#define ERR_CHG_NOINPUT		-31				/* �a����Ȃ��I���G���[ */
#define ERR_CHG_NORIREKI	-32				/* �����Ȃ����V�[�g�G���[ */
#define ERR_CHG_RESTNONE	-33				/* �]��ʏI���G���[ */
#define ERR_CHG_NOHORYU		-34				/* �]��ʕۗ��G���[ */
#define ERR_CHG_CHANGING	-35				/* �������G���[ */
#define ERR_CHG_CHGMAX		-36				/* �X���b�g�t���G���[ */
#define ERR_CHG_ITEMNFND	-37				/* �Y���i�i�Ȃ��G���[ */
#define ERR_CHG_NOTENOUGH	-38				/* �ʕs���G���[ */
#define ERR_CHG_RENUM		-39				/* ���ʕύX�G���[ */
#define ERR_CHG_NOPAS		-40				/* PAS���m��G���[ */
#define ERR_CHG_DELETE		-41				/* �폜�X���b�g�Ȃ��G���[ */
#define ERR_CHG_RECEIPT		-42				/* ���V�[�g�͈̓I�[�o�[�G���[ */
#define ERR_CHG_CANTALL		-43				/* ���O����!=�I���G���[ */
#define ERR_CHG_KAKUTEI		-44				/* �m���̗a�ʓ��̓G���[ */
#define ERR_CHG_KAKUCAN		-45				/* ���ʑ����̊m�����G���[ */
#define ERR_CHG_USERCAN		-46				/* ���q�l�ɂ��L�����Z�� */
#define ERR_CHG_KSERR		-47				/* �j�r�Ƃ̒ʐM�G���[ */
#define ERR_CHG_KAKUERR		-48				/* �m��G���[ */
#define ERR_CHG_TENYU		-49				/* ����͑��쐧���G���[(�����g�p) */
#define	ERR_CHG_SAVING		-50				/* �ƍ��@�����݃G���[ */
#define ERR_CHG_CARD		-51				/* �J�[�h�ƍ��@�ʐM�G���[ */
#define ERR_CHG_LICENSE		-87				/* ���쐧�����J�[�h�G���[ */
#define ERR_CHG_RETURN		-88				/* �߂��ʏ������쐧���G���[ */
#define ERR_CHG_PICKUP		-89				/* �E���ʏ������쐧���G���[ */
#define ERR_CHG_MODOSHI		-94				/* �߂��������쐧���G���[ */
#define ERR_CHG_ITEMPRICE	-95				/* �i�i�P���G���[ */
#define ERR_CHG_ITEMPRICE2	-103		/* �i�i�P���G���[(�[���G���[) */
#define ERR_CHG_ITEMDATE	-96				/* �i�i���t�G���[ */
#define ERR_CHG_LIM_BACK	-98				/* �ԕi�����G���[ */
#define ERR_CHG_LIM_RETURN	-99			/* �߂������G���[ */
#define ERR_TICKET_ILLE		-60				/* �`�P�b�g���G���[ */
#define ERR_TICKET_NFND		-61				/* �`�P�b�g�₢���킹���Ȃ��G���[ */
#define ERR_TICKET_ALRDY	-62				/* �`�P�b�g�g�p�ς݃G���[ */
#define ERR_TICKET_COMM		-63				/* �`�P�b�g�ʐM�G���[ */
#define ERR_TICKET_PAS		-64				/* �`�P�b�gPAS�Ⴂ�G���[ */
#define ERR_TICKET_OFFLINE	-65				/* �`�P�b�g�I�t���C���x�� */
#define ERR_TICKET_ONLINE   -109			/* �`�P�b�g�I�����C���敪�ُ�(�G�[�X) */
#define ERR_TICKET_PAPER	-66				/* �`�P�b�g���؂�G���[ */
#define ERR_TICKET_READ		-67				/* �A���L�[�������G���[ */
#define ERR_INPUT_CODE		-68				/* ����͂ɂ��`�P�b�g�R�[�h�G���[ */
#define ERR_TICKET_DATA		-69				/* �`�P�b�g �o�[�R�[�h�f�[�^�ُ� */
#define ERR_TDAT_PAS		-111			/* �`�P�b�g�o�[�R�[�hPAS�Ⴂ */
#define ERR_TDAT_CODE		-112			/* �`�P�b�g�o�[�R�[�h�f�[�^�ُ� */
#define ERR_TDAT_TP			-113			/* �`�P�b�g�o�[�R�[�hTP�ԍ��ُ� */
#define ERR_TDAT_DATE		-114			/* �`�P�b�g�o�[�R�[�h���t�ُ� */
#define ERR_TICKET_TYPE		-115			/* �`�P�b�g��ʃf�[�^�ُ� */
#define ERR_TDAT_NUM		-116			/* �`�P�b�g�ʐ��ُ� */
#define ERR_TICKET_PASS		-110			/* �p�X���[�h�ُ� */
#define ERR_TICKET_BARHED	-117			/* �o�[�R�[�h�w�b�_�ُ� */
#define ERR_TICKET_PASTYPE	-118			/* �o�[�R�[�h�w�b�_�ُ� */
#define ERR_TICKET_ERROR	-119			/* �����@��ُ� */
#define ERR_TICKET_SERIAL	-120			/* �`�P�b�g�V���A���ԍ����͈͊O */
#define ERR_HORYU_NONPOS	-140			/* POS���ڑ� */
#define ERR_TICKET_NONTP	-141			/* TP���ڑ� */
#define ERR_HARAI_COMM		-70				/* ���o�@�ʐM�G���[ */
#define ERR_HARAI_ERROR		-71				/* ���o�@���o�G���[ */
#define ERR_HARAI_CHECK		-72				/* ���o�@���o�G���[(�_��) */
#define ERR_HARAI_END		-73				/* ���o�@���o�G���[(17H-18H�ʐM���f) */
#define ERR_NET_CHANGE		-80				/* �l�b�g���[�N���Ō����Ɩ��� */
#define ERR_CHG_CANCELN		-81				/* �ʐM�G���[�Ȃǂɂ�钙�ʏ����̒��f */
#define ERR_CHG_CANCEL2		-82				/* �ʐM�G���[�Ȃǂɂ�钙�ʏ����̒��f */
#define ERR_CHG_CANCEL3		-83				/* �ʐM�G���[�Ȃǂɂ�钙�ʏ����̒��f */
#define ERR_CHG_CANCEL4		-84				/* �ʐM�G���[�Ȃǂɂ�钙�ʏ����̒��f */
#define ERR_CHG_CANCEL5		-85				/* �ʐM�G���[�Ȃǂɂ�钙�ʏ����̒��f */
#define ERR_CHG_CANCEL		-86
#define ERR_MEMBER_KSERR	-101			/* ��������j�r�Ƃ̒ʐM�G���[ */
#define ERR_MEMBER_CAERR	-102			/* ��������b�`�Ƃ̒ʐM�G���[ */
#define ERR_CD_NONE			-90				/* �J�[�h���}�� */
#define ERR_CD_PASSIN		-91				/* �ƍ��@�p�X���[�h���͒� */
#define ERR_CD_PASS_NO		-92				/* �ƍ��@�p�X���[�h������ */
#define ERR_CD_PASS_OK		-93				/* �ƍ��@�p�X���[�h���͍ς� */
#define ERR_FAIL_TRANS		-150			/* ���M�G���[ */
#define ERR_CHG_DENY		-151			/* ���M�G���[ */
#define ERR_RETRY_OVER		-97				/* ���M�G���[ */
#define ERR_CHANGE_CLOSE	-100			/* �S�Ă̐ڑ��@��̒�~���s���܂���ł��� */

/* ���ʏ����֘A(01/09�`) */
#define E_CARDIN_TIMEOUT	-160			/* KS���޲ݒʐM�F��ѱ�� */
#define E_CARDIN_INVALID	-161			/* KS���޲ݒʐM�F������� */
#define E_CARDIN_NOEXIST	-162			/* KS���޲ݒʐM�F������݂��Ȃ� */
#define E_CARDIN_LOCK		-163			/* KS���޲ݒʐM�Fۯ��� */
#define E_CARDIN_REPUB		-164			/* KS���޲ݒʐM�F�Ĕ��s���� */
#define E_POINT_TIMEOUT		-165			/* KS�߲�ı��ߒʐM�F��ѱ�� */
#define E_POINT_OVER		-166			/* KS�߲�ı��ߒʐM�F�a�����߲�Đ����� */
#define E_PASS_TIMEOUT		-167			/* KS�Ïؓo�^�ʐM�F��ѱ�� */
#define E_PASS_KSERR		-168			/* KS�Ïؓo�^�ʐM�FKS����װ���� */
#define E_PASS_TIMEOUT_CA	-189			/* CA�Ïؓo�^�ʐM�F��ѱ�� */
#define E_PASS_KSERR_CA		-190			/* CA�Ïؓo�^�ʐM�FKS����װ���� */
#define E_TYOIN_TIMEOUT		-169			/* KS����(�a��)�ʐM�F��ѱ�� */
#define E_TYOIN_OVER		-170			/* KS����(�a��)�ʐM�F�ő�l���ް */
#define E_TYOIN_KSERR		-171			/* KS����(�a��)�ʐM�FKS����װ���� */
#define E_TYOOUT_TIMEOUT	-172			/* KS����(���o)�ʐM�F��ѱ�� */
#define E_TYOOUT_KSERR		-173			/* KS����(���o)�ʐM�FKS����װ���� */
#define E_CARDOUT_TIMEOUT	-174			/* KS���ޱ�ĒʐM�F��ѱ�� */
#define E_FORCE_BUTTON_Y	-175			/* CDS������ދ����r�o�F���ޔr�o���� */
#define E_FORCE_BUTTON_N	-185
#define E_FORCE_POFF		-176			/* CDS������ދ����r�o�FCDS�d���f */
#define E_ERR_CARDIN		-177			/* CDS�ɉ�����ވȊO�̶��ޑ}�� */
#define E_READ_DATA			-178			/* CDS������ޓǎ�莸�s */
#define E_CARD_EJECT		-179			/* CDS���ދl�܂� */
#define E_POLLING_REPLY		-180			/* CDS�߰�ݸޒʐM�������װ */
#define E_POLLING_REPLY2	-186			/* CDS�߰�ݸޒʐM�������װ */
#define E_POLLING_REPLY3	-187			/* CDS�߰�ݸޒʐM�������װ */
#define E_PULL_OUT			-181			/* CDS������ވ����� */
#define E_PULL_OUT2			-188			/* CDS������ވ�����(���ʓǍ��E�i�i�I������) */
#define E_CARD_LEFT			-182			/* CDS������ގ��Y�� */
#define E_CDS_POWEROFF		-183			/* CDS���ˑR���ޱ�ď�ԂɂȂ��� */
#define E_CONNECTCFG_CDS	-184			/* �ڑ��@��ݒ��CDS���ڑ���� */

/* �����߈󎚔͈́A�X�L�b�v����/�Ȃ� */
#define PRINT_SELF	0x00				/* �󎚔͈́F��POS */
#define PRINT_ALL 	0x01				/* �󎚔͈́F�S�X */
#define SKIP_OFF	 	0x00				/* �X�L�b�v�F�Ȃ� */
#define SKIP_ON 		0x01				/* �X�L�b�v�F���� */

/* �[���G���[�����@�G���[�R�[�h */
#define ARCNET_DOWN_ERR		1000	/* �A�[�N�l�b�g�_�E�� */

/*---[ �\���̐ݒ�(�t�@�C���ƒ������Ă������) ]------------------------------*/

/*
 *
 *	�\���̒�`
 *	����t�@�C��
 *
 */

/*
 *	�t�@�C�����X�V
 */
typedef struct {
	DWORD update_date;				/* �X�V���t(�����`��) */
	DWORD update_time;				/* �X�V����(�����`��) */
	BYTE update_flag;				/* �X�V�t���O(>=0x80�ōX�V����) */
    BYTE wp_record;					/* �L�����O�񏑂����݈ʒu */
	BYTE reserve[2];				/* �\�� */
} pos_update_t;

/*
 *	�L�����
 */
typedef struct {
	DWORD record_date;				/* �L�����t(�����`��) */
	DWORD record_time;				/* �L������(�����`��) */
	DWORD open_date;				/* �J�X���t(�����`��) */
	DWORD open_time;				/* �J�X����(�����`��) */
	DWORD close_date;				/* �X���t(�����`��) */
	DWORD close_time;				/* �X����(�����`��) */
    DWORD daily_rireki_serial;		/* �X���̗��𐔂��L�� */
  /*	BYTE record_C4;					 C4�C���f�b�N�X(���g�p) */
  /*	BYTE reserve_c4;				 C4�\�� */
  /*	BYTE reserve[32];				 �\�� */
} pos_record_t;

/*
 *
 *	�\���̒�`
 *	�ݒ�t�@�C��
 *
 */

/*
 *	POS��{�ݒ�
 */
typedef struct {
	BYTE hall_name[HALL_NAME_LEN];	/* �z�[������(\0�Ȃ�) */
	BYTE hall_name2[HALL_NAME_LEN];	/* �z�[������(\0�Ȃ�) */
	BYTE disp_type_a1[TYPE_NAME_LEN];	/* A�X���1�̕\��������(\0�Ȃ�) */
	BYTE disp_type_a2[TYPE_NAME_LEN];	/* A�X���2�̕\��������(\0�Ȃ�) */
	BYTE disp_type_a3[TYPE_NAME_LEN];	/* A�X���3�̕\��������(\0�Ȃ�) */
	BYTE disp_type_a4[TYPE_NAME_LEN];	/* A�X���4�̕\��������(\0�Ȃ�) */
	BYTE disp_type_b1[TYPE_NAME_LEN];	/* B�X���1�̕\��������(\0�Ȃ�) */
	BYTE disp_type_b2[TYPE_NAME_LEN];	/* B�X���2�̕\��������(\0�Ȃ�) */
	BYTE disp_type_b3[TYPE_NAME_LEN];	/* B�X���3�̕\��������(\0�Ȃ�) */
	BYTE disp_type_b4[TYPE_NAME_LEN];	/* B�X���4�̕\��������(\0�Ȃ�) */
	WORD disp_left_flag;							/* �q���c��ʕ\���L��(TRUE�ŕ\��) */
	BYTE tenyu_card_flag;							/* �J�[�h�ɂ�����͋���(TRUE:�L��) */
	BYTE lcd_clear_flag;							/* �����I������LCD�����t���O(TRUE:����) */
	WORD tama_kan;										/* �ʊ��Z�l�i�R�C���ꖇ������̋ʐ��j */
	BYTE savings_force_flag;					/* ���ʖ₢���킹�Ȃ��t���O */
	BYTE tokushu_auto_select;					/* ����i�i�������Z�ݒ�(TRUE:�L��) */
	BYTE ticket_continue_flag;				/* �`�P�b�g�ǂݏo���A���ǂݍ��݊m�F�t���O(TRUE:�L��) */
	BYTE ext_offer_flag;							/* ����i�i�œK���t���O�iTRUE:�L���j */
	BYTE card_interlock_flag;					/* CDS �J�[�h�C�� �C���^�[���b�N�t���O(TRUE:�L��) */
	BYTE err_ticket_read;							/* �G���[�`�P�b�g�����ǂݍ��� */
	BYTE barcode_buzzer;							/* �o�[�R�[�h�X�L�������iTRUE:�L���j */
	BYTE yesterday_zaiko;							/* �݌ɊǗ��ݒ� 0:�݌ɊǗ��Ȃ� 1:�S�X�݌ɊǗ�
																			 2:�ePOS�݌ɊǗ� 3:�{���݌ɊǗ� */
  BYTE change_receipt;							/* ���������V�[�g�������󎚁iTRUE:�L���j */
	BYTE pos_member_ca;								/* CA�ւ̉�������ʒm(TRUE:����AFALSE:�Ȃ�) */
	WORD pos_member_ca_retry;					/* CA�ւ̉�������ʒm���g���C�� */
	WORD pos_member_ca_timeout;				/* CA�ւ̉�������ʒm�^�C���A�E�g */
	BYTE horyu_card_flag;							/* �J�[�h�ɂ��ۗ�����������(TRUE:�L��) */
	BYTE return_card_flag;						/* �J�[�h�ɂ��ԕi��������(TRUE:�L��) */
	BYTE codein_card_flag;						/* �J�[�h�ɂ��R�[�h���͋���(TRUE:�L��) */
	BYTE all_nakajime_flag;						/* �S�i�i�����߈� 0:���g�p 1:�g�p */
	BYTE admin_operation;							/* �ӔC�ҊǗ��@�\�L���t���O(TRUE:�L��) */
	BYTE return_trans_flag;						/* �ԕi�A�߂������t���O */
	WORD com_code;										/* ��ЃR�[�h(0000�`9998) */
} pos_basic_t;


/*
 *	�����萔�ݒ�
 */
typedef struct {
	WORD tanka_a1;					/* A�X���1�P��(1��/�~) */
	WORD canend_a1;					/* A�X���1�I���\�� */
	WORD horyu_a1;					/* A�X���1�ۗ������s�\�� */
	WORD tanka_a2;					/* A�X���2�P��(1��/�~) */
	WORD canend_a2;					/* A�X���2�I���\�� */
	WORD horyu_a2;					/* A�X���2�ۗ������s�\�� */
	WORD tanka_a3;					/* A�X���3�P��(1��/�~) */
	WORD canend_a3;					/* A�X���3�I���\�� */
	WORD horyu_a3;					/* A�X���3�ۗ������s�\�� */
	WORD tanka_a4;					/* A�X���4�P��(1��/�~) */
	WORD canend_a4;					/* A�X���4�I���\�� */
	WORD horyu_a4;					/* A�X���4�ۗ������s�\�� */
	WORD tanka_b1;					/* B�X���1�P��(1��/�~) */
	WORD canend_b1;					/* B�X���1�I���\�� */
	WORD horyu_b1;					/* B�X���1�ۗ������s�\�� */
	WORD tanka_b2;					/* B�X���2�P��(1��/�~) */
	WORD canend_b2;					/* B�X���2�I���\�� */
	WORD horyu_b2;					/* B�X���2�ۗ������s�\�� */
	WORD tanka_b3;					/* B�X���3�P��(1��/�~) */
	WORD canend_b3;					/* B�X���3�I���\�� */
	WORD horyu_b3;					/* B�X���3�ۗ������s�\�� */
	WORD tanka_b4;					/* B�X���4�P��(1��/�~) */
	WORD canend_b4;					/* B�X���4�I���\�� */
	WORD horyu_b4;					/* B�X���4�ۗ������s�\�� */
	DWORD reserve1[3];			/* �\�� */
} pos_const_t;

/*
 *	�萔�g�ݍ��킹�ݒ�
 */
typedef struct {
	DWORD start_time;				/* �J�n����(�����`��) */
	DWORD end_time;					/* �I������(�����`��) */
	WORD pattern;					/* �p�^�[��(0�`CONST_MAX-1) */
	WORD reserve;					/* �\�� */
} pos_constset_t;

/*
 *	�v���@(�`�P�b�g�v�����^)�ݒ�
 */
typedef struct {
	BYTE no;						/* �v���@�̔ԍ�(0�`KEISUU_MAX-1) */
	BYTE counter;					/* �ݒu�J�E���^�ԍ�(0�`) */
	BYTE floor;						/* �ݒu�t���A�ԍ�(0�`) */
	BYTE tanka;						/* �����P�� */
	WORD shop;						/* �ݒu�X�ܔԍ�(0�`) */
	WORD extdev;					/* ���̑��A�O�t���@��̗L���ݒ� */
	BYTE waku_char;					/* �a���茔�̘g���� */
	BYTE waku_flag;					/* �a���茔�̘g�L��(TRUE�ŗL��) */
	BYTE name_flag;					/* �a���茔�̓X���L��(TRUE�ŗL��) */
	BYTE tokushu_flag;				/* �a���茔�̓���i�i�L��(TRUE�ŗL��) */
	WORD tokushu_num;				/* �a���茔�̓���i�i���Z�� */
	BYTE pas_flag;					/* �v���@��PAS�t���O(PAS_x�萔) */
	BYTE maker;						/* �v���@�̃��[�J�[(0�`) */
	BYTE type;						/* �v���@�̌^��(0�`) */
	BYTE pulse;						/* �v���@�̃p���X�� */
	BYTE genka;						/* �����R�[�h */
	BYTE online;					/* �I�����C���敪 TRUE:�I�����C�� */
	BYTE password[3];				/* �p�X���[�h(�G�[�X�d��AD�Ŏg�p) */
	BYTE reserve;					/* �\�� */
} tp_setting_t;

/*
 *	���o�@�ݒ�
 */
typedef struct {
	WORD no;						/* ���o�@�̔ԍ�(0�`HARAI_MAX-1) */
	BYTE online;					/* �I�����C���敪 TRUE:�I�����C�� */
	BYTE maker;						/* ���o�@�̃��[�J�[(0�`) */
	BYTE type;						/* ���o�@�̃^�C�v(0�`) */
	BYTE pulse;						/* ���o�@�̃p���X�� */
	WORD reserve2;					/* �\���Q */
} harai_setting_t;

/*
 *	POS�ݒ�
 */
typedef struct {
	BYTE no;						/* POS�̔ԍ�(0�`POS_MAX-1) */
	BYTE counter;					/* �ݒu�J�E���^�ԍ�(0�`) */
	BYTE floor;						/* �ݒu�t���A�ԍ�(0�`) */
	BYTE pos_syubetu;			/* POS�̎��(�S��ʑΉ�) */
	BYTE shop;						/* �ݒu�X�ܔԍ�(0�`) */
	BYTE saving_join;			/* ���ʓX�܍��Z 0:�Ȃ� 1:���� */
	BYTE point_access;		/* �|�C���g���Z 0:���Z�G���A 1:���X�|�C���g�G���A */
	BYTE point_join;			/* ���ʓX�܍��Z 0:�Ȃ� 1:���� */
	BYTE b_start_pos_no;	/* ���ʃG���A��B�X�ɂ���POS�̊J�n�ԍ�(0�`7) */
	BYTE physics_syubetu;		/* �e��ʂ̕������(0:P 1:S) */
	BYTE waku_char;					/* �ۗ����̘g���� */
	BYTE waku_flag;					/* �ۗ����̘g�L��(TRUE�ŗL��) */
	BYTE name_flag;					/* �ۗ����̓X���L��(TRUE�ŗL��) */
	BYTE tokushu_flag;				/* �ۗ����̓���i�i�L��(TRUE�ŗL��) */
	WORD tokushu_num;				/* �ۗ����̓���i�i���Z�� */
	WORD lane[PAS_MAX][LANE_MAX];	/* �O�������o���@���[���ݒ�(P,A,S�̏�) */
	BYTE mars_tokushu_num;			/* �}�[�X���o�@�i�i�� */
	BYTE tokushu_wari[3]; 			/* �}�[�X���o�@�i�i���� */
	BYTE mars1_tanka[3]; 			/* �}�[�X���o�@�i�i�P�ʒP�� */
	BYTE mars2_tanka[3];			/* �}�[�X���o�@�i�i�Q�ʒP�� */
	BYTE mars3_tanka[3];			/* �}�[�X���o�@�i�i�R�ʒP�� */
	BYTE mars4_tanka[3];		 	/* �}�[�X���o�@�i�i�S�ʒP�� */
	BYTE s_select_popup_str[4]; 	/* ��ʑI��POPUP������1�`4   2003.02.17 sugiura */
  BYTE glory_status_flag;		/* �O���[����ԃ`�F�b�N�t���O 0:�s�� 1:�s��Ȃ� */
	BYTE reserve2[3];				/* �\�� */
#if 0
	BYTE reserve5[12];				/* �\�� */
	BYTE reserve2[4];				/* �\�� */
	BYTE reserve3[4];				/* �\�� */
	BYTE reserve4[4];				/* �\�� */
#endif
} pos_setting_t;

/*
 *	�l�b�g���[�N�O���[�v�⏕
 */
typedef struct {
	BYTE arc_id;					/* ARCNET ID $00:���ڑ� */
	BYTE type;						/* �@���� $00:�e $01:�q $02:TP $03:CA $04:KS */
	BYTE no;						/* POS�܂���TP�̔ԍ�(0�`) */
	BYTE alive_flag;				/* �N����Ԃ�TRUE */
	BYTE open_flag;					/* �c�Ə�Ԃ�TRUE */
	BYTE change_flag;				/* �����\��Ԃ�TRUE */
	BYTE hakken_flag;				/* �����\��Ԃ�TRUE */
	BYTE clear_flag;				/* �N���A��Ԃ�TRUE */
	BYTE rec_flag;					/* �L����Ԃ�TRUE */
	BYTE htpsts_flag;				/* �t�@�C���]������TRUE */
	BYTE p_alive_flag;				/* �ePOS����̎w�����L�����邽�� */
	BYTE p_open_flag;				/* �ePOS����̎w�����L�����邽�� */
	BYTE p_change_flag;				/* �ePOS����̎w�����L�����邽�� */
	BYTE p_hakken_flag;				/* �ePOS����̎w�����L�����邽�� */
	BYTE p_clear_flag;				/* �ePOS����̎w�����L�����邽�� */
	BYTE p_rec_flag;				/* �ePOS����̎w�����L�����邽�� */
	BYTE p_htpsts_flag;				/* �ePOS����̎w�����L�����邽�� */
    BYTE err_status;				/* �G���[�X�e�[�^�X�L�� */
} pos_group_dat_t;

/*
 *	�l�b�g���[�N�O���[�v�Ǘ�
 */
typedef struct {
	pos_group_dat_t pos[POS_MAX];	/* �ePOS�ڍ׃X�e�[�^�X */
	pos_group_dat_t tp[TP_MAX];		/* �eTP�ڍ׃X�e�[�^�X */
  BYTE			tp_err[TP_MAX];	/*  */
	pos_group_dat_t ks;				/* KS�ڍ׃X�e�[�^�X */
 	pos_group_dat_t ca;				/* CA�ڍ׃X�e�[�^�X */
 	BYTE srif[SRIF_MAX];			/* SRIF�ڑ��L�� */
 	BYTE jihana[JIHANA_MAX];	/* ���̋@�ڑ��L�� */
 	BYTE vcd[V_CDS_MAX];			/* VCD�ڑ��L�� */
 	BYTE cds[CARDINQ_MAX];		/* CDS�ڑ��L�� */
 	BYTE jrl_dest_id;					/* JRL���M��ARC_ID */
	BYTE has_dest_id;					/* �����Ɖ�ADP ARC_ID */
	BYTE parent_pos_id;				/* �ePOS ARC_ID */
	BYTE reserve;							/* �\�� */
} pos_group_t;

/*
 *	���v�P�󎚑I��ݒ�
 */
typedef struct {
	WORD prn_code1;					/* �󎚏����R�[�h�P */
	WORD prn_code2;					/* �󎚏����R�[�h�Q */
	WORD prn_code3;					/* �󎚏����R�[�h�R */
	WORD prn_code4;					/* �󎚏����R�[�h�S */
	WORD prn_code5;					/* �󎚏����R�[�h�T */
	WORD reserved;					/* �\�� */
} pos_nikkei1_print_t;

/*
 *	���v�Q�󎚑I��ݒ�
 */
typedef struct {
	WORD prn_code1;					/* �󎚏����R�[�h�P */
	WORD prn_code2;					/* �󎚏����R�[�h�Q */
	WORD prn_code3;					/* �󎚏����R�[�h�R */
	WORD prn_code4;					/* �󎚏����R�[�h�S */
	WORD prn_code5;					/* �󎚏����R�[�h�T */
	WORD reserved;					/* �\�� */
} pos_nikkei2_print_t;

/*
 *	���v�R�󎚑I��ݒ�
 */
typedef struct {
	WORD prn_code1;					/* �󎚏����R�[�h�P */
	WORD prn_code2;					/* �󎚏����R�[�h�Q */
	WORD prn_code3;					/* �󎚏����R�[�h�R */
	WORD prn_code4;					/* �󎚏����R�[�h�S */
	WORD prn_code5;					/* �󎚏����R�[�h�T */
	WORD reserved;					/* �\�� */
} pos_nikkei3_print_t;

/*
 *	���v�S�󎚑I��ݒ�
 */
typedef struct {
	WORD prn_code1;					/* �󎚏����R�[�h�P */
	WORD prn_code2;					/* �󎚏����R�[�h�Q */
	WORD prn_code3;					/* �󎚏����R�[�h�R */
	WORD prn_code4;					/* �󎚏����R�[�h�S */
	WORD prn_code5;					/* �󎚏����R�[�h�T */
	WORD reserved;					/* �\�� */
} pos_nikkei4_print_t;

/*
 *	�ӔC�ҕʑ��쐧���ݒ�
 */
typedef struct {
	BYTE seigen_flag[64];			/* ���ꂼ��TRUE,FALSE�ő��쐧����݂��� */
} pos_seigen_t;

/*
 *	�ӔC�ғo�^�ݒ�(PMF_ADMIN �\����):28+2byte/1record
 */
typedef struct {
	BYTE 	gcode;					/* �O���[�v�R�[�h */
	BYTE 	reserve[3];				/* �\�� */
	DWORD	unyou_limit;			/* �^�p���������t���O */
	DWORD	change_limit;			/* �������������t���O */
	DWORD	zaiko_limit;			/* �݌ɏ��������t���O */
	DWORD	kaiin_limit;			/* ������������t���O */
	DWORD	config_limit;			/* �ݒ菈�������t���O */
	DWORD	reserve2;				/* �\��2 */
} pos_admin_t;

/*
 *	�L�[���i�ڑ��ݒ�
 */
typedef struct {
	BYTE name[EXTDEV_NAME_LEN];		/* �O���ڑ��@�햼(\0�Ȃ�) */
	BYTE type[3];					/* ���͎�� 'ARC' */
	BYTE id;						/* ARC ID */
} pos_extdev_t;

/*
 *	�ڑ��@��ݒ�
 */
typedef struct {
	BYTE pos[POS_MAX];				/* �ڑ��L��(TRUE�Őڑ�) */
	BYTE tp[TP_MAX];				/* �ڑ��L��(TRUE�Őڑ�) */
	BYTE cardinq[CARDINQ_MAX];		/* �ڑ��L��(TRUE�Őڑ�) */
	BYTE jihana[JIHANA_MAX];		/* �ڑ��L��(TRUE�Őڑ�) */
	BYTE vending_cds[V_CDS_MAX];	/* �ڑ��L��(TRUE�Őڑ�) */
	BYTE ca;						/* �ڑ��L��(TRUE�Őڑ�) */
	BYTE ks;						/* �ڑ��L��(TRUE�Őڑ�) */
  BYTE srif[SRIF_MAX];			/* �ڑ��L��(TRUE�Őڑ�) */
	BYTE resreve1[16];				/* �\��1 */
	BYTE group1_parent_pos_id;	/* �O���[�v1 �ePOS ID */
	BYTE group1_ca_id;	/* �O���[�v1 CA ID */
	BYTE group1_ks_id;	/* �O���[�v1 KS ID */
	BYTE group1_jrl_id;	/* �O���[�v1 JRL���M��J�nID */
	BYTE group1_tp_id;	/* �O���[�v1 TP(JCR)�J�nID */
	BYTE group1_srif_id;	/* �O���[�v1 SRIF�J�n ID */
	BYTE group1_vcd_id;	/* �O���[�v1 VCD�J�n ID */
	BYTE group1_hsa_id;	/* �O���[�v1 HSA�J�n ID */
	BYTE group1_jihana_id;	/* �O���[�v1 ���̋@�J�n ID */
	BYTE resreve2[2];		/* �\��2 */
	BYTE group2_parent_pos_id;	/* �O���[�v2 �ePOS ID */
	BYTE group2_ca_id;	/* �O���[�v2 CA ID */
	BYTE group2_ks_id;	/* �O���[�v2 KS ID */
	BYTE group2_jrl_id;	/* �O���[�v2 JRL���M��J�nID */
	BYTE group2_tp_id;	/* �O���[�v2 TP(JCR)�J�nID */
	BYTE group2_srif_id;	/* �O���[�v2 SRIF�J�n ID */
	BYTE group2_vcd_id;	/* �O���[�v2 VCD�J�n ID */
	BYTE group2_hsa_id;	/* �O���[�v2 HSA�J�n ID */
	BYTE group2_jihana_id;	/* �O���[�v2 ���̋@�J�n ID */
	BYTE resreve3[2];		/* �\��3 */
	BYTE resreve4[16];			/* �\��4 */
} pos_connect_t;

/*
 *
 *	�\���̒�`
 *	�}�X�^�t�@�C��
 *
 */

/*
 *	����}�X�^
 */
typedef struct {
	WORD nID;						/* ����R�[�h(1�`BUMON_MAX) */
	BYTE name[BUMON_NAME_LEN];		/* ����̖���(\0�Ȃ�) */
	BYTE reserve[2];
} pos_bumon_t;

/*
 *	���i�}�X�^
 */
typedef struct {
	BYTE barcode[BC_BIN_LEN];		/* �o�[�R�[�h(BIN�`��) */
	DWORD numcode;					/* ���[�U���i�R�[�h */
	WORD bumon;						/* �����镔��(1�`BUMON_MAX) */
	WORD nID;						/* �������i�R�[�h(1�`ITEM_MAX) */
	BYTE name[ITEM_NAME_LEN];		/* ���i��(\0�Ȃ�) */
	BYTE reserve1[2];				/* �\�� */
	WORD tax;						/* �ېŃ^�C�v(TAX_x�萔) */
	DWORD tax_rate;					/* ����ŗ�(0�`100) */
	DWORD a_1_price;				/* 1��ʌ����P��(A�X) */
	DWORD a_2_price;				/* 2��ʌ����P��(A�X) */
	DWORD a_3_price;				/* 3��ʌ����P��(A�X) */
	DWORD a_4_price;				/* 4��ʌ����P��(A�X) */
	DWORD last_date;				/* �Ō�Ɏg�p�������t(�����`��) */
	DWORD last_time;				/* �Ō�Ɏg�p��������(�����`��) */
	DWORD en_date;					/* �f�[�^��L���ɂ���N����(�����`��) */
	DWORD en_time;					/* �f�[�^��L���ɂ��鎞��(�����`��) */
	DWORD b_1_price;				/* 1��ʌ����P��(B�X) */
	DWORD b_2_price;				/* 2��ʌ����P��(B�X) */
	DWORD b_3_price;				/* 3��ʌ����P��(B�X) */
	DWORD b_4_price;				/* 4��ʌ����P��(B�X) */
} pos_item_t;

/*
 *	�݌Ƀ}�X�^
 */
typedef struct {
	DWORD numcode;					/* ���[�U���i�R�[�h */
	long zaiko;							/* �݌ɐ� */
	long import;						/* ���ɐ� */
	long export_4;					/* ���4�o�ɐ� */
	long chosei;						/* ������ */
	long tana;							/* �I���� */
	long export_1;					/* ���1�o�ɐ� */
	long export_2;					/* ���2�o�ɐ� */
	long export_3;					/* ���3�o�ɐ� */
	WORD nID;								/* �������i�R�[�h(1�`ITEM_MAX) */
	short gosa;							/* �덷�� */
	long zaiko_all;					/* �S�X�݌ɐ�(�ePOS�̂�) */
	short move;							/* �ړ��� */
	BYTE syori_flag;				/* �����t���O(���ʃr�b�g������ɁE�I���E�����E�ړ��E�W�v) */
	BYTE reserve1;					/* �\���P */
} pos_zaiko_t;


/*
 *	�O���@�폤�i�ݒ�
 */
typedef struct {
	BYTE barcode[BC_BIN_LEN];		/* �o�[�R�[�h(BIN�`��) */
	BYTE search_mode;				/* ���i�R�[�h�������[�h */
	BYTE dumy1;						/* �\���P */
	BYTE dumy2[2];					/* �\���Q */
} pos_extitem_t;

/*
*  �ԕi�A�߂���������
*/
typedef struct {
	DWORD nID;							/* ���i�R�[�h�A�V���A���ԍ� */
	DWORD num;							/* �� */
} back_rireki_t;


/*
 *	�w�蒆���ߐݒ�
 */
typedef struct {
	WORD nID;						/* �������i�R�[�h(1�`ITEM_MAX) */
	BYTE dmy1[2];					/* �\�� */
} T_CLOSE_PRINT;

/*
 *
 */
typedef struct tagDmyPmf {
	BYTE dmy[2];
} T_DMY_PMF;

/*
 *	���̋@�A�_�v�^(�x�m��C��)����f�[�^
 */
typedef struct {
	DWORD rece_date;				/* ��M���t(���g�p��0) */
	DWORD rece_time;				/* ��M���� */
	DWORD code;						/* ���i�R�[�h */
	BYTE name[16];					/* ���i��(�X�y�[�X�l�߁A\0�Ȃ�) */
	BYTE reserve1[4];				/* �\�� */
	DWORD cash_price;				/* �������i */
	DWORD p_price;					/* �ʒP�� */
	DWORD s_price;					/* ���_���P�� */
	DWORD p_item_price;				/* �i�i�ʒP�� */
	DWORD s_item_price;				/* �i�i���_���P�� */
	DWORD p_member_price;			/* ����ʒP�� */
	DWORD s_member_price;			/* ������_���P�� */
	DWORD cash_out;					/* �������� */
	DWORD p_out;					/* �ʔ��� */
	DWORD s_out;					/* ���_������ */
	DWORD p_item_out;				/* �i�i�ʔ��� */
	DWORD s_item_out;				/* �i�i���_������ */
	DWORD p_member_out;				/* ����ʔ��� */
	DWORD s_member_out;				/* ������_������ */
	BYTE reserve2[8];				/* �\�� */
} pos_vending_t;

/*
 *	�L�[�A�C�e���ݒ�
 */
typedef struct {
	BYTE barcode[BC_BIN_LEN];		/* �o�[�R�[�h(BIN�`��) */
} pos_keyitem_t;

/*
 *
 *	�\���̒�`
 *	�����֘A
 *
 */

/*---[ �W���[�i���\���̐ݒ� ]------------------------------------------------*/
/*
 *	�����������ʁi�K�v�Ȃ炱��ɃL���X�g�j
 */
typedef struct {
	BYTE dummy1[16];				/* �����f�[�^ */
	BYTE dummy2[14];				/* 30�o�C�g�A���C�������g */
} pos_j_common_t;

/*
 *	���������w�b�_
 */
typedef struct {
	WORD 	type1;					/* �����敪�P(GRIREKI_x�萔) */
	WORD 	serial;					/* ����V���A�� */
	DWORD 	date;					/* ���t(�����`��) */
	DWORD 	time;					/* ����(�����`��) */
	WORD 	pos_no;					/* POS�̔ԍ�(0�`POS_MAX-1) */
	BYTE 	number;					/* �ȍ~�̗�����(0�`) */
	BYTE 	admin;					/* �ӔC�Ҕԍ�(0�`ADMIN_MAX-1) */
	BYTE 	error;					/* 0x00:�G���[�Ȃ��A0x01:�G���[���� */
	BYTE 	dummy;					/* �\�� */
	WORD	regi_sheet;				/* ���V�[�g�ԍ�(�����������ȊO��0���i�[) */
	BYTE	dummy2[8];				/* �\���Q */
	WORD	pmf_header;				/* PMF�w�b�_(0xEEFF) */
} pos_j_header_t;

/*
 *	���������E��ʌi�i
 */
typedef struct {
	WORD	type2;					/* �����敪�Q(GRIREKI_IPPAN) */
	WORD 	pas_flag;				/* PAS�t���O(PAS_x�萔) */
	DWORD 	nID;					/* ���i�R�[�h(1�`ITEM_MAX) */
	WORD 	num;					/* �i�i����(1�`) */
	BYTE 	reserve[2];				/* �\�� */
	DWORD 	price;					/* �i�i�ʐ�(�P���~����) */
} pos_j_ippan_t;

/*
 *	���������E����i�i
 */
typedef struct {
	WORD type2;						/* �����敪�Q(GRIREKI_TOKUSHU) */
	WORD num1;						/* ����i�i���� �� */
	WORD num2;						/* ����i�i�� �� */
	WORD num3;						/* ����i�i�� �� */
	WORD num4;						/* ����i�i�� �� */
	WORD pas_flag;					/* PAS�t���O(PAS_x�萔) */
	DWORD price;					/* �i�i�ʐ�(�P���~���ʂ̂S���v) */
} pos_j_tokushu_t;

/*
 *	���������E���ʎw��
 */
typedef struct {
	WORD type2;						/* �����敪�Q(GRIREKI_SAVINGS) */
	WORD pas_flag;					/* PAS�t���O(PAS_x�萔) */
	DWORD code;						/* ����ԍ� */
	DWORD num;						/* ���ʋʐ�(1�`) */
	DWORD arng_num;					/* (POS3��ʑΉ� 2002/01/30 DDS.Shimizu) */
} pos_j_savings_t;

/*
 *	���������E���ʑJ��
 */
typedef struct {
	WORD		type2;				/* �����敪�Q(GRIREKI_SAVINGSDAT) */
	WORD		pas_flag;			/* PAS�t���O */
	DWORD		num;				/* ���ۂ�KS�ɗa����A�����o�����ʐ� */
	DWORD		chot1;				/* �J�[�h�C�����̒��ʐ� */
	DWORD		chot2;				/* ������̒��ʐ� */
	BYTE		type;				/* 0x01:�a����A0x02:���o�� */
	BYTE		dmy[13];			/* �\�� */
} pos_j_savingsdat_t;

/*
 *	���������E���������G���[
 */
typedef struct {
	WORD		type2;				/* �����敪�Q(GRIREKI_CHGERR) */
	WORD		type;				/* �G���[�敪 */
	WORD		code;				/* �G���[�R�[�h */
	BYTE		dat1;				/* �f�[�^�P */
	BYTE		dat2;				/* �f�[�^�Q */
	BYTE		dat3;				/* �f�[�^�R */
	BYTE		dat4;				/* �f�[�^�S */
	WORD		dat5;				/* �f�[�^�T */
	DWORD		dat6;				/* �f�[�^�U */
	DWORD		dat7;				/* �f�[�^�V */
	DWORD		dat8;				/* �f�[�^�W */
	DWORD		dat9;				/* �f�[�^�X */
	WORD		dat10;				/* �f�[�^�P�O */
} pos_j_chgerr_t;

/*
 *	���������E�a����
 */
typedef struct {
	WORD type2;						/* �����敪�Q(GRIREKI_DEPOSIT) */
	WORD dep_type;					/* �a����敪(GRIREKI_x�萔) */
	DWORD nID;						/* ����ԍ�,�ۗ����ԍ� */
	DWORD num;						/* �i�i�ʐ� */
	WORD pas_flag;					/* PAS�t���O(PAS_x�萔) */
	WORD machine_type;				/* �@��ԍ� */
	WORD mashine_id;				/* ���@�ԍ� */
	DWORD slot_org;					/* �����_�������O��(POS3��ʑΉ� 2002/01/30 DDS.Shimizu) */
	WORD reserve[10];				/* �\�� */
} pos_j_deposit_t;

/*
 *	���������E�]���
 */
typedef struct {
	WORD type2;						/* �����敪�Q(GRIREKI_REST) */
	WORD rest_type;					/* �]��ʏ����敪 */
	DWORD nID;						/* ����ԍ�,�ۗ����ԍ� */
	DWORD num;						/* �i�i�ʐ� */
	DWORD total;					/* ���ʐ��i�X�V��j*/
	WORD  pas_flag;					/* PAS�t���O */
	BYTE reserve[12];				/* �\�� */
} pos_j_rest_t;

/*
 *	���������E��ʌi�i�ԕi�^�߂�
 */
typedef struct {
	WORD type2;						/* �����敪�Q(GIREKI_MIPPAN) */
	WORD pas_flag;					/* PAS�t���O(PAS_x�萔) */
	DWORD nID;						/* ���i�R�[�h */
	WORD num;						/* ���i���� */
	WORD reserve;					/* �\�� */
	DWORD price;					/* �ʐ� */
	WORD dep_type;					/* �a����敪(GRIREKI_x�萔) */
} pos_j_mippan_t;

/*
 *	���������E����i�i�ԕi�^�߂�
 */
typedef struct {
	WORD type2;						/* �����敪�Q(GIREKI_MTOKUSHU) */
	WORD num1;						/* ����i�i���� �� */
	WORD num2;						/* ����i�i�� �� */
	WORD num3;						/* ����i�i�� �� */
	WORD num4;						/* ����i�i�� �� */
	WORD pas_flag;					/* PAS�t���O(PAS_x�萔) */
	DWORD price;					/* �i�i�ʐ�(�P���~���ʂ̂S���v) */
	WORD dep_type;					/* �a����敪(GRIREKI_x�萔) */
} pos_j_mtokushu_t;

/*
 *	�ۗ�������
 */
typedef struct {
	DWORD date;						/* ���s���t(�����`��) */
	DWORD time;						/* ���s����(�����`��) */
	BYTE pas_flag;					/* PAS�t���O(PAS_x�萔) */
	BYTE admin;						/* �ӔC�Ҕԍ�(0�`ADMIN_MAX-1) */
	WORD nID;						/* �ۗ����ʂ��ԍ�(1�`) */
	DWORD num;						/* �ۗ��ʐ� */
	BYTE re_issue;					/* �Ĕ��s��(0�`9) */
	BYTE flag;						/* �����ς݃t���O(>=0x80�ŏ�����) */
} pos_j_horyu_t;

/*
 *	�[���G���[����
 */
typedef struct {
	DWORD date;						/* �������t(�����`��) */
	DWORD time;						/* ��������(�����`��) */
	WORD pos_no;					/* POS�ԍ� */
	WORD error;						/* �G���[�R�[�h */
	BYTE machine_type;				/* �@��ԍ� */
	BYTE machine_id;				/* ���@�ԍ� */
	WORD ticket_no;					/* �`�P�b�g�V���A���ԍ� */
	BYTE reserve3[14];				/* �\���R */
} pos_j_error_t;

/*
 *	���������E�^�p����
 */
typedef struct {
	WORD 	type2;					/* �����敪�Q */
	BYTE 	reserve[28];			/* �\�� */
} pos_j_unyou_t;

/*
 *	���������E�^�p�G���[����
 */
typedef struct {
	WORD	type2;					/* �����敪�Q(GRIREKI_UNYERR) */
	WORD	code;					/* �G���[�R�[�h */
	DWORD	tp;						/* TP1�`TP32(bit0�`bit31) */
	BYTE	pos;					/* POS1�`POS8(bit0�`bit7) */
	BYTE	cds;					/* CDS1�`CDS8(bit0�`bit7) */
	BYTE	reserve[20];			/* �\�� */
} pos_j_unyerr_t;

/*
 *	���������E�݌ɊǗ�����
 */
typedef struct {
	WORD 	type2;					/* �����敪�Q */
	WORD	reserve;				/* �\���P */
	DWORD	nID;					/* �i�i�R�[�h */
	long	num;					/* �����i�i�� */
	long	zaiko1;					/* ���_�݌ɐ�(�����O) */
	long	zaiko2;					/* ���_�݌ɐ�(������) */
	short	gosa_all;				/* �덷��(�S�X) */
	short	gosa;					/* �덷�� */
	BYTE	pos_no;					/* �ړ���POS�ԍ�(�ړ��̂�) */
	BYTE 	reserve2[5];			/* �\�� */
} pos_j_zaiko_t;

/*
 *	���������E�݌ɊǗ��G���[����
 */
typedef struct {
	WORD	type2;					/* �����敪�Q(GRIREKI_ZAIKERR) */
	WORD	code;					/* �G���[�R�[�h */
	BYTE	pos_no;					/* POS1�`POS8(bit0�`bit7) */
	BYTE	reserve[25];			/* �\�� */
} pos_j_zaikoerr_t;

/*
 *	���������E����[������
 */
typedef struct {
	WORD 	type2;					/* �����敪�Q */
	BYTE	reserve;				/* �\�� */
	BYTE	chg_type;				/* ��ԕύX��� */
	DWORD	code1;					/* ����ԍ�1 */
	DWORD	code2;					/* ����ԍ�2 */
	DWORD	chot1;					/* �ڍs�O���ʐ� */
	DWORD	chom1;					/* �ڍs�O�����_���� */
	DWORD	chot2;					/* �ڍs�㒙�ʐ� */
	DWORD	chom2;					/* �ڍs�㒙���_���� */
	WORD 	password;				/* �N���A�O�Ïؔԍ� */
} pos_j_kaiin_t;

/*
 *	���������E����[���G���[����
 */
typedef struct {
	WORD	type2;					/* �����敪�Q(GRIRKEI_KERR) */
	WORD	ecode;					/* �G���[�R�[�h */
	DWORD	kcode1;					/* �����G���[����ԍ��P */
	DWORD	kcode2;					/* �����G���[����ԍ��Q */
	BYTE	reserve[18];			/* �\�� */
} pos_j_kaiinerr_t;

/*
 *	���������E�����萔�ݒ菈��(1)
 */
typedef struct {
	WORD 	type2;					/* �����敪2(GRIREKI_EXCHG) */
	WORD	p_tanka;				/* �o�P�� */
	WORD	p_servece;				/* �o�T�[�r�X�� */
	WORD	p_canservice;			/* �o�T�[�r�X�L���� */
	WORD	p_canend;				/* �o�I���\�ʐ� */
	WORD	p_horyu;				/* �o�ۗ������s�ʐ� */
	WORD	p_tesuu;				/* �o�萔���ʐ�(����) */
	WORD	p_limit;				/* �o�����l(����) */
	WORD	p_limitout;				/* �o�����O�萔���ʐ� */
	WORD	a_tanka;				/* �`�P�� */
	WORD	a_service;				/* �`�T�[�r�X�� */
	WORD	a_cansevice;			/* �`�T�[�r�X�L���� */
	WORD	a_canend;				/* �`�I���\�ʐ� */
	WORD	a_horyu;				/* �`�ۗ������s�ʐ� */
	WORD	a_tesuu;				/* �`�萔���ʐ�(����) */
} pos_j_chgcfg1_t;

/*
 *	�����萔�ݒ菈������(2)
 */
typedef struct {
	WORD 	type2;					/* �����敪2(GRIREKI_EXCHG) */
	WORD	a_limit;				/* �`�����l(����) */
	WORD	a_limitout;				/* �`�����O�萔���ʐ� */
	WORD	s_tanka;				/* �r�P�� */
	WORD	s_service;				/* �r�T�[�r�X�� */
	WORD	s_cansevice;			/* �r�T�[�r�X�L���� */
	WORD	s_canend;				/* �r�I���\�ʐ� */
	WORD	s_horyu;				/* �r�ۗ������s�ʐ� */
	WORD	s_tesuu;				/* �r�萔���ʐ�(����) */
	WORD	s_limit;				/* �r�����l(����) */
	WORD	s_limitout;				/* �r�����O�萔���ʐ� */
	BYTE	reserve[8];				/* �\�� */
} pos_j_chgcfg2_t;

/*
 *	���������E�����ݒ菈��
 */
typedef struct {
	WORD	type2;					/* �����敪2(GRIREKI_TIMEDEF) */
	BYTE	reserve[2];				/* �\�� */
	DWORD	p_date;					/* �ݒ�O���� */
	DWORD	p_time;					/* �ݒ�O���� */
	DWORD	date;					/* �ݒ���� */
	DWORD	time;					/* �ݒ莞�� */
	BYTE	reserve2[10];			/* �\���Q */
} pos_j_timedef_t;

/*
 *	���������E���[I/F�ݒ菈���P
 */
typedef struct {
	WORD	type2;					/* �����敪2(GRIREKI_SRIF) */
	WORD	srif_no;				/* �ݒ�SRI/F�ԍ�(0�`SRIF_MAX-1) */
	BYTE	reserve2[2];			/* �\���Q */
	BYTE	syu;					/* ���(0x00:�ʁA0x01;���_��) */
	BYTE	pt;						/* �|�C���g�\��(0x00:�Ȃ��A0x01:����) */
	DWORD	max_tama;				/* ���o����l(��) */
	DWORD	max_med;				/* ���o����l(���_��) */
	WORD	b1_tesu_t;				/* �{�^��1�萔��(��) */
	WORD	b1_harai_t;				/* �{�^��1���o��(��) */
	WORD	b2_tesu_t;				/* �{�^��2�萔��(��) */
	WORD	b2_harai_t;				/* �{�^��2���o��(��) */
	WORD	b3_tesu_t;				/* �{�^��3�萔��(��) */
	WORD	b3_harai_t;				/* �{�^��3���o��(��) */
	WORD	b4_tesu_t;				/* �{�^��4�萔��(��) */
} pos_j_srif_t;

/*
 *	���������E���[I/F�ݒ菈���Q
 */
typedef struct {
	WORD	type2;					/* �����敪2(GRIREKI_SRIF) */
	BYTE	reserve[2];				/* �\�� */
	BYTE	reserve2[2];			/* �\���Q */
	WORD	b4_harai_t;				/* �{�^��4���o��(��) */
	WORD	b5_tesu_t;				/* �{�^��5�萔��(��) */
	WORD	b5_harai_t;				/* �{�^��5���o��(��) */
	WORD	b6_tesu_t;				/* �{�^��6�萔��(��) */
	WORD	b6_harai_t;				/* �{�^��6���o��(��) */
	WORD	b1_tesu_m;				/* �{�^��1�萔��(���_��) */
	WORD	b1_harai_m;				/* �{�^��1���o��(���_��) */
	WORD	b2_tesu_m;				/* �{�^��2�萔��(���_��) */
	WORD	b2_harai_m;				/* �{�^��2���o��(���_��) */
	WORD	b3_tesu_m;				/* �{�^��3�萔��(���_��) */
	WORD	b3_harai_m;				/* �{�^��3���o��(���_��) */
	WORD	b4_tesu_m;				/* �{�^��4�萔��(���_��) */
} pos_j_srif2_t;

/*
 *	���������E���[I/F�ݒ菈���R
 */
typedef struct {
	WORD	type2;					/* �����敪2(GRIREKI_SRIF) */
	BYTE	reserve[2];				/* �\�� */
	BYTE	reserve2[2];			/* �\���Q */
	WORD	b4_harai_m;				/* �{�^��4���o��(���_��) */
	WORD	b5_tesu_m;				/* �{�^��5�萔��(���_��) */
	WORD	b5_harai_m;				/* �{�^��5���o��(���_��) */
	WORD	b6_tesu_m;				/* �{�^��6�萔��(���_��) */
	WORD	b6_harai_m;				/* �{�^��6���o��(���_��) */
	BYTE	reserve3[14];			/* �\���R */
} pos_j_srif3_t;

/*
 *	���������E�ݒ�G���[����
 */
typedef struct {
	WORD	type2;					/* �����敪�Q(GRIREKI_CFGERR) */
	WORD	code;					/* �G���[�R�[�h */
	DWORD	tp;						/* TP1�`TP32(bit0�`bit31) */
	BYTE	pos;					/* POS1�`POS8(bit0�`bit7) */
	BYTE	cds;					/* CDS1�`CDS8(bit0�`bit7) */
	BYTE	srif;					/* SRIF1�`SRIF(bit0�`bit7) */
	BYTE	reserve[19];			/* �\�� */
} pos_j_configerr_t;

/*
 *	���������E�J�[�h����
 */
typedef struct {
	WORD	type2;					/* �����敪�Q(0x0000) */
	WORD	reserve;				/* �\�� */
	DWORD	card_no;				/* �J�[�h�ԍ� */
	BYTE	type;					/* �����^�C�v(�J�[�h�C���^�A�E�g) */
	BYTE	reserve2[21];			/* �\��2 */
} pos_j_card_t;

/*
 *	���������E�J�[�h�G���[����
 */
typedef struct {
	WORD	type2;					/* �����敪�Q(0x0000) */
	WORD	code;					/* �G���[�R�[�h */
	BYTE	reserve[26];			/* �\�� */
} pos_j_carderr_t;

/*
 *	�G���[����
 */
typedef struct {
	WORD	type1;					/* �����敪1(GRIREKI_ERROR) */
	WORD	type2;					/* �����敪2 */
	BYTE	reserve[2];				/* �\�� */
	WORD	error;					/* �G���[��� */
	BYTE	harai_type;				/* ���o�@���(���o�@�G���[�̂�) */
	BYTE	reserve2[21];
} pos_j_err_t;


/*---[ �\���̐ݒ�(���v�t�@�C��) ]--------------------------------------------*/
/*
 *	�ӔC�ҕʃR�[�h�t�@�C��
 */
typedef struct {
	WORD code;						/* �ݒ�R�[�h */
} pos_admin_code_t;

/*
 *	���p�t�@�C��
 *		�EPOS���v
 *		�EPOS�ӔC�ҕʓ��v
 *		�E�ePOS�W�v����
 */
#if 0		// pos_toukei_t�@��z��^�ɕύX���܂����B�@���J
typedef struct  {
	DWORD	guest;						/* �q�� */
	DWORD	n_change_1;				/* ���1������ */
	DWORD	n_change_2;				/* ���2������ */
	DWORD	n_change_3;				/* ���3������ */
	DWORD	n_change_4;				/* ���4������ */
	DWORD	change_1;					/* ���1�����ʐ� */
	DWORD	change_2;					/* ���2�����ʐ� */
	DWORD	change_3;					/* ���3�����ʐ� */
	DWORD	change_4;					/* ���4�����ʐ� */

	long	tokushu_1[TOKUSHU_MAX];	/* ���1����i�i�� */
	long	tokushu_2[TOKUSHU_MAX];	/* ���2����i�i�� */
	long	tokushu_3[TOKUSHU_MAX];	/* ���3����i�i�� */
	long	tokushu_4[TOKUSHU_MAX];	/* ���4����i�i�� */

	DWORD	mharai_true_1[4];			/* ���1�}�[�X�A�q���^�b�N�����o�@ �i�i1.2.3.4 */
	DWORD	mharai_true_2[4];			/* ���2�}�[�X�A�q���^�b�N�����o�@ �i�i1.2.3.4 */
	DWORD	mharai_true_3[4];			/* ���3�}�[�X�A�q���^�b�N�����o�@ �i�i1.2.3.4 */
	DWORD	mharai_true_4[4];			/* ���4�}�[�X�A�q���^�b�N�����o�@ �i�i1.2.3.4 */
	DWORD	mharai_false_1[4];		/* ���1�}�[�X�����o�@ ���������i�i1.2.3.4 */
	DWORD	mharai_false_2[4];		/* ���2�}�[�X�����o�@ ���������i�i1.2.3.4 */
	DWORD	mharai_false_3[4];		/* ���3�}�[�X�����o�@ ���������i�i1.2.3.4 */
	DWORD	mharai_false_4[4];		/* ���4�}�[�X�����o�@ ���������i�i1.2.3.4 */

	DWORD	n_return_1;				/* ���1�߂��ʏ����� */
	DWORD	n_return_2;				/* ���2�߂��ʏ����� */
	DWORD	n_return_3;				/* ���3�߂��ʏ����� */
	DWORD	n_return_4;				/* ���4�߂��ʏ����� */
	DWORD	return_1;					/* ���1�߂��ʋʐ� */
	DWORD	return_2;					/* ���2�߂��ʋʐ� */
	DWORD	return_3;					/* ���3�߂��ʋʐ� */
	DWORD	return_4;					/* ���4�߂��ʋʐ� */

	DWORD	n_pickup_1;				/* ���1�E���ʏ����� */
	DWORD	n_pickup_2;				/* ���2�E���ʏ����� */
	DWORD	n_pickup_3;				/* ���3�E���ʏ����� */
	DWORD	n_pickup_4;				/* ���4�E���ʏ����� */
	DWORD	pickup_1;					/* ���1�E���ʋʐ� */
	DWORD	pickup_2;					/* ���2�E���ʋʐ� */
	DWORD	pickup_3;					/* ���3�E���ʋʐ� */
	DWORD	pickup_4;					/* ���4�E���ʋʐ� */

	DWORD	g_withdraw_1;			/* ���1���ʕ��o�q�� */
	DWORD	g_withdraw_2;			/* ���2���ʕ��o�q�� */
	DWORD	g_withdraw_3;			/* ���3���ʕ��o�q�� */
	DWORD	g_withdraw_4;			/* ���4���ʕ��o�q�� */
	DWORD	withdraw_1;				/* ���1���ʕ��o�ʐ� */
	DWORD	withdraw_2;				/* ���2���ʕ��o�ʐ� */
	DWORD	withdraw_3;				/* ���3���ʕ��o�ʐ� */
	DWORD	withdraw_4;				/* ���4���ʕ��o�ʐ� */
	DWORD	g_deposit_1;			/* ���1���ʗa���q�� */
	DWORD	g_deposit_2;			/* ���2���ʗa���q�� */
	DWORD	g_deposit_3;			/* ���3���ʗa���q�� */
	DWORD	g_deposit_4;			/* ���4���ʗa���q�� */
	DWORD	deposit_1;				/* ���1���ʗa���ʐ� */
	DWORD	deposit_2;				/* ���2���ʗa���ʐ� */
	DWORD	deposit_3;				/* ���3���ʗa���ʐ� */
	DWORD	deposit_4;				/* ���4���ʗa���ʐ� */

	DWORD	i_ha_1;					/* ���1�[�ʌi�i�� */
	DWORD	i_ha_2;					/* ���2�[�ʌi�i�� */
	DWORD	i_ha_3;					/* ���3�[�ʌi�i�� */
	DWORD	i_ha_4;					/* ���4�[�ʌi�i�� */
	DWORD	ha_1;						/* ���1�[�ʋʐ� */
	DWORD	ha_2;						/* ���2�[�ʋʐ� */
	DWORD	ha_3;						/* ���3�[�ʋʐ� */
	DWORD	ha_4;						/* ���4�[�ʋʐ� */

	DWORD	n_over_1;				/* ���1�]��ʏ����� */
	DWORD	n_over_2;				/* ���2�]��ʏ����� */
	DWORD	n_over_3;				/* ���3�]��ʏ����� */
	DWORD	n_over_4;				/* ���4�]��ʏ����� */
	DWORD	over_1;					/* ���1�]��ʋʐ� */
	DWORD	over_2;					/* ���2�]��ʋʐ� */
	DWORD	over_3;					/* ���3�]��ʋʐ� */
	DWORD	over_4;					/* ���4�]��ʋʐ� */
	DWORD	back_ha;				/* �[�ʌi�i�߂��� */

	DWORD	n_get_1;					/* ���1��荞�ݏ����� */
	DWORD	n_get_2;					/* ���2��荞�ݏ����� */
	DWORD	n_get_3;					/* ���3��荞�ݏ����� */
	DWORD	n_get_4;					/* ���4��荞�ݏ����� */
	DWORD	get_1;						/* ���1��荞�݋ʐ� */
	DWORD	get_2;						/* ���2��荞�݋ʐ� */
	DWORD	get_3;						/* ���3��荞�݋ʐ� */
	DWORD	get_4;						/* ���4��荞�݋ʐ� */

	DWORD	n_service_1;				/* ���1�T�[�r�X������ */
	DWORD	n_service_2;				/* ���2�T�[�r�X������ */
	DWORD	n_service_3;				/* ���3�T�[�r�X������ */
	DWORD	n_service_4;				/* ���4�T�[�r�X������ */
	DWORD	service_1;					/* ���1�T�[�r�X�ʐ� */
	DWORD	service_2;					/* ���2�T�[�r�X�ʐ� */
	DWORD	service_3;					/* ���3�T�[�r�X�ʐ� */
	DWORD	service_4;					/* ���4�T�[�r�X�ʐ� */

	WORD	n_tenyu_1;				/* ���1����͉� */
	WORD	c_tenyu_1;				/* ���1����͌����� */
	WORD	n_tenyu_2;				/* ���2����͉� */
	WORD	c_tenyu_2;				/* ���2����͌����� */
	WORD	n_tenyu_3;				/* ���3����͉� */
	WORD	c_tenyu_3;				/* ���3����͌����� */
	WORD	n_tenyu_4;				/* ���4����͉� */
	WORD	c_tenyu_4;				/* ���4����͌����� */
	DWORD	tenyu_1;					/* ���1����͋ʐ� */
	DWORD	tenyu_2;					/* ���2����͋ʐ� */
	DWORD	tenyu_3;					/* ���3����͋ʐ� */
	DWORD	tenyu_4;					/* ���4����͋ʐ� */

	WORD	n_henpin_1;				/* ���1�ԕi�� */
	WORD	c_henpin_1;				/* ���1�ԕi������ */
	WORD	n_henpin_2;				/* ���2�ԕi�� */
	WORD	c_henpin_2;				/* ���2�ԕi������ */
	WORD	n_henpin_3;				/* ���3�ԕi�� */
	WORD	c_henpin_3;				/* ���3�ԕi������ */
	WORD	n_henpin_4;				/* ���4�ԕi�� */
	WORD	c_henpin_4;				/* ���4�ԕi������ */
	DWORD	henpin_1;					/* ���1�ԕi�ʐ� */
	DWORD	henpin_2;					/* ���2�ԕi�ʐ� */
	DWORD	henpin_3;					/* ���3�ԕi�ʐ� */
	DWORD	henpin_4;					/* ���4�ԕi�ʐ� */

	DWORD	n_horyu_1;				/* ���1�ۗ������s���� */
	DWORD	n_horyu_2;				/* ���2�ۗ������s���� */
	DWORD	n_horyu_3;				/* ���3�ۗ������s���� */
	DWORD	n_horyu_4;				/* ���4�ۗ������s���� */
	DWORD	horyu_1;					/* ���1�ۗ������s�ʐ� */
	DWORD	horyu_2;					/* ���2�ۗ������s�ʐ� */
	DWORD	horyu_3;					/* ���3�ۗ������s�ʐ� */
	DWORD	horyu_4;					/* ���4�ۗ������s�ʐ� */
	
	DWORD	n_horyue_1;				/* ���1�ۗ����ǂݎ�萬������ */
	DWORD	n_horyue_2;				/* ���2�ۗ����ǂݎ�萬������ */
	DWORD	n_horyue_3;				/* ���3�ۗ����ǂݎ�萬������ */
	DWORD	n_horyue_4;				/* ���4�ۗ����ǂݎ�萬������ */
	DWORD	horyue_1;					/* ���1�ۗ����ǂݎ�萬���ʐ� */
	DWORD	horyue_2;					/* ���2�ۗ����ǂݎ�萬���ʐ� */
	DWORD	horyue_3;					/* ���3�ۗ����ǂݎ�萬���ʐ� */
	DWORD	horyue_4;					/* ���4�ۗ����ǂݎ�萬���ʐ� */

	DWORD	n_hold_1;				/* ���1�a���茔�ǂݎ�茏�� */
	DWORD	n_hold_2;				/* ���2�a���茔�ǂݎ�茏�� */
	DWORD	n_hold_3;				/* ���3�a���茔�ǂݎ�茏�� */
	DWORD	n_hold_4;				/* ���4�a���茔�ǂݎ�茏�� */
	DWORD	hold_1;					/* ���1�a���茔�ǂݎ��ʐ� */
	DWORD	hold_2;					/* ���2�a���茔�ǂݎ��ʐ� */
	DWORD	hold_3;					/* ���3�a���茔�ǂݎ��ʐ� */
	DWORD	hold_4;					/* ���4�a���茔�ǂݎ��ʐ� */

	DWORD	n_holde_1;				/* ���1�a���茔�����ό��� */
	DWORD	n_holde_2;				/* ���2�a���茔�����ό��� */
	DWORD	n_holde_3;				/* ���3�a���茔�����ό��� */
	DWORD	n_holde_4;				/* ���4�a���茔�����ό��� */
	DWORD	holde_1;					/* ���1�a���茔�����ϋʐ� */
	DWORD	holde_2;					/* ���2�a���茔�����ϋʐ� */
	DWORD	holde_3;					/* ���3�a���茔�����ϋʐ� */
	DWORD	holde_4;					/* ���4�a���茔�����ϋʐ� */

	DWORD	clear_num;				/* �N���A�� */
	DWORD	power_num;				/* �d�������� */
	DWORD	admchg_num;				/* �ӔC�ғ���ւ��� */
	DWORD	chosei_num;				/* ������ */
	DWORD	reserve5;				/* �\�� */
	DWORD	reserve6;				/* �\�� */
	DWORD	reserve7;				/* �\�� */
	DWORD	reserve8;				/* �\�� */
	DWORD	reserve9;				/* �\�� */
} pos_toukei_t;
#endif

typedef struct  {
	DWORD	guest;						/* �q�� */
	DWORD	n_change[4];				/* ������ [���(�P�`�S)]*/
	DWORD	change[4];					/* �����ʐ� [���(�P�`�S)]*/

	long	tokushu[4][TOKUSHU_MAX];	/* ����i�i�� [���(�P�`�S)][TOKUSHU_MAX]*/

	DWORD	mharai_true[4][4];			/* �}�[�X�A�q���^�b�N�����o�@ [���(�P�`�S)][�i�i1.2.3.4] */
	DWORD	mharai_false[4][4];			/* �}�[�X�����o�@ �������� [���(�P�`�S)][�i�i1.2.3.4] */

	DWORD	n_return[4];				/* �߂��ʏ����� [���(�P�`�S)] */
	DWORD	return_[4];					/* �߂��ʋʐ� [���(�P�`�S)] */

	DWORD	n_pickup[4];				/* �E���ʏ����� [���(�P�`�S)] */
	DWORD	pickup[4];					/* �E���ʋʐ� [���(�P�`�S)] */

	DWORD	g_withdraw[4];				/* ���ʕ��o�q�� [���(�P�`�S)] */
	DWORD	withdraw[4];				/* ���ʕ��o�ʐ� [���(�P�`�S)] */
	DWORD	g_deposit[4];				/* ���ʗa���q�� [���(�P�`�S)] */
	DWORD	deposit[4];					/* ���ʗa���ʐ� [���(�P�`�S)] */

	DWORD	i_ha[4];					/* �[�ʌi�i�� [���(�P�`�S)] */
	DWORD	ha[4];						/* �[�ʋʐ� [���(�P�`�S)] */

	DWORD	n_over[4];					/* �]��ʏ����� [���(�P�`�S)] */
	DWORD	over[4];					/* �]��ʋʐ� [���(�P�`�S)] */
	DWORD	back_ha;					/* �[�ʌi�i�߂��� */

	DWORD	n_get[4];					/* ��荞�ݏ����� [���(�P�`�S)] */
	DWORD	get[4];						/* ��荞�݋ʐ� [���(�P�`�S)] */

	DWORD	n_service[4];				/* �T�[�r�X������ [���(�P�`�S)] */
	DWORD	service[4];					/* �T�[�r�X�ʐ� [���(�P�`�S)] */

	WORD	n_tenyu[4][2];				/* ����͉� [���(�P�`�S)] [����͌����� ���(�P�`�S)] */
	DWORD	tenyu[4];					/* ����͋ʐ� [���(�P�`�S)] */

	WORD	n_henpin[4][2];				/* �ԕi�� [���(�P�`�S)] [ �ԕi������  ���(�P�`�S)] */
	DWORD	henpin[4];					/* �ԕi�ʐ� [���(�P�`�S)] */

	DWORD	n_horyu[4];					/* �ۗ������s���� [���(�P�`�S)] */
	DWORD	horyu[4];					/* �ۗ������s�ʐ� [���(�P�`�S)] */

	DWORD	n_horyue[4];				/* �ۗ����ǂݎ�萬������ [���(�P�`�S)] */
	DWORD	horyue[4];					/* �ۗ����ǂݎ�萬���ʐ� [���(�P�`�S)] */

	DWORD	n_hold[4];					/* �a���茔�ǂݎ�茏�� [���(�P�`�S)] */
	DWORD	hold[4];					/* �a���茔�ǂݎ��ʐ� [���(�P�`�S)] */

	DWORD	n_holde[4];					/* �a���茔�����ό��� [���(�P�`�S)] */
	DWORD	holde[4];					/* �a���茔�����ϋʐ� [���(�P�`�S)] */

	DWORD	clear_num;					/* �N���A�� */
	DWORD	power_num;					/* �d�������� */
	DWORD	admchg_num;					/* �ӔC�ғ���ւ��� */
	DWORD	chosei_num;					/* ������ */
	DWORD	reserve5;					/* �\�� */
	DWORD	reserve6;					/* �\�� */
	DWORD	reserve7;					/* �\�� */
	DWORD	reserve8;					/* �\�� */
	DWORD	reserve9;					/* �\�� */

} pos_toukei_t;
					


/*
 *	�`�P�b�g�v�����^�W�v(00/11/01 �ǉ�)
 */
typedef struct {
	DWORD	sum_num;			/* ���͑��� */
	DWORD	print_num;				/* �a�茔�����ʐ�(�ʏ�) */
	DWORD	print_err_num;			/* �a�茔�����ʐ�(�G���[) */
	DWORD	noinq_num;				/* ���Ɖ�ʐ�(�ʏ�) */
	DWORD	noinq_err_num;		/* ���Ɖ�ʐ�(�G���[) */
	DWORD	savings_num;			/* ���ʐ� */
	DWORD	rtn_num;				/* �]��ʕԋp�ʐ� */
	WORD	sum_ticket;			/* ���͑����� */
	WORD	print_ticket;				/* �a�茔��������(�ʏ�) */
	WORD	print_err_ticket;			/* �a�茔��������(�G���[) */
	WORD	noinq_ticket;				/* ���Ɖ��(�ʏ�) */
	WORD	noinq_err_ticket;		/* ���Ɖ��(�G���[) */
	WORD	savings_ticket;			/* ���ʌ��� */
	WORD	rtn_ticket;				/* �]��ʕԋp���� */
	BYTE	pas_ticket;				/* ��� */
	BYTE	reserve[5];				/* �\���P */
} tp_sum_t;

/*---[ �\���̐ݒ�(�����Ǘ��p) ]----------------------------------------------*/

/*
 *	�������e���|�����\����
 */
typedef struct {
	WORD flag;						/* 0:���g�p 1:���� 2:�ԕi */
	WORD bumon;						/* ����R�[�h */
	BYTE name[ITEM_NAME_LEN];		/* �i�i�̖��O(\0�Ȃ�) */
	BYTE name_term;					/* '\0' */
	WORD nID;						/* ���i�R�[�h */
	DWORD num;						/* ���i�� */
	DWORD price;					/* �P��(P,A,S��pas_type�Ɋ�Â�) */
	DWORD date;						/* �I�������N���� */
	DWORD time;						/* �I���������� */
} change_tmp_t;

/*
 *	����i�i�v�Z�p�\����
 */
typedef struct tagExtraOffer {
	unsigned long 	total,
					dai,
					chu,
					syo;
} T_EXTRA_OFFER;

/*
 *	���i�ҏW�p�\����
 */
typedef struct {
	BYTE bc_head[BC_HEAD_LEN];		/* �o�[�R�[�h �w�b�h */
	BYTE bc_code[BC_CODE_LEN];		/* �o�[�R�[�h �R�[�h */
	DWORD nID;						/* ���i�R�[�h */
	WORD bumon;						/* �����镔�� */
	BYTE name[ITEM_NAME_LEN];		/* ���i�� */
	BYTE reserve[2];				/* �\��\0 */
	WORD tax;						/* �ېŃ^�C�v */
	WORD key;						/* �A�C�e���L�[ */
	DWORD tax_rate;					/* ����ŗ� */
	DWORD p_price;					/* P�����P�� */
	DWORD a_price;					/* A�����P�� */
	DWORD s_price;					/* S�����P�� */
	int en_year;					/* �L���N */
	int en_month;					/* �L���� */
	int en_day;						/* �L���� */
} pos_in_item_t;

/*
 *	�`�P�b�g�\����
 *	���ۗ����\���̂Ƃ��Ă��g�p
 */
typedef struct {
	BYTE type;						/* �@����(TP��2) */
	BYTE tp_no;						/* TP/POS�̔ԍ�(1�`TP_MAX/1�`POS_MAX) */
	DWORD serial_no;				/* �`�P�b�g�ʂ��ԍ� */
	BYTE ken_type;					/* �����(�a���茔��0) */
	BYTE pas_type;					/* PAS���(PAS_P or PAS_A or PAS_S or PAS_EX) */
	DWORD num;						/* ��(���_��)�̐� */
	WORD maker;						/* ���[�J�[�R�[�h */
	WORD online;					/* �I�����C���敪(�G�[�X�d���Ŏg�p) */
	BYTE year;						/* �������t �N */
	BYTE month;						/* �������t �� */
	BYTE day;						/* �������t �� */
	BYTE password[3];				/* �p�X���[�h */
} pos_ticket_t;

/*
 *	�V�Z�O�\���ݒ�T�u�\����
 */
typedef struct {
	BYTE offset;					/* �̾�āi�\���ʒu) */
	BYTE itemno;					/* ���ڂm�n */
	BYTE size;						/* ���ށi���ڻ���) */
	BYTE color;						/* �F�i�\���F�j */
} pos_segconfig_dat_t;

/*
 *	�V�Z�O�\���ݒ�\����
 */
typedef struct {
	pos_segconfig_dat_t segu[10];	/* �ݒ�\���ڐ��� */
} pos_segconfig_t;

/*
 *	���ʓ���󎚗p�\����
 */
typedef struct {
	unsigned long	nippou_make_ps_tm;		/*	����쐬���t(�J�X���t)	*/
	unsigned char	dmy1[12];					/*	*/
	unsigned long	master_kaiinsu;			/*	�������		*/
	unsigned long	del_kaiinsu;			/*	�폜����		*/
	unsigned short	today_kaiinsu;			/*	�{�����X�����	*/
	unsigned short	dumy2;					/*	��а			*/
	unsigned long	pt;						/* 	�߲�Đ� (�݌v): �����݂܂ł̗݌v�߲��  */
	unsigned long	pt_ari_kaiinsu;			/*	�݌v�߲�Ă���������		*/
	unsigned long	chotama;				/* 	���ʐ�  �ʐ�	*/
	unsigned long 	chomed;					/* 	���ʐ�  ���ِ�	*/
	unsigned long	chotama_ari_kaiinsu;	/*	���ʕۗL�������	*/
	unsigned long 	chomeda_ari_kaiinsu;		/*	�����ٕۗL�������	*/
	unsigned long	chotama_yen;			/* 	���ʐ�  �ʐ�(�~���Z)	*/
	unsigned long 	chomed_yen;				/* 	���ʐ�  ���ِ�(�~���Z)	*/
	unsigned short 	today_ptup;    			/* 	�{���߲�Đ� ����(+) 	*/
    unsigned short 	today_ptdown;    		/* 	�{���߲�Đ� ����(-)  */
    long	today_ptupdown;			/*	�{���߲�Đ�	������	 */
    unsigned long 	today_chotamacnt;		/* 	(�{��)���ʗ��p��   */
    unsigned long	today_chomedacnt;			/*	(�{��)�����ٗ��p��   */
    unsigned long 	today_reptamacnt;		/* 	(�{��)����ڰ(��)��        */
    unsigned long 	today_repmedacnt;		/* 	(�{��)����ڰ(����)��        */
    unsigned long 	today_reptamasu;		/* 	(�{��)����ڰ�ʐ�        */
    unsigned long 	today_repmedasu;		/* 	(�{��)����ڰ���ِ�        */
    unsigned long 	today_reptama_yen;		/* 	(�{��)����ڰ�ʐ�(�~���Z) */
    unsigned long 	today_repmeda_yen;		/* 	(�{��)����ڰ���ِ�(�~���Z) */
    unsigned long	today_reptamatesu;		/* 	(�{��)����ڰ�萔�ʗ�     */
    unsigned long	today_repmedatesu;		/* 	(�{��)����ڰ�萔���ٗ�     */
    unsigned long	today_reptamatesu_yen;	/* 	(�{��)����ڰ�萔�ʗ�(�~���Z)        */
    unsigned long	today_repmedatesu_yen;	/* 	(�{��)����ڰ�萔���ٗ�(�~���Z)        */
    unsigned long 	today_chotamain;		/* 	(�{��)���ʎ����   */
    unsigned long 	today_chomedain;		/* 	(�{��)�����َ����   */
    unsigned long 	today_chotamain_yen;	/* 	(�{��)���ʎ����(�~���Z)    */
    unsigned long 	today_chomedain_yen;	/* 	(�{��)�����َ����(�~���Z)    */
    unsigned long 	today_chotamaout;		/* 	(�{��)�i�i�Œ��ʈ�����     */
    unsigned long 	today_chomedaout;		/* 	(�{��)�i�i�Œ����و�����     */
    unsigned long 	today_chotamaout_yen;	/* 	(�{��)�i�i�Œ��ʈ�����(�~���Z)    */
    unsigned long 	today_chomedaout_yen;	/* 	(�{��)�i�i�Œ����و�����(�~���Z)    */
    long	today_chotamaupdown;	/*	(�{��)���ʑ�����	*/
    long	today_chomedaupdown;	/*	(�{��)�����ّ�����	*/
    unsigned short	today_ptup_kaiinsu;		/*	(�{��)�߲�đ����l��	*/
    unsigned short	today_ptdown_kaiinsu;	/*	(�{��)�߲�Ĉ��o�l��	*/
    unsigned short	today_ptupdown_kaiinsu;		/*	(�{��)�߲�đ����l��	*/
    unsigned short	today_chotamain_kaiinsu;	/*	(�{��)���ʗa���l��	*/
    unsigned short	today_reptama_kaiinsu;		/*	(�{��)����ڲ�l��	*/
    unsigned short	today_reputesutama_kaiinsu;	/*	(�{��)����ڲ�萔�����o�l��	*/
    unsigned short	today_chotamaout_kaiinsu;	/*	(�{��)���ʈ��o�l��	*/
    unsigned short	today_chotamaupdown_kaiinsu;	/*	(�{��)���ʑ����l��	*/
    unsigned short	today_chomedain_kaiinsu;	/*	(�{��)�����ٗa���l��	*/
    unsigned short	today_repmeda_kaiinsu;		/*	(�{��)�����ِl��	*/
    unsigned short	today_reputesumeda_kaiinsu;	/*	(�{��)�����َ萔�����o�l��	*/
    unsigned short	today_chomedaout_kaiinsu;	/*	(�{��)�����و��o�l��	*/
    unsigned short	today_chomedaupdown_kaiinsu;/*	(�{��)�����ّ����l��	*/
    unsigned short 	dumy3;
    
    unsigned long	pt_ave;						/* 	�߲�Đ� (�݌v����)  */
    unsigned short 	today_ptup_ave;    			/* 	�{���߲�Đ� ����(+)(����) 	*/
    unsigned short 	today_ptdown_ave;    		/* 	�{���߲�Đ� ����(-)(����)  */
    long	today_ptupdown_ave;			/*	�{���߲�Đ�	������(����)	 */
    
    unsigned long	chotama_ave;				/* 	���ʐ�(����)	*/
    unsigned long 	today_chotamain_ave;		/* 	(�{��)���ʎ����(����)   */
    unsigned long 	today_reptamasu_ave;		/* 	(�{��)����ڰ�ʐ�(����)  */
    unsigned long	today_reptamatesu_ave;		/* 	(�{��)����ڰ�萔�ʗ�(����)     */
    unsigned long 	today_chotamaout_ave;		/* 	(�{��)�i�i�Œ��ʈ�����(����)      */
    unsigned long	today_chotamaupdown_ave;	/*	(�{��)���ʑ�����(����)		*/
    
    unsigned long	chomeda_ave;				/* 	�����ِ�(����)	*/
    unsigned long 	today_chomedain_ave;		/* 	(�{��)�����َ����(����)   */
    unsigned long 	today_repmedasu_ave;		/* 	(�{��)�����ِ�(����)  */
    unsigned long	today_repmedatesu_ave;		/* 	(�{��)�����َ萔��(����)     */
    unsigned long 	today_chomedaout_ave;		/* 	(�{��)�i�i�Œ����و�����(����)      */
    unsigned long	today_chomedaupdown_ave;	/*	(�{��)�����ّ�����(����)		*/
    
    
    unsigned long	chotama_ave_yen;				/* 	���ʐ�(����)(�~���Z)	*/
    unsigned long 	today_chotamain_ave_yen;		/* 	(�{��)���ʎ����(����)(�~���Z)   */
    unsigned long 	today_reptamasu_ave_yen;		/* 	(�{��)����ڰ�ʐ�(����)(�~���Z)  */
    unsigned long	today_reptamatesu_ave_yen;		/* 	(�{��)����ڰ�萔�ʗ�(����)(�~���Z)     */
    unsigned long 	today_chotamaout_ave_yen;		/* 	(�{��)�i�i�Œ��ʈ�����(����)(�~���Z)      */
    unsigned long	today_chotamaupdown_ave_yen;	/*	(�{��)���ʑ�����(�~���Z)		*/
    
    unsigned long	chomeda_ave_yen;				/* 	�����ِ�(����)(�~���Z)	*/
    unsigned long 	today_chomedain_ave_yen;		/* 	(�{��)�����َ����(����)(�~���Z)   */
    unsigned long 	today_repmedasu_ave_yen;		/* 	(�{��)�����ِ�(����)(�~���Z)  */
    unsigned long	today_repmedatesu_ave_yen;		/* 	(�{��)�����َ萔��(����)(�~���Z)     */
    unsigned long 	today_chomedaout_ave_yen;		/* 	(�{��)�i�i�Œ����و�����(����)(�~���Z)      */
    unsigned long	today_chomedaupdown_ave_yen;	/*	(�{��)�����ّ�����(�~���Z)		*/
    
    unsigned char	dumy4[92];				/*	��а	*/
} pos_cydmnip_t;


typedef struct {
	unsigned long date;								/* �N���� */
	unsigned short time;							/* ���� */
	unsigned char stype;							/* �����敪 */
	unsigned char dmy1;								/* �\���P */
	unsigned long kkaiin_no1;						/* ����ԍ��P */
	unsigned long kkaiin_no2;						/* ����ԍ��Q */
	unsigned char dmy2[8];							/* �\���Q  */
} T_MEMBER_HISTORY;


/* ���[I/F�ݒ��ް��\���� */
typedef struct {
	WORD tesu;
	WORD harai;
} T_SRIF_BUTTON;


/* ���[I/F�ݒ��ް��\���� */
typedef struct {
	BYTE syubetu;
	BYTE pt;
	WORD max_tama;
	WORD max_med;
	T_SRIF_BUTTON tama[6];
	T_SRIF_BUTTON med[6];
} T_SRIF_CONF;


/*---[ �A�v���P�[�V�������[�N]-----------------------------------------------*/

/* ��POS �o�[�W������� */
extern DWORD pos_prog_ver;						/* POS�v���O�����o�[�W���� */
extern DWORD pos_prog_sum;						/* POS�v���O����CRC */
extern DWORD tp_prog_ver;						/* TP�v���O�����o�[�W���� */
extern DWORD tp_prog_sum;						/* TP�v���O����CRC */
extern DWORD pos_setfile_sum;					/* �ݒ�t�@�C��SUM */
extern DWORD pos_mstfile_date;					/* �}�X�^�[�t�@�C�����t */
extern DWORD pos_mstfile_time;					/* �}�X�^�[�t�@�C������ */

/* ��POS �X�e�[�^�X�t���O */
extern BYTE pos_boot_flag;						/* POS����N�����TRUE */
extern BYTE pos_parent_flag;					/* �ePOS�̏ꍇTRUE */
extern BYTE pos_offline_flag;					/* �I�t���C�����[�h��TRUE */
extern BYTE pos_total_flag;						/* �W�v������TRUE */
extern BYTE pos_recvok_flag;					/* HALNET�d����MOK��Ԃ�TRUE */

extern BYTE pos_open_flag;						/* �c�Ə�Ԃ�TRUE */
extern BYTE pos_change_flag;					/* �����\��Ԃ�TRUE */
extern BYTE pos_hakken_flag;					/* �����\��Ԃ�TRUE */
extern BYTE pos_clear_flag;						/* �N���A��Ԃ�TRUE */
extern BYTE pos_rec_flag;						/* �L����Ԃ�TRUE */
extern BYTE pos_htpsts_flag;					/* �t�@�C���]������TRUE */
extern BYTE pos_sys_rec_flag;					/* �V�X�e���|�L��������Ԃ�TRUE */
extern BYTE pos_sys_clr_flag;					/* �V�X�e���|�N���A������Ԃ�TRUE */
extern BYTE pos_pclr_flag;						/* OPEN�O�N���A������Ԃ�TRUE */
extern BYTE pos_testmode_flag;					/* �e�X�g�������[�h��TRUE */
extern BYTE pos_stock_flag;						/* �I�������敪�t���O
													0:��POS
													1:�S�X
													2:�i�i�I����ʂ��J���Ă��Ȃ�
													*/
extern BYTE pos_sansho_flag;					/* �݌ɎQ�ƁA�󎚔͈�(0:��POS�A1:�S�X) */
extern BYTE pos_skip_flag;					  /* �X�L�b�v����/�Ȃ��t���O(0:�Ȃ��A1:����) */
extern BYTE prn_noskip_all_flag;			/* �S�i�i������(�����)�󎚃t���O */
extern WORD	all_item_nid[ITEM_MAX];			/* �X�L�b�v���莞�̑S�i�i��nID�i�[ */
extern WORD	skip_item_count;				/* �X�L�b�v���莞�̌i�i�J�E���g�� */
extern WORD all_noskip_bumonID; 		/* �S�i�i�����߃X�L�b�v�Ȃ����g�p */
extern WORD bumon_prn_title_flag; 		/* ����ʈ󎚂̃^�C�g���󎚂���/���Ȃ��Ɏg�p */
extern BYTE pos_group_no; 						/* POS�̃O���[�vNo�Ɏg�p */
extern BYTE force_stop_flag;					/* TP������~��Ԃ�TRUE */
extern BYTE saving_shop_no;						/* ���ʓX��No 0:A�X 1:B�X */
extern BYTE saving_join_flag;					/* ���ʓX�܍��Z�t���O 0:���Z�Ȃ� 1:���Z����*/
extern BYTE point_erea_flag;					/* �|�C���g�A�N�Z�X�t���O 0:���Z�G���A 1:���X�|�C���g�G���A*/
extern BYTE point_join_flag;					/* �|�C���g�X�܍��Z�t���O 0:���Z�Ȃ� 1:���Z����*/
extern BYTE welcome_point;						/* ���X�|�C���g�l*/
extern BYTE other_no_move;						/* ������ʂ֎����ڍs���ɁA���肷��܂ő���ʂֈڍs�ł��Ȃ�����t���O 1�F�ڍs�s�� */
extern BYTE exe_all_nakajime_flag;		/* (�ePOS)�S�X�S�i�i�����߈󎚂��s�������̃t���O 0:�X���� 1�F�S�i�i������ */
extern BYTE rec_all_nakajime_flag;		/* (�qPOS)�S�X�S�i�i�����ߓd������M�������̃t���O 0:�X���� 1�F�S�i�i������ */
extern BYTE kind_syubetu;							/* �g�p�����ʃt���O(4���) */
extern BYTE physic_syubetu;						/* �e��ʂ̕������(4���) */
extern BYTE all_kind_syubetu;					/* �S�X���̎g�p�����ʃt���O(4���) */
extern BYTE all_physic_syubetu;				/* �S�X���̊e��ʂ̕������(4���) */
extern BYTE disp_syubetu_char[4][TYPE_NAME_LEN + 1];		/* �e��ʂ̕�����(A,B�X�e4���) */
extern BYTE select_popup_str[4];			/* ��ʑI��POPUP������1�`4(�\���̒l���Z�b�g�����) */


/* �ePOS �o�[�W������� */
extern DWORD ppos_prog_ver;						/* POS�v���O�����o�[�W���� */
extern DWORD ppos_prog_sum;						/* POS�v���O����CRC */
extern DWORD ptp_prog_ver;						/* TP�v���O�����o�[�W���� */
extern DWORD ptp_prog_sum;						/* TP�v���O����CRC */
extern DWORD ppos_setfile_sum;					/* �ݒ�t�@�C��SUM */
extern DWORD ppos_mstfile_date;					/* �}�X�^�[�t�@�C�����t */
extern DWORD ppos_mstfile_time;					/* �}�X�^�[�t�@�C������ */

/* �ePOS �X�e�[�^�X�t���O */
extern BYTE ppos_open_flag;						/* �c�Ə�Ԃ�TRUE */
extern BYTE ppos_change_flag;					/* �����\��Ԃ�TRUE */
extern BYTE ppos_hakken_flag;					/* �����\��Ԃ�TRUE */
extern BYTE ppos_clear_flag;					/* �N���A��Ԃ�TRUE */
extern BYTE ppos_rec_flag;						/* �L����Ԃ�TRUE */
extern BYTE ppos_htpsts_flag;					/* �t�@�C���]������TRUE */

/* CA�X�e�[�^�X�t���O */
extern DWORD ca_syukei_flag;					/* CA�W�v����TRUE */

/* POSPC�X�e�[�^�X�t���O */
extern WORD pospc_syukei_state;				/* POSPC�W�v������� */
extern BYTE	 pospc_filler_flag;					/* POSPC�ւ̃t�B���[���M�t���O */
extern WORD pos_close_state;					/* �ePOS�X������� */

/*
 *
 *	�V�X�e�����[�N
 *
 */
extern char* update_time;						/* �v���O�������t */
extern BYTE admin_no;							/* �ӔC�Ҕԍ�(0�`ADMIN_MAX-1)*/
extern BYTE pos_no;								/* POS�ԍ�(0�`POS_MAX-1) */
extern BYTE close_error_id;						/* �X�����G���[�@��ARC_ID */
extern DWORD err_machine;						/* �X�����G���[�ԍ�
													POS:bit0�`bit7(POS1�`POS8) ���v�f�[�^�G���[
													    bit8�`bit15(POS1�`POS8) �݌Ƀf�[�^�G���[
													TP :bit0�`bit31(TP1�`TP32) TP�f�[�^�G���[
													CDS:bit0�`bit7(CDS1�`CDS8) CDS�f�[�^�G���[
													*/
extern BYTE master_ok_flag;						/* TRUE:Ͻ���M����OK bit0=�i�i�Abit1=�ӔC�� */




/*
 *
 *	������
 *	�O���[�o�����[�N
 *
 */
extern ulong total_ball;						/* �݌v���͋�(���ʊ܂�) */
extern ulong totalex_ball;						/* �݌v���͋�(���ʊ܂܂�) */
extern ulong left_ball;							/* �c��(���ʊ܂�) */
extern ulong leftex_ball;						/* �c��(���ʊ܂܂�) */
extern ulong rest_ball;							/* �]��(���ʊ܂�) */
extern ulong saveorg_ball;						/* ����(��)�����J�n��(1��ʖ�) */
extern ulong saveorg_ar;							/* ����(��)�����J�n��(2��ʖ�) */
extern ulong saveorg_slot;						/* ����(�X���b�g)�����J�n��(3��ʖ�) */
extern ulong saveorg_ex;							/* ����(�X���b�g)�����J�n��(4��ʖ�) */
extern ulong arng_num;
extern ulong saveorg;							/* ����(�ʁE�X���b�g)�J�n�� */
extern ulong savein;							/* ����(�ʁE�X���b�g)�a���� */
extern ulong saveout;							/* ����(�ʁE�X���b�g)���o�� */
extern ulong savesim;							/* ����(�ʁE�X���b�g)���ʐ� */
extern uint ticket_num;							/* �`�P�b�g�� */
extern int pas_type;							/* ������PAS��� */
extern int change_cursor;						/* �������J�[�\�� */
extern int change_top;							/* �������\���g�b�v�ԍ� */
extern int change_level;						/* �������x�� */
extern int seg_level;							/* �V�Z�O���x�� */
extern BYTE change_auto_flag;					/* �����Čv�Z�t���O */
extern BYTE change_ronly_flag;					/* Read Only�t���O */
extern BYTE change_card_flag;					/* ����J�[�h�}���t���O */
extern BYTE card_on;							/* ���ʏ����ς� 	*/
extern BYTE change_savings_flag;				/* ���ʎw��t���O */
extern int tokushu_items;						/* ����i�i��(0�`TOKUSHU_MAX)*/
extern pos_item_t tokushu_item[TOKUSHU_MAX];	/* ����i�i�f�[�^ */
extern ulong tokushu_num[TOKUSHU_MAX];			/* ����i�i�����v�Z�� */
extern ulong tokushu_select[TOKUSHU_MAX];		/* ����i�i�m��� */
extern change_tmp_t change_tmp[CHANGE_MAX];		/* �����p�e���|���� */
extern WORD	mars_harai_num[2][TOKUSHU_MAX];		/* �i�i1,2,3,4�̕��o���� */
extern back_rireki_t back_rireki[BACK_ITEM_MAX];	/* �ԕi�A�߂������\���� */
extern uchar tokushu_dec_flag;					/* �����P�������_�Ή� */
extern uchar ks_errcode;						/* KS�����M�����װ���� */
extern uchar cds_cardout_flag;					/* TRUE:CDS���ޔr�o�װ�� */
extern uchar yesterday_close_flag;				/* �O���X�����m�F�t���O(TRUE:�m�F�ς�) */
extern uchar g_glory_status_flag;					/* �O���[����ԃ`�F�b�N�t���O 0:�s�� 1:�s��Ȃ� */
extern uchar chg_horyuu_admin_flag;				/* ����(�ۗ�������)���ɐӔC�҃J�[�h�������ꂽ? 1:�����ꂽ */


/*
 *
 *	�\���̊֘A(�펞���[�N�ɕێ��������)
 *
 */
extern pos_basic_t pos_basic;					/* POS��{�ݒ� */
extern pos_const_t pos_const;					/* POS�萔�ݒ� */
extern pos_toukei_t pos_toukei;					/* ���̋@��̓��v�f�[�^ */
extern harai_setting_t pos_harai;				/* �ڑ����o�@�̏�� */
extern pos_group_t pos_group;					/* �l�b�g���[�N�O���[�v */
extern pos_setting_t pos_setting[POS_MAX];		/* �ePOS�̏�� */
extern tp_setting_t tp_setting[TP_MAX];			/* �eTP�̏�� */
extern pos_zaiko_t b_zaiko_blk[9];


/*----------------------------------------------------------------------------
 *
 * 99.2.15 �ȍ~�ǉ���
 *
 *--------------------------------------------------------------------------*/

#define ADMINISTRATOR_MAX	ADMIN_MAX			/* �ӔC�ҍő吔 */
#define ADMIN_LIMIT_LEVEL_MAX	6				/* ���쐧�����x���ő吔 */
#define ADMIN_LIMIT_HEAD_MAX	64				/* ���쐧�����ڍő吔 */

#if 0
/*
 * �ӔC�ҏ��o�^�ݒ�
 */
typedef struct {
	short num;				/* �ԍ� */
	short level;			/* ���샌�x�� */
	unsigned char name[16];	/* �ӔC�Җ�		*/
} admin_info_t;

extern admin_info_t admin_info_work[ADMINISTRATOR_MAX];
extern char admin_limit[ADMIN_LIMIT_LEVEL_MAX][ADMIN_LIMIT_HEAD_MAX];
extern admin_info_t administrator;
#endif

#if 0
/*
 * �Ǎ��݃`�P�b�g���(01/04/05)
 */
typedef struct {
	BYTE 	barcode[BC_CODE_LEN];		/* �o�[�R�[�h�f�[�^ */
	BYTE 	req_flag;					/* �Ɖ���t���O(TRUE:�Ɖ) */
	clock_t	timer;						/* �Ɖ�ʔ�����̃^�C�} */
} ticket_data_t;

extern ticket_data_t	ticket_work;
#endif

/*
 *	�ӔC�Ґ������������[�N�\����(22byte)
 */
typedef struct {
	unsigned char gcode;		/* �O���[�v�R�[�h */
	unsigned char code;			/* �ӔC�҃R�[�h */
	unsigned long unyou;		/* �^�p���� */
	unsigned long change;		/* �������� */
	unsigned long zaiko;		/* �݌ɊǗ����� */
	unsigned long kaiin;		/* �����񏈗� */
	unsigned long config;		/* �ݒ�ύX���� */
} admin_operation_t;

extern admin_operation_t admin_limit;		/* �ӔC�Ґ������ */
extern uchar poscs_flag;					/* != 0:KS�J�[�h�C���ςݒʒm */
extern uchar transaction_no;			/* �g�����U�N�V�����ԍ� */

#endif	/* _pos_work_h_ */
