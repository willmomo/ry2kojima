#include "hslib.h"

// コマンドライン解析

int cmdline::chkarg(int argc, char** argv, const char* option_rule)
{
	m_exefile = argv[0];	// 実行ファイル名

	for (int i = 1; i < argc; i++) {
		int c = argv[i][0];
		if (c == '-' || c == '/') {
			c = argv[i][1];
			char* p = strchr(option_rule, c);

			if (p == NULL) {
				// サポートしていないオプション
				m_options[c];

			} else if (*(p + 1) == ':') {
				// オプションには、引数が必須
				if (argv[i][2] != '\0') {
					m_options[c] = cmdline_option(2, &argv[i][2]);
				} else if (i + 1 < argc) {
					m_options[c] = cmdline_option(2, argv[i+1]);
					++i;
				} else {
					m_options[c] = cmdline_option(-2, std::string());	// 引数がない
				}

			} else if (*(p + 1) == '#') {
				// オプションには、引数があることもある
				if (argv[i][2] != '\0') {
					m_options[c] = cmdline_option(2, &argv[i][2]);
				} else if (i + 1 < argc) {
					m_options[c] = cmdline_option(2, argv[i+1]);
					++i;
				} else {
					m_options[c] = cmdline_option(1, std::string());
				}

			} else {
				// オプションには、引数がない
				m_options[c] = cmdline_option(1, std::string());
			}

		} else {
			// オプション以外は、ファイル一覧に追加
			m_file_list.push_back(argv[i]);
		}
	}

	return 0;
}

