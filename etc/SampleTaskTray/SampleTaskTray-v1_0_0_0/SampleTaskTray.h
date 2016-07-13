#if !defined(__SNTPD_H__)
#define __SNTPD_H__

/*---------------------------------------------------------------------
	�v���O�����̓���ݒ�
---------------------------------------------------------------------*/
typedef struct {
	int m_daemon_mode;			/* �f�[�������[�h 1: on */
	char m_ntp_server[1024];	/* NTP�T�[�o�[�� */
	int m_noadjust;				/* ���v�킹�����Ȃ� 1: ���Ȃ� */
	int m_interval;				/* ���v���킹�Ԋu(sec) */

	int m_allowable_offset;		/* ���e�덷(�b) */
	int m_allowable_delay;		/* ���e�x��(�b) */
	int m_allowable_max_offset;	/* ���e�ő�덷(�b) */
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
