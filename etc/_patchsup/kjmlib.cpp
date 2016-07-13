
#include "kjmlib.h"
#include <vector>


BOOL kjm_UrlComponents::CrackUrl(LPCSTR lpszUrl) {

	char bufScheme[1024];
	char bufHostName[1024];
	char bufUserName[1024];
	char bufPassword[1024];
	char bufUrlPath[1024];
	char bufExtraInfo[1024];

	URL_COMPONENTS uc;

	ZeroMemory(&uc, sizeof(uc));

	uc.dwStructSize = sizeof(uc);
	uc.lpszScheme = bufScheme;
	uc.dwSchemeLength = sizeof(bufScheme);
	uc.lpszHostName = bufHostName;
	uc.dwHostNameLength = sizeof(bufHostName);
	uc.lpszUserName = bufUserName;
	uc.dwUserNameLength = sizeof(bufUserName);
	uc.lpszPassword = bufPassword;
	uc.dwPasswordLength = sizeof(bufPassword);
	uc.lpszUrlPath = bufUrlPath;
	uc.dwUrlPathLength = sizeof(bufUrlPath);
	uc.lpszExtraInfo = bufExtraInfo;
	uc.dwExtraInfoLength = sizeof(bufExtraInfo);

	BOOL ret = InternetCrackUrl(lpszUrl, 0, 0, &uc);
	if (ret) {
		m_szScheme = uc.lpszScheme;
		m_nScheme = uc.nScheme;
		m_szHostName = uc.lpszHostName;
		m_nPort = uc.nPort;
		m_szUserName = uc.lpszUserName;
		m_szPassword = uc.lpszPassword;
		m_szUrlPath = uc.lpszUrlPath;
		m_szExtraInfo = uc.lpszExtraInfo;
	}

	return ret;
}


std::string kjm_ExpandEnvironmentStrings(LPCTSTR lpSrc) {

	// 変換後に必要なバッファサイズを取得
	DWORD dwNeeded = ::ExpandEnvironmentStrings(lpSrc, NULL, 0);
	if (dwNeeded == 0) {
		return std::string();
	}

	// バッファを用意して、再呼び出し
	std::vector<TCHAR> dst(dwNeeded);
	DWORD dwRet = ::ExpandEnvironmentStrings(lpSrc, dst.begin(), dst.size());
	if (dwRet == 0 || dwRet > dwNeeded) {
		return std::string();
	}

	// 変換後の文字列をstringにして返す
	return std::string(dst.begin());
}


