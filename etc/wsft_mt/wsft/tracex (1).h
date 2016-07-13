#if !defined(__TRACEX_H)
#define __TRACEX_H

enum TRACE_LEVEL {
	TRACE_LEVEL_DEBUG = 0,	// デバッグ出力
	TRACE_LEVEL_INFO,		// 情報出力
	TRACE_LEVEL_WARNING,	// 警告
	TRACE_LEVEL_ERROR,		// エラー
	TRACE_LEVEL_FATAL		// 致命傷
};

/*	表示レベル	*/
typedef enum {
	TRACE_OUTPUT_LOG_MAX = 0,	/*	トレース専用ログ(最大)		*/
	TRACE_OUTPUT_LOG     = 1,	/*	トレース専用ログ(普通)		*/
	TRACE_OUTPUT_LOG_MIN = 2,	/*	トレース専用ログ(最小)		*/
	TRACE_DEBUG_MAX      = 3,	/*	デバッグ用ログ(最大)		*/
	TRACE_DEBUG          = 4,	/*	デバッグ用ログ(普通)		*/
	TRACE_DEBUG_MIN      = 5,	/*	デバッグ用ログ(最小)		*/
	TRACE_WARNING_MAX    = 6,	/*	警告ログ(最大)				*/
	TRACE_WARNING        = 7,	/*	デバッグ用ログ(普通)		*/
	TRACE_WARNING_MIN    = 8,	/*	デバッグ用ログ(最小)		*/
	TRACE_ERROR_SLIGHT   = 9,	/*	エラーログ(軽微)			*/
	TRACE_ERROR_FATAL    = 10,	/*	エラーログ(重大)			*/
	TRACE_ERROR_SYSTEM   = 11,	/*	エラーログ(システム絡み)	*/
	TRACE_INFO_MAX       = 12,	/*	情報表示(常に表示)			*/
	TRACE_INFO           = 13,	/*	情報表示(常に表示)			*/
	TRACE_INFO_MIN       = 14,	/*	情報表示(常に表示)			*/
	TRACE_SYSTEM_INFO	 = 15,	/*	処理開始と終了を書込むとき (書式を変えたいので)	*/
	TRACE_NO_OUTPUT      = 17,	/*	ダミー(最後=これで、非表示を実現している)	*/
	TRACE_ERROR_DATABASE = 18,	/*	データベースに起因する問題	*/
	TRACE_SYSTEM_BUG     = 19,	/*	バグなので必ず表示			*/
} ETraceStatus ;

/*	トレースレベル設定	*/
#define	TRACELEVEL_ALL_MAX		TRACE_OUTPUT_LOG_MAX	/*	全て表示する							*/
#define	TRACELEVEL_LOG_MAX		TRACE_OUTPUT_LOG_MAX	/*	ログ情報以上を表示する					*/
#define	TRACELEVEL_DEBUG_MAX	TRACE_DEBUG_MAX			/*	デバッグ情報以上を表示する				*/
#define	TRACELEVEL_WARNING_MAX	TRACE_WARNING_MAX		/*	警告情報以上を表示する					*/
#define	TRACELEVEL_ERROR_SLIGHT	TRACE_ERROR_SLIGHT		/*	エラー情報(軽微)以上を表示する			*/
#define	TRACELEVEL_ERROR_FATAL	TRACE_ERROR_FATAL		/*	エラー情報(重大)以上を表示する			*/
#define	TRACELEVEL_ERROR_SYSTEM	TRACE_ERROR_SYSTEM		/*	エラー情報(システム絡み)以上を表示する	*/
#define	TRACELEVEL_INFO			TRACE_INFO				/*	情報以上を表示する						*/
#define	TRACELEVEL_NO_OUTPUT	TRACE_NO_OUTPUT			/*	何も表示しない							*/

typedef struct {
	char dir[_MAX_PATH];	// 出力先フォルダ
	char pre[_MAX_PATH];	// prefix
	char suf[_MAX_PATH];	// suffix
} TRACEX_OPT;

void TraceXStart(TRACEX_OPT* opt);
void TraceX(TRACE_LEVEL level, const char* format, ...);
void TraceD(const char* format, ...);
void TraceI(const char* format, ...);
void TraceW(const char* format, ...);
void TraceE(const char* format, ...);
void TraceF(const char* format, ...);

#endif
