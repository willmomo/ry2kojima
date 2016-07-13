#include <windows.h>
#include <windowsx.h>

class XDC
{
private:
	HDC m_hdc;
	PAINTSTRUCT m_ps;
public:
	XDC(){};
	~XDC(){};
};
