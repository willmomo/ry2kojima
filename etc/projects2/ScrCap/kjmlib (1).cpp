//---------------------------------------------------------------------
// 自分専用ライブラリ
//---------------------------------------------------------------------
#include "kjmlib.h"

//---------------------------------------------------------------------
// ユーティリティクラス
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// GetModuleFileName 相当
//---------------------------------------------------------------------
kjm::_tstring kjm::util::get_module_file_name(HMODULE hModule) {
	kjm::_tarray fname(MAX_PATH);

	GetModuleFileName(hModule, fname.begin(), fname.size());

	return fname.begin();
}

//---------------------------------------------------------------------
// Path関係のユーティリティクラス
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// PathAppend 相当
//---------------------------------------------------------------------
kjm::_tstring kjm::path::append(const kjm::_tstring& src, const kjm::_tstring& more) {
	kjm::_tarray fname(MAX_PATH);

	PathAppend(lstrcpyn(fname.begin(), src.c_str(), fname.size()), more.c_str());

	return fname.begin();
}

//---------------------------------------------------------------------
// PathRemoveFileSpec 相当
//---------------------------------------------------------------------
kjm::_tstring kjm::path::remove_file_spec(const kjm::_tstring& src) {
	kjm::_tarray fname(MAX_PATH);

	PathRemoveFileSpec(lstrcpyn(fname.begin(), src.c_str(), fname.size()));

	return fname.begin();
}

//---------------------------------------------------------------------
// ファイル名部分を変更する。remove_file_spec + append 処理。
//---------------------------------------------------------------------
kjm::_tstring kjm::path::rename_file_spec(const kjm::_tstring& src, const kjm::_tstring& newName) {
	return kjm::path::append(
		kjm::path::remove_file_spec(src), newName);
}
