//------------------------------------------------------------
//Tv
//   óüÖW¤ÊTu[`
//
//ì¬Ò
//   TokaiJ
//
//õl
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
extern BOOL IsInjiAllStop();			// ©2003/7/16 (d¹ON©®óÎ)ÇÁ Fukuyama
#endif