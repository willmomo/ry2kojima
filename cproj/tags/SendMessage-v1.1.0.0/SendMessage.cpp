//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>

#include "kjmlib-20120601/kjmlib.h"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShowHelp(const kjm::_tstring &errmsg = kjm::_tstring()) {
	int mbIcon = MB_ICONINFORMATION;
	kjm::_tstring msg;

	if (!errmsg.empty()) {
		mbIcon = MB_ICONERROR;
		msg = errmsg + _T("\n\n");
	}

	msg +=
		_T("使用法\n")
		_T(" -t, --title=<window title>\n")
		_T("  検索するウィンドウのタイトル\n")
		_T(" -c, --class=<class name>\n")
		_T("  検索するウィンドウのクラス名\n")
		_T(" -m, --message=<window message>\n")
		_T("  送信するメッセージ\n")
		_T(" -w, --wparam=<message parameter>\n")
		_T("  送信するメッセージのwParam\n")
		_T(" -l, --lparam=<message parameter>\n")
		_T("  送信するメッセージのlParam\n")
		_T(" -s, --silent\n")
		_T("  エラーメッセージの抑止")
		_T(" -?, --help\n")
		_T("  このヘルプの表示\n\n");
	msg += _T("[") + kjm::util::getModuleFileName() + _T("] running.");

	MessageBox(
		NULL, msg.c_str(),
		(_T("SendMessage help (ver.") + kjm::util::getFileVersion() + _T(")")).c_str(),
		mbIcon | MB_OK);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int doSendMessage(const kjm::_tstring &clsName, const kjm::_tstring &title, const kjm::_tstring &msg, const kjm::_tstring &wParam, const kjm::_tstring &lParam, bool silentMode) {
	HWND hwnd = FindWindow((clsName.empty() ? NULL : clsName.c_str()), (title.empty() ? NULL : title.c_str()));
	if (hwnd == NULL) {
		if (!silentMode) {
			MessageBox(
				NULL,
				(_T("[") + title + _T("]/[") + clsName + _T("]というタイトル/クラス名のウィンドウが見つかりません。")).c_str(),
				_T("SendMessage ERROR"),
				MB_ICONERROR | MB_OK);
		}
		return -1;
	}

	TCHAR *ep = NULL;
	UINT num = _tcstoul(msg.c_str(), &ep, 0);
	if (*ep != 0) {
		if (msg == _T("WM_CLOSE"))			num = WM_CLOSE;
		else if (msg == _T("WM_COMMAND"))	num = WM_COMMAND;
		else {
			if (!silentMode) {
				kjm::_tstring err = (_T("[") + msg + _T("] という文字列メッセージはサポートしていません。以下の文字列をサポートします：\n\n"));
				err += _T("WM_CLOSE, WM_COMMAND");

				MessageBox(
					NULL,
					err.c_str(),
					_T("SendMessage ERROR"),
					MB_ICONERROR | MB_OK);
			}
			return -1;
		}
	}

	ep = NULL;
	UINT wp = 0;
	if (wParam.empty() == false) {
		wp = _tcstoul(wParam.c_str(), &ep, 0);
	}

	ep = NULL;
	UINT lp = 0;
	if (lParam.empty() == false) {
		lp = _tcstoul(lParam.c_str(), &ep, 0);
	}

	return SendMessage(hwnd, num, wp, lp);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int WINAPI tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {

	kjm::optionInfoList opts;

	opts.push_back(kjm::optionInfo(_T("title"), _T("t"), kjm::required_argument));
	opts.push_back(kjm::optionInfo(_T("class"), _T("c"), kjm::required_argument));
	opts.push_back(kjm::optionInfo(_T("message"), _T("m"), kjm::required_argument));
	opts.push_back(kjm::optionInfo(_T("wparam"), _T("w"), kjm::required_argument));
	opts.push_back(kjm::optionInfo(_T("lparam"), _T("l"), kjm::required_argument));
	opts.push_back(kjm::optionInfo(_T("silent"), _T("s"), kjm::no_argument));
	opts.push_back(kjm::optionInfo(_T("help"), _T("?"), kjm::no_argument));

	kjm::cmdLine args;

	args.parse(__argc, __targv, opts);

	// ヘルプを表示して終了
	if (args.hasOption(_T("help"))) {
		ShowHelp();
		return 0;
	}

	// ウィンドウタイトルが指定されていない時は、エラー終了
	if (!args.hasOption(_T("title")) && !args.hasOption(_T("class"))) {
		ShowHelp(_T("ERROR: ウィンドウタイトル(--title)またはクラス名(--class)を指定してください。"));
		return 1;
	}

	// メッセージが指定されていない時は、エラー終了
	if (!args.hasOption(_T("message"))) {
		ShowHelp(_T("ERROR: メッセージ(--message)を指定してください。"));
		return 1;
	}

	return doSendMessage(
		args.getOptArgument(_T("class")),
		args.getOptArgument(_T("title")),
		args.getOptArgument(_T("message")),
		args.getOptArgument(_T("wparam")),
		args.getOptArgument(_T("lparam")),
		args.hasOption(_T("silent"))
	);
}
