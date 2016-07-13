#include <windows.h>
#include <atlbase.h>
#include <iostream>

// カレントのmycom.dllは、import用にコピーしてきた
#import "mycom.dll" no_namespace named_guids

using namespace std;

int main(int argc, char** argv)
{
	if (FAILED(CoInitialize(NULL))) {
		return -1;
	}

	IMyComXPtr p;

	if (SUCCEEDED(p.CreateInstance(CLSID_MyComX))) {
		cout << "IsValid = " << CComVariant(p->IsValid()).boolVal << endl;

		p.Release();
	} else {
		cerr << "error: CreateInstance(CLSID_MyComX)" << endl;
	}

	CoUninitialize();

	return 0;
}
