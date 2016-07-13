#pragma once

#include <windows.h>
#include <tchar.h>
#include "kjmlib.h"
#if defined(_DEBUG)
#pragma comment(lib, "../kjmlib/Debug/kjmlibmtd.lib")
#else
#pragma comment(lib, "../kjmlib/Release/kjmlibmt.lib")
#endif
