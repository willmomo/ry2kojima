// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します


// Windows ヘッダー ファイル:
#include <windows.h>
#include <windowsx.h>
#include <shlwapi.h>

// C ランタイム ヘッダー ファイル
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <sstream>
#include <vector>

// ローカル ヘッダー ファイル
#include "../wssasup/wssasup.h"

#pragma comment( lib, "shlwapi.lib" )

#if defined(_DEBUG)
#pragma comment(lib, "../libd/wssasup.lib")
#else
#pragma comment(lib, "../lib/wssasup.lib")
#endif

#if !defined( countof )
#define countof(A)	(sizeof((A)) / sizeof(*(A)))
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
