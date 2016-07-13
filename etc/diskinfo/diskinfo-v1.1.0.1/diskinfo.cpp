//-----------------------------------------------------------------------------
// ディスクの容量を表示するプログラム。
// 使用率/未使用率を戻り値にするオプションがある。
//
// kjmlib : Version 0.2012.08.10 がベストマッチライブラリ
//-----------------------------------------------------------------------------
#include ".\kjmlib-20120810\kjmlib.h"

//-----------------------------------------------------------------------------
// 使用方法の表示
//-----------------------------------------------------------------------------
void usage() {
	_tprintf(_T("使用法："));
	_tprintf(_T("usage : diskinfo [ドライブ名 or 共有名] [options]\n"));
	_tprintf(_T("\n"));
	_tprintf(_T(" -r, --result=<戻り値の種類>\n"));
	_tprintf(_T("    used_percent : 使用率を戻り値とする(0～100)\n"));
	_tprintf(_T("    free_percent : 未使用率を戻り値とする(0～100)\n"));
	_tprintf(_T(" -v, --version\n"));
	_tprintf(_T("    バージョン情報の表示\n"));
	_tprintf(_T(" -?, --help\n"));
	_tprintf(_T("    このヘルプを表示\n"));
	_tprintf(_T("\n"));
	_tprintf(_T("ドライブを指定せずに実行すると、PCに存在するドライブをすべて列挙する。\n"));
	_tprintf(_T("この場合、result オプションを付けても、最後のドライブの値になってしまうため、\n"));
	_tprintf(_T("あまり意味が無い。\n"));
}

//-----------------------------------------------------------------------------
// ディスク(ドライブ)の情報を出力
//-----------------------------------------------------------------------------
int printDiskInfo(const kjm::_tstring& disk, const kjm::_tstring& resultType) {
	int errorlevel = 0;

	ULARGE_INTEGER freeBytesAvailableToCaller;
	ULARGE_INTEGER totalNumberOfBytes;
	ULARGE_INTEGER totalNumberOfFreeBytes;

	_tprintf(_T("%-30s: %s\n"), _T("ディスク"), disk.c_str());
	if (_istalpha(disk[0])) {
		kjm::drive d(disk);
		_tprintf(_T("%-30s: %s\n"), _T("ディスクのタイプ"), d.get_DriveTypeString().c_str());

		if ((d.get_DriveType() == DRIVE_REMOVABLE) && (!d.isDiskInDrive())) {
			_tprintf(_T("  No disk in drive.\n"));
			return 0;
		}
	}

	BOOL bRet = GetDiskFreeSpaceEx(disk.c_str(), &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes);
	if (bRet) {
		_tprintf(_T("%-30s: %14I64u バイト\n"), _T("ディスクの総容量"), totalNumberOfBytes.QuadPart);
		_tprintf(_T("%-30s: %14I64u バイト\n"), _T("ディスクの空き容量(全体)"), totalNumberOfFreeBytes.QuadPart);
		_tprintf(_T("%-30s: %14I64u バイト\n"), _T("ディスクの空き容量(ユーザー毎)"), freeBytesAvailableToCaller.QuadPart);

		if (!resultType.empty()) {
			if (resultType == _T("used_percent")) {
				// ディスクの使用率を戻り値とする(0～100)%
				errorlevel = ((totalNumberOfBytes.QuadPart - totalNumberOfFreeBytes.QuadPart) * 100) / totalNumberOfBytes.QuadPart;
				_tprintf(_T("%-30s: %3d %%\n"), _T("ディスクの使用率"), errorlevel);
			} else if (resultType == _T("free_percent")) {
				// ディスクの未使用率を戻り値とする(0～100)%
				errorlevel = (totalNumberOfFreeBytes.QuadPart * 100) / totalNumberOfBytes.QuadPart;
				_tprintf(_T("%-30s: %3d %%\n"), _T("ディスクの未使用率"), errorlevel);
			} else {
				errorlevel = -1;
				_tprintf(_T("result オプションには、used_percent か free_percent が指定できます。\n"));
				usage();
			}
		}
	} else {
		DWORD dw = GetLastError();
		if (dw == ERROR_NOT_READY) {
			_tprintf(_T("  The device is not ready.\n"));
		} else {
			_ftprintf(stderr, _T("GetDiskFreeSpaceEx(%s, ...) に失敗しました(error %u)。\n"), disk.c_str(), GetLastError());
			errorlevel = -1;
		}
	}

	return errorlevel;
}

//-----------------------------------------------------------------------------
// プログラムスタート
//-----------------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {
	_tsetlocale(LC_ALL, _T("japan"));

	// 引数解析
	kjm::optionInfoList opts;
	opts.push_back(kjm::optionInfo(_T("result"), _T("r"), kjm::required_argument));
	opts.push_back(kjm::optionInfo(_T("version"), _T("v"), kjm::no_argument));
	opts.push_back(kjm::optionInfo(_T("help"), _T("?"), kjm::no_argument));

	kjm::cmdLine cl;
	cl.parse(argc, argv, opts);

	if (cl.hasOption(_T("version"))) {
		_tprintf(_T("%s ver.%s\n"), kjm::util::getModuleFileName().c_str(), kjm::util::getFileVersion().c_str());
		return 0;
	}

	if (cl.hasOption(_T("help"))) {
		usage();
		return 0;
	}

	int errorlevel = 0;

	if (cl.get_argCount() == 0) {
		kjm::driveList driveList;
		for (int i = 0; i < driveList.get_Size(); i++) {
			kjm::drive drive = driveList.get_Drive(i);

			errorlevel = printDiskInfo(drive.get_DriveLetter(), cl.hasOption(_T("result")) ? cl.getOptArgument(_T("result")) : _T(""));
			_tprintf(_T("\n"));
		}
	} else if (cl.get_argCount() == 1) {
		errorlevel = printDiskInfo(cl.get_argument(0), cl.hasOption(_T("result")) ? cl.getOptArgument(_T("result")) : _T(""));
	} else {
		usage();
		return -1;
	}

	return errorlevel;
}
