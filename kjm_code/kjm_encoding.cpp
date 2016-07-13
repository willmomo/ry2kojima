/*
	kjm_encoding ver.0.2015.3.27

	MBCSとUnicodeを扱うためのクラスと各種宣言。
*/

#include "kjm_encoding.h"
#include <vector>

// Unicodeをasciiに変換
// !defined(UNICODE)の時に使われる関数
std::string kjm::encoding::to_mbs(const std::string& src) {
	return src;
}

// Unicodeをasciiに変換
// defined(UNICODE)の時に使われる関数
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
