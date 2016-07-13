#pragma once

#pragma warning(disable : 4786)

#ifdef UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "version.lib")
#include <tlhelp32.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <share.h>

#ifdef USE_WSSLOG
#include "wsslog.h"
#pragma comment(lib, "../kjmlib3_v1/wsslog.lib")
#endif

#ifdef UNICODE
#define _tstring			std::wstring
#define _tcout				std::wcout
#define _tostringstream		std::wostringstream
#define _tostream			std::wostream
#else
#define _tstring			std::string
#define _tcout				std::cout
#define _tostringstream		std::ostringstream
#define _tostream			std::ostream
#endif

_tostream& operator<<(_tostream& os, __int64 i);

namespace kl3 {

	class util {
	public:
		static _tstring get_module_file_name();

		static _tstring get_file_version(const _tstring &strFileName);

		static _tstring get_error_message(DWORD dwError);

		static _tstring create_backup_filename(const _tstring &strOriginalFilename);

		static _tstring chonp(const _tstring &str);

		// �ċA�I�Ƀt�H���_���쐬�\�� CreateDirectory �֐�
		static bool create_directory(const _tstring& strPathName);

		// �ċA�I�Ƀt�H���_���폜�\�� RemoveDirectory �֐�
		static bool remove_directory(const _tstring& strPathName);

		// �e���|�����t�@�C���p�̃f�B���N�g���̃p�X���擾���܂��B
		static _tstring get_temp_path();

		// �������u������BVB��StrReplace�̂悤�Ȋ֐��B
		static _tstring str_replace(const _tstring& src, const _tstring& strFind, const _tstring& strReplace);

	private:
		util() {};
		~util() {};
	};

	class path {
	public:
		static _tstring append(const _tstring& strPath, const _tstring& strMore);

		static _tstring remove_file_spec(const _tstring& strPath);

		static _tstring rename_file_spec(const _tstring& strPath, const _tstring& strFileSpec);

		static bool file_exists(const _tstring& strPath);

		static _tstring find_file_name(const _tstring& strPath);

		// PathRenameExtension �֐��̃��b�p�[
		static _tstring rename_extension(const _tstring& strPath, const _tstring& strExt);

		// PathRemoveExtension �֐��̃��b�p�[
		static _tstring remove_extension(const _tstring& strPath);

	private:
		path() {};
		~path() {};
	};

	class tool_help {
	public:
		tool_help() : m_hSnapshot(INVALID_HANDLE_VALUE) {};

		virtual ~tool_help();

		virtual bool create_snapshot();

		virtual void close_handle();

		virtual bool process_first();

		virtual bool process_next();

		virtual _tstring get_exe_file() {
			return m_pe.szExeFile;
		};

		virtual DWORD get_process_id() {
			return m_pe.th32ProcessID;
		};

	private:
		HANDLE m_hSnapshot;
		PROCESSENTRY32 m_pe;
	};

	class process {
	public:
		process();

		virtual ~process();

		virtual bool create_process(const _tstring& strCommandLine);

		virtual bool open_process(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);

		virtual void close_handle();

		virtual bool terminate_process(UINT uExitCode);

	private:
		PROCESS_INFORMATION m_processInformation;

	public:
		static bool terminate_process(DWORD dwProcessId, UINT uExitCode);
	};

	//-----------------------------------------------------------------
	// ��r�I�ėp���O�N���X
	//-----------------------------------------------------------------
	class log {
	public:
		// �f�t�H���g�R���X�g���N�^
		log();
		
		// ���ʎq�t���R���X�g���N�^
		log(const _tstring& strIdent) : m_strIdent(strIdent) {};

		// �f�X�g���N�^
		virtual ~log() {};

		// ���O�̏o�͊֐�
		virtual void write(LPCTSTR format, ...);

		// ���O�̏o�͊֐�(��ʏo�͂���)
		virtual void writee(LPCTSTR format, ...);

		// ���ʎq�̐ݒ�
		virtual void set_ident(const _tstring& strNewIdent) {
			m_strIdent = strNewIdent;
		};

		// ���O�t�@�C���̃v���t�B�N�X��ݒ肷��
		virtual void set_prefix(const _tstring& strPrefix);

	public:
		_tstring m_strIdent;		// ���O�̎��ʎq
		_tstring m_strBaseFileName;	// ��{�ɂȂ郍�O�t�@�C����
	};

	//-----------------------------------------------------------------
	// ���Ԃ������N���X
	//-----------------------------------------------------------------
	class time {
	public:
//#define FT_1SEC		(__int64)(FT_1MSEC * 1000)

		static const __int64 FT_1USEC;	// = 10
		static const __int64 FT_1MSEC;	// = FT_1USEC * 1000
		static const __int64 FT_1SEC;	// = FT_1MSEC * 1000

	public:
		time() { memset(&m_ft, 0, sizeof(m_ft)); };

		time(FILETIME ft) : m_ft(ft) {};

		// SYSTEMTIME���󂯎��R���X�g���N�^
		time(const SYSTEMTIME& st);

		virtual ~time() {};

		// �������Ԃ����[�J�����Ԃɕϊ�
		virtual kl3::time &to_local_time();

		// ���Ԃ𕶎���(yyyy/mm/dd hh:mm:ss)�ɕϊ�
		virtual _tstring to_string() const;

		// ���Ԃ𕶎���(YYYYMMDD)�ɕϊ�
		virtual _tstring to_string_yyyymmdd() const;

		// ���Ԃ��������ǂ�����r����
		virtual bool operator==(const kl3::time& rhs) const;

		// ���Ԃ̈����Z������
		virtual __int64 operator-(const kl3::time& rhs) const;

		// ������FILETIME��__int64�̒l�Ƃ��ĕԂ�
		virtual __int64 to_int64() const;

		// ���݂̃��[�J���������擾����
		static kl3::time get_local_time();

	private:
		FILETIME m_ft;
	};

	//-----------------------------------------------------------------
	// Find*File�֐������b�v�����N���X
	//-----------------------------------------------------------------
	class find_file {
	public:
		find_file() : m_hFind(INVALID_HANDLE_VALUE), m_strWildcard(_T("")) {};

		virtual ~find_file();

		virtual bool find_first_file(const _tstring& strFileName);

		virtual bool find_next_file();

		virtual void find_close();

		virtual bool is_dots() {
			return ((lstrcmp(m_findData.cFileName, _T(".")) == 0) || (lstrcmp(m_findData.cFileName, _T("..")) == 0));
		};

		virtual bool is_directory() {
			return ((m_findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
		};

		virtual _tstring get_file_name() {
			return m_findData.cFileName;
		};

		virtual _tstring get_full_name() {
			return kl3::path::rename_file_spec(m_strWildcard, m_findData.cFileName);
		};

		virtual kl3::time get_last_write_time() {
			return kl3::time(m_findData.ftLastWriteTime);
		};

		// �t�@�C���̃T�C�Y��Ԃ�
		virtual __int64 get_file_size() {
			LARGE_INTEGER li = {m_findData.nFileSizeLow, m_findData.nFileSizeHigh};
			return li.QuadPart;
		};

	private:
		_tstring m_strWildcard;
		HANDLE m_hFind;
		WIN32_FIND_DATA m_findData;
	};

	//---------------------------------------------------------------------
	// �h���C�u�̏���ێ�����N���X
	//---------------------------------------------------------------------
	class drive {
	public:
		// �f�t�H���g�R���X�g���N�^
		drive() : m_driveNo(-1) {};

		// �R�s�[�R���X�g���N�^
		drive(const drive& src) : m_driveNo(src.m_driveNo) {};

		// �h���C�u�ԍ����w�肷��R���X�g���N�^
		drive(int driveNo) : m_driveNo(driveNo) {};

		// �h���C�u���^�[���w�肷��R���X�g���N�^
		drive(const _tstring& driveLetter);

		// �f�X�g���N�^
		virtual ~drive() {};

		// ������Z�q
		drive& operator=(const drive& rhs);

		// �h���C�u�ԍ����擾
		int get_DriveNo() { return m_driveNo; };

		// �h���C�u���^�[���擾
		_tstring get_DriveLetter() { return _tstring(1, (m_driveNo + 'A')) + _T(":"); };

		// �h���C�u�^�C�v���擾
		UINT get_DriveType() { return GetDriveType(get_DriveLetter().c_str()); };

		// �h���C�u�^�C�v�𕶎���Ŏ擾
		_tstring get_DriveTypeString();

	private:
		int m_driveNo;	// �h���C�u�ԍ�(-1=���I�� 0=A 1=B...)
	};

	//---------------------------------------------------------------------
	// �h���C�u�̈ꗗ��ێ�����N���X
	//---------------------------------------------------------------------
	class drive_list {
	public:
		// �f�t�H���g�R���X�g���N�^
		drive_list();

		// �R�s�[�R���X�g���N�^
		drive_list(const drive_list& src) : m_driveList(src.m_driveList) {};

		// �f�X�g���N�^
		virtual ~drive_list() {};

		// ������Z�q
		drive_list& operator=(const drive_list& rhs);

		// �h���C�u�ꗗ�̐����擾
		int get_Size() { return m_driveList.size(); };

		// �h���C�u�I�u�W�F�N�g���擾
		drive get_Drive(int index) { return m_driveList[index]; };

	private:
		std::vector<drive> m_driveList;	// PC�ɂȂ����Ă���h���C�u�ꗗ
	};

	//-----------------------------------------------------------------
	// GetTickCount�Ōo�ߎ��Ԃ��v��N���X
	//-----------------------------------------------------------------
	class tick_counter {
	public:
		// �f�t�H���g�R���X�g���N�^
		tick_counter() : m_t1(GetTickCount()), m_t2(0) {};

		// �f�X�g���N�^
		~tick_counter() {};

		// �v���J�n
		void start() { m_t1 = GetTickCount(); m_t2 = 0; };

		// �v���I��
		void stop() { m_t2 = GetTickCount(); };

		// �o�ߎ���(ms)
		DWORD elapse() { return (m_t2 == 0) ? (GetTickCount() - m_t1) : (m_t2 - m_t1); };

		// �o�ߎ���(sec)
		DWORD elapse_sec() { return elapse() / 1000; };

		// �o�ߎ���(min)
		DWORD elapse_min() { return elapse_sec() / 60; };

	private:
		DWORD m_t1;	// �J�n�������̎���(�R���X�g���N�^�Astart�ōX�V)
		DWORD m_t2;	// �I���������̎���(stop�ōX�V)
	};

	//-----------------------------------------------------------------
	// folder �I�u�W�F�N�g
	//-----------------------------------------------------------------
	class folder {
	public:
		// �f�t�H���g�R���X�g���N�^
		folder();

		// �R�s�[�R���X�g���N�^
		folder(const folder &src);

		// �t�H���_���������ɂƂ�R���X�g���N�^
		folder(const _tstring &strPath);

		// �f�X�g���N�^
		virtual ~folder();

		// �t�H���_���̂��ׂẴt�@�C����Ԃ�
		virtual std::vector<_tstring> get_files();

		// �t�H���_���̂��ׂẴT�u�t�H���_��Ԃ�
		virtual std::vector<_tstring> get_sub_folders();

		// ������Z�q
		folder &operator=(const folder &rhs);

	private:
		_tstring m_path;
		std::vector<_tstring> m_files;
		std::vector<_tstring> m_sub_folders;
	};

	//-----------------------------------------------------------------
	// ���C�u���������O�o�͂Ɏg�p���� kl3::log �I�u�W�F�N�g��ݒ�
	//-----------------------------------------------------------------
	void set_log_object(kl3::log* pLog);
};
