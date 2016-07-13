/*
	kjm_optparse	ver.0.2015.3.27

	�I�v�V�����̉�͂��s���N���X
*/

#include "kjm_optparse.h"

//=====================================================================
// class optinfo
//=====================================================================

// �f�t�H���g�R���X�g���N�^
kjm::optinfo::optinfo() {
	this->ShortOption = '\0';
	this->LongOption = _T("");
	this->OptArgRequirement = kjm::no_argument;

	this->HasOption = false;
	this->OptArg = _T("");
}

// �I�v�V�������w��t���R���X�g���N�^
kjm::optinfo::optinfo(TCHAR shortOption, const _tstring& longOption, kjm::optarg_requirement flag) {
	this->ShortOption = shortOption;
	this->LongOption = longOption;
	this->OptArgRequirement = flag;

	this->HasOption = false;
	this->OptArg = _T("");
}

// �I�v�V�������w��t���R���X�g���N�^(�V���[�g�v�V��������)
kjm::optinfo::optinfo(const _tstring& longOption, kjm::optarg_requirement flag) {
	this->ShortOption = '\0';
	this->LongOption = longOption;
	this->OptArgRequirement = flag;

	this->HasOption = false;
	this->OptArg = _T("");
}

// �R�s�[�R���X�g���N�^
kjm::optinfo::optinfo(const kjm::optinfo& src) {
	this->ShortOption = src.ShortOption;
	this->LongOption = src.LongOption;
	this->OptArgRequirement = src.OptArgRequirement;

	this->HasOption = src.HasOption;
	this->OptArg = src.OptArg;
}

// ������Z�q
kjm::optinfo& kjm::optinfo::operator=(const kjm::optinfo& rhs) {
	if (this != &rhs) {
		this->ShortOption = rhs.ShortOption;
		this->LongOption = rhs.LongOption;
		this->OptArgRequirement = rhs.OptArgRequirement;

		this->HasOption = rhs.HasOption;
		this->OptArg = rhs.OptArg;
	}
	return *this;
}

//=====================================================================
// class optparse
//=====================================================================

// �R�s�[�R���X�g���N�^
kjm::optparse::optparse(const kjm::optparse& src) {
	this->m_opts = src.m_opts;
}

// ������Z�q
kjm::optparse& kjm::optparse::operator=(const kjm::optparse& rhs) {
	if (this != &rhs) {
		this->m_opts = rhs.m_opts;
	}
	return *this;
}

// ���p�\�ȃI�v�V������ǉ�����
void kjm::optparse::add(const kjm::optinfo& opt) {
	// �����O�I�v�V�������L�[�ɂ��āAmap�ɒǉ�����B
	this->m_opts[opt.LongOption] = opt;
}

// �V���[�g�I�v�V�������w�肵�ăI�v�V��������T��
kjm::optinfo& kjm::optparse::findOption(TCHAR shortOpt) {
	for (OPTION_MAP_Iter i = m_opts.begin(); i != m_opts.end(); i++) {
		if (i->second.ShortOption == shortOpt) {
			return i->second;
		}
	}
	throw kjm::invalidOption(kjm::encoding::to_mbs(_T("-") + _tstring(1, shortOpt)));
}

// �����O�I�v�V�������w�肵���I�v�V��������T��
kjm::optinfo& kjm::optparse::findOption(const _tstring& longOpt) {
	if (m_opts.find(longOpt) != m_opts.end()) {
		return m_opts[longOpt];
	}
	throw kjm::invalidOption(kjm::encoding::to_mbs(_T("--") + longOpt));
}

// �����O�I�v�V��������A�z�z�񕗂ɃI�v�V���������擾
kjm::optinfo& kjm::optparse::operator[](const _tstring& longOpt) {
	return findOption(longOpt);
}

// �I�v�V�����̉��
kjm::optparse::ARGS kjm::optparse::parse(int argc, TCHAR** argv) {
	std::list<_tstring> args;
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			TCHAR c = argv[i][1];
			if (c == '-') {
				// �����O�I�v�V�����̏���
				_tstring work(&argv[i][2]);
				_tstring::size_type pos = work.find_first_of('=');
				_tstring option_name = _tstring(work, 0, pos);

				optinfo& opt = findOption(option_name);
				opt.HasOption = true;
				if (opt.OptArgRequirement == required_argument) {
					if (pos != _tstring::npos) {
						opt.OptArg = _tstring(work, pos + 1);
					} else {
						// �����̃I�v�V�������w�肳��Ă��Ȃ��I
						throw kjm::missingArgument(kjm::encoding::to_mbs(_T("--") + option_name));
					}
				} else if (opt.OptArgRequirement == optional_argument) {
					if (pos != _tstring::npos) {
						opt.OptArg = _tstring(work, pos + 1);
					} else {
						// �����̃I�v�V�������w�肳��Ă��Ȃ�(optional�Ȃ̂Ő���)
					}
				}
			} else {
				// �V���[�g�v�V�����̏���
				optinfo& opt = findOption(c);
				opt.HasOption = true;
				if (opt.OptArgRequirement == required_argument) {
					if (argv[i][2] != '\0') {
						// �V���[�g�I�v�V�����ɑ����ăI�v�V�����̈����������Ă��鎞
						opt.OptArg = &argv[i][1];
					} else if (argv[i][2] == '\0' && (i + 1) < argc) {
						// �V���[�g�I�v�V�����̎��̈���������ꍇ
						// optional_argument�ƈႢ�A�K�������Ƃ��Ĉ���
						opt.OptArg = argv[i + 1];
						++i;
					} else {
						// �����̃I�v�V�������w�肳��Ă��Ȃ��I
						throw kjm::missingArgument(kjm::encoding::to_mbs(_T("-") + _tstring(1, c)));
					}
				} else if (opt.OptArgRequirement == optional_argument) {
					if (argv[i][2] != '\0') {
						// �V���[�g�I�v�V�����ɑ����ăI�v�V�����̈����������Ă��鎞
						opt.OptArg = &argv[i][1];
					} else if (argv[i][2] == '\0' && (i + 1) < argc) {
						// �V���[�g�I�v�V�����̎��̈���������ꍇ
						if (argv[i + 1][0] != '-') {	// �I�v�V�����łȂ���΁A�����Ƃ��Ĉ���
							opt.OptArg = argv[i + 1];
							++i;
						}
					}
				}
			}
		} else {
			// �I�v�V�����ȊO�̈���
			args.push_back(argv[i]);
		}
	}
	return args;
}
