#pragma once


#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <sstream>


//---------------------------------------------------------------------
// �֐��̐錾
//---------------------------------------------------------------------
std::string kjm_GetDlgItemText(HWND hDlg, int nIDDlgItem);
std::string kjm_FormatMessageBySystem(DWORD dwError);


//---------------------------------------------------------------------
// �֐�����API�Ŗ�肪�N�������ꍇthrow�����O�̌^
//---------------------------------------------------------------------
class kjm_win32runtime_error : public std::runtime_error {
public:
	DWORD m_errCode;

	kjm_win32runtime_error(DWORD errCode) : std::runtime_error(kjm_FormatMessageBySystem(errCode)), m_errCode(errCode) {};
};
