#pragma once

#include <windows.h>

#include <string>

namespace kjm {
	//-----------------------------------------------------------------
	// ���[�e�B���e�B�֐��S
	//
	// ���[�e�B���e�B�N���X�Ȃ̂ŁA�C���X�^���X�����Ȃ��B
	//-----------------------------------------------------------------
	class util {
	public:
		// MBCS��UNICODE�ɕϊ�
		static std::wstring to_stringW(const std::string &str);

		// UNICODE��MBCS�ɕϊ�
		static std::string to_stringA(const std::wstring &str);
		
	private:
		// �f�t�H���g �R���X�g���N�^
		util() {};

		// �f�X�g���N�^
		~util() {};
	};

	//-----------------------------------------------------------------
	// GetTickCount���g���Čo�ߎ��Ԃ��v��
	//-----------------------------------------------------------------
	class tick_counter {
	public:
		// �f�t�H���g �R���X�g���N�^
		tick_counter();

		// �R�s�[ �R���X�g���N�^
		tick_counter(const tick_counter &src);

		// �f�X�g���N�^
		~tick_counter();

		// ������Z�q
		tick_counter &operator =(const tick_counter &rhs);

		// �v���J�n
		void start();

		// �v���I��
		DWORD stop();

		// �o�ߎ���
		DWORD elapse();

		// �o�ߎ���(�b)
		DWORD elapse_sec();

		// �o�ߎ���(��)
		DWORD elapse_min();

	private:
		DWORD m_t1;		// �J�n����
		DWORD m_t2;		// �I������
	};

	//-----------------------------------------------------------------
	// �p�X�����������
	//
	// ���[�e�B���e�B�N���X�Ȃ̂ŁA�C���X�^���X�����Ȃ��B
	//-----------------------------------------------------------------
	class path {
	public:
		// PathAppendW �����̊֐�
		static std::wstring appendW(const std::wstring &strPath, const std::wstring &strMore);

		// PathAppendA�����̊֐�
		static std::string appendA(const std::string &strPath, const std::string &strMore);

#ifdef UNICODE
#define append	appendW
#else
#define append	appendA
#endif

		// PathRemoveFileSpecW �����̊֐�
		static std::wstring remove_file_specW(const std::wstring& strPath);

		// PathRemoveFileSpecA �����̊֐�
		static std::string remove_file_specA(const std::string& strPath);

#ifdef UNICODE
#define remove_file_spec	remove_file_specW
#else
#define remove_file_spec	remove_file_specA
#endif

	private:
		// �f�t�H���g �R���X�g���N�^
		path() {};

		// �f�X�g���N�^
		~path() {};
	};
};
