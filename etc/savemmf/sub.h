#if !defined(__SUB_H)
#define __SUB_H

#include <windows.h>
#include <string>

using namespace std;

wstring StrToWStr(const char* psz);
string WStrToStr(const wchar_t* pwsz);
string WildCardToRegExp(const char* psz);
string GetSystemMessage(DWORD code);

#endif
