#pragma once

#define _WIN32_WINNT 0x400

#include <windows.h>
#include <vector>
#include <ctime>

#include "smart.h"

#pragma comment( lib, "shlwapi.lib" )

#define countof(A)	(sizeof((A))/sizeof((A)[0]))

namespace kjm {


	//-----------------------------------------------------------------
	// time_t�^�������N���X
	//-----------------------------------------------------------------
	class time {
	private:
		time_t m_time;

	public:
		time() : m_time( 0 ) {}; 
		time( time_t t ) : m_time( t ) {};
		virtual ~time() {};

		std::string ctime();

	public:
		static kjm::time GetCurrentTime();
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
		library();
		virtual ~library();

	public:
		BOOL LoadLibraryEx( LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags );
		BOOL FreeLibrary();
	};


	//-----------------------------------------------------------------
	// ���W�X�g�����Ǘ�����N���X
	//-----------------------------------------------------------------
	class registry {
	private:
		HKEY m_hKey;

	public:
		// �A�N�Z�T
		HKEY getKey() { return m_hKey; };

	public:
		registry();
		virtual ~registry();

	public:
		LONG RegOpenKeyEx( HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired );
		LONG RegCloseKey();
		LONG RegQueryValueEx( LPCTSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, std::vector<BYTE>& data );
	};


	//-----------------------------------------------------------------
	// �C�x���g���O��1���R�[�h�����Ǘ�����N���X
	//-----------------------------------------------------------------
	class eventlog_record {
	private:
		std::string m_source_name;		// �C�x���g�\�[�X
		std::vector<BYTE> m_raw_record;	// �����
		
		EVENTLOGRECORD* getRaw() { return (EVENTLOGRECORD*)m_raw_record.begin(); };
		std::string DispMessage( const char *SourceName, const char *EntryName, const char **Args, DWORD MessageId);
		int GetArgs(const EVENTLOGRECORD *pBuf, std::vector<char*>& args );
		BOOL GetModuleNameFromSourceName( const char *SourceName, const char *EntryName, std::vector<TCHAR>& ExpandedName);

	public:
		// �A�N�Z�T
		DWORD RecordNumber() { return getRaw()->RecordNumber; };
		kjm::time TimeGenerated() { return kjm::time( getRaw()->TimeGenerated ); };
		kjm::time TimeWritten() { return kjm::time( getRaw()->TimeWritten ); };
		DWORD EventID() { return getRaw()->EventID; };
		WORD EventType() { return getRaw()->EventType; };
		WORD EventCategory() { return getRaw()->EventCategory; };
		std::string EventCategoryText() { return DispMessage( SourceName().c_str(), "CategoryMessageFile", NULL, EventCategory()); };
		std::string EventIDText() {
			std::vector<char*> Args;
			GetArgs(getRaw(), Args);

			return DispMessage(SourceName().c_str(), "EventMessageFile", (const char **)Args.begin(), EventID());
		};
		DWORD DataLength() { return getRaw()->DataLength; };
		DWORD DataOffset() { return getRaw()->DataOffset; };
		std::string SourceName();
		std::string ComputerName();
		std::vector<BYTE> Data();

	public:
		eventlog_record();
		eventlog_record( const kjm::eventlog_record& src );
		eventlog_record( LPCTSTR lpSourceName, const EVENTLOGRECORD* src );
		virtual ~eventlog_record();

		kjm::eventlog_record& operator= ( const kjm::eventlog_record& src );
	};


	//-----------------------------------------------------------------
	// �C�x���g���O���Ǘ�����N���X
	//-----------------------------------------------------------------
	class eventlog {
	private:
		HANDLE m_hEventLog;
		std::string m_unc_server_name;
		std::string m_source_name;

	public:
		// �A�N�Z�T
		HANDLE getHandle() { return m_hEventLog; };

	public:
		eventlog();
		virtual ~eventlog();

	public:
		BOOL OpenEventLog( LPCTSTR lpUNCServerName, LPCTSTR lpSourceName );
		BOOL OpenBackupEventLog( LPCTSTR lpUNCServerName, LPCTSTR lpFileName );
		BOOL CloseEventLog();
		BOOL ReadEventLog( DWORD dwReadFlags, DWORD dwRecordOffset, kjm::eventlog_record& record );
		BOOL OpenBackupEventLog( LPCTSTR lpUNCServerName, LPCTSTR lpSourceName, LPCTSTR lpFileName );
		BOOL BackupEventLog( LPCTSTR lpBackupFileName ) {
	};



	//-----------------------------------------------------------------
	// �����h���C�u(SMART)����������N���X
	//-----------------------------------------------------------------
	ATTRVALUEANDNAME attrValueAndNames[];
	#define	NUM_ATTRIBUTE_STRUCTS	30

	class physical_drive {
	private:
		HANDLE m_disk_handle;

	public:
		// �A�N�Z�T
		HANDLE getHandle() { return m_disk_handle; };
		int getTemperature();
		int getDrivePowerCycleCount();
		int getPowerOnHours();
		int getStartStopCount();

	public:
		physical_drive();
		virtual ~physical_drive();

		BOOL OpenDrive( LPCTSTR driveName );
		BOOL CloseDrive();

		bool isSupportedSMART();
		int identifyDevice(IDENTIFYDATA* identifyData);
		int smartDisable();
		int smartEnable();
		int smartEnableDisableAutoSave(char enable);
		int smartReturnStatus();
		int smartReadAttributeValues(SMARTATTRIBUTESDATA* attrData);
		int smartReadAttributeThresholds(SMARTHRESHOLDSDATA* thData);

		std::string toCSVString();
	};


	//-----------------------------------------------------------------
	// �p�X�p�̏�����ǉ����� string �N���X
	//-----------------------------------------------------------------
	class path_string : public std::string {
	private:

	public:
		path_string();
		path_string( const path_string& rhs );
		path_string( std::string::const_pointer s );
		virtual ~path_string();

		BOOL FileExists();
		path_string& RemoveFileSpec();
		path_string& Append( LPCTSTR pMore );
		path_string& RenameExtension( LPCTSTR pszExt );

	public:
		static path_string GetModuleFileName( HMODULE hModule = NULL );
	};


	//-----------------------------------------------------------------
	// ������ like �ȕ֗��֐�
	//-----------------------------------------------------------------
	std::string& chomp( std::string& s );

	//-----------------------------------------------------------------
	// API�����S�Ɉ����֐��Q
	//-----------------------------------------------------------------
	DWORD ExpandEnvironmentStrings( LPCTSTR lpSrc, std::vector<TCHAR>& rDst );
	DWORD FormatMessage( DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, std::vector<TCHAR>& buffer, va_list* Arguments );
	DWORD GetModuleFileName( HMODULE hModule, std::vector<TCHAR>& rFilename );
	DWORD GetPrivateProfileString( LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, std::vector<TCHAR>& returnedString, LPCTSTR lpFileName );
	std::string kjm_LoadString( HINSTANCE hInstance, UINT uID );

};
