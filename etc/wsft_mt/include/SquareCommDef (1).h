/****************************************************************************/
/*							���ʒ�` �w�b�_�t�@�C��							*/
/****************************************************************************/
/*	2006/06/30													Y.Okajima	*/
/****************************************************************************/
//----------------------------------------------------------------------------
/*
 *	$Id$
 */
//----------------------------------------------------------------------------
#if !defined( __SQUARECOMMDEF_H__ )
#define	__SQUARECOMMDEF_H__

//----------------------------------------------------------------------------
/****************************************************************************/
/*	���O�p���ʖ�															*/
/****************************************************************************/
#define		SQUARE_TCP_COMM_APP_TITLE				"SQTCPP"
#define		SQUARE_UDP_COMM_APP_TITLE				"SQUDPP"

//----------------------------------------------------------------------------
/****************************************************************************/
/*	�s�b�o���M���N�G�X�g���b�Z�[�W											*/
/****************************************************************************/
#define		UWM_SQUARE_COMM_TCP_SEND				"UWM_SQUARE_COMM_TCP_SEND"
#define		UWM_SQUARE_COMM_TCP_SET_DATE			"UWM_SQUARE_COMM_TCP_SET_DATE"

#define		UWM_SQUARE_COMM_TCP_SEND_COMPLETE		"UWM_SQUARE_COMM_TCP_SEND_COMPLETE"
#define		UWM_SQUARE_COMM_TCP_SEND_COMPLETE_ERROR	"UWM_SQUARE_COMM_TCP_SEND_COMPLETE_ERROR"

//----------------------------------------------------------------------------
/****************************************************************************/
/*	�t�c�o���M���N�G�X�g���b�Z�[�W											*/
/****************************************************************************/
#define		UWM_SQUARE_COMM_UDP_SEND				"UWM_SQUARE_COMM_UDP_SEND"

//----------------------------------------------------------------------------
/****************************************************************************/
/*	�T�[�o�[�����ڑ���														*/
/****************************************************************************/
#define	SQUARE_COMM_TCP_MAX_CONN			4
#define	SQUARE_COMM_TCP_PORT_START			10601
#define	SQUARE_COMM_TCP_PORT_END			10605
#define	SQUARE_COMM_UDP_PORT_START			20601
#define	SQUARE_COMM_UDP_PORT_END			20602

//----------------------------------------------------------------------------
/****************************************************************************/
/*	�I���R�[�h																*/
/****************************************************************************/
enum ENUM_SQUARE_COMM_RESULT {
	SQUARE_COMM_RESULT_CODE_NG = -1,			// �G���[�I��
	SQUARE_COMM_RESULT_CODE_OK = 0,				// ����I��
	SQUARE_COMM_RESULT_CODE_ENDSESSION = 9998,	// �����I��
	SQUARE_COMM_RESULT_CODE_ABORT = 9999		// �����I��
};

//----------------------------------------------------------------------------
/****************************************************************************/
/*	�d�����																*/
/****************************************************************************/
enum	ENUM_SQUARE_MSG_ID {
	SQUARE_MSG_ID_UNKNOWN = 0,					// �s��

	SQUARE_MSG_ID_TM001 = 1001,					// TM001
	SQUARE_MSG_ID_TM002 = 1002,					// TM002

	SQUARE_MSG_ID_SU001 = 2001,					// SU001
	SQUARE_MSG_ID_SU005 = 2005,					// SU005
	SQUARE_MSG_ID_SU101 = 2101,					// SU101
	SQUARE_MSG_ID_SU102 = 2102,					// SU102

	SQUARE_MSG_ID_EA001 = (-1001),				// EA001
	SQUARE_MSG_ID_EA002 = (-1002),				// EA002

	SQUARE_MSG_ID_EE001 = (-5001),				// EE001
	SQUARE_MSG_ID_EE101 = (-5101),				// EE101
	SQUARE_MSG_ID_EE201 = (-5201),				// EE201
	SQUARE_MSG_ID_EE301 = (-5301),				// EE301
	SQUARE_MSG_ID_EE302 = (-5007),				// EE302
	SQUARE_MSG_ID_EE303 = (-5303),				// EE303
	SQUARE_MSG_ID_EE304 = (-5304),				// EE304
	SQUARE_MSG_ID_EE305 = (-5305),				// EE305
	SQUARE_MSG_ID_EE401 = (-5401),				// EE401
	SQUARE_MSG_ID_EE402 = (-5402),				// EE402
	SQUARE_MSG_ID_EE403 = (-5403),				// EE403
	SQUARE_MSG_ID_EE404 = (-5404),				// EE404
	SQUARE_MSG_ID_EE410 = (-5410),				// EE410
	SQUARE_MSG_ID_EE501 = (-5501),				// EE501
	SQUARE_MSG_ID_EE502 = (-5502),				// EE502
	SQUARE_MSG_ID_EF001 = (-6001),				// EF001
	SQUARE_MSG_ID_EF002 = (-6002),				// EF002
	SQUARE_MSG_ID_EH001 = (-8001),				// EH001
	SQUARE_MSG_ID_EH002 = (-8002),				// EH002
	SQUARE_MSG_ID_EH003 = (-8003),				// EH003
	SQUARE_MSG_ID_EH101 = (-8101),				// EH101
	SQUARE_MSG_ID_EH102 = (-8102),				// EH102
	SQUARE_MSG_ID_EH201 = (-8201),				// EH201
	SQUARE_MSG_ID_EH301 = (-8301),				// EH301
	SQUARE_MSG_ID_EL001 = (-12001),				// EL001
	SQUARE_MSG_ID_EL002 = (-12002),				// EL002
	SQUARE_MSG_ID_EL003 = (-12003),				// EL003
	SQUARE_MSG_ID_EL004 = (-12004),				// EL004
	SQUARE_MSG_ID_EL005 = (-12005),				// EL005
	SQUARE_MSG_ID_EL006 = (-12006),				// EL006
	SQUARE_MSG_ID_EL101 = (-12101),				// EL101
	SQUARE_MSG_ID_EL201 = (-12201),				// EL201
	SQUARE_MSG_ID_EL202 = (-12202),				// EL202
	SQUARE_MSG_ID_EL203 = (-12203),				// EL203
	SQUARE_MSG_ID_EL204 = (-12204),				// EL204
	SQUARE_MSG_ID_EL205 = (-12205),				// EL205
	SQUARE_MSG_ID_EL301 = (-12301)				// EL301
};

//----------------------------------------------------------------------------
/****************************************************************************/
/*	�d���h�c																*/
/****************************************************************************/
#define		SQUARE_HEADER_MSG_ID_TM001			"TM001"			// TM001
#define		SQUARE_HEADER_MSG_ID_TM002			"TM002"			// TM002

#define		SQUARE_HEADER_MSG_ID_SU001			"SU001"			// SU001
#define		SQUARE_HEADER_MSG_ID_SU005			"SU005"			// SU005
#define		SQUARE_HEADER_MSG_ID_SU101			"SU101"			// SU101
#define		SQUARE_HEADER_MSG_ID_SU102			"SU102"			// SU102

#define		SQUARE_HEADER_MSG_ID_EA001			"EA001"			// EA001
#define		SQUARE_HEADER_MSG_ID_EA002			"EA002"			// EA002
#define		SQUARE_HEADER_MSG_ID_EE001			"EE001"			// EE001
#define		SQUARE_HEADER_MSG_ID_EE101			"EE101"			// EE101
#define		SQUARE_HEADER_MSG_ID_EE201			"EE201"			// EE201
#define		SQUARE_HEADER_MSG_ID_EE301			"EE301"			// EE301
#define		SQUARE_HEADER_MSG_ID_EE302			"EE302"			// EE302
#define		SQUARE_HEADER_MSG_ID_EE303			"EE303"			// EE303
#define		SQUARE_HEADER_MSG_ID_EE304			"EE304"			// EE304
#define		SQUARE_HEADER_MSG_ID_EE305			"EE305"			// EE305
#define		SQUARE_HEADER_MSG_ID_EE401			"EE401"			// EE401
#define		SQUARE_HEADER_MSG_ID_EE402			"EE402"			// EE402
#define		SQUARE_HEADER_MSG_ID_EE403			"EE403"			// EE403
#define		SQUARE_HEADER_MSG_ID_EE404			"EE404"			// EE404
#define		SQUARE_HEADER_MSG_ID_EE410			"EE410"			// EE410
#define		SQUARE_HEADER_MSG_ID_EE501			"EE501"			// EE501
#define		SQUARE_HEADER_MSG_ID_EE502			"EE502"			// EE502
#define		SQUARE_HEADER_MSG_ID_EF001			"EF001"			// EF001
#define		SQUARE_HEADER_MSG_ID_EF002			"EF002"			// EF002
#define		SQUARE_HEADER_MSG_ID_EH001			"EH001"			// EH001
#define		SQUARE_HEADER_MSG_ID_EH002			"EH002"			// EH002
#define		SQUARE_HEADER_MSG_ID_EH003			"EH003"			// EH003
#define		SQUARE_HEADER_MSG_ID_EH101			"EH101"			// EH101
#define		SQUARE_HEADER_MSG_ID_EH102			"EH102"			// EH102
#define		SQUARE_HEADER_MSG_ID_EH201			"EH201"			// EH201
#define		SQUARE_HEADER_MSG_ID_EH301			"EH301"			// EH301
#define		SQUARE_HEADER_MSG_ID_EL001			"EL001"			// EL001
#define		SQUARE_HEADER_MSG_ID_EL002			"EL002"			// EL002
#define		SQUARE_HEADER_MSG_ID_EL003			"EL003"			// EL003
#define		SQUARE_HEADER_MSG_ID_EL004			"EL004"			// EL004
#define		SQUARE_HEADER_MSG_ID_EL005			"EL005"			// EL005
#define		SQUARE_HEADER_MSG_ID_EL006			"EL006"			// EL006
#define		SQUARE_HEADER_MSG_ID_EL101			"EL101"			// EL101
#define		SQUARE_HEADER_MSG_ID_EL201			"EL201"			// EL201
#define		SQUARE_HEADER_MSG_ID_EL202			"EL202"			// EL202
#define		SQUARE_HEADER_MSG_ID_EL203			"EL203"			// EL203
#define		SQUARE_HEADER_MSG_ID_EL204			"EL204"			// EL204
#define		SQUARE_HEADER_MSG_ID_EL205			"EL205"			// EL205
#define		SQUARE_HEADER_MSG_ID_EL301			"EL301"			// EL301

//----------------------------------------------------------------------------
/****************************************************************************/
/*	�[������																*/
/****************************************************************************/
#define		SQUARE_HEADER_TERM_NAME_TCAN		"TCAN"			// ���M���[������
#define		SQUARE_HEADER_TERM_NAME_CC			"CC  "			// ���M���[������
#define		SQUARE_HEADER_TERM_NAME_TC			"TC  "			// ���M���[������
#define		SQUARE_HEADER_TERM_NAME_UNKNOWN		"    "			// ���M���[������

//----------------------------------------------------------------------------
/****************************************************************************/
/*	�l�b�g���[�N�ԍ�														*/
/****************************************************************************/
#define		SQUARE_HEADER_SRC_NETADDRESS		8				// ���M���l�b�g���[�N�ԍ�

//----------------------------------------------------------------------------
/****************************************************************************/
/*	�[���ԍ�																*/
/****************************************************************************/
#define		SQUARE_HEADER_SRC_STATION			0				// ���M���[���ԍ�
#define		SQUARE_HEADER_DEST_STATION			201				// ���M��[���ԍ�

//----------------------------------------------------------------------------
/****************************************************************************/
/*	��ԍ�																	*/
/****************************************************************************/
//#define		SQUARE_HEADER_TERMNO_BASE			8000			// ��ԍ��i�x�[�X�ԍ��j
//					�ˁ@HALLINFO_INI_DEFAULT_SUHEADER_TERMNO_BASE�@T2X004967_PBANKV�Ȃ��Ή�

//----------------------------------------------------------------------------
/****************************************************************************/
/*	�j��(Header)															*/
/****************************************************************************/
#define		SQUARE_HEADER_WEEK_SUN				0x00			// ���j��
#define		SQUARE_HEADER_WEEK_MON				0x01			// ���j��
#define		SQUARE_HEADER_WEEK_TUE				0x02			// �Ηj��
#define		SQUARE_HEADER_WEEK_WED				0x03			// ���j��
#define		SQUARE_HEADER_WEEK_THU				0x04			// �ؗj��
#define		SQUARE_HEADER_WEEK_FRI				0x05			// ���j��
#define		SQUARE_HEADER_WEEK_SAT				0x06			// �y�j��

//----------------------------------------------------------------------------
/****************************************************************************/
/*	����d���敪(SU001)													*/
/****************************************************************************/
#define		SQUARE_SU001_USE_SU005				5				// SU005�g�p

//----------------------------------------------------------------------------
/****************************************************************************/
/*	����d�����M�Ԋu(SU001)												*/
/****************************************************************************/
#define		SQUARE_SU001_DEFAULT_INTERVAL_SU005	2*60*1000		// �Q��

//----------------------------------------------------------------------------
/****************************************************************************/
/*	�V�X�e���t���O(SU001)													*/
/****************************************************************************/
#define		SQUARE_SU001_TYPE_SQUEARE			0				// SQUARE
#define		SQUARE_SU001_TYPE_SQUEARE2			1				// SQUARE2

//----------------------------------------------------------------------------
/****************************************************************************/
/*	�A���[���ʒm�i����n�F���܁A����n�F��ʁA�ُ�n�j(SU001)				*/
/****************************************************************************/
#define		SQUARE_SU001_IGNORE_ALARM			0				// �ʒm���Ȃ�
#define		SQUARE_SU001_NOTIFY_ALARM			1				// �ʒm����


//----------------------------------------------------------------------------
/****************************************************************************/
/*	�ő呗�M����(SU005)														*/
/****************************************************************************/
#define		SU005_SEND_DATA_MAX					6

//----------------------------------------------------------------------------
/****************************************************************************/
/*	�敪(SU005)																*/
/****************************************************************************/
#define		SQUARE_SU005_TYPE_DAI				1				// ��
#define		SQUARE_SU005_TYPE_KEISUKI			2				// �v���@
#define		SQUARE_SU005_TYPE_KINKO				3				// ����
#define		SQUARE_SU005_TYPE_RYOGAEKI			4				// ���֋@
#define		SQUARE_SU005_TYPE_KENBAIKI			5				// �����@
#define		SQUARE_SU005_TYPE_GAIKASHIKI		6				// �O�݋@
#define		SQUARE_SU005_TYPE_SEISANKI			7				// ���Z�@
#define		SQUARE_SU005_TYPE_HAKKENKINKO		8				// ��������

//----------------------------------------------------------------------------
/****************************************************************************/
/*	��ԍ��I�t�Z�b�g(SU005)													*/
/****************************************************************************/
#define		SQUARE_SU005_DAI_ID_OFFSET			   0			// ��@�@�@�F�{�O
#define		SQUARE_SU005_KEISUKI_ID_OFFSET		4000			// �v���@�@�F�{�S�O�O�O
#define		SQUARE_SU005_KINKO_ID_OFFSET		4300			// ���Ɂ@�@�F�{�S�R�O�O
#define		SQUARE_SU005_RYOGAEKI_ID_OFFSET		4500			// ���֋@�@�F�{�S�T�O�O
#define		SQUARE_SU005_KENBAIKI_ID_OFFSET		4600			// �����@�@�F�{�S�U�O�O
#define		SQUARE_SU005_GAIKASHIKI_ID_OFFSET	4800			// �O�݋@�@�F�{�S�W�O�O
#define		SQUARE_SU005_SEISANKI_ID_OFFSET		4900			// ���Z�@�@�F�{�S�X�O�O
#define		SQUARE_SU005_HAKKENKINKO_ID_OFFSET	4700			// �������ɁF�{�S�V�O�O

//----------------------------------------------------------------------------
/****************************************************************************/
/*	�V�Z���ԏ��(SU005)													*/
/****************************************************************************/
#define		SQUARE_SU005_DAI_KAD_CND			0x00000001		//�ғ���
#define		SQUARE_SU005_DAI_UTI_CND			0x00000002		//�Ŏ~��
#define		SQUARE_SU005_DAI_TOK_CND			0x00000004		//���ܒ�
#define		SQUARE_SU005_DAI_HIB_CND			0x00000010		//���m��
#define		SQUARE_SU005_DAI_RTK_CND			0x00000080		//�A������
#define		SQUARE_SU005_DAI_YOK_CND			0x00000100		//�\����
#define		SQUARE_SU005_DAI_KAR_CND			0x00000200		//��M��
#define		SQUARE_SU005_DAI_HOU_CND			0x00000400		//���u��
#define		SQUARE_SU005_DAI_CHK_CND			0x00000800		//���ӑ�
#define		SQUARE_SU005_DAI_FRM_CND			0x00004000		//�ؘg�J
#define		SQUARE_SU005_DAI_PNL_CND			0x00008000		//���J
#define		SQUARE_SU005_DAI_FTK_CND			0x00010000		//�������蒆
#define		SQUARE_SU005_DAI_OP1_CND			0x00020000		//�n�o�P
#define		SQUARE_SU005_DAI_OP2_CND			0x00040000		//�n�o�Q
#define		SQUARE_SU005_DAI_OP3_CND			0x00080000		//�n�o�R
#define		SQUARE_SU005_DAI_MAD_CND			0x00100000		//���g�J
#define		SQUARE_SU005_DAI_SN2_CND			0x00200000		//�s���Q
#define		SQUARE_SU005_DAI_SN1_CND			0x00400000		//�s���P
#define		SQUARE_SU005_DAI_CAL_CND			0x00800000		//��ďo
#define		SQUARE_SU005_DAI_KTR_CND			0x01000000		//�݋@�g���u��
#define		SQUARE_SU005_DAI_KCD_CND			0x02000000		//�݋@���
#define		SQUARE_SU005_DAI_D01_CND			0x04000000		//100�f��
#define		SQUARE_SU005_DAI_D05_CND			0x08000000		//500�f��
#define		SQUARE_SU005_DAI_D10_CND			0x10000000		//1000�f��
#define		SQUARE_SU005_DAI_DCD_CND			0x20000000		//�J�[�h�f��
#define		SQUARE_SU005_DAI_MTK_CND			0x40000000		//���g����
#define		SQUARE_SU005_DAI_AUT_CND			0x80000000		//�����Ŏ~

//----------------------------------------------------------------------------
/****************************************************************************/
/*	�V�Z���ԏ��(SU005)													*/
/****************************************************************************/
#define		SQUARE_SU005_NASCA_REPLAY_FLG_OFF	0x00			// �n�e�e
#define		SQUARE_SU005_NASCA_REPLAY_FLG_ON	0x01			// �n�m

//----------------------------------------------------------------------------
/****************************************************************************/
/*	�f��(SU005)																*/
/****************************************************************************/
#define		SQUARE_SU005_LINE_NORMAL			0x00			// �n�e�e
#define		SQUARE_SU005_LINE_BREAK				0x01			// �n�m

//----------------------------------------------------------------------------
/****************************************************************************/
/*	�ő�f�[�^����(SU101)													*/
/****************************************************************************/
#define		SQUARE_SU101_DATA_MAX				14

//----------------------------------------------------------------------------
/****************************************************************************/
/*	��ԃt���O(SU101)														*/
/****************************************************************************/
#define		SQUARE_SU101_STATUS_FLG_KOUKAKU		0x01			// ���m
#define		SQUARE_SU101_STATUS_FLG_RENZOKU		0x02			// �A��
#define		SQUARE_SU101_STATUS_FLG_MADOWAKU	0x04			// ���g
#define		SQUARE_SU101_STATUS_FLG_SENSOR1		0x08			// �Z���T�[�P
#define		SQUARE_SU101_STATUS_FLG_SENSOR2		0x10			// �Z���T�[�Q
#define		SQUARE_SU101_STATUS_FLG_MOCHIDAMA	0x20			// ����
#define		SQUARE_SU101_STATUS_FLG_JITAN		0x40			// ���Z
#define		SQUARE_SU101_STATUS_FLG_REG			0x80			// �q�d�f

//----------------------------------------------------------------------------
/****************************************************************************/
/*	��ʋ敪(SU102)															*/
/****************************************************************************/
// �V�Z��n
#define		SQUARE_SU102_KIND_YUGI						  1			// �V�Z��
#define		SQUARE_SU102_KIND_CALL_BUTTON				  2			// �ďo�{�^��

// �݋@�n
#define		SQUARE_SU102_KIND_SAND						 11			// �T���h
#define		SQUARE_SU102_KIND_KINKO						 12			// ����
#define		SQUARE_SU102_KIND_GAIKASHIKI				 13			// �O�݋@
#define		SQUARE_SU102_KIND_CARD_GAIKASHIKI			 14			// �J�[�h�O�݋@
#define		SQUARE_SU102_KIND_CARD_HAKKENKI				 15			// �J�[�h�����@
#define		SQUARE_SU102_KIND_HIKIOTOSHIKI				 16			// ���ʈ����@
#define		SQUARE_SU102_KIND_HAKKEN_PREMIUM			 17			// �����v���~�A��
#define		SQUARE_SU102_KIND_RYOGAEKI					 18			// ���֋@
#define		SQUARE_SU102_KIND_DAI_REPLAYKI				 19			// �e��ăv���C�@
#define		SQUARE_SU102_KIND_CARD_SEISANKI				 20			// �J�[�h���Z�@
#define		SQUARE_SU102_KIND_HAKKEN_KINKO				 21			// ��������
#define		SQUARE_SU102_KIND_DAI_CARD_HAKKENKI			 22			// �e��J�[�h�����@

// �i�i�@��n
#define		SQUARE_SU102_KIND_KEIHIN_KEISU				 41			// �i�i�v���@
#define		SQUARE_SU102_KIND_KEIHIN_RESET				 42			// �i�i���Z�b�g
#define		SQUARE_SU102_KIND_KEIHIN_HENKYAKU			 43			// �i�i�ԋp�@
#define		SQUARE_SU102_KIND_KEIHIN_DAI_KEISU			 44			// �e��i�i�v���@

// �O���@��n
#define		SQUARE_SU102_KIND_SIMA_LAMP					101			// �������v
#define		SQUARE_SU102_KIND_LIFT_TANK_EMPTY			102			// ���t�g�_���i�^���N��j
#define		SQUARE_SU102_KIND_LIFT_TANK_FILL			103			// ���t�g�_���i�^���N���j
#define		SQUARE_SU102_KIND_SAND_TROUBLE				104			// �T���h�_���i�g���u���j
#define		SQUARE_SU102_KIND_SAND_STATUS				105			// �T���h�_���i��ԁj
#define		SQUARE_SU102_KIND_KINKO_TROUBLE				106			// ���ɓ_���i�g���u���j
#define		SQUARE_SU102_KIND_KINKO_STATUS				107			// ���ɓ_���i��ԁj
#define		SQUARE_SU102_KIND_GAIKASHIKI_TROUBLE		108			// �O�݋@�_���i�g���u���j
#define		SQUARE_SU102_KIND_GAIKASHIKI_STATUS			109			// �O�݋@�_���i��ԁj
#define		SQUARE_SU102_KIND_HAKKENKI_TROUBLE			110			// �J�[�h�����@�_���i�g���u���j
#define		SQUARE_SU102_KIND_HAKKENKI_STATUS			111			// �J�[�h�����@�_���i��ԁj
#define		SQUARE_SU102_KIND_HIKIOTOSHIKI_TROUBLE		112			// ���ʈ����@�_���i�g���u���j
#define		SQUARE_SU102_KIND_HIKIOTOSHIKI_STATUS		113			// ���ʈ����@�_���i��ԁj
#define		SQUARE_SU102_KIND_KEIHIN_KEISUKI_TROUBLE	114			// �i�i�v���@�_���i�g���u���j
#define		SQUARE_SU102_KIND_KEIHIN_KEISUKI_STATUS		115			// �i�i�v���@�_���i��ԁj
#define		SQUARE_SU102_KIND_KEIHIN_HENKYAKUKI_TROUBLE	116			// �i�i�ԋp�@�_���i�g���u���j
#define		SQUARE_SU102_KIND_KEIHIN_HENKYAKUKI_STATUS	117			// �i�i�ԋp�@�_���i��ԁj
#define		SQUARE_SU102_KIND_ETC						118			// ���̑��_��

//----------------------------------------------------------------------------
/****************************************************************************/
/*	����(SU102)																*/
/****************************************************************************/
#define		SQUARE_SU102_EVENT_NORMAL_UCHIDOME_AUTO		  1		// �����Ŏ~����				����n(���)
#define		SQUARE_SU102_EVENT_NORMAL_UCHIDOME_MANUAL	  2		// �蓮�Ŏ~����				����n(���)
#define		SQUARE_SU102_EVENT_NORMAL_UCHIDOME_NOTIFY	  3		// �Ŏ~�\���ʒm				����n(���)
#define		SQUARE_SU102_EVENT_NORMAL_UCHIDOME_CANCEL	  4		// �Ŏ~�\�����				����n(���)
#define		SQUARE_SU102_EVENT_NORMA_TOKUSYO_START		  5		// ���܌��o�ʒm				����n(����)
#define		SQUARE_SU102_EVENT_NORMAL_TOKUSYO_END		  6		// ���܏I���ʒm				����n(����)
#define		SQUARE_SU102_EVENT_TOKUSYO_FIRST_HIT_START	  7		// �����茟�o�ʒm			����n(����)
#define		SQUARE_SU102_EVENT_TOKUSYO_FIRST_HIT_END	  8		// ������I���ʒm			����n(����)
#define		SQUARE_SU102_EVENT_ERROR_NOTICE_START		  9		// ���ӑ䌟�o�ʒm			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_NOTICE_END			 10		// ���ӑ�I���ʒm			�ُ�n
#define		SQUARE_SU102_EVENT_NORMAL_AUTO_CLEAR		 11		// �����N���A�J���ʒm		����n(���)
#define		SQUARE_SU102_EVENT_NORMAL_MANUAL_CLEAR		 12		// �蓮�N���A�J���ʒm		����n(���)
#define		SQUARE_SU102_EVENT_NORMAL_LEAVE_SET			 13		// ���u��Z�b�g�ʒm			����n(���)
#define		SQUARE_SU102_EVENT_NORMAL_LEAVE_PASSED		 14		// ���u�䎞�Ԓʉߒʒm		����n(���)
#define		SQUARE_SU102_EVENT_ERROR_EMPTY				 15		// ��M�ُ픭���ʒm			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_EMPTY_RECOVERY		 16		// ��M�ُ한�A�ʒm			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_OUT				 17		// �A�E�g�ُ�ʒm			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_OUT_RECOVERY		 18		// �A�E�g�ُ����			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_SAFE				 19		// �Z�[�t�ُ�ʒm			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_SAFE_RECOVERY		 20		// �Z�[�t�ُ����			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_WINDOW_OPEN		 21		// ���g�I�[�v��				�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_WINDOW_CLOSE		 22		// ���g�N���[�Y				�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_FRAME_OPEN			 23		// �ؘg�I�[�v��				�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_FRAME_CLOSE		 24		// �ؘg�N���[�Y				�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_COVER_OPEN			 25		// ���I�[�v��				�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_COVER_CLOSE		 26		// ���N���[�Y				�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_SENSOR1_DETECT		 27		// �Z���T�[�P���o			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_SENSOR1_CLEAR		 28		// �Z���T�[�P����			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_SENSOR2_DETECT		 29		// �Z���T�[�Q���o			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_SENSOR2_CLEAR		 30		// �Z���T�[�Q����			�ُ�n
#define		SQUARE_SU102_EVENT_NORMAL_LEAVE_CLEAR		 31		// ���u������ʒm			����n(���)
#define		SQUARE_SU102_EVENT_ERROR_WINDOW_TOKUSYO		 32		// ���g���ܔ���				�ُ�n
#define		SQUARE_SU102_EVENT_NORMAL_ADD_BALL			 33		// �ʐ��ǉ��ʒm				����n(���)
#define		SQUARE_SU102_EVENT_NORMAL_ADD_BALL_OPEN		 34		// �ʐ��ǉ��J���ʒm			����n(���)
#define		SQUARE_SU102_EVENT_NORMAL_MODIFY_BALL		 35		// �����ʏC���ʒm			����n(���)
#define		SQUARE_SU102_EVENT_NORMAL_MODIFY_BALL_OPEN	 36		// �����ʏC���J���ʒm		����n(���)
#define		SQUARE_SU102_EVENT_ERROR_KASHIKI_BREAKE_LINE 37		// �݋@�f�����o�ʒm			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_KASHIKI_RECOVERY	 38		// �݋@�f�����A�ʒm			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_KEIHIN_BREAK_LINE	 39		// �i�i�f�����o�ʒm			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_KEIHIN_RECOVERY	 40		// �i�i�f�����A�ʒm			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_KINKO_ERROR		 41		// ���ɃT���h�덷�ُ�		�ُ�n
#define		SQUARE_SU102_EVENT_NORMAL_DAI_CALL_START	 42		// ��ďo���o				����n(���)
#define		SQUARE_SU102_EVENT_NORMAL_DAI_CALL_END		 43		// ��ďo�I��				����n(���)
#define		SQUARE_SU102_EVENT_ERROR_KASHIKI_DATA_ERROR	 44		// �݋@�f�[�^�ُ�ʒm		�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_KEIHIN_DATA_ERROR	 45		// �i�i�f�[�^�ُ�ʒm		�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_OUTSIDE			 46		// �O���ُ픭���ʒm			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_OUTSIDE_RECOVERY	 47		// �O���ُ한�A�ʒm			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_TOKUSYO_START_CNT	 48		// ���܎n�����ُ팟�o		�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_TOKUSYU_CONTINUE_A	 49		// ���ܘA���ُ�`���o		�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_TOKUSYU_CONTINUE_B	 50		// ���ܘA���ُ�a���o		�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_TOKUSYU_DEDAMA_A	 51		// ���܏o�ʈُ�`���o		�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_TOKUSYO_DEDAMA_B	 52		// ���܏o�ʈُ�a���o		�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_START_CNT_A		 53		// �n�����ُ�`���o			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_START_CNT_B		 54		// �n�����ُ�a���o			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_IRREGULAR_PLAY_A	 55		// �ϑ��v���[�`���o			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_IRREGULAR_PLAY_B	 56		// �ϑ��v���[�a���o			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_NORMAL_SAFE		 57		// �ʏ�Z�[�t�ُ팟�o		�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_TOKUSYO_OUT		 58		// ���܃A�E�g�ُ팟�o		�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_NORMAL_DEDAMA		 59		// �ʏ�o�ʈُ팟�o			�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_HIGH_DEDAMA		 60		// ���m�����o�ʗ��ُ팟�o	�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_OP1_DEDAMA			 61		// �n�o�P���o�ʗ��ُ팟�o	�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_TOKUSYO			 62		// ���܈ُ�					�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_HIGH				 63		// ���m���ُ�				�ُ�n
#define		SQUARE_SU102_EVENT_ERROR_DEDAMA				 64		// �o�ʈُ�					�ُ�n


//----------------------------------------------------------------------------
/****************************************************************************/
/*	�ʐM�G���[�����														*/
/****************************************************************************/
#define		SQUARE_COMM_COMM_ERROR_COUNT_MAX			3


#endif
//----------------------------------------------------------------------------
/*
 * $Log: /GNw3.2.0.x/include/SquareCommDef.h $
 * 
 * 9     09/01/19 11:10 Nakatani
 * T2X004967_PBANKV�Ȃ��Ή�
 */
//----------------------------------------------------------------------------
