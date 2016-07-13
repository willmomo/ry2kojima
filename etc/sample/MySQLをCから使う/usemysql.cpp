#include <iostream>
#include "my_global.h"
#include "mysql.h"


using namespace std;


int main() {

	MYSQL* p = mysql_init( NULL );

	MYSQL* p2 = mysql_real_connect( p, "hn2server", "hn2user", "hn2pass", "hn2db", 0, NULL, 0 );
	
	int ret = mysql_query( p, "insert into t_carderrmsg values (41, 'noerror')" );

	cout << "mysql_errno = " << mysql_errno( p ) << endl;

	/*
	MYSQL_RES* res = mysql_store_result( p );

	MYSQL_ROW row;
	while ( row = mysql_fetch_row( res ) ) {
	}

	mysql_free_result( res );
*/

	mysql_close( p );

	return 0;
}
