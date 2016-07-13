// ObjSample.h : CObjSample ÇÃêÈåæ

#ifndef __OBJSAMPLE_H_
#define __OBJSAMPLE_H_

#include "resource.h"       // ÉÅÉCÉì ÉVÉìÉ{Éã

/////////////////////////////////////////////////////////////////////////////
// CObjSample
class ATL_NO_VTABLE CObjSample : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CObjSample, &CLSID_ObjSample>,
	public IDispatchImpl<IObjSample, &IID_IObjSample, &LIBID_COMSAMPLELib>
{
public:
	CObjSample()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_OBJSAMPLE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CObjSample)
	COM_INTERFACE_ENTRY(IObjSample)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IObjSample
public:
	STDMETHOD(VariantRet)(/*[out, retval]*/ VARIANT *data);
	STDMETHOD(StringArg)(/*[in]*/ BSTR data);
	STDMETHOD(VariantPArg)(/*[in, out]*/ VARIANT *data);
	STDMETHOD(VariantArg)(/*[in]*/ VARIANT data);
};

#endif //__OBJSAMPLE_H_
