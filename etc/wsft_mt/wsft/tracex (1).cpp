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
std::string GetExeName();	// adhoc プロトタイプ

//---------------------------------------------------------------------

static const TRACE_LEVEL s_traceLevel = TRACE_LEVEL_INFO;
// static TRACE_LEVEL s_traceLevel = TRACE_LEVEL_DEBUG;

//---------------------------------------------------------------------

void TraceXStart(TRACEX_OPT* opt)
{
	TRACEX_OPT s_opt;		// 動作オプション
	char s_fname[_MAX_PATH];	// ログ出力ファイル名

	if (opt == NULL)
		memset(&s_opt, 0, sizeof(s_opt));
	else
		s_opt = *opt;


	// dirの最後の'\'を保証する
	if (s_opt.dir[strlen(s_opt.dir)-1] != '\\') {
		strcat(s_opt.dir, "\\");
	}

#if 1
	char temp[MAX_PATH];
	char logpath[MAX_PATH];
	char wk[MAX_PATH];

	// 基準ＰＡＴＨ
	GetWinSousaStr("Pathinf", "BasePath", &logpath[0], _MAX_PATH);
	if( logpath[strlen(logpath) - 1] != '\\' ) {
		strcat(logpath, "\\");
	}

	// ログＰＡＴＨ
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
	// 環境変数TEMP or TMPを参照して出力先ディレクトリとする
	// どちらも未設定であれば、".\"にする
	char *p, temp[_MAX_PATH], check[_MAX_PATH];
	if ((p = getenv("TEMP")) == NULL) {
		if ((p = getenv("TMP")) == NULL) {
			p = ".\\";
		}
	}
	strcpy(temp, p);

	if (temp[strlen(temp) - 1] != '\\') {
		strcat(temp, "\\");	// 最後の'\'を保証する
	}

	struct _stat statBuf;
	sprintf(check, "%s.", temp);	// "hogehoge\.ファイルを調べる
	if (_stat(check, &statBuf) != 0) {	// 無いときは、
		strcpy(temp, ".\\");			// ".\"を出力ディレクトリとする
	}

	time_t t = time(NULL);
	struct tm* ptm = localtime(&t);

	// ファイル名を作成
	sprintf(s_fname, "%swsft%02d%02d.log",
		temp, ptm->tm_mon + 1, ptm->tm_mday);
#endif
}

//---------------------------------------------------------------------

static void vTraceX(TRACE_LEVEL level, const char* format, va_list arg)
{
	std::vector<char> s_buffer(2048);		// 1行最大2048文字まで
	std::vector<char> s_buffer2(2048);		// 1行最大2048文字まで

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
