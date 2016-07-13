
#include "sntpsv.h"


// ���O�̏o�͐�
kjm::log g_log;	// �O���[�o���ȃ��O�o�̓N���X

//---------------------------------------------------------------------
// �T�[�r�X���C���X�g�[������
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
			g_log.writee(_T("install: %s(%s) [%s] ���T�[�r�X�Ƃ��ēo�^���܂����B"), serviceDisplayName.c_str(), serviceName.c_str(), path);
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
// �T�[�r�X���A���C���X�g�[������
//---------------------------------------------------------------------
int uninstall() {
	int result = 0;

	_tstring serviceName = kjm::util::loadString(SERVICE_NAME);

	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCManager != NULL) {
		SC_HANDLE hService = OpenService(hSCManager, serviceName.c_str(), DELETE);
		if (hService != NULL) {
			if (DeleteService(hService)) {
				g_log.writee(_T("uninstall: �T�[�r�X(%s)���폜���܂����B"), serviceName.c_str());
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
// �T�[�r�X 'SntpService' �̏�Ԃ�\������
//---------------------------------------------------------------------
int status() {
	int result = 0;

	_tstring serviceName = kjm::util::loadString(SERVICE_NAME);

	_tcout << serviceName << _T(" �̏��\n");

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
				_tcout << _T("�T�[�r�X�̏�Ԃ��A�擾�ł��܂���B\n");
				_tcout << _T("�G���[�R�[�h: ") << GetLastError() << _T("\n");
				result = -1;
			}
			CloseServiceHandle(hService);
		} else {
			_tcout << _T("�T�[�r�X���A�I�[�v���ł��܂���B\n");
			_tcout << _T("�G���[�R�[�h: ") << GetLastError() << _T("\n");
			result = -1;
		}
		CloseServiceHandle(hSCManager);
	} else {
		_tcout << _T("�T�[�r�X�}�l�[�W���[�ɃA�N�Z�X�ł��܂���B\n");
		_tcout << _T("�G���[�R�[�h: ") << GetLastError() << _T("\n");
		result = -1;
	}

	return result;
}


SERVICE_STATUS_HANDLE g_ssh = NULL;
DWORD g_state = SERVICE_STOPPED;
sntpd_thread g_serviceThread;			// sntpd�̃X���b�h

//---------------------------------------------------------------------
// �T�[�r�X��Ԃ̒ʒm
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
// �T�[�r�X�n���h��
//
// [���l]
// �T�[�r�X��[�J�n]�́AServiceMain�֐����Ă΂��_�ɒ��ӁB
//---------------------------------------------------------------------
VOID WINAPI SntpdServiceHandler( DWORD fdwControl ) {

	switch (fdwControl) {
	case SERVICE_CONTROL_PAUSE:
		// �ꎞ��~�w��
		if (g_state == SERVICE_RUNNING) {
			g_serviceThread.suspend();
			g_state = SERVICE_PAUSED;
		}
		break;

	case SERVICE_CONTROL_CONTINUE:
		// �ĊJ�w��
		if (g_state == SERVICE_PAUSED) {
			g_serviceThread.resume();
			g_state = SERVICE_RUNNING;
		}
		break;

	case SERVICE_CONTROL_STOP:
		// ��~�w��
		// �T�[�r�X���ꎞ��~���̂Ƃ��́A�ĊJ������
		if (g_state == SERVICE_PAUSED) {
			g_serviceThread.resume();
			g_state = SERVICE_RUNNING;
		}

		// �T�[�r�X���I��������
		if (g_state == SERVICE_RUNNING) {
			g_serviceThread.end();

			// ���ۂɏI�������m����̂́AServiceMain�֐��B
		}
		break;

	default:
		break;
	}

	ReportServiceStatus(g_ssh, g_state);
}


//---------------------------------------------------------------------
// �T�[�r�X�̃��C������
//---------------------------------------------------------------------
VOID WINAPI SntpdServiceMain(DWORD dwArgc, LPTSTR* lpszArgv) {

	_tstring serviceName = kjm::util::loadString(SERVICE_NAME);

	g_ssh = RegisterServiceCtrlHandler(serviceName.c_str(), SntpdServiceHandler);
	if ( g_ssh == NULL ) {
		MessageBox( NULL,
			_T("SntpdService�T�[�r�X���J�n�ł��܂���ł����B"),
			_T("SntpdService�G���["),
			MB_OK | MB_ICONERROR | MB_SERVICE_NOTIFICATION );
		return;
	}

	// sntpd�X���b�h���N������
	g_serviceThread.run();

	// �������I�������|�[�g
	ReportServiceStatus(g_ssh, g_state = SERVICE_RUNNING);

	// �T�[�r�X�X���b�h���I������܂ŁA�~�܂�
	g_serviceThread.wait();

	ReportServiceStatus(g_ssh, g_state = SERVICE_STOPPED);
}


//---------------------------------------------------------------------
// �v���O�����E�X�^�[�g
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
		MessageBox(NULL, _T("sntp�T�[�r�X�ŃG���[���������܂����B"), _T("sntp�G���["), MB_OK | MB_ICONERROR | MB_SERVICE_NOTIFICATION);
	}

	WSACleanup();

	return 0;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int g_stratum;			/* �O��̑���̊K�w */
unsigned long g_ident;	/* �O��̒ʐM���� */
NTP_TIME g_st;			/* �O��̏��� */

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
	if (nt._seconds & 0x80000000) {
		return ((double)nt._seconds * 1000.0) + (units / 1000000.0);
	} else {
		return ((((double)(unsigned)0x80000000) + (double)nt._seconds) * 1000.0) + (units / 1000000.0);
	}

	/* �ȉ��A�ȈՋߎ��l�v�Z */
	/* return ((double)nt._seconds * 1000.0) + (nt._fractions / 5000000); */
}

/*---------------------------------------------------------------------
	���ݎ��Ԃ𕶎���
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
	TCHAR bufNow[96];

	lstrcpy(bufNow, strltime().c_str());

	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft9);
	*(__int64*)&ft9 += offset * FT_MSEC;
	FileTimeToSystemTime(&ft9, &st);
	
	if (SetSystemTime(&st) == FALSE) {
		g_log.write(_T("���v�ݒ莸�s: error %d"), GetLastError());
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
	�z�X�g������AIP�A�h���X���擾����
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
	time_t t = time(NULL);
	if (t < TIME_T20360207) {	/* 2036/2/7 6:28:16 ���� */
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
	NTP_TIME�𕶎���
---------------------------------------------------------------------*/
const TCHAR *strntptime(NTP_TIME nt) {
	static TCHAR s_buf[80];
#if defined(USE_SYSTEMTIME)
	FILETIME ft;
	SYSTEMTIME st;
	double units;	/* ns �P�� */

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
	double units;	/* ns �P�� */

	units = ((double)nt._fractions * UNITS_PER_S / NTP_PER_S);
	if (nt._seconds & 0x80000000) {
		t = (nt._seconds >= SEC19700101) ? nt._seconds - SEC19700101 : 0;
	} else {
		t = TIME_T20360207 + nt._seconds;
	}
	_tcsftime(&work[0], work.size(), _T("%Y/%m/%d %H:%M:%S"), gmtime(&t));
	_stprintf(s_buf, _T("%s.%03d"), &work[0], (unsigned)(units / 1000000));

	/* �ȉ��A�ȈՋߎ��l�v�Z */
	/* sprintf(s_buf, "%s.%03d", work,nt._fractions / 5000000); */
#endif
	return s_buf;
}

/*---------------------------------------------------------------------
	NTP�d������ʂɕ\��
---------------------------------------------------------------------*/
void print_ntp_packet(const sntp_packet_t *p, int full) {
	unsigned long ri;
	int stratum;
	TCHAR c[4];
	
	stratum = (p->Control & 0x00ff0000) >> 16;

	g_log.write(_T("Control                : %08Xh"), p->Control);
	g_log.write(_T(" LI VN MODE �K�w �|�[�����O ���x"));
	g_log.write(_T(" %2d %2d %4d %4d %10d %4d"), 
		(p->Control & 0xc0000000) >> 30,	/* LI */
		(p->Control & 0x38000000) >> 27,	/* VN */
		(p->Control & 0x07000000) >> 24,	/* MODE */
		stratum,							/* �K�w */
		(p->Control & 0x0000ff00) >> 8,		/* �|�[�����O */
		(p->Control & 0x000000ff)			/* ���x */
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

//---------------------------------------------------------------------
// �I�t�Z�b�g���ő勖�e�͈͓����ǂ������ׂ�
//---------------------------------------------------------------------
bool sntpd_thread::isAllowableMaxOffset(double offset) const {
	// �ݒ�l 0 �́A���e�͈͓�������s��Ȃ��B
	if (this->m_allowable_max_offset == 0) {
		return true;
	}

	// �I�t�Z�b�g�̐�Βl���ݒ�l�ȉ��Ȃ�A���e�͈͂Ƃ���B
	if (fabs(offset) <= (double)this->m_allowable_max_offset * 1000.0) {
		return true;
	}

	return false;
}

/*---------------------------------------------------------------------
	sntp ���C������
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
	g_log.write(_T("<sntp ���M> (%s) %s"), ntpHost, strltime().c_str());

	/* sntp���N�G�X�g�d���쐬�Ƒ��M */
	memset(&sbuf, 0, sizeof(sbuf));
	sbuf.Control = 0x23000000;
	sbuf.Transmit_Timestamp = ntp_time();
	hton(&sbuf);
	int ret = sendto(s, (char*)&sbuf, sizeof(sbuf) - (128+4), 0, (struct sockaddr*)&to, sizeof(to));
	if (ret == SOCKET_ERROR) {
		g_log.write(_T("<sntp> sendto error: %d"), WSAGetLastError());
	}

	/* �^�C���A�E�g���Ԃ�ݒ肷�� */
	DWORD tout = 500;
	ret = setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&tout, sizeof(tout));
	if (ret == SOCKET_ERROR) {
		g_log.write(_T("<sntp> setsockopt error: %d"), WSAGetLastError());
	}

	/* sntp�����҂� */
	ret = recvfrom(s, (char*)&rbuf, sizeof(rbuf), 0, (struct sockaddr*)&from, &fromlen);
	if (ret > 0) {
		ft4 = ntp_time();

		ntoh(&rbuf);
		g_log.write(_T("<sntp ��M> %d �o�C�g"), ret);
		print_ntp_packet(&rbuf, 1);

		if (((rbuf.Control & 0xc0000000) >> 30) == 3) {
			g_log.write(_T("Leap not in sync."));
		} else {
			ft1 = rbuf.Originate_Timestamp;
			ft2 = rbuf.Receive_Timestamp;
			ft3 = rbuf.Transmit_Timestamp;

			g_log.write(_T("��M����           (T4): %s [%08x %08x]"), strntptime(ft4), ft4._seconds, ft4._fractions);

			offset = ((ntp_msec(ft2) - ntp_msec(ft1)) + (ntp_msec(ft3) - ntp_msec(ft4))) / 2;	/* ���Ԃ̂��� */
			delay  = (ntp_msec(ft4) - ntp_msec(ft1)) - (ntp_msec(ft2) - ntp_msec(ft3));			/* �d���̒x�� */
			g_log.write(_T("offset: %f ms / delay %f ms"), offset, delay);

			if (rbuf.Reference_Timestamp._seconds == 0) {
				g_log.write(_T("Server has gone too long without sync server. But, sync ..."));
			}

			if ((fabs(delay)  < ((double)this->m_allowable_delay  * 1000.0)) &&
				(fabs(offset) > ((double)this->m_allowable_offset * 1000.0)) ) {
				if (this->m_noadjust != 1) {

					// �������ő勖�e�͈͂Ȃ�A���v�����킹��
					if (isAllowableMaxOffset(offset)) {
						adjust_time(offset);
					} else {
						g_log.write(_T("������ %f > %f �Ȃ̂ŁA���v�����킹�܂���B"),
							fabs(offset), (double)this->m_allowable_max_offset * 1000.0);
					}
				}
			}
		}

		g_stratum = (rbuf.Control & 0x00ff0000) >> 16;
		g_ident = ntohl(to.sin_addr.s_addr);
		g_st = ntp_time();
	} else {
		g_log.write(_T("sntp ��M�^�C���A�E�g"));
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
			// �T�[�o�[�����w�肳��Ă��Ȃ��Ƃ��́A�������Ȃ��B
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
// �f�[�����҂��󂯏���
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

	/* �^�C���A�E�g���Ԃ�ݒ肷�� */
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
		NTP_TIME recvTime = ntp_time();	// ��M���Ԃ�ۑ�

		g_log.write(_T("-------- -------- -------- -------- -------- "));

		ntoh(&rbuf);

		g_log.write(_T("[sntpd ��M] from %s:%d"), _tinet_ntoa(from.sin_addr), ntohs(from.sin_port));
		
		print_ntp_packet(&rbuf, 0);

		memset(&sbuf, 0, sizeof(sbuf));

		sbuf.Control = rbuf.Control;

		sbuf.Control = (sbuf.Control & 0x3fffffff) | 0x00000000;	// LI
		sbuf.Control = (sbuf.Control & 0xf8ffffff) | 0x04000000;	// MODE
		sbuf.Control = (sbuf.Control & 0xff00ffff) | 0x00020000;	// �K�w (��������̂Ȃ��f�t�H���g)
		sbuf.Control = (sbuf.Control & 0xffffff00) | 0x000000ed;	// ���x

		sbuf.Originate_Timestamp = rbuf.Transmit_Timestamp;

		if (this->m_ntp_server.empty()) {
			// �O���ɍ��킹�ɍs���Ȃ��Ƃ��́A�u������M����v�Ɖ�������B
			sbuf.Reference_Identifier = 'LOCL';
			sbuf.Reference_Timestamp = ntp_time();
			sbuf.Control = (sbuf.Control & 0xff00ffff) | 0x00010000;	// �K�w
		} else if (g_ident != 0) {
			sbuf.Reference_Identifier = g_ident;
			sbuf.Reference_Timestamp = g_st;
			sbuf.Control = (sbuf.Control & 0xff00ffff) | ((g_stratum + 1) << 16);	// �K�w
		}

		// �d������M�������Ԃ��Z�b�g
		sbuf.Receive_Timestamp = recvTime;

		// �d���𑗐M���鎞�Ԃ��Z�b�g
		sbuf.Transmit_Timestamp = ntp_time();

		g_log.write(_T("[sntpd ���M]"));
		print_ntp_packet(&sbuf, 1);
		hton(&sbuf);
		sendto(s, (char*)&sbuf, sizeof(sbuf) - (128+4), 0, (struct sockaddr*)&from, fromlen);
	}
}

//-----------------------------------------------------------------------------
// sntpd �̃��[�J�[�X���b�h
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

	// �X���b�h�̐擪�ŁAini �t�@�C������ݒ��ǂݍ���
	_tstring ini = kjm::util::pathRenameExtension(kjm::util::getModuleFileName(), _T(".ini"));

	g_log.write(_T("ini file = [%s]"), ini.c_str());

	this->m_daemon = GetPrivateProfileInt(_T("option"), _T("daemon"), 0, ini.c_str());
	this->m_ntp_server = kjm::util::getPrivateProfileString(_T("option"), _T("server"), _T(""), ini);
	this->m_interval = GetPrivateProfileInt(_T("option"), _T("interval"), 300, ini.c_str());
	this->m_noadjust = GetPrivateProfileInt(_T("option"), _T("noadjust"), 0, ini.c_str());
	this->m_allowable_offset = GetPrivateProfileInt(_T("option"), _T("AllowableOffset"), 1, ini.c_str());
	this->m_allowable_delay = GetPrivateProfileInt(_T("option"), _T("AllowableDelay"), 1, ini.c_str());
	this->m_allowable_max_offset = GetPrivateProfileInt(_T("option"), _T("AllowableMaxOffset"), 3600, ini.c_str());

	// ����󋵂����O�Ɏc��
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
