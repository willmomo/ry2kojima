//---------------------------------------------------------------------
// kjmlib.h : ���[�e�B���e�B�I�Ɏg���郉�C�u����
//
// �ق��̃R�[�h�ƃo�b�e�B���O���Ȃ��悤�ɁAnamespace kjm �ŁA�쐬���Ă���܂��B
// stl�x�[�X�̏����΂���Ȃ̂ŁAdll �� exe ���܂����Ŏg�p���邱�Ƃ́A�ł��܂���B
//
// USE_WSSLOG �� define ���Ă����ƁAwsslog.dll ���g���āA���O���c���B
//
// kjmlib.cpp �ƃZ�b�g�ŁA�������Ďg�p���Ă��������B
// shlwapi.lib �������N���Ă��������B
//
// 2007.09.05
//---------------------------------------------------------------------

#pragma once

#if !defined(__KJMLIB_H)
#define __KJMLIB_H

#include <windows.h>
#include <tchar.h>
#include <shellapi.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <string>
#include <string>
#include <vector>

#include <assert.h>
#include <process.h>

#if defined(USE_WSSLOG)
#include "wsslog.h"
#endif


namespace kjm {
	// OutputDebugString ���ȒP�Ɏg�����߂̊֐�
	void ods(LPCTSTR format, ...);

	void log_info(const char* signature, const char* format, ...);	// ��񃍃O���c���֐�
	void log_error(const char* signature, const char* format, ...);	// �G���[���O���c���֐�

	BOOL create_directory(LPCTSTR lpPathName);	// �ċA�I�Ƀf�B���N�g�����쐬�ł��� CreateDirectory �֐�
	
	// �p�X��������������
	//
	class PathString
	{
	private:
		std::string m_path;

	public:
		// �R���X�g���N�^
		PathString() : m_path() {};
		PathString( const PathString& src ) : m_path( src.m_path ) {};
		PathString( const char* src ) : m_path( src ) {};

		// �f�X�g���N�^
		virtual ~PathString() {};

	public:
		// ������ւ� const pointer ��߂�
		const char* c_str() const { return m_path.c_str(); };

		// ������Ƀp�X����������
		PathString& Append( const PathString& more ) {
			std::vector<char> buf( MAX_PATH );
			
			///::lstrcpy( buf.begin(), m_path.c_str() );
			::lstrcpy( &buf.at(0), m_path.c_str() );
			///::PathAppend( buf.begin(), more.c_str() );
			::PathAppend( &buf.at(0), more.c_str() );

			///m_path = buf.begin();
			m_path = &buf.at(0);

			return *this;
		};

	public:
		// ������Z�q
		PathString& operator= ( const PathString& src ) {
			if (this == &src)	return *this;	// ����I�u�W�F�N�g����`�F�b�N
			m_path = src.c_str();
			return *this;
		};
	};

	// API�֐��̃��b�p�[�N���X
	//
	class win32 {
	private:
		win32() {};		// �f�t�H���g�R���X�g���N�^
		~win32() {};	// �f�X�g���N�^

	public:
		// ExpandEnvironmentStrings �̃��b�p�[
		//
		static std::string expand_environment_strings(LPCTSTR lpSrc) {
			DWORD dwSizeNeed;

			// �K�v�ȃo�b�t�@�T�C�Y���擾
			dwSizeNeed = ::ExpandEnvironmentStrings(lpSrc, NULL, 0);
			if (dwSizeNeed == 0) {
				return std::string();	// �G���[
			}

			// �K�v�ȃo�b�t�@���m�ۂ��āA�Ď��s
			std::vector<char> buf(dwSizeNeed);
			///dwSizeNeed = ::ExpandEnvironmentStrings(lpSrc, buf.begin(), buf.size());
			dwSizeNeed = ::ExpandEnvironmentStrings(lpSrc, &buf.at(0), buf.size());
			if (dwSizeNeed != buf.size() - 1) {
				return std::string();	// �G���[
			}

			///return std::string(buf.begin());	// �W�J���ꂽ������
			return std::string(&buf.at(0));	// �W�J���ꂽ������
		};

		static std::string format_message(DWORD dwErrorCode);	// �G���[�R�[�h����V�X�e���G���[���b�Z�[�W���擾����B
		static std::string str_replace(const std::string& src, LPCTSTR pFind, LPCTSTR pReplace);	// �������u������BVB��StrReplace�̂悤�Ȋ֐��B
	};

	//-------------------------------------------------------------------------
	// ���W�X�g���������N���X
	//-------------------------------------------------------------------------
	class registry {
	public:
		// �f�t�H���g�R���X�g���N�^
		registry() : m_hkey(NULL) {}

		// �R�s�[�R���X�g���N�^
		registry(const registry& src);

		// �f�X�g���N�^
		virtual ~registry() { reg_close_key(); }

		// RegOpenKeyEx �̃��b�p�[
		//
		// hkey        �J���L�[�ւ̃n���h��
		// lpSubKey    �J���ׂ��T�u�L�[�̖��O�ւ̃A�h���X
		// samDesired  �Z�L�����e�B�A�N�Z�X�}�X�N
		LONG reg_open_key_ex(HKEY hkey, LPCTSTR lpSubKey, REGSAM samDesired);

		// RegCloseKey �̃��b�p�[
		LONG reg_close_key();

		// RegQueryValueEx �̃��b�p�[
		//
		// lpValueName  �������ׂ��l�̖��O�ւ̃A�h���X
		// lpType       �l�̃^�C�v�̂��߂̃o�b�t�@�ւ̃A�h���X
		// lpData       �f�[�^�̃o�b�t�@�ւ̃A�h���X
		// lpcbData     �f�[�^�̃o�b�t�@�̃T�C�Y�ւ̃A�h���X
		LONG reg_query_value_ex(LPTSTR lpValueName, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData) const;

		// RegSetValueEx �̃��b�p�[
		//
		// lpValueName  �������ׂ��l�̖��O�ւ̃A�h���X
		// lpType       �l�̃^�C�v�̂��߂̃o�b�t�@�ւ̃A�h���X
		// lpData       �f�[�^�̃o�b�t�@�ւ̃A�h���X
		// lpcbData     �f�[�^�̃o�b�t�@�̃T�C�Y�ւ̃A�h���X
		LONG reg_set_value_ex(LPCTSTR lpValueName, DWORD dwType, CONST BYTE *lpData, DWORD cbData) {
			assert(m_hkey != NULL);
			return ::RegSetValueEx(m_hkey, lpValueName, 0, dwType, lpData, cbData);
		}
		
		// RegSetValueEx �̃��b�p�[
		//
		// lpValueName  �������ׂ��l�̖��O�ւ̃A�h���X
		// dwData       �f�[�^�̃o�b�t�@�ւ̃A�h���X
		LONG reg_set_value_ex(LPCTSTR lpValueName, DWORD dwData) {
			return reg_set_value_ex(lpValueName, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData));

		}

		// ��������擾����֐�
		//
		// �l�̃^�C�v��
		//   REG_SZ �̂Ƃ��́A���̂܂ܕԂ��B
		//   REG_EXPAND_SZ �̂Ƃ��́AExpandEnvironmentStrings ��ʂ��Ă���Ԃ��B
		//   REG_DWORD �̂Ƃ��́A_ultoa ��ʂ��Ă���Ԃ��B
		std::string get_string(LPTSTR lpValueName, LPCTSTR lpDefault) const;

	public:
		// ������Z�q
		registry& operator =(const registry& rhs);

	private:
		HKEY m_hkey;	// �I�[�v�����̃��W�X�g���ւ̃n���h��
	};


	//-----------------------------------------------------------------
	// ���[�e�B���e�B�N���X
	//
	// [�T�v]
	// �X�^�e�B�b�N�����o�[�݂̂Ȃ̂ŁA�C���X�^���X���ł��Ȃ��B
	//-----------------------------------------------------------------
	class util {
	private:
		util() {};
		~util() {};

	public:
		//-------------------------------------------------------------
		// �R�s�[���̒����ɂ��邷�ׂẴt�@�C�����R�s�[��ɃR�s�[����
		//
		// [����]
		// pszSrcPath  �R�s�[���t�H���_�̖��O
		// pszDstPath  �R�s�[��t�H���_�̖��O
		//
		// [�߂�l]
		// ����I��: NO_ERROR
		// �G���[�I��: Win32�G���[�R�[�h
		//-------------------------------------------------------------
		static DWORD CopyFiles(LPCTSTR pszSrcPath, LPCTSTR pszDstPath);

		// �v���Z�X�̃��W���[�������擾(GetModuleFileName �֐��̃��b�p�[)
		static std::basic_string<TCHAR> getModuleFileName();

		// �v���Z�X����string�Ŏ擾����֐�
		static std::string GetModuleName() {
			return std::string(::PathFindFileName(kjm::util::getModuleFileName().c_str()));
		}

		// ini �t�@�C�����當������擾����(GetPrivateProfileString �֐��̃��b�p�[)
		static std::basic_string<TCHAR> getPrivateProfileString(LPCTSTR section, LPCTSTR key, LPCTSTR defValue, LPCTSTR iniFile);

		static DWORD get_ipaddr(LPCSTR lpName);	// �z�X�g���܂��́AIP�����񂩂�IP�A�h���X���擾

		//-------------------------------------------------------------
		// �f�B���N�g�����ċA�I�ɍ폜����
		//-------------------------------------------------------------
		static bool remove_folder(LPCTSTR pszPath);

		// �p�X������̊g���q��ύX����
		static std::basic_string<TCHAR> renameExtension(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& ext);

		// �p�X������̃t�@�C������ύX����
		static std::basic_string<TCHAR> renameFileSpec(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& fname);
	};


	//=========================================================================
	// �f�B���N�g����T�u�f�B���N�g����ʂ��āA�쐬�A�ړ��A����ї񋓂��邽�߂�
	// �ÓI���\�b�h�����J���܂��B
	//=========================================================================
	class directory {
	public:
		// �t�@�C���܂��̓f�B���N�g���A����т��̓��e��V�����ꏊ�ɃR�s�[���܂��B
		static BOOL copy(LPCTSTR sourceDirName, LPCTSTR destDirName);

	private:
		directory() {};
		~directory() {};
	};


	/**
	 *	@brief	NOTIFYICONDATA �����b�v����N���X
	 */
	class BiNotifyIcon : private NOTIFYICONDATA {
	public:

		// �f�t�H���g�E�R���X�g���N�^
		BiNotifyIcon();

		// �f�X�g���N�^
		virtual ~BiNotifyIcon();

	public:
		// �^�X�N�g���C�ɃA�C�R����ǉ�����B
		BOOL Add(HWND hWnd, UINT uID, UINT uFlags, UINT uCallbackMessage, HICON hIcon, const char* pszTip);

		// �^�X�N�g���C����A�C�R�����폜����
		BOOL Delete();

		// �^�X�N�g���C�A�C�R���̏���ύX����B
		BOOL Modify(UINT uFlags, UINT uCallbackMessage, HICON hIcon, char* pszTip);
	};

	
	// class mutex �̓������O�p�̌^
	//
	struct mutex_error {
		enum error_code {
			not_initialized = 1,	// mutex ����������
			already_initialized		// mutex ���������ς�
		};

		error_code error;
		mutex_error(error_code e) : error(e) {};
	};

	// mutex �Ǘ��N���X
	//
	// note: �쐬�����n���h���́A�f�X�g���N�^�Ŕj�������B
	//       �܂��Alock(WaitForSingleObject)�����܂܂̃n���h���́A
	//       �K�v��unlock(ReleaseMutex)���Ă���A�j�������B
	//
	class mutex {
	private:
		HANDLE m_hMutex;		// �Ǘ����� mutex �ւ̃n���h��
		DWORD m_create_status;	// mutex �쐬���̏�ԕϐ�
		DWORD m_last_error;		// API�g�p���̃G���[�ێ�
		int m_lock_count;		// lock������

		// �ʓ|�Ȃ̂ŁA�C���X�^���X�̕����͕s���B
		mutex(const mutex &src) {};
		mutex& operator=(const mutex &src) {};

	public:
		// mutex �쐬���ɓ����� mutex �����݂����Ƃ��́Ature ��Ԃ�
		//
		bool is_exists() { return (m_create_status == ERROR_ALREADY_EXISTS); };

		// mutex�쐬�ς݂��ǂ����̃e�X�g
		//
		bool is_open() { return (m_hMutex != NULL); };

	public:
		// �R���X�g���N�^�ƃf�X�g���N�^
		//
		mutex() : m_hMutex(NULL), m_create_status(0), m_lock_count(0) {};
		mutex(const std::string& name) : m_hMutex(NULL), m_create_status(0), m_lock_count(0) {
			create(name);	// ��O�͋N���肦�Ȃ��B�`�F�b�N�́Ais_open�Ń`�F�b�N
		};
		virtual ~mutex() { close(); };

		// mutex �̍쐬
		// 
		// arg : name = mutex �̖��O
		// ret : true = ����I���Afalse = �ȏ�I��
		// exp : mutex_error
		// note:
		//
		bool create(const std::string& name) {
			// �������ς� instance �̎��́A��O�� throw
			if (m_hMutex)	throw mutex_error(mutex_error::already_initialized);

			// mutex �̍쐬
			m_hMutex = ::CreateMutex(NULL, FALSE, name.c_str());
			m_create_status = ::GetLastError();
			return (m_hMutex != 0);
		};
		
		// mutex �����
		//
		// arg : �Ȃ�
		// ret : �Ȃ�
		// exp : �Ȃ�
		// note: ���������̃C���X�^���X�Ɏg�p���Ă�ok
		//       lock���̃I�u�W�F�N�g�ɑ΂��ẮAunlock���Ă���N���[�Y����B
		//
		void close() {
			if (m_hMutex) {
				while (m_lock_count)
					if (!unlock())
						break;

				::CloseHandle(m_hMutex);
				m_hMutex = NULL;
			}
		};

		// �w�莞�ԃV�O�i����ԂɂȂ�̂�҂�
		//
		// arg : dwMillisec = �^�C���A�E�g����(ms)
		// ret : WaitForSingleObject API �Q��
		// exp : mutex_error
		// note: mutex���������ς݂̏ꍇ�́AWaitForSingleObject���ĂԂ����B
		//
		DWORD lock(DWORD dwMillisec) {
			if (!m_hMutex)	throw mutex_error(mutex_error::not_initialized);
			
			DWORD dw = ::WaitForSingleObject(m_hMutex, dwMillisec);
			if (dw == WAIT_OBJECT_0 || dw == WAIT_ABANDONED) {
				++m_lock_count;
			}

			return dw;
		};

		// mutex���J������
		//
		// arg : �Ȃ�
		// ret : ReleaseMutex API �Q��
		// exp : mutex_error
		// note: mutex���������ς݂̏ꍇ�́AReleaseMutex ���ĂԂ����B
		//
		BOOL unlock() {
			if (!m_hMutex)	throw mutex_error(mutex_error::not_initialized);
			
			BOOL ret = ::ReleaseMutex(m_hMutex);
			if (ret) {
				--m_lock_count;
			}

			return ret;
		};
	};

	// class memfile �̓������O�p�̌^
	//
	struct memfile_error {
		enum error_code {
			not_initialized = 1,	// memfile ����������
			already_initialized		// memfile ���������ς�
		};
		error_code error;

		memfile_error(error_code e) : error(e) {};
	};

	// �������}�b�v�h�t�@�C���Ǘ��N���X
	//
	// note: �쐬�����n���h���́A�f�X�g���N�^�Ŕj�������B
	//
	template<class T> class memfile {
	private:
		HANDLE m_hFile;
		DWORD m_create_status;

		// �ʓ|�Ȃ̂ŁA�C���X�^���X�̕����͕s���B
		memfile<T>(const memfile<T> &src) {};
		memfile<T>& operator=(const memfile<T> &src) {};

	public:
		// memfile�쐬���ɓ�����memfile�����݂����Ƃ��́Ature ��Ԃ�
		//
		bool is_exists() { return (m_create_status == ERROR_ALREADY_EXISTS); };

		// memfile�쐬�ς݂��ǂ����̃e�X�g
		//
		bool is_open() { return (m_hFile != NULL); };

	public:
		// �R���X�g���N�^�ƃf�X�g���N�^
		//
		memfile<T>() : m_hFile(NULL), m_create_status(0) {};
		memfile<T>(const std::string& name, DWORD size = 0) : m_hFile(NULL), m_create_status(0) {
			// �ǂ������O�͋N���肦�Ȃ��B�������̐��ۂ́Ais_open�Ŋm�F�B
			if (size == 0)	open(name);
			else			create(name, size);
		};
		virtual ~memfile<T>() { close(); };

		// �������}�b�v�h�t�@�C���̍쐬
		//
		// arg : name = �������}�b�v�h�t�@�C���̖��O
		//       size = �T�C�Y(�v�f��)
		// ret : false = ���s�Btrue = ����
		// exp : memfile_error
		// note:
		//
		bool create(const std::string& name, DWORD size) {
			// �������ς�instance�̂Ƃ��́A��O��throw
			if (m_hFile) throw memfile_error(memfile_error::already_initialized);

			// �������}�b�v�h�t�@�C���̍쐬
			m_hFile = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(T) * size, name.c_str());
			m_create_status = ::GetLastError();
			return (m_hFile != NULL);
		};

		// �������}�b�v�h�t�@�C�����J��
		//
		// arg : name = �������}�b�v�h�t�@�C���̖��O
		// ret : false = ���s�Btrue = ����
		// exp : memfile_error
		// note:
		//
		bool open(const std::string& name) {
			// �������ς�instance�̎��́A��O��throw
			if (m_hFile) throw memfile_error(memfile_error::already_initialized);

			// �������}�b�v�h�t�@�C�����J��
			m_hFile = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, name.c_str());
			return (m_hFile != NULL);
		};

		// �������}�b�v�h�t�@�C�������
		//
		// arg : �Ȃ�
		// ret : �Ȃ�
		// exp : �Ȃ�
		// note: ����������instance�Ŏg�p���Ă�ok
		//
		void close() {
			if (m_hFile) {
				::CloseHandle(m_hFile);
				m_hFile = NULL;
			}
		};

		// �������}�b�v�h�t�@�C������f�[�^��ǂݍ���
		//
		// arg : buffer = �ǂݍ��ރo�b�t�@�ւ̃|�C���^
		//       offset = �擪�ʒu�ւ̃I�t�Z�b�g
		//       size = �ǂݍ��ޗv�f��
		//       pmtx = �r������Ɏg�p����mutex�ւ̃|�C���^(NULL��)
		//       timeout = �r���^�C���A�E�g����
		//
		int read(T* buffer, size_t offset, size_t size, mutex* pmtx, DWORD timeout) {
			// ��������instance�̎��́A��O��throw
			if (!m_hFile)	throw memfile_error(memfile_error::not_initialized);

			int ret = 0;
			DWORD dw = WAIT_OBJECT_0;

			// mutex�ւ̃|�C���^���^�����Ă���Ƃ��́A�܂�lock
			if (pmtx)	dw = pmtx->lock(timeout);

			if (dw == WAIT_OBJECT_0 || dw == WAIT_ABANDONED) {
				
				T* p = (T*)::MapViewOfFile(m_hFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
				if (p) {
					memmove(buffer, &p[offset], sizeof(T) * size);
					::UnmapViewOfFile(p);
				} else {
					ret = -1;
				}

				// mutex�ւ̃|�C���^���^�����Ă���Ƃ��́Aunlock
				if (pmtx)	pmtx->unlock();
			} else {
				ret = -2;
			}

			return ret;
		}

		// �������}�b�v�h�t�@�C���փf�[�^����������
		//
		// arg : buffer = �ǂݍ��ރo�b�t�@�ւ̃|�C���^
		//       offset = �擪�ʒu�ւ̃I�t�Z�b�g
		//       size = �ǂݍ��ޗv�f��
		//       pmtx = �r������Ɏg�p����mutex�ւ̃|�C���^(NULL��)
		//       timeout = �r���^�C���A�E�g����
		//
		template <class T> int write(T* buffer, size_t offset, size_t size, mutex* pmtx, DWORD timeout) {
			// ��������instance�̎��́A��O��throw
			if (!m_hFile)	throw memfile_error(memfile_error::not_initialized);

			int ret = 0;
			DWORD dw = WAIT_OBJECT_0;

			// mutex�ւ̃|�C���^���^�����Ă���Ƃ��́A�܂�lock
			if (pmtx)	dw = pmtx->lock(timeout);

			if (dw == WAIT_OBJECT_0 || dw == WAIT_ABANDONED) {
				
				T* p = (T*)::MapViewOfFile(m_hFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
				if (p) {
					memmove(&p[offset], buffer, sizeof(T) * size);
					::UnmapViewOfFile(p);
				} else {
					ret = -1;
				}

				// mutex�ւ̃|�C���^���^�����Ă���Ƃ��́Aunlock
				if (pmtx)	pmtx->unlock();
			} else {
				ret = -2;
			}

			return ret;
		}
	};

	// class lockfile �̓������O�p�̌^
	//
	struct lockfile_error {
		enum error_code {
			not_initialized = 1	// lockfile ����������
		};
		error_code error;

		lockfile_error(error_code e) : error(e) {};
	};

	// lockfile���ȒP�Ɉ����N���X
	//
	class lockfile {
	private:
		HANDLE m_hFile;			// ���b�N�t�@�C���ւ̃n���h��
		std::string m_fullname;	// ���b�N�t�@�C���ւ̃t���p�X��

	public:
		// �R���X�g���N�^�E�f�X�g���N�^
		//
		lockfile(const std::string& fullname) : m_hFile(INVALID_HANDLE_VALUE), m_fullname(fullname) {};
		virtual ~lockfile() { unlock(); };

		// ���b�N����
		//
		bool lock() {
			// ���łɎ����Ń��b�N���Ă���Ƃ��́Atrue��Ԃ�
			if (m_hFile != INVALID_HANDLE_VALUE)	return true;

			m_hFile = ::CreateFile(m_fullname.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE, NULL);
			if (m_hFile == INVALID_HANDLE_VALUE) {

				// lock�Ɏ��s���Ă��A�S�~�̉\��������̂ō폜�����݂�B
				::DeleteFile(m_fullname.c_str());
				m_hFile = ::CreateFile(m_fullname.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE, NULL);
			}
			
			return (m_hFile != INVALID_HANDLE_VALUE);
		};

		// ���b�N����������
		//
		void unlock() {
			if (m_hFile != INVALID_HANDLE_VALUE) {
				::CloseHandle(m_hFile);
				m_hFile = INVALID_HANDLE_VALUE;
			}
		};
	};

#if 0
	//-----------------------------------------------------------------
	// �Ȉ�SOCKET�N���X
	//-----------------------------------------------------------------
	class in_socket {
	private:
		SOCKET m_sock;

	public:
		in_socket() : m_sock(INVALID_SOCKET) {};	// �R���X�g���N�^
		virtual ~in_socket() { close_socket(); };	// �f�X�g���N�^

		// �\�P�b�g���J��
		bool open_socket(int type) {
			assert(m_sock == INVALID_SOCKET);
			return (m_sock = socket(AF_INET, type, 0)) != INVALID_SOCKET;
		};

		// �\�P�b�g�����
		int close_socket() {
			int ret = 0;
			if (m_sock != INVALID_SOCKET) {
				ret = ::closesocket(m_sock);
				m_sock = INVALID_SOCKET;
			}
			return ret;
		};

		// �`�����M
		int send_to(const char* to_addr, short port, const char* buf, int len) {
			assert(m_sock != INVALID_SOCKET);

			sockaddr_in to;
			memset(&to, 0, sizeof(to));
			to.sin_family = AF_INET;
			to.sin_addr.S_un.S_addr = kjm::util::get_ipaddr(to_addr);
			to.sin_port = htons(port);

			return sendto(m_sock, buf, len, 0, (sockaddr*)&to, sizeof(to));
		};
	};
#endif

	/*!
	 *	�X���b�h�Ǘ��N���X
	 */

	/*
		-- �g���� --

		1. thread ��h������

		class my_thread : public thread { ... }

		2. �������z�֐� worker() �� end() ���`����
		   �X���b�h���I�����邽�߂̋�̓I�ȏ����́A�h���N���X���s���B
		   �g�p���鑤�́A my_thread::end() ���Ăׂ΁A�X���b�h���I������B

		class my_thread : public thread {
		private:
			bool m_loop;	// �I�����邽�߂̃t���O(worker���Ō���)
		public:
			my_thread() : m_loop(true) {};	// �R���X�g���N�^
			virtual ~my_thread() {};		// �f�X�g���N�^
			unsigned worker() {
				while (m_loop) {
					Sleep(200);
					// TODO: ����������
				}
				return 0;
			};
			void end() { m_loop = false; };
		};

		3. �X���b�h�̊J�n�ƏI��

		// �J�n
		my_thread myThrd;	// �C���X�^���X��
		myThrd.run();		// �X���b�h�̎��s

		// �I��
		myThrd.end();		// �X���b�h�̏I��
		myThrd.wait(5000);	// �X���b�h�I���܂őҋ@(5�b)

		wait() ���\�b�h�ŃX���b�h�n���h�������̂ŁA�i���I�ȃC���X�^���X���g�p���āA
		��{�I�ɂ��̏��ԂŌĂяo���B
	*/

	class thread {
	private:
		HANDLE m_hThread;
		CRITICAL_SECTION m_cs;

	public:
		thread() : m_hThread( NULL ) {
			InitializeCriticalSection( &m_cs );
		};
		virtual ~thread() {
			DeleteCriticalSection( &m_cs );
		};

		void lock() { EnterCriticalSection( &m_cs ); };
		void unlock() { LeaveCriticalSection( &m_cs ); };

		//! �X���b�h�����쒆���ǂ������ׂ�
		bool still_active() { 
			DWORD code;
			GetExitCodeThread( m_hThread, &code );
			return ( code == STILL_ACTIVE );
		};

		//! �X���b�h���N������B
		void run() {
			m_hThread = ( HANDLE )_beginthreadex( NULL, 0, thread_func, this, 0, NULL );
		};

		/*!
		 * �X���b�h���I��������B�h���N���X�Œ�`����B
		 * �I�����@�́Aworker method �̎����Ɉˑ�����B
		 */
		virtual void end() = 0;

		//! �X���b�h�̈ꎞ��~
		DWORD suspend() {
			return SuspendThread( m_hThread );
		};

		//! �X���b�h�̍ĊJ
		DWORD resume() {
			return ResumeThread( m_hThread );
		};

		/*!
		 * �X���b�h�ɌĂяo�����֐��B�h���N���X�Œ�`����B
		 * end method�̌Ăяo���ŏI���ł���悤�Ɏ�������B
		 */
		virtual unsigned worker() = 0;

		//---------------------------------------------------------------------
		// �X���b�h�̏I����҂B
		//---------------------------------------------------------------------
		DWORD wait(DWORD timeout = INFINITE) {
			DWORD ret = 0;
			if (m_hThread != NULL) {
				ret = WaitForSingleObject(m_hThread, timeout);
				CloseHandle(m_hThread);
				m_hThread = NULL;
			}
			return ret;
		}

		/*!
		 * ���ʂ̃X���b�h�֐��B
		 * args�ɂ́Athread�N���X����h�������N���X�I�u�W�F�N�g�ւ�
		 * �|�C���^���󂯎��Bp->worker() �����s���邾���B
		 */
		static unsigned __stdcall thread_func( void* args ) {
			return ( ( thread* )args )->worker();
		};
	};

	//-------------------------------------------------------------------------
	// �C�x���g���O�ɃA�N�Z�X���邽�߂̃N���X
	//
	// ���݂̂Ƃ���A�o�b�N�A�b�v�̂��߂̃��\�b�h�����Ȃ����߁A�C���X�^���X��
	// �ł���悤�ɂȂ��Ă��܂���B
	//-------------------------------------------------------------------------
	class event_log {
	private:
		// �f�t�H���g�R���X�g���N�^
		event_log() {}

		// �f�X�g���N�^
		~event_log() {}

	public:
		// �C�x���g���O���o�b�N�A�b�v����
		//
		// �o�b�N�A�b�v��̃t�H���_���Ȃ��ꍇ�A�G���[�ɂȂ�܂��B
		// �o�b�N�A�b�v��ɓ����t�@�C��������ꍇ�A�G���[�ɂȂ�܂��B
		static BOOL backup(LPCTSTR sourceName, LPCTSTR backupFileName);

	};

	//-------------------------------------------------------------------------
	// �v���Z�X�̋N���Ɋւ���N���X
	//-------------------------------------------------------------------------
	class process {
	public:
		// �f�t�H���g �R���X�g���N�^
		process();

		// �f�X�g���N�^
		virtual ~process() { this->Close(); }

	public:
		// �X�^�[�g�A�b�v�����N���A����
		void ClearSI() { memset(&m_si, 0, sizeof(m_si)); }

		// �X�^�[�g�A�b�v���(wShowWindow)���擾����
		WORD GetSIShowWindow() const { return m_si.wShowWindow; }

		// �X�^�[�g�A�b�v���(wShowWindow)��ݒ肷��
		void SetSIShowWindow(WORD newShowWindow);

		// �X�^�[�g�A�b�v���(wShowWindow)���N���A(���g�p��)����
		void ClearSIShowWindow();

		// �v���Z�XID�擾
		DWORD GetProcessId() const { return m_pi.dwProcessId; }

		// ���s�t�@�C���̃R�}���h���C���擾
		LPCTSTR GetCommandLine() const { return m_szCommandLine; }

	public:
		// �v���Z�X�̎��s
		virtual BOOL createProcess(const std::basic_string<TCHAR>& commandLine);

		// �v���Z�X�n���h�������B�v���Z�X�̏I���ł͂Ȃ��̂Œ��ӁB
		// �v���Z�X�̏I���́A�v���Z�X�Ԃ̎�茈�߂ɂ��B
		void Close();

		// �v���Z�X�̏I����҂�
		virtual DWORD WaitForTermination(DWORD dwMilliseconds = INFINITE) { return ::WaitForSingleObject(m_pi.hProcess, dwMilliseconds); }

		// �v���Z�X�̋N��������҂�
		virtual DWORD waitForInputIdle(DWORD dwMilliseconds = INFINITE) { return ::WaitForInputIdle(m_pi.hProcess, dwMilliseconds); }

		// �v���Z�X�Ɋ֘A�t����ꂽ�E�B���h�E��Ԃ�
		HWND GetProcessWindow();

	private:
		HWND m_hwnd;				// GetProcessWindow�֐��Ŏg�p

		STARTUPINFO m_si;					// �X�^�[�g�A�b�v���
		PROCESS_INFORMATION m_pi;			// �v���Z�X���
		TCHAR m_szCommandLine[MAX_PATH];	// ���s�R�}���h���C���L�^�p
											// Create�֐����s��L��
		
		static BOOL CALLBACK process_EnumWindowsProc(HWND hwnd, LPARAM lParam);
	};
}

#endif
