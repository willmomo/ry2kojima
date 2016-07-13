//-----------------------------------------------------------------------------
// kjmBase ‚ÌÀ‘•
//-----------------------------------------------------------------------------
#include "kjmBase.h"

//-----------------------------------------------------------------------------
// SJIS •¶š—ñ‚ğ Unicode •¶š—ñ‚É•ÏŠ·
//-----------------------------------------------------------------------------
std::wstring kjm::util::toUnicode(const std::string &str) {
	if (str.empty()) return L"";
	int nSizeNeeded = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	std::vector<wchar_t> buf(nSizeNeeded);
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &buf[0], buf.size());
	return &buf[0];
}

//-----------------------------------------------------------------------------
// Unicode •¶š—ñ‚ğ sjis •¶š—ñ‚É•ÏŠ·
//-----------------------------------------------------------------------------
std::string kjm::util::toSJIS(const std::wstring& str) {
	if (str.empty()) return "";
	int nSizeNeeded = ::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	std::vector<char> buf(nSizeNeeded);
	::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, &buf[0], buf.size(), NULL, NULL);
	return &buf[0];
}

//-----------------------------------------------------------------------------
// s––‚Ì‰üs‚ğæ‚èœ‚­(Unicode”Å)
//-----------------------------------------------------------------------------
std::wstring kjm::util::chomp(const std::wstring &str) {
	std::wstring result = str;
	while (!result.empty() && ((*result.rbegin() == 0x0d) || (*result.rbegin() == 0x0a))) {
		result.erase(result.length() - 1);
	}
	return result;
}
