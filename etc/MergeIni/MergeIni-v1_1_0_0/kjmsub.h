//---------------------------------------------------------------------------------------
// kjmsub ver.0.2010.7.29
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

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include <cassert>
#include <ctime>

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
	// ���ʂ̃��O����
	//-------------------------------------------------------------------------
	class log {
	public:
		// �f�t�H���g�R���X�g���N�^
		log() : m_ident() {};

		// �f�X�g���N�^
		~log() {};

		// �G���[���O
		void writeError(LPCTSTR pszFormat, ...);

		// ��񃍃O
		void writeInfo(LPCTSTR pszFormat, ...);

		// �S���O
		void writeAll(LPCTSTR pszFormat, ...);

		// �Â����O�̍폜
		void deleteOldLog();

		// ���ʎq�̎擾
		_tstring get_Ident() { return m_ident; };

		// ���ʎq�̐ݒ�
		void set_Ident(const _tstring& value) { m_ident = value; };

	private:
		_tstring m_ident;
	};

	//-------------------------------------------------------------------------
	// ���[�e�B���e�B�֐��p��static�N���X
	// [���l]
	// ���ׂĂ̊֐��́Astatic�Ő錾����܂��B
	//-------------------------------------------------------------------------
	class util {
	public:
		// �o�b�t�@����CRC16���v�Z����
		static unsigned short kjm::util::calcCRC16(unsigned char *p, unsigned int n);

		// �t�@�C���n���h������CRC16���v�Z����
		static unsigned short kjm::util::calcCRC16(FILE* fp);

		// �t�@�C��������CRC16���v�Z����
		static unsigned short kjm::util::calcCRC16(const _tstring& pfname);

		// ������I�[�̉��s����菜��
		static _tstring& chomp(_tstring& s);

		// CopyFile API�Ɠ��l�̓���������֐�(�G���[�ӏ����ڍׂɓ��肵�����Ƃ��Ɏg�p)
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
		static int copyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists);

		// Windows���V���b�g�_�E��������
		static int exitWindows();

		// ���ϐ���W�J����
		static DWORD expandEnvironmentStrings(LPCTSTR lpSrc, std::vector<TCHAR>& rDst);

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

		// �w��t�@�C���̃T�C�Y��Ԃ�(GetFileSize API�̃��b�p�[)
		// [����]  lpszFileName  �t�@�C����
		// [�߂�l] 64bit�̃t�@�C���T�C�Y
		static __int64 getFileSize(LPCTSTR lpszFileName);

		// �t�@�C���̃o�[�W�������\�[�X���擾
		static _tstring getFileVersion(const _tstring &strFileName);

		// Win32API GetModuleFileName �����S�Ɏg���֐�
		static _tstring getModuleFileName(HMODULE hModule = NULL);

		// ���[�N�G���A�̒��S�ɃE�B���h�E���ړ�����
		static BOOL moveCenterWorkarea(HWND hwnd);

		// SetComputerNameEx�֐�(vc6�p)
		static BOOL __SetComputerNameEx(COMPUTER_NAME_FORMAT NameType, LPCTSTR lpBuffer);

		// PathRenameExtension�̃��b�p�[
		static _tstring& pathRenameExtension(_tstring& strPath, LPCTSTR pszExt);

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
		virtual _tstring toString();

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

	private:
		///time_t m_time;
		FILETIME m_ft;
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

		// ���������t�@�C���̃t�@�C�����������擾
		virtual _tstring getFileName() { return m_findData.cFileName; };

		// ���������t�@�C���̃t���p�X�����擾
		virtual _tstring getFullName() { return path::renameFileSpec(m_strWildcard, m_findData.cFileName); };

		// ���������t�@�C���̍ŏI�X�V�������擾
		virtual dateTime getLastWriteTime() { return dateTime(m_findData.ftLastWriteTime); };

		// ���������t�@�C���̃T�C�Y���擾
		virtual __int64 getFileSize() {
			LARGE_INTEGER li = {m_findData.nFileSizeLow, m_findData.nFileSizeHigh};
			return li.QuadPart;
		};

	private:
		_tstring m_strWildcard;

		HANDLE m_hFind;

		WIN32_FIND_DATA m_findData;
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

}

