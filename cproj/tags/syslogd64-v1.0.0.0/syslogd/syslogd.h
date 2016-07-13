#pragma once

#define	_WIN32_WINNT	0x0400

#include <winsock2.h>
#include <windows.h>
#include <tchar.h>
#pragma comment(lib, "ws2_32.lib")
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <stdio.h>
#include <errno.h>
#include <process.h>

#include "./kjmlib/kjmlib.h"

#include "resource.h"

class syslogd_thread : public kjm::thread {
public:
	syslogd_thread() {};
	~syslogd_thread() {};

	virtual unsigned worker();
	virtual void end();
	virtual DWORD suspend();
	virtual DWORD resume();
	virtual DWORD wait(DWORD dwTimeout = INFINITE);
};

extern CRITICAL_SECTION g_criticalSection;
