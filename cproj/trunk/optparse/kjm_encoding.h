/*
*/

#pragma once

#include <tchar.h>
#include <string>

namespace kjm {

	typedef std::basic_string<TCHAR>	_tstring;

	//=================================================================
	// 文字コードに関する
	//=================================================================

	class encoding {
	private:
		encoding() {}
		~encoding() {}

	public:

		// Unicodeをasciiに変換
		// !defined(UNICODE)の時に使われる関数
		static std::string to_mbs(const std::string& src);

		// Unicodeをasciiに変換
		// defined(UNICODE)の時に使われる関数
		static std::string to_mbs(const std::wstring& src);
	};
}
