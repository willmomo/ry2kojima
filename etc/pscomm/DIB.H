
#ifndef DIB_INCLUDED
#define DIB_INCLUDED

#include <windows.h>
#include <string.h>
#include "errors.h"
#include "palette.h"

// WIDTHBYTES�́A�X�L�������C����1�{������̃r�b�g�����擾���āA�ł��߂�
// WORD�l�Ɋۂ߂�

#define WIDTHBYTES(bits)      (((bits) + 31) / 32 * 4)


   // DIB�w�b�_�[���w���|�C���^���w�肷��ƁAWindows 3.0�`����DIB�Ȃ��TRUE�A
   // ����ȊO(PM�`����DIB)�Ȃ��FALSE��Ԃ�

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
