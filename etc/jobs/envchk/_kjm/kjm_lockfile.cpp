//---------------------------------------------------------------------
// kjm::lockfile class
//---------------------------------------------------------------------


#include "kjm_lockfile.h"


//---------------------------------------------------------------------
// ���b�N����
//---------------------------------------------------------------------
bool kjm::lockfile::lock() {

	// ���łɎ����Ń��b�N���Ă���Ƃ��́Atrue��Ԃ�
	if ( m_hFile != INVALID_HANDLE_VALUE )	return true;

	m_hFile = ::CreateFile( m_fullname.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE, NULL );
	if (m_hFile == INVALID_HANDLE_VALUE) {

		// lock�Ɏ��s���Ă��A�S�~�̉\��������̂ō폜�����݂�B
		::DeleteFile( m_fullname.c_str() );
		m_hFile = ::CreateFile( m_fullname.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE, NULL );
	}
	
	return ( m_hFile != INVALID_HANDLE_VALUE );
}


//---------------------------------------------------------------------
// ���b�N����������
//---------------------------------------------------------------------
void kjm::lockfile::unlock() {
	
	if ( m_hFile != INVALID_HANDLE_VALUE ) {
		::CloseHandle( m_hFile );
		m_hFile = INVALID_HANDLE_VALUE;
	}
}

