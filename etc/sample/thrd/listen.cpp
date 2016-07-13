#include "listen.h"


/*
 */
void listen_thread::clean_servers() {

	// new した、server_thread で、すでに終わっているものはリストから取り除く
	list<server_thread*>::iterator i = m_servers.begin();

	while ( i != m_servers.end() ) {
		if ( !( *i )->still_active() ) {
			delete *i;
			i = m_servers.erase( i );
		} else {
			i = ++i;
		}
	}
}


/*!
 *	スレッドを終了させる
 */
void listen_thread::end() {

	// ソケットを閉じて、自信のスレッドを終了させる。
	lock();
	shutdown( m_sock, SD_SEND );
	char buf[ 512 ];
	while ( recv( m_sock, buf, sizeof( buf ), 0 ) > 0 )
		;

	closesocket( m_sock );
	m_sock = INVALID_SOCKET;
	unlock();

	// new した、server_thread を全て終わらせる
	while ( !m_servers.empty() ) {
		server_thread* p = m_servers.back();
		m_servers.pop_back();

		// スレッドが生きているときは、終了させる
		if ( p->still_active() ) {
			p->end();
			p->wait();
		}

		// オブジェクトを破棄。
		delete p;
	}
}


/*!
 *	listenスレッド
 */
unsigned listen_thread::worker() {
	
	// ソケットを作成
	m_sock = socket( AF_INET, SOCK_STREAM, 0 );

	// ポートに割り当てる
	sockaddr_in myaddr;
	memset( &myaddr, 0, sizeof( myaddr ) );
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons( 12346 );
	myaddr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind( m_sock, ( sockaddr* )&myaddr, sizeof( myaddr ) );

	// listen 開始
	listen( m_sock, 5 );

	// 接続待ちループ
	// end method で、listen socket をクローズすることで、
	// エラーを発生させてループを抜ける。
	while ( true ) {

		sockaddr_in from;
		int from_len = sizeof( from );
		SOCKET s = accept( m_sock, ( sockaddr* )&from, &from_len );
		if ( s == INVALID_SOCKET )	break;

		// クライアントからの接続あり
		// server_thread を作成して run。
		// 作成した server_thread は、リストに登録して、end method
		// 実行時に終了処理と、delete を行う。
		
		server_thread* p = new server_thread( s, from );
		m_servers.push_back( p );

		p->run();

		clean_servers();
	}

	// end method 以外の何らかの理由で終わったときのために、
	// closesocket を行う。
	lock();
	if ( m_sock != INVALID_SOCKET ) {
		shutdown( m_sock, SD_SEND );
		char buf[ 512 ];
		while ( recv( m_sock, buf, sizeof( buf ), 0 ) > 0 )
			;

		closesocket( m_sock );
		m_sock = INVALID_SOCKET;
	}
	unlock();

	return 0;
}
