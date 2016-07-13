//---------------------------------------------------------------------
// kjm::util class
//
// static member �݂̂̃N���X�ł��B
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
	// static �ȃ��[�e�B���e�B�֐����W�߂��N���X
	//-----------------------------------------------------------------
	class util {
	private:
		// �C���X�^���X�������Ȃ�
		util() {};
		~util() {};

	public:


		//-----------------------------------------------------------------
		// ������ like �ȕ֗��֐�
		//-----------------------------------------------------------------
		static std::string& chomp( std::string& s );

		//-----------------------------------------------------------------
		// API�����S�Ɉ����֐��Q
		//-----------------------------------------------------------------
		static DWORD ExpandEnvironmentStrings( LPCTSTR lpSrc, std::vector<TCHAR>& rDst );
		static DWORD FormatMessage( DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, std::vector<TCHAR>& buffer, va_list* Arguments );
		static DWORD GetModuleFileName( HMODULE hModule, std::vector<TCHAR>& rFilename );
		static DWORD GetPrivateProfileString( LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, std::vector<TCHAR>& returnedString, LPCTSTR lpFileName );
		static std::string kjm_LoadString( HINSTANCE hInstance, UINT uID );


		//-----------------------------------------------------------------
		// �����P�ɕ֗��Ȋ֐�
		//-----------------------------------------------------------------
		static BOOL MakeDirectory( LPCTSTR lpDir );	// ���K�w�Ή��̃f�B���N�g���쐬
	};
}


#endif
