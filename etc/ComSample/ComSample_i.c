/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Dec 21 17:52:56 2003
 */
/* Compiler settings for D:\work\ComSample\ComSample.idl:
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

const IID IID_IObjSample = {0x0103BAFC,0x9F94,0x468C,{0x99,0x00,0x83,0xB8,0xEB,0x1B,0x8E,0xB0}};


const IID IID_ISharedMem = {0x52051C73,0x1877,0x4B7F,{0xBA,0x39,0x46,0xDA,0xF1,0x60,0xE8,0x5B}};


const IID LIBID_COMSAMPLELib = {0xE668FDD8,0x4152,0x43DF,{0xAF,0xBD,0xE7,0x06,0x47,0x44,0xAB,0x02}};


const CLSID CLSID_ObjSample = {0x04F5B444,0x0AB3,0x41DC,{0x8B,0x95,0xEB,0x7A,0xAB,0x91,0xD0,0xB1}};


const CLSID CLSID_SharedMem = {0xDA39F402,0x91A3,0x4A93,{0xAC,0xFB,0x8C,0xCE,0x25,0x0B,0xDC,0x83}};


#ifdef __cplusplus
}
#endif

