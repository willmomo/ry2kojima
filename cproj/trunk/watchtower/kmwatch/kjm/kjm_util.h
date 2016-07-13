/*
	ver:20151018-1203
*/
#pragma once

#include <Windows.h>
#include <tchar.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <string>
#include <vector>

namespace kjm {
	class util {
	private:
		util() {}
		~util() {}

	public:
		static std::basic_string<TCHAR> get_TempPath();
		static std::basic_string<TCHAR> get_ModuleFileName(HMODULE hModule=NULL);
	};
}
