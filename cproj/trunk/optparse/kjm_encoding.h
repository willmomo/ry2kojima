/*
*/

#pragma once

#include <tchar.h>
#include <string>

namespace kjm {

	typedef std::basic_string<TCHAR>	_tstring;

	//=================================================================
	// �����R�[�h�Ɋւ���
	//=================================================================

	class encoding {
	private:
		encoding() {}
		~encoding() {}

	public:

		// Unicode��ascii�ɕϊ�
		// !defined(UNICODE)�̎��Ɏg����֐�
		static std::string to_mbs(const std::string& src);

		// Unicode��ascii�ɕϊ�
		// defined(UNICODE)�̎��Ɏg����֐�
		static std::string to_mbs(const std::wstring& src);
	};
}
