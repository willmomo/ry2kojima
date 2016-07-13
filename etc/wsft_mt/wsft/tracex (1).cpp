#include <windows.h>
#include <shlwapi.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "tracex.h"
#include "wsft.h"
#include "wsslog.h"
#include "GetIniFile.h"

#include <vector>

#include <string>
std::string GetExeName();	// adhoc �v���g�^�C�v

//---------------------------------------------------------------------

static const TRACE_LEVEL s_traceLevel = TRACE_LEVEL_INFO;
// static TRACE_LEVEL s_traceLevel = TRACE_LEVEL_DEBUG;

//---------------------------------------------------------------------

void TraceXStart(TRACEX_OPT* opt)
{
	TRACEX_OPT s_opt;		// ����I�v�V����
	char s_fname[_MAX_PATH];	// ���O�o�̓t�@�C����

	if (opt == NULL)
		memset(&s_opt, 0, sizeof(s_opt));
	else
		s_opt = *opt;


	// dir�̍Ō��'\'��ۏ؂���
	if (s_opt.dir[strlen(s_opt.dir)-1] != '\\') {
		strcat(s_opt.dir, "\\");
	}

#if 1
	char temp[MAX_PATH];
	char logpath[MAX_PATH];
	char wk[MAX_PATH];

	// ��o�`�s�g
	GetWinSousaStr("Pathinf", "BasePath", &logpath[0], _MAX_PATH);
	if( logpath[strlen(logpath) - 1] != '\\' ) {
		strcat(logpath, "\\");
	}

	// ���O�o�`�s�g
	GetWinSousaStr("Pathinf", "LogPath", &temp[0], _MAX_PATH);
	if( temp[strlen(temp) - 1] != '\\' ) {
		strcat(temp, "\\");
	}
	strcat(logpath, temp);

	time_t t = time(NULL);
	struct tm *ptm = localtime(&t);

	wsprintf(wk,"%s%04d%02d%02d\\", logpath, ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday);
	wsprintf(s_fname,"%swsft%04d%02d%02d.log", wk, ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday);
#else
	// ���ϐ�TEMP or TMP���Q�Ƃ��ďo�͐�f�B���N�g���Ƃ���
	// �ǂ�������ݒ�ł���΁A".\"�ɂ���
	char *p, temp[_MAX_PATH], check[_MAX_PATH];
	if ((p = getenv("TEMP")) == NULL) {
		if ((p = getenv("TMP")) == NULL) {
			p = ".\\";
		}
	}
	strcpy(temp, p);

	if (temp[strlen(temp) - 1] != '\\') {
		strcat(temp, "\\");	// �Ō��'\'��ۏ؂���
	}

	struct _stat statBuf;
	sprintf(check, "%s.", temp);	// "hogehoge\.�t�@�C���𒲂ׂ�
	if (_stat(check, &statBuf) != 0) {	// �����Ƃ��́A
		strcpy(temp, ".\\");			// ".\"���o�̓f�B���N�g���Ƃ���
	}

	time_t t = time(NULL);
	struct tm* ptm = localtime(&t);

	// �t�@�C�������쐬
	sprintf(s_fname, "%swsft%02d%02d.log",
		temp, ptm->tm_mon + 1, ptm->tm_mday);
#endif
}

//---------------------------------------------------------------------

static void vTraceX(TRACE_LEVEL level, const char* format, va_list arg)
{
	std::vector<char> s_buffer(2048);		// 1�s�ő�2048�����܂�
	std::vector<char> s_buffer2(2048);		// 1�s�ő�2048�����܂�

	vsprintf(&s_buffer[0], format, arg);
	wsprintf(&s_buffer2[0], "(%s)%s", GetExeName().c_str(), &s_buffer[0]);
	Winssa_Log("WSFT", level, &s_buffer2[0]);
}

//---------------------------------------------------------------------

void TraceX(TRACE_LEVEL level, const char* format, ...)
{
	if (level < s_traceLevel)
		return;

	va_list arg;
	va_start(arg, format);
	vTraceX(level, format, arg);
	va_end(arg);
}

//---------------------------------------------------------------------

void TraceD(const char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	vTraceX(TRACE_LEVEL_DEBUG, format, arg);
	va_end(arg);
}

//---------------------------------------------------------------------

void TraceI(const char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	vTraceX(TRACE_LEVEL_INFO, format, arg);
	va_end(arg);
}

//---------------------------------------------------------------------

void TraceW(const char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	vTraceX(TRACE_LEVEL_WARNING, format, arg);
	va_end(arg);
}

//---------------------------------------------------------------------

void TraceE(const char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	vTraceX(TRACE_LEVEL_ERROR, format, arg);
	va_end(arg);
}

//---------------------------------------------------------------------

void TraceF(const char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	vTraceX(TRACE_LEVEL_FATAL, format, arg);
	va_end(arg);
}
