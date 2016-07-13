#include <windows.h>
#include <windowsx.h>

int* p;

int hoge()
{
#if 0
	try
	{
		int f = *p;
		return f;
	}
	catch(...)
	{
		return -1;
	}
#else
	__try
	{
		int f = *p;
		return f;
	}
	__except(GetExceptionCode(), EXCEPTION_EXECUTE_HANDLER)
	{
		return -1;
	}
#endif
}

int main()
{
	hoge();

	return 0;
}
