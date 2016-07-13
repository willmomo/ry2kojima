
#pragma once

#if !defined(__HSCP_H)
#define __HSCP_H

// hscp���K�v�Ƃ���w�b�_

#include <stdio.h>
#include <memory.h>
#include <windows.h>
#include <shlwapi.h>
#include <winbase.h>
#include <process.h>
#include <lm.h>
#include <ctime>
#include <assert.h>
#include "wsslog.h"

// export/import �}�N��

#if defined(HSCP_EXPORTS)
#define HSCP_API	__declspec(dllexport)
#else
#define HSCP_API	__declspec(dllimport)
#endif

////////// �v���O�������R�����g���̓}�N�� //////////
/*
	�ȉ��̂悤�ɁA�v���O�������ɃR�����g�����Ă����ƁF

	#pragma hsMSG(������TODO�����Ă���)

	�R���p�C�����ɁA�ȉ��̂悤�ȃ��b�Z�[�W���o�͂���܂��F

	�\�[�X�t�@�C����(�s�ԍ�) : ������TODO�����Ă���

	���b�Z�[�W��""�ŁA�͂ޕK�v�͂���܂���B
*/

#define hsSTR2(n)	#n
#define hsSTR(n)	hsSTR2(n)
#define hsMSG(msg)	message(__FILE__ "(" hsSTR(__LINE__) ") : " hsSTR2(msg))
#define hsMSG0(msg)	message(hsSTR2(msg))	// �����Ȃ��̃��b�Z�[�W

// ���[�e�B���e�B�}�N��

#if !defined( countof )
#define countof(A)	(sizeof((A)) / sizeof((A)[0]))
#endif

// min_num <= num <= max_num �̂Ƃ��A�^��Ԃ��}�N��

#define	hsINRANGE(min_num, num, max_num)	(((min_num) <= (num)) && ((num) <= (max_num)))

// �\����

// �N���������b���r�b�g�t�B�[���h{yyyy yyym mmmd dddd hhhh hnnn nnns ssss}
// �ŕ\������`�B�N�̊J�n��1910����B���A���̊J�n����1����B�b�́A1/2�B

#if !defined(PSTM_T)
#define PSTM_T
typedef struct {
	unsigned sec	: 5;	// �b(1/2�b)
	unsigned min	: 6;	// ��(0�`59��)
	unsigned hour	: 5;	// ��(0�`23��)
	unsigned mday	: 5;	// ��(1�`31��)
	unsigned mon	: 4;	// ��(1�`12��)
	unsigned year	: 7;	// �N(0=1910�N����)
} pstm_t;
#endif
typedef pstm_t	hsPSTM;		// hscp�p�����K���ɉ��������O

// �N���������j���r�b�g�t�B�[���h{yyyy yyym mmmd dddd hhhh hnnn nnnw wwww}
// �ŕ\������`�B�N�̊J�n��1910����B���A���̊J�n����1����B

typedef struct {
	unsigned wday	: 5;	// �j(0=��)
	unsigned min	: 6;	// ��(0�`59��)
	unsigned hour	: 5;	// ��(0�`23��)
	unsigned mday	: 5;	// ��(1�`31��)
	unsigned mon	: 4;	// ��(1�`12��)
	unsigned year	: 7;	// �N(0=1910�N����)
} pwtm_t;
typedef pwtm_t	hsPWTM;		// hscp�p�����K���ɉ��������O

// hscp��export����N���X���C�u����

///////////////////////////////////////////////////////////////////////
// ���b�N�t�@�C�������S�Ɉ����N���X(hsCLockfile)

class HSCP_API hsCLockfile
{
private:
	HANDLE m_hFile;

public:
	// �R���X�g���N�^
	hsCLockfile();

	// �f�X�g���N�^
	virtual ~hsCLockfile();

public:
	// ���b�N�t�@�C���̍쐬
	BOOL Lock(const char *pszLockfile, BOOL fDeleteIfError);

	// ���b�N�t�@�C���̍쐬
	BOOL LockEx(const char* pszLockfile, BOOL fDeleteIfError, DWORD dwWaitMs, DWORD dwRetry);

	// ���b�N�t�@�C���̍폜
	BOOL Unlock();
};

// �G�N�X�|�[�g

HSCP_API BOOL WINAPI hsIsEmbeddedOS();
HSCP_API HANDLE WINAPI hsCreateLockfile(const char *pszLockfileName, BOOL fDeleteIfError);
HSCP_API HANDLE WINAPI hsCreateLockfileEx(const char* pszLockfileName, BOOL fDeleteIfError, DWORD dwWaitMs, DWORD dwRetry);
HSCP_API BOOL WINAPI hsDeleteLockfile(HANDLE hFile);
HSCP_API int WINAPI IsIniEntryExists(const char* pszSection, const char* pszKey, const char* pszIniFile);
HSCP_API int WINAPI IsSharedFolder(const char *pszServerName, const char *pszShareName);
HSCP_API int WINAPI MakePSTM(int y, int m, int d, int h, int n, int s);
HSCP_API BOOL WINAPI hsDeleteFiles(const char *pszWildCard, int nNumOfSave);
HSCP_API BOOL WINAPI hsIsSpaceStr(LPCTSTR parg);

HSCP_API double DiffFileTimeNow(const FILETIME* pFileTime);
HSCP_API double DiffPsTimeNow(unsigned long ps_tim);
HSCP_API time_t ToTimePSTM(DWORD pstm);
HSCP_API DWORD GetWin32ErrorMessage(DWORD dwError, char* pszMsg, size_t size);
HSCP_API int chomp(char* p);
HSCP_API int RemoveOldFiles(const char* pszFolder, int nDays);

HSCP_API int __stdcall RunProg(const char *cmd, const char* outfile);
HSCP_API long __stdcall	check_ip( char *strAddr);
HSCP_API long __stdcall CheckBit(long data, long bit);
HSCP_API long __stdcall SetBit(long data, long bit, long setmode);

#endif

// �萔��`
#define ANS_OK				0		// ������IP�m�F
#define ANS_NG				1		// ������IP���m�F�ł��Ȃ�
#define ANS_ERR_SYS			2		// CreateProcess�G���[
#define ANS_ERR_TMPFILE		3		// ping���ʃt�@�C���̃I�[�v���G���[

#define TMPFILENAME			"ping.tmp"		// ping���ʂ��i�[�����t�@�C��



//---------------------------------------------------------------------
// �V����GetVersionInfoEx�p�̍\���̂��g���������Avc6, vb6���ł͂��Ȃ�
// �Ȃ����߂��̃w�b�_(DLL)�Œ񋟂���B
//
// ����hs���t���ȊO�́AVS2005�̂��̂Ɠ����B
//
// ** �g���� **
//
// HS_OSVERSIONINFOEX buf;
// memset(&buf, 0, sizeof(buf));
// buf.dwOSVersionInfoSize = sizeof(buf);
// GetVersionEx((LPOSVERSIONIFNO)&buf);
//
// �Q�lURL: http://msdn2.microsoft.com/en-us/library/ms724833.aspx
//
// 2007.09.04 r.kojima
//

// VS2005��OSVERSIONINFOEX�̑���
//
typedef struct _HS_OSVERSIONINFOEX 
{
	DWORD dwOSVersionInfoSize;
	DWORD dwMajorVersion;
	DWORD dwMinorVersion;
	DWORD dwBuildNumber;
	DWORD dwPlatformId;
	TCHAR szCSDVersion[128];
	WORD wServicePackMajor;
	WORD wServicePackMinor;
	WORD wSuiteMask;
	BYTE wProductType;
	BYTE wReserved;
} HS_OSVERSIONINFOEX, *PHS_OSVERSIONINFOEX, *LPHS_OSVERSIONINFOEX;


// wSuiteMask �̒�`����HS_���t���Ă���ȊO�́AVS2005�̂��̂Ɠ���
//
#define HS_VER_SUITE_BACKOFFICE					0x00000004	// Microsoft BackOffice components are installed. 
#define HS_VER_SUITE_BLADE						0x00000400	// Windows Server 2003, Web Edition is installed. 
#define HS_VER_SUITE_COMPUTE_SERVER				0x00004000	// Windows Server 2003, Compute Cluster Edition is installed. 
#define HS_VER_SUITE_DATACENTER					0x00000080	// Windows Server 2008 Datacenter, Windows Server 2003, Datacenter Edition or Windows 2000 Datacenter Server is installed. 
#define HS_VER_SUITE_ENTERPRISE					0x00000002	// Windows Server 2008 Enterprise, Windows Server 2003, Enterprise Edition, Windows 2000 Advanced Server, 
															// or Windows NT Server 4.0 Enterprise Edition is installed. Refer to the Remarks section for more information about this bit flag. 
#define HS_VER_SUITE_EMBEDDEDNT					0x00000040	// Windows XP Embedded is installed. 
#define HS_VER_SUITE_PERSONAL					0x00000200	// Windows Vista Home Premium, Windows Vista Home Basic, or Windows XP Home Edition is installed. 
#define HS_VER_SUITE_SINGLEUSERTS				0x00000100	// Remote Desktop is supported, but only one interactive session is supported. 
															// This value is set unless the system is running in application server mode. 
#define HS_VER_SUITE_SMALLBUSINESS				0x00000001	// Microsoft Small Business Server was once installed on the system, 
															// but may have been upgraded to another version of Windows. Refer to the Remarks section for more information about this bit flag. 
#define HS_VER_SUITE_SMALLBUSINESS_RESTRICTED	0x00000020	// Microsoft Small Business Server is installed with the restrictive client license in force. 
															// Refer to the Remarks section for more information about this bit flag. 
#define HS_VER_SUITE_STORAGE_SERVER				0x00002000	// Windows Storage Server 2003 R2 or Windows Storage Server 2003 is installed. 
#define HS_VER_SUITE_TERMINAL					0x00000010	// Terminal Services is installed. This value is always set. 
															// If VER_SUITE_TERMINAL is set but VER_SUITE_SINGLEUSERTS is not set, the system is running in application server mode.
#define HS_VER_SUITE_WH_SERVER					0x00008000	// Windows Home Server is installed. 

// wProductType �̒�`�B����HS_���t���Ă���ȊO�́AVS2005�̂��̂Ɠ���
//
#define HS_VER_NT_DOMAIN_CONTROLLER	0x0000002	// The system is a domain controller. 
#define HS_VER_NT_SERVER			0x0000003	// The system is a server. Note that a server that is also a domain controller is reported as VER_NT_DOMAIN_CONTROLLER, not VER_NT_SERVER.
#define HS_VER_NT_WORKSTATION		0x0000001	// The operating system is Windows Vista, Windows XP Professional, 
												// Windows XP Home Edition, Windows 2000 Professional, or Windows NT Workstation 4.0. 
