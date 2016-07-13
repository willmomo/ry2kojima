//---------------------------------------------------------------------
// kjm::util class
//---------------------------------------------------------------------


#include "kjm_util.h"



//---------------------------------------------------------------------
// 他言語 like な便利関数
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// 行末の改行を取り除く
//---------------------------------------------------------------------
std::string& kjm::util::chomp( std::string& s ) {

	// 行末に改行文字があるときは、取り除く
	while ( !s.empty() && ( *( s.rbegin() ) == '\n' ) ) {
		s.erase( s.end() - 1 );
	}

	return s;
}


//---------------------------------------------------------------------
// Win32API ExpandEnvironmentStringsを安全に使う関数
//---------------------------------------------------------------------
DWORD kjm::util::ExpandEnvironmentStrings( LPCTSTR lpSrc, std::vector<TCHAR>& rDst ) {

	DWORD dwRet;
	
	while ( ( dwRet = ::ExpandEnvironmentStrings( lpSrc, rDst.begin(), rDst.size() ) ) > rDst.size() ) {
		if ( dwRet == 0 ) {
			break;
		} else {
			rDst.resize( dwRet );
		}
	}

	return dwRet;
}

//---------------------------------------------------------------------
// Win32API FormatMessageを安全に使う関数
//---------------------------------------------------------------------
DWORD kjm::util::FormatMessage( DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, std::vector<TCHAR>& buffer, va_list* Arguments ) {

	LPTSTR pMessage = NULL;

	DWORD dwRet = ::FormatMessage(
		dwFlags | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		lpSource,
		dwMessageId,
		dwLanguageId,
		(LPSTR)&pMessage,
		0,
		Arguments);

	if ( pMessage ) {
		buffer.resize( lstrlen( pMessage ) + 1 );
		lstrcpy( buffer.begin(), pMessage );
		::LocalFree( pMessage );
	}

	return dwRet;
}


//---------------------------------------------------------------------
// Win32API GetModuleFileName を安全に使う関数
//---------------------------------------------------------------------
DWORD kjm::util::GetModuleFileName( HMODULE hModule, std::vector<TCHAR>& rFilename ) {

	rFilename.resize( MAX_PATH );

	DWORD dwRet;
	while ( ( dwRet = ::GetModuleFileName( hModule, rFilename.begin(), rFilename.size() ) ) == rFilename.size() ) {
		rFilename.resize( rFilename.size() * 2 );
	}

	return dwRet;
}


//---------------------------------------------------------------------
// Win32API GetPrivateProfileString を安全に使う関数
//---------------------------------------------------------------------
DWORD kjm::util::GetPrivateProfileString( LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, std::vector<TCHAR>& returnedString, LPCTSTR lpFileName ) {

	DWORD dwRet = 512;

	do {
		returnedString.resize( dwRet * 2 );
		dwRet = ::GetPrivateProfileString( lpAppName, lpKeyName, lpDefault, returnedString.begin(), returnedString.size(), lpFileName );
	} while ( dwRet == returnedString.size() - 1 || dwRet == returnedString.size() - 2 );

	return dwRet;
}

//---------------------------------------------------------------------
// Win32API LoadString を安全に使う関数
//---------------------------------------------------------------------
std::string kjm::util::kjm_LoadString( HINSTANCE hInstance, UINT uID ) {

	int ret = 0;
	std::vector<char> work( 512 );

	while ( ( ret = ::LoadString( hInstance, uID, work.begin(), work.size() ) ) == work.size() ) {

		if ( ret == 0 ) {
			// エラーなので、空文字列を返して終了。
			return std::string();
		}

		work.resize( work.size() * 2 );
	}

	return std::string( work.begin() );
}


//---------------------------------------------------------------------
// ただ単に便利な関数
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// 多階層ディレクトリの作成。
//
// @param lpPathName 作成するフォルダのフルパス。
// @return 正常終了のときTRUE、フォルダが作成できなかったときFALSEを返す。
//---------------------------------------------------------------------
BOOL kjm::util::MakeDirectory( LPCTSTR lpDir ) {

	// フォルダの指定が異常の時は、異常終了
	if ( lpDir == NULL )	return FALSE;	// NULLが指定されている
	if ( lpDir[0] == '\0' )	return FALSE;	// 空文字列

	// フォルダが存在すれば、正常終了
	if ( ::PathFileExists( lpDir ) )	return TRUE;

	// ルートフォルダを指しているにもかかわらず、フォルダが無いときは、異常終了
	if ( ::PathIsRoot( lpDir ) )		return FALSE;

	// 一つ上のフォルダを作成する
	std::vector<char> dir( MAX_PATH );
	lstrcpy( dir.begin(), lpDir );
	::PathAppend( dir.begin(), _T( ".." ) );
	if ( MakeDirectory( lpDir ) ) {
		return ::CreateDirectory( lpDir, NULL );
	} else {
		return FALSE;
	}
}
