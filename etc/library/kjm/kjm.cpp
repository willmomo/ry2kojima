#include <windows.h>
#include <iostream>
#include "kjmfunc.h"

int main(int argc, char** argv)
{
	time_t t = time(NULL);

	kjm::DateAdd(t, -365, &t);

	tm tmBuf = *localtime(&t);

	kjm::DateAdd(&tmBuf, 365, &tmBuf);

	TCHAR szBuffer[32];
	kjm::DateAdd(TEXT("20041230"), -30, szBuffer, countof(szBuffer));

	std::cout << ctime(&t);
	std::cout << asctime(&tmBuf);
	std::cout << szBuffer << std::endl;

	return 0;
}
