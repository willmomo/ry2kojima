///////////////////////////////////////////////////////////////////////
// dchook.h

#pragma once

#if defined(DCHOOK_EXPORTS)
#define DCHOOK_API	__declspec(dllexport)
#else
#define DCHOOK_API	__declspec(dllimport)
#endif

//////// �v���g�^�C�v ////////

DCHOOK_API void DcHook();
DCHOOK_API void DcUnhook();
