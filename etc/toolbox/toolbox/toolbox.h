#pragma once

#if !defined( __TOOLBOX_H )
#define __TOOLBOX_H


#define _WIN32_WINNT 0x400


#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include <shlwapi.h>
#pragma comment( lib, "shlwapi.lib" )

#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include <process.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#include "../wsslog/wsslog.h"
#include "../_kjm/kjm.h"


#define LOGC_NAME	_T( "TLBOX" )	// ÉçÉOÇ…Ç¬ÇØÇÈéØï éq


class toolboxini {
private:
	static const bool m_default_toolbox_watch_smart;
	static const bool m_default_toolbox_watch_eventlog;
	static const int m_default_smart_interval;
	static const int m_default_smart_warning;
	static const std::string m_default_smart_log_folder;
	static const int m_default_eventlog_interval;
	static const int m_default_eventlog_warning;
	static const std::string m_default_eventlog_log_folder;

	std::string m_filename;

	struct toolbox_section {
		bool watch_smart;
		bool watch_eventlog;
	} m_toolbox;

	struct smart_section {
		int interval;
		bool warning;
		std::string log_folder;
	} m_smart;

	struct eventlog_section {
		int interval;
		bool warning;
		std::string log_folder;
	} m_eventlog;

public:
	bool get_ToolboxWatchSmart() { return m_toolbox.watch_smart; };
	bool get_ToolboxWatchEventlog() { return m_toolbox.watch_eventlog; };
	int get_SmartInterval() { return m_smart.interval; };
	bool get_SmartWarning() { return m_smart.warning; };
	std::string get_SmartLogFolder() { return m_smart.log_folder; };
	int get_EventlogInterval() { return m_eventlog.interval; };
	bool get_EventlogWarning() { return m_eventlog.warning; };
	std::string get_EventlogLogFolder() { return m_eventlog.log_folder; };

public:
	toolboxini();
	virtual ~toolboxini() {};

	bool Load( const char* filename );
	bool Reload();

};

// toolbox.cpp prototype
extern toolboxini g_toolboxini;
extern bool g_end_flag;
extern std::string g_smart_warning;
extern std::string g_evlog_warning;


// smartthrd.cpp prototype
unsigned __stdcall smart_thread( void* arg );


// evlogthrd.cpp prototype
unsigned __stdcall eventlog_thread( void* arg );


// tlcommon.cpp prototype
void BackupLog( const char* filename );


#endif
