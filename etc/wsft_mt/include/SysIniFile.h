#ifndef __INIFILEGET_H_INCLUDED__
#define __INIFILEGET_H_INCLUDED__
//------------------------------------------------------------
//�T�v
//   �V�X�e���h�����t�@�C���֌W
//
//�쐬��
//   Tokai���J
//
//���l
//
//   �V�X�e���h�����t�@�C���֌W�̒�`
//------------------------------------------------------------

#include <windows.h>
//------------------------------------------------------------
//	Define�錾
//------------------------------------------------------------
//	�V�X�e���h�����t�@�C���o������
//#define	DEF_INIFILENAME ".\\WinSousa.ini"

//	�Z�N�V������

//	�L�[��

//	�f�t�H���g�l



//------------------------------------------------------------
//	�\���̐錾
//------------------------------------------------------------
typedef struct {						// �V�X�e�����e�[�u��
	long System;						// �V�X�e�� (0:HC-1000,!0:HAL-300i)
	long ModelNo;						// ���f���ԍ�
	long EmgTerm;						// �ً}�p�[��(0=no 1=yes)
	long LineMode;						// ���C�����[�h(0:OFFLINE,1:ONLINE)
	long StopPrint;						// �󎚂̗L��
	long KeyboardType;					// ���ް������ (0:HC-1000,1:HAL-300i)
	long ConnectPtn;					// �ڑ��p�^�[���i0:�����ڑ�,1:�蓮�ڑ��j
	char DefaultWss[MAX_PATH];			// �呀���PC��
	long TerminalType;					// �^�p�[���̎g�p�^�C�v	(0:�ʏ�̉^�[�Ƃ��Ďg�p 1.�����[�g�^�[�~�i���Ƃ��Ďg�p)
	long KadoCntType;					// �ғ����J�E���g �i0:����00��30���͂`�r�̃t�@�C������擾 1:����ŃJ�E���g����j
	long BestDataPtn;					// �x�X�g��t�@�C���̑��M 0:�x�X�g��E�������ߑ�t�@�C���̂ݑ��M	1:�R�[�i�[�x�X�g�䓙�̃t�@�C�������M
	long ReadKishuTypeSet;				// MPC�@��^�C�v�ݒ�捞�@�\ 0:�Ȃ�/1:���� 20051101 add tomida
	long MaskBeltErr;					// �уG���[�}�X�N 0:�}�X�N���Ȃ�/1:�}�X�N 20060106 tomida
	long SystemTime;					// �V�X�e������	0:WSS�ɍ��킹��/1:AS�ɍ��킹�� 20060127 hashimoto
	long SystemTimeSetMinutes;			// SetTime=0(WSS��AS�ɍ��킹��)�̎��A�V�X�e���������킹���J�Xn���O�܂łƂ���
	long ConnectDaiLamp;				// �䃉���v�ڑ�	0:��/1:�L 20060202 hashimoto
	long ConnectIB;						// ��̫Ұ����ް�ސڑ�	0:��/1:�L 20060202 hashimoto
	long PowerOFFWarningHours;			// �d���؂�Y��Ōx���\�����鎞��	2007.08.02 add hashimoto(T2X003966)
	long DeadLockTime;					// �f�b�h���b�N���莞��(��)		�� �����Ԋu1���܂�
	long DeadLockCnt;					// �f�b�h���b�N�����(����ɏ����Ԋu���Ԃ������Čo�ߎ��ԂƔ�r)	�� �����Ԋu2���ȏ�
	long MaskKakudaiKeisu;				// �e��v���}�X�N�ݒ�
	long FtcChkKakudaiKeisuData;		// FTC�e��v���f�[�^���擾����/���Ȃ��ݒ� 0:�擾���Ȃ��@1:�擾����(�f�t�H���g)
										// ��1:�擾����ꍇ�ł��A�呀�슎��GetCnctFTC()==1�̏ꍇ�̂ݓ���ΏۂƂȂ�܂��B
	long StartUpGFTP;					// GFTP�N��			0:�N�����Ȃ��@1:�N��	
	long StartUpSquareTCP;				// SquareTCP�N��	0:�N�����Ȃ��@1:�N��

} SYSINIFILE_SYSTEM;

typedef struct {
	long StopHasseiInji;				// �����󎚒�~
} SYSINIFILE_DEBUG;
/*		** HC2007 �萔�ύX **
typedef struct {						// ��ʕ\���e�[�u��
//	char moji[SHUBETSU][8];				// ��ʕ��� 1���o�[�W�������R�Œ�`���Ă���̂� 2002.05.22 koketsu
	char moji[4][8];					// ��ʕ���	�����͂S�Œ�ɂ��Ă��� 2002.05.22 koketsu
} SYSINIFILE_SHUBETSU;
*/
typedef struct {						// �p�X���e�[�u��
	char Base[_MAX_PATH];				// ��{�p�X
	char Online[_MAX_PATH];				// �I�����C���p�X
	char Tantai[_MAX_PATH];				// �P�̃p�X
	char Kioku[_MAX_PATH];				// �L���p�X
	char RecoverKioku[_MAX_PATH];		// ���J�o���L���p�X
	char Log[_MAX_PATH];				// ���O�p�X
	char Kpc[_MAX_PATH];				// �j�o�b�p�X
	char Init[_MAX_PATH];				// �C�j�V���p�X
	char Unyou[_MAX_PATH];				// �^�p�p�X
	char Arakajime[_MAX_PATH];			// �\�߃p�X
	char UnyouKari[_MAX_PATH];			// �^�p���p�X
	char ArakajimeKari[_MAX_PATH];		// �\�߉��p�X
	char Mpc[_MAX_PATH];				// MPC�p�X
	char Share[_MAX_PATH];				// share�p�X
	char Hn2[_MAX_PATH];				// HN2�p�X
	char Csv[_MAX_PATH];				// Csv�p�X
	char Files[_MAX_PATH];				// Files�p�X
	char Ftc[_MAX_PATH];				// Ftc�p�X
	char TempGL01[_MAX_PATH];			// TempGL/DSP/01�p�X
	char TempGL02[_MAX_PATH];			// TempGL/DSP/02�p�X
} SYSINIFILE_PATH;

typedef struct {						// �`�r�ʐM�h�o���e�[�u��
	char Ipadr[16];						// IP�A�h���X
	long Rport;							// ���[�h�|�[�g�A�h���X
	long Wport;							// ���C�g�|�[�g�A�h���X
} SYSINIFILE_IP;

typedef struct {
	int nLogLevel;						// ���O ���x��
} LOG_SECTION;

typedef struct{
	char dsp1copypath1[MAX_PATH];	// DSP1�f�[�^�t�@�C���R�s�[��1
	char dsp1copypath2[MAX_PATH];	// DSP1�f�[�^�t�@�C���R�s�[��2
	char dsp1copypath3[MAX_PATH];	// DSP1�f�[�^�t�@�C���R�s�[��3
	char dsp1copypath4[MAX_PATH];	// DSP1�f�[�^�t�@�C���R�s�[��4
	char dsp1copypath5[MAX_PATH];	// DSP1�f�[�^�t�@�C���R�s�[��5
	char dsp2copypath1[MAX_PATH];	// DSP2�f�[�^�t�@�C���R�s�[��1
	char dsp2copypath2[MAX_PATH];	// DSP2�f�[�^�t�@�C���R�s�[��2
	char dsp2copypath3[MAX_PATH];	// DSP2�f�[�^�t�@�C���R�s�[��3
	char dsp2copypath4[MAX_PATH];	// DSP2�f�[�^�t�@�C���R�s�[��4
	char dsp2copypath5[MAX_PATH];	// DSP2�f�[�^�t�@�C���R�s�[��5
} HALLINFO_DSP;

typedef struct{
	long MakeTeijiData;				// �莞�ғ��f�[�^�쐬
	long TeijiDataKind;				// �莞�ғ��f�[�^���
} HALLINFO_KISHU_TEIJI_KADO;

typedef struct {
	SYSINIFILE_SYSTEM sys;				// �V�X�e�����
	SYSINIFILE_DEBUG dbg;				// �f�o�b�O�p
//	SYSINIFILE_SHUBETSU sbt;			// ��ʕ\��			** HC2007 �萔�ύX **
	SYSINIFILE_PATH path;				// �p�X���
	SYSINIFILE_IP fltAs1;				// �t�@�C���ʐM�@�`�r���P
	SYSINIFILE_IP fltAs2;				// �t�@�C���ʐM�@�`�r���Q
	SYSINIFILE_IP fltAs3;				// �t�@�C���ʐM�@�`�r���R
	SYSINIFILE_IP fltAs4;				// �t�@�C���ʐM�@�`�r���S
	SYSINIFILE_IP fltAs5;				// �t�@�C���ʐM�@�`�r���T
	SYSINIFILE_IP opeAs1;				// �^�p�ʐM�ʐM�@�`�r���P
	LOG_SECTION secLog;					// [Log]�Z�N�V����
	HALLINFO_DSP dsp;
	HALLINFO_KISHU_TEIJI_KADO KishuTeijiKado;	// hallinfo.ini[�@��ʒ莞�ғ���]
} SYSINIFILE;
typedef SYSINIFILE SysIniFile;

//------------------------------------------------------------
//	�v���g�^�C�v�錾
//------------------------------------------------------------
extern BOOL GetSystemIniFile(SysIniFile *sysIniFile);

#endif	//	__INIFILEGET_H_INCLUDED__