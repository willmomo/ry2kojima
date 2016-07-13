#include "asfile2csv.h"

using namespace std;

int conv4103(const char* fname, bool raw, int endian)
{
	int ret = 0;

	ifstream ifs;
	ifs.open(fname, ios::in | ios::binary);
	if (ifs.is_open()) {
		FDATA_SMCON buf;

		// ƒwƒbƒ_‚Ìo—Í
		cout << "rno,";
		cout << "indata,";
		cout << "dancnt,";
		if (raw) {
			cout << "err_door,";
		} else {
			cout << "err,door,";
		}
		cout << "othercnt,";
		cout << "flg,";
		cout << endl;

		int rno = 0;
		while (ifs.read((char*)&buf, sizeof(buf)).good()) {
			cout << rno << ",";
			cout << (endian ? ntoh(buf.indata) : buf.indata) << ",";
			cout << (int)buf.dancnt << ",";
			if (raw) {
				cout << (int)buf.err_door << ",";
			} else {
				cout << (int)((buf.err_door & 0xf0) >> 4) << ",";
				cout << (int)(buf.err_door & 0x0f) << ",";
			}
			cout << (int)buf.othercnt << ",";
			cout << (int)buf.flg << ",";
			cout << endl;
			++rno;
		}

		ifs.close();
	} else {
		cerr << "file '" << fname << "' open error." << endl;
		ret = -1;
	}

	return ret;
}
