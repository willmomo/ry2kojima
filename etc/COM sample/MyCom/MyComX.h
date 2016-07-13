// MyComX.h : CMyComX ÇÃêÈåæ

#ifndef __MYCOMX_H_
#define __MYCOMX_H_

#include "resource.h"       // ÉÅÉCÉì ÉVÉìÉ{Éã

/////////////////////////////////////////////////////////////////////////////
// CMyComX
class ATL_NO_VTABLE CMyComX : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMyComX, &CLSID_MyComX>,
	public IDispatchImpl<IMyComX, &IID_IMyComX, &LIBID_MYCOMLib>
{
public:
	CMyComX()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MYCOMX)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMyComX)
	COM_INTERFACE_ENTRY(IMyComX)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IMyComX
public:
	STDMETHOD(IsValid)(/*[out, retval]*/ VARIANT_BOOL *ret);
};

#endif //__MYCOMX_H_
