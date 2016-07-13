/*
	kjm_encoding ver.0.2015.3.27

	MBCSとUnicodeを扱うためのクラスと各種宣言。
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
	// 文字コードに関する処理
	//=================================================================

	class encoding {
	private:
		encoding() {}
		~encoding() {}

	public:
		// 一般的なロケールを設定する関数
		static void setLocale() {
			_tsetlocale(LC_ALL, _T("Japanese"));
		}

		// Unicodeをasciiに変換
		// !defined(_UNICODE)の時に使われる関数
		static std::string to_mbs(const std::string& src);

		// Unicodeをasciiに変換
		// defined(_UNICODE)の時に使われる関数
		static std::string to_mbs(const std::wstring& src);
	};
}
