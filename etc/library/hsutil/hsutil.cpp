#include "hsutil.h"

// HsFindWindowEnumProcに渡すパラメータ用構造体

typedef struct {
	LPCTSTR lpClassName;	// HsFindWindowに渡された引数
	LPCTSTR lpWindowName;	//  .. 同上 ..
	DWORD dwFlags;			//  .. 同上 ..
	HWND hwnd;				// マッチしたウィンドウへのハンドル
} HSFW_ENUM_PARAM, *PHSFW_ENUM_PARAM;

#define MAX_CLASS_NAME	2048
#define MAX_WINDOW_NAME	2048

//---------------------------------------------------------------------
// [概要]
// HsFindWindowから呼び出される列挙関数
//
// [説明]
// パラメータで指定されたフラグに従って、文字列を比較。
// 文字列を不一致と判断したときは、列挙を継続。
//---------------------------------------------------------------------

static BOOL CALLBACK HsFindWindowEnumProc(HWND hwnd, LPARAM lParam)
{
	PHSFW_ENUM_PARAM pParam = (PHSFW_ENUM_PARAM)lParam;

	// クラス名が指定されているときは、フラグに従って比較
	if (pParam->lpClassName) {
		TCHAR szClassName[MAX_CLASS_NAME];
		GetClassName(hwnd, szClassName, MAX_CLASS_NAME);

		// 将来いろいろな比較方法に対応(するかも)
		if (pParam->dwFlags == HSFW_FLAG_MATCHHEAD) {
			if (_tcsnicmp(pParam->lpClassName, szClassName, _tcslen(pParam->lpClassName)) != 0) {
				return TRUE;
			}
		}
	}

	// ウィンドウ名が指定されているときは、フラグに従って比較
	if (pParam->lpWindowName) {
		TCHAR szWindowName[MAX_WINDOW_NAME];
		GetWindowText(hwnd, szWindowName, MAX_WINDOW_NAME);

		// 将来いろいろな比較方法に対応(するかも)
		if (pParam->dwFlags == HSFW_FLAG_MATCHHEAD) {
			if (_tcsnicmp(pParam->lpWindowName, szWindowName, _tcslen(pParam->lpWindowName)) != 0) {
				return TRUE;
			}
		}
	}

	pParam->hwnd = hwnd;	// 一致したウィンドウハンドルを戻す

	return FALSE;
}

//---------------------------------------------------------------------
// [概要]
// 文字列の部分マッチ対応のFindWindow
//
// [引数]
// lpClassName	: FindWindow参照
// lpWindowName	: FindWindow参照
// dwFlags		: HSFW_FLAG_SAMEFINDWINDOW	: 完全一致で検索
//				: HSFW_FLAG_MATCHHEAD		: 先頭部分一致で検索
//
// [戻り値]
// ウィンドウが見つかったときは、ウィンドウハンドルを戻す。
// ウィンドウが見つからなかったときは、NULLを戻す。
//
// [説明]
// FindWindow関数が文字列の完全一致(ただし、ignore-case)なので、
// 動的に変化するウィンドウを探すために先頭部分一致に拡張。
//---------------------------------------------------------------------

HSUTIL_API HWND WINAPI HsFindWindow(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwFlags)
{
	// フラグが指定されないときは、FindWindowと同じ
	if (dwFlags == 0) {
		return FindWindow(lpClassName, lpWindowName);
	}

	// フラグが指定されているときは、EnumWindowを使って一致するウィンドウを探す
	HSFW_ENUM_PARAM param = {lpClassName, lpWindowName, dwFlags, NULL};
	EnumWindows(HsFindWindowEnumProc, (LPARAM)&param);

	return param.hwnd;
}

//---------------------------------------------------------------------
// [概要]
// DLL entry point
//---------------------------------------------------------------------

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	return TRUE;
}
