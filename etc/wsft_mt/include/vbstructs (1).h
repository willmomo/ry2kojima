#ifndef __vbstructs_h__
#define __vbstructs_h__

//DLL�֐��̃I�v�V����
#define HC_OPT_BODY 0
#define HC_OPT_HEAD 1
#define MAX_OPT		2

//�G���[�萔
#define HALCONVERR_WRITE		0x1002		//�������݃G���[
#define HALCONVERR_CAST			0x1003		//�ϊ��G���[
#define HALCONVERR_ILLEGALFID	0x2001		//�s���ȃt�@�C���ԍ�
#define HALCONVERR_ILLEGALOPT	0x2002		//�s���ȃI�v�V�����l
// ---- 2001/09/17 �ǉ� ----
#define HALCONVERR_LOGPATH		0x3001		//���O�t�@�C���̃p�X���w�肳��Ă��Ȃ�
#define HALCONVERR_MAXSIZE		0x3002		//���O�̕����T�C�Y���s���A�܂��͎w�肳��Ă��Ȃ�
#define HALCONVERR_TRACELEVEL	0x3003		//�g���[�X���x�����s���A�܂��͎w�肳��Ă��Ȃ�
#define HALCONVERR_SAVEDDATE	0x3004		//�ێ��������s���A�܂��͎w�肳��Ă��Ȃ�
#define HALCONVERR_TRACEVER		0x3005		//���O�o��DLL�̃o�[�W�������s���A�܂��͎w�肳��Ă��Ȃ�
// ---- 2001/09/17 �����܂� ----
// ---- 2001/09/19�ǉ� ----
#define HALCONVERR_TRACESTART	0x3006		//TraceLogStart()�Ɏ��s����

//�t�@�C���ԍ��萔
#define FNO1000			1000
#define FNO1001			1001
#define R_SYSTEM_IJO	4006
#define R_CLEAR			4007
#define R_TIMESET		4008
#define R_TEIDEN		4009
#define R_SETTEI		4010
#define R_IJO			4022
#define R_UTIDOME		4023
#define R_HASSEI		4024
#define I_KIOKU			4101
#define F_DAI			4102
#define F_ZENTEN		4103
#define F_SHIMA			4104
#define F_KISHU			4105
#define F_SHUBETSU		4106
#define F_TOTAL			4107
#define F_KINKO			4108
#define I_TEIJI			4109
#define F_T_SHUBETSU	4110
#define F_T_TOTAL		4111
#define I_T_KADO		4112
#define F_T_SBT_KADO	4113
#define F_T_ZEN_KADO	4114
#define F_T_KISHU_KADO	4115
#define F_T_DSP_DATA	4117


// ---- 2001/09/14 �R�����g�A�E�g ----
/*
//�֐����e�[�u���̓Y�����萔
#define FUNC_RAW2VB		0
#define FUNC_VB2RAW		1

//�������t�@�C�����e�[�u���̓Y�����萔
#define FILE_SYSIJO		0
#define FILE_CLEAR		1
#define FILE_TIMESET	2
#define FILE_TEIDEN		3
#define FILE_SETTEI		4
#define FILE_IJO		5
#define FILE_UTIDOME	6
#define FILE_HASSEI		7
#define FILE_KIOKU		8
#define FILE_DAI		9
#define FILE_ZENTEN		10
#define FILE_SHIMA		11
#define FILE_KISHU		12
#define FILE_SHUBETU	13
#define FILE_TOTAL		14
#define FILE_KINKO		15
#define FILE_TEIJI		16
#define FILE_T_SHUBETU	17
#define FILE_T_TOTAL	18
#define FILE_T_KADO		19
#define FILE_T_SBT_KADO 20
#define FILE_T_ZEN_KADO 21
#define FILE_T_KSH_KADO 22
*/
// ---- 2001/09/14 �����܂� ----


//�I���W�i���w�b�_�[�p�^��`
typedef unsigned short	ushort;
typedef unsigned char	uchar;
typedef unsigned long	ulong;


// ---- 2001/09/14 �R�����g�A�E�g ----
/*
//�֐����e�[�u��
const char FuncNames[2][12] = { "hc_raw2vb()", "hc_vb2raw()" };

//�G���[���b�Z�[�W�p�������t�@�C�����e�[�u��
const char MemFiles[23][27] = {
	"�V�X�e���ُ헚��", "�N���A����", "���v�ݒ藚��", "��d����", "�ݒ藚��", "�ُ헚��",
	"�Ŏ~����", "��������", "�����E�L�����", "�����E��t�@�C��", "�����E�S�X�t�@�C��", "�����E���t�@�C��",
	"�����E�@��t�@�C��", "�����E��ʏW�v", "�����E�S�X�W�v�t�@�C��", "�����E���ɏW�v", "�莞���", "�����E�莞��ʏW�v",
	"�����E�莞�S�X�W�v", "�����E�莞�ғ����", "�����E�莞��ʉғ��t�@�C��", "�����E�莞�S�X�ғ��t�@�C��", "�����E�莞�@��ғ��t�@�C��"
};
*/
// ---- 2001/09/14 �����܂� ----

/*	VB�p�\����	*/

/****************************************/
/*	VB�p�V�X�e���ُ헚��(4006)			*/
/****************************************/
typedef struct{
	long cnt;
	long dmy[3];
} VB_REKI_SYSIJO_HED;

typedef struct {
	char sysijyo_pw_tim[16];
	UCHAR code1;
	UCHAR code2;
	long no;
} VB_REKI_SYSIJO;


/****************************************/
/*	VB�p�N���A����(4007)				*/
/****************************************/
typedef struct{
	char clr_pw_tim[16];
	UCHAR code1;
	UCHAR code2;
	long dmy[3];
	long urikin;
} VB_REKI_CLEAR;


/****************************************/
/*	VB�p���v�ݒ藚��(4008)				*/
/****************************************/
typedef struct{
	char mae_pw_tim[16];
	UCHAR code1;
	UCHAR code2;
	long dmy[3];
	char set_pw_tim[16];
} VB_REKI_TIMSET;


/****************************************/
/*	VB�p��d����(4009)					*/
/****************************************/
typedef struct{
	char pwon_pw_tim[16];
	UCHAR code1;
	UCHAR code2;
	long dmy[2];
	UCHAR eflg;
	UCHAR dmy1;
	char pwoff_pw_tim[16];
} VB_REKI_TEIDEN;


/****************************************/
/*	VB�p�ݒ藚��(4010)					*/
/****************************************/
typedef struct{
	char pw_tim[16];
	UCHAR hcd1;
	UCHAR hcd2;
	long set_fno[5];
} VB_REKI_SETTEI;


/****************************************/
/*	VB�p�ُ헚��(4022)					*/
/****************************************/
typedef struct{
	long cnt;
	long hasei;
	long suu;
	long dmy[5];
} VB_REKI_IJO_HED;

typedef struct{
	UCHAR code1;
	UCHAR code2;
	UCHAR htime[2];
	long no;
	UCHAR jflg[2];
	long tflg;
	long cnt;
	UCHAR dmy[2];
	UCHAR ftime[2];
} VB_REKI_IJO;


/****************************************/
/*	VB�p�Ŏ~����(4023)					*/
/****************************************/
typedef struct{
	long cnt;
	long hasei;
	long suu;
	long dmy[3];
} VB_REKI_UTI_HED;

typedef struct{
	UCHAR code1;
	UCHAR code2;
	UCHAR time[2];
	long no;
	UCHAR jflg[2];
	long tflg;
	long cnt;
} VB_REKI_UTI;


/****************************************/
/*	VB�p��������(4024)					*/
/****************************************/
typedef struct{
	long cnt;
	long wpos;
	long dmy[4];
} VB_REKI_HASSEI_HED;

typedef struct{
	UCHAR code1;
	UCHAR code2;
	UCHAR time[2];
	long no;
	UCHAR jflg[2];
	long tflg;
	long cnt;
} VB_REKI_HASSEI;


/****************************************/
/*	VB�p�L�����(4101)					*/
/****************************************/
typedef struct{
	long sync;
	UCHAR kioku_index;
	UCHAR kioku_fg;
	UCHAR emark;
	UCHAR dmy1[3];
	char down_pw_tim[16];
	char pwon_pw_tim[16];
	char clear_pw_tim[16];
	char open_pw_tim[16];
	char close_pw_tim[16];
	char sime_pw_tim[16];
	char kioku_pw_tim[16];
	long eigyou_min_cnt;
	UCHAR clear_cnt;
	UCHAR clear_cnt_0;
	UCHAR clear_cnt_1;
	UCHAR down_cnt;
	UCHAR down_cnt_1;
	UCHAR down_cnt_2;
	UCHAR kioku_cnt;
	UCHAR dmy2[3];
	char settei_ps_tim[19];
	char now_pw_tim[16];
	UCHAR dmy3[8];
	struct TENPO_KIOKU_JYOHO {			/* AS(SH3)�X�܋L�����	*/
		unsigned char	t_mark;			/* 00:�X��:(=$41)A�X,(=$42)B�X	*/
		unsigned char	t_holiday;		/* 01:�c�Ɠ�FLAG (bit7=1)�J�X��,(bit7=0)�x�X��	*/
		unsigned short	t_eday_md;		/* 02:�c�Ɠ��t�Fbit15-8=��,bit7-0=��	*/
		unsigned short	t_clear_hm;		/* 04:�N���A�����Fbit15-8=��,bit7-0=��	*/
		unsigned short	t_open_hm;		/* 06:�J�X�����Fbit15-8=��,bit7-0=��	*/
		unsigned short	t_clos_hm;		/* 08:�X�����Fbit15-8=��,bit7-0=��	*/ 
		unsigned short	t_sime_hm;		/* 0a:���؎����Fbit15-8=��,bit7-0=��	*/
		unsigned short	t_eigyo_m;		/* 0c:�c�Ǝ��ԁF�ݐϕ�					*/
		unsigned char	t_dmy2[2];		/* 0e:	*/
	} tenpo_kioku_jyoho[2];				/* 10:	*/
	unsigned char	dmy4[160];			/* 160: */
} VB_INF_KIOKU;


/****************************************/
/*	VB�p��f�[�^(4102)					*/
/****************************************/
typedef struct{
	long jflg;
	long out;
	long saf;
	long start;
	long tout;
	long tsaf;
	long t2out;
	long t2saf;
	long t2start;
	long bcnt;
	long ucnt;
	long tcnt;
	long t1cnt;
	long t2cnt;
	long kcnt;
	long kt1cnt;

	long lcnt;
	long lt1cnt;
	long lt1max;
	long lout;
	long lsaf;
	long lbout;
	long lmoti;

	long start1;
	long start2;
	long start3;
	long t2start1;
	long t2start2;
	long t2start3;
	long t1start2;

	long ukomi;
	long t1ukomi;
	short minsa;
	short maxsa;
	long minout;
	long maxout;
	long maxmoti;
	long maxukomi;

	long kadofg;
	UCHAR kado;
	UCHAR kyaku;

	long hasfg;
	UCHAR door1_2;
	UCHAR call1_2;
	UCHAR fus_ddan;
	UCHAR ijo_ten;
	UCHAR g1_g2dan;
	UCHAR cddan_uijo;

	UCHAR ukind;
	UCHAR dmy1;
	long gen1uri;
	long gen2uri;
	long carduri;
	long daiuri;
	long hosei;

	UCHAR k_suu;
	UCHAR k_kado;
	long k_kadofg;
	UCHAR k_t1cnt;
	UCHAR k_tcnt;
	UCHAR k_kcnt;
	UCHAR k_lcnt;
	long k_out;
	long k_saf;
	long k_tout;
	long k_tsaf;
	long k_start;
	long k_uri;
	long k_repkos;
	long k_reptesu;

	long r_bonus;

	long toutmax;
	long toutmin;
	short tsamax;
	short tsamin;
	long boutmax;
	long boutmin;
	short bsamax;
	short bsamin;
	long bstartmax;
	long bstartmin;
	short bmsamax;
	short bmsamin;
	short tmotimax;
	short tmotimin;

	long kr_bonus;

	UCHAR tflg;
	UCHAR dmy2;
	UCHAR e_hour;
	UCHAR e_min;
	long e_t1out;
	long e_t1saf;
	long e_bout;
	long e_bsaf;
	long e_bstart;
	long e_bmsa;
	long e_buri;
	long e_motichg;
	long e_motiever;
	UCHAR yobi2[2];
	long san_ukos;
	long chk_dat;
	long k_keisu;
	long k_cyokos;
} VB_FDATA_DAI;


/****************************************/
/*	VB�p�S�X�f�[�^(���R��)(4103)	*/
/****************************************/
typedef struct{
	long indata;
	short dancnt;
	short err_door;
	short othercnt;
	short flg;
} VB_FDATA_SMCON;



/****************************************/
/*	VB�p���E�@��f�[�^(4104,4105)		*/
/****************************************/
typedef struct{
	long daisu;
	long jflg;
	long out;
	long saf;
	long start;
	long tout;
	long tsaf;
	long t2out;
	long t2saf;
	long t2start;
	long bcnt;
	long ucnt;
	long tcnt;
	long t1cnt;
	long t2cnt;
	long kcnt;
	long kt1cnt;

	long lcnt;
	long lt1cnt;
	long lt1max;
	long lout;
	long lsaf;
	long lbout;
	long lmoti;

	long start1;
	long start2;
	long start3;
	long t2start1;
	long t2start2;
	long t2start3;
	long t1start2;

	long ukomi;
	long t1ukomi;
	long maxmoti;
	long maxukomi;

	long kado;
	long kyaku;

	long hasfg;
	long door1;
	long door2;
	long call1;
	long call2;
	UCHAR fusnct;
	UCHAR ddancnt;
	UCHAR ijocnt;
	UCHAR tencnt;
	UCHAR g1dancnt;
	UCHAR g2dancnt;
	UCHAR cddancnt;
	UCHAR uijocnt;

	UCHAR ukind;
	UCHAR dmy1;
	long gen1uri;
	long gen2uri;
	long carduri;
	long daiuri;
	long hosei;

	long k_suu;
	long k_kado;
	long k_t1cnt;
	long k_tcnt;
	long k_kcnt;
	long k_lcnt;
	long k_out;
	long k_saf;
	long k_tout;
	long k_tsaf;
	long k_start;
	long k_uri;
	long k_repkos;
	long k_repkin;
	long k_reptesu;
	long k_reptesukin;
	long k_cyokos;
	long k_cyokin;
	long k_keisu;

	long r_bonus;
	long kr_bonus;

	long g_kado;
	long g_k_kado;

	UCHAR yobi[24];
} VB_FDATA_SK;


/****************************************/
/*	VB�p��ʏW�v�E�t���A�f�[�^(4106,4110)*/
/****************************************/
typedef struct{
	long daisu;
	long jflg;
	long out;
	long saf;
	long start;
	long tout;
	long tsaf;
	long t2out;
	long t2saf;
	long t2start;
	long bcnt;
	long ucnt;
	long tcnt;
	long t1cnt;
	long t2cnt;
	long kcnt;
	long kt1cnt;

	long lcnt;
	long lt1cnt;
	long lt1max;
	long lout;
	long lsaf;
	long lbout;
	long lmoti;

	long ukomi;
	long t1ukomi;

	long kado;
	long kyaku;

	long hasfg;
	long door1;
	long door2;
	long call1;
	long call2;
	UCHAR fuscnt;
	UCHAR ddancnt;
	UCHAR ijocnt;
	UCHAR tencnt;
	UCHAR g1dancnt;
	UCHAR g2dancnt;
	UCHAR cddancnt;
	UCHAR uijocnt;

	UCHAR ukind;
	UCHAR dmy1;
	long gen1uri;
	long gen2uri;
	long carduri;
	long daiuri;
	long hosei;

	long k_suu;
	long k_kado;
	long k_t1cnt;
	long k_tcnt;
	long k_kcnt;
	long k_lcnt;
	long k_out;
	long k_saf;
	long k_tout;
	long k_tsaf;
	long k_start;
	long k_uri;
	long k_repkos;
	long k_reptesu;
	long k_cyokos;

	long sg_danflg;
	long urikin;
	long t_keikos;
	long t_g_keikos;
	long sg_gen1uri;
	long sg_gen2uri;
	long sg_carduri;
	long sg_repkos;
	long sg_reptesu;
	long sg_premkos;
	long sg_cyokos;
	long kinko;
	long sand;
	long sandkinko;
	long poskos;
	long g_poskos;
	long k_keisu;

	long r_bonus;
	long kr_bonus;

	long g_kado;
	long g_k_kado;

	UCHAR yobi[8];
} VB_FDATA_SBT;


/****************************************/
/*	VB�p�S�X�W�v(4107,4111)				*/
/****************************************/
typedef struct{
	long daisu;
	long jflg;

	long kado;

	long hasfg;

	UCHAR g1dancnt;
	UCHAR g2dancnt;
	UCHAR cddancnt;
	UCHAR uijocnt;

	UCHAR ukind;
	UCHAR dmy1[3];
	long gen1uri;
	long gen2uri;
	long carduri;
	long daiuri;
	long hosei;

	long k_suu;
	long k_kado;
	long k_uri;
	long k_repkin;
	long k_reptesukin;
	long k_cyokin;

	long sg_danflg;
	long urikin;
	long t_keikin;
	long t_g_keikin;
	long sg_gen1uri;
	long sg_gen2uri;
	long sg_carduri;
	long sg_repkin;
	long sg_reptesukin;
	long sg_premkin;
	long sg_cyokin;
	long kinko;
	long sand;
	long sandkinko;
	long poskin;
	long g_poskin;

	long cd_hfg;
	long cd_hdanfg;
	long cd_htotal;
	long cd_h1kei;
	long cd_h2kei;
	long cd_h3kei;
	long cd_h5kei;
	long cd_h10kei;

	long zihan;
	long ta;

	long d_keikin;
	long g_d_keikin;
	long hd_keikin;
	long g_hd_keikin;
	long k_keikin;
	long g_duri_keikin;

	long g_kado;
	long g_k_kado;

	UCHAR yobi[72];
} VB_FDATA_TOTAL;



/****************************************/
/*	VB�p���ɏW�v(4108)					*/
/****************************************/
typedef struct{
	UCHAR sima[4];
	long gen1kei;
	long gen2kei;
	long sandkinko;
	long cardkei;
	long kinko;
	UCHAR g1dancnt;
	UCHAR g2dancnt;
	UCHAR skkdancnt;
	UCHAR cddancnt;
	UCHAR kkdancnt;
	UCHAR errcnt;
	UCHAR doorcnt;
	UCHAR kktencnt;
} VB_FDATA_KINKO;



/****************************************/
/*	VB�p�莞���(4109)					*/
/****************************************/
typedef struct{
	UCHAR kiotim[2];
	long egyo;
} VB_FDATA_TIME_INF;


//!��̍\���̂Ɠ����B�܂Ƃ߂�H
/****************************************/
/*	VB�p�莞�ғ����(4112)				*/
/****************************************/
typedef struct{
	UCHAR kiotim[2];
	long egyo;
} VB_FDATA_KADO_INF;


/****************************************/
/*	VB�p�莞��ʉғ�(4113)				*/
/****************************************/
typedef struct{
	long daisu;
	long tcnt;
	long out;
	long saf;
	long urikin;
	long hosei;
	long kado;
	long k_kado;
	long keikos;
	UCHAR yobi[4];
} VB_FDATA_KADO_SBT;


/****************************************/
/*	VB�p�莞�S�X�ғ�(4114)				*/
/****************************************/
typedef struct{
	long daisu;
	long tcnt;
	long yobi[2];
	long urikin;
	long hosei;
	long kado;
	long k_kado;
	long keikin;
	long g_keikin;
	long d_keikin;
	long g_d_keikin;
	long hd_keikin;
	long g_hd_keikin;
} VB_FDATA_KADO_ZEN;


/****************************************/
/*	VB�p�莞�@��ғ�(4115)				*/
/****************************************/
typedef struct{
	long daisu;
	long tcnt;
	long out;
	long saf;
	long urikin;
	long hosei;
	long kado;
	long k_kado;
} VB_FDATA_KADO_KISHU;


#endif