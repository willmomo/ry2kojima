//---------------------------------------------------------------------
// kjm::registry class
//---------------------------------------------------------------------


#include "kjm_registry.h"


//---------------------------------------------------------------------
// RegOpenKeyEx/RegCloseKeyを対にするクラス
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// レジストリを開く
//---------------------------------------------------------------------
LONG kjm::registry::RegOpenKeyEx( HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired ) {

	// すでに開いているかもしれないので、閉じる
	RegCloseKey();

	LONG ret = ::RegOpenKeyEx( hKey, lpSubKey, ulOptions, samDesired, &m_hKey );

	return ret;
}


//---------------------------------------------------------------------
// レジストリを閉じる
//---------------------------------------------------------------------
LONG kjm::registry::RegCloseKey() {
	LONG ret = ERROR_SUCCESS;

	if ( m_hKey ) {
		ret = ::RegCloseKey( m_hKey );
		m_hKey = NULL;
	}

	return ret;
}


//---------------------------------------------------------------------
// レジストリから値を取得
//---------------------------------------------------------------------
LONG kjm::registry::RegQueryValueEx( LPCTSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, std::vector<BYTE>& data ) {

	LONG ret;
	DWORD cbData = 0;

	// 必要なバッファサイズを取得
	ret = ::RegQueryValueEx( m_hKey, lpValueName, lpReserved, lpType, NULL, &cbData );
	if ( ret == ERROR_SUCCESS ) {

		// バッファを確保して正式に呼び出す
		data.resize( cbData );
		ret = ::RegQueryValueEx( m_hKey, lpValueName, lpReserved, lpType, data.begin(), &cbData );
	}

	return ret;
}
