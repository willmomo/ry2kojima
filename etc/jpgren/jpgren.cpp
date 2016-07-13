//-----------------------------------------------------------------------------
// JPEGâÊëúÇéBâeì˙éûÇ…ÉäÉlÅ[ÉÄÇ∑ÇÈ
//-----------------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>

#include <stdio.h>
#include <locale.h>

#include "kjmsub.h"

typedef struct {
	WORD d1;
	WORD d2;
	WORD d3;
	BYTE d4[6];
} h1_t;

typedef struct {
	WORD d1;
	WORD d2;
	DWORD d3;
} h2_t;

int do_main(FILE* fp) {
	h1_t buf_h1;
	h2_t buf_h2;

	if (fread(&buf_h1, sizeof(buf_h1), 1, fp) != 1) {
		return -1;
	}

	if ((buf_h1.d1 != 0xd8ff) || (buf_h1.d2 != 0xe1ff) || (memcmp(buf_h1.d4, "Exif\0\0", 6) != 0)) {
		return -1;
	}

	if (fread(&buf_h2, sizeof(buf_h2), 1, fp) != 1) {
		return -1;
	}

	bool bigEnd = (buf_h2.d1 == 0x4d4d);

	WORD tagCnt;
	if (fread(&tagCnt, sizeof(tagCnt), 1, fp) != 1) {
		return -1;
	}

	return 0;
}

int _tmain(int argc, TCHAR** argv) {
	_tsetlocale(LC_ALL, _T("japanese"));

	if (argc != 2) return 0;

	kjm::folderInfo folder(argv[1]);

	std::vector<kjm::fileInfo> files = folder.getFiles();

	for (size_t i = 0; i < files.size(); i++) {
		_tprintf(_T("%s\n"), files[i].fullName().c_str());

		FILE* fp = _tfopen(files[i].fullName().c_str(), _T("rb"));
		if (fp) {
			do_main(fp);
			fclose(fp);
		}
	}

	return 0;
}
