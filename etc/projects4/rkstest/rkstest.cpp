#include <stdio.h>
#include <stdlib.h>


#include "../rkslib/rkslib.h"
#pragma comment(lib, "../rkslib/Library/rkslibd.lib")


void foo2(const array<char>& ar) {
	ar.size();
	const char* p = ar;
}


void foo() {
	array<char> buf(256);
	buf.resize(512);

	buf.size();
	buf.top();

	foo2(buf);
}


int main() {
	foo();

	bool ret = remove_directory("E:\\work\\use");

	return 0;
}
