//-----------------------------------------------------------------------------
// FindWindow APIを使ってウィンドウを探す。
//
// -c クラス名
//   クラス名からウィンドウを探す
// -w ウィンドウタイトル
//   ウィンドウタイトルからウィンドウを探す
//-----------------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>

#include "kjmlib.h"

#include "resource.h"

//-----------------------------------------------------------------------------
// プログラムスタート
//-----------------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {

	kjm::cmdLine opt;
	opt.parse(argc, argv);

	HWND hwnd;
	if (opt.hasOption(_T("c"))) {
		_tprintf(_T("クラス名 '%s' からウィンドウを探します。\n"), opt.getOptArgument(_T("c")).c_str());
		hwnd = FindWindow(opt.getOptArgument(_T("c")).c_str(), NULL);
	} else if (opt.hasOption(_T("w"))) {
		_tprintf(_T("ウィンドウタイトル '%s' からウィンドウを探します。\n"), opt.getOptArgument(_T("w")).c_str());
		hwnd = FindWindow(NULL, opt.getOptArgument(_T("w")).c_str());
	} else {
		_tprintf(_T("使用法：FindWindow {-c <クラス名>|-w <クラス名>}\n"));
		return -1;
	}

	if (hwnd) {
		_tprintf(_T("ウィンドウが見つかりました(ハンドル: %p)\n"), hwnd);
	} else {
		_tprintf(_T("ウィンドウが見つかりません。error: %d\n"), GetLastError());
	}

	return 0;
}
