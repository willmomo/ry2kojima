
#if !defined(__RKLOG_H__)
#define __RKLOG_H__

#ifdef RKLOG_EXPORTS
#define RKLOG_API	__declspec(dllexport) __stdcall
#else
#define RKLOG_API	__declspec(dllimport) __stdcall
#endif

enum RKLOG_ERROR_LEVEL {
	RKL_TRACE = 0,
	RKL_WARNING,
	RKL_ERROR,
	RKL_FATAL,
	RKL_INFO
};

BOOL RKLOG_API RKLogVB(int lvl, const char *msg);
int RKLOG_API RKLGetLogLevel();
int RKLOG_API RKLSetLogLevel(int newLogLevel);
BOOL RKLOG_API RKLGetLogPrefix(char *pszLogPrefix, DWORD dwSize);
BOOL RKLOG_API RKLSetLogPrefix(const char *pszLogPrefix);
BOOL RKLOG_API RKLGetLogPath(char *pszLogPath, DWORD dwSize);
BOOL RKLOG_API RKLSetLogPath(const char *pszLogPath);
BOOL RKLOG_API RKLGetLogModule(char *pszLogModule, DWORD dwSize);
BOOL RKLOG_API RKLSetLogModule(const char *pszLogModule);
BOOL RKLOG_API RKLClearLog();
int RKLOG_API RKLGetLogPeriod();
int RKLOG_API RKLSetLogPeriod(int newLogPeriod);
BOOL RKLOG_API RKLSaveOptions(const char *pszIniName);
BOOL RKLOG_API RKLLoadOptions(const char *pszIniName);

#endif
