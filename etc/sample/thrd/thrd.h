#pragma once


#include <windows.h>
#include <process.h>


/*!
 *	スレッド管理クラス
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

	//! スレッドが動作中かどうか調べる
	bool still_active() { 
		DWORD code;
		GetExitCodeThread( m_hThread, &code );
		return ( code == STILL_ACTIVE );
	};

	//! スレッドを起動する。
	void run() {
		m_hThread = ( HANDLE )_beginthreadex( NULL, 0, thread_func, this, 0, NULL );
	};

	/*!
	 * スレッドを終了させる。派生クラスで定義する。
	 * 終了方法は、worker method の実装に依存する。
	 */
	virtual void end() = 0;

	//! スレッドの一時停止
	DWORD suspend() {
		return SuspendThread( m_hThread );
	};

	//! スレッドの再開
	DWORD resume() {
		return ResumeThread( m_hThread );
	};

	/*!
	 * スレッドに呼び出される関数。派生クラスで定義する。
	 * end methodの呼び出しで終了できるように実装する。
	 */
	virtual unsigned worker() = 0;

	/*!
	 * スレッドの終了を待つ。
	 */
	DWORD wait( DWORD timeout = INFINITE ) {
		DWORD ret = WaitForSingleObject( m_hThread, timeout );
		CloseHandle( m_hThread );
		return ret;
	};

	/*!
	 * 共通のスレッド関数。
	 * argsには、threadクラスから派生したクラスオブジェクトへの
	 * ポインタを受け取る。p->worker() を実行するだけ。
	 */
	static unsigned __stdcall thread_func( void* args ) {
		return ( ( thread* )args )->worker();
	};
};
