// kmwatch.cpp : DLL アプリケーション用のエントリ ポイントを定義します。
//

#define _WIN32_WINNT	0x0400

#define STRICT
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "kmwatch.h"

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
HHOOK g_hhkKeybd = NULL;
HHOOK g_hhkMouse = NULL;
#pragma data_seg()

HINSTANCE g_hinstDLL;
UINT WM_KMWATCH;
UINT WM_KMWATCH_MOUSE;

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

LRESULT CALLBACK MouseProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code >= 0) {
		if (code == HC_ACTION) {
			switch (wParam) {
			case WM_NCMOUSEMOVE:	// 0x00A0
			case WM_MOUSEMOVE:		// 0x0200
				// マウスの移動は無視します。
				break;

			default:
				// 引っかかることが確認できているマウスイベント
				{
					MSLLHOOKSTRUCT* p = (MSLLHOOKSTRUCT*)lParam;

					FILE* fp = fopen("mouse.log", "a+");
					if (fp) {
						fprintf(fp, "[%04X] wParam = 0x%08X {pt = %u, mouseData = %u, flags = %08X, time = %u, dwExtraInfo = %u}\n",
							GetCurrentProcessId(),
							wParam,
							p->pt, p->mouseData, p->flags, p->time, p->dwExtraInfo);
						fclose(fp);
					}
				}
				
				PostMessage(g_hwndTgt, WM_KMWATCH_MOUSE, wParam, lParam);
				break;
			}
		}
	}

	return CallNextHookEx(g_hhkMouse, code, wParam, lParam);
}

const char* getEventName(WPARAM wParam) {
	static char s_msg[80];

	switch (wParam) {
	case WM_LBUTTONDOWN:
		lstrcpy(s_msg, _T("WM_LBUTTONDOWN"));
		break;
	
	case WM_LBUTTONUP:
		lstrcpy(s_msg, _T("WM_LBUTTONUP"));
		break;

	case WM_RBUTTONDOWN:
		lstrcpy(s_msg, _T("WM_RBUTTONDOWN"));
		break;

	case WM_RBUTTONUP:
		lstrcpy(s_msg, _T("WM_RBUTTONUP"));
		break;

	case WM_MOUSEWHEEL:
		lstrcpy(s_msg, _T("WM_MOUSEWHEEL"));
		break;

	default:
		wsprintf(s_msg, _T("0x%04X"), wParam);
		break;
	}

	return s_msg;

}

LRESULT CALLBACK MouseProcLL(int code, WPARAM wParam, LPARAM lParam)
{
	if (code >= 0) {
		if (code == HC_ACTION) {
			switch (wParam) {
			case WM_NCMOUSEMOVE:	// 0x00A0
			case WM_MOUSEMOVE:		// 0x0200
			case WM_MOUSEWHEEL:		// 0x020A
				// マウスの移動は無視します。
				break;

			default:
				// 引っかかることが確認できているマウスイベント
				{
					MSLLHOOKSTRUCT* p = (MSLLHOOKSTRUCT*)lParam;

					FILE* fp = fopen("mouse.log", "a+");
					if (fp) {
						fprintf(fp, "[%4d] wParam(%-14s) {pt(%4d,%4d), mouseData(%u), flags(%08X), time(%u), dwExtraInfo(%u)}\n",
							GetCurrentProcessId(),
							getEventName(wParam),
							wParam,
							p->pt.x, p->pt.y, p->mouseData, p->flags, p->time, p->dwExtraInfo);
						fclose(fp);
					}
				}
				
				PostMessage(g_hwndTgt, WM_KMWATCH_MOUSE, wParam, lParam);
				break;
			}
		}
	}

	return CallNextHookEx(g_hhkMouse, code, wParam, lParam);
}

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
			PostMessage(g_hwndTgt, WM_KMWATCH, wParam, lParam);
		}
	}

	return CallNextHookEx(g_hhkKeybd, code, wParam, lParam);
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

BOOL KMWATCH_API SetHook(HWND hwnd)
{
	BOOL ret = FALSE;

	// 未hookのときだけhook
	if (!g_hhkKeybd) {
		g_hwndTgt = hwnd;
		g_hhkKeybd = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_hinstDLL, 0);
		///g_hhkMouse = SetWindowsHookEx(WH_MOUSE, MouseProc, g_hinstDLL, 0);
		g_hhkMouse = SetWindowsHookEx(WH_MOUSE_LL, MouseProcLL, g_hinstDLL, 0);
		if (g_hhkKeybd && g_hhkMouse) {
			ret = TRUE;
		} else {
			FreeHook();
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

void KMWATCH_API FreeHook()
{
	// hook済みの時だけunhook
	if (g_hhkKeybd) {
		UnhookWindowsHookEx(g_hhkKeybd);
		g_hhkKeybd = NULL;
	}

	if (g_hhkMouse) {
		UnhookWindowsHookEx(g_hhkMouse);
		g_hhkMouse = NULL;
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
		WM_KMWATCH = RegisterWindowMessage(KMWATCH_MESSAGE);
		WM_KMWATCH_MOUSE = RegisterWindowMessage(KMWATCH_MOUSE_MESSAGE);
	}

	return TRUE;
}
