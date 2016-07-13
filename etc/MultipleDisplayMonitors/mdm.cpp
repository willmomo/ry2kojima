//---------------------------------------------------------------------
// マルチモニタのプログラム
//
// EnumDisplayMonitors などを使う場合は、
// WINVER=0x0500
// を定義しておく必要がある。
//---------------------------------------------------------------------

#include <windows.h>
#include <iostream>


BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	MONITORINFOEX mi;
	mi.cbSize = sizeof(mi);
	if (GetMonitorInfo(hMonitor, &mi)) {
		std::cout << "[ " << mi.szDevice << " ]\n";
		std::cout << "rcMonitor: (" << mi.rcMonitor.left << "," << mi.rcMonitor.top << ")-(" << mi.rcMonitor.right << "," << mi.rcMonitor.bottom << ")\n";
		std::cout << "rcWork:    (" << mi.rcWork.left << "," << mi.rcWork.top << ")-(" << mi.rcWork.right << "," << mi.rcWork.bottom << ")\n";

		if (mi.dwFlags & MONITORINFOF_PRIMARY) {
			std::cout << "This is the primary display monitor.\n";
		}

		std::cout << "\n";
	}
	return TRUE;
}


int main(int argc, char** argv) {
	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);
	return 0;
}
