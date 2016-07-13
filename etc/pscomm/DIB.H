
#ifndef DIB_INCLUDED
#define DIB_INCLUDED

#include <windows.h>
#include <string.h>
#include "errors.h"
#include "palette.h"

// WIDTHBYTESは、スキャンラインの1本当たりのビット数を取得して、最も近い
// WORD値に丸める

#define WIDTHBYTES(bits)      (((bits) + 31) / 32 * 4)


   // DIBヘッダーを指すポインタを指定すると、Windows 3.0形式のDIBならばTRUE、
   // それ以外(PM形式のDIB)ならばFALSEを返す

#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD) (lpbi)) == sizeof (BITMAPINFOHEADER))


#ifdef __cplusplus
extern "C" {
#endif
void     RealizeDIBPalette    (HDC hDC, LPBITMAPINFO lpbmi);
WORD     DIBNumColors         (LPSTR lpbi);
LPSTR    FindDIBBits          (LPSTR lpbi);
WORD     PaletteSize          (LPSTR lpbi);
HPALETTE CreateDIBPalette     (HANDLE hDIB);
DWORD    DIBHeight            (LPSTR lpDIB);
DWORD    DIBWidth             (LPSTR lpDIB);
HBITMAP  DIBToBitmap          (HANDLE hDIB, HPALETTE hPal);
HANDLE   BitmapToDIB          (HBITMAP hBitmap, HPALETTE hPal);
void     InitBitmapInfoHeader (LPBITMAPINFOHEADER lpBmInfoHdr,
                                            DWORD dwWidth,
                                            DWORD dwHeight,
                                              int nBPP);
#ifdef __cplusplus
}
#endif

#endif // DIB_INCLUDED

