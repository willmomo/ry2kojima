
#pragma once

#include <tchar.h>

#include <map>
#include <vector>
#include <regex>

namespace kjm2 {

	typedef std::basic_regex<TCHAR> _tregex;
	typedef std::match_results<const TCHAR *> _tcmatch;
	typedef std::match_results<std::basic_string<TCHAR>::const_iterator> _tsmatch;

	/*
	kjm2::cmdLine obj;
	obj.parse(argc, argv);
	if (obj.m_named.find(_T("version")) != obj.m_named.end()) {
		kjm2::cmdLine::optarg_pair optarg = obj.m_named[_T("version")];
		if (optarg.first) {
			// オプションに引数が指定された
		} else {
			// オプションに引数が無かった
		}
	}
	*/

	class cmdLine {
	public:
		typedef std::pair<bool, std::basic_string<TCHAR>>	optarg_pair;

	private:
		std::map<std::basic_string<TCHAR>, optarg_pair> m_named;
		std::vector<std::basic_string<TCHAR>> m_unnamed;

	public:
		std::map<std::basic_string<TCHAR>, optarg_pair>& get_Named() { return m_named; }
		std::vector<std::basic_string<TCHAR>>& get_Unnamed() { return m_unnamed; }

	public:
		cmdLine() {
		}

		void parse(int argc, TCHAR** argv) {
			_tregex reg(_T("^[\\-/]{1,2}([^\\-/]+?)([:=](.+))?$"));
			for (int i = 1; i < argc; i++) {
				if (std::regex_match(argv[i], reg)) {
					_tcmatch matches;
					std::regex_search(argv[i], matches, reg);
					m_named[matches[1].str()] = optarg_pair(matches[3].matched, matches[3].str());
				} else {
					m_unnamed.push_back(argv[i]);
				}
			}
		}
	};
};
