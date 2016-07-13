//
// �v���Z�X���s�Ɋւ���R�[�h
//
// process : Version 0.2015.12.23
//
// r.kojima
//
#pragma once

#include "kjmbase.h"

namespace kjm {
	//
	//
	//
	class process {
	public:
		process();
		virtual ~process() { this->Close(); }

	public:
		// �X�^�[�g�A�b�v�����N���A����
		void ClearSI() { memset(&m_si, 0, sizeof(m_si)); }

		// �X�^�[�g�A�b�v���(wShowWindow)���擾����
		WORD get_ShowWindow() const { return m_si.wShowWindow; }

		// �X�^�[�g�A�b�v���(wShowWindow)��ݒ肷��
		// ShowWindow API �̈����Ɠ���
		// SW_SHOWDEFAULT, SW_HIDE �Ȃ�
		void set_ShowWindow(WORD newShowWindow);

		// �X�^�[�g�A�b�v���(wShowWindow)���N���A(���g�p��)����
		void clear_ShowWindow();

		// �v���Z�XID�擾
		DWORD GetProcessId() const { return m_pi.dwProcessId; }

		// ���s�t�@�C���̃R�}���h���C���擾
		kjm::_tstring GetCommandLine() const { return m_szCommandLine; }

		// �v���Z�X�̏I���R�[�h���擾
		DWORD getExitCode();

		// �v���Z�X�n���h�����L�����ǂ������ׂ�
		bool IsHandleAvailable() { return (m_pi.hThread != NULL && m_pi.hProcess != NULL) ? true : false; }

		// �v���Z�X�����쒆���ǂ������ׂ�
		bool IsStillActive() { return (IsHandleAvailable() && (getExitCode() == STILL_ACTIVE)) ? true : false; }

	public:
		// �v���Z�X�̎��s
		virtual BOOL createProcess(const kjm::_tstring& commandLine);

		// OpenProcess API�̃��b�p�[
		//
		// [����]
		// dwDesiredAccess  �A�N�Z�X�t���O
		// bInheritHandle   �p���t���O
		// dwProcessId      �v���Z�XID
		//
		// [�߂�l]
		// �֐�����������ƁAtrue ���Ԃ�B
		// �֐������s����ƁAfalse ���Ԃ�B
		bool openProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId) {
			memset(&m_pi, 0, sizeof(m_pi));

			m_pi.dwProcessId = dwProcessId;
			m_pi.hProcess = ::OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);

			return m_pi.hProcess != NULL;
		};

		// �v���Z�X�n���h�������B�v���Z�X�̏I���ł͂Ȃ��̂Œ��ӁB
		// �v���Z�X�̏I���́A�v���Z�X�Ԃ̎�茈�߂ɂ��B
		void Close();

		// �v���Z�X�̏I����҂�
		virtual DWORD waitForTermination(DWORD dwMilliseconds = INFINITE) { return ::WaitForSingleObject(m_pi.hProcess, dwMilliseconds); }

		// �v���Z�X�̋N��������҂�
		virtual DWORD waitForInputIdle(DWORD dwMilliseconds = INFINITE) { return ::WaitForInputIdle(m_pi.hProcess, dwMilliseconds); }

		// �v���Z�X�Ɋ֘A�t����ꂽ�E�B���h�E��Ԃ�
		HWND GetProcessWindow();

		// �v���Z�X�������I��
		virtual BOOL kill(UINT uExitCode) {
			return ::TerminateProcess(m_pi.hProcess, uExitCode);
		}

	public:
		// �v���Z�XID����v���Z�X���I�������郆�[�e�B���e�B�֐�
		//
		// [����]
		// dwProcessId  �v���Z�XID
		// uExitCode    exit code for the process
		// pLog         ���O�I�u�W�F�N�g�ւ̃|�C���^�B!NULL �̂Ƃ��A������g�p���ă��O���c���B
		//
		// [�߂�l]
		// �֐�����������ƁATRUE ���Ԃ�B
		// �֐������s����ƁAFALSE ���Ԃ�B
		static BOOL terminateProcess(DWORD dwProcessId, UINT uExitCode);

		// �ȈՃv���Z�X���s
		static int run(const kjm::_tstring& strCommand, int intWindowStyle=SW_SHOWNORMAL, bool bWaitOnReturn=false);

	private:
		HWND m_hwnd;				// GetProcessWindow�֐��Ŏg�p

		STARTUPINFO m_si;					// �X�^�[�g�A�b�v���
		PROCESS_INFORMATION m_pi;			// �v���Z�X���
		TCHAR m_szCommandLine[MAX_PATH];	// ���s�R�}���h���C���L�^�p
											// Create�֐����s��L��
		
		static BOOL CALLBACK process_EnumWindowsProc(HWND hwnd, LPARAM lParam);
	};

	//
	//
	//
	class Process {
	public:
		//-------------------------------------------------------------
		// �f�t�H���g �R���X�g���N�^
		//-------------------------------------------------------------
		Process() {
			memset(&m_startupInfo, 0, sizeof(m_startupInfo));
			memset(&m_processInformation, 0, sizeof(m_processInformation));
		};

		//-------------------------------------------------------------
		// �f�X�g���N�^
		//-------------------------------------------------------------
		virtual ~Process() { Close(); };

		//-------------------------------------------------------------
		// �v���Z�X����
		//-------------------------------------------------------------
		BOOL Create(const kjm::_tstring& commandLine) {
			m_startupInfo.cb = sizeof(m_startupInfo);

			// CreateProcess �̓�Ԗڂ̈������ALPTSTR �̂��߁A���[�J���ϐ��ɑ�����Ďg�p����B
			std::vector<TCHAR> cmd(commandLine.begin(), commandLine.end());
			cmd.push_back('\0');

			return CreateProcess(NULL, &cmd[0], NULL, NULL, FALSE, 0, NULL, NULL, &m_startupInfo, &m_processInformation);
		}

		//-------------------------------------------------------------
		// �v���Z�X�n���h���N���[�Y
		//-------------------------------------------------------------
		BOOL Close() {
			BOOL bRet = TRUE;
			if (m_processInformation.hThread) {
				if (!CloseHandle(m_processInformation.hThread)) {
					bRet = FALSE;
				}
			}
			if (m_processInformation.hProcess) {
				if (!CloseHandle(m_processInformation.hProcess)) {
					bRet = FALSE;
				}
			}
			memset(&m_startupInfo, 0, sizeof(m_startupInfo));
			memset(&m_processInformation, 0, sizeof(m_processInformation));
			return bRet;
		};

		//-------------------------------------------------------------
		// �v���Z�X�������I��
		//-------------------------------------------------------------
		BOOL Terminate(UINT uExitCode) { return ::TerminateProcess(m_processInformation.hProcess, uExitCode); };

		//-------------------------------------------------------------
		// �v���Z�X�I����҂�
		//-------------------------------------------------------------
		DWORD Wait(DWORD dwMilliseconds = INFINITE) { return WaitForSingleObject(m_processInformation.hProcess, dwMilliseconds); };

		//-------------------------------------------------------------
		// �I���R�[�h���擾����
		//-------------------------------------------------------------
		DWORD GetExitCodeProcess(LPDWORD lpExitCode) { return ::GetExitCodeProcess(m_processInformation.hProcess, lpExitCode); };

	private:
		//-------------------------------------------------------------
		// �R�s�[�Ƒ�����֎~
		//-------------------------------------------------------------
		Process(const Process& src) {};
		void operator=(const Process& rhs) {};

	public:
		//-------------------------------------------------------------
		// STARTUPINFO::wShowWindow �ւ̃A�N�Z�T
		//-------------------------------------------------------------
		WORD get_ShowWindow() { return m_startupInfo.wShowWindow; };
		void set_ShowWindow(WORD value) {
			m_startupInfo.dwFlags |= STARTF_USESHOWWINDOW;
			m_startupInfo.wShowWindow = value;
		};

	private:
		STARTUPINFO m_startupInfo;
		PROCESS_INFORMATION m_processInformation;
	};
};
