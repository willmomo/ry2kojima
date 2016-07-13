#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

// format�R�}���h�����s����
void RunFormat(char* pszCommand)
{
	SECURITY_ATTRIBUTES sa          = {0};
	STARTUPINFO         si          = {0};
	PROCESS_INFORMATION pi          = {0};
	HANDLE              hPipeOutputRead  = NULL;
	HANDLE              hPipeOutputWrite = NULL;
	HANDLE              hPipeInputRead   = NULL;
	HANDLE              hPipeInputWrite  = NULL;
	BOOL                bTest = 0;
	DWORD               dwNumberOfBytesRead = 0;
	CHAR                szBuffer[256];

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	// Create pipe for standard output redirection.
	CreatePipe(&hPipeOutputRead,  // read handle
		&hPipeOutputWrite, // write handle
		&sa,      // security attributes
		0      // number of bytes reserved for pipe - 0 default
		);

	// Create pipe for standard input redirection.
	CreatePipe(&hPipeInputRead,  // read handle
		&hPipeInputWrite, // write handle
		&sa,      // security attributes
		0      // number of bytes reserved for pipe - 0 default
		);

	// Make child process use hPipeOutputWrite as standard out,
	// and make sure it does not show on screen.
	si.cb = sizeof(si);
	si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdInput   = hPipeInputRead;
	si.hStdOutput  = hPipeOutputWrite;
	si.hStdError   = hPipeOutputWrite;

	CreateProcess (
		NULL, pszCommand,
		NULL, NULL,
		TRUE, 0,
		NULL, NULL,
		&si, &pi);

	// Now that handles have been inherited, close it to be safe.
	// You don't want to read or write to them accidentally.
	CloseHandle(hPipeOutputWrite);
	CloseHandle(hPipeInputRead);

	// Now test to capture DOS application output by reading
	// hPipeOutputRead.  Could also write to DOS application
	// standard input by writing to hPipeInputWrite.

	WriteFile(
		hPipeInputWrite,
		"\x0d\x0a\x0d\x0aN\x0d\x0a",
		7,
		&dwNumberOfBytesRead,
		NULL
		);

	while (TRUE) {
		bTest = ReadFile(
			hPipeOutputRead,      // handle of the read end of our pipe
			&szBuffer,            // address of buffer that receives data
			256,                  // number of bytes to read
			&dwNumberOfBytesRead, // address of number of bytes read
			NULL                  // non-overlapped.
			);

		if (!bTest){
			CHAR szMsg[256];
			wsprintf(szMsg, "Error #%d reading pipe.", GetLastError());
			::MessageBox(NULL, szMsg, pszCommand, MB_OK);
			break;
		}

		// do something with data.
		szBuffer[dwNumberOfBytesRead] = 0;  // null terminate
		::MessageBox(NULL, szBuffer, pszCommand, MB_OK);
	}

	// Wait for CONSPAWN to finish.
	WaitForSingleObject (pi.hProcess, INFINITE);

	// Close all remaining handles
	CloseHandle (pi.hProcess);
	CloseHandle (hPipeOutputRead);
	CloseHandle (hPipeInputWrite);
}

// �w��R�}���h�����s����
void Run(char* pszCommand)
{
	SECURITY_ATTRIBUTES sa          = {0};
	STARTUPINFO         si          = {0};
	PROCESS_INFORMATION pi          = {0};
	HANDLE              hPipeOutputRead  = NULL;
	HANDLE              hPipeOutputWrite = NULL;
	HANDLE              hPipeInputRead   = NULL;
	HANDLE              hPipeInputWrite  = NULL;
	BOOL                bTest = 0;
	DWORD               dwNumberOfBytesRead = 0;
	CHAR                szBuffer[256];

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	// Create pipe for standard output redirection.
	CreatePipe(&hPipeOutputRead,  // read handle
		&hPipeOutputWrite, // write handle
		&sa,      // security attributes
		0      // number of bytes reserved for pipe - 0 default
		);

	// Create pipe for standard input redirection.
	CreatePipe(&hPipeInputRead,  // read handle
		&hPipeInputWrite, // write handle
		&sa,      // security attributes
		0      // number of bytes reserved for pipe - 0 default
		);

	// Make child process use hPipeOutputWrite as standard out,
	// and make sure it does not show on screen.
	si.cb = sizeof(si);
	si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdInput   = hPipeInputRead;
	si.hStdOutput  = hPipeOutputWrite;
	si.hStdError   = hPipeOutputWrite;

	CreateProcess (
		NULL, pszCommand,
		NULL, NULL,
		TRUE, 0,
		NULL, NULL,
		&si, &pi);

	// Now that handles have been inherited, close it to be safe.
	// You don't want to read or write to them accidentally.
	CloseHandle(hPipeOutputWrite);
	CloseHandle(hPipeInputRead);

	// Now test to capture DOS application output by reading
	// hPipeOutputRead.  Could also write to DOS application
	// standard input by writing to hPipeInputWrite.

	while (TRUE) {
		bTest = ReadFile(
			hPipeOutputRead,      // handle of the read end of our pipe
			&szBuffer,            // address of buffer that receives data
			256,                  // number of bytes to read
			&dwNumberOfBytesRead, // address of number of bytes read
			NULL                  // non-overlapped.
			);

		if (!bTest){
			char szMsg[256];
			wsprintf(szMsg, "Error #%d reading pipe.", GetLastError());
			::MessageBox(NULL, szMsg, pszCommand, MB_OK);
			break;
		}

		// do something with data.
		szBuffer[dwNumberOfBytesRead] = 0;  // null terminate
		::MessageBox(NULL, szBuffer, pszCommand, MB_OK);
	}

	// Wait for CONSPAWN to finish.
	WaitForSingleObject (pi.hProcess, INFINITE);

	// Close all remaining handles
	CloseHandle (pi.hProcess);
	CloseHandle (hPipeOutputRead);
	CloseHandle (hPipeInputWrite);
}

// EXE�t�@�C�������s���āA���o�͂��p�C�v����
void OnRunExe()
{
	Run("XCOPY.EXE /?");
}

// COM�t�@�C�������s���āA���o�͂��p�C�v����
void OnRunCom()
{
	Run("FORMAT.COM /?");
}

// COM�t�@�C����bash��Ŏ��s���āA���o�͂��p�C�v����
void OnRunComOnBash()
{
	Run("command.com /c FORMAT.COM /?");
}

// format.com�����s���āA���o�͂��p�C�v����
void OnFormat()
{
	RunFormat("FORMAT.COM a:");
}

// format.com�����s���āA���o�͂��p�C�v����
void OnFormatOnBash()
{
	RunFormat("command.com /c FORMAT.COM a:");
}

// �v���O���� �X�^�[�g
int main(int argc, char** argv)
{
	BOOL bContinue = TRUE;
	int no;

	do {
		printf("1: EXE�t�@�C���̎��s (xcopy.exe /?)\n");
		printf("2: COM�t�@�C���̎��s (format.com /?)\n");
		printf("3: COM�t�@�C����command.com��Ŏ��s (command.com /c format.com /?)\n");
		printf("4: format.com a:\n");
		printf("5: command.com /c format.com a:\n");
		printf("0: -- �I�� --\n");
		printf("> ");

		scanf("%d", &no);
		
		switch (no) {
		case 0:
			bContinue = FALSE;
			break;
		case 1:
			OnRunExe();
			break;
		case 2:
			OnRunCom();
			break;
		case 3:
			OnRunComOnBash();
			break;
		case 4:
			OnFormat();
			break;
		case 5:
			OnFormatOnBash();
			break;
		}
	} while (bContinue);

	return 0;
}
