//---------------------------------------------------------------------
// kjm::memfile class
//---------------------------------------------------------------------

#pragma once


#include <windows.h>
#include <string>
#include "kjm_mutex.h"


namespace kjm {

	//-----------------------------------------------------------------
	// class memfile �̓������O�p�̌^
	//-----------------------------------------------------------------
	struct memfile_error {
		enum error_code {
			not_initialized = 1,	// memfile ����������
			already_initialized		// memfile ���������ς�
		};
		error_code error;

		memfile_error(error_code e) : error(e) {};
	};


	//-----------------------------------------------------------------
	// �������}�b�v�h�t�@�C���Ǘ��N���X
	//
	// note: �쐬�����n���h���́A�f�X�g���N�^�Ŕj�������B
	//-----------------------------------------------------------------
	template<class T> class memfile {
	private:
		HANDLE m_hFile;
		DWORD m_create_status;

		// �ʓ|�Ȃ̂ŁA�C���X�^���X�̕����͕s���B
		memfile<T>(const memfile<T> &src) {};
		memfile<T>& operator=(const memfile<T> &src) {};

	public:
		// memfile�쐬���ɓ�����memfile�����݂����Ƃ��́Ature ��Ԃ�
		//
		bool is_exists() { return (m_create_status == ERROR_ALREADY_EXISTS); };

		// memfile�쐬�ς݂��ǂ����̃e�X�g
		//
		bool is_open() { return (m_hFile != NULL); };

	public:
		//-------------------------------------------------------------
		// �R���X�g���N�^�ƃf�X�g���N�^
		//-------------------------------------------------------------
		memfile<T>() : m_hFile( NULL ), m_create_status( 0 ) {};
		memfile<T>( const std::string& name, DWORD size = 0 ) : m_hFile( NULL ), m_create_status( 0 ) {
			// �ǂ������O�͋N���肦�Ȃ��B�������̐��ۂ́Ais_open�Ŋm�F�B
			if ( size == 0 )	open( name );
			else				create( name, size );
		};
		virtual ~memfile<T>() { close(); };

		// �������}�b�v�h�t�@�C���̍쐬
		bool create(const std::string& name, DWORD size);

		// �������}�b�v�h�t�@�C�����J��
		bool open( const std::string& name );

		// �������}�b�v�h�t�@�C�������
		void close();

		// �������}�b�v�h�t�@�C������f�[�^��ǂݍ���
		int read( T* buffer, size_t offset, size_t size, mutex* pmtx, DWORD timeout );

		// �������}�b�v�h�t�@�C���փf�[�^����������
		int write( T* buffer, size_t offset, size_t size, mutex* pmtx, DWORD timeout );
	};
}
