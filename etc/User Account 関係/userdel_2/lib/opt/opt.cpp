#include <string>
#include <vector>
#include <iostream>
#include "opt.h"

//---------------------------------------------------------------------
//[概要]	オプションの取得
//[戻り値]	true: オプションが指定されている
//			false: オプションは指定されていない

bool COpt::Get(int option)
{
	std::vector<OPTION>::iterator i;
	for (i = m_options.begin(); i != m_options.end(); i++) {
		if (i->option == option) {
			return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------
//[概要]	オプションの取得(パラメータ取得付き)
//[戻り値]	true: オプションが指定されている
//			false: オプションは指定されていない

bool COpt::Get(int option, std::string& parm)
{
	std::vector<OPTION>::iterator i;
	for (i = m_options.begin(); i != m_options.end(); i++) {
		if (i->option == option) {
			parm = i->parm;
			return true;
		}
	}

	parm.erase();
	return false;
}

//---------------------------------------------------------------------
//[概要]	nonオプションの取得

bool COpt::GetNonOption(int index, std::string& parm)
{
	if (index >= m_nonoptions.size())
		return false;

	parm = m_nonoptions[index];
	return true;
}

//---------------------------------------------------------------------
//[概要]	オプションの解析

int COpt::Parse(int argc, char** argv, char* optstring)
{
	OPTION temp;
	int option;

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-' || argv[i][0] == '/') {
			// optionの処理
			for (int j = 1; argv[i][j] != '\0'; j++) {
				option = argv[i][j];
				char* p = strchr(optstring, option);

				if (p == NULL) {
					std::cerr << "オプション -" << (char)option << " は、無効です。" << std::endl;
					return -1;
				}

				if (p[1] == '=') {
					if (argv[i][j+1] == '\0') {
						++i;
						if (i < argc) {
							temp.option = option;
							temp.parm = std::string(argv[i]);
							m_options.push_back(temp);
							break;
						} else {
							std::cerr << "オプション -"<< (char)option << " には、パラメータが必要です。" << std::endl;
							return -1;
						}
					} else if (argv[i][j+1] == '=') {
						temp.option = option;
						temp.parm = std::string(&argv[i][j+2]);
						m_options.push_back(temp);
						break;
					} else {
						temp.option = option;
						temp.parm = std::string(&argv[i][j+1]);
						m_options.push_back(temp);
						break;
					}
				} else {
					temp.option = option;
					temp.parm.erase();
					m_options.push_back(temp);
				}
			}
		} else {
			// non-optionの処理
			m_nonoptions.push_back(std::string(argv[i]));
		}
	}

	return 0;
}

