/*
	http://www.venus.dti.ne.jp/~yoshi-o/NTP/NTP-SNTP_Format.html
*/

#if defined(_WIN32)
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#endif

#if defined(_OS9000)
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <modes.h>
#include <UNIX\os9def.h>
#include <ctype.h>
#include <stdlib.h>
#include <sg_codes.h>
#include <spf\spf.h>
#include "__os__.h"
#endif

#include <stdio.h>
#include <math.h>
#include <time.h>

#pragma pack(1)

typedef struct {
	unsigned _seconds;
	unsigned _fractions;
} NTP_TIME;

typedef struct sntp_packet {
	int Control;
	int Root_Delay;
	int Root_Dispersion;
	int Reference_Identifier;
	NTP_TIME Reference_Timestamp;
	NTP_TIME Originate_Timestamp;
	NTP_TIME Receive_Timestamp;
	NTP_TIME Transmit_Timestamp;
	int Key_Identifier;
	char Message_Digest[128];
} sntp_packet_t;

unsigned long g_ident;	/* �O��̒ʐM���� */
NTP_TIME g_st;		/* �O��̏��� */

#define FT19000101	((__int64)94354848000000000)	/* FILETIME�^�ŁA1900/01/01 00:00:00 */
#define FT19700101	((__int64)116444736000000000)	/* FILETIME�^�ŁA1970/01/01 00:00:00 */
#define FT_USEC		((__int64)10)
#define FT_MSEC		(FT_USEC * (__int64)1000)
#define FT_SEC		(FT_MSEC * (__int64)1000)
#define SEC19700101	2208988800						/* 1900/01/01 ���� 1970/01/01 �܂ł̕b�� */

/*---------------------------------------------------------------------
	���ݎ��Ԃ𕶎���
---------------------------------------------------------------------*/
const char *strltime() {
	static char s_buf[80];
	time_t t;

	t = time(NULL);
	strftime(s_buf, sizeof(s_buf), "%Y/%m/%d %H:%M:%S", localtime(&t));

	return s_buf;
}

/*---------------------------------------------------------------------
	���v�����킹��
---------------------------------------------------------------------*/
void adjust_time(double offset) {
	/**
	SYSTEMTIME st;
	FILETIME ft9;

	printf("adjust: %s -> ", strltime());

	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft9);
	*(__int64*)&ft9 += offset * FT_MSEC;
	FileTimeToSystemTime(&ft9, &st);
	SetSystemTime(&st);

	printf("%s\n", strltime());
	**/
}

/*---------------------------------------------------------------------
	�v���O�����̓���ݒ�
---------------------------------------------------------------------*/
struct {
	int m_daemon_mode;		/* �f�[�������[�h 1: on */
	char m_ntp_server[80];	/* NTP�T�[�o�[�� */
} g_options;

/*---------------------------------------------------------------------
	NTP���Ԃ� double �� msec ��
---------------------------------------------------------------------*/
double ntp_msec(NTP_TIME nt) {
	return ((double)nt._seconds * 1000.0) + (nt._fractions / 5000000);
}

/*---------------------------------------------------------------------
	NTP_TIME�𕶎���
---------------------------------------------------------------------*/
const char *strntptime(NTP_TIME nt) {
	static char s_buf[80];
#if defined(USE_SYSTEMTIME)
	FILETIME ft;
	SYSTEMTIME st;

	*(__int64*)&ft = ((__int64)nt._seconds * FT_SEC + FT19000101) + (nt._fractions / 5000000) * FT_MSEC;
	FileTimeToSystemTime(&ft, &st);
	sprintf(s_buf, "%04d/%02d/%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
#else
	char work[80];
	time_t t;

	t = (nt._seconds >= SEC19700101) ? nt._seconds - SEC19700101 : 0;
	strftime(work, sizeof(work), "%Y/%m/%d %H:%M:%S", gmtime(&t));
	sprintf(s_buf, "%s.%03d", work,nt._fractions / 5000000);
#endif
	return s_buf;
}

/*---------------------------------------------------------------------
	1900/01/01 ���N�_�Ƃ��� UTC ���Ԃ� NTP_TIME �\���̂�Ԃ�
---------------------------------------------------------------------*/
NTP_TIME ntp_time() {
	NTP_TIME result;
#if defined(USE_SYSTEMTIME)
	SYSTEMTIME st;
	FILETIME ft;

	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);
	result._seconds = ((*(__int64*)&ft - FT19000101) / ((__int64)10 * (__int64)1000 * (__int64)1000));
	result._fractions = st.wMilliseconds * 5000000;
#else
	result._seconds = time(NULL) + 2208988800;
	result._fractions = 0;
#endif
	return result;
}

/*---------------------------------------------------------------------
	NTP�d������ʂɕ\��
---------------------------------------------------------------------*/
void print_ntp_packet(const sntp_packet_t *p, int full) {
	unsigned long ri;

	printf("Control             : %08Xh\n", p->Control);
	printf(" �[�b �o�[�W���� ���[�h �K�w �|�[�����O ���x\n");
	printf(" %4d %10d %6d %4d %10d %4d\n", 
		(p->Control & 0xc0000000) >> 30, 
		(p->Control & 0x38000000) >> 27,
		(p->Control & 0x07000000) >> 24,
		(p->Control & 0x00ff0000) >> 16,
		(p->Control & 0x0000ff00) >> 8,
		(p->Control & 0x000000ff));

	if (full) {
		printf("Root_Delay          : %d\n", p->Root_Delay);
		printf("Root_Dispersion     : %d\n", p->Root_Dispersion);
		ri = htonl(p->Reference_Identifier);
		printf("Reference_Identifier: %s\n", inet_ntoa(*(struct in_addr*)&ri));
		printf("Reference_Timestamp : %s\n", strntptime(p->Reference_Timestamp));
		printf("Originate_Timestamp : %s\n", strntptime(p->Originate_Timestamp));
		printf("Receive_Timestamp   : %s\n", strntptime(p->Receive_Timestamp));
	}

	printf("Transmit_Timestamp  : %s\n", strntptime(p->Transmit_Timestamp));
}

/***
FILETIME ntpTimeToFileTime(__int64 ntpTime) {
	unsigned int a;
	FILETIME ft;
	
	a = (ntpTime & 0xffffffff00000000) >> 32;
	*(__int64*)&ft = FT19000101 + ((__int64)a * (__int64)10 * (__int64)1000 * (__int64)1000);
	return ft;
}

__int64 fileTimeToNtpTime(FILETIME ft) {
	return ((*(__int64*)&ft - FT19000101) / ((__int64)10 * (__int64)1000 * (__int64)1000)) << 32;
}
***/

/*---------------------------------------------------------------------
	�z�X�g������AIP�A�h���X���擾����
---------------------------------------------------------------------*/
unsigned long get_ipaddr(const char *hostname) {
	unsigned long result;
	struct hostent* phe;

	result = inet_addr(hostname);
	if (result == INADDR_NONE) {
		phe = gethostbyname(hostname);
		if (phe) {
			result = *(unsigned long*)phe->h_addr;
		}
	}

	return result;
}

/*---------------------------------------------------------------------
	ntp�d�����l�b�g���[�N�o�C�g�I�[�_�[�ɕϊ�
---------------------------------------------------------------------*/
void hton(sntp_packet_t* buf) {
	buf->Control = htonl(buf->Control);
	buf->Root_Delay = htonl(buf->Root_Delay);
	buf->Root_Dispersion = htonl(buf->Root_Dispersion);
	buf->Reference_Identifier = htonl(buf->Reference_Identifier);
	buf->Reference_Timestamp._seconds = htonl(buf->Reference_Timestamp._seconds);
	buf->Reference_Timestamp._fractions = htonl(buf->Reference_Timestamp._fractions);
	buf->Originate_Timestamp._seconds = htonl(buf->Originate_Timestamp._seconds);
	buf->Originate_Timestamp._fractions = htonl(buf->Originate_Timestamp._fractions);
	buf->Receive_Timestamp._seconds = htonl(buf->Receive_Timestamp._seconds);
	buf->Receive_Timestamp._fractions = htonl(buf->Receive_Timestamp._fractions);
	buf->Transmit_Timestamp._seconds = htonl(buf->Transmit_Timestamp._seconds);
	buf->Transmit_Timestamp._fractions = htonl(buf->Transmit_Timestamp._fractions);
}

/*---------------------------------------------------------------------
	ntp�d�����z�X�g�o�C�g�I�[�_�[�ɕϊ�
---------------------------------------------------------------------*/
void ntoh(sntp_packet_t* buf) {
	buf->Control = ntohl(buf->Control);
	buf->Root_Delay = ntohl(buf->Root_Delay);
	buf->Root_Dispersion = ntohl(buf->Root_Dispersion);
	buf->Reference_Identifier = ntohl(buf->Reference_Identifier);
	buf->Reference_Timestamp._seconds = ntohl(buf->Reference_Timestamp._seconds);
	buf->Reference_Timestamp._fractions = ntohl(buf->Reference_Timestamp._fractions);
	buf->Originate_Timestamp._seconds = ntohl(buf->Originate_Timestamp._seconds);
	buf->Originate_Timestamp._fractions = ntohl(buf->Originate_Timestamp._fractions);
	buf->Receive_Timestamp._seconds = ntohl(buf->Receive_Timestamp._seconds);
	buf->Receive_Timestamp._fractions = ntohl(buf->Receive_Timestamp._fractions);
	buf->Transmit_Timestamp._seconds = ntohl(buf->Transmit_Timestamp._seconds);
	buf->Transmit_Timestamp._fractions = ntohl(buf->Transmit_Timestamp._fractions);
}

/*---------------------------------------------------------------------
	sntp ���C������
---------------------------------------------------------------------*/
void sntp_main(const char* ntpHost, SOCKET s) {
	struct sockaddr_in to;
	int rets;
	int tout = 1000;
	struct sockaddr_in from;
	int fromlen = sizeof(from);
	int ret;
	sntp_packet_t sbuf;
	sntp_packet_t rbuf;
	NTP_TIME ft1;
	NTP_TIME ft2;
	NTP_TIME ft3;
	NTP_TIME ft4;
	double offset;
	double delay;

	memset(&to, 0, sizeof(to));
	to.sin_family = AF_INET;
#if defined(_OS9000)
	to.sin_addr.s_addr = get_ipaddr(ntpHost);
#else
	to.sin_addr.S_un.S_addr = get_ipaddr(ntpHost);
#endif
	to.sin_port = htons(123);

	printf("\n%s sntp ���M(%s)\n", strltime(), ntpHost);

	/* sntp���N�G�X�g�d���쐬�Ƒ��M */
	memset(&sbuf, 0, sizeof(sbuf));
	sbuf.Control = 0x23000000;
	sbuf.Transmit_Timestamp = ntp_time();
	hton(&sbuf);
	rets = sendto(s, (char*)&sbuf, sizeof(sbuf) - (128+4), 0, (struct sockaddr*)&to, sizeof(to));

	/* �^�C���A�E�g���Ԃ�ݒ肷�� */
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&tout, sizeof(tout));

	/* sntp�����҂� */
	ret = recvfrom(s, (char*)&rbuf, sizeof(rbuf), 0, (struct sockaddr*)&from, &fromlen);
	if (ret > 0) {
		ntoh(&rbuf);
		printf("%s sntp ��M %d �o�C�g\n", strltime(), ret);
		print_ntp_packet(&rbuf, 1);

		ft1 = rbuf.Originate_Timestamp;
		ft2 = rbuf.Receive_Timestamp;
		ft3 = rbuf.Transmit_Timestamp;
		ft4 = ntp_time();

		printf("T1: %s\n", strntptime(ft1));
		printf("T2: %s\n", strntptime(ft2));
		printf("T3: %s\n", strntptime(ft3));
		printf("T4: %s\n", strntptime(ft4));

		offset = ((ntp_msec(ft2) - ntp_msec(ft1)) + (ntp_msec(ft3) - ntp_msec(ft4))) / 2;	/* ���Ԃ̂��� */
		delay  = (ntp_msec(ft4) - ntp_msec(ft1)) - (ntp_msec(ft2) - ntp_msec(ft3));			/* �d���̒x�� */
		printf("offset: %f ms / delay %f ms\n", offset, delay);

		if ((fabs(delay) < 1) && (fabs(offset) > 1)) {
			adjust_time(offset);
		}

#if defined(_OS9000)
		g_ident = ntohl(to.sin_addr.s_addr);
#else
		g_ident = ntohl(to.sin_addr.S_un.S_addr);
#endif
		g_st = ntp_time();
	} else {
		printf("%s sntp ��M�^�C���A�E�g\n", strltime());
	}
}

/*---------------------------------------------------------------------
	sntp client
---------------------------------------------------------------------*/
void sntp(const char* ntpHost) {
	SOCKET s;

	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) != INVALID_SOCKET) {
		sntp_main(ntpHost, s);
		closesocket(s);
	} else {
		printf("sntp: socket error: %d\n", WSAGetLastError());
	}
}

/*---------------------------------------------------------------------
	sntpd main
---------------------------------------------------------------------*/
void sntpd_main(SOCKET s) {
	clock_t lastTick = clock();
	struct sockaddr_in me;
	int tout = 1000;

	memset(&me, 0, sizeof(me));
	me.sin_family = AF_INET;
#if defined(_OS9000)
	me.sin_addr.s_addr = INADDR_ANY;
#else
	me.sin_addr.S_un.S_addr = INADDR_ANY;
#endif
	me.sin_port = htons(123);
	if (bind(s, (struct sockaddr*)&me, sizeof(me)) != 0) {
		printf("sntpd: bind error: %d\n", WSAGetLastError());
		return;
	}

	/* �^�C���A�E�g���Ԃ�ݒ肷�� */
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&tout, sizeof(tout));

	while (1) {
		sntp_packet_t rbuf;
		struct sockaddr_in from;
		int fromlen = sizeof(from);
		int ret;
		sntp_packet_t sbuf;

		printf("�҂�: %s [%d s]\r", strltime(), (clock() - lastTick) / CLOCKS_PER_SEC);

		ret = recvfrom(s, (char*)&rbuf, sizeof(rbuf), 0, (struct sockaddr*)&from, &fromlen);
		if (ret <= 0) {
			if (strlen(g_options.m_ntp_server) != 0) {
				if ((clock() - lastTick) / CLOCKS_PER_SEC > 256) {
					sntp(g_options.m_ntp_server);
					lastTick = clock();
				}
			}
			continue;
		}

		printf("\n");

		ntoh(&rbuf);

		printf("[sntpd ��M] %s\n", strltime());
		
		print_ntp_packet(&rbuf, 0);

		memset(&sbuf, 0, sizeof(sbuf));

		sbuf.Control = rbuf.Control;

		sbuf.Control = (sbuf.Control & 0xf8ffffff) | 0x04000000;	/* LI , VN , MODE */
		sbuf.Control = (sbuf.Control & 0xff00ffff) | 0x00020000;	/* �K�w */
		sbuf.Control = (sbuf.Control & 0xffffff00) | 0x00000000;	/* ���x */

		sbuf.Originate_Timestamp = rbuf.Transmit_Timestamp;

		if (g_ident != 0) {
			sbuf.Reference_Identifier = g_ident;
			sbuf.Reference_Timestamp = g_st;
		}

		/* �d������M�������Ԃ��Z�b�g */
		sbuf.Receive_Timestamp = ntp_time();

		/* �d���𑗐M���鎞�Ԃ��Z�b�g */
		sbuf.Transmit_Timestamp = ntp_time();

		printf("[sntpd ���M] %s\n", strltime());
		print_ntp_packet(&sbuf, 1);
		hton(&sbuf);
		sendto(s, (char*)&sbuf, sizeof(sbuf) - (128+4), 0, (struct sockaddr*)&from, fromlen);
	}
}

/*---------------------------------------------------------------------
	sntp daemon mode
---------------------------------------------------------------------*/
void sntpd() {
	SOCKET s;
	
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) != INVALID_SOCKET) {
		sntpd_main(s);
		closesocket(s);
	} else {
		printf("sntpd: socket error: %d\n", WSAGetLastError());
	}
}

/*---------------------------------------------------------------------
	�g�p�@�̕\��
---------------------------------------------------------------------*/
void usage() {
	printf("�g�p�@�Fsntpd [options]\n");
	printf(" -d                �f�[�������[�h\n");
	printf(" -c <NTP�T�[�o�[>  NTP�T�[�o�[�Ɏ��v���킹�ɍs��\n");
	printf("  (ntp1.jst.mfeed.ad.jp, time.windows.com, time-b.nist.gov)\n");

	exit(0);
}

/*---------------------------------------------------------------------
	�������
---------------------------------------------------------------------*/
void chkarg(int argc, char **argv) {
	int i;

	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			if (argv[i][1] == 'd') {
				g_options.m_daemon_mode = 1;
			} else if (argv[i][1] == 'c') {
				if (argv[i][2] == '\0') {
					strcpy(g_options.m_ntp_server, argv[i+1]);
					++i;
				} else {
					strcpy(g_options.m_ntp_server, &argv[i][2]);
				}
			} else {
				usage();
			}
		}
	}
}

/*---------------------------------------------------------------------
	�v���O�����X�^�[�g
---------------------------------------------------------------------*/
int main(int argc, char** argv) {
	WSADATA wsaData;

	WSAStartup(MAKEWORD(2, 0), &wsaData);

	chkarg(argc, argv);

	if (g_options.m_daemon_mode) {
		sntpd();
	} else {
		if (strlen(g_options.m_ntp_server) > 0) {
			sntp(g_options.m_ntp_server);
		} else {
			usage();
		}
	}

	WSACleanup();
	return 0;
}
