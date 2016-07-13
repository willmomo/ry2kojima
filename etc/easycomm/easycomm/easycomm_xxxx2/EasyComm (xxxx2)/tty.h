#ifndef __TTY_H
#define __TTY_H

////////////////////////////////////////////////////////////////////////////
//	ヘッダ

#include <windows.h>
#include <windowsx.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////
//	TTYクラス

class XTTY
{
public:
	HINSTANCE m_hinst;
	HWND m_hwnd;
	BOOL m_fConnect;			// 接続中フラグ TRUE:オンライン FALSE:オフライン
	int m_idComDev;				// COMデバイスの識別子
	int m_xPos, m_yPos;			// カーソルのX,Y座標
	BYTE m_abScreen[80 * 25];		// 受信データ保存バッファ
	LOGFONT m_lf;				// TTYが使用するフォント情報
	HFONT m_hfont;				// TTYが使用するフォント
	BOOL m_fLocalEcho;			// ローカル エコーの有無 TRUE:有り FALSE:無し

	XTTY(HINSTANCE hinst, HWND hwnd);	// コンストラクタ
	~XTTY();				// デストラクタ

	int Connect(BOOL fConnect);		// 接続
	void OpenComm_Error(int errCode);	// OpenCommのエラーメッセージ
	void ScrollScreen();			// スクリーンバッファのスクロール
	void CommErrorMessage();		// GetCommErrorを使用してのエラーメッセージ ダイアログ
	void WriteLog(const char * buf, int n);	// ログバッファへの文字列書き込み
	int WriteComm2(const char * buf, int n);	// WriteLogに対応したWriteComm関数

	void OnChar(WPARAM wParam, LPARAM lParam);		// WM_CHARメッセージ処理
	void OnCommNotify(WPARAM wParam, LPARAM lParam);	// WM_COMMNOTIFYメッセージ処理
	void OnPaint();						// WM_PAINTメッセージ処理
};

#endif
