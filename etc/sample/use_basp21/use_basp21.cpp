#include <windows.h>
#include <iostream>
#include <conio.h>
#include "bregexp.h"


#pragma comment(lib, "bregexp.lib")


using namespace std;


int main(int argc, char** argv) {

	BREGEXP* prxp = NULL;

	char* txt[] = {
		"mpc_finf.txt",
		"dsp1234567890123456.txt",
		"dsp1234567890123456.xml",
		"dsp1234567890-123456.txt",
		"mpc1234567890123456.txt",
	};

	char msg[256];
	for (int i = 0; i < 5; i++) {
		int matched = BMatch("/.+[0-9]{16}\\.txt/", txt[i], txt[i] + strlen(txt[i]), &prxp, msg);
		if (matched == 1) {
			cout << "/.+[0-9]{16}\\.txt/" << "‚Éˆê’v‚µ‚½ " << txt[i] << endl;
		} else {
			cout << "/.+[0-9]{16}\\.txt/" << "‚Éˆê’v‚µ‚È‚¢ " << txt[i] << endl;
		}

		if (prxp) {
			BRegfree(prxp);
			prxp = NULL;
		}
	}

	cout << "Hit any key...";
	_getch();

	return 0;
}
