//---------------------------------------------------------------------
// KillProc.exe : �w�肵���v���Z�X�������I������A�v���P�[�V����
//---------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>
#include <tlhelp32.h>

#include "kjmlib.h"

//---------------------------------------------------------------------
// �O���[�o���ϐ�
//---------------------------------------------------------------------
kjm::log g_log;			// �A�v���P�[�V�����S�̂̃��O
TCHAR g_name[MAX_PATH];		// -n�Ŏw�肳�ꂽ���s�t�@�C����


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
int killProcesses(const _tstring& lpszExeName) {
	g_log.write(_T("killProcesses('%s') enter."), lpszExeName.c_str());

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		g_log.write(_T("killProcesses: CreateToolhelp32Snapshot() error: %u."), GetLastError());
		return -1;
	}

	PROCESSENTRY32 pe;
	ZeroMemory(&pe, sizeof(pe));
	pe.dwSize = sizeof(pe);

	int nKilled = 0;
	BOOL ret = Process32First(hSnap, &pe);
	while (ret) {
		if (lstrcmpi(lpszExeName.c_str(), pe.szExeFile) == 0) {
			kjm::process::terminateProcess(pe.th32ProcessID, 0);
		}
		ret = Process32Next(hSnap, &pe);
	}

	if (CloseHandle(hSnap) == FALSE) {
		g_log.write(_T("killProcesses: CloseHandle(hSnap = 0x%p) error: %u."), hSnap, GetLastError());
	}

	g_log.write(_T("killProcesses() leave with %d."), nKilled);
	return nKilled;
}


//---------------------------------------------------------------------
// �v���O�����E�X�^�[�g
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	g_log.write("---- '%s' start. ----", kjm::util::getModuleFileName().c_str());

	_tstring imageName;
	kjm::commandLine cl;
	cl.parse(__argc, __targv, _T("?vn"));
	std::vector<_tstring> opts = cl.getOptList();

	for (std::vector<_tstring>::size_type i = 0; i < opts.size(); i++) {
		if (opts[i] == _T("name") || opts[i] == _T("n")) {
			imageName = cl.getOptArg(opts[i]);
		}
	}

	if (imageName.empty()) {
		MessageBox(NULL, _T("usage: KillProc --name=<image name>"), _T("usage: KillProc"), MB_OK | MB_ICONINFORMATION);
		return 0;
	}

	// -n�I�v�V�����Ŏw�肳�ꂽ���s�t�@�C���̃v���Z�X�������I������B
	if (!imageName.empty()) {
		killProcesses(imageName);
	}

	g_log.write("---- '%s' end. ----", kjm::util::getModuleFileName().c_str());
	return 0;
}
