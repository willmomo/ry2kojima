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
		_T("�g�p�@\n")
		_T(" -t, --title=<window title>\n")
		_T("  ��������E�B���h�E�̃^�C�g��\n")
		_T(" -c, --class=<class name>\n")
		_T("  ��������E�B���h�E�̃N���X��\n")
		_T(" -m, --message=<window message>\n")
		_T("  ���M���郁�b�Z�[�W\n")
		_T(" -w, --wparam=<message parameter>\n")
		_T("  ���M���郁�b�Z�[�W��wParam\n")
		_T(" -l, --lparam=<message parameter>\n")
		_T("  ���M���郁�b�Z�[�W��lParam\n")
		_T(" -s, --silent\n")
		_T("  �G���[���b�Z�[�W�̗}�~")
		_T(" -?, --help\n")
		_T("  ���̃w���v�̕\��\n\n");
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
				(_T("[") + title + _T("]/[") + clsName + _T("]�Ƃ����^�C�g��/�N���X���̃E�B���h�E��������܂���B")).c_str(),
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
				kjm::_tstring err = (_T("[") + msg + _T("] �Ƃ��������񃁃b�Z�[�W�̓T�|�[�g���Ă��܂���B�ȉ��̕�������T�|�[�g���܂��F\n\n"));
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

	// �w���v��\�����ďI��
	if (args.hasOption(_T("help"))) {
		ShowHelp();
		return 0;
	}

	// �E�B���h�E�^�C�g�����w�肳��Ă��Ȃ����́A�G���[�I��
	if (!args.hasOption(_T("title")) && !args.hasOption(_T("class"))) {
		ShowHelp(_T("ERROR: �E�B���h�E�^�C�g��(--title)�܂��̓N���X��(--class)���w�肵�Ă��������B"));
		return 1;
	}

	// ���b�Z�[�W���w�肳��Ă��Ȃ����́A�G���[�I��
	if (!args.hasOption(_T("message"))) {
		ShowHelp(_T("ERROR: ���b�Z�[�W(--message)���w�肵�Ă��������B"));
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
