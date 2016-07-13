#if !defined(__KJMLOG_H__)
#define __KJMLOG_H__

void KLogInit();
void KLogWrite(const char *format, ...);
void KLogWriteE(const char *format, ...);

#endif
