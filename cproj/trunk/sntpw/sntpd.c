/*
	http://www.venus.dti.ne.jp/~yoshi-o/NTP/NTP-SNTP_Format.html
*/

#if defined(_WIN32)
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
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

#include "sntpd.h"

#pragma pack(1)

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

ntp_options g_options;

int g_stratum;			/* 前回の相手の階層 */
unsigned long g_ident;	/* 前回の通信相手 */
NTP_TIME g_st;			/* 前回の処理 */

static int s_loop = 1;		/* sntp_thread を制御するフラグ */
static int s_adjustNow = 0;	/* sntp_thread で今すぐ時計合わせを制御するフラグ */

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
	sntp_thread を停止させる
---------------------------------------------------------------------*/
void sntp_thread_end() {
	s_loop = 0;
}

/*---------------------------------------------------------------------
	sntp_thread で今すぐ同期させる
---------------------------------------------------------------------*/
void sntp_thread_adjust() {
	s_adjustNow = 1;
}

/*---------------------------------------------------------------------
	sntp の動作設定を設定する
---------------------------------------------------------------------*/
void sntp_set_options(int dameon_mode, const char *ntp_server, int noadjust, int interval) {
	g_options.m_daemon_mode = dameon_mode;
	if (ntp_server)
		strcpy(g_options.m_ntp_server, ntp_server);
	else
		g_options.m_ntp_server[0] = 0;
	g_options.m_noadjust = noadjust;
	g_options.m_interval = interval;
}

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
	NTP時間を double の msec 化

The fraction part is an unsigned binary fraction of a second i.e. MSB
== 0.5s so depending on where you get your high resolution time from:

unsigned long ntp.fraction = (double)units/UNITS_PER_S*NTP_PER_S;
where:
#define UNITS_PER_S	1E9		// or impl defined symbol
#define NTP_PER_S			4294967296
---------------------------------------------------------------------*/
double ntp_msec(NTP_TIME nt) {
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
const char *strntptime(NTP_TIME nt) {
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
NTP_TIME ntp_time() {
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
void print_ntp_packet(const sntp_packet_t *p, int full) {
	unsigned long ri;
	int stratum;
	int c[4];
	
	stratum = (p->Control & 0x00ff0000) >> 16;

	KLogWriteE("Control                : %08Xh", p->Control);
	KLogWriteE(" LI VN MODE 階層 ポーリング 精度");
	KLogWriteE(" %2d %2d %4d %4d %10d %4d", 
		(p->Control & 0xc0000000) >> 30,	/* LI */
		(p->Control & 0x38000000) >> 27,	/* VN */
		(p->Control & 0x07000000) >> 24,	/* MODE */
		stratum,							/* 階層 */
		(p->Control & 0x0000ff00) >> 8,		/* ポーリング */
		(p->Control & 0x000000ff)			/* 精度 */
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
	受信タイムアウトの設定
---------------------------------------------------------------------*/
void _setrecvtimeo(SOCKET sock, DWORD to) {
#if !defined(_OS9000)
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&to, sizeof(to));
#else
	struct spf_popts opt;
	u_int32 opt_size;

	opt_size = sizeof(opt);
	_os_gs_popt(sock, &opt_size, &opt);
	opt.pd_iotime = to / 10;		/* 受信タイムアウト値(×10ms)を設定 */
	_os_ss_popt(sock, opt_size, &opt);
#endif
}

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
void sntp_main(const char* ntpHost, SOCKET s, const ntp_options *opt) {
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
	KLogWriteE("<sntp 送信> (%s) %s", ntpHost, strltime());

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
		KLogWriteE("<sntp 受信> %d バイト", ret);
		print_ntp_packet(&rbuf, 1);

		if (((rbuf.Control & 0xc0000000) >> 30) == 3) {
			KLogWriteE("Leap not in sync.");
		} else {
			ft1 = rbuf.Originate_Timestamp;
			ft2 = rbuf.Receive_Timestamp;
			ft3 = rbuf.Transmit_Timestamp;

			KLogWriteE("受信時間           (T4): %s [%08x %08x]", strntptime(ft4), ft4._seconds, ft4._fractions);

			offset = ((ntp_msec(ft2) - ntp_msec(ft1)) + (ntp_msec(ft3) - ntp_msec(ft4))) / 2;	/* 時間のずれ */
			delay  = (ntp_msec(ft4) - ntp_msec(ft1)) - (ntp_msec(ft2) - ntp_msec(ft3));			/* 電文の遅延 */
			KLogWriteE("offset: %f ms / delay %f ms", offset, delay);

			if (rbuf.Reference_Timestamp._seconds == 0) {
				KLogWriteE("Server has gone too long without sync server. But, sync ...");
			}

			if ((fabs(delay)  < ((double)opt->m_allowable_delay  * 1000.0)) &&
				(fabs(offset) > ((double)opt->m_allowable_offset * 1000.0)) ) {
				if (opt->m_noadjust != 1) {
					if (opt->m_allowable_max_offset == 0) {
						// 時差が大きすぎても設定する(v1.4.0.0から)
						adjust_time(offset);
					} else {
						if (fabs(offset) > (double)opt->m_allowable_max_offset * 1000.0) {
							KLogWriteE("時差が %f > %f なので、時計を合わせません。",
								fabs(offset), (double)opt->m_allowable_max_offset * 1000.0);
						} else {
							adjust_time(offset);
						}
					}
				}
			}
		}

		g_stratum = (rbuf.Control & 0x00ff0000) >> 16;
		g_ident = ntohl(to.sin_addr.s_addr);
		g_st = ntp_time();
	} else {
		KLogWriteE("sntp 受信タイムアウト");
	}
}

/*---------------------------------------------------------------------
	sntp client
---------------------------------------------------------------------*/
void sntp(const char* ntpHost, const ntp_options *opt) {
	SOCKET s;
	char servers[1024];
	char *p;

	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) != INVALID_SOCKET) {
		strcpy(servers, ntpHost);
		p = strtok(servers, ",");
		while (p != NULL) {
			// サーバー名が指定されていないときは、何もしない。
			if (strlen(p) > 0) {
				sntp_main(p, s, opt);
			}
			p = strtok(NULL, ",");
		}
		closesocket(s);
	} else {
		KLogWriteE("sntp: socket error: %d\n", WSAGetLastError());
	}
}

/*---------------------------------------------------------------------
	sntp client
---------------------------------------------------------------------*/
void sntp_loop(const char* ntpHost, int interval) {
	DWORD tick;

	do {
		sntp(ntpHost, &g_options);

		printf("待ち: %s [%d sec]\n", strltime(), interval);
		tick = GetTickCount();
		while (((GetTickCount() - tick) / 1000) < interval) {
			Sleep(1000);
		}
	} while (1);
}

/*---------------------------------------------------------------------
	sntp client
---------------------------------------------------------------------*/
unsigned __stdcall sntp_thread(void* argv) {
	ntp_options npt_opt;
	DWORD tick;

	npt_opt = *(ntp_options *)argv;

	s_loop = 1;
	do {
		sntp(npt_opt.m_ntp_server, &npt_opt);
		s_adjustNow = 0;

		if (npt_opt.m_interval <= 0) {
			break;
		} else {
			printf("待ち: %s [%d sec]\n", strltime(), npt_opt.m_interval);
			tick = GetTickCount();
			while (((GetTickCount() - tick) / 1000) < npt_opt.m_interval) {
				if (s_loop == 0 || s_adjustNow != 0) break;
				Sleep(1000);
			}

			if (s_adjustNow) KLogWrite("(sntp) 即時時計合わせの指示がありました。");
		}
	} while (s_loop);

	return 0;
}

/*---------------------------------------------------------------------
	sntpd main
---------------------------------------------------------------------*/
void sntpd_main(SOCKET s, int intervalSec, const char *ntp_server, const ntp_options *opt) {
	time_t lastTime = 0;
	struct sockaddr_in me;
	NTP_TIME recvTime;

	memset(&me, 0, sizeof(me));
	me.sin_family = AF_INET;
	me.sin_addr.s_addr = INADDR_ANY;
	me.sin_port = htons(123);
	if (bind(s, (struct sockaddr*)&me, sizeof(me)) != 0) {
		KLogWriteE("sntpd: bind error: %d", WSAGetLastError());
		return;
	}

	printf("待ち: %s [%d sec]\n", strltime(), intervalSec);

	/* タイムアウト時間を設定する */
	_setrecvtimeo(s, 1000);

	s_loop = 1;
	while (s_loop) {
		sntp_packet_t rbuf;
		struct sockaddr_in from;
		int fromlen = sizeof(from);
		int ret;
		sntp_packet_t sbuf;

#if !defined(_OS9000)
		printf("待ち: %s [%d/%d sec]\r", strltime(), (time(NULL) - lastTime), intervalSec);
#endif

		ret = recvfrom(s, (char*)&rbuf, sizeof(rbuf), 0, (struct sockaddr*)&from, &fromlen);
		if (ret <= 0) {
			if (strlen(ntp_server) != 0) {
				if ((s_adjustNow != 0) || (lastTime == 0) || (abs(time(NULL) - lastTime) > intervalSec)) {
					if (s_adjustNow) KLogWrite("(sntpd) 即時時計合わせの指示がありました。");
					s_adjustNow = 0;
					sntp(ntp_server, opt);
					lastTime = time(NULL);
				}
			}
			continue;
		}
		recvTime = ntp_time();	// 受信時間を記録

#if !defined(_OS9000)
		printf("待ち: %s [%d sec]\n", strltime(), intervalSec);
#endif
		puts("\n");
		KLogWrite("-------- -------- -------- -------- -------- ");

		ntoh(&rbuf);

		KLogWriteE("[sntpd 受信] from %s:%d", inet_ntoa(from.sin_addr), ntohs(from.sin_port));
		
		print_ntp_packet(&rbuf, 0);

		memset(&sbuf, 0, sizeof(sbuf));

		sbuf.Control = rbuf.Control;

		sbuf.Control = (sbuf.Control & 0x3fffffff) | 0x00000000;	/* LI */
		sbuf.Control = (sbuf.Control & 0xf8ffffff) | 0x04000000;	/* MODE */
		sbuf.Control = (sbuf.Control & 0xff00ffff) | 0x00020000;	/* 階層 *//* 当たり障りのないデフォルト */
		sbuf.Control = (sbuf.Control & 0xffffff00) | 0x000000ed;	/* 精度 */

		sbuf.Originate_Timestamp = rbuf.Transmit_Timestamp;

		if (strlen(ntp_server) == 0) {
			/* 外部に合わせに行かないときは、「自分を信じる」と応答する。 */
			sbuf.Reference_Identifier = 'LOCL';
			sbuf.Reference_Timestamp = ntp_time();
			sbuf.Control = (sbuf.Control & 0xff00ffff) | 0x00010000;	/* 階層 */
		} else if (g_ident != 0) {
			sbuf.Reference_Identifier = g_ident;
			sbuf.Reference_Timestamp = g_st;
			sbuf.Control = (sbuf.Control & 0xff00ffff) | ((g_stratum + 1) << 16);	/* 階層 */
		}

		/* 電文を受信した時間をセット */
		sbuf.Receive_Timestamp = recvTime;

		/* 電文を送信する時間をセット */
		sbuf.Transmit_Timestamp = ntp_time();

		KLogWriteE("[sntpd 送信]");
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
		sntpd_main(s,
			(g_options.m_interval <= 0) ? (5 * 60) : g_options.m_interval,
			g_options.m_ntp_server,
			&g_options);
		closesocket(s);
	} else {
		KLogWriteE("sntpd: socket error: %d\n", WSAGetLastError());
	}
}

/*---------------------------------------------------------------------
	sntp daemon thread mode
---------------------------------------------------------------------*/
unsigned __stdcall sntpd_thread(void* argv) {
	SOCKET s;
	ntp_options npt_opt;

	npt_opt = *(ntp_options *)argv;
	
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) != INVALID_SOCKET) {
		sntpd_main(s,
			(npt_opt.m_interval <= 0) ? (5 * 60) : npt_opt.m_interval,
			npt_opt.m_ntp_server,
			&npt_opt);
		closesocket(s);
	} else {
		KLogWriteE("sntpd: socket error: %d", WSAGetLastError());
	}

	return 0;
}

/*---------------------------------------------------------------------
	使用法の表示
---------------------------------------------------------------------*/
void usage() {
	printf("使用法：sntpd [options]\n");
	printf(" -d                デーモンモード\n");
	printf(" -c <NTPサーバー>  NTPサーバーに時計合わせに行く\n");
	printf("  (ntp.nict.jp, ntp1.jst.mfeed.ad.jp, time.windows.com, time-b.nist.gov)\n");
	printf(" -i <interval sec> 時計合わせの間隔\n");
	printf("  -d を指定しているときは、デーモンが時計を合わせる間隔。");
	printf("  -d 未指定のときは、クライアントモードで継続的に時計を合わせる。");
	printf(" -n                no adjust\n");

	exit(0);
}

/*---------------------------------------------------------------------
	引数解析
---------------------------------------------------------------------*/
void chkarg(int argc, char **argv) {
	int i;

	g_options.m_allowable_delay = 1;
	g_options.m_allowable_offset = 1;
	g_options.m_allowable_max_offset = 3600;

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
			} else if (argv[i][1] == 'i') {
				if (argv[i][2] == '\0') {
					g_options.m_interval = atoi(argv[i+1]);
					++i;
				} else {
					g_options.m_interval = atoi(&argv[i][2]);
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

	KLogInit();

#if !defined(_OS9000)
	{
		char drv[_MAX_PATH];
		char path[_MAX_PATH];
		char logpath[_MAX_PATH];
		_splitpath(_pgmptr, drv, path, NULL, NULL);
		_makepath(logpath, drv, path, NULL, NULL);
		PathAppend(logpath, "log");
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
			if (g_options.m_interval <= 0) {
				sntp(g_options.m_ntp_server, &g_options);
			} else {
				sntp_loop(g_options.m_ntp_server, g_options.m_interval);
			}
		} else {
			usage();
		}
	}

	WSACleanup();
	return 0;
}
