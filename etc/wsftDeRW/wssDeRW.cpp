//-----------------------------------------------------------------------------
// wsft.dll ���g���āA�ǂݏ�������T���v���v���O����
//-----------------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>
#include "kjmsub.h"

#include "wsft.h"
#pragma comment(lib, "wsft.lib")

#define LOGC_NAME	_T("WsDeRW")

//-----------------------------------------------------------------------------
// �g�p�@�̕\��
//-----------------------------------------------------------------------------
void usage() {
	_tout << _T(" -f, --file=<��������|�Ǎ��݃t�@�C����>\n");
	_tout << _T(" -h, --host=<�z�X�g��>\n");
	_tout << _T(" -l, --loop=<���[�v��>\n");
	_tout << _T(" -m, --mode=<read|write>\n");
	_tout << _T(" -n, --no=<�t�@�C���ԍ�>\n");
}

//-----------------------------------------------------------------------------
// �I�v�V�����̃`�F�b�N
//-----------------------------------------------------------------------------
bool chkarg(const _tstring& host, const _tstring& mode, int no) {
	if (mode != _T("read") && mode != _T("write")) {
		_tout << _T("--mode=<read|write> �Ŏw�肵�Ă��������B\n");
		return false;
	}

	if (no < 1000 || no > 9999) {
		_tout << _T("--no=<1000�`9999> �Ŏw�肵�Ă��������B\n");
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// �Ǎ��ݏ���
//
// 0 ����I��
// -1 ���擾�G���[
// -2 �t�@�C�������݃G���[
//-----------------------------------------------------------------------------
int readAction(int no, const _tstring& file, int& wsftErr) {
	int result = 0;

	long fsize = 0;
	if ((wsftErr = wsft_readfile(no, 0, (char*)&fsize, 0)) == 0) {
		std::vector<char> buf(fsize);
		_tout << fsize << _T(" �o�C�g�̃f�[�^��Ǎ��ݒ�...") << std::ends;
		wsftErr = wsft_readfile(no, 0, &buf[0], buf.size());
		_tout << _T("\n");

		_tofstream ofs;
		ofs.open(file.c_str(), std::ios::out | std::ios::binary);
		if (ofs.is_open()){
			ofs.write(&buf[0], buf.size());
			ofs.close();

		} else {
			result = -2;
		}
	} else {
		result = -1;
	}

	return result;
}

//-----------------------------------------------------------------------------
// �����ݏ���
//
// 0 ����I��
// -1 ���擾�G���[
// -2 �t�@�C�������݃G���[
//-----------------------------------------------------------------------------
int writeAction(int no, const _tstring& file, int& wsftErr) {
	int result = 0;

	long fsize = 0;
	if ((wsftErr = wsft_readfile(no, 0, (char*)&fsize, 0)) == 0) {
		std::vector<char> buf(fsize);

		_tifstream ifs;
		ifs.open(file.c_str(), std::ios::in | std::ios::binary);
		if (ifs.is_open()) {
			ifs.read(&buf[0], buf.size());
			ifs.close();

			_tout << fsize << _T(" �o�C�g�̃f�[�^�������ݒ�...") << std::ends;
			wsftErr = wsft_writefile(no, 0, &buf[0], buf.size());
			_tout << _T("\n");

		} else {
			result = -2;
		}
	} else {
		result = -1;
	}

	return result;
}

//-----------------------------------------------------------------------------
// �A�v���P�[�V�����̏����J�n
//-----------------------------------------------------------------------------
void doMain(const kjm::commandLine& args) {
	// �����̏���
	_tstring host = _T("as1");
	_tstring mode = _T("");
	_tstring file = _T("");
	int no = 0;
	int loop = 1;

	std::vector<_tstring> opts = args.getOptList();

	if (opts.size() == 0 || args.getArgListCount() != 0) {
		usage();
		return;
	}

	for (int i = 0; i < opts.size(); i++) {
		if ((opts[i] == _T("host") || opts[i] == _T("h")))		host = args.getOptArg(opts[i]);
		else if ((opts[i] == _T("no") || opts[i] == _T("n")))	no = _ttoi(args.getOptArg(opts[i]).c_str());
		else if ((opts[i] == _T("mode") || opts[i] == _T("m")))	mode = args.getOptArg(opts[i]);
		else if ((opts[i] == _T("file") || opts[i] == _T("f")))	file = args.getOptArg(opts[i]);
		else if ((opts[i] == _T("loop") || opts[i] == _T("l")))	loop = _ttoi(args.getOptArg(opts[i]).c_str());
	}

	// �I�v�V�����̃`�F�b�N
	if (chkarg(host, mode, no) == false) {
		usage();
		return;
	}

	// �t�@�C�������w�肳��Ă��Ȃ��Ƃ��́A�����ō쐬����B
	if (file.empty()) {
		_otstringstream oss;
		oss.imbue(std::locale("C"));
		oss << _T("mf") << no << _T(".bin");
		file = oss.str();
	}

	// wsft�̏�����
	wsft_start(NULL);

	// �ʐM���ݒ�
	wsft_setmaxsize(8192);
	wsft_setservstr(host.c_str(), wsft_getopt(WSFT_OPT_RPORT), wsft_getopt(WSFT_OPT_WPORT));

	// wsft�Ńt�@�C�����擾����
	for (int i = 0; i < loop; i++) {
		_tout << _T("[") << std::setw(3) << (i + 1) << _T("] ");

		int wsftErr = 0;
		if (mode == "read") {
			switch (readAction(no, file, wsftErr)) {
				case -1:
					_tout << _T("�t�@�C�����擾�G���[: ") << wsftErr << _T("\n");
					break;
				case -2:
					_tout << _T("�t�@�C�� '") << file << _T("' ���I�[�v���ł��܂���B\n");
					break;
				default:
					_tout << host << _T(" �� SMF �t�@�C�� ") << no <<  _T(" ���A'") << file << _T("' �ɕۑ����܂����B\n");
					break;
			}
		} else if (mode == "write") {
			switch (writeAction(no, file, wsftErr)) {
				case -1:
					_tout << _T("�t�@�C�����擾�G���[: ") << wsftErr << _T("\n");
					break;
				case -2:
					_tout << _T("�t�@�C�� '") << file << _T("' ���I�[�v���ł��܂���B\n");
					break;
				default:
					_tout << file << _T("' �� ") << host << _T(" �� SMF �t�@�C�� ") << no <<  _T(" �ɏ������݂܂����B\n");
					break;
			}
		}
	}

	// wsft�̃N���[���i�b�v
	wsft_end();
}

//-----------------------------------------------------------------------------
// �v���O�����X�^�[�g
//-----------------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {
	STL_SETLOCALE_JAPAN;

	Winssa_Log(LOGC_NAME, LOGLV_INFO, _T("----[ %s (%s) ]---- start."),
		kjm::util::getModuleFileName().c_str(), kjm::util::getFileVersion().c_str());
	WLDeleteOldLog();

	// �A�v���P�[�V�����̏����J�n
	doMain(kjm::commandLine(argc, argv, _T("fhmn")));

	Winssa_Log(LOGC_NAME, LOGLV_INFO, _T("----[ %s (%s) ]---- end."),
		kjm::util::getModuleFileName().c_str(), kjm::util::getFileVersion().c_str());

	return 0;
}
