//---------------------------------------------------------------------
// kjmlib : Version 0.2015.12.8
//
// �v���R���p�C���ς݃w�b�_���g�p���Ȃ��悤�ɐݒ肵�Ă��������B
//
// r.kojima
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
#pragma comment(lib, "version.lib")

#include <lm.h>
#pragma comment(lib, "netapi32.lib")

// kjm::util::createShortcut�Ŏg�p����.
#include <shlobj.h>

// kjm::odbcUtil�Ŏg�p����w�b�_
#if defined(USE_ODBC32)
#include <sql.h>
#include <sqlext.h>
#pragma comment(lib, "odbc32.lib")
#endif

#include <string>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <iomanip>

// vs2010����Fback_inserter���g�����߂ɃC���N���[�h���K�v
#include <iterator>

#include <assert.h>
#include <process.h>
#include <time.h>
#include <share.h>
#include <locale.h>
#include <sys/stat.h>
#include <io.h>
#include <fcntl.h>

#include "option.h"

#if defined(USE_WSSLOG)
#include "wsslog.h"
// #pragma comment(lib, "wsslog.lib")
#else
#define LOGLV_INFO	1
#define LOGLV_WARNING	2
#define LOGLV_ERR	3
void Winssa_Log(LPCTSTR ident, int level, LPCTSTR format, ...);
void WLDeleteOldLog();
#endif


#if defined(USE_MSXML6)
#import <msxml6.dll> named_guids
#endif


// Windows �� DOS ����Unicode���o�͂ł���悤�ɂ���B
// Windows ���̏��X�̏��������A������Ȃǂ��\���ł���悤�ɂȂ�֗�
// printf�n�Acout�n�ǂ���ɂ��L��
// MBCS�̎��͉������Ȃ������ɂȂ�B
//
// !!!! �������A�o�͌��ʂ�Unicde�ɂȂ�̂ŁA
// !!!! ���_�C���N�g���č쐬�����t�@�C����Unicode �ɂȂ�B
// !!!! �R�}���h���C���R�}���h�Ƃ��Ďg���Ȃ�A
// !!!! ���ɂ���uSTL_SETLOCALE_JAPAN�v�̕��������₷���B
//
#if defined(UNICODE)
#define SET_UNICODE_MODE	{ \
	_setmode(_fileno(stdout), _O_U16TEXT); \
	_setmode(_fileno(stderr), _O_U16TEXT); \
}
#else
#define SET_UNICODE_MODE
#endif

// STL �� setlocale(LC_ALL, _T("japan")) ���������́A�ȉ��̒ʂ�B
// std::locale �R���X�g���N�^�͏�ɁA_MBCS��������󂯎��B
#define STL_SETLOCALE_JAPAN		std::locale::global(std::locale("japanese"))
#define C_SETLOCALE_JAPAN		_tsetlocale(LC_ALL, _T("japanese"))

// ���{�ꃍ�P�[�����x�[�X�ɐ��l�J�e�S���t�@�Z�b�g����C���P�[���ɒu��������Ƃ�����肩���B
///#define STL_SETLOCALE_JAPAN		{ std::locale loc(std::locale("japanese"), "C", std::locale::numeric); std::locale::global(loc); }

// �������Astd::locale::global(std::locale("japanese")) �őS�̂���{��ɂ��Ă���ƁA
// 2,010/10/15 �݂����ȕ����񂪏o���オ���Ă��܂��B
// �����������邽�߂ɁA***.imbue(std::locale("C")) ����K�v������B
// _otstringstream oss;
// oss.imbue(std::locale("C"));

#if !defined(countof)
#define countof(A)	(sizeof((A)) / sizeof((A)[0]))
#endif

// WinMain �g�p���� __argv ���g��������}�N��
// !!! tchar.h �ɒ�`����Ă���悤�Ȃ̂ŁA������̃R�����g�A�E�g !!! <2012/06/01>
#if !defined(_INC_TCHAR)
#if defined(UNICODE)
#define __targv		__wargv
#else
#define __targv		__argv
#endif
#endif

// WinMain �� int WINAPI tWinMain(HINSTANCE, HINSTANCE LPTSTR, int) �Ə�����悤�ɂ��邽�߂̃}�N��
// !!! tchar.h �� _tWinMain ������̂ŁA���������g�����B
#if defined(UNICODE)
#define tWinMain	wWinMain
#else
#define tWinMain	WinMain
#endif

// __FILE__ ���A��� MBCS �Ȃ̂ŁA_T__FILE__ �� UNICODE �Ɨ����ł���悤�ɂ��邽�߂̃}�N��
#define	WIDEN2(x)	L ## x
#define	WIDEN(x)	WIDEN2(x)

#if defined(UNICODE)
#define _T__FILE__	WIDEN(__FILE__)
#else
#define _T__FILE__	__FILE__
#endif

#if defined(UNICODE)
#define _tout				std::wcout
#define _tcout				std::wcout
#define _terr				std::wcerr
#define _tcerr				std::wcerr
#define _otstream			std::wostream
#define _otstringstream		std::wostringstream
#define _tifstream			std::wifstream
#define _tstringstream		std::wstringstream
#else
#define _tout				std::cout
#define _tcout				std::cout
#define _terr				std::cerr
#define _tcerr				std::cerr
#define _otstream			std::ostream
#define _otstringstream		std::ostringstream
#define _tifstream			std::ifstream
#define _tstringstream		std::stringstream
#endif

// __int64 �� iostream �ɏo�͂��邽�߂̊֐�
#if _MSC_VER < 1500		// ���Ȃ��Ƃ��Avs2008����́A__int64���󂯎��ostream�����݂���̂ŁA�����ɂ���B
_otstream& operator<<(_otstream& os, __int64 i);
#endif


#if _MSC_VER <= 1200	// VC6 ���傫��
#define INVALID_FILE_ATTRIBUTES	0xffffffff
#endif

// A <= X <= B �𔻒肷��}�N��
#define InRANGE(A, X, B)	(((A) <= (X)) && ((X) <= (B)))


#include "kjmbase.h"
#include "path.h"

namespace kjm {


	class systemException {
	public:
		systemException(DWORD code, const _tstring& message) : m_code(code), m_message(message) {};
		virtual ~systemException() {};

		// �G���[�R�[�h���擾
		DWORD getCode() { return m_code; };

		// �G���[���b�Z�[�W���擾
		_tstring getMessage() { return m_message; };

	private:
		DWORD m_code;
		_tstring m_message;

	private:
		systemException() {};
	};

	//-----------------------------------------------------------------
	// �֐����Ŕ�������Win32�G���[���O�Ƃ��ė����N���X
	//-----------------------------------------------------------------
	class Win32Exception {
	public:
		Win32Exception(DWORD dwError) : m_dwError(dwError) {};
		virtual ~Win32Exception() {};

	public:
		DWORD m_dwError;
	};

	//-------------------------------------------------------------------------
	// ���̃l�[���X�y�[�X�Ŏg����A��ʓI�ȗ�O�^
	//-------------------------------------------------------------------------
	class exception {
	public:
		exception() {};
		exception(DWORD errCode, const _tstring& errMsg) : m_errCode(errCode), m_errMsg(errMsg) {}
		virtual ~exception() {}

	public:
		DWORD m_errCode;
		_tstring m_errMsg;
	};

	//-------------------------------------------------------------------------
	// �����̌`���������ł���ꍇ�̗�O
	//-------------------------------------------------------------------------
	class formatException : public kjm::exception {
	public:
		formatException() : kjm::exception(1, _T("���t�Ǝ����̗L���ȕ�����`�����i�[���Ă��܂���B")) {}
		virtual ~formatException() {}
	};

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
	
	//=================================================================
	// �����R�[�h�Ɋւ��鏈��
	//=================================================================

	class encoding {
	private:
		encoding() {}
		~encoding() {}

	public:
		// ��ʓI�ȃ��P�[����ݒ肷��֐�
		static void setLocale() {
			_tsetlocale(LC_ALL, _T("Japanese"));
		}

		// Unicode��ascii�ɕϊ�
		// !defined(_UNICODE)�̎��Ɏg����֐�
		static std::string to_mbs(const std::string& src);

		// Unicode��ascii�ɕϊ�
		// defined(_UNICODE)�̎��Ɏg����֐�
		static std::string to_mbs(const std::wstring& src);
	};

	//-------------------------------------------------------------------------
	// base64 encode���s���N���X
	//-------------------------------------------------------------------------
	class base64 {
	public:
		static std::string encode(const std::vector<char>& data);
		static std::vector<char> decode(const std::string& str);
	};

	//-------------------------------------------------------------------------
	// ��ǉ����s���N���X
	//-------------------------------------------------------------------------
	class crypt {
	public:
		static std::string decrypt(const std::string& str);
		static std::string encrypt(const std::string& str);
	};

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

	//-----------------------------------------------------------------
	// �N���e�B�J���Z�N�V�������Ǘ�����N���X
	//-----------------------------------------------------------------
	class CriticalSection : public CRITICAL_SECTION {
	public:
		//-------------------------------------------------------------
		// �R���X�g���N�^(�N���e�B�J���Z�N�V�����̏����������˂�)
		//-------------------------------------------------------------
		CriticalSection() {
			::InitializeCriticalSection(this);
		};

		//-------------------------------------------------------------
		// �f�X�g���N�^
		//-------------------------------------------------------------
		~CriticalSection() {
			::DeleteCriticalSection(this);
		};

		//-------------------------------------------------------------
		// ���L���̎擾
		//-------------------------------------------------------------
		VOID Enter() {
			::EnterCriticalSection(this);
		};

		//-------------------------------------------------------------
		// ���L���̔j��
		//-------------------------------------------------------------
		VOID Leave() {
			::LeaveCriticalSection(this);
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

		// ���[�N�G���A�̒��S�ɃE�B���h�E���ړ�������B
		static BOOL MoveCenterWorkarea(HWND hwnd);
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

		// RegEnumValue �̃��b�p�[
		LONG reg_enum_value(DWORD dwIndex, kjm::_tstring& valueName, DWORD& type, std::vector<BYTE>& data);

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
		// CopyFileEx �̊ȒP�����B
		static BOOL copyFileEx(LPCTSTR lpExistingFileName,
								  LPCTSTR lpNewFileName,
								  LPPROGRESS_ROUTINE lpProgressRoutine,
								  LPVOID lpData,
								  LPBOOL pbCancel,
								  DWORD dwCopyFlags);

		// �������u������BVB��StrReplace�̂悤�Ȋ֐��B
		static _tstring strReplace(const _tstring& src, const _tstring& strFind, const _tstring& strReplace);

		// �o�b�t�@����CRC16���v�Z����
		static unsigned short calcCRC16(unsigned char *p, unsigned int n);

		// �t�@�C���n���h������CRC16���v�Z����
		static unsigned short calcCRC16(FILE* fp);

		// �t�@�C��������CRC16���v�Z����
		static unsigned short calcCRC16(const _tstring& pfname);

		// PathRenameExtension�̃��b�p�[
		static _tstring &pathRenameExtension(_tstring &strPath, LPCTSTR pszExt);

		// Unicode ������� ANSI ������ɕϊ�
		static std::string toAnsi(const std::wstring& src);

		// ANSI ������� Unicode ������ɕϊ�
		static std::wstring toUnicode(const std::string& src);

		// Unicode ������� ANSI ������ɕϊ�
		static std::string toAnsi(const std::wstring& src, int cp);

		// ANSI ������� Unicode ������ɕϊ�
		static std::wstring toUnicode(const std::string& src, int cp);

		// ������I�[�̉��s����菜��
		static std::basic_string<TCHAR>& chomp(std::basic_string<TCHAR>& s);

		// �R�s�[���̒����ɂ��邷�ׂẴt�@�C�����R�s�[��ɃR�s�[����
		//
		// [����]
		// pszSrcPath  �R�s�[���t�H���_�̖��O
		// pszDstPath  �R�s�[��t�H���_�̖��O
		//
		// [�߂�l]
		// ����I��: NO_ERROR
		// �G���[�I��: Win32�G���[�R�[�h
		static DWORD CopyFiles(LPCTSTR pszSrcPath, LPCTSTR pszDstPath);

		// �V�X�e���W���̃G���[���b�Z�[�W���쐬
		static std::basic_string<TCHAR> formatMessageBySystem(DWORD dwErr);

		// �ėp�I�ȃt�H�[�}�b�g���b�Z�[�W�̏���
		static DWORD formatMessage( DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, std::vector<TCHAR>& buffer, va_list* Arguments );

		// 1�s�ǂݍ��݊֐�
		//
		// @return 0 ����I��
		// @return 1 eof�œǂݍ��ݏI��
		// @erturn -1 error�œǂݍ��ݏI��
		static int getline(FILE *fp, kjm::_tstring &str);

		// Win32API GetModuleFileName �����S�Ɏg���֐�
		static _tstring getModuleFileName(HMODULE hModule = NULL);

		// ���W���[���t�@�C���Ɠ�����ini�t�@�C�����擾����
		static kjm::_tstring getModuleIniFile(HMODULE hModule = NULL) {
			return kjm::util::pathRenameExtension(kjm::util::getModuleFileName(), _T(".ini"));
		}

		// ���W���[�������݂���t�H���_�����擾����
		static kjm::_tstring getModulePath(HMODULE hModule = NULL);

		// string ��Ԃ� GetComputerName �֐�
		static std::basic_string<TCHAR> getComputerName();

		// �v���Z�X����string�Ŏ擾����֐�
		static std::basic_string<TCHAR> GetModuleName() {
			return std::basic_string<TCHAR>(::PathFindFileName(kjm::util::getModuleFileName().c_str()));
		}

		// ���ϐ���W�J����
		static DWORD expandEnvironmentStrings(LPCTSTR lpSrc, std::vector<TCHAR>& rDst);

		// ���ϐ��̒l���擾����
		static _tstring getEnvironmentVariable(LPCTSTR lpName);

		// �t�@�C���̃o�[�W�������\�[�X���擾
		static _tstring getFileVersion(const _tstring &strFileName);

		// �t�@�C���̃o�[�W�������\�[�X���擾
		static _tstring getFileVersion() {
			return kjm::util::getFileVersion(kjm::util::getModuleFileName());
		}

		// ini �t�@�C�����當������擾����(GetPrivateProfileString �֐��̃��b�p�[)
		static std::basic_string<TCHAR> getPrivateProfileString(const std::basic_string<TCHAR>& section,
			const std::basic_string<TCHAR>& key, const std::basic_string<TCHAR>& defValue, const std::basic_string<TCHAR>& iniFile);
		static _tstring get_private_profile_string(const _tstring& lpSectionName, const _tstring& lpKeyName, const _tstring& lpDefault, const _tstring& lpFileName) {
			return kjm::util::getPrivateProfileString(lpSectionName, lpKeyName, lpDefault, lpFileName);
		}

		static DWORD get_ipaddr(LPCTSTR lpName);	// �z�X�g���܂��́AIP�����񂩂�IP�A�h���X���擾

		// �f�B���N�g�����ċA�I�ɍ폜����
		static bool remove_folder(LPCTSTR pszPath);

		// �f�B���N�g�����ċA�I�ɍ폜����
		static void removeFolder(const _tstring& pszPath);

		// �������Ԃ����S�� sprintf �֐�
		static std::basic_string<TCHAR> sprintf_str(LPCTSTR format, ...);

		// �t�@�C�����������擾
		static std::basic_string<TCHAR> findFileName(const std::basic_string<TCHAR>& path);

		// �p�X������Ƀp�X��ǉ�
		static std::basic_string<TCHAR> append(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& more);
		static std::basic_string<TCHAR> path_append(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& more) {
			return kjm::util::append(path, more);
		}

		// �p�X������̏I�[ \ ���폜����
		static std::basic_string<TCHAR> removeBackslash(const std::basic_string<TCHAR>& path);

		// �p�X������̊g���q���폜����
		static std::basic_string<TCHAR> removeExtension(const std::basic_string<TCHAR>& path);

		// �p�X������̊g���q��ύX����
		static std::basic_string<TCHAR> renameExtension(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& ext);

		// ���W���[���̃p�X������̊g���q��ύX����
		static std::basic_string<TCHAR> renameExtension(const std::basic_string<TCHAR>& ext, HMODULE hModule = NULL);

		// �p�X������̃t�@�C������ύX����
		static std::basic_string<TCHAR> renameFileSpec(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& fname);

		// �p�X������̃t�@�C�����������폜����
		static std::basic_string<TCHAR> removeFileSpec(const std::basic_string<TCHAR>& path);

		// �f�B���N�g�����ǂ������ׂ�
		static bool isDirectory(const std::basic_string<TCHAR>& path) { return (PathIsDirectory(path.c_str())) ? true : false; }

		// LoadString �֐��̃��b�p�[
		static _tstring loadString(UINT uID);
		static _tstring load_string(UINT uID, HINSTANCE hInstance = NULL);

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
		static bool deleteFiles(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildcard, int nDays = 0);

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

		// GetFullPathName API�̃��b�p�[
		static int getFullPathName(const kjm::_tstring& fileName, kjm::_tstring& pathPart, kjm::_tstring& filePart);

		// ���l�� human readable format �ȕ�����ɂ���
		static _tstring toHumanReadable(__int64 value);

		// �R�s�[������ CopyFile API ���g���B
		// �R�s�[��̓Ǎ������𗎂Ƃ��@�\�t���B
		//
		// [�߂�l]  1   ����I��(�R�s�[��̓ǂݍ��ݑ����𗎂Ƃ���)
		// [�߂�l]  0   ����I��
		// [�߂�l] -1   CopyFile API�̎��s
		// [�߂�l] -12  �R�s�[��t�@�C���̑����擾���s
		// [�߂�l] -13  �R�s�[��t�@�C���̓ǂݍ��ݑ������Z�b�g���s
		static int copyFile2(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists, BOOL bFailIfReadonly=TRUE);

		// �������������������
		static _tstring toLower(const _tstring& src);

		// �������啶��������
		static _tstring toUpper(const _tstring& src);

		// ������̑O������A�󔒁A�^�u�A���s��������菜��
		static kjm::_tstring trimLeft(const kjm::_tstring &s);

		// ������O��́A�󔒁A�^�u�A���s��������菜��
		static kjm::_tstring trim(const kjm::_tstring &s);

		// _MSC_VER ����Ή�����R���p�C���̖��O���擾
		static _tstring getMscVerText(int mscVer = _MSC_VER);

		// ���O�̔��Ȃ��t�H���_���쐬
		static kjm::_tstring makeTempFolder(const kjm::_tstring& basePath, const kjm::_tstring& prefix);


		static std::string toString(const std::vector<char>& data);
		static std::wstring toString(const std::vector<wchar_t>& data);
		static std::string toAnsiString(std::vector<BYTE>& data);
		static std::wstring toUnicodeString(std::vector<BYTE>& data);
		static _tstring get_environment_variable(const _tstring& lpName);
		static _tstring path_rename_extension(const _tstring& pszPath, const _tstring& pszExt);
		static _tstring get_module_filename(HMODULE hModule = NULL);

		//---------------------------------------------------------------------
		// pszPath�Ŏw�肵���f�B���N�g�����́A���ݓ�������nDay���Â��t�@�C��
		// ���폜���܂��B
		//
		// �폜�Ώۂ́ApszPath�����̃t�@�C�������ł��B
		//---------------------------------------------------------------------
		static void cleanup_dir(const _tstring& pszPath, int nDay);

		static std::basic_string<TCHAR> str_replace(
			const std::basic_string<TCHAR>& strSrc,			// �u����������������܂ޕ�����
			const std::basic_string<TCHAR>& strFind,		// �������镶����
			const std::basic_string<TCHAR>& strReplace) {	// �u�����镶����

			std::basic_string<TCHAR> result = strSrc;
			std::basic_string<TCHAR>::size_type pos = 0;
			while ((pos = result.find(strFind, pos)) != std::basic_string<TCHAR>::npos) {
				result.replace(pos, strFind.length(), strReplace);
			}
			return result;
		}

		//-------------------------------------------------------------
		// SHFileOperation �� FO_DELETE ���g�����t�@�C���̍폜
		//-------------------------------------------------------------
		static int FODelete(LPCTSTR lpPathName) {
			// SHFileOperation�̕�����́A\0\0�ŏI���K�v��������ۂ�
			TCHAR szFrom[MAX_PATH];
			szFrom[::lstrlen(::lstrcpy(szFrom, lpPathName)) + 1] = '\0';

			SHFILEOPSTRUCT fileOp;
			memset(&fileOp, 0, sizeof(fileOp));
			fileOp.wFunc = FO_DELETE;
			fileOp.pFrom = szFrom;
			fileOp.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;

			return ::SHFileOperation(&fileOp);
		};

		static BOOL MakeDirectory(LPCTSTR pszFolder) {
			if (PathIsRoot(pszFolder))	return TRUE;
			if (PathFileExists(pszFolder))	return TRUE;

			TCHAR szParent[MAX_PATH];
			PathRemoveFileSpec(lstrcpy(szParent, pszFolder));

			if (kjm::util::MakeDirectory(szParent)) {
				if (CreateDirectory(pszFolder, FALSE)) {
					return TRUE;
				}
			}

		#if defined(USE_WSSLOG)
			Winssa_Log(_T("KJMLIB"), LOGLV_ERR, _T("�t�H���_�쐬�ُ�: error %d: %s"), GetLastError(), pszFolder);
		#endif

			return FALSE;
		}
		//-------------------------------------------------------------
		// �e���|�����p�̃f�B���N�g���p�X���擾���܂��B
		//-------------------------------------------------------------
		static std::basic_string<TCHAR> get_temp_path() {
			std::vector<TCHAR> work(MAX_PATH);
			DWORD dwRet = ::GetTempPath(work.size(), &work[0]);
			return &work[0];
		}
	};

	//-------------------------------------------------------------------------
	// �p�X��������������
	//-------------------------------------------------------------------------
	class PathString {
	private:
		std::basic_string<TCHAR> m_path;

	public:
		// �R���X�g���N�^
		PathString() : m_path() {}
		PathString( const PathString& src ) : m_path( src.m_path ) {}
		PathString( LPCTSTR src ) : m_path( src ) {}
		PathString(const std::basic_string<TCHAR>& src) : m_path(src) {}

		// �f�X�g���N�^
		virtual ~PathString() {};

	public:
		// ������ւ� const pointer ��߂�
		LPCTSTR c_str() const { return m_path.c_str(); }

		// �����������Ԃ�
		std::basic_string<TCHAR> getString() const { return m_path; }

		// ������Ƀp�X����������
		PathString& Append(const PathString& more);

		// �g���q����菜��
		PathString& removeExtension();

		// �g���q��ύX����
		PathString& renameExtension(const kjm::_tstring& newExt);

		// PathRemoveFileSpec �̃��b�p�[
		PathString& RemoveFileSpec() {
			std::vector<TCHAR> work(MAX_PATH);
			::PathRemoveFileSpec(lstrcpy(&work[0], m_path.c_str()));
			m_path = &work[0];
			return *this;
		}

		// const char* �ւ̃L���X�g
		operator const TCHAR *() { return m_path.c_str(); };

		//-------------------------------------------------------------
		// ���[�g�f�B���N�g�����擾����
		//-------------------------------------------------------------
		std::basic_string<TCHAR> GetRoot() {
			std::vector<TCHAR> work(MAX_PATH);
			lstrcpy(&work[0], this->m_path.c_str());

			while (PathIsRoot(&work[0]) == FALSE) {
				if (!PathRemoveFileSpec(&work[0])) {
					return _T("");
				}
			}

			return &work[0];
		}
	public:
		// ������Z�q
		PathString& operator= (const PathString& src);
	};

	//-----------------------------------------------------------------
	// ���Ԃ������N���X
	//-----------------------------------------------------------------
	class time {
	private:
		time_t m_time;

		time_t filetime_to_timet(const FILETIME& ft) {
			ULARGE_INTEGER ull;
			ull.LowPart = ft.dwLowDateTime;
			ull.HighPart = ft.dwHighDateTime;
			return ull.QuadPart / 10000000ULL - 11644473600ULL;
		}

		void TimetToFileTime( time_t t, LPFILETIME pft )
		{
			LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000;
			pft->dwLowDateTime = (DWORD) ll;
			pft->dwHighDateTime = ll >>32;
		}

	public:
		time() : m_time(0) {}
		time(time_t t) : m_time(t) {}
		time(LPCTSTR time_text) { parse(time_text); }
		time(SYSTEMTIME src) {
			FILETIME ft;
			SystemTimeToFileTime(&src, &ft);
			m_time = filetime_to_timet(ft);
		}
		time(FILETIME src) {
			m_time = filetime_to_timet(src);
		}
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

		static kjm::time get_system_time() {
			SYSTEMTIME st;
			GetSystemTime(&st);
			return kjm::time(st);
		}

		static kjm::time get_local_time() {
			SYSTEMTIME st;
			GetLocalTime(&st);
			return kjm::time(st);
		}
		//-------------------------------------------------------------
		// ������ɕϊ����ĕԂ�
		//-------------------------------------------------------------
		std::basic_string<TCHAR> toString() {
			FILETIME gft;
			TimetToFileTime(m_time, &gft);

			FILETIME ft;
			SYSTEMTIME st;
			::FileTimeToLocalFileTime(&gft, &ft);
			::FileTimeToSystemTime(&ft, &st);
			
			TCHAR buf[512];
			wsprintf(buf, _T("%d�N%d��%d���A%d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

			return buf;
		};
	};

	//-------------------------------------------------------------------------
	// �����������N���X
	//-------------------------------------------------------------------------

// FILETIME �������₷�����Ԃɕϊ����邽�߂̒萔
#define FT_1USEC			(__int64)10
#define FT_1MSEC			(FT_1USEC * 1000)
#define FT_1SEC				(FT_1MSEC * 1000)
#define FT_1MIN				(FT_1SEC  *   60)	// FILETIME�^��1���̒l
#define FT_1HOUR			(FT_1MIN  *   60)	// FILETIME�^��1���Ԃ̒l
#define FT_1DAY				(FT_1HOUR *   24)	// FILETIME�^��1���̒l
#define FT_19700101			(116444736000000000)		// time_t �̊J�n�n�_��\��FILETIME�̒l
#define FT_19700101_SEC		(FT_19700101 / FT_1SEC)

	class timeSpan {
	public:
		static const long long TicksPerMillisecond = (long long)10000;
		static const long long TicksPerSecond = TicksPerMillisecond * (long long)1000;
		static const long long TicksPerMinute = TicksPerSecond * (long long)60;
		static const long long TicksPerHour = TicksPerMinute * (long long)60;
		static const long long TicksPerDay = TicksPerHour * (long long)24;

		static const long long TicksPer19700101 = (long long)116444736000000000;		// time_t�̊J�n�n�_��\��FILETIME�̒l
		static const long long TicksPerSec19700101 = TicksPer19700101 / TicksPerSecond;	// time_t�̊J�n�n�_��\���b�̒l

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

		// __int64�ɕϊ�
		operator __int64 () { return m_span; }

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

	/// <summary>kjm::dateTime �I�u�W�F�N�g���\�������̎�� (���n�����A���E���莞�� (UTC)�A�܂��́A���̂�����ł��Ȃ�) ���w�肵�܂��B</summary>
	enum dateTimeKind {
		unspecified = 0,	/// ���n�����Ɛ��E���莞�� (UTC) �̂�������w�肳��Ă��Ȃ����Ƃ�\���܂��B
		utc,				/// UTC �����ł��邱�Ƃ�\���܂��B
		local				/// ���n�����ł��邱�Ƃ�\���܂��B
	};

	//-------------------------------------------------------------------------
	// ���Ԃ������N���X
	//-------------------------------------------------------------------------
	class dateTime {
	private:
		FILETIME m_ft;
		SYSTEMTIME m_st;
		dateTimeKind m_kind;

	public:
		// �f�t�H���g�R���X�g���N�^
		dateTime() : m_ft(), m_kind(unspecified) {
			FileTimeToSystemTime(&m_ft, &m_st);
		}

		// �R�s�[�R���X�g���N�^
		dateTime(const dateTime& src) : m_ft(src.m_ft), m_st(src.m_st), m_kind(src.m_kind) {}

		// FILETIME�ŏ���������R���X�g���N�^
		dateTime(FILETIME src, dateTimeKind kind = unspecified) : m_ft(src), m_kind(kind) {
			FileTimeToSystemTime(&m_ft, &m_st);
		}

		// SYSTEMTIME �ŏ���������R���X�g���N�^
		dateTime(const SYSTEMTIME& st, dateTimeKind kind = unspecified ) : m_kind(kind) {
			SystemTimeToFileTime(&st, &m_ft);
			FileTimeToSystemTime(&m_ft, &m_st);
		}

		// �N�A���A���A���A���A�b�ŏ���������R���X�g���N�^
		dateTime(int year, int month, int day, int hour, int minute, int second) : m_kind(unspecified) {
			SYSTEMTIME st = {year, month, 0, day, hour, minute, second, 0};
			SystemTimeToFileTime(&st, &m_ft);
			FileTimeToSystemTime(&m_ft, &m_st);
		}

		// �f�X�g���N�^
		virtual ~dateTime() {}

		// �������l�� int64 �Ŏ擾
		__int64 get_Value() const { return *(__int64*)&m_ft; }
		
		// ���ݎ�����__int64�ŕԂ�
		__int64 getTime() const { return *(__int64*)&m_ft; }

		// �������l�� FILETIME �ŕԂ�
		FILETIME get_FILETIME_Value() const { return m_ft; }

		// ���[�J�����ԂɕύX
		dateTime toLocalTime() const {
			if (m_kind == local) return *this;
			FILETIME ftLocal;
			FileTimeToLocalFileTime(&m_ft, &ftLocal);
			return kjm::dateTime(ftLocal, local);
		}
		
		// UTC�ɕϊ�
		dateTime toUniversalTime() const {
			if (m_kind == utc) return *this;
			FILETIME ftUTC;
			LocalFileTimeToFileTime(&m_ft, &ftUTC);
			return kjm::dateTime(ftUTC, utc);
		}

		// ���t���t�H�[�}�b�g���ďo��
		_tstring toLongDateString() const;

		// ���Ԃ��t�H�[�}�b�g���ďo��
		_tstring toLongTimeString() const;

		// �������t�H�[�}�b�g���ďo��
		_tstring toString(const _tstring& format) const;

		// �������t�H�[�}�b�g���ďo��
		_tstring toString() const;

		// ���Ԃ𕶎���(YYYYMMDD)�ɕϊ�
		virtual _tstring toString_yyyymmdd() const;

		// ���t�𑫂��� dateTime ��Ԃ�
		dateTime addDays(int nDays) const;

		// ������Z�q
		dateTime& operator=(const dateTime& rhs) {
			if (this == &rhs) return *this;
			m_ft = rhs.m_ft;
			m_st = rhs.m_st;
			m_kind = rhs.m_kind;
			return *this;
		}

		// dateTime �̈����Z
		timeSpan operator -(const dateTime& rhs) { return kjm::timeSpan(get_Value() - rhs.get_Value()); }
		
		// ��r���Z�q
		bool operator==( const dateTime& rhs ) const { return getTime() == rhs.getTime(); };
		
		// ��r���Z�q
		bool operator!=( const dateTime& rhs ) const { return !( *this == rhs ); };
		
		// ��r���Z�q
		bool operator<( const dateTime& rhs ) const { return getTime() < rhs.getTime(); };
		
		// ��r���Z�q
		bool operator>( const dateTime& rhs ) const { return rhs < *this; };
		
		// ��r���Z�q
		bool operator<=( const dateTime& rhs ) const { return !( rhs < *this ); };
		
		// ��r���Z�q
		bool operator>=( const dateTime& rhs ) const { return !( *this < rhs ); };

		// SYSTEMTIME �ւ̃L���X�g
		operator SYSTEMTIME() const { return this->m_st; }

		// time_t �ւ̃L���X�g
		// time�֐��Ŗ߂���鎞�Ԃ́AUTC�Ȃ̂ŁAUTC��Ԃ��悤�ɂ���
		operator time_t() const {
			FILETIME ft = toUniversalTime().m_ft;;
			ULARGE_INTEGER ull;
			ull.LowPart = ft.dwLowDateTime;
			ull.HighPart = ft.dwHighDateTime;
			return ull.QuadPart / 10000000ULL - 11644473600ULL;
		}

		// �����擾
		int get_Minute() const {
			return m_st.wMinute;
		}

		// �����擾
		int get_Hour() const {
			return m_st.wHour;
		}

		// �����擾
		int get_Day() const {
			return m_st.wDay;
		}
	
		// �����擾
		int get_Month() const {
			return m_st.wMonth;
		}

		// �j�����擾
		int get_Week() const {
			return m_st.wDayOfWeek;
		}

		// ���̃C���X�^���X�̒l�ɁA�w�肳�ꂽ�b�������Z�����V���� dateTime ��Ԃ��܂��B
		dateTime addSeconds(LONGLONG value) const {
			LONGLONG ll = get_Value() + (kjm::timeSpan::TicksPerSecond * value);
			return dateTime(*(FILETIME*)&ll);
		}

	public:
		// ���݂̃��[�J���������擾����
		static dateTime getLocalTime() {
			SYSTEMTIME st;
			::GetLocalTime(&st);
			return dateTime(st, local);
		}

		// ���݂̃��[�J���������擾����
		static dateTime now() {
			return kjm::dateTime::getLocalTime();
		}

		// ���݂̃V�X�e������(UTC)���擾����
		static dateTime getSystemTime() {
			SYSTEMTIME st;
			GetSystemTime(&st);
			return dateTime(st, utc);
		}

		// ���݂̃V�X�e������(UTC)���擾����
		static dateTime utcNow() {
			return kjm::dateTime::getSystemTime();
		}

		// ������̓����Ɠ����� dateTime �I�u�W�F�N�g�𐶐�
		static dateTime parse(const kjm::_tstring& text);

		// �����������ɕϊ����Ă݂�
		static bool tryParse(const kjm::_tstring& text, dateTime& result);

		// �t�H�[�}�b�g�ɂ����ĉ�͂���֐� -- !!! �K�v�\���Ȏ����������Ă��܂��� !!!
		//
		// ��) dateTime::parseExact(_T("20150915171300"), _T("yyyyMMddHHmmss"));
		// ��) dateTime::parseExact(_T("2004/08/24 20:23:06"), _T("yyyy/MM/dd HH:mm:ss"));
		// ��) dateTime::parseExact(_T("2004�N08��24��20��23��06�b"), _T("yyyy�NMM��dd��HH��mm��ss�b"));
		//
		// yyyy = 4���̔N(�̂���)
		// MM = 2���̌�(�̂���)
		// dd = 2���̓�(�̂���)
		// HH = 2����24��(�̂���)
		// mm = 2���̕�(�̂���)
		// ss = 2���̕b(�̂���)
		//
		static dateTime parseExact(const std::basic_string<TCHAR>& s, const std::basic_string<TCHAR>& f) {
			TCHAR work[2] = {0};
			int _Y,_M,_D,_h,_m,_s;
			_Y = _M = _D = _h = _m = _s = 0;
			LPCTSTR pf = f.c_str(), ps = s.c_str();
			while (*pf != '\0') {
				if (*ps == '\0') throw formatException();
				if (*pf == 'y') {
					if (_istdigit(work[0] = *(ps++)) == 0) throw formatException();
					_Y = _Y * 10 + _ttoi(&work[0]);
				} else if (*pf == 'M') {
					if (_istdigit(work[0] = *(ps++)) == 0) throw formatException();
					_M = _M * 10 + _ttoi(&work[0]);
				} else if (*pf == 'd') {
					if (_istdigit(work[0] = *(ps++)) == 0) throw formatException();
					_D = _D * 10 + _ttoi(&work[0]);
				} else if (*pf == 'H') {
					if (_istdigit(work[0] = *(ps++)) == 0) throw formatException();
					_h = _h * 10 + _ttoi(&work[0]);
				} else if (*pf == 'm') {
					if (_istdigit(work[0] = *(ps++)) == 0) throw formatException();
					_m = _m * 10 + _ttoi(&work[0]);
				} else if (*pf == 's') {
					if (_istdigit(work[0] = *(ps++)) == 0) throw formatException();
					_s = _s * 10 + _ttoi(&work[0]);
				} else if (*pf != *(ps++)) {
					throw formatException();
				}
				++pf;
			}
			return dateTime(_Y, _M, _D, _h, _m, _s);
		}

		static bool tryParseExact(const std::basic_string<TCHAR>& s, const std::basic_string<TCHAR>& f, dateTime& result) {
			bool ret = true;
			try {
				result = kjm::dateTime::parseExact(s, f);
			} catch (exception ex) {
				ret = false;
			}
			return ret;
		}
	};

	//=====================================================================
	// FindFile�n�֐��̃��b�p�[
	//=====================================================================
	class FindFile {
	public:
		//-------------------------------------------------------------
		// �f�t�H���g �R���X�g���N�^
		//-------------------------------------------------------------
		FindFile() : m_hFindFile(INVALID_HANDLE_VALUE) {};

		//-------------------------------------------------------------
		// �f�X�g���N�^
		//-------------------------------------------------------------
		virtual ~FindFile() { Close(); };

		//-------------------------------------------------------------
		// FindFirstFile�֐��̃��b�p�[
		//-------------------------------------------------------------
		bool FindFirst(const std::basic_string<TCHAR>& lpPathName, const std::basic_string<TCHAR>& lpWildcard) {
			assert(m_hFindFile == INVALID_HANDLE_VALUE);

			// ������t�H���_��ۑ�
			lstrcpy(this->m_findPath, lpPathName.c_str());

			::PathAppend(::lstrcpy(m_fileName, lpPathName.c_str()), lpWildcard.c_str());
			//SetLastError(ERROR_SUCCESS);
			m_hFindFile = ::FindFirstFile(m_fileName, &m_findFileData);
			return (m_hFindFile != INVALID_HANDLE_VALUE);
		};

		//-------------------------------------------------------------
		// FindNextFile�֐��̃��b�p�[
		//-------------------------------------------------------------
		bool FindNext() { return (::FindNextFile(m_hFindFile, &m_findFileData) != FALSE); };

		//-------------------------------------------------------------
		// FindClose�֐��̃��b�p�[
		//-------------------------------------------------------------
		bool Close() {
			bool bResult = true;
			if (m_hFindFile != INVALID_HANDLE_VALUE) {
				bResult = (::FindClose(m_hFindFile) != FALSE);
				m_hFindFile = INVALID_HANDLE_VALUE;
			}
			return bResult;
		};

	private:
		//-------------------------------------------------------------
		// �R�s�[�Ƒ�����֎~
		//-------------------------------------------------------------
		FindFile(const FindFile& src) {};
		void operator=(const FindFile& rhs) {};

	public:
		//-------------------------------------------------------------
		// �����Ɏg���t���p�X�����擾
		//-------------------------------------------------------------
		std::basic_string<TCHAR> GetFindFileName() { return m_fileName; };

		//-------------------------------------------------------------
		// �t�@�C�����f�B���N�g�����ǂ������ׂ�
		//-------------------------------------------------------------
		bool IsDirectory() { return ((m_findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0); };

		//-------------------------------------------------------------
		// �t�@�C����"."�܂���".."���ǂ������ׂ�
		//-------------------------------------------------------------
		bool IsDots() { return (::lstrcmp(m_findFileData.cFileName, _T(".")) == 0) || (::lstrcmp(m_findFileData.cFileName, _T("..")) == 0); };

		//-------------------------------------------------------------
		// �t�@�C�������擾
		//-------------------------------------------------------------
		std::basic_string<TCHAR> GetFileName() { return m_findFileData.cFileName; };

		// ��΃p�X���擾
		std::basic_string<TCHAR> get_FullName() {
			return kjm::path::combine(m_findPath, m_findFileData.cFileName);
		}

		//-------------------------------------------------------------
		// �ŏI�X�V�����擾
		//-------------------------------------------------------------
		kjm::time get_lastWriteTime() { return kjm::time(this->m_findFileData.ftLastWriteTime); };

		//-------------------------------------------------------------
		// �t�@�C���̃T�C�Y���擾
		//-------------------------------------------------------------
		__int64 get_fileSize() {
			LARGE_INTEGER li;
			li.LowPart = this->m_findFileData.nFileSizeLow;
			li.HighPart = this->m_findFileData.nFileSizeHigh;
			return li.QuadPart;
		};

	private:
		TCHAR m_findPath[MAX_PATH];
		TCHAR m_fileName[MAX_PATH];
		HANDLE m_hFindFile;
		WIN32_FIND_DATA m_findFileData;
	};

	//-------------------------------------------------------------------------
	// C����� fopen �����S�Ɏg���N���X
	//-------------------------------------------------------------------------
	class cfile {
	public:
		// �f�t�H���g�R���X�g���N�^
		cfile() : m_fp(NULL) {}

		// �J���t���R���X�g���N�^
		cfile(const _tstring& fname, const _tstring& mode, int shflag = 0) : m_fp(NULL) {
			this->open(fname, mode, shflag);
		}

		// �f�X�g���N�^
		virtual ~cfile();

		// fopen�̑���
		//
		// shflag _SH_DENYNO=�ǂݎ�肨��я������݃A�N�Z�X�������܂��B
		//        _SH_DENYRD=�t�@�C���ւ̓ǂݎ��A�N�Z�X�����ۂ��܂��B
		//        _SH_DENYRW=�t�@�C���ւ̓ǂݎ�肨��я������݃A�N�Z�X�����ۂ��܂��B
		//        _SH_DENYWR=�t�@�C���ւ̏������݃A�N�Z�X�����ۂ��܂��B
		virtual bool open(const _tstring& fname, const _tstring& mode, int shflag = 0);

		// fclose�̑���
		virtual bool close();

		// �t�@�C�����J���Ă��邩�ǂ������ׂ�
		virtual bool is_open() const {
			return (m_fp != NULL);
		}

		// fseek�֐�
		// origin {SEEK_CUR, SEEK_END, SEEK_SET}
		virtual int fseek(__int64 offset, int origin) {
			return _fseeki64(m_fp, offset, origin);
		}

		// ��s�Ǎ�
		virtual int getline(_tstring& lineBuf) {
			return kjm::util::getline(m_fp, lineBuf);
		}

		// ��s������
		virtual void writeline(_tstring& lineBuf) {
			_fputts(lineBuf.c_str(), m_fp);
			_fputts(_T("\n"), m_fp);
		}

		// �w��o�C�g�Ǎ�
		template<class T>
		int read(std::vector<T>& buffer, int count = 1) {
			buffer.resize(count);
			return fread(&buffer[0], sizeof(T), count, m_fp);
		}

		// �t�@�C���n���h����Ԃ�
		virtual FILE* get_handle() const {
			return m_fp;
		}
	private:
		FILE* m_fp;
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

		// �t�@�C���̑������擾����B
		static DWORD getAttributes(const _tstring &path);

		// �t�@�C���̑�����ݒ肷��B
		static void setAttributes(const _tstring& path, DWORD fa);

		// �t�@�C�����폜����B
		static void deleteFile(const _tstring& path, bool force = false);

		// �t�@�C���̃T�C�Y���擾����
		static __int64 getFileSize(HANDLE hFile);

		static void copy(const _tstring& sourceFileName, const _tstring& destFileName, bool overwrite=false) {
			if (::CopyFile(sourceFileName.c_str(), destFileName.c_str(), (overwrite) ? FALSE : TRUE) == FALSE) {
				DWORD code = ::GetLastError();

				kjm::_tostringstream oss;
				oss << _T("kjm::file::copy('") << sourceFileName << _T("','") << destFileName << _T("',") << overwrite << _T("): ")
					<< _T("error ") << code << _T(" : ");

				throw kjm::systemException(code, oss.str());
			}
		}
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
		// �R�[���o�b�N�֐��ɃN���X��n�����߂� template �֐�
		// kjm::direcoty::copy �̈����Ƃ��Ď��̂悤�ɋL�q���܂�
		//
		// kjm::direcoty::copy(
		//		sourceDirName,
		//		destDirName,
		//		kjm::direcoty::is_cancel_callback<hoge, &hoge::callback_func>, this));
		template<typename T, bool (T::*F)()>
		static bool is_cancel_callback(void* parg) {
			T* obj = reinterpret_cast<T*>(parg);
			return (obj->*F)();
		}

		// �t�@�C���܂��̓f�B���N�g���A����т��̓��e��V�����ꏊ�ɃR�s�[���܂��B
		static BOOL copy(LPCTSTR sourceDirName, LPCTSTR destDirName, bool (*is_cancel_callback)(void*), void* callback_arg);

		static void copy(const _tstring& sourceDirName, const _tstring& destDirName, bool (*is_cancel_callback)(void*), void* callback_arg) {
			int i;
			DWORD dwRet;

			// �R�s�[��̃f�B���N�g�����쐬����
			dwRet = kjm::directory::createDirectory2(destDirName);
			if (dwRet != 0) {
				kjm::_tostringstream oss;
				oss << _T("kjm::directory::copy('") << sourceDirName << _T("','") << destDirName << _T("')")
					<< _T(": error ") << dwRet << _T(" : �f�B���N�g�� '") << destDirName << _T("' ���A�쐬�ł��܂���B");

				throw kjm::systemException(dwRet, oss.str());
			}

			// �t�@�C�����R�s�[����
			kjm::_tstrings files = kjm::directory::getFiles(sourceDirName);
			for (i = 0; i < (int)files.size(); i++) {
				if ((*is_cancel_callback)(callback_arg)) {
					break;
				}

				kjm::file::copy(
					files[i],
					kjm::path::combine(destDirName, kjm::path::getFileName(files[i]))
				);
			}

			// �f�B���N�g�����ċA�I�ɃR�s�[����
			kjm::_tstrings directories = kjm::directory::getDirectories(sourceDirName);
			for (i = 0; i < (int)directories.size(); i++) {
				if ((*is_cancel_callback)(callback_arg)) {
					break;
				}

				kjm::directory::copy(
					directories[i],
					kjm::path::combine(destDirName, kjm::path::getFileName(directories[i])),
					is_cancel_callback, callback_arg
				);
			}
		}

		// �f�B���N�g�����̃T�u�f�B���N�g����񋓂���B
		static _tstrings getDirectories(const _tstring& path) {
			kjm::_tstrings result;

			WIN32_FIND_DATA data;
			HANDLE hFind = ::FindFirstFile(kjm::path::combine(path, _T("*.*")).c_str(), &data);
			if (hFind != INVALID_HANDLE_VALUE) {
				do {
					if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
						if ((::lstrcmp(data.cFileName, _T(".")) != 0) && (::lstrcmp(data.cFileName, _T("..")) != 0)) {
							result.push_back(kjm::path::combine(path, data.cFileName));
						}
					}
				} while (::FindNextFile(hFind, &data));
				::FindClose(hFind);
			}

			return result;
		}

		// �t�@�C����񋓂��܂��B
		static std::vector<std::basic_string<TCHAR> > getFiles(
			const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildCard = _T("*.*"));

		// �f�B���N�g����񋓂��܂��B
		static std::vector<std::basic_string<TCHAR> > getSubFolders(
			const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildCard = _T("*.*"));

		// �f�B���N�g�����쐬����B
		static void createDirectory(const _tstring& path);
		static DWORD createDirectory2(const _tstring& path) {
			DWORD dwRet = 0;

			// ���� path �����[�g�f�B���N�g���Ȃ�Aparent �� empty �ɂȂ�B
			kjm::_tstring parent = kjm::path::getDirectoryName(path);

			if (!parent.empty() && kjm::directory::exists(path) == false) {
				if ((dwRet = kjm::directory::createDirectory2(parent)) == 0) {
					if (::CreateDirectory(path.c_str(), NULL) == FALSE) {
						dwRet = ::GetLastError();
					}
				}
			}

			return dwRet;
		}

		static kjm::_tstrings glob(const kjm::_tstring& pattern);

		// �w�肵���p�X���f�B�X�N��̊����̃f�B���N�g�����Q�Ƃ��Ă��邩�ǂ������m�F���܂��B
		static bool exists(const _tstring& path) {
			return (::PathFileExists(path.c_str()) != FALSE) ? true : false;
		}

	private:
		directory() {};
		~directory() {};
	};

	//=========================================================================
	// �t�@�C���������T�|�[�g����N���X
	//=========================================================================
	class findFile {
	public:
		// �f�t�H���g�R���X�g���N�^
		findFile() : m_hFind(INVALID_HANDLE_VALUE), m_strWildcard(_T("")) {}

		// �f�X�g���N�^
		virtual ~findFile() { findClose(); }

		// �����J�n
		virtual bool findFirstFile(const _tstring& strFileName) {
			m_strWildcard = strFileName;

			m_hFind = FindFirstFile(m_strWildcard.c_str(), &m_findData);
			return (m_hFind != INVALID_HANDLE_VALUE);
		}

		// ��������
		virtual bool findNextFile() {
			return (FindNextFile(m_hFind, &m_findData) != FALSE);
		}

		// �����I��
		virtual void findClose() {
			if (m_hFind != INVALID_HANDLE_VALUE) {
				FindClose(m_hFind);
				m_hFind = INVALID_HANDLE_VALUE;
			}
		}

		// ���������t�@�C�����A'.' or '..' ���ǂ������ׂ�
		virtual bool isDots() {
			return ((lstrcmp(m_findData.cFileName, _T(".")) == 0) || (lstrcmp(m_findData.cFileName, _T("..")) == 0));
		}

		// ���������t�@�C�����A�f�B���N�g�����ǂ������ׂ�
		virtual bool isDirectory() {
			return ((m_findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
		}

		// ���������t�@�C�����A�ǂݍ��ݐ�p���ǂ������ׂ�
		virtual bool isReadonly() {
			return ((m_findData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0);
		}

		// ���������t�@�C���̃t�@�C�����������擾
		virtual _tstring getFileName() { return m_findData.cFileName; }

		// ���������t�@�C���̃t���p�X�����擾
		virtual _tstring getFullName() { return path::renameFileSpec(m_strWildcard, m_findData.cFileName); }

		// ���������t�@�C���̑�����Ԃ�
		virtual DWORD getFileAttributes() { return m_findData.dwFileAttributes; }

		// ���������t�@�C���̃T�C�Y���擾
		virtual __int64 getFileSize() {
			LARGE_INTEGER li = {m_findData.nFileSizeLow, m_findData.nFileSizeHigh};
			return li.QuadPart;
		}

		dateTime getCreationTime() const { return dateTime(m_findData.ftCreationTime, kjm::utc); }
		dateTime getLastAccessTime() const { return dateTime(m_findData.ftLastAccessTime, kjm::utc); }
		dateTime getLastWriteTime() const { return dateTime(m_findData.ftLastWriteTime, kjm::utc); }

		// ���������t�@�C���̑S���� WIN32_FIND_DATA �^�ɃR�s�[����
		virtual void copyWin32FindData(WIN32_FIND_DATA& rBuf) { rBuf = m_findData; }

	private:
		_tstring m_strWildcard;

		HANDLE m_hFind;

		WIN32_FIND_DATA m_findData;
	};

	//=========================================================================
	// �t�@�C���̑������`�����l
	//=========================================================================
	enum fileAttributes {
		readOnly = FILE_ATTRIBUTE_READONLY,						// ReadOnly = 1,
		hidden = FILE_ATTRIBUTE_HIDDEN,							// Hidden = 2,
		system = FILE_ATTRIBUTE_SYSTEM,							// System = 4,
		directory = FILE_ATTRIBUTE_DIRECTORY,					// Directory = 16,
		archive = FILE_ATTRIBUTE_ARCHIVE,						// Archive = 32,
#if _MSC_VER > 1200	// VC++6����
		device = FILE_ATTRIBUTE_DEVICE,							// Device = 64,
#endif
		normal = FILE_ATTRIBUTE_NORMAL,							// Normal = 128,
		temporary = FILE_ATTRIBUTE_TEMPORARY,					// Temporary = 256,
		sparseFile = FILE_ATTRIBUTE_SPARSE_FILE,				// SparseFile = 512,
		reparsePoint = FILE_ATTRIBUTE_REPARSE_POINT,			// ReparsePoint = 1024,
		compressed = FILE_ATTRIBUTE_COMPRESSED,					// Compressed = 2048,
		offline = FILE_ATTRIBUTE_OFFLINE,						// Offline = 4096,
		notContentIndexed = FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,	// NotContentIndexed = 8192,
		encrypted = FILE_ATTRIBUTE_ENCRYPTED,					// Encrypted = 16384,
	};

	//=========================================================================
	// �t�@�C�� ��\������N���X
	//=========================================================================
	class fileInfo {
	private:
		// �R�s�[�R���X�g���N�^�Ƒ�����Z�q�Ŏg�������o�[�R�s�[�֐�
		void copy_from_fileInfo(const fileInfo& src);

	public:
		// �f�t�H���g�R���X�g���N�^
		fileInfo();

		// �R�s�[�R���X�g���N�^
		fileInfo(const fileInfo& src);

		// WIN32_FIND_DATA �ŏ���������R���X�g���N�^
		fileInfo(const _tstring& strParentPath, const WIN32_FIND_DATA& rFindData);

		// �t�@�C�����ŏ���������R���X�g���N�^
		fileInfo(const _tstring& strName);

		// �f�X�g���N�^
		virtual ~fileInfo() {}

		// ������Z�q
		fileInfo& operator =(const fileInfo& rhs);

		/// �t�@�C�������݂��邩�ǂ����������l���擾���܂��B
		bool exists() const { return kjm::path::fileExists(this->m_fullPath); }

		/// ��΃p�X���擾
		_tstring fullName() const { return m_fullPath; }

		// �t�@�C����������Ԃ�
		//_tstring getFileName() { return path::findFileName(m_fullPath); };
		_tstring name() const { return kjm::path::findFileName(m_fullPath); }

		// �쐬����(UTC)��Ԃ�
		dateTime creationTimeUTC() const { return m_creationTime; }

		// �A�N�Z�X����(UTC)��Ԃ�
		dateTime lastAccessTimeUTC() const { return m_lastAccessTime; }

		// �X�V����(UTC)��Ԃ�
		dateTime lastWriteTimeUTC() const { return m_lastWriteTime; }

		// �쐬����(Local)��Ԃ�
		dateTime creationTime() const { return m_creationTime.toLocalTime(); }

		// �A�N�Z�X����(Local)��Ԃ�
		dateTime lastAccessTime() const { return m_lastAccessTime.toLocalTime(); }

		// �X�V����(Local)��Ԃ� 
		dateTime lastWriteTime() const { return m_lastWriteTime.toLocalTime(); }

		// �t�@�C���̑�����Ԃ�
		fileAttributes attributes() const { return (kjm::fileAttributes)m_dwFileAttributes; }

		// �t�@�C���̑�����Ԃ�
		DWORD get_Attributes() const { return m_dwFileAttributes; }

		// �t�@�C���̑����������Ԃ�
		kjm::_tstring get_AttribText(const kjm::_tstring& format=_T("ASHR")) const;

		// �t�@�C���̃T�C�Y��Ԃ�
		__int64 length() const { return m_fileSize; }

		// �t�@�C�����u�A�[�J�C�u�\�v���ǂ������ׂ�
		bool isArchive() const { return ((m_dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0); }

		// �t�@�C�����u�B���t�@�C���v���ǂ������ׂ�
		bool isHidden() const { return ((m_dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0); }

		// �t�@�C�����u�ǂݎ���p�v���ǂ������ׂ�
		bool isReadonly() const { return ((m_dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0); }

		// �t�@�C�����u�V�X�e���v���ǂ������ׂ�
		bool isSystem() const { return ((m_dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0); }

		// �t�@�C�����usymbolic link or junction�v�����ǂ������ׂ�

		bool isSymbolicLink() const {
			return ((m_dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);
		}

		// �t�@�C���̃o�[�W��������Ԃ�
		_tstring getFileVersion() const { return kjm::util::getFileVersion(m_fullPath); }

	private:
		_tstring m_fullPath;		// �t�@�C���̃t���p�X

		// -- �����ւ����� --
		dateTime m_creationTime;	// �쐬����
		dateTime m_lastAccessTime;	// �A�N�Z�X����
		dateTime m_lastWriteTime;	// �ŏI�X�V����

		__int64 m_fileSize;			// �t�@�C���̃T�C�Y
		DWORD m_dwFileAttributes;	// �t�@�C���̑���

	private:
		// ����ێ����Ă��邩�ǂ����̃t���O�B
		// �t�@�C���T�C�Y�A�ŏI�X�V���t�Ȃǂ̏��������Ă���Ƃ��� true
		// WIN32_FIND_DATA �Ő�������Ă���Ƃ��́Atrue �ɂȂ�B
		bool m_hasInfo;
	};

	//=========================================================================
	// �f�B���N�g�� ��\������N���X
	//=========================================================================
	class folderInfo {
	public:
		// �f�t�H���g�R���X�g���N�^
		folderInfo() {}

		// �R�s�[�R���X�g���N�^
		folderInfo(const folderInfo& src) : m_fullPath(src.m_fullPath) {}

		// WIN32_FIND_DATA �ŏ���������R���X�g���N�^
		folderInfo(const _tstring& strParentPath, const WIN32_FIND_DATA& rFindData) {
			m_fullPath = path::append(strParentPath, rFindData.cFileName);
		}

		// �f�B���N�g�����ŏ���������R���X�g���N�^
		folderInfo(const _tstring& strName) : m_fullPath(strName) {}

		// �f�X�g���N�^
		virtual ~folderInfo() {}

		// ������Z�q
		folderInfo& operator =(const folderInfo& rhs) {
			if (this != &rhs) m_fullPath = rhs.m_fullPath;
			return *this;
		}

		// ���������t�@�C���̃t�@�C�����������擾
		_tstring getFileName() const { return path::findFileName(m_fullPath); }

		// �f�B���N�g�������̃t�@�C���ꗗ���擾
		std::vector<fileInfo> getFiles(const _tstring& strWC = _T("*.*")) const;

		// �f�B���N�g�������̃f�B���N�g���ꗗ���擾
		std::vector<folderInfo> getSubFolders() const;

		// �f�B���N�g���̃t���p�X��Ԃ�
		_tstring getFullPath() const { return m_fullPath; }

		// �f�B���N�g�������݂��邩�ǂ�����Ԃ�
		bool exist(DWORD& detail) {
			bool ret = kjm::path::fileExists(m_fullPath);
			detail = ::GetLastError();
			return ret;
		}

		// �f�B���N�g�������݂��邩�ǂ�����Ԃ�
		bool exist() const { return kjm::path::fileExists(m_fullPath); }

		// �f�B���N�g�����쐬����
		void create() { kjm::directory::createDirectory(m_fullPath); }

		// �f�B���N�g���̑�����Ԃ�
		DWORD get_Attributes() const { return GetFileAttributes(m_fullPath.c_str()); }

	public:
		_tstring m_fullPath;	// �f�B���N�g���̃t���p�X
	};

	//-------------------------------------------------------------------------
	// ���݂̊�����уv���b�g�t�H�[���Ɋւ�����A����т����𑀍삷���
	// �i��񋟂��܂��B(.NET�̕����I����)
	//-------------------------------------------------------------------------
	class environment {
	public:

		// ���[�J�� �R���s���[�^�� NetBIOS �����擾���܂��B
		static _tstring get_MachineName() { return kjm::util::getComputerName(); }

		// �w�肵��������ɖ��ߍ��܂�Ă���e���ϐ��̖��O���A���̕ϐ��̒l��\��������Œu�����A�u����̕�����S�̂�Ԃ��܂��B
		static _tstring expandEnvironmentVariables(const _tstring& name);

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

	//-------------------------------------------------------------------------
	// mutex �Ǘ��N���X
	//
	// note: �쐬�����n���h���́A�f�X�g���N�^�Ŕj�������B
	//       �܂��Alock(WaitForSingleObject)�����܂܂̃n���h���́A
	//       �K�v��unlock(ReleaseMutex)���Ă���A�j�������B
	//-------------------------------------------------------------------------
	class mutex {
	private:
		HANDLE m_hMutex;		// �Ǘ����� mutex �ւ̃n���h��
		DWORD m_create_status;	// mutex �쐬���̏�ԕϐ�
		DWORD m_last_error;		// API�g�p���̃G���[�ێ�
		int m_lock_count;		// lock������

	public:
		// mutex �쐬���ɓ����� mutex �����݂����Ƃ��́Ature ��Ԃ�
		bool is_exists() { return (m_create_status == ERROR_ALREADY_EXISTS); }

		// mutex�쐬�ς݂��ǂ����̃e�X�g
		bool is_open() const { return (m_hMutex != NULL); }

	public:
		// �f�t�H���g�R���X�g���N�^
		mutex() : m_hMutex(NULL), m_create_status(0), m_lock_count(0) {}
		
		// �R�s�[�R���X�g���N�^
		mutex(const mutex& src);
		
		// �~���[�e�b�N�I�u�W�F�N�g�쐬�t���R���X�g���N�^
		mutex(const _tstring &name);
		
		// �f�X�g���N�^
		virtual ~mutex() { close(); }

		// Mutex�̍쐬
		bool create(const _tstring &name);
		
		// mutex �����
		void close();

		// �w�莞�ԃV�O�i����ԂɂȂ�̂�҂�
		DWORD lock(DWORD dwMillisec);

		// mutex���J������
		BOOL unlock();
		
		// ������Z�q
		mutex& operator=(const mutex& src);

	private:
		// �n���h���������ʊ֐�
		void __duplicate(const mutex& src);
	};

	//-----------------------------------------------------------------
	// �ȈՐ��w�n�֐�
	//-----------------------------------------------------------------
	class math {
	public:
		// �S�����̌v�Z
		template <class T>
		static double percentage(T numerator, T denominator) {
			return (denominator == 0.0) ? 100.0 : ((double)numerator / (double)denominator) * 100.0;
		}
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

	//-----------------------------------------------------------------
	// ���b�N�t�@�C�������S�Ɉ������߂̃N���X
	//-----------------------------------------------------------------
	class Lockfile {
	public:
		//-------------------------------------------------------------
		// �R���X�g���N�^
		//-------------------------------------------------------------
		Lockfile() : m_hFile(INVALID_HANDLE_VALUE) {};

		//-------------------------------------------------------------
		// �f�X�g���N�^
		//-------------------------------------------------------------
		virtual ~Lockfile() { Unlock(); };

		//-------------------------------------------------------------
		// ���b�N����
		//-------------------------------------------------------------
		BOOL Lock(const std::basic_string<TCHAR>& strLockfile);

		//-------------------------------------------------------------
		// �A�����b�N����
		//-------------------------------------------------------------
		BOOL Unlock();

		// ���b�N�����ǂ������ׂ�
		bool IsLocked() { return (m_hFile != INVALID_HANDLE_VALUE); };

	private:
		//-------------------------------------------------------------
		// �R�s�[�Ƒ����F�߂Ȃ�
		//-------------------------------------------------------------
		Lockfile(const Lockfile& src) {};
		void operator=(const Lockfile& lhs) {};

		//-------------------------------------------------------------
		// �����o�[�ϐ�
		//-------------------------------------------------------------
		HANDLE m_hFile;
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



	// Convert�N���X
	class convert {
	private:
		convert() {}
		~convert() {}

	public:
		static std::basic_string<TCHAR> toString(int value) {
			TCHAR str[128];
			return std::basic_string<TCHAR>(_itot(value, str, 10));
		}
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
		iniFile() : m_fileName() {}

		// �R�s�[�R���X�g���N�^
		iniFile(const iniFile& src) : m_fileName(src.m_fileName) {}

		// �t�@�C������^����R���X�g���N�^
		iniFile(const _tstring& szFileName) : m_fileName(szFileName) {}

		// �f�X�g���N�^
		virtual ~iniFile() {}

		// ������Z�q
		kjm::iniFile& operator=(const iniFile& rhs) {
			if (this == &rhs) return *this;
			this->m_fileName = rhs.m_fileName;
			return *this;
		}

		// �Z�N�V�����f�[�^���擾
		std::vector<TCHAR> getSectionData(const _tstring& section) const;

		// �Z�N�V�����f�[�^����������
		BOOL writeSectionData(const _tstring& section, const std::vector<TCHAR>& sectionData);

		// �Z�N�V�����ꗗ���擾
		_tstrings getSectionNames() const;

		// �Z�N�V�������̃L�[�ꗗ���擾
		_tstrings getKeyNames(const _tstring& strSectionName) const;

		// ini�t�@�C������l���擾
		_tstring getString(const _tstring& strSection,
			const _tstring& strKey, const _tstring& strDefault = _T(""), BOOL* pfUseDefault = NULL) const;

		// ini�t�@�C�����琔�l���擾
		UINT getInt(const kjm::_tstring& strSection, const kjm::_tstring& strKey, UINT defValue = 0, BOOL* pfUseDefault = NULL) const;

		// ini�t�@�C���ɒl����������
		BOOL writeString(const _tstring& strSection, const _tstring& strKey, const _tstring& strValue);

		// �ʂ�ini�t�@�C���̓��e���}�[�W����
		void mergeIniFile(const iniFile& newIniFile);

		// ini�t�@�C�������擾
		_tstring get_fileName() const { return m_fileName; };

		// ini�t�@�C������ݒ�
		void set_fileName(const _tstring& value) { m_fileName = value; };

	private:
		_tstring m_fileName;	// ini�t�@�C���̖��O

	public:
		// ���W���[�����ɑΉ�����ini�t�@�C���������N���X���쐬����
		static iniFile getModuleIniFile(HMODULE hModule = NULL) {
			return iniFile(kjm::util::getModuleIniFile(hModule));
		}
	};

	//-----------------------------------------------------------------
	// INI�t�@�C���������N���X
	//-----------------------------------------------------------------
	class IniFile {
	public:
		//-------------------------------------------------------------
		// �f�t�H���g �R���X�g���N�^
		//-------------------------------------------------------------
		IniFile() {
			TCHAR buf[MAX_PATH];
			::GetModuleFileName(NULL, buf, sizeof(buf));
			::PathRenameExtension(buf, _T(".ini"));
			m_iniFile = buf;
		}

		//-------------------------------------------------------------
		// �f�X�g���N�^
		//-------------------------------------------------------------
		virtual ~IniFile() {}

		//-------------------------------------------------------------
		// ��������擾����
		//-------------------------------------------------------------
		std::basic_string<TCHAR> GetString(
			const std::basic_string<TCHAR>& strSection,
			const std::basic_string<TCHAR>& strKey,
			const std::basic_string<TCHAR>& strDefault) const {

			const int GROWSIZE = 768;	// ���������A���� resize �ŕ����񂪎擾�ł��邩�Ȃ��Ǝv���T�C�Y

			std::vector<TCHAR> buf;
			int ret;
			do {
				buf.resize(buf.size() + GROWSIZE);
				ret = ::GetPrivateProfileString(strSection.c_str(), strKey.c_str(), strDefault.c_str(), &buf[0], buf.size(), m_iniFile.c_str());
			} while (ret == buf.size() - 1);
			return &buf[0];
		}

		//-------------------------------------------------------------
		// �l���擾����
		//-------------------------------------------------------------
		UINT GetUInt(const std::basic_string<TCHAR>& strSection, const std::basic_string<TCHAR>& strKey, UINT uDefault) const {
			return ::GetPrivateProfileInt(strSection.c_str(), strKey.c_str(), uDefault, m_iniFile.c_str());
		};

		//-------------------------------------------------------------
		// ���������������
		//-------------------------------------------------------------
		void WriteString(const std::basic_string<TCHAR>& strSection, const std::basic_string<TCHAR>& strKey, const std::basic_string<TCHAR>& strValue) {
			::WritePrivateProfileString(strSection.c_str(), strKey.c_str(), strValue.c_str(), m_iniFile.c_str());
		};

		//-------------------------------------------------------------
		// �l(UINT)����������
		//-------------------------------------------------------------
		void WriteUInt(const std::basic_string<TCHAR>& strSection, const std::basic_string<TCHAR>& strKey, UINT uValue) {
			TCHAR work[16];	// 4294967295(0xffffffff)���������ނ̂ɏ\���ȃo�b�t�@
			wsprintf(work, _T("%u"), uValue);
			::WritePrivateProfileString(strSection.c_str(), strKey.c_str(), work, m_iniFile.c_str());
		};

		//-------------------------------------------------------------
		// INI �t�@�C���̖��O��ݒ肷��
		//-------------------------------------------------------------
		void set_IniFile(const std::basic_string<TCHAR>& strIniFile) { m_iniFile = strIniFile; };

		//-------------------------------------------------------------
		// INI �t�@�C���̖��O���擾����
		//-------------------------------------------------------------
		std::basic_string<TCHAR> get_IniFile() { return m_iniFile; };

	private:
		std::basic_string<TCHAR> m_iniFile;
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

		// �h���C�u�Ƀ��f�B�A���}������Ă��邩�ǂ������ׂ�
		// �ǂ̃h���C�u�^�C�v�ɂ��g���邪�A
		// get_DriveType() == DRIVE_REMOVABLE �̂Ƃ��A�g���Ă��������B
		bool isDiskInDrive();

	private:
		int m_driveNo;	// �h���C�u�ԍ�(-1=���I�� 0=A 1=B...)
	};

	//---------------------------------------------------------------------
	// �h���C�u�̈ꗗ��ێ�����N���X
	//---------------------------------------------------------------------
	class driveList {
	public:
		// �f�t�H���g�R���X�g���N�^
		driveList();

		// �R�s�[�R���X�g���N�^
		driveList(const driveList& src) : m_driveList(src.m_driveList) {};

		// �f�X�g���N�^
		virtual ~driveList() {};

		// ������Z�q
		driveList& operator=(const driveList& rhs);

		// �h���C�u�ꗗ�̐����擾
		int get_Size() { return m_driveList.size(); };

		// �h���C�u�I�u�W�F�N�g���擾
		drive get_Drive(int index) { return m_driveList[index]; };

	private:
		std::vector<drive> m_driveList;	// PC�ɂȂ����Ă���h���C�u�ꗗ
	};

	//-------------------------------------------------------------------------
	// ���ʂ̃��O����
	//-------------------------------------------------------------------------
	class log {
	public:
		// �f�t�H���g�R���X�g���N�^
		log();

		// �f�X�g���N�^
		~log() {};

		// ���O�̃V�X�e�������擾
		_tstring get_MyName();

		// ���O�V�X�e���̏������O�Ɏc��
		void writeMyName();

		// �G���[���O
		void writeError(LPCTSTR pszFormat, ...);

		// ��񃍃O
		void writeInfo(LPCTSTR pszFormat, ...);

		// �S���O
		void writeAll(LPCTSTR pszFormat, ...);

		// ���O�̏o�͊֐�
		virtual void write(LPCTSTR format, ...);

		// ���O�̏o�͊֐�(��ʏo�͂���)
		virtual void writee(LPCTSTR format, ...);

		// �Â����O�̍폜
		void deleteOldLog();

		// ���ʎq�̎擾
		_tstring get_Ident() { return m_ident; };

		// ���ʎq�̐ݒ�
		void set_Ident(const _tstring& value) { m_ident = value; };

		// ���O�t�@�C���̃v���t�B�N�X��ݒ肷��
		// ������� {YMD} ���܂߂�ƁA���t���Ƃ̃t�@�C�����쐬����悤�ɂȂ�B
		virtual void set_prefix(const _tstring& strPrefix);

		// ���O�t�@�C���̏o�͐��ݒ肷��
		// ���l���w�肷��ƁA�悭�g���ꏊ���w��ł��܂��B
#define LOGPATH_TEMP	0
#define LOGPATH_EXEPATH	1
		virtual void set_log_path(const _tstring& strPath);
		virtual void set_log_path(int nMode, const kjm::_tstring& more = kjm::_tstring());

		// ���O�̕ۑ���� %TEMP%\<subdir> �ɂ���
		virtual void setLogPath_Temp(const _tstring& subdir = kjm::_tstring());

		// ���O�̕ۑ���� �X�N���v�g�Ɠ����ꏊ\<subdir> �ɂ���
		virtual void setLogPath_App(const _tstring& subdir = kjm::_tstring());

		// ���O�t�@�C���̏o�͐���擾����
		virtual _tstring get_log_path();

		// ���O�̃t�@�C�������[����ݒ肷��
		virtual void set_LogNameType(int nNameType);

		// ���O�̃t�@�C�������[�����擾����
		virtual long get_LogNameType();

		// �X�g�b�v�t���O�ւ̃A�N�Z�T
		bool get_Stop() const { return this->m_stop; }
		void set_Stop(bool value) { this->m_stop = value; }

		// ���O�̍ő�T�C�Y��ݒ�
		int get_RotateSize() const { return this->m_rotateSize; }
		void set_RotateSize(int value) { this->m_rotateSize = value; }
		int get_maxLogSize() const { return this->get_RotateSize(); }	// obsolate
		void set_maxLogSize(int value) { this->set_RotateSize(value); }	// obsolate

		// ���O�̃��[�e�[�V�������ւ̃A�N�Z�T
		int get_RotateCount() const { return this->m_rotateCount; }
		void set_RotateCount(int value) { this->m_rotateCount = value; }

	private:
		// ���O���V�t�g����
		void shift(const _tstring& fname);

	private:
		int m_rotateCount;			// ���O���V�t�g����Ƃ��̍ő�t�@�C����
		int m_rotateSize;			// ���O���V�t�g����P��(0�ŃV�t�g���Ȃ�)
		bool m_stop;				// ���O���~�߂�Ƃ� true �ɂ���A�f�t�H���g�� true
		_tstring m_ident;
		_tstring m_strBaseFileName;	// ��{�ɂȂ郍�O�t�@�C����
		_tstring m_strPrefix;		// ���O�t�@�C���̐擪�ɕt���閼�O
		_tstring m_strIdent;		// ���O�̎��ʎq
		_tstring m_strLogPath;		// ���O�̏o�͐�(�R���X�g���N�^��%TEMP%�ɐݒ肳���)
	};

	// �f�t�H���g�ŗp�Ӎς݂̃��O�I�u�W�F�N�g��Ԃ��֐�
	// �R�����g���΁A���ɕϐ��錾�ȂǂȂ��ŁA�^�p�\�B
	kjm::log& GetLog();

	//=================================================================
	// �������K�v�ȃI�v�V�����Ɉ������^�����Ȃ������ꍇ�̗�O
	//=================================================================

	class missingArgument : public std::exception {
	public:
		missingArgument(const std::string& msg) : std::exception(("missing argument: " + msg).c_str()) {}
		~missingArgument() {}
	};

	//=================================================================
	// ��`����Ă��Ȃ��I�v�V�������^����ꂽ�ꍇ�̗�O
	//=================================================================

	class invalidOption : public std::exception {
	public:
		invalidOption(const std::string& msg) : std::exception(("invalid option: " + msg).c_str()) {}
		~invalidOption() {}
	};

	//-------------------------------------------------------------------------
	// GetTickCount�Ōo�ߎ��Ԃ��v��N���X
	//-------------------------------------------------------------------------
	class tickCounter {
	public:
		// �f�t�H���g�R���X�g���N�^
		tickCounter() : m_t1(GetTickCount()), m_t2(0), m_t3(GetTickCount()), m_totalCount(0), m_count(0) {}

		// �����w��t���R���X�g���N�^
		tickCounter(__int64 totalCount) : m_t1(GetTickCount()), m_t2(0), m_t3(GetTickCount()), m_totalCount(totalCount), m_count(0) {}

		// �f�X�g���N�^
		~tickCounter() {}

		// �v���J�n
		void start() { m_t3 = m_t1 = GetTickCount(); m_t2 = 0; }

		// �v���I��
		void stop() { m_t2 = GetTickCount(); }

		// ���b�v�^�C���L�^
		// �^�b�v�^�C�����w��msec�ȏ�̎��A���b�v�^�C�����L�^����
		// ���b�v�������́Atrue ��Ԃ�
		bool lap(DWORD laptime_msec = 0) {
			bool ret = false;
			if (lapElapse() >= laptime_msec) {
				m_t3 = GetTickCount();
				ret = true;
			}
			return ret;
		}

		// �o�ߎ���(ms)
		DWORD elapse() { return (m_t2 == 0) ? (GetTickCount() - m_t1) : (m_t2 - m_t1); }

		// �o�ߎ���(sec)
		DWORD elapseSec() { return elapse() / 1000; }

		// �o�ߎ���(min)
		DWORD elapseMin() { return elapseSec() / 60; }

		// lap����̌o�ߎ���(ms)
		DWORD lapElapse() { return (m_t2 == 0) ? (GetTickCount() - m_t3) : (m_t2 - m_t3); }

		// lap����̌o�ߎ���(sec)
		DWORD lapElapseSec() { return lapElapse() / 1000; }

		// lap����̌o�ߎ���(min)
		DWORD lapElapseMin() { return lapElapseSec() / 60; }

		// �c�莞�Ԃ��v�邽�߂̑���
		__int64 m_totalCount;

		// �c�莞�Ԃ��v�邽�߂̐i����
		__int64 m_count;

		// �i����
		int progress() { return (int)((m_count * 100) / m_totalCount); }

		// ����c�莞��(msec)
		DWORD estimateRemainingTime() {
			return (DWORD)(((m_totalCount - m_count) * elapse()) / m_count);
		}

	private:
		DWORD m_t1;	// �J�n�������̎���(�R���X�g���N�^�Astart�ōX�V)
		DWORD m_t2;	// �I���������̎���(stop�ōX�V)
		DWORD m_t3;	// ���b�v�^�C�����v�鎞��(lap�ōX�V)
	};

	//---------------------------------------------------------------------
	// ODBC���������[�e�B���e�B�N���X
	// ���p����Ƃ��́AUSE_ODBC32���`����
	//---------------------------------------------------------------------
#pragma region USE_ODBC32
#if defined(USE_ODBC32)
	class odbcUtil {
	private:
		odbcUtil() {}
		~odbcUtil() {}

	public:
		//-------------------------------------------------------------
		// �C���X�g�[���ς݂�ODBC�h���C�o�ꗗ���擾
		//-------------------------------------------------------------
		static void getDriverList(std::vector<kjm::_tstring>& drivers);

		//-------------------------------------------------------------
		// �C���X�g�[���ς݂�ODBC�h���C�o�ꗗ���擾(ODBC2.0��)
		//-------------------------------------------------------------
		static void getDriverList_ODBCv2(std::vector<kjm::_tstring>& drivers);

		//-------------------------------------------------------------
		// �C���X�g�[���ς݂̃h���C�o�𖼑O�Ō���
		//
		// ������Ȃ����́A�󕶎����Ԃ��B
		//-------------------------------------------------------------
		static kjm::_tstring findDriver(const kjm::_tstring& pattern);

		//-------------------------------------------------------------
		// �h���C�o���C���X�g�[���ς݂��ǂ������ׂ�
		//-------------------------------------------------------------
		static bool isInstalled(const kjm::_tstring& driverName);
	};
#endif
#pragma endregion USE_ODBC32

	//-------------------------------------------------------------------------
	// ���ȒP��XML����f�[�^��Ǎ���class
	//
	// ���̃N���X�𗘗p����Ƃ��́A
	// �v���W�F�N�g�S�̂ɁAUSE_MSXML6 ���`���Ă��������B
	//
	// CoInitialize(NULL); �� CoUninitialize(); �́A�A�v���P�[�V�����ōs���Ă��������B
	//-------------------------------------------------------------------------
#pragma region USE_MSXML6
#if defined(USE_MSXML6)
	class xmlez {
	private:
		xmlez() {};
		~xmlez() {};

	public:
		// ���ȒP��XML����f�[�^��Ǎ���
		static kjm::_tstring getText(const kjm::_tstring& xmlFile, const kjm::_tstring &tagName);
	};
#endif
#pragma endregion USE_MSXML6

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
		bool m_loop;

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
		bool still_active(LPDWORD pcode = NULL) { 
			DWORD code = 0;
			if (!GetExitCodeThread( m_hThread, &code )) {
				code = 0;	// �G���[�������́A�����Ă��Ȃ����ɂ���
			}
			if (pcode) *pcode = code;
			bool ret = ( code == STILL_ACTIVE );
			return ret;
		};

		//! �X���b�h���N������B
		void run() {
			m_loop = true;

			// �X���b�h���쒆�ɍēx���s���悤�Ƃ���̂́A�n���h�����[�N�𐶂ݏo���̂Ŗ��
			// �X���b�h���~�܂��Ă��邱�Ƃ��`�F�b�N����
			assert(still_active() == false);	// Debug �r���h�Ȃ� assert �ŋ����I��
			if (still_active()) return;			// Release �Ȃ瓮�������ӂ�����ďI��
			wait();	// �n���h������Ă���N�������ɓ���
			m_hThread = ( HANDLE )_beginthreadex( NULL, 0, thread_func, this, 0, NULL );
		}
		void begin() { run(); }

		/*!
		 * �X���b�h���I��������B�h���N���X�Œ�`����B
		 * �I�����@�́Aworker method �̎����Ɉˑ�����B
		 */
		virtual void end() {
			m_loop = false;
		}

		virtual bool is_end() const {
			return !m_loop;
		}
		virtual bool still_loop() const {
			return m_loop;
		}

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
		// �X���b�h�̏I����҂���
		// �n���h���́A�ʓr close ����K�v������B
		virtual DWORD wait_only(DWORD dwTimeout = INFINITE) {
			return WaitForSingleObject(m_hThread, dwTimeout);
		}
		// �X���b�h�n���h�������
		// �X���b�h�������Ă��Ă�����̂ŊǗ��ł��Ȃ��Ȃ�܂��B
		void close() {
			if (m_hThread != NULL) {
				CloseHandle(m_hThread);
				m_hThread = NULL;
			}
		}

		/*!
		 * ���ʂ̃X���b�h�֐��B
		 * args�ɂ́Athread�N���X����h�������N���X�I�u�W�F�N�g�ւ�
		 * �|�C���^���󂯎��Bp->worker() �����s���邾���B
		 */
		static unsigned __stdcall thread_func( void* args ) {
			unsigned ret = ( ( thread* )args )->worker();
			return ret;
		};
	};
#endif

	//---------------------------------------------------------------------
	// �R�}���h���C���v���O�����̊�{�`
	//---------------------------------------------------------------------
	class program {
	protected:
		int m_argc;
		TCHAR** m_argv;
		std::vector<kjm::optionInfo> m_options;
		kjm::cmdLine m_cmdLine;

	public:
		program() {}
		virtual ~program() {}

	public:
		int run(int argc, TCHAR** argv) {
			C_SETLOCALE_JAPAN;
			this->m_argc = argc;
			this->m_argv = argv;
			int exitCode = 0;
			if (this->parseOption(exitCode) == false) {
				return exitCode;
			}
			return this->doMain();
		}

	public:
		virtual bool parseOption(int& exitCode) { return true; }

		virtual int doMain() { return 0; }

		// �o�[�W�������̕\��
		virtual void printVersion() {
			kjm::_tstring fname = kjm::util::getModuleFileName();
			_tcout << fname << _T(" ") << kjm::util::getFileVersion(fname) << _T("\n");
		}

		// �g�p�@�̕\��
		virtual void printUsage() {
			printVersion();
			_tcout << _T("usage: \n");

			kjm::optionInfoList::const_iterator ci;
			for (ci = m_options.begin(); ci != m_options.end(); ci++) {
				_tcout << _T("  ");
				if ((*ci).m_shortOption.empty()) {
					_tcout << _T("    ");
				} else {
					_tcout << _T("-") << (*ci).m_shortOption << _T(", ");
				}
				_tcout << _T("--") << (*ci).m_longOption;
				if ((*ci).m_optionArgInfo == kjm::no_argument) {
					_tcout << _T("\n");
				} else if ((*ci).m_optionArgInfo == kjm::optional_argument) {
					if (!(*ci).m_argDescription.empty()) {
						_tcout << _T("[=") << (*ci).m_argDescription << _T("]\n");
					} else {
						_tcout << _T("[=ARGUMENT]\n");
					}
				} else {
					if (!(*ci).m_argDescription.empty()) {
						_tcout << _T("=") << (*ci).m_argDescription << _T("\n");
					} else {
						_tcout << _T("=ARGUMENT\n");
					}
				}

				if (!(*ci).m_optDescription.empty()) {
					_tcout << _T("\t") << (*ci).m_optDescription << _T("\n");
				}
			}
		}
	};
}

std::basic_ostream<TCHAR>& operator<<(std::basic_ostream<TCHAR>& lhs, kjm::PathString& p);

/****************************************************************************
  lpExistingFileName�̃t�@�C����lpNewFileName�ɃR�s�[���܂��B�R�s�[���s���ɂ�
  �w��񐔕����g���C���s���܂��B

  <�p�����[�^>
  lpExistingFileName  �R�s�[��
  lpNewFileName       �R�s�[��
  bFailIfExists       �㏑�����s����(FALSE�ŏ㏑�����s��)
  retry               �Ď��s������s����
  wait                �Ď��s���s���܂ŉ��b�҂̂�

  <�߂�l>
  �R�s�[�ɐ��������Ƃ�TRUE�A�R�s�[�Ɏ��s�����Ƃ�FALSE��Ԃ��B
****************************************************************************/
BOOL IOUtil__CopyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists, int retry, int wait, LPDWORD pdwErr = NULL);

#endif
