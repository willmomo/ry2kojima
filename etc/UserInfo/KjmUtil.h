// KjmUtil.h
//


#pragma once

// �ˑ��w�b�_�t�@�C��
#include <string>
#include <vector>


// �����p���[�e�B���e�B�N���X
//
class KjmUtil {
public:
	// �}���`�o�C�g����������C�h������ɕϊ�
	static std::wstring StrToWStr( const char* str );
	static std::wstring StrToWStr( const std::string& str ) {
		return StrToWStr( str.c_str() );
	};

	// ���C�h��������}���`�o�C�g������ɕϊ�
	static std::string WStrToStr( const wchar_t* wstr );
	static std::string WStrToStr( const std::wstring& wstr ) {
		return WStrToStr( wstr.c_str() );
	};
};
