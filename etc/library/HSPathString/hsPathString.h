
#pragma once

#if !defined( __HSPATHSTRING_H )
#define __HSPATHSTRING_H

#include <string>

//---------------------------------------------------------------------
// Path文字列管理クラス

class HSPathString
{
private:
	std::string m_path;

public:
	HSPathString() : m_path() {};
	HSPathString( const HSPathString& pPath );
	HSPathString( const char* pPath ) : m_path( pPath ) {};
	virtual ~HSPathString() {};

public:
	HSPathString& AddBackslash();
	HSPathString& Append( const HSPathString& pMore );	// パスの追加

public:
	const char* c_str() const { return m_path.c_str(); };

public:
	HSPathString& operator= ( const HSPathString& pPath );
	HSPathString& operator+= ( const HSPathString& pMore );
	HSPathString operator+ ( const HSPathString& pMore );
};

#endif
