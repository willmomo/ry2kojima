/*
	kjm_encoding ver.0.2015.3.27

	MBCS��Unicode���������߂̃N���X�Ɗe��錾�B
*/

#pragma once

#include <tchar.h>
#include <string>

namespace kjm {

	typedef std::basic_string<TCHAR>	_tstring;	// string/wstring synonym
	typedef std::basic_ostream<TCHAR>	_tostream;	// ostream/wostream synonym

#if defined(_UNICODE)
#define _tcin	std::wcin
#define _tcout	std::wcout
#define	_tcerr	std::wcerr
#define	_tclog	std::wclog
#else
#define _tcin	std::cin
#define _tcout	std::cout
#define	_tcerr	std::cerr
#define	_tclog	std::clog
#endif

	//=================================================================
	// �����R�[�h�Ɋւ��鏈��
	//=================================================================

	class encoding {
	private:
		encoding() {}
		~encoding() {}

	public:
		// ��ʓI�ȃ��P�[����ݒ肷��֐�
		static void setLocale() {
			_tsetlocale(LC_ALL, _T("Japanese"));
		}

		// Unicode��ascii�ɕϊ�
		// !defined(_UNICODE)�̎��Ɏg����֐�
		static std::string to_mbs(const std::string& src);

		// Unicode��ascii�ɕϊ�
		// defined(_UNICODE)�̎��Ɏg����֐�
		static std::string to_mbs(const std::wstring& src);
	};
}
