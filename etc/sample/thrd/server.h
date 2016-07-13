#pragma once


#pragma warning( disable : 4786 )

#include <winsock2.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <stdio.h>
#include "thrd.h"


#pragma comment( lib, "ws2_32.lib" )


using namespace std;


/*!
 *	サーバー用スレッド
 */
class server_thread : public thread {
private:
	vector<char> m_ans;			// 応答伝聞格納バッファ
	vector<string> m_lines;		// 解析した行データ
	map<string, string> m_cmds;	// 行データをコマンドに分解
	SOCKET m_sock;
	sockaddr_in m_from;

public:
	server_thread( SOCKET s, sockaddr_in& from ) {
		m_sock = s;
		m_from = from;
	};

	virtual ~server_thread() {};
	
	virtual void end() {
		// ソケットを閉じて、自信のスレッドを終了させる。
		lock();
		shutdown( m_sock, SD_SEND );
		char buf[ 512 ];
		while ( recv( m_sock, buf, sizeof( buf ), 0 ) > 0 )
			;

		closesocket( m_sock );
		m_sock = INVALID_SOCKET;
		unlock();
	};

	//! 電文を受信して、解析処理するスレッド
	virtual unsigned worker();

	// 電文解析
	void parse( vector<char>& buf );

	// 文字列先頭の空白を取り除く
	void ltrim( string& str ) {
		while ( str[ 0 ] == ' ' ) {
			str.erase( str.begin() );
		}
	};

	// 文字列の終端改行を取り除く
	void chomp( string& line ) {
		while ( line.rbegin() != line.rend() && ( *line.rbegin() == '\r' || *line.rbegin() == '\n' ) ) {
			line.erase( line.end() - 1 );
		}
	};

	// コマンドを解析実行
	void action();

	// コマンドを分析
	void parse();

	// getfile コマンド処理
	void cmd_getfile();
};
