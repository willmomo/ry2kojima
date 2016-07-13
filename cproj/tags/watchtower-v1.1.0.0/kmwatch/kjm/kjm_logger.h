#pragma once

#include <Windows.h>
#include <tchar.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <string>
#include <vector>

namespace kjm {
	class logger {
	private:
		std::basic_string<TCHAR> m_path;
		std::basic_string<TCHAR> m_name;
		HANDLE m_mutex;

	public:
		logger();
		logger(const logger& src);
		virtual ~logger();
		logger& operator=(const logger& rhs);

	public:
		void write(int level, LPCTSTR fmt, va_list argptr);
		void write_f(LPCTSTR fmt, ...);
		void write_e(LPCTSTR fmt, ...);
		void write_w(LPCTSTR fmt, ...);
		void write_i(LPCTSTR fmt, ...);
		void write_d(LPCTSTR fmt, ...);
		void write_t(LPCTSTR fmt, ...);
		void rotate();
	};
}
