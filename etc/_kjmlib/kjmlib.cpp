#include "kjmlib.h"


//---------------------------------------------------------------------
// [�T�v]
// �V�X�e���W���̃G���[���b�Z�[�W���쐬
//---------------------------------------------------------------------
std::string kjm_FormatMessageBySystem(DWORD dwError) {

	std::string ret;
	PVOID pBuffer;

	DWORD dwRet = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, (char*)&pBuffer, 8, NULL);
	if (dwRet == 0) {
		std::stringstream ss;
		ss << "FormatMessage(" << dwError << ") failed: error " << GetLastError();
		ret = ss.str();
	} else {
		ret = (char*)pBuffer;
		LocalFree(pBuffer);
	}

	return ret;
}


//---------------------------------------------------------------------
// [�T�v]
// std::string��Ԃ�GetDlgItemText
//
// [��O]
// win32runtime_error�𓊂���
//---------------------------------------------------------------------
std::string kjm_GetDlgItemText(HWND hDlg, int nIDDlgItem) {

	HWND hwndCtrl = GetDlgItem(hDlg, nIDDlgItem);
	if (hwndCtrl == NULL) {
		throw kjm_win32runtime_error(GetLastError());
	}

	int nLen = GetWindowTextLength(hwndCtrl);
	if (nLen == 0 && GetLastError() != ERROR_SUCCESS) {
		throw kjm_win32runtime_error(GetLastError());
	}

	std::vector<TCHAR> work(nLen + 1);
	if (GetWindowText(hwndCtrl, work.begin(), work.size()) == 0 && GetLastError() != ERROR_SUCCESS) {
		throw kjm_win32runtime_error(GetLastError());
	}

	return work.begin();
}
