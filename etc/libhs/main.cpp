#include <windows.h>
#include "libhs.h"

int main(int argc, char **argv)
{
	FILE* fp = fopen("hoge.binx", "rb");
	if (fp) {
		fputs("hoge-", fp);
		fclose(fp);
	}

	return 0;
}
