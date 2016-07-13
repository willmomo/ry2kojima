//=============================================================================
//
//=============================================================================

#pragma once


#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>
#include <process.h>

#if defined(UNICODE)
#define _tstring	std::wstring
#define _tcout		std::wcout
#else
#define _tstring	std::string
#define _tcout		std::cout
#endif


namespace kjm {

	//-------------------------------------------------------------------------
	// ���O�N���X
	//
	// �v���Z�X���ɁA�O���[�o���A�N�Z�X�\�ȃC���X�^���X����������A
	// �e�X���b�h�ŁA�g���܂킹��N���X�B
	//-------------------------------------------------------------------------
	class log {
	public:
		log();
		~log();

	public:
		// ���O�ɏ���
		void write(const _tstring& msg);

	private:
		_tstring m_logName;
		CRITICAL_SECTION m_cs;
	};


	//-------------------------------------------------------------------------
	// ���[�e�B���e�B�֐��̃N���X
	//-------------------------------------------------------------------------
	class util {
	private:
		util() {}
		~util() {}

	public:
		// CreateDirectory �֐��̃��b�p�[
		static bool create_directory(const _tstring& path);

		// GetEnvironmentVariable �֐��̃��b�p�[
		static _tstring get_environment_variable(const _tstring& env);

		// GetModuleFileName �֐��̃��b�p�[
		static _tstring get_module_file_name();

		// GetPrivateProfileString �֐��̃��b�p�[
		static _tstring get_private_profile_string(const _tstring& src, const _tstring& key, const _tstring& def, const _tstring& ini);

		// LoadString �֐��̃��b�p�[
		static _tstring load_string(UINT uID);

		// PathAppend �֐��̃��b�p�[
		static _tstring path_append(const _tstring& path, const _tstring& more);

		// PathFileExists �֐��̃��b�p�[
		static bool path_file_exists(const _tstring& path);

		// PathFindFileName �֐��̃��b�p�[
		static _tstring path_find_file_name(const _tstring& path);

		// PathIsRoot �֐��̃��b�p�[
		static bool path_is_root(const _tstring& path);

		// PathRemoveExtension �֐��̃��b�p�[
		static _tstring path_remove_extension(const _tstring& path);

		// PathRenameExtension �֐��̃��b�p�[
		static _tstring path_rename_extension(const _tstring& path, const _tstring& ext);

		// PathRemoveFileSpec �֐��̃��b�p�[
		static _tstring path_remove_file_spec(const _tstring& path);

		// �t���p�X����t�@�C��������(�g���q����)�����o��
		static _tstring path_split_fname(const _tstring& path);
	};

	//-------------------------------------------------------------------------
	// �X���b�h�N���X
	//-------------------------------------------------------------------------
	class thread {
	public:
		thread() : m_loop(false), m_hThread(NULL) {}
		virtual ~thread() {}

	public:
		// �X���b�h�J�n
		bool begin();

		// �X���b�h�֘A�ϐ������
		void close();

		// �X���b�h���I��
		void end();

		// �I���w�������Ă��邩�ǂ����𒲂ׂ�
		bool is_end();

		// �X���b�h���ĊJ
		DWORD resume();

		// �X���b�h���ꎞ���f
		DWORD suspend();

		// �X���b�h�I���܂ő҂�
		void wait();

		// �������Ȃ��_�~�[�X���b�h(��ʎ���)
		virtual unsigned worker();

	private:
		// ���ʃ��[�J�[�X���b�h
		static unsigned __stdcall worker_func(void* parg);

	private:
		HANDLE m_hThread;
		bool m_loop;
	};
}

