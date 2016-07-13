// ApiWrap.cpp : CApiWrap �̃C���v�������e�[�V����
#include "stdafx.h"
#include "Kjmlib.h"
#include "ApiWrap.h"

/////////////////////////////////////////////////////////////////////////////
// CApiWrap


/*!
 *	API GetModuleFileName��VB�ŗ��p���邽�߂�wrap�������\�b�h
 *
 *	@param hModule	[in]���W���[���̃n���h��
 *	@param pVal		[out,retval]�߂�l�ւ̃|�C���^
 */

STDMETHODIMP CApiWrap::GetModuleFileNameAPI(long hModule, BSTR *pVal)
{
	char filename[ MAX_PATH ];
	::GetModuleFileName( ( HMODULE )hModule, filename, sizeof( filename ) );

	*pVal = CComBSTR( filename ).Copy();

	return S_OK;
}
