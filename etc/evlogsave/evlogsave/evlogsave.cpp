//-----------------------------------------------------------------------------
// イベントログを簡単に保存するサンプル
//
// -o 保存先フォルダを指定(指定がないと即終了)
// -s 保存に数を指定(デフォルト30)
//
// <PC名>-YYYYMMDD-[system|application].evt というファイルを作成します。
//-----------------------------------------------------------------------------
#include "kjmlib.h"

//-----------------------------------------------------------------------------
// プログラムスタート
//-----------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	kjm::cmdLine cl;
	kjm::optionInfoList opts;
	opts.push_back(kjm::optionInfo(_T("o"), _T("o"), kjm::required_argument));
	opts.push_back(kjm::optionInfo(_T("s"), _T("s"), kjm::required_argument));
	cl.parse(__argc, __targv, opts);

	if (cl.hasOption(_T("o")) == false || cl.getOptArgument(_T("o")).empty() == true) {
		return -1;
	}

	kjm::_tstring outPath = cl.getOptArgument(_T("o"));
	kjm::_tstring machineName = kjm::environment::get_MachineName();
	int saveDays = (cl.hasOption(_T("s")) ? _ttoi(cl.getOptArgument(_T("s")).c_str()) : 30);

	kjm::create_directory(outPath.c_str());

	{	// 古いファイルの削除ロジック
		std::vector<kjm::_tstring> files = kjm::directory::getFiles(outPath, machineName + _T("*.evt"));
		for (std::vector<kjm::_tstring>::iterator i = files.begin(); i != files.end(); i++) {
			if ((kjm::dateTime::now() - kjm::file::getLastWriteTime(*i)).totalDays() > 30) {
				::DeleteFile((*i).c_str());
			}
		}
	}

	try {
		kjm::time now = kjm::time::GetTickCount();

		kjm::_tstring fname = kjm::util::append(outPath, machineName + now.strftime(_T("-%Y%m%d-")) + _T("system.evt"));
		::DeleteFile(fname.c_str());
		kjm::event_log::backup(_T("system"), fname);

		fname = kjm::util::append(outPath, machineName + now.strftime(_T("-%Y%m%d-")) + _T("application.evt"));
		::DeleteFile(fname.c_str());
		kjm::event_log::backup(_T("application"), fname);
	} catch (kjm::kjmException& e) {
		_tprintf(_T("例外: error %d: %s\n"), e.m_errorCode, e.m_msg.c_str());
	}

	return 0;
}
