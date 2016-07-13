#if !defined(__TRACEX_H)
#define __TRACEX_H

enum TRACE_LEVEL {
	TRACE_LEVEL_DEBUG = 0,	// デバッグ出力
	TRACE_LEVEL_INFO,		// 情報出力
	TRACE_LEVEL_WARNING,	// 警告
	TRACE_LEVEL_ERROR,		// エラー
	TRACE_LEVEL_FATAL,		// 致命傷
};

typedef struct {
	char dir[_MAX_PATH];	// 出力先フォルダ
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
