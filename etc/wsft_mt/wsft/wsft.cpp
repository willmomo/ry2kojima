/**************************************************************************************/
/***																				***/
/***								wsft.cpp										***/
/***																				***/
/**************************************************************************************/
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include "wsft.h"
#include "wsft_def.h"
//�@�ύX�@***�@FUMIYA.K  02.01.07 >>

#include "wsini_def.h"
#include "wsft_err.h"
#include "vbstructs.h"

#include "tracex.h"
#include "GetIniFile.h"

#include <map>
#include <string>

__declspec(thread) static St_SocketInfo SockInfo ;

#define FILE_COUNT_MAX		(10000)

#pragma data_seg(".shared")
LONG g_Suspended = TRUE;		// �ʐM�ĊJ�׸�
#pragma data_seg()

extern DWORD g_dwTOCount[2];	//[0]:�S��M���s�� [1]:�^�C���A�E�g������

std::string GetExeName()
{
	char szExeName[_MAX_PATH];	// Exe�̃t���p�X��
	std::string exeName;

	// ���s���W���[���̃t�@�C�������O���[�o���ϐ��Ɋi�[�@**FUMIYA
	GetModuleFileName(NULL, szExeName, sizeof(szExeName));
	PathFindFileName(szExeName);

	exeName.assign(szExeName);

	return exeName;
}

/*--------------------------------------------------------------/
		�t�@�C���ԍ�������I�ȃC���f�b�N�X�ɕϊ�����

	[����]
		unsigned long	fid		;	SMF�t�@�C���ԍ�

	[�߂�l]
		�t�@�C���ԍ��ɑΉ������O����n�܂�C���f�b�N�X
/--------------------------------------------------------------*/
int FileNoToIndex( unsigned long fid )
{
	int		ret = 0 ;

	return((fid < FILE_COUNT_MAX) ?fid :0);

}

/*----------------------------------------------------------------------------------/
							�g���[�X�o�͂̊J�n

	[�֐���]
		TraceStart

	[����]
		�Ȃ�

	[�߂�l]
		int				:	==0 ���� !=0 �G���[

/----------------------------------------------------------------------------------*/
int TraceStart()
{
	TRACEX_OPT opt;

	strcpy(opt.dir, "d:\\work");	// �o�͐�f�B���N�g��
	strcpy(opt.pre, "wsft");		// �t�@�C�����擪
	strcpy(opt.suf, ".log");		// �t�@�C�������
	TraceXStart(&opt);

	return 0;
}

/*--------------------------------------------------------------/
						�ݒ�t�@�C���ǂݍ���

	[�֐���]
		ReadIniFile

	[����]
		StartFlag	:	Wsft_Start�֐�����Ă΂ꂽ���ǂ���
						�Â����O�t�@�C���̍폜�����̗L�����؂�ւ��܂�

	[�߂�l]
		== 0		:	����
		!= 0		:	�G���[(�G���[�ԍ�)

/--------------------------------------------------------------*/
int ReadIniFile()
{
	const u_short VERSION = 0x100;		// ����DLL���T�|�[�g����d���̃o�[�W����

	TraceStart() ;

	// ���@�[�W�����ɒ萔�l���́@**FUMIYA
	SockInfo.ver = VERSION;

	// INI�t�@�C�����MAXSIZE�擾�@**FUMIYA
	long lData;
	GetWinSousaInt("Wsft", "maxsize", &lData);
	SockInfo.maxsize = (unsigned short)lData;

	// INI�t�@�C�����A�h���X�擾�@**FUMIYA
	GetWinSousaStr("Wsft", "addr", SockInfo.Addr, sizeof(SockInfo.Addr));

	// �f�t�H���g�l�̓Ǎ�
	// INI�t�@�C�����f�t�H���g�ǂݍ��݃|�[�g�擾�@**FUMIYA
	GetWinSousaInt("Wsft", "Port_Read", &lData);
	SockInfo.IniInfo.rPort = (unsigned short)lData;

	// INI�t�@�C�����f�t�H���g�����݃|�[�g�擾�@**FUMIYA
	GetWinSousaInt("Wsft", "Port_Write", &lData);
	SockInfo.IniInfo.wPort = (unsigned short)lData;

	// INI�t�@�C�����f�t�H���g�^�C���A�E�g�擾�@**FUMIYA
	GetWinSousaInt("Wsft", "timeoutsec", &lData);
	SockInfo.IniInfo.timeout = lData;

	// INI�t�@�C�����f�t�H���g���g���C�񐔎擾�@**FUMIYA
	GetWinSousaInt("Wsft", "retrycount", &lData);
	SockInfo.IniInfo.retrycount = lData;

	return 0;
}

/*--------------------------------------------------------------/
					�v���g�R������������

	[�֐���]
		wsft_start

	[����]
		HWND hwnd	:	�ďo���̃E�B���h�E�n���h��

	[�߂�l]
		== 0		:	����
		!= 0		:	�G���[(�G���[�ԍ�)

/--------------------------------------------------------------*/
int __stdcall wsft_start(HWND hwnd)
{
	int	ret = 0 ;

	//	���O�o��
	TraceI("wsft_start() : �������������s���܂��B");

	//	�f�t�H���g�l�Őݒ肵�Ă���
	SockInfo.rPort = SockInfo.IniInfo.rPort ;
	SockInfo.wPort = SockInfo.IniInfo.wPort ;
	SockInfo.timeout = SockInfo.IniInfo.timeout ;
	SockInfo.retrycount = SockInfo.IniInfo.retrycount ;

	//	�T�[�o�[�����ݒ�t�@�C���̓��e�ŏ���������
	SockInfo.saAddr = getIPAddr( SockInfo.Addr );

	//	�V�[�P���X�ԍ���������
	SockInfo.seqnum = 0 ;
	SockInfo.StartFlag = TRUE  ;

	return ret ;
}

/*--------------------------------------------------------------/
		�ʐM�ΏۃT�[�oIP�A�h���X�̐ݒ�(str ����o�^)

	[�֐���]
		wsft_set_serveraddr_str

	[����]
		char				*str	:	�ΏۃT�[�o�[IP�̕�����

	[�߂�l]
		== 0		:	����
		!= 0		:	�G���[(�G���[�ԍ�)

/--------------------------------------------------------------*/
int wsft_set_serveraddr_str(const char *addrstr)
{
	int		ret = 0;

	//	�T�[�o�[�̐ݒ�
	//	������̕ۑ�����IP�A�h���X�����߂�
	strcpy(SockInfo.Addr, addrstr);

	//
	SockInfo.saAddr = getIPAddr( SockInfo.Addr );
	if (SockInfo.saAddr == 0xFFFFFFFF) {
		TraceE("wsft_setserv(): �z�X�g��[%s]�͉����ł��܂���ł���", SockInfo.Addr);
		ret = WSFTERR_OPTION_HOSTNAME;
	}

	return ret ;
}

/*--------------------------------------------------------------/
		�ʐM�ΏۃT�[�oIP�A�h���X�̐ݒ�(u_long ����o�^)

	[�֐���]
		wsft_set_serveraddr

	[����]
		u_long				addr	:	�ΏۃT�[�o�[IP

	[�߂�l]
		== 0		:	����
		!= 0		:	�G���[(�G���[�ԍ�)

/--------------------------------------------------------------*/
int wsft_set_serveraddr(u_long addr)
{
	int		ret = 0;
	char	addrstr[64];

	//	u_long�ŃZ�b�g���ꂽ�A�h���X�𕶎���ɕϊ�����
	//	wsft_setservstr()���ŃR�[������Ă��� inet_addr() ��
	//	0xAABBCCDD�ł�����ɓ��삷��
	sprintf(addrstr, "0x%08lx", addr);
	return wsft_set_serveraddr_str(addrstr);
}

/*--------------------------------------------------------------/
					�ʐM�ΏۃT�[�o�[�̐ݒ�

	[�֐���]
		wsft_setserv

	[����]
		char			*servstr	:	�ΏۃT�[�o�[IP�̕�����
		int				rport		:	�ǂݍ��ݗp�|�[�g�ԍ�
		int				wport		:	�������ݗp�|�[�g�ԍ�

	[�߂�l]
		== 0		:	����
		!= 0		:	�G���[(�G���[�ԍ�)

/--------------------------------------------------------------*/
int _stdcall wsft_setservstr(const char *servstr, int rport, int wport)
{
	int	ret = 0;

	//	�K��wsft_start���Ăяo���K�v������
	if( SockInfo.StartFlag == FALSE ){
		TraceE("wsft_setservstr() : �������������s���Ă��܂���B" ) ;
		ret = WSFTERR_INIT_NO_INIT ;
		return ret ;
	}

	ret = wsft_set_serveraddr_str(servstr);			// ������ŃT�[�oIP�ݒ�
	if (ret != 0)	return ret;

	ret = wsft_setopt(WSFT_OPT_RPORT, rport);		// �Ǎ��݃|�[�g
	if (ret != 0)	return ret;

	ret = wsft_setopt(WSFT_OPT_WPORT, wport);		// �����݃|�[�g
	return ret ;
}
/*--------------------------------------------------------------/
					�ʐM�ΏۃT�[�o�[�̐ݒ�

	[�֐���]
		wsft_setserv

	[����]
		unsigned long	addr	:	�ΏۃT�[�o�[�̃A�h���X
		int				rport	:	�ǂݍ��ݗp�|�[�g�ԍ�
		int				wport	:	�������ݗp�|�[�g�ԍ�

	[�߂�l]
		== 0		:	����
		!= 0		:	�G���[(�G���[�ԍ�)

/--------------------------------------------------------------*/
int _stdcall wsft_setserv(u_long addr, int rport, int wport)
{
	int	ret = 0;

	//	�K��wsft_start���Ăяo���K�v������
	if( SockInfo.StartFlag == FALSE ){
		TraceE("wsft_setserv() : �������������s���Ă��܂���B" ) ;
		ret = WSFTERR_INIT_NO_INIT ;
		return ret ;
	}

	ret = wsft_set_serveraddr(addr);				// ���l�ŃT�[�oIP�ݒ�
	if (ret != 0)	return ret;

	ret = wsft_setopt(WSFT_OPT_RPORT, rport);		// �Ǎ��݃|�[�g
	if (ret != 0)	return ret;

	ret = wsft_setopt(WSFT_OPT_WPORT, wport);		// �����݃|�[�g
	return ret ;
}

/*--------------------------------------------------------------/
				�^�C���A�E�g�Ȃǂ̃I�v�V�����̐ݒ�

	[�֐���]
		wsft_setopt

	[����]
		char	*opt	:	�ݒ肷��I�v�V�����̎w��
		int		value	:	�ݒ肷��I�v�V�����̒l

	[�߂�l]
		== 0		:	����
		!= 0		:	�G���[(�G���[�ԍ�)

/--------------------------------------------------------------*/
int _stdcall wsft_setopt(char *opt, u_long value)
{
	int	ret = 0 ;

	//	�K��wsft_start���Ăяo���K�v������
	if( SockInfo.StartFlag == FALSE ){
		TraceE("wsft_setopt() : �������������s���Ă��܂���B" ) ;
		ret = WSFTERR_INIT_NO_INIT ;
		return ret ;
	}

	//	�ʐM��A�h���X�̐ݒ�
	if( stricmp( opt, WSFT_OPT_SAADDR ) == 0 ){
		TraceD("wsft_setopt() : �ʐM����T�[�o�[�� [%ld] �ɐݒ肵�܂��B", value ) ;
		ret = wsft_set_serveraddr(value);

	//	�ǂݍ��݃|�[�g�ύX
	} else if( stricmp( opt, WSFT_OPT_RPORT ) == 0 ){
		// �Ǎ��݃|�[�g�ԍ��ݒ�
		// 0: �f�t�H���g�l�g�p / 1-65535: �ݒ� / ���̑�: �G���[(�|�[�g�͕ύX���Ȃ�)
		if (value == 0) {
			SockInfo.rPort = SockInfo.IniInfo.rPort;
		} else if (1 <= value && value <= 65535) {
			SockInfo.rPort = (u_short)value;
		} else {
			TraceE("wsft_setopt(): �Ǎ��݃|�[�g[%d]�́A�����B�������܂����B", value);
			ret = WSFTERR_OPTION_RPORT;
		}

	//	�������݃|�[�g�ύX
	} else if( stricmp( opt, WSFT_OPT_WPORT ) == 0 ){
		// �����݃|�[�g�ԍ��ݒ�
		// 0: �f�t�H���g�l�g�p / 1-65535: �ݒ� / ���̑�: �G���[(�|�[�g�͕ύX���Ȃ�)
		if (value == 0) {
			SockInfo.wPort = SockInfo.IniInfo.wPort;
		} else if (1 <= value && value <= 65535) {
			SockInfo.wPort = (u_short)value;
		} else {
			TraceE("wsft_setopt(): �����݃|�[�g[%d]�́A�����B�������܂����B", value);
			ret = WSFTERR_OPTION_WPORT;
		}

	//	�^�C���A�E�g����
	} else if( stricmp( opt, WSFT_OPT_TIMEOUT ) == 0 ){
		if (SockInfo.timeout != (int)value) {
			// �l�ύX���̂݃��O�Ɏc��
			TraceF("wsft_setopt() : �^�C���A�E�g���Ԃ� [%ld] �ɐݒ肵�܂��B", value ) ;
		}
		SockInfo.timeout = value ;

	//	���g���C��
	} else if( stricmp( opt, WSFT_OPT_RETRY ) == 0 ){
		TraceF("wsft_setopt() : ���g���C�񐔂� [%ld] �ɐݒ肵�܂��B", value ) ;
		if( value <= 0 ){
			TraceE("wsft_setopt() : ���g���C�񐔂ɔ͈͊O�̒l [%ld] ���w�肳��܂����B", value ) ;
			return WSFTERR_OPTION_RETRY ;
		}else{
			SockInfo.retrycount = value ;
		}

	//	�Y�����鍀�ږ������݂��Ȃ��Ƃ�
	} else {
		TraceE("wsft_setopt() : �w�肳�ꂽ�ݒ荀��<%s>�̓T�|�[�g����Ă��܂���B", opt ) ;
		ret = WSFTERR_OPTION_NAME ;
	}

	return ret ;
}


/*--------------------------------------------------------------/
			�^�C���A�E�g�Ȃǂ̃I�v�V�����̐ݒ�l���擾

	[�֐���]
		wsft_getopt

	[����]
		char	*opt	:	�擾����I�v�V�����̎w��

	[�߂�l]
		�w�肳�ꂽ�I�v�V�����̐ݒ�l

/--------------------------------------------------------------*/
u_long _stdcall wsft_getopt(const char* opt)
{
	unsigned long	ret = 0 ;

	//	�K��wsft_start���Ăяo���K�v������
	if( SockInfo.StartFlag == FALSE ){
		TraceE("wsft_getopt() : �������������s���Ă��܂���B" ) ;
		ret = -1 ;
		return ret ;
	}
	//	<--- �����܂�

	//	�ʐM��A�h���X�̐ݒ�
	if( stricmp( opt, WSFT_OPT_SAADDR ) == 0 ){
		ret = SockInfo.saAddr ;
		TraceD("wsft_getopt() : �ʐM��A�h���X [%ld] ��Ԃ��܂��B", ret ) ;

	//	�ǂݍ��݃|�[�g�ύX
	} else if( stricmp( opt, WSFT_OPT_RPORT ) == 0 ){
		ret = SockInfo.rPort ;
		TraceD("wsft_getopt() : �ǂݍ��ݗp�|�[�g�ԍ� [%ld] ��Ԃ��܂��B", ret ) ;

	//	�������݃|�[�g�ύX
	} else if( stricmp( opt, WSFT_OPT_WPORT ) == 0 ){
		ret = SockInfo.wPort ;
		TraceD("wsft_getopt() : �������ݗp�|�[�g�ԍ� [%ld] ��Ԃ��܂��B", ret ) ;

	//	�^�C���A�E�g����
	} else if( stricmp( opt, WSFT_OPT_TIMEOUT ) == 0 ){
		ret = SockInfo.timeout ;
		TraceD("wsft_getopt() : �^�C���A�E�g���� [%ld] ��Ԃ��܂��B", ret ) ;

	//	���g���C��
	} else if( stricmp( opt, WSFT_OPT_RETRY ) == 0 ){
		ret = SockInfo.retrycount ;
		TraceD("wsft_getopt() : ���g���C�� [%ld] ��Ԃ��܂��B", ret ) ;

	//	�Y�����鍀�ږ������݂��Ȃ��Ƃ�
	} else {
		TraceE("wsft_getopt() : �w�肳�ꂽ�ݒ荀��<%s>�̓T�|�[�g����Ă��܂���B", opt ) ;
		//	�G���[�l��-1��Ԃ�
		ret = -1 ;
	}

	return ret ;
}


/*---------------------------------------------------------------------------/
						�t�@�C�����擾

	[�֐���]
		wsft_infofile

	[����]
		unsigned short	fid		:	�����擾����t�@�C����smf�t�@�C���ԍ�
		unsigned short	rec		:	���R�[�h�w��(0�̎��͑S���R�[�h)
		unsigned long	*fsize	:	�t�@�C���i���R�[�h�j�T�C�Y��Ԃ�
		unsigned long	*fr		:	�ʐM���̃t���[������Ԃ�
		unsigned long	*flag	:	�t���O��Ԃ�
	[�߂�l]
		== 0		:	����
		!= 0		:	�G���[(�G���[�ԍ�)

/--------------------------------------------------------------------------*/
int _stdcall wsft_infofile(u_short fid, u_short rec, u_long* fsize, u_short *fr, u_short *flag)
{
	//	�K��wsft_start���Ăяo���K�v������
	if (SockInfo.StartFlag == FALSE) {
		TraceE("wsft_infofile() : �������������s���Ă��܂���B" ) ;
		return WSFTERR_INIT_NO_INIT;
	}

	TraceD("wsft_infofile() : �t�@�C�������擾���܂��B �t�@�C���ԍ�=[%ld], ���R�[�h=[%ld]", fid, rec);

	int ret = SetServer(&SockInfo, SockInfo.rPort);		// �\�P�b�g�̍쐬
	if (ret == 0) {
		//	�t�@�C���ʐM
		ret = com_infofile(&SockInfo, fid, rec, fsize, fr, flag, 'F');
		//	�\�P�b�g�̉��
		CloseSocket( &SockInfo ) ;
	}

	TraceD("wsft_infofile() : �߂�l (%ld) �Ŋ֐�[wsft_infofile]���I�����܂��B", ret ) ;

	return ret ;
}


/*----------------------------------------------------------------------------------/
									�t�@�C����ǂݍ���

	[�֐���]
		wsft_readfile

	[����]
		unsigned short	fid		:	�ǂݍ��ރt�@�C����smf�t�@�C���ԍ�
		unsigned short	rec		:	���R�[�h�w��(0�̎��͑S���R�[�h=�t�@�C��)
		char			*buf	:	�o�b�t�@
		long			bufsize	:	�o�b�t�@�T�C�Y

	[�߂�l]
		==0						:	����I��
		!=0						:	�G���[�i�G���[�ԍ��j

/----------------------------------------------------------------------------------*/
int _stdcall wsft_readfile(u_short fid, u_long rec, char* buf, u_long bufsize)
{
	int	ret = 0 ;

	//	�K��wsft_start���Ăяo���K�v������
	if( SockInfo.StartFlag == FALSE ){
		TraceE("wsft_readfile() : �������������s���Ă��܂���B" ) ;
		ret = WSFTERR_INIT_NO_INIT ;
		return ret ;
	}

	TraceD("wsft_readfile() : �t�@�C�����擾���܂��B �t�@�C���ԍ�=[%ld], ���R�[�h=[%ld]", fid, rec ) ;

	//	�T�[�o�[�ݒ�
	ret = SetServer(&SockInfo, SockInfo.rPort ) ;


	// Return file size on SMF in (long*)buf, if bufsize is 0
	if (bufsize == 0) {
		u_long fsize;
		u_short fr, flag;
		ret = com_infofile( &SockInfo, fid, rec, &fsize, &fr, &flag, 'F' ) ;
		if( ret != 0 ){
			TraceD("com_readfile() : com_infofile�֐���߂�l (%d) �ŏI�����܂��B", ret ) ;
		} else {
			*(long*)buf = fsize;
		}
		return ret ;
	}

	if( ret == 0 ){
		//	�t�@�C���ʐM
		ret = com_readfile( &SockInfo, fid, rec, buf, bufsize, 'F' ) ;
		//	�\�P�b�g�̉��
		CloseSocket( &SockInfo ) ;
	}

	TraceD("wsft_readfile() : �߂�l (%ld) �Ŋ֐�[wsft_readfile]���I�����܂��B", ret ) ;

	return ret ;
}

//*******************************************************************************
// �֐����Fwsft_read_rec
// �߂�l�F==0 ����	!=0 �װ����
// ��  ���Ffid		̧�ٔԍ�
//		   starec   �ǂݎn��ں���No(�O�͎w��ł��܂���)
//		   endrec   �ǂݏI���ں���No(�O�͎w��ł��܂���)
//		   buf		�ǂݍ����ޯ̧
//		   recsize  1ں��޻���
// ��  �e�F�����Ŏw�肳�ꂽ�ǂݎn��ں��ނ���ǂݏI���ں��ނ�ǂݍ��݂܂��B
// �쐬���F02.03.14 M.NAKAYAMA
//********************************************************************************
int _stdcall wsft_read_rec( unsigned short fid, unsigned long starec,
						   unsigned long endrec, char *buf, unsigned long recsize )
{
	int	ret = 0 ;
	unsigned long i = 0;
	unsigned long cnt = 0;
	unsigned long bufsize = 0;
	unsigned long rec = 0;

	//	�K��wsft_start���Ăяo���K�v������
	if( SockInfo.StartFlag == FALSE )
	{
		TraceE("wsft_readfile() : �������������s���Ă��܂���B" ) ;
		ret = WSFTERR_INIT_NO_INIT ;
		return ret ;
	}

//�ǂݍ���ں��ތ������擾
	//�ǂݎn�߂��ǂݏI��肪�O���Ƃ��߁I
	if( (starec > endrec) || starec == 0 || endrec == 0 )
	{
		TraceD("wsft_read_rec() : �����������ł�" ) ;
		return -1;
	}
	//ں��ތ���
	cnt = endrec - starec;

	TraceD("wsft_read_rec() : �t�@�C�����擾���܂��B �t�@�C���ԍ�=[%ld], ���R�[�h=[%ld]�`[%ld]", fid, starec, endrec ) ;

	//	�T�[�o�[�ݒ�
	ret = SetServer(&SockInfo, SockInfo.rPort ) ;

	// ���R�[�h���܂Ƃ߂ēǂݍ��ފ֐��ǉ��B October 4, 2003 r.kojima
	ret = com_readfile2(&SockInfo, fid, starec, recsize, cnt + 1, buf, recsize * (cnt + 1), 'F');

	//	�\�P�b�g�̉��
	CloseSocket( &SockInfo ) ;

	TraceD("wsft_read_rec() : �߂�l (%ld) �Ŋ֐�[wsft_read_rec]���I�����܂��B", ret ) ;

	return ret ;
}

/*----------------------------------------------------------------------------------/
								�t�@�C������������

	[�֐���]
		wsft_writefile

	[����]
		unsigned short	fid		:	�������ރt�@�C����smf�t�@�C���ԍ�
		unsigned short	rec		:	���R�[�h�w��(0�̎��͑S���R�[�h=�t�@�C��)
		char			*buf	:	�o�b�t�@
		unsigned long	bufsize	:	�o�b�t�@�T�C�Y

	[�߂�l]
		==0						:	����I��
		!=0						:	�G���[�i�G���[�ԍ��j

/----------------------------------------------------------------------------------*/
int _stdcall wsft_writefile(u_short fid, u_short rec, char* buf, u_long bufsize)
{
	int	ret = 0 ;
	short	idx ;
	long	stime ;

	static std::map<int, DWORD> LastWriteTime;


	//	�K��wsft_start���Ăяo���K�v������
	if( SockInfo.StartFlag == FALSE ){
		TraceE("wsft_writefile() : �������������s���Ă��܂���B" ) ;
		ret = WSFTERR_INIT_NO_INIT ;
		return ret ;
	}

	idx = FileNoToIndex( fid ) ;
	//	�O��̏������ݗv������̌o�ߎ��Ԃ��擾����
	stime = (GetTickCount() - LastWriteTime[idx]) ;
	if (stime < 3000) {	// 3�b�o�߂��ĂȂ��Ƃ��͑҂�
		TraceD("wsft_writefile() : �O��̏������ݗv������R�b�ȏ�o�߂��Ă��Ȃ����߂R�b�ԑҋ@���܂��B");
		Sleep(4000 - stime);	// �]�T��������4�b���̑҂����Ԃ�p��
	}

	TraceW("wsft_writefile(fid:=%u, rec:=%u, buf:=0x%p, bufsize:=%u): write to %s (port %u)",
		fid, rec, buf, bufsize, inet_ntoa(*(IN_ADDR*)&SockInfo.saAddr), SockInfo.wPort);

	//TraceW("wsft_writefile(): fid=%u rec=%u bufsize=%u", fid, rec, bufsize);

	//	�T�[�o�[�ݒ�
	//TraceW("wsft_writefile(): �������ݐ�...server=%s port=%ld.",inet_ntoa(*(IN_ADDR*)&SockInfo.saAddr), SockInfo.wPort);

//	SockInfo.timeout += 3000;	// FROM�A�N�Z�X�̉\��������̂�+3sec�̃^�C���A�E�g�ɕύX
	ret = SetServer( &SockInfo, SockInfo.wPort ) ;
//	SockInfo.timeout -= 3000;	// ���ɔ����Ă������Ɩ߂��Ă���

	if( ret == 0 ){
		//	�t�@�C���ʐM
		ret = com_writefile( &SockInfo, fid, rec, buf, bufsize, 'F' ) ;
		//	�\�P�b�g�̉��
		CloseSocket( &SockInfo ) ;
		//	�Ō�̍X�V���s�������Ԃ��擾���Ă���
		LastWriteTime[idx] = GetTickCount() ;
	}

	TraceD("wsft_writefile() : �߂�l (%ld) �Ŋ֐�[wsft_writefile]���I�����܂��B", ret ) ;

	return ret ;
}


/*----------------------------------------------------------------------------------/
							�A�h���X�w��ɂ��t�@�C���擾

	[�֐���]
		wsft_readaddr

	[����]
		unsigned long	address	:	�ǂݍ��ރA�h���X
		unsigned short	size	:	�ǂݍ��ރT�C�Y
		char			*buf	:	�o�b�t�@
		unsigned long	bufsize	:	�o�b�t�@�T�C�Y

	[�߂�l]
		==0						:	����I��
		!=0						:	�G���[�i�G���[�ԍ��j

/----------------------------------------------------------------------------------*/
int _stdcall wsft_readaddr(u_long address, u_short size, char* buf, u_long bufsize)
{
	int	ret = 0 ;

	//	�K��wsft_start���Ăяo���K�v������
	if( SockInfo.StartFlag == FALSE ){
		TraceE("wsft_readaddr() : �������������s���Ă��܂���B" ) ;
		ret = WSFTERR_INIT_NO_INIT ;
		return ret ;
	}

	TraceD("wsft_readfile() : �A�h���X�w��ɂ��t�@�C���擾���J�n���܂��B �J�n�A�h���X=[%ld], �T�C�Y=[%ld]", address, size ) ;

	//	�T�[�o�[�ݒ�
	ret = SetServer( &SockInfo, SockInfo.rPort ) ;

	if( ret == 0 ){
		//	�t�@�C���ʐM(�A�h���X)
		ret = com_readfile( &SockInfo, address, size, buf, bufsize, 'A' ) ;
		//	�\�P�b�g�̉��
		CloseSocket( &SockInfo ) ;
	}

	TraceD("wsft_readaddr() : �߂�l (%ld) �Ŋ֐�[wsft_readaddr]���I�����܂��B", ret ) ;

	return ret ;
}


/*----------------------------------------------------------------------------------/
							�A�h���X�w��ɂ�鏑������

	[�֐���]
		wsft_writeaddr

	[����]
		unsigned short	fid		:	�������ރt�@�C����smf�t�@�C���ԍ�
		char			*buf	:	�o�b�t�@
		unsigned long	bufsize	:	�o�b�t�@�T�C�Y

	[�߂�l]
		==0						:	����I��
		!=0						:	�G���[�i�G���[�ԍ��j

/----------------------------------------------------------------------------------*/
int _stdcall wsft_writeaddr(u_long address, char* buf, u_long bufsize)
{
	int	ret = 0 ;

	//	�K��wsft_start���Ăяo���K�v������
	if( SockInfo.StartFlag == FALSE ){
		TraceE("wsft_writeaddr() : �������������s���Ă��܂���B" ) ;
		ret = WSFTERR_INIT_NO_INIT ;
		return ret ;
	}

	TraceD("wsft_writefile() : �A�h���X�w��ɂ��t�@�C���������݂��J�n���܂��B �J�n�A�h���X=[%ld], �T�C�Y=[%ld]", address, bufsize ) ;

	//	�T�[�o�[�ݒ�
	ret = SetServer( &SockInfo, SockInfo.wPort ) ;

	if( ret == 0 ){
		//	�t�@�C���ʐM
		ret = com_writefile( &SockInfo, address, bufsize, buf, bufsize, 'A' ) ;
		//	�\�P�b�g�̉��
		CloseSocket( &SockInfo ) ;
	}

	TraceD("wsft_addrfile() : �߂�l (%ld) �Ŋ֐�[wsft_addrfile]���I�����܂��B", ret ) ;

	return ret ;
}


//---------------------------------------------------------------------
// wsft�̏I��

int _stdcall wsft_end( void )
{
	int	ret = 0 ;

	//	�K��wsft_start���Ăяo���K�v������
	if( SockInfo.StartFlag == FALSE ){
		TraceE("wsft_end() : �������������s���Ă��܂���B" ) ;
		ret = WSFTERR_INIT_NO_INIT ;
		return ret ;
	}

	TraceI("wsft_end() : ���\�[�X�̊J�����s���܂��B" ) ;

	SockInfo.StartFlag = FALSE ;

	return ret ;
}

//---------------------------------------------------------------------
// �T�|�[�g����v���g�R���o�[�W������Ԃ�

u_short _stdcall wsft_getver()
{
	return SockInfo.ver;
}


long _stdcall wsft_dllversion()
{
	return 0x03141406;
}

//---------------------------------------------------------------------
//	�ʐM���ĊJ����
//
//	�����׸ނ̺��۰ق�WssCom���s���܂��B

int _stdcall wsft_resume()
{
	LONG prevValue = InterlockedExchange(&g_Suspended, TRUE);
	if (prevValue != TRUE) {

		Winssa_Log(LOGC_WSFT, LOGLV_INFO, "(%s)wsft_resume(): �ʐM���ĊJ���܂��B", GetExeName().c_str());
	}

	return 0;
}

//---------------------------------------------------------------------
//	�ʐM���~����
//
//	�����׸ނ̺��۰ق�WssCom���s���܂��B

int _stdcall wsft_stop()
{
	LONG prevValue = InterlockedExchange(&g_Suspended, FALSE);
	if (prevValue != FALSE) {

		Winssa_Log(LOGC_WSFT, LOGLV_INFO, "(%s)wsft_stop(): �ʐM���~���܂��B", GetExeName().c_str());
	}

	return 0;
}

//---------------------------------------------------------------------
//  DLL_PROCESS_ATTACH����

static BOOL OnProcessAttach(HINSTANCE hinstDLL)
{
	// �Ƃ肠�������O�̏�����
	TraceXStart(NULL);

	// �ʐM�p�\���̂̃f�[�^��INI�t�@�C������Ǎ��@**FUMIYA
	ReadIniFile();

	//WinSock�̃C�j�V�����C�Y���s��
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 0);
	int rc = WSAStartup( wVersionRequested, &wsaData ) ;
	if (rc != 0) {
		TraceF("%s(%d): WinSock�̏������Ɏ��s���܂����B", __FILE__, __LINE__);
		return FALSE;
	}
	//�o�[�W�����`�F�b�N���s��
	if (wsaData.wVersion != wVersionRequested) {
		TraceF("%s(%d): WinSock�̃o�[�W�������Ⴂ�܂��B", __FILE__, __LINE__);
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------
// DLL_PROCESS_DETACH����
static BOOL OnProcessDetach(HINSTANCE hinstDLL)
{
	if (g_dwTOCount[0])
		TraceF("CloseSocket: �^�C���A�E�g������ %.3f (%u/%u)",
			(double)g_dwTOCount[1] / (double)g_dwTOCount[0] * 100.0, g_dwTOCount[1], g_dwTOCount[0]);
	else
		TraceF("CloseSocket: �^�C���A�E�g������ ... 1����ʐM���Ă��܂����");

	// �v���Z�X�̃��\�[�X��������AWinSock�̎g�p���~���܂��B�@**FUMIYA
	WSACleanup();

	return TRUE;
}

//---------------------------------------------------------------------
// DLL�̃G���g���[�|�C���g

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		return OnProcessAttach(hinstDLL);
	case DLL_PROCESS_DETACH:
		return OnProcessDetach(hinstDLL);
	}

	return TRUE;
}
