#include <windows.h>
#include "eventlog.h"

eventlog_record::eventlog_record( const EVENTLOGRECORD* record ) {

	// ŒÅ’è’·•”•ª‚Ì“WŠJ
	m_length = record->Length;
	m_reserved = record->Reserved;
	m_record_number = record->RecordNumber;
	m_time_generated = record->TimeGenerated;
	m_time_written = record->TimeWritten;
	m_event_id = record->EventID;
	m_event_type = record->EventType;
	m_num_strings = record->NumStrings;
	m_event_category = record->EventCategory;
	m_reserved_flags = record->ReservedFlags;
	m_closing_record_number = record->ClosingRecordNumber;
	m_string_offset = record->StringOffset;
	m_user_sid_length = record->UserSidLength;
	m_user_sid_offset = record->UserSidOffset;
	m_data_length = record->DataLength;
	m_data_offset = record->DataOffset;

	// ‰Â•Ï’·•”•ª‚Ì“WŠJ
	char* cp = ( char* )record;
	cp += sizeof( EVENTLOGRECORD );

	m_source_name = cp;
	cp += strlen( cp ) + 1;

	m_computer_name = cp;
	cp += strlen( cp ) + 1;
}


eventlog_record::~eventlog_record() {
}

