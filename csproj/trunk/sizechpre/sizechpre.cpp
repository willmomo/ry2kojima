//
// ��ʂ̉𑜓x���w�肳�ꂽ���̂ƈႤ�ꍇ�Asizech.exe���Ăяo���v���O����
//
// ��ȍ\���v���p�e�B
//   C/C++/�R�[�h����/�����^�C�����C�u����=�}���`�X���b�h(/MT)
//   �}�j�t�F�X�g�c�[��/���o��/DPI�F���̗L����=�͂�
// 
#include "stdafx.h"
#include "sizechpre.h"
#include "./kjmlib/kjmlib.h"
#include "./kjmlib/monitor.h"
#include "./kjmlib/process.h"

// �v���O�����X�^�[�g
///
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	kjm::GetLog().setLogPath_App(_T("log"));
	kjm::GetLog().set_Stop(false);
	kjm::GetLog().writeInfo(_T("---- program start ---- ---- ----"));

	kjm::optionInfoList opts;
	opts.push_back(kjm::optionInfo(_T("width"), _T("w"), kjm::required_argument));
	opts.push_back(kjm::optionInfo(_T("height"), _T("h"), kjm::required_argument));

	kjm::cmdLine args;
	args.parse(__argc, __targv, opts);

	int width = 0;
	if (args.hasOption(_T("width"))) width = _ttoi(args.getOptArgument(_T("width")).c_str());

	int height = 0;
	if (args.hasOption(_T("height"))) height = _ttoi(args.getOptArgument(_T("height")).c_str());

	kjm::GetLog().writeInfo(_T("width = %d"), width);
	kjm::GetLog().writeInfo(_T("height = %d"), height);

	kjm::monitors mons;
	mons.enumDisplayMonitors();

	kjm::GetLog().writeInfo(_T("monitor count = %d"), mons.get_MonitorCount());

	if ((mons.get_MonitorCount() == 1) && (width > 0) && (height > 0)) {
		// �V���O���f�B�X�v���C�̎������������s��

		int cxscreen = GetSystemMetrics(SM_CXSCREEN);
		int cyscreen = GetSystemMetrics(SM_CYSCREEN);

		kjm::GetLog().writeInfo(_T("cxscreen = %d"), cxscreen);
		kjm::GetLog().writeInfo(_T("cyscreen = %d"), cyscreen);

		if (cxscreen != width || cyscreen != height) {
			kjm::_tstring cmd = kjm::util::renameFileSpec(kjm::util::getModuleFileName(), _T("sizech.exe"));
			cmd = kjm::util::sprintf_str(_T("\"%s\" %d,%d"), cmd.c_str(), width, height);
			kjm::process::run(cmd);
		}
	}

	kjm::GetLog().writeInfo(_T("---- program end ---- ---- ----"));
	return 0;
}
