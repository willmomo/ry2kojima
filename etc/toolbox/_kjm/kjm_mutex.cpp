//---------------------------------------------------------------------
// kjm::mutex class
//---------------------------------------------------------------------


#include "kjm_mutex.h"


//---------------------------------------------------------------------
// mutex �̍쐬
// 
// arg : name = mutex �̖��O
// ret : true = ����I���Afalse = �ȏ�I��
// exp : mutex_error
// note:
//---------------------------------------------------------------------
bool kjm::mutex::create( const std::string& name ) {
	
	// �������ς� instance �̎��́A��O�� throw
	if ( m_hMutex ) {
		throw mutex_error( mutex_error::already_initialized );
	}

	// mutex �̍쐬
	m_hMutex = ::CreateMutex( NULL, FALSE, name.c_str() );
	m_create_status = ::GetLastError();
	return ( m_hMutex != 0 );
}


//---------------------------------------------------------------------
// mutex �����
//
// note: ���������̃C���X�^���X�Ɏg�p���Ă�ok
//       lock���̃I�u�W�F�N�g�ɑ΂��ẮAunlock���Ă���N���[�Y����B
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
// �w�莞�ԃV�O�i����ԂɂȂ�̂�҂�
//
// arg : dwMillisec = �^�C���A�E�g����(ms)
// ret : WaitForSingleObject API �Q��
// exp : mutex_error
// note: mutex���������ς݂̏ꍇ�́AWaitForSingleObject���ĂԂ����B
//---------------------------------------------------------------------
DWORD kjm::mutex::lock( DWORD dwMillisec ) {

	// mutex ���������̏ꍇ�́A��O�� throw
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
// mutex���J������
//
// ret : ReleaseMutex API �Q��
// exp : mutex_error
// note: mutex���������ς݂̏ꍇ�́AReleaseMutex ���ĂԂ����B
//---------------------------------------------------------------------
BOOL kjm::mutex::unlock() {
	
	// mutex ���������̏ꍇ�́A��O�� throw
	if ( !m_hMutex ) {
		throw mutex_error( mutex_error::not_initialized );
	}
	
	BOOL ret = ::ReleaseMutex(m_hMutex);
	if ( ret ) {
		--m_lock_count;
	}

	return ret;
}
