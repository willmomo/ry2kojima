#if !defined(__KJMLOG_H__)
#define __KJMLOG_H__

#if defined(__cplusplus)
extern "C" {
#endif

void KLogInit();
void KLogTerm();
void KLogGetLogPath(char *buf);
void KLogSetLogPath(const char *buf);
void KLogSetLogName(const char *buf);
void KLogWrite(const char *format, ...);
void KLogWriteE(const char *format, ...);

#if defined(__cplusplus)
}
#endif

#endif
