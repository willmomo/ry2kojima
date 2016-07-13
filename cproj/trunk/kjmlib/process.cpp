//
// �v���Z�X�Ɋւ���R�[�h
//
// process : Version 0.2015.12.23
//
// r.kojima
//
#include "process.h"
#include <assert.h>

//=====================================================================
//
//  process �N���X�̎���
//
//=====================================================================

//---------------------------------------------------------------------
// GetProcessWindow�֐��Ŏg�p����R�[���o�b�N
//
// [����]
// HWND hwnd		�񋓂��ꂽ�E�B���h�E�ւ̃n���h��
// LPARAM lParam	rkProcess�ւ̃|�C���^�B�L���X�g���Ďg��
//
// [���l]
// �񋓂��ꂽ�E�B���h�E�Ɠ����v���Z�XID�������̂��������ꍇ
// ���̃n���h���������o�[�ϐ��ɕۑ����A�񋓂��I������B
//---------------------------------------------------------------------
BOOL CALLBACK kjm::process::process_EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	kjm::process* p = reinterpret_cast<kjm::process*>(lParam);

	DWORD dwProcessId;
	GetWindowThreadProcessId(hwnd, &dwProcessId);
	if (dwProcessId == p->m_pi.dwProcessId) {
		p->m_hwnd = hwnd;
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------
// �f�t�H���g�E�R���X�g���N�^
//---------------------------------------------------------------------
kjm::process::process() {
	memset(&m_si, 0, sizeof(m_si));
	memset(&m_pi, 0, sizeof(m_pi));
	memset(&m_szCommandLine, 0, sizeof(m_szCommandLine));
}

// �X�^�[�g�A�b�v���(wShowWindow)��ݒ肷��
// ShowWindow API �̈����Ɠ���
//
void kjm::process::set_ShowWindow(WORD newShowWindow) {
	m_si.wShowWindow = newShowWindow;
	m_si.dwFlags |= STARTF_USESHOWWINDOW;
}

// �X�^�[�g�A�b�v���(wShowWindow)���N���A(���g�p��)����
//
void kjm::process::clear_ShowWindow() {
	m_si.wShowWindow = 0;
	m_si.dwFlags &= ~STARTF_USESHOWWINDOW;
}

//---------------------------------------------------------------------
// �v���Z�XID����v���Z�X���I�������郆�[�e�B���e�B�֐�
//---------------------------------------------------------------------
BOOL kjm::process::terminateProcess(DWORD dwProcessId, UINT uExitCode) {
	BOOL bRet = FALSE;
	kjm::process proc;
	if (proc.openProcess(PROCESS_TERMINATE, FALSE, dwProcessId)) {
		bRet = proc.kill(uExitCode);
	}
	return bRet;
}

// �v���Z�X�̎��s
//
BOOL kjm::process::createProcess(const kjm::_tstring& commandLine) {
	if (m_pi.hProcess)	return FALSE;	// ���łɉ������s���Ă���

	lstrcpy(m_szCommandLine, commandLine.c_str());
	m_si.cb = sizeof(m_si);
	BOOL ret = CreateProcess(NULL, m_szCommandLine, NULL, NULL, TRUE, 0, NULL, NULL, &m_si, &m_pi);
	return ret;
}

//---------------------------------------------------------------------
// �v���Z�X�n���h�������
//
// [����]
// �v���Z�X�n���h�������B�v���Z�X�̏I���ł͂Ȃ��̂Œ��ӁB
// �v���Z�X�̏I���́A�v���Z�X�Ԃ̎�茈�߂ɂ��B
//---------------------------------------------------------------------
void kjm::process::Close() {
	if (m_pi.hThread)	CloseHandle(m_pi.hThread);
	if (m_pi.hProcess)	CloseHandle(m_pi.hProcess);
	memset(&m_pi, 0, sizeof(m_pi));
}

//---------------------------------------------------------------------
// �v���Z�X�̏I���R�[�h���擾����
//---------------------------------------------------------------------
DWORD kjm::process::getExitCode() {
	DWORD exitCode = 0;
	::GetExitCodeProcess(this->m_pi.hProcess, &exitCode);
	return exitCode;
}

//---------------------------------------------------------------------
// �v���Z�X�Ɋ֘A�t����ꂽ�E�B���h�E��Ԃ�
//---------------------------------------------------------------------
HWND kjm::process::GetProcessWindow() {
	assert(m_pi.hProcess);

	if (!m_pi.hProcess)	return NULL;

	m_hwnd = NULL;
	EnumWindows(kjm::process::process_EnumWindowsProc, reinterpret_cast<LPARAM>(this));
	return m_hwnd;
}

// �ȈՃv���Z�X���s
//
// bWaitOnReturn == true: ����Ɏ��s�o������ 0�A���s������ -1
// bWaitOnReturn != true: ���s�����v���Z�X�̖߂�l
//
int kjm::process::run(const kjm::_tstring& strCommand, int intWindowStyle, bool bWaitOnReturn) {
	int result;

	kjm::process proc;
	proc.set_ShowWindow(intWindowStyle);
	BOOL ret = proc.createProcess(strCommand);
	if (bWaitOnReturn) {
		proc.waitForTermination();
		result = proc.getExitCode();
	} else {
		result = (ret ? 0 : -1);
	}
	return result;
}

//=====================================================================
//
//  Process �N���X�̎���
//
//=====================================================================

