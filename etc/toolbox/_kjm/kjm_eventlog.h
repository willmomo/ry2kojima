//---------------------------------------------------------------------
// kjm::eventlog class
//---------------------------------------------------------------------

#pragma once


#include <windows.h>
#include <vector>
#include <string>
#include "kjm_time.h"


namespace kjm {

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
		eventlog_record() {};
		eventlog_record( const kjm::eventlog_record& src )
			: m_source_name( src.m_source_name ), m_raw_record( src.m_raw_record ) {};
		eventlog_record( LPCTSTR lpSourceName, const EVENTLOGRECORD* src );
		virtual ~eventlog_record() {};

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
		std::string m_file_name;

	public:
		// アクセサ
		HANDLE getHandle() { return m_hEventLog; };

	public:
		eventlog() : m_hEventLog( NULL ) {};
		virtual ~eventlog() { CloseEventLog(); };

	public:
		BOOL OpenEventLog( LPCTSTR lpUNCServerName, LPCTSTR lpSourceName );
		BOOL OpenBackupEventLog( LPCTSTR lpUNCServerName, LPCTSTR lpSourceName, LPCTSTR lpFileName );
		BOOL CloseEventLog();
		BOOL ReadEventLog( DWORD dwReadFlags, DWORD dwRecordOffset, kjm::eventlog_record& record );
		
		// イベントログをバックアップする
		BOOL BackupEventLog( LPCTSTR lpBackupFileName ) {
			return ::BackupEventLog( m_hEventLog, lpBackupFileName );
		};
	};

}
