//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
#include "patcher-config.h"


//---------------------------------------------------------------------
// �ő�� startup �Z�N�V�������擾
//---------------------------------------------------------------------
int patcherIni::getMaxStartupSection() const {
	// �󂢂Ă��� startup �Z�N�V������T��
	kjm::_tstring sectionName;
	kjm::_tstrings sections;
	int no = 0;
	do {
		++no;
		sectionName = kjm::util::sprintf_str((no == 1) ? _T("startup") : _T("startup%d"), no);
		sections = this->getSectionData2(sectionName);
	} while (sections.size() > 0);

	return no - 1;
}

//---------------------------------------------------------------------
// Startup �Z�N�V�����ɃG���g���[���u�ǉ��v����
//---------------------------------------------------------------------
void patcherIni::appendStartupSection(const _tstring& title, const _tstring& commandLine, const _tstring& currentDirectory) {
	int next = this->getMaxStartupSection() + 1;
	kjm::_tstring sectionName = kjm::util::sprintf_str((next == 1) ? _T("startup") : _T("startup%d"), next);

	this->writeString(sectionName, _T("Title"), title);
	this->writeString(sectionName, _T("CommandLine"), commandLine);
	this->writeString(sectionName, _T("CurrentDirectory"), currentDirectory);
}

//---------------------------------------------------------------------
// �^�C�g������ Startup �Z�N�V������T��
//---------------------------------------------------------------------
int patcherIni::findStartupSection(const _tstring& title) const {
	int sectionMax = this->getMaxStartupSection();

	BOOL useDefault = FALSE;
	kjm::_tstring sectionName;
	for (int i = 1; i <= sectionMax; i++) {
		sectionName = kjm::util::sprintf_str((i == 1) ? _T("startup") : _T("startup%d"), i);
		_tstring keyValue = this->getString(sectionName, _T("Title"), _T(""), &useDefault);
		if (kjm::icmp(title, keyValue)) {
			return i;
		}
	}

	return 0;
}

//=====================================================================

//---------------------------------------------------------------------
// �g�p�@��\��
//---------------------------------------------------------------------
void usage() {
	_putts(
		_T("usage: patcher-config command [OPTIONS] target-ini-file\n")
		_T("\n")
		_T("[command]\n")
		_T("  find-startup    startup �Z�N�V��������G���g���[������\n")
		_T("  append-startup  startup �ɃG���g���[��ǉ�\n")
		_T("\n")
		_T("[options]\n")
		_T("  --title=<title>\n")
		_T("  --command-line=<command line>\n")
		_T("  --current-directory=<working directory>\n")
		_T("\n")
		_T("ex) title �Ɉ�v���� startup �Z�N�V������T��\n")
		_T("  patcher-config find-startup --title=���C���A�v���Ď� C:\\patcher\\patcher_mpc2.ini\n")
		_T("  ������Ȃ����́A0 ��Ԃ��B�����������́A�����������̔ԍ���Ԃ��B\n")
		_T("\n")
		_T("ex) startup �Z�N�V�����Ɏw��̍��ڂ�ǉ�����B\n")
		_T("  patcher-config append-startup --title=���C���A�v���Ď� --command-line=C:\\MPC2\\GMCGazer.exe --current-directory=C:\\MPC2 C:\\patcher\\patcher_mpc2.ini\n")
		_T("  �d���̃`�F�b�N�͍s���Ȃ��B\n")
	);
}

//---------------------------------------------------------------------
// <find-start> �R�}���h�̎��s
//
// [startup*] �Z�N�V��������A�ړI�̃Z�N�V��������������
//---------------------------------------------------------------------
int doFindStartup(const _tstring& iniFile, const kjm::commandLine& cl) {
	_tstring title, commandLine, currentDirectory;

	if (cl.hasKey(_T("title"))) title = cl.getOptArg(_T("title"));
	if (cl.hasKey(_T("command-line"))) commandLine = cl.getOptArg(_T("command-line"));
	if (cl.hasKey(_T("current-directory"))) currentDirectory = cl.getOptArg(_T("current-directory"));

	patcherIni ini(iniFile);

	return ini.findStartupSection(title);
}

//---------------------------------------------------------------------
// <append-start> �R�}���h�̎��s
//
// [startup*] �Z�N�V��������A�ړI�̃Z�N�V��������������
//---------------------------------------------------------------------
int doAppendStartup(const _tstring& iniFile, const kjm::commandLine& cl) {
	_tstring title, commandLine, currentDirectory;

	if (cl.hasKey(_T("title"))) title = cl.getOptArg(_T("title"));
	if (cl.hasKey(_T("command-line"))) commandLine = cl.getOptArg(_T("command-line"));
	if (cl.hasKey(_T("current-directory"))) currentDirectory = cl.getOptArg(_T("current-directory"));

	patcherIni ini(iniFile);

	ini.appendStartupSection(title, commandLine, currentDirectory);

	return 0;
}

//---------------------------------------------------------------------
// �v���O�����X�^�[�g
//---------------------------------------------------------------------
int _tmain(int argc, TCHAR ** argv) {
	C_SETLOCALE_JAPAN;

	kjm::commandLine cl;
	cl.parse(argc, argv, _T("?v"));
	std::vector<_tstring> opts = cl.getOptList();

	if (cl.getArgListCount() != 2) {
		usage();
		return -1;
	}

	_tstring lowerCommand = kjm::util::toLower(cl.getArgList()[0]);

	int exitCode;

	if (lowerCommand == _T("find-startup")) {
		// startup �Z�N�V��������̌���
		exitCode = doFindStartup(cl.getArgList()[1], cl);
	} else if (lowerCommand == _T("append-startup")) {
		// startup �Z�N�V�����̒ǉ�
		exitCode = doAppendStartup(cl.getArgList()[1], cl);
	} else {
		exitCode = -1;
	}

	return exitCode;
}
