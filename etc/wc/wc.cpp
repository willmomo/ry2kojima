#include <windows.h>
#include <shlwapi.h>
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <mbctype.h>

#pragma comment(lib, "shlwapi.lib")


struct wc_info {
	size_t m_size;
	void* m_p;

	int m_count_byte;
	int m_count_char;
};


void wc(wc_info& wci) {

	char mbc[2];
	bool lead_byte = false;

	char* p = (char*)wci.m_p;

	for (size_t i = 0; i < wci.m_size; i++) {

		if (!lead_byte) {
			if (_ismbblead(p[i])) {
				// 2バイトコードの可能性があるので、フラグを立てて保留
				mbc[0] = p[i];
				lead_byte = true;
			} else {
				// 1バイトコードなので、カウント+1
				++wci.m_count_byte;
				++wci.m_count_char;
			}
		} else {
			if (_ismbbtrail(p[i])) {
				// 2バイトコード確定。フラグを落としてカウントアップ
				mbc[1] = p[i];
				lead_byte = false;
				wci.m_count_byte += 2;
				++wci.m_count_char;
			} else {
				// 異常な文字。カウントせずにフラグだけ戻す
				lead_byte = false;
			}
		}
	}
}


int main(int argc, char** argv) {

	char szPath[MAX_PATH];

	for (int i = 1; i < argc; i++) {

		PathRemoveFileSpec(lstrcpy(szPath, argv[i]));

		WIN32_FIND_DATA findData;
		HANDLE hFind = FindFirstFile(argv[i], &findData);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				char szFile[MAX_PATH];

				PathAppend(lstrcpy(szFile, szPath), findData.cFileName);

				printf("%s\n", szFile);

				wc_info wci;

				FILE* fp = fopen(szFile, "r");
				if (fp) {
					ZeroMemory(&wci, sizeof(wci));

					wci.m_size = _filelength(fileno(fp));
					wci.m_p = malloc(wci.m_size);
					fread(wci.m_p, sizeof(char), wci.m_size, fp);
					fclose(fp);

					wc(wci);

					printf("バイト数: %u\n", wci.m_count_byte);
					printf("  文字数: %u\n", wci.m_count_char);

					free(wci.m_p);
				}
			} while (FindNextFile(hFind, &findData));

			FindClose(hFind);
		}
	}

	_getch();

	return 0;
}
