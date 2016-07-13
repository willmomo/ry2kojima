
#pragma once

#if !defined( __PATHSTRING_H )
#define __PATHSTRING_H

#include <windows.h>
#include <shlwapi.h>
#include <string>
#include <vector>

#pragma comment( lib, "shlwapi.lib" )

namespace kjm {

	// �p�X��������������
	//
	class PathString
	{
	private:
		std::string m_path;

	public:
		// �R���X�g���N�^
		PathString() : m_path() {};
		PathString( const PathString& src ) : m_path( src.m_path ) {};
		PathString( const char* src ) : m_path( src ) {};

		// �f�X�g���N�^
		virtual ~PathString() {};

	public:
		// ������ւ� const pointer ��߂�
		const char* c_str() const { return m_path.c_str(); };

		// ������Ƀp�X����������
		PathString& Append( const PathString& more ) {
			std::vector<char> buf( MAX_PATH );
			
			::lstrcpy( buf.begin(), m_path.c_str() );
			::PathAppend( buf.begin(), more.c_str() );

			m_path = buf.begin();

			return *this;
		};

	public:
		// ������Z�q
		PathString& operator= ( const PathString& src ) {
			m_path = src.c_str();
			return *this;
		};
	};
}
		
#endif
