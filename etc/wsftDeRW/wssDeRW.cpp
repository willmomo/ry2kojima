//-----------------------------------------------------------------------------
// wsft.dll を使って、読み書きするサンプルプログラム
//-----------------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>
#include "kjmsub.h"

#include "wsft.h"
#pragma comment(lib, "wsft.lib")

#define LOGC_NAME	_T("WsDeRW")

//-----------------------------------------------------------------------------
// 使用法の表示
//-----------------------------------------------------------------------------
void usage() {
	_tout << _T(" -f, --file=<書き込み|読込みファイル名>\n");
	_tout << _T(" -h, --host=<ホスト名>\n");
	_tout << _T(" -l, --loop=<ループ回数>\n");
	_tout << _T(" -m, --mode=<read|write>\n");
	_tout << _T(" -n, --no=<ファイル番号>\n");
}

//-----------------------------------------------------------------------------
// オプションのチェック
//-----------------------------------------------------------------------------
bool chkarg(const _tstring& host, const _tstring& mode, int no) {
	if (mode != _T("read") && mode != _T("write")) {
		_tout << _T("--mode=<read|write> で指定してください。\n");
		return false;
	}

	if (no < 1000 || no > 9999) {
		_tout << _T("--no=<1000～9999> で指定してください。\n");
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// 読込み処理
//
// 0 正常終了
// -1 情報取得エラー
// -2 ファイル書込みエラー
//-----------------------------------------------------------------------------
int readAction(int no, const _tstring& file, int& wsftErr) {
	int result = 0;

	long fsize = 0;
	if ((wsftErr = wsft_readfile(no, 0, (char*)&fsize, 0)) == 0) {
		std::vector<char> buf(fsize);
		_tout << fsize << _T(" バイトのデータを読込み中...") << std::ends;
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
// 書込み処理
//
// 0 正常終了
// -1 情報取得エラー
// -2 ファイル書込みエラー
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

			_tout << fsize << _T(" バイトのデータを書込み中...") << std::ends;
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
// アプリケーションの処理開始
//-----------------------------------------------------------------------------
void doMain(const kjm::commandLine& args) {
	// 引数の準備
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

	// オプションのチェック
	if (chkarg(host, mode, no) == false) {
		usage();
		return;
	}

	// ファイル名が指定されていないときは、自動で作成する。
	if (file.empty()) {
		_otstringstream oss;
		oss.imbue(std::locale("C"));
		oss << _T("mf") << no << _T(".bin");
		file = oss.str();
	}

	// wsftの初期化
	wsft_start(NULL);

	// 通信先を設定
	wsft_setmaxsize(8192);
	wsft_setservstr(host.c_str(), wsft_getopt(WSFT_OPT_RPORT), wsft_getopt(WSFT_OPT_WPORT));

	// wsftでファイルを取得する
	for (int i = 0; i < loop; i++) {
		_tout << _T("[") << std::setw(3) << (i + 1) << _T("] ");

		int wsftErr = 0;
		if (mode == "read") {
			switch (readAction(no, file, wsftErr)) {
				case -1:
					_tout << _T("ファイル情報取得エラー: ") << wsftErr << _T("\n");
					break;
				case -2:
					_tout << _T("ファイル '") << file << _T("' がオープンできません。\n");
					break;
				default:
					_tout << host << _T(" の SMF ファイル ") << no <<  _T(" を、'") << file << _T("' に保存しました。\n");
					break;
			}
		} else if (mode == "write") {
			switch (writeAction(no, file, wsftErr)) {
				case -1:
					_tout << _T("ファイル情報取得エラー: ") << wsftErr << _T("\n");
					break;
				case -2:
					_tout << _T("ファイル '") << file << _T("' がオープンできません。\n");
					break;
				default:
					_tout << file << _T("' を ") << host << _T(" の SMF ファイル ") << no <<  _T(" に書き込みました。\n");
					break;
			}
		}
	}

	// wsftのクリーンナップ
	wsft_end();
}

//-----------------------------------------------------------------------------
// プログラムスタート
//-----------------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {
	STL_SETLOCALE_JAPAN;

	Winssa_Log(LOGC_NAME, LOGLV_INFO, _T("----[ %s (%s) ]---- start."),
		kjm::util::getModuleFileName().c_str(), kjm::util::getFileVersion().c_str());
	WLDeleteOldLog();

	// アプリケーションの処理開始
	doMain(kjm::commandLine(argc, argv, _T("fhmn")));

	Winssa_Log(LOGC_NAME, LOGLV_INFO, _T("----[ %s (%s) ]---- end."),
		kjm::util::getModuleFileName().c_str(), kjm::util::getFileVersion().c_str());

	return 0;
}
