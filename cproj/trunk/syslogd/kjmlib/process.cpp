//---------------------------------------------------------------------
// class kjm::process �̎���
//---------------------------------------------------------------------
#include "kjmlib.h"

//---------------------------------------------------------------------
// �f�t�H���g �R���X�g���N�^
//---------------------------------------------------------------------
kjm::process::process() {
	memset(&m_si, 0, sizeof(m_si));
	memset(&m_pi, 0, sizeof(m_pi));

	m_si.cb = sizeof(m_si);
}

//---------------------------------------------------------------------
// �w�肵���v���Z�X�����s����
//---------------------------------------------------------------------
BOOL kjm::process::run(const _tstring& pCommandLine) {
	TCHAR temp[MAX_PATH];

	BOOL bRet = CreateProcess(
		NULL,
		lstrcpy(temp, pCommandLine.c_str()),
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&m_si,
		&m_pi
	);

	return bRet;
}

//---------------------------------------------------------------------
// �v���Z�X�n���h�������B
// �v���Z�X�̏I����҂̂́Await�֐��Ȃ̂Œ��ӁB
//---------------------------------------------------------------------
void kjm::process::close() {
	if (m_pi.hThread != NULL) {
		CloseHandle(m_pi.hThread);
	}

	if (m_pi.hProcess != NULL) {
		CloseHandle(m_pi.hProcess);
	}

	memset(&m_pi, 0, sizeof(m_pi));
}
