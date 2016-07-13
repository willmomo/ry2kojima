
#include "kjm_encoding.h"

#include <vector>

// Unicode‚ğascii‚É•ÏŠ·
// !defined(UNICODE)‚Ì‚Ég‚í‚ê‚éŠÖ”
std::string kjm::encoding::to_mbs(const std::string& src) {
	return src;
}

// Unicode‚ğascii‚É•ÏŠ·
// defined(UNICODE)‚Ì‚Ég‚í‚ê‚éŠÖ”
std::string kjm::encoding::to_mbs(const std::wstring& src) {
	std::vector<char> mbstr;
	size_t returnValue = 0;
	errno_t ret = wcstombs_s(&returnValue, NULL, 0, src.c_str(), src.length());
	if (ret == 0) {
		mbstr.resize(returnValue);
		ret = wcstombs_s(&returnValue, &mbstr[0], mbstr.size(), src.c_str(), src.length());
	}
	return &mbstr[0];
}
