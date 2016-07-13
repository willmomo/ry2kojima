#pragma once

#include <windows.h>
#include <tchar.h>

#include <lm.h>
#pragma comment(lib, "netapi32.lib")

#include <locale>
#include <iostream>
#include <vector>

#if !defined(UNICODE)
#define _tcout	std::cout
#else
#define _tcout	std::wcout
#endif

namespace kjm {
	typedef std::basic_string<TCHAR>	_tstring;

	// ���[�e�B���e�B�֐��̂��߂̃N���X
	class util {
	private:
		util() {};
		~util() {};

	public:
		// ���P�[������{��ɂ��邽�߂̊ȒP�Ȋ֐�
		static void setLocaleJapan() { std::locale::global(std::locale("japan")); };

		// UNICODE�������Shift JIS������ɕϊ�
		static std::string convertToAscii(const std::wstring& src);

		// Shift JIS�������UNICODE������ɕϊ�
		static std::wstring convertToUnicode(const std::string& src);
	};

	// lanman �֌W�̃��[�e�B���e�B�֐��̂��߂̃N���X
	class lm {
	private:
		lm() {};
		~lm() {};

	public:
		static kjm::_tstring getWorkgroupName();
	};

	// �A�v���P�[�V�����̂��߂̃N���X
	//
	// ���̃N���X��h��(class myApp : public kjm::app)���āA
	// myApp theApp;
	// �ƁA�O���[�o���ϐ�����p�ӂ���B
	class app {
	public:
		app();
		virtual ~app() {};

	public:
		virtual int run() = 0;
	};
};
