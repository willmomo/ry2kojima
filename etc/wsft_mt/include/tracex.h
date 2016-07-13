#if !defined(__TRACEX_H)
#define __TRACEX_H

enum TRACE_LEVEL {
	TRACE_LEVEL_DEBUG = 0,	// �f�o�b�O�o��
	TRACE_LEVEL_INFO,		// ���o��
	TRACE_LEVEL_WARNING,	// �x��
	TRACE_LEVEL_ERROR,		// �G���[
	TRACE_LEVEL_FATAL,		// �v����
};

typedef struct {
	char dir[_MAX_PATH];	// �o�͐�t�H���_
	char pre[_MAX_PATH];	// prefix
} TRACEX_OPT;

void TraceXStart(TRACEX_OPT* opt);
void TraceX(TRACE_LEVEL level, const char* format, ...);
void TraceD(const char* format, ...);
void TraceI(const char* format, ...);
void TraceW(const char* format, ...);
void TraceE(const char* format, ...);
void TraceF(const char* format, ...);

#endif
