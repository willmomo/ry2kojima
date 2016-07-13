
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <locale>
#include <iomanip>

#include "smart.h"
#include "kjmutil.h"


int _tmain(int argc, TCHAR** argv) {

	std::locale loc("japanese");
	std::_tcout.imbue(loc);

	kjm::smart disk;
	std::vector<kjm::smart_attr> data;

	if (disk.open_device(0)) {
		if (disk.is_supported()) {
			disk.load_info(&data);

			for (int i = 0; i < data.size(); i++) {
				std::_tcout
					<< std::setw(3)  << std::right << (int)data[i].id << _T(" ")
					<< std::setw(41) << std::left  << data[i].title << _T("")
					<< std::setw(3)  << std::right << (int)data[i].value << _T(" ")
					<< std::setw(3)  << (int)data[i].worst << _T(" ")
					<< std::setw(3)  << (int)data[i].threshold << _T(" ")
					<< std::setw(6)  << (int)data[i].vender
					<< _T("\n");
			}

			int status = disk.status();
			std::_tcout << _T("f’f: ") << ((status == 0) ? _T("–â‘è‚È‚µ") : ((status == 1) ? _T("–â‘è‚ ‚è") : _T("“à•”ƒGƒ‰["))) << _T("\n");
		} else {
			std::_tcout << _T("smart not supported: ") << kjm::kjmutil::format_message(GetLastError()) << _T("\n");
		}
	} else {
		std::_tcout << _T("disk.open_device error: ") << kjm::kjmutil::format_message(GetLastError()) << _T("\n");
	}

	return 0;
}
