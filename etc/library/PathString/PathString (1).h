
#pragma once

#if !defined( __PATHSTRING_H )
#define __PATHSTRING_H

#include <windows.h>
#include <shlwapi.h>
#include <string>
#include <vector>

#pragma comment( lib, "shlwapi.lib" )

namespace kjm {

	// パス名を扱う文字列
	//
	class PathString
	{
	private:
		std::string m_path;

	public:
		// コンストラクタ
		PathString() : m_path() {};
		PathString( const PathString& src ) : m_path( src.m_path ) {};
		PathString( const char* src ) : m_path( src ) {};

		// デストラクタ
		virtual ~PathString() {};

	public:
		// 文字列への const pointer を戻す
		const char* c_str() const { return m_path.c_str(); };

		// 文字列にパスを結合する
		PathString& Append( const PathString& more ) {
			std::vector<char> buf( MAX_PATH );
			
			::lstrcpy( buf.begin(), m_path.c_str() );
			::PathAppend( buf.begin(), more.c_str() );

			m_path = buf.begin();

			return *this;
		};

	public:
		// 代入演算子
		PathString& operator= ( const PathString& src ) {
			m_path = src.c_str();
			return *this;
		};
	};
}
		
#endif
