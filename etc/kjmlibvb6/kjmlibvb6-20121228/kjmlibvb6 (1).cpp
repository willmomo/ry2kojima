//-----------------------------------------------------------------------------
// vb6で使わせたくない関数を隠すDLL
//-----------------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>

//-----------------------------------------------------------------------------
// エントリーポイント
//-----------------------------------------------------------------------------
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	return TRUE;
}
