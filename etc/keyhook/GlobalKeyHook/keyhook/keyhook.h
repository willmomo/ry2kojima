#pragma once

#if defined(KEYHOOK_EXPORTS)
#define KEYHOOK_API	__declspec(dllexport)
#else
#define KEYHOOK_API	__declspec(dllimport)
#endif

#define KEYHOOK_MESSAGE	"GLOBAL_KEYHOOK"

BOOL KEYHOOK_API SetHook(HWND hwnd);
void KEYHOOK_API FreeHook();
