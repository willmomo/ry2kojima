//---------------------------------------------------------------------
// �T�v
//	DLL�̃G���g���[�|�C���g�ƌ��J�֐�
//
// �쐬��
//	��
//
// ���l
//
//
//---------------------------------------------------------------------
// WssLog.cpp : DLL �A�v���P�[�V�����p�̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <assert.h>
#include "resource.h"
#include "wsslog.h"

///#pragma data_seg(".shared")
long s_traceLevel = LOGLV_WARNING;	// �ʏ��WARNING�ȏ�̃��O�̂݋L�^
BOOL s_bAttached = FALSE;			// ����Attach�ς�? TRUE=�ς�
///#pragma data_seg()

#define BUFFER_SIZE	2048

static char s_buff[BUFFER_SIZE];

//#define	MTX_SYSLOG		"mtx_syslog"		// �r�����L�e�[�u��

//------------------------------------------------------------
// �C�j�t�@�C�����
//------------------------------------------------------------
char m_MTX_SYSLOG[_MAX_PATH];			// �r�����L�e�[�u����
char m_BasePath[_MAX_PATH];				// ��{�p�X
char m_LogPath[_MAX_PATH];				// ���O�p�X
int  m_nLogLevel;						// ���O ���x��
int  m_nSaveDays;						// ���O�ۑ�����

HANDLE g_hmtxSyslog;					// ���O�����ݔr���pmutex

// �_���v�p�f�[�^
BOOL m_bDump = FALSE;					// �_���v
void *m_pDumpAdr;						// �_���v�A�h���X
long m_lDumpSize;						// �_���v�T�C�Y

void GetIniFile();
BOOL MakeDirectory(LPTSTR lpPathName);
BOOL Dump_Start(FILE *fp);
BOOL DeleteOldLog();


//---------------------------------------------------------------------
// �T�v
//	�O���Ɍ��J���郍�O�֐�
//
// ����
//	const char* sikibetu	: ���O���ʕ�����
//	long  loglevel				: ���O���x��
//	const char *format		 :���O������
//
// �߂�l
//	�Ȃ�
//
// �@�\����
//	�t�H�[�}�b�g�ɏ]��������������O�t�@�C���ɏo�͂���
//
// ���l
//	�Ȃ�
//---------------------------------------------------------------------


void WINAPI Winssa_Log(const char *sikibetu,long loglevel,const char * format, ...)
{
	//---- [2003/03/19 R.Kojima] --------------------------------------
	// vsprintf�̓����ŕ��������_�����g�p����P�[�X�������Ă��A�R�[�h��
	// �ɕ��������_�����g�p����ꏊ���Ȃ��ƁA�����J�͕��������_���C�u��
	// ���������N���Ȃ��B
	// ���̏�ԂŁA������%f���g�p�����"R6002 floating point not
	// loaded"�G���[���������Ă��܂��B
	// �ϐ����g���ĕ��������_�����g�p���鏈���������āA������蕂������
	// �_���C�u�����������N������B
	volatile double force_a_linker_to_link_a_floating_point_library;

	force_a_linker_to_link_a_floating_point_library = 1.1;

	va_list arg;
	char buf[256];
	char logfname[32];

	char Path_Log[_MAX_PATH];

	int	iRet;

	if (loglevel == LOGLV_ALWAYS) {
		;										//�@���O���x���F�K�����O���o��(��).�p�ɂɋN������̂Ɋւ��Ă͎g�p�֎~
	} else if (loglevel < s_traceLevel) {			//�@���O���x�����������̂Ń��O�͂Ƃ�Ȃ�
		return;
	}

	DWORD dwRet = WaitForSingleObject(g_hmtxSyslog, 3000);
	if (dwRet == WAIT_OBJECT_0 || dwRet == WAIT_ABANDONED) {	//  ����

		va_start(arg, format);

		vsprintf(s_buff, format, arg);

		SYSTEMTIME	ptm;
		GetLocalTime(&ptm);

		sprintf(buf,"%02d:%02d:%02d [%-6s]{%d}:",
				ptm.wHour, ptm.wMinute, ptm.wSecond, sikibetu, loglevel);

		sprintf(Path_Log,"%s%04d%02d%02d", m_LogPath, ptm.wYear, ptm.wMonth, ptm.wDay);

		iRet = _access(Path_Log,0);
		if(iRet != 0){						// �L���t�H���_���Ȃ�
			MakeDirectory(Path_Log);
		}

		PathAddBackslash(Path_Log);

		//  �f�o�b�O�o��
		OutputDebugString(buf);
		OutputDebugString(s_buff);
		OutputDebugString("\n");

		//�@���O�t�@�C�������쐬
		sprintf(logfname,"%04d%02d%02d.log",
					ptm.wYear, ptm.wMonth, ptm.wDay);

		strcat(Path_Log, logfname);

		// ���O�t�@�C���ɏo��
		FILE* fp = fopen(Path_Log, "a");
		if (fp) {
			fprintf(fp, "%s", buf);
			fprintf(fp, "%s", s_buff);
			fprintf(fp, "\n");
			if (m_bDump) {
				m_bDump = FALSE;
				Dump_Start(fp);
			}
			fclose(fp);
		}

		ReleaseMutex(g_hmtxSyslog);		//  Mutex�̉��
	}
	else {
		assert( 0 );
	}

	force_a_linker_to_link_a_floating_point_library = 2.2;
}

//---------------------------------------------------------------------
// �T�v
//	�O���Ɍ��J���郍�O�֐�
//
// ����
//	const char* sikibetu	: ���O���ʕ�����
//	long  loglevel			: ���O���x��
//	const void *buffer		 :�_���v�f�[�^�̃A�h���X
//	long  size				 :�_���v�f�[�^�̃T�C�Y
//
// �߂�l
//	�Ȃ�
//
// �@�\����
//	�_���v�f�[�^�����O�t�@�C���ɏo�͂���
//
// ���l
//	�Ȃ�
//---------------------------------------------------------------------


void WINAPI Winssa_Dump(const char *sikibetu,long loglevel, const void* buffer, long size)
{
	m_bDump = TRUE;			// �_���v���[�h
	m_pDumpAdr = (void *)buffer;	// �_���v�A�h���X
	m_lDumpSize = size;		// �_���v�T�C�Y
	
	Winssa_Log(sikibetu, loglevel, "<Dump>Start");
}

void WINAPI WinssaVB_Dump(const char *sikibetu,long loglevel, const void* buffer, long size)
{
	Winssa_Dump(sikibetu, loglevel, buffer, size);
}

//---------------------------------------------------------------------
// �T�v
//	�O���Ɍ��J���郍�O�֐�
//
// ����
//	const char* sikibetu	: ���O���ʕ�����
//	long  loglevel				: ���O���x��
//	const char *format		 :���O������
//
// �߂�l
//	�Ȃ�
//
// �@�\����
//	�t�H�[�}�b�g�ɏ]��������������O�t�@�C���ɏo�͂���
//
// ���l
//	�Ȃ�
//---------------------------------------------------------------------

void WINAPI WinssaVB_Log(const char *sikibetu,long loglevel,const char * format)
{
	Winssa_Log(sikibetu, loglevel, format);
}

//---------------------------------------------------------------------
//	[�T�v]
//	���O���x���̕ύX

void WINAPI Winssa_Loglevel(long loglevel)
{
	WLSetLogLevel(loglevel);
}

//---------------------------------------------------------------------
//	[�T�v]
//	���O���x���̐ݒ�

void WINAPI WLSetLogLevel(long nNewLogLevel)
{
	if (nNewLogLevel < LOGLV_DEBUG || nNewLogLevel > LOGLV_FATALERR)
		return;

	InterlockedExchange(&s_traceLevel, nNewLogLevel);
}

//---------------------------------------------------------------------
//	[�T�v]
//	���O���x���̎擾

long WINAPI WLGetLogLevel()
{
	long nWork;
	InterlockedExchange(&nWork, s_traceLevel);
	return nWork;
}

//---------------------------------------------------------------------
//	[�T�v]
//	DLL�G���g���[�|�C���g

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH) {

		// ����process attach�����A���O���x����ݒ肷��
		if (s_bAttached == FALSE) {
			s_bAttached = TRUE;
			GetIniFile();
			WLSetLogLevel(m_nLogLevel);

			g_hmtxSyslog = CreateMutex(NULL, FALSE, m_MTX_SYSLOG);	// ���O�t�@�C���pmutex

			// �ۑ������𒴂������O���폜
			if (m_nSaveDays > 0) {
				DeleteOldLog();
			}
		}

	} else if (fdwReason == DLL_PROCESS_DETACH) {
		if (g_hmtxSyslog != NULL) {
			ReleaseMutex(g_hmtxSyslog);
			CloseHandle(g_hmtxSyslog);
		}
	}

    return TRUE;
}

//---------------------------------------------------------------------
//	[�T�v]
//	�������t�@�C���iWinSousa.ini�j���烍�O�p�X�ƃ��O���x���̎擾

void GetIniFile()
{
	char cPath[_MAX_PATH];
	char cDrive[_MAX_DRIVE];
	char cDir[_MAX_DIR];
	char cName[_MAX_FNAME];
	char cLogPath[_MAX_PATH];

	// ���W���[���̃t���p�X���擾����B 
	GetModuleFileName (NULL, cPath, _MAX_PATH);

	// �p�X�����\���v�f�ɕ�������B
	_splitpath( cPath, cDrive, cDir, cName, NULL );

	// ���W���[���i�[�f�B���N�g�����Z�b�g����B
	strcpy(m_BasePath, cDrive);
	strcat(m_BasePath, cDir);

	// VB�̏ꍇ�̓J�����g�f�B���N�g���Ƃ���B
	// "C:\Program Files\Microsoft Visual Studio\VB98\VB6.EXE"
	// "C:\Program Files\Microsoft Visual Studio\"�̃t�@�C����ǂ݂ɂ����̂�h���B
	if (_stricmp(cName, "vb6") == 0) {
		// �J�����g�f�B���N�g�����Z�b�g����B
		GetCurrentDirectory(_MAX_PATH, m_BasePath);
		strcat(m_BasePath, "\\");
	}

	// WinSousa.ini���Z�b�g����B
	strcpy(cPath, m_BasePath);
	strcat(cPath, "WinSousa.ini");

	// ���O�o�`�s�g�̎擾
	GetPrivateProfileString("Log", "LogPath", "", cLogPath, _MAX_PATH, cPath);
	if (strlen(cLogPath) == 0) {
		GetPrivateProfileString("Pathinf", "LogPath", "Log", cLogPath, _MAX_PATH, cPath);
	}

	// ���O�t���p�X��.\Log\
	// ��{�p�X�Z�b�g
	strcpy(m_LogPath, m_BasePath);
	PathAddBackslash(m_LogPath);
	//���O�o�`�s�g�Z�b�g
	strcat(m_LogPath, cLogPath);
	PathAddBackslash(m_LogPath);

	// LogLevel�̎擾
	m_nLogLevel = GetPrivateProfileInt("Log", "LogLevel", 2, cPath);

	// �ۑ������̎擾
	m_nSaveDays = GetPrivateProfileInt("Log", "SaveDays", 90, cPath);

	// �~���[�e�b�N�X�����쐬����B(C:\temp\temp.exe �� mtx_C:/temp/)
	// �~���[�e�b�N�X���@"mtx_" + m_BasePath
	strcpy(m_MTX_SYSLOG, "mtx_");
	strcat(m_MTX_SYSLOG, m_BasePath);

	// �~���[�e�b�N�X����[\]���g�p�ł��Ȃ����߁A�@[\]��[/]�ɒu��������
	for (int i=0 ; i<(int)strlen(m_MTX_SYSLOG) ; i++) {
		if (m_MTX_SYSLOG[i] == '\\')
			m_MTX_SYSLOG[i] = '/';
	}
}

//---------------------------------------------------------------------
//	[�T�v]
//	 ���K�w�f�B���N�g���̍쐬�B
//
// �߂�l
//	TRUE:����@FALSE:�ُ�
//
BOOL MakeDirectory(LPTSTR lpPathName)
{
	int	iRet;

	// �f�B���N�g���̗L���`�F�b�N
	iRet = _access(lpPathName,0);
	// �f�B���N�g�������݂Ȃ� �߂�
	if (iRet == 0) return TRUE;

	int i,iLen;
	char cDir[_MAX_PATH];

	strcpy(cDir, lpPathName);
	PathAddBackslash(cDir);
	iLen = strlen(cDir);

	// �K�w�f�B���N�g���������쐬����
	for (i=4 ; i<iLen ; i++) {
		if (cDir[i] == '\\' || cDir[i] == '/') {
			cDir[i] = 0;
			// �f�B���N�g���̗L���`�F�b�N
			iRet = _access(cDir,0);
			// �Ȃ���΍쐬����
			if (iRet != 0) {
				// �f�B���N�g����V�K�쐬
				iRet = _mkdir(cDir);
				// ���s������G���[�Ŗ߂�
				if (iRet != 0) return FALSE;
			}
			cDir[i] = '\\';
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------
//	[�T�v]
//	 �f�[�^���_���v�`���ŏo�͂���
//
// �߂�l
//	TRUE:����@FALSE:�ُ�
//
BOOL Dump_Start(FILE *fp)
{
	// �w�b�_�[���̏o��
	char cHead[] = "     +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F 0123456789ABCDEF \n";
	OutputDebugString(cHead);
	fprintf(fp, cHead);
	
	int i, ix;
	char cwk[10];
	char *cHex = &cHead[5];
	char *cAsc = &cHead[5+16*3];
	BOOL bKanji = FALSE;
	unsigned char* pstr = (unsigned char*)m_pDumpAdr;
	long size = m_lDumpSize;

	memset(cHex, 0 , sizeof(cHex));
	memset(cAsc, 0 , sizeof(cAsc));
	for (i=0 ; i<size ; i++, pstr++) {
		ix = i % 16;
		if (ix == 0) {
			memset(cHead, ' ', sizeof(cHead)-2);
			sprintf(cwk, "%04X", i);
			memcpy(cHead, cwk, 4);
		}
		sprintf(cwk, "%02X", *pstr);
		memcpy(&cHex[ix*3], cwk, 2);

		if (!bKanji) {
			if (IsDBCSLeadByte(*pstr)) {	// �����̂P�o�C�g�ځH
				if (i+1 < size) {
					cAsc[ix] = *pstr;
					cAsc[ix+1] = *(pstr+1);
					bKanji = !bKanji;
				}
				else {
					cAsc[ix] = '.';
				}
			}
			else if (!iscntrl(*pstr)) {	//���䕶��
				cAsc[ix] = *pstr;
			}
			else {
				cAsc[ix] = '.';
			}
		}
		else {
			bKanji = !bKanji;
		}
		if (ix == 15 || i+1 == size) {
			OutputDebugString(cHead);
			fprintf(fp, cHead);
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------
// �T�v
//	�ۑ����t�̎擾
//
// �߂�l
//	�Ȃ�
//
void GetSaveDate(char *cSaveDate)
{
	time_t lNow;
	struct tm stTm;
	
	// �V�X�e�������𓾂܂��B
	time( &lNow );

	// �����l�����n�����ɕϊ����܂��B
	stTm = *localtime( &lNow );

	// ���t�̈����Z
	stTm.tm_mday -= m_nSaveDays;

	// �����\���� (�s���S�ȏꍇ������܂�) �𐳋K�����ꂽ�l�������S�ɒ�`���ꂽ�\���̂ɕϊ�����
	mktime(&stTm);

	sprintf(cSaveDate,"%04d%02d%02d", stTm.tm_year+1900, stTm.tm_mon+1, stTm.tm_mday);
}

//---------------------------------------------------------------------
// �T�v
//	���t���r���āA�ۑ����Ԃ��Â��t�@�C�����폜����
//
// �߂�l
//	�Ȃ�
//
BOOL DeleteLog(const char *cFileName, const char* cSaveDate)
{
	char cDir[_MAX_PATH];
	char cPath[_MAX_PATH];

	//"20"��T��
	if (strlen(cFileName)==8 && memcmp(cFileName, "20", 2) == 0) {
		//�t�@�C�����̓��t���ۑ����Ԃ��Â��Ȃ�
		if (strcmp(cFileName, cSaveDate) <= 0) {
			// �f�B���N�g�����̍쐬
			sprintf(cDir, "%s%s", m_LogPath, cFileName);
			// �t�@�C�����̍쐬
			sprintf(cPath, "%s%s\\%s.log", m_LogPath, cFileName, cFileName);

			// �t�@�C�����폜����
			if (_unlink(cPath) == -1) {
				Winssa_Log("WSSLOG", LOGLV_WARNING, "�t�@�C�����폜�ł��܂���B�t�@�C��=[%s]", cPath);
				return FALSE;
			}
			//�f�B���N�g�����폜����
			if (_rmdir(cDir) == -1) {
				Winssa_Log("WSSLOG", LOGLV_WARNING, "�f�B���N�g�����폜�ł��܂���B�f�B���N�g��=[%s]", cDir);
				return FALSE;
			}
		}
	}
	return TRUE;
}


//---------------------------------------------------------------------
// �T�v
//	�ۑ����Ԃ𒴂���Log�t�@�C���̍폜
//
// �߂�l
//	TRUE:����@FALSE:�ُ�
//
BOOL DeleteOldLog()
{
	// �ۑ����t�̎擾
	char cSaveDate[10];
	GetSaveDate(cSaveDate);

	// ���O�i�[�t�H���_�̃e���v���[�g��ǉ�����
	char cFind[_MAX_PATH];
	sprintf(cFind, "%s*", m_LogPath);

    // �w�肵���p�^�[���̃t�@�C����T���܂��B 
    struct _finddata_t stFind;
    long hFile;

    hFile = _findfirst( cFind, &stFind );
	if (hFile != -1L ) {
		do {
			//�ۑ����Ԃ𒴂����t�@�C�����폜����
			DeleteLog(stFind.name, cSaveDate);
		}
		// �c��̃t�@�C����T���܂��B
		while( _findnext( hFile, &stFind ) == 0 );

		//���\�[�X���J������
		_findclose( hFile );
	}

	return TRUE;
}

