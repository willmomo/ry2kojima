//---------------------------------------------------------------------
// kjm::library class
//---------------------------------------------------------------------


#include "kjm_library.h"



//---------------------------------------------------------------------
// LoadLibraryEx/FreeLibrary��΂ɂ���N���X
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// ���C�u�����̃��[�h
//---------------------------------------------------------------------
BOOL kjm::library::LoadLibraryEx( LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags ) {
	
	// ���łɃ��[�h���Ă��邩������Ȃ��̂ŁA�J��
	FreeLibrary();

	m_hLibModule = ::LoadLibraryEx( lpLibFileName, hFile, dwFlags );
	
	return ( m_hLibModule ) ? TRUE : FALSE;
}


//---------------------------------------------------------------------
// ���C�u�����̊J��
//---------------------------------------------------------------------
BOOL kjm::library::FreeLibrary() {
	BOOL ret = TRUE;

	if ( m_hLibModule ) {
		ret = ::FreeLibrary( m_hLibModule );
		m_hLibModule = NULL;
	}

	return ret;
}
