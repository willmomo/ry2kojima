/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Aug 15 14:52:24 2003
 */
/* Compiler settings for D:\work\MyCom\MyCom.idl:
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

#ifndef __MyCom_h__
#define __MyCom_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IMyComX_FWD_DEFINED__
#define __IMyComX_FWD_DEFINED__
typedef interface IMyComX IMyComX;
#endif 	/* __IMyComX_FWD_DEFINED__ */


#ifndef __MyComX_FWD_DEFINED__
#define __MyComX_FWD_DEFINED__

#ifdef __cplusplus
typedef class MyComX MyComX;
#else
typedef struct MyComX MyComX;
#endif /* __cplusplus */

#endif 	/* __MyComX_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IMyComX_INTERFACE_DEFINED__
#define __IMyComX_INTERFACE_DEFINED__

/* interface IMyComX */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IMyComX;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("685E132E-B485-410E-850B-4936E56326C5")
    IMyComX : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsValid( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMyComXVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMyComX __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMyComX __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMyComX __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMyComX __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMyComX __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMyComX __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMyComX __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsValid )( 
            IMyComX __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);
        
        END_INTERFACE
    } IMyComXVtbl;

    interface IMyComX
    {
        CONST_VTBL struct IMyComXVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMyComX_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMyComX_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMyComX_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMyComX_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMyComX_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMyComX_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMyComX_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMyComX_IsValid(This,ret)	\
    (This)->lpVtbl -> IsValid(This,ret)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMyComX_IsValid_Proxy( 
    IMyComX __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);


void __RPC_STUB IMyComX_IsValid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMyComX_INTERFACE_DEFINED__ */



#ifndef __MYCOMLib_LIBRARY_DEFINED__
#define __MYCOMLib_LIBRARY_DEFINED__

/* library MYCOMLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_MYCOMLib;

EXTERN_C const CLSID CLSID_MyComX;

#ifdef __cplusplus

class DECLSPEC_UUID("EC268291-0CF2-446A-AD07-5E74A1FD64AC")
MyComX;
#endif
#endif /* __MYCOMLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
