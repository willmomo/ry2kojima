// MyComX.cpp : CMyComX �̃C���v�������e�[�V����
#include "stdafx.h"
#include "MyCom.h"
#include "MyComX.h"

/////////////////////////////////////////////////////////////////////////////
// CMyComX


STDMETHODIMP CMyComX::IsValid(VARIANT_BOOL *ret)
{
	// ���ݎ��Ԃ���K���� TRUE/FALSE ��߂�
	*ret = CComVariant((GetTickCount() % 2) ? true : false).boolVal;

	return S_OK;
}
