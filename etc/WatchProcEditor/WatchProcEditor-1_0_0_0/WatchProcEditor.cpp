#include "kjmlib.h"


//-----------------------------------------------------------------------------
// �o�[�W�������̕\��
//-----------------------------------------------------------------------------
void version() {
	_tcout << kjm::util::getModuleFileName() << _T(" ") << kjm::util::getFileVersion(kjm::util::getModuleFileName()) << _T("\n");
}

//-----------------------------------------------------------------------------
// �g�p�@�̕\��
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
// �ǉ�����
//-----------------------------------------------------------------------------
bool doAppend(const kjm::cmdLine& cl) {
	kjm::_tstring fname = cl.getOptArgument(_T("file"));
	kjm::_tstring windowTitle = cl.getOptArgument(_T("window-title"));
	kjm::_tstring program = cl.getOptArgument(_T("program"));
	kjm::_tstring execpath = cl.getOptArgument(_T("execpath"));

	if (fname.empty() || windowTitle.empty() || program.empty()) {
		_tcout << _T("�ǉ����[�h�̂Ƃ��́A--file, --window-title, --program ��K���w�肷��K�v������܂��B\n");
		return false;
	}

	// �Z�N�V�������Ō�܂ŗ񋓂���
	kjm::iniFile ini(fname);
	_otstringstream oss;
	for (int i = 1;; i++) {
		oss.str(_T(""));
		oss << _T("WATCH") << i;
		
		kjm::_tstring sec = oss.str();

		kjm::_tstring curWindowTitle = ini.getString(sec, _T("WINDOW_TITLE"));
		
		if (curWindowTitle.empty()) {
			// �G���g���[��o�^���ďI��
			ini.writeString(sec, _T("WINDOW_TITLE"), windowTitle);
			ini.writeString(sec, _T("PROGRAM"), program);
			ini.writeString(sec, _T("EXECPATH"), execpath);

			_tcout
				<< _T("�ȉ��̓��e�ŁA�o�^���܂����B\n")
				<< _T("[") << sec << _T("]\n")
				<< _T("WINDOW_TITLE=") << ini.getString(sec, _T("WINDOW_TITLE")) << _T("\n")
				<< _T("PROGRAM=") << ini.getString(sec, _T("PROGRAM")) << _T("\n")
				<< _T("EXECPATH=") << ini.getString(sec, _T("EXECPATH")) << _T("\n");
			break;
		} else if (lstrcmpi(curWindowTitle.c_str(), windowTitle.c_str()) == 0) {
			// ���łɓo�^����Ă���
			_tcout
				<< _T("�ȉ��̓��e�ŁA���łɓo�^����Ă��܂��B\n")
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
// �\������
//-----------------------------------------------------------------------------
bool doView(const kjm::cmdLine& cl) {
	kjm::_tstring fname = cl.getOptArgument(_T("file"));

	if (fname.empty()) {
		_tcout << _T("�\�����[�h�̂Ƃ��́A--file ��K���w�肷��K�v������܂��B\n");
		return false;
	}

	// �Z�N�V�������Ō�܂ŗ񋓂���
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

		// ���łɓo�^����Ă���
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
// �v���O�����X�^�[�g
//-----------------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {
	STL_SETLOCALE_JAPAN;

	kjm::optionInfoList options;

	options.push_back(kjm::optionInfo(_T("file"), _T("f"),kjm::required_argument, _T(""), _T("INI�t�@�C���̖��O")));

	options.push_back(kjm::optionInfo(_T("append"), _T("a"), kjm::no_argument, _T("�ǉ����[�h�Ŏ��s")));
	options.push_back(kjm::optionInfo(_T("view"), _T("v"), kjm::no_argument, _T("�\�����[�h�Ŏ��s")));

	options.push_back(kjm::optionInfo(_T("window-title"), _T("w"), kjm::required_argument, _T(""), _T("WINDOW_TITLE�L�[�̒l")));
	options.push_back(kjm::optionInfo(_T("program"), _T("p"), kjm::required_argument, _T(""), _T("PROGRAM�L�[�̒l")));
	options.push_back(kjm::optionInfo(_T("execpath"), _T("e"), kjm::required_argument, _T(""), _T("EXECPATH�L�[�̒l")));
	options.push_back(kjm::optionInfo(_T("help"), _T(""), kjm::no_argument, _T("���̃w���v��\���B")));
	options.push_back(kjm::optionInfo(_T("version"), _T(""), kjm::no_argument, _T("�o�[�W��������\���B")));

	kjm::cmdLine cl;

	cl.parse(argc, argv, options);

	if (cl.hasOption(_T("version"))) {
		version();
		return 0;
	}

	if (cl.hasOption(_T("append"))) {
		// �ǉ����������s
		doAppend(cl);
	} else if (cl.hasOption(_T("view"))) {
		// �\���������s
		doView(cl);
	} else {
		usage(options);
	}

	return 0;
}
