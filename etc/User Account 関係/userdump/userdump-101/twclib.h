#if !defined(__TWCLIB_H)
#define __TWCLIB_H

#include <windows.h>

#include <string>

using namespace std;

#include "getopt.h"
#include "regex.h"

//---------------------------------------------------------------------
//	プロトタイプ

int TWCVersion();
string WStrToStr(const wchar_t* pwsz);
wstring StrToWStr(const char* psz);
string WildCardToRegExp(const char* psz);
string GetSystemMessage(DWORD code);

#endif
