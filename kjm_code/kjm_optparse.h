/*
	kjm_optparse	ver.0.2015.3.27

	オプションの解析を行うクラス
*/
/*
こんな感じで使います。
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

#include "kjm_encoding.h"

#include <map>
#include <list>
#include <vector>

namespace kjm {

	//=================================================================
	// 引数が必要なオプションに引数が与えられなかった場合の例外
	//=================================================================

	class missingArgument : public std::exception {
	public:
		missingArgument(const std::string& msg) : std::exception(("missing argument: " + msg).c_str()) {}
		~missingArgument() {}
	};

	//=================================================================
	// 定義されていないオプションが与えられた場合の例外
	//=================================================================

	class invalidOption : public std::exception {
	public:
		invalidOption(const std::string& msg) : std::exception(("invalid option: " + msg).c_str()) {}
		~invalidOption() {}
	};

	//=================================================================
	// オプションの引数の「必要条件」を表す数値
	//=================================================================

	enum optarg_requirement {
		no_argument = 0,
		required_argument = 1,
		optional_argument = 2
	};

	//=================================================================
	// オプションの情報を指定するクラス
	//=================================================================

	class optinfo {
	public:
		// ショートオプション
		TCHAR ShortOption;

		// ロングオプション
		_tstring LongOption;

		// オプション引数の「必要条件」
		optarg_requirement OptArgRequirement;

		// ここから先の変数はオプションの解析結果を収める

		// このオプションが指定されていたとき、true
		bool HasOption;

		// このオプションの引数
		_tstring OptArg;

	public:
		// デフォルトコンストラクタ
		optinfo();

		// デストラクタ
		virtual ~optinfo() {}

		// コピーコンストラクタ
		optinfo(const optinfo& src);

		// 代入演算子
		optinfo& operator=(const optinfo& rhs);

		// オプション情報指定付きコンストラクタ
		optinfo(TCHAR shortOption, const _tstring& longOption, optarg_requirement flag = kjm::no_argument);

		// オプション情報指定付きコンストラクタ(ショートプション無し)
		optinfo(const _tstring& longOption, kjm::optarg_requirement flag = kjm::no_argument);
	};

	//=================================================================
	// オプションを解析するクラス
	//=================================================================

	class optparse {
	public:
		typedef std::list<_tstring>	ARGS;

	private:
		typedef std::map<_tstring, optinfo>	OPTION_MAP;
		typedef std::map<_tstring, optinfo>::iterator	OPTION_MAP_Iter;

		std::map<_tstring, optinfo> m_opts;	// 管理するオプション情報

	private:
		// ショートオプションを指定してオプション情報を探す
		optinfo& findOption(TCHAR shortOpt);

		// ロングオプションを指定したオプション情報を探す
		optinfo& findOption(const _tstring& longOpt);

	public:
		// デフォルトコンストラクタ
		optparse() {}

		// デストラクタ
		virtual ~optparse() {}

		// コピーコンストラクタ
		optparse(const optparse& src);

		// 代入演算子
		optparse& operator=(const optparse& rhs);

		// ロングオプションから連想配列風にオプション情報を取得
		optinfo& operator[](const _tstring& longOpt);

		// 利用可能なオプションを追加する
		void add(const optinfo& opt);

		// オプションの解析
		ARGS parse(int argc, TCHAR** argv);
	};
}
