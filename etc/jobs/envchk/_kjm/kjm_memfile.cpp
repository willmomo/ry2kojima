//---------------------------------------------------------------------
// kjm::memfile class
//---------------------------------------------------------------------


#include "kjm_memfile.h"


//---------------------------------------------------------------------
// メモリマップドファイルの作成
//
// arg : name = メモリマップドファイルの名前
//       size = サイズ(要素数)
// ret : false = 失敗。true = 成功
// exp : memfile_error
// note:
//---------------------------------------------------------------------
template<class T>
bool kjm::memfile::create( const std::string& name, DWORD size ) {

	// 初期化済みinstanceのときは、例外をthrow
	if ( m_hFile ) {
		throw memfile_error( memfile_error::already_initialized );
	}

	// メモリマップドファイルの作成
	m_hFile = ::CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof( T ) * size, name.c_str() );
	m_create_status = ::GetLastError();
	return ( m_hFile != NULL );
}


//---------------------------------------------------------------------
// メモリマップドファイルを開く
//
// arg : name = メモリマップドファイルの名前
// ret : false = 失敗。true = 成功
// exp : memfile_error
// note:
//---------------------------------------------------------------------
template<class T>
bool kjm::memfile::open( const std::string& name ) {

	// 初期化済みinstanceの時は、例外をthrow
	if ( m_hFile ) {
		throw memfile_error( memfile_error::already_initialized );
	}

	// メモリマップドファイルを開く
	m_hFile = ::OpenFileMapping( FILE_MAP_ALL_ACCESS, TRUE, name.c_str() );
	return ( m_hFile != NULL );
}


//---------------------------------------------------------------------
// メモリマップドファイルを閉じる
//
// arg : なし
// ret : なし
// exp : なし
// note: 未初期化のinstanceで使用してもok
//---------------------------------------------------------------------
template<class T>
void kjm::memfile::close() {
	
	if ( m_hFile ) {
		::CloseHandle( m_hFile );
		m_hFile = NULL;
	}
}


//---------------------------------------------------------------------
// メモリマップドファイルからデータを読み込む
//
// arg : buffer = 読み込むバッファへのポインタ
//       offset = 先頭位置へのオフセット
//       size = 読み込む要素数
//       pmtx = 排他制御に使用するmutexへのポインタ(NULL可)
//       timeout = 排他タイムアウト時間
//---------------------------------------------------------------------
template<class T>
int kjm::memfile::read( T* buffer, size_t offset, size_t size, mutex* pmtx, DWORD timeout ) {

	// 未初期化instanceの時は、例外をthrow
	if ( !m_hFile ) {
		throw memfile_error( memfile_error::not_initialized );
	}

	int ret = 0;
	DWORD dw = WAIT_OBJECT_0;

	// mutexへのポインタが与えられているときは、まずlock
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

		// mutexへのポインタが与えられているときは、unlock
		if ( pmtx ) {
			pmtx->unlock();
		}
	} else {
		ret = -2;
	}

	return ret;
}


//---------------------------------------------------------------------
// メモリマップドファイルへデータを書き込む
//
// arg : buffer = 読み込むバッファへのポインタ
//       offset = 先頭位置へのオフセット
//       size = 読み込む要素数
//       pmtx = 排他制御に使用するmutexへのポインタ(NULL可)
//       timeout = 排他タイムアウト時間
//---------------------------------------------------------------------
template <class T> 
int kjm::memfile::write( T* buffer, size_t offset, size_t size, mutex* pmtx, DWORD timeout ) {
	
	// 未初期化instanceの時は、例外をthrow
	if ( !m_hFile ) {
		throw memfile_error( memfile_error::not_initialized );
	}

	int ret = 0;
	DWORD dw = WAIT_OBJECT_0;

	// mutexへのポインタが与えられているときは、まずlock
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

		// mutexへのポインタが与えられているときは、unlock
		if ( pmtx ) {
			pmtx->unlock();
		}
	} else {
		ret = -2;
	}

	return ret;
}
