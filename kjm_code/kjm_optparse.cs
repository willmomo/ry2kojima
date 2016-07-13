/*
	kjm_optparse.h	ver.0.2015.3.26

こんな感じで使います。
<code>
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace optparse {
	class Program {
		static void Main(string[] args) {

			kjm.optparse opts = new kjm.optparse();

			opts.add(new kjm.optinfo('h', "help", kjm.optarg_requirement.no_argument));
			opts.add(new kjm.optinfo('\0', "version", kjm.optarg_requirement.no_argument));
			opts.add(new kjm.optinfo('f', "file", kjm.optarg_requirement.required_argument));

			try {
				List<string> arg = opts.parse(args);

				foreach (string one in arg) {
					string s = one;
				}

				bool b = opts["help"].HasOption;
				b = opts["version"].HasOption;

			} catch(Exception ex) {
				string p = ex.Message;
			}

		}
	}
}
</code>
*/

using System;
using System.Collections.Generic;

namespace kjm {
	using ARGS = List<string>;

	//=================================================================
	// 引数が必要なオプションに引数が与えられなかった場合の例外
	//=================================================================

	public class missingArgument : Exception {
		public missingArgument(string msg) : base(("missing argument: " + msg)) {}
	}

	//=================================================================
	// 定義されていないオプションが与えられた場合の例外
	//=================================================================

	public class invalidOption : Exception {
		public invalidOption(string msg) : base(("invalid option: " + msg)) {}
	}

	//=================================================================
	// オプションの引数の「必要条件」を表す数値
	//=================================================================

	public enum optarg_requirement {
		no_argument = 0,
		required_argument = 1,
		optional_argument = 2
	}

	//=================================================================
	// オプションの情報を指定するクラス
	//=================================================================

	public class optinfo {
		// ショートオプション
		public char ShortOption { get; set; }

		// ロングオプション
		public string LongOption { get; set; }

		// オプション引数の「必要条件」
		public optarg_requirement OptArgRequirement { get; set; }

		// ここから先の変数はオプションの解析結果を収める

		// このオプションが指定されていたとき、true
		public bool HasOption { get; set; }

		// このオプションの引数
		public string OptArg { get; set; }

		// オプション情報指定付きコンストラクタ
		public optinfo(char shortOption, string longOption, optarg_requirement flag) {
			this.ShortOption = shortOption;
			this.LongOption = longOption;
			this.OptArgRequirement = flag;

			this.HasOption = false;
			this.OptArg = "";
		}
	}

	//=================================================================
	// オプションを解析するクラス
	//=================================================================

	public class optparse {

		//typedef std::map<std::basic_string<TCHAR>, optinfo>	OPTION_MAP;
		//typedef std::map<std::basic_string<TCHAR>, optinfo>::iterator	OPTION_MAP_Iter;

		private Dictionary<string, optinfo> m_opts = new Dictionary<string, optinfo>();	// 管理するオプション情報

		// ショートオプションを指定してオプション情報を探す
		public optinfo findOption(char shortOpt) {
			foreach (KeyValuePair<string, optinfo> pair in m_opts) {
				if (pair.Value.ShortOption == shortOpt) {
					return pair.Value;
				}
			}
			throw new invalidOption("-" + shortOpt);
		}

		// ロングオプションを指定したオプション情報を探す
		public optinfo findOption(string longOpt) {
			if (m_opts.ContainsKey(longOpt)) {	
				return m_opts[longOpt];
			}
			throw new invalidOption("--" + longOpt);
		}

		// デフォルトコンストラクタ
		public optparse() {}

		// ロングオプションから連想配列風にオプション情報を取得
		public optinfo this [string longOpt] {
			get {
				return findOption(longOpt);
			}
		}

		// 利用可能なオプションを追加する
		public void add(optinfo opt) {
			m_opts[opt.LongOption] = opt;
		}

		// オプションの解析
		public ARGS parse(string[] args) {
			ARGS result = new ARGS();
			for (int i = 0; i < args.Length; i++) {
				if (args[i][0] == '-') {
					char c = args[i][1];
					if (c == '-') {
						// ロングオプションの処理
						string work = args[i].Substring(2);
						int pos = work.IndexOf('=');
						string option_name = (pos == -1) ? work : work.Substring(0, pos);

						optinfo opt = findOption(option_name);
						opt.HasOption = true;
						if (opt.OptArgRequirement == kjm.optarg_requirement.required_argument) {
							if (pos != -1) {
								opt.OptArg = work.Substring(pos + 1);
							} else {
								// 引数のオプションが指定されていない！
								throw new kjm.missingArgument("--" + option_name);
							}
						} else if (opt.OptArgRequirement == kjm.optarg_requirement.optional_argument) {
							if (pos != -1) {
								opt.OptArg = work.Substring(pos + 1);
							} else {
								// 引数のオプションが指定されていない(optionalなので正常)
							}
						}
					} else {
						// ショートプションの処理
						optinfo opt = findOption(c);
						opt.HasOption = true;
						if (opt.OptArgRequirement == kjm.optarg_requirement.required_argument) {
							if (args[i][2] != '\0') {
								// ショートオプションに続いてオプションの引数が書いてある時
								opt.OptArg = args[i].Substring(1);
							} else if (args[i][2] == '\0' && (i + 1) < args.Length) {
								// ショートオプションの次の引数がある場合
								// optional_argumentと違い、必ず引数として扱う
								opt.OptArg = args[i + 1];
								++i;
							} else {
								// 引数のオプションが指定されていない！
								throw new missingArgument("-" + c);
							}
						} else if (opt.OptArgRequirement == kjm.optarg_requirement.optional_argument) {
							if (args[i][2] != '\0') {
								// ショートオプションに続いてオプションの引数が書いてある時
								opt.OptArg = args[i].Substring(1);
							} else if (args[i][2] == '\0' && (i + 1) < args.Length) {
								// ショートオプションの次の引数がある場合
								if (args[i + 1][0] != '-') {	// オプションでなければ、引数として扱う
									opt.OptArg = args[i + 1];
									++i;
								}
							}
						}
					}
				} else {
					// オプション以外の引数
					result.Add(args[i]);
				}
			}
			return result;
		}

	}
}
