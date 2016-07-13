#if !defined(__ETC_H__)
#define __ETC_H__

#include <string>

using namespace std;

//---------------------------------------------------------------------
// プロトタイプ

wstring StringToWString(const string& str);
string WStringToString(const wstring& wstr);
wchar_t* AsciiToUnicode(const char* psz);
char* UnicodeToAscii(const wchar_t* pwsz);
int chomp(char* psz);
char* mid(const char* src, int start, int length, char* dst);

#endif
