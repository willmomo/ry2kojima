#pragma once

#if defined(KMWATCH_EXPORTS)
#define KMWATCH_API	__declspec(dllexport)
#else
#define KMWATCH_API	__declspec(dllimport)
#endif

#define KMWATCH_MESSAGE	"GLOBAL_KMWATCH"
#define KMWATCH_MOUSE_MESSAGE	"GLOBAL_KMWATCH_MOUSE"

BOOL KMWATCH_API SetHook(HWND hwnd);
void KMWATCH_API FreeHook();
