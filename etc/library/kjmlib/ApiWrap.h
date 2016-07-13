// ApiWrap.h : CApiWrap ÇÃêÈåæ

#ifndef __APIWRAP_H_
#define __APIWRAP_H_

#include "resource.h"       // ÉÅÉCÉì ÉVÉìÉ{Éã

/////////////////////////////////////////////////////////////////////////////
// CApiWrap
class ATL_NO_VTABLE CApiWrap : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CApiWrap, &CLSID_ApiWrap>,
	public IDispatchImpl<IApiWrap, &IID_IApiWrap, &LIBID_KJMLIBLib>
{
public:
	CApiWrap()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_APIWRAP)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CApiWrap)
	COM_INTERFACE_ENTRY(IApiWrap)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IApiWrap
public:
	STDMETHOD(GetModuleFileNameAPI)(/*[in]*/ long hModule, /*[out,retval]*/ BSTR* pVal);
};

#endif //__APIWRAP_H_
