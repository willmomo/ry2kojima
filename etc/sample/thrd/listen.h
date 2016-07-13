#pragma once


#include <list>
#include "server.h"


#pragma comment( lib, "ws2_32.lib" )


using namespace std;


/*!
 *	listen用スレッド
 */
class listen_thread : public thread {
private:
	SOCKET m_sock;
	list<server_thread*> m_servers;

public:
	listen_thread() : m_sock( INVALID_SOCKET ), m_servers() {};
	virtual ~listen_thread() {};

	void clean_servers();
	virtual void end();
	virtual unsigned worker();
};
