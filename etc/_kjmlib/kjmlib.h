#pragma once


#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <sstream>


//---------------------------------------------------------------------
// 関数の宣言
//---------------------------------------------------------------------
std::string kjm_GetDlgItemText(HWND hDlg, int nIDDlgItem);
std::string kjm_FormatMessageBySystem(DWORD dwError);


//---------------------------------------------------------------------
// 関数内のAPIで問題が起こった場合throwする例外の型
//---------------------------------------------------------------------
class kjm_win32runtime_error : public std::runtime_error {
public:
	DWORD m_errCode;

	kjm_win32runtime_error(DWORD errCode) : std::runtime_error(kjm_FormatMessageBySystem(errCode)), m_errCode(errCode) {};
};
