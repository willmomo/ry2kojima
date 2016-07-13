#include "syslogd.h"


// ���O�̏o�͐�
extern _tstring s_logPath;

//---------------------------------------------------------------------
// �T�[�r�X���C���X�g�[������
//---------------------------------------------------------------------
void install(LPCTSTR path) {
	_tstring serviceName = kjm::util::load_string(SERVICE_NAME);
	_tstring serviceDisplayName = kjm::util::load_string(SERVICE_DISPLAY_NAME);

	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCManager != NULL) {

		SC_HANDLE hService = CreateService(
			hSCManager,
			serviceName.c_str(),
			serviceDisplayName.c_str(),
			SERVICE_ALL_ACCESS,
			SERVICE_WIN32_OWN_PROCESS,
			// SERVICE_DEMAND_START,	//SERVICE_AUTO_START,
			SERVICE_AUTO_START,
			SERVICE_ERROR_NORMAL,
			path,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL);

		if (hService != NULL) {
			CloseServiceHandle(hService);
		}

		CloseServiceHandle(hSCManager);
	}
}


//---------------------------------------------------------------------
// �T�[�r�X���A���C���X�g�[������
//---------------------------------------------------------------------
void uninstall() {
	_tstring serviceName = kjm::util::load_string(SERVICE_NAME);

	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCManager != NULL) {
		SC_HANDLE hService = OpenService(hSCManager, serviceName.c_str(), DELETE);
		if (hService != NULL) {
			DeleteService(hService);

			CloseServiceHandle(hService);
		}
		CloseServiceHandle(hSCManager);
	}
}


//---------------------------------------------------------------------
// �T�[�r�X 'SyslogdService' �̏�Ԃ�\������
//---------------------------------------------------------------------
void status() {
	_tstring serviceName = kjm::util::load_string(SERVICE_NAME);

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
			}
			CloseServiceHandle(hService);
		} else {
			_tcout << _T("�T�[�r�X���A�I�[�v���ł��܂���B\n");
			_tcout << _T("�G���[�R�[�h: ") << GetLastError() << _T("\n");
		}
		CloseServiceHandle(hSCManager);
	} else {
		_tcout << _T("�T�[�r�X�}�l�[�W���[�ɃA�N�Z�X�ł��܂���B\n");
		_tcout << _T("�G���[�R�[�h: ") << GetLastError() << _T("\n");
	}
}


SERVICE_STATUS_HANDLE g_ssh = NULL;
DWORD g_state = SERVICE_STOPPED;
syslogd_thread g_syslogd;			// syslogd�̃X���b�h
CRITICAL_SECTION g_criticalSection;	// syslogd�X���b�h�̊ԂŁA���O���o�����߂̔r��

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
VOID WINAPI SyslogdServiceHandler( DWORD fdwControl ) {

	switch (fdwControl) {
	case SERVICE_CONTROL_PAUSE:
		// �ꎞ��~�w��
		if (g_state == SERVICE_RUNNING) {
			g_syslogd.suspend();
			g_state = SERVICE_PAUSED;
		}
		break;

	case SERVICE_CONTROL_CONTINUE:
		// �ĊJ�w��
		if (g_state == SERVICE_PAUSED) {
			g_syslogd.resume();
			g_state = SERVICE_RUNNING;
		}
		break;

	case SERVICE_CONTROL_STOP:
		// ��~�w��
		// �T�[�r�X���ꎞ��~���̂Ƃ��́A�ĊJ������
		if (g_state == SERVICE_PAUSED) {
			g_syslogd.resume();
			g_state = SERVICE_RUNNING;
		}

		// �T�[�r�X���I��������
		if (g_state == SERVICE_RUNNING) {
			g_syslogd.end();

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
VOID WINAPI SyslogdServiceMain(DWORD dwArgc, LPTSTR* lpszArgv) {

	_tstring serviceName = kjm::util::load_string(SERVICE_NAME);

	g_ssh = RegisterServiceCtrlHandler(serviceName.c_str(), SyslogdServiceHandler);
	if ( g_ssh == NULL ) {
		MessageBox( NULL,
			_T("SyslogdService�T�[�r�X���J�n�ł��܂���ł����B"),
			_T("SyslogdService�G���["),
			MB_OK | MB_ICONERROR | MB_SERVICE_NOTIFICATION );
		return;
	}

	s_logPath = kjm::util::get_private_profile_string(_T("option"), _T("logpath"),
		kjm::util::path_append(kjm::util::get_environment_variable(_T("TEMP")).c_str(), _T("syslogd")),
		kjm::util::path_rename_extension(kjm::util::get_module_filename().c_str(), _T(".ini")).c_str());

	// syslogd�X���b�h���N������
	g_syslogd.begin();

	// �������I�������|�[�g
	ReportServiceStatus(g_ssh, g_state = SERVICE_RUNNING);

	// �T�[�r�X�X���b�h���I������܂ŁA�~�܂�
	g_syslogd.wait();
	g_syslogd.close();

	ReportServiceStatus(g_ssh, g_state = SERVICE_STOPPED);
}


//---------------------------------------------------------------------
// �v���O�����E�X�^�[�g
//---------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {
	
	if (argc != 1) {
		if (lstrcmp(argv[1], _T("install")) == 0) {
			install(kjm::util::get_module_filename().c_str());
		} else if (lstrcmp(argv[1], _T("uninstall")) == 0) {
			uninstall();
		} else if (lstrcmp(argv[1], _T("status")) == 0) {
			status();
		}

		return 0;
	}

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	InitializeCriticalSection(&g_criticalSection);

	TCHAR szServiceName[32];
	lstrcpy(szServiceName, kjm::util::load_string(SERVICE_NAME).c_str());

	SERVICE_TABLE_ENTRY services[] = {
		{szServiceName, SyslogdServiceMain},
		{NULL, NULL}
	};

	if (!StartServiceCtrlDispatcher(services)) {
		MessageBox(NULL, _T("syslogd�T�[�r�X�ŃG���[���������܂����B"), _T("syslogd�G���["), MB_OK | MB_ICONERROR | MB_SERVICE_NOTIFICATION);
	}

	DeleteCriticalSection(&g_criticalSection);

	WSACleanup();

	return 0;
}
