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
} eireki_KEN;

typedef struct _EIGYO_RIREKI_A{
	unsigned char	code1;
	unsigned char	code2;
	unsigned short	hhmms0;
	unsigned short	data1;
	unsigned short	data2;
	unsigned short	data3;
	unsigned short	data4;
	long			ldata5;
} eigyou_rireki_A;

typedef struct _EIGYO_RIREKI_B{
	unsigned char	code1;
	unsigned char	code2;
	unsigned short	hhmms0;
	unsigned short	data1;
	unsigned short	data2;
	unsigned short	data3;
	unsigned short	data4;
	unsigned short	data5;
	unsigned short	data6;
} eigyou_rireki_B;

typedef struct _EIGYO_RIREKI_C{
	unsigned char	code1;
	unsigned char	code2;
	unsigned short	hhmms0;
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

} eigyou_rireki_C;

//------------------------------------------------------------
// �c�Ɨ������p��
//------------------------------------------------------------

typedef union _EIGYO_RIREKI {

	eireki_KEN			eireki_KEN;
	eigyou_rireki_A		e_rireki_A;
	eigyou_rireki_B		e_rireki_B;
	eigyou_rireki_C		e_rireki_C;
} EIGYO_RIREKI;

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

typedef union _TOKUSYOU_RIREKI {
	tokureki_KEN	toku_ken;
	TOKREK_KIOKU	toku_rek;
} TOKU_RIREKI;


//------------------------------------------------------------
// �p�l���f�[�^�i�[�p�\����
//------------------------------------------------------------
typedef struct _dai_status {
unsigned long dai_status[DAISU];
} dai_status_t;

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
#if 0
	unsigned char	dcode1;			/* �d������1 0x22�Œ�	*/
	unsigned char	dcode2;			/* �d������2 ox03�Œ�	*/
	unsigned char	subcode;		/* ��޺���				*/
									/*	3 = �ғ����		*/
									/*	4 = ���ޱ��			*/

	unsigned char	yobi1;			/* ......				*/
	unsigned char	yobi2[2];		/* ......				*/
	unsigned char	format;			/* ̫�ϯ� 0x02�Œ�		*/
	unsigned char	jflg;			/* ����׸� 0x00�Œ�	*/
#endif
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

#endif

