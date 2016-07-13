//---------------------------------------------------------------------
// kjmbase : Version 0.2015.12.8
//
// r.kojima
//---------------------------------------------------------------------

#pragma once

#pragma warning(disable : 4786)
#pragma warning(disable : 4996)

#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <sstream>

namespace kjm {
	typedef std::basic_string<TCHAR>	_tstring;
	typedef std::vector<_tstring>	_tstrings;
	typedef std::basic_ostringstream<TCHAR>	_tostringstream;

	class base {
	public:
		// Unicode•¶Žš—ñ‚ðAscii•¶Žš—ñ‚É•ÏŠ·
		static std::string toAscii(const std::wstring& src) {
			std::vector<char> buf(WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, NULL, 0, NULL, NULL));
			WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, &buf[0], buf.size(), NULL, NULL);
			return &buf[0];
		}

		// Unicode•¶Žš—ñ‚ðAscii•¶Žš—ñ‚É•ÏŠ·
		static std::string toAscii(const std::string& src) {
			return src;
		}

		// Ascii•¶Žš—ñ‚ðUnicode•¶Žš—ñ‚É•ÏŠ·
		static std::wstring toUnicode(const std::string& src) {
			std::vector<wchar_t> buf(MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0));
			MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, &buf[0], buf.size());
			return &buf[0];
		}

		// Ascii•¶Žš—ñ‚ðUnicode•¶Žš—ñ‚É•ÏŠ·
		static std::wstring toUnicode(const std::wstring& src) {
			return src;
		}
	};
}
