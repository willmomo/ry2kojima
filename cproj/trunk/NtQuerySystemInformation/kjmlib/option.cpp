//
// 引数解析に関するコード
//
// option : Version 0.2015.12.24
//
// r.kojima
//
#include "option.h"

//=============================================================================
// コマンドライン解析に渡すオプション情報構造体
//=============================================================================
//---------------------------------------------------------------------
// 代入演算子
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
// コマンドライン解析クラス
//=============================================================================
//---------------------------------------------------------------------
// コマンドライン解析
//---------------------------------------------------------------------
int kjm::cmdLine::parse(int argc, TCHAR** argv, const optionInfoList& opts) {

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			TCHAR c = argv[i][1];	// オプション文字

			if (c == '-') {	// ロングオプション
				// 一致するロングオプションを検索
				size_t j;
				for (j = 0; j < opts.size(); j++) {
					if (_tcsncicmp(opts[j].m_longOption.c_str(), &argv[i][2], opts[j].m_longOption.length()) == 0) {
						break;
					}
				}
				if (j < opts.size()) {
					// 一致した
					const kjm::optionInfo& oi = opts[j];

					if (oi.m_optionArgInfo == kjm::no_argument) {
						// オプションには引数が無い場合
						this->m_options[oi.m_longOption] = _T("");
					} else if (oi.m_optionArgInfo == kjm::required_argument) {
						if (argv[i][2 + oi.m_longOption.length()] == '=') {
							// オプションの引数が続いている場合
							this->m_options[oi.m_longOption] = &argv[i][2 + oi.m_longOption.length() + 1];
						} else {
							// どうしよう・・・
							// assert(0);
						}
					} else {
						if (argv[i][2 + oi.m_longOption.length()] == '=') {
							// オプションの引数が続いている場合
							this->m_options[oi.m_longOption] = &argv[i][2 + oi.m_longOption.length() + 1];
						} else {
							// オプションには引数が無い場合
							this->m_options[oi.m_longOption] = _T("");
						}
					}
				}
			} else {
				kjm::optionInfo oi(std::basic_string<TCHAR>(1, c), std::basic_string<TCHAR>(1, c), kjm::optional_argument);
	#if _MSC_VER <= 1200	// VC++6以下
				// 検索のためだけの関数オブジェクトが必要になってしまう。
				// 本来の言語仕様では、このようなクラスは不要。
				optionInfoList::const_iterator ci = 
					std::find_if(opts.begin(), opts.end(), optionInfoShortNameFinder(std::basic_string<TCHAR>(1, c)));
	#elif _MSC_VER <= 1500	// VC++2008
				// VC++2008では、bind2ndに指定する関数は、引数が参照になっていてはいけないため、
				// ポインタを渡す関数を特別に作成して使用している。
				// 本来の言語仕様では、このようなクラスは不要。
				optionInfoList::const_iterator ci = 
					std::find_if(opts.begin(), opts.end(),
					bind2nd(mem_fun_ref(&kjm::optionInfo::isSameShortOption_P), &std::basic_string<TCHAR>(1, c)));
	#else	// VC++2010以降
				optionInfoList::const_iterator ci = 
					std::find_if(opts.begin(), opts.end(),
					bind2nd(mem_fun_ref(&kjm::optionInfo::isSameShortOption), std::basic_string<TCHAR>(1, c)));
	#endif
				if (ci != opts.end()) {
					oi = *ci;
				}

				if (oi.m_optionArgInfo == kjm::no_argument) {
					// オプションには引数が無い場合
					this->m_options[oi.m_longOption] = _T("");
				} else if (oi.m_optionArgInfo == kjm::required_argument) {
					if (argv[i][2] != 0) {
						// オプションの引数が続いている場合
						this->m_options[oi.m_longOption] = &argv[i][2];
					} else if ((argv[i][2] == 0) && (i+1 < argc) && (argv[i+1][0] != '-')) {
						// オプションの引数が次の argv に続いている場合
						this->m_options[oi.m_longOption] = argv[++i];
					} else {
						// どうしよう・・・
						// assert(0);
					}
				} else {
					if (argv[i][2] != 0) {
						// オプションの引数が続いている場合
						this->m_options[oi.m_longOption] = &argv[i][2];
					} else if ((argv[i][2] == 0) && (i+1 < argc) && (argv[i+1][0] != '-')) {
						// オプションの引数が次の argv に続いている場合
						this->m_options[oi.m_longOption] = argv[++i];
					} else {
						// オプションには引数が無い場合
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
// コマンドライン解析
//---------------------------------------------------------------------
int kjm::cmdLine::chkarg(int argc, TCHAR **argv, const kjm::_tstring &shortOpts) {
	kjm::optionInfoList opts;

	// ショートオプション定義文字列から optionInfoList を生成する
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

