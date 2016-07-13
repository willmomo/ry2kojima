// ObjSample.cpp : CObjSample のインプリメンテーション
#include "stdafx.h"
#include "ComSample.h"
#include "ObjSample.h"

/////////////////////////////////////////////////////////////////////////////
// CObjSample


STDMETHODIMP CObjSample::VariantArg(VARIANT data)
{
	data.lVal = 2;

	return S_OK;
}

STDMETHODIMP CObjSample::VariantPArg(VARIANT *data)
{
	VARIANT work;
	VariantInit(&work);
	VariantChangeType(&work, &work, 0, VT_I2);
	work.iVal = 5;

	VariantCopy(data, &work);

	return S_OK;
}

STDMETHODIMP CObjSample::StringArg(BSTR data)
{
	// TODO: この位置にインプリメント用のコードを追加してください

	return S_OK;
}

STDMETHODIMP CObjSample::VariantRet(VARIANT *data)
{
	VariantInit(data);
	data->vt = VT_ARRAY | VT_VARIANT;

	SAFEARRAYBOUND sab = {128, 0};
	SAFEARRAY* psa = SafeArrayCreate(VT_VARIANT, 1, &sab);

	VARIANT *p;
	SafeArrayAccessData(psa, (LPVOID*)&p);
	for (int i = 0; i < 128; i++) {
		VariantInit(&p[i]);
		p[i].vt = VT_UI1;
		p[i].bVal = i;
	}
	SafeArrayUnaccessData(psa);

	data->parray = psa;

	return S_OK;
}
