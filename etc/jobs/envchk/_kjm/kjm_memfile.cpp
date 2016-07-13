//---------------------------------------------------------------------
// kjm::memfile class
//---------------------------------------------------------------------


#include "kjm_memfile.h"


//---------------------------------------------------------------------
// �������}�b�v�h�t�@�C���̍쐬
//
// arg : name = �������}�b�v�h�t�@�C���̖��O
//       size = �T�C�Y(�v�f��)
// ret : false = ���s�Btrue = ����
// exp : memfile_error
// note:
//---------------------------------------------------------------------
template<class T>
bool kjm::memfile::create( const std::string& name, DWORD size ) {

	// �������ς�instance�̂Ƃ��́A��O��throw
	if ( m_hFile ) {
		throw memfile_error( memfile_error::already_initialized );
	}

	// �������}�b�v�h�t�@�C���̍쐬
	m_hFile = ::CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof( T ) * size, name.c_str() );
	m_create_status = ::GetLastError();
	return ( m_hFile != NULL );
}


//---------------------------------------------------------------------
// �������}�b�v�h�t�@�C�����J��
//
// arg : name = �������}�b�v�h�t�@�C���̖��O
// ret : false = ���s�Btrue = ����
// exp : memfile_error
// note:
//---------------------------------------------------------------------
template<class T>
bool kjm::memfile::open( const std::string& name ) {

	// �������ς�instance�̎��́A��O��throw
	if ( m_hFile ) {
		throw memfile_error( memfile_error::already_initialized );
	}

	// �������}�b�v�h�t�@�C�����J��
	m_hFile = ::OpenFileMapping( FILE_MAP_ALL_ACCESS, TRUE, name.c_str() );
	return ( m_hFile != NULL );
}


//---------------------------------------------------------------------
// �������}�b�v�h�t�@�C�������
//
// arg : �Ȃ�
// ret : �Ȃ�
// exp : �Ȃ�
// note: ����������instance�Ŏg�p���Ă�ok
//---------------------------------------------------------------------
template<class T>
void kjm::memfile::close() {
	
	if ( m_hFile ) {
		::CloseHandle( m_hFile );
		m_hFile = NULL;
	}
}


//---------------------------------------------------------------------
// �������}�b�v�h�t�@�C������f�[�^��ǂݍ���
//
// arg : buffer = �ǂݍ��ރo�b�t�@�ւ̃|�C���^
//       offset = �擪�ʒu�ւ̃I�t�Z�b�g
//       size = �ǂݍ��ޗv�f��
//       pmtx = �r������Ɏg�p����mutex�ւ̃|�C���^(NULL��)
//       timeout = �r���^�C���A�E�g����
//---------------------------------------------------------------------
template<class T>
int kjm::memfile::read( T* buffer, size_t offset, size_t size, mutex* pmtx, DWORD timeout ) {

	// ��������instance�̎��́A��O��throw
	if ( !m_hFile ) {
		throw memfile_error( memfile_error::not_initialized );
	}

	int ret = 0;
	DWORD dw = WAIT_OBJECT_0;

	// mutex�ւ̃|�C���^���^�����Ă���Ƃ��́A�܂�lock
	if ( pmtx ) {
		dw = pmtx->lock( timeout );
	}

	if ( dw == WAIT_OBJECT_0 || dw == WAIT_ABANDONED ) {
		
		T* p = ( T* )::MapViewOfFile( m_hFile, FILE_MAP_ALL_ACCESS, 0, 0, 0 );
		if ( p ) {
			::memmove( buffer, &p[ offset ], sizeof( T ) * size );
			::UnmapViewOfFile( p );
		} else {
			ret = -1;
		}

		// mutex�ւ̃|�C���^���^�����Ă���Ƃ��́Aunlock
		if ( pmtx ) {
			pmtx->unlock();
		}
	} else {
		ret = -2;
	}

	return ret;
}


//---------------------------------------------------------------------
// �������}�b�v�h�t�@�C���փf�[�^����������
//
// arg : buffer = �ǂݍ��ރo�b�t�@�ւ̃|�C���^
//       offset = �擪�ʒu�ւ̃I�t�Z�b�g
//       size = �ǂݍ��ޗv�f��
//       pmtx = �r������Ɏg�p����mutex�ւ̃|�C���^(NULL��)
//       timeout = �r���^�C���A�E�g����
//---------------------------------------------------------------------
template <class T> 
int kjm::memfile::write( T* buffer, size_t offset, size_t size, mutex* pmtx, DWORD timeout ) {
	
	// ��������instance�̎��́A��O��throw
	if ( !m_hFile ) {
		throw memfile_error( memfile_error::not_initialized );
	}

	int ret = 0;
	DWORD dw = WAIT_OBJECT_0;

	// mutex�ւ̃|�C���^���^�����Ă���Ƃ��́A�܂�lock
	if ( pmtx ) {
		dw = pmtx->lock( timeout );
	}

	if ( dw == WAIT_OBJECT_0 || dw == WAIT_ABANDONED ) {
		
		T* p = ( T* )::MapViewOfFile( m_hFile, FILE_MAP_ALL_ACCESS, 0, 0, 0 );
		if ( p ) {
			::memmove( &p[offset], buffer, sizeof( T ) * size );
			::UnmapViewOfFile( p );
		} else {
			ret = -1;
		}

		// mutex�ւ̃|�C���^���^�����Ă���Ƃ��́Aunlock
		if ( pmtx ) {
			pmtx->unlock();
		}
	} else {
		ret = -2;
	}

	return ret;
}
