#include "kjmlib.h"
#include "mp3tag.h"

int loadFile(const _tstring& fname) {
	mp3tag file;

	if (file.load(fname)) {
		_tcout << _T("Load error: ") << fname << _T("\n");
		return -1;
	}

	if (file.hasID3v1tag()) {
		_tcout << file.get_v1tag() << _T("\n");;
	}

	for (int i = 0; i < file.get_tagCount(); i++) {
		ID3v2tag tag = file.get_v2tag(i);

		_tcout << _T("[block ") << std::dec << (i+1) << _T("]\n");
		_tcout << tag << std::endl;

		for (int j = 0; j < tag.get_frameCount(); j++) {
			_tcout << _T("\n<frame ") << std::dec << (j+1) << _T(">\n");
			id3frame frame = tag.get_frame(j);
			_tcout << frame;
		}
	}

	return 0;
}

int _tmain(int argc, TCHAR** argv) {
	//SET_UNICODE_MODE;
	C_SETLOCALE_JAPAN;

	if (argc != 2) {
		return 1;
	}

	_tstring fname = argv[1];

	loadFile(fname);

	return 0;
}
