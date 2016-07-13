//---------------------------------------------------------------------------------------
// kjmsub ver.0.2010.8.27
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
#include <tlhelp32.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "ws2_32.lib")

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
#define _otstream			std::wostream
#define _otstringstream		std::wostringstream
#else
#define _tstring			std::string
#define _tout				std::cout
#define _otstream			std::ostream
#define _otstringstream		std::ostringstream
#endif

// STL �� setlocale(LC_ALL, _T("japan")) ���������́A�ȉ��̒ʂ�B
// std::locale �R���X�g���N�^�͏�ɁA_MBCS��������󂯎��B
#define STL_SETLOCALE_JAPAN		std::locale::global(std::locale("japan"))

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
} COMPUTER_NAME_FORMAT ;
#endif

#if _MSC_VER <= 1200
#define WS_EX_LAYERED 0x80000
#define LWA_COLORKEY	1
#define LWA_ALPHA		2
#endif

// FILETIME �������₷�����Ԃɕϊ����邽�߂̒萔
#define FT_1USEC			10
#define FT_1MSEC			(FT_1USEC * 1000)
#define FT_1SEC				(FT_1MSEC * 1000)
#define FT_19700101			(116444736000000000)		// time_t �̊J�n�n�_��\��FILETIME�̒l
#define FT_19700101_SEC		(FT_19700101 / FT_1SEC)

// __int64 �� iostream �ɏo�͂��邽�߂̊֐�
#if _MSC_VER < 1500		// ���Ȃ��Ƃ��Avs2008����́A__int64���󂯎��ostream�����݂���̂ŁA�����ɂ���B
_otstream& operator<<(_otstream& os, __int64 i);
#endif

namespace kjm {
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
	// CPU �g�p�����擾����N���X
	//-------------------------------------------------------------------------
	class cpuUsage {
	public:
		// �f�t�H���g�R���X�g���N�^
		cpuUsage() {};

		// �f�X�g���N�^
		virtual ~cpuUsage() {};

		// �p�t�H�[�}���X�擾�J�n
		BOOL StartPerf() {
			m_osvi.dwOSVersionInfoSize = sizeof(m_osvi);
			GetVersionEx(&m_osvi);

			if (m_osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
				return StartPerf95();
			else if (m_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
				return StartPerfNT();

			return FALSE;
		}

		// �p�t�H�[�}���X�̎擾�I��
		BOOL StopPerf() {
			if (m_osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
				return StopPerf95();
			else if (m_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
				return StopPerfNT();

			return FALSE;
		}

		// �p�t�H�[�}���X�f�[�^�̎擾
		INT GetCPUUsage() {
			if (m_osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
				return GetCPUUsage95();
			else if (m_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
				return GetCPUUsageNT();

			SetLastError((DWORD)CO_E_WRONGOSFORAPP);	// �o�[�W�����G���[

			return -1;
		}

	private:
		// �p�t�H�[�}���X�̎擾�J�n(NT��)(�_�~�[�֐�)
		BOOL StartPerfNT() { return TRUE; }

		// �p�t�H�[�}���X�̎擾�J�n(95��)
		BOOL StartPerf95();

		// �p�t�H�[�}���X�̎擾�I��(NT��)(�_�~�[�֐�)
		BOOL StopPerfNT() { return TRUE; }

		// �p�t�H�[�}���X�̎擾�I��(95��)
		BOOL StopPerf95();

		// �p�t�H�[�}���X�f�[�^�̎擾�p�T�|�[�g�֐�(NT��)
		PVOID CounterDataPtr(PPERF_DATA_BLOCK pDataBlock, DWORD dwObjectNameTitleIndex, WCHAR* pszInstanceName, DWORD dwCounterNameTitleIndex);

		// �p�t�H�[�}���X�f�[�^�̎擾(NT��)
		INT GetCPUUsageNT();

		// �p�t�H�[�}���X�f�[�^�̎擾(95��)
		INT GetCPUUsage95();

	private:
		OSVERSIONINFO m_osvi;
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
		static bool createDirectory(LPCTSTR strPathName);

		// Windows���V���b�g�_�E��������
		// [����]
		// uFlags  ExitWindowsEx API ���Q��
		static int exitWindows(UINT uFlags);

		// ���ϐ���W�J����
		static DWORD expandEnvironmentStrings(LPCTSTR lpSrc, std::vector<TCHAR>& rDst);

		// �V�X�e���W���̃G���[���b�Z�[�W���쐬
		static _tstring formatMessageBySystem(DWORD dwErr);

		// �ėp�I�ȃt�H�[�}�b�g���b�Z�[�W�̏���
		static DWORD formatMessage( DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, std::vector<TCHAR>& buffer, va_list* Arguments );

		// �z�X�g���܂��́AIP�����񂩂�IP�A�h���X���擾
		static DWORD getAddr(LPCSTR lpName);

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

		// �w��t�@�C���̃T�C�Y��Ԃ�(GetFileSize API�̃��b�p�[)
		// [����]  lpszFileName  �t�@�C����
		// [�߂�l] 64bit�̃t�@�C���T�C�Y
		static __int64 getFileSize(LPCTSTR lpszFileName);

		// �t�@�C���̃o�[�W�������\�[�X���擾
		static _tstring getFileVersion(const _tstring &strFileName);

		// �t�@�C���̃o�[�W�������\�[�X���擾(�����Ȃ��o�[�W����)
		static _tstring getFileVersion() { return kjm::util::getFileVersion(kjm::util::getModuleFileName()); }

		// Win32API GetModuleFileName �����S�Ɏg���֐�
		static _tstring getModuleFileName(HMODULE hModule = NULL);

		// �e���|�����t�@�C���p�̃f�B���N�g���̃p�X���擾���܂��B
		static _tstring getTempPath() {
			std::vector<TCHAR> work(MAX_PATH);
			GetTempPath(work.size(), &work[0]);
			return &work[0];
		}

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

	private:
		util() {};
		~util() {};
	};

	//-------------------------------------------------------------------------
	// ���Ԃ������N���X
	//-------------------------------------------------------------------------
	class dateTime {
	public:
		// �f�t�H���g�R���X�g���N�^
		dateTime() { memset(&m_ft, 0, sizeof(&m_ft)); }; 
		
		// time_t �ŏ���������R���X�g���N�^
		dateTime(time_t t) { *(__int64*)&m_ft = (__int64)t * FT_1SEC + FT_19700101; };

		// FILETIME �ŏ���������R���X�g���N�^
		dateTime(FILETIME ft) : m_ft(ft) {};

		// SYSTEMTIME �ŏ���������R���X�g���N�^
		dateTime(const SYSTEMTIME& st) { ::SystemTimeToFileTime(&st, &m_ft); }
		
		// ������\�������񕶎���(yyyy/mm/dd hh:mm:ss)�ŏ���������R���X�g���N�^
		dateTime(LPCTSTR time_text) { parse(time_text); };
		
		// �f�X�g���N�^
		virtual ~dateTime() {};

		// �ێ�������t���Actime �֐��ŕ�����ɂ��ĕԂ��B
		_tstring ctime() {
			time_t t = getTimeT();
			return util::chomp(_tstring(::_tctime(&t)));
		};

		// UTC�����[�J���^�C���ɕϊ�����
		virtual dateTime &toLocalTime() {
			FileTimeToLocalFileTime(&m_ft, &m_ft);
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
		_tstring strftime(LPCTSTR format) {
			TCHAR buf[512];
			time_t t = getTimeT();
			::_tcsftime(buf, sizeof(buf), format, localtime(&t));
			return _tstring(buf);
		};

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

		// ���ݓ�����ێ�����C���X�^���X��Ԃ��B
		static dateTime getCurrentTime() {
			return dateTime(::time(NULL));
		};

		// ���݂̃��[�J���������擾����
		static dateTime getLocalTime() {
			SYSTEMTIME st;
			::GetLocalTime(&st);
			return dateTime(st);
		}

	private:
		///time_t m_time;
		FILETIME m_ft;
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
		int getArgListCount() { return m_argList.size(); };

		// �I�v�V�����ꗗ���擾����
		std::vector<_tstring> getOptList() {
			std::vector<_tstring> result;
			std::map<_tstring, _tstring>::iterator i;
			for (i = m_optList.begin(); i != m_optList.end(); i++) {
				result.push_back((*i).first);
			}
			return result;
		}

		// �I�v�V���������������ǂ������ׂ�
		bool hasKey(const _tstring& key) {
			return (m_optList.find(key) != m_optList.end());
		}

		// �I�v�V�����̈������擾����
		_tstring getOptArg(const _tstring& key) {
			return hasKey(key) ? m_optList[key] : _T("");
		}

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
		dateTime TimeGenerated() { return dateTime( getRaw()->TimeGenerated ); };
		
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
		dateTime get_TimeWritten() { return dateTime( getRaw()->TimeWritten ); };

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

		// �g���q�̕ύX
		static _tstring renameExtension(const _tstring& strPath, const _tstring& newExt) {
			TCHAR szTemp[MAX_PATH];
			PathRenameExtension(lstrcpy(szTemp, strPath.c_str()), newExt.c_str());
			return szTemp;
		}

		// �t���p�X����t�@�C������ύX����
		static _tstring renameFileSpec(const _tstring& strPath, const _tstring& strFileSpec) {
			return path::append(path::removeFileSpec(strPath), strFileSpec);
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

		// ���������t�@�C���̍ŏI�X�V�������擾
		virtual dateTime getLastWriteTime() { return dateTime(m_findData.ftLastWriteTime); };

		// ���������t�@�C���̑S���� WIN32_FIND_DATA �^�ɃR�s�[����
		virtual void copyWin32FindData(WIN32_FIND_DATA& rBuf) { rBuf = m_findData; }

	private:
		_tstring m_strWildcard;

		HANDLE m_hFind;

		WIN32_FIND_DATA m_findData;
	};

	//-------------------------------------------------------------------------
	// �t�@�C�� ��\������N���X
	//-------------------------------------------------------------------------
	class file {
	public:
		// �f�t�H���g�R���X�g���N�^
		file() : m_hasInfo(false) {};

		// �R�s�[�R���X�g���N�^
		file(const file& src) : m_fullPath(src.m_fullPath), m_lastWriteTime(src.m_lastWriteTime), m_fileSize(src.m_fileSize), m_dwFileAttributes(src.m_dwFileAttributes), m_hasInfo(src.m_hasInfo) {};

		// WIN32_FIND_DATA �ŏ���������R���X�g���N�^
		file(const _tstring& strParentPath, const WIN32_FIND_DATA& rFindData) {
			m_fullPath = path::append(strParentPath, rFindData.cFileName);
			m_lastWriteTime = dateTime(rFindData.ftLastWriteTime);

			LARGE_INTEGER li = {rFindData.nFileSizeLow, rFindData.nFileSizeHigh};
			m_fileSize = li.QuadPart;

			m_dwFileAttributes = rFindData.dwFileAttributes;

			m_hasInfo = true;	// [���������Ă���]�ŁA������
		};

		// �t�@�C�����ŏ���������R���X�g���N�^
		file(const _tstring& strName) : m_fullPath(strName) {
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
		virtual ~file() {};

		// ������Z�q
		file& operator =(const file& rhs) {
			if (this != &rhs) m_fullPath = rhs.m_fullPath;
			return *this;
		};

		// �t�@�C���̗L����Ԃ�
		bool exists() { return path::fileExists(m_fullPath); };

		// �t�@�C���̃t���p�X��Ԃ�
		_tstring getFullPath() { return m_fullPath; };

		// �t�@�C����������Ԃ�
		_tstring getFileName() { return path::findFileName(m_fullPath); };

		// �t�@�C���̍ŏI�X�V������Ԃ�
		dateTime getLastWriteTime() {
			assert(m_hasInfo != false);
			return m_lastWriteTime;
		}

		// �t�@�C���̑�����Ԃ�
		DWORD getFileAttributes() {
			assert(m_hasInfo != false);
			return m_dwFileAttributes;
		}

		// �t�@�C���̃T�C�Y��Ԃ�
		__int64 getFileSize() {
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
	class folder {
	public:
		// �f�t�H���g�R���X�g���N�^
		folder() {};

		// �R�s�[�R���X�g���N�^
		folder(const folder& src) : m_fullPath(src.m_fullPath) {};

		// WIN32_FIND_DATA �ŏ���������R���X�g���N�^
		folder(const _tstring& strParentPath, const WIN32_FIND_DATA& rFindData) {
			m_fullPath = path::append(strParentPath, rFindData.cFileName);
		};

		// �f�B���N�g�����ŏ���������R���X�g���N�^
		folder(const _tstring& strName) : m_fullPath(strName) {};

		// �f�X�g���N�^
		virtual ~folder() {};

		// ������Z�q
		folder& operator =(const folder& rhs) {
			if (this != &rhs) m_fullPath = rhs.m_fullPath;
			return *this;
		};

		// �f�B���N�g�������̃t�@�C���ꗗ���擾
		std::vector<file> getFiles(const _tstring& strWC = _T("*.*"));

		// �f�B���N�g�������̃f�B���N�g���ꗗ���擾
		std::vector<folder> getSubFolders();

		// �t�@�C����������Ԃ�
		_tstring getFileName() { return path::findFileName(m_fullPath); };

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
		thread() : m_hThread( NULL ), m_cs(NULL), m_loop(true) {
		};

		virtual ~thread() {
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
			m_hThread = ( HANDLE )_beginthreadex( NULL, 0, thread_func, this, 0, NULL );
		};

		// �X���b�h���I��������B�h���N���X�Œ�`����B
		// �I�����@�́Aworker method �̎����Ɉˑ�����B
		virtual void end() { m_loop = false; };

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
		DWORD wait( DWORD timeout = INFINITE ) {
			DWORD ret = WaitForSingleObject( m_hThread, timeout );
			CloseHandle( m_hThread );
			return ret;
		};

		// ���[�v���ׂ����ǂ�����Ԃ�
		bool stillLoop() { return m_loop; };

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

	//-------------------------------------------------------------------------
	// �\�P�b�g�������N���X
	//-------------------------------------------------------------------------
	class wsaSocket {
	public:
		// �f�t�H���g�R���X�g���N�^
		wsaSocket() : m_sock(INVALID_SOCKET) {};

		// �f�X�g���N�^
		virtual ~wsaSocket() { closeSocket(); };

		// UDP �\�P�b�g�̍쐬
		// 0 ����I��
		// -1 socket �G���[
		// -2 bind �G���[
		int createUdpSocket(short port = 0) {
			m_sock = socket(AF_INET, SOCK_DGRAM, 0);
			if (m_sock == INVALID_SOCKET) {
				return -1;
			}

			if (port != 0) {
				if (bindPort(port) != 0) {
					closeSocket();
					return -2;
				}
			}

			return 0;
		}

		// �\�P�b�g�̔j��
		int closeSocket() {
			int ret = 0;
			if (m_sock != INVALID_SOCKET) {
				ret = closesocket(m_sock);
				m_sock = INVALID_SOCKET;
			}
			return ret;
		}

		// ��M�^�C���A�E�g���Ԃ�ݒ�
		int setRecvTimeout(int timeout) {
			return setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
		}

		// �|�[�g�Ƀo�C���h����
		int bindPort(short port) {
			sockaddr_in myaddr;
			memset(&myaddr, 0, sizeof(myaddr));
			myaddr.sin_family = AF_INET;
			myaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
			myaddr.sin_port = htons(port);

			return bind(m_sock, (sockaddr*)&myaddr, sizeof(myaddr));
		}

		// �ȒP��M
		int recvFromEasy(std::vector<char>& buf) {
			char work[512];
			sockaddr_in from;
			int fromlen;

			fromlen = sizeof(from);
			int ret =recvfrom(m_sock, work, sizeof(work) - 1, 0, (sockaddr*)&from, &fromlen);

			if (ret > 0) {
				buf.assign(&work[0], &work[ret]);
			}

			return ret;
		}

		// �ȒP���M
		int sendToEasy(const char* dataPtr, int dataLen, const _tstring& host, short port) {
			struct sockaddr_in to;
			memset(&to, 0, sizeof(to));
			to.sin_family = AF_INET;
			to.sin_addr.S_un.S_addr = kjm::util::getAddr(host.c_str());
			to.sin_port = htons(port);

			return sendto(m_sock, dataPtr, dataLen, 0, (sockaddr*)&to, sizeof(to));
		}

		// �ȒP���M(�������)
		int sendToEasy(const _tstring& data, const _tstring& host, short port) {
			return sendToEasy(data.c_str(), data.size(), host, port);
		}

	public:
		// winsock 2.0 �̏�����
		static int startup() {
			WSADATA wsaData;
			return WSAStartup(MAKEWORD(2, 0), &wsaData);
		}

		// wisncok �̏I��
		static int cleanup() { return WSACleanup(); };

	private:
		SOCKET m_sock;
	};

	//-------------------------------------------------------------------------
	// �v���Z�X�̎��s�ƊǗ����s���N���X
	//-------------------------------------------------------------------------
	class process {
	public:
		// �f�t�H���g�R���X�g���N�^
		process() {
			memset(&m_si, 0, sizeof(m_si));
			memset(&m_pi, 0, sizeof(m_pi));
		};

		// �f�X�g���N�^
		virtual ~process() { close(); };

		// �v���Z�X���s
		virtual BOOL run(const _tstring& pCommandLine) {
			memset(&m_si, 0, sizeof(m_si));
			memset(&m_pi, 0, sizeof(m_pi));

			m_si.cb = sizeof(m_si);
			TCHAR temp[MAX_PATH];

			BOOL bRet = CreateProcess(
				NULL,
				lstrcpy(temp, pCommandLine.c_str()),
				NULL,
				NULL,
				FALSE,
				0,
				NULL,
				NULL,
				&m_si,
				&m_pi
			);

			return bRet;
		}

		// �v���Z�X�I����҂�
		virtual DWORD wait(DWORD dwTimeout = INFINITE) {
			return WaitForSingleObject(m_pi.hProcess, dwTimeout);
		};

		// �v���Z�X�n���h�������
		// �v���Z�X�̏I����҂̂́Await�֐��Ȃ̂Œ��ӁB
		virtual void close() {
			if (m_pi.hThread != NULL) {
				CloseHandle(m_pi.hThread);
			}

			if (m_pi.hProcess != NULL) {
				CloseHandle(m_pi.hProcess);
			}

			memset(&m_pi, 0, sizeof(m_pi));
		}

	private:
		STARTUPINFO m_si;
		PROCESS_INFORMATION m_pi;
	};

	//-------------------------------------------------------------------------
	// �c�[���w���p�������N���X
	//-------------------------------------------------------------------------
	class toolHelp {
	public:
		// �f�t�H���g�R���X�g���N�^
		toolHelp() : m_hSnapshot(INVALID_HANDLE_VALUE) {};

		// �f�X�g���N�^
		virtual ~toolHelp() { closeHandle(); }

		// �X�i�b�v�V���b�g�̍쐬
		virtual bool createSnapshot() {
			this->m_hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			
			return (m_hSnapshot != INVALID_HANDLE_VALUE);
		}

		// �n���h�������
		virtual void closeHandle() {
			if (this->m_hSnapshot != INVALID_HANDLE_VALUE) {
				CloseHandle(this->m_hSnapshot);
				this->m_hSnapshot = INVALID_HANDLE_VALUE;
			}
		}

		// �v���Z�X��񋓂���(����)
		virtual bool processFirst() {
			memset(&m_pe, 0, sizeof(m_pe));
			m_pe.dwSize = sizeof(m_pe);

			return (Process32First(m_hSnapshot, &m_pe) != FALSE);
		}

		// �v���Z�X��񋓂���(���ڈȍ~)
		virtual bool processNext() {
			return (Process32Next(m_hSnapshot, &m_pe) != FALSE);
		}

		// ���s�t�@�C���̖��O���擾
		virtual _tstring getExeFile() {
			return m_pe.szExeFile;
		};

		// �v���Z�XID���擾
		virtual DWORD getProcessId() {
			return m_pe.th32ProcessID;
		};

	public:
		// �v���Z�X���X�g���擾����
		static std::vector<PROCESSENTRY32> getProcessList() {
			std::vector<PROCESSENTRY32> result;
			toolHelp th;
			if (th.createSnapshot()) {
				if (th.processFirst()) {
					do {
						result.push_back(th.m_pe);
					} while (th.processNext());
				}
				th.closeHandle();
			}
			return result;
		};

	private:
		HANDLE m_hSnapshot;
		PROCESSENTRY32 m_pe;
	};
}

