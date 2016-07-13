#if !defined(__SNTPD_H__)
#define __SNTPD_H__

/*---------------------------------------------------------------------
	プログラムの動作設定
---------------------------------------------------------------------*/
typedef struct {
	int m_daemon_mode;		/* デーモンモード 1: on */
	char m_ntp_server[80];	/* NTPサーバー名 */
	int m_noadjust;			/* 時計わせをしない 1: しない */
	int m_interval;			/* 時計合わせ間隔(sec) */
} ntp_options;

#if defined(__cplusplus)
extern "C" {
#endif

void sntp_set_options(int dameon_mode, const char *ntp_server, int noadjust, int interval);
void sntp_thread_end();
unsigned __stdcall sntp_thread(void* argv);

#if defined(__cplusplus)
}
#endif

#endif
