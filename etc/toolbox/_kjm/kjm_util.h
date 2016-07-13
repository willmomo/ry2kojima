//---------------------------------------------------------------------
// kjm::util class
//
// static member のみのクラスです。
//---------------------------------------------------------------------

#pragma once


#if !defined( __KJM_UTIL_H )
#define __KJM_UTIL_H


#define _WIN32_WINNT 0x400


#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <shlwapi.h>



#define countof(A)	(sizeof((A))/sizeof((A)[0]))


namespace kjm {

	//-----------------------------------------------------------------
	// static なユーティリティ関数を集めたクラス
	//-----------------------------------------------------------------
	class util {
	private:
		// インスタンス化させない
		util() {};
		~util() {};

	public:


		//-----------------------------------------------------------------
		// 他言語 like な便利関数
		//-----------------------------------------------------------------
		static std::string& chomp( std::string& s );

		//-----------------------------------------------------------------
		// APIを安全に扱う関数群
		//-----------------------------------------------------------------
		static DWORD ExpandEnvironmentStrings( LPCTSTR lpSrc, std::vector<TCHAR>& rDst );
		static DWORD FormatMessage( DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, std::vector<TCHAR>& buffer, va_list* Arguments );
		static DWORD GetModuleFileName( HMODULE hModule, std::vector<TCHAR>& rFilename );
		static DWORD GetPrivateProfileString( LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, std::vector<TCHAR>& returnedString, LPCTSTR lpFileName );
		static std::string kjm_LoadString( HINSTANCE hInstance, UINT uID );


		//-----------------------------------------------------------------
		// ただ単に便利な関数
		//-----------------------------------------------------------------
		static BOOL MakeDirectory( LPCTSTR lpDir );	// 多階層対応のディレクトリ作成
	};
}


#endif
