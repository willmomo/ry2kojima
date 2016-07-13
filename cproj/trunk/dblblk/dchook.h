///////////////////////////////////////////////////////////////////////
// dchook.h

#pragma once

#if defined(DCHOOK_EXPORTS)
#define DCHOOK_API	__declspec(dllexport)
#else
#define DCHOOK_API	__declspec(dllimport)
#endif

//////// プロトタイプ ////////

DCHOOK_API void DcHook();
DCHOOK_API void DcUnhook();
