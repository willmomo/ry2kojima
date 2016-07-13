//
// ������͂Ɋւ���R�[�h
//
// option : Version 0.2015.12.24
//
// r.kojima
//
#pragma once

#include "kjmbase.h"
#include <map>
#include <list>
#include <algorithm>

namespace kjm {
	//=================================================================
	// �I�v�V�����̈����́u�K�v�����v��\�����l
	//=================================================================

	enum optarg_requirement {
		no_argument = 0,
		required_argument = 1,
		optional_argument = 2
	};

	//-------------------------------------------------------------------------
	// �R�}���h���C����͂ɓn���I�v�V�������\����
	//-------------------------------------------------------------------------
//	static const int no_argument = 0;
//	static const int optional_argument = 1;
//	static const int required_argument = 2;

	class optionInfo {
	public:
		_tstring m_longOption;	// �����O�I�v�V����������
		_tstring m_shortOption;	// �V���[�g�I�v�V����������
		int m_optionArgInfo;					// 0 = �I�v�V�����Ȃ�, 1 = �I�v�V�������Ƃ肤��, 2 = �I�v�V�������K�{
		_tstring m_optDescription;	// �I�v�V�����̐���
		_tstring m_argDescription;	// �����̐���

		// �f�t�H���g�R���X�g���N�^
		optionInfo() : m_longOption(), m_shortOption(), m_optionArgInfo(kjm::no_argument), m_optDescription(), m_argDescription() {}

		// ���������R���X�g���N�^
		optionInfo(const _tstring& longOption, const _tstring& shortOption, int optionArgInfo, const _tstring& optDescription = _T(""), const _tstring& argDescription = _T(""))
			: m_longOption(longOption), m_shortOption(shortOption), m_optionArgInfo(optionArgInfo), m_optDescription(optDescription), m_argDescription(argDescription) {}

		// �R�s�[�R���X�g���N�^
		optionInfo(const kjm::optionInfo& src)
			: m_longOption(src.m_longOption), m_shortOption(src.m_shortOption), m_optionArgInfo(src.m_optionArgInfo), m_optDescription(src.m_optDescription), m_argDescription(src.m_argDescription) {}

		// ������Z�q
		optionInfo& operator=(const kjm::optionInfo& rhs);

		// �V���[�g�I�v�V�������������ǂ������ׂ�(�Q�ƃo�[�W����)
		bool isSameShortOption(const _tstring& name) const { return (m_shortOption == name); }

		// �����O�I�v�V�������������ǂ������ׂ�(�Q�ƃo�[�W����)
		bool isSameLongOption(const _tstring& name) const { return (m_longOption == name); }

#if _MSC_VER <= 1500	// VC++2008
		// �V���[�g�I�v�V�������������ǂ������ׂ�(�|�C���^�o�[�W����)
		bool isSameShortOption_P(const _tstring* name) const { return (m_shortOption == *name); }

		// �����O�I�v�V�������������ǂ������ׂ�(�|�C���^�o�[�W����)
		bool isSameLongOption_P(const _tstring* name) const { return (m_longOption == *name); }
#endif
	};

	typedef std::vector<kjm::optionInfo>	optionInfoList;

	//-------------------------------------------------------------------------
	// �R�}���h���C����̓N���X
	//-------------------------------------------------------------------------
	class cmdLine {
	public:
		// �f�t�H���g�R���X�g���N�^
		cmdLine() {}

		// �f�X�g���N�^
		virtual ~cmdLine() {}

		// �R�}���h���C�����
		virtual int parse(int argc, TCHAR** argv, const optionInfoList& opts);

		// �R�}���h���C�����
		virtual int parse(int argc, TCHAR** argv) { return parse(argc, argv, optionInfoList()); }

		// �R�}���h���C�����
		virtual int chkarg(int argc, TCHAR **argv, const _tstring &shortOpts);

	public:
		// �I�v�V���������邩�ǂ������ׂ�
		bool hasOption(const _tstring &key) const
			{ return (m_options.find(key) != m_options.end()); }

		// �I�v�V�����̈������擾����
		_tstring getOptArgument(const _tstring &key) const
			{ return (hasOption(key) ? (*m_options.find(key)).second : _T("")); }

		// �I�v�V�����̈������擾����	
		_tstring get_option_args(TCHAR key) { return getOptArgument(_tstring(1, key)); }

		// �I�v�V�����ȊO�̈����̐����擾����
		int get_argCount() { return this->m_arguments.size(); }

		// N �Ԗڂ̃I�v�V�����ȊO�̈������擾����
		_tstring get_argument(int n) { return this->m_arguments.at(n); }

		// �I�v�V����������ꍇ�A�����Ȃ��̂Ƃ��� 1 �A�����L�̂Ƃ��� 2 ��Ԃ��B�I�v�V�������Ȃ���� 0
		int get_option_state(const _tstring &key)
			{ return (this->hasOption(key)) ? 0 : ( (this->m_options[key].empty()) ? 1 : 2 ); }

		// �I�v�V����������ꍇ�A�����Ȃ��̂Ƃ��� 1 �A�����L�̂Ƃ��� 2 ��Ԃ��B�I�v�V�������Ȃ���� 0
		int get_option_state(TCHAR key) { return get_option_state(_tstring(1, key)); }

	private:
		// �I�v�V�����Ƃ��̈���
		std::map<_tstring, _tstring> m_options;

		// �I�v�V�����ȊO�̈���
		std::vector<_tstring> m_arguments;
	};

};
