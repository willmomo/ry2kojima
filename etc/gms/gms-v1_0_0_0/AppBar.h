#pragma once

#include <windows.h>
#include <assert.h>

#define ABE_FLOAT	4

class CAppBar
{
public:
	struct APPBARCTRL {
		HWND hwnd;			// AppBar にする window のハンドル
		UINT uEdge;			// 初期ポジション
		UINT uMsg;			// AppBar メッセージ ID
		SIZE m_sizeFloat;	// 非dock時の window の幅と高さ
		SIZE m_sizeDock;	// dock 時の window の幅と高さ
	};

private:
	APPBARDATA m_abd;
	SIZE m_sizeFloating;	// ABE_FLOAT時の幅と高さ
	SIZE m_sizeDocking;		// Dock時の幅/高さ
	POINT m_ptOffset;		// window drag時、左上のマウス座標からのオフセット
	bool m_fRegisterd;		// システムに登録済みのときはtrue、まだのときはfalse
	
	RECT m_rcTarget;		// window 移動時/後の矩形
	UINT m_uEdgeTarget;		// window 移動時/後のDock位置

public:
	UINT GetEdge() const {
		return m_abd.uEdge;
	};

public:

	CAppBar();				// コンストラクタ
	virtual ~CAppBar();		// デストラクタ

	void Attach(const APPBARCTRL* pabc);
	void Detach();

	void MoveWindow(const RECT* lprc, BOOL bRepaint=TRUE);	// 所属するWindowを移動する
	void QueryPos(UINT uNewEdge, RECT* lprc);				// 新しい端に適した矩形を計算する

	void Dock();							// 端に dock させる
	void Dock(UINT uEdge);					// 指定端に dock させる
	void Undock();							// システムから切り離す
	UINT PtToEdge(LPPOINT lppt);			// スクリーン座標から、dock すべき edge を返す

	void OnEnterSizeMove();					// WM_ENTERSIZEMOVE メッセージ処理
	void OnExitSizeMove();					// WM_EXITSIZEMOVE メッセージ処理
	void OnSizing(RECT* lprc);				// WM_SIZING メッセージ処理
	void OnMoving(RECT* lprc);				// WM_MOVING メッセージ処理
	UINT OnNCHitTest(WPARAM wParam, LPARAM lParam);	// WM_NCHITTEST メッセージ処理
	void OnWindowPosChanged();				// WM_WINDOWPOSCHANGED メッセージ処理
	void OnActivate();						// WM_ACTIVATE メッセージ処理
	void OnAppBarMessage(WPARAM wParam);	// AppBar 通知メッセージ処理
};
