//---------------------------------------------------------------------
// lec_crc : main.cpp : lec ópÇÃ CRC ÇåvéZÇ∑ÇÈÅB
//---------------------------------------------------------------------

#include <stdio.h>
#include <io.h>
#include <vector>

using namespace std;

extern "C" unsigned short __stdcall lec_calccrc(unsigned char *p, unsigned int n);

int main(int argc, char** argv)
{
	for (int i = 1; i < argc; i++)
	{
		FILE* fp = fopen(argv[i], "rb");
		if (fp)
		{
			long flen = _filelength(_fileno(fp));
			vector<unsigned char> buf(flen);
			fread(buf.begin(), sizeof(unsigned char), buf.size(), fp);
			printf("%s CRC = 0x%04X\n", argv[i], lec_calccrc(buf.begin(), buf.size()));
			fclose(fp);
		}
	}

	return 0;
}
