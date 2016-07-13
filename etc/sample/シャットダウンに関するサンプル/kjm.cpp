

#include "kjm.h"

bool kjm::AccessControl::GetShutdownPrivilege() {

	// get a token for this process.
	HANDLE hToken;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		return false;
	}

	// get the LUID for the shutdown privilege.
	TOKEN_PRIVILEGES tkp;
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// get the shutdown privilege for this process.
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0);

	// Cannot test the return value of AdjustTokenPrivileges.

	if (GetLastError() != ERROR_SUCCESS) {
		return false;
	}

	return true;
}


bool kjm::SystemShutdown::Shutdown(bool force, bool forceIfHung) {
	
	if (!kjm::AccessControl::GetShutdownPrivilege()) {
		return false;
	}

	UINT flag = EWX_SHUTDOWN;
	if (force)	flag |= EWX_FORCE;
	if (forceIfHung)	flag |= EWX_FORCEIFHUNG;

	if (!ExitWindowsEx(flag, 0)) {
		return false;
	}

	return true;
}


bool kjm::SystemShutdown::Reboot(bool force, bool forceIfHung) {
	
	if (!kjm::AccessControl::GetShutdownPrivilege()) {
		return false;
	}

	UINT flag = EWX_REBOOT;
	if (force)	flag |= EWX_FORCE;
	if (forceIfHung)	flag |= EWX_FORCEIFHUNG;

	if (!ExitWindowsEx(flag, 0)) {
		return false;
	}

	return true;
}


bool kjm::SystemShutdown::PowerOff(bool force, bool forceIfHung) {
	
	if (!kjm::AccessControl::GetShutdownPrivilege()) {
		return false;
	}

	UINT flag = EWX_POWEROFF;
	if (force)	flag |= EWX_FORCE;
	if (forceIfHung)	flag |= EWX_FORCEIFHUNG;

	if (!ExitWindowsEx(flag, 0)) {
		return false;
	}

	return true;
}


bool kjm::SystemShutdown::Logoff(bool force, bool forceIfHung) {

	UINT flag = EWX_LOGOFF;
	if (force)	flag |= EWX_FORCE;
	if (forceIfHung)	flag |= EWX_FORCEIFHUNG;

	if (!ExitWindowsEx(flag, 0)) {
		return false;
	}

	return true;
}
