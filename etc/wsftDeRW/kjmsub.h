//---------------------------------------------------------------------------------------
// kjmsub (20101217)
//---------------------------------------------------------------------------------------
/*
	�ykjm::log �Ή����郍�O���C�u�����ɂ��āz
	ver.0.4.0.0 ����ǉ��B
	USE_WSSLOG ���`���邱�ƂŁAWssLog.dll ���g���ă��O���o�͂ł���悤�ɂȂ�܂��B

	�ykjm::util::getFileSize �ɂ��āz
	ver.0.3.0.0 ����ǉ��B

	�ykjm::util::exitWindows �ɂ��āz
	ver.0.2.0.0 ����ǉ��B

	�yGetComputerNameEx�ɂ��āz
	
	Visual C++ 6�ł́AGetComputerNameEx�֐�����`����Ă��܂���B
	kjm::util::__GetComputerNameEx�̗p�Ɏ������邱�ƂŁA���p���邱�Ƃ��ł��܂��B

	�R���s���[�^���ɂ́ANetBIOS����DNS�z�X�g��������܂����AWindows2000�ȍ~�ł́ADNS�z�X
	�g�������C���ɂȂ�܂��B�܂��A�����I�ȃR���s���[�^���ƃN���X�^�@�\���g�����Ƃ�(���Ǝv
	��)�ɕt���A�_���I�ȃR���s���[�^��������܂��B

	��ʓI�ȃR���s���[�^�����擾����ɂ́A��������ComputerNamePhysicalDnsHostname ���w
	�肵�āAGetComputerNameEx�֐����Ăяo���܂��B

	�yVisual C++ �̃o�[�W��������ɂ��āz

	_MSC_VER == 1200	Visual C++ 6.0
	_MSC_VER == 1310	Visual C++ .NET 2003
	_MSC_VER == 1400	Visual C++ 2005
	_MSC_VER == 1500	Visual C++ 2008
*/
#pragma once

#pragma warning(disable : 4786)

#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#endif

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "version.lib")

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <locale>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <cassert>
#include <ctime>
#include <process.h>

#if defined(USE_WSSLOG)
#include "wsslog.h"
#pragma comment(lib, "wsslog.lib")
#endif

#if defined(UNICODE)
#define _tstring			std::wstring
#define _tout				std::wcout
#define _tcout				std::wcout
#define _otstream			std::wostream
#define _otstringstream		std::wostringstream
#define _tifstream			std::wifstream
#define _tofstream			std::wofstream
#else
#define _tstring			std::string
#define _tout				std::cout
#define _tcout				std::cout
#define _otstream			std::ostream
#define _otstringstream		std::ostringstream
#define _tifstream			std::ifstream
#define _tofstream			std::ofstream
#endif

// STL �� setlocale(LC_ALL, _T("japan")) ���������́A�ȉ��̒ʂ�B
// std::locale �R���X�g���N�^�͏�ɁA_MBCS��������󂯎��B
#define STL_SETLOCALE_JAPAN		std::locale::global(std::locale("japanese"))

// _countof �́A���Ȃ��Ƃ� vs2008 ����W���Œ�`����Ă���
#if !defined(_countof)
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

// winbase.h �Ő錾����Ă���AGet/SetComputerNameEx�p�̈����̒l
// Visual C++ 6.0 �ȉ��̂Ƃ��ɗL���ɂ���B
#if _MSC_VER <= 1200
typedef enum _COMPUTER_NAME_FORMAT {
    ComputerNameNetBIOS,
    ComputerNameDnsHostname,
    ComputerNameDnsDomain,
    ComputerNameDnsFullyQualified,
    ComputerNamePhysicalNetBIOS,
    ComputerNamePhysicalDnsHostname,
    ComputerNamePhysicalDnsDomain,
    ComputerNamePhysicalDnsFullyQualified,
    ComputerNameMax
} COMPUTER_NAME_FORMAT;
#endif

// FILETIME �������₷�����Ԃɕϊ����邽�߂̒萔
#define FT_1USEC			(__int64)10
#define FT_1MSEC			(FT_1USEC * 1000)
#define FT_1SEC				(FT_1MSEC * 1000)
#define FT_1MIN				(FT_1SEC  *   60)	// FILETIME�^��1���̒l
#define FT_1HOUR			(FT_1MIN  *   60)	// FILETIME�^��1���Ԃ̒l
#define FT_1DAY				(FT_1HOUR *   24)	// FILETIME�^��1���̒l
#define FT_19700101			(116444736000000000)		// time_t �̊J�n�n�_��\��FILETIME�̒l
#define FT_19700101_SEC		(FT_19700101 / FT_1SEC)

// __int64 �� iostream �ɏo�͂��邽�߂̊֐�
#if _MSC_VER < 1500		// ���Ȃ��Ƃ��Avs2008����́A__int64���󂯎��ostream�����݂���̂ŁA�����ɂ���B
_otstream& operator<<(_otstream& os, __int64 i);
#endif

namespace kjm {
	/******************************************************************
		�f�B���N�g�����̃t�@�C���ꗗ���擾
	******************************************************************/
	bool dir(LPCTSTR pszPath, LPCTSTR pszWildcard, std::vector<_tstring>& result);

	//-------------------------------------------------------------------------
	// HBRUSH ���Ǘ�����N���X
	//-------------------------------------------------------------------------
	class Brush {
	public:
		// �f�t�H���g�R���X�g���N�^
		Brush() : m_hbr(NULL) {};
		
		// �\���b�h�u���V���쐬����R���X�g���N�^
		Brush(COLORREF crColor) : m_hbr(NULL) { createSolidBrush(crColor); };

		// �f�X�g���N�^
		virtual ~Brush() { deleteObject(); };

		// CreateSolidBrush �̃��b�p�[
		bool createSolidBrush(COLORREF crColor) {
			return ((m_hbr = CreateSolidBrush(crColor)) != NULL);
		};

		// DeleteObject �̃��b�p�[
		bool deleteObject() {
			bool bRet = true;
			if (m_hbr != NULL) {
				bRet = (DeleteObject(m_hbr) != FALSE);
				assert(bRet != FALSE);	// �G���[�ɂȂ�Ƃ́A����?
				m_hbr = NULL;
			}
			return bRet;
		};

		// HGDIOBJ �ւ̃L���X�g
		operator HGDIOBJ () { return m_hbr; };

	private:
		// �R�s�[�Ƒ���������Ȃ�
		Brush(const Brush& src) {};
		Brush& operator =(const Brush& rhs) { return *this; };

	private:
		HBRUSH m_hbr;
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
		virtual void set_prefix(const _tstring& strPrefix);

		// ���O�t�@�C���̏o�͐��ݒ肷��
		virtual void set_log_path(const _tstring& strPath);

		// ���O�t�@�C���̏o�͐���擾����
		virtual _tstring get_log_path();

	private:
		_tstring m_ident;
		_tstring m_strBaseFileName;	// ��{�ɂȂ郍�O�t�@�C����
		_tstring m_strPrefix;		// ���O�t�@�C���̐擪�ɕt���閼�O
		_tstring m_strIdent;		// ���O�̎��ʎq
		_tstring m_strLogPath;		// ���O�̏o�͐�(�R���X�g���N�^��%TEMP%�ɐݒ肳���)
	};

	//-------------------------------------------------------------------------
	// kjm::util::copyFolder �̏ڍ׏��擾�N���X
	//-------------------------------------------------------------------------
	struct copyFolderInfo {
		_tstring srcName;	// �G���[�������̃R�s�[���t�@�C����
		_tstring dstName;	// �G���[�������̃R�s�[��t�@�C����
		int copyFileError;	// kjm::util::copyFile �̃G���[�R�[�h
		DWORD dwError;		// GetLastError �̃G���[�R�[�h

		copyFolderInfo() : srcName(), dstName(), copyFileError(0), dwError(0) {};
	};

	//-------------------------------------------------------------------------
	// ���݂̊�����уv���b�g�t�H�[���Ɋւ�����A����т����𑀍삷���i
	// ��񋟂��܂��B
	//-------------------------------------------------------------------------
	class environment {
	private:
		environment() {}
		~environment() {}

	public:
		// �w�肵��������ɖ��ߍ��܂�Ă���e���ϐ��̖��O���A���̕ϐ��̒l��\��������Œu�����A�u����̕�����S�̂�Ԃ��܂��B
		static _tstring expandEnvironmentVariables(const _tstring& name);
	};

	//-------------------------------------------------------------------------
	// ���[�e�B���e�B�֐��p��static�N���X
	// [���l]
	// ���ׂĂ̊֐��́Astatic�Ő錾����܂��B
	//-------------------------------------------------------------------------
	class util {
	public:
		// �o�b�t�@����CRC16���v�Z����
		static unsigned short calcCRC16(unsigned char *p, unsigned int n);

		// �t�@�C���n���h������CRC16���v�Z����
		static unsigned short calcCRC16(FILE* fp);

		// �t�@�C��������CRC16���v�Z����
		static unsigned short calcCRC16(const _tstring& pfname);

		// ������I�[�̉��s����菜��
		static _tstring& chomp(_tstring& s);

		// CopyFile API�Ɠ��l�̓���������֐�(�G���[�ӏ����ڍׂɓ��肵�����Ƃ��Ɏg�p)
		// [�߂�l]  1   ����I��(�R�s�[��̓ǂݍ��ݑ����𗎂Ƃ���)
		// [�߂�l]  0   ����I��
		// [�߂�l] -1   �R�s�[���t�@�C���̃I�[�v���Ɏ��s
		// [�߂�l] -2   �R�s�[��t�@�C���̃I�[�v���Ɏ��s
		// [�߂�l] -3   �R�s�[���t�@�C���̓ǂݍ��݂Ɏ��s
		// [�߂�l] -4   �R�s�[��t�@�C���̏������݂Ɏ��s
		// [�߂�l] -5   �R�s�[��t�@�C���Ɏw��T�C�Y�������݂ł��Ȃ�
		// [�߂�l] -6   �R�s�[���t�@�C���̃^�C���X�^���v�擾���s
		// [�߂�l] -7   �R�s�[��t�@�C���̃^�C���X�^���v�X�V���s
		// [�߂�l] -8   �R�s�[���t�@�C�������̂Ɏ��s
		// [�߂�l] -9   �R�s�[��t�@�C�������̂Ɏ��s
		// [�߂�l] -10  �R�s�[���t�@�C���̑����擾���s
		// [�߂�l] -11  �R�s�[��t�@�C���̑����X�V���s
		// [�߂�l] -12  �R�s�[��t�@�C���̑����擾���s
		// [�߂�l] -13  �R�s�[��t�@�C���̓ǂݍ��ݑ������Z�b�g���s
		static int copyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists, BOOL bFailIfReadonly=TRUE);

		// �R�s�[������ CopyFile API ���g���B
		// �R�s�[��̓Ǎ������𗎂Ƃ��@�\�t���B
		//
		// [�߂�l]  1   ����I��(�R�s�[��̓ǂݍ��ݑ����𗎂Ƃ���)
		// [�߂�l]  0   ����I��
		// [�߂�l] -1   CopyFile API�̎��s
		// [�߂�l] -12  �R�s�[��t�@�C���̑����擾���s
		// [�߂�l] -13  �R�s�[��t�@�C���̓ǂݍ��ݑ������Z�b�g���s
		static int copyFile2(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists, BOOL bFailIfReadonly=TRUE);

		// �t�H���_���R�s�[����B
		//
		// pCopyFolderInfo �� !NULL �̏ꍇ�A�G���[�������̏ڍ׏����L�^����B
		static int copyFolder(const _tstring& srcFolder, const _tstring& dstFolder, bool (*funcIsExclude)(const _tstring& strFname) = NULL, copyFolderInfo* pCopyFolderInfo = NULL, bool ignoreCopyError = false);

		// �f�B���N�g�����ċA�I�ɍ쐬����(
		static bool createDirectory(const _tstring& strPathName);

		// Windows���V���b�g�_�E��������
		static int exitWindows();

		// ���ϐ���W�J����
		static DWORD expandEnvironmentStrings(LPCTSTR lpSrc, std::vector<TCHAR>& rDst);
		//static DWORD expandEnvironmentStrings();

		// �V�X�e���W���̃G���[���b�Z�[�W���쐬
		static _tstring formatMessageBySystem(DWORD dwErr);

		// �ėp�I�ȃt�H�[�}�b�g���b�Z�[�W�̏���
		static DWORD formatMessage( DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, std::vector<TCHAR>& buffer, va_list* Arguments );

		// GetComputerNameEx�֐�(vc6�p)
		static BOOL __GetComputerNameEx(COMPUTER_NAME_FORMAT NameType, LPTSTR lpBuffer, LPDWORD nSize);

		// string��Ԃ�GetComputerNameEx�֐�
		// [��O] throw ::GetLastError();
		static _tstring getComputerNameEx(COMPUTER_NAME_FORMAT NameType);

		// string��Ԃ�GetDlgItemText�֐�
		// [����] hDlg        �_�C�A���O�{�b�N�X�̃n���h��
		// [����] nIDDlgItem  �R���g���[���̎��ʎq
		// [��O] throw ::GetLastError();
		static _tstring getDlgItemText(HWND hDlg, int nIDDlgItem);

		static _tstring getEnvironmentVariable(LPCTSTR lpName);

		// �w��t�@�C���̃T�C�Y��Ԃ�(GetFileSize API�̃��b�p�[)
		// [����]  lpszFileName  �t�@�C����
		// [�߂�l] 64bit�̃t�@�C���T�C�Y
		static __int64 getFileSize(LPCTSTR lpszFileName);

		// �t�@�C���̃o�[�W�������\�[�X���擾
		static _tstring getFileVersion(const _tstring &strFileName);

		// �J�����g���W���[��(���s�t�@�C��)�̃o�[�W�������\�[�X���擾
		static _tstring getFileVersion() { return kjm::util::getFileVersion(kjm::util::getModuleFileName()); }

		// Win32API GetModuleFileName �����S�Ɏg���֐�
		static _tstring getModuleFileName(HMODULE hModule = NULL);

		// GetPrivateProfileString�֐��̃��b�p�[
		static _tstring getPrivateProfileString(LPCTSTR lpSectionName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPCTSTR lpFileName);

		// �e���|�����t�@�C���p�̃f�B���N�g���̃p�X���擾���܂��B
		static _tstring getTempPath() {
			std::vector<TCHAR> work(MAX_PATH);
			GetTempPath(work.size(), &work[0]);
			return &work[0];
		}

		// LoadString�֐��̃��b�p�[
		static _tstring load_string(UINT uID, HINSTANCE hInstance = NULL);

		// ���[�N�G���A�̒��S�ɃE�B���h�E���ړ�����
		static BOOL moveCenterWorkarea(HWND hwnd);

		// PathRenameExtension�̃��b�p�[
		static _tstring& pathRenameExtension(_tstring& strPath, LPCTSTR pszExt);

		// �f�B���N�g�����ċA�I�ɍ폜����
		static bool removeFolder(LPCTSTR pszPath);

		// SetComputerNameEx�֐�(vc6�p)
		static BOOL __SetComputerNameEx(COMPUTER_NAME_FORMAT NameType, LPCTSTR lpBuffer);

		// SetLayeredWindowAttributes�֐�(vc6�p)
		static BOOL setLayeredWindowAttributes(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);

		// �������u������BVB��StrReplace�̂悤�Ȋ֐��B
		static _tstring strReplace(const _tstring& src, const _tstring& strFind, const _tstring& strReplace);

		// Unicode ������� Ansi ������ɕϊ�
		static std::string toAnsi(const std::wstring& src);

		// Ansi ������� Unicode ������ɕϊ�
		static std::wstring toUnicode(const std::string& src);

		// ���l�� human readable format �ȕ�����ɂ���
		static _tstring toHumanReadable(__int64 value);

		// �������������������
		static _tstring toLower(const _tstring& src);

		// �������啶��������
		static _tstring toUpper(const _tstring& src);

		// ���S�� vsprintf
		static int vsprintf_vec(std::vector<TCHAR>& buf, LPCTSTR format, va_list argptr);

		// ���S�� sprintf 
		//
		// [����]
		// ��Ԗڂ̈������Avector<TCHAR>�ւ̎Q�ƂɂȂ��Ă���ȊO�A
		// sprintf�֐��Ɠ����ł��B
		static int sprintf_vec(std::vector<TCHAR>& buf, LPCTSTR format, ...);

		// �����g���[�X�̂��߂̃��O�I�u�W�F�N�g���w�肷��
		static void set_logObj(kjm::log* pLogObj);

		//---------------------------------------------------------------------
		// pszPath�Ŏw�肵���f�B���N�g�����́A���ݓ�������nDay���Â��t�@�C��
		// ���폜���܂��B
		//
		// �폜�Ώۂ́ApszPath�����̃t�@�C�������ł��B
		//---------------------------------------------------------------------
		static void cleanupDir(LPCTSTR pszPath, int nDay);

		// UNICODE�������Shift JIS������ɕϊ�
		static std::string convertToAscii(const std::wstring& src);

		// Shift JIS�������UNICODE������ɕϊ�
		static std::wstring convertToUnicode(const std::string& src);

	private:
		util() {};
		~util() {};
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
	public:
		// �f�t�H���g�R���X�g���N�^
		dateTime(kjm::dateTimeKind kind = kjm::unspecified) : m_kind(kind) { memset(&m_ft, 0, sizeof(&m_ft)); }; 
		
		/// �R�s�[�R���X�g���N�^
		dateTime(const kjm::dateTime& src) : m_kind(src.m_kind), m_ft(src.m_ft) {}

		// time_t �ŏ���������R���X�g���N�^
		dateTime(time_t t, kjm::dateTimeKind kind = kjm::unspecified) : m_kind(kind) { *(__int64*)&m_ft = (__int64)t * FT_1SEC + FT_19700101; };

		// FILETIME �ŏ���������R���X�g���N�^
		dateTime(FILETIME ft, kjm::dateTimeKind kind = kjm::unspecified) : m_ft(ft), m_kind(kind) {};

		// SYSTEMTIME �ŏ���������R���X�g���N�^
		dateTime(const SYSTEMTIME& st, kjm::dateTimeKind kind = kjm::unspecified) : m_kind(kind) { ::SystemTimeToFileTime(&st, &m_ft); }
		
		// ������\�������񕶎���(yyyy/mm/dd hh:mm:ss)�ŏ���������R���X�g���N�^
		dateTime(LPCTSTR time_text, kjm::dateTimeKind kind = kjm::unspecified) : m_kind(kind) { parse(time_text); };
		
		// �f�X�g���N�^
		virtual ~dateTime() {};

		// �ێ�������t���Actime �֐��ŕ�����ɂ��ĕԂ��B
		_tstring ctime() {
			TCHAR buffer[32];
			time_t t = getTimeT();
			::_tctime_s(buffer, _countof(buffer), &t);
			return util::chomp(_tstring(buffer));
		}

		/// UTC�����[�J���^�C���ɕϊ�����
		dateTime &toLocalTime() {
			if (this->m_kind == kjm::local) return *this;
			this->m_kind = kjm::local;
			FileTimeToLocalFileTime(&m_ft, &m_ft);
			return *this;
		}

		/// <summary>UTC �ɕϊ�����</summary>
		dateTime& toUniversalTime() {
			if (this->m_kind == kjm::utc) return *this;
			this->m_kind = kjm::utc;
			::LocalFileTimeToFileTime(&m_ft, &m_ft);
			return *this;
		}

		// ������ɕϊ�
		virtual _tstring toString() const;

		// ���Ԃ𕶎���(YYYYMMDD)�ɕϊ�
		virtual _tstring toString_yyyymmdd() const;

		// �����������Ă��Ȃ����ǂ������ׂ�
		bool isEmpty() { return (getTime() == 0); };

		// �N��Ԃ�
		int year() const {
			SYSTEMTIME st;
			FileTimeToSystemTime(&m_ft, &st);
			return st.wYear;
		}
		
		// ����Ԃ�
		int month() const {
			SYSTEMTIME st;
			FileTimeToSystemTime(&m_ft, &st);
			return st.wMonth;
		}
		
		// ����Ԃ�
		int date() const {
			SYSTEMTIME st;
			FileTimeToSystemTime(&m_ft, &st);
			return st.wDay;
		}
		
		// ����Ԃ�
		int hour() const {
			SYSTEMTIME st;
			FileTimeToSystemTime(&m_ft, &st);
			return st.wHour;
		}
		
		// ����Ԃ�
		int minute() const {
			SYSTEMTIME st;
			FileTimeToSystemTime(&m_ft, &st);
			return st.wMinute;
		}
		
		// �b��Ԃ�
		int second() const {
			SYSTEMTIME st;
			FileTimeToSystemTime(&m_ft, &st);
			return st.wSecond;
		}
		
		// �j����Ԃ�
		int weekday() const {
			SYSTEMTIME st;
			FileTimeToSystemTime(&m_ft, &st);
			return st.wDayOfWeek;
		}

		// �b��ǉ�����
		dateTime& addSecond(int sec) {
			*(__int64*)&m_ft *= (sec * FT_1SEC);
			return *this;
		}

		// ���ݎ��Ԃ�time_t�ŕԂ�
		time_t getTimeT() const { return getTime() / FT_1SEC - FT_19700101_SEC; };
	
		// ���Ԃ�time_t�Őݒ肷��
		void setTimeT(time_t value) { setTime(value * FT_19700101); };
		
		// ���ݎ�����__int64�ŕԂ�
		__int64 getTime() const { return *(__int64*)&m_ft; };

		// ���Ԃ�__int64�Őݒ肷��
		void setTime(__int64 value) { *(__int64*)&m_ft = value; };

		// ���Ԃ��t�H�[�}�b�g���ĕ�����ɂ���
		_tstring strftime(LPCTSTR format);

		// ������\�������񕶎���(yyyy/mm/dd hh:mm:ss)�ŃI�u�W�F�N�g������������
		void parse(LPCTSTR time_text);

		// ������Z�q(time_t�^)
		void operator=(time_t t) { setTimeT(t); };
		
		// ������Z�q(������\��������(yyyy/mm/dd hh:mm:ss))
		void operator=(LPCTSTR time_text) { parse(time_text); };

		// ���ԍ����v�Z����
		double operator-(const dateTime& rhs) const { time_t t = getTimeT(); return difftime(t, rhs.getTimeT()); };
		
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

		// �t�@�C���ɕۑ�����
		bool save(FILE* fp);
		
		// �t�@�C������ǂݍ���
		bool load(FILE* fp);

	public:
		/// ���̃C���X�^���X���\�������̎�� (���n�����A���E���莞�� (UTC)�A�܂��́A���̂ǂ���ł��Ȃ�) �������l���擾���܂��B
		kjm::dateTimeKind get_kind() { return this->m_kind; }

	public:

		// ���ݓ�����ێ�����C���X�^���X��Ԃ��B
		static dateTime getCurrentTime() {
			return dateTime(::time(NULL), kjm::utc);
		};

		// ���݂̃��[�J���������擾����
		static dateTime getLocalTime() {
			SYSTEMTIME st;
			::GetLocalTime(&st);
			return dateTime(st, kjm::local);
		}

		// ���݂�UTC�������擾����
		static dateTime getSystemTime() {
			SYSTEMTIME st;
			::GetSystemTime(&st);
			return dateTime(st, kjm::utc);
		}

	private:
		///time_t m_time;
		FILETIME m_ft;
		kjm::dateTimeKind m_kind;
	};

	//-----------------------------------------------------------------
	// �R�}���h���C���������
	//-----------------------------------------------------------------
	class commandLine {
	public:
		// �f�t�H���g �R���X�g���N�^
		commandLine() {}

		// ������͕t���R���X�g���N�^
		commandLine(int argc, TCHAR** argv, TCHAR* hasArgShortOpts) { parse(argc, argv, hasArgShortOpts); }

		// �R�s�[�R���X�g���N�^
		commandLine(const commandLine& src) : m_optList(src.m_optList), m_argList(src.m_argList) {}

		// �f�X�g���N�^
		virtual ~commandLine() {}

		// �������
		//
		// @arg  hasArgShortOpts  �����������Ƃ�����V���[�g�I�v�V������񋓂���������ւ̃|�C���^
		//
		int parse(int argc, TCHAR** argv, TCHAR* hasArgShortOpts);

		// �����ꗗ���擾����
		std::vector<_tstring> getArgList() { return m_argList; }

		// �I�v�V�����ȊO�̈����̐���Ԃ�
		int getArgListCount() const { return m_argList.size(); };

		// �I�v�V�����ꗗ���擾����
		std::vector<_tstring> getOptList() const;

		// �I�v�V���������������ǂ������ׂ�
		bool hasKey(const _tstring& key) const { return (m_optList.find(key) != m_optList.end()); }

		// �I�v�V�����̈������擾����
		_tstring getOptArg(const _tstring& key) const;

		// ������Z�q
		commandLine& operator =(const commandLine& rhs) {
			if (this != &rhs) {
				m_optList = rhs.m_optList;
				m_argList = rhs.m_argList;
			}
			return *this;
		}

	private:
		// ��͂����I�v�V������ۑ�����ϐ�
		std::map<_tstring, _tstring> m_optList;

		// �I�v�V�����ȊO�̈�����ۑ�����ϐ�
		std::vector<_tstring> m_argList;
	};

	//-----------------------------------------------------------------
	// �C�x���g���O��1���R�[�h�����Ǘ�����N���X
	//-----------------------------------------------------------------
	class eventLogRecord {
	public:
		// �C�x���g���O�̃��R�[�h�ԍ���Ԃ�
		DWORD RecordNumber() { return getRaw()->RecordNumber; };
		
		// �C�x���g���O�̍쐬������Ԃ�
		dateTime TimeGenerated() { return dateTime(getRaw()->TimeGenerated, kjm::utc); };
		
		// �C�x���g���O�̃J�e�S����Ԃ�
		WORD EventCategory() { return getRaw()->EventCategory; };
		
		// �C�x���g���O�̃J�e�S���������Ԃ�
		_tstring EventCategoryText() { return DispMessage(get_SourceName().c_str(), _T("CategoryMessageFile"), NULL, EventCategory()); };
		
		// �C�x���g���O��ID�������Ԃ�
		_tstring EventIDText() {
			std::vector<TCHAR*> Args;
			GetArgs(getRaw(), Args);

			return DispMessage(get_SourceName().c_str(), _T("EventMessageFile"), reinterpret_cast<TCHAR **>(&Args[0]), get_EventID());
		};
		
		// �C�x���g���O�̃f�[�^����Ԃ�
		DWORD DataLength() { return getRaw()->DataLength; };
		
		// �C�x���g���O�f�[�^�ւ̃I�t�Z�b�g��Ԃ�
		DWORD DataOffset() { return getRaw()->DataOffset; };
		
		// �C�x���g���O�̃R���s���[�^�����擾
		_tstring ComputerName();
		
		// �C�x���g���O�̃f�[�^���擾
		std::vector<BYTE> Data();

		// �C�x���g�̓��t�������擾
		dateTime get_TimeWritten() { return dateTime(getRaw()->TimeWritten, kjm::utc); };

		// �C�x���g�^�C�v(���)���擾
		// @retval EVENTLOG_ERROR_TYPE, EVENTLOG_WARNING_TYPE, EVENTLOG_INFORMATION_TYPE, EVENTLOG_AUDIT_SUCCESS, EVENTLOG_AUDIT_FAILURE �̂ǂꂩ
		WORD get_EventType() { return getRaw()->EventType; };

		// �C�x���g�\�[�X���擾
		_tstring get_SourceName() { return ((LPTSTR)&m_raw_record[0] + sizeof(EVENTLOGRECORD)); };

		// �C�x���gID���擾
		DWORD get_EventID() {
			EVENTLOGRECORD* p = getRaw();
			return p->EventID;
		};

	public:
		// �f�t�H���g�R���X�g���N�^
		eventLogRecord() {};

		// �������t���R���X�g���N�^
		eventLogRecord(LPCTSTR lpSourceName, const EVENTLOGRECORD* src);

		// �R�s�[�R���X�g���N�^
		eventLogRecord( const eventLogRecord& src )
			: m_source_name( src.m_source_name ), m_raw_record( src.m_raw_record ) {};
			
		// �f�X�g���N�^
		virtual ~eventLogRecord() {};

		// ������Z�q
		kjm::eventLogRecord& operator= ( const kjm::eventLogRecord& src );

	private:
		_tstring m_source_name;		// �C�x���g�\�[�X
		std::vector<BYTE> m_raw_record;	// �����
		
		// �������L���X�g����
		EVENTLOGRECORD* getRaw() { return (EVENTLOGRECORD*)&m_raw_record[0]; };

		_tstring DispMessage(LPCTSTR SourceName, LPCTSTR EntryName, TCHAR **Args, DWORD MessageId);

		int GetArgs(const EVENTLOGRECORD *pBuf, std::vector<TCHAR*>& args );

		BOOL GetModuleNameFromSourceName(LPCTSTR SourceName, LPCTSTR EntryName, std::vector<TCHAR>& ExpandedName);
	};

	//-----------------------------------------------------------------
	// �C�x���g���O���Ǘ�����N���X
	//-----------------------------------------------------------------
	class eventLog {
	public:
		// �f�t�H���g�R���X�g���N�^
		eventLog() : m_hEventLog(NULL) {};

		// �f�X�g���N�^
		virtual ~eventLog() { closeEventLog(); };

	public:
		// �C�x���g���O���J��(OpenEventLog API�Q��)
		//
		// lpSourceName�ɂ͒ʏ�A"Application", "Security", "System"�̂ǂꂩ���w�肷��B
		BOOL openEventLog(LPCTSTR lpUNCServerName, LPCTSTR lpSourceName);

		// �o�b�N�A�b�v�C�x���g���O���J��
		BOOL OpenBackupEventLog( LPCTSTR lpUNCServerName, LPCTSTR lpSourceName, LPCTSTR lpFileName );

		// �C�x���g���O�����
		BOOL closeEventLog();

		// �C�x���g��1���R�[�h�ǂݍ���
		BOOL readEventLog(DWORD dwReadFlags, DWORD dwRecordOffset, eventLogRecord& record);
		
		// �C�x���g���O���o�b�N�A�b�v����
		BOOL BackupEventLog( LPCTSTR lpBackupFileName ) {
			return ::BackupEventLog( m_hEventLog, lpBackupFileName );
		};

	private:
		// �R�s�[�R���X�g���N�^(�R�s�[�֎~)
		eventLog(const eventLog& src) {};

		// ������Z�q(����֎~)
		eventLog& operator=(const eventLog& rhs) { return *this; };

	private:
		HANDLE m_hEventLog;
		_tstring m_uncServerName;
		_tstring m_sourceName;
		_tstring m_fileName;
	};

	//-------------------------------------------------------------------------
	// Path�֘A�֐��p�X�^�e�B�b�N�N���X
	//-------------------------------------------------------------------------
	class path {
	public:
		// �p�X����������
		static _tstring append(const _tstring& strPath, const _tstring& strMore) {
			TCHAR szTemp[MAX_PATH];
			PathAppend(lstrcpy(szTemp, strPath.c_str()), strMore.c_str());
			return szTemp;
		}

		// PathRemoveExtension �֐��̃��b�p�[
		static _tstring removeExtension(const _tstring& strPath) {
			TCHAR szTemp[MAX_PATH];
			PathRemoveExtension(lstrcpy(szTemp, strPath.c_str()));
			return szTemp;
		}

		// �t���p�X����t�@�C��������菜��
		static _tstring removeFileSpec(const _tstring& strPath) {
			TCHAR szTemp[MAX_PATH];
			PathRemoveFileSpec(lstrcpy(szTemp, strPath.c_str()));
			return szTemp;
		}

		// �t���p�X����t�@�C������ύX����
		static _tstring renameFileSpec(const _tstring& strPath, const _tstring& strFileSpec) {
			return path::append(path::removeFileSpec(strPath), strFileSpec);
		}

		// �g���q��ύX����
		static _tstring renameExtension(const _tstring& pszPath, const _tstring& pszExt) {
			TCHAR szTemp[MAX_PATH];
			PathRenameExtension(lstrcpy(szTemp, pszPath.c_str()), pszExt.c_str());
			return szTemp;
		}

		// �t�@�C�������݂��邩�ǂ������ׂ�
		static bool fileExists(const _tstring& strPath) {
			return (PathFileExists(strPath.c_str()) != FALSE);
		}

		// �t���p�X����A�t�@�C���������o��
		static _tstring findFileName(const _tstring& strPath) {
			return PathFindFileName(strPath.c_str());
		}

		// �f�B���N�g�����ǂ������ׂ�
		static bool isDirectory(const _tstring& strPath) {
			return (PathIsDirectory(strPath.c_str()) != FALSE);
		}

	private:
		path() {};
		~path() {};
	};

	//-------------------------------------------------------------------------
	// �t�@�C���������T�|�[�g����N���X
	//-------------------------------------------------------------------------
	class findFile {
	public:
		// �f�t�H���g�R���X�g���N�^
		findFile() : m_hFind(INVALID_HANDLE_VALUE), m_strWildcard(_T("")) {};

		// �f�X�g���N�^
		virtual ~findFile() { findClose(); };

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
		};

		// ���������t�@�C�����A�f�B���N�g�����ǂ������ׂ�
		virtual bool isDirectory() {
			return ((m_findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
		};

		// ���������t�@�C�����A�ǂݍ��ݐ�p���ǂ������ׂ�
		virtual bool isReadonly() {
			return ((m_findData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0);
		}

		// ���������t�@�C���̃t�@�C�����������擾
		virtual _tstring getFileName() { return m_findData.cFileName; };

		// ���������t�@�C���̃t���p�X�����擾
		virtual _tstring getFullName() { return path::renameFileSpec(m_strWildcard, m_findData.cFileName); };

		// ���������t�@�C���̑�����Ԃ�
		virtual DWORD getFileAttributes() { return m_findData.dwFileAttributes; }

		// ���������t�@�C���̃T�C�Y���擾
		virtual __int64 getFileSize() {
			LARGE_INTEGER li = {m_findData.nFileSizeLow, m_findData.nFileSizeHigh};
			return li.QuadPart;
		};

		dateTime getLastWriteTime() const { return dateTime(m_findData.ftLastWriteTime, kjm::utc); }

		// ���������t�@�C���̑S���� WIN32_FIND_DATA �^�ɃR�s�[����
		virtual void copyWin32FindData(WIN32_FIND_DATA& rBuf) { rBuf = m_findData; }

	private:
		_tstring m_strWildcard;

		HANDLE m_hFind;

		WIN32_FIND_DATA m_findData;
	};

	enum fileAttributes {
		readOnly = FILE_ATTRIBUTE_READONLY,						// ReadOnly = 1,
		hidden = FILE_ATTRIBUTE_HIDDEN,							// Hidden = 2,
		system = FILE_ATTRIBUTE_SYSTEM,							// System = 4,
		directory = FILE_ATTRIBUTE_DIRECTORY,					// Directory = 16,
		archive = FILE_ATTRIBUTE_ARCHIVE,						// Archive = 32,
		device = FILE_ATTRIBUTE_DEVICE,							// Device = 64,
		normal = FILE_ATTRIBUTE_NORMAL,							// Normal = 128,
		temporary = FILE_ATTRIBUTE_TEMPORARY,					// Temporary = 256,
		sparseFile = FILE_ATTRIBUTE_SPARSE_FILE,				// SparseFile = 512,
		reparsePoint = FILE_ATTRIBUTE_REPARSE_POINT,			// ReparsePoint = 1024,
		compressed = FILE_ATTRIBUTE_COMPRESSED,					// Compressed = 2048,
		offline = FILE_ATTRIBUTE_OFFLINE,						// Offline = 4096,
		notContentIndexed = FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,	// NotContentIndexed = 8192,
		encrypted = FILE_ATTRIBUTE_ENCRYPTED,					// Encrypted = 16384,
	};


	class file {
	private:
		file() {}
		~file() {}

	public:
		// �t�@�C���̑������擾���܂��B
		static kjm::fileAttributes getAttributes(const _tstring &path) { return (kjm::fileAttributes)::GetFileAttributes(path.c_str()); }

		static void setAttributes(const _tstring& path, kjm::fileAttributes fa) { ::SetFileAttributes(path.c_str(), fa); }
	};

	//-------------------------------------------------------------------------
	// �t�@�C�� ��\������N���X
	//-------------------------------------------------------------------------
	class fileInfo {
	public:
		// �f�t�H���g�R���X�g���N�^
		fileInfo() : m_hasInfo(false) {};

		// �R�s�[�R���X�g���N�^
		fileInfo(const fileInfo& src) : m_fullPath(src.m_fullPath), m_lastWriteTime(src.m_lastWriteTime), m_fileSize(src.m_fileSize), m_dwFileAttributes(src.m_dwFileAttributes), m_hasInfo(src.m_hasInfo) {};

		// WIN32_FIND_DATA �ŏ���������R���X�g���N�^
		fileInfo(const _tstring& strParentPath, const WIN32_FIND_DATA& rFindData) {
			m_fullPath = path::append(strParentPath, rFindData.cFileName);
			m_lastWriteTime = dateTime(rFindData.ftLastWriteTime, kjm::utc);

			LARGE_INTEGER li = {rFindData.nFileSizeLow, rFindData.nFileSizeHigh};
			m_fileSize = li.QuadPart;

			m_dwFileAttributes = rFindData.dwFileAttributes;

			m_hasInfo = true;	// [���������Ă���]�ŁA������
		};

		// �t�@�C�����ŏ���������R���X�g���N�^
		fileInfo(const _tstring& strName) : m_fullPath(strName) {
			findFile ff;
			if (ff.findFirstFile(strName)) {
				m_lastWriteTime = ff.getLastWriteTime();
				m_fileSize = ff.getFileSize();
				m_dwFileAttributes = ff.getFileAttributes();
	
				m_hasInfo = true;	// [���������Ă���]�ŁA������
			} else {
				m_hasInfo = false;	// [���������Ă��Ȃ�]�ŁA������
			}
		};

		// �f�X�g���N�^
		virtual ~fileInfo() {};

		// ������Z�q
		fileInfo& operator =(const fileInfo& rhs) {
			if (this != &rhs) m_fullPath = rhs.m_fullPath;
			return *this;
		};

		/// �t�@�C�������݂��邩�ǂ����������l���擾���܂��B
		bool exists() const { return kjm::path::fileExists(this->m_fullPath); }

		/// ��΃p�X���擾
		_tstring fullName() const { return m_fullPath; };

		// �t�@�C����������Ԃ�
		//_tstring getFileName() { return path::findFileName(m_fullPath); };
		_tstring name() const { return kjm::path::findFileName(m_fullPath); }

		/// <summary>�ŏI�X�V�������擾</summary>
		/// <returns>�ŏI�X�V���������� kjm::dateTime �I�u�W�F�N�g�B</returns>
		dateTime lastWriteTime() const { return kjm::dateTime(m_lastWriteTime).toLocalTime(); }

		// �t�@�C���̑�����Ԃ�
		fileAttributes attributes() {
			assert(m_hasInfo != false);
			return (kjm::fileAttributes)m_dwFileAttributes;
		}

		// �t�@�C���̃T�C�Y��Ԃ�
		__int64 length() const {
			assert(m_hasInfo != false);
			return m_fileSize;
		}

		// �t�@�C�����u�A�[�J�C�u�\�v���ǂ������ׂ�
		bool isArchive() {
			assert(m_hasInfo != false);
			return ((m_dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0);
		}

		// �t�@�C�����u�B���t�@�C���v���ǂ������ׂ�
		bool isHidden() {
			assert(m_hasInfo != false);
			return ((m_dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0);
		}

		// �t�@�C�����u�ǂݎ���p�v���ǂ������ׂ�
		bool isReadonly() {
			assert(m_hasInfo != false);
			return ((m_dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0);
		}

		// �t�@�C�����u�V�X�e���v���ǂ������ׂ�
		bool isSystem() {
			assert(m_hasInfo != false);
			return ((m_dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0);
		}

		// �t�@�C���̃o�[�W��������Ԃ�
		_tstring getFileVersion() { return kjm::util::getFileVersion(m_fullPath); };

	private:
		_tstring m_fullPath;		// �t�@�C���̃t���p�X
		dateTime m_lastWriteTime;	// �ŏI�X�V����
		__int64 m_fileSize;			// �t�@�C���̃T�C�Y
		DWORD m_dwFileAttributes;	// �t�@�C���̑���

	private:
		// ����ێ����Ă��邩�ǂ����̃t���O�B
		// �t�@�C���T�C�Y�A�ŏI�X�V���t�Ȃǂ̏��������Ă���Ƃ��� true
		// WIN32_FIND_DATA �Ő�������Ă���Ƃ��́Atrue �ɂȂ�B
		bool m_hasInfo;
	};

	//-------------------------------------------------------------------------
	// �f�B���N�g�� ��\������N���X
	//-------------------------------------------------------------------------
	class folderInfo {
	public:
		// �f�t�H���g�R���X�g���N�^
		folderInfo() {};

		// �R�s�[�R���X�g���N�^
		folderInfo(const folderInfo& src) : m_fullPath(src.m_fullPath) {};

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
		_tstring getFileName() { return path::findFileName(m_fullPath); };

		// �f�B���N�g�������̃t�@�C���ꗗ���擾
		std::vector<fileInfo> getFiles(const _tstring& strWC = _T("*.*"));

		// �f�B���N�g�������̃f�B���N�g���ꗗ���擾
		std::vector<folderInfo> getSubFolders();

		// �f�B���N�g���̃t���p�X��Ԃ�
		_tstring getFullPath() { return m_fullPath; };

	public:
		_tstring m_fullPath;	// �f�B���N�g���̃t���p�X
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
		iniFile(const _tstring& szFileName) : m_fileName(szFileName) {};

		// �f�X�g���N�^
		virtual ~iniFile() {};

		// ������Z�q
		kjm::iniFile& operator=(const iniFile& rhs) {
			if (this == &rhs) return *this;
			this->m_fileName = rhs.m_fileName;
			return *this;
		};

		// �Z�N�V�����ꗗ���擾
		std::vector<_tstring> getSectionNames() const;

		// �Z�N�V�������̃L�[�ꗗ���擾
		std::vector<_tstring> getKeyNames(const _tstring& strSectionName) const;

		// ini�t�@�C������l���擾
		_tstring getString(const _tstring& strSection, const _tstring& strKey, const _tstring& strDefault, BOOL* pfUseDefault = NULL) const;

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
	};

	//-------------------------------------------------------------------------
	// ���C�u�������W���[�����Ǘ�����N���X
	//-------------------------------------------------------------------------
	class library {
	private:
		HINSTANCE m_hLibModule;

	public:
		// �Ǘ����Ă���C���X�^���X�̃n���h�����擾
		HINSTANCE getInstance() { return m_hLibModule; };

	public:
		// �R���X�g���N�^
		library() : m_hLibModule( NULL ) {};
		
		// �f�X�g���N�^
		virtual ~library() { FreeLibrary(); };

	public:
		// ���C�u���������[�h����
		BOOL LoadLibraryEx( LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags );
		
		// ���C�u�������������
		BOOL FreeLibrary();
	};

	//-------------------------------------------------------------------------
	// ���W�X�g�����Ǘ�����N���X
	//-------------------------------------------------------------------------
	class registry {
	private:
		HKEY m_hKey;

	public:
		// �n���h����Ԃ�
		HKEY getKey() { return m_hKey; };

	public:
		// �f�t�H���g�R���X�g���N�^
		registry() : m_hKey( NULL ) {};
		
		// �f�X�g���N�^
		virtual ~registry() { RegCloseKey(); };

	public:
		// �L�[���J��
		LONG RegOpenKeyEx( HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired );
		
		// �L�[�����
		LONG RegCloseKey();
		
		// �L�[�̒l���擾
		LONG RegQueryValueEx( LPCTSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, std::vector<BYTE>& data );
	};

	// �f�B���N�g����T�u�f�B���N�g����ʂ��āA�쐬�A�ړ��A����ї񋓂��邽�߂̐ÓI���\�b�h�����J���܂��B���̃N���X�͌p���ł��܂���B
	class directory {
	public:
		// �w�肵���p�X���f�B�X�N��̊����̃f�B���N�g�����Q�Ƃ��Ă��邩�ǂ������m�F���܂��B
		static bool exists(const _tstring& path) { return (::PathFileExists(path.c_str()) != FALSE); }

	private:
		directory() {};
		~directory() {};
	};

	//-------------------------------------------------------------------------
	// �X���b�h�Ǘ��N���X
	//-------------------------------------------------------------------------
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
#if defined(_MT)
	class thread {
	private:
		bool m_loop;
		HANDLE m_hThread;
		CRITICAL_SECTION* m_cs;

	public:
		thread() : m_hThread( NULL ), m_cs(NULL), m_loop(false) {
		};

		virtual ~thread() {
			close();
		};

		void close() {
			if (m_hThread != NULL) {
				CloseHandle(m_hThread);
				m_hThread = NULL;
			}
		};

		void lock() { if (m_cs) EnterCriticalSection( m_cs ); };
		void unlock() { if (m_cs) LeaveCriticalSection( m_cs ); };

		// ���b�N�Ɏg�p����N���e�B�J���Z�N�V������ݒ肷��
		void setCriticalSection(CRITICAL_SECTION* pcs) { m_cs = pcs; };

		// �X���b�h�����쒆���ǂ������ׂ�
		bool still_active() { 
			DWORD code;
			GetExitCodeThread( m_hThread, &code );
			return ( code == STILL_ACTIVE );
		};

		// �X���b�h���N������B
		void run() {
			m_loop = true;
			m_hThread = ( HANDLE )_beginthreadex( NULL, 0, thread_func, this, 0, NULL );
		};

		// �X���b�h���I��������B�h���N���X�Œ�`����B
		// �I�����@�́Aworker method �̎����Ɉˑ�����B
		virtual void end() {
			m_loop = false;
		};

		// �X���b�h�̈ꎞ��~
		DWORD suspend() {
			return SuspendThread( m_hThread );
		};

		// �X���b�h�̍ĊJ
		DWORD resume() {
			return ResumeThread( m_hThread );
		};

		// �X���b�h�ɌĂяo�����֐��B�h���N���X�Œ�`����B
		// end method �̌ďo�ŏI���ł���悤�Ɏ�������B
		virtual unsigned worker() = 0;

		// �X���b�h�̏I����҂B
		DWORD wait(DWORD timeout=INFINITE)
			{ return WaitForSingleObject(m_hThread, timeout); }

		bool still_loop() {
			return m_loop;
		};

		/*!
		 * ���ʂ̃X���b�h�֐��B
		 * args�ɂ́Athread�N���X����h�������N���X�I�u�W�F�N�g�ւ�
		 * �|�C���^���󂯎��Bp->worker() �����s���邾���B
		 */
		static unsigned __stdcall thread_func(void* args) {
			return ((thread*)args)->worker();
		}
	};
#endif

	//-------------------------------------------------------------------------
	// GetTickCount�Ōo�ߎ��Ԃ��v��N���X
	//-------------------------------------------------------------------------
	class tickCounter {
	public:
		// �f�t�H���g�R���X�g���N�^
		tickCounter() : m_t1(GetTickCount()), m_t2(0) {};

		// �f�X�g���N�^
		~tickCounter() {};

		// �v���J�n
		void start() { m_t1 = GetTickCount(); m_t2 = 0; };

		// �v���I��
		void stop() { m_t2 = GetTickCount(); };

		// �o�ߎ���(ms)
		DWORD elapse() { return (m_t2 == 0) ? (GetTickCount() - m_t1) : (m_t2 - m_t1); };

		// �o�ߎ���(sec)
		DWORD elapseSec() { return elapse() / 1000; };

		// �o�ߎ���(min)
		DWORD elapseMin() { return elapseSec() / 60; };

	private:
		DWORD m_t1;	// �J�n�������̎���(�R���X�g���N�^�Astart�ōX�V)
		DWORD m_t2;	// �I���������̎���(stop�ōX�V)
	};

	class process {
	public:
		process();
		virtual ~process() { close(); };

		virtual BOOL run(LPCTSTR pCommandLine);

		virtual DWORD wait(DWORD dwTimeout = INFINITE) {
			return WaitForSingleObject(m_pi.hProcess, dwTimeout);
		};

		virtual void close();

	private:
		STARTUPINFO m_si;
		PROCESS_INFORMATION m_pi;
	};


#define ENCODE_SHIFT_JIS			932				// shift_jis	���{�� (�V�t�g JIS)
#define ENCODE_UTF_16				1200			// utf-16		Unicode
#define ENCODE_UTF_16_BIG_ENDIAN	1201			// unicodeFFFE	Unicode (Big-Endian)
#define ENCODE_UTF_32				12000	
#define ENCODE_UTF_32_BIG_ENDIAN	12001
#define ENCODE_US_ASCII				20127			// us-ascii		US-ASCII
#define ENCODE_JIS					50220			// iso-2022-jp	���{�� (JIS)
#define ENCODE_EUC_JP				51932			// euc-jp		���{�� (EUC)
#define ENCODE_UTF_8				65001			// utf-8		Unicode (UTF-8)
#define ENCODE_UTF_8N				-(ENCODE_UTF_8)	// utf-8(BOM�t��)

	// �ȉ��̃y�[�W�̃\�[�X�R�[�h�����ɁAC++�p�ɏ��������܂����B
	// http://www.geocities.jp/gakaibon/tips/csharp2008/charset-check-samplecode4.html
	class encoding {
	private:
		encoding() {}
		~encoding() {}

	public:
		static int GetTextEncoding(const std::vector<BYTE>& bs);

		static bool IsJis(const std::vector<BYTE>& bs);

		static bool IsAscii(const std::vector<BYTE>& bs);

		static bool IsSJIS(const std::vector<BYTE>& bs, int& sjis);

		static bool IsEUC(const std::vector<BYTE>& bs, int& euc);

		static bool IsUTF8(const std::vector<BYTE>& bs, int& utf8, bool& bomFlag);

		static HRESULT convertINetString(
			LPDWORD lpdwMode,
			DWORD dwSrcEncoding,
			DWORD dwDstEncoding,
			LPCSTR lpSrcStr,
			LPINT lpnSrcSize,
			LPBYTE lpDstStr,
			LPINT lpnDstSize
		);
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
	// class mutex �̓������O�p�̌^
	//-------------------------------------------------------------------------
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

		// �ʓ|�Ȃ̂ŁA�C���X�^���X�̕����͕s���B
		mutex(const mutex &src) {};
		mutex& operator=(const mutex &src) {};

	public:

		// mutex �쐬���ɓ����� mutex �����݂����Ƃ��́Ature ��Ԃ�
		bool is_exists() { return (m_create_status == ERROR_ALREADY_EXISTS); };

		// mutex�쐬�ς݂��ǂ����̃e�X�g
		bool is_open() { return (m_hMutex != NULL); };

	public:

		// �R���X�g���N�^�ƃf�X�g���N�^
		mutex() : m_hMutex(NULL), m_create_status(0), m_lock_count(0) {};
		mutex(const _tstring& name) : m_hMutex(NULL), m_create_status(0), m_lock_count(0) {
			create(name);	// ��O�͋N���肦�Ȃ��B�`�F�b�N�́Ais_open�Ń`�F�b�N
		};
		virtual ~mutex() { close(); };

		// mutex �̍쐬
		// 
		// arg : name = mutex �̖��O
		// ret : true = ����I���Afalse = �ȏ�I��
		// exp : mutex_error
		// note:
		bool create(const _tstring& name) {
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
		BOOL unlock() {
			if (!m_hMutex)	throw mutex_error(mutex_error::not_initialized);
			
			BOOL ret = ::ReleaseMutex(m_hMutex);
			if (ret) {
				--m_lock_count;
			}

			return ret;
		};
	};

	//-------------------------------------------------------------------------
	// class memfile �̓������O�p�̌^
	//-------------------------------------------------------------------------
	struct memfile_error {
		enum error_code {
			not_initialized = 1,	// memfile ����������
			already_initialized		// memfile ���������ς�
		};
		error_code error;

		memfile_error(error_code e) : error(e) {};
	};

	//-------------------------------------------------------------------------
	// �������}�b�v�h�t�@�C���Ǘ��N���X
	//
	// note: �쐬�����n���h���́A�f�X�g���N�^�Ŕj�������B
	//-------------------------------------------------------------------------
	template<class T> class memfile {
	private:
		HANDLE m_hFile;
		DWORD m_create_status;
		T* m_pt;

		// �ʓ|�Ȃ̂ŁA�C���X�^���X�̕����͕s���B
		memfile<T>(const memfile<T> &src) {};
		memfile<T>& operator=(const memfile<T> &src) {};

	public:
		// memfile�쐬���ɓ�����memfile�����݂����Ƃ��́Ature ��Ԃ�
		bool is_exists() { return (m_create_status == ERROR_ALREADY_EXISTS); };

		// memfile�쐬�ς݂��ǂ����̃e�X�g
		bool is_open() { return (m_hFile != NULL); };

	public:
		// �R���X�g���N�^�ƃf�X�g���N�^
		memfile<T>() : m_hFile(NULL), m_create_status(0), m_pt(NULL) {};
		memfile<T>(LPCTSTR name, DWORD size = 0) : m_hFile(NULL), m_create_status(0), m_pt(NULL) {
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
		bool create(LPCTSTR name, DWORD size) {
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
		bool open(LPCTSTR name) {
			// �������ς�instance�̎��́A��O��throw
			if (m_hFile) throw memfile_error(memfile_error::already_initialized);

			// �������}�b�v�h�t�@�C�����J��
			m_hFile = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, name);
			return (m_hFile != NULL);
		};

		// �������}�b�v�h�t�@�C�������
		//
		// arg : �Ȃ�
		// ret : �Ȃ�
		// exp : �Ȃ�
		// note: ����������instance�Ŏg�p���Ă�ok
		void close() {
			if (m_hFile) {
				unmapViewOfFile();

				::CloseHandle(m_hFile);
				m_hFile = NULL;
			}
		};

		// �������}�b�v�h�t�@�C���̃|�C���^���擾
		T* mapViewOfFile(DWORD dwDesiredAccess=FILE_MAP_ALL_ACCESS, DWORD dwFileOffsetHigh=0, DWORD dwFileOffsetLow=0, DWORD dwNumberOfBytesToMap=0) {
			return (m_pt = (T*)MapViewOfFile(m_hFile, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap));
		}

		// �������}�b�v�h�t�@�C���̃|�C���^�����
		void unmapViewOfFile() {
			if (m_pt != NULL) {
				UnmapViewOfFile(m_pt);
				m_pt = NULL;
			}
		}

		// �������}�b�v�h�t�@�C������f�[�^��ǂݍ���
		//
		// arg : buffer = �ǂݍ��ރo�b�t�@�ւ̃|�C���^
		//       offset = �擪�ʒu�ւ̃I�t�Z�b�g
		//       size = �ǂݍ��ޗv�f��
		//       pmtx = �r������Ɏg�p����mutex�ւ̃|�C���^(NULL��)
		//       timeout = �r���^�C���A�E�g����
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

	class csvFile {
	public:
		csvFile() {}
		virtual ~csvFile() { if (is_open()) close(); }

	public:
		void open(LPCTSTR fname);
		void close() { m_ifs.close(); }
		int is_open() { return m_ifs.is_open(); }
		bool getline();

		_tstring operator[](int index) { return m_cols.at(index); }
		std::vector<_tstring>::size_type col_count() { return m_cols.size(); };

	private:
		void parse_csv(const _tstring& line);

	private:
		_tifstream m_ifs;
		std::vector<_tstring> m_cols;
	};
}

