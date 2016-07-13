#pragma once

#if !defined( __EVENTLOG_H )
#define __EVENTLOG_H

#include <string>
#include <time.h>

class eventlog_record {
private:
	// EVENTLOGRECORD 固定長部分
	DWORD m_length;
	DWORD m_reserved;
	DWORD m_record_number;
	time_t m_time_generated;
	time_t m_time_written;
	DWORD m_event_id;
	WORD m_event_type;
	WORD m_num_strings;
	WORD m_event_category;
	WORD m_reserved_flags;
	DWORD m_closing_record_number;
	DWORD m_string_offset;
	DWORD m_user_sid_length;
	DWORD m_user_sid_offset;
	DWORD m_data_length;
	DWORD m_data_offset;

	// EVENTLOGRECORD 可変長部分
	std::string m_source_name;
	std::string m_computer_name;

public:
	// 単純なアクセサ
	DWORD RecordNumber() {
		return m_record_number;
	};
	time_t TimeGenerated() {
		return m_time_generated;
	};
	time_t TimeWritten() {
		return m_time_written;
	};
	DWORD EventID() { 
		return m_event_id;
	};
	std::string EventTypeText() {
		switch ( m_event_type ) {
		case EVENTLOG_SUCCESS:			return std::string( "成功");
		case EVENTLOG_ERROR_TYPE:		return std::string( "エラー" );
		case EVENTLOG_WARNING_TYPE:		return std::string( "警告" );
		case EVENTLOG_INFORMATION_TYPE:	return std::string( "情報" );
		case EVENTLOG_AUDIT_SUCCESS:	return std::string( "監査成功" );
		case EVENTLOG_AUDIT_FAILURE:	return std::string( "監査失敗" );
		default:	return std::string( "不明");
		}
	};
	WORD EventCategory() {
		return m_event_category;
	};
	std::string SourceName() {
		return m_source_name;
	};
	std::string ComputerName() {
		return m_computer_name;
	};

public:
	eventlog_record( const EVENTLOGRECORD *record );
	~eventlog_record();

};

#endif
