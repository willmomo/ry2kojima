#pragma warning(disable : 4786)

#include <iostream>
#include "kjmlib.h"

using namespace std;

bool g_crc = false;		// --crc オプションありのとき true
bool g_csv = false;		// --csv オプションありのとき true

kjm::_tstring g_path;

//---------------------------------------------------------------------
// 一覧開始
//---------------------------------------------------------------------
void listing(const kjm::_tstring& wc) {
	kjm::folderInfo fld(kjm::path::removeFileSpec(wc));

	// まずファイルだけを列挙する
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

  // 次に再帰的にフォルダを処理する。
	// すべてのファイルが検索できるように *.* に置き換えて検索。

	std::vector<kjm::folderInfo> subFolders = fld.getSubFolders();
	for (std::vector<kjm::folderInfo>::size_type j = 0; j < subFolders.size(); j++) {
		listing(kjm::path::append(subFolders[j].getFullPath(), kjm::path::findFileName(wc)));
	}
}


//---------------------------------------------------------------------
// プログラム スタート
//---------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {
	std::locale::global(std::locale("japanese"));

	kjm::commandLine cl(argc, argv, _T(""));

	g_crc = cl.hasKey(_T("crc"));	// --crc オプションがある？
	g_csv = cl.hasKey(_T("csv"));	// --csv オプションがある?

	kjm::_tstrings arguments = cl.getArgList();	// オプション以外の引数を取得

	// オプション解析
	if (cl.hasKey(_T("help")) || cl.hasKey(_T("?")) || cl.hasKey(_T("version")) || arguments.size() == 0) {
		_tout << kjm::util::getModuleFileName() << _T(" ver.") << kjm::util::getFileVersion(kjm::util::getModuleFileName()) << _T("\n");
		_tout << _T("build [ ") << kjm::util::getMscVerText() << _T(" ]\n");
#if defined(UNICODE)
		_tout << _T("code  [ UNICODE ]\n");
#else
		_tout << _T("code  [ MBCS ]\n");
#endif
		_tout << _T("使用法：dir2 フォルダ名 or ワイルドカード\n");
		_tout << _T("\n");
		_tout << _T(" --crc  crc16を計算して出力する。\n");
		_tout << _T(" --csv  csv形式で出力する。\n");

		return -1;
	}

	for (kjm::_tstrings::size_type i = 0; i < arguments.size(); i++) {
		kjm::_tstring wc = arguments[i];

		// 与えられたパスが相対パスのときは、カレントディレクトリからの絶対パスに
		// 変換してから処理を行う。
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
