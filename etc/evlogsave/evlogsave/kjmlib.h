//---------------------------------------------------------------------
// kjmlib : Version 0.2011.8.31
//
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

#pragma warning(disable : 4786)
#pragma warning(disable : 4996)

#if !defined(__KJMLIB_H)
#define __KJMLIB_H

#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#endif

#if !defined(_WINSOCKAPI_)
// winsock.h ���C���N���[�h����Ă���Ƃ��́Awinsock2.h �́A�C���N���[�h���܂���B
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#include <windows.h>
#include <tchar.h>
#include <shellapi.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <lm.h>
#pragma comment(lib, "netapi32.lib")

#include <shlobj.h>
// kjm::util::createShortcut�Ŏg�p����.

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <assert.h>
#include <process.h>
#include <time.h>

#if defined(USE_WSSLOG)
#include "wsslog.h"
// #pragma comment(lib, "wsslog.lib")
#endif

#if !defined(countof)
#define countof(A)	(sizeof((A)) / sizeof((A)[0]))
#endif

// WinMain �g�p���� __argv ���g��������}�N��
#if defined(UNICODE)
#define __targv		__wargv
#else
#define __targv		__argv
#endif

namespace kjm {

	// TCHAR���g�p����������N���X
	typedef std::basic_string<TCHAR>	_tstring;

	//-------------------------------------------------------------------------
	// kjmlib �Ŏg���ėp��O
	//-------------------------------------------------------------------------
	class kjmException {
	public:
		kjmException(DWORD errorCode, const std::basic_string<TCHAR>& msg) : m_errorCode(errorCode), m_msg(msg) {}
		kjmException(const kjmException& src) : m_errorCode(src.m_errorCode), m_msg(src.m_msg) {}
		virtual ~kjmException() {}

	public:
		DWORD m_errorCode;
		std::basic_string<TCHAR> m_msg;

	private:
		kjmException() {}
	};

	void setVerboseMode(bool mode);
	bool getVerboseMode();

	// OutputDebugString ���ȒP�Ɏg�����߂̊֐�
	void ods(LPCTSTR format, ...);

	void log_info(LPCTSTR signature, LPCTSTR format, ...);	// ��񃍃O���c���֐�
	void log_error(LPCTSTR signature, LPCTSTR format, ...);	// �G���[���O���c���֐�

	BOOL create_directory(LPCTSTR lpPathName);	// �ċA�I�Ƀf�B���N�g�����쐬�ł��� CreateDirectory �֐�
	
	//-------------------------------------------------------------------------
	// CSV �t�@�C���������N���X
	//-------------------------------------------------------------------------
	class csvFile {
	public:
		// �R���X�g���N�^
		csvFile() {}

		// �f�X�g���N�^
		virtual ~csvFile() { this->close(); }

		// CSV �t�@�C�����J��
		bool open(LPCTSTR pszFileName);

		// CSV �t�@�C�������
		void csvFile::close();

		// CSV �t�@�C������ 1 �s�ǂݍ���
		int parseCsvCol(std::vector<std::basic_string<TCHAR> >& cols);

	private:
		std::basic_ifstream<TCHAR> m_fr;
	};

	//-------------------------------------------------------------------------
	// �p�X��������������
	//-------------------------------------------------------------------------
	class PathString {
	private:
		std::basic_string<TCHAR> m_path;

	public:
		// �R���X�g���N�^
		PathString() : m_path() {};
		PathString( const PathString& src ) : m_path( src.m_path ) {};
		PathString( LPCTSTR src ) : m_path( src ) {};
		PathString(const std::basic_string<TCHAR>& src) : m_path(src) {};

		// �f�X�g���N�^
		virtual ~PathString() {};

	public:
		// ������ւ� const pointer ��߂�
		LPCTSTR c_str() const { return m_path.c_str(); };

		// �����������Ԃ�
		std::basic_string<TCHAR> getString() const { return m_path; }

		// ������Ƀp�X����������
		PathString& Append( const PathString& more ) {
			std::vector<TCHAR> buf( MAX_PATH );
			
			::lstrcpy(&buf[0], m_path.c_str() );
			::PathAppend(&buf[0], more.c_str() );

			m_path = &buf[0];

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

	//-------------------------------------------------------------------------
	// netapi32.lib �Œ񋟂����֐��̃��b�p�[
	// lm.h �̃C���N���[�h�ƁAnetapi32.lib �������N
	//-------------------------------------------------------------------------
	class netapi32 {
	private:
		netapi32() {}
		~netapi32() {}

	public:
		// ���[�N�O���[�v�����擾����
		static std::basic_string<TCHAR> getWorkgroupName();
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
		static std::basic_string<TCHAR> expand_environment_strings(LPCTSTR lpSrc) {
			DWORD dwSizeNeed;

			// �K�v�ȃo�b�t�@�T�C�Y���擾
			dwSizeNeed = ::ExpandEnvironmentStrings(lpSrc, NULL, 0);
			if (dwSizeNeed == 0) {
				return std::basic_string<TCHAR>();	// �G���[
			}

			// �K�v�ȃo�b�t�@���m�ۂ��āA�Ď��s
			std::vector<TCHAR> buf(dwSizeNeed);
			dwSizeNeed = ::ExpandEnvironmentStrings(lpSrc, &buf[0], buf.size());
			if (dwSizeNeed != buf.size() - 1) {
				return std::basic_string<TCHAR>();	// �G���[
			}

			return std::basic_string<TCHAR>(&buf[0]);	// �W�J���ꂽ������
		};

		static std::basic_string<TCHAR> format_message(DWORD dwErrorCode);	// �G���[�R�[�h����V�X�e���G���[���b�Z�[�W���擾����B
		static std::basic_string<TCHAR> str_replace(const std::basic_string<TCHAR>& src, LPCTSTR pFind, LPCTSTR pReplace);	// �������u������BVB��StrReplace�̂悤�Ȋ֐��B
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
		std::basic_string<TCHAR> get_string(LPTSTR lpValueName, LPCTSTR lpDefault) const;

	public:
		// �A�N�Z�T
		HKEY getKey() { return m_hkey; };
		LONG RegOpenKeyEx( HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired );
		LONG RegCloseKey();
		LONG RegQueryValueEx( LPCTSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, std::vector<BYTE>& data );

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
		// PathRenameExtension�̃��b�p�[
		static std::basic_string<TCHAR>& pathRenameExtension(std::basic_string<TCHAR>& strPath, LPCTSTR pszExt);

		//---------------------------------------------------------------------
		// Unicode ������� ANSI ������ɕϊ�
		//---------------------------------------------------------------------
		static std::string toAnsi(const std::wstring& src);

		// ANSI ������� Unicode ������ɕϊ�
		static std::wstring toUnicode(const std::string& src);

		// ������I�[�̉��s����菜��
		static std::basic_string<TCHAR>& chomp(std::basic_string<TCHAR>& s);

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

		// �V�X�e���W���̃G���[���b�Z�[�W���쐬
		static std::basic_string<TCHAR> formatMessageBySystem(DWORD dwErr);

		// �ėp�I�ȃt�H�[�}�b�g���b�Z�[�W�̏���
		static DWORD formatMessage( DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, std::vector<TCHAR>& buffer, va_list* Arguments );

#if 0	// �s�v�֐�
		// �v���Z�X�̃��W���[�������擾(GetModuleFileName �֐��̃��b�p�[)
		///static std::basic_string<TCHAR> getModuleFileName();
#endif

		// Win32API GetModuleFileName �����S�Ɏg���֐�
		static std::basic_string<TCHAR> getModuleFileName(HMODULE hModule = NULL);

		// string ��Ԃ� GetComputerName �֐�
		static std::basic_string<TCHAR> getComputerName();

		// �v���Z�X����string�Ŏ擾����֐�
		static std::basic_string<TCHAR> GetModuleName() {
			return std::basic_string<TCHAR>(::PathFindFileName(kjm::util::getModuleFileName().c_str()));
		}

		// ���ϐ���W�J����
		static DWORD expandEnvironmentStrings(LPCTSTR lpSrc, std::vector<TCHAR>& rDst);

		// ini �t�@�C�����當������擾����(GetPrivateProfileString �֐��̃��b�p�[)
		static std::basic_string<TCHAR> getPrivateProfileString(const std::basic_string<TCHAR>& section,
			const std::basic_string<TCHAR>& key, const std::basic_string<TCHAR>& defValue, const std::basic_string<TCHAR>& iniFile);

		static DWORD get_ipaddr(LPCTSTR lpName);	// �z�X�g���܂��́AIP�����񂩂�IP�A�h���X���擾

		// �f�B���N�g�����ċA�I�ɍ폜����
		static bool remove_folder(LPCTSTR pszPath);

		// �������Ԃ����S�� sprintf �֐�
		static std::basic_string<TCHAR> sprintf_str(LPCTSTR format, ...);

		// �t�@�C�����������擾
		static std::basic_string<TCHAR> findFileName(const std::basic_string<TCHAR>& path);

		// �p�X������Ƀp�X��ǉ�
		static std::basic_string<TCHAR> append(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& more);

		// �p�X������̏I�[ \ ���폜����
		static std::basic_string<TCHAR> removeBackslash(const std::basic_string<TCHAR>& path);

		// �p�X������̊g���q��ύX����
		static std::basic_string<TCHAR> renameExtension(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& ext);

		// �p�X������̃t�@�C������ύX����
		static std::basic_string<TCHAR> renameFileSpec(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& fname);

		// �p�X������̃t�@�C�����������폜����
		static std::basic_string<TCHAR> removeFileSpec(const std::basic_string<TCHAR>& path);

		// �f�B���N�g�����ǂ������ׂ�
		static bool isDirectory(const std::basic_string<TCHAR>& path) { return (PathIsDirectory(path.c_str())) ? true : false; }

		// �t�@�C���̗L�����m�F����
		static bool fileExists(const std::basic_string<TCHAR>& path);

		// �J�����g�f�B���N�g����ύX����
		static bool setCurrentDirectory(const std::basic_string<TCHAR>& pathName);
		
		// �V���[�g�J�b�g�̍쐬
		//
		// strFileName : �����N�t�@�C���̖��O
		// strFile : �����N���쐬����t�@�C���̖��O
		// strArgs : �����N�ɓn������
		// strDir : ��ƃf�B���N�g��
		//
		static bool createShortcut(
			const std::basic_string<TCHAR>& strFileName,
			const std::basic_string<TCHAR>& strFile,
			const std::basic_string<TCHAR>& strArgs, 
			const std::basic_string<TCHAR>& strDir);

		// �X�y�V�����t�H���_���擾����
		// ���Ȃ��Ƃ��Avs2008����L���Ȋ֐�
#if _WIN32_IE >= 0x0700
		static std::basic_string<TCHAR> getSpecialFolderLocation(int nFolder);
#endif

		// �����𕶎���ɕϊ�
		static std::basic_string<TCHAR> to_a(int value, int radix = 10);

		// �����𕶎���ɕϊ�(printf�����̃t�H�[�}�b�g���w��ł���)
		static std::basic_string<TCHAR> to_a(int value, const std::basic_string<TCHAR>& format);

		// �����t�@�C�����폜�o�� DeleteFile 
		static bool deleteFiles(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildcard);

		// %TEMP%�t�H���_���擾����
		static std::basic_string<TCHAR> getTempPath();

		// �w��t�H���_�ȉ��̓ǂݍ��ݐ�p�������O��
		static bool UnsetReadOnly(const std::basic_string<TCHAR>& path);

		// �w��̋��L�t�H���_�Ƌ��L�p�X�̑��݂�ۏ؂���
		static int guaranteeSharedFolder(LPCTSTR pszSharedFolder, LPCTSTR pszSharedPath);

		// ���S�� vsprintf
		static int vsprintf_vec(std::vector<TCHAR>& buf, LPCTSTR format, va_list argptr);

		// ���S�� sprintf 
		//
		// [����]
		// ��Ԗڂ̈������Avector<TCHAR>�ւ̎Q�ƂɂȂ��Ă���ȊO�A
		// sprintf�֐��Ɠ����ł��B
		static int sprintf_vec(std::vector<TCHAR>& buf, LPCTSTR format, ...);

		// ������̕���
		static std::vector<std::basic_string<TCHAR> > split(const std::basic_string<TCHAR>& src, TCHAR c);

		// ������ɕϊ�
		static std::basic_string<TCHAR> toString(DWORD value);

		// ������̔�r(�啶���E�������𖳎����Ĕ�r)
		static bool equalsIgnoreCase(const std::basic_string<TCHAR>& lhs, const std::basic_string<TCHAR>& rhs);
	};

	//-----------------------------------------------------------------
	// ���Ԃ������N���X
	//-----------------------------------------------------------------
	class time {
	private:
		time_t m_time;

	public:
		time() : m_time(0) {}
		time(time_t t) : m_time(t) {}
		time(LPCTSTR time_text) { parse(time_text); }
		virtual ~time() {}

		// �ێ�������t���Actime �֐��ŕ�����ɂ��ĕԂ��B
		std::basic_string<TCHAR> ctime() {
			return kjm::util::chomp(std::basic_string<TCHAR>(::_tctime(&m_time)));
		}

		bool is_empty() { return (m_time == 0); }

		int year() const { return localtime( &m_time )->tm_year + 1900; }
		int month() const { return localtime( &m_time )->tm_mon + 1; }
		int date() const { return localtime( &m_time )->tm_mday; }
		int hour() const { return localtime( &m_time )->tm_hour; }
		int minute() const { return localtime( &m_time )->tm_min; }
		int second() const { return localtime( &m_time )->tm_sec; }
		int weekday() const { return localtime( &m_time )->tm_wday; }

		time& add_second(int sec);

		time_t get_time() const { return m_time; }
		std::basic_string<TCHAR> strftime(LPCTSTR format);

		void parse(LPCTSTR time_text );

		void operator=( time_t t ) { m_time = t; }
		void operator=( LPCTSTR time_text ) { parse( time_text ); }

		double operator-(const time& rhs) const { return difftime(m_time, rhs.m_time); }
		bool operator==( const time& rhs ) const { return m_time == rhs.m_time; }
		bool operator!=( const time& rhs ) const { return !( *this == rhs ); }
		bool operator<( const time& rhs ) const { return m_time < rhs.m_time; }
		bool operator>( const time& rhs ) const { return rhs < *this; }
		bool operator<=( const time& rhs ) const { return !( rhs < *this ); }
		bool operator>=( const time& rhs ) const { return !( *this < rhs ); }

		bool save(FILE* fp);
		bool load(FILE* fp);


	public:

		// ���ݓ�����ێ�����C���X�^���X��Ԃ��B
		static kjm::time GetCurrentTime() {
			return kjm::time(::time(NULL));
		}
	};

	//-------------------------------------------------------------------------
	// �����������N���X
	//-------------------------------------------------------------------------
	class timeSpan {
	public:
		// �f�t�H���g�R���X�g���N�^
		timeSpan() : m_span(0) {}

		// �R�s�[�R���X�g���N�^
		timeSpan(const timeSpan& src) : m_span(src.m_span) {}

		// __int64�ŏ���������R���X�g���N�^
		timeSpan(__int64 src) : m_span(src) {}

		// �f�X�g���N�^
		~timeSpan() {}

		// ������Z�q
		timeSpan& operator =(const timeSpan& rhs);

		// �~���b�ŕԂ�
		double totalMilliseconds() {
			__int64 result = m_span;			// original (100 nsec)

			result = result / (__int64)10;		// to usec
			result = result / (__int64)1000;	// to msec

			return (double)result;
		}

		// �b���ŕԂ�
		double totalSeconds() { return this->totalMilliseconds() / 1000.0; }

		// ���ŕԂ�
		double totalMinutes() { return this->totalSeconds() / 60.0; }

		// ���ԂŕԂ�
		double totalHours() { return this->totalMinutes() / 60.0; }

		// �����ŕԂ�
		double totalDays() { return this->totalHours() / 24.0; }

	private:
		__int64 m_span;
	};

#define DTK_UNSPECIFIED	0	// DateTimeKind.Unspecified ����
#define DTK_UTC			1	// DateTimeKind.Utc ����
#define DTK_LOCAL		2	// DateTimeKind.Local ����

	//-------------------------------------------------------------------------
	// ���Ԃ������N���X
	//-------------------------------------------------------------------------
	class dateTime {
	public:
		// �f�t�H���g�R���X�g���N�^
		dateTime() : m_kind(DTK_UNSPECIFIED) { memset(&m_ft, 0, sizeof(m_ft)); }

		// �R�s�[�R���X�g���N�^
		dateTime(const dateTime& src) : m_ft(src.m_ft), m_kind(src.m_kind) {}

		// FILETIME�ŏ���������R���X�g���N�^
		dateTime(FILETIME src, int kind = DTK_UNSPECIFIED) : m_ft(src), m_kind(kind) {}

		// �f�X�g���N�^
		virtual ~dateTime() {}

		// ������Z�q
		dateTime& operator=(const dateTime& rhs);

		// �������l�� int64 �Ŏ擾
		__int64 get_Value() const { return *(__int64*)&m_ft; }

		// dateTime �̈����Z
		timeSpan operator -(const dateTime& rhs) { return kjm::timeSpan(get_Value() - rhs.get_Value()); }

		// ���[�J�����ԂɕύX
		dateTime toLocalTime();

		// ���t���t�H�[�}�b�g���ďo��
		_tstring toLongDateString();

		// ���Ԃ��t�H�[�}�b�g���ďo��
		_tstring toLongTimeString();

	public:
		// ���[�J�����ݓ���������dateTime�I�u�W�F�N�g�𐶐�
		static dateTime now();

		// UTC���ݓ��������� dateTime �I�u�W�F�N�g�𐶐�
		static dateTime utcNow();

	private:
		FILETIME m_ft;
		int m_kind;		// DTK_UNSPECIFIED, DTK_UTC, DTK_LOCAL
	};

	//-------------------------------------------------------------------------
	// �t�@�C���������X�^�e�B�b�N�N���X
	//-------------------------------------------------------------------------
	class file {
	public:
		// �ŏI�X�V�������擾(���[�J������)
		static dateTime getLastWriteTime(const _tstring& path) { return getLastWriteTimeUtc(path).toLocalTime(); }

		// �ŏI�X�V�������擾(UTC)
		static dateTime getLastWriteTimeUtc(const _tstring& path);

	private:
		file() {}
		~file() {}
	};

	//=========================================================================
	// �f�B���N�g����T�u�f�B���N�g����ʂ��āA�쐬�A�ړ��A����ї񋓂��邽�߂�
	// �ÓI���\�b�h�����J���܂��B
	//=========================================================================
	class directory {
	public:
		// �t�@�C���܂��̓f�B���N�g���A����т��̓��e��V�����ꏊ�ɃR�s�[���܂��B
		static BOOL copy(LPCTSTR sourceDirName, LPCTSTR destDirName);

		// �t�@�C����񋓂��܂��B
		static std::vector<std::basic_string<TCHAR> > getFiles(
			const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildCard = _T("*.*"));

		// �f�B���N�g����񋓂��܂��B
		static std::vector<std::basic_string<TCHAR> > getSubFolders(
			const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildCard = _T("*.*"));

	private:
		directory() {};
		~directory() {};
	};

	//-------------------------------------------------------------------------
	// ���݂̊�����уv���b�g�t�H�[���Ɋւ�����A����т����𑀍삷���
	// �i��񋟂��܂��B(.NET�̕����I����)
	//-------------------------------------------------------------------------
	class environment {
	public:

		// ���[�J�� �R���s���[�^�� NetBIOS �����擾���܂��B
		static _tstring get_MachineName() { return kjm::util::getComputerName(); }

	private:
		environment() {}
		~environment() {}
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
		BOOL Add(HWND hWnd, UINT uID, UINT uFlags, UINT uCallbackMessage, HICON hIcon, LPCTSTR pszTip);

		// �^�X�N�g���C����A�C�R�����폜����
		BOOL Delete();

		// �^�X�N�g���C�A�C�R���̏���ύX����B
		BOOL Modify(UINT uFlags, UINT uCallbackMessage, HICON hIcon, LPCTSTR pszTip);
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
		mutex(const std::basic_string<TCHAR>& name) : m_hMutex(NULL), m_create_status(0), m_lock_count(0) {
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
		bool create(const std::basic_string<TCHAR>& name) {
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

	//-----------------------------------------------------------------
	// ���C�u�������W���[�����Ǘ�����N���X
	//-----------------------------------------------------------------
	class library {
	private:
		HINSTANCE m_hLibModule;

	public:
		// �A�N�Z�T
		HINSTANCE getInstance() { return m_hLibModule; };

	public:
		library() : m_hLibModule( NULL ) {};
		virtual ~library() { FreeLibrary(); };

	public:
		BOOL LoadLibraryEx( LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags );
		BOOL FreeLibrary();
	};

	// lockfile���ȒP�Ɉ����N���X
	//
	class lockfile {
	private:
		HANDLE m_hFile;			// ���b�N�t�@�C���ւ̃n���h��
		std::basic_string<TCHAR> m_fullname;	// ���b�N�t�@�C���ւ̃t���p�X��

	public:
		// �R���X�g���N�^�E�f�X�g���N�^
		//
		lockfile(const std::basic_string<TCHAR>& fullname) : m_hFile(INVALID_HANDLE_VALUE), m_fullname(fullname) {};
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
		static void backup(const kjm::_tstring& sourceName, const kjm::_tstring& backupFileName);

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

		// �v���Z�X�̏I���R�[�h���擾
		DWORD getExitCode();

		// �v���Z�X�n���h�����L�����ǂ������ׂ�
		bool IsHandleAvailable() { return (m_pi.hThread != NULL && m_pi.hProcess != NULL) ? true : false; }

		// �v���Z�X�����쒆���ǂ������ׂ�
		bool IsStillActive() { return (IsHandleAvailable() && (getExitCode() == STILL_ACTIVE)) ? true : false; }

	public:
		// �v���Z�X�̎��s
		virtual BOOL createProcess(const std::basic_string<TCHAR>& commandLine);

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

	private:
		HWND m_hwnd;				// GetProcessWindow�֐��Ŏg�p

		STARTUPINFO m_si;					// �X�^�[�g�A�b�v���
		PROCESS_INFORMATION m_pi;			// �v���Z�X���
		TCHAR m_szCommandLine[MAX_PATH];	// ���s�R�}���h���C���L�^�p
											// Create�֐����s��L��
		
		static BOOL CALLBACK process_EnumWindowsProc(HWND hwnd, LPARAM lParam);
	};

	//-------------------------------------------------------------------------
	// ��{�\�P�b�g�N���X
	//-------------------------------------------------------------------------
	class baseSocket {
	public:
		// sockaddr_in �����������邽�߂֗̕��֐�
		// name �̃A�h���X��Ԃ��܂��B
		static void initSockaddrIn(sockaddr_in& name, const std::basic_string<TCHAR>& host, short port);

	public:
		// �R���X�g���N�^
		baseSocket() : m_sock(INVALID_SOCKET) {}

		// �f�X�g���N�^
		virtual ~baseSocket() { closeSocket(); }

		// �\�P�b�g���J��
		bool openSocket(int af, int type, int protocol);

		// �ڑ�
		bool connectSocket(const sockaddr_in& name);

		// ��M�^�C���A�E�g�̐ݒ�
		bool setRecvTimeout(int msTimeout);

		// �o�C���h����(�|�[�g�ԍ����w�肷��ȒP�o�[�W������������)
		bool bindSocket(unsigned short port);

		// ��M����
		int recvfrom(std::vector<char>& buf, sockaddr_in& from);

		// ��M����
		int recv(std::vector<char>& buf);

		// ��M����
		void recvAll(std::vector<char>& buf);

		// �S�Ď�M���������f����֐�
		virtual bool isRecvAll(const std::vector<char>& buf);

		// ���M����
		int sendto(const std::vector<char>& buf, const sockaddr_in& to);

		// ���M����
		int send(const char* pStr);

		// �ؒf����
		int shutdown(int how);

		// �\�P�b�g�����
		bool closeSocket();

		// �D��ɐؒf����
		void gracefulShutdown();

	public:
		// Winsock������
		static int startup();

		// Winsock��n��
		static int cleanup() { return ::WSACleanup(); }

	private:
		SOCKET m_sock;
		int m_lastError;
	};

	//-------------------------------------------------------------------------
	// UDP�\�P�b�g�N���X
	//-------------------------------------------------------------------------
	class udpSocket : public kjm::baseSocket {
	public:
		// �R���X�g���N�^
		udpSocket() {}

		// �f�X�g���N�^
		virtual ~udpSocket() {}

		// �\�P�b�g���J��
		bool openSocket() { return baseSocket::openSocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); }
	};

	//-------------------------------------------------------------------------
	// TCP�\�P�b�g�N���X
	//-------------------------------------------------------------------------
	class tcpSocket : public kjm::baseSocket {
	public:
		// �R���X�g���N�^
		tcpSocket() {}

		// �f�X�g���N�^
		virtual ~tcpSocket() {}

		// �\�P�b�g���J��
		bool openSocket() { return baseSocket::openSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP); }
	};

	//-------------------------------------------------------------------------
	// �R�}���h���C����͂ɓn���I�v�V�������\����
	//-------------------------------------------------------------------------
	static const int no_argument = 0;
	static const int optional_argument = 1;
	static const int required_argument = 2;

	struct optionInfo {
		std::basic_string<TCHAR> m_longOption;	// �����O�I�v�V����������(���g�p)
		std::basic_string<TCHAR> m_shortOption;	// �V���[�g�I�v�V����������
		int m_optionArgInfo;					// 0 = �I�v�V�����Ȃ�, 1 = �I�v�V�������Ƃ肤��, 2 = �I�v�V�������K�{

		// �f�t�H���g�R���X�g���N�^
		optionInfo() : m_longOption(), m_shortOption(), m_optionArgInfo(kjm::no_argument) {}

		// ���������R���X�g���N�^
		optionInfo(const std::basic_string<TCHAR>& longOption, const std::basic_string<TCHAR>& shortOption, int optionArgInfo)
			: m_longOption(longOption), m_shortOption(shortOption), m_optionArgInfo(optionArgInfo) {}

		// �R�s�[�R���X�g���N�^
		optionInfo(const kjm::optionInfo& src)
			: m_longOption(src.m_longOption), m_shortOption(src.m_shortOption), m_optionArgInfo(src.m_optionArgInfo) {}

		// ������Z�q
		optionInfo& operator=(const kjm::optionInfo& rhs);

		// �V���[�g�I�v�V�������������ǂ������ׂ�(�Q�ƃo�[�W����)
		bool isSameShortOption(const std::basic_string<TCHAR>& name) const { return (m_shortOption == name); }

		// �����O�I�v�V�������������ǂ������ׂ�(�Q�ƃo�[�W����)
		bool isSameLongOption(const std::basic_string<TCHAR>& name) const { return (m_longOption == name); }

#if _MSC_VER <= 1500	// VC++2008
		// �V���[�g�I�v�V�������������ǂ������ׂ�(�|�C���^�o�[�W����)
		bool isSameShortOption_P(const std::basic_string<TCHAR>* name) const { return (m_shortOption == *name); }

		// �����O�I�v�V�������������ǂ������ׂ�(�|�C���^�o�[�W����)
		bool isSameLongOption_P(const std::basic_string<TCHAR>* name) const { return (m_longOption == *name); }
#endif
	};

	typedef std::vector<kjm::optionInfo>	optionInfoList;

	//-------------------------------------------------------------------------
	// �R�}���h���C����̓N���X
	//-------------------------------------------------------------------------
	class cmdLine {
	public:
		// �f�t�H���g�R���X�g���N�^
		cmdLine() {}

		// �f�X�g���N�^
		virtual ~cmdLine() {}

		// �R�}���h���C�����
		virtual int parse(int argc, TCHAR** argv, const optionInfoList& opts);

		// �R�}���h���C�����
		virtual int parse(int argc, TCHAR** argv) { return parse(argc, argv, optionInfoList()); }

	public:
		// �I�v�V���������邩�ǂ������ׂ�
		bool hasOption(const std::basic_string<TCHAR>& key) { return (m_options.find(key) != m_options.end()) ? true : false; }

		// �I�v�V�����̈������擾����
		std::basic_string<TCHAR> getOptArgument(const std::basic_string<TCHAR>& key);

		// �I�v�V�����ȊO�̈����̐����擾����
		int get_argCount() { return this->m_arguments.size(); }

		// N �Ԗڂ̃I�v�V�����ȊO�̈������擾����
		std::basic_string<TCHAR> get_argument(int n) { return this->m_arguments.at(n); }

	private:
		// �I�v�V�����Ƃ��̈���
		std::map<std::basic_string<TCHAR>, std::basic_string<TCHAR> > m_options;

		// �I�v�V�����ȊO�̈���
		std::vector<std::basic_string<TCHAR> > m_arguments;
	};

	//-----------------------------------------------------------------
	// �C�x���g���O��1���R�[�h�����Ǘ�����N���X
	//-----------------------------------------------------------------
	class eventLogRecord {
	public:
		// ���R�[�h�ԍ�
		DWORD RecordNumber() { return getRaw()->RecordNumber; }

		kjm::time TimeGenerated() { return kjm::time( getRaw()->TimeGenerated ); }

		WORD EventCategory() {
			EVENTLOGRECORD* p = getRaw();
			return getRaw()->EventCategory;
		}

		std::basic_string<TCHAR> EventCategoryText() {
			return DispMessage(get_SourceName().c_str(), _T("CategoryMessageFile"), std::vector<std::basic_string<TCHAR> >(), EventCategory()); };

		std::basic_string<TCHAR> EventIDText();

		DWORD DataLength() { return getRaw()->DataLength; }
		DWORD DataOffset() { return getRaw()->DataOffset; }

		std::basic_string<TCHAR> ComputerName();

		std::vector<BYTE> Data();

		// �C�x���g�̓��t�������擾
		kjm::time get_TimeWritten() { return kjm::time( getRaw()->TimeWritten ); };

		// �C�x���g�^�C�v(���)���擾
		// @retval EVENTLOG_ERROR_TYPE, EVENTLOG_WARNING_TYPE, EVENTLOG_INFORMATION_TYPE, EVENTLOG_AUDIT_SUCCESS, EVENTLOG_AUDIT_FAILURE �̂ǂꂩ
		WORD get_EventType() { return getRaw()->EventType; };

		// �C�x���g�\�[�X���擾
		std::basic_string<TCHAR> get_SourceName() { return (LPCTSTR)(&m_raw_record[0] + sizeof(EVENTLOGRECORD)); }

		// �C�x���gID���擾
		DWORD get_EventID() { return getRaw()->EventID; }

	public:
		// �f�t�H���g�R���X�g���N�^
		eventLogRecord() {};

		// �������t���R���X�g���N�^
		eventLogRecord(LPCTSTR lpSourceName, const EVENTLOGRECORD* src);

		eventLogRecord( const eventLogRecord& src )
			: m_source_name( src.m_source_name ), m_raw_record( src.m_raw_record ) {};
		virtual ~eventLogRecord() {};

		kjm::eventLogRecord& operator= ( const kjm::eventLogRecord& src );

	private:
		std::basic_string<TCHAR> m_source_name;		// �C�x���g�\�[�X
		std::vector<BYTE> m_raw_record;	// �����
		
		// �������L���X�g����
		EVENTLOGRECORD* getRaw() { return (EVENTLOGRECORD*)&m_raw_record[0]; }

		std::basic_string<TCHAR> DispMessage(
			const std::basic_string<TCHAR>& SourceName, LPCTSTR EntryName, const std::vector<std::basic_string<TCHAR> >& strings, DWORD MessageId);

		int GetArgs(const EVENTLOGRECORD *pBuf, std::vector<std::basic_string<TCHAR> >& args );

		BOOL GetModuleNameFromSourceName(LPCTSTR SourceName, LPCTSTR EntryName, std::vector<TCHAR>& ExpandedName);
	};

	//-----------------------------------------------------------------
	// �C�x���g���O���Ǘ�����N���X
	//-----------------------------------------------------------------
	class eventLog {
	public:
		// �f�t�H���g�R���X�g���N�^
		eventLog() : m_hEventLog(NULL) {}

		// �f�X�g���N�^
		virtual ~eventLog() { closeEventLog(); }

	public:
		// �C�x���g���O���J��(OpenEventLog API�Q��)
		//
		// lpSourceName�ɂ͒ʏ�A"Application", "Security", "System"�̂ǂꂩ���w�肷��B
		BOOL openEventLog(LPCTSTR lpUNCServerName, LPCTSTR lpSourceName);

		// �C�x���g���O���J��(OpenEventLog API�Q��)
		BOOL openEventLog(LPCTSTR lpSourceName) { return openEventLog(NULL, lpSourceName); }

		// �A�v���P�[�V�����C�x���g���O���J��
		BOOL openApplicationEventLog(LPCTSTR lpUNCServerName = NULL) { return openEventLog(lpUNCServerName, _T("Application")); }

		// �V�X�e���C�x���g���O���J��
		BOOL openSystemEventLog(LPCTSTR lpUNCServerName = NULL) { return openEventLog(lpUNCServerName, _T("System")); }

		BOOL OpenBackupEventLog( LPCTSTR lpUNCServerName, LPCTSTR lpSourceName, LPCTSTR lpFileName );

		// �C�x���g���O�����
		BOOL closeEventLog();

		// �C�x���g��1���R�[�h�ǂݍ���
		BOOL readEventLog(DWORD dwReadFlags, DWORD dwRecordOffset, eventLogRecord& record);
		
		// �C�x���g���O���o�b�N�A�b�v����
		BOOL BackupEventLog( LPCTSTR lpBackupFileName ) {
			return ::BackupEventLog( m_hEventLog, lpBackupFileName );
		};

		// �C�x���g���O���o�b�N�A�b�v����
		// �ُ펞�́Akjm::kjmException �𔭐����܂��B
		static void backup(LPCTSTR sourceName, LPCTSTR backupFileName);

	private:
		// �R�s�[�R���X�g���N�^(�R�s�[�֎~)
		eventLog(const eventLog& src) {}

		// ������Z�q(����֎~)
		eventLog& operator=(const eventLog& rhs) { return *this; }

	private:
		HANDLE m_hEventLog;
		std::basic_string<TCHAR> m_uncServerName;
		std::basic_string<TCHAR> m_sourceName;
		std::basic_string<TCHAR> m_fileName;
	};

	//-------------------------------------------------------------------------
	// ini�t�@�C������������N���X
	//-------------------------------------------------------------------------
	class iniFile {
	public:
		// �R���X�g���N�^
		iniFile() : m_fileName() {};

		// �R�s�[�R���X�g���N�^
		iniFile(const iniFile& src) : m_fileName(src.m_fileName) {};

		// �t�@�C������^����R���X�g���N�^
		iniFile(const std::basic_string<TCHAR>& szFileName) : m_fileName(szFileName) {};

		// �f�X�g���N�^
		virtual ~iniFile() {};

		// ������Z�q
		kjm::iniFile& operator=(const iniFile& rhs) {
			if (this == &rhs) return *this;
			this->m_fileName = rhs.m_fileName;
			return *this;
		};

		// �Z�N�V�����ꗗ���擾
		std::vector<std::basic_string<TCHAR> > getSectionNames() const;

		// �Z�N�V�������̃L�[�ꗗ���擾
		std::vector<std::basic_string<TCHAR> > getKeyNames(const std::basic_string<TCHAR>& strSectionName) const;

		// ini�t�@�C������l���擾
		std::basic_string<TCHAR> getString(const std::basic_string<TCHAR>& strSection,
			const std::basic_string<TCHAR>& strKey, const std::basic_string<TCHAR>& strDefault = _T(""), BOOL* pfUseDefault = NULL) const;

		// ini�t�@�C���ɒl����������
		BOOL writeString(const std::basic_string<TCHAR>& strSection, const std::basic_string<TCHAR>& strKey, const std::basic_string<TCHAR>& strValue);

		// �ʂ�ini�t�@�C���̓��e���}�[�W����
		void mergeIniFile(const iniFile& newIniFile);

		// ini�t�@�C�������擾
		std::basic_string<TCHAR> get_fileName() const { return m_fileName; };

		// ini�t�@�C������ݒ�
		void set_fileName(const std::basic_string<TCHAR>& value) { m_fileName = value; };

	private:
		std::basic_string<TCHAR> m_fileName;	// ini�t�@�C���̖��O
	};

#if defined(_MT)
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
		DWORD wait( DWORD timeout = INFINITE ) {
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
#endif
}

#endif
