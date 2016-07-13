//---------------------------------------------------------------------
// 超簡易ファイル比較ツール
//
// 不一致を素早く検出し、さっさと終了したいので、不一致チェックに特化し
// たツール。
//
// [終了コード]
// 0 ファイルは、同じ内容。
// 1 オプションが正しく指定されていない。
// 2 一つ目に指定したファイルが、存在しない。
// 3 二つ目に指定したファイルが、存在しない。
// 4 二つのファイルのサイズが不一致。
// 5 二つのファイルの内容が不一致。
// 6 一つ目のファイルでリードエラー。
// 7 二つ目のファイルでリードエラー。
//
// 2008/10/23 r.kojima
//   終了コードの細分化。
//   バージョンリソースの追加。
//---------------------------------------------------------------------
#include "kjmlib.h"		// kjmlib[d].lib ver.1.2.0.0を想定
#include "getopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include "resource.h"


#define READ_SIZE (8192 * 10)

//---------------------------------------------------------------------
// 使用法を表示
//---------------------------------------------------------------------
void usage() {
	_tout << kjm::util::getModuleFileName() << _T(" ver.") << kjm::util::getFileVersion(kjm::util::getModuleFileName()) << _T("\n");
	_tout << _T("build [ ") << kjm::util::getMscVerText() << _T(" ]\n");
#if defined(UNICODE)
	_tout << _T("code  [ UNICODE ]\n");
#else
	_tout << _T("code  [ MBCS ]\n");
#endif
	_putts(_T("使用法：tfc [--progress] <ファイル1> <ファイル2>"));
}


//---------------------------------------------------------------------
// プログラム・スタート
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
			m_progress = true;	// 進行状況を表示
			break;

		default:
			usage();
			return 1;
			break;
		}
	}

	// オプションが指定されていないときは、エラー
	if (g_optind + 2 != argc) {
		usage();
		return 1;
	}

	// 比較コマンドを出力
	_tprintf(_T("TFC: '%s' '%s'\n"), argv[g_optind + 0], argv[g_optind + 1]);

	// file1 を開く
	FILE* fp1 = _tfopen(argv[g_optind + 0], _T("rb"));
	if (fp1 == NULL) {
		_tprintf(_T("cannot open file1 '%s'. errno: %d\n"), argv[g_optind + 0], errno);
		return 2;
	}

	// file2 を開く
	FILE* fp2 = _tfopen(argv[g_optind + 1], _T("rb"));
	if (fp2 == NULL) {
		_tprintf(_T("cannot open file2 '%s'. errno: %d\n"), argv[g_optind + 1], errno);
		fclose(fp1);
		return 3;
	}

	// ファイルのサイズを取得
	__int64 size1 = _filelengthi64(_fileno(fp1));
	__int64 size2 = _filelengthi64(_fileno(fp2));
	if (size1 != size2) {
		_putts(_T("サイズ不一致"));
		return 4;
	}

	// READ_SIZE バイトずつ読み込んで比較する
	char buf1[READ_SIZE];
	char buf2[READ_SIZE];

	int result = 0;
	int ret1, ret2;
	__int64 cnt = 0;

	do {
		ret1 = fread(buf1, sizeof(char), READ_SIZE, fp1);
		if (ferror(fp1)) {
			_tperror(_T("<ファイル1>の読み込みでエラー"));
			result = 6;
			break;
		}

		ret2 = fread(buf2, sizeof(char), READ_SIZE, fp2);
		if (ferror(fp2)) {
			_tperror(_T("<ファイル2>の読み込みでエラー"));
			result = 7;
			break;
		}

		if (m_progress) {
			cnt += ret1;
			_tprintf(_T("compare %3d%%\r"), (size1 == 0) ? 100 : (cnt * 100 / size1));
		}

		// 読み込みサイズを確認
		if (ret1 != ret2) {
			if (m_progress) _putts(_T(""));
			_putts(_T("サイズ不一致"));
			result = 4;
			break;
		}

		// 内容を比較
		if (memcmp(buf1, buf2, ret1) != 0) {
			if (m_progress) _putts(_T(""));
			_putts(_T("内容不一致"));
			result = 5;
			break;
		}

		// データを読みきっていれば、ループを抜ける
	} while (ret1 == READ_SIZE);

	if (m_progress) {
		_putts(_T(""));
	}

	// ファイルを閉じて終了
	fclose(fp1);
	fclose(fp2);

	return result;
}
