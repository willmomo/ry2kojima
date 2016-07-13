#include "kjmlib.h"


//-----------------------------------------------------------------------------
// バージョン情報の表示
//-----------------------------------------------------------------------------
void version() {
	_tcout << kjm::util::getModuleFileName() << _T(" ") << kjm::util::getFileVersion(kjm::util::getModuleFileName()) << _T("\n");
}

//-----------------------------------------------------------------------------
// 使用法の表示
//-----------------------------------------------------------------------------
void usage(const kjm::optionInfoList& opts) {
	version();
	_tcout << _T("usage: \n");

	kjm::optionInfoList::const_iterator ci;
	for (ci = opts.begin(); ci != opts.end(); ci++) {
		_tcout << _T("  ");
		if ((*ci).m_shortOption.empty()) {
			_tcout << _T("    ");
		} else {
			_tcout << _T("-") << (*ci).m_shortOption << _T(", ");
		}
		_tcout << _T("--") << (*ci).m_longOption;
		if ((*ci).m_optionArgInfo == kjm::no_argument) {
			_tcout << _T("\n");
		} else if ((*ci).m_optionArgInfo == kjm::optional_argument) {
			if (!(*ci).m_argDescription.empty()) {
				_tcout << _T("[=") << (*ci).m_argDescription << _T("]\n");
			} else {
				_tcout << _T("[=ARGUMENT]\n");
			}
		} else {
			if (!(*ci).m_argDescription.empty()) {
				_tcout << _T("=") << (*ci).m_argDescription << _T("\n");
			} else {
				_tcout << _T("=ARGUMENT\n");
			}
		}

		if (!(*ci).m_optDescription.empty()) {
			_tcout << _T("\t") << (*ci).m_optDescription << _T("\n");
		}
	}
}

//-----------------------------------------------------------------------------
// 追加処理
//-----------------------------------------------------------------------------
bool doAppend(const kjm::cmdLine& cl) {
	kjm::_tstring fname = cl.getOptArgument(_T("file"));
	kjm::_tstring windowTitle = cl.getOptArgument(_T("window-title"));
	kjm::_tstring program = cl.getOptArgument(_T("program"));
	kjm::_tstring execpath = cl.getOptArgument(_T("execpath"));

	if (fname.empty() || windowTitle.empty() || program.empty()) {
		_tcout << _T("追加モードのときは、--file, --window-title, --program を必ず指定する必要があります。\n");
		return false;
	}

	// セクションを最後まで列挙する
	kjm::iniFile ini(fname);
	_otstringstream oss;
	for (int i = 1;; i++) {
		oss.str(_T(""));
		oss << _T("WATCH") << i;
		
		kjm::_tstring sec = oss.str();

		kjm::_tstring curWindowTitle = ini.getString(sec, _T("WINDOW_TITLE"));
		
		if (curWindowTitle.empty()) {
			// エントリーを登録して終了
			ini.writeString(sec, _T("WINDOW_TITLE"), windowTitle);
			ini.writeString(sec, _T("PROGRAM"), program);
			ini.writeString(sec, _T("EXECPATH"), execpath);

			_tcout
				<< _T("以下の内容で、登録しました。\n")
				<< _T("[") << sec << _T("]\n")
				<< _T("WINDOW_TITLE=") << ini.getString(sec, _T("WINDOW_TITLE")) << _T("\n")
				<< _T("PROGRAM=") << ini.getString(sec, _T("PROGRAM")) << _T("\n")
				<< _T("EXECPATH=") << ini.getString(sec, _T("EXECPATH")) << _T("\n");
			break;
		} else if (lstrcmpi(curWindowTitle.c_str(), windowTitle.c_str()) == 0) {
			// すでに登録されている
			_tcout
				<< _T("以下の内容で、すでに登録されています。\n")
				<< _T("[") << sec << _T("]\n")
				<< _T("WINDOW_TITLE=") << curWindowTitle << _T("\n")
				<< _T("PROGRAM=") << ini.getString(sec, _T("PROGRAM")) << _T("\n")
				<< _T("EXECPATH=") << ini.getString(sec, _T("EXECPATH")) << _T("\n");

			break;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// 表示処理
//-----------------------------------------------------------------------------
bool doView(const kjm::cmdLine& cl) {
	kjm::_tstring fname = cl.getOptArgument(_T("file"));

	if (fname.empty()) {
		_tcout << _T("表示モードのときは、--file を必ず指定する必要があります。\n");
		return false;
	}

	// セクションを最後まで列挙する
	kjm::iniFile ini(fname);
	_otstringstream oss;
	for (int i = 1;; i++) {
		oss.str(_T(""));
		oss << _T("WATCH") << i;
		
		kjm::_tstring sec = oss.str();

		kjm::_tstring curWindowTitle = ini.getString(sec, _T("WINDOW_TITLE"));
		
		if (curWindowTitle.empty()) {
			break;
		}

		// すでに登録されている
		_tcout
			<< _T("[") << sec << _T("]\n")
			<< _T("WINDOW_TITLE=") << curWindowTitle << _T("\n")
			<< _T("PROGRAM=") << ini.getString(sec, _T("PROGRAM")) << _T("\n")
			<< _T("EXECPATH=") << ini.getString(sec, _T("EXECPATH")) << _T("\n")
			<< _T("\n");
	}

	return true;
}

//-----------------------------------------------------------------------------
// プログラムスタート
//-----------------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {
	STL_SETLOCALE_JAPAN;

	kjm::optionInfoList options;

	options.push_back(kjm::optionInfo(_T("file"), _T("f"),kjm::required_argument, _T(""), _T("INIファイルの名前")));

	options.push_back(kjm::optionInfo(_T("append"), _T("a"), kjm::no_argument, _T("追加モードで実行")));
	options.push_back(kjm::optionInfo(_T("view"), _T("v"), kjm::no_argument, _T("表示モードで実行")));

	options.push_back(kjm::optionInfo(_T("window-title"), _T("w"), kjm::required_argument, _T(""), _T("WINDOW_TITLEキーの値")));
	options.push_back(kjm::optionInfo(_T("program"), _T("p"), kjm::required_argument, _T(""), _T("PROGRAMキーの値")));
	options.push_back(kjm::optionInfo(_T("execpath"), _T("e"), kjm::required_argument, _T(""), _T("EXECPATHキーの値")));
	options.push_back(kjm::optionInfo(_T("help"), _T(""), kjm::no_argument, _T("このヘルプを表示。")));
	options.push_back(kjm::optionInfo(_T("version"), _T(""), kjm::no_argument, _T("バージョン情報を表示。")));

	kjm::cmdLine cl;

	cl.parse(argc, argv, options);

	if (cl.hasOption(_T("version"))) {
		version();
		return 0;
	}

	if (cl.hasOption(_T("append"))) {
		// 追加処理を実行
		doAppend(cl);
	} else if (cl.hasOption(_T("view"))) {
		// 表処理を実行
		doView(cl);
	} else {
		usage(options);
	}

	return 0;
}
