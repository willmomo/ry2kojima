//---------------------------------------------------------------------
// kjm::library class
//---------------------------------------------------------------------


#include "kjm_library.h"



//---------------------------------------------------------------------
// LoadLibraryEx/FreeLibraryを対にするクラス
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// ライブラリのロード
//---------------------------------------------------------------------
BOOL kjm::library::LoadLibraryEx( LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags ) {
	
	// すでにロードしているかもしれないので、開放
	FreeLibrary();

	m_hLibModule = ::LoadLibraryEx( lpLibFileName, hFile, dwFlags );
	
	return ( m_hLibModule ) ? TRUE : FALSE;
}


//---------------------------------------------------------------------
// ライブラリの開放
//---------------------------------------------------------------------
BOOL kjm::library::FreeLibrary() {
	BOOL ret = TRUE;

	if ( m_hLibModule ) {
		ret = ::FreeLibrary( m_hLibModule );
		m_hLibModule = NULL;
	}

	return ret;
}
