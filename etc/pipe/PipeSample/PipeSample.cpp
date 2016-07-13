#include <windows.h>
#include <windowsx.h>
#include "resource.h"

// format�R�}���h�����s����
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

	// �q�v���Z�X��stdout/stdin�����_�C���N�g���邽�߂̃p�C�v���쐬
	CreatePipe(&hPipeOutputRead, &hPipeOutputWrite, &sa, 0);
	CreatePipe(&hPipeInputRead, &hPipeInputWrite, &sa, 0);

	// �q�v���Z�X��stdin��hPiprInputRead�ɁAstdout,stderr��hPipeOutputWrite��
	// �ڑ�������ԂŋN������
	si.cb = sizeof(si);
	si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdInput   = hPipeInputRead;
	si.hStdOutput  = hPipeOutputWrite;
	si.hStdError   = hPipeOutputWrite;
	CreateProcess(NULL, pszCommand, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

	// �q�v���Z�X���n���h�����p�������̂Ŏ����̃n���h�������
	CloseHandle(hPipeOutputWrite);
	CloseHandle(hPipeInputRead);

	// format���s���ɗv�������L�[����
	// <Enter><Enter>N<Enter>���p�C�v�ɏ�������
	DWORD dwNumberOfBytesWritten;
	WriteFile(hPipeInputWrite, "\x0d\x0a\x0d\x0aN\x0d\x0a", 7, &dwNumberOfBytesWritten, NULL);

	while (TRUE) {

		// Windows98�̏ꍇ�A�q�v���Z�X���I�����Ă�ReadFile���G���[��Ԃ��Ȃ�����
		// �p�C�v�̖��ǃf�[�^�Ǝq�v���Z�X�̏�Ԃ��Ď����ă��[�v���甲����
		// WindowsNT4.0�̏ꍇ�AReadFile�̃G���[�Ŕ�����
		DWORD dwExitCode, dwTotalBytesAvail;
		GetExitCodeProcess(pi.hProcess, &dwExitCode);
		PeekNamedPipe(hPipeOutputRead, NULL, 0, NULL, &dwTotalBytesAvail, NULL);
		if (dwTotalBytesAvail == 0 && dwExitCode != STILL_ACTIVE)
			break;

		// �p�C�v����ǂݍ���(�q�v���Z�X��stdout/stderr���ǂݍ��߂�)
		bTest = ReadFile(hPipeOutputRead, &szBuffer, 256, &dwNumberOfBytesRead, NULL);

		if (!bTest){
			CHAR szMsg[256];
			wsprintf(szMsg, "Error #%d reading pipe.", GetLastError());
			::MessageBox(hwnd, szMsg, pszCommand, MB_OK);
			break;
		}

		// ��M�f�[�^�����b�Z�[�W�{�b�N�X�ŕ\������
		szBuffer[dwNumberOfBytesRead] = 0;  // NULL���������ĕ����񉻂���
		::MessageBox(hwnd, szBuffer, pszCommand, MB_OK);
	}

	// �v���Z�X���I������܂ő҂B�Ƃ������A���łɏI�����Ă�͂��B
	WaitForSingleObject(pi.hProcess, INFINITE);

	// ���ׂẴn���h�������
	CloseHandle (pi.hProcess);
	CloseHandle (hPipeOutputRead);
	CloseHandle (hPipeInputWrite);
}

// �w��R�}���h�����s����
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

	// �q�v���Z�X��stdout/stdin�����_�C���N�g���邽�߂̃p�C�v���쐬
	CreatePipe(&hPipeOutputRead, &hPipeOutputWrite, &sa, 0);
	CreatePipe(&hPipeInputRead, &hPipeInputWrite, &sa, 0);

	// �q�v���Z�X��stdin��hPiprInputRead�ɁAstdout,stderr��hPipeOutputWrite��
	// �ڑ�������ԂŋN������
	si.cb = sizeof(si);
	si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdInput   = hPipeInputRead;
	si.hStdOutput  = hPipeOutputWrite;
	si.hStdError   = hPipeOutputWrite;
	CreateProcess(NULL, pszCommand, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

	// �q�v���Z�X���n���h�����p�������̂Ŏ����̃n���h�������
	CloseHandle(hPipeOutputWrite);
	CloseHandle(hPipeInputRead);

	while (TRUE) {

		// Windows98�̏ꍇ�A�q�v���Z�X���I�����Ă�ReadFile���G���[��Ԃ��Ȃ�����
		// �p�C�v�̖��ǃf�[�^�Ǝq�v���Z�X�̏�Ԃ��Ď����ă��[�v���甲����
		// WindowsNT4.0�̏ꍇ�AReadFile�̃G���[�Ŕ�����
		DWORD dwExitCode, dwTotalBytesAvail;
		GetExitCodeProcess(pi.hProcess, &dwExitCode);
		PeekNamedPipe(hPipeOutputRead, NULL, 0, NULL, &dwTotalBytesAvail, NULL);
		if (dwTotalBytesAvail == 0 && dwExitCode != STILL_ACTIVE)
			break;

		// �p�C�v����ǂݍ���(�q�v���Z�X��stdout/stderr���ǂݍ��߂�)
		bTest = ReadFile(hPipeOutputRead, &szBuffer, 256, &dwNumberOfBytesRead, NULL);

		if (!bTest){
			CHAR szMsg[256];
			wsprintf(szMsg, "Error #%d reading pipe.", GetLastError());
			::MessageBox(hwnd, szMsg, pszCommand, MB_OK);
			break;
		}

		// ��M�f�[�^�����b�Z�[�W�{�b�N�X�ŕ\������
		szBuffer[dwNumberOfBytesRead] = 0;  // NULL���������ĕ����񉻂���
		::MessageBox(hwnd, szBuffer, pszCommand, MB_OK);
	}

	// �v���Z�X���I������܂ő҂B�Ƃ������A���łɏI�����Ă�͂��B
	WaitForSingleObject(pi.hProcess, INFINITE);

	// ���ׂẴn���h�������
	CloseHandle (pi.hProcess);
	CloseHandle (hPipeOutputRead);
	CloseHandle (hPipeInputWrite);
}

// WM_COMMAND::IDC_RUN_EXE�v���V�[�W��
// EXE�t�@�C�������s���āA���o�͂��p�C�v����
BOOL OnRunExe(HWND hwnd)
{
	EnableWindow(GetDlgItem(hwnd, IDC_RUN_EXE), FALSE);
	Run(hwnd, "XCOPY.EXE /?");
	EnableWindow(GetDlgItem(hwnd, IDC_RUN_EXE), TRUE);

	return TRUE;
}

// WM_COMMAND::IDC_RUN_COM�v���V�[�W��
// COM�t�@�C�������s���āA���o�͂��p�C�v����
BOOL OnRunCom(HWND hwnd)
{
	EnableWindow(GetDlgItem(hwnd, IDC_RUN_COM), FALSE);
	Run(hwnd, "FORMAT.COM /?");
	EnableWindow(GetDlgItem(hwnd, IDC_RUN_COM), TRUE);

	return TRUE;
}

// WM_COMMAND::IDC_RUN_COM_ON_BASH�v���V�[�W��
// COM�t�@�C����bash��Ŏ��s���āA���o�͂��p�C�v����
BOOL OnRunComOnBash(HWND hwnd)
{
	EnableWindow(GetDlgItem(hwnd, IDC_RUN_COM_ON_BASH), FALSE);
	Run(hwnd, "command.com /c FORMAT.COM /?");
	EnableWindow(GetDlgItem(hwnd, IDC_RUN_COM_ON_BASH), TRUE);

	return TRUE;
}

// WM_COMMAND::IDC_FORMAT�v���V�[�W��
// format.com�����s���āA���o�͂��p�C�v����
BOOL OnFormat(HWND hwnd)
{
	EnableWindow(GetDlgItem(hwnd, IDC_FORMAT), FALSE);
	RunFormat(hwnd, "FORMAT.COM a:");
	EnableWindow(GetDlgItem(hwnd, IDC_FORMAT), TRUE);

	return TRUE;
}

// WM_COMMAND::IDC_FORMAT_ON_BASH�v���V�[�W��
// format.com�����s���āA���o�͂��p�C�v����
BOOL OnFormatOnBash(HWND hwnd)
{
	EnableWindow(GetDlgItem(hwnd, IDC_FORMAT_ON_BASH), FALSE);
	RunFormat(hwnd, "command.com /c FORMAT.COM a:");
	EnableWindow(GetDlgItem(hwnd, IDC_FORMAT_ON_BASH), TRUE);

	return TRUE;
}

// IDD_MAIN�_�C�A���O�E�v���V�[�W��
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

// �v���O�����E�X�^�[�g
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, (DLGPROC)Main_DlgProc);
}
