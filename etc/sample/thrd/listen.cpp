#include "listen.h"


/*
 */
void listen_thread::clean_servers() {

	// new �����Aserver_thread �ŁA���łɏI����Ă�����̂̓��X�g�����菜��
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
 *	�X���b�h���I��������
 */
void listen_thread::end() {

	// �\�P�b�g����āA���M�̃X���b�h���I��������B
	lock();
	shutdown( m_sock, SD_SEND );
	char buf[ 512 ];
	while ( recv( m_sock, buf, sizeof( buf ), 0 ) > 0 )
		;

	closesocket( m_sock );
	m_sock = INVALID_SOCKET;
	unlock();

	// new �����Aserver_thread ��S�ďI��点��
	while ( !m_servers.empty() ) {
		server_thread* p = m_servers.back();
		m_servers.pop_back();

		// �X���b�h�������Ă���Ƃ��́A�I��������
		if ( p->still_active() ) {
			p->end();
			p->wait();
		}

		// �I�u�W�F�N�g��j���B
		delete p;
	}
}


/*!
 *	listen�X���b�h
 */
unsigned listen_thread::worker() {
	
	// �\�P�b�g���쐬
	m_sock = socket( AF_INET, SOCK_STREAM, 0 );

	// �|�[�g�Ɋ��蓖�Ă�
	sockaddr_in myaddr;
	memset( &myaddr, 0, sizeof( myaddr ) );
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons( 12346 );
	myaddr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind( m_sock, ( sockaddr* )&myaddr, sizeof( myaddr ) );

	// listen �J�n
	listen( m_sock, 5 );

	// �ڑ��҂����[�v
	// end method �ŁAlisten socket ���N���[�Y���邱�ƂŁA
	// �G���[�𔭐������ă��[�v�𔲂���B
	while ( true ) {

		sockaddr_in from;
		int from_len = sizeof( from );
		SOCKET s = accept( m_sock, ( sockaddr* )&from, &from_len );
		if ( s == INVALID_SOCKET )	break;

		// �N���C�A���g����̐ڑ�����
		// server_thread ���쐬���� run�B
		// �쐬���� server_thread �́A���X�g�ɓo�^���āAend method
		// ���s���ɏI�������ƁAdelete ���s���B
		
		server_thread* p = new server_thread( s, from );
		m_servers.push_back( p );

		p->run();

		clean_servers();
	}

	// end method �ȊO�̉��炩�̗��R�ŏI������Ƃ��̂��߂ɁA
	// closesocket ���s���B
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
