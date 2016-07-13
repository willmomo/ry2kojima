/**
	@file
	@brief	パッチのバッチをサポートするメインルーチン
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
	/// コンストラクタ
	cmdinfo() : m_file(), m_message(), m_view(false) {
	};

	/**
		このプログラム固有のパラメータ解析
		@note
		以下のパラメータを分析する@n
		-f @<ファイル名@>@n
		-m @<メッセージ@>
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
	string m_file;		///< -f で指定されたファイル名
	string m_message;	///< -m で指定されたメッセージ
	bool m_view;		///< -v が指定されているかどうか

	HINSTANCE m_hInstance;	///< WinMainの引数
};

cmdinfo g_ci;


/**
	指定されたファイルに、指定されたメッセージを追記する。
*/
void file_w(const char* fname)
{
	kjm::file f;
	if (f.fopen(fname, "a+")) {
		f.fprintf("[%s] %s\n", kjm::time::now().strftime("%Y/%m/%d %H:%M:%S").c_str(), g_ci.m_message.c_str());
	} else {
		g_log.log_f("file_w(): error %d: ファイル '%s' が、オープンできません。", errno, fname);
	}
}


/**
	指定されたファイルから、全行を逆順にリストに読み込む。
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
		g_log.log_f("file_r(): error %d: ファイル '%s' が、オープンできません。", errno, fname);
	}
}

/**
	指定されたファイルに、指定されたメッセージをFTPを使って追記する。
*/
void ftp_w()
{
	kjm::Internet internetSession;
	kjm::Internet ftpSession;

	if (internetSession.InetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0)) {

		if (ftpSession.InetConnect(internetSession, g_uc.m_szHostName.c_str(), g_uc.m_nPort, g_uc.m_szUserName.c_str(), g_uc.m_szPassword.c_str(), INTERNET_SERVICE_FTP, 0, 0)) {

			// 受信するローカルファイル名を作成
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
			MessageBox(NULL, msg, "パッチ記録確認エラー", MB_OK);
		}
	} else {
		char msg[512];
		wsprintf(msg, "internetSession error : %d\n", ftpSession.get_lastError());
		MessageBox(NULL, msg, "パッチ記録確認エラー", MB_OK);
	}
}

/**
	指定されたファイルから、全行を逆順にリストにFTPを使って読み込む。
*/
void ftp_r()
{
	kjm::Internet internetSession;
	kjm::Internet ftpSession;

	if (internetSession.InetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0)) {

		if (ftpSession.InetConnect(internetSession, g_uc.m_szHostName.c_str(), g_uc.m_nPort, g_uc.m_szUserName.c_str(), g_uc.m_szPassword.c_str(), INTERNET_SERVICE_FTP, 0, 0)) {

			// 受信するローカルファイル名を作成
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
			MessageBox(NULL, msg, "パッチ記録確認エラー", MB_OK);
		}
	} else {
		char msg[512];
		wsprintf(msg, "internetSession error : %d\n", ftpSession.get_lastError());
		MessageBox(NULL, msg, "パッチ記録確認エラー", MB_OK);
	}
}


/**
	リストを表示するダイアログのプロシージャ
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
	グローバルの文字列リストから、10行出力
*/
void v()
{
	DialogBox(g_ci.m_hInstance, MAKEINTRESOURCE(IDD_VIEW), NULL, View_DlgProc);
}


/**
	プログラムスタート
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	g_log.logsig();

	g_ci.m_hInstance = hInstance;
	g_ci.parse_command_line(__argc, __argv);

	g_log.log_f("-f : %s", g_ci.m_file.c_str());
	g_log.log_f("-m : %s", g_ci.m_message.c_str());

	if (g_ci.m_file.empty())	return -1;		// ファイル名が指定されていない

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
