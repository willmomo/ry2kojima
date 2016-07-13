/*
	ver:20151019-1657
*/
#pragma once

#include <Windows.h>
#include <tchar.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <string>
#include <vector>

namespace kjm {
	class path {
	private:
		path() {}
		~path() {}

	public:
		static std::basic_string<TCHAR> append(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& more);
		static std::basic_string<TCHAR> findFileName(const std::basic_string<TCHAR>& path);
		static std::basic_string<TCHAR> removeExtension(const std::basic_string<TCHAR>& path);
	};
}
