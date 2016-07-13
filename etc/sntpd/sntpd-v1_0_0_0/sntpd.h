#if !defined(__SNTPD_H__)
#define __SNTPD_H__

/*---------------------------------------------------------------------
	�v���O�����̓���ݒ�
---------------------------------------------------------------------*/
typedef struct {
	int m_daemon_mode;		/* �f�[�������[�h 1: on */
	char m_ntp_server[80];	/* NTP�T�[�o�[�� */
	int m_noadjust;			/* ���v�킹�����Ȃ� 1: ���Ȃ� */
	int m_interval;			/* ���v���킹�Ԋu(sec) */
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
