//-----------------------------------------------------------------------------
//
// �����p���C�u����
//
//-----------------------------------------------------------------------------
#pragma once

// Windows�̃w�b�_
#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

// STL�̃w�b�_
#include <locale>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

// cout / wscout ��؂�ւ���}�N��
#if defined(UNICODE) || defined(_UNICODE)
#define _tout	std::wcout
#else
#define _tout	std::cout
#endif

namespace rk {

	typedef std::basic_string<TCHAR>		string;				// string / wstring ��؂�ւ���
	typedef std::vector<string>				strings;			// string[] / wstring[] ��
	typedef std::basic_ostringstream<TCHAR>	_tostringstream;	// ostringstream / wostringstream ��؂�ւ���


	//=========================================================================
	// �f�B���N�g���ƃT�u�f�B���N�g�����쐬�A�폜�A����ї񋓂��邽�߂̃C���X�^
	// ���X ���\�b�h�����J���܂��B
	//=========================================================================
	class DirectoryInfo {
	public:
		DirectoryInfo(const string& path) : m_path(path) {};
		~DirectoryInfo() {};

	private:
		DirectoryInfo() {};

	private:
		string m_path;
	};


	//=========================================================================
	// �f�B���N�g����T�u�f�B���N�g����ʂ��āA�쐬�A�ړ��A����ї񋓂��邽�߂�
	// �ÓI���\�b�h�����J���܂��B
	//=========================================================================
	class Directory {
	public:
		// �����̃t�H���_��V�����t�H���_�ɃR�s�[���܂��B
		static void Copy(const string& sourceDirName, const string& destDirName, bool overwrite=false);

		// path �Ŏw�肵�����ׂẴf�B���N�g���ƃT�u�f�B���N�g�����쐬���܂��B
		// �֐����� CreateDirectory ���� API �Ƃ��Ԃ�̂ŁACreateFolder ��I���B
		static DirectoryInfo CreateFolder(const string& path);

		// �w�肵���p�X���f�B�X�N��̊����̃f�B���N�g�����Q�Ƃ��Ă��邩�ǂ������m�F���܂��B
		static bool Exists(const string& path);

		// �w�肵���f�B���N�g�����̃T�u�f�B���N�g���̖��O���擾���܂��B
		static strings GetDirectories(const string& path);

		// �w�肵���f�B���N�g�����̃t�@�C������Ԃ��܂��B
		static strings GetFiles(const string& path);

	private:
		Directory() {};
		~Directory() {};
	};


	//=========================================================================
	// �t�@�C���̍쐬�A�R�s�[�A�폜�A�ړ��A�I�[�v���̂��߂̐ÓI���\�b�h��񋟂�
	// �܂��B
	//=========================================================================
	class File {
	public:
		// �����̃t�@�C����V�����t�@�C���ɃR�s�[���܂��B
		static void Copy(const string& sourceFileName, const string& destFileName, bool overwrite=false);

	private:
		File() {};
		~File() {};
	};


	//=========================================================================
	// ��O�̊�{�N���X���`���܂��B
	//=========================================================================
	class SystemException {
	public:
		SystemException(DWORD dwError, string msg) : m_dwError(dwError), m_message(msg) {};
		virtual ~SystemException() {};

		DWORD get_Code() { return m_dwError; };
		string get_Message() { return m_message; };

		// �G���[�R�[�h�ƃp�����[�^�����ʓI�ȃG���[���b�Z�[�W���쐬����B
		static string mkmsg(DWORD dwErr, const string& p1=_T(""));

	private:
		DWORD m_dwError;
		string m_message;
	};


	//=========================================================================
	// I/O �G���[�����������Ƃ��ɃX���[������O�B
	//=========================================================================
	class IOException : public SystemException {
	public:
		IOException(DWORD dwError, string msg) : SystemException(dwError, msg) {};
		virtual ~IOException() {};
	};


	//=========================================================================
	// �I�y���[�e�B���O �V�X�e���� I/O �G���[�܂��͓���̎�ނ̃Z�L�����e�B �G���[�̂��߂ɃA�N�Z�X�����ۂ���ꍇ�A�X���[������O�B
	//=========================================================================
	class UnauthorizedAccessException : public SystemException {
	public:
		UnauthorizedAccessException(DWORD dwError, string msg) : SystemException(dwError, msg) {};
		virtual ~UnauthorizedAccessException() {};
	};


	//=========================================================================
	// �t�@�C���܂��̓f�B���N�g���̃p�X�����i�[���� string �C���X�^���X�ő���
	// �����s���܂��B�����̑���́A�v���b�g�t�H�[���ԂŎ��s����܂��B
	//=========================================================================
	class Path {
	public:
		// 2 �̃p�X��������������܂��B
		static string Combine(const string& path1, const string& path2);

		// �w�肵���p�X������̃f�B���N�g������Ԃ��܂��B
		static string GetDirectoryName(const string& path);

		// �w�肵���p�X������̃t�@�C�����Ɗg���q��Ԃ��܂��B
		static string GetFileName(const string& path);

	private:
		Path() {};
		~Path() {};
	};
}
