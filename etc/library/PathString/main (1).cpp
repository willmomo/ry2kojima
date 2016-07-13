
#include <iostream>
#include "PathString.h"

int main( int argc, char** argv )
{
	kjm::PathString path = kjm::PathString().Append("d:\\hoge").Append("a").Append("b");

	std::cout << "path = " << path.c_str() << std::endl;

	kjm::PathString basePath = path;

	std::cout << basePath.Append( "kioku" ).c_str() << std::endl;
	std::cout << basePath.Append( "lockfile" ).c_str() << std::endl;

	return 0;
}
