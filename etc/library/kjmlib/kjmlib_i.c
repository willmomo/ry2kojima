/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Feb 02 16:05:40 2006
 */
/* Compiler settings for C:\Documents and Settings\kojima\My Documents\My Projects\_library\kjmlib\kjmlib.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IApiWrap = {0x6E38045A,0x4E10,0x45DB,{0xBA,0x59,0x48,0x6A,0xF6,0x3C,0xE0,0x5B}};


const IID LIBID_KJMLIBLib = {0x4FF529F9,0xF9A8,0x431B,{0x82,0x18,0x28,0xC3,0x12,0x1F,0x8B,0xD9}};


const CLSID CLSID_ApiWrap = {0x6A1874C3,0xBCDE,0x41B4,{0x99,0x7B,0xC3,0xA8,0xEB,0xB8,0x9D,0x0E}};


#ifdef __cplusplus
}
#endif

