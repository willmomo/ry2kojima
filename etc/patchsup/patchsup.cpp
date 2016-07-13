/**
	@file
	@brief	�p�b�`�̃o�b�`���T�|�[�g���郁�C�����[�`��
*/

#pragma warning(disable : 4786)

#include <windows.h>
#include <windowsx.h>
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

#include "../kjmlib/kjmlib.h"
#include <list>
#include "resource.h"


using namespace std;


kjm::logfile g_log;
kjm::UrlComponents g_uc;


list<string> g_lines;


class cmdinfo : public kjm::command_line_info {
public:
	/// �R���X�g���N�^
	cmdinfo() : m_file(), m_message(), m_view(false) {
	};

	/**
		���̃v���O�����ŗL�̃p�����[�^���
		@note
		�ȉ��̃p�����[�^�𕪐͂���@n
		-f @<�t�@�C����@>@n
		-m @<���b�Z�[�W@>
	*/
	virtual bool parse_command(bool flag, const char* current, const char* next) {
		if (flag) {
			if (lstrcmpi(current, "f") == 0) {
				if (next) {
					m_file = next;
					return true;
				} else {
					return false;
				}
			} else if (lstrcmpi(current, "m") == 0) {
				if (next) {
					m_message = next;
					return true;
				} else {
					return false;
				}
			} else if (lstrcmp(current, "v") == 0) {
				m_view = true;
				return false;
			}
		}

		return false;
	};

public:
	string m_file;		///< -f �Ŏw�肳�ꂽ�t�@�C����
	string m_message;	///< -m �Ŏw�肳�ꂽ���b�Z�[�W
	bool m_view;		///< -v ���w�肳��Ă��邩�ǂ���

	HINSTANCE m_hInstance;	///< WinMain�̈���
};

cmdinfo g_ci;


/**
	�w�肳�ꂽ�t�@�C���ɁA�w�肳�ꂽ���b�Z�[�W��ǋL����B
*/
void file_w(const char* fname)
{
	kjm::file f;
	if (f.fopen(fname, "a+")) {
		f.fprintf("[%s] %s\n", kjm::time::now().strftime("%Y/%m/%d %H:%M:%S").c_str(), g_ci.m_message.c_str());
	} else {
		g_log.log_f("file_w(): error %d: �t�@�C�� '%s' ���A�I�[�v���ł��܂���B", errno, fname);
	}
}


/**
	�w�肳�ꂽ�t�@�C������A�S�s���t���Ƀ��X�g�ɓǂݍ��ށB
*/
void file_r(const char* fname)
{
	kjm::file f;
	if (f.fopen(fname, "r")) {
		string str;
		while ((str = f.fgets()).empty() == false) {
			g_lines.push_front(str);
		}
	} else {
		g_log.log_f("file_r(): error %d: �t�@�C�� '%s' ���A�I�[�v���ł��܂���B", errno, fname);
	}
}

/**
	�w�肳�ꂽ�t�@�C���ɁA�w�肳�ꂽ���b�Z�[�W��FTP���g���ĒǋL����B
*/
void ftp_w()
{
	kjm::Internet internetSession;
	kjm::Internet ftpSession;

	if (internetSession.InetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0)) {

		if (ftpSession.InetConnect(internetSession, g_uc.m_szHostName.c_str(), g_uc.m_nPort, g_uc.m_szUserName.c_str(), g_uc.m_szPassword.c_str(), INTERNET_SERVICE_FTP, 0, 0)) {

			// ��M���郍�[�J���t�@�C�������쐬
			char tempPath[MAX_PATH];
			char tempFileName[MAX_PATH];

			GetTempPath(sizeof(tempPath), tempPath);
			GetTempFileName(tempPath, "pat", 0, tempFileName);

			ftpSession.InetFtpGetFile(g_uc.m_szUrlPath.c_str(), tempFileName, FALSE, FILE_ATTRIBUTE_TEMPORARY, FTP_TRANSFER_TYPE_ASCII, 0);

			file_w(tempFileName);

			ftpSession.InetFtpPutFile(tempFileName, g_uc.m_szUrlPath.c_str(), INTERNET_FLAG_TRANSFER_ASCII, 0);

			DeleteFile(tempFileName);
		} else {
			char msg[512];
			wsprintf(msg, "InetConnect error : %d\n", ftpSession.get_lastError());
			MessageBox(NULL, msg, "�p�b�`�L�^�m�F�G���[", MB_OK);
		}
	} else {
		char msg[512];
		wsprintf(msg, "internetSession error : %d\n", ftpSession.get_lastError());
		MessageBox(NULL, msg, "�p�b�`�L�^�m�F�G���[", MB_OK);
	}
}

/**
	�w�肳�ꂽ�t�@�C������A�S�s���t���Ƀ��X�g��FTP���g���ēǂݍ��ށB
*/
void ftp_r()
{
	kjm::Internet internetSession;
	kjm::Internet ftpSession;

	if (internetSession.InetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0)) {

		if (ftpSession.InetConnect(internetSession, g_uc.m_szHostName.c_str(), g_uc.m_nPort, g_uc.m_szUserName.c_str(), g_uc.m_szPassword.c_str(), INTERNET_SERVICE_FTP, 0, 0)) {

			// ��M���郍�[�J���t�@�C�������쐬
			char tempPath[MAX_PATH];
			char tempFileName[MAX_PATH];

			GetTempPath(sizeof(tempPath), tempPath);
			GetTempFileName(tempPath, "pat", 0, tempFileName);

			ftpSession.InetFtpGetFile(g_uc.m_szUrlPath.c_str(), tempFileName, FALSE, FILE_ATTRIBUTE_TEMPORARY, FTP_TRANSFER_TYPE_ASCII, 0);

			file_r(tempFileName);

			DeleteFile(tempFileName);
		} else {
			char msg[512];
			wsprintf(msg, "InetConnect error : %d\n", ftpSession.get_lastError());
			MessageBox(NULL, msg, "�p�b�`�L�^�m�F�G���[", MB_OK);
		}
	} else {
		char msg[512];
		wsprintf(msg, "internetSession error : %d\n", ftpSession.get_lastError());
		MessageBox(NULL, msg, "�p�b�`�L�^�m�F�G���[", MB_OK);
	}
}


/**
	���X�g��\������_�C�A���O�̃v���V�[�W��
*/
BOOL CALLBACK View_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {
	case WM_INITDIALOG:
		{
			int i;
			list<string>::reverse_iterator it;

			for (i = 0, it = g_lines.rbegin(); i < 10 && it != g_lines.rend(); i++, it++) {
				ListBox_AddString(GetDlgItem(hwnd, IDC_LIST1), it->c_str());
			}
		}
		return FALSE;

	case WM_COMMAND:
		{
			WORD wID = LOWORD(wParam);

			switch (wID) {
			case IDOK:
				EndDialog(hwnd, 0);
				return TRUE;
			
			case IDCANCEL:
				EndDialog(hwnd, 0);
				return TRUE;
			}
		}
		break;
	}

	return FALSE;
}


/**
	�O���[�o���̕����񃊃X�g����A10�s�o��
*/
void v()
{
	DialogBox(g_ci.m_hInstance, MAKEINTRESOURCE(IDD_VIEW), NULL, View_DlgProc);
}


/**
	�v���O�����X�^�[�g
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	g_log.logsig();

	g_ci.m_hInstance = hInstance;
	g_ci.parse_command_line(__argc, __argv);

	g_log.log_f("-f : %s", g_ci.m_file.c_str());
	g_log.log_f("-m : %s", g_ci.m_message.c_str());

	if (g_ci.m_file.empty())	return -1;		// �t�@�C�������w�肳��Ă��Ȃ�

	if (!PathIsURL(g_ci.m_file.c_str())) {
		g_ci.m_file = string("file://") + g_ci.m_file;
	}
	g_ci.m_file = kjm::win32::ExpandEnvironmentStrings(g_ci.m_file.c_str());
	g_uc.CrackUrl(g_ci.m_file.c_str());

	if (g_uc.m_nScheme == INTERNET_SCHEME_FILE) {
		file_r(g_uc.m_szUrlPath.c_str());
		if (g_ci.m_view) {
			v();
		}
		if (!g_ci.m_message.empty()) {
			file_w(g_uc.m_szUrlPath.c_str());
		}
	} else if (g_uc.m_nScheme == INTERNET_SCHEME_FTP) {
		ftp_r();
		if (g_ci.m_view) {
			v();
		}
		if (!g_ci.m_message.empty()) {
			ftp_w();
		}
	}

	return 0;
}
