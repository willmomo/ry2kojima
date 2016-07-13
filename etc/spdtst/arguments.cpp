//---------------------------------------------------------------------
// CArgumentsクラスのインプリメント

#pragma warning(disable : 4786)	// 抑止:'identifier' : デバッグ情報で識別子が 'number' 文字に切り捨てられました。

#define __STDC__	1

#include "arguments.h"

//---------------------------------------------------------------------
// [概要]
// 引数の解析
//---------------------------------------------------------------------

int CArguments::ParseArgument(int argc, char **argv, struct option *long_options)
{
	int i, c, option_index = -1;
	std::string short_options;

	// long_optionsからshort_optionsを作成
	for (i = 0; long_options[i].name != NULL; i++) {
		if (long_options[i].val != 0) {
			short_options += (char)long_options[i].val;
			
			if (long_options[i].has_arg == required_argument) {
				short_options += ':';
			} else if (long_options[i].has_arg == optional_argument) {
				short_options += "::";
			}
		}
	}

	while ((c = getopt_long(argc, argv, short_options.c_str(), long_options, &option_index)) != EOF) {

		if (option_index >= 0) {
			// long optionが見つかった
			m_options[long_options[option_index].name] = optarg ? optarg : "";
		} else if (c != '?') {
			// short optionが見つかった

			// 対応するlong optionを探す
			for (i = 0; long_options[i].val != c && long_options[i].name != NULL; i++)
				;
				
			m_options[long_options[i].name] = optarg ? optarg : "";
		} else {
			// 残りは認識できないオプション
			m_erroptions.push_back(optopt ? std::string(1, (char)optopt) : argv[optind-1]);
		}

		option_index = -1;
	}

	// オプション以外の引数を登録
	for (; optind < argc; ++optind) {
		m_arguments.push_back(argv[optind]);
	}

	return 0;
}
