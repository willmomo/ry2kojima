#ifndef __TTY_H
#define __TTY_H

////////////////////////////////////////////////////////////////////////////
//	ヘッダ

#include <windows.h>
#include <windowsx.h>
// #include <string.h>
// #include <process.h>

////////////////////////////////////////////////////////////////////////////
//	TTYクラス

#define CN_EVENT	0x01
#define CN_RECEIVE	0x02

class XTTY
{
public:
	OVERLAPPED  m_os ;
	BOOL m_fContinue;
	DWORD m_arg[3];
	HANDLE m_thread;
	DWORD m_threadID;
	HINSTANCE m_hinst;
	HWND m_hwnd;
	BOOL m_fConnect;			// 接続中フラグ TRUE:オンライン FALSE:オフライン
	HANDLE m_idComDev;			// COMデバイスの識別子
	int m_xPos, m_yPos;			// カーソルのX,Y座標
	BYTE m_abScreen[80 * 25];		// 受信データ保存バッファ
	LOGFONT m_lf;				// TTYが使用するフォント情報
	HFONT m_hfont;				// TTYが使用するフォント
	BOOL m_fLocalEcho;			// ローカル エコーの有無 TRUE:有り FALSE:無し

	XTTY(HINSTANCE hinst, HWND hwnd);	// コンストラクタ
	~XTTY();				// デストラクタ

	int Connect(BOOL fConnect);		// 接続
	void OpenComm_Error();			// OpenCommのエラーメッセージ
	void ScrollScreen();			// スクリーンバッファのスクロール
	void CommErrorMessage();		// GetCommErrorを使用してのエラーメッセージ ダイアログ
	void WriteLog(const char * buf, int n);	// ログバッファへの文字列書き込み
	int WriteComm2(const char * buf, int n);	// WriteLogに対応したWriteComm関数
	friend DWORD WINAPI CommEvent(LPDWORD arg);		// WaitCommEventのためのスレッド

	void OnChar(WPARAM wParam, LPARAM lParam);		// WM_CHARメッセージ処理
	void OnCommNotify(WPARAM wParam, LPARAM lParam);	// WM_COMMNOTIFYメッセージ処理
	void OnPaint();						// WM_PAINTメッセージ処理
};

#endif
