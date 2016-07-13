//--------------------------------------------------------------------

#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#include <stdio.h>
#include <time.h>
#include "rklog.h"

//--------------------------------------------------------------------
//	�萔

#define	DEFAULT_PERIOD	60
#define MUTEX_NAME		"MUTEX_RKLOG_DLL"
#define SECTION_NAME	_T("rklog")

//--------------------------------------------------------------------
//	�O���[�o���ϐ�

HANDLE g_hmtx;
char g_szLogPath[MAX_PATH];		// ���O�t�@�C�����L�^����f�B���N�g��
char g_szLogPrefix[MAX_PATH];	// ���O�t�@�C�����̐擪�ɂ��镶����
char g_szLogModule[MAX_PATH];	// �������g���Ă��郂�W���[���̃t�@�C����
int g_nLogLevel = RKL_WARNING;
int g_nPeriod = DEFAULT_PERIOD;

//--------------------------------------------------------------------
//	[�T�v]
//	���O�L�^�t�H���_���쐬����

static BOOL CreateFolder(LPCTSTR pszFolder)
{
	char *pszWork;

	pszWork = (char*)malloc(MAX_PATH);
	lstrcpy(pszWork, pszFolder);

	if (!PathFileExists(pszWork)) {
		if (PathIsRoot(pszWork)) {
			free(pszWork);
			return FALSE;
		}

		char *pszWork2;
		pszWork2 = (char *)malloc(MAX_PATH);
		lstrcpy(pszWork2, pszWork);
		PathAddBackslash(pszWork2);
		lstrcpy(pszWork2, "..");
		PathCanonicalize(pszWork, pszWork2);
		free(pszWork2);

		if (CreateFolder(pszWork)) {
			BOOL ret = CreateDirectory(pszFolder, NULL);
			free(pszWork);
			return ret;
		} else {
			free(pszWork);
			return FALSE;
		}
	}

	free(pszWork);

	return TRUE;
}

//--------------------------------------------------------------------
//	[�T�v]
//	���O�t�@�C�������쐬

static LPTSTR CreateLogName(LPTSTR fname, struct tm *ptm)
{
	time_t t = time(NULL);
	*ptm = *localtime(&t);

	sprintf(fname, "%s%s%04d%02d%02d.txt",
		g_szLogPath, g_szLogPrefix, ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday);

	return fname;
}

//--------------------------------------------------------------------
//	[�T�v]

BOOL RKLOG_API RKLClearLog()
{
	time_t t_now = time(NULL);
	struct tm tmWork = *localtime(&t_now);
	struct tm ntm;
	memset(&ntm, 0, sizeof(ntm));
	ntm.tm_year = tmWork.tm_year;
	ntm.tm_mon = tmWork.tm_mon;
	ntm.tm_mday = tmWork.tm_mday;
	t_now = mktime(&ntm);

	BOOL fRet;
	char szFindPath[MAX_PATH];

	lstrcpy(szFindPath, g_szLogPath);
	lstrcat(szFindPath, "*.*");

	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(szFindPath, &fd);
	if (hFind) {
		BOOL ret;
		do {
			if (strcmp(fd.cFileName, ".") != 0 && strcmp(fd.cFileName, "..") != 0) {
				SYSTEMTIME st;
				FileTimeToSystemTime(&fd.ftLastWriteTime, &st);

				struct tm ftm;
				memset(&ftm, 0, sizeof(ftm));
				ftm.tm_year = st.wYear - 1900;
				ftm.tm_mon = st.wMonth - 1;
				ftm.tm_mday = st.wDay;
				time_t t_file = mktime(&ftm);

				if ((int)(difftime(t_now, t_file) / (24 * 60 * 60)) > g_nPeriod) {
					char buf[MAX_PATH];
					lstrcpy(buf, g_szLogPath);
					lstrcat(buf, fd.cFileName);
					DeleteFile(buf);
				}
			}

			ret = FindNextFile(hFind, &fd);
		} while (ret);

		FindClose(hFind);

		fRet = TRUE;
	} else {
		fRet = FALSE;
	}

	return fRet;
}

//--------------------------------------------------------------------
//	[�T�v]
//	���O�o��

BOOL RKLOG_API RKLogVB(int lvl, LPCTSTR msg)
{
	static bool s_firstCall = true;
	static char szLvl[] = {'T', 'W', 'E', 'F', 'I'};

	if (s_firstCall) {
		s_firstCall = false;
		RKLClearLog();
	}

	if (lvl < 0 || lvl > 4) {
		return FALSE;
	}

	if (!CreateFolder(g_szLogPath)) {
		return FALSE;
	}

	BOOL ret;
	char fname[MAX_PATH];
	struct tm ltm;
	FILE *fp;

	WaitForSingleObject(g_hmtx, 500);

	fp = fopen(CreateLogName(fname, &ltm), "a+");
	if (fp) {
		fprintf(fp, "%c %02d:%02d:%02d <%s> %s\n", szLvl[lvl],
			ltm.tm_hour, ltm.tm_min, ltm.tm_sec, g_szLogModule, msg);
		fclose(fp);

		ret = TRUE;
	} else {
		ret = FALSE;
	}

	ReleaseMutex(g_hmtx);

	return ret;
}

//--------------------------------------------------------------------
//	[�T�v]
//	�L�������̎擾

int RKLOG_API RKLGetLogPeriod()
{
	return g_nPeriod;
}

//--------------------------------------------------------------------
//	[�T�v]
//	�L�������̐ݒ�

int RKLOG_API RKLSetLogPeriod(int newLogPeriod)
{
	int oldPeriod = g_nPeriod;

	if (newLogPeriod <= 0) {
		return -1;
	}

	g_nPeriod = newLogPeriod;

	return oldPeriod;
}

//--------------------------------------------------------------------
//	[�T�v]
//	���O���x���̎擾

int RKLOG_API RKLGetLogLevel()
{
	return g_nLogLevel;
}

//--------------------------------------------------------------------
//	[�T�v]
//	���O���x���̐ݒ�

int RKLOG_API RKLSetLogLevel(int newLogLevel)
{
	int oldLogLevel = g_nLogLevel;

	if (newLogLevel < RKL_TRACE || newLogLevel > RKL_INFO) {
		return -1;
	}

	g_nLogLevel = newLogLevel;

	return oldLogLevel;
}

//--------------------------------------------------------------------
//	[�T�v]
//	���O�t�@�C���̐擪�ɂ��镶������擾

BOOL RKLOG_API RKLGetLogPrefix(LPTSTR pszLogPrefix, DWORD dwSize)
{
	if (dwSize < (strlen(g_szLogPrefix) + 1)) {
		return FALSE;
	}

	lstrcpy(pszLogPrefix, g_szLogPrefix);

	return TRUE;
}

//--------------------------------------------------------------------
//	[�T�v]
//	���O�t�@�C���̐擪�ɂ��镶�����ݒ�

BOOL RKLOG_API RKLSetLogPrefix(LPCTSTR pszLogPrefix)
{
	if (pszLogPrefix == NULL) {
		lstrcpy(g_szLogPrefix, "rkl");
	} else {
		lstrcpy(g_szLogPrefix, pszLogPrefix);
	}

	return TRUE;
}

//--------------------------------------------------------------------
//	[�T�v]
//	���O�t�@�C�����L�^����f�B���N�g�����擾

BOOL RKLOG_API RKLGetLogPath(LPTSTR pszLogPath, DWORD dwSize)
{
	if (dwSize < (strlen(g_szLogPath) + 1)) {
		return FALSE;
	}

	lstrcpy(pszLogPath, g_szLogPath);

	return TRUE;
}

//--------------------------------------------------------------------
//	[�T�v]
//	���O�t�@�C�����L�^����f�B���N�g����ݒ�

BOOL RKLOG_API RKLSetLogPath(LPCTSTR pszLogPath)
{
	if (pszLogPath == NULL) {
		GetTempPath(sizeof(g_szLogPath), g_szLogPath);
		PathAddBackslash(g_szLogPath);
		lstrcat(g_szLogPath, "rklog\\");
	} else {
		lstrcpy(g_szLogPath, pszLogPath);
		PathAddBackslash(g_szLogPath);
	}

	return TRUE;
}

//--------------------------------------------------------------------
//	[�T�v]
//	���O�Ɏc�����ʖ����擾

BOOL RKLOG_API RKLGetLogModule(LPTSTR pszLogModule, DWORD dwSize)
{
	if (dwSize < (strlen(g_szLogModule) + 1)) {
		return FALSE;
	}

	lstrcpy(pszLogModule, g_szLogModule);

	return TRUE;
}

//--------------------------------------------------------------------
//	[�T�v]
//	���O�Ɏc�����ʖ���ݒ�
//
//	[���l]
//	������NULL�̎��́A����DLL���g�p���Ă�����s�t�@�C������ݒ肷��B

BOOL RKLOG_API RKLSetLogModule(LPCTSTR pszLogModule)
{
	char temp[MAX_PATH];

	if (pszLogModule == NULL) {
		GetModuleFileName(NULL, temp, sizeof(temp));
		lstrcpy(g_szLogModule, PathFindFileName(temp));
	} else {
		lstrcpy(g_szLogModule, pszLogModule);
	}

	return TRUE;
}

//--------------------------------------------------------------------
//	[�T�v]
//	INI�t�@�C������I�v�V������ǂݍ���

BOOL RKLOG_API RKLLoadOptions(LPCTSTR pszIniName)
{
	GetPrivateProfileString(SECTION_NAME, "LogPath", g_szLogPath, g_szLogPath, sizeof(g_szLogPath), pszIniName);
	GetPrivateProfileString(SECTION_NAME, "LogPrefix", g_szLogPrefix, g_szLogPrefix, sizeof(g_szLogPrefix), pszIniName);
	GetPrivateProfileString(SECTION_NAME, "LogModule", g_szLogModule, g_szLogModule, sizeof(g_szLogModule), pszIniName);
	g_nLogLevel = GetPrivateProfileInt(SECTION_NAME, "LogLevel", g_nLogLevel, pszIniName);
	g_nPeriod = GetPrivateProfileInt(SECTION_NAME, "Period", g_nPeriod, pszIniName);

	return TRUE;
}

//--------------------------------------------------------------------
//	[�T�v]
//	INI�t�@�C���փI�v�V��������������

BOOL RKLOG_API RKLSaveOptions(LPCTSTR pszIniName)
{
	WritePrivateProfileString(SECTION_NAME, "LogPath", g_szLogPath, pszIniName);
	WritePrivateProfileString(SECTION_NAME, "LogPrefix", g_szLogPrefix, pszIniName);
	WritePrivateProfileString(SECTION_NAME, "LogModule", g_szLogModule, pszIniName);

	char buf[256];
	wsprintf(buf, "%d", g_nLogLevel);
	WritePrivateProfileString(SECTION_NAME, "LogLevel", buf, pszIniName);
	
	wsprintf(buf, "%d", g_nPeriod);
	WritePrivateProfileString(SECTION_NAME, "Period", buf, pszIniName);

	return TRUE;
}

//--------------------------------------------------------------------
//	[�T�v]
//	DLL_PROCESS_ATTACH�̏���

static BOOL OnProcessAttach(HINSTANCE hinstDLL)
{
	if (RKLSetLogPath(NULL) == FALSE) {
		return FALSE;
	}

	if (RKLSetLogPrefix(NULL) == FALSE) {
		return FALSE;
	}

	if (RKLSetLogModule(NULL) == FALSE) {
		return FALSE;
	}

	g_hmtx = CreateMutex(NULL, FALSE, MUTEX_NAME);
	if (g_hmtx == NULL) {
		return FALSE;
	}

	return TRUE;
}

//--------------------------------------------------------------------
//	[�T�v]
//	DLL_PROCESS_DETACH�̏���

static BOOL OnProcessDetach(HINSTANCE hinstDLL)
{
	if (g_hmtx != NULL) {
		CloseHandle(g_hmtx);
	}

	return TRUE;
}

//--------------------------------------------------------------------
//	[�T�v]
//	DLL�G���g���[�|�C���g

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
