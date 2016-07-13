#include "hslib.h"

// �R�}���h���C�����

int cmdline::chkarg(int argc, char** argv, const char* option_rule)
{
	m_exefile = argv[0];	// ���s�t�@�C����

	for (int i = 1; i < argc; i++) {
		int c = argv[i][0];
		if (c == '-' || c == '/') {
			c = argv[i][1];
			char* p = strchr(option_rule, c);

			if (p == NULL) {
				// �T�|�[�g���Ă��Ȃ��I�v�V����
				m_options[c];

			} else if (*(p + 1) == ':') {
				// �I�v�V�����ɂ́A�������K�{
				if (argv[i][2] != '\0') {
					m_options[c] = cmdline_option(2, &argv[i][2]);
				} else if (i + 1 < argc) {
					m_options[c] = cmdline_option(2, argv[i+1]);
					++i;
				} else {
					m_options[c] = cmdline_option(-2, std::string());	// �������Ȃ�
				}

			} else if (*(p + 1) == '#') {
				// �I�v�V�����ɂ́A���������邱�Ƃ�����
				if (argv[i][2] != '\0') {
					m_options[c] = cmdline_option(2, &argv[i][2]);
				} else if (i + 1 < argc) {
					m_options[c] = cmdline_option(2, argv[i+1]);
					++i;
				} else {
					m_options[c] = cmdline_option(1, std::string());
				}

			} else {
				// �I�v�V�����ɂ́A�������Ȃ�
				m_options[c] = cmdline_option(1, std::string());
			}

		} else {
			// �I�v�V�����ȊO�́A�t�@�C���ꗗ�ɒǉ�
			m_file_list.push_back(argv[i]);
		}
	}

	return 0;
}

