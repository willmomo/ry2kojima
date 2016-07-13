//-----------------------------------------------------------------------------
//	DLL�̃G���g���[�|�C���g�ƌ��J�֐�
//
// �쐬��
//	��
//
// [���l]
// 2008.08.04 ����
//
// USE_THREAD �� define �����ꍇ�A�X���b�h�𗘗p�����x�������݂��s��
// ���W���[�����쐬�ł���B
// �{�i�I�Ɏg���悤�ɂȂ����ꍇ�́A�X���b�h�^�C�v�Ƃ�������Ȃ��^�C�v��
// �����������^�C���Ő؂蕪����悤�ɍH�v���~�����Ƃ���B
//
//-----------------------------------------------------------------------------
// WssLog.cpp : DLL �A�v���P�[�V�����p�̃G���g�� �|�C���g���`���܂��B
//

#pragma warning(disable : 4786)

#include <windows.h>
#include <shlwapi.h>
#include <time.h>
#include <assert.h>
#include <queue>
#include "wsslog.h"
#include "FixIniPath.h"
#include "kjmlib.h"
#include "syslogc.h"
#include "resource.h"


///#pragma data_seg(".shared")
long s_traceLevel = LOGLV_WARNING;	// �ʏ��WARNING�ȏ�̃��O�̂݋L�^
BOOL s_bAttached = FALSE;			// ����Attach�ς�? TRUE=�ς�
///#pragma data_seg()

#define BUFFER_SIZE	20480

static char s_buff[BUFFER_SIZE];

//#define	MTX_SYSLOG		"mtx_syslog"		// �r�����L�e�[�u��

//-----------------------------------------------------------------------------
// �C�j�t�@�C�����
//-----------------------------------------------------------------------------
char m_MTX_SYSLOG[MAX_PATH];			// �r�����L�e�[�u����
char m_appName[MAX_PATH];
char m_BasePath[MAX_PATH];				// ��{�p�X
char m_LogPath[MAX_PATH];				// ���O�p�X
int  m_nLogLevel;						// ���O ���x��
int  m_nSaveDays;						// ���O�ۑ�����
int  m_logNameType;						// 0 = YYYYMMDD.log , 1 = YYYYMMDD_HH.log
std::string m_syslogServer;				// if not empty, syslog �������āA���O�������o�����[�h
HANDLE g_hmtxSyslog;					// ���O�����ݔr���pmutex

// �_���v�p�f�[�^
BOOL m_bDump = FALSE;					// �_���v
void *m_pDumpAdr;						// �_���v�A�h���X
long m_lDumpSize;						// �_���v�T�C�Y

std::queue<std::string> g_logQ;
syslog_client g_syslogc;

HINSTANCE g_hinstDLL;

//-----------------------------------------------------------------------------
// syslog_client �� worker thread �̎���
//-----------------------------------------------------------------------------
unsigned syslog_client::worker() {

	int qsize;
	std::string msg;
	char logfname[32];
	char Path_Log[MAX_PATH];

	while (m_loop) {

		lock();
		if (!g_logQ.empty()) {
			msg = g_logQ.front();
			g_logQ.pop();
			qsize = g_logQ.size();
		} else {
			msg.erase();
		}
		unlock();

		if (msg.empty()) {
			Sleep(200);
			continue;
		}
#if 1
		// �����ŁAsyslog��file�o�͂�؂蕪����
#if 0
		if (!m_syslogServer.empty()) {

			printf("send sntpd %s\n", m_syslogServer.c_str());

			SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
			kjm::in_socket sock;
			if (sock.open_socket(SOCK_DGRAM)) {

				printf("s = %d\n", s);

				std::vector<wchar_t> buf2((msg.length() + 1) * 2);
				std::vector<char> buf3(buf2.size());
				MultiByteToWideChar(CP_ACP, 0, msg.c_str(), -1, buf2.begin(), buf2.size());
				WideCharToMultiByte(CP_UTF8, 0, buf2.begin(), -1, buf3.begin(), buf3.size(), NULL, NULL);

				int ret = sock.send_to(m_syslogServer.c_str(), 514, buf3.begin(), lstrlen(buf3.begin()));

				printf("sendto result = %d\n", ret);
			}
		}
#endif
		{
			DWORD dwRet = WaitForSingleObject(g_hmtxSyslog, 3000);

			if (dwRet == WAIT_OBJECT_0 || dwRet == WAIT_ABANDONED) {	//  ����
				SYSTEMTIME	ptm;
				GetLocalTime(&ptm);

				static FILETIME s_ft;
				FILETIME ft;
				SystemTimeToFileTime(&ptm, &ft);

				sprintf(Path_Log,"%s%04d%02d%02d", m_LogPath, ptm.wYear, ptm.wMonth, ptm.wDay);

				kjm::create_directory(Path_Log);

				PathAddBackslash(Path_Log);

				//  �f�o�b�O�o��
				OutputDebugString(msg.c_str());
				OutputDebugString("\n");

				//�@���O�t�@�C�������쐬
				if (m_logNameType == 1) {
					sprintf(logfname,"%04d%02d%02d_%02d.log", ptm.wYear, ptm.wMonth, ptm.wDay, ptm.wHour);
				} else {
					sprintf(logfname,"%04d%02d%02d.log", ptm.wYear, ptm.wMonth, ptm.wDay);
				}

				PathAppend(Path_Log, logfname);

				// ���O�t�@�C���ɏo��
				FILE* fp = fopen(Path_Log, "a");
				if (fp) {
					if (*(__int64*)&ft < *(__int64*)&s_ft) {
						// ���Ԃ̋t�������m
						SYSTEMTIME st1, st2;
						FileTimeToSystemTime(&s_ft, &st1);
						FileTimeToSystemTime(&ft, &st2);

						fprintf(fp, "<%02d:%02d:%02d.%03d", ptm.wHour, ptm.wMinute, ptm.wSecond, ptm.wMilliseconds);
						fprintf(fp, " %4d> ", qsize);
						fprintf(fp, "<< ���Ԃ̋t�������m���܂���: %04d/%02d/%02d %02d:%02d:%02d.%03d �� %04d/%02d/%02d %02d:%02d:%02d.%03d >>",
							st1.wYear, st1.wMonth, st1.wDay, st1.wHour, st1.wMinute, st1.wSecond, st1.wMilliseconds,
							st2.wYear, st2.wMonth, st2.wDay, st2.wHour, st2.wMinute, st2.wSecond, st2.wMilliseconds);
						fprintf(fp, "\n");
					}

					s_ft = ft;

					fprintf(fp, "<%02d:%02d:%02d.%03d", ptm.wHour, ptm.wMinute, ptm.wSecond, ptm.wMilliseconds);
					fprintf(fp, " %4d> ", qsize);
					fprintf(fp, "%s", msg.c_str());
					fprintf(fp, "\n");

					fclose(fp);
				} else {
				}

				ReleaseMutex(g_hmtxSyslog);		//  Mutex�̉��
			} else {
				assert( 0 );
			}
		}
#endif
	}

	return 0;
}


//-----------------------------------------------------------------------------
// ���O�̕ۑ���p�X���擾�B
//
// [����]
// pszBuffer  �p�X�����擾����o�b�t�@�ւ̃|�C���^
// nSize      �o�b�t�@�̃T�C�Y
//
// [�߂�l]
// �p�X�����󂯎�邽�߂ɕK�v�ȃo�b�t�@�̃T�C�Y�B
//
// [����]
// pszBuffer �� NULL �̏ꍇ�A�K�v�ȃo�b�t�@��Ԃ��܂��B
//
// nSize �Ŏw�肳�ꂽ�G���A���s�\���ȏꍇ�AnSize - 1 ���������R�s�[���܂��B
//-----------------------------------------------------------------------------
int WSSLOG_API WLGetLogPath(char* pszBuffer, int nSize) {
	if (pszBuffer) {
		memset(pszBuffer, 0, nSize);
		strncpy(pszBuffer, m_LogPath, nSize - 1);
	}
	return strlen(m_LogPath) + 1;
}


//-----------------------------------------------------------------------------
// ���O�̕ۑ���p�X��ݒ�B
//
// [����]
// pszPath  �V�������O�̕ۑ���
//
// [����]
// �V�������O�̕ۑ���Ɋ��ϐ����܂܂�Ă����ꍇ�A�W�J����܂��B
//-----------------------------------------------------------------------------
void WSSLOG_API WLSetLogPath(const char* pszPath) {

	// �w�肳�ꂽ�p�X���ANULL ���́A�󕶎���̂Ƃ��́A�J�����g�f�B���N�g����ݒ�B
	if ((pszPath == NULL) || (lstrlen(pszPath) == 0)) {
		GetCurrentDirectory(MAX_PATH, m_LogPath);
		PathAddBackslash(m_LogPath);
		return;
	}

	char work[MAX_PATH];

	// ���ϐ���W�J���܂��B
	ExpandEnvironmentStrings(pszPath, m_LogPath, MAX_PATH);

	if (PathIsRelative(m_LogPath)) {
		// ���΃p�X���w�肳�ꂽ�Ƃ��́A�J�����g�f�B���N�g������̐�΃p�X�ɕϊ����܂��B
		GetCurrentDirectory(MAX_PATH, work);
		PathAppend(work, m_LogPath);
		lstrcpy(m_LogPath, work);
	} else {
		// ��΃p�X���w�肳��Ă��Ă��A�h���C�u���������Ă��Ȃ��Ƃ�������
		// �h���C�u���������Ă��Ȃ��Ƃ��́A�J�����g�f�B���N�g���̃h���C�u�����蓖�Ă�B
		if (PathGetDriveNumber(m_LogPath) == -1) {
			GetCurrentDirectory(MAX_PATH, work);
			PathStripToRoot(work);
			PathAppend(work, m_LogPath);
			lstrcpy(m_LogPath, work);
		}
	}

	// �p�X���̏I�[�� \ �����Ă����܂��B
	PathAddBackslash(m_LogPath);
}


//-----------------------------------------------------------------------------
// ini�t�@�C��(WinSousa.ini)����A���O�p�X�ƃ��O���x���̎擾
//
// [����]
// �{�Ɨ��̊֌W�ɂȂ邽�߁AGetIniFile.dll�̊֐��́A�g���Ȃ��̂Œ��ӁB
//-----------------------------------------------------------------------------
void GetIniFile() {
	char cPath[MAX_PATH];
	char cLogPath[MAX_PATH];

	// WinSousa.ini���Z�b�g����B
	PathAppend(strcpy(cPath, m_BasePath), "WinSousa.ini");

	// ���O�o�`�s�g�̎擾
	GetPrivateProfileString("Log", "LogPath", "", cLogPath, _MAX_PATH, cPath);
	if (strlen(cLogPath) == 0) {
		GetPrivateProfileString("Pathinf", "LogPath", "Log", cLogPath, _MAX_PATH, cPath);
	}

	// ���W���[����.ini �� LogPath �̋L�q������ꍇ�A�������D�悷��
	{
		char temp[MAX_PATH];
		char szModuleName[MAX_PATH];
		GetModuleFileName(NULL, szModuleName, sizeof(szModuleName));
		PathRenameExtension(szModuleName, ".ini");
		GetPrivateProfileString("WssLog", "LogPath", "", temp, sizeof(temp), szModuleName);
		if (lstrlen(temp) != 0) {
			lstrcpy(cLogPath, temp);
		}
	}


	// ���O�t���p�X
	{
		TCHAR temp[MAX_PATH];
		PathAppend(lstrcpy(temp, m_BasePath), cLogPath);
		WLSetLogPath(temp);
	}

	// ���O�t�@�C�����̌`
	m_logNameType = GetPrivateProfileInt(_T("Log"), _T("LogNameType"), 1, cPath);

	// LogLevel�̎擾
	m_nLogLevel = GetPrivateProfileInt(_T("Log"), _T("LogLevel"), 2, cPath);

	// �ۑ������̎擾
	m_nSaveDays = GetPrivateProfileInt(_T("Log"), _T("SaveDays"), DEFAULT_LOG_SAVEDAYS, cPath);

	// �~���[�e�b�N�X�����쐬����B(C:\temp\temp.exe �� mtx_C:/temp/)
	// �~���[�e�b�N�X���@"mtx_" + m_BasePath
	lstrcat(lstrcpy(m_MTX_SYSLOG, _T("mtx_")), m_BasePath);

	// �~���[�e�b�N�X����[\]���g�p�ł��Ȃ����߁A�@[\]��[/]�ɒu��������
	for (int i=0 ; i<(int)lstrlen(m_MTX_SYSLOG) ; i++) {
		if (m_MTX_SYSLOG[i] == '\\') {
			m_MTX_SYSLOG[i] = '/';
		}
	}

	// ���W�X�g������ syslog mode ���ǂ����A�擾����B
	kjm::registry reg;
	if (reg.reg_open_key_ex(HKEY_LOCAL_MACHINE, _T("Software\\Sanko\\Wss"), KEY_READ) == ERROR_SUCCESS) {
		m_syslogServer = reg.get_string(_T("SyslogServer"), _T(""));
	}

}

//-----------------------------------------------------------------------------
// �ۑ����t�̎擾
//-----------------------------------------------------------------------------
void GetSaveDate(LPTSTR cSaveDate) {
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


//-----------------------------------------------------------------------------
// �ۑ����Ԃ𒴂��� Log �t�H���_���폜����B
//
// [�߂�l]
// ��� TRUE �B
//-----------------------------------------------------------------------------
BOOL WSSLOG_API WLDeleteOldLog() {
	Winssa_Log("WSSLOG", LOGLV_INFO, "<%s> WLDeleteOldLog() enter.", m_appName);

	// �ۑ����t�̎擾
	char cSaveDate[16];
	GetSaveDate(cSaveDate);
	Winssa_Log("WSSLOG", LOGLV_INFO, "<%s> WLDeleteOldLog(): '%s' ���Â����t�t�H���_���폜���܂��B", m_appName, cSaveDate);

	// ���O�t�H���_���������郏�C���h�J�[�h��p��
	char wc[MAX_PATH];
	PathAppend(lstrcpy(wc, m_LogPath), "*.*");
	Winssa_Log("WSSLOG", LOGLV_INFO, "<%s> WLDeleteOldLog(): '%s' ���������܂��B", m_appName, wc);

	// ���O�t�H���_���
	WIN32_FIND_DATA findData;
	HANDLE hFind = FindFirstFile(wc, &findData);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				// �f�B���N�g��������Ώۂɂ���
				if ((lstrcmp(findData.cFileName, ".") != 0) && (lstrcmp(findData.cFileName, "..") != 0)) {
					// '.' �� '..' �ȊO��Ώۂɂ���
					char fullName[MAX_PATH];
					PathAppend(lstrcpy(fullName, m_LogPath), findData.cFileName);

					if (lstrcmp(findData.cFileName, cSaveDate) <= 0) {
						Winssa_Log("WSSLOG", LOGLV_INFO, "<%s> WLDeleteOldLog(): �t�H���_ '%s' ���폜���܂��B", m_appName, fullName);

						if (!kjm::util::remove_folder(fullName)) {
							Winssa_Log("WSSLOG", LOGLV_ERR, "<%s> WLDeleteOldLog(): error %u : �t�H���_�̍폜�Ɏ��s���܂����B", m_appName, GetLastError());
						}
					} else {
						// �c���t�@�C���̓��O�Ɏc���܂���B
						/// Winssa_Log("WSSLOG", LOGLV_INFO, "<%s> WLDeleteOldLog(): �t�H���_ '%s' �͎c���܂��B", m_appName, fullName);
					}
				}
			}
		} while (FindNextFile(hFind, &findData));
		FindClose(hFind);
	} else {
		Winssa_Log("WSSLOG", LOGLV_ERR, "<%s> WLDeleteOldLog(): error %u : �t�H���_�̌����Ɏ��s���܂����B", m_appName, GetLastError());
	}

	Winssa_Log("WSSLOG", LOGLV_INFO, "<%s> WLDeleteOldLog() leave.", m_appName);
	return TRUE;
}


//-----------------------------------------------------------------------------
// �O���Ɍ��J���郍�O�֐�
//
// [����]
//	const char* sikibetu	: ���O���ʕ�����
//	long  loglevel				: ���O���x��
//	const char *format		 :���O������
//
// [�@�\����]
//	�t�H�[�}�b�g�ɏ]��������������O�t�@�C���ɏo�͂���
//-----------------------------------------------------------------------------
void WINAPI Winssa_Log(const char *sikibetu,long loglevel,const char * format, ...) {

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

	if (loglevel == LOGLV_ALWAYS) {
		;										//�@���O���x���F�K�����O���o��(��).�p�ɂɋN������̂Ɋւ��Ă͎g�p�֎~
	} else if (loglevel < s_traceLevel) {			//�@���O���x�����������̂Ń��O�͂Ƃ�Ȃ�
		return;
	}

#if defined(USE_THREAD)
	// �X���b�h���[�h�̎��́A���O��������쐬���āAqueue��push����B
	{
		va_start(arg, format);

		vsprintf(s_buff, format, arg);

		SYSTEMTIME	ptm;
		GetLocalTime(&ptm);

		sprintf(buf,"%02d:%02d:%02d.%03d [%-6s]{%04X:%04X}L%d:",
				ptm.wHour, ptm.wMinute, ptm.wSecond, ptm.wMilliseconds, sikibetu, 
				GetCurrentProcessId(), GetCurrentThreadId(), loglevel);

		// ���O��������쐬���āAqueue��push����B
		std::string msg = buf;
		msg += s_buff;

		g_syslogc.lock();
		g_logQ.push(msg);
		g_syslogc.unlock();
	}
#else
	char logfname[MAX_PATH];
	char Path_Log[MAX_PATH];

	// �ʏ탂�[�h�̎��́A���̏�Ńt�@�C���ɏ����o���B
	DWORD dwRet = WaitForSingleObject(g_hmtxSyslog, 3000);
	if (dwRet == WAIT_OBJECT_0 || dwRet == WAIT_ABANDONED) {	//  ����

		va_start(arg, format);

		_vsnprintf(s_buff, sizeof(s_buff), format, arg);

		SYSTEMTIME	ptm;
		GetLocalTime(&ptm);

		static FILETIME s_ft;
		FILETIME ft;
		SystemTimeToFileTime(&ptm, &ft);

		sprintf(buf,"%02d:%02d:%02d.%03d [%-6s]{%04X:%04X}L%d:",
				ptm.wHour, ptm.wMinute, ptm.wSecond, ptm.wMilliseconds, sikibetu,
				GetCurrentProcessId(), GetCurrentThreadId(), loglevel);

		sprintf(Path_Log,"%s%04d%02d%02d", m_LogPath, ptm.wYear, ptm.wMonth, ptm.wDay);

		kjm::create_directory(Path_Log);

		PathAddBackslash(Path_Log);

		//  �f�o�b�O�o��
		OutputDebugString(buf);
		OutputDebugString(s_buff);
		OutputDebugString("\n");

		//�@���O�t�@�C�������쐬
		if (m_logNameType == 1) {
			sprintf(logfname,"%04d%02d%02d_%02d.log", ptm.wYear, ptm.wMonth, ptm.wDay, ptm.wHour);
		} else {
			sprintf(logfname,"%04d%02d%02d.log", ptm.wYear, ptm.wMonth, ptm.wDay);
		}

		PathAppend(Path_Log, logfname);

		// ���O�t�@�C���ɏo��
		FILE* fp = fopen(Path_Log, "a");
		if (fp) {
			if (*(__int64*)&ft < *(__int64*)&s_ft) {
				// ���Ԃ̋t�������m
				SYSTEMTIME st1, st2;
				FileTimeToSystemTime(&s_ft, &st1);
				FileTimeToSystemTime(&ft, &st2);

				fprintf(fp, "%s", buf);
				fprintf(fp, "<%s> << ���Ԃ̋t�������m���܂���: %04d/%02d/%02d %02d:%02d:%02d.%03d �� %04d/%02d/%02d %02d:%02d:%02d.%03d >>",
					m_appName,
					st1.wYear, st1.wMonth, st1.wDay, st1.wHour, st1.wMinute, st1.wSecond, st1.wMilliseconds,
					st2.wYear, st2.wMonth, st2.wDay, st2.wHour, st2.wMinute, st2.wSecond, st2.wMilliseconds);
				fprintf(fp, "\n");
			}

			s_ft = ft;

			fprintf(fp, "%s", buf);
			fprintf(fp, "%s", s_buff);
			fprintf(fp, "\n");

			fclose(fp);
		}

		ReleaseMutex(g_hmtxSyslog);		//  Mutex�̉��
	}
	else {
		assert( 0 );
	}
#endif

	force_a_linker_to_link_a_floating_point_library = 2.2;
}


//-----------------------------------------------------------------------------
// TraceLog �� wsslog �Œu�������邽�߂̃��O�J�n�֐�
//
// �_�~�[�ł��B��ɐ�����Ԃ��܂��B
//-----------------------------------------------------------------------------
int WINAPI TraceLogStart(unsigned char mainver, unsigned char minnerver, DWORD stTraceInfo) {
	return 0;
}


//-----------------------------------------------------------------------------
// TraceLog �� wsslog �Œu�������邽�߂̃��O�J�n�֐�
//
// WLDeleteOldLog �ɒu�������܂��B
//-----------------------------------------------------------------------------
int WINAPI TraceLogDeleteOldFile(short index/* = 0*/) {
	WLDeleteOldLog();
	return 0;
}


//-----------------------------------------------------------------------------
// TraceLog �� wsslog �Œu�������邽�߂̃��O�֐�
//
// tracestatus �́A0�`19�܂ł����O���x���ŁA����ȊO�̒l�́A���ʏ��ɂȂ��Ă���B
// (tracestatus % 20) �Ƃ��āA
//   15 �ȏ�, LOGLV_ALWAYS
//   12 �ȏ�, LOGLV_INFO
//    9 �ȏ�, LOGLV_ERR
//    6 �ȏ�, LOGLV_WARNING
//    3 �ȏ�, LOGLV_DEBUG
//    0 �ȏ�, LOGLV_TRACE
// �ɒu�������� Winssa_Log �ɓn���B
//-----------------------------------------------------------------------------
void WINAPI TraceLog(short tracestatus, const char *format, ...) {
	std::vector<char> buffer(256);
	va_list marker;

	va_start(marker, format);
	while (_vsnprintf(&buffer[0], buffer.size(), format, marker) == -1) {
		buffer.resize(buffer.size() * 2);
	}
	va_end(marker);

	int loglv;
	int tracelevel = tracestatus % 20;
	if (tracelevel >=15)		loglv = LOGLV_ALWAYS;
	else if (tracelevel >= 12)	loglv = LOGLV_INFO;
	else if (tracelevel >= 9)	loglv = LOGLV_ERR;
	else if (tracelevel >= 6)	loglv = LOGLV_WARNING;
	else if (tracelevel >= 3)	loglv = LOGLV_DEBUG;
	else						loglv = LOGLV_TRACE;

	Winssa_Log("TRCLOG", loglv, "%s", &buffer[0]);
}


//-----------------------------------------------------------------------------
//	�O���Ɍ��J���郍�O�֐�
//
// [����]
//	const char* sikibetu	: ���O���ʕ�����
//	long  loglevel				: ���O���x��
//	const char *format		 :���O������
//
// [�@�\����]
//	�t�H�[�}�b�g�ɏ]��������������O�t�@�C���ɏo�͂���
//-----------------------------------------------------------------------------
void WINAPI WinssaVB_Log(const char *sikibetu,long loglevel,const char * format) {
	Winssa_Log(sikibetu, loglevel, format);
}


//-----------------------------------------------------------------------------
//	�O���Ɍ��J���郍�O�֐�
//
// [����]
//	const char* sikibetu	: ���O���ʕ�����
//	long  loglevel			: ���O���x��
//	const void *buffer		 :�_���v�f�[�^�̃A�h���X
//	long  size				 :�_���v�f�[�^�̃T�C�Y
//
// �@�\����
//	�_���v�f�[�^�����O�t�@�C���ɏo�͂���
//-----------------------------------------------------------------------------
void WINAPI Winssa_Dump(const char *sikibetu,long loglevel, const void* buffer, long size) {
	m_bDump = TRUE;			// �_���v���[�h
	m_pDumpAdr = (void *)buffer;	// �_���v�A�h���X
	m_lDumpSize = size;		// �_���v�T�C�Y

	Winssa_Log(sikibetu, loglevel, "<Dump>Start");
}


//-----------------------------------------------------------------------------
// VB�p�̃_���v�f�[�^�����O����֐�
//-----------------------------------------------------------------------------
void WINAPI WinssaVB_Dump(const char *sikibetu,long loglevel, const void* buffer, long size) {
	Winssa_Dump(sikibetu, loglevel, buffer, size);
}


//-----------------------------------------------------------------------------
// ���O�t�@�C���̃^�C�v��ݒ肷��
//-----------------------------------------------------------------------------
void WSSLOG_API WLSetLogNameType(int nNameType) {
	m_logNameType = nNameType;
}


//-----------------------------------------------------------------------------
// ���݂̃��O�t�@�C���̃^�C�v���擾����
//-----------------------------------------------------------------------------
long WSSLOG_API WLGetLogNameType() {
	return m_logNameType;
}


//-----------------------------------------------------------------------------
// ���O���x���̎擾
//-----------------------------------------------------------------------------
long WINAPI WLGetLogLevel() {
	long nWork;
	InterlockedExchange(&nWork, s_traceLevel);
	return nWork;
}


//-----------------------------------------------------------------------------
// ���O���x���̐ݒ�
//-----------------------------------------------------------------------------
void WINAPI WLSetLogLevel(long nNewLogLevel) {
	if (nNewLogLevel < LOGLV_DEBUG || nNewLogLevel > LOGLV_FATALERR)
		return;

	InterlockedExchange(&s_traceLevel, nNewLogLevel);
}


//-----------------------------------------------------------------------------
// ���O���x���̐ݒ�
//
// [���l]
// WLSetLogLevel �֐��𗘗p���Ă��������B
//-----------------------------------------------------------------------------
void WINAPI Winssa_Loglevel(long loglevel) {
	WLSetLogLevel(loglevel);
}


//-----------------------------------------------------------------------------
// �L���[�ɗ��܂������O���Ȃ��Ȃ�܂ő҂�
//
// [���l]
// �X���b�h���[�h�̎��̂ݗL���Ȋ֐��B
// �ʏ탂�[�h�̎��́A�������Ȃ��B
//-----------------------------------------------------------------------------
void WSSLOG_API WLFlushLog() {
#if defined(USE_THREAD)
	while (1) {
		g_syslogc.lock();
		bool bEmpty = g_logQ.empty();
		g_syslogc.unlock();

		if (bEmpty) {
			break;
		} else {
			Sleep(50);
		}
	}
#endif
}

//-----------------------------------------------------------------------------
// �������g�̎��ʏ���Ԃ��B
//-----------------------------------------------------------------------------
void WSSLOG_API WLGetMyName(char *buf, int size) {
	GetModuleFileName(g_hinstDLL, buf, size);
	lstrcat(buf, "|RK edition");
//	strcpy(buf, "WLGetMyName called");
}

//-----------------------------------------------------------------------------
// �ۑ��������擾(default: 90 (days))
//-----------------------------------------------------------------------------
int WSSLOG_API WLGetSaveDays() {
	return m_nSaveDays;
}

//-----------------------------------------------------------------------------
// �ۑ�������ݒ�
//-----------------------------------------------------------------------------
void WSSLOG_API WLSetSaveDays(int newValue) {
	m_nSaveDays = newValue;
}

//-----------------------------------------------------------------------------
//	[�T�v]
//	DLL�G���g���[�|�C���g
//-----------------------------------------------------------------------------
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {

	UNREFERENCED_PARAMETER(lpvReserved);

	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		g_hinstDLL = hinstDLL;

		// winsock ��������
#if 0
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif
		// �A�v���P�[�V��������INI�t�@�C���̓Ǎ�������肷��
		FixIniPath(m_appName, m_BasePath);

		// ����process attach�����A���O���x����ݒ肷��
		if (s_bAttached == FALSE) {
			s_bAttached = TRUE;
			GetIniFile();
			WLSetLogLevel(m_nLogLevel);

			g_hmtxSyslog = CreateMutex(NULL, FALSE, m_MTX_SYSLOG);	// ���O�t�@�C���pmutex

		}

#if defined(USE_THREAD)
		// syslog client�p�X���b�h�N��!
		g_syslogc.run();
#endif

		break;

	case DLL_PROCESS_DETACH:

#if defined(USE_THREAD)
		// syslog client�p�X���b�h�I��!
		g_syslogc.end();
		g_syslogc.wait(5000);
#endif

		// �v���Z�X�I�����ɁA�~���[�e�b�N�X��j������B
		if (g_hmtxSyslog != NULL) {
			ReleaseMutex(g_hmtxSyslog);
			CloseHandle(g_hmtxSyslog);
			g_hmtxSyslog = NULL;
		}
#if 0
		WSACleanup();
#endif
		break;
	}

	return TRUE;
}
