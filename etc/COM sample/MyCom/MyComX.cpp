// MyComX.cpp : CMyComX のインプリメンテーション
#include "stdafx.h"
#include "MyCom.h"
#include "MyComX.h"

/////////////////////////////////////////////////////////////////////////////
// CMyComX


STDMETHODIMP CMyComX::IsValid(VARIANT_BOOL *ret)
{
	// 現在時間から適当に TRUE/FALSE を戻す
	*ret = CComVariant((GetTickCount() % 2) ? true : false).boolVal;

	return S_OK;
}
