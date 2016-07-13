//---------------------------------------------------------------------------
// AppBar を扱うためのクラス。
// kjm 名前空間だが、kjmlibとは、わけて存在。
//
// ver.0.2012.08.20
//---------------------------------------------------------------------------
#pragma once

#include <windows.h>
#include <tchar.h>
#include <shellapi.h>

namespace kjm {

	class appBar {
	public:
		appBar() : m_fRegistered(FALSE), m_hwndZOrder(NULL) { memset(&m_abd, 0, sizeof(m_abd)); }

		virtual ~appBar() {}

		BOOL isRegistered();

		BOOL regist(HWND hwnd, UINT uCallbackMessage);

		void remove();

		void setPos(int uEdge, SIZE size);

		void callbackMessage(WPARAM wParam, LPARAM lParam);

	private:
		RECT queryPos(int uEdge, SIZE size);

	private:
		BOOL m_fRegistered;
		APPBARDATA m_abd;
		HWND m_hwndZOrder;
	};
};
