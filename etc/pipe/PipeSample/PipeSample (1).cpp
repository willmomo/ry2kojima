#include <windows.h>
#include <windowsx.h>
#include "resource.h"

// formatコマンドを実行する
void RunFormat(HWND hwnd, char* pszCommand)
{
	SECURITY_ATTRIBUTES sa = {0};
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0};
	HANDLE hPipeOutputRead  = NULL;
	HANDLE hPipeOutputWrite = NULL;
	HANDLE hPipeInputRead   = NULL;
	HANDLE hPipeInputWrite  = NULL;
	BOOL bTest = 0;
	DWORD dwNumberOfBytesRead = 0;
	CHAR szBuffer[256];

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	// 子プロセスのstdout/stdinをリダイレクトするためのパイプを作成
	CreatePipe(&hPipeOutputRead, &hPipeOutputWrite, &sa, 0);
	CreatePipe(&hPipeInputRead, &hPipeInputWrite, &sa, 0);

	// 子プロセスをstdinをhPiprInputReadに、stdout,stderrにhPipeOutputWriteに
	// 接続した状態で起動する
	si.cb = sizeof(si);
	si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdInput   = hPipeInputRead;
	si.hStdOutput  = hPipeOutputWrite;
	si.hStdError   = hPipeOutputWrite;
	CreateProcess(NULL, pszCommand, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

	// 子プロセスがハンドルを継承したので自分のハンドルを閉じる
	CloseHandle(hPipeOutputWrite);
	CloseHandle(hPipeInputRead);

	// format実行中に要求されるキー入力
	// <Enter><Enter>N<Enter>をパイプに書き込む
	DWORD dwNumberOfBytesWritten;
	WriteFile(hPipeInputWrite, "\x0d\x0a\x0d\x0aN\x0d\x0a", 7, &dwNumberOfBytesWritten, NULL);

	while (TRUE) {

		// Windows98の場合、子プロセスが終了してもReadFileがエラーを返さないため
		// パイプの未読データと子プロセスの状態を監視してループから抜ける
		// WindowsNT4.0の場合、ReadFileのエラーで抜ける
		DWORD dwExitCode, dwTotalBytesAvail;
		GetExitCodeProcess(pi.hProcess, &dwExitCode);
		PeekNamedPipe(hPipeOutputRead, NULL, 0, NULL, &dwTotalBytesAvail, NULL);
		if (dwTotalBytesAvail == 0 && dwExitCode != STILL_ACTIVE)
			break;

		// パイプから読み込み(子プロセスのstdout/stderrが読み込める)
		bTest = ReadFile(hPipeOutputRead, &szBuffer, 256, &dwNumberOfBytesRead, NULL);

		if (!bTest){
			CHAR szMsg[256];
			wsprintf(szMsg, "Error #%d reading pipe.", GetLastError());
			::MessageBox(hwnd, szMsg, pszCommand, MB_OK);
			break;
		}

		// 受信データをメッセージボックスで表示する
		szBuffer[dwNumberOfBytesRead] = 0;  // NULLをくっつけて文字列化する
		::MessageBox(hwnd, szBuffer, pszCommand, MB_OK);
	}

	// プロセスが終了するまで待つ。というか、すでに終了してるはず。
	WaitForSingleObject(pi.hProcess, INFINITE);

	// すべてのハンドルを閉じる
	CloseHandle (pi.hProcess);
	CloseHandle (hPipeOutputRead);
	CloseHandle (hPipeInputWrite);
}

// 指定コマンドを実行する
void Run(HWND hwnd, char* pszCommand)
{
	SECURITY_ATTRIBUTES sa = {0};
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0};
	HANDLE hPipeOutputRead  = NULL;
	HANDLE hPipeOutputWrite = NULL;
	HANDLE hPipeInputRead   = NULL;
	HANDLE hPipeInputWrite  = NULL;
	BOOL bTest = 0;
	DWORD dwNumberOfBytesRead = 0;
	CHAR szBuffer[256];

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	// 子プロセスのstdout/stdinをリダイレクトするためのパイプを作成
	CreatePipe(&hPipeOutputRead, &hPipeOutputWrite, &sa, 0);
	CreatePipe(&hPipeInputRead, &hPipeInputWrite, &sa, 0);

	// 子プロセスをstdinをhPiprInputReadに、stdout,stderrにhPipeOutputWriteに
	// 接続した状態で起動する
	si.cb = sizeof(si);
	si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdInput   = hPipeInputRead;
	si.hStdOutput  = hPipeOutputWrite;
	si.hStdError   = hPipeOutputWrite;
	CreateProcess(NULL, pszCommand, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

	// 子プロセスがハンドルを継承したので自分のハンドルを閉じる
	CloseHandle(hPipeOutputWrite);
	CloseHandle(hPipeInputRead);

	while (TRUE) {

		// Windows98の場合、子プロセスが終了してもReadFileがエラーを返さないため
		// パイプの未読データと子プロセスの状態を監視してループから抜ける
		// WindowsNT4.0の場合、ReadFileのエラーで抜ける
		DWORD dwExitCode, dwTotalBytesAvail;
		GetExitCodeProcess(pi.hProcess, &dwExitCode);
		PeekNamedPipe(hPipeOutputRead, NULL, 0, NULL, &dwTotalBytesAvail, NULL);
		if (dwTotalBytesAvail == 0 && dwExitCode != STILL_ACTIVE)
			break;

		// パイプから読み込み(子プロセスのstdout/stderrが読み込める)
		bTest = ReadFile(hPipeOutputRead, &szBuffer, 256, &dwNumberOfBytesRead, NULL);

		if (!bTest){
			CHAR szMsg[256];
			wsprintf(szMsg, "Error #%d reading pipe.", GetLastError());
			::MessageBox(hwnd, szMsg, pszCommand, MB_OK);
			break;
		}

		// 受信データをメッセージボックスで表示する
		szBuffer[dwNumberOfBytesRead] = 0;  // NULLをくっつけて文字列化する
		::MessageBox(hwnd, szBuffer, pszCommand, MB_OK);
	}

	// プロセスが終了するまで待つ。というか、すでに終了してるはず。
	WaitForSingleObject(pi.hProcess, INFINITE);

	// すべてのハンドルを閉じる
	CloseHandle (pi.hProcess);
	CloseHandle (hPipeOutputRead);
	CloseHandle (hPipeInputWrite);
}

// WM_COMMAND::IDC_RUN_EXEプロシージャ
// EXEファイルを実行して、入出力をパイプする
BOOL OnRunExe(HWND hwnd)
{
	EnableWindow(GetDlgItem(hwnd, IDC_RUN_EXE), FALSE);
	Run(hwnd, "XCOPY.EXE /?");
	EnableWindow(GetDlgItem(hwnd, IDC_RUN_EXE), TRUE);

	return TRUE;
}

// WM_COMMAND::IDC_RUN_COMプロシージャ
// COMファイルを実行して、入出力をパイプする
BOOL OnRunCom(HWND hwnd)
{
	EnableWindow(GetDlgItem(hwnd, IDC_RUN_COM), FALSE);
	Run(hwnd, "FORMAT.COM /?");
	EnableWindow(GetDlgItem(hwnd, IDC_RUN_COM), TRUE);

	return TRUE;
}

// WM_COMMAND::IDC_RUN_COM_ON_BASHプロシージャ
// COMファイルをbash上で実行して、入出力をパイプする
BOOL OnRunComOnBash(HWND hwnd)
{
	EnableWindow(GetDlgItem(hwnd, IDC_RUN_COM_ON_BASH), FALSE);
	Run(hwnd, "command.com /c FORMAT.COM /?");
	EnableWindow(GetDlgItem(hwnd, IDC_RUN_COM_ON_BASH), TRUE);

	return TRUE;
}

// WM_COMMAND::IDC_FORMATプロシージャ
// format.comを実行して、入出力をパイプする
BOOL OnFormat(HWND hwnd)
{
	EnableWindow(GetDlgItem(hwnd, IDC_FORMAT), FALSE);
	RunFormat(hwnd, "FORMAT.COM a:");
	EnableWindow(GetDlgItem(hwnd, IDC_FORMAT), TRUE);

	return TRUE;
}

// WM_COMMAND::IDC_FORMAT_ON_BASHプロシージャ
// format.comを実行して、入出力をパイプする
BOOL OnFormatOnBash(HWND hwnd)
{
	EnableWindow(GetDlgItem(hwnd, IDC_FORMAT_ON_BASH), FALSE);
	RunFormat(hwnd, "command.com /c FORMAT.COM a:");
	EnableWindow(GetDlgItem(hwnd, IDC_FORMAT_ON_BASH), TRUE);

	return TRUE;
}

// IDD_MAINダイアログ・プロシージャ
BOOL CALLBACK Main_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			EndDialog(hwnd, 0);
			return TRUE;
		case IDC_RUN_EXE:
			return OnRunExe(hwnd);
		case IDC_RUN_COM:
			return OnRunCom(hwnd);
		case IDC_RUN_COM_ON_BASH:
			return OnRunComOnBash(hwnd);
		case IDC_FORMAT:
			return OnFormat(hwnd);
		case IDC_FORMAT_ON_BASH:
			return OnFormatOnBash(hwnd);
		}
		break;
	}

	return FALSE;
}

// プログラム・スタート
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, (DLGPROC)Main_DlgProc);
}
