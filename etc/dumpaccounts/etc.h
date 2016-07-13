#if !defined(__ETC_H__)
#define __ETC_H__

#include <string>

using namespace std;

//---------------------------------------------------------------------
// プロトタイプ

wchar_t* AsciiToUnicode(const char* psz);
char* UnicodeToAscii(const wchar_t* pwsz);
wstring StringToWString(const string& str);
string WStringToString(const wstring& wstr);

#endif
