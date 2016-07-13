#include <stdio.h>
#include <conio.h>
#include "listen.h"

int main( int argc, char** argv ) {

	WSADATA wsaData;
	WSAStartup( MAKEWORD( 2, 0 ), &wsaData );

	listen_thread t;

	t.run();

	printf( "hit any key..." );
	_getch();

	t.end();
	t.wait( 1000 );

	WSACleanup();

	return 0;
}
