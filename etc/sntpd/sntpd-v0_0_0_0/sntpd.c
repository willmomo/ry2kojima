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
#include "kjmlog.h"

#if !defined(_OS9000)
#include <direct.h>
#include <mbstring.h>
#include <sys/stat.h>
#endif

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

/*---------------------------------------------------------------------
	�v���O�����̓���ݒ�
---------------------------------------------------------------------*/
struct {
	int m_daemon_mode;		/* �f�[�������[�h 1: on */
	char m_ntp_server[80];	/* NTP�T�[�o�[�� */
	int m_noadjust;			/* ���v�킹�����Ȃ� 1: ���Ȃ� */
} g_options;

int g_stratum;			/* �O��̑���̊K�w */
unsigned long g_ident;	/* �O��̒ʐM���� */
NTP_TIME g_st;			/* �O��̏��� */

#define FT19000101	((__int64)94354848000000000)	/* FILETIME�^�ŁA1900/01/01 00:00:00 */
#define FT19700101	((__int64)116444736000000000)	/* FILETIME�^�ŁA1970/01/01 00:00:00 */
#define FT_USEC		((__int64)10)
#define FT_MSEC		(FT_USEC * (__int64)1000)
#define FT_SEC		(FT_MSEC * (__int64)1000)
#define SEC19700101	2208988800						/* 1900/01/01 ���� 1970/01/01 �܂ł̕b�� */

#define UNITS_PER_S	1e9
#define NTP_PER_S	4294967296.0

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
#if defined(_OS9000)
	time_t utime;

	utime = time(NULL);
	utime += offset / 1000;

	_os_setime(utime);
#else
	SYSTEMTIME st;
	FILETIME ft9;
	char bufNow[96];

	strcpy(bufNow, strltime());

	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft9);
	*(__int64*)&ft9 += offset * FT_MSEC;
	FileTimeToSystemTime(&ft9, &st);
	SetSystemTime(&st);

	KLogWriteE("adjust: %s -> %s", bufNow, strltime());
#endif
}

/*---------------------------------------------------------------------
	NTP���Ԃ� double �� msec ��

The fraction part is an unsigned binary fraction of a second i.e. MSB
== 0.5s so depending on where you get your high resolution time from:

unsigned long ntp.fraction = (double)units/UNITS_PER_S*NTP_PER_S;
where:
#define UNITS_PER_S	1E9		// or impl defined symbol
#define NTP_PER_S			4294967296
---------------------------------------------------------------------*/
double ntp_msec(NTP_TIME nt) {
	double units;	/* ns �P�� */
	units = ((double)nt._fractions * UNITS_PER_S / NTP_PER_S);
	return ((double)nt._seconds * 1000.0) + (units / 1000000.0);
	
	/* �ȉ��A�ȈՋߎ��l�v�Z */
	/* return ((double)nt._seconds * 1000.0) + (nt._fractions / 5000000); */
}

/*---------------------------------------------------------------------
	NTP_TIME�𕶎���
---------------------------------------------------------------------*/
const char *strntptime(NTP_TIME nt) {
	static char s_buf[80];
#if defined(USE_SYSTEMTIME)
	FILETIME ft;
	SYSTEMTIME st;
	double units;	/* ns �P�� */

	units = ((double)nt._fractions * UNITS_PER_S / NTP_PER_S);
	*(__int64*)&ft = ((__int64)nt._seconds * FT_SEC + FT19000101) + (units / 1000000) * FT_MSEC;
	FileTimeToSystemTime(&ft, &st);
	sprintf(s_buf, "%04d/%02d/%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
#else
	char work[80];
	time_t t;
	double units;	/* ns �P�� */

	units = ((double)nt._fractions * UNITS_PER_S / NTP_PER_S);
	t = (nt._seconds >= SEC19700101) ? nt._seconds - SEC19700101 : 0;
	strftime(work, sizeof(work), "%Y/%m/%d %H:%M:%S", gmtime(&t));
	sprintf(s_buf, "%s.%03d", work, (unsigned)(units / 1000000));

	/* �ȉ��A�ȈՋߎ��l�v�Z */
	/* sprintf(s_buf, "%s.%03d", work,nt._fractions / 5000000); */
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
	result._fractions = ((double)st.wMilliseconds * 1000000.0) / UNITS_PER_S * NTP_PER_S;

	/* �ȉ��A�ȈՋߎ��l�v�Z */
	/* result._fractions = st.wMilliseconds * 5000000; */
#else
	result._seconds = time(NULL) + 2208988800;
	result._fractions = ((double)0.0 * 1000000.0) / UNITS_PER_S * NTP_PER_S;;
#endif
	return result;
}

/*---------------------------------------------------------------------
	NTP�d������ʂɕ\��
---------------------------------------------------------------------*/
void print_ntp_packet(const sntp_packet_t *p, int full) {
	unsigned long ri;
	int stratum;
	int c[4];
	
	stratum = (p->Control & 0x00ff0000) >> 16;

	KLogWriteE("Control                : %08Xh", p->Control);
	KLogWriteE(" LI VN MODE �K�w �|�[�����O ���x");
	KLogWriteE(" %2d %2d %4d %4d %10d %4d", 
		(p->Control & 0xc0000000) >> 30,	/* LI */
		(p->Control & 0x38000000) >> 27,	/* VN */
		(p->Control & 0x07000000) >> 24,	/* MODE */
		stratum,							/* �K�w */
		(p->Control & 0x0000ff00) >> 8,		/* �|�[�����O */
		(p->Control & 0x000000ff)			/* ���x */
	);

	if (full) {
		KLogWriteE("Root_Delay             : %d", p->Root_Delay);
		KLogWriteE("Root_Dispersion        : %d", p->Root_Dispersion);

		ri = htonl(p->Reference_Identifier);
		if (stratum == 1) {
			c[0] = (ri & 0x000000ff);		if (!isprint(c[0])) c[0] = '_';
			c[1] = (ri & 0x0000ff00) >> 8;	if (!isprint(c[1])) c[1] = '_';
			c[2] = (ri & 0x00ff0000) >> 16;	if (!isprint(c[2])) c[2] = '_';
			c[3] = (ri & 0xff000000) >> 24;	if (!isprint(c[3])) c[3] = '_';

			KLogWriteE("Reference_Identifier   : %s '%c%c%c%c'",
				inet_ntoa(*(struct in_addr*)&ri), c[0], c[1], c[2], c[3]);
		} else {
			KLogWriteE("Reference_Identifier   : %s", inet_ntoa(*(struct in_addr*)&ri));
		}

		KLogWriteE("Reference_Timestamp    : %s [%08x.%08x]", strntptime(p->Reference_Timestamp), p->Reference_Timestamp._seconds, p->Reference_Timestamp._fractions);
		KLogWriteE("Originate_Timestamp(T1): %s [%08x.%08x]", strntptime(p->Originate_Timestamp), p->Originate_Timestamp._seconds, p->Originate_Timestamp._fractions);
		KLogWriteE("Receive_Timestamp  (T2): %s [%08x.%08x]", strntptime(p->Receive_Timestamp), p->Receive_Timestamp._seconds, p->Receive_Timestamp._fractions);
	}

	KLogWriteE("Transmit_Timestamp (T3): %s [%08x.%08x]", strntptime(p->Transmit_Timestamp), p->Transmit_Timestamp._seconds, p->Transmit_Timestamp._fractions);
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
	��M�^�C���A�E�g�̐ݒ�
---------------------------------------------------------------------*/
void _setrecvtimeo(SOCKET sock, DWORD to) {
#if !defined(_OS9000)
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&to, sizeof(to));
#else
	struct spf_popts opt;
	u_int32 opt_size;

	opt_size = sizeof(opt);
	_os_gs_popt(sock, &opt_size, &opt);
	opt.pd_iotime = to / 10;		/* ��M�^�C���A�E�g�l(�~10ms)��ݒ� */
	_os_ss_popt(sock, opt_size, &opt);
#endif
}

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
	to.sin_addr.s_addr = get_ipaddr(ntpHost);
	to.sin_port = htons(123);

	puts("");
	KLogWrite("******** ******** ******** ******** ******** ");
	KLogWriteE("<sntp ���M> (%s)", ntpHost);

	/* sntp���N�G�X�g�d���쐬�Ƒ��M */
	memset(&sbuf, 0, sizeof(sbuf));
	sbuf.Control = 0x23000000;
	sbuf.Transmit_Timestamp = ntp_time();
	hton(&sbuf);
	sendto(s, (char*)&sbuf, sizeof(sbuf) - (128+4), 0, (struct sockaddr*)&to, sizeof(to));

	/* �^�C���A�E�g���Ԃ�ݒ肷�� */
	_setrecvtimeo(s, 1000);

	/* sntp�����҂� */
	ret = recvfrom(s, (char*)&rbuf, sizeof(rbuf), 0, (struct sockaddr*)&from, &fromlen);
	if (ret > 0) {
		ft4 = ntp_time();

		ntoh(&rbuf);
		KLogWriteE("<sntp ��M> %d �o�C�g", ret);
		print_ntp_packet(&rbuf, 1);

		if (((rbuf.Control & 0xc0000000) >> 30) == 3) {
			KLogWriteE("Leap not in sync.");
		} else {
			ft1 = rbuf.Originate_Timestamp;
			ft2 = rbuf.Receive_Timestamp;
			ft3 = rbuf.Transmit_Timestamp;

			KLogWriteE("��M����           (T4): %s [%08x %08x]", strntptime(ft4), ft4._seconds, ft4._fractions);

			offset = ((ntp_msec(ft2) - ntp_msec(ft1)) + (ntp_msec(ft3) - ntp_msec(ft4))) / 2;	/* ���Ԃ̂��� */
			delay  = (ntp_msec(ft4) - ntp_msec(ft1)) - (ntp_msec(ft2) - ntp_msec(ft3));			/* �d���̒x�� */
			KLogWriteE("offset: %f ms / delay %f ms", offset, delay);

			if (rbuf.Reference_Timestamp._seconds == 0) {
				KLogWriteE("Server has gone too long without sync server. But, sync ...");
			}

			if ((fabs(delay) < 1000.0) && (fabs(offset) > 1000.0)) {
				if (g_options.m_noadjust != 1) {
					adjust_time(offset);
				}
			}
		}

		g_stratum = (rbuf.Control & 0x00ff0000) >> 16;
		g_ident = ntohl(to.sin_addr.s_addr);
		g_st = ntp_time();
	} else {
		KLogWriteE("sntp ��M�^�C���A�E�g");
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
	time_t lastTime = time(NULL);
	struct sockaddr_in me;

	memset(&me, 0, sizeof(me));
	me.sin_family = AF_INET;
	me.sin_addr.s_addr = INADDR_ANY;
	me.sin_port = htons(123);
	if (bind(s, (struct sockaddr*)&me, sizeof(me)) != 0) {
		KLogWriteE("sntpd: bind error: %d\n", WSAGetLastError());
		return;
	}

	/* �^�C���A�E�g���Ԃ�ݒ肷�� */
	_setrecvtimeo(s, 1000);

	while (1) {
		sntp_packet_t rbuf;
		struct sockaddr_in from;
		int fromlen = sizeof(from);
		int ret;
		sntp_packet_t sbuf;

		printf("�҂�: %s [%d s]\r", strltime(), (time(NULL) - lastTime));

		ret = recvfrom(s, (char*)&rbuf, sizeof(rbuf), 0, (struct sockaddr*)&from, &fromlen);
		if (ret <= 0) {
			if (strlen(g_options.m_ntp_server) != 0) {
				if (abs(time(NULL) - lastTime) > 256) {
					sntp(g_options.m_ntp_server);
					lastTime = time(NULL);
				}
			}
			continue;
		}

		puts("\n");
		KLogWrite("-------- -------- -------- -------- -------- ");

		ntoh(&rbuf);

		KLogWriteE("[sntpd ��M] from %s:%d", inet_ntoa(from.sin_addr), ntohs(from.sin_port));
		
		print_ntp_packet(&rbuf, 0);

		memset(&sbuf, 0, sizeof(sbuf));

		sbuf.Control = rbuf.Control;

		sbuf.Control = (sbuf.Control & 0x3fffffff) | 0x00000000;	/* LI */
		sbuf.Control = (sbuf.Control & 0xf8ffffff) | 0x04000000;	/* MODE */
		sbuf.Control = (sbuf.Control & 0xff00ffff) | 0x00020000;	/* �K�w *//* ��������̂Ȃ��f�t�H���g */
		sbuf.Control = (sbuf.Control & 0xffffff00) | 0x000000ed;	/* ���x */

		sbuf.Originate_Timestamp = rbuf.Transmit_Timestamp;

		if (strlen(g_options.m_ntp_server) == 0) {
			/* �O���ɍ��킹�ɍs���Ȃ��Ƃ��́A�u������M����v�Ɖ�������B */
			sbuf.Reference_Identifier = 'LOCL';
			sbuf.Control = (sbuf.Control & 0xff00ffff) | 0x00010000;	/* �K�w */
		} else if (g_ident != 0) {
			sbuf.Reference_Identifier = g_ident;
			sbuf.Reference_Timestamp = g_st;
			sbuf.Control = (sbuf.Control & 0xff00ffff) | ((g_stratum + 1) << 16);	/* �K�w */
		}

		/* �d������M�������Ԃ��Z�b�g */
		sbuf.Receive_Timestamp = ntp_time();

		/* �d���𑗐M���鎞�Ԃ��Z�b�g */
		sbuf.Transmit_Timestamp = ntp_time();

		KLogWriteE("[sntpd ���M]");
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
		KLogWriteE("sntpd: socket error: %d\n", WSAGetLastError());
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
	printf(" -n                no adjust\n");

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
			} else if (argv[i][1] == 'n') {
				g_options.m_noadjust = 1;
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

	KLogInit();

#if !defined(_OS9000)
	{
		char drv[_MAX_PATH];
		char path[_MAX_PATH];
		char logpath[_MAX_PATH];
		_splitpath(_pgmptr, drv, path, NULL, NULL);
		_makepath(logpath, drv, path, NULL, NULL);
		path_append(logpath, "log");
		_mkdir(logpath);
		KLogSetLogPath(logpath);
	}
#endif

	KLogWrite("---- %s start. ----", argv[0]);

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
