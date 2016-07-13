// ApiWrap.cpp : CApiWrap のインプリメンテーション
#include "stdafx.h"
#include "Kjmlib.h"
#include "ApiWrap.h"

/////////////////////////////////////////////////////////////////////////////
// CApiWrap


/*!
 *	API GetModuleFileNameをVBで利用するためにwrapしたメソッド
 *
 *	@param hModule	[in]モジュールのハンドル
 *	@param pVal		[out,retval]戻り値へのポインタ
 */

STDMETHODIMP CApiWrap::GetModuleFileNameAPI(long hModule, BSTR *pVal)
{
	char filename[ MAX_PATH ];
	::GetModuleFileName( ( HMODULE )hModule, filename, sizeof( filename ) );

	*pVal = CComBSTR( filename ).Copy();

	return S_OK;
}
