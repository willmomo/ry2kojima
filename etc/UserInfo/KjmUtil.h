// KjmUtil.h
//


#pragma once

// 依存ヘッダファイル
#include <string>
#include <vector>


// 自分用ユーティリティクラス
//
class KjmUtil {
public:
	// マルチバイト文字列をワイド文字列に変換
	static std::wstring StrToWStr( const char* str );
	static std::wstring StrToWStr( const std::string& str ) {
		return StrToWStr( str.c_str() );
	};

	// ワイド文字列をマルチバイト文字列に変換
	static std::string WStrToStr( const wchar_t* wstr );
	static std::string WStrToStr( const std::wstring& wstr ) {
		return WStrToStr( wstr.c_str() );
	};
};
