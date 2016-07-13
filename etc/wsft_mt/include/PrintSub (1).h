//------------------------------------------------------------
//概要
//   印刷関係共通サブルーチン
//
//作成者
//   Tokai水谷
//
//備考
//
//------------------------------------------------------------
#if !defined(_PRINT_SUB_H)
#define _PRINT_SUB_H

//------------------------------------------------------------
//
//------------------------------------------------------------
extern BOOL GetLogFont(HDC hdc, LOGFONT *logFont);
extern BOOL SelectPrintFont(HDC hdc, HFONT *hBaseFont, HFONT *hPrtFont);
extern BOOL SelectBaseFont(HDC hdc, HFONT *hBaseFont, HFONT *hPrtFont);
extern BOOL IsInjiStop();
extern BOOL IsInjiAllStop();			// ←2003/7/16 (電源ON自動印字対応)追加 Fukuyama
#endif