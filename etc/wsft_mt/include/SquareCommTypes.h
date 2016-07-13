/****************************************************************************/
/*			 �r�p�t�`�q�d�ʐM�p�\���̒�`	�w�b�_�[�t�@�C��				*/
/****************************************************************************/
/*	2006/06/30													Y.Okajima	*/
/****************************************************************************/
//----------------------------------------------------------------------------
/*
 *	$Id$
 */
//----------------------------------------------------------------------------
#if !defined( __SQUARECOMM_TYPES_H__ )
#define		__SQUARECOMM_TYPES_H__

//----------------------------------------------------------------------------
/****************************************************************************/
/*	�C���N���[�h�t�@�C��													*/
/****************************************************************************/
#if !defined(_MFC_VER)
#include	<windows.h>
#endif
#include	"SquareCommDef.h"
//----------------------------------------------------------------------------
#pragma pack(push, 1)

//----------------------------------------------------------------------------
/****************************************************************************/
/*	�w�b�_�[��																*/
/****************************************************************************/
struct square_header_t {
	char	command[5];										// �R�}���h
	char	date[8];										// ���t
	char	time[6];										// ����
	BYTE	srcNetAddress;									// ���M���l�b�g���[�N�ԍ�
	char	srcMachine[4];									// ���M���[������
	short	srcStation;										// ���M���[���ԍ�
	char	destMachine[4];									// ���M��[������
	short	destStation;									// ���M��[���ԍ�
	char	endMachine[4];									// ���[�[���@��
	BYTE	endStation;										// ���[�[���ԍ��i���C���ԍ��j
	BYTE	arcAddress;										// �A�[�N�l�b�g�A�h���X
	BYTE	portNo;											// �|�[�g�ԍ�
	BYTE	week;											// �j��
	long	length;											// �f�[�^��
	short	boardNo;										// ��ԍ�
	char	padding2[1];									// ���[�U�[�I�v�V����
	BYTE	key;											// �Í��L�[
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�s�l�O�O�P�d���i�v���j													*/
/****************************************************************************/
struct req_tm001_t {
	char	version[8];										// �o�[�W�����ԍ�
	char	date[8];										// �摜���t
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�O�O�P�d���i�v���j													*/
/****************************************************************************/
struct req_su001_t {
	short	machineNo;										// ���@
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�O�O�P�d���i�����j���@�ԍ��`�ڑ��@��䐔							*/
/****************************************************************************/
struct ans_su001_header_t {
	short	machineNo;										// ���@
	short	jlpmcnt;										// �ڑ��@��䐔
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�O�O�P�d���i�����j�A�[�N�l�b�g�A�h���X�`�[�����					*/
/****************************************************************************/
struct ans_su001_kiki_info_t {
	BYTE	arcAddress;										// �A�[�N�l�b�g�A�h���X
	char	filler1;										// �p�f�B���O
	short	boardLNo;										// �����ݒu��ԍ�
	short	boardRNo;										// �E���ݒu��ԍ�
	char	machine[4];										// �[�����
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�O�O�P�d���i�����j�ăv���C���o�P�ʁ`�ăv���C���Ïؔԍ�����			*/
/****************************************************************************/
struct ans_su001_bm_info_t {
	short	payout_unit;									// �ăv���C���o�P��
	short	payout_fee;										// �ăv���C�萔���P��
	BYTE	MB102_interval;									// MB102�d�����M�Ԋu
	BYTE	rpl_pass_chk;									// �ăv���C���Ïؔԍ�����
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�O�O�P�d���i�����j�X�܉^�p���o�[�W�����`							*/
/****************************************************************************/
struct ans_su001_data_t {
	short					shop_info_ver;					// �X�܉^�p���o�[�W�����ԍ�
	char					comp_code[4];					// �̎ЃR�[�h
	char					shop_code[5];					// �X�R�[�h
	BYTE					sandKind;						// �T���h���
	ans_su001_bm_info_t		bm_info[2];						// ��/�l���
	BYTE					discharge;						// �r�o�d���敪
	BYTE					boardDataType;					// ����d���敪
	short					boardInterval;					// ����d�����M�Ԋu
	short					boardShiftInterval;				// ����d�����@�ԑ��M�Ԋu
	BYTE					systemFlg;						// �V�X�e���t���O
	BYTE					alarmHitOK;						// �A���[���ʒm�i����n:���܁j
	BYTE					alarmNormalOK;					// �A���[���ʒm�i����n:��ʁj
	BYTE					alarmNG;						// �A���[���ʒm�i�ُ�n�j
	BYTE					pointDspFlg;					// �|�C���g�\���敪	
	BYTE					reserved[9];					// �\��̈�
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�O�O�P�d���i�����j													*/
/****************************************************************************/
struct ans_su001_t {
	ans_su001_header_t		header;							// �f�[�^�w�b�_
	ans_su001_kiki_info_t	kiki_info;						// ��
	ans_su001_data_t		data;							// �ݒ�f�[�^
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�O�O�T�d���i����j												*/
/****************************************************************************/
struct req_su005_data_dai_t {
	BYTE	kind;											// �敪
	char	padding1[1];									// �p�f�B���O�P
	short	boardNo;										// ��ԍ��i�V�X�e��ID�j
	short	hitHistoryCount;								// ���ܗ�������
	short	stopCount;										// �ł��~�߉�
	long	outCount;										// �A�E�g�݌v
	long	safeCount;										// �Z�[�t�݌v
	short	hitCount;										// ����/BIG�񐔗݌v
	short	highProvableCount;								// ���m��/REG�񐔗݌v
	short	startCount;										// �n�����݌v
	short	op1Count;										// OP1���݌v
	short	op2Count;										// OP2���݌v
	short	op3Count;										// OP3���݌v
	long	outCounthigh;									// ���m���݌v�A�E�g���݌v
	long	safeCounthigh;									// ���m���݌v�Z�[�t���݌v
	short	startCounthigh;									// ���m���݌v�n�����݌v
	short	op1Counthigh;									// ���m���݌vOP1���݌v
	short	op2Counthigh;									// ���m���݌vOP2���݌v
	short	op3Counthigh;									// ���m���݌vOP3���݌v
	long	outCounthit;									// ���ܒ��݌v�A�E�g���݌v
	long	safeCounthit;									// ���ܒ��݌v�Z�[�t���݌v
	short	startCounthit;									// ���ܒ��݌v�n�����݌v
	short	op1Counthit;									// ���ܒ��݌vOP1���݌v
	short	op2Counthit;									// ���ܒ��݌vOP2���݌v
	short	op3Counthit;									// ���ܒ��݌vOP3���݌v
	short	normalhitCount;									// �ʏ���܉񐔗݌v
	short	startCountafter;								// ���܌�n����
	long	outCountafter;									// ���܌�A�E�g
	long	safeCountafter;									// ���܌�Z�[�t
	short	windowCount;									// ���g�I�[�v���񐔗݌v
	short	windowHitCount;									// ���g���܉񐔗݌v
	short	injustice1Count;								// �s���P��
	short	injustice2Count;								// �s���Q��
	char	padding3[2];									// �p�f�B���O�R
	short	replayBall;										// �e��ăv���C��
	long	ballCount;										// �e��v����
	long	cashSell;										// ��������
	long	cardSell;										// �J�[�h����
	long	cardSaleAmt;									// �J�[�h�������z
	unsigned long	cnd;									// �V�Z���ԏ��
	BYTE	palseFlg;										// �i�X�J�ăv���C�t���O
	char	padding4[1];									// �p�f�B���O�S
	short	dlOutNormal;									// �f�B���C���A�E�g�i�ʏ�j
	short	dlOutHigh;										// �f�B���C���A�E�g�i���m�j
	short	dlOutJitan;										// �f�B���C���A�E�g�i���Z�j
	short	dlSafeNormal;									// �f�B���C���Z�[�t�i�ʏ�j
	short	dlSafeHigh;										// �f�B���C���Z�[�t�i���m�j
	short	dlSafeJitan;									// �f�B���C���Z�[�t�i���Z�j
	short	dlStartNormal;									// �f�B���C���n�����i�ʏ�j
	short	dlStartHigh;									// �f�B���C���n�����i���m�j
	short	dlStartJitan;									// �f�B���C���n�����i���Z�j
	short	dlOp1Normal;									// �f�B���C��OP1�񐔁i�ʏ�j
	short	dlOp1High;										// �f�B���C��OP1�񐔁i���m�j
	short	dlOp1Jitan;										// �f�B���C��OP1�񐔁i���Z�j
	short	dlOp2Normal;									// �f�B���C��OP2�񐔁i�ʏ�j
	short	dlOp2High;										// �f�B���C��OP2�񐔁i���m�j
	short	dlOp2Jitan;										// �f�B���C��OP2�񐔁i���Z�j
	short	dlOp3Normal;									// �f�B���C��OP3�񐔁i�ʏ�j
	short	dlOp3High;										// �f�B���C��OP3�񐔁i���m�j
	short	dlOp3Jitan;										// �f�B���C��OP3�񐔁i���Z�j
	char	padding5[2];									// �p�f�B���O�T
	long	outCountJitan;									// ���Z/AT/CT���݌v�A�E�g���݌v
	long	safeCountJitan;									// ���Z/AT/CT���݌v�Z�[�t���݌v
	short	startCountJitan;								// ���Z/AT/CT���݌v�n�����݌v
	short	op1CountJitan;									// ���Z/AT/CT���݌vOP1���݌v
	short	op2CountJitan;									// ���Z/AT/CT���݌vOP2���݌v
	short	op3CountJitan;									// ���Z/AT/CT���݌vOP3���݌v
	short	jitanCount;										// ���Z/AT/CT��
	short	maxMY;											// �ő�l��
	short	minMY;											// �ő呹��
	char	padding6[2];									// �p�f�B���O�U
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�O�O�T�d���i�v���@���j											*/
/****************************************************************************/
struct req_su005_data_keisuki_t {
	BYTE	kind;											// �敪
	char	padding1[1];									// �p�f�B���O�P
	short	boardNo;										// ��ԍ��i�V�X�e��ID�j
	long	ballCount;										// �v���l
	BYTE	negaCond;										// �f��
	char	padding2[159];									// �p�f�B���O�Q
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�O�O�T�d���i���ɏ��j												*/
/****************************************************************************/
struct req_su005_data_kinko_t {
	BYTE	kind;											// �敪
	char	padding1[1];									// �p�f�B���O�P
	short	boardNo;										// ��ԍ��i�V�X�e��ID�j
	long	n100Count;										// 100�~�J�E���g
	long	n500Count;										// 500�~�J�E���g
	long	n1000Count;										// 1000�~�J�E���g
	long	n2000Count;										// 2000�~�J�E���g
	long	n5000Count;										// 5000�~�J�E���g
	long	n10000Count;									// 10000�~�J�E���g
	long	safeboxSell;									// ���ɔ�����z
	BYTE	negaCond;										// �f��
	char	padding2[135];									// �p�f�B���O�Q
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�O�O�T�d���i���֋@���j											*/
/****************************************************************************/
struct req_su005_data_ryogaeki_t {
	BYTE	kind;											// �敪
	char	padding1[1];									// �p�f�B���O�P
	short	boardNo;										// ��ԍ��i�V�X�e��ID�j
	long	n10000inCount;									// 10000�~�����݌v
	long	n5000inCount;									// 5000�~�����݌v
	long	n1000inCount;									// 1000�~�����݌v
	long	n500inCount;									// 500�~�����݌v
	long	n1000outCount;									// 1000�~�o���݌v
	long	n500outCount;									// 500�~�o���݌v
	long	n100outCount;									// 100�~�o���݌v
	BYTE	negaCond;										// �f��
	char	padding2[135];									// �p�f�B���O�Q
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�O�O�T�d���i�����@���j											*/
/****************************************************************************/
struct req_su005_data_kenbaiki_t {
	BYTE	kind;											// �敪
	char	padding1[1];									// �p�f�B���O�P
	short	boardNo;										// ��ԍ��i�V�X�e��ID�j
	long	premiumCount;									// �v���~�A���ʑ݉񐔗݌v
	long	premiumbCount;									// �v���~�A���ʑ݌��݌v
	long	colum1Sell;										// �R�����P���㖇���݌v
	long	colum2Sell;										// �R�����Q���㖇���݌v
	long	colum3Sell;										// �R�����R���㖇���݌v
	long	colum4Sell;										// �R�����S���㖇���݌v
	long	colum5Sell;										// �R�����T���㖇���݌v
	long	SellCount;										// ���㍇�v����
	long	SellAmount;										// ���㍇�v���z
	BYTE	negaCond;										// �f��
	char	padding2[127];									// �p�f�B���O�Q
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�O�O�T�d���i�O�݋@���j											*/
/****************************************************************************/
struct req_su005_data_gaikashiki_t {
	BYTE	kind;											// �敪
	char	padding1[1];									// �p�f�B���O�P
	short	boardNo;										// ��ԍ��i�V�X�e��ID�j
	long	cashSell;										// ��������
	long	cardSell;										// �J�[�h����
	BYTE	negaCond;										// �f��
	char	padding2[155];									// �p�f�B���O�Q
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�O�O�T�d���i���Z�@���j											*/
/****************************************************************************/
struct req_su005_data_seisanki_t {
	BYTE	kind;											// �敪
	char	padding1[1];									// �p�f�B���O�P
	short	boardNo;										// ��ԍ��i�V�X�e��ID�j
	long	adjustMount;									// ���Z���z
	long	n100Count;										// 100�~�J�E���g
	long	n500Count;										// 500�~�J�E���g
	long	n1000Count;										// 1000�~�J�E���g
	BYTE	negaCond;										// �f��
	char	padding2[147];									// �p�f�B���O�Q
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�O�O�T�d���i�������ɏ��j											*/
/****************************************************************************/
struct req_su005_data_hakkenkinko_t {
	BYTE	kind;											// �敪
	char	padding1[1];									// �p�f�B���O�P
	short	boardNo;										// ��ԍ��i�V�X�e��ID�j
	long	n100Count;										// 100�~�J�E���g
	long	n500Count;										// 500�~�J�E���g
	long	n1000Count;										// 1000�~�J�E���g
	long	n2000Count;										// 2000�~�J�E���g
	long	n5000Count;										// 5000�~�J�E���g
	long	n10000Count;									// 10000�~�J�E���g
	long	safeboxSell;									// ���ɔ�����z
	BYTE	negaCond;										// �f��
	char	padding2[135];									// �p�f�B���O�Q
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�O�O�T�d���i�v���j�f�[�^											*/
/****************************************************************************/
union req_su005_data_t {
	struct {
		BYTE	kind;										// �敪
		char	padding1[1];								// �p�f�B���O�P
		short	boardNo;									// ��ԍ��i�V�X�e��ID�j
	}	common;
	req_su005_data_dai_t			dai;					// ����
	req_su005_data_keisuki_t		keisuki;				// �v���@���
	req_su005_data_kinko_t			kinko;					// ���ɏ��
	req_su005_data_ryogaeki_t		ryogaeki;				// ���֋@���
	req_su005_data_kenbaiki_t		kenbaiki;				// �����@���
	req_su005_data_gaikashiki_t		gaikashiki;			// �O�݋@���
	req_su005_data_seisanki_t		seisanki;				// ���Z�@���
	req_su005_data_hakkenkinko_t	hakken_kinko;			// �������ɏ��
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�O�O�T�d���i�v���j													*/
/****************************************************************************/
struct req_su005_t {
	long				number;										// ����
	req_su005_data_t	data;
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�P�O�P�d���i�v���j�f�[�^											*/
/****************************************************************************/
struct req_su101_data_t {
	short	boardNo;										// ��ԍ�
	short	rirekiCnt;										// ���W����ԍ��iFrom�j
};											
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�P�O�P�d���i�v���j													*/
/****************************************************************************/
struct req_su101_t {
	long				dataCnt;							// �����i�䐔�j
	req_su101_data_t	data[1];							// �f�[�^�i�ρj
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�P�O�P�d���i�����j�f�[�^											*/
/****************************************************************************/
struct ans_su101_data_t {
	short	boardNo;										// ��ԍ�
	short	rirekiNo;										// ����ԍ�
	byte	hitCount;										// ���܉�
	byte	status;											// ��ԃt���O
	byte	hour;											// ������
	byte	minute;											// ������
	long	outCountNotHitD;								// ���܊O�A�E�g�i�f�B���C����j
	long	outCountNotHit;									// ���܊O�A�E�g�i�f�B���C�Ȃ��j
	long	safeCountNotHitD;								// ���܊O�Z�[�t�i�f�B���C����j
	long	safeCountNotHit;								// ���܊O�Z�[�t�i�f�B���C�Ȃ��j
	short	startCountNotHitD;								// ���܊O�n�����i�f�B���C����j
	short	startCountNotHit;								// ���܊O�n�����i�f�B���C�Ȃ��j
	short	op1CountNotHitD;								// ���܊O�n�o�P�񐔁i�f�B���C����j
	short	op1CountNotHit;									// ���܊O�n�o�P�񐔁i�f�B���C�Ȃ��j
	short	op2CountNotHitD;								// ���܊O�n�o�Q�񐔁i�f�B���C����j
	short	op2CountNotHit;									// ���܊O�n�o�Q�񐔁i�f�B���C�Ȃ��j
	short	op3CountNotHitD;								// ���܊O�n�o�R�񐔁i�f�B���C����j
	short	op3CountNotHit;									// ���܊O�n�o�R�񐔁i�f�B���C�Ȃ��j
	long	outCountHitD;									// ���ܒ��A�E�g�i�f�B���C����j
	long	outCountHit;									// ���ܒ��A�E�g�i�f�B���C�Ȃ��j
	long	safeCountHitD;									// ���ܒ��Z�[�t�i�f�B���C����j
	long	safeCountHit;									// ���ܒ��Z�[�t�i�f�B���C�Ȃ��j
	short	startCountHitD;									// ���ܒ��n�����i�f�B���C����j
	short	startCountHit;									// ���ܒ��n�����i�f�B���C�Ȃ��j
	short	op1CountHitD;									// ���ܒ��n�o�P�񐔁i�f�B���C����j
	short	op1CountHit;									// ���ܒ��n�o�P�񐔁i�f�B���C�Ȃ��j
	short	op2CountHitD;									// ���ܒ��n�o�Q�񐔁i�f�B���C����j
	short	op2CountHit;									// ���ܒ��n�o�Q�񐔁i�f�B���C�Ȃ��j
	short	op3CountHitD;									// ���ܒ��n�o�R�񐔁i�f�B���C����j
	short	op3CountHit;									// ���ܒ��n�o�R�񐔁i�f�B���C�Ȃ��j
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�P�O�P�d���i�����j													*/
/****************************************************************************/
struct ans_su101_t {
	long				dataCnt;							// ����
	ans_su101_data_t	data[1];							// �f�[�^���i�ρj
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�t�P�O�Q�d���i�v���j													*/
/****************************************************************************/
struct req_su102_t {
	short	kind;											// ��ʋ敪
	short	boardNo;										// �@��ԍ�
	long	alarmDate;										// �A���[�����۔����N����
	long	alarmTime;										// �A���[�����۔��������b
	short	alarmNo;										// �A���[������No.
	short	moneyType;										// �������
	// ��ȉ��A���۔������_�̑����	
	long	outCount;										// �A�E�g�݌v
	long	safeCount;										// �Z�[�t�݌v
	short	starttCount;									// �n�����݌v
	char	padding1[2];									// �p�f�B���O�P
	long	outCountHitD;									// ���ܒ��A�E�g�i�f�B���C����j
	long	outCountHit;									// ���ܒ��A�E�g�i�f�B���C�Ȃ��j
	long	safeCountHitD;									// ���ܒ��Z�[�t�i�f�B���C����j
	long	safeCountHit;									// ���ܒ��Z�[�t�i�f�B���C�Ȃ��j
	short	startCountHitD;									// ���ܒ��n�����i�f�B���C����j
	short	startCountHit;									// ���ܒ��n�����i�f�B���C�Ȃ��j
	long	outCountHighD;									// ���m���A�E�g�i�f�B���C����j
	long	outCountHigh;									// ���m���A�E�g�i�f�B���C�Ȃ��j
	long	safeCountHighD;									// ���m���Z�[�t�i�f�B���C����j
	long	safeCountHigh;									// ���m���Z�[�t�i�f�B���C�Ȃ��j
	short	startCountHighD;								// ���m���n�����i�f�B���C����j
	short	startCountHigh;									// ���m���n�����i�f�B���C�Ȃ��j
	long	outCountJitanD;									// ���Z���A�E�g�i�f�B���C����j
	long	outCountJitan;									// ���Z���A�E�g�i�f�B���C�Ȃ��j
	long	safeCountJitanD;								// ���Z���Z�[�t�i�f�B���C����j
	long	safeCountJitan;									// ���Z���Z�[�t�i�f�B���C�Ȃ��j
	short	startCountJitanD;								// ���Z���n�����i�f�B���C����j
	short	startCountJitan;								// ���Z���n�����i�f�B���C�Ȃ��j
	long	cashSell;										// ��������
	long	cardSell;										// �J�[�h����
	// ��ȉ��A���۔������_�̎��ە⑫���ځ�	
	long	reAlarm;										// �ăA���[����
	long	heniOut;										// �ψڃA�E�g
	long	heniSafe;										// �ψڃZ�[�t
	long	heniStart;										// �ψڎn����
	long	sabunOut;										// �A�E�g����
	long	sabunSafe;										// �Z�[�t����
	long	sabunDedama;									// �o�ʍ���
	long	startCountNotHit;								// ���܊O�n����
	long	findDedama;										// ���o���_���܏o��
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�p�t�`�q�d�ʐM�f�[�^���i���L�j										*/
/****************************************************************************/
union	square_data_t {
	BYTE			raw[4096];
	req_tm001_t		reqTM001;
	req_su001_t		reqSU001;
	ans_su001_t		ansSU001;
	req_su005_t		reqSU005;
	req_su101_t		reqSU101;
	ans_su101_t		ansSU101;
	req_su102_t		reqSU102;
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�p�t�`�q�d�ʐM�d���i���L�j											*/
/****************************************************************************/
struct square_msg_t {
	square_header_t		header;
	square_data_t		data;
};
//----------------------------------------------------------------------------
/****************************************************************************/
/*	�r�p�t�`�q�d�ʐM�d���i���L�j											*/
/****************************************************************************/
typedef struct {
	short	kind;											// ��ʋ敪
	short	boardNo;										// �@��ԍ�
	long	alarmDate;										// �A���[�����۔����N����
	long	alarmTime;										// �A���[�����۔��������b
	short	alarmNo;										// �A���[������No.
	char	dmy[2];											// �\��
	long	simaNo;											// ���ԍ�
} ALARM_COLLECT_INFO;

#pragma pack(pop)
//----------------------------------------------------------------------------

#endif
//----------------------------------------------------------------------------
/*
 * $Log$
 */
//----------------------------------------------------------------------------
