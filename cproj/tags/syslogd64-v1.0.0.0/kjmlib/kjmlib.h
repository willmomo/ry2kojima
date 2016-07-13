//-----------------------------------------------------------------------------
// バージョン: 1.3.0.0
// 更新日: 2009/03/03
//-----------------------------------------------------------------------------

#pragma once

#ifdef UNICODE
#define	_UNICODE
#endif

#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <vector>
#include <process.h>

#include "getopt.h"

#define countof(A)	(sizeof(A)/sizeof((A)[0]))

#define FT_1MSEC	((__int64)10000)	// FILETIME型の1ミリ秒の値
#define FT_1SEC		(FT_1MSEC * 1000)	// FILETIME型の1秒の値
#define FT_1MIN		(FT_1SEC  *   60)	// FILETIME型の1分の値
#define FT_1HOUR	(FT_1MIN  *   60)	// FILETIME型の1時間の値
#define FT_1DAY		(FT_1HOUR *   24)	// FILETIME型の1日の値

#ifdef UNICODE
#define	_tstring	std::wstring
#else
#define	_tstring	std::string
#endif

#ifdef UNICODE
#define _tcout	std::wcout
#else
#define _tcout	std::cout
#endif

namespace kjm {
	class util {
	public:
		static std::wstring strA2strW(const char* src);

		static std::string strW2strA(const wchar_t* src);

		//---------------------------------------------------------------------
		// pszPathで指定したディレクトリ内の、現在日時からnDayより古いファイル
		// を削除します。
		//
		// 削除対象は、pszPath直下のファイルだけです。
		//---------------------------------------------------------------------
		static void cleanup_dir(const _tstring& pszPath, int nDay);

		static _tstring get_environment_variable(const _tstring& lpName);

		static _tstring get_module_filename(HMODULE hModule = NULL);

		static _tstring get_private_profile_string(const _tstring& lpSectionName, const _tstring& lpKeyName, const _tstring& lpDefault, const _tstring& lpFileName);

		static _tstring load_string(UINT uID, HINSTANCE hInstance = NULL);

		static _tstring path_append(const _tstring& pPath, const _tstring& pMore);

		static _tstring path_remove_filespec(const _tstring& path);

		static _tstring path_rename_extension(const _tstring& pszPath, const _tstring& pszExt);

		static bool create_directory(const _tstring& path);

	private:
		util() {};
		~util() {};
	};

#ifdef _MT
	class thread {
	public:
		thread() : m_loop(false), m_hThread(NULL) {};
		virtual ~thread() {
			close();
		};

		void close() {
			if (m_hThread != NULL) {
				CloseHandle(m_hThread);
				m_hThread = NULL;
			}
		};

		virtual void begin() {
			m_loop = true;
			m_hThread = (HANDLE)_beginthreadex(NULL, 0, thread_main, this, 0, NULL);
		};
		
		virtual void end() {
			m_loop = false;
		};
		
		virtual DWORD suspend() {
			return SuspendThread(m_hThread);
		};
		
		virtual DWORD resume() {
			return ResumeThread(m_hThread);
		};
		
		virtual DWORD wait(DWORD dwTimeout = INFINITE) {
			return WaitForSingleObject(m_hThread, dwTimeout);
		};

		virtual unsigned worker() {
			// この関数のもっともシンプルな実装
			while (still_loop()) {
				Sleep(100);
			};
			return 0;
		};

	protected:
		bool still_loop() {
			return m_loop;
		};

		static unsigned __stdcall thread_main(void* pvarg) {
			return ((thread*)pvarg)->worker();
		};

	private:
		bool m_loop;
		HANDLE m_hThread;
	};
#endif

	class time {
	public:
		time() {
			memset(&m_ft, 0, sizeof(m_ft));
		};

		time(FILETIME src) : m_ft(src) {};

		time(SYSTEMTIME src) {
			SystemTimeToFileTime(&src, &m_ft);
		};

		~time() {};

	public:
		__int64 operator-(const kjm::time& rhs) {
			if (this == &rhs) return 0;
			return *(__int64*)&m_ft - *(__int64*)&rhs.m_ft;
		};

	public:
		static kjm::time get_system_time() {
			SYSTEMTIME st;
			GetSystemTime(&st);
			return kjm::time(st);
		};

		static kjm::time get_local_time() {
			SYSTEMTIME st;
			GetLocalTime(&st);
			return kjm::time(st);
		};

	private:
		FILETIME m_ft;
	};

	class process {
	public:
		process();
		virtual ~process() { close(); };

		virtual BOOL run(const _tstring& pCommandLine);

		virtual DWORD wait(DWORD dwTimeout = INFINITE) {
			return WaitForSingleObject(m_pi.hProcess, dwTimeout);
		};

		virtual void close();

	private:
		STARTUPINFO m_si;
		PROCESS_INFORMATION m_pi;
	};
}
