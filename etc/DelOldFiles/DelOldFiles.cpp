
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	TCHAR szFileName[] = _T("D:\\vsswork\\hc1000_w41\\CardRProc\\Debug\\Log\\*.*");
	WIN32_FIND_DATA findData;

	HANDLE hFind = FindFirstFile(szFileName, &findData);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			SYSTEMTIME st;
			FILETIME ft;

			FileTimeToLocalFileTime(&findData.ftLastWriteTime, &ft);
			FileTimeToSystemTime(&ft, &st);

			printf("%-13s %04d/%02d/%02d %2d:%02d:%02d.%03d\n", findData.cFileName,
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

		} while (FindNextFile(hFind, &findData));
		FindClose(hFind);
	}

	return 0;
}
