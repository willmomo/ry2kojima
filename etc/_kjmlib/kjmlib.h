#pragma once


#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <sstream>


//---------------------------------------------------------------------
// ŠÖ”‚ÌéŒ¾
//---------------------------------------------------------------------
std::string kjm_GetDlgItemText(HWND hDlg, int nIDDlgItem);
std::string kjm_FormatMessageBySystem(DWORD dwError);


//---------------------------------------------------------------------
// ŠÖ”“à‚ÌAPI‚Å–â‘è‚ª‹N‚±‚Á‚½ê‡throw‚·‚é—áŠO‚ÌŒ^
//---------------------------------------------------------------------
class kjm_win32runtime_error : public std::runtime_error {
public:
	DWORD m_errCode;

	kjm_win32runtime_error(DWORD errCode) : std::runtime_error(kjm_FormatMessageBySystem(errCode)), m_errCode(errCode) {};
};
