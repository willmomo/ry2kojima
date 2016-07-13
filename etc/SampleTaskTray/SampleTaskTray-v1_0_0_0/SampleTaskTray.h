#if !defined(__SNTPD_H__)
#define __SNTPD_H__

/*---------------------------------------------------------------------
	プログラムの動作設定
---------------------------------------------------------------------*/
typedef struct {
	int m_daemon_mode;			/* デーモンモード 1: on */
	char m_ntp_server[1024];	/* NTPサーバー名 */
	int m_noadjust;				/* 時計わせをしない 1: しない */
	int m_interval;				/* 時計合わせ間隔(sec) */

	int m_allowable_offset;		/* 許容誤差(秒) */
	int m_allowable_delay;		/* 許容遅延(秒) */
	int m_allowable_max_offset;	/* 許容最大誤差(秒) */
} ntp_options;

#if defined(__cplusplus)
extern "C" {
#endif

void SampleTaskTray_set_options(int dameon_mode, const char *ntp_server, int noadjust, int interval);
void SampleTaskTray_thread_end();
void sntp_thread_adjust();
unsigned __stdcall SampleTaskTray_thread(void* argv);
unsigned __stdcall SampleTaskTray_thread(void* argv);

#if defined(__cplusplus)
}
#endif

#endif
