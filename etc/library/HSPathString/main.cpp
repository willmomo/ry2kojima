
#include <iostream>
#include "hsPathString.h"

int main( int argc, char** argv )
{
	HSPathString path = HSPathString( "D:\\hoge" ) + "kioku";

	std::cout << path.c_str() << "\n";

	return 0;
}
