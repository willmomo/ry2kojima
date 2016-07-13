#if !defined(__SNTPD_H__)
#define __SNTPD_H__

/*---------------------------------------------------------------------
	プログラムの動作設定
---------------------------------------------------------------------*/
typedef struct {
	int m_noadjust;				/* 時計わせをしない 1: しない */
	int m_allowable_offset;		/* 許容誤差(秒) */
	int m_allowable_delay;		/* 許容遅延(秒) */
	int m_allowable_max_offset;	/* 許容最大誤差(秒) */
} ntp_options;

#if defined(__cplusplus)
extern "C" {
#endif

int sntp(const char* ntpHost, const ntp_options *opt);
void sntp_socket_startup();
void sntp_socket_cleanup();

#if defined(__cplusplus)
}
#endif

#endif
