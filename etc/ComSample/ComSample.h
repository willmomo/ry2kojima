/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Dec 21 17:52:56 2003
 */
/* Compiler settings for D:\work\ComSample\ComSample.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ComSample_h__
#define __ComSample_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IObjSample_FWD_DEFINED__
#define __IObjSample_FWD_DEFINED__
typedef interface IObjSample IObjSample;
#endif 	/* __IObjSample_FWD_DEFINED__ */


#ifndef __ISharedMem_FWD_DEFINED__
#define __ISharedMem_FWD_DEFINED__
typedef interface ISharedMem ISharedMem;
#endif 	/* __ISharedMem_FWD_DEFINED__ */


#ifndef __ObjSample_FWD_DEFINED__
#define __ObjSample_FWD_DEFINED__

#ifdef __cplusplus
typedef class ObjSample ObjSample;
#else
typedef struct ObjSample ObjSample;
#endif /* __cplusplus */

#endif 	/* __ObjSample_FWD_DEFINED__ */


#ifndef __SharedMem_FWD_DEFINED__
#define __SharedMem_FWD_DEFINED__

#ifdef __cplusplus
typedef class SharedMem SharedMem;
#else
typedef struct SharedMem SharedMem;
#endif /* __cplusplus */

#endif 	/* __SharedMem_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IObjSample_INTERFACE_DEFINED__
#define __IObjSample_INTERFACE_DEFINED__

/* interface IObjSample */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IObjSample;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0103BAFC-9F94-468C-9900-83B8EB1B8EB0")
    IObjSample : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE VariantArg( 
            /* [in] */ VARIANT data) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE VariantPArg( 
            /* [out][in] */ VARIANT __RPC_FAR *data) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StringArg( 
            /* [in] */ BSTR data) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE VariantRet( 
            /* [retval][out] */ VARIANT __RPC_FAR *data) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IObjSampleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IObjSample __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IObjSample __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IObjSample __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IObjSample __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IObjSample __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IObjSample __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IObjSample __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *VariantArg )( 
            IObjSample __RPC_FAR * This,
            /* [in] */ VARIANT data);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *VariantPArg )( 
            IObjSample __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *data);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StringArg )( 
            IObjSample __RPC_FAR * This,
            /* [in] */ BSTR data);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *VariantRet )( 
            IObjSample __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *data);
        
        END_INTERFACE
    } IObjSampleVtbl;

    interface IObjSample
    {
        CONST_VTBL struct IObjSampleVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IObjSample_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IObjSample_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IObjSample_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IObjSample_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IObjSample_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IObjSample_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IObjSample_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IObjSample_VariantArg(This,data)	\
    (This)->lpVtbl -> VariantArg(This,data)

#define IObjSample_VariantPArg(This,data)	\
    (This)->lpVtbl -> VariantPArg(This,data)

#define IObjSample_StringArg(This,data)	\
    (This)->lpVtbl -> StringArg(This,data)

#define IObjSample_VariantRet(This,data)	\
    (This)->lpVtbl -> VariantRet(This,data)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IObjSample_VariantArg_Proxy( 
    IObjSample __RPC_FAR * This,
    /* [in] */ VARIANT data);


void __RPC_STUB IObjSample_VariantArg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IObjSample_VariantPArg_Proxy( 
    IObjSample __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *data);


void __RPC_STUB IObjSample_VariantPArg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IObjSample_StringArg_Proxy( 
    IObjSample __RPC_FAR * This,
    /* [in] */ BSTR data);


void __RPC_STUB IObjSample_StringArg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IObjSample_VariantRet_Proxy( 
    IObjSample __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *data);


void __RPC_STUB IObjSample_VariantRet_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IObjSample_INTERFACE_DEFINED__ */


#ifndef __ISharedMem_INTERFACE_DEFINED__
#define __ISharedMem_INTERFACE_DEFINED__

/* interface ISharedMem */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISharedMem;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("52051C73-1877-4B7F-BA39-46DAF160E85B")
    ISharedMem : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Open( 
            /* [in] */ BSTR name,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Close( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadByte( 
            /* [in] */ long size,
            /* [retval][out] */ VARIANT __RPC_FAR *ret) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Ptr( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISharedMemVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISharedMem __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISharedMem __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISharedMem __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ISharedMem __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ISharedMem __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ISharedMem __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ISharedMem __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Open )( 
            ISharedMem __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Close )( 
            ISharedMem __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadByte )( 
            ISharedMem __RPC_FAR * This,
            /* [in] */ long size,
            /* [retval][out] */ VARIANT __RPC_FAR *ret);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Ptr )( 
            ISharedMem __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        END_INTERFACE
    } ISharedMemVtbl;

    interface ISharedMem
    {
        CONST_VTBL struct ISharedMemVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISharedMem_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISharedMem_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISharedMem_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISharedMem_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISharedMem_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISharedMem_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISharedMem_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISharedMem_Open(This,name,ret)	\
    (This)->lpVtbl -> Open(This,name,ret)

#define ISharedMem_Close(This,ret)	\
    (This)->lpVtbl -> Close(This,ret)

#define ISharedMem_ReadByte(This,size,ret)	\
    (This)->lpVtbl -> ReadByte(This,size,ret)

#define ISharedMem_get_Ptr(This,pVal)	\
    (This)->lpVtbl -> get_Ptr(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISharedMem_Open_Proxy( 
    ISharedMem __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);


void __RPC_STUB ISharedMem_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISharedMem_Close_Proxy( 
    ISharedMem __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);


void __RPC_STUB ISharedMem_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISharedMem_ReadByte_Proxy( 
    ISharedMem __RPC_FAR * This,
    /* [in] */ long size,
    /* [retval][out] */ VARIANT __RPC_FAR *ret);


void __RPC_STUB ISharedMem_ReadByte_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISharedMem_get_Ptr_Proxy( 
    ISharedMem __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISharedMem_get_Ptr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISharedMem_INTERFACE_DEFINED__ */



#ifndef __COMSAMPLELib_LIBRARY_DEFINED__
#define __COMSAMPLELib_LIBRARY_DEFINED__

/* library COMSAMPLELib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_COMSAMPLELib;

EXTERN_C const CLSID CLSID_ObjSample;

#ifdef __cplusplus

class DECLSPEC_UUID("04F5B444-0AB3-41DC-8B95-EB7AAB91D0B1")
ObjSample;
#endif

EXTERN_C const CLSID CLSID_SharedMem;

#ifdef __cplusplus

class DECLSPEC_UUID("DA39F402-91A3-4A93-ACFB-8CCE250BDC83")
SharedMem;
#endif
#endif /* __COMSAMPLELib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
