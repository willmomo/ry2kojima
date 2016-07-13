#pragma once


#include <windows.h>
#include <process.h>


/*!
 *	�X���b�h�Ǘ��N���X
 */
class thread {
private:
	HANDLE m_hThread;
	CRITICAL_SECTION m_cs;

public:
	thread() : m_hThread( NULL ) {
		InitializeCriticalSection( &m_cs );
	};
	virtual ~thread() {
		DeleteCriticalSection( &m_cs );
	};

	void lock() { EnterCriticalSection( &m_cs ); };
	void unlock() { LeaveCriticalSection( &m_cs ); };

	//! �X���b�h�����쒆���ǂ������ׂ�
	bool still_active() { 
		DWORD code;
		GetExitCodeThread( m_hThread, &code );
		return ( code == STILL_ACTIVE );
	};

	//! �X���b�h���N������B
	void run() {
		m_hThread = ( HANDLE )_beginthreadex( NULL, 0, thread_func, this, 0, NULL );
	};

	/*!
	 * �X���b�h���I��������B�h���N���X�Œ�`����B
	 * �I�����@�́Aworker method �̎����Ɉˑ�����B
	 */
	virtual void end() = 0;

	//! �X���b�h�̈ꎞ��~
	DWORD suspend() {
		return SuspendThread( m_hThread );
	};

	//! �X���b�h�̍ĊJ
	DWORD resume() {
		return ResumeThread( m_hThread );
	};

	/*!
	 * �X���b�h�ɌĂяo�����֐��B�h���N���X�Œ�`����B
	 * end method�̌Ăяo���ŏI���ł���悤�Ɏ�������B
	 */
	virtual unsigned worker() = 0;

	/*!
	 * �X���b�h�̏I����҂B
	 */
	DWORD wait( DWORD timeout = INFINITE ) {
		DWORD ret = WaitForSingleObject( m_hThread, timeout );
		CloseHandle( m_hThread );
		return ret;
	};

	/*!
	 * ���ʂ̃X���b�h�֐��B
	 * args�ɂ́Athread�N���X����h�������N���X�I�u�W�F�N�g�ւ�
	 * �|�C���^���󂯎��Bp->worker() �����s���邾���B
	 */
	static unsigned __stdcall thread_func( void* args ) {
		return ( ( thread* )args )->worker();
	};
};
