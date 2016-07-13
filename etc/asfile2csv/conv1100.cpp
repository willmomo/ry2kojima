#include "asfile2csv.h"

using namespace std;

static void conv1103_sub1(const char* group)
{
	cout << group << " ŒŸ’m,";
	cout << group << " ‹q‰¹º,";
	cout << group << " “X‰¹º,";
	cout << group << " •\Ž¦,";
	cout << group << " ˆóŽš,";
	cout << group << " ƒJƒƒ‰,";
	cout << group << " ƒJƒƒ‰—Dæ,";
	cout << group << " ’fü,";
}

static void conv1103_sub2(unsigned short flag)
{
	cout << (flag & 0x0001 ? "’âŽ~" : "—LŒø") << ",";
	cout << (flag & 0x0002 ? "’âŽ~" : "—LŒø") << ",";
	cout << (flag & 0x0004 ? "’âŽ~" : "—LŒø") << ",";
	cout << (flag & 0x0008 ? "’âŽ~" : "—LŒø") << ",";
	cout << (flag & 0x0010 ? "’âŽ~" : "—LŒø") << ",";
	cout << (flag & 0x0020 ? "’âŽ~" : "—LŒø") << ",";
	cout << (flag & 0x0040 ? "’âŽ~" : "—LŒø") << ",";
	cout << (flag & 0x8000 ? "’âŽ~" : "—LŒø") << ",";
}

int conv1103(const char* fname, bool raw, int endian)
{
	int ret = 0;
	int i;

	ifstream ifs;
	ifs.open(fname, ios::in | ios::binary);
	if (ifs.is_open()) {
		fno1103_t buf;

		// ƒwƒbƒ_‚Ìo—Í
		cout << "rno,";
		if (raw) {
    		cout << "sanddan_stopfg,";
    		cout << "sandijo_stopfg,";
    		cout << "fusdan_stopfg,";
    		cout << "daifus_stopfg,";
    		cout << "daiijo_stopfg,";
    		cout << "daiten_stopfg,";
    		cout << "kinkoten_stopfg,";
    		cout << "tanijo_stopfg,";
    		cout << "revten_stopfg,";
    		cout << "risekiten_stopfg,";
    		cout << "shokuten_stopfg,";
    		cout << "zkeidan_stopfg,";
    		cout << "zerror_stopfg,";
    		cout << "zdoor_stopfg,";
    		cout << "zdoorten_stopfg,";
    		cout << "daicall_stopfg,";
    		cout << "daidoor_stopfg,";
			for (i = 0; i < countof(buf.yobi); i++) {
    			cout << "yobi(" << i << "),";
			}
		} else {
			conv1103_sub1("ƒTƒ“ƒh’fü");
			conv1103_sub1("ƒTƒ“ƒhˆÙí");
			conv1103_sub1("•s³’fü");
			conv1103_sub1("‘ä•s³");
			conv1103_sub1("‘äˆÙí");
			conv1103_sub1("‘ä“_ŒŸ");
			conv1103_sub1("‹àŒÉ“_ŒŸ");
			conv1103_sub1("’[––ˆÙí");
			conv1103_sub1("‘ä—\–ñ“_ŒŸ");
			conv1103_sub1("—£È“_ŒŸ");
			conv1103_sub1("HŽ–“_ŒŸ");
			conv1103_sub1("‘S“XŒv”’fü");
			conv1103_sub1("‘S“XƒGƒ‰[“ü—Í");
			conv1103_sub1("‘S“XƒhƒA“ü—Í");
			conv1103_sub1("‘S“XƒhƒA“_ŒŸ");
			conv1103_sub1("‘äŒÄo");
			conv1103_sub1("‘äƒhƒA");
		}
		cout << endl;

		int rno = 0;
		while (ifs.read((char*)&buf, sizeof(buf)).good()) {
			cout << rno << ",";
			if (raw) {
    			cout << (endian ? ntoh(buf.sanddan_stopfg) : buf.sanddan_stopfg) << ",";
    			cout << (endian ? ntoh(buf.sandijo_stopfg) : buf.sandijo_stopfg) << ",";
    			cout << (endian ? ntoh(buf.fusdan_stopfg) : buf.fusdan_stopfg) << ",";
    			cout << (endian ? ntoh(buf.daifus_stopfg) : buf.daifus_stopfg) << ",";
    			cout << (endian ? ntoh(buf.daiijo_stopfg) : buf.daiijo_stopfg) << ",";
    			cout << (endian ? ntoh(buf.daiten_stopfg) : buf.daiten_stopfg) << ",";
    			cout << (endian ? ntoh(buf.kinkoten_stopfg) : buf.kinkoten_stopfg) << ",";
    			cout << (endian ? ntoh(buf.tanijo_stopfg) : buf.tanijo_stopfg) << ",";
    			cout << (endian ? ntoh(buf.revten_stopfg) : buf.revten_stopfg) << ",";
    			cout << (endian ? ntoh(buf.risekiten_stopfg) : buf.risekiten_stopfg) << ",";
    			cout << (endian ? ntoh(buf.shokuten_stopfg) : buf.shokuten_stopfg) << ",";
    			cout << (endian ? ntoh(buf.zkeidan_stopfg) : buf.zkeidan_stopfg) << ",";
    			cout << (endian ? ntoh(buf.zerror_stopfg) : buf.zerror_stopfg) << ",";
    			cout << (endian ? ntoh(buf.zdoor_stopfg) : buf.zdoor_stopfg) << ",";
    			cout << (endian ? ntoh(buf.zdoorten_stopfg) : buf.zdoorten_stopfg) << ",";
    			cout << (endian ? ntoh(buf.daicall_stopfg) : buf.daicall_stopfg) << ",";
    			cout << (endian ? ntoh(buf.daidoor_stopfg) : buf.daidoor_stopfg) << ",";
				for (i = 0; i < countof(buf.yobi); i++) {
					cout << (endian ? ntoh(buf.yobi[i]) : buf.yobi[i]) << ",";
				}
			} else {
    			conv1103_sub2(endian ? ntoh(buf.sanddan_stopfg) : buf.sanddan_stopfg);
    			conv1103_sub2(endian ? ntoh(buf.sandijo_stopfg) : buf.sandijo_stopfg);
    			conv1103_sub2(endian ? ntoh(buf.fusdan_stopfg) : buf.fusdan_stopfg);
    			conv1103_sub2(endian ? ntoh(buf.daifus_stopfg) : buf.daifus_stopfg);
    			conv1103_sub2(endian ? ntoh(buf.daiijo_stopfg) : buf.daiijo_stopfg);
    			conv1103_sub2(endian ? ntoh(buf.daiten_stopfg) : buf.daiten_stopfg);
    			conv1103_sub2(endian ? ntoh(buf.kinkoten_stopfg) : buf.kinkoten_stopfg);
    			conv1103_sub2(endian ? ntoh(buf.tanijo_stopfg) : buf.tanijo_stopfg);
    			conv1103_sub2(endian ? ntoh(buf.revten_stopfg) : buf.revten_stopfg);
    			conv1103_sub2(endian ? ntoh(buf.risekiten_stopfg) : buf.risekiten_stopfg);
    			conv1103_sub2(endian ? ntoh(buf.shokuten_stopfg) : buf.shokuten_stopfg);
    			conv1103_sub2(endian ? ntoh(buf.zkeidan_stopfg) : buf.zkeidan_stopfg);
    			conv1103_sub2(endian ? ntoh(buf.zerror_stopfg) : buf.zerror_stopfg);
    			conv1103_sub2(endian ? ntoh(buf.zdoor_stopfg) : buf.zdoor_stopfg);
    			conv1103_sub2(endian ? ntoh(buf.zdoorten_stopfg) : buf.zdoorten_stopfg);
    			conv1103_sub2(endian ? ntoh(buf.daicall_stopfg) : buf.daicall_stopfg);
    			conv1103_sub2(endian ? ntoh(buf.daidoor_stopfg) : buf.daidoor_stopfg);
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

int conv1106(const char* fname, bool raw, int endian)
{
	int ret = 0;
	int i;

	ifstream ifs;
	ifs.open(fname, ios::in | ios::binary);
	if (ifs.is_open()) {
		fno1106_t buf;

		// ƒwƒbƒ_‚Ìo—Í
		cout << "rno,";
		cout << "pas,";
		cout << "type,";
		cout << "uchi,";
		if (raw) {
			cout << "uchifg,";
		} else {
			cout << "Ž©“®‘ÅŽ~‘ä,T2’†‘ÅŽ~’âŽ~,";
		}
		cout << "kaihou_timer,";
		cout << "dmy1,";
		cout << "tohosei,";
		if (raw) {
			cout << "tokufg,";
		} else {
			cout << "“ÁÜƒx[ƒX,T2’†‰¹º’âŽ~,T2’†“ÁÜo—Í,";
		}
		cout << "safijo_saf,";
		cout << "safijo_out,";
		cout << "safijo_timer,";
		cout << "tcntijo_bo,";
		cout << "tcntijo_t1,";
		cout << "tdamaijo_tymax,";
		cout << "staijo_start,";
		cout << "staijo_out,";
		cout << "staijo_timer,";
		cout << "outten_saf,";
		cout << "outten_start,";
		cout << "saften_out,";
		cout << "saften_start,";
		cout << "saften_t1out,";
		cout << "tdamaten_tymin,";
		cout << "staten_saf,";
		cout << "staten_out,";
		cout << "stslump_ss1,";
		cout << "stslump_so,";
		cout << "stslump_bo,";
		cout << "stslump_sbst,";
		cout << "stslump_sbkn,";
		cout << "stslump_sbbn,";
		cout << "next_uti,";
		cout << "outstart_out,";
		cout << "stslump_sharai,";
		cout << "outstart_start,";
		if (raw) {
			for (i = 0; i < countof(buf.yobi1); i++) {
				cout << "yobi1(" << i << "),";
			}
			for (i = 0; i < countof(buf.yobi2); i++) {
				cout << "yobi2(" << i << "),";
			}
		}
		cout << endl;

		int rno = 0;
		while (ifs.read((char*)&buf, sizeof(buf)).good()) {
			cout << rno << ",";
			cout << (unsigned)buf.pas << ",";
			cout << (unsigned)buf.type << ",";
			cout << (endian ? ntoh(buf.uchi) : buf.uchi) << ",";
			if (raw) {
				cout << (endian ? ntoh(buf.uchifg) : buf.uchifg) << ",";
				cout << (endian ? ntoh(buf.kaihou_timer) : buf.kaihou_timer) << ",";
			} else {
				cout << ((endian ? ntoh(buf.uchifg) : buf.uchifg) & 0x0001 ? "on," : "off,");
				cout << ((endian ? ntoh(buf.uchifg) : buf.uchifg) & 0x0002 ? "on," : "off,");
				switch (endian ? ntoh(buf.kaihou_timer) : buf.kaihou_timer) {
				case 0:		cout << "Ž©“®ŠJ•ú‚µ‚È‚¢,";	break;
				case 1:		cout << "‘¦ŠJ•ú,";			break;
				default:	cout << (endian ? ntoh(buf.kaihou_timer) : buf.kaihou_timer) << ",";	break;
				}
			}
			cout << (endian ? ntoh(buf.dmy1) : buf.dmy1) << ",";
			cout << (endian ? ntoh(buf.tohosei) : buf.tohosei) << ",";
			if (raw) {
				cout << (endian ? ntoh(buf.tokufg) : buf.tokufg) << ",";
			} else {
				cout << ((endian ? ntoh(buf.tokufg) : buf.tokufg) & 0x0001 ? "on," : "off,");
				cout << ((endian ? ntoh(buf.tokufg) : buf.tokufg) & 0x0002 ? "on," : "off,");
				cout << ((endian ? ntoh(buf.tokufg) : buf.tokufg) & 0x0004 ? "on," : "off,");
			}
			cout << (endian ? ntoh(buf.safijo_saf) : buf.safijo_saf) << ",";
			cout << (endian ? ntoh(buf.safijo_out) : buf.safijo_out) << ",";
			cout << (endian ? ntoh(buf.safijo_timer) : buf.safijo_timer) << ",";
			cout << (endian ? ntoh(buf.tcntijo_bo) : buf.tcntijo_bo) << ",";
			cout << (endian ? ntoh(buf.tcntijo_t1) : buf.tcntijo_t1) << ",";
			cout << (endian ? ntoh(buf.tdamaijo_tymax) : buf.tdamaijo_tymax) << ",";
			cout << (endian ? ntoh(buf.staijo_start) : buf.staijo_start) << ",";
			cout << (endian ? ntoh(buf.staijo_out) : buf.staijo_out) << ",";
			cout << (endian ? ntoh(buf.staijo_timer) : buf.staijo_timer) << ",";
			cout << (endian ? ntoh(buf.outten_saf) : buf.outten_saf) << ",";
			cout << (endian ? ntoh(buf.outten_start) : buf.outten_start) << ",";
			cout << (endian ? ntoh(buf.saften_out) : buf.saften_out) << ",";
			cout << (endian ? ntoh(buf.saften_start) : buf.saften_start) << ",";
			cout << (endian ? ntoh(buf.saften_t1out) : buf.saften_t1out) << ",";
			cout << (endian ? ntoh(buf.tdamaten_tymin) : buf.tdamaten_tymin) << ",";
			cout << (endian ? ntoh(buf.staten_saf) : buf.staten_saf) << ",";
			cout << (endian ? ntoh(buf.staten_out) : buf.staten_out) << ",";
			cout << (unsigned)buf.stslump_ss1 << ",";
			cout << (unsigned)buf.stslump_so << ",";
			cout << (endian ? ntoh(buf.stslump_bo) : buf.stslump_bo) << ",";
			cout << (endian ? ntoh(buf.stslump_sbst) : buf.stslump_sbst) << ",";
			cout << (unsigned)buf.stslump_sbkn << ",";
			cout << (unsigned)buf.stslump_sbbn << ",";
			cout << (endian ? ntoh(buf.next_uti) : buf.next_uti) << ",";
			cout << (unsigned)buf.outstart_out << ",";
			cout << (unsigned)buf.stslump_sharai << ",";
			cout << (endian ? ntoh(buf.outstart_start) : buf.outstart_start) << ",";
			if (raw) {
				for (i = 0; i < countof(buf.yobi1); i++) {
					cout << (unsigned)buf.yobi1[i] << ",";
				}
				for (i = 0; i < countof(buf.yobi2); i++) {
					cout << (unsigned)buf.yobi2[i] << ",";
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
