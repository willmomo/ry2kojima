#if !defined(__HSUTIL_H)
#define __HSUTIL_H

// ライブラリに必要なヘッダ

#include <windows.h>
#include <tchar.h>

// export用マクロ(def fileに書いてあるから不要といえば不要)

#ifdef HSUTIL_EXPORTS
#define HSUTIL_API __declspec(dllexport)
#else
#define HSUTIL_API __declspec(dllimport)
#endif

// HsFindWindow 用定数

#define HSFW_FLAG_SAMEFINDWINDOW	0x00	// FindWindowと同じ動作にする
#define HSFW_FLAG_MATCHHEAD			0x01	// 文字列を先頭からの部分一致で判定

HSUTIL_API HWND WINAPI HsFindWindow(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwFlags);

#endif
