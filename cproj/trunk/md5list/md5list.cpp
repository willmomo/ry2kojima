//-----------------------------------------------------------------------------
// -r  指定フォルダ以下すべてのファイルのMD5SUMを計算する。
// -s  指定フォルダを表示しないように出力する。
//-----------------------------------------------------------------------------
/*
  だいたい、次の流れでMD5SUMチェックができます。

  >md5list -r -s "\\ts00\share\@開発\document\IFBOX\v1.0 (4.3.0.0)\modules\4.3.0.0 P01\ifpc">ifpc.md5
  >move ifpc.md5 "\\ts00\share\@開発\document\IFBOX\v1.0 (4.3.0.0)\modules\4.3.0.0 P01\ifpc"

  >pushd "\\ts00\share\@開発\document\IFBOX\v1.0 (4.3.0.0)\modules\4.3.0.0 P01\ifpc"
  >md5sum -v -c ifpc.md5 2>md5chk.txt
*/

#include "md5.h"
#include <stdio.h>
//#include <iostream.h>
#include <iostream>
#include "windows.h"
#include <winbase.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "kjmlib.h"

#include "resource.h"

class md5listProgram : public kjm::program {
private:
	kjm::_tstring m_base;

public:
	md5listProgram() {}
	virtual ~md5listProgram() {}

	virtual bool parseOption(int& exitCode);
	virtual int doMain();

	void md5list(const kjm::_tstring& path);
	int checkFile();
};

//-----------------------------------------------------------------------------
// MD5が記載されているファイルを読みながらチェックする
//-----------------------------------------------------------------------------
int md5listProgram::checkFile() {
	int ret = 0;

	FILE* fp = _tfopen(this->m_cmdLine.getOptArgument(_T("check")).c_str(), _T("r"));
	if (fp) {
		kjm::_tstring line;

		while (kjm::util::getline(fp, line) == 0) {
			kjm::_tstring md5sum = kjm::_tstring(line).substr(0, 32);
			kjm::_tstring fname = kjm::_tstring(line).substr(34);

			kjm::_tstring path = kjm::path::combine(this->m_cmdLine.get_argument(0), fname);
			FILE* fp2 = _tfopen(path.c_str(), _T("rb"));
			if (fp2) {
				MD5 cMd5(fp2);
				char* pchHexDigest = cMd5.hex_digest();
				if (pchHexDigest) {
					if (lstrcmp(md5sum.c_str(), kjm::util::toUnicode(pchHexDigest).c_str()) == 0) {
						if (this->m_cmdLine.hasOption(_T("shy")) == false && this->m_cmdLine.hasOption(_T("silent")) == false) {
							_tprintf(_T("[ok] %s %s\n"), md5sum.c_str(), path.c_str());
						}
					} else {
						if (this->m_cmdLine.hasOption(_T("silent")) == false) {
							_tprintf(_T("[NG] %s\n"), md5sum.c_str());
							_tprintf(_T("[NG] %s %s\n"), kjm::util::toUnicode(pchHexDigest).c_str(), path.c_str());
						}
						ret = 1;
					}
					delete[] pchHexDigest;
				}
				fclose(fp2);
			} else {
				_tprintf(_T("ファイル '%s' が、オープンできません。\n"), path.c_str());
			}
		}
		fclose(fp);
	} else {
		_tprintf(_T("ファイル '%s' が、オープンできません。\n"), this->m_cmdLine.getOptArgument(_T("check")).c_str());
		ret = 1;
	}

	return ret;
}

//-----------------------------------------------------------------------------
// MD5SUMを計算して画面に出力
//-----------------------------------------------------------------------------
void md5listProgram::md5list(const kjm::_tstring& path) {
	kjm::_tstring findPath;		// 検索するディレクトリ
	kjm::_tstring findPattern;	// 検索するファイルのパターン

	if (kjm::util::isDirectory(path)) {
		findPath = path;
		findPattern = _T("*.*");
	} else {
		findPath = kjm::path::removeFileSpec(path);
		findPattern = kjm::path::findFileName(path);
	}

	// まずはファイルの MD5 を求める
	kjm::_tstrings files = kjm::directory::getFiles(findPath, findPattern);
	for (size_t i = 0; i < files.size(); i++) {

		// 拡張子 md5 のファイルは除外
		if (lstrcmpi(kjm::path::getExtension(files[i]).c_str(), _T(".md5")) == 0) {
			continue;
		}

		FILE* fp = _tfopen(files[i].c_str(), _T("rb"));
		if (fp) {
			MD5 cMd5(fp);
			char* pchHexDigest = cMd5.hex_digest();
			if (pchHexDigest) {
				if (m_cmdLine.hasOption(_T("short"))) {
					_tprintf(_T("%s *%s\n"), kjm::util::toUnicode(pchHexDigest).c_str(), files[i].substr(m_base.length()).c_str());
				} else {
					_tprintf(_T("%s *%s\n"), kjm::util::toUnicode(pchHexDigest).c_str(), files[i].c_str());
				}
				delete[] pchHexDigest;
			}
			fclose(fp);
		} else {
			_tprintf(_T("fopen('%s') error: %d\n"), files[i].c_str(), errno);
		}
	}

	// 再帰オプションが ON のときは、ディレクトリを下っていく
	if (m_cmdLine.hasOption(_T("recursive"))) {
		kjm::_tstrings dirs = kjm::directory::getSubFolders(findPath);
		for (size_t i = 0; i < dirs.size(); i++) {
			md5list(kjm::path::combine(dirs[i], findPattern));
		}
	}
}

//-----------------------------------------------------------------------------
// 引数解析処理
//-----------------------------------------------------------------------------
bool md5listProgram::parseOption(int& exitCode) {
	bool fContinue = true;

	m_options.push_back(kjm::optionInfo(_T("recursive"), _T("r"), kjm::no_argument, _T("サブディレクトリの中もMD5を計算する。")));
	m_options.push_back(kjm::optionInfo(_T("short"), _T("s"), kjm::no_argument, _T("ファイル名を短く表示する。")));
	m_options.push_back(kjm::optionInfo(_T("check"), _T("c"), kjm::required_argument, _T("MD5をチェックする。")));
	m_options.push_back(kjm::optionInfo(_T("shy"), _T(""), kjm::no_argument, _T("チェックのとき、NGだけを報告する。")));
	m_options.push_back(kjm::optionInfo(_T("silent"), _T(""), kjm::no_argument, _T("チェックのとき、一切の表示を行わない。")));
	m_options.push_back(kjm::optionInfo(_T("help"), _T(""), kjm::no_argument, _T("このヘルプを表示。")));
	m_options.push_back(kjm::optionInfo(_T("version"), _T(""), kjm::no_argument, _T("バージョン情報を表示。")));
	m_cmdLine.parse(m_argc, m_argv, m_options);

	if (m_cmdLine.hasOption(_T("version"))) {
		printVersion();
		fContinue = false;
	} else if (m_cmdLine.hasOption(_T("help"))) {
		printUsage();
		fContinue = false;
	}

	return fContinue;
}

//-----------------------------------------------------------------------------
// プログラムのメイン処理
//-----------------------------------------------------------------------------
int md5listProgram::doMain() {
	int ret = 0;

	if (this->m_cmdLine.hasOption(_T("check"))) {
		ret = this->checkFile();
	} else {
		for (int i = 0; i < m_cmdLine.get_argCount(); i++) {
			if (kjm::util::isDirectory(m_cmdLine.get_argument(i))) {
				m_base = kjm::util::removeBackslash(m_cmdLine.get_argument(i));
			} else {
				m_base = kjm::util::removeBackslash(kjm::path::removeFileSpec(m_cmdLine.get_argument(i)));
			}
			md5list(m_cmdLine.get_argument(i));
		}
	}

	return ret;
}

//=============================================================================

//-----------------------------------------------------------------------------
// 唯一無二のアプリケーションインスタンス
//-----------------------------------------------------------------------------
md5listProgram theProgram;

//-----------------------------------------------------------------------------
// プログラムスタート
//-----------------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {

	return theProgram.run(argc, argv);
}
