//---------------------------------------------------------------------
// kjm::mutex class
//---------------------------------------------------------------------


#include "kjm_mutex.h"


//---------------------------------------------------------------------
// mutex の作成
// 
// arg : name = mutex の名前
// ret : true = 正常終了、false = 以上終了
// exp : mutex_error
// note:
//---------------------------------------------------------------------
bool kjm::mutex::create( const std::string& name ) {
	
	// 初期化済み instance の時は、例外を throw
	if ( m_hMutex ) {
		throw mutex_error( mutex_error::already_initialized );
	}

	// mutex の作成
	m_hMutex = ::CreateMutex( NULL, FALSE, name.c_str() );
	m_create_status = ::GetLastError();
	return ( m_hMutex != 0 );
}


//---------------------------------------------------------------------
// mutex を閉じる
//
// note: 未初期化のインスタンスに使用してもok
//       lock中のオブジェクトに対しては、unlockしてからクローズする。
//---------------------------------------------------------------------
void kjm::mutex::close() {

	if ( m_hMutex ) {
		
		while ( m_lock_count ) {
			if ( !unlock() ) {
				break;
			}
		}

		::CloseHandle( m_hMutex );
		m_hMutex = NULL;
	}
}


//---------------------------------------------------------------------
// 指定時間シグナル状態になるのを待つ
//
// arg : dwMillisec = タイムアウト時間(ms)
// ret : WaitForSingleObject API 参照
// exp : mutex_error
// note: mutexが初期化済みの場合は、WaitForSingleObjectを呼ぶだけ。
//---------------------------------------------------------------------
DWORD kjm::mutex::lock( DWORD dwMillisec ) {

	// mutex 未初期化の場合は、例外を throw
	if ( !m_hMutex ) {
		throw mutex_error(mutex_error::not_initialized);
	}
	
	DWORD dw = ::WaitForSingleObject( m_hMutex, dwMillisec );
	if ( dw == WAIT_OBJECT_0 || dw == WAIT_ABANDONED ) {
		++m_lock_count;
	}

	return dw;
};

		
//---------------------------------------------------------------------
// mutexを開放する
//
// ret : ReleaseMutex API 参照
// exp : mutex_error
// note: mutexが初期化済みの場合は、ReleaseMutex を呼ぶだけ。
//---------------------------------------------------------------------
BOOL kjm::mutex::unlock() {
	
	// mutex 未初期化の場合は、例外を throw
	if ( !m_hMutex ) {
		throw mutex_error( mutex_error::not_initialized );
	}
	
	BOOL ret = ::ReleaseMutex(m_hMutex);
	if ( ret ) {
		--m_lock_count;
	}

	return ret;
}
