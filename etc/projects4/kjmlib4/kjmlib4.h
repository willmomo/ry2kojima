//=====================================================================
// kjmlib4.h (ver.1.0.0.0 build 2010.03.23)
//=====================================================================
#pragma once

#include <windows.h>
#include <vector>

namespace kjm4 {
	//=================================================================
	// ���[�e�B���e�B�N���X
	//=================================================================
	class util {
	public:
		// SJIS�������UNICODE������ɕϊ�����
		static bool sjis_to_unicode(const char* sjis_string, std::vector<wchar_t>& unicode_string);

		// �f�B���N�g�����ċA�I�ɍ쐬����(UNICODE�o�[�W����)
		static bool create_directory(const wchar_t* strPathName);

		// �f�B���N�g�����ċA�I�ɍ쐬����(ASCII�o�[�W����)
		static bool create_directory(const char* strPathName);

		// �f�B���N�g�����ċA�I�ɍ폜����(UNICODE�o�[�W����)
		static bool remove_folder(const wchar_t* pszPath);

		// �f�B���N�g�����ċA�I�ɍ폜����(ASCII�o�[�W����)
		static bool remove_folder(const char* pszPath);

	private:
		// �C���X�^���X�����֎~����
		util() {};
		~util() {};
	};
};
