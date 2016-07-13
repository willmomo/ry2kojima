//---------------------------------------------------------------------
// kjm::lockfile class
//---------------------------------------------------------------------


#include "kjm_lockfile.h"


//---------------------------------------------------------------------
// ロックする
//---------------------------------------------------------------------
bool kjm::lockfile::lock() {

	// すでに自分でロックしているときは、trueを返す
	if ( m_hFile != INVALID_HANDLE_VALUE )	return true;

	m_hFile = ::CreateFile( m_fullname.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE, NULL );
	if (m_hFile == INVALID_HANDLE_VALUE) {

		// lockに失敗しても、ゴミの可能性があるので削除を試みる。
		::DeleteFile( m_fullname.c_str() );
		m_hFile = ::CreateFile( m_fullname.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE, NULL );
	}
	
	return ( m_hFile != INVALID_HANDLE_VALUE );
}


//---------------------------------------------------------------------
// ロックを解除する
//---------------------------------------------------------------------
void kjm::lockfile::unlock() {
	
	if ( m_hFile != INVALID_HANDLE_VALUE ) {
		::CloseHandle( m_hFile );
		m_hFile = INVALID_HANDLE_VALUE;
	}
}

