/*
	kjm_optparse	ver.0.2015.3.27

	オプションの解析を行うクラス
*/

#include "kjm_optparse.h"

//=====================================================================
// class optinfo
//=====================================================================

// デフォルトコンストラクタ
kjm::optinfo::optinfo() {
	this->ShortOption = '\0';
	this->LongOption = _T("");
	this->OptArgRequirement = kjm::no_argument;

	this->HasOption = false;
	this->OptArg = _T("");
}

// オプション情報指定付きコンストラクタ
kjm::optinfo::optinfo(TCHAR shortOption, const _tstring& longOption, kjm::optarg_requirement flag) {
	this->ShortOption = shortOption;
	this->LongOption = longOption;
	this->OptArgRequirement = flag;

	this->HasOption = false;
	this->OptArg = _T("");
}

// オプション情報指定付きコンストラクタ(ショートプション無し)
kjm::optinfo::optinfo(const _tstring& longOption, kjm::optarg_requirement flag) {
	this->ShortOption = '\0';
	this->LongOption = longOption;
	this->OptArgRequirement = flag;

	this->HasOption = false;
	this->OptArg = _T("");
}

// コピーコンストラクタ
kjm::optinfo::optinfo(const kjm::optinfo& src) {
	this->ShortOption = src.ShortOption;
	this->LongOption = src.LongOption;
	this->OptArgRequirement = src.OptArgRequirement;

	this->HasOption = src.HasOption;
	this->OptArg = src.OptArg;
}

// 代入演算子
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

// コピーコンストラクタ
kjm::optparse::optparse(const kjm::optparse& src) {
	this->m_opts = src.m_opts;
}

// 代入演算子
kjm::optparse& kjm::optparse::operator=(const kjm::optparse& rhs) {
	if (this != &rhs) {
		this->m_opts = rhs.m_opts;
	}
	return *this;
}

// 利用可能なオプションを追加する
void kjm::optparse::add(const kjm::optinfo& opt) {
	// ロングオプションをキーにして、mapに追加する。
	this->m_opts[opt.LongOption] = opt;
}

// ショートオプションを指定してオプション情報を探す
kjm::optinfo& kjm::optparse::findOption(TCHAR shortOpt) {
	for (OPTION_MAP_Iter i = m_opts.begin(); i != m_opts.end(); i++) {
		if (i->second.ShortOption == shortOpt) {
			return i->second;
		}
	}
	throw kjm::invalidOption(kjm::encoding::to_mbs(_T("-") + _tstring(1, shortOpt)));
}

// ロングオプションを指定したオプション情報を探す
kjm::optinfo& kjm::optparse::findOption(const _tstring& longOpt) {
	if (m_opts.find(longOpt) != m_opts.end()) {
		return m_opts[longOpt];
	}
	throw kjm::invalidOption(kjm::encoding::to_mbs(_T("--") + longOpt));
}

// ロングオプションから連想配列風にオプション情報を取得
kjm::optinfo& kjm::optparse::operator[](const _tstring& longOpt) {
	return findOption(longOpt);
}

// オプションの解析
kjm::optparse::ARGS kjm::optparse::parse(int argc, TCHAR** argv) {
	std::list<_tstring> args;
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			TCHAR c = argv[i][1];
			if (c == '-') {
				// ロングオプションの処理
				_tstring work(&argv[i][2]);
				_tstring::size_type pos = work.find_first_of('=');
				_tstring option_name = _tstring(work, 0, pos);

				optinfo& opt = findOption(option_name);
				opt.HasOption = true;
				if (opt.OptArgRequirement == required_argument) {
					if (pos != _tstring::npos) {
						opt.OptArg = _tstring(work, pos + 1);
					} else {
						// 引数のオプションが指定されていない！
						throw kjm::missingArgument(kjm::encoding::to_mbs(_T("--") + option_name));
					}
				} else if (opt.OptArgRequirement == optional_argument) {
					if (pos != _tstring::npos) {
						opt.OptArg = _tstring(work, pos + 1);
					} else {
						// 引数のオプションが指定されていない(optionalなので正常)
					}
				}
			} else {
				// ショートプションの処理
				optinfo& opt = findOption(c);
				opt.HasOption = true;
				if (opt.OptArgRequirement == required_argument) {
					if (argv[i][2] != '\0') {
						// ショートオプションに続いてオプションの引数が書いてある時
						opt.OptArg = &argv[i][1];
					} else if (argv[i][2] == '\0' && (i + 1) < argc) {
						// ショートオプションの次の引数がある場合
						// optional_argumentと違い、必ず引数として扱う
						opt.OptArg = argv[i + 1];
						++i;
					} else {
						// 引数のオプションが指定されていない！
						throw kjm::missingArgument(kjm::encoding::to_mbs(_T("-") + _tstring(1, c)));
					}
				} else if (opt.OptArgRequirement == optional_argument) {
					if (argv[i][2] != '\0') {
						// ショートオプションに続いてオプションの引数が書いてある時
						opt.OptArg = &argv[i][1];
					} else if (argv[i][2] == '\0' && (i + 1) < argc) {
						// ショートオプションの次の引数がある場合
						if (argv[i + 1][0] != '-') {	// オプションでなければ、引数として扱う
							opt.OptArg = argv[i + 1];
							++i;
						}
					}
				}
			}
		} else {
			// オプション以外の引数
			args.push_back(argv[i]);
		}
	}
	return args;
}
