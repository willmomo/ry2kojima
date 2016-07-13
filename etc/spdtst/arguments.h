//---------------------------------------------------------------------
// [�T�v]
// argc, argv ����͂���N���X(getopt���g�p)

#pragma once

#if !defined(__ARGUMENTS_H)
#define __ARGUMENTS_H

#include <deque>
#include <map>
#include <string>
#include "getopt.h"

//---------------------------------------------------------------------
// [�T�v]
// argc, argv ����͂���N���X(getopt���g�p)
//---------------------------------------------------------------------

class CArguments
{
private:
	std::map<std::string, std::string> m_options;	// �L�[=�X�C�b�`, �l=�X�C�b�`�̒l
	std::deque<std::string> m_arguments;		// option�ȊO�̈���
	std::deque<std::string> m_erroptions;		// �G���[�I�v�V����

public:
	CArguments(){};
	CArguments(int argc, char **argv, struct option *long_options)
		{ ParseArgument(argc, argv, long_options); };

	virtual ~CArguments(){};

public:
	bool has_error()
		{ return (m_erroptions.size() != 0) ? true : false; };

	bool has_option(const char *option)
		{ return (m_options.find(option) != m_options.end()) ? true : false; };

	std::string get_option(const char *option)
		{ return m_options[option]; };

	int get_argument_count()
		{ return m_arguments.size(); };

	std::string get_argument(int index)
		{ return m_arguments[index]; };

	int ParseArgument(int argc, char **argv, struct option *long_options);
};

#endif
