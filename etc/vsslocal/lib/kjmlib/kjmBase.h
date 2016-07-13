//-----------------------------------------------------------------------------
// namespace kjm で汎用的に使う処理
//-----------------------------------------------------------------------------
#pragma once

#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>

namespace kjm {
	class util {
	private:
		util() {}
		~util() {}

	public:
		// sjis 文字列を Unicode 文字列に変換
		static std::wstring toUnicode(const std::string& str);

		// Unicode 文字列を sjis 文字列に変換
		static std::string toSJIS(const std::wstring& str);

		// 行末の改行を取り除く(Unicode版)
		static std::wstring chomp(const std::wstring& str);

		// 行末の改行を取り除く(SJIS版)
		static std::string chomp(const std::string& str) {
			return kjm::util::toSJIS(kjm::util::chomp(kjm::util::toUnicode(str)));
		}
	};
}
