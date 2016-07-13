//---------------------------------------------------------------------
// original_dibapi から必要なものだけを cpp で書きなおしたもの
//---------------------------------------------------------------------
#include "dibapi.h"
#include <assert.h>

// Dib Header Marker - used in writing DIBs to files
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

/****************************************************************************

 FUNCTION   : MyWrite(int fh, VOID FAR *pv, DWORD ul)

 PURPOSE    : Writes data in steps of 32k till all the data is written.
              Normal _lwrite uses a WORD as 3rd parameter, so it is
              limited to 32767 bytes, but this procedure is not.

 RETURNS    : 0 - If write did not proceed correctly.
              number of bytes written otherwise.
 
  History:   Date      Author       Reason
             9/15/91   Mark Bader   Based on DIBVIEW

 ****************************************************************************/
static DWORD MyWrite(int iFileHandle, VOID *lpBuffer, DWORD dwBytes) {
	DWORD dwBytesTmp = dwBytes;			// Save # of bytes for return value
	BYTE *hpBuffer = (LPBYTE)lpBuffer;	// make a huge pointer to the data

	// Write out the data in 32767 byte chunks.

	while (dwBytes > 32767) {
		if (_lwrite(iFileHandle, (LPSTR)hpBuffer, (WORD)32767) != 32767)
			return 0;
		dwBytes -= 32767;
		hpBuffer += 32767;
	}

	/* Write out the last chunk (which is < 32767 bytes) */
	if (_lwrite(iFileHandle, (LPSTR)hpBuffer, (WORD)dwBytes) != (WORD)dwBytes)
		return 0;

	return dwBytesTmp;
}

//---------------------------------------------------------------------
// This function gets the number of palette entries on the specified device
//
// [引数]
// hDC  device context
//
// [戻り値]
// number of palette entries on device
//---------------------------------------------------------------------
int PalEntriesOnDevice(HDC hDC) {
	// Find out the number of palette entries on this device.
	int nColors = GetDeviceCaps(hDC, SIZEPALETTE);

	// For non-palette devices, we'll use the # of system colors for our palette size.
	if (nColors == 0)
		nColors = GetDeviceCaps(hDC, NUMCOLORS);

	assert(nColors);
	return nColors;
}

/*************************************************************************
 *
 * GetSystemPalette()
 *
 * Parameters:
 *
 * None
 *
 * Return Value:
 *
 * HPALETTE         - handle to a copy of the current system palette
 *
 * Description:
 *
 * This function returns a handle to a palette which represents the system
 * palette.  The system RGB values are copied into our logical palette using
 * the GetSystemPaletteEntries function.  
 *
 * History:   
 *            
 *    Date      Author               Reason        
 *    6/01/91   Garrett McAuliffe    Created        
 *    9/15/91   Patrick Schreiber    Added header and comments
 *    12/20/91  Mark Bader           Added GetSystemPaletteEntries call
 *
 ************************************************************************/


HPALETTE FAR GetSystemPalette(void)
{
   HDC hDC;                // handle to a DC
   static HPALETTE hPal = NULL;   // handle to a palette
   HANDLE hLogPal;         // handle to a logical palette
   LPLOGPALETTE lpLogPal;  // pointer to a logical palette
   int nColors;            // number of colors

   /* Find out how many palette entries we want. */

   hDC = GetDC(NULL);
   if (!hDC)
      return NULL;
   nColors = PalEntriesOnDevice(hDC);   // Number of palette entries

   /* Allocate room for the palette and lock it. */
   hLogPal = GlobalAlloc(GHND, sizeof(LOGPALETTE) + nColors * sizeof(
             PALETTEENTRY));

   /* if we didn't get a logical palette, return NULL */
   if (!hLogPal)
      return NULL;

   /* get a pointer to the logical palette */
   lpLogPal = (LPLOGPALETTE)GlobalLock(hLogPal);

   /* set some important fields */
   lpLogPal->palVersion = PALVERSION;
   lpLogPal->palNumEntries = nColors;

   /* Copy the current system palette into our logical palette */

   GetSystemPaletteEntries(hDC, 0, nColors, 
                           (LPPALETTEENTRY)(lpLogPal->palPalEntry));

   /*  Go ahead and create the palette.  Once it's created,
    *  we no longer need the LOGPALETTE, so free it.
    */

   hPal = CreatePalette(lpLogPal);

   /* clean up */
   GlobalUnlock(hLogPal);
   GlobalFree(hLogPal);
   ReleaseDC(NULL, hDC);

   return hPal;
}

/*************************************************************************
 *
 * DIBNumColors()
 *
 * Parameter:
 *
 * LPSTR lpDIB      - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * WORD             - number of colors in the color table
 *
 * Description:
 *
 * This function calculates the number of colors in the DIB's color table
 * by finding the bits per pixel for the DIB (whether Win3.0 or OS/2-style
 * DIB). If bits per pixel is 1: colors=2, if 4: colors=16, if 8: colors=256,
 * if 24, no colors in color table.
 *
 * History:   Date      Author               Reason
 *            6/01/91   Garrett McAuliffe    Created
 *            9/15/91   Patrick Schreiber    Added header and comments
 *
 ************************************************************************/


WORD FAR DIBNumColors(LPSTR lpDIB)
{
   WORD wBitCount;  // DIB bit count

   /*  If this is a Windows-style DIB, the number of colors in the
    *  color table can be less than the number of bits per pixel
    *  allows for (i.e. lpbi->biClrUsed can be set to some value).
    *  If this is the case, return the appropriate value.
    */

   if (IS_WIN30_DIB(lpDIB))
   {
      DWORD dwClrUsed;

      dwClrUsed = ((LPBITMAPINFOHEADER)lpDIB)->biClrUsed;
      if (dwClrUsed)
     return (WORD)dwClrUsed;
   }

   /*  Calculate the number of colors in the color table based on
    *  the number of bits per pixel for the DIB.
    */
   if (IS_WIN30_DIB(lpDIB))
      wBitCount = ((LPBITMAPINFOHEADER)lpDIB)->biBitCount;
   else
      wBitCount = ((LPBITMAPCOREHEADER)lpDIB)->bcBitCount;

   /* return number of colors based on bits per pixel */
   switch (wBitCount)
      {
   case 1:
      return 2;

   case 4:
      return 16;

   case 8:
      return 256;

   default:
      return 0;
      }
}

/*************************************************************************
 *
 * PaletteSize()
 *
 * Parameter:
 *
 * LPSTR lpDIB      - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * WORD             - size of the color palette of the DIB
 *
 * Description:
 *
 * This function gets the size required to store the DIB's palette by
 * multiplying the number of colors by the size of an RGBQUAD (for a
 * Windows 3.0-style DIB) or by the size of an RGBTRIPLE (for an OS/2-
 * style DIB).
 *
 * History:   Date      Author             Reason
 *            6/01/91   Garrett McAuliffe  Created
 *            9/15/91   Patrick Schreiber  Added header and comments
 *
 ************************************************************************/


WORD FAR PaletteSize(LPSTR lpDIB)
{
   /* calculate the size required by the palette */
   if (IS_WIN30_DIB (lpDIB))
      return (DIBNumColors(lpDIB) * sizeof(RGBQUAD));
   else
      return (DIBNumColors(lpDIB) * sizeof(RGBTRIPLE));
}

/*************************************************************************
 *
 * BitmapToDIB()
 *
 * Parameters:
 *
 * HBITMAP hBitmap  - specifies the bitmap to convert
 *
 * HPALETTE hPal    - specifies the palette to use with the bitmap
 *
 * Return Value:
 *
 * HDIB             - identifies the device-dependent bitmap
 *
 * Description:
 *
 * This function creates a DIB from a bitmap using the specified palette.
 *
 * History:   Date      Author               Reason
 *            6/01/91   Garrett McAuliffe    Created
 *            9/15/91   Patrick Schreiber    Added header and comments
 *            12/10/91  Patrick Schreiber    Added bits per pixel validation
 *                                           and check GetObject return value
 *
 ************************************************************************/


HDIB BitmapToDIB(HBITMAP hBitmap, HPALETTE hPal) {
	BITMAP bm;					// bitmap structure
	BITMAPINFOHEADER bi;		// bitmap header
	LPBITMAPINFOHEADER lpbi;	// pointer to BITMAPINFOHEADER
	DWORD dwLen;				// size of memory block
	HANDLE hDIB, h;				// handle to DIB, temp handle
	HDC hDC;					// handle to DC
	WORD biBits;				// bits per pixel

	/* check if bitmap handle is valid */

	if (!hBitmap)
		return NULL;

	/* fill in BITMAP structure, return NULL if it didn't work */
	if (!GetObject(hBitmap, sizeof(bm), (LPSTR)&bm))
		return NULL;

	/* if no palette is specified, use default palette */
	if (hPal == NULL)
		hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);

	/* calculate bits per pixel */
	biBits = bm.bmPlanes * bm.bmBitsPixel;

	/* make sure bits per pixel is valid */
	if (biBits <= 1)
		biBits = 1;
	else if (biBits <= 4)
		biBits = 4;
	else if (biBits <= 8)
		biBits = 8;
	else /* if greater than 8-bit, force to 24-bit */
		biBits = 24;

	/* initialize BITMAPINFOHEADER */
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bm.bmWidth;
	bi.biHeight = bm.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = biBits;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	/* calculate size of memory block required to store BITMAPINFO */
	dwLen = bi.biSize + PaletteSize((LPSTR)&bi);

	/* get a DC */
	hDC = GetDC(NULL);

	/* select and realize our palette */
	hPal = SelectPalette(hDC, hPal, FALSE);
	RealizePalette(hDC);

	/* alloc memory block to store our bitmap */
	hDIB = GlobalAlloc(GHND, dwLen);

	/* if we couldn't get memory block */
	if (!hDIB) {
		/* clean up and return NULL */
		SelectPalette(hDC, hPal, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
		return NULL;
	}

	/* lock memory and get pointer to it */
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);

	/* use our bitmap info. to fill BITMAPINFOHEADER */
	*lpbi = bi;

	// call GetDIBits with a NULL lpBits param, so it will calculate the
	// biSizeImage field for us
	GetDIBits(hDC, hBitmap, 0, (WORD)bi.biHeight, NULL, (LPBITMAPINFO)lpbi,
	DIB_RGB_COLORS);

	/* get the info. returned by GetDIBits and unlock memory block */
	bi = *lpbi;
	GlobalUnlock(hDIB);

	/* if the driver did not fill in the biSizeImage field, make one up */
	if (bi.biSizeImage == 0)
		bi.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth * biBits) * bm.bmHeight;

	/* realloc the buffer big enough to hold all the bits */
	dwLen = bi.biSize + PaletteSize((LPSTR)&bi) + bi.biSizeImage;
	if (h = GlobalReAlloc(hDIB, dwLen, 0))
		hDIB = h;
	else {
		/* clean up and return NULL */
		GlobalFree(hDIB);
		hDIB = NULL;
		SelectPalette(hDC, hPal, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
		return NULL;
	}

	/* lock memory block and get pointer to it */
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);

	// call GetDIBits with a NON-NULL lpBits param, and actualy get the
	// bits this time
	if (GetDIBits(hDC, hBitmap, 0, (WORD)bi.biHeight, (LPSTR)lpbi + (WORD)lpbi->biSize + PaletteSize((LPSTR)lpbi), (LPBITMAPINFO)lpbi,DIB_RGB_COLORS) == 0) {
		/* clean up and return NULL */
		GlobalUnlock(hDIB);
		hDIB = NULL;
		SelectPalette(hDC, hPal, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
		return NULL;
	}
	bi = *lpbi;

	/* clean up */
	GlobalUnlock(hDIB);
	SelectPalette(hDC, hPal, TRUE);
	RealizePalette(hDC);
	ReleaseDC(NULL, hDC);

	/* return handle to the DIB */
	return hDIB;
}

/*************************************************************************
 *
 * CopyScreenToBitmap()
 *
 * Parameter:
 *
 * LPRECT lpRect    - specifies the window
 *
 * Return Value:
 *
 * HDIB             - identifies the device-dependent bitmap
 *
 * Description:
 *
 * This function copies the specified part of the screen to a device-
 * dependent bitmap.
 *
 * History:   Date      Author             Reason
 *            9/15/91   Patrick Schreiber  Created
 *            9/25/91   Patrick Schreiber  Added header and comments
 *
 ************************************************************************/


HBITMAP FAR CopyScreenToBitmap(LPRECT lpRect)
{
   HDC hScrDC, hMemDC;           // screen DC and memory DC
   HBITMAP hBitmap, hOldBitmap;  // handles to deice-dependent bitmaps
   int nX, nY, nX2, nY2;         // coordinates of rectangle to grab
   int nWidth, nHeight;          // DIB width and height
   int xScrn, yScrn;             // screen resolution

   /* check for an empty rectangle */

   if (IsRectEmpty(lpRect))
      return NULL;

   /*  create a DC for the screen and create
    *  a memory DC compatible to screen DC
    */
   hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
   hMemDC = CreateCompatibleDC(hScrDC);

   /* get points of rectangle to grab */
   nX = lpRect->left;
   nY = lpRect->top;
   nX2 = lpRect->right;
   nY2 = lpRect->bottom;

   /* get screen resolution */
   xScrn = GetDeviceCaps(hScrDC, HORZRES);
   yScrn = GetDeviceCaps(hScrDC, VERTRES);

   /* make sure bitmap rectangle is visible */
   if (nX < 0)
      nX = 0;
   if (nY < 0)
      nY = 0;
   if (nX2 > xScrn)
      nX2 = xScrn;
   if (nY2 > yScrn)
      nY2 = yScrn;
   nWidth = nX2 - nX;
   nHeight = nY2 - nY;

   /* create a bitmap compatible with the screen DC */
   hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);

   /* select new bitmap into memory DC */
   hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

   /* bitblt screen DC to memory DC */
   BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY);

   /*  select old bitmap back into memory DC and get handle to
    *  bitmap of the screen
    */
   hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);

   /* clean up */
   DeleteDC(hScrDC);
   DeleteDC(hMemDC);

   /* return handle to the bitmap */
   return hBitmap;
}

/*************************************************************************
 *
 * CopyScreenToDIB()
 *
 * Parameter:
 *
 * LPRECT lpRect    - specifies the window
 *
 * Return Value:
 *
 * HDIB             - identifies the device-independent bitmap
 *
 * Description:
 *
 * This function copies the specified part of the screen to a device-
 * independent bitmap.
 *
 * History:   Date      Author             Reason
 *            9/15/91   Patrick Schreiber  Created
 *            9/25/91   Patrick Schreiber  Added header and comments
 *            12/10/91  Patrick Schreiber  Released palette
 *
 ************************************************************************/


HDIB FAR CopyScreenToDIB(LPRECT lpRect)
{
	HBITMAP hBitmap;    // handle to device-dependent bitmap
	HPALETTE hPalette;  // handle to palette
	HDIB hDIB = NULL;   // handle to DIB

	// get the device-dependent bitmap in lpRect by calling
	// CopyScreenToBitmap and passing it the rectangle to grab

	hBitmap = CopyScreenToBitmap(lpRect);

	/* check for a valid bitmap handle */
	if (!hBitmap)
		return NULL;

	/* get the current palette */
	hPalette = GetSystemPalette();

	/* convert the bitmap to a DIB */
	hDIB = BitmapToDIB(hBitmap, hPalette);

	/* clean up */
	DeleteObject(hPalette);
	DeleteObject(hBitmap);

	/* return handle to the packed-DIB */
	return hDIB;
}

//---------------------------------------------------------------------
// Saves the specified DIB into the specified file name on disk.  No
// error checking is done, so if the file already exists, it will be
// written over.
//
// [引数]
// HDIB hDib - Handle to the dib to save
// LPSTR lpFileName - pointer to full pathname to save DIB under
//
// [戻り値]
// 0 if successful, or one of:
// ERR_INVALIDHANDLE
// ERR_OPEN
// ERR_LOCK
//
// [備考]
// The DIB API were not written to handle OS/2 DIBs, so this
// function will not save a file if it is not a Windows DIB.
//---------------------------------------------------------------------
WORD SaveDIB(HDIB hDib, LPCTSTR lpFileName) {
	BITMAPFILEHEADER bmfHdr; // Header for Bitmap file
	LPBITMAPINFOHEADER lpBI;   // Pointer to DIB info structure
	int fh;     // file handle for opened file
	OFSTRUCT of;     // OpenFile structure
	DWORD dwDIBSize;
	DWORD dwError;   // Error return from MyWrite

	if (!hDib)
		return ERR_INVALIDHANDLE;

	fh = OpenFile(lpFileName, &of, OF_CREATE | OF_READWRITE);
	if (fh == -1)
		return ERR_OPEN;

	// Get a pointer to the DIB memory, the first of which contains
	// a BITMAPINFO structure
	lpBI = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	if (!lpBI)
		return ERR_LOCK;

	// Check to see if we're dealing with an OS/2 DIB.  If so, don't
	// save it because our functions aren't written to deal with these
	// DIBs.

	if (lpBI->biSize != sizeof(BITMAPINFOHEADER)) {
		GlobalUnlock(hDib);
		return ERR_NOT_DIB;
	}

	// Fill in the fields of the file header

	// Fill in file type (first 2 bytes must be "BM" for a bitmap)
	bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM"

	// Calculating the size of the DIB is a bit tricky (if we want to
	// do it right).  The easiest way to do this is to call GlobalSize()
	// on our global handle, but since the size of our global memory may have
	// been padded a few bytes, we may end up writing out a few too
	// many bytes to the file (which may cause problems with some apps,
	// like HC 3.0).
	//
	// So, instead let's calculate the size manually.
	//
	// To do this, find size of header plus size of color table.  Since the
	// first DWORD in both BITMAPINFOHEADER and BITMAPCOREHEADER conains
	// the size of the structure, let's use this.

	dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPSTR)lpBI);  // Partial Calculation

	// Now calculate the size of the image

	if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4)) {

		// It's an RLE bitmap, we can't calculate size, so trust the
		// biSizeImage field

		dwDIBSize += lpBI->biSizeImage;
	} else {
		DWORD dwBmBitsSize;  // Size of Bitmap Bits only

		// It's not RLE, so size is Width (DWORD aligned) * Height

		dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;

		dwDIBSize += dwBmBitsSize;

		// Now, since we have calculated the correct size, why don't we
		// fill in the biSizeImage field (this will fix any .BMP files which 
		// have this field incorrect).

		lpBI->biSizeImage = dwBmBitsSize;
	}


	// Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER)

	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;

	// Now, calculate the offset the actual bitmap bits will be in
	// the file -- It's the Bitmap file header plus the DIB header,
	// plus the size of the color table.
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize + PaletteSize((LPSTR)lpBI);

	// Write the file header
	_lwrite(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));

	// Write the DIB header and the bits -- use local version of
	// MyWrite, so we can write more than 32767 bytes of data
	dwError = MyWrite(fh, (LPSTR)lpBI, dwDIBSize);
	GlobalUnlock(hDib);
	_lclose(fh);

	if (dwError == 0)
		return ERR_OPEN; // oops, something happened in the write
	else
		return 0; // Success code
}

//---------------------------------------------------------------------
// Frees memory associated with a DIB
//---------------------------------------------------------------------
void DestroyDIB(HDIB hDib) {
   GlobalFree(hDib);
}
