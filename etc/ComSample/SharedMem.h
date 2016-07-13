// SharedMem.h : CSharedMem ÇÃêÈåæ

#ifndef __SHAREDMEM_H_
#define __SHAREDMEM_H_

#include "resource.h"       // ÉÅÉCÉì ÉVÉìÉ{Éã

/////////////////////////////////////////////////////////////////////////////
// CSharedMem
class ATL_NO_VTABLE CSharedMem : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSharedMem, &CLSID_SharedMem>,
	public IDispatchImpl<ISharedMem, &IID_ISharedMem, &LIBID_COMSAMPLELib>
{
public:
	CSharedMem()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SHAREDMEM)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CSharedMem)
	COM_INTERFACE_ENTRY(ISharedMem)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// ISharedMem
public:
	STDMETHOD(get_Ptr)(/*[out, retval]*/ long *pVal);
	STDMETHOD(ReadByte)(/*[in]*/ long size, /*[out, retval]*/ VARIANT *ret);
	STDMETHOD(Close)(/*[out, retval]*/ VARIANT_BOOL *ret);
	STDMETHOD(Open)(/*[in]*/ BSTR name, /*[out, retval]*/ VARIANT_BOOL *ret);
private:
	PVOID m_pSharedMem;
	HANDLE m_hSharedMem;
};

#endif //__SHAREDMEM_H_
