#include "toolbox.h"


const bool toolboxini::m_default_toolbox_watch_smart = true;
const bool toolboxini::m_default_toolbox_watch_eventlog = true;

const int toolboxini::m_default_smart_interval = 60 * 60;
const bool toolboxini::m_default_smart_warning = true;
const std::string toolboxini::m_default_smart_log_folder = "";

const int toolboxini::m_default_eventlog_interval = 60 * 60;
const bool toolboxini::m_default_eventlog_warning = true;
const std::string toolboxini::m_default_eventlog_log_folder = "";

toolboxini::toolboxini() {

	m_toolbox.watch_smart = m_default_toolbox_watch_smart;
	m_toolbox.watch_eventlog = m_default_toolbox_watch_eventlog;

	m_smart.interval = m_default_smart_interval;
	m_smart.warning = m_default_smart_warning;
	m_smart.log_folder = m_default_smart_log_folder;

	m_eventlog.interval = m_default_eventlog_interval;
	m_eventlog.warning = m_default_eventlog_warning;
	m_eventlog.log_folder = m_default_eventlog_log_folder;
}


toolboxini::~toolboxini() {

}


bool toolboxini::Load( const char* filename ) {

	m_filename = filename;

	return Reload();
}


bool toolboxini::Reload() {

	std::vector<char> work;

	m_toolbox.watch_smart = ( ::GetPrivateProfileInt( "toolbox", "watch_smart", m_default_toolbox_watch_smart, m_filename.c_str() ) == 0 ) ? false : true;
	m_toolbox.watch_eventlog = ( ::GetPrivateProfileInt( "toolbox", "watch_eventlog", m_default_toolbox_watch_eventlog, m_filename.c_str() ) == 0 ) ? false : true;

	m_smart.interval = ::GetPrivateProfileInt( "smart", "interval", m_default_smart_interval, m_filename.c_str() );
	m_smart.warning = ( ::GetPrivateProfileInt( "smart", "warning", m_default_smart_warning, m_filename.c_str() ) == 0 ) ? false : true;
	kjm::GetPrivateProfileString( "smart", "log_folder", m_default_smart_log_folder.c_str(), work, m_filename.c_str() );
	m_smart.log_folder = work.begin();

	m_eventlog.interval = ::GetPrivateProfileInt( "eventlog", "interval", m_default_eventlog_interval, m_filename.c_str() );
	m_eventlog.warning = ( ::GetPrivateProfileInt( "eventlog", "warning", m_default_eventlog_warning, m_filename.c_str() ) == 0 ) ? false : true;
	kjm::GetPrivateProfileString( "eventlog", "log_folder", m_default_eventlog_log_folder.c_str(), work, m_filename.c_str() );
	m_eventlog.log_folder = work.begin();

	return true;
}
