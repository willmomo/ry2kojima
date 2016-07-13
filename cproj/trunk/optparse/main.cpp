#include "kjm_optparse.h"

#include <locale.h>
#include <tchar.h>

int _tmain(int argc, TCHAR** argv) {
	_tsetlocale(LC_ALL, _T("japanese"));

	kjm::optparse opts;

	opts.add(kjm::optinfo('h', _T("help"), kjm::no_argument));
	opts.add(kjm::optinfo('\0', _T("version"), kjm::no_argument));
	opts.add(kjm::optinfo('f', _T("file"), kjm::required_argument));

	try {
		kjm::optparse::ARGS args = opts.parse(argc, argv);

		for (kjm::optparse::ARGS::iterator i = args.begin(); i != args.end(); i++) {
			std::basic_string<TCHAR> s = *i;
		}

		bool b = opts[_T("help")].HasOption;
		b = opts[_T("version")].HasOption;

	} catch(std::exception ex) {
		const char* p = ex.what();
	}

	return 0;
}
