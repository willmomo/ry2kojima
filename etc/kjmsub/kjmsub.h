#pragma once

#include <windows.h>
#include <tchar.h>

#include <lm.h>
#pragma comment(lib, "netapi32.lib")

#include <locale>
#include <iostream>
#include <vector>

#if !defined(UNICODE)
#define _tcout	std::cout
#else
#define _tcout	std::wcout
#endif

namespace kjm {
	typedef std::basic_string<TCHAR>	_tstring;

	// ユーティリティ関数のためのクラス
	class util {
	private:
		util() {};
		~util() {};

	public:
		// ロケールを日本語にするための簡単な関数
		static void setLocaleJapan() { std::locale::global(std::locale("japan")); };

		// UNICODE文字列をShift JIS文字列に変換
		static std::string convertToAscii(const std::wstring& src);

		// Shift JIS文字列をUNICODE文字列に変換
		static std::wstring convertToUnicode(const std::string& src);
	};

	// lanman 関係のユーティリティ関数のためのクラス
	class lm {
	private:
		lm() {};
		~lm() {};

	public:
		static kjm::_tstring getWorkgroupName();
	};

	// アプリケーションのためのクラス
	//
	// このクラスを派生(class myApp : public kjm::app)して、
	// myApp theApp;
	// と、グローバル変数を一つ用意する。
	class app {
	public:
		app();
		virtual ~app() {};

	public:
		virtual int run() = 0;
	};
};
