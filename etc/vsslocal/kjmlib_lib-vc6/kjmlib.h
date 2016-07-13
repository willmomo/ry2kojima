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
		// MBCS文字列をUNICODE文字列に変換
		static std::wstring toWString(const std::string& src);

		// UNICODE文字列をMBCS文字列に変換
		static std::string toString(const std::wstring& src);

		// 指定されたユーザー名とパスワードで、ログオン出来るかどうか調べる。
		static DWORD checkUserPass(const std::string& strUser, const std::string& strPass) {
			return kjm::util::checkUserPass(kjm::util::toWString(strUser), kjm::util::toWString(strPass));
		}

		// 指定されたユーザー名とパスワードで、ログオン出来るかどうか調べる。
		static DWORD checkUserPass(const std::wstring& strUser, const std::wstring& strPass);
	};

	class lm {
	private:
		lm() {};
		~lm() {};

	public:
		// ワークグループ名を取得
		static std::string getWorkgroupNameA() {
			return kjm::util::toString(kjm::lm::getWorkgroupNameW());
		}

		// ワークグループ名を取得
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
		// winsockの初期化
		static void startup();

		// winsockの終了
		static void cleanup() {
			WSACleanup();
		}

		// IPアドレスを取得
		static unsigned long getIPAddr(const std::string& name);

		// IPアドレスを取得
		static unsigned long getIPAddr(const std::wstring& name) {
			return kjm::winsock::getIPAddr(kjm::util::toString(name));
		}

		// IPアドレスを文字列化
		static std::string toIPStringA(unsigned long ip);

		// IPアドレスを文字列化
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
