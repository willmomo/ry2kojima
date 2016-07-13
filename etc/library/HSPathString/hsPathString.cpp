
#include <windows.h>
#include <shlwapi.h>
#include <vector>
#include "hsPathString.h"

#pragma comment( lib, "shlwapi.lib" )

//---------------------------------------------------------------------
// コピー・コンストラクタ

HSPathString::HSPathString( const HSPathString& pPath )
{
	m_path = pPath.c_str();
}

//---------------------------------------------------------------------
// 代入演算子

HSPathString& HSPathString::operator =( const HSPathString& pPath )
{
	m_path = pPath.c_str();
	return *this;
}

//---------------------------------------------------------------------
//

HSPathString& HSPathString::operator +=( const HSPathString& pPath )
{
	this->Append( pPath );
	return *this;
}

//---------------------------------------------------------------------
//

HSPathString HSPathString::operator +( const HSPathString& pMore )
{
	return HSPathString( *this ).Append( pMore );
}

//---------------------------------------------------------------------
// パスの追加

HSPathString& HSPathString::Append( const HSPathString& pMore )
{
	std::vector<char> work( MAX_PATH );

	lstrcpyn( work.begin(), m_path.c_str(), work.size() );
	PathAppend( work.begin(), pMore.c_str() );
	m_path.assign( work.begin() );

	return *this;
}

//---------------------------------------------------------------------
// \ マークの追加

HSPathString& HSPathString::AddBackslash()
{
	std::vector<char> work( MAX_PATH );

	lstrcpyn( work.begin(), m_path.c_str(), work.size() );
	PathAddBackslash( work.begin() );
	m_path.assign( work.begin() );

	return *this;
}
