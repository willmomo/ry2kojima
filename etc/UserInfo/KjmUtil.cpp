
#include "KjmUtil.h"


// マルチバイト文字列をワイド文字列に変換
//
std::wstring KjmUtil::StrToWStr(const char* str) {
	// ポインタがNULLのときは、空文字列を返す
	if (str == NULL) {
		return std::wstring();
	}

	// 必要な文字数を計算
	size_t size = mbstowcs(NULL, str, strlen(str));

	// バッファを確保
	std::vector<wchar_t> buf(size);

	// 変換
	mbstowcs(&buf[0], str, strlen(str));

	// 変換結果をwstringにして返す
	return std::wstring(buf.begin(), buf.end());
}


// ワイド文字列をマルチバイト文字列に変換
//
std::string KjmUtil::WStrToStr(const wchar_t* wstr) {
	// ポインタがNULLのときは、空文字列を返す
	if (wstr == NULL) {
		return std::string();
	}

	// 必要な文字数を計算
	size_t size = wcstombs(NULL, wstr, wcslen(wstr));

	// バッファを確保
	std::vector<char> buf(size);

	// 変換
	wcstombs(&buf[0], wstr, wcslen(wstr));

	// 変換結果をwstringにして返す
	return std::string(buf.begin(), buf.end());
}
