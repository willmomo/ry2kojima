//---------------------------------------------------------------------
// �v���Z�X�������N���X
//
// 2009/01/24 ����  �V�K�쐬
//---------------------------------------------------------------------
#include "kjmProcess.h"
#include <vector>

//---------------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//---------------------------------------------------------------------
kjm::Process::Process() {
	ZeroMemory(&m_pi, sizeof(m_pi));
	ZeroMemory(&m_si, sizeof(m_si));
	m_si.cb = sizeof(m_si);
}

//---------------------------------------------------------------------
// �v���Z�X�����쒆���ǂ������ׂ�
//---------------------------------------------------------------------
bool kjm::Process::IsStillActive() {
	DWORD dwExitCode;

	::GetExitCodeProcess(m_pi.hProcess, &dwExitCode);

	return (dwExitCode == STILL_ACTIVE);
}

//---------------------------------------------------------------------
// �v���Z�X�����s����
//
// [�߂�l]
// true ����I���Bfalse �G���[�I���B
//---------------------------------------------------------------------
bool kjm::Process::Run(LPCTSTR cmdLine) {
	// ���łɕʂ̃v���Z�X���s���̎��́A�G���[��Ԃ��B
	if (m_pi.hProcess != NULL) {
		return false;
	}

	// �v���Z�X���s
	std::vector<TCHAR> work(MAX_PATH + lstrlen(cmdLine) + 1);
	lstrcpy(work.begin(), cmdLine);
	return (::CreateProcess(NULL, work.begin(), NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &m_si, &m_pi) != FALSE);
}

//---------------------------------------------------------------------
// �v���Z�X�̃n���h�������
//
// [���l]
// �v���Z�X�����s���Ă��Ȃ��Ă��A����ɏ����ł��܂��B
// �f�X�g���N�^�ŌĂ�ł���̂ŁA���L���Ȃ��Ă��\���܂���B
//---------------------------------------------------------------------
void kjm::Process::CloseProcessHandle() {
	// �X���b�h�̃n���h�������
	if (m_pi.hThread) {
		::CloseHandle(m_pi.hThread);
		m_pi.hThread = NULL;
	}

	// �v���Z�X�̃n���h�������
	if (m_pi.hProcess) {
		::CloseHandle(m_pi.hProcess);
		m_pi.hProcess = NULL;
	}
}
