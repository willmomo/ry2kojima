
#include <windows.h>

#import "msxml.dll" named_guids

void foo() {

	MSXML::IXMLDOMDocumentPtr doc;
	HRESULT hr = doc.CreateInstance( MSXML::CLSID_DOMDocument );

	//doc->async = false;
	doc->load( "EnshutsuAuto.xml" );
}

int main( int argc, char** argv ) {

	CoInitialize( NULL );

	for ( int i = 0; i < 60; i++) {
		foo();
		Sleep( 1000 );
	}

	CoUninitialize();
	
	return 0;
}
