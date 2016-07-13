#include "server.h"


void server_thread::cmd_getfile() {

	string work;

	m_ans.clear();

	FILE* fp = fopen( m_cmds[ "arg1" ].c_str(), "rb" );
	if ( fp ) {

		work = "ans: ok\r\n";
		m_ans.insert( m_ans.end(), work.begin(), work.end() );

		work = "cmd: " + m_cmds[ "cmd" ] + "\r\n";
		m_ans.insert( m_ans.end(), work.begin(), work.end() );

		work = "arg1: " + m_cmds[ "arg1" ] + "\r\n";
		m_ans.insert( m_ans.end(), work.begin(), work.end() );

		vector<char> content;
		char buf[ 8192 ];
		int ret;
		while ( true ) {
			ret = fread( buf, sizeof( char ), 8192, fp );

			content.insert( content.end(), &buf[ 0 ], &buf[ ret ] );

			if ( ret < 8192 ) {
				break;
			}
		}

		char work2[ 1024 ];
		sprintf( work2, "content-length: %d\r\n", content.size() );
		work = work2;
		m_ans.insert( m_ans.end(), work.begin(), work.end() );

		work = "\r\n";
		m_ans.insert( m_ans.end(), work.begin(), work.end() );

		m_ans.insert( m_ans.end(), content.begin(), content.end() );

		fclose( fp );
	}
}


void server_thread::parse() {

	for ( int i = 0; i < m_lines.size(); i++ ) {

		int pos = m_lines[ i ].find( ':' );
		if ( pos != string::npos ) {

			string cmd( &m_lines[ i ][ 0 ], &m_lines[ i ][ pos ] );
			string val( &m_lines[ i ][ pos + 1 ], m_lines[ i ].end() );
			ltrim( val );

			m_cmds[ cmd ] = val;
		}
	}
}


void server_thread::action() {

	// コマンドに分解
	parse();

	// コマンドに応じたアクション
	if ( m_cmds[ "cmd" ] == "getfile" ) {

		cmd_getfile();

	} else {
	}

	if ( !m_ans.empty() ) {

#if 1
		send( m_sock, m_ans.begin(), m_ans.size(), 0 );
#else
		int sum = 0;
		for ( int i = 0; i < m_ans.size(); i += 8192 ) {
			int rc = send( m_sock, &m_ans[ i ], min( 8192, m_ans.size() - i ), 0 );
			printf( " rc = %d, sum = %d\n", rc, sum += rc );
			Sleep( 100 );
		}
#endif
	}

	m_lines.clear();
	m_ans.clear();
}


unsigned server_thread::worker() {

	bool loop = true;
	vector<char> total_buf;
	char buf[ 8192 ];

	while ( loop ) {

		// とりあえず受信
		int ret = recv( m_sock, buf, sizeof( buf ), 0 );
		if ( ret == 0 || ret == SOCKET_ERROR )	break;

		// 受信データを解析前バッファに追加
		total_buf.insert( total_buf.end(), &buf[ 0 ], &buf[ ret ] );

		// 電文解析
		vector<char>::iterator pos;
		while ( ( pos = find( total_buf.begin(), total_buf.end(), '\n' ) ) != total_buf.end() ) {

			string line( total_buf.begin(), pos + 1 );
			total_buf.erase( total_buf.begin(), pos + 1 );

			//send( m_sock, line.c_str(), line.size(), 0 );
			chomp( line );
			if ( line == "" )	loop = false;

			m_lines.push_back( line );
		}

		if ( loop == false ) {
			// コマンドの解析と処理
			action();
		}
	}

	lock();
	if ( m_sock != INVALID_SOCKET ) {

		shutdown( m_sock, SD_SEND );
		while ( recv( m_sock, buf, sizeof( buf ), 0 ) > 0 )
			;
		
		closesocket( m_sock );
		m_sock = INVALID_SOCKET;
	}
	unlock();

	return 0;
}
