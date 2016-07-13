//---------------------------------------------------------------------
// [概要]
// argc, argv を解析するクラス(getoptを使用)

#pragma once

#if !defined(__ARGUMENTS_H)
#define __ARGUMENTS_H

#include <deque>
#include <map>
#include <string>
#include "getopt.h"

//---------------------------------------------------------------------
// [概要]
// argc, argv を解析するクラス(getoptを使用)
//---------------------------------------------------------------------

class CArguments
{
private:
	std::map<std::string, std::string> m_options;	// キー=スイッチ, 値=スイッチの値
	std::deque<std::string> m_arguments;		// option以外の引数
	std::deque<std::string> m_erroptions;		// エラーオプション

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
