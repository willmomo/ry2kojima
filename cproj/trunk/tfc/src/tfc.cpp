//---------------------------------------------------------------------
// ���ȈՃt�@�C����r�c�[��
//
// �s��v��f�������o���A�������ƏI���������̂ŁA�s��v�`�F�b�N�ɓ�����
// ���c�[���B
//
// [�I���R�[�h]
// 0 �t�@�C���́A�������e�B
// 1 �I�v�V�������������w�肳��Ă��Ȃ��B
// 2 ��ڂɎw�肵���t�@�C�����A���݂��Ȃ��B
// 3 ��ڂɎw�肵���t�@�C�����A���݂��Ȃ��B
// 4 ��̃t�@�C���̃T�C�Y���s��v�B
// 5 ��̃t�@�C���̓��e���s��v�B
// 6 ��ڂ̃t�@�C���Ń��[�h�G���[�B
// 7 ��ڂ̃t�@�C���Ń��[�h�G���[�B
//
// 2008/10/23 r.kojima
//   �I���R�[�h�̍ו����B
//   �o�[�W�������\�[�X�̒ǉ��B
//---------------------------------------------------------------------
#include "kjmlib.h"		// kjmlib[d].lib ver.1.2.0.0��z��
#include "getopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include "resource.h"


#define READ_SIZE (8192 * 10)

//---------------------------------------------------------------------
// �g�p�@��\��
//---------------------------------------------------------------------
void usage() {
	_tout << kjm::util::getModuleFileName() << _T(" ver.") << kjm::util::getFileVersion(kjm::util::getModuleFileName()) << _T("\n");
	_tout << _T("build [ ") << kjm::util::getMscVerText() << _T(" ]\n");
#if defined(UNICODE)
	_tout << _T("code  [ UNICODE ]\n");
#else
	_tout << _T("code  [ MBCS ]\n");
#endif
	_putts(_T("�g�p�@�Ftfc [--progress] <�t�@�C��1> <�t�@�C��2>"));
}


//---------------------------------------------------------------------
// �v���O�����E�X�^�[�g
//---------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {
	STL_SETLOCALE_JAPAN;

	bool m_progress = false;

	option options[] = {
		{_T("progress"), no_argument, NULL, _T('p')},
		{NULL, no_argument, NULL, 0}
	};

	int c;
	int opt_index;
	while ((c = getopt_long(argc, argv, _T(""), options, &opt_index)) != -1) {
		switch (c) {
		case 'p':
			m_progress = true;	// �i�s�󋵂�\��
			break;

		default:
			usage();
			return 1;
			break;
		}
	}

	// �I�v�V�������w�肳��Ă��Ȃ��Ƃ��́A�G���[
	if (g_optind + 2 != argc) {
		usage();
		return 1;
	}

	// ��r�R�}���h���o��
	_tprintf(_T("TFC: '%s' '%s'\n"), argv[g_optind + 0], argv[g_optind + 1]);

	// file1 ���J��
	FILE* fp1 = _tfopen(argv[g_optind + 0], _T("rb"));
	if (fp1 == NULL) {
		_tprintf(_T("cannot open file1 '%s'. errno: %d\n"), argv[g_optind + 0], errno);
		return 2;
	}

	// file2 ���J��
	FILE* fp2 = _tfopen(argv[g_optind + 1], _T("rb"));
	if (fp2 == NULL) {
		_tprintf(_T("cannot open file2 '%s'. errno: %d\n"), argv[g_optind + 1], errno);
		fclose(fp1);
		return 3;
	}

	// �t�@�C���̃T�C�Y���擾
	__int64 size1 = _filelengthi64(_fileno(fp1));
	__int64 size2 = _filelengthi64(_fileno(fp2));
	if (size1 != size2) {
		_putts(_T("�T�C�Y�s��v"));
		return 4;
	}

	// READ_SIZE �o�C�g���ǂݍ���Ŕ�r����
	char buf1[READ_SIZE];
	char buf2[READ_SIZE];

	int result = 0;
	int ret1, ret2;
	__int64 cnt = 0;

	do {
		ret1 = fread(buf1, sizeof(char), READ_SIZE, fp1);
		if (ferror(fp1)) {
			_tperror(_T("<�t�@�C��1>�̓ǂݍ��݂ŃG���["));
			result = 6;
			break;
		}

		ret2 = fread(buf2, sizeof(char), READ_SIZE, fp2);
		if (ferror(fp2)) {
			_tperror(_T("<�t�@�C��2>�̓ǂݍ��݂ŃG���["));
			result = 7;
			break;
		}

		if (m_progress) {
			cnt += ret1;
			_tprintf(_T("compare %3d%%\r"), (size1 == 0) ? 100 : (cnt * 100 / size1));
		}

		// �ǂݍ��݃T�C�Y���m�F
		if (ret1 != ret2) {
			if (m_progress) _putts(_T(""));
			_putts(_T("�T�C�Y�s��v"));
			result = 4;
			break;
		}

		// ���e���r
		if (memcmp(buf1, buf2, ret1) != 0) {
			if (m_progress) _putts(_T(""));
			_putts(_T("���e�s��v"));
			result = 5;
			break;
		}

		// �f�[�^��ǂ݂����Ă���΁A���[�v�𔲂���
	} while (ret1 == READ_SIZE);

	if (m_progress) {
		_putts(_T(""));
	}

	// �t�@�C������ďI��
	fclose(fp1);
	fclose(fp2);

	return result;
}
