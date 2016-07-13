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
	// time_t型を扱うクラス
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
	// ライブラリモジュールを管理するクラス
	//-----------------------------------------------------------------
	class library {
	private:
		HINSTANCE m_hLibModule;

	public:
		// アクセサ
		HINSTANCE getInstance() { return m_hLibModule; };

	public:
		library();
		virtual ~library();

	public:
		BOOL LoadLibraryEx( LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags );
		BOOL FreeLibrary();
	};


	//-----------------------------------------------------------------
	// レジストリを管理するクラス
	//-----------------------------------------------------------------
	class registry {
	private:
		HKEY m_hKey;

	public:
		// アクセサ
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
	// イベントログの1レコード分を管理するクラス
	//-----------------------------------------------------------------
	class eventlog_record {
	private:
		std::string m_source_name;		// イベントソース
		std::vector<BYTE> m_raw_record;	// 生情報
		
		EVENTLOGRECORD* getRaw() { return (EVENTLOGRECORD*)m_raw_record.begin(); };
		std::string DispMessage( const char *SourceName, const char *EntryName, const char **Args, DWORD MessageId);
		int GetArgs(const EVENTLOGRECORD *pBuf, std::vector<char*>& args );
		BOOL GetModuleNameFromSourceName( const char *SourceName, const char *EntryName, std::vector<TCHAR>& ExpandedName);

	public:
		// アクセサ
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
	// イベントログを管理するクラス
	//-----------------------------------------------------------------
	class eventlog {
	private:
		HANDLE m_hEventLog;
		std::string m_unc_server_name;
		std::string m_source_name;

	public:
		// アクセサ
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
	// 物理ドライブ(SMART)を処理するクラス
	//-----------------------------------------------------------------
	ATTRVALUEANDNAME attrValueAndNames[];
	#define	NUM_ATTRIBUTE_STRUCTS	30

	class physical_drive {
	private:
		HANDLE m_disk_handle;

	public:
		// アクセサ
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
	// パス用の処理を追加した string クラス
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
	// 他言語 like な便利関数
	//-----------------------------------------------------------------
	std::string& chomp( std::string& s );

	//-----------------------------------------------------------------
	// APIを安全に扱う関数群
	//-----------------------------------------------------------------
	DWORD ExpandEnvironmentStrings( LPCTSTR lpSrc, std::vector<TCHAR>& rDst );
	DWORD FormatMessage( DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, std::vector<TCHAR>& buffer, va_list* Arguments );
	DWORD GetModuleFileName( HMODULE hModule, std::vector<TCHAR>& rFilename );
	DWORD GetPrivateProfileString( LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, std::vector<TCHAR>& returnedString, LPCTSTR lpFileName );
	std::string kjm_LoadString( HINSTANCE hInstance, UINT uID );

};
