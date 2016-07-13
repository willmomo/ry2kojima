/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Aug 15 14:52:24 2003
 */
/* Compiler settings for D:\work\MyCom\MyCom.idl:
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

const IID IID_IMyComX = {0x685E132E,0xB485,0x410E,{0x85,0x0B,0x49,0x36,0xE5,0x63,0x26,0xC5}};


const IID LIBID_MYCOMLib = {0xC9998AAD,0x409E,0x43D9,{0xA9,0xF6,0xF9,0xDA,0x64,0xAE,0x5A,0x29}};


const CLSID CLSID_MyComX = {0xEC268291,0x0CF2,0x446A,{0xAD,0x07,0x5E,0x74,0xA1,0xFD,0x64,0xAC}};


#ifdef __cplusplus
}
#endif

