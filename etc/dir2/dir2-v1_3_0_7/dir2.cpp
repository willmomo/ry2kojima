#pragma warning(disable : 4786)

#include <iostream>
#include "kjmlib.h"

using namespace std;

bool g_crc = false;		// --crc �I�v�V��������̂Ƃ� true
bool g_csv = false;		// --csv �I�v�V��������̂Ƃ� true

kjm::_tstring g_path;

//---------------------------------------------------------------------
// �ꗗ�J�n
//---------------------------------------------------------------------
void listing(const kjm::_tstring& wc) {
	kjm::folderInfo fld(kjm::path::removeFileSpec(wc));

	// �܂��t�@�C��������񋓂���
	std::vector<kjm::fileInfo> files = fld.getFiles(kjm::path::findFileName(wc));
	for (std::vector<kjm::file>::size_type i = 0; i < files.size(); i++) {
		if (g_csv) {
			_tout << _T("\"") << files[i].name() << _T("\",");
			_tout << _T("\"") << (kjm::path::removeFileSpec(files[i].fullName()).c_str() + g_path.length()) << _T("\",");
			_tout << files[i].getFileVersion() << _T(",");
			_tout << files[i].lastWriteTime().toString() << _T(",");
			_tout << files[i].length();

			if (g_crc) {
				_tout << _T(",") << kjm::util::calcCRC16(files[i].fullName()) << _T("\n");
			} else {
				_tout << _T("\n");
			}
		} else {
			_tout << files[i].name() << _T("\t");
			_tout << (kjm::path::removeFileSpec(files[i].fullName()).c_str() + g_path.length()) << _T("\t");
			_tout << files[i].getFileVersion() << _T("\t");
			_tout << files[i].lastWriteTime().toString() << _T("\t");
			_tout << files[i].length();

			if (g_crc) {
				_tout << _T("\t") << kjm::util::calcCRC16(files[i].fullName()) << _T("\n");
			} else {
				_tout << _T("\n");
			}
		}
	}

  // ���ɍċA�I�Ƀt�H���_����������B
	// ���ׂẴt�@�C���������ł���悤�� *.* �ɒu�������Č����B

	std::vector<kjm::folderInfo> subFolders = fld.getSubFolders();
	for (std::vector<kjm::folderInfo>::size_type j = 0; j < subFolders.size(); j++) {
		listing(kjm::path::append(subFolders[j].getFullPath(), kjm::path::findFileName(wc)));
	}
}


//---------------------------------------------------------------------
// �v���O���� �X�^�[�g
//---------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {
	std::locale::global(std::locale("japanese"));

	kjm::commandLine cl(argc, argv, _T(""));

	g_crc = cl.hasKey(_T("crc"));	// --crc �I�v�V����������H
	g_csv = cl.hasKey(_T("csv"));	// --csv �I�v�V����������?

	kjm::_tstrings arguments = cl.getArgList();	// �I�v�V�����ȊO�̈������擾

	// �I�v�V�������
	if (cl.hasKey(_T("help")) || cl.hasKey(_T("?")) || cl.hasKey(_T("version")) || arguments.size() == 0) {
		_tout << kjm::util::getModuleFileName() << _T(" ver.") << kjm::util::getFileVersion(kjm::util::getModuleFileName()) << _T("\n");
		_tout << _T("build [ ") << kjm::util::getMscVerText() << _T(" ]\n");
#if defined(UNICODE)
		_tout << _T("code  [ UNICODE ]\n");
#else
		_tout << _T("code  [ MBCS ]\n");
#endif
		_tout << _T("�g�p�@�Fdir2 �t�H���_�� or ���C���h�J�[�h\n");
		_tout << _T("\n");
		_tout << _T(" --crc  crc16���v�Z���ďo�͂���B\n");
		_tout << _T(" --csv  csv�`���ŏo�͂���B\n");

		return -1;
	}

	for (kjm::_tstrings::size_type i = 0; i < arguments.size(); i++) {
		kjm::_tstring wc = arguments[i];

		// �^����ꂽ�p�X�����΃p�X�̂Ƃ��́A�J�����g�f�B���N�g������̐�΃p�X��
		// �ϊ����Ă��珈�����s���B
		if (PathIsRelative(wc.c_str())) {
			std::vector<TCHAR> cd(MAX_PATH);
			GetCurrentDirectory(cd.size(), &cd[0]);
			wc = kjm::path::append(&cd[0], wc);
		}

		if (kjm::path::isDirectory(wc)) {
			wc = kjm::path::append(wc, _T("*.*"));
		}

		g_path = kjm::path::removeFileSpec(wc);

		listing(wc);
	}

	return 0;
}
