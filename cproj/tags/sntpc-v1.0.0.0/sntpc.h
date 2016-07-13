#if !defined(__SNTPD_H__)
#define __SNTPD_H__

/*---------------------------------------------------------------------
	�v���O�����̓���ݒ�
---------------------------------------------------------------------*/
typedef struct {
	int m_noadjust;				/* ���v�킹�����Ȃ� 1: ���Ȃ� */
	int m_allowable_offset;		/* ���e�덷(�b) */
	int m_allowable_delay;		/* ���e�x��(�b) */
	int m_allowable_max_offset;	/* ���e�ő�덷(�b) */
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
