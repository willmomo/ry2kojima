
/*!
 */

#include "stdafx.h"

/*!
 *	受信スレッド起動
 */
void remote_message_server::begin() {

	assert( m_thread == NULL );

	m_thread = ( HANDLE )_beginthreadex( NULL, 0, remote_message_server::recv_thread, this, 0, NULL );
}


/*!
 *	受信スレッド終了
 */
void remote_message_server::end( int timeout ) {

	if ( m_thread != NULL ) {
		m_end = true;

		WaitForSingleObject( m_thread, timeout );

		CloseHandle( m_thread );
		m_thread = NULL;
		m_end = false;
	}
}


/*!
 *	受信スレッド
 */
unsigned __stdcall remote_message_server::recv_thread( void* args ) {

	remote_message_server* rms = ( remote_message_server* )args;

	// ソケットの作成
	SOCKET s = socket( AF_INET, SOCK_DGRAM, 0 );

	// 受信のためソケットをbind
	sockaddr_in mysock;
	memset( &mysock, 0, sizeof( mysock ) );
	mysock.sin_family = AF_INET;
	mysock.sin_addr.S_un.S_addr = INADDR_ANY;
	mysock.sin_port = htons( 12345 );
	bind( s, ( sockaddr* )&mysock, sizeof( mysock ) );

	// 受信
	while ( !rms->m_end ) {

		timeval timeout;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		FD_SET read_set;
		FD_ZERO( &read_set );
		FD_SET( s, &read_set );

		int ret = select( FD_SETSIZE, &read_set, NULL, NULL, &timeout );

		if ( FD_ISSET( s, &read_set ) ) {

			// 受信
			char buf[ 1024 ];
			sockaddr_in from;
			int fromlen = sizeof( from );

			recvfrom( s, buf, sizeof( buf ), 0, ( sockaddr* )&from, &fromlen );

			DWORD msg, wp, lp, ret;
			sscanf( buf, "%d,%d,%d", &msg, &wp, &lp );
			ret = SendMessage( rms->get_hwnd(), msg, wp, lp );

			{
				char buf[ 512 ];
				wsprintf( buf, "%d %d %d\n", msg, wp, lp );
				OutputDebugString( buf );
			}

			sprintf( buf, "%d,%d,%d,%d", ret, msg, wp, lp );
			sendto( s, buf, strlen( buf ), 0, ( sockaddr* )&from, fromlen );

		} else {
			OutputDebugString( "timed out...\n" );
		}
	}

	// ソケットの破棄
	closesocket( s );

	return 0;
}
