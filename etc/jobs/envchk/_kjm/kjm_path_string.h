//---------------------------------------------------------------------
// kjm::path_string class
//---------------------------------------------------------------------

#pragma once


#include <windows.h>
#include <shlwapi.h>
#include <string>


namespace kjm {

	//-----------------------------------------------------------------
	// �p�X�p�̏�����ǉ����� string �N���X
	//-----------------------------------------------------------------
	class path_string : public std::string {
	private:

	public:
		path_string() : std::basic_string<char>() {};
		path_string( const path_string& rhs ) : std::basic_string<char>( rhs ) {};
		path_string( std::string::const_pointer s ) : std::basic_string<char>( s ) {};
		virtual ~path_string() {};

		// PathFileExists�̃��b�p�[
		BOOL FileExists() {
			return ::PathFileExists( this->c_str() );
		};

		path_string& RemoveFileSpec();
		path_string& Append( LPCTSTR pMore );
		path_string& RenameExtension( LPCTSTR pszExt );

	public:
		static path_string GetModuleFileName( HMODULE hModule = NULL );
	};

}
