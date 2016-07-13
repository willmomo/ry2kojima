//
// ������͂Ɋւ���R�[�h
//
// option : Version 0.2015.12.24
//
// r.kojima
//
#include "option.h"

//=============================================================================
// �R�}���h���C����͂ɓn���I�v�V�������\����
//=============================================================================
//---------------------------------------------------------------------
// ������Z�q
//---------------------------------------------------------------------
kjm::optionInfo& kjm::optionInfo::operator=(const kjm::optionInfo& rhs) {
	if (this == &rhs) return *this;
	m_longOption = rhs.m_longOption;
	m_shortOption = rhs.m_shortOption;
	m_optionArgInfo = rhs.m_optionArgInfo;
	m_optDescription = rhs.m_optDescription;
	m_argDescription = rhs.m_argDescription;
	return *this;
}

//=============================================================================
// �R�}���h���C����̓N���X
//=============================================================================
//---------------------------------------------------------------------
// �R�}���h���C�����
//---------------------------------------------------------------------
int kjm::cmdLine::parse(int argc, TCHAR** argv, const optionInfoList& opts) {

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			TCHAR c = argv[i][1];	// �I�v�V��������

			if (c == '-') {	// �����O�I�v�V����
				// ��v���郍���O�I�v�V����������
				size_t j;
				for (j = 0; j < opts.size(); j++) {
					if (_tcsncicmp(opts[j].m_longOption.c_str(), &argv[i][2], opts[j].m_longOption.length()) == 0) {
						break;
					}
				}
				if (j < opts.size()) {
					// ��v����
					const kjm::optionInfo& oi = opts[j];

					if (oi.m_optionArgInfo == kjm::no_argument) {
						// �I�v�V�����ɂ͈����������ꍇ
						this->m_options[oi.m_longOption] = _T("");
					} else if (oi.m_optionArgInfo == kjm::required_argument) {
						if (argv[i][2 + oi.m_longOption.length()] == '=') {
							// �I�v�V�����̈����������Ă���ꍇ
							this->m_options[oi.m_longOption] = &argv[i][2 + oi.m_longOption.length() + 1];
						} else {
							// �ǂ����悤�E�E�E
							// assert(0);
						}
					} else {
						if (argv[i][2 + oi.m_longOption.length()] == '=') {
							// �I�v�V�����̈����������Ă���ꍇ
							this->m_options[oi.m_longOption] = &argv[i][2 + oi.m_longOption.length() + 1];
						} else {
							// �I�v�V�����ɂ͈����������ꍇ
							this->m_options[oi.m_longOption] = _T("");
						}
					}
				}
			} else {
				kjm::optionInfo oi(std::basic_string<TCHAR>(1, c), std::basic_string<TCHAR>(1, c), kjm::optional_argument);
	#if _MSC_VER <= 1200	// VC++6�ȉ�
				// �����̂��߂����̊֐��I�u�W�F�N�g���K�v�ɂȂ��Ă��܂��B
				// �{���̌���d�l�ł́A���̂悤�ȃN���X�͕s�v�B
				optionInfoList::const_iterator ci = 
					std::find_if(opts.begin(), opts.end(), optionInfoShortNameFinder(std::basic_string<TCHAR>(1, c)));
	#elif _MSC_VER <= 1500	// VC++2008
				// VC++2008�ł́Abind2nd�Ɏw�肷��֐��́A�������Q�ƂɂȂ��Ă��Ă͂����Ȃ����߁A
				// �|�C���^��n���֐�����ʂɍ쐬���Ďg�p���Ă���B
				// �{���̌���d�l�ł́A���̂悤�ȃN���X�͕s�v�B
				optionInfoList::const_iterator ci = 
					std::find_if(opts.begin(), opts.end(),
					bind2nd(mem_fun_ref(&kjm::optionInfo::isSameShortOption_P), &std::basic_string<TCHAR>(1, c)));
	#else	// VC++2010�ȍ~
				optionInfoList::const_iterator ci = 
					std::find_if(opts.begin(), opts.end(),
					bind2nd(mem_fun_ref(&kjm::optionInfo::isSameShortOption), std::basic_string<TCHAR>(1, c)));
	#endif
				if (ci != opts.end()) {
					oi = *ci;
				}

				if (oi.m_optionArgInfo == kjm::no_argument) {
					// �I�v�V�����ɂ͈����������ꍇ
					this->m_options[oi.m_longOption] = _T("");
				} else if (oi.m_optionArgInfo == kjm::required_argument) {
					if (argv[i][2] != 0) {
						// �I�v�V�����̈����������Ă���ꍇ
						this->m_options[oi.m_longOption] = &argv[i][2];
					} else if ((argv[i][2] == 0) && (i+1 < argc) && (argv[i+1][0] != '-')) {
						// �I�v�V�����̈��������� argv �ɑ����Ă���ꍇ
						this->m_options[oi.m_longOption] = argv[++i];
					} else {
						// �ǂ����悤�E�E�E
						// assert(0);
					}
				} else {
					if (argv[i][2] != 0) {
						// �I�v�V�����̈����������Ă���ꍇ
						this->m_options[oi.m_longOption] = &argv[i][2];
					} else if ((argv[i][2] == 0) && (i+1 < argc) && (argv[i+1][0] != '-')) {
						// �I�v�V�����̈��������� argv �ɑ����Ă���ꍇ
						this->m_options[oi.m_longOption] = argv[++i];
					} else {
						// �I�v�V�����ɂ͈����������ꍇ
						this->m_options[oi.m_longOption] = _T("");
					}
				}
			}
		} else {
			this->m_arguments.push_back(argv[i]);
		}
	}
	return 0;
}

//---------------------------------------------------------------------
// �R�}���h���C�����
//---------------------------------------------------------------------
int kjm::cmdLine::chkarg(int argc, TCHAR **argv, const kjm::_tstring &shortOpts) {
	kjm::optionInfoList opts;

	// �V���[�g�I�v�V������`�����񂩂� optionInfoList �𐶐�����
	for (kjm::_tstring::size_type i = 0; i < shortOpts.length(); i++) {
		int optArg = no_argument;
		TCHAR c = shortOpts[i];
		if (((i + 1) < shortOpts.length()) && (shortOpts[i + 1] == _T(':'))) {
			optArg = required_argument;
		}

		opts.push_back(kjm::optionInfo(_T(""), kjm::_tstring(1, c), optArg));
	}

	return this->parse(argc, argv, opts);
}

