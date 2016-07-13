//---------------------------------------------------------------------
// kjm::path_string class
//---------------------------------------------------------------------


#include "kjm_util.h"
#include "kjm_path_string.h"


#pragma comment( lib, "shlwapi.lib" )


//---------------------------------------------------------------------
// パス用の処理を追加した string クラス
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// PathRemoveFileSpecのラッパー
//---------------------------------------------------------------------
kjm::path_string& kjm::path_string::RemoveFileSpec() {

	std::vector<char> work( begin(), end() );
	work.push_back( '\0' );

	if ( ::PathRemoveFileSpec( work.begin() ) ) {
		*this = work.begin();
	}

	return *this;
}


//---------------------------------------------------------------------
// PathAppendのラッパー
//---------------------------------------------------------------------
kjm::path_string& kjm::path_string::Append( LPCTSTR pMore) {

	std::vector<char> work( begin(), end() );
	work.push_back( '\0' );
	work.resize( work.size() + strlen( pMore ) + MAX_PATH );	// 十分なバッファを確保

	if ( ::PathAppend( work.begin(), pMore ) ) {
		*this = work.begin();
	}

	return *this;
}


//---------------------------------------------------------------------
// PathRenameExtensionのラッパー
//---------------------------------------------------------------------
kjm::path_string& kjm::path_string::RenameExtension( LPCTSTR pszExt ) {

	std::vector<char> work( begin(), end() );
	work.push_back( '\0' );
	work.resize( work.size() + strlen( pszExt ) + MAX_PATH );	// 十分なバッファを確保

	if ( ::PathRenameExtension( work.begin(), pszExt ) ) {
		*this = work.begin();
	}

	return *this;
}


//---------------------------------------------------------------------
// path_stringを作成するファクトリメソッド
//---------------------------------------------------------------------
kjm::path_string kjm::path_string::GetModuleFileName( HMODULE hModule /*=NULL*/ ) {

	std::vector<char> filename;
	kjm::util::GetModuleFileName( NULL, filename );

	return kjm::path_string( filename.begin() );
}


