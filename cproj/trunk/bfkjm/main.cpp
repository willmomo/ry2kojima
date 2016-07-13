//
// C++ ‚ÌÀ‘•‚Æ‘ŠŒİ‚ÉˆÃ†E•œ†‚Å‚«‚éÀ‘•B
//

#include "blowfish.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "resource.h"

using namespace std;

int main(int argc, char** argv) {
	if (argc != 3) {
		cout << "usage: bfkjm.exe [key] [data]" << endl;
		return 0;
	}

	if (argv[2][0] != '?') {
		vector<char> enc = kjm::blowfish::encrypt(argv[1], argv[2]);

		cout << '?' << hex << setfill('0');
		for_each(enc.begin(), enc.end(), [](int n) { cout << setw(2) << (n & 0xff); });
		cout << endl;
	} else {
		std::string stemp(&argv[2][1]);
		std::string work;
		while (!stemp.empty()) {
			work.push_back((char)strtol(stemp.substr(0, 2).c_str(), NULL, 16));
			stemp.erase(0, 2);
		}

		vector<char> dec = kjm::blowfish::decrypt(argv[1], work);

		cout << string(dec.begin(), dec.end()) << endl;
	}
	return 0;
}
