//-----------------------------------------------------------------------------
// �f�B�X�N�̗e�ʂ�\������v���O�����B
// �g�p��/���g�p����߂�l�ɂ���I�v�V����������B
//
// kjmlib : Version 0.2012.08.10 ���x�X�g�}�b�`���C�u����
//-----------------------------------------------------------------------------
#include ".\kjmlib-20120810\kjmlib.h"

//-----------------------------------------------------------------------------
// �g�p���@�̕\��
//-----------------------------------------------------------------------------
void usage() {
	_tprintf(_T("�g�p�@�F"));
	_tprintf(_T("usage : diskinfo [�h���C�u�� or ���L��] [options]\n"));
	_tprintf(_T("\n"));
	_tprintf(_T(" -r, --result=<�߂�l�̎��>\n"));
	_tprintf(_T("    used_percent : �g�p����߂�l�Ƃ���(0�`100)\n"));
	_tprintf(_T("    free_percent : ���g�p����߂�l�Ƃ���(0�`100)\n"));
	_tprintf(_T(" -v, --version\n"));
	_tprintf(_T("    �o�[�W�������̕\��\n"));
	_tprintf(_T(" -?, --help\n"));
	_tprintf(_T("    ���̃w���v��\��\n"));
	_tprintf(_T("\n"));
	_tprintf(_T("�h���C�u���w�肹���Ɏ��s����ƁAPC�ɑ��݂���h���C�u�����ׂė񋓂���B\n"));
	_tprintf(_T("���̏ꍇ�Aresult �I�v�V������t���Ă��A�Ō�̃h���C�u�̒l�ɂȂ��Ă��܂����߁A\n"));
	_tprintf(_T("���܂�Ӗ��������B\n"));
}

//-----------------------------------------------------------------------------
// �f�B�X�N(�h���C�u)�̏����o��
//-----------------------------------------------------------------------------
int printDiskInfo(const kjm::_tstring& disk, const kjm::_tstring& resultType) {
	int errorlevel = 0;

	ULARGE_INTEGER freeBytesAvailableToCaller;
	ULARGE_INTEGER totalNumberOfBytes;
	ULARGE_INTEGER totalNumberOfFreeBytes;

	_tprintf(_T("%-30s: %s\n"), _T("�f�B�X�N"), disk.c_str());
	if (_istalpha(disk[0])) {
		kjm::drive d(disk);
		_tprintf(_T("%-30s: %s\n"), _T("�f�B�X�N�̃^�C�v"), d.get_DriveTypeString().c_str());

		if ((d.get_DriveType() == DRIVE_REMOVABLE) && (!d.isDiskInDrive())) {
			_tprintf(_T("  No disk in drive.\n"));
			return 0;
		}
	}

	BOOL bRet = GetDiskFreeSpaceEx(disk.c_str(), &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes);
	if (bRet) {
		_tprintf(_T("%-30s: %14I64u �o�C�g\n"), _T("�f�B�X�N�̑��e��"), totalNumberOfBytes.QuadPart);
		_tprintf(_T("%-30s: %14I64u �o�C�g\n"), _T("�f�B�X�N�̋󂫗e��(�S��)"), totalNumberOfFreeBytes.QuadPart);
		_tprintf(_T("%-30s: %14I64u �o�C�g\n"), _T("�f�B�X�N�̋󂫗e��(���[�U�[��)"), freeBytesAvailableToCaller.QuadPart);

		if (!resultType.empty()) {
			if (resultType == _T("used_percent")) {
				// �f�B�X�N�̎g�p����߂�l�Ƃ���(0�`100)%
				errorlevel = ((totalNumberOfBytes.QuadPart - totalNumberOfFreeBytes.QuadPart) * 100) / totalNumberOfBytes.QuadPart;
				_tprintf(_T("%-30s: %3d %%\n"), _T("�f�B�X�N�̎g�p��"), errorlevel);
			} else if (resultType == _T("free_percent")) {
				// �f�B�X�N�̖��g�p����߂�l�Ƃ���(0�`100)%
				errorlevel = (totalNumberOfFreeBytes.QuadPart * 100) / totalNumberOfBytes.QuadPart;
				_tprintf(_T("%-30s: %3d %%\n"), _T("�f�B�X�N�̖��g�p��"), errorlevel);
			} else {
				errorlevel = -1;
				_tprintf(_T("result �I�v�V�����ɂ́Aused_percent �� free_percent ���w��ł��܂��B\n"));
				usage();
			}
		}
	} else {
		DWORD dw = GetLastError();
		if (dw == ERROR_NOT_READY) {
			_tprintf(_T("  The device is not ready.\n"));
		} else {
			_ftprintf(stderr, _T("GetDiskFreeSpaceEx(%s, ...) �Ɏ��s���܂���(error %u)�B\n"), disk.c_str(), GetLastError());
			errorlevel = -1;
		}
	}

	return errorlevel;
}

//-----------------------------------------------------------------------------
// �v���O�����X�^�[�g
//-----------------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {
	_tsetlocale(LC_ALL, _T("japan"));

	// �������
	kjm::optionInfoList opts;
	opts.push_back(kjm::optionInfo(_T("result"), _T("r"), kjm::required_argument));
	opts.push_back(kjm::optionInfo(_T("version"), _T("v"), kjm::no_argument));
	opts.push_back(kjm::optionInfo(_T("help"), _T("?"), kjm::no_argument));

	kjm::cmdLine cl;
	cl.parse(argc, argv, opts);

	if (cl.hasOption(_T("version"))) {
		_tprintf(_T("%s ver.%s\n"), kjm::util::getModuleFileName().c_str(), kjm::util::getFileVersion().c_str());
		return 0;
	}

	if (cl.hasOption(_T("help"))) {
		usage();
		return 0;
	}

	int errorlevel = 0;

	if (cl.get_argCount() == 0) {
		kjm::driveList driveList;
		for (int i = 0; i < driveList.get_Size(); i++) {
			kjm::drive drive = driveList.get_Drive(i);

			errorlevel = printDiskInfo(drive.get_DriveLetter(), cl.hasOption(_T("result")) ? cl.getOptArgument(_T("result")) : _T(""));
			_tprintf(_T("\n"));
		}
	} else if (cl.get_argCount() == 1) {
		errorlevel = printDiskInfo(cl.get_argument(0), cl.hasOption(_T("result")) ? cl.getOptArgument(_T("result")) : _T(""));
	} else {
		usage();
		return -1;
	}

	return errorlevel;
}
