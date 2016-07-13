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

unsigned long g_ident;	/* 前回の通信相手 */
NTP_TIME g_st;		/* 前回の処理 */

#define FT19000101	((__int64)94354848000000000)	/* FILETIME型で、1900/01/01 00:00:00 */
#define FT19700101	((__int64)116444736000000000)	/* FILETIME型で、1970/01/01 00:00:00 */
#define FT_USEC		((__int64)10)
#define FT_MSEC		(FT_USEC * (__int64)1000)
#define FT_SEC		(FT_MSEC * (__int64)1000)
#define SEC19700101	2208988800						/* 1900/01/01 から 1970/01/01 までの秒数 */

/*---------------------------------------------------------------------
	現在時間を文字列化
---------------------------------------------------------------------*/
const char *strltime() {
	static char s_buf[80];
	time_t t;

	t = time(NULL);
	strftime(s_buf, sizeof(s_buf), "%Y/%m/%d %H:%M:%S", localtime(&t));

	return s_buf;
}

/*---------------------------------------------------------------------
	時計を合わせる
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
	プログラムの動作設定
---------------------------------------------------------------------*/
struct {
	int m_daemon_mode;		/* デーモンモード 1: on */
	char m_ntp_server[80];	/* NTPサーバー名 */
} g_options;

/*---------------------------------------------------------------------
	NTP時間を double の msec 化
---------------------------------------------------------------------*/
double ntp_msec(NTP_TIME nt) {
	return ((double)nt._seconds * 1000.0) + (nt._fractions / 5000000);
}

/*---------------------------------------------------------------------
	NTP_TIMEを文字列化
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
	1900/01/01 を起点とする UTC 時間を NTP_TIME 構造体を返す
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
	NTP電文を画面に表示
---------------------------------------------------------------------*/
void print_ntp_packet(const sntp_packet_t *p, int full) {
	unsigned long ri;

	printf("Control             : %08Xh\n", p->Control);
	printf(" 閏秒 バージョン モード 階層 ポーリング 精度\n");
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
	ホスト名から、IPアドレスを取得する
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
	ntp電文をネットワークバイトオーダーに変換
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
	ntp電文をホストバイトオーダーに変換
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
	sntp メイン処理
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

	printf("\n%s sntp 送信(%s)\n", strltime(), ntpHost);

	/* sntpリクエスト電文作成と送信 */
	memset(&sbuf, 0, sizeof(sbuf));
	sbuf.Control = 0x23000000;
	sbuf.Transmit_Timestamp = ntp_time();
	hton(&sbuf);
	rets = sendto(s, (char*)&sbuf, sizeof(sbuf) - (128+4), 0, (struct sockaddr*)&to, sizeof(to));

	/* タイムアウト時間を設定する */
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&tout, sizeof(tout));

	/* sntp応答待ち */
	ret = recvfrom(s, (char*)&rbuf, sizeof(rbuf), 0, (struct sockaddr*)&from, &fromlen);
	if (ret > 0) {
		ntoh(&rbuf);
		printf("%s sntp 受信 %d バイト\n", strltime(), ret);
		print_ntp_packet(&rbuf, 1);

		ft1 = rbuf.Originate_Timestamp;
		ft2 = rbuf.Receive_Timestamp;
		ft3 = rbuf.Transmit_Timestamp;
		ft4 = ntp_time();

		printf("T1: %s\n", strntptime(ft1));
		printf("T2: %s\n", strntptime(ft2));
		printf("T3: %s\n", strntptime(ft3));
		printf("T4: %s\n", strntptime(ft4));

		offset = ((ntp_msec(ft2) - ntp_msec(ft1)) + (ntp_msec(ft3) - ntp_msec(ft4))) / 2;	/* 時間のずれ */
		delay  = (ntp_msec(ft4) - ntp_msec(ft1)) - (ntp_msec(ft2) - ntp_msec(ft3));			/* 電文の遅延 */
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
		printf("%s sntp 受信タイムアウト\n", strltime());
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

	/* タイムアウト時間を設定する */
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&tout, sizeof(tout));

	while (1) {
		sntp_packet_t rbuf;
		struct sockaddr_in from;
		int fromlen = sizeof(from);
		int ret;
		sntp_packet_t sbuf;

		printf("待ち: %s [%d s]\r", strltime(), (clock() - lastTick) / CLOCKS_PER_SEC);

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

		printf("[sntpd 受信] %s\n", strltime());
		
		print_ntp_packet(&rbuf, 0);

		memset(&sbuf, 0, sizeof(sbuf));

		sbuf.Control = rbuf.Control;

		sbuf.Control = (sbuf.Control & 0xf8ffffff) | 0x04000000;	/* LI , VN , MODE */
		sbuf.Control = (sbuf.Control & 0xff00ffff) | 0x00020000;	/* 階層 */
		sbuf.Control = (sbuf.Control & 0xffffff00) | 0x00000000;	/* 精度 */

		sbuf.Originate_Timestamp = rbuf.Transmit_Timestamp;

		if (g_ident != 0) {
			sbuf.Reference_Identifier = g_ident;
			sbuf.Reference_Timestamp = g_st;
		}

		/* 電文を受信した時間をセット */
		sbuf.Receive_Timestamp = ntp_time();

		/* 電文を送信する時間をセット */
		sbuf.Transmit_Timestamp = ntp_time();

		printf("[sntpd 送信] %s\n", strltime());
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
	使用法の表示
---------------------------------------------------------------------*/
void usage() {
	printf("使用法：sntpd [options]\n");
	printf(" -d                デーモンモード\n");
	printf(" -c <NTPサーバー>  NTPサーバーに時計合わせに行く\n");
	printf("  (ntp1.jst.mfeed.ad.jp, time.windows.com, time-b.nist.gov)\n");

	exit(0);
}

/*---------------------------------------------------------------------
	引数解析
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
	プログラムスタート
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
