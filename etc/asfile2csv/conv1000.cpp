#include "asfile2csv.h"

using namespace std;

int conv1001(const char* fname, bool raw, int endian)
{
	int ret = 0;

	ifstream ifs;
	ifs.open(fname, ios::in | ios::binary);
	if (ifs.is_open()) {
		fno1001_t buf;

		// ヘッダの出力
		cout << "rno,";
		if (raw) {
			cout << "rsdn_nid,";
		} else {
			cout << "rs_nid,dn_nid,";
		}
		cout << "tn_nid,";
		if (raw) {
			cout << "d2_nid,";
		} else {
			cout << "port,d2_nid,";
		}
		cout << "d2_subid,";
		cout << "dno,";
		cout << "pas,";
		cout << "kishu,";
		cout << "sima,";
		cout << "type,";
		cout << "floor,";
		if (raw) {
			cout << "sh_lamp,";
		} else {
			cout << "port1_A,port1_B,port2_A,port2_B,port3_A,port3_B,port4_A,port4_B,";
		}
		cout << "form,";
		cout << "kinko,";
		cout << "urisyu1,";
		cout << "scale1,";
		cout << "urisyu2,";
		cout << "scale2,";
		cout << "urisyu3,";
		cout << "scale3";
		cout << endl;

		int rno = 0;
		while (ifs.read((char*)&buf, sizeof(buf)).good()) {
			cout << rno << ",";
			if (raw) {
				cout << (unsigned)buf.rsdn_nid << ",";
			} else {
				cout << (unsigned)((buf.rsdn_nid & 0xe0) >> 5) << "," << (unsigned)(buf.rsdn_nid & 0x1f) << ",";
			}
			cout << (unsigned)buf.tn_nid << ",";
			if (raw) {
				cout << (unsigned)buf.d2_nid << ",";
			} else {
				cout << (unsigned)((buf.d2_nid & 0xe0) >> 5) << "," << (unsigned)(buf.d2_nid & 0x1f) << ",";
			}
			cout << (unsigned)buf.d2_subid << ",";
			cout << ((endian) ? ntoh(buf.dno) : buf.dno) << ",";
			cout << (unsigned)buf.pas << ",";
			cout << (unsigned)buf.kishu << ",";
			cout << (unsigned)buf.sima << ",";
			cout << (unsigned)buf.type << ",";
			cout << (unsigned)buf.floor << ",";
			if (raw) {
				cout << (unsigned)buf.sh_lamp << ",";
				cout << (unsigned)buf.form << ",";
			} else {
				cout << ((buf.sh_lamp & 0x01) ? "有," : "無,") << ((buf.sh_lamp & 0x10) ? "有," : "無,");
				cout << ((buf.sh_lamp & 0x02) ? "有," : "無,") << ((buf.sh_lamp & 0x20) ? "有," : "無,");
				cout << ((buf.sh_lamp & 0x03) ? "有," : "無,") << ((buf.sh_lamp & 0x30) ? "有," : "無,");
				cout << ((buf.sh_lamp & 0x04) ? "有," : "無,") << ((buf.sh_lamp & 0x40) ? "有," : "無,");
				switch (buf.form) {
				case 1:		cout << "1:1,";	break;
				case 2:		cout << "2:1,";	break;
				default:	cout << (unsigned)buf.form << ",";	break;
				}
			}
			cout << (unsigned)buf.kinko << ",";
			if (raw) {
				cout << (unsigned)buf.urisyu1 << ",";
			} else {
				switch (buf.urisyu1) {
				case 1:		cout << "現金,";		break;
				case 2:		cout << "現金金庫,";	break;
				case 3:		cout << "カード,";		break;
				case 4:		cout << "個数,";		break;
				default:	cout << (unsigned)buf.urisyu1 << ",";	break;
				}
			}
			cout << (int)buf.scale1 << ",";
			if (raw) {
				cout << (unsigned)buf.urisyu2 << ",";
			} else {
				switch (buf.urisyu2) {
				case 1:		cout << "現金,";		break;
				case 2:		cout << "現金金庫,";	break;
				case 3:		cout << "カード,";		break;
				case 4:		cout << "個数,";		break;
				default:	cout << (unsigned)buf.urisyu2 << ",";	break;
				}
			}
			cout << (int)buf.scale2 << ",";
			if (raw) {
				cout << (unsigned)buf.urisyu3 << ",";
			} else {
				switch (buf.urisyu3) {
				case 1:		cout << "現金,";		break;
				case 2:		cout << "現金金庫,";	break;
				case 3:		cout << "カード,";		break;
				case 4:		cout << "個数,";		break;
				default:	cout << (unsigned)buf.urisyu3 << ",";	break;
				}
			}
			cout << (int)buf.scale3 << ",";

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

int conv1002(const char* fname, bool raw, int endian)
{
	int ret = 0;
	int i;

	ifstream ifs;
	ifs.open(fname, ios::in | ios::binary);
	if (ifs.is_open()) {
		fno1002_t buf;

		// ヘッダの出力
		cout << "rno,";
		if (raw) {
			cout << "rsdn_nid,";
			cout << "d2_nid,";
		} else {
			cout << "rs_nid,dn_nid,";
			cout << "port,d2_nid,";
		}
		cout << "d2_pi,";
		cout << "d2_err,";
    	cout << "disp1,";
    	cout << "disp2,";
    	cout << "floor,";
    	cout << "kind,";
		if (raw) {
	    	cout << "inp,";
		} else {
			cout << "計数PI,断線DI,エラーPI,ドアDI,";
		}
    	cout << "pas,";
    	cout << "scale,";
		if (raw) {
			for (i = 0; i < countof(buf.yobi); i++) {
    			cout << "yobi(" << i << "),";
			}
		}
		cout << endl;

		int rno = 0;
		while (ifs.read((char*)&buf, sizeof(buf)).good()) {
			cout << rno << ",";

			if (raw) {
    			cout << (unsigned)buf.rsdn_nid << ",";
    			cout << (unsigned)buf.d2_nid << ",";
			} else {
    			cout << (unsigned)((buf.rsdn_nid & 0xe0) >> 5) << ",";
    			cout << (unsigned)(buf.rsdn_nid & 0x1f) << ",";
    			cout << (unsigned)((buf.d2_nid & 0xe0) >> 5) << ",";
    			cout << (unsigned)(buf.d2_nid & 0x1f) << ",";
			}
    		cout << (unsigned)buf.d2_pi << ",";
    		cout << (unsigned)buf.d2_err << ",";
			cout << (unsigned)(endian ? ntoh(buf.disp1) : buf.disp1) << ",";
			cout << (unsigned)(endian ? ntoh(buf.disp2) : buf.disp2) << ",";
    		cout << (unsigned)buf.floor << ",";
			if (raw) {
    			cout << (unsigned)buf.kind << ",";
	    		cout << (unsigned)buf.inp << ",";
			} else {
				switch (buf.kind) {
				case 1:		cout << "金庫,";			break;
				case 2:		cout << "景品機,";			break;
				case 3:		cout << "現金販売機,";		break;
				case 4:		cout << "カード販売機,";	break;
				case 5:		cout << "リプレイ玉,";		break;
				case 6:		cout << "プレミアム玉,";	break;
				case 7:		cout << "カード発行機,";	break;
				case 8:		cout << "POS,";				break;
				case 9:		cout << "自販機,";			break;
				case 10:	cout << "その他,";			break;
				default:	cout << "unknown " << (unsigned)buf.kind << ",";	break;
				}
				cout << (buf.inp & 0x01 ? "有," : "無,");
				cout << (buf.inp & 0x02 ? "有," : "無,");
				cout << (buf.inp & 0x03 ? "有," : "無,");
				cout << (buf.inp & 0x04 ? "有," : "無,");
			}
    		cout << (unsigned)buf.pas << ",";
    		cout << (int)buf.scale << ",";
			if (raw) {
				for (i = 0; i < countof(buf.yobi); i++) {
    				cout << (int)buf.yobi[i] << ",";
				}
			}

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
