/*
	ver:20151019-1657
*/

#include "kjm_path.h"

// パスを結合する(PathAppend)

std::basic_string<TCHAR> kjm::path::append(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& more) {
	std::vector<TCHAR> work(MAX_PATH);
	PathAppend(lstrcpy(&work[0], path.c_str()), more.c_str());
	return &work[0];
}

// ファイル名を返す(PathFindFileName)

std::basic_string<TCHAR> kjm::path::findFileName(const std::basic_string<TCHAR>& path) {
	return PathFindFileName(path.c_str());
}

// 拡張子を削除して返す
std::basic_string<TCHAR> kjm::path::removeExtension(const std::basic_string<TCHAR>& path) {
	std::vector<TCHAR> work(MAX_PATH);
	PathRemoveExtension(lstrcpy(&work[0], path.c_str()));
	return &work[0];
}

