//////////////////////////////////////////////////////////////////////
//	[�T�v]
//	�v���Z�X�Ǘ��N���X
//
//	[�쐬��]
//	Ryota Kojima
//
//	[�쐬��
//	2003/10/15

#include "rkProcess.h"

//////////////////////////////////////////////////////////////////////
//	[�T�v]
//	GetProcessWindow�֐��Ŏg�p����R�[���o�b�N
//
//	[����]
//	HWND hwnd		�񋓂��ꂽ�E�B���h�E�ւ̃n���h��
//	LPARAM lParam	rkProcess�ւ̃|�C���^�B�L���X�g���Ďg��
//
//	[���l]
//	�񋓂��ꂽ�E�B���h�E�Ɠ����v���Z�XID�������̂��������ꍇ
//	���̃n���h���������o�[�ϐ��ɕۑ����A�񋓂��I������B

BOOL CALLBACK rkProcess::EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	rkProcess* p = reinterpret_cast<rkProcess*>(lParam);

	DWORD dwProcessId;
	GetWindowThreadProcessId(hwnd, &dwProcessId);
	if (dwProcessId == p->m_pi.dwProcessId) {
		p->m_hwnd = hwnd;
		return FALSE;
	}

	return TRUE;
}
