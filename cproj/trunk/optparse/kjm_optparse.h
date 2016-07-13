/*
	kjm_optparse.h	ver.0.2015.3.26

����Ȋ����Ŏg���܂��B
<code>
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

		bool b = opts[_T("help")].get_HasOption();
		b = opts[_T("version")].get_HasOption();

	} catch(std::exception ex) {
		const char* p = ex.what();
	}

	return 0;
}
</code>
*/
#pragma once

#include <tchar.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include <vector>

namespace kjm {

	//=================================================================
	// �������K�v�ȃI�v�V�����Ɉ������^�����Ȃ������ꍇ�̗�O
	//=================================================================

	class missingArgument : public std::exception {
	public:
		missingArgument(const std::string& msg) : std::exception(("missing argument: " + msg).c_str()) {}
		~missingArgument() {}
	};

	//=================================================================
	// ��`����Ă��Ȃ��I�v�V�������^����ꂽ�ꍇ�̗�O
	//=================================================================

	class invalidOption : public std::exception {
	public:
		invalidOption(const std::string& msg) : std::exception(("invalid option: " + msg).c_str()) {}
		~invalidOption() {}
	};

	//=================================================================
	// �I�v�V�����̈����́u�K�v�����v��\�����l
	//=================================================================

	enum optarg_requirement {
		no_argument = 0,
		required_argument = 1,
		optional_argument = 2
	};

	//=================================================================
	// �I�v�V�����̏����w�肷��N���X
	//=================================================================

	class optinfo {
	public:
		// �V���[�g�I�v�V����
		TCHAR ShortOption;

		// �����O�I�v�V����
		std::basic_string<TCHAR> LongOption;

		// �I�v�V���������́u�K�v�����v
		optarg_requirement OptArgRequirement;

		// ���������̕ϐ��̓I�v�V�����̉�͌��ʂ����߂�

		// ���̃I�v�V�������w�肳��Ă����Ƃ��Atrue
		bool HasOption;

		// ���̃I�v�V�����̈���
		std::basic_string<TCHAR> OptArg;

	public:
		// �f�t�H���g�R���X�g���N�^
		optinfo();

		// �f�X�g���N�^
		virtual ~optinfo() {}

		// �R�s�[�R���X�g���N�^
		optinfo(const optinfo& src);

		// ������Z�q
		optinfo& operator=(const optinfo& rhs);

		// �I�v�V�������w��t���R���X�g���N�^
		optinfo(TCHAR shortOption, const std::basic_string<TCHAR>& longOption, optarg_requirement flag);
	};

	//=================================================================
	// �I�v�V��������͂���N���X
	//=================================================================

	class optparse {
	public:
		typedef std::list<std::basic_string<TCHAR>>	ARGS;

	private:
		typedef std::map<std::basic_string<TCHAR>, optinfo>	OPTION_MAP;
		typedef std::map<std::basic_string<TCHAR>, optinfo>::iterator	OPTION_MAP_Iter;

		std::map<std::basic_string<TCHAR>, optinfo> m_opts;	// �Ǘ�����I�v�V�������

	private:
		// �V���[�g�I�v�V�������w�肵�ăI�v�V��������T��
		optinfo& findOption(TCHAR shortOpt);

		// �����O�I�v�V�������w�肵���I�v�V��������T��
		optinfo& findOption(const std::basic_string<TCHAR>& longOpt);

	public:
		// �f�t�H���g�R���X�g���N�^
		optparse() {}

		// �f�X�g���N�^
		virtual ~optparse() {}

		// �R�s�[�R���X�g���N�^
		optparse(const optparse& src);

		// ������Z�q
		optparse& operator=(const optparse& rhs);

		// �����O�I�v�V��������A�z�z�񕗂ɃI�v�V���������擾
		optinfo& operator[](const std::basic_string<TCHAR>& longOpt);

		// ���p�\�ȃI�v�V������ǉ�����
		void add(const optinfo& opt);

		// �I�v�V�����̉��
		ARGS parse(int argc, TCHAR** argv);
	};
}
