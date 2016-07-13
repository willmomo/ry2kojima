#include "kjm_tool.h"


// 行末の改行を取り除く

kjm::_tstring kjm::tool::chomp(const kjm::_tstring& str) {
	while (!s.empty() && (*(s.rbegin()) == '\n' || *(s.rbegin()) == '\r')) {
		s.erase(s.end() - 1);
	}
	return s;
}

// エラーコードでメッセージの作成

kjm::_tstring kjm::tool::formatMessage(DWORD dwError) {
	kjm::_tstring ret;
	PVOID pBuffer;

	DWORD dwRet = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErr, 0, (LPTSTR)&pBuffer, 8, NULL);
	if (dwRet == 0) {
		kjm::_tstringstream ss;
		ss << _T("FormatMessage(") << dwErr << _T(") failed: error ") << dwErr;
		ret = ss.str();
	} else {
		ret = (LPTSTR)pBuffer;
		LocalFree(pBuffer);
	}

	return kjm::tool::chomp(ret);
}
