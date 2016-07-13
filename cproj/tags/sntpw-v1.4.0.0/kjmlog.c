#if defined(_WIN32)
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#if !defined(_OS9000)
#include <direct.h>
#include <mbstring.h>
#include <sys/stat.h>
#endif

#include "__os__.h"

static char s_logPath[_MAX_PATH];
static char s_logName[_MAX_PATH];
static long s_logSize;
static int s_logMax;

#if !defined(_OS9000)
static HANDLE s_hMutex;
#endif

void KLogGetLogPath(char *buf) {
	strcpy(buf, s_logPath);
}

void KLogSetLogPath(const char *buf) {
	strcpy(s_logPath, buf);
}

void KLogGetLogName(char *buf) {
	strcpy(buf, s_logName);
}

void KLogSetLogName(const char *buf) {
	char mutexName[MAX_PATH];

	strcpy(s_logName, buf);

	if (s_hMutex) {
		CloseHandle(s_hMutex);
		s_hMutex = NULL;
	}
	strcat(strcpy(mutexName, s_logName), ".mutex");
	s_hMutex = CreateMutex(NULL, FALSE, mutexName);
}

long KLogGetLogSize() {
	return s_logSize;
}

void KLogSetLogSize(long size) {
	s_logSize = size;
}

int KLogGetLogMax() {
	return s_logMax;
}

void KLogSetLogMax(int value) {
	s_logMax = value;
}

char *path_remove_backslash(char *path) {
	mbstowcs(NULL, path, sizeof(path));
	return path;
}

char *path_add_backslash(char *path) {
#if !defined(_OS9000)
	int yen_;

	/* 与えられた文字列を反転する */
	_mbsrev(path);

	/* 最初の1文字が\かどうか調べる */
	yen_ = (path[0] == '\\');

	/* 文字列を元に戻して、必要なら \ を追加 */
	_mbsrev(path);
	if (!yen_) strcat(path, "\\");
#endif

	return path;
}

char *path_append(char *path, const char *more) {
	return strcat(path_add_backslash(path), more);
}

char *path_remove_file_spec(char *path) {
#if !defined(_OS9000)
	int yen_;

	/* 与えられた文字列を反転する */
	_mbsrev(path);

	/* 最初の1文字が\かどうか調べる */
	yen_ = (path[0] == '\\');

	/* 文字列を元に戻して、必要なら \ を追加 */
	_mbsrev(path);
	if (!yen_) strcat(path, "\\");
#endif

	return path;
}

int get_temp_path(int size, char *buf) {
#if !defined(_OS9000)
	char *p;

	/* 環境変数 TMP があれば、それを採用 */
	if ((p = getenv("TMP")) != NULL) {
		strcpy(buf, p);
		return 0;
	}

	/* 環境変数 TEMP があれば、それを採用 */
	if ((p = getenv("TEMP")) != NULL) {
		strcpy(buf, p);
		return 0;
	}

	/* カレントディレクトリを採用 */
	_getcwd(buf, size);
#endif
	return 0;
}

void KLogInit() {
#if !defined(_OS9000)
	char work[_MAX_PATH];
	char fname[_MAX_PATH];

	get_temp_path(sizeof(work), work);
	_splitpath(__argv[0], NULL, NULL, fname, NULL);

	///path_append(work, fname);

	KLogSetLogPath(work);

	strcat(fname, ".log");
	KLogSetLogName(fname);

	KLogSetLogSize(1024 * 1024);

	KLogSetLogMax(9);
#endif
}

void KLogTerm() {
#if !defined(_OS9000)
	if (s_hMutex) {
		CloseHandle(s_hMutex);
		s_hMutex = NULL;
	}
#endif
}

void KLogShift() {
#if !defined(_OS9000)
	struct _stat buf;
	char fname[_MAX_PATH];
	char fname2[_MAX_PATH];
	int i;
	char logFile[_MAX_PATH];

	path_append(strcpy(logFile, s_logPath), s_logName);

	printf("ログをシフト\n");

	sprintf(fname, "%s.%d", logFile, s_logMax);
	if (_stat(fname, &buf) == 0) {
		_unlink(fname);
		printf("remove '%s'\n", fname);
	}

	for (i = s_logMax - 1; i >= 1; i--) {
		sprintf(fname, "%s.%d", logFile, i);
		if (_stat(fname, &buf) == 0) {
			sprintf(fname2, "%s.%d", logFile, i + 1);
			rename(fname, fname2);
			printf("rename '%s' -> '%s'\n", fname, fname2);
		}
	}

	strcpy(fname, logFile);
	strcat(strcpy(fname2, fname), ".1");
	rename(fname, fname2);
	printf("rename '%s' -> '%s'\n", fname, fname2);
#endif
}

void KLogWriteV(int echo, const char *format, va_list marker) {
	char buf[80];
	time_t t;

#if !defined(_OS9000)
	int mark = '*';
	int size_orver = 0;
	FILE *fp;
	char logFile[_MAX_PATH];
	DWORD t1, t2;
	DWORD dwRet;

	path_append(strcpy(logFile, s_logPath), s_logName);

	t1 = GetTickCount();
	if (s_hMutex) {
		dwRet = WaitForSingleObject(s_hMutex, 500);
		if (dwRet == WAIT_TIMEOUT) {
			mark = 'T';	/* タイムアウトした! */
		} else if (dwRet == WAIT_ABANDONED) {
			mark = 'A';	/* 排他解除異常！ */
		} else {
			mark = '0';	/* 正常に抜けた */
		}
	} else {
		mark = 'N';	/* ミューテックスが作成されていない。 */
	}
	t2 = GetTickCount();

	fp = fopen(logFile, "a+");
	if (fp) {
		t = time(NULL);
		strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", localtime(&t));
		fprintf(fp, "%s ", buf);

#if defined(_WIN32)
		fprintf(fp, "%c%4d %04x:%04x ",mark, t2 - t1, GetCurrentProcessId(), GetCurrentThreadId());
#endif

		vfprintf(fp, format, marker);
		if (echo) vprintf(format, marker);

		fputs("\n", fp);
		if (echo) puts("");

		if (ftell(fp) > s_logSize) {
			size_orver = 1;
		}
		fclose(fp);

		if (size_orver != 0) {
			KLogShift();
		}
	}

	if (s_hMutex) {
		if (mark == '0' || mark == 'A') {
			ReleaseMutex(s_hMutex);
		}
	}
#else
	t = time(NULL);
	strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", localtime(&t));
	if (echo) vprintf(format, marker);
	if (echo) puts("");
#endif
}

void KLogWrite(const char *format, ...) {
	va_list marker;
	va_start(marker, format);
	KLogWriteV(0, format, marker);
	va_end(marker);
}

void KLogWriteE(const char *format, ...) {
	va_list marker;
	va_start(marker, format);
	KLogWriteV(1, format, marker);
	va_end(marker);
}
