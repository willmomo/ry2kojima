#ifndef __TTY_H
#define __TTY_H

////////////////////////////////////////////////////////////////////////////
//	ヘッダ

#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <ctype.h>

extern HINSTANCE tty_m_hinst;
extern HWND tty_m_hwnd;
extern BOOL tty_m_fConnect;			// 接続中フラグ TRUE:オンライン FALSE:オフライン
extern int tty_m_idComDev;				// COMデバイスの識別子
extern BOOL tty_m_fLocalEcho;			// ローカル エコーの有無 TRUE:有り FALSE:無し

// プロトタイプ宣言
void XTTY_XTTY(HINSTANCE hinst, HWND hwnd);	// コンストラクタ
void XTTY_deXTTY();				// デストラクタ

int XTTY_Connect(BOOL fConnect);		// 接続
void XTTY_OpenComm_Error(int errCode);	// OpenCommのエラーメッセージ
void XTTY_ScrollScreen();			// スクリーンバッファのスクロール
void XTTY_CommErrorMessage();		// GetCommErrorを使用してのエラーメッセージ ダイアログ
void XTTY_WriteLog(const char * buf, int n);	// ログバッファへの文字列書き込み
int XTTY_WriteComm2(const char * buf, int n);	// WriteLogに対応したWriteComm関数
BOOL XTTY_ReadDCB(DCB *pdcb);
BOOL XTTY_ReadCom(char *pcom);
BOOL XTTY_WriteDCB(DCB *pdcb);
BOOL XTTY_WriteCom(char *pcom);

void XTTY_OnChar(WPARAM wParam, LPARAM lParam);		// WM_CHARメッセージ処理
void XTTY_OnCommNotify(WPARAM wParam, LPARAM lParam);	// WM_COMMNOTIFYメッセージ処理
void XTTY_OnPaint(PAINTSTRUCT *ps);						// WM_PAINTメッセージ処理


#endif
