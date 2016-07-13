#include <string>
#include <vector>
#include <iostream>
#include "opt.h"

//---------------------------------------------------------------------
//[�T�v]	�I�v�V�����̎擾
//[�߂�l]	true: �I�v�V�������w�肳��Ă���
//			false: �I�v�V�����͎w�肳��Ă��Ȃ�

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
//[�T�v]	�I�v�V�����̎擾(�p�����[�^�擾�t��)
//[�߂�l]	true: �I�v�V�������w�肳��Ă���
//			false: �I�v�V�����͎w�肳��Ă��Ȃ�

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
//[�T�v]	non�I�v�V�����̎擾

bool COpt::GetNonOption(int index, std::string& parm)
{
	if (index >= m_nonoptions.size())
		return false;

	parm = m_nonoptions[index];
	return true;
}

//---------------------------------------------------------------------
//[�T�v]	�I�v�V�����̉��

int COpt::Parse(int argc, char** argv, char* optstring)
{
	OPTION temp;
	int option;

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-' || argv[i][0] == '/') {
			// option�̏���
			for (int j = 1; argv[i][j] != '\0'; j++) {
				option = argv[i][j];
				char* p = strchr(optstring, option);

				if (p == NULL) {
					std::cerr << "�I�v�V���� -" << (char)option << " �́A�����ł��B" << std::endl;
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
							std::cerr << "�I�v�V���� -"<< (char)option << " �ɂ́A�p�����[�^���K�v�ł��B" << std::endl;
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
			// non-option�̏���
			m_nonoptions.push_back(std::string(argv[i]));
		}
	}

	return 0;
}

