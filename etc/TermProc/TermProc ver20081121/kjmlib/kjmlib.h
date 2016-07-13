//---------------------------------------------------------------------
// �����p���C�u����
//
// ���ׂẴN���X�́Anamespace kjm�Ő錾�����B
//---------------------------------------------------------------------
#pragma once


#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <string>
#include <vector>
#include <share.h>

#include "getopt.h"


namespace kjm {

	//---------------------------------------------------------------------
	// �ȈՃ��O�N���X
	//
	// [����]
	// %TEMP%\<���s�t�@�C����>YYYYMMDD.log �Ƃ����t�@�C���ɁA�ǂ�ǂ񃍃O��
	// �ǋL����N���X�B
	// �t�@�C���̍폜�������Ȃ��A�r����������x�̃��g���C�����邾���A���[
	// �e�B���e�B�I�N���X�A
	//---------------------------------------------------------------------
	class log {
	public:
		// �f�t�H���g�R���X�g���N�^
		log() {};

		// �f�X�g���N�^
		~log() {};

		// ���O������
		//
		// [����]
		// format  �������䕶����
		void writeLog(const char* format, ...);

	private:
		std::string m_strLogFile;	// ���O�t�@�C���̖��O
	};

	//---------------------------------------------------------------------
	// WIN32�֐��̊ȈՌĂяo���N���X
	//
	// [����]
	// static �֐��݂̂ō\�������B�C���X�^���X���ł��Ȃ��N���X�B
	//---------------------------------------------------------------------
	class win32 {
	public:
		// GetEnvironmentVariable�̃��b�p�[
		//
		// [����]
		// lpName  ���ϐ��̖��O�ւ̃|�C���^
		//
		// [�߂�l]
		// �w�肳�ꂽ���ϐ������l��Ԃ��B
		static std::string getEnvironmentVariable(LPCTSTR lpName);

		// GetModuleFileName�̃��b�p�[
		//
		// [����]
		// hModule  ���W���[���̃n���h��
		//
		// [�߂�l]
		// ���s�t�@�C���̖��O��Ԃ��B
		static std::string getModuleFileName(HMODULE hModule = NULL);

		// PathAppend �̃��b�p�[
		//
		// [����]
		// strPath  ���ɂȂ�t���p�X�̃t�@�C����
		// strMore  ��������p�X�̖��O
		//
		// [�߂�l]
		// �������ꂽ�V�����t�@�C����������
		static std::string pathAppend(const std::string& strPath, const std::string& strMore);

		// PathFindFileName �̃��b�p�[
		//
		// [����]
		// strPath  �t�@�C��������������t���p�X�̃t�@�C����
		//
		// [�߂�l]
		// �t�@�C���������̕�����
		static std::string pathFindFileName(const std::string& strPath);

		// PathRemoveExtension �̃��b�p�[
		//
		// [����]
		// strPath  �g���q����菜�������t�@�C����
		//
		// [�߂�l]
		// �g���q����菜�����V����������
		static std::string pathRemoveExtension(const std::string& strPath);

		// �t�@�C������������菜���ĐV�����t�@�C��������������
		// PathRemoveFileSpec �� PathAppend �����������悤�Ȋ֐�
		//
		// [����]
		// strPath  ���ɂȂ�t���p�X�̃t�@�C����
		// strMore  �ύX���� FileSpec
		//
		// [�߂�l]
		// �V�����t���p�X�̃t�@�C�����B
		static std::string pathRenameFileSpec(const std::string& strPath, const std::string& strMore);

		// PathRenameExtension �̃��b�p�[
		//
		// [����]
		// strPath  ���ɂӂ���t���p�X�̃t�@�C����
		// strExt   �u��������g���q
		//
		// [�߂�l]
		// �g���q��u���������V�����t�@�C����������
		static std::string pathRenameExtension(const std::string& strPath, const std::string& strExt);

	private:
		win32() {};
		~win32() {};
	};


	//---------------------------------------------------------------------
	// ini�t�@�C���A�N�Z�X�N���X
	//
	// [����]
	// �N���X��錾���邾���ŁA���s�t�@�C����.ini �ɃA�N�Z�X�ł���悤�ɂ�
	// ��֗��N���X�B
	//---------------------------------------------------------------------
	class iniFile {
	public:
		// �f�t�H���g�R���X�g���N�^
		//
		// [����]
		// ���s�t�@�C����.ini ���f�t�H���g�ŃA�N�Z�X����悤�ɒ��������B
		iniFile();

		// �R�s�[�R���X�g���N�^
		iniFile(const iniFile& src) : m_strIniFile(src.m_strIniFile) {};

		// �f�X�g���N�^
		~iniFile() {};

		// ������Z�q
		iniFile& operator =(const iniFile& rhs);

	private:
		std::string m_strIniFile;	// ini�t�@�C���̖��O
	};


	//---------------------------------------------------------------------
	// �v���Z�X�֌W�N���X
	//---------------------------------------------------------------------
	class process {
	public:
		// �R���X�g���N�^
		process() : m_hProc(NULL), m_pLog(NULL) {};

		// ���O�I�u�W�F�N�g���w�肷��R���X�g���N�^
		process(kjm::log* pLog) : m_hProc(NULL), m_pLog(pLog) {};

		// �f�X�g���N�^
		virtual ~process() { closeHandle(); };

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
			if (m_pLog) {
				m_pLog->writeLog(_T(">> kjm::process::openProcess(%u, %d, %u)"), dwDesiredAccess, bInheritHandle, dwProcessId);
				writePropertyLog();
			}

			m_hProc = ::OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);

			if (m_pLog) {
				if (m_hProc == NULL)	m_pLog->writeLog(_T("   OpenProcess(%u, %d, %u) error(%u)"), dwDesiredAccess, bInheritHandle, dwProcessId, ::GetLastError());
				m_pLog->writeLog(_T("<< kjm::process::openProcess() bool(%p != NULL)."), m_hProc);
			}
			return m_hProc != NULL;
		};

		// CloseHandle API�̃��b�p�[
		//
		// [�߂�l]
		// �֐�����������ƁATRUE ���Ԃ�B
		// �֐������s����ƁAFALSE ���Ԃ�B
		//
		// [����]
		// �֐��̐����A���s�ɂ�����炸�A�n���h����NULL�ɏ����������B
		BOOL closeHandle() {
			if (m_pLog) {
				m_pLog->writeLog(_T(">> kjm::process::closeHandle()"));
				writePropertyLog();
			}

			BOOL bRet = TRUE;
			if (m_hProc != NULL) {
				bRet = ::CloseHandle(m_hProc);
				m_hProc = NULL;
			}

			if (m_pLog)	m_pLog->writeLog(_T("<< kjm::process::closeHandele() BOOL(%d)."), bRet);
			return bRet;
		};

		// TerminateProcess API�̃��b�p�[
		//
		// [����]
		// uExitCode  exit code for the process
		//
		// [�߂�l]
		// �֐�����������ƁATRUE ���Ԃ�B
		// �֐������s����ƁAFALSE ���Ԃ�B
		BOOL terminateProcess(UINT uExitCode) {
			if (m_pLog) {
				m_pLog->writeLog(_T(">> kjm::process::terminateProcess(%u)"), uExitCode);
				writePropertyLog();
			}

			BOOL bRet = ::TerminateProcess(m_hProc, uExitCode);

			if (m_pLog) {
				if (bRet == FALSE) {
					m_pLog->writeLog(_T("   TerminateProcess(%p, %u) error(%d)"), m_hProc, uExitCode, ::GetLastError());
				}
				m_pLog->writeLog(_T("<< kjm::process::terminateProcess() BOOL(%d)"), bRet);
			}
			return bRet;
		};

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
		static BOOL terminateProcess(DWORD dwProcessId, UINT uExitCode, kjm::log* pLog = NULL) {
			if (pLog)	pLog->writeLog(_T(">> static kjm::process::terminateProcess(%u, %u, %p)"), dwProcessId, uExitCode, pLog);

			BOOL bRet = FALSE;
			kjm::process proc(pLog);
			if (proc.openProcess(PROCESS_TERMINATE, FALSE, dwProcessId)) {
				bRet = proc.terminateProcess(uExitCode);
			}

			if (pLog)	pLog->writeLog(_T("<< static kjm::process::terminateProcess() BOOL(%d)."), bRet);
			return bRet;
		};

	private:
		HANDLE m_hProc;		// �I�[�v�����Ă���v���Z�X�̃n���h��
		kjm::log* m_pLog;	// ���O�I�u�W�F�N�g�ւ̃|�C���^

		// �v���p�e�B(�����ϐ�)���\�Ȃ烍�O�ɏo�͂���
		void writePropertyLog() {
			if (m_pLog) {
				m_pLog->writeLog(_T("   this->m_hProc = %p"), m_hProc);
			}
		};
	};

};	// namespace kjm

