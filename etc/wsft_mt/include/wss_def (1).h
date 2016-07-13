/* WAIS START */
//----------------------------------------------
//�T�v
//	�v�h�����실�ʊe���`
//
//�쐬��
//	Tokai ��
//
//���l
//	�v��������e���`��錾����
//----------------------------------------------

#if !defined(_WSS_DEF__H)
#define _WSS_DEF__H

//-------------------------------------------------
//	�E�B���h�E�^�C�g����(FindWindow()�Ŏw�肷��Ȃ܂��j
//-------------------------------------------------
#define	WTL_APCTRL		"���� - WssAppCtrl"				// �A�v���P�[�V�����Ǘ��v���Z�X
#define	WTL_COM			"���� - WssCom"					// �ʐM�Ǘ��v���Z�X
#define	WTL_SYUKEI		"Syukei"						// �W�v�v���Z�X
#define	WTL_SYSSETTEI	"�V�X�e���ݒ胁�j���["			// �V�X�e���ݒ�v���Z�X
#define	WTL_GYOSETTEI	"�Ɩ��ݒ�g�b�v���j���["		// �Ɩ��ݒ�v���Z�X
#define	WTL_UNYOU		"Unyou"							// �^�p��ʃv���Z�X
#define WTL_COMPROC		"ComProc"						// ComProc�v���Z�X	(MsgView.ini��[System]ComProcTitle�ł��̒ʂ�ݒ肵�ĉ�����)
#define WTL_WINKT		"Win����[��"					// WinKT�̃^�C�g��
#define	WTL_TNIO		"���� - Tnio"					// �s�m�h�n�v���Z�X
#define	WTL_UPS			"���� - Ups"					// �t�o�r�v���Z�X
#define WTL_TOUINJ		"���� - Touinji"				// ���߈󎚃v���Z�X
#define WTL_SWOFF		"�J�MSWOFF���"					// �J�M�r�v�n�e�e��ʃv���Z�X
#define WTL_NOP			"�󔒉��"						// �󔒉�ʃv���Z�X
#define	WTL_START		"�������"					// �����オ���ʃv���Z�X
#define	WTL_INFO		"�C���t�H���[�V�������"		// �C���t�H���[�V������ʃv���Z�X
#define	WTL_DEMIREAL	"demireal"						// demireal�v���Z�X
#define WTL_MPCPROC		"mpcproc"						// mpcproc�v���Z�X
#define WTL_MAILPROC	"���[�����m�点"				// mailproc�v���Z�X	
#define WTL_GFTP		"GFTP"							// GFTP�v���Z�X	
#define WTL_FANPROC		"fanproc"						// fanproc�v���Z�X	
// ---> 2006/02/10 Y.Okajima
#define	WTL_DLG			"WssDialog Notification Window"	// �_�C�A���O�v���Z�X�E�B���h�E�^�C�g��
// <--- 2006/02/10 Y.Okajima
#define	WTL_SIMAKANRI	"SimaKanri"						// ���Ǘ��v���Z�X�E�B���h�E�^�C�g��
#define	WTL_HALLMAP		"HallMap"						// �z�[���}�b�v���C�A�E�^�[�v���Z�X�E�B���h�E�^�C�g��
#define WTL_TCPCOMM		"SquareTcpComm Main Window"		// SquareTcpComm�v���Z�X�E�B���h�E�^�C�g��
#define	WTL_UDPCOMM		"SquareUdpComm Main Window"		// SquareUdpComm�v���Z�X�E�B���h�E�^�C�g��
#define WTL_WSSHUT		"Win���� �V���b�^�["

//------------------------------------------------------------
//	��ʃv���Z�XNO�i��ʐ؂�ւ��Ŏw�肷��m�n�j
//------------------------------------------------------------
#define GAMNO_UNYOU				1		// �^�p��ʂm�n
#define GAMNO_GYOUSET			2		// �Ɩ��ݒ��ʂm�n
#define GAMNO_SYSSET			3		// �V�X�e���ݒ��ʂm�n
#define GAMNO_SWOFF				4		// �d���n�e�e��ʂm�n
#define GAMNO_NOP				5		// �󔒉�ʂm�n
#define GAMNO_START				6		// �����オ���ʂm�n
#define GAMNO_CARIV				7		// �L�����u���[�V������ʂm�n
#define GAMNO_INFO				8		// �C���t�H�[���[�V������ʂm�n
#define GAMNO_WINKT				9		// Win����[���m�n
#define GAMNO_Mail			   10		// ���[�����m�点�_�C�A���O�m�n
#define GAMNO_SIMAKANRI		   11		// ���Ǘ��[���_�C�A���O�m�n

//------------------------------------------------------------
//	�v���Z�X�E�H�b�`�h�b�O�^�C���E�g�G���[���t���O
//------------------------------------------------------------
#define	WATCH_COM			0x0001		// �ʐM�v���Z�X
#define	WATCH_TNIO			0x0002		// �s�m�h�n�v���Z�X
#define	WATCH_SYUKEI		0x0004		// �W�v�v���Z�X
#define	WATCH_UPS			0x0008		// �t�o�r�v���Z�X

//-------------------------------------------------
//	���b�Z�[�W�C�x���g
//-------------------------------------------------

// TNIO�v���Z�X�|�|��TNIO�@RS�|232C����X���b�h���b�Z�[�W�C�x���g
#define	WM_RS232CEND	WM_USER + 1000		// RS�|232C����X���b�h�I��

// �A�v���P�[�V�����Ǘ��|�|����ʃv���Z�X���b�Z�[�W�C�x���g
#define	WM_APC_CHANGE	WM_USER + 1001		// ��ʐ؂�ւ�
#define	WM_APC_DIALOG	WM_USER + 1002		// �_�C�A���O�\���i���[�_���w��j

// �A�v���P�[�V�����Ǘ��|�|���v�����^������v�����b�Z�[�W�C�x���g
#define	WM_APC_PRFEED	WM_USER + 1003		// �v�����^������
// �A�v���P�[�V�����Ǘ��|�|���ʐM�v���Z�X���b�Z�[�W�C�x���g
#define	WM_APC_ONSEISTOP	WM_USER + 1004		// �����X�g�b�v�v��


// �A�v���P�[�V�����Ǘ��|�|���S�v���Z�X���b�Z�[�W�C�x���g
#define	WM_APC_SYURYOU	WM_USER + 1009		// �I��

//  �ʐM�v���Z�X�|�|���W�v�������b�Z�[�W�C�x���g
#define WM_COM_START	WM_USER + 1010		// �W�v�X�^�[�g
#define	WM_COM_KIOKU	WM_USER + 1011		// �L��
#define	WM_COM_RESET	WM_USER + 1012		// �ݒ�X�V
#define	WM_COM_CLEAR	WM_USER + 1013		// �N���A
//  �ʐM�v���Z�X�|�|����ʃv���Z�X���b�Z�[�W�C�x���g
#define	WM_COM_DIALOG	WM_USER + 1014		// �_�C�A���O�\���i���[�_���w��j
//  �ʐM�v���Z�X�|�|���󎚗v�����b�Z�[�W�C�x���g
#define	WM_COM_PRINT	WM_USER + 1015		// �󎚗v��
//  �ʐM�v���Z�X�|�|���A�v���P�[�V�����Ǘ��v���Z�X���b�Z�[�W�C�x���g
#define	WM_COM_NOTKIOKU	WM_USER + 1016		// ���L���ʒm
// WssCom �� WssAppCtrl ��MPC��POS�f�[�^�捞�Y��x����ON/OFF���b�Z�[�W
// wParam�͏��0
// lParam == 0 �Ȃ� OFF lParam == 1 �Ȃ�ON
#define WM_COM_MPCPOSWARN	(WM_USER + 1017)
#define WM_COM_TOKUREKI	WM_USER + 1018		// ���ܗ����t�@�C���擾		T2X002549	2005.07.20	nao
#define WM_COM_MMP_UPDATE	(WM_USER + 1019)	// �������}�b�v�h�t�@�C�������X�V

//  �W�v�����|�|���A�v���P�[�V�����Ǘ��v���Z�X & �ʐM�v���Z�X
#define	WM_SYU_KIOKUEND		WM_USER + 1020		// �L������
//  �W�v�����|�|���ʐM�v���Z�X
#define	WM_SYU_SYUKEIEND	WM_USER + 1021		// �W�v����
#define	WM_SYU_CLEAREND		WM_USER + 1022		// �N���A����

// �s�m�h�n�|�|���A�v���P�[�V�����Ǘ�
#define	WM_TNIO_KAGI	WM_USER + 1030		// �J�M�ω��ʒm
// �s�m�h�n�|�|����ʃv���Z�X�C�A�v���P�[�V�����v���Z�X
#define	WM_TNIO_CARD	WM_USER + 1031		// �J�[�h�h�m�^�n�t�s�ʒm

// �t�o�r�|�|���A�v���P�[�V�����Ǘ�
#define WM_UPS_TEIDEN_ON	WM_USER + 1040		// ��d����
#define WM_UPS_TEIDEN_OFF	WM_USER + 1041		// ��d��
#define WM_UPS_SYSDOWN		WM_USER + 1042		// �V�X�e���V���b�g�_�E������
#define WM_UPS_BATERY_OLD	WM_USER + 1043		// �o�b�e���[�V�����ʒm

// �V�X�e���ݒ��ʁ|�|���ʐM�v���Z�X
#define WM_SYS_SET		WM_USER + 1050		 	// �ݒ�X�V�ʒm
#define WM_SYS_OPE		WM_USER + 1055			// �^�p����ʒm�i�ݒ�N���A�A�L���N���Aetc)	2002.06.21 �C�� by Koketu

// �Ɩ��ݒ��ʁ|�|���ʐM�v���Z�X
#define WM_GYOU_SET		WM_USER + 1060			// �ݒ�X�V�ʒm
// �Ɩ��ݒ��ʁ|�|���A�v���P�[�V�����Ǘ�
#define WM_GYOU_START	WM_USER + 1061			// �N�������ʒm
#define WM_GYOU_GAMEN	WM_USER + 1062			// ��ʐ؂�ւ��v��
#define WM_GYOU_SEIGOU	WM_USER + 1063		 	// �������G���[���������ʒm
#define WM_GYOU_ERR		WM_USER + 1064		 	// ���̑��G���[�����ʒm

// �V�X�e���ݒ��ʁ|�|���A�v���P�[�V�����Ǘ�
#define WM_SYS_GAMEN		WM_USER + 1070		 // ��ʐ؂�ւ��v��
#define WM_SYS_START		WM_USER + 1071		 // �N�������ʒm
#define WM_SYS_SEIGOU		WM_USER + 1072	 	// �������G���[���������ʒm
#define WM_SYS_ERR			WM_USER + 1073	 	// ���̑��G���[�����ʒm
#define WM_SYS_ARAEND		WM_USER + 1074	 	// �\�ߐݒ�I���ʒm

// �^�p��ʁ|�|���A�v���P�[�V�����Ǘ�
#define WM_UNYOU_START		WM_USER + 1080		 // �N�������ʒm
#define WM_WINKT_START		WM_USER + 1085		 // �N�������ʒm(WinKT)
#define WM_SIMAKANRI_START	WM_USER + 1086		 // �N�������ʒm(���Ǘ��[��)

// �J�M�r�v�n�e�e��ʁ|�|���A�v���P�[�V�����Ǘ�
#define WM_SWOFF_YES		WM_USER + 1090		 // �u�͂��v�u�������v�Łu�͂��v���I�����ꂽ�ʒm
#define WM_SWOFF_ERR		WM_USER + 1091		 //  �G���[�����ʒm

// �L�[�{�[�h�t�b�N�ʒmDLL
#define WM_KEYHOOK_KEY	WM_USER + 1100

// ���߈� -->���b�Z�[�W �n���h���󎚗v��
#define WM_TOU_TOUINJI WM_USER + 1200
// �����オ���ʁ|�|���A�v���P�[�V�����Ǘ�
#define WM_TATI_ERR		WM_USER + 1300			 //  �G���[�����ʒm
// �󔒉�ʁ|�|���A�v���P�[�V�����Ǘ�
#define WM_NOP_ERR		WM_USER + 1310			 //  �G���[�����ʒm
// �C���t�H���[�V������ʁ|�|���A�v���P�[�V�����Ǘ�
#define WM_INFO_ERR		WM_USER + 1320			 //  �G���[�����ʒm

#define WM_USER_TEIJIRELOAD	(WM_USER+1321)	// �莞�W�v�̃f�B�X�N����ēǍ���

#define WM_USER_PRE_PT_SEIRI (WM_USER + 1322)  //�|�C���g����������	add 2004.11.01 hashimoto
#define WM_ENSHUTSU_KOCHO_PREVIEW		 (WM_USER + 1330)	//	�D�����E�@��v���r���[�C�x���g�N��
#define WM_ENSHUTSU_KOCHO_ENDPREVIEW	 (WM_USER + 1331)	//	�D�����E�@��v���r���[��~�C�x���g�N��
#define WM_ENSHUTSU_MANUAL_DOEVENT		 (WM_USER + 1332)	//	�蓮�C�x���g�N��
#define WM_ENSHUTSU_AUTO_DOEVENT	  	 (WM_USER + 1333)	//	�����C�x���g�N��
#define WM_ENSHUTSU_COMMON_KILL			 (WM_USER + 1334)	//	�S��~
#define WM_GYOU_LAMP_PREVIEW             (WM_USER + 1335)	//  �z�[�����o(�䃉���v�v���r���[)
#define WM_ENSHUTSU_MONITOR_CLEARLOG	 (WM_USER + 1336)	//	�D�����E�@�탂�j�^�̃��O

#define WSSM_GETINJISW	(WM_APP + 0)		 // ������SW�̏�Ԏ擾		//yumi
#define WSSM_SETINJISW	(WM_APP + 1)		 // ������SW�̏�Ԑݒ�		//yumi
#define WM_APP_SWKHOOK	(WM_APP + 3)		//yumi
#define WSSM_GETREMOTESTATE	(WM_APP + 4)	// �����[�g�[���̒ʐM��Ԏ擾	2003.9.29 nao
#define WSSM_SETREMOTESTATE	(WM_APP + 5)	// �����[�g�[���̒ʐM��Ԑݒ�	2003.9.29 nao
#define WM_KMWATCH_KEYBOARD	(WM_APP + 6)	// �L�[�{�[�h����ʒm	2003.10.15 nao
#define WM_KMWATCH_MOUSE	(WM_APP + 7)	// �}�E�X����ʒm		2003.10.15 nao
#define WSSM_AUT_REMOTECHG	(WM_APP + 8)	// �����ؒf�^�����ڑ�		2003.10.20 nao
#define WM_LEFTBTN_DOWN		(WM_APP + 9)	// �}�E�X���{�^���N���b�N 2004.03.22 nakatani
#define WM_SHUTDOWN			(WM_APP + 10)	// �V���b�g�_�E�� or �ċN���ʒm 2004.03.22 nakatani
#define WM_MAILLIGHTON		(WM_APP + 11)	// �p�g���C�g�_���ʒm 2005.07.31 Fukuyama

#define		WM_USER_RECV_931A			1100
#define		WM_USER_SEND_931A_ACK		1101
#define		WM_USER_SEND_9315			1102
#define		WM_USER_RECV_9315_ACK		1103
#define		WM_USER_NOTICE_LIGTH_OFF	1104

// ---> 2006/02/10 Y.Okajima P-BANK�ʐM�G���[�ʒm
#define	WM_PBANK_COMM_ERROR			(WM_USER + 1400)	// P-BANK�ʐM�G���[�ʒm
#define	WM_PBANK_GETDATA_ERROR		(WM_USER + 1401)	// P-BANK�m��f�[�^�b�q�b�G���[�ʒm
#define	WM_PBANK_GETPASTDATA_ERROR	(WM_USER + 1402)	// P-BANK�ߋ��f�[�^�b�q�b�G���[�ʒm
// <--- 2006/02/10 Y.Okajima P-BANK�ʐM�G���[�ʒm
#define	WM_PBANK_DATAMODE_CHANGE	(WM_USER + 1403)	// P-BANK�f�[�^���[�h�ύX�ʒm

//-------------------------------------------------
//	�L���ς݃t�@�C����
//-------------------------------------------------
#define FLN_KIOKUZUMI		"Kiokuzumi.dat"

//-------------------------------------------------
//	�c�Ɨ����t�@�C������(128 * 1024 + 1)
//-------------------------------------------------
#define EIREKI_SUU			131073
//-------------------------------------------------
//	MPC�c�Ɨ����t�@�C������
//-------------------------------------------------
#define EIREKI_MPC_SUU			1000000
//-------------------------------------------------
//	��ԗ����t�@�C������()
//-------------------------------------------------
#define YAKANREKI_SUU			1024

#define TOKUREKI_SUU		28670				// AS1��������̓��ܗ����t�@�C���̃��R�[�h��
//#define TOKUREKI_SUUx3		(TOKUREKI_SUU * 3)	// AS3�����̓��ܗ����t�@�C���̃��R�[�h��
#define TOKUREKI_SUUx3		(TOKUREKI_SUU * MAX_AS)	// AS3�����̓��ܗ����t�@�C���̃��R�[�h��

#define MOCHIREKI_SUU		3000
#define MOCHIREKI_SUUx3		(MOCHIREKI_SUU * MAX_AS)	// AS3�����̎��ʏƍ��@�����t�@�C���̃��R�[�h��

#define SOFTSW 2

#endif

