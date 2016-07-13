//---------------------------------------------------------------------
// kjm::registry class
//---------------------------------------------------------------------

#pragma once


#include <windows.h>
#include <vector>


namespace kjm {

	//-----------------------------------------------------------------
	// レジストリを管理するクラス
	//-----------------------------------------------------------------
	class registry {
	private:
		HKEY m_hKey;

	public:
		// アクセサ
		HKEY getKey() { return m_hKey; };

	public:
		registry() : m_hKey( NULL ) {};
		virtual ~registry() { RegCloseKey(); };

	public:
		LONG RegOpenKeyEx( HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired );
		LONG RegCloseKey();
		LONG RegQueryValueEx( LPCTSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, std::vector<BYTE>& data );
	};

}
