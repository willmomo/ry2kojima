#include "syslogd.h"

// ���O�̏o�͐�(�O�̂��߂ɏ����t�H���_�͎w�肵�Ă���)
_tstring s_logPath = kjm::util::path_append(kjm::util::get_environment_variable(_T("TEMP")).c_str(), _T("syslogd"));

//---------------------------------------------------------------------
// ��M�f�[�^�����O�Ɏc��
//---------------------------------------------------------------------
void Logging(const sockaddr_in* from, LPCTSTR pszText) {
	EnterCriticalSection(&g_criticalSection);

	SYSTEMTIME st;
	GetLocalTime(&st);

	// �o�̓t�@�C�����̍쐬
	///_tstring tempPath = kjm::util::get_environment_variable(_T("TEMP"));
	///tempPath = kjm::util::path_append(tempPath.c_str(), _T("syslogd"));

	static DWORD s_lastDelTime = 0;	// �ŏI�폜����
	if ((s_lastDelTime == 0) ||
		((GetTickCount() - s_lastDelTime) > 24 * 60 * 60 * 1000)) {

		int nSave = GetPrivateProfileInt(_T("option"), _T("savedays"), 90,
			kjm::util::path_rename_extension(kjm::util::get_module_filename().c_str(), _T(".ini")).c_str());

		kjm::util::cleanup_dir(s_logPath.c_str(), nSave);

		s_lastDelTime = GetTickCount();
	}

	// �t�@�C���������̍쐬
	TCHAR fname[MAX_PATH];
	wsprintf(fname, _T("syslogd%04d%02d%02d.log"), st.wYear, st.wMonth, st.wDay);

	// �t���p�X���̍쐬
	_tstring fullPath = kjm::util::path_append(s_logPath.c_str(), fname);

	FILE* fp = _tfopen(fullPath.c_str(), _T("a+"));
	if (fp == NULL && errno == ENOENT) {
		// �t�H���_�������Ď��s���Ă���悤�Ȃ̂ŁA�t�H���_�̍쐬����
		// �ēx�A�I�[�v�������݂�
		CreateDirectory(s_logPath.c_str(), NULL);
		fp = _tfopen(fullPath.c_str(), _T("a+"));
	}

	if (fp) {
		_ftprintf(fp, _T("%04d/%02d/%02d %02d:%02d:%02d.%03d "), 
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		_ftprintf(fp, _T(" [%04X] "), GetCurrentThreadId());
		if (from == NULL) {
			_ftprintf(fp, _T("%-16s %5u "), _T("0.0.0.0"), 0);
		} else {
			_ftprintf(fp, _T("%-16s %5u "), inet_ntoa(from->sin_addr), ntohs(from->sin_port));
		}

		_ftprintf(fp, _T("%s\n"), pszText);

		fclose(fp);
	}

	LeaveCriticalSection(&g_criticalSection);
}

//---------------------------------------------------------------------
// ��M�f�[�^���o�C�i���f�[�^�Ƃ��ăt�@�C���Ɏc��
//---------------------------------------------------------------------
void LoggingBinary(const sockaddr_in* from, char* pData, size_t size) {
	static DWORD s_seq = 0;

	SYSTEMTIME st;
	GetLocalTime(&st);

	// �o�̓t�@�C�����̍쐬
	///_tstring tempPath = kjm::util::get_environment_variable(_T("TEMP"));
	///tempPath = kjm::util::path_append(tempPath.c_str(), _T("syslogd"));

	// �t�@�C���̍폜�́ALogging�֐��ɔC����

	// �t�@�C���������̍쐬
	// �~���b�܂œ����ł��A�V�[�P���X�ԍ����ς��̂ŁA�t�@�C�����̏d���͖������̂Ƃ��Ĉ����B
	TCHAR fname[MAX_PATH];
	wsprintf(fname, _T("syslogd%04d%02d%02d_%02d%02d%02d_%03d_%u.bin"),
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, s_seq++);

	// �t���p�X���̍쐬
	_tstring fullPath = kjm::util::path_append(s_logPath.c_str(), fname);

	FILE* fp = _tfopen(fullPath.c_str(), _T("wb"));
	if (fp == NULL && errno == ENOENT) {
		// �t�H���_�������Ď��s���Ă���悤�Ȃ̂ŁA�t�H���_�̍쐬����
		// �ēx�A�I�[�v�������݂�
		CreateDirectory(s_logPath.c_str(), NULL);
		fp = _tfopen(fullPath.c_str(), _T("wb"));
	}

	if (fp) {
		fwrite(pData, sizeof(char), size, fp);
		fclose(fp);

		TCHAR work[512];
		wsprintf(work, _T("%4d �o�C�g�̃o�C�i���f�[�^����M '%s' �ɕۑ��B"), size, fname);
		Logging(from, work);
	}
}


//---------------------------------------------------------------------
// ��M�����f�[�^���o�C�i�����ǂ������ׂ�
//---------------------------------------------------------------------
bool is_binary(const char* p, size_t size) {
	for (int i = 0; i < size; i++) {
		if (_ismbcprint(p[i] & 0xff) == 0) {
			// �o�C�i���̉\������
			if (_ismbblead(p[i] & 0xff) != 0 && _ismbbtrail(p[i+1] & 0xff) != 0) {
				// ��o�C�g�����������̂ŁA�ꕶ����΂��Ď���
				i += 1;
			} else {
				// ��o�C�g�����ł͂Ȃ������̂ŁA�o�C�i���Ɣ��f
				return true;
			}
		}
	}

	return false;
}

//---------------------------------------------------------------------
// syslog�̐S����
//---------------------------------------------------------------------
unsigned syslogd_thread::worker() {
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == INVALID_SOCKET) {
		return 1;
	}

	//	�^�C���A�E�g�̐ݒ�
	//	�~���b�P�ʂŐݒ�
	int timeout = 300;
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));

	sockaddr_in myaddr;
	memset(&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(514);

	if (bind(s, (sockaddr*)&myaddr, sizeof(myaddr)) != 0) {
		MessageBox(NULL,
			_T("bind�Ɏ��s���܂����B"),
			_T("Syslogd Service�G���["),
			MB_OK | MB_ICONERROR | MB_SERVICE_NOTIFICATION);
		return 2;
	}

	char buf1[8192];
	wchar_t buf2[8192];
	char buf3[8192];
	sockaddr_in from;
	int fromlen;

	Logging(NULL, _T("---- logging �J�n ----"));

	// ��M���镶����̕����R�[�h���K�肷��B���́AUTF8���ASJIS�̂ǂ��炩�B
	// ��M�悪TeraStation�̎��AUTF8���w�肷��B
	_tstring charset = kjm::util::get_private_profile_string(_T("option"), _T("charset"), _T(""),
		kjm::util::path_rename_extension(kjm::util::get_module_filename().c_str(), _T(".ini")).c_str());

	while (still_loop()) {
		fromlen = sizeof(from);
		int ret = recvfrom(s, buf1, sizeof(buf1) - 1, 0, (sockaddr*)&from, &fromlen);

		if (ret > 0) {
			buf1[ret] = '\0';

			if (charset.empty()) {
				if (is_binary(buf1, ret) == false) {
					Logging(&from, buf1);
				} else {
					LoggingBinary(&from, buf1, ret);
				}
			} else {
				MultiByteToWideChar(CP_UTF8, 0, buf1, -1, buf2, sizeof(buf2));
				WideCharToMultiByte(CP_ACP, 0, buf2, -1, buf3, sizeof(buf3), NULL, NULL);

				// �I�[�̉��s����菜��
				if (buf3[lstrlen(buf3) - 1] == '\n') {
					buf3[lstrlen(buf3) - 1] = '\0';
					--ret;
				}

				Logging(&from, buf3);
			}

		}
	}

	Logging(NULL, _T("---- logging �I�� ----"));

	closesocket(s);

	return 0;
}

//---------------------------------------------------------------------
// �X���b�h�̏I��(���O���c��)
//---------------------------------------------------------------------
void syslogd_thread::end() {
	Logging(NULL, _T("---- �X���b�h�I���w�� ----"));
	thread::end();
}

//---------------------------------------------------------------------
// �T�X�y���h(���O���c��)
//---------------------------------------------------------------------
DWORD syslogd_thread::suspend() {
	Logging(NULL, _T("---- �ꎞ��~�w�� ----"));
	return thread::suspend();
}

//---------------------------------------------------------------------
// ���W���[��(���O���c��)
//---------------------------------------------------------------------
DWORD syslogd_thread::resume() {
	Logging(NULL, _T("---- �ĊJ�w�� ----"));
	return thread::resume();
}

//---------------------------------------------------------------------
// �I���ҋ@(���O���c��)
//---------------------------------------------------------------------
DWORD syslogd_thread::wait(DWORD dwTimeout) {
	Logging(NULL, _T("---- �X���b�h�I���ҋ@ ----"));
	DWORD dwRet = thread::wait(dwTimeout);
	Logging(NULL, _T("---- �X���b�h�I�� ----"));

	return dwRet;
}
