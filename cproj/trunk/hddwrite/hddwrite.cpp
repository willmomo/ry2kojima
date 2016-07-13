#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <time.h>

DWORD g_total;
DWORD g_avg10;

void writeOneFile(LPCTSTR pszFname) {
	static BYTE s_buf1MB[1024 * 1024];

	for (int i = 0; i < _countof(s_buf1MB); i++) {
		s_buf1MB[i] = rand();
	}

	FILE* fp = _tfopen(pszFname, _T("wb"));
	if (fp) {
		DWORD dwStart = GetTickCount();
		for (int j = 0; j < 1024; j++) {
			if (fwrite(s_buf1MB, sizeof(BYTE), _countof(s_buf1MB), fp) != _countof(s_buf1MB)) {
				_tprintf(_T("\n!!! WRITE ERROR : %d !!!\n"), errno);
				exit(-1);
			}
		}
		DWORD dwEnd = GetTickCount();
		fclose(fp);
		g_avg10 += (dwEnd - dwStart);
		_tprintf(_T("%7dms"), dwEnd - dwStart);
	}
}

void writeAllFile() {
	TCHAR fname[MAX_PATH];

	g_avg10 = 0;
	for (int i = 0; i < 10; i++) {
		wsprintf(fname, _T("hoge.%d"), i);
		writeOneFile(fname);
		if (i == 4) {
			_tprintf(_T("\n"));
		}
	}
	g_total += 10;
	_tprintf(_T("\n<< %8d Average:%7dms >>\n"), g_total, g_avg10 / 10);
}

void writeInfinite() {
	while (1) {
		writeAllFile();
	}
}

int _tmain(int argc, TCHAR **argv) {

	srand(time(NULL));
	writeInfinite();

	return 0;
}
