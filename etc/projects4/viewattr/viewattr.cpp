//=====================================================================
// すごく細かい属性の表示
//=====================================================================
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <stdio.h>


//---------------------------------------------------------------------
// プログラムスタート
//---------------------------------------------------------------------
int main(int argc, char** argv) {
	if (argc != 2) {
		printf("viewattr <フォルダ名>\n");
		return -1;
	}

	char wc[MAX_PATH];
	PathAppend(lstrcpy(wc, argv[1]), "*.*");

	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(wc, &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			printf("%s\n", data.cFileName);

			if (data.dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
				printf("  FILE_ATTRIBUTE_READONLY\n");
				data.dwFileAttributes &= ~FILE_ATTRIBUTE_READONLY;
			}

			if (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) {
				printf("  FILE_ATTRIBUTE_HIDDEN\n");
				data.dwFileAttributes &= ~FILE_ATTRIBUTE_HIDDEN;
			}

			if (data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) {
				printf("  FILE_ATTRIBUTE_SYSTEM\n");
				data.dwFileAttributes &= ~FILE_ATTRIBUTE_SYSTEM;
			}

			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				printf("  FILE_ATTRIBUTE_DIRECTORY\n");
				data.dwFileAttributes &= ~FILE_ATTRIBUTE_DIRECTORY;
			}

			if (data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) {
				printf("  FILE_ATTRIBUTE_ARCHIVE\n");
				data.dwFileAttributes &= ~FILE_ATTRIBUTE_ARCHIVE;
			}

			if (data.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED) {
				printf("  FILE_ATTRIBUTE_ENCRYPTED\n");
				data.dwFileAttributes &= ~FILE_ATTRIBUTE_ENCRYPTED;
			}

			if (data.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) {
				printf("  FILE_ATTRIBUTE_NORMAL\n");
				data.dwFileAttributes &= ~FILE_ATTRIBUTE_NORMAL;
			}

			if (data.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY) {
				printf("  FILE_ATTRIBUTE_TEMPORARY\n");
				data.dwFileAttributes &= ~FILE_ATTRIBUTE_TEMPORARY;
			}

			if (data.dwFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE) {
				printf("  FILE_ATTRIBUTE_SPARSE_FILE\n");
				data.dwFileAttributes &= ~FILE_ATTRIBUTE_SPARSE_FILE;
			}

			if (data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
				printf("  FILE_ATTRIBUTE_REPARSE_POINT\n");
				data.dwFileAttributes &= ~FILE_ATTRIBUTE_REPARSE_POINT;
			}

			if (data.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED) {
				printf("  FILE_ATTRIBUTE_COMPRESSED\n");
				data.dwFileAttributes &= ~FILE_ATTRIBUTE_COMPRESSED;
			}

			if (data.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE) {
				printf("  FILE_ATTRIBUTE_OFFLINE\n");
				data.dwFileAttributes &= ~FILE_ATTRIBUTE_OFFLINE;
			}

			if (data.dwFileAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) {
				printf("  FILE_ATTRIBUTE_NOT_CONTENT_INDEXED\n");
				data.dwFileAttributes &= ~FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
			}

			if (data.dwFileAttributes != 0) {
				printf("  unknown bits: 0x%08X\n", data.dwFileAttributes);
			}
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	} else {
		printf("FindFirstFile error(%d).\n", GetLastError());
	}

	return 0;
}
