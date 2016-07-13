#pragma once

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <windows.h>
#include <tchar.h>

#include <time.h>
#include <math.h>

#include "./kjmlib_20130904/kjmlib.h"

#include "resource.h"

extern kjm::log g_log;	// グローバルなログ出力クラス


//-----------------------------------------------------------------------------

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

class sntpd_thread : public kjm::thread {
public:
	// コンストラクタ
	sntpd_thread() : m_daemon(0), m_ntp_server(), m_interval(5 * 60), m_allowable_delay(1), m_allowable_offset(1), m_allowable_max_offset(3600), m_noadjust(0) {};

	// デストラクタ
	virtual ~sntpd_thread() {};

	// ワーカースレッド
	virtual unsigned worker();

	void sntpd_main(SOCKET s);

	void sntp();

	void sntp_main(LPCTSTR server, SOCKET s);

private:
	int m_daemon;
	_tstring m_ntp_server;
	int m_interval;
	int m_allowable_delay;
	int m_allowable_offset;
	int m_allowable_max_offset;
	int m_noadjust;

	// オフセットが最大許容範囲内かどうか調べる
	bool isAllowableMaxOffset(double offset) const;

};
