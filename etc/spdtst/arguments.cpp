//---------------------------------------------------------------------
// CArguments�N���X�̃C���v�������g

#pragma warning(disable : 4786)	// �}�~:'identifier' : �f�o�b�O���Ŏ��ʎq�� 'number' �����ɐ؂�̂Ă��܂����B

#define __STDC__	1

#include "arguments.h"

//---------------------------------------------------------------------
// [�T�v]
// �����̉��
//---------------------------------------------------------------------

int CArguments::ParseArgument(int argc, char **argv, struct option *long_options)
{
	int i, c, option_index = -1;
	std::string short_options;

	// long_options����short_options���쐬
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
			// long option����������
			m_options[long_options[option_index].name] = optarg ? optarg : "";
		} else if (c != '?') {
			// short option����������

			// �Ή�����long option��T��
			for (i = 0; long_options[i].val != c && long_options[i].name != NULL; i++)
				;
				
			m_options[long_options[i].name] = optarg ? optarg : "";
		} else {
			// �c��͔F���ł��Ȃ��I�v�V����
			m_erroptions.push_back(optopt ? std::string(1, (char)optopt) : argv[optind-1]);
		}

		option_index = -1;
	}

	// �I�v�V�����ȊO�̈�����o�^
	for (; optind < argc; ++optind) {
		m_arguments.push_back(argv[optind]);
	}

	return 0;
}
