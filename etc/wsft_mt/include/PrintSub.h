//------------------------------------------------------------
//�T�v
//   ����֌W���ʃT�u���[�`��
//
//�쐬��
//   Tokai���J
//
//���l
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
extern BOOL IsInjiAllStop();			// ��2003/7/16 (�d��ON�����󎚑Ή�)�ǉ� Fukuyama
#endif