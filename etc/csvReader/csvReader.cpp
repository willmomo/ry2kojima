#pragma warning(disable : 4786)

#include "kjmsub.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int _tmain(int argc, TCHAR** argv) {
	_tout.imbue(std::locale("japanese"));

	kjm::csvFile f;

	f.open(_T("e:\\work\\export.csv"));

	if (f.is_open()) {
		while (f.getline()) {
			_tout << _T("(") << f.col_count() << _T(") ");
			for (int i = 0; i < f.col_count(); i++) {
				_tout << _T("[") << f[i] << _T("]");
			}
			_tout << _T("\n");
		}

		f.close();
	}

	return 0;
}
