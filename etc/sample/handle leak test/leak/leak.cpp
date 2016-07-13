#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>


#import <msxml.dll> named_guids


using namespace std;


void leak_main( const string& fname ) {

	MSXML::IXMLDOMDocumentPtr doc;
	HRESULT hr = doc.CreateInstance( MSXML::CLSID_DOMDocument );

	try {
		doc->async = false;
		if ( !doc->load( fname.c_str() ) ) {

			cout << ( const char* )doc->parseError->reason;

		}
	} catch( _com_error& e ) {
		cout << fname << "\n";
	}
}


void leak_loop() {

	ifstream ifs;

	ifs.open( "c:\\list.txt" );
	if ( ifs.is_open() ) {

		string line;

		while ( !ifs.eof() ) {
			getline( ifs, line );

			leak_main( line );
		}

		ifs.close();
	}
}


int main( int argc, char** argv ) {

	CoInitialize( NULL );

	DWORD s = GetTickCount();

	leak_loop();

	DWORD e = GetTickCount();

	cout << ( e - s ) << " ms\n";

	CoUninitialize();

	return 0;
}
