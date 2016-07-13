#ifndef __winrek_h__
#define __winrek_h__

#include "asdef.h"
#include "Rs_kioku.h"


//------------------------------------------------------------
// �V�X�e���N���A�����\����
//------------------------------------------------------------
typedef struct {
	short	syubetu;				// �O���V�X�e���N���A�@�P���ݒ�N���A�@�Q���L���N���A
	short	yobi;					// �\��
	long	Date[6];				//	�V�X�e���N���A�N����		[0] = �N   [1] = �� [2]=��
									//								[3] = �j�� [4] = �� [5]=��
} SysClear_Rek_t;

//------------------------------------------------------------
// �ݒ�X�V�����\����
//------------------------------------------------------------
typedef struct {
	long	upd_date[6];		//	�X�V�N����		[0] = �N�@ [1] = ���@[2] = ��
								//					[3] = �j�� [4] = ��  [5] = ��
	short	upd_fno;			// �X�V�t�@�C���m�n
	short	yobi;
} settei_upd_t;

//------------------------------------------------------------
// �ғ����E����ғ��������\����
//------------------------------------------------------------
typedef union{
	char size[512];
	struct{
		long flag;			// =1�@�L�^�ς݃��R�[�h
		char hm[2];			// �������^��
		short kado;			// �ғ���
		short kaiKado;		// ����ғ���
		short reserved;		// �\��
	} d;
} KadoRek_t;



//------------------------------------------------------------
// �c�Ɨ����\����
//------------------------------------------------------------
typedef struct _EIREKI_KEN{
	unsigned long kensuu;
	unsigned long dmy1;
	unsigned long dmy2;
	unsigned long dmy3;
	unsigned long dmy4;
} eireki_KEN;

typedef struct _EIGYO_RIREKI_A{
	unsigned char	code1;
	unsigned char	code2;
	unsigned short	dmy;
	unsigned short	data1;
	unsigned short	data2;
	unsigned short	data3;
	unsigned short	data4;
	long			ldata5;
	unsigned long	has_ps_tim;		// ����(ps_tim)
} eigyou_rireki_A;

typedef struct _EIGYO_RIREKI_B{
	unsigned char	code1;
	unsigned char	code2;
	unsigned short	dmy;
	unsigned short	data1;
	unsigned short	data2;
	unsigned short	data3;
	unsigned short	data4;
	unsigned short	data5;
	unsigned short	data6;
	unsigned long	has_ps_tim;		// ����(ps_tim)
} eigyou_rireki_B;

typedef struct _EIGYO_RIREKI_C{
	unsigned char	code1;
	unsigned char	code2;
	unsigned short	dmy;
	unsigned char	data1;
	unsigned char	data2;
	unsigned char	data3;
	unsigned char	data4;
	unsigned char	data5;
	unsigned char	data6;
	unsigned char	data7;
	unsigned char	data8;
	unsigned char	data9;
	unsigned char	data10;
	unsigned char	data11;
	unsigned char	data12;
	unsigned long	has_ps_tim;		// ����(ps_tim)
} eigyou_rireki_C;

//------------------------------------------------------------
// �c�Ɨ������p��
//------------------------------------------------------------

#pragma pack( push, 1 )
typedef union _EIGYO_RIREKI {

	eireki_KEN			eireki_KEN;
	eigyou_rireki_A		e_rireki_A;
	eigyou_rireki_B		e_rireki_B;
	eigyou_rireki_C		e_rireki_C;
} EIGYO_RIREKI;
#pragma pack( pop )

// �������Ԃ��l�������\����
typedef struct {
	eigyou_rireki_A baseData;	// �c�Ɨ�����{�f�[�^
	u_long fuk_ps_tim;	// ��������(ps_tim)
	u_short dmy2;
	u_short dmy;		// �������Ԃ̃t���O�Ƃ��Ďg�p
} ERIREK_VB;

// mpc�����c�Ɨ����\����
typedef struct {
	char code1;				// �����R�[�h1
	char code2;				// �����R�[�h2
	short hhmms0_h;			// ��������
	short hhmms0_f;			// ��������
	short data[6];			// ���t���O
	char dmy[2];
	unsigned long eigyo_date;		// �c�Ɠ�(YYYYMMDD)
	unsigned long hno;				// �����ԍ�
	unsigned long hno_hassei;		// ���������ԍ�(�����ɑΉ����锭���ԍ�)
	long dmy1[2];
	char dmy2[3];
	char inner_work;		// ������ƃ��[�N
} EREKI_MPC;

//------------------------------------------------------------
// ��ԗ����\����
//------------------------------------------------------------
typedef struct _YAKAN_RIREKI {

	unsigned char	code1;
	unsigned char	code2;
	unsigned short	hhmms0;
	unsigned short	no;
	unsigned short	state;
	unsigned short	stop;
	unsigned short	data;
	unsigned short	yobi;
	unsigned short	mmdd;
} YAKAN_RIREKI;

//------------------------------------------------------------
// ���ܗ������p��
//------------------------------------------------------------
typedef struct _TOKUSYOU_RIREKI_KEN{
	unsigned long kensuu;
	unsigned long dmy1[7];
} tokureki_KEN;

// fno6102
typedef union _TOKUSYOU_RIREKI {
	tokureki_KEN	toku_ken;
	TOKREK_KIOKU	toku_rek;
} TOKU_RIREKI;

// fno6105
typedef union _TOKUSYOU_RIREKI2 {
	tokureki_KEN	toku_ken;
	TOKREK2_KIOKU	toku_rek;
} TOKU_RIREKI2;

//------------------------------------------------------------
// ���������\����
//------------------------------------------------------------
typedef union{
	char size[2048];
	struct{
		long flag;						// =1�@�L�^�ς݃��R�[�h
		char hm[2];						// �������^��
		short reserved;					// �\��
		double kikai[SHUBETSU+1];		// �@�B����(P,A,S,X,�S�X)
		double keihin[SHUBETSU+1];		// �i�i����(P,A,S,X,�S�X)
		double hos_kikai[SHUBETSU+1];	// �␳�@�B����(P,A,S,X,�S�X)
		double hos_keihin[SHUBETSU+1];	// �␳�i�i����(P,A,S,X,�S�X)
		double kis_kikai[KISHU];			// �@��ʋ@�B����
		double kis_hoskikai[KISHU];		// �@��ʕ␳�@�B����
	} d;
} WarisuuRek_t;

// 2002.11.07 ����V�Z���� �ǉ�->
#define MAX_KAILOG	(KAIINLOG_RSUU + DAISU)
//------------------------------------------------------------
// ����V�Z�����\����
//------------------------------------------------------------
typedef struct _wssfno5201_00 {
	unsigned long	qs_idx;			/* ں��޲��ޯ��(qsort�p)*/// 2002.12.11 kaoru �΍�(�����������ԏ��ɂȂ�)
	unsigned long	kaiid;			/* ���ID				*/
	unsigned char	subno;			/* ��ޔԍ�				*/
	unsigned char	pas;			/* ���					*/
	unsigned short	daino;			/* ��\���ԍ�			*/
	unsigned char	intim[2];		/* ����IN ����(BCD)		*/
	unsigned char	outtim[2];		/* ����OUT ����(BCD)	*/
	unsigned char	t1cnt;			/* T1��				*/
	unsigned char	k1cnt;			/* �m�ςP��			*/
	unsigned short	out;			/* �݌v���				*/
	unsigned short	saf;			/* �݌v���				*/
	unsigned short	genuri;			/* ���ތ���1+����2����	*/
	unsigned short	carduri;		/* ���޶��ޔ���			*/
	unsigned short	saf_ov;			/* ��̂̂��ӂꕪ		*/
	unsigned long	cyokos;			/* ����					*/
	unsigned short	start;			/* �݌v����				*/
	unsigned short	ukomi;			/* �ō���				*/
	unsigned char	tcnt;			/* ����					*/
	unsigned char	t2cnt;			/* ����2��			*/
	unsigned char	kcnt;			/* �m�ω�				*/
	unsigned char	kt1cnt;			/* �m�ϒ�T1��			*/
	unsigned short	t1out;			/* ���ܒ����-����2�����	*/
	unsigned short	t1saf;			/* ���ܒ����-����2�����	*/
	unsigned short	t2out;			/* ����2���				*/
	unsigned short	t2saf;			/* ����2���				*/
	unsigned short	t2start;		/* ����2����			*/
	unsigned short	point;			/* �߲�Đ�				*/
	unsigned short	cdin_errcode;	/* ����IN �ʐM�װ����	*/
	unsigned short	cdout_errcode;	/* ����OUT �ʐM�װ����	*/
	unsigned short	repkos;			/* ����ڲ��				*/
	unsigned short	reptesu;		/* ����ڲ�萔��			*/
} wssfno5201_00_t;

typedef wssfno5201_00_t RPLY_LOG;	/* �V�Z����(�݌v�ް�)	*/
typedef wssfno5201_00_t KPLY_LOG;	/* �V�Z����(�X�ް�)	*/
typedef wssfno5201_00_t KAIIN_LOG;	/* �������				*/
typedef wssfno5201_00_t KAIPLY_LOG;	/* ����V�Z����			*/
// 2002.11.07 ����V�Z���� �ǉ�<-


// Rply_Log.bin��HN2������CSV������Ƃ��Ɏg���g����RPLY_LOG
// �J�n�A�E�g�A�J�n�Z�[�t�A�A����ԍ����ǉ�

typedef struct {
	RPLY_LOG base;
	unsigned long rdb;			// �A����ԍ�
	unsigned long prev_out;		// ���R�[�h�J�n���A�E�g
	unsigned long prev_safe;	// ���R�[�h�J�n���Z�[�t
	unsigned long prev_t1cnt;	// ���R�[�h�J�n��T1��
	unsigned long prev_start;	// ���R�[�h�J�n���݌v�X�^�[�g
	unsigned long prev_t1out;	// ���R�[�h�J�n��T1�A�E�g
} RPLY_LOG_EX, fno9024_t;

#endif

