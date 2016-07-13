//-----------------------------------------------------------------------------
// FindWindow API���g���ăE�B���h�E��T���B
//
// -c �N���X��
//   �N���X������E�B���h�E��T��
// -w �E�B���h�E�^�C�g��
//   �E�B���h�E�^�C�g������E�B���h�E��T��
//-----------------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>

#include "kjmlib.h"

#include "resource.h"

//-----------------------------------------------------------------------------
// �v���O�����X�^�[�g
//-----------------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {

	kjm::cmdLine opt;
	opt.parse(argc, argv);

	HWND hwnd;
	if (opt.hasOption(_T("c"))) {
		_tprintf(_T("�N���X�� '%s' ����E�B���h�E��T���܂��B\n"), opt.getOptArgument(_T("c")).c_str());
		hwnd = FindWindow(opt.getOptArgument(_T("c")).c_str(), NULL);
	} else if (opt.hasOption(_T("w"))) {
		_tprintf(_T("�E�B���h�E�^�C�g�� '%s' ����E�B���h�E��T���܂��B\n"), opt.getOptArgument(_T("w")).c_str());
		hwnd = FindWindow(NULL, opt.getOptArgument(_T("w")).c_str());
	} else {
		_tprintf(_T("�g�p�@�FFindWindow {-c <�N���X��>|-w <�N���X��>}\n"));
		return -1;
	}

	if (hwnd) {
		_tprintf(_T("�E�B���h�E��������܂���(�n���h��: %p)\n"), hwnd);
	} else {
		_tprintf(_T("�E�B���h�E��������܂���Berror: %d\n"), GetLastError());
	}

	return 0;
}
