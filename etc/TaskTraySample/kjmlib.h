//-----------------------------------------------------------------------------
// 2012/03/14 r.kojima
//-----------------------------------------------------------------------------
#pragma once

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

namespace kjm {
	//-------------------------------------------------------------------------
	// タスクトレイアイコンを扱うクラス
	//-------------------------------------------------------------------------
	class taskTrayIcon {
	public:
		// コンストラクタ
		taskTrayIcon() { memset(&m_nid, 0, sizeof(m_nid)); };

		// デストラクタ
		virtual ~taskTrayIcon() { deleteIcon(); };

		// タスクトレイにアイコンを追加する
		BOOL addIcon(HWND hwnd, UINT uID, HICON hIcon, UINT uCallback, LPCTSTR pszTip);

		// タスクトレイからアイコンを取り除く
		BOOL deleteIcon();

	private:
		// コピー・代入は禁止
		taskTrayIcon(const taskTrayIcon& src) {};
		taskTrayIcon& operator =(const taskTrayIcon& src) { return *this; };

	private:
		NOTIFYICONDATA m_nid;
	};
}
