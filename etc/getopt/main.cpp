#include <windows.h>
#include <tchar.h>
#include <locale.h>
#include <stdio.h>
#include "getopt.h"


int _tmain(int argc, TCHAR** argv) {

	_tsetlocale(LC_ALL, _T("japanese"));

	option long_options[] = {
		{_T("mode"), required_argument, NULL, 0},
		{NULL, no_argument, NULL, 0}
	};

	while (1) {
		int opt_index;
		int c = getopt_long(argc, argv, _T(""), long_options, &opt_index);

		if (c == -1) {
			break;
		}

		if (c == 0) {
			_tprintf(_T("%s = %s\n"), long_options[opt_index].name, optarg);
		}
	}

	return 0;
}
