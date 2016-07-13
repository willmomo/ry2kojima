#define WINVER	0x0500

#include <windows.h>
#include <tchar.h>
#include <stdio.h>



BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {

	(*(long*)dwData)++;

	return TRUE;
}


int GetDisplayCount() {
	int result = 0;

	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&result);

	return result;
}

int _tmain(int argc, TCHAR** argv) {

	GetDisplayCount();
	
	return 0;
}
