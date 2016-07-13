#if !defined(__SYSUTILS_H__)
#define __SYSUTILS_H__

// 必要なヘッダをインクルード

#include <string>

// 半透明機能をウィンドウに追加するときの定数

#define WS_EX_LAYERED	0x80000

// プロトタイプ宣言

bool IsLayered(HWND hwnd);
int RemoveSubFolders(const char *strFolder, const char *strExcept);
int SetLayered(HWND hWnd, bool bLayered);
int SetOpacity(HWND hwnd, double dOpacity);
std::string ReplaceStr(const char* pszSrc, const char* pszFind, const char* pszReplaceWith, int nStart = 1, int nCount = -1);
std::wstring StrToWStr(LPCSTR pszSrc);
std::string WStrToStr(LPCWSTR pwszSrc);

#endif
