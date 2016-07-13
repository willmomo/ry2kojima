#if !defined(__KJMLOG_H__)
#define __KJMLOG_H__

#if defined(__cplusplus)
extern "C" {
#endif

void KLogInit();
void KLogSetLogPath(const char *buf);
void KLogWrite(const char *format, ...);
void KLogWriteE(const char *format, ...);
char *path_append(char *path, const char *more);

#if defined(__cplusplus)
}
#endif

#endif
