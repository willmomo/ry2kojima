//---------------------------------------------------------------------
// kjm::lockfile class
//---------------------------------------------------------------------

#pragma once


#include <windows.h>
#include <string>


namespace kjm {

	//-----------------------------------------------------------------
	// class lockfile �̓������O�p�̌^
	//-----------------------------------------------------------------
	struct lockfile_error {
		enum error_code {
			not_initialized = 1	// lockfile ����������
		};
		error_code error;

		lockfile_error(error_code e) : error(e) {};
	};


	//-----------------------------------------------------------------
	// lockfile���ȒP�Ɉ����N���X
	//-----------------------------------------------------------------
	class lockfile {
	private:
		HANDLE m_hFile;			// ���b�N�t�@�C���ւ̃n���h��
		std::string m_fullname;	// ���b�N�t�@�C���ւ̃t���p�X��


	private:
		//-------------------------------------------------------------
		// �f�t�H���g�R���X�g���N�^�͕���
		//-------------------------------------------------------------
		lockfile() : m_hFile( INVALID_HANDLE_VALUE ), m_fullname() {};

	public:
		//-------------------------------------------------------------
		// �R���X�g���N�^�E�f�X�g���N�^
		//-------------------------------------------------------------
		lockfile(const std::string& fullname) : m_hFile(INVALID_HANDLE_VALUE), m_fullname(fullname) {};
		virtual ~lockfile() { unlock(); };

		// ���b�N����
		bool lock();

		// ���b�N����������
		void unlock();
	};

}
