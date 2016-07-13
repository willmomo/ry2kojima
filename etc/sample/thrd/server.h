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
 *	�T�[�o�[�p�X���b�h
 */
class server_thread : public thread {
private:
	vector<char> m_ans;			// �����`���i�[�o�b�t�@
	vector<string> m_lines;		// ��͂����s�f�[�^
	map<string, string> m_cmds;	// �s�f�[�^���R�}���h�ɕ���
	SOCKET m_sock;
	sockaddr_in m_from;

public:
	server_thread( SOCKET s, sockaddr_in& from ) {
		m_sock = s;
		m_from = from;
	};

	virtual ~server_thread() {};
	
	virtual void end() {
		// �\�P�b�g����āA���M�̃X���b�h���I��������B
		lock();
		shutdown( m_sock, SD_SEND );
		char buf[ 512 ];
		while ( recv( m_sock, buf, sizeof( buf ), 0 ) > 0 )
			;

		closesocket( m_sock );
		m_sock = INVALID_SOCKET;
		unlock();
	};

	//! �d������M���āA��͏�������X���b�h
	virtual unsigned worker();

	// �d�����
	void parse( vector<char>& buf );

	// ������擪�̋󔒂���菜��
	void ltrim( string& str ) {
		while ( str[ 0 ] == ' ' ) {
			str.erase( str.begin() );
		}
	};

	// ������̏I�[���s����菜��
	void chomp( string& line ) {
		while ( line.rbegin() != line.rend() && ( *line.rbegin() == '\r' || *line.rbegin() == '\n' ) ) {
			line.erase( line.end() - 1 );
		}
	};

	// �R�}���h����͎��s
	void action();

	// �R�}���h�𕪐�
	void parse();

	// getfile �R�}���h����
	void cmd_getfile();
};
