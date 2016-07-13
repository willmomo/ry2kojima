#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <locale.h>
#include <vector>

#include <stdio.h>
#include <direct.h>
#include <io.h>

char g_buf[8192 * 10];
TCHAR cDrv = 'C';

//-----------------------------------------------------------------------------
// ストレスファイルの削除
//-----------------------------------------------------------------------------
void RemoveAll(LPCTSTR path) {
	WIN32_FIND_DATA data;
	TCHAR wc[MAX_PATH];
	PathAppend(lstrcpy(wc, path), _T("*.*"));

	HANDLE hFind = FindFirstFile(wc, &data);
	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}

	do {
		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			TCHAR fname[MAX_PATH];
			PathAppend(lstrcpy(fname, path), data.cFileName);
			_tprintf(_T("削除 '%s' ... "), fname);

			if (DeleteFile(fname)) {
				_tprintf(_T("done\n"));
			} else {
				_tprintf(_T("error %d\n"), GetLastError());
			}
		}
	} while (FindNextFile(hFind, &data));

	FindClose(hFind);
}

//-----------------------------------------------------------------------------
// ストレスファイルの作成
//-----------------------------------------------------------------------------
int MakeAll(LPCTSTR path) {
	_tprintf(_T("ストレスファイルを '%s' に作成。\n"), path);

	__int64 freeBytes;
	GetDiskFreeSpaceEx(path, (PULARGE_INTEGER)&freeBytes, NULL, NULL);
	_tprintf(_T("空き容量：%13I64u\n"), freeBytes);

	// 空き容量が 1GB を切ったら中身を可能なかぎり削除して次の処理を行う。
	if (freeBytes < 1000000000) {
		RemoveAll(path);
	}

	freeBytes /= 2;
	_tprintf(_T("空き÷２：%13I64u\n"), freeBytes);

	TCHAR fname[MAX_PATH];
	TCHAR fspec[MAX_PATH];
	_stprintf(fspec, _T("%04X-%08X.bin"), GetCurrentProcessId(), GetTickCount());

	PathAppend(lstrcpy(fname, path), fspec);
	FILE* fp = _tfopen(fname, _T("wb"));
	if (fp) {
		__int64 wsize = 0;
		DWORD t1 = GetTickCount();
		for (__int64 i = 0; i < (freeBytes / sizeof(g_buf)); i++) {
			_tprintf(_T("作成中　：%13I64u\r"), i * sizeof(g_buf));
			int ret = fwrite(g_buf, sizeof(char), _countof(g_buf), fp);
			wsize += ret;
			if (ret != _countof(g_buf)) {
				_tprintf(_T("\n書込み中断\n"));
				break;
			}
		}
		DWORD t2 = GetTickCount();
		_tprintf(_T("\n書込み性能：%d Kbps\n"), (wsize / ((t2 - t1) / 1000)) / 1024);
		fclose(fp);
	}
	_putts(_T(""));

	return 0;
}

//-----------------------------------------------------------------------------
// システム内の全ファイルを読み込む処理
//-----------------------------------------------------------------------------
int ReadAll(LPCTSTR path) {
	std::vector<TCHAR> wc(MAX_PATH);

	PathAppend(lstrcpy(&wc[0], path), _T("*.*"));
	WIN32_FIND_DATA findData;
	HANDLE hFind = FindFirstFile(&wc[0], &findData);
	if (hFind == INVALID_HANDLE_VALUE) {
		return -1;
	}

	do {
		//_tprintf(_T("\r%s "), findData.cFileName);

		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if ((lstrcmp(findData.cFileName, _T(".")) != 0) && (lstrcmp(findData.cFileName, _T("..")) != 0)) {
				std::vector<TCHAR> child(MAX_PATH);
				PathAppend(lstrcpy(&child[0], path), findData.cFileName);
				ReadAll(&child[0]);
			}
		} else {
			std::vector<TCHAR> fname(MAX_PATH);
			PathAppend(lstrcpy(&fname[0], path), findData.cFileName);
			FILE* fp = _tfopen(&fname[0], _T("rb"));
			if (fp) {
				__int64 fsize = _filelengthi64(fileno(fp));
				__int64 done = 0;
				__int64 n;

				while ((n = fread(g_buf, sizeof(char), _countof(g_buf), fp)) == _countof(g_buf)) {
					done += n;
					_tprintf(_T("read test [%13I64d/%13I64d] %3d%%\r"), done, fsize, done * 100 / fsize);
				}

				if (fsize > 0) {
					done += n;
					_tprintf(_T("read test [%13I64d/%13I64d] %3d%%\r"), done, fsize, done * 100 / fsize);
				}

				fclose(fp);
			}
		}
	} while (FindNextFile(hFind, &findData));

	FindClose(hFind);

	return 0;
}

//-----------------------------------------------------------------------------
// プログラム スタート
//-----------------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {
	_tsetlocale(LC_ALL, _T("japanese"));

	if (argc == 2) {
		cDrv = argv[1][0];
	}

	TCHAR stressPath[MAX_PATH];
	_stprintf(stressPath, _T("%c:\\__hddstress__"), cDrv);

	TCHAR readPath[MAX_PATH];
	_stprintf(readPath, _T("%c:\\"), cDrv);

	// Ctrl+C でしか止まりません。
	for (;;) {
		// ストレステスト用ファイルを作成するフォルダを準備
		_tmkdir(stressPath);

		// ストレステスト用のファイルを作成
		MakeAll(stressPath);

		// 全ファイルを読み込むテスト
		ReadAll(readPath);
		_putts(_T(""));
	}

	return 0;
}
