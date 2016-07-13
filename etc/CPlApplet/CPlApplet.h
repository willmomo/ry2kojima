#pragma once

#if defined(CPLAPPLET_EXPORTS)
#define CPLAPPLET_API	__declspec(dllexport) __stdcall
#else
#define CPLAPPLET_API	__declspec(dllimport) __stdcall
#endif
