//---------------------------------------------------------------------
// ReadCheck.exe コマンドラインバージョン
//---------------------------------------------------------------------
#include "kjmlib.h"
#include <fcntl.h>
#include <io.h>


//---------------------------------------------------------------------
int g_fileCount = 0;	// 処理したファイルの数
int g_totalCount = 0;	// 全体の進捗を表すファイルの総数

//---------------------------------------------------------------------
// バージョン情報の表示
//---------------------------------------------------------------------
void version() {
	_tcout
		<< _T("(")
		<< kjm::util::removeFileSpec( kjm::util::getModuleFileName() )
		<< _T(")")
		<< std::endl;
	_tcout
		<< kjm::util::GetModuleName()
		<< _T(" version ")
		<< kjm::util::getFileVersion()
		<< std::endl;
}

//---------------------------------------------------------------------
// 使用法の表示
//---------------------------------------------------------------------
void usage() {
	version();
	_tcout
		<< _T("usage: ") << kjm::util::removeExtension( kjm::util::GetModuleName() ) << _T(" [switches] <folder>\n")
		<< _T("  switches: [--fast] 高速チェックモード\n")
		<< std::ends;
}

//---------------------------------------------------------------------
// ファイルの読み込みチェック(ANSI C バージョン)
//---------------------------------------------------------------------
bool readCheckOne_DOS(const kjm::_tstring& fname) {
	bool result = true;
	static char buf[8 * 1024 * 1024];

	_tcout << fname << std::endl;
	FILE* fp = _tfopen(fname.c_str(), _T("rb"));
	if (fp) {
		fseek(fp, 0, SEEK_END);
		__int64 nSize = _ftelli64(fp);
		__int64 nRead = 0;
		fseek(fp, 0, SEEK_SET);

		if (nSize > 0) {
			while (1) {
				int ret = fread(buf, sizeof(char), _countof(buf), fp);
				nRead += ret;

				_tcout << _T("  ") << nRead << _T("/") << nSize << _T("(") << std::fixed << std::setprecision(2) << (((double)nRead / (double)nSize) * 100.0) << _T("%)\r");
				if (ret != _countof(buf)) {
					_tcout << _T("\n");
					if (ferror(fp) != 0) {

						_tcout << _T("  errno ") << errno << _T(" : ") << _T("read error \n");
						result = false;
					}
					break;
				}
			}
		}
		fclose(fp);
	} else {
		_tcout << _T("  errno ") << errno << _T(" : ") << _T("open error \n");
		result = false;
	}

	return result;
}

//---------------------------------------------------------------------
// 進捗状況の出力
//---------------------------------------------------------------------
void printProgress(__int64 bytesRead, __int64 totalBytes, DWORD nowTime, DWORD startTime) {
	_tcout
		<< _T("  ") << bytesRead << _T("/") << totalBytes
		<< _T("(") << std::fixed << std::setprecision(2) << kjm::math::percentage(bytesRead, totalBytes) << _T("%)");

	// 経過時間が、一秒を超えているときは、コピー速度も出力
	if ((nowTime - startTime) > 1000) {
		_tcout
			<< _T(" [") << std::setw(5) << kjm::util::toHumanReadable( ((double)bytesRead / ((double)(nowTime - startTime) / 1000.0))) << _T("bps]");
	}
}

//---------------------------------------------------------------------
// ファイルの読み込みチェック(Win32APIバージョン)
//---------------------------------------------------------------------
bool readCheckOne(const kjm::_tstring& fname, DWORD bytesPerSector, bool fastMode) {
	bool result = true;

	_tcout << _T("残り(") << (--g_totalCount) << _T(") ") << fname << std::endl;

	DWORD openFlag = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN | (fastMode ? 0 : FILE_FLAG_NO_BUFFERING);
	HANDLE hFile = CreateFile(fname.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, openFlag, NULL);

	if (hFile != INVALID_HANDLE_VALUE) {

		// ファイルアクセスのバイト数をセクタサイズの整数倍に合わせる
		// バッファ アドレスをメモリ上でのディスクのセクタ境界に調整する
		char* buffer = new char[bytesPerSector * 2];
		char* p = (char *) ((DWORD) (buffer + bytesPerSector - 1) & ~(bytesPerSector - 1));

		__int64 li1 = kjm::file::getFileSize(hFile);
		__int64 li2 = 0;

		DWORD lastPrintTime = 0;
		DWORD dwStart = GetTickCount();
		while (true) {
			DWORD bytesRead;
			if (ReadFile(hFile, p, bytesPerSector, &bytesRead, NULL) == FALSE) {
				DWORD dwError = GetLastError();
				_tcerr << _T("[ng] read error ") << dwError << _T(": ") << kjm::util::formatMessageBySystem(dwError) << std::endl;
				result = false;
				break;
			}

			li2 += bytesRead;

			if ((GetTickCount() - lastPrintTime) > 500) {
				lastPrintTime = GetTickCount();

				// 500ms毎に進捗を出力する
				printProgress(li2, li1, lastPrintTime, dwStart);
				_tcout << _T("\r");
			}

			if (bytesRead != bytesPerSector) {
				lastPrintTime = GetTickCount();

				// 最後の結果出力
				printProgress(li2, li1, lastPrintTime, dwStart);
				_tcout << _T(" ... [ok]\n");
				break;
			}
		}

		delete[] buffer;

		CloseHandle(hFile);
	} else {
		DWORD dwError = GetLastError();
		_tcerr << _T("[ng] open error ") << dwError << _T(": ") << kjm::util::formatMessageBySystem(dwError) << std::endl;

		if (dwError == ERROR_SHARING_VIOLATION || dwError == ERROR_ACCESS_DENIED) {
			// 共有違反のときは、処理を継続させます。
		} else {
			result = false;
		}
	}

	return result;
}

//---------------------------------------------------------------------
// ファイルとフォルダを列挙して読込テスト
//---------------------------------------------------------------------
int readCheck(const kjm::_tstring& path, DWORD bytesPerSector, bool fastMode) {
	kjm::folderInfo fi(path);

	// 中のファイルをチェック
	std::vector<kjm::fileInfo> files = fi.getFiles();
	for (std::vector<kjm::fileInfo>::const_iterator i = files.begin(); i != files.end(); i++) {
		if (readCheckOne(i->fullName(), bytesPerSector, fastMode) == false) {
			return -1;
		}
	}

	// サブフォルダを再帰処理
	std::vector<kjm::folderInfo> folders = fi.getSubFolders();
	for (std::vector<kjm::folderInfo>::iterator i = folders.begin(); i != folders.end(); i++) {
		if (readCheck(i->getFullPath(), bytesPerSector, fastMode) != 0) {
			return -1;
		}
	}

	return 0;
}

//---------------------------------------------------------------------
// ファイルの総数を数える
//---------------------------------------------------------------------
void countTotal(const kjm::_tstring& path) {
	kjm::folderInfo fi(path);

	// 中のファイルを数に加える
	g_totalCount += fi.getFiles().size();
	_tcout << _T("総ファイル数 : ") << g_totalCount << _T("\r");

	// サブフォルダを再帰処理
	std::vector<kjm::folderInfo> folders = fi.getSubFolders();
	for (std::vector<kjm::folderInfo>::iterator i = folders.begin(); i != folders.end(); i++) {
		countTotal(i->getFullPath());
	}
}

//---------------------------------------------------------------------
// プログラムスタート
//---------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {
	SET_UNICODE_MODE;	// コンソールでの多言語出力に対応

	kjm::optionInfoList opts;
	opts.push_back(kjm::optionInfo(_T("help"), _T("?"), kjm::no_argument));
	opts.push_back(kjm::optionInfo(_T("version"), _T("v"), kjm::no_argument));
	opts.push_back(kjm::optionInfo(_T("fast"), _T("f"), kjm::no_argument));

	kjm::cmdLine cl;
	cl.parse(argc, argv, opts);

	if (cl.hasOption(_T("version"))) {
		version();
		return 0;
	} else if (cl.hasOption(_T("help"))) {
		usage();
		return 0;
	} else if (cl.get_argCount() != 1) {
		usage();
		return 1;
	}

	int exitCode = -1;

	// ディスクのセクタサイズを取得
	DWORD bytesPerSector = 0;
	if (GetDiskFreeSpace(cl.get_argument(0).c_str(), NULL, &bytesPerSector, NULL, NULL)) {
		_tcout
			<< kjm::util::sprintf_str(_T("1セクタ当たりのバイト数: %d バイト"), bytesPerSector)
			<< std::endl;

		// ファイルの総数を数えておく
		g_totalCount = 0;
		countTotal(cl.get_argument(0));
		_tcout << std::endl;

		exitCode = readCheck(cl.get_argument(0), bytesPerSector,
			cl.hasOption(_T("fast")) ? true : false);
	} else {
		DWORD dwError = GetLastError();
		_tcerr
			<< kjm::util::sprintf_str(_T("'%s'のセクタサイズが取得できません。error %u: %s"),
				cl.get_argument(0).c_str(), dwError, kjm::util::formatMessageBySystem(dwError).c_str())
			<< std::endl;
		exitCode = -1;
	}

	return exitCode;
}
