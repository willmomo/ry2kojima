//-----------------------------------------------------------------------------
// kjmBase �̎���
//-----------------------------------------------------------------------------
#include "kjmBase.h"

//-----------------------------------------------------------------------------
// SJIS ������� Unicode ������ɕϊ�
//-----------------------------------------------------------------------------
std::wstring kjm::util::toUnicode(const std::string &str) {
	if (str.empty()) return L"";
	int nSizeNeeded = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	std::vector<wchar_t> buf(nSizeNeeded);
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &buf[0], buf.size());
	return &buf[0];
}

//-----------------------------------------------------------------------------
// Unicode ������� sjis ������ɕϊ�
//-----------------------------------------------------------------------------
std::string kjm::util::toSJIS(const std::wstring& str) {
	if (str.empty()) return "";
	int nSizeNeeded = ::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	std::vector<char> buf(nSizeNeeded);
	::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, &buf[0], buf.size(), NULL, NULL);
	return &buf[0];
}

//-----------------------------------------------------------------------------
// �s���̉��s����菜��(Unicode��)
//-----------------------------------------------------------------------------
std::wstring kjm::util::chomp(const std::wstring &str) {
	std::wstring result = str;
	while (!result.empty() && ((*result.rbegin() == 0x0d) || (*result.rbegin() == 0x0a))) {
		result.erase(result.length() - 1);
	}
	return result;
}
