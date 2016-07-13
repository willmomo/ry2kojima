
/*!
 */

#pragma once

#if !defined( __RMSV_H )
#define __RMSV_H

/*!
 */
#include <windows.h>
#include <process.h>

/*!
 */
class remote_message_server {
private:
	HANDLE m_thread;	// スレッドへのハンドル
	bool m_end;			// スレッド終了フラグ
	HWND m_hwnd;		// 外部から受信したメッセージをSendMessageするウィンドウへのハンドル

public:
	// コンストラクタ・デストラクタ
	remote_message_server() : m_thread( NULL ), m_end( false ), m_hwnd( NULL ) {};
	virtual ~remote_message_server() {};

public:
	void set_hwnd( HWND hwnd ) { m_hwnd = hwnd; };
	HWND get_hwnd() { return m_hwnd; };

public:
	// 受信スレッド起動
	void begin();

	// 受信スレッド終了
	void end( int timeout );

	// 受信スレッド
	static unsigned __stdcall recv_thread( void* args );
};

#endif
