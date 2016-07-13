//---------------------------------------------------------------------
// TermProc.exe : �w�肵���v���Z�X�������I������A�v���P�[�V����
//---------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>
#include <tlhelp32.h>

#include "../kjmlib/kjmlib.h"
#if defined(_DEBUG)
#pragma comment(lib, "../kjmlib/kjmlibd.lib")
#else
#pragma comment(lib, "../kjmlib/kjmlib.lib")
#endif


//---------------------------------------------------------------------
// �O���[�o���ϐ�
//---------------------------------------------------------------------
kjm::log g_log;			// �A�v���P�[�V�����S�̂̃��O
TCHAR g_name[MAX_PATH];	// -n�Ŏw�肳�ꂽ���s�t�@�C����


//---------------------------------------------------------------------
// �w�肵�����s�t�@�C���������A�v���P�[�V�����������I������
//
// [����]
// lpszExeName	�����I������v���Z�X�̎��s�t�@�C����(�C���[�W��)
//
// [�߂�l]
// �폜�����v���Z�X�̐��B
// -1 : �X�i�b�v�V���b�g�̍쐬�Ɏ��s
//---------------------------------------------------------------------
int killProcesses(LPCTSTR lpszExeName)
{
	g_log.writeLog(_T("killProcesses('%s') enter."), lpszExeName);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		g_log.writeLog(_T("killProcesses: CreateToolhelp32Snapshot() error: %u."), GetLastError());
		return -1;
	}

	PROCESSENTRY32 pe;
	ZeroMemory(&pe, sizeof(pe));
	pe.dwSize = sizeof(pe);

	int nKilled = 0;
	BOOL ret = Process32First(hSnap, &pe);
	while (ret) {
		if (lstrcmpi(lpszExeName, pe.szExeFile) == 0) {
			kjm::process::terminateProcess(pe.th32ProcessID, 0, &g_log);
		}
		ret = Process32Next(hSnap, &pe);
	}

	if (CloseHandle(hSnap) == FALSE) {
		g_log.writeLog(_T("killProcesses: CloseHandle(hSnap = 0x%p) error: %u."), hSnap, GetLastError());
	}

	g_log.writeLog(_T("killProcesses() leave with %d."), nKilled);
	return nKilled;
}


//---------------------------------------------------------------------
// �v���O�����E�X�^�[�g
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	g_log.writeLog("---- '%s' start. ----", kjm::win32::getModuleFileName().c_str());

	option options[] = {
		{_T("name"), required_argument, NULL, 'n'},
		{NULL, no_argument, NULL, 0}
	};

	int c;
	int opt_index;
	while ((c = getopt_long(__argc, __argv, _T("n:"), options, &opt_index)) != -1) {
		switch (c) {
		case 'n':
			lstrcpy(g_name, g_optarg);
			break;
		}
	}

	// -n�I�v�V�����Ŏw�肳�ꂽ���s�t�@�C���̃v���Z�X�������I������B
	if (g_name[0] != '\0') {
		killProcesses(g_name);
	}

	g_log.writeLog("---- '%s' end. ----", kjm::win32::getModuleFileName().c_str());
	return 0;
}
