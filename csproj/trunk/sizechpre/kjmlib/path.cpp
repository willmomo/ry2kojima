//---------------------------------------------------------------------
// kjmpath : Version 0.2015.12.8
//
// プリコンパイル済みヘッダを使用しないように設定してください。
//
// r.kojima
//---------------------------------------------------------------------
#include "path.h"


//
// 末尾に \ を追加
//
kjm::_tstring kjm::path::addBackslash(const kjm::_tstring& path) {

	// MAX_PATH を超えると PathAddBackslash が使えなくなるので、自作

	kjm::_tstring result = path;
#if defined(_MBCS)
	if (_ismbstrail((unsigned char *)&result[0], (unsigned char *)&result[result.length() - 1])) {
		result.push_back(_T('\\'));
	} else if (*result.rbegin() != _T('\\')) {
		result.push_back(_T('\\'));
	}
#else
	if (*result.rbegin() != _T('\\')) {
		result.push_back(_T('\\'));
	}
#endif
	return result;
}


//
// ファイルが存在するかどうか調べる
//
bool kjm::path::fileExists(const kjm::_tstring& strPath) {
	kjm::_tstring work = strPath;
	if (work.length() > MAX_PATH) {
		work = kjm::path::makeVeryLongPath(work);
	}
	BOOL bRet = PathFileExists(work.c_str());
	return (bRet ? true : false);
}

//
// \\?\D:\very long path, \\?\UNC\server\share\very long path に変換する
//
kjm::_tstring kjm::path::makeVeryLongPath(const kjm::_tstring& path) {

	// すでに very long path のときは、そのままリターン
	if (path[0] == _T('\\') && path[1] == _T('\\') && path[2] == _T('?')) {
		return path;
	}

	kjm::_tstring result;

	if (path[0] == _T('\\')) {
		// UNCパスの時
		result = _T("\\\\?\\UNC");
		result += &path[1];
	} else {
		// 通常パスの時
		result = _T("\\\\?\\");
		result += path;
	}

	return result;
}

//
// \\?\ または \\?\UNC を取り除く
//
kjm::_tstring kjm::path::removeVeryLongPath(const kjm::_tstring& path) {
	// very long path 以外の時は、そのままリターン
	if (path[0] != _T('\\') || path[1] != _T('\\') || path[2] != _T('?')) {
		return path;
	}

	kjm::_tstring result;

	if (path[4] == _T('U')) {
		// UNCパスの時
		result = _T('\\');
		result += &path[7];
	} else {
		// 通常パスの時
		result = &path[4];
	}

	return result;
}

//
// 指定したパス文字列の拡張子(ピリオドを含む)を返す。
//
kjm::_tstring kjm::path::getExtension(const kjm::_tstring& path) {
	std::vector<TCHAR> work(MAX_PATH + path.length());
	::lstrcpy(&work[0], path.c_str());
	kjm::_tstring result = ::PathFindExtension(&work[0]);
	return result;
}


//
// 2つの文字列を1つのパスに結合します。
//
kjm::_tstring kjm::path::combine(const kjm::_tstring& path1, const kjm::_tstring& path2) {
	return kjm::path::addBackslash(path1) + path2;
}

//
// ファイル名部分を取得
//
kjm::_tstring kjm::path::getFileName(const kjm::_tstring& path) {
	return ::PathFindFileName(path.c_str());
}

//
// 相対パスを作成する
//
kjm::_tstring kjm::path::relativePathTo(const kjm::_tstring& from, DWORD attrFrom, const kjm::_tstring& to, DWORD attrTo) {
	std::vector<TCHAR> path(MAX_PATH);
	if (::PathRelativePathTo(&path[0], from.c_str(), attrFrom, to.c_str(), attrTo) == FALSE) {
		kjm::_tostringstream oss;
		oss << _T("kjm::path::relativePathTo error: PathRelativePathTo(\"") << from << _T("\", ") 
			<< attrFrom << _T(", \"") << to << _T("\", ") << attrTo << _T(") に失敗しました。");
		throw std::runtime_error(kjm::base::toAscii(oss.str()));
	}
	return &path[0];
}

//
// PathRemoveExtension 関数のラッパー
//
kjm::_tstring kjm::path::removeExtension(const kjm::_tstring& strPath) {
	TCHAR szTemp[MAX_PATH];
	PathRemoveExtension(lstrcpy(szTemp, strPath.c_str()));
	return szTemp;
}

//
// フルパスからファイル名を取り除く
//
kjm::_tstring kjm::path::removeFileSpec(const kjm::_tstring& strPath) {
	std::vector<TCHAR> work;
	work.insert(work.begin(), strPath.begin(), strPath.end());
	work.push_back(_T('\0'));
	BOOL b = PathRemoveFileSpec(&work[0]);
	if (b == FALSE) {
		kjm::_tostringstream oss;
		oss << _T("kjm::path::removeFileSpec error: PathRemoveFileSpec(\"") << &work[0] << _T("\") に失敗しました。");
		throw std::runtime_error(kjm::base::toAscii(oss.str()));
	}
	return &work[0];
}

//
// 指定したパス文字列のディレクトリ情報を返します。
//
// [戻り値]
// path がルート ディレクトリを示す場合、空文字列を返す。
//
kjm::_tstring kjm::path::getDirectoryName(const kjm::_tstring& path) {
	TCHAR work[MAX_PATH];
	::PathRemoveFileSpec(::lstrcpy(work, path.c_str()));

	// 変換後の文字列に変化がないときは、ルートディレクトリと判断して、空文字列を返す。
	if (::lstrcmpi(path.c_str(), work) == 0) return kjm::_tstring();

	return work;
}
