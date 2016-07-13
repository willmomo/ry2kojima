//-----------------------------------------------------------------------------
// namespace kjm �Ŕėp�I�Ɏg������
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
		// sjis ������� Unicode ������ɕϊ�
		static std::wstring toUnicode(const std::string& str);

		// Unicode ������� sjis ������ɕϊ�
		static std::string toSJIS(const std::wstring& str);

		// �s���̉��s����菜��(Unicode��)
		static std::wstring chomp(const std::wstring& str);

		// �s���̉��s����菜��(SJIS��)
		static std::string chomp(const std::string& str) {
			return kjm::util::toSJIS(kjm::util::chomp(kjm::util::toUnicode(str)));
		}
	};
}
