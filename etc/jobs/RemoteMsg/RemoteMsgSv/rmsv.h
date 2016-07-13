
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
	HANDLE m_thread;	// �X���b�h�ւ̃n���h��
	bool m_end;			// �X���b�h�I���t���O
	HWND m_hwnd;		// �O�������M�������b�Z�[�W��SendMessage����E�B���h�E�ւ̃n���h��

public:
	// �R���X�g���N�^�E�f�X�g���N�^
	remote_message_server() : m_thread( NULL ), m_end( false ), m_hwnd( NULL ) {};
	virtual ~remote_message_server() {};

public:
	void set_hwnd( HWND hwnd ) { m_hwnd = hwnd; };
	HWND get_hwnd() { return m_hwnd; };

public:
	// ��M�X���b�h�N��
	void begin();

	// ��M�X���b�h�I��
	void end( int timeout );

	// ��M�X���b�h
	static unsigned __stdcall recv_thread( void* args );
};

#endif
