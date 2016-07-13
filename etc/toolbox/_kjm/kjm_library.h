//---------------------------------------------------------------------
// kjm::time class
//---------------------------------------------------------------------

#pragma once


#include <windows.h>


namespace kjm {

	//-----------------------------------------------------------------
	// ライブラリモジュールを管理するクラス
	//-----------------------------------------------------------------
	class library {
	private:
		HINSTANCE m_hLibModule;

	public:
		// アクセサ
		HINSTANCE getInstance() { return m_hLibModule; };

	public:
		library() : m_hLibModule( NULL ) {};
		virtual ~library() { FreeLibrary(); };

	public:
		BOOL LoadLibraryEx( LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags );
		BOOL FreeLibrary();
	};

}
