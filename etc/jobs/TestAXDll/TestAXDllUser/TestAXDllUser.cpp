#include <windows.h>
#include <iostream>

#import "../TestAXDll/TestAXDll.dll" no_namespace named_guids

using namespace std;

int main( int argc, char** argv ) {

	::CoInitialize( NULL );

	_clsTestAXDll* pTestAXDll;
	HRESULT res = ::CoCreateInstance( CLSID_clsTestAXDll, NULL, CLSCTX_INPROC_SERVER, IID__clsTestAXDll, (void**)&pTestAXDll );
	if ( SUCCEEDED( res ) ) {
		
		cout << pTestAXDll->RetDouble_ValLong_ValLong( 10, 4 ) << endl;

		double dret;
		VARIANT_BOOL ret = pTestAXDll->RetBoolean_ValLong_ValLong_RefDouble( 10, 4, &dret );

		cout<< ret << " , " << dret << endl;

		pTestAXDll->Release();
	}

	::CoUninitialize();

	return 0;
}
