//////////////////////////////////////////////////////////////////////
//	[�T�v]
//	�v���Z�X�Ǘ��N���X
//
//	[�쐬��]
//	Ryota Kojima
//
//	[�쐬��
//	2003/10/15

#if !defined(__RKPROCESS_H)
#define __RKPROCESS_H

//////////////////////////////////////////////////////////////////////
//	�w�b�_

#include <windows.h>
#include <windowsx.h>

//////////////////////////////////////////////////////////////////////
//	[�T�v]
//	�v���Z�X�Ǘ��N���X

class rkProcess
{
private:
	HWND m_hwnd;				// GetProcessWindow�֐��Ŏg�p
	static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

	STARTUPINFO m_si;					// �X�^�[�g�A�b�v���
	PROCESS_INFORMATION m_pi;			// �v���Z�X���
	TCHAR m_szCommandLine[MAX_PATH];	// ���s�R�}���h���C���L�^�p
										// Create�֐����s��L��

public:
	// �f�t�H���g �R���X�g���N�^
	rkProcess()
	{
		ZeroMemory(&m_si, sizeof(m_si));
		ZeroMemory(&m_pi, sizeof(m_pi));
		ZeroMemory(&m_szCommandLine, sizeof(m_szCommandLine));
	};
	// �f�X�g���N�^
	virtual ~rkProcess() { Close(); };

public:
	// �X�^�[�g�A�b�v�����N���A����
	VOID ClearSI() { ZeroMemory(&m_si, sizeof(m_si)); };
	// �X�^�[�g�A�b�v���(wShowWindow)���擾����
	WORD GetSIShowWindow() const { return m_si.wShowWindow; };
	// �X�^�[�g�A�b�v���(wShowWindow)��ݒ肷��
	VOID SetSIShowWindow(WORD newShowWindow)
	{
		m_si.wShowWindow = newShowWindow;
		m_si.dwFlags |= STARTF_USESHOWWINDOW;
	};
	// �X�^�[�g�A�b�v���(wShowWindow)���N���A(���g�p��)����
	VOID ClearSIShowWindow()
	{
		m_si.wShowWindow = 0;
		m_si.dwFlags &= ~STARTF_USESHOWWINDOW;
	};

	// �v���Z�XID�擾
	DWORD GetProcessId() const { return m_pi.dwProcessId; };
	// ���s�t�@�C���̃R�}���h���C���擾
	LPCTSTR GetCommandLine() const { return m_szCommandLine; };

public:
	// �v���Z�X�̎��s
	virtual BOOL Create(LPTSTR lpCommandLine)
	{
		if (m_pi.hProcess)	return FALSE;	// ���łɉ������s���Ă���
		lstrcpy(m_szCommandLine, lpCommandLine);
		m_si.cb = sizeof(m_si);
		return CreateProcess(NULL, lpCommandLine, NULL, NULL, TRUE, 0, NULL, NULL, &m_si, &m_pi);
	};
	// �v���Z�X�n���h�������B�v���Z�X�̏I���ł͂Ȃ��̂Œ��ӁB
	// �v���Z�X�̏I���́A�v���Z�X�Ԃ̎�茈�߂ɂ��B
	VOID Close()
	{
		if (m_pi.hThread)	CloseHandle(m_pi.hThread);
		if (m_pi.hProcess)	CloseHandle(m_pi.hProcess);
		ZeroMemory(&m_pi, sizeof(m_pi));
	};
	// �v���Z�X�̏I����҂�
	virtual DWORD WaitForTermination(DWORD dwMilliseconds = INFINITE)
	{
		return ::WaitForSingleObject(m_pi.hProcess, dwMilliseconds);
	};
	// �v���Z�X�̋N��������҂�
	virtual DWORD WaitForInputIdle(DWORD dwMilliseconds = INFINITE)
	{
		return ::WaitForInputIdle(m_pi.hProcess, dwMilliseconds);
	};
	// �v���Z�X�Ɋ֘A�t����ꂽ�E�B���h�E��Ԃ�
	HWND GetProcessWindow()
	{
		if (!m_pi.hProcess)	return NULL;
		m_hwnd = NULL;
		EnumWindows(rkProcess::EnumWindowsProc, reinterpret_cast<LPARAM>(this));
		return m_hwnd;
	};
};

#endif
