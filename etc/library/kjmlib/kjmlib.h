/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Feb 02 16:05:40 2006
 */
/* Compiler settings for C:\Documents and Settings\kojima\My Documents\My Projects\_library\kjmlib\kjmlib.idl:
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

#ifndef __kjmlib_h__
#define __kjmlib_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IApiWrap_FWD_DEFINED__
#define __IApiWrap_FWD_DEFINED__
typedef interface IApiWrap IApiWrap;
#endif 	/* __IApiWrap_FWD_DEFINED__ */


#ifndef __ApiWrap_FWD_DEFINED__
#define __ApiWrap_FWD_DEFINED__

#ifdef __cplusplus
typedef class ApiWrap ApiWrap;
#else
typedef struct ApiWrap ApiWrap;
#endif /* __cplusplus */

#endif 	/* __ApiWrap_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IApiWrap_INTERFACE_DEFINED__
#define __IApiWrap_INTERFACE_DEFINED__

/* interface IApiWrap */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IApiWrap;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6E38045A-4E10-45DB-BA59-486AF63CE05B")
    IApiWrap : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetModuleFileNameAPI( 
            /* [in] */ long hModule,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IApiWrapVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IApiWrap __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IApiWrap __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IApiWrap __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IApiWrap __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IApiWrap __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IApiWrap __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IApiWrap __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetModuleFileNameAPI )( 
            IApiWrap __RPC_FAR * This,
            /* [in] */ long hModule,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        END_INTERFACE
    } IApiWrapVtbl;

    interface IApiWrap
    {
        CONST_VTBL struct IApiWrapVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IApiWrap_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IApiWrap_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IApiWrap_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IApiWrap_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IApiWrap_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IApiWrap_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IApiWrap_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IApiWrap_GetModuleFileNameAPI(This,hModule,pVal)	\
    (This)->lpVtbl -> GetModuleFileNameAPI(This,hModule,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IApiWrap_GetModuleFileNameAPI_Proxy( 
    IApiWrap __RPC_FAR * This,
    /* [in] */ long hModule,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IApiWrap_GetModuleFileNameAPI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IApiWrap_INTERFACE_DEFINED__ */



#ifndef __KJMLIBLib_LIBRARY_DEFINED__
#define __KJMLIBLib_LIBRARY_DEFINED__

/* library KJMLIBLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_KJMLIBLib;

EXTERN_C const CLSID CLSID_ApiWrap;

#ifdef __cplusplus

class DECLSPEC_UUID("6A1874C3-BCDE-41B4-997B-C3A8EBB89D0E")
ApiWrap;
#endif
#endif /* __KJMLIBLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
