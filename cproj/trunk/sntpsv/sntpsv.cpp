
#include "sntpsv.h"


// ログの出力先
kjm::log g_log;	// グローバルなログ出力クラス

//---------------------------------------------------------------------
// サービスをインストールする
//---------------------------------------------------------------------
int install(LPCTSTR path) {
	int result = 0;

	_tstring serviceName = kjm::util::loadString(SERVICE_NAME);
	_tstring serviceDisplayName = kjm::util::loadString(SERVICE_DISPLAY_NAME);

	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCManager != NULL) {

		SC_HANDLE hService = CreateService(
			hSCManager,
			serviceName.c_str(),
			serviceDisplayName.c_str(),
			SERVICE_ALL_ACCESS,
			SERVICE_WIN32_OWN_PROCESS,
			SERVICE_AUTO_START,
			SERVICE_ERROR_NORMAL,
			path,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL);

		if (hService != NULL) {
			g_log.writee(_T("install: %s(%s) [%s] をサービスとして登録しました。"), serviceDisplayName.c_str(), serviceName.c_str(), path);
			CloseServiceHandle(hService);
		} else {
			result = -1;
			DWORD dwErr = GetLastError();
			g_log.writee(_T("install: CreateService error: %d : %s"), dwErr, kjm::util::formatMessageBySystem(dwErr).c_str());
		}

		CloseServiceHandle(hSCManager);
	} else {
		result = -1;
		DWORD dwErr = GetLastError();
		g_log.writee(_T("install: OpenSCManager error: %d : %s"), dwErr, kjm::util::formatMessageBySystem(dwErr).c_str());
	}

	return result;
}


//---------------------------------------------------------------------
// サービスをアンインストールする
//---------------------------------------------------------------------
int uninstall() {
	int result = 0;

	_tstring serviceName = kjm::util::loadString(SERVICE_NAME);

	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCManager != NULL) {
		SC_HANDLE hService = OpenService(hSCManager, serviceName.c_str(), DELETE);
		if (hService != NULL) {
			if (DeleteService(hService)) {
				g_log.writee(_T("uninstall: サービス(%s)を削除しました。"), serviceName.c_str());
			} else {
				result = -1;
				g_log.writee(_T("uninstall: DeleteService error: %d"), GetLastError());
			}

			CloseServiceHandle(hService);
		} else {
			result = -1;
			g_log.writee(_T("uninstall: OpenService error: %d"), GetLastError());
		}
		CloseServiceHandle(hSCManager);
	} else {
		result = -1;
		g_log.writee(_T("uninstall: OpenSCManager error: %d"), GetLastError());
	}

	return result;
}


//---------------------------------------------------------------------
// サービス 'SntpService' の状態を表示する
//---------------------------------------------------------------------
int status() {
	int result = 0;

	_tstring serviceName = kjm::util::loadString(SERVICE_NAME);

	_tcout << serviceName << _T(" の状態\n");

	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCManager != NULL) {
		SC_HANDLE hService = OpenService(hSCManager, serviceName.c_str(), SERVICE_ALL_ACCESS);
		if (hService != NULL) {
			SERVICE_STATUS serviceStatus;
			if (QueryServiceStatus(hService, &serviceStatus)) {
				_tcout << _T("            dwServiceType = ");
				switch (serviceStatus.dwServiceType) {
				case SERVICE_WIN32_OWN_PROCESS:		_tcout << _T("SERVICE_WIN32_OWN_PROCESS\n");	break;
				case SERVICE_WIN32_SHARE_PROCESS:	_tcout << _T("SERVICE_WIN32_SHARE_PROCESS\n");	break;
				case SERVICE_KERNEL_DRIVER:			_tcout << _T("SERVICE_KERNEL_DRIVER\n");		break;
				case SERVICE_FILE_SYSTEM_DRIVER:	_tcout << _T("SERVICE_FILE_SYSTEM_DRIVER\n");	break;
				case SERVICE_INTERACTIVE_PROCESS:	_tcout << _T("SERVICE_INTERACTIVE_PROCESS\n");	break;
				default:	_tcout << _T("unknown number ") << serviceStatus.dwServiceType << _T("\n");	break;
				}

				_tcout << _T("           dwCurrentState = ");
				switch (serviceStatus.dwCurrentState) {
				case SERVICE_STOPPED:			_tcout << _T("SERVICE_STOPPED\n");			break;
				case SERVICE_START_PENDING:		_tcout << _T("SERVICE_START_PENDING\n");	break;
				case SERVICE_STOP_PENDING:		_tcout << _T("SERVICE_STOP_PENDING\n");		break;
				case SERVICE_RUNNING:			_tcout << _T("SERVICE_RUNNING\n");			break;
				case SERVICE_CONTINUE_PENDING:	_tcout << _T("SERVICE_CONTINUE_PENDING\n");	break;
				case SERVICE_PAUSE_PENDING:		_tcout << _T("SERVICE_PAUSE_PENDING\n");	break;
				case SERVICE_PAUSED:			_tcout << _T("SERVICE_PAUSED\n");			break;
				default:	_tcout << _T("unknown number ") << serviceStatus.dwCurrentState << _T("\n");	break;
				}

				_tcout << _T("       dwControlsAccepted = ");
				switch (serviceStatus.dwControlsAccepted) {
				case SERVICE_ACCEPT_STOP:			_tcout << _T("SERVICE_ACCEPT_STOP");			break;
				case SERVICE_ACCEPT_PAUSE_CONTINUE:	_tcout << _T("SERVICE_ACCEPT_PAUSE_CONTINUE");	break;
				case SERVICE_ACCEPT_SHUTDOWN:		_tcout << _T("SERVICE_ACCEPT_SHUTDOWN");		break;
				default:	_tcout << serviceStatus.dwControlsAccepted << _T("\n");	break;
				}

				_tcout << _T("          dwWin32ExitCode = ") << serviceStatus.dwWin32ExitCode << _T("\n");
				_tcout << _T("dwServiceSpecificExitCode = ") << serviceStatus.dwServiceSpecificExitCode << _T("\n");
				_tcout << _T("             dwCheckPoint = ") << serviceStatus.dwCheckPoint << _T("\n");
				_tcout << _T("               dwWaitHint = ") << serviceStatus.dwWaitHint << _T("\n");
			} else {
				_tcout << _T("サービスの状態が、取得できません。\n");
				_tcout << _T("エラーコード: ") << GetLastError() << _T("\n");
				result = -1;
			}
			CloseServiceHandle(hService);
		} else {
			_tcout << _T("サービスが、オープンできません。\n");
			_tcout << _T("エラーコード: ") << GetLastError() << _T("\n");
			result = -1;
		}
		CloseServiceHandle(hSCManager);
	} else {
		_tcout << _T("サービスマネージャーにアクセスできません。\n");
		_tcout << _T("エラーコード: ") << GetLastError() << _T("\n");
		result = -1;
	}

	return result;
}


SERVICE_STATUS_HANDLE g_ssh = NULL;
DWORD g_state = SERVICE_STOPPED;
sntpd_thread g_serviceThread;			// sntpdのスレッド

//---------------------------------------------------------------------
// サービス状態の通知
//---------------------------------------------------------------------
void ReportServiceStatus(SERVICE_STATUS_HANDLE ssh, DWORD dwState) {

	SERVICE_STATUS status = {
		SERVICE_WIN32_OWN_PROCESS,
		dwState,
		SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE,
		0, 0, 0, 0
	};

	SetServiceStatus(ssh, &status);
}


//---------------------------------------------------------------------
// サービスハンドラ
//
// [備考]
// サービスの[開始]は、ServiceMain関数が呼ばれる点に注意。
//---------------------------------------------------------------------
VOID WINAPI SntpdServiceHandler( DWORD fdwControl ) {

	switch (fdwControl) {
	case SERVICE_CONTROL_PAUSE:
		// 一時停止指示
		if (g_state == SERVICE_RUNNING) {
			g_serviceThread.suspend();
			g_state = SERVICE_PAUSED;
		}
		break;

	case SERVICE_CONTROL_CONTINUE:
		// 再開指示
		if (g_state == SERVICE_PAUSED) {
			g_serviceThread.resume();
			g_state = SERVICE_RUNNING;
		}
		break;

	case SERVICE_CONTROL_STOP:
		// 停止指示
		// サービスが一時停止中のときは、再開させる
		if (g_state == SERVICE_PAUSED) {
			g_serviceThread.resume();
			g_state = SERVICE_RUNNING;
		}

		// サービスを終了させる
		if (g_state == SERVICE_RUNNING) {
			g_serviceThread.end();

			// 実際に終了を検知するのは、ServiceMain関数。
		}
		break;

	default:
		break;
	}

	ReportServiceStatus(g_ssh, g_state);
}


//---------------------------------------------------------------------
// サービスのメイン処理
//---------------------------------------------------------------------
VOID WINAPI SntpdServiceMain(DWORD dwArgc, LPTSTR* lpszArgv) {

	_tstring serviceName = kjm::util::loadString(SERVICE_NAME);

	g_ssh = RegisterServiceCtrlHandler(serviceName.c_str(), SntpdServiceHandler);
	if ( g_ssh == NULL ) {
		MessageBox( NULL,
			_T("SntpdServiceサービスを開始できませんでした。"),
			_T("SntpdServiceエラー"),
			MB_OK | MB_ICONERROR | MB_SERVICE_NOTIFICATION );
		return;
	}

	// sntpdスレッドを起動する
	g_serviceThread.run();

	// 初期化終了をレポート
	ReportServiceStatus(g_ssh, g_state = SERVICE_RUNNING);

	// サービススレッドが終了するまで、止まる
	g_serviceThread.wait();

	ReportServiceStatus(g_ssh, g_state = SERVICE_STOPPED);
}


//---------------------------------------------------------------------
// プログラム・スタート
//---------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {
	C_SETLOCALE_JAPAN;

	g_log.set_Stop(false);

	if (argc != 1) {
		if (lstrcmp(argv[1], _T("install")) == 0) {
			return install(kjm::util::getModuleFileName().c_str());
		} else if (lstrcmp(argv[1], _T("uninstall")) == 0) {
			return uninstall();
		} else if (lstrcmp(argv[1], _T("status")) == 0) {
			return status();
		}

		return 0;
	}

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	TCHAR szServiceName[32];
	lstrcpy(szServiceName, kjm::util::loadString(SERVICE_NAME).c_str());

	SERVICE_TABLE_ENTRY services[] = {
		{szServiceName, SntpdServiceMain},
		{NULL, NULL}
	};

	if (!StartServiceCtrlDispatcher(services)) {
		MessageBox(NULL, _T("sntpサービスでエラーが発生しました。"), _T("sntpエラー"), MB_OK | MB_ICONERROR | MB_SERVICE_NOTIFICATION);
	}

	WSACleanup();

	return 0;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int g_stratum;			/* 前回の相手の階層 */
unsigned long g_ident;	/* 前回の通信相手 */
NTP_TIME g_st;			/* 前回の処理 */

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
	現在時間を文字列化
---------------------------------------------------------------------*/
_tstring strltime() {
	SYSTEMTIME st;

	GetLocalTime(&st);

	std::vector<TCHAR> buf(80);
	_sntprintf(&buf[0], buf.size(), _T("%04d/%02d/%02d %02d:%02d:%02d"),
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	return &buf[0];
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
	TCHAR bufNow[96];

	lstrcpy(bufNow, strltime().c_str());

	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft9);
	*(__int64*)&ft9 += offset * FT_MSEC;
	FileTimeToSystemTime(&ft9, &st);
	
	if (SetSystemTime(&st) == FALSE) {
		g_log.write(_T("時計設定失敗: error %d"), GetLastError());
	}

	g_log.write(_T("adjust: %s -> %s"), bufNow, strltime().c_str());
#endif
}

LPCTSTR _tinet_ntoa(struct in_addr in) {
#if !defined(UNICODE)
	return inet_ntoa(in);
#else
	static wchar_t s_work[512];
	lstrcpy(s_work, kjm::util::toUnicode(inet_ntoa(in)).c_str());
	return s_work;
#endif
}

/*---------------------------------------------------------------------
	ホスト名から、IPアドレスを取得する
---------------------------------------------------------------------*/
unsigned long get_ipaddr(LPCTSTR arg) {

	char hostname[256];
#if defined(UNICODE)
	//const char *hostname = kjm::util::to_mbcs(arg).c_str();
	strcpy(hostname, kjm::util::toAnsi(arg).c_str());
#else
	//const char *hostname = arg;
	strcpy(hostname, arg);
#endif

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
	NTP_TIMEを文字列化
---------------------------------------------------------------------*/
const TCHAR *strntptime(NTP_TIME nt) {
	static TCHAR s_buf[80];
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
	_stprintf(s_buf, _T("%04d/%02d/%02d %02d:%02d:%02d.%03d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
#else
	std::vector<TCHAR> work(80);
	time_t t;
	double units;	/* ns 単位 */

	units = ((double)nt._fractions * UNITS_PER_S / NTP_PER_S);
	if (nt._seconds & 0x80000000) {
		t = (nt._seconds >= SEC19700101) ? nt._seconds - SEC19700101 : 0;
	} else {
		t = TIME_T20360207 + nt._seconds;
	}
	_tcsftime(&work[0], work.size(), _T("%Y/%m/%d %H:%M:%S"), gmtime(&t));
	_stprintf(s_buf, _T("%s.%03d"), &work[0], (unsigned)(units / 1000000));

	/* 以下、簡易近似値計算 */
	/* sprintf(s_buf, "%s.%03d", work,nt._fractions / 5000000); */
#endif
	return s_buf;
}

/*---------------------------------------------------------------------
	NTP電文を画面に表示
---------------------------------------------------------------------*/
void print_ntp_packet(const sntp_packet_t *p, int full) {
	unsigned long ri;
	int stratum;
	TCHAR c[4];
	
	stratum = (p->Control & 0x00ff0000) >> 16;

	g_log.write(_T("Control                : %08Xh"), p->Control);
	g_log.write(_T(" LI VN MODE 階層 ポーリング 精度"));
	g_log.write(_T(" %2d %2d %4d %4d %10d %4d"), 
		(p->Control & 0xc0000000) >> 30,	/* LI */
		(p->Control & 0x38000000) >> 27,	/* VN */
		(p->Control & 0x07000000) >> 24,	/* MODE */
		stratum,							/* 階層 */
		(p->Control & 0x0000ff00) >> 8,		/* ポーリング */
		(p->Control & 0x000000ff)			/* 精度 */
	);

	if (full) {
		g_log.write (_T("Root_Delay             : %d"), p->Root_Delay);
		g_log.write (_T("Root_Dispersion        : %d"), p->Root_Dispersion);

		ri = htonl(p->Reference_Identifier);
		if (stratum == 1) {
			c[0] = (ri & 0x000000ff);		if (!_istprint(c[0])) c[0] = '_';
			c[1] = (ri & 0x0000ff00) >> 8;	if (!_istprint(c[1])) c[1] = '_';
			c[2] = (ri & 0x00ff0000) >> 16;	if (!_istprint(c[2])) c[2] = '_';
			c[3] = (ri & 0xff000000) >> 24;	if (!_istprint(c[3])) c[3] = '_';

			g_log.write (_T("Reference_Identifier   : %s '%c%c%c%c'"),
				_tinet_ntoa(*(struct in_addr*)&ri), c[0], c[1], c[2], c[3]);
		} else {
			g_log.write (_T("Reference_Identifier   : %s"), _tinet_ntoa(*(struct in_addr*)&ri));
		}

		g_log.write (_T("Reference_Timestamp    : %s [%08x.%08x]"), strntptime(p->Reference_Timestamp), p->Reference_Timestamp._seconds, p->Reference_Timestamp._fractions);
		g_log.write (_T("Originate_Timestamp(T1): %s [%08x.%08x]"), strntptime(p->Originate_Timestamp), p->Originate_Timestamp._seconds, p->Originate_Timestamp._fractions);
		g_log.write (_T("Receive_Timestamp  (T2): %s [%08x.%08x]"), strntptime(p->Receive_Timestamp), p->Receive_Timestamp._seconds, p->Receive_Timestamp._fractions);
	}

	g_log.write (_T("Transmit_Timestamp (T3): %s [%08x.%08x]"), strntptime(p->Transmit_Timestamp), p->Transmit_Timestamp._seconds, p->Transmit_Timestamp._fractions);
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

//---------------------------------------------------------------------
// オフセットが最大許容範囲内かどうか調べる
//---------------------------------------------------------------------
bool sntpd_thread::isAllowableMaxOffset(double offset) const {
	// 設定値 0 は、許容範囲内判定を行わない。
	if (this->m_allowable_max_offset == 0) {
		return true;
	}

	// オフセットの絶対値が設定値以下なら、許容範囲とする。
	if (fabs(offset) <= (double)this->m_allowable_max_offset * 1000.0) {
		return true;
	}

	return false;
}

/*---------------------------------------------------------------------
	sntp メイン処理
---------------------------------------------------------------------*/
void sntpd_thread::sntp_main(LPCTSTR ntpHost, SOCKET s) {
	struct sockaddr_in to;
	struct sockaddr_in from;
	int fromlen = sizeof(from);
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

	g_log.write(_T("******** ******** ******** ******** ******** "));
	g_log.write(_T("<sntp 送信> (%s) %s"), ntpHost, strltime().c_str());

	/* sntpリクエスト電文作成と送信 */
	memset(&sbuf, 0, sizeof(sbuf));
	sbuf.Control = 0x23000000;
	sbuf.Transmit_Timestamp = ntp_time();
	hton(&sbuf);
	int ret = sendto(s, (char*)&sbuf, sizeof(sbuf) - (128+4), 0, (struct sockaddr*)&to, sizeof(to));
	if (ret == SOCKET_ERROR) {
		g_log.write(_T("<sntp> sendto error: %d"), WSAGetLastError());
	}

	/* タイムアウト時間を設定する */
	DWORD tout = 500;
	ret = setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&tout, sizeof(tout));
	if (ret == SOCKET_ERROR) {
		g_log.write(_T("<sntp> setsockopt error: %d"), WSAGetLastError());
	}

	/* sntp応答待ち */
	ret = recvfrom(s, (char*)&rbuf, sizeof(rbuf), 0, (struct sockaddr*)&from, &fromlen);
	if (ret > 0) {
		ft4 = ntp_time();

		ntoh(&rbuf);
		g_log.write(_T("<sntp 受信> %d バイト"), ret);
		print_ntp_packet(&rbuf, 1);

		if (((rbuf.Control & 0xc0000000) >> 30) == 3) {
			g_log.write(_T("Leap not in sync."));
		} else {
			ft1 = rbuf.Originate_Timestamp;
			ft2 = rbuf.Receive_Timestamp;
			ft3 = rbuf.Transmit_Timestamp;

			g_log.write(_T("受信時間           (T4): %s [%08x %08x]"), strntptime(ft4), ft4._seconds, ft4._fractions);

			offset = ((ntp_msec(ft2) - ntp_msec(ft1)) + (ntp_msec(ft3) - ntp_msec(ft4))) / 2;	/* 時間のずれ */
			delay  = (ntp_msec(ft4) - ntp_msec(ft1)) - (ntp_msec(ft2) - ntp_msec(ft3));			/* 電文の遅延 */
			g_log.write(_T("offset: %f ms / delay %f ms"), offset, delay);

			if (rbuf.Reference_Timestamp._seconds == 0) {
				g_log.write(_T("Server has gone too long without sync server. But, sync ..."));
			}

			if ((fabs(delay)  < ((double)this->m_allowable_delay  * 1000.0)) &&
				(fabs(offset) > ((double)this->m_allowable_offset * 1000.0)) ) {
				if (this->m_noadjust != 1) {

					// 時差が最大許容範囲なら、時計を合わせる
					if (isAllowableMaxOffset(offset)) {
						adjust_time(offset);
					} else {
						g_log.write(_T("時差が %f > %f なので、時計を合わせません。"),
							fabs(offset), (double)this->m_allowable_max_offset * 1000.0);
					}
				}
			}
		}

		g_stratum = (rbuf.Control & 0x00ff0000) >> 16;
		g_ident = ntohl(to.sin_addr.s_addr);
		g_st = ntp_time();
	} else {
		g_log.write(_T("sntp 受信タイムアウト"));
	}
}

/*---------------------------------------------------------------------
	sntp client
---------------------------------------------------------------------*/
void sntpd_thread::sntp() {
	SOCKET s;
	TCHAR servers[1024];
	TCHAR *p;

	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) != INVALID_SOCKET) {
		lstrcpy(servers, this->m_ntp_server.c_str());
		p = _tcstok(servers, _T(","));
		while (p != NULL) {
			// サーバー名が指定されていないときは、何もしない。
			if (lstrlen(p) > 0) {
				sntp_main(p, s);
			}
			p = _tcstok(NULL, _T(","));
		}
		closesocket(s);
	} else {
		g_log.write(_T("sntp: socket error: %d"), WSAGetLastError());
	}
}

//-----------------------------------------------------------------------------
// デーモン待ち受け処理
//-----------------------------------------------------------------------------
void sntpd_thread::sntpd_main(SOCKET s) {
	time_t lastTime = 0;
	struct sockaddr_in me;

	memset(&me, 0, sizeof(me));
	me.sin_family = AF_INET;
	me.sin_addr.s_addr = INADDR_ANY;
	me.sin_port = htons(123);
	if (bind(s, (struct sockaddr*)&me, sizeof(me)) != 0) {
		g_log.write(_T("sntpd: bind error: %d"), WSAGetLastError());
		return;
	}

	/* タイムアウト時間を設定する */
	DWORD to = 500;
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&to, sizeof(to));

	while ( !this->is_end() ) {
		sntp_packet_t rbuf;
		struct sockaddr_in from;
		int fromlen = sizeof(from);
		int ret;
		sntp_packet_t sbuf;

		ret = recvfrom(s, (char*)&rbuf, sizeof(rbuf), 0, (struct sockaddr*)&from, &fromlen);
		if (ret <= 0) {

			if (m_ntp_server.c_str() != 0) {
				if ((lastTime == 0) || (abs((int)(time(NULL) - lastTime)) > m_interval)) {
					sntp();
					lastTime = time(NULL);
				}
			}

			continue;
		}
		NTP_TIME recvTime = ntp_time();	// 受信時間を保存

		g_log.write(_T("-------- -------- -------- -------- -------- "));

		ntoh(&rbuf);

		g_log.write(_T("[sntpd 受信] from %s:%d"), _tinet_ntoa(from.sin_addr), ntohs(from.sin_port));
		
		print_ntp_packet(&rbuf, 0);

		memset(&sbuf, 0, sizeof(sbuf));

		sbuf.Control = rbuf.Control;

		sbuf.Control = (sbuf.Control & 0x3fffffff) | 0x00000000;	// LI
		sbuf.Control = (sbuf.Control & 0xf8ffffff) | 0x04000000;	// MODE
		sbuf.Control = (sbuf.Control & 0xff00ffff) | 0x00020000;	// 階層 (当たり障りのないデフォルト)
		sbuf.Control = (sbuf.Control & 0xffffff00) | 0x000000ed;	// 精度

		sbuf.Originate_Timestamp = rbuf.Transmit_Timestamp;

		if (this->m_ntp_server.empty()) {
			// 外部に合わせに行かないときは、「自分を信じる」と応答する。
			sbuf.Reference_Identifier = 'LOCL';
			sbuf.Reference_Timestamp = ntp_time();
			sbuf.Control = (sbuf.Control & 0xff00ffff) | 0x00010000;	// 階層
		} else if (g_ident != 0) {
			sbuf.Reference_Identifier = g_ident;
			sbuf.Reference_Timestamp = g_st;
			sbuf.Control = (sbuf.Control & 0xff00ffff) | ((g_stratum + 1) << 16);	// 階層
		}

		// 電文を受信した時間をセット
		sbuf.Receive_Timestamp = recvTime;

		// 電文を送信する時間をセット
		sbuf.Transmit_Timestamp = ntp_time();

		g_log.write(_T("[sntpd 送信]"));
		print_ntp_packet(&sbuf, 1);
		hton(&sbuf);
		sendto(s, (char*)&sbuf, sizeof(sbuf) - (128+4), 0, (struct sockaddr*)&from, fromlen);
	}
}

//-----------------------------------------------------------------------------
// sntpd のワーカースレッド
//-----------------------------------------------------------------------------
unsigned sntpd_thread::worker() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	_tsetlocale(LC_ALL, _T("japanese"));

	TCHAR exePath[MAX_PATH];
	GetModuleFileName(NULL, exePath, MAX_PATH);
	PathRemoveFileSpec(exePath);
	SetEnvironmentVariable(_T("EXEPATH"), exePath);

#if 1
	_tstring logPath = kjm::util::getPrivateProfileString(_T("log"), _T("path"),
		kjm::util::append(kjm::util::getEnvironmentVariable(_T("TEMP")).c_str(), _T("sntpd")),
		kjm::util::renameExtension(kjm::util::getModuleFileName().c_str(), _T(".ini")).c_str());
	logPath = kjm::environment::expandEnvironmentVariables(logPath);
	g_log.write(_T("log path change : '%s' -> '%s'"), g_log.get_log_path().c_str(), logPath.c_str());
	g_log.set_log_path(logPath);
#else
	_tstring logPath = kjm::util::getPrivateProfileString(_T("option"), _T("logpath"),
		kjm::util::append(kjm::util::getEnvironmentVariable(_T("TEMP")).c_str(), _T("sntpd")),
		kjm::util::renameExtension(kjm::util::getModuleFileName().c_str(), _T(".ini")).c_str());
#endif

	g_log.write(_T("sntpd start."));

	// スレッドの先頭で、ini ファイルから設定を読み込む
	_tstring ini = kjm::util::pathRenameExtension(kjm::util::getModuleFileName(), _T(".ini"));

	g_log.write(_T("ini file = [%s]"), ini.c_str());

	this->m_daemon = GetPrivateProfileInt(_T("option"), _T("daemon"), 0, ini.c_str());
	this->m_ntp_server = kjm::util::getPrivateProfileString(_T("option"), _T("server"), _T(""), ini);
	this->m_interval = GetPrivateProfileInt(_T("option"), _T("interval"), 300, ini.c_str());
	this->m_noadjust = GetPrivateProfileInt(_T("option"), _T("noadjust"), 0, ini.c_str());
	this->m_allowable_offset = GetPrivateProfileInt(_T("option"), _T("AllowableOffset"), 1, ini.c_str());
	this->m_allowable_delay = GetPrivateProfileInt(_T("option"), _T("AllowableDelay"), 1, ini.c_str());
	this->m_allowable_max_offset = GetPrivateProfileInt(_T("option"), _T("AllowableMaxOffset"), 3600, ini.c_str());

	// 動作状況をログに残す
	g_log.write(_T("daemon = %d"), this->m_daemon);
	g_log.write(_T("server = %s"), this->m_ntp_server.c_str());
	g_log.write(_T("interval = %d (sec)"), this->m_interval);
	g_log.write(_T("noadjust = %d"), this->m_noadjust);
	g_log.write(_T("allowableOffset = %d (sec)"), this->m_allowable_offset);
	g_log.write(_T("allowableDelay = %d (sec)"), this->m_allowable_delay);
	g_log.write(_T("allowableMaxOffset = %d (sec)"), this->m_allowable_max_offset);

	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s != INVALID_SOCKET) {
		sntpd_main(s);
		closesocket(s);
	} else {
		g_log.write(_T("sntpd: socket error: %d"), WSAGetLastError());
	}

	g_log.write(_T("sntpd end."));

	WSACleanup();
	return 0;
}


///----------------------------------------------------------------------------------------
