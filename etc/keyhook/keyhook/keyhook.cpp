// keyhook.cpp : DLL アプリケーション用のエントリ ポイントを定義します。
//

#define STRICT
#include <windows.h>
#include "keyhook.h"

// ここが肝！
// どうにも、よくわからんのだが、g_hwndTgtをshareにしておかないと、メッセージが届かない
// しかし、こうするとこのDLLを起動して後にフックしたプログラムしか有効にならん・・・
//
// KeyboardProcがそのときアクティブなメモリ空間で呼ばれるとしたら、可能性もあるのだが、
// そしたら、その中で使用しているグローバル変数全てを、shareにしないといけないと思う、
// しかし実際は、g_hwndTgtだけshareすれば動く・・・なぞ。誰か教えて？
//
// 対処法としては、g_hhkもshareして、二つめ移行のプロセスからのフックを失敗させるか
// g_hwndTgtを複数もてるように改良するか・・・
// 今回は、sampleなのでg_hhkをshareして、一人目のプロセスのみフック可能とした。

#pragma data_seg(".sharedata")
HWND g_hwndTgt = NULL;	// メッセージ送信先ウィンドウ
HHOOK g_hhk = NULL;
#pragma data_seg()

HINSTANCE g_hinstDLL;
UINT WM_KEYHOOK;

//---------------------------------------------------------------------
// 概要
//	キーボードフックプロシージャ
//
// 引数
//	マニュアル参照で
//
// 戻り値
//	常に、CallNextHookExの戻り値
//---------------------------------------------------------------------

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code >= 0) {
		if (code == HC_ACTION) {
			BYTE keyState[256];
			GetKeyboardState(keyState);

//			lParam = 
//				((keyState[VK_CONTROL] & 0x80) ? 0x00ff0000 : 0x00cc0000) |
//				((keyState[VK_MENU] & 0x80) ? 0x0000ff00 : 0x0000cc00) |
//				((keyState['P'] & 0x80) ? 0x000000ff : 0x000000cc);

			PostMessage(g_hwndTgt, WM_KEYHOOK, wParam, lParam);
		}
	}

	return CallNextHookEx(g_hhk, code, wParam, lParam);
}

//---------------------------------------------------------------------
// 概要
//	キーボードフックの開始
//
// 引数
//	hwnd	hookしたメッセージの送信先
//
// 戻り値
//	TRUE:	正常終了
//	FALSE:	異常終了
//---------------------------------------------------------------------

BOOL KEYHOOK_API SetHook(HWND hwnd)
{
	BOOL ret = FALSE;

	// 未hookのときだけhook
	if (!g_hhk) {
		g_hwndTgt = hwnd;
		g_hhk = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_hinstDLL, 0);
		if (g_hhk) {
			ret = TRUE;
		}
	}

	return ret;
}

//---------------------------------------------------------------------
// 概要
//	キーボードフックの終了
//
// 引数
//	なし
//
// 戻り値
//	なし
//---------------------------------------------------------------------

void KEYHOOK_API FreeHook()
{
	// hook済みの時だけunhook
	if (g_hhk) {
		UnhookWindowsHookEx(g_hhk);
		g_hhk = NULL;
	}
}

//---------------------------------------------------------------------
// 引数
//	hinstDLL	DLLのハンドル
//	fdwReason	DLLの呼び出し理由
//	lpvReserved	reservedだってば!
//
// 戻り値
//	常にTRUE
//---------------------------------------------------------------------

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID plvReserved)
{
	// 初回呼び出しで、ハンドルの保存と共有するメッセージを登録
	if (fdwReason == DLL_PROCESS_ATTACH) {
		g_hinstDLL = hinstDLL;
		WM_KEYHOOK = RegisterWindowMessage(KEYHOOK_MESSAGE);
	}

	return TRUE;
}
