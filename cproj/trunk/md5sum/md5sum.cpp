/*
*/

#include "kjmlib.h"
#include "md5.h"
#include "resource.h"

bool g_binaryMode = true;

kjm::_tstring calc_md5(const kjm::_tstring& fname, bool verbose) {
	bool first = false;
	kjm::_tstring result;

	FILE* fp = _tfopen(fname.c_str(), (g_binaryMode ? _T("rb") : _T("r")));
	if (fp) {
		MD5 md5;

		unsigned char buffer[1024];
		int len;
		__int64 rlen = 0;
		__int64 flen = _filelengthi64(_fileno(fp));

		kjm::tickCounter cnt(flen);
		while (len = fread(buffer, sizeof(char), countof(buffer), fp)) {
			cnt.m_count += len;
			if (cnt.lap(1000)) {
				if (verbose && cnt.elapseSec() > 3) {
					if (first == false) {
						_tprintf(_T("計算中 << %s >>\n"), kjm::path::findFileName(fname).c_str());
						first = true;
					}

					_tprintf(_T("  %lld/%lld %d%% 後 %d 秒  \r"),
						cnt.m_count, cnt.m_totalCount, cnt.progress(), cnt.estimateRemainingTime()/1000);
				}
			}
			md5.update(buffer, len);
		}
		if (verbose && cnt.elapseSec() > 3) {
			_tprintf(_T("  %lld/%lld %d%%               \n"),
				cnt.m_count, cnt.m_totalCount, cnt.progress());
		}
		md5.finalize();
		fclose(fp);

#if defined(UNICODE)
		result = kjm::util::toUnicode(md5.hex_digest());
#else
		result = md5.hex_digest();
#endif
	} else {
		_tprintf(_T("md5sum: %s: %s\n"), fname.c_str(), _tcserror(errno));
	}

	return result;
}

// main 処理
void get_md5(const kjm::_tstring& fname, const kjm::cmdLine& cmd) {
	
	// output オプションが指定されているときは、進捗報告を行う。
	bool verbose = cmd.hasOption(_T("output"));

	kjm::_tstring md5 = calc_md5(fname, verbose);

	_tprintf(_T("%s %c%s\n"), md5.c_str(), (g_binaryMode ? '*' : ' '), fname.c_str());

	// oputput オプションが指定されているときは、指定ファイルにも追記する
	if (cmd.hasOption(_T("output"))) {
		FILE* fp = _tfopen(cmd.getOptArgument(_T("output")).c_str(), _T("a+"));
		if (fp) {
			_ftprintf(fp, _T("%s %c%s\n"), md5.c_str(), (g_binaryMode ? '*' : ' '), fname.c_str());
			fclose(fp);
		}
	}
}

/*
 * md5ファイルの中と対象ファイルのmd5を突き合わせる
 */
int check_md5(const kjm::_tstring& fname, const kjm::cmdLine& cmd) {
	int file_cnt = 0;
	int ng_cnt = 0;
	bool result = false;
	kjm::cfile file;
	if (file.open(fname, _T("r"))) {
		kjm::_tstring lineBuf;

		while (file.getline(lineBuf) == 0) {
			kjm::_tstrings cols = kjm::util::split(lineBuf, ' ');

			//_tprintf(_T("%s\n"), lineBuf.c_str());
			//_tprintf(_T("%d\n"), cols.size());
			
			if (cols.size() == 2) {
				// cols[0] が md5
				// cols[1] が ファイル名のハズ

				//_tprintf(_T("%s\n"), cols[1].c_str());
				kjm::_tstring fname = (cols[1].at(0) == '*' ? cols[1].substr(1) : cols[1]);
				kjm::_tstring md5 = calc_md5(fname, (cmd.hasOption(_T("status")) ? false : true));
				//_tprintf(_T("%s\n"), md5.c_str());

				++file_cnt;
				if (cols[0] == md5) {
					if (cmd.hasOption(_T("status")) == false && cmd.hasOption(_T("quiet")) == false) {
						_tprintf(_T("%s: 完了\n"), fname.c_str());
					}
				} else {
					++ng_cnt;
					if (cmd.hasOption(_T("status")) == false) {
						_tprintf(_T("%s: %s\n"), fname.c_str(), (md5.empty() ? _T("オープンまたは読み込みに失敗しました") : _T("失敗")));
					} else {
						// status 指定の時は、一つ異常が見つかれば、即終了
						break;
					}
				}
			}
		}
		file.close();

		if (cmd.hasOption(_T("status")) == false && ng_cnt > 0) {
			_tprintf(_T("md5sum: WARNING: %d of %d computed %s did NOT match\n"),
				ng_cnt, file_cnt, (file_cnt > 1 ? _T("checksums") : _T("checksum")) );
		}
	}

	return ng_cnt;
}

void version() {
	_tcout << _T("md5sum ") << kjm::util::getFileVersion() << _T("\n");
	exit( EXIT_SUCCESS );
}

void usage(int exitCode) {
	_tcout << _T("Usage: md5sum [OPTION]... [FILE]...") << _T("\n");
	_tcout << _T("With no FILE, or when FILE is -, read standard input.") << _T("\n");
	_tcout << _T("\n");
	_tcout << _T("  -b, --binary            read in binary mode (default)") << _T("\n");
	_tcout << _T("  -c, --check             read MD5 sums from the FILEs and check them") << _T("\n");
	_tcout << _T("  -o, --output            md5の計算結果を指定ファイルに出力") << _T("\n");
	_tcout << _T("  -t, --text              read in text mode") << _T("\n");
	_tcout << _T("\n");
	_tcout << _T("The following three options are useful only when verifying checksums:\n");
	_tcout << _T("      --quiet             don't print OK for each successfully verified file") << _T("\n");
	_tcout << _T("      --status            don't output anything, status code shows success") << _T("\n");
	_tcout << _T("  -w, --warn              warn about improperly formatted checksum lines") << _T("\n");
	_tcout << _T("\n");
	_tcout << _T("      --help     この使い方を表示して終了") << _T("\n");
	_tcout << _T("      --version  バージョン情報を表示して終了") << _T("\n");
	_tcout << _T("\n");
	_tcout << _T("チェックサムは RFC 1321 に記されている通りに計算されます. 照合の際に入力\n");
	_tcout << _T("するファイルは, このプログラムによって出力された形式でなければなりません.\n");
	_tcout << _T("特にオプションを指定しないで実行した場合には, それぞれのファイル毎に\n");
	_tcout << _T("チェックサム, タイプに関する印(バイナリには `*', テキストには ` '), \n");
	_tcout << _T("及びファイル名を表示します.\n");

	exit( exitCode );
}

// program start
int _tmain(int argc, TCHAR** argv) {
	//SET_UNICODE_MODE;
	STL_SETLOCALE_JAPAN;

	kjm::optionInfoList opts;
	opts.push_back(kjm::optionInfo(_T("binary"), _T("b"), kjm::no_argument));
	opts.push_back(kjm::optionInfo(_T("check"), _T("c"), kjm::no_argument));
	opts.push_back(kjm::optionInfo(_T("text"), _T("t"), kjm::no_argument));
	opts.push_back(kjm::optionInfo(_T("output"), _T("o"), kjm::required_argument));
	opts.push_back(kjm::optionInfo(_T("quiet"), _T(""), kjm::no_argument));
	opts.push_back(kjm::optionInfo(_T("status"), _T(""), kjm::no_argument));
	opts.push_back(kjm::optionInfo(_T("warn"), _T("w"), kjm::no_argument));
	opts.push_back(kjm::optionInfo(_T("help"), _T(""), kjm::no_argument));
	opts.push_back(kjm::optionInfo(_T("version"), _T(""), kjm::no_argument));

	kjm::cmdLine cmd;
	cmd.parse(argc, argv, opts);

	int exit_code = 0;

	if (cmd.hasOption(_T("version"))) {
		version();	// バージョンを表示して正常終了。
	}

	if (cmd.hasOption(_T("help"))) {
		usage( EXIT_SUCCESS );	// 使用法を表示して正常終了。
	}

	for (int i = 0; i < cmd.get_argCount(); i++) {
		kjm::_tstrings files = kjm::directory::glob(cmd.get_argument(i));

		for (int j = 0; j < files.size(); j++) {
			if (cmd.hasOption(_T("check"))) {
				if (check_md5(files[j], cmd) != 0) {
					exit_code = 1;
				}

				if (exit_code != 0 && cmd.hasOption(_T("status")) != false) {
					break;
				}
			} else {
				get_md5(files[j], cmd);
			}
		}
	}

	return exit_code;
}
