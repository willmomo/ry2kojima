#include "kjmlib.h"

//-----------------------------------------------------------------------------
// プログラムスタート
//-----------------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {

	std::string test = "1234";
	std::vector<char> work(test.begin(), test.end());

	printf("size %d\n", work.size());
	printf("work %s\n", &work[0]);
	work.push_back('\0');
	printf("size %d\n", work.size());
	printf("work %s\n", &work[0]);

	return 0;
}
