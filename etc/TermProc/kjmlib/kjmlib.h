//---------------------------------------------------------------------
// �����p���C�u����
//
// ���ׂẴN���X�́Anamespace kjm�Ő錾�����B
//---------------------------------------------------------------------
#pragma once

#if defined(UNICODE)
#define _UNICODE
#endif

#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <tlhelp32.h>
#include <string>
#include <vector>
#include <share.h>

#include "getopt.h"


namespace kjm {
	typedef std::basic_string<TCHAR>	_tstring;

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
		void writeLog(LPCTSTR format, ...);

	private:
		kjm::_tstring m_strLogFile;	// ���O�t�@�C���̖��O
	};


	//---------------------------------------------------------------------
	// ������Ƃ�������֐��������߂��N���X
	//---------------------------------------------------------------------
	class util {
	public:
		static bool is_archive(const WIN32_FIND_DATA& data) {
			return ((data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0);
		};

		static bool is_system(const WIN32_FIND_DATA& data) {
			return ((data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0);
		};

		static bool is_readonly(const WIN32_FIND_DATA& data) {
			return ((data.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0);
		};

		static bool is_directory(const WIN32_FIND_DATA& data) {
			return ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
		};

		static bool is_hidden(const WIN32_FIND_DATA& data) {
			return ((data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0);
		};

		static bool is_dots(const WIN32_FIND_DATA& data) {
			return ((::lstrcmp(data.cFileName, _T(".")) == 0) || (::lstrcmp(data.cFileName, _T("..")) == 0));
		};

	private:
		util() {};
		~util() {};
	};

	//---------------------------------------------------------------------
	// WIN32�֐��̊ȈՌĂяo���N���X
	//
	// [����]
	// static �֐��݂̂ō\�������B�C���X�^���X���ł��Ȃ��N���X�B
	//---------------------------------------------------------------------
	class win32 {
	public:
		// �ċA�I�Ƀt�H���_���쐬�ł���֐�
		//
		static BOOL makeDirectory(LPCTSTR pszFolder);

		// GetEnvironmentVariable�̃��b�p�[
		//
		// [����]
		// lpName  ���ϐ��̖��O�ւ̃|�C���^
		//
		// [�߂�l]
		// �w�肳�ꂽ���ϐ������l��Ԃ��B
		static kjm::_tstring getEnvironmentVariable(LPCTSTR lpName);

		// GetModuleFileName�̃��b�p�[
		//
		// [����]
		// hModule  ���W���[���̃n���h��
		//
		// [�߂�l]
		// ���s�t�@�C���̖��O��Ԃ��B
		static kjm::_tstring getModuleFileName(HMODULE hModule = NULL);

		// PathAppend �̃��b�p�[
		//
		// [����]
		// strPath  ���ɂȂ�t���p�X�̃t�@�C����
		// strMore  ��������p�X�̖��O
		//
		// [�߂�l]
		// �������ꂽ�V�����t�@�C����������
		static kjm::_tstring pathAppend(const kjm::_tstring& strPath, const kjm::_tstring& strMore);

		// PathFindFileName �̃��b�p�[
		//
		// [����]
		// strPath  �t�@�C��������������t���p�X�̃t�@�C����
		//
		// [�߂�l]
		// �t�@�C���������̕�����
		static kjm::_tstring pathFindFileName(const kjm::_tstring& strPath);

		// PathRemoveExtension �̃��b�p�[
		//
		// [����]
		// strPath  �g���q����菜�������t�@�C����
		//
		// [�߂�l]
		// �g���q����菜�����V����������
		static kjm::_tstring pathRemoveExtension(const kjm::_tstring& strPath);

		// �t�@�C������������菜���ĐV�����t�@�C��������������
		// PathRemoveFileSpec �� PathAppend �����������悤�Ȋ֐�
		//
		// [����]
		// strPath  ���ɂȂ�t���p�X�̃t�@�C����
		// strMore  �ύX���� FileSpec
		//
		// [�߂�l]
		// �V�����t���p�X�̃t�@�C�����B
		static kjm::_tstring pathRenameFileSpec(const kjm::_tstring& strPath, const kjm::_tstring& strMore);

		// PathRenameExtension �̃��b�p�[
		//
		// [����]
		// strPath  ���ɂӂ���t���p�X�̃t�@�C����
		// strExt   �u��������g���q
		//
		// [�߂�l]
		// �g���q��u���������V�����t�@�C����������
		static kjm::_tstring pathRenameExtension(const kjm::_tstring& strPath, const kjm::_tstring& strExt);

	private:
		win32() {};
		~win32() {};
	};


	//-----------------------------------------------------------------
	// MAX_PATH �𒴂��镶�����������N���X
	//-----------------------------------------------------------------
	class pathString {
	public:
		//-------------------------------------------------------------
		// �f�t�H���g �R���X�g���N�^
		//-------------------------------------------------------------
		pathString() : m_str() {};

		//-------------------------------------------------------------
		// �R�s�[ �R���X�g���N�^
		//-------------------------------------------------------------
		pathString(const pathString& src) : m_str(src.m_str) {};

		//-------------------------------------------------------------
		// _tstring ���󂯎��R���X�g���N�^
		//-------------------------------------------------------------
		pathString(const _tstring& src) : m_str(src) {};

		//-------------------------------------------------------------
		// �f�X�g���N�^
		//-------------------------------------------------------------
		virtual ~pathString() {};

		//-------------------------------------------------------------
		// ������Z�q
		//-------------------------------------------------------------
		pathString& operator=(const pathString& rhs) {
			if (this == &rhs) return *this;
			m_str = rhs.m_str;
			return *this;
		};

		//-------------------------------------------------------------
		// ������̒�����Ԃ�
		//-------------------------------------------------------------
		int length() { return m_str.length(); };

		//-------------------------------------------------------------
		// ����������ւ̃A�N�Z�X
		//-------------------------------------------------------------
		LPCTSTR c_str() { return m_str.c_str(); };

		//-------------------------------------------------------------
		// LPCTSTR �ւ̃L���X�g
		//-------------------------------------------------------------
		operator LPCTSTR() { return m_str.c_str(); };

		//-------------------------------------------------------------
		// \\?\ \\?\UNC ��ǉ������������Ԃ�
		//
		// CopyFile �Ȃǂ́AMAX_PATH �𒴂����t�@�C�����𐳏�ɏ������邽�߂ɂ́A
		// ���� \\?\ or \\?\UNC �������p�X������������Ƃ��ēn���K�v������B
		//-------------------------------------------------------------
		_tstring GetOverMaxPath();

		//-------------------------------------------------------------
		// �p�X������̌��� '\' ��ǉ�����
		//-------------------------------------------------------------
		pathString& AddBackslash();

		//-------------------------------------------------------------
		// �p�X������̌���
		//
		// param: pMore: �ǉ����镶����
		//-------------------------------------------------------------
		pathString& Append(const _tstring& pMore);

	private:
		_tstring m_str;	// �Ǘ�������
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
		kjm::_tstring m_strIniFile;	// ini�t�@�C���̖��O
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

	// toolhelp�̃T�|�[�g�N���X
	class toolhelp {
	public:
		toolhelp() : m_pLog(NULL) {};
		toolhelp(kjm::log* pLog) : m_pLog(pLog) {};
		virtual ~toolhelp() {};

		bool createSnapProcess();
		int snapProcessSize() { return m_peList.size(); };
		PROCESSENTRY32& getSnapProcess(int n) { return m_peList.at(n); };

	private:
		std::vector<PROCESSENTRY32> m_peList;
		kjm::log* m_pLog;
	};


	//---------------------------------------------------------------------
	// FindFile�n�֐��̃��b�v�N���X
	//---------------------------------------------------------------------
	class find_file {
	public:
		find_file() : m_hFind(INVALID_HANDLE_VALUE), m_path(_T(".")), m_pattern(_T("*.*")) {};

		virtual ~find_file() { find_close(); };

		bool find_files() {
			bool bRet = false;
			if (m_hFind == INVALID_HANDLE_VALUE) {
				kjm::pathString(m_path).Append(m_pattern);
				m_hFind = ::FindFirstFile(kjm::pathString(m_path).Append(m_pattern), &m_data);
				bRet = (m_hFind != INVALID_HANDLE_VALUE);
				m_last_error = ::GetLastError();
			} else {
				bRet = (::FindNextFile(m_hFind, &m_data) != FALSE);
				m_last_error = ::GetLastError();
			}
			return bRet;
		};

		void find_close() {
			if (m_hFind != INVALID_HANDLE_VALUE) {
				::FindClose(m_hFind);
				m_hFind = INVALID_HANDLE_VALUE;
			}
		};

		bool is_archive() const { return kjm::util::is_archive(m_data); };

		bool is_system() { return kjm::util::is_system(m_data); };

		bool is_readonly() { return kjm::util::is_readonly(m_data); };

		bool is_directory() { return kjm::util::is_directory(m_data); };

		bool is_hidden() { return kjm::util::is_hidden(m_data); };

		bool is_dots() { return kjm::util::is_dots(m_data); };

		_tstring get_filename() { return m_data.cFileName; };

		FILETIME get_last_write_time() const { return m_data.ftLastWriteTime; };

		__int64 get_file_size() {
			LARGE_INTEGER ui64 = {m_data.nFileSizeLow, m_data.nFileSizeHigh};
			return ui64.QuadPart;
		};

		void set_path(LPCTSTR value) { m_path = value; };

		DWORD get_last_error() { return m_last_error; };
	private:
		DWORD m_last_error;
		HANDLE m_hFind;
		WIN32_FIND_DATA m_data;
		_tstring m_path;
		_tstring m_pattern;
	};

};	// namespace kjm

