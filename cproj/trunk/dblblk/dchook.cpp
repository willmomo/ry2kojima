///////////////////////////////////////////////////////////////////////
// dchook.cpp

#include <windows.h>
#include "dchook.h"

//////// グローバル変数 /////////

HINSTANCE g_hinst = NULL;
HHOOK g_hhk = NULL;

///////////////////////////////////////////////////////////////////////
// マウスフックプロシージャ

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		LRESULT ret = CallNextHookEx(g_hhk, nCode, wParam, lParam);

		// WM_LBUTTONDBLCLK だけは、無条件に処理したことにする
		// これで、アプリケーションにダブルクリックを処理させない

		return (wParam == WM_LBUTTONDBLCLK) ? 1 : ret;
	}

	return CallNextHookEx(g_hhk, nCode, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////
// マウスフック開始

DCHOOK_API void DcHook()
{
	if (g_hhk == NULL)
	{
		g_hhk = SetWindowsHookEx(WH_MOUSE, MouseProc, g_hinst, 0);
	}
}

///////////////////////////////////////////////////////////////////////
// マウスフック終了

DCHOOK_API void DcUnhook()
{
	if (g_hhk)
	{
		UnhookWindowsHookEx(g_hhk);
		g_hhk = NULL;
	}
}

///////////////////////////////////////////////////////////////////////
// エントリーポイント

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		g_hinst = hinstDLL;
	}

	return TRUE;
}
