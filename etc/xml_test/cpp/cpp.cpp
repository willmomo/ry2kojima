// cpp.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"

#import <msxml3.dll> raw_interfaces_only
using namespace MSXML2;

const CLSID CLSID_DOMDocument = {
	0xF5078F18,
		0xC551,
		0x11D3,
		0x89, 0xB9,
		0x00, 0x00, 0xF8, 0x1F, 0xE2, 0x21
};

const IID IID_IXMLDOMDocument = {
	0x2933BF88,
	0x7B36,
	0x11D2,
	0xB2, 0x0E, 0x00, 0xC0, 0x4F, 0x98, 0x3E, 0x60};

//const CLSID CLSID_DOMDocument = {0xF5, 0x07, 0x8F, 0x18, 0xC5, 0x51, 0x11, 0xD3, 0x89, 0xB9, 0x00, 0x00, 0xF8, 0x1F, 0xE2, 0x21};

BSTR CBStr(LPCSTR pszAscii)
{
	int len = MultiByteToWideChar(CP_ACP, 0, pszAscii, -1, NULL, 0);
	if (len < lstrlen(pszAscii))
		len = lstrlen(pszAscii) * 2;
	wchar_t *pwstr = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, pszAscii, -1, pwstr, MAX_PATH);
	return SysAllocString(pwstr);
}

VARIANT CVariant(LPCTSTR pszAscii)
{
	VARIANT vstr;
	BSTR pbstr = CBStr(pszAscii);
	vstr.vt = VT_BSTR;
	V_BSTR(&vstr) = pbstr;
	return vstr;
}

int main(int argc, char* argv[])
{
	IXMLDOMDocument * pdoc;

	CoInitialize(NULL);

	
	CoCreateInstance(__uuidof(DOMDocument), NULL, CLSCTX_INPROC_SERVER, __uuidof(IXMLDOMDocument), (void**)&pdoc);
	pdoc->put_async(VARIANT_FALSE);

	VARIANT_BOOL vb;
	pdoc->load(CVariant("D:\\home\\ebichu\\Projects\\xml_test\\datareq.xml"), &vb);
	pdoc->save(CVariant("D:\\home\\ebichu\\Projects\\xml_test\\dataans.xml"));
		
	getch();

	return 0;
}

