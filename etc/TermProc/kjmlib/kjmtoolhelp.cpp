// class kjm::toolhelp‚ÌŽÀ‘•

#include "kjmlib.h"


bool kjm::toolhelp::createSnapProcess() {
	if (m_pLog)	m_pLog->writeLog(_T(">> kjm::toolhelp::createSnapProcess()"));
	bool bRet = false;

	HANDLE hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 pe;
		ZeroMemory(&pe, sizeof(pe));
		pe.dwSize = sizeof(pe);

		BOOL ret = Process32First(hSnap, &pe);
		while (ret) {
			m_peList.push_back(pe);
			ret = Process32Next(hSnap, &pe);
		}

		CloseHandle(hSnap);
		bRet = true;
	} else {
		if (m_pLog)	m_pLog->writeLog(_T("   kjm::toolhelp::createSnapProcess(): CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) error: %u."), GetLastError());
	}

	if (m_pLog)	m_pLog->writeLog(_T("<< kjm::toolhelp::createSnapProcess() bool(%d)"), bRet);
	return bRet;
}
