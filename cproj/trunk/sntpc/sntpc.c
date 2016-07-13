/*
	http://www.venus.dti.ne.jp/~yoshi-o/NTP/NTP-SNTP_Format.html
*/

#include "sntpc.h"
#include "__os__.h"

#if defined(_WIN32)

#pragma warning(disable:4996)

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#elif defined(__linux__)

#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define WSAGetLastError()	(errno)
#define closesocket	close
#define INVALID_SOCKET (-1)
typedef int SOCKET;
typedef unsigned long DWORD;

#elif defined(_OS9000)

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

#else
#error unknown system.
#endif

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>

#if defined(_WIN32)
#pragma pack(1)
#endif

/*
	if ((_seconds & 0x80000000) == 0)
		_seconds is offset of 2036/2/7 6:28:16
	else
		_seconds is offset of 1900/1/1 0:00:00
*/
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

#define FT19000101	((__int64)94354848000000000)	/* FILETIME型で、1900/01/01 00:00:00 */
#define FT19700101	((__int64)116444736000000000)	/* FILETIME型で、1970/01/01 00:00:00 */
#define FT20360207	((__int64)137304520960000000)	/* FILETIME型で、2036/02/07 06:28:16 */
#define FT_USEC		((__int64)10)
#define FT_MSEC		(FT_USEC * (__int64)1000)
#define FT_SEC		(FT_MSEC * (__int64)1000)
#define SEC19700101	2208988800						/* 1900/01/01 から 1970/01/01 までの秒数 */
#define SEC20360207	4294967296						/* 1900/01/01 から 2036/02/07 06:28:16 までの秒数 */

#define TIME_T19700101	((time_t)0)					/* time_t 型で 1970/01/01 00:00:00 の値 */
#define TIME_T20360207	((time_t)2085978496)		/* time_t 型で 2036/02/07 06:28:16 の値 */

#define UNITS_PER_S	1e9
#define NTP_PER_S	4294967296.0

/*---------------------------------------------------------------------
	現在時間を文字列化
---------------------------------------------------------------------*/
static const char *strltime() {
#if defined(__linux__)
	static char s_buf[80];
	struct timespec tp;
	struct tm tmres;

	clock_gettime(CLOCK_REALTIME, &tp);
	localtime_r(&tp.tv_sec, &tmres);
	strftime(s_buf, sizeof(s_buf), "%Y/%m/%d %H:%M:%S", &tmres);

	return s_buf;
#else
	static char s_buf[80];
	time_t t;

	t = time(NULL);
	strftime(s_buf, sizeof(s_buf), "%Y/%m/%d %H:%M:%S", localtime(&t));

	return s_buf;
#endif
}

/*---------------------------------------------------------------------
	時計を合わせる
---------------------------------------------------------------------*/
static void adjust_time(double offset) {
#if defined(_WIN32)
	SYSTEMTIME st;
	FILETIME ft9;
	char bufNow[96];

	strcpy(bufNow, strltime());

	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft9);
	*(__int64*)&ft9 += offset * FT_MSEC;
	FileTimeToSystemTime(&ft9, &st);
	SetSystemTime(&st);

	printf("adjust: %s -> %s\n", bufNow, strltime());
#elif defined(__linux__)
	struct timespec tp;
	struct tm tmres;
	time_t utime;

	clock_gettime(CLOCK_REALTIME, &tp);
	localtime_r(&tp.tv_sec, &tmres);

	utime = time(NULL);
	utime += offset / 1000;

	stime(&utime);
#elif defined(_OS9000)
	time_t utime;
	char bufNow[96];

	strcpy(bufNow, strltime());
	utime = time(NULL);
	utime += offset / 1000;

	_os_setime(utime);

	printf("adjust: %s -> %s\n", bufNow, strltime());
#else
#error unknown system
#endif
}

/*---------------------------------------------------------------------
	NTP時間を double の msec 化

The fraction part is an unsigned binary fraction of a second i.e. MSB
== 0.5s so depending on where you get your high resolution time from:

unsigned long ntp.fraction = (double)units/UNITS_PER_S*NTP_PER_S;
where:
#define UNITS_PER_S	1E9		// or impl defined symbol
#define NTP_PER_S			4294967296
---------------------------------------------------------------------*/
static double ntp_msec(NTP_TIME nt) {
	double units;	/* ns 単位 */

	units = ((double)nt._fractions * UNITS_PER_S / NTP_PER_S);
	if (nt._seconds & 0x80000000) {
		return ((double)nt._seconds * 1000.0) + (units / 1000000.0);
	} else {
		return ((((double)(unsigned)0x80000000) + (double)nt._seconds) * 1000.0) + (units / 1000000.0);
	}

	/* 以下、簡易近似値計算 */
	/* return ((double)nt._seconds * 1000.0) + (nt._fractions / 5000000); */
}

/*---------------------------------------------------------------------
	NTP_TIMEを文字列化
---------------------------------------------------------------------*/
static const char *strntptime(NTP_TIME nt) {
	static char s_buf[80];
#if defined(USE_SYSTEMTIME)
	FILETIME ft;
	SYSTEMTIME st;
	double units;	/* ns 単位 */

	units = ((double)nt._fractions * UNITS_PER_S / NTP_PER_S);
	if (nt._seconds & 0x80000000) {
		*(__int64*)&ft = ((__int64)nt._seconds * FT_SEC + FT19000101) + (units / 1000000) * FT_MSEC;
	} else {
		*(__int64*)&ft = ((__int64)nt._seconds * FT_SEC + FT20360207) + (units / 1000000) * FT_MSEC;
	}
	FileTimeToSystemTime(&ft, &st);
	sprintf(s_buf, "%04d/%02d/%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
#else
	char work[80];
	time_t t;
	double units;	/* ns 単位 */

	units = ((double)nt._fractions * UNITS_PER_S / NTP_PER_S);
	if (nt._seconds & 0x80000000) {
		t = (nt._seconds >= SEC19700101) ? nt._seconds - SEC19700101 : 0;
	} else {
		t = TIME_T20360207 + nt._seconds;
	}
	strftime(work, sizeof(work), "%Y/%m/%d %H:%M:%S", gmtime(&t));
	sprintf(s_buf, "%s.%03d", work, (unsigned)(units / 1000000));

	/* 以下、簡易近似値計算 */
	/* sprintf(s_buf, "%s.%03d", work,nt._fractions / 5000000); */
#endif
	return s_buf;
}

/*---------------------------------------------------------------------
	1900/01/01 を起点とする UTC 時間を NTP_TIME 構造体を返す
---------------------------------------------------------------------*/
static NTP_TIME ntp_time() {
	NTP_TIME result;
#if defined(USE_SYSTEMTIME)
	SYSTEMTIME st;
	FILETIME ft;

	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);
	result._seconds = ((*(__int64*)&ft - FT19000101) / ((__int64)10 * (__int64)1000 * (__int64)1000));
	result._fractions = ((double)st.wMilliseconds * 1000000.0) / UNITS_PER_S * NTP_PER_S;

	/* 以下、簡易近似値計算 */
	/* result._fractions = st.wMilliseconds * 5000000; */
#else
	time_t t = time(NULL);
	if (t < TIME_T20360207) {	/* 2036/2/7 6:28:16 未満 */
		result._seconds = t + SEC19700101;
		result._fractions = ((double)0.0 * 1000000.0) / UNITS_PER_S * NTP_PER_S;;
	} else {
		result._seconds = t - TIME_T20360207;
		result._fractions = ((double)0.0 * 1000000.0) / UNITS_PER_S * NTP_PER_S;;
	}
#endif
	return result;
}

/*---------------------------------------------------------------------
	NTP電文を画面に表示
---------------------------------------------------------------------*/
static void print_ntp_packet(const sntp_packet_t *p, int full) {
	unsigned long ri;
	int stratum;
	int c[4];
	
	stratum = (p->Control & 0x00ff0000) >> 16;

	/* LI VN Mode Stratum(階層) Poll(ポーリング) Precision(精度) */
	printf("Control                : %08Xh\n", p->Control);
	printf(" LI VN MODE Stra Poll Prec\n");
	printf(" %2d %2d %4d %4d %4d %4d\n", 
		(p->Control & 0xc0000000) >> 30,	/* LI */
		(p->Control & 0x38000000) >> 27,	/* VN */
		(p->Control & 0x07000000) >> 24,	/* MODE */
		stratum,							/* 階層 */
		(p->Control & 0x0000ff00) >> 8,		/* ポーリング */
		(p->Control & 0x000000ff)			/* 精度 */
	);

	if (full) {
		printf("Root_Delay             : %d\n", p->Root_Delay);
		printf("Root_Dispersion        : %d\n", p->Root_Dispersion);

		ri = htonl(p->Reference_Identifier);
		if (stratum == 1) {
			c[0] = (ri & 0x000000ff);		if (!isprint(c[0])) c[0] = '_';
			c[1] = (ri & 0x0000ff00) >> 8;	if (!isprint(c[1])) c[1] = '_';
			c[2] = (ri & 0x00ff0000) >> 16;	if (!isprint(c[2])) c[2] = '_';
			c[3] = (ri & 0xff000000) >> 24;	if (!isprint(c[3])) c[3] = '_';

			printf("Reference_Identifier   : %s '%c%c%c%c'\n",
				inet_ntoa(*(struct in_addr*)&ri), c[0], c[1], c[2], c[3]);
		} else {
			printf("Reference_Identifier   : %s\n", inet_ntoa(*(struct in_addr*)&ri));
		}

		printf("Reference_Timestamp    : %s [%08x.%08x]\n", strntptime(p->Reference_Timestamp), p->Reference_Timestamp._seconds, p->Reference_Timestamp._fractions);
		printf("Originate_Timestamp(T1): %s [%08x.%08x]\n", strntptime(p->Originate_Timestamp), p->Originate_Timestamp._seconds, p->Originate_Timestamp._fractions);
		printf("Receive_Timestamp  (T2): %s [%08x.%08x]\n", strntptime(p->Receive_Timestamp), p->Receive_Timestamp._seconds, p->Receive_Timestamp._fractions);
	}

	printf("Transmit_Timestamp (T3): %s [%08x.%08x]\n", strntptime(p->Transmit_Timestamp), p->Transmit_Timestamp._seconds, p->Transmit_Timestamp._fractions);
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
	受信タイムアウトの設定
---------------------------------------------------------------------*/
static void _setrecvtimeo(SOCKET sock, DWORD to) {
#if defined(_WIN32)
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&to, sizeof(to));
#elif defined(__linux__)
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&to, sizeof(to));
#elif defined(_OS9000)
	struct spf_popts opt;
	u_int32 opt_size;

	opt_size = sizeof(opt);
	_os_gs_popt(sock, &opt_size, &opt);
	opt.pd_iotime = to / 10;		/* 受信タイムアウト値(×10ms)を設定 */
	_os_ss_popt(sock, opt_size, &opt);
#else
#error unknown system.
#endif
}

/*---------------------------------------------------------------------
	ホスト名から、IPアドレスを取得する
---------------------------------------------------------------------*/
static unsigned long get_ipaddr(const char *hostname) {
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
	ntp電文をネットワークバイトオーダーに変換
---------------------------------------------------------------------*/
static void hton(sntp_packet_t* buf) {
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
	ntp電文をホストバイトオーダーに変換
---------------------------------------------------------------------*/
static void ntoh(sntp_packet_t* buf) {
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
	sntp メイン処理

	return code:
	-10 : 受信タイムアウト
---------------------------------------------------------------------*/
static int sntp_main(const char* ntpHost, SOCKET s, const ntp_options *opt) {
	int result = 0;
	struct sockaddr_in to;
	struct sockaddr_in from;
	socklen_t fromlen = sizeof(from);
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

	printf("<sntp send> (%s) %s\n", ntpHost, strltime());

	/* sntpリクエスト電文作成と送信 */
	memset(&sbuf, 0, sizeof(sbuf));
	sbuf.Control = 0x23000000;
	sbuf.Transmit_Timestamp = ntp_time();
	hton(&sbuf);
	sendto(s, (char*)&sbuf, sizeof(sbuf) - (128+4), 0, (struct sockaddr*)&to, sizeof(to));

	/* タイムアウト時間を設定する */
	_setrecvtimeo(s, 1000);

	/* sntp応答待ち */
	ret = recvfrom(s, (char*)&rbuf, sizeof(rbuf), 0, (struct sockaddr*)&from, &fromlen);
	if (ret > 0) {
		ft4 = ntp_time();

		ntoh(&rbuf);
		printf("<sntp recv> %d byte(s)\n", ret);
		print_ntp_packet(&rbuf, 1);

		if (((rbuf.Control & 0xc0000000) >> 30) == 3) {
			puts("Leap not in sync.");
		} else {
			ft1 = rbuf.Originate_Timestamp;
			ft2 = rbuf.Receive_Timestamp;
			ft3 = rbuf.Transmit_Timestamp;

			printf("Receive time       (T4): %s [%08x %08x]\n", strntptime(ft4), ft4._seconds, ft4._fractions);

			offset = ((ntp_msec(ft2) - ntp_msec(ft1)) + (ntp_msec(ft3) - ntp_msec(ft4))) / 2;	/* 時間のずれ */
			delay  = (ntp_msec(ft4) - ntp_msec(ft1)) - (ntp_msec(ft2) - ntp_msec(ft3));			/* 電文の遅延 */
			printf("offset: %f ms / delay %f ms\n", offset, delay);

			if (rbuf.Reference_Timestamp._seconds == 0) {
				puts("Server has gone too long without sync server. But, sync ...");
			}

			if ((fabs(delay)  < ((double)opt->m_allowable_delay  * 1000.0)) &&
				(fabs(offset) > ((double)opt->m_allowable_offset * 1000.0)) ) {
				if (opt->m_noadjust != 1) {
					if (opt->m_allowable_max_offset == 0) {
						/* 時差が大きすぎても設定する(v1.4.0.0から) */
						adjust_time(offset);
					} else {
						if (fabs(offset) > (double)opt->m_allowable_max_offset * 1000.0) {
							printf("時差が %f > %f なので、時計を合わせません。\n",
								fabs(offset), (double)opt->m_allowable_max_offset * 1000.0);
						} else {
							adjust_time(offset);
						}
					}
				}
			}
		}
	} else {
		printf("sntp 受信タイムアウト\n");
		result = -10;
	}

	return result;
}

/*---------------------------------------------------------------------
	sntp client

	return code:
	-1 : socket error
---------------------------------------------------------------------*/
int sntp(const char* ntpHost, const ntp_options *opt) {
	int result = 0;
	SOCKET s;

	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) != INVALID_SOCKET) {
		result = sntp_main(ntpHost, s, opt);
		closesocket(s);
	} else {
		printf("sntp: socket error: %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

void sntp_socket_startup() {
#if defined(_WIN32)
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif
}

void sntp_socket_cleanup() {
#if defined(_WIN32)
	WSACleanup();
#endif
}
