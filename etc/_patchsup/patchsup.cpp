/**
	file:
	brief:	�p�b�`��Ƃ̋L�^���T�|�[�g����R�}���h
	note:	unicode�́A�l�����Ă��Ȃ��B
*/

#include <windows.h>
#include <tchar.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "kjmlib.h"


using namespace std;


#define	LOG_FILE	"patchsup.log"


struct option_info {
	HINSTANCE m_hInstance;	// WinMain�̃C���X�^���X

	std::string m_f;		// -f�I�v�V����
	std::string m_m;		// -m�I�v�V����
	bool m_v;				// -v�I�v�V����

	option_info() {
		m_f = "";
		m_m = "";
		m_v = false;
	};
} g_option_info;


/**
	�������ʂ����O�Ɏc���B
*/
void logging(const char* format, ...) {

	FILE* fp = fopen(LOG_FILE, "a+");
	if (fp) {
		va_list marker;
		va_start(marker, format);	// �ό̈����̏�����
		vfprintf(fp, format, marker);
		va_end(marker);				// �ό̈����̃��Z�b�g
		fclose(fp);
	}
}


/**
	�����̃`�F�b�N
*/
int chkarg() {
	for (int i = 1; i < __argc; i++) {
		if (__argv[i][0] == '-' || __argv[i][0] == '/') {
			if (__argv[i][1] == 'f') {
				// -u�I�v�V�����B�����ݐ�t�@�C�����w�肷��I�v�V����
				if (__argv[i][2] != '\0') {
					// ���������������w�肳��Ă���
					g_option_info.m_f = kjm_ExpandEnvironmentStrings(&__argv[i][2]);
				} else if (i + 1 < __argc) {
					// ���̃I�v�V�������A���̃I�v�V�����̈���
					i++;
					g_option_info.m_f = kjm_ExpandEnvironmentStrings(__argv[i]);
				} else {
					// �s�K�؂ȃI�v�V�����w��
					return -1;
				}
			} else if (__argv[i][1] == 'm') {
				// -m�I�v�V�����B�����ݕ�������w�肷��I�v�V����
				if (__argv[i][2] != '\0') {
					// ���������������w�肳��Ă���
					g_option_info.m_m = &__argv[i][2];
				} else if (i + 1 < __argc) {
					// ���̃I�v�V�������A���̃I�v�V�����̈���
					i++;
					g_option_info.m_m = __argv[i];
				} else {
					// �s�K�؂ȃI�v�V�����w��
					return -1;
				}
			} else if (__argv[i][1] == 'v') {
				// -v�I�v�V�����B�O��܂ł̌��ʂ�\������
				g_option_info.m_v = true;
			}
		}
	}

	return 0;
}


/**
*/
BOOL CALLBACK MainDlg_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (umsg) {
	case WM_COMMAND:
		{
			WORD wID = LOWORD(wParam);
			if (wID == IDC_LOG) {
			}
		}
		break;
	}

	return FALSE;
}


/**
	���s�L�^���_�C�A���O�ŕ\��
*/
void show_dialog(vector<string>& logs) {
	
	DialogBox(g_option_info.m_hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDlg_DlgProc);
}


/**
	�w��t�@�C���Ƀp�b�`�L�^����������
*/
void do_main_file(kjm_UrlComponents& uc) {

	// �p�b�`�L�^�̍ŐV10����\������
	if (g_option_info.m_v) {
		kjm::file patch_log;
		if (patch_log.fopen(uc.m_szUrlPath.c_str(), "r")) {
			while (1) {
				string line = patch_log.read_line();
				if (line.empty()) {
					break;
				}
			}
		}
	}

	if (!g_option_info.m_m.empty()) {
		kjm::file patch_log;
		if (patch_log.fopen(uc.m_szUrlPath.c_str(), "a+")) {
			patch_log.fprintf("[%s] %s\n", kjm::time::now().strftime("%Y/%m/%d %H:%M:%S").c_str(), g_option_info.m_m.c_str());
		}
	}
}


/**
	�w��t�@�C����ftp�Ńp�b�`�L�^����������
*/
void do_main_ftp(kjm_UrlComponents& uc) {

	kjm_Internet internetSession;
	kjm_Internet ftpSession;
	kjm_Internet ftpFile;

	if (internetSession.InetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0)) {

		if (ftpSession.InetConnect(internetSession.get_handle(), uc.m_szHostName.c_str(), uc.m_nPort, uc.m_szUserName.c_str(), uc.m_szPassword.c_str(), INTERNET_SERVICE_FTP, 0, 0)) {

			DWORD rp = 0;
			std::vector<char> buf;

			if (ftpFile.InetFtpOpenFile(ftpSession.get_handle(), uc.m_szUrlPath.c_str(), GENERIC_READ, FTP_TRANSFER_TYPE_ASCII, 0)) {

				DWORD dwAvailable = 0;
				ftpFile.InetQueryDataAvailable(&dwAvailable, 0, 0);

				ftpFile.InetReadFile(buf);

				ftpFile.InetCloseHandle();
			}

			if (ftpFile.InetFtpOpenFile(ftpSession.get_handle(), uc.m_szUrlPath.c_str(), GENERIC_WRITE, FTP_TRANSFER_TYPE_ASCII, 0)) {

				char work[1024];
				wsprintf(work, _T("[%s] %s\x0d\x0a"), kjm::time::now().strftime("%Y/%m/%d %H:%M:%S").c_str());

				buf.insert(buf.end(), &work[0], &work[lstrlen(work)]);

				DWORD written = 0;
				ftpFile.InetWriteFile(buf.begin(), buf.size(), &written);

				ftpFile.InetCloseHandle();
			}
		} else {
			char buf[512];
			wsprintf(buf, _T("InetConnect error : %d\n"), ftpSession.get_lastError());
			OutputDebugString(buf);
		}
	}
}


/**
	�I�v�V�����ɉ����ď����𕪊�
*/
void do_main() {

	kjm_UrlComponents uc;
	uc.CrackUrl(g_option_info.m_f.c_str());

	if (uc.m_nScheme == INTERNET_SCHEME_FILE) {
		// �t�@�C���ւ̏����o������
		do_main_file(uc);
	} else if (uc.m_nScheme == INTERNET_SCHEME_FTP) {
		// FTP�Ńt�@�C���ɏ����o������
		do_main_ftp(uc);
	}
}


/**
	�v���O�����X�^�[�g
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	g_option_info.m_hInstance = hInstance;

	// �I�v�V�����`�F�b�N
	if (chkarg() != 0) {
		return -1;
	}

	if (g_option_info.m_f.empty()) {
		return -1;
	}

	do_main();

	return 0;
}
