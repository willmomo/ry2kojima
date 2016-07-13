#include "twclib.h"

string GetSystemMessage(DWORD code)
{
	string s;
	LPTSTR p;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, NULL, code, 0, (LPTSTR)&p, 256, NULL);
	s.assign(p);
	LocalFree(p);

	return s;
}
