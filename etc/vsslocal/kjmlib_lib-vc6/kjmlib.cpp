#include "kjmlib.h"

#include <locale>
#include <vector>

kjm::application* g_pApplication = NULL;

kjm::application::application() { g_pApplication = this; };

// MBCS文字列をUNICODE文字列に変換
std::wstring kjm::util::toWString(const std::string& src) {
	std::vector<wchar_t> work;

	DWORD dwLen = MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0);

	work.resize(dwLen);

	MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, &work[0], work.size());

	return &work[0];
}

// UNICODE文字列をMBCS文字列に変換
std::string kjm::util::toString(const std::wstring& src) {
	std::vector<char> work;

	DWORD dwLen = WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, NULL, 0, NULL, NULL);

	work.resize(dwLen);

	WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, &work[0], work.size(), NULL, NULL);

	return &work[0];
}

// 指定されたユーザー名とパスワードで、ログオン出来るかどうか調べる。
DWORD kjm::util::checkUserPass(const std::wstring& strUser, const std::wstring& strPass) {
	std::vector<wchar_t> user(strUser.begin(), strUser.end());
	user.push_back('\0');

	std::vector<wchar_t> pass(strPass.begin(), strPass.end());
	pass.push_back('\0');

	DWORD dwResult = 0;
	HANDLE hToken = NULL;
	if (LogonUserW(&user[0], NULL, &pass[0], LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &hToken)) {
		CloseHandle(hToken);
	} else {
		dwResult = GetLastError();
	}

	return dwResult;
}

// ワークグループ名を取得する
std::wstring kjm::lm::getWorkgroupNameW() {
	std::wstring result;

	NET_API_STATUS status;
	LPBYTE pByte;

	status = NetWkstaGetInfo(NULL, 100, &pByte);
	if (status == NERR_Success) {
		LPWKSTA_INFO_100 p = (LPWKSTA_INFO_100)pByte;
		result = (wchar_t*)p->wki100_langroup;
		NetApiBufferFree(pByte);
	}

	return result;
}

// winsockの初期化
void kjm::winsock::startup() {
	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);
}

// IPアドレスを取得
unsigned long kjm::winsock::getIPAddr(const std::string& name) {
	unsigned long ip;

	ip = inet_addr(name.c_str());
	if (ip == INADDR_NONE) {
		hostent* p = gethostbyname(name.c_str());
		if (p != NULL) {
			ip = *((unsigned long*)p->h_addr_list[0]);
		}
	}

	return ip;
}

// IPアドレスを文字列化
std::string kjm::winsock::toIPStringA(unsigned long ip) {
	struct in_addr addr;
	addr.S_un.S_addr = ip;

	return inet_ntoa(addr);
}

// _MBCS 時のエントリーポイント
/*
int main(int argc, char** argv) {
	return g_pApplication->run();
}
*/

// _UNICODE/UNICODE 時のエントリーポイント
int wmain(int argc, wchar_t** argv) {
	std::locale::global(std::locale("japan"));

	return g_pApplication->run();
}
