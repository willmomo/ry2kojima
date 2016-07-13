//=====================================================================
// kjmlib4.h (ver.1.0.0.0 build 2010.03.23)
//=====================================================================
#pragma once

#include <windows.h>
#include <vector>

namespace kjm4 {
	//=================================================================
	// ユーティリティクラス
	//=================================================================
	class util {
	public:
		// SJIS文字列をUNICODE文字列に変換する
		static bool sjis_to_unicode(const char* sjis_string, std::vector<wchar_t>& unicode_string);

		// ディレクトリを再帰的に作成する(UNICODEバージョン)
		static bool create_directory(const wchar_t* strPathName);

		// ディレクトリを再帰的に作成する(ASCIIバージョン)
		static bool create_directory(const char* strPathName);

		// ディレクトリを再帰的に削除する(UNICODEバージョン)
		static bool remove_folder(const wchar_t* pszPath);

		// ディレクトリを再帰的に削除する(ASCIIバージョン)
		static bool remove_folder(const char* pszPath);

	private:
		// インスタンス化を禁止する
		util() {};
		~util() {};
	};
};
