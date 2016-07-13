#pragma once

#include <windows.h>
#include <tchar.h>

#include <lm.h>
#pragma comment(lib, "netapi32.lib")

//#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <string>

#if defined(_UNICODE) || defined(UNICODE)
#define	_tout	std::wcout
#else
#define	_tout	std::cout
#endif

namespace kjm {
	class util {
	private:
		util() {};
		~util() {};

	public:
		// MBCS�������UNICODE������ɕϊ�
		static std::wstring toWString(const std::string& src);

		// UNICODE�������MBCS������ɕϊ�
		static std::string toString(const std::wstring& src);

		// �w�肳�ꂽ���[�U�[���ƃp�X���[�h�ŁA���O�I���o���邩�ǂ������ׂ�B
		static DWORD checkUserPass(const std::string& strUser, const std::string& strPass) {
			return kjm::util::checkUserPass(kjm::util::toWString(strUser), kjm::util::toWString(strPass));
		}

		// �w�肳�ꂽ���[�U�[���ƃp�X���[�h�ŁA���O�I���o���邩�ǂ������ׂ�B
		static DWORD checkUserPass(const std::wstring& strUser, const std::wstring& strPass);
	};

	class lm {
	private:
		lm() {};
		~lm() {};

	public:
		// ���[�N�O���[�v�����擾
		static std::string getWorkgroupNameA() {
			return kjm::util::toString(kjm::lm::getWorkgroupNameW());
		}

		// ���[�N�O���[�v�����擾
		static std::wstring getWorkgroupNameW();
#if defined(_UNICODE) || defined(UNICODE)
#define	getWorkgroupName	getWorkgroupNameW
#else
#define	getWorkgroupName	getWorkgroupNameA
#endif
	};

	class winsock {
	private:
		winsock() {};
		~winsock() {};

	public:
		// winsock�̏�����
		static void startup();

		// winsock�̏I��
		static void cleanup() {
			WSACleanup();
		}

		// IP�A�h���X���擾
		static unsigned long getIPAddr(const std::string& name);

		// IP�A�h���X���擾
		static unsigned long getIPAddr(const std::wstring& name) {
			return kjm::winsock::getIPAddr(kjm::util::toString(name));
		}

		// IP�A�h���X�𕶎���
		static std::string toIPStringA(unsigned long ip);

		// IP�A�h���X�𕶎���
		static std::wstring toIPStringW(unsigned long ip) {
			return kjm::util::toWString(kjm::winsock::toIPStringA(ip));
		}
#if defined(_UNICODE) || defined(UNICODE)
#define toIPString	toIPStringW
#else
#define toIPString	toIPStringA
#endif
	};

	class application {
	public:
		application();
		virtual ~application() {};

	public:
		virtual int run() = 0;
	};
};
