#include "kjmlib.h"
#include <queue>
#include "resource.h"

void usage() {
	_tcout << kjm::util::getModuleFileName() << std::endl;
	_tcout << _T("version ") << kjm::util::getFileVersion() << std::endl;
	_tcout << _T("findfile <検索フォルダ> <ファイル名の一部>") << std::endl;
}

int subDirs(const kjm::_tstring& path, std::queue<kjm::_tstring>& dirs) {
	kjm::folderInfo dir(path);
	std::vector<kjm::folderInfo> sub = dir.getSubFolders();
	for (int i = 0; i < sub.size(); i++) {
		dirs.push(sub[i].getFullPath());
		_tcout << _T("フォルダ列挙中... ") << dirs.size() << _T("\r");
		subDirs(sub[i].getFullPath(), dirs);
	}
	return 0;
}

int findfile(const kjm::_tstring& path, const kjm::_tstring& name) {
	std::queue<kjm::_tstring> dirs;
	bool must_crlf = false;

	dirs.push(path);
	subDirs(path, dirs);
	_tcout << _T("\n");

	while (!dirs.empty()) {

		// 画面に表示
		//_tprintf(_T("残りディレクトリ: %d  \r"), dirs.size());
		_tcout << _T("残りディレクトリ: ") << dirs.size() << _T("  \r");
		must_crlf = true;

		kjm::_tstring target = dirs.front();
		dirs.pop();
		kjm::folderInfo dir(target);

		if (dir.getFileName().find(name) != kjm::_tstring::npos) {
			if (must_crlf) {
				_tcout << _T("\n");
				must_crlf = false;
			}
			//_tprintf(_T("\n発見: [ %s ] \n"), dir.getFullPath().c_str());
			_tcout << _T("発見: [ ") << dir.getFullPath() << _T(" ] \n");
		}

		// ファイルを探す
		std::vector<kjm::fileInfo> files = dir.getFiles();
		for (int i = 0; i < files.size(); i++) {
			if (files[i].name().find(name) != kjm::_tstring::npos) {
				if (must_crlf) {
					_tcout << _T("\n");
					must_crlf = false;
				}
				//_tprintf(_T("\n発見: [ %s ] \n"), files[i].fullName().c_str());
				_tcout << _T("発見: [ ") << files[i].fullName() << _T(" ] \n");
			}
		}
	}

	return 0;
}

int _tmain(int argc, TCHAR** argv) {
	SET_UNICODE_MODE;

	if (argc != 3) {
		usage();
		return 1;
	}

	findfile(argv[1], argv[2]);

	return 0;
}
