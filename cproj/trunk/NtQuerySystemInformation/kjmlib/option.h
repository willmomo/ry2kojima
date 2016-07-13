//
// 引数解析に関するコード
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
	// オプションの引数の「必要条件」を表す数値
	//=================================================================

	enum optarg_requirement {
		no_argument = 0,
		required_argument = 1,
		optional_argument = 2
	};

	//-------------------------------------------------------------------------
	// コマンドライン解析に渡すオプション情報構造体
	//-------------------------------------------------------------------------
//	static const int no_argument = 0;
//	static const int optional_argument = 1;
//	static const int required_argument = 2;

	class optionInfo {
	public:
		_tstring m_longOption;	// ロングオプション文字列
		_tstring m_shortOption;	// ショートオプション文字列
		int m_optionArgInfo;					// 0 = オプションなし, 1 = オプションをとりうる, 2 = オプションが必須
		_tstring m_optDescription;	// オプションの説明
		_tstring m_argDescription;	// 引数の説明

		// デフォルトコンストラクタ
		optionInfo() : m_longOption(), m_shortOption(), m_optionArgInfo(kjm::no_argument), m_optDescription(), m_argDescription() {}

		// 初期化つきコンストラクタ
		optionInfo(const _tstring& longOption, const _tstring& shortOption, int optionArgInfo, const _tstring& optDescription = _T(""), const _tstring& argDescription = _T(""))
			: m_longOption(longOption), m_shortOption(shortOption), m_optionArgInfo(optionArgInfo), m_optDescription(optDescription), m_argDescription(argDescription) {}

		// コピーコンストラクタ
		optionInfo(const kjm::optionInfo& src)
			: m_longOption(src.m_longOption), m_shortOption(src.m_shortOption), m_optionArgInfo(src.m_optionArgInfo), m_optDescription(src.m_optDescription), m_argDescription(src.m_argDescription) {}

		// 代入演算子
		optionInfo& operator=(const kjm::optionInfo& rhs);

		// ショートオプションが同じかどうか調べる(参照バージョン)
		bool isSameShortOption(const _tstring& name) const { return (m_shortOption == name); }

		// ロングオプションが同じかどうか調べる(参照バージョン)
		bool isSameLongOption(const _tstring& name) const { return (m_longOption == name); }

#if _MSC_VER <= 1500	// VC++2008
		// ショートオプションが同じかどうか調べる(ポインタバージョン)
		bool isSameShortOption_P(const _tstring* name) const { return (m_shortOption == *name); }

		// ロングオプションが同じかどうか調べる(ポインタバージョン)
		bool isSameLongOption_P(const _tstring* name) const { return (m_longOption == *name); }
#endif
	};

	typedef std::vector<kjm::optionInfo>	optionInfoList;

	//-------------------------------------------------------------------------
	// コマンドライン解析クラス
	//-------------------------------------------------------------------------
	class cmdLine {
	public:
		// デフォルトコンストラクタ
		cmdLine() {}

		// デストラクタ
		virtual ~cmdLine() {}

		// コマンドライン解析
		virtual int parse(int argc, TCHAR** argv, const optionInfoList& opts);

		// コマンドライン解析
		virtual int parse(int argc, TCHAR** argv) { return parse(argc, argv, optionInfoList()); }

		// コマンドライン解析
		virtual int chkarg(int argc, TCHAR **argv, const _tstring &shortOpts);

	public:
		// オプションがあるかどうか調べる
		bool hasOption(const _tstring &key) const
			{ return (m_options.find(key) != m_options.end()); }

		// オプションの引数を取得する
		_tstring getOptArgument(const _tstring &key) const
			{ return (hasOption(key) ? (*m_options.find(key)).second : _T("")); }

		// オプションの引数を取得する	
		_tstring get_option_args(TCHAR key) { return getOptArgument(_tstring(1, key)); }

		// オプション以外の引数の数を取得する
		int get_argCount() { return this->m_arguments.size(); }

		// N 番目のオプション以外の引数を取得する
		_tstring get_argument(int n) { return this->m_arguments.at(n); }

		// オプションがある場合、引数なしのときは 1 、引数有のときは 2 を返す。オプションがなければ 0
		int get_option_state(const _tstring &key)
			{ return (this->hasOption(key)) ? 0 : ( (this->m_options[key].empty()) ? 1 : 2 ); }

		// オプションがある場合、引数なしのときは 1 、引数有のときは 2 を返す。オプションがなければ 0
		int get_option_state(TCHAR key) { return get_option_state(_tstring(1, key)); }

	private:
		// オプションとその引数
		std::map<_tstring, _tstring> m_options;

		// オプション以外の引数
		std::vector<_tstring> m_arguments;
	};

};
