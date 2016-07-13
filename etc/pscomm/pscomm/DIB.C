//-----------------------------------------------------------------------------
// DIB.C
//
// このファイルには、DIBの操作や情報の取得を行う関数が集められています。
// これらの多くは、DIBがWindows 3.0形式かOS/2形式かを考慮する処理をアプリ
// ケーション本体から切り離すために提供されているものです。
//
// このモジュールにある関数は、渡されたDIBのポインタやハンドルが、次の2つのい
// ずれか一方の形式のメモリブロックを示していると仮定しています。
//
//       a) BITMAPINFOHEADER + カラーテーブル + DIBビット(3.0形式のDIB)
//       b) BITMAPCOREHEADER + カラーテーブル + DIBビット(OS/2 PM形式)
//
// これらの構造体の説明は、Windows 3.1 SDKのマニュアルに記載されています。
//
// このモジュールの関数の多くはShowDIBのものを流用し、OS/2のDIBを処理できる
// ように修正しました。
//
// このモジュールの関数は、DIBがすべてWindows 3.0形式であると仮定してOS/2
// DIB用のコードを削除すれば効率的に(より高速で小さく)なります。この場合、
// DIBが必ずWindows 3.0形式のDIBに変換されるように、DIBファイルを読み取る
// コード部分を修正する必要があります。DIBViewでそのようにしていないのは、
// DIBViewが、ディスプレイドライバとプリンタドライバをテストできるように作成
// されているためです(Windows 3.0形式のDIBをサポートするドライバはOS/2のDIB
// もサポートすると考えられます)。このような処理は、ShowDIBアプリケーション
// を参考にしてください。
//-----------------------------------------------------------------------------


#include "dib.h"


//---------------------------------------------------------------------
//
// 関数:       FindDIBBits
//
// 目的:       DIBを指すポインタを指定すると、DIBのビットマップビットを
//             指すポインタを返します。
//
// パラメータ: lpbi - DIBヘッダー(BITMAPINFOHEADERまたはBITMAPCOREHEADER)
//                    を指すポインタ
//
//---------------------------------------------------------------------

LPSTR FindDIBBits (LPSTR lpbi)
{
   return (lpbi + *(LPDWORD)lpbi + PaletteSize (lpbi));
}


//---------------------------------------------------------------------
//
// 関数:       DIBNumColors
//
// 目的:       DIBを指すポインタを指定すると、DIBカラーテーブルの色数を
//             返します。
//
// パラメータ: lpbi - DIBヘッダー(BITMAPINFOHEADERまたはBITMAPCOREHEADER)
//                    を指すポインタ
//
//---------------------------------------------------------------------

WORD DIBNumColors (LPSTR lpbi)
{
   WORD wBitCount;

   //  Windows形式のDIBならば、カラーテーブルの色数は、ピクセル当たりの
   //  ビット数以下である(つまり、lpbi->biClrUsedに値が設定されている)。
   //  この場合は適切な値を返す
   if (IS_WIN30_DIB (lpbi))
      {
      DWORD dwClrUsed;

      dwClrUsed = ((LPBITMAPINFOHEADER) lpbi)->biClrUsed;

      if (dwClrUsed)
         return (WORD) dwClrUsed;
      }

   // DIBのピクセル当たりのビット数を元に、カラーテーブルの色数を計算
   if (IS_WIN30_DIB (lpbi))
      wBitCount = ((LPBITMAPINFOHEADER) lpbi)->biBitCount;
   else
      wBitCount = ((LPBITMAPCOREHEADER) lpbi)->bcBitCount;

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


//---------------------------------------------------------------------
//
// 関数:       PaletteSize
//
// 目的:       DIBを指すポインタを指定すると、DIBカラーテーブルのバイト
//             数を返します。
//
// パラメータ: lpbi - DIBヘッダー(BITMAPINFOHEADERまたはBITMAPCOREHEADER)
//                    を指すポインタ
//
//---------------------------------------------------------------------

WORD PaletteSize (LPSTR lpbi)
{
   if (IS_WIN30_DIB (lpbi))
      return (DIBNumColors (lpbi) * sizeof (RGBQUAD));
   else
      return (DIBNumColors (lpbi) * sizeof (RGBTRIPLE));
}


//---------------------------------------------------------------------
//
// 関数:       CreateDIBPalette
//
// 目的:       DIBのハンドルを指定すると、論理パレットを作成して、
//             そのパレットのハンドルを返します。(ShowDIBからの流用)
//
// パラメータ: hDIB - HDIBヘッダー(BITMAPINFOHEADERまたはBITMAPCOREHEADER)
//                    を含むグローバルメモリを識別するハンドル
//
//---------------------------------------------------------------------

HPALETTE CreateDIBPalette (HANDLE hDIB)
{
   LPLOGPALETTE     lpPal;
   HANDLE           hLogPal;
   HPALETTE         hPal = NULL;
   int              i, wNumColors;
   LPSTR            lpbi;
   LPBITMAPINFO     lpbmi;
   LPBITMAPCOREINFO lpbmc;
   BOOL             bWinStyleDIB;

   if (!hDIB)
      return NULL;

   lpbi         = GlobalLock (hDIB);
   lpbmi        = (LPBITMAPINFO) lpbi;
   lpbmc        = (LPBITMAPCOREINFO) lpbi;
   wNumColors   = DIBNumColors (lpbi);
   bWinStyleDIB = IS_WIN30_DIB (lpbi);

   if (wNumColors)
      {
      hLogPal = GlobalAlloc (GHND, sizeof (LOGPALETTE) +
                             sizeof (PALETTEENTRY) * wNumColors);

      if (!hLogPal)
         {
         DIBError (ERR_CREATEPAL);
         GlobalUnlock (hDIB);
         return NULL;
         }

      lpPal = (LPLOGPALETTE) GlobalLock (hLogPal);

      lpPal->palVersion    = PALVERSION;
      lpPal->palNumEntries = wNumColors;

      for (i = 0;  i < wNumColors;  i++)
         {
         if (bWinStyleDIB)
            {
            lpPal->palPalEntry[i].peRed   = lpbmi->bmiColors[i].rgbRed;
            lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
            lpPal->palPalEntry[i].peBlue  = lpbmi->bmiColors[i].rgbBlue;
            lpPal->palPalEntry[i].peFlags = 0;
            }
         else
            {
            lpPal->palPalEntry[i].peRed   = lpbmc->bmciColors[i].rgbtRed;
            lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen;
            lpPal->palPalEntry[i].peBlue  = lpbmc->bmciColors[i].rgbtBlue;
            lpPal->palPalEntry[i].peFlags = 0;
            }
         }

      hPal = CreatePalette (lpPal);

      if (!hPal)
         DIBError (ERR_CREATEPAL);

      GlobalUnlock (hLogPal);
      GlobalFree   (hLogPal);
   }

   GlobalUnlock (hDIB);

   return hPal;
}


//---------------------------------------------------------------------
//
// 関数:       DIBHeight
//
// 目的:       DIBを指すポインタを指定すると、DIBの高さを返します。
//             (Win30のDIBの高さフィールドがDWORD型であるため)DWORDを
//             返しますが、Windows 3.0形式では上位ワードが使われていない
//             ことに注意してください。
//
// パラメータ: lpDIB - DIBヘッダー(BITMAPINFOHEADERまたはBITMAPCOREHEADER)
//                     を指すポインタ
//
//---------------------------------------------------------------------

DWORD DIBHeight (LPSTR lpDIB)
{
   LPBITMAPINFOHEADER lpbmi;
   LPBITMAPCOREHEADER lpbmc;

   lpbmi = (LPBITMAPINFOHEADER) lpDIB;
   lpbmc = (LPBITMAPCOREHEADER) lpDIB;

   if (lpbmi->biSize == sizeof (BITMAPINFOHEADER))
      return lpbmi->biHeight;
   else
      return (DWORD) lpbmc->bcHeight;
}


//---------------------------------------------------------------------
//
// 関数:       DIBWidth
//
// 目的:       DIBを指すポインタを指定すると、DIBの幅を返します。
//             (Windows 3.0形式のDIBの幅フィールドがDWORD型であるため)
//             DWORDを返しますが、3.0では上位ワードが使われていないこと
//             に注意してください。
//
// パラメータ: lpDIB - DIBヘッダー(BITMAPINFOHEADERまたはBITMAPCOREHEADER)
//                     を指すポインタ
//
//---------------------------------------------------------------------

DWORD DIBWidth (LPSTR lpDIB)
{
   LPBITMAPINFOHEADER lpbmi;
   LPBITMAPCOREHEADER lpbmc;

   lpbmi = (LPBITMAPINFOHEADER) lpDIB;
   lpbmc = (LPBITMAPCOREHEADER) lpDIB;

   if (lpbmi->biSize == sizeof (BITMAPINFOHEADER))
      return lpbmi->biWidth;
   else
      return (DWORD) lpbmc->bcWidth;
}


//---------------------------------------------------------------------
//
// 関数:       DIBToBitmap
//
// 目的:       DIBの仕様とパレットを含むグローバルメモリのハンドルを
//             指定すると、DDBを返します。DDBは指定されたパレットを
//             使って作成されます。
//
// パラメータ: hDIB - DIB (BITMAPINFOHEADERまたはBITMAPCOREHEADER)の
//                    仕様を含むグローバルメモリを識別するハンドル
//             hPal - DDBを作成するときに転送するパレット(NULLの場合は
//                    デフォルトのパレットを使用)
//
//---------------------------------------------------------------------

HBITMAP DIBToBitmap (HANDLE hDIB, HPALETTE hPal)
{
   LPSTR    lpDIBHdr, lpDIBBits;
   HBITMAP  hBitmap;
   HDC      hDC;
   HPALETTE hOldPal = NULL;

   if (!hDIB)
      return NULL;

   lpDIBHdr  = GlobalLock (hDIB);
   lpDIBBits = FindDIBBits (lpDIBHdr);
   hDC       = GetDC (NULL);

   if (!hDC)
      {
      GlobalUnlock (hDIB);
      return NULL;
      }

   if (hPal)
      hOldPal = SelectPalette (hDC, hPal, FALSE);

   RealizePalette (hDC);

   hBitmap = CreateDIBitmap (hDC,
                             (LPBITMAPINFOHEADER) lpDIBHdr,
                             CBM_INIT,
                             lpDIBBits,
                             (LPBITMAPINFO) lpDIBHdr,
                             DIB_RGB_COLORS);

   if (!hBitmap)
      DIBError (ERR_CREATEDDB);

   if (hOldPal)
      SelectPalette (hDC, hOldPal, FALSE);

   ReleaseDC (NULL, hDC);
   GlobalUnlock (hDIB);

   return hBitmap;
}


//---------------------------------------------------------------------
//
// 関数:       InitBitmapInfoHeader
//
// 目的:       DIBの幅、高さ、ピクセル当たりのビット数を指定すると、
//             BITMAPINFOHEADERの標準的な初期化を行います。
//
//             ここでいう標準とは、ある値を基にして、関連するメンバに
//             値を設定することです。biSizeImageを計算し、biCompression
//             メンバに「圧縮なし」を設定して、それ以外のフィールドは
//             すべて0にします。
//
//             DIBでは、BitsPixelには1、4、8、24しか設定できないことに
//             注意します。このルーチンでは、これらの値のいずれかが使わ
//             れるようにしています(指定されたnBPPに最も近い値を使用)。
//
// パラメータ: lpBmInfoHdr - 設定するBITMAPINFOHEADER構造体を指すfarポインタ
//             dwWidth     - DIBの幅(上位ワードはWindows 3.0および3.1では
//                           未使用のため0でなければならない)
//             dwHeight    - DIBの高さ(上位ワードはWindows 3.0および3.1では
//                           未使用のため0でなければならない)
//             nBPP        - DIBのピクセル当たりのビット数
//
//---------------------------------------------------------------------

void InitBitmapInfoHeader (LPBITMAPINFOHEADER lpBmInfoHdr,
                                        DWORD dwWidth,
                                        DWORD dwHeight,
                                          int nBPP)
{
   _fmemset (lpBmInfoHdr, 0, sizeof (BITMAPINFOHEADER));

   lpBmInfoHdr->biSize      = sizeof (BITMAPINFOHEADER);
   lpBmInfoHdr->biWidth     = dwWidth;
   lpBmInfoHdr->biHeight    = dwHeight;
   lpBmInfoHdr->biPlanes    = 1;

   if (nBPP <= 1)
      nBPP = 1;
   else if (nBPP <= 4)
      nBPP = 4;
   else if (nBPP <= 8)
      nBPP = 8;
   else
      nBPP = 24;

   lpBmInfoHdr->biBitCount  = nBPP;
   lpBmInfoHdr->biSizeImage = WIDTHBYTES (dwWidth * nBPP) * dwHeight;
}


//---------------------------------------------------------------------
//
// 関数:       BitmapToDIB
//
// 目的:       DDBとパレットを指定すると、DIBの仕様を含むグローバルメモ
//             リのハンドルを返します。DIBは、渡されたパレットの色を
//             使って作成されます。(ShowDIBからの流用)
//
// パラメータ: hBitmap - デフォルトのスクリーンディスプレイデバイスと
//                       互換性のあるDDBを識別するハンドル
//             hPal    - DIBを作成するときのパレット(NULLの場合は
//                       デフォルトのパレットを使用)
//
//---------------------------------------------------------------------

HANDLE BitmapToDIB (HBITMAP hBitmap, HPALETTE hPal)
{
   BITMAP             Bitmap;
   BITMAPINFOHEADER   bmInfoHdr;
   LPBITMAPINFOHEADER lpbmInfoHdr;
   LPSTR              lpBits;
   HDC                hMemDC;
   HANDLE             hDIB;
   HPALETTE           hOldPal = NULL;

   // 設定 - 渡されたビットマップが有効なものかどうかを確認し、
   // ビットマップの情報(高さ、幅など)を取得する。その後で、
   // BITMAPINFOHEADERに値を設定する
   if (!hBitmap)
      return NULL;

   if (!GetObject (hBitmap, sizeof (Bitmap), (LPSTR) &Bitmap))
      return NULL;

   InitBitmapInfoHeader (&bmInfoHdr,
                         Bitmap.bmWidth,
                         Bitmap.bmHeight,
                         Bitmap.bmPlanes * Bitmap.bmBitsPixel);

   // DIBのメモリを割り当て、そこにBITMAPINFOHEADERを設定し、
   // ビットマップビットを格納する位置を決定する
   hDIB = GlobalAlloc (GHND, sizeof (BITMAPINFOHEADER) +
             PaletteSize ((LPSTR) &bmInfoHdr) + bmInfoHdr.biSizeImage);

   if (!hDIB)
      return NULL;

   lpbmInfoHdr  = (LPBITMAPINFOHEADER) GlobalLock (hDIB);
   *lpbmInfoHdr = bmInfoHdr;
   lpBits       = FindDIBBits ((LPSTR) lpbmInfoHdr);

   // ビットマップを保持するDCが必要。パレットが渡されていれば、
   // そのパレットをDCに選択する
   hMemDC       = GetDC (NULL);

   if (hPal)
      {
      hOldPal = SelectPalette (hMemDC, hPal, FALSE);
      RealizePalette (hMemDC);
      }

   // DIBを取得する準備が完了。ドライバを呼び出し、ビットマップを
   // 取得する。ドライバはグローバルメモリブロックにカラーテーブルと
   // ビットマップビットを設定する
   if (!GetDIBits (hMemDC,
                   hBitmap,
                   0,
                   Bitmap.bmHeight,
                   lpBits,
                   (LPBITMAPINFO) lpbmInfoHdr,
                   DIB_RGB_COLORS))
      {
      GlobalUnlock (hDIB);
      GlobalFree (hDIB);
      hDIB = NULL;
      }
   else
      GlobalUnlock (hDIB);

   // 最後に後始末をして戻る
   if (hOldPal)
      SelectPalette (hMemDC, hOldPal, FALSE);

   ReleaseDC (NULL, hMemDC);

   return hDIB;
}
