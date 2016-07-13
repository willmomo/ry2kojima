

#pragma once

#if !defined(__KJM_H)
#define __KJM_H

#define _WIN32_WINNT	0x0500

#include <windows.h>

namespace kjm {

	class AccessControl {
	private:
		AccessControl() {};
		~AccessControl() {};

	public:
		static bool GetShutdownPrivilege();
	};

	class SystemShutdown {
	private:
		SystemShutdown() {};
		~SystemShutdown() {};

	public:
		static bool Logoff(bool force=false, bool forceIfHung=false);
		static bool Shutdown(bool force=false, bool forceIfHung=false);
		static bool Reboot(bool force=false, bool forceIfHung=false);
		static bool PowerOff(bool force=false, bool forceIfHung=false);
	};
}


#endif
