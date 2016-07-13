//---------------------------------------------------------------------
// kjm::mutex class
//---------------------------------------------------------------------

#pragma once


#include <windows.h>
#include <string>


namespace kjm {

	//-----------------------------------------------------------------
	// class mutex �̓������O�p�̌^
	//-----------------------------------------------------------------
	struct mutex_error {
		enum error_code {
			not_initialized = 1,	// mutex ����������
			already_initialized		// mutex ���������ς�
		};

		error_code error;
		mutex_error(error_code e) : error(e) {};
	};


	//-----------------------------------------------------------------
	// mutex �Ǘ��N���X
	//
	// note: �쐬�����n���h���́A�f�X�g���N�^�Ŕj�������B
	//       �܂��Alock(WaitForSingleObject)�����܂܂̃n���h���́A
	//       �K�v��unlock(ReleaseMutex)���Ă���A�j�������B
	//-----------------------------------------------------------------
	class mutex {
	private:
		HANDLE m_hMutex;		// �Ǘ����� mutex �ւ̃n���h��
		DWORD m_create_status;	// mutex �쐬���̏�ԕϐ�
		DWORD m_last_error;		// API�g�p���̃G���[�ێ�
		int m_lock_count;		// lock������

		// �ʓ|�Ȃ̂ŁA�C���X�^���X�̕����͕s���B
		mutex(const mutex &src) {};
		mutex& operator=(const mutex &src) {};

	public:
		// mutex �쐬���ɓ����� mutex �����݂����Ƃ��́Ature ��Ԃ�
		//
		bool is_exists() { return (m_create_status == ERROR_ALREADY_EXISTS); };

		// mutex�쐬�ς݂��ǂ����̃e�X�g
		//
		bool is_open() { return (m_hMutex != NULL); };

	public:
		//-------------------------------------------------------------
		// �R���X�g���N�^�ƃf�X�g���N�^
		//-------------------------------------------------------------
		mutex() : m_hMutex(NULL), m_create_status(0), m_lock_count(0) {};
		mutex(const std::string& name) : m_hMutex(NULL), m_create_status(0), m_lock_count(0) {
			create(name);	// ��O�͋N���肦�Ȃ��B�`�F�b�N�́Ais_open�Ń`�F�b�N
		};
		virtual ~mutex() { close(); };


		// mutex �̍쐬
		bool create( const std::string& name );

		// mutex �����
		void close();

		// �w�莞�ԃV�O�i����ԂɂȂ�̂�҂�
		DWORD lock( DWORD dwMillisec );

		// mutex ���J������
		BOOL unlock();
	};
}
