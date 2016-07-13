// SharedMem.cpp : CSharedMem のインプリメンテーション
#include "stdafx.h"
#include "ComSample.h"
#include "SharedMem.h"

/////////////////////////////////////////////////////////////////////////////
// CSharedMem


STDMETHODIMP CSharedMem::Open(BSTR name, VARIANT_BOOL *ret)
{
	USES_CONVERSION;
//	m_hSharedMem = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, OLE2T(name));
	m_hSharedMem = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "okok.bin");

	if (m_hSharedMem == NULL) {
		*ret = CComVariant(false).boolVal;
		return S_OK;
	}

	m_pSharedMem = MapViewOfFile(m_hSharedMem, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	*ret = CComVariant(true).boolVal;

	return S_OK;
}

STDMETHODIMP CSharedMem::Close(VARIANT_BOOL *ret)
{
	UnmapViewOfFile(m_pSharedMem);
	CloseHandle(m_hSharedMem);

	*ret = CComVariant(true).boolVal;

	return S_OK;
}

STDMETHODIMP CSharedMem::ReadByte(long size, VARIANT *ret)
{
	VariantInit(ret);
	ret->vt = VT_ARRAY|VT_UI1;

	SAFEARRAYBOUND sab = {128, 0};
	SAFEARRAY *psa = SafeArrayCreate(VT_UI1, 1, &sab);

	BYTE c = ((LPBYTE)m_pSharedMem)[0];

#if 0
	LPBYTE p;
	SafeArrayAccessData(psa, (LPVOID*)&p);
	memmove(p, m_pSharedMem, 5);
	SafeArrayUnaccessData(psa);
	VARIANT *p;
	SafeArrayAccessData(psa, (LPVOID*)&p);
	for (int i = 0; i < size; i++) {
		VariantInit(&p[i]);
		p[i].vt = VT_UI1;
		p[i].bVal = ((unsigned char *)m_pSharedMem)[i];
	}
	SafeArrayUnaccessData(psa);
#endif

	ret->parray = psa;

	return S_OK;
}

STDMETHODIMP CSharedMem::get_Ptr(long *pVal)
{
	*pVal = (long)m_hSharedMem;

	return S_OK;
}
