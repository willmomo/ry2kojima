//---------------------------------------------------------------------
// kjm::util class
//---------------------------------------------------------------------


#include "kjm_util.h"



//---------------------------------------------------------------------
// ������ like �ȕ֗��֐�
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// �s���̉��s����菜��
//---------------------------------------------------------------------
std::string& kjm::util::chomp( std::string& s ) {

	// �s���ɉ��s����������Ƃ��́A��菜��
	while ( !s.empty() && ( *( s.rbegin() ) == '\n' ) ) {
		s.erase( s.end() - 1 );
	}

	return s;
}


//---------------------------------------------------------------------
// Win32API ExpandEnvironmentStrings�����S�Ɏg���֐�
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
// Win32API FormatMessage�����S�Ɏg���֐�
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
// Win32API GetModuleFileName �����S�Ɏg���֐�
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
// Win32API GetPrivateProfileString �����S�Ɏg���֐�
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
// Win32API LoadString �����S�Ɏg���֐�
//---------------------------------------------------------------------
std::string kjm::util::kjm_LoadString( HINSTANCE hInstance, UINT uID ) {

	int ret = 0;
	std::vector<char> work( 512 );

	while ( ( ret = ::LoadString( hInstance, uID, work.begin(), work.size() ) ) == work.size() ) {

		if ( ret == 0 ) {
			// �G���[�Ȃ̂ŁA�󕶎����Ԃ��ďI���B
			return std::string();
		}

		work.resize( work.size() * 2 );
	}

	return std::string( work.begin() );
}


//---------------------------------------------------------------------
// �����P�ɕ֗��Ȋ֐�
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// ���K�w�f�B���N�g���̍쐬�B
//
// @param lpPathName �쐬����t�H���_�̃t���p�X�B
// @return ����I���̂Ƃ�TRUE�A�t�H���_���쐬�ł��Ȃ������Ƃ�FALSE��Ԃ��B
//---------------------------------------------------------------------
BOOL kjm::util::MakeDirectory( LPCTSTR lpDir ) {

	// �t�H���_�̎w�肪�ُ�̎��́A�ُ�I��
	if ( lpDir == NULL )	return FALSE;	// NULL���w�肳��Ă���
	if ( lpDir[0] == '\0' )	return FALSE;	// �󕶎���

	// �t�H���_�����݂���΁A����I��
	if ( ::PathFileExists( lpDir ) )	return TRUE;

	// ���[�g�t�H���_���w���Ă���ɂ�������炸�A�t�H���_�������Ƃ��́A�ُ�I��
	if ( ::PathIsRoot( lpDir ) )		return FALSE;

	// ���̃t�H���_���쐬����
	std::vector<char> dir( MAX_PATH );
	lstrcpy( dir.begin(), lpDir );
	::PathAppend( dir.begin(), _T( ".." ) );
	if ( MakeDirectory( lpDir ) ) {
		return ::CreateDirectory( lpDir, NULL );
	} else {
		return FALSE;
	}
}
