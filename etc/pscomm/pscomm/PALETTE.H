
#ifndef PALETTE_INCLUDED
#define PALETTE_INCLUDED

/* PALETTE.H */

// ウィンドウ拡張バイトのサイズ(PALINFO構造体のハンドルが格納される)
#define PAL_CBWNDEXTRA  (1 * sizeof (WORD))

typedef struct
   {
   HPALETTE hPal;                      // 表示されているパレットを識別する
                                       // ハンドル
   WORD     wEntries;                  // パレットエントリの個数
   int      nSquareSize;               // パレットセルのサイズ(PAL_SIZEを参照)
   HWND     hInfoWnd;                  // 情報バーウィンドウのハンドル
   int      nRows, nCols;              // ウィンドウのサイズ
   int      cxSquare, cySquare;        // パレットセルの幅と高さ(ピクセル単位)
   WORD     wEntry;                    // 現在選択されているパレットセル
   } PALINFO, FAR *LPPALINFO;


// ウィンドウワード
#define WW_PAL_HPALINFO 0              // PALINFO構造体を識別するハンドル

// 次の定義はCopyPaletteChangingFlags()のためのもの
#define DONT_CHANGE_FLAGS -1

// LOGPALETTEのpalVersionメンバに設定されるパレットバージョン
#define PALVERSION   0x300

// PaletteWndProc()でパレットを表示する方法を示す列挙型リスト
enum PAL_SIZE
   {
   PALSIZE_TINY = 0,
   PALSIZE_SMALL,
   PALSIZE_MEDIUM,
   PALSIZE_LARGE
   };


// Pallette Windowsのメニュー定義
//  注: TINYからFITWNDまで連続した数値でなければならない。数値の関係を変更
//      した場合は、PALETTE.CのWM_INITMENUPOPUPメッセージとWM_COMMANDメッ
//      セージの処理も変更しなければならない

#define IDM_PAL_TINY        1000       // 最小のパレットセル
#define IDM_PAL_SMALL       1001       // 小さなパレットセル
#define IDM_PAL_MEDIUM      1002       // 中位のパレットセル
#define IDM_PAL_LARGE       1003       // 大きなパレットセル


// DIBVIEW.RCの文字列テーブルの文字列の定義
#define IDS_PAL_RGB         2048       // PALETTEENTRY.peFlags=0の文字列
#define IDS_PAL_RESERVED    2049       // PALETTEENTRY.peFlags=1の文字列
#define IDS_PAL_EXPLICIT    2050       // PALETTEENTRY.peFlags=2の文字列
#define IDS_PAL_ERROR       2051       // PALETTEENTRY.peFlags=3の文字列
#define IDS_PAL_NOCOLLAPSE  2052       // PALETTEENTRY.peFlags=4の文字列

#define IDS_PAL_NOPAL       2053       // パレットが渡されていないときの文字列
#define IDS_PAL_DISPRGB     2054       // ステータス行の書式文字列


// 関数プロトタイプ
long FAR PASCAL __export PaletteWndProc (HWND hwnd,
				UINT message,
				WPARAM wParam,
				LPARAM lParam);

void SetPaletteWindowsPal (HWND hWnd, HPALETTE hPal);

HPALETTE GetSystemPalette (void);

HPALETTE CopyPaletteChangingFlags (HPALETTE hPal, BYTE bNewFlag);

void MyAnimatePalette (HWND hWnd, HPALETTE hPal);

int ColorsInPalette (HPALETTE hPal);

#define CopyPalette(hPal)  CopyPaletteChangingFlags (hPal,(BYTE) DONT_CHANGE_FLAGS)

#define CopyPalForAnimation(hPal) CopyPaletteChangingFlags (hPal, PC_RESERVED)

#endif // PALETTE_INCLUDED
