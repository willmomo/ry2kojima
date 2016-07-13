using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace kjm {
	enum OptArgInfo : int {
		NoArgument = 0,			// 引数を取らないオプション
		OptionalArgument = 1,	// 引数を取ってもとらなくてもよいオプション
		RequiredArgument = 2,	// 必ず引数を取るオプション
	};

	class OptInfo {
		public char ShortOption { get; set; }
		public string LongOption { get; set; }
		public OptArgInfo OptArgInfo { get; set; }
		//----
		public bool On { get; set; }	// フラグが指定されたときは true (default: false)

		public OptInfo() {
		}

		public OptInfo(char shortOption, string longOption, OptArgInfo optArgInfo) {
			ShortOption = shortOption;
			LongOption = longOption;
			OptArgInfo = optArgInfo;
			//----
			On = false;
		}

		public OptInfo(char shortOption) {
			ShortOption = shortOption;
		}

		public OptInfo(string longOption) {
			LongOption = longOption;
		}
	}

	class CmdLine {
		private List<OptInfo> m_optInfo = new List<OptInfo>();

		// 未定義のオプションを発見した時、このリストに加える
		public List<OptInfo> ErrorOptions { get; internal set; }

		public List<string> Arguments { get; internal set; }

		public CmdLine() {
			Arguments = new List<string>();
		}

		public void AddOption(OptInfo optInfo) {
			m_optInfo.Add(optInfo);
		}

		public void Parse(string[] args) {
			string pattern = "^--(.+)?(=.*)?$";
			for (int i = 0; i < args.Length; i++) {
				string oneArg = args[i];
				char c1 = oneArg[0];
				if (c1 == '-') {
					char c2 = oneArg[1];
					if (c2 == '-') {
						// 二文字目が '-' のときはロングオプションとして処理する
						int index = m_optInfo.FindIndex(x => x.LongOption == oneArg.Substring(2));
						if (index >= 0) {
							// ロングオプションは定義されている
							m_optInfo[index].On = true;

							// オプションに付けられた引数を取っておく
							string optionArgument = null;

							int eqIndex = oneArg.IndexOf('=');
							if (eqIndex >= 0) {
								// --option=argument 形式で引数が指定されている
								optionArgument = oneArg.Substring(eqIndex + 1);
							} else {
								if ((i + 1) < args.Length) {
									// --option argument 形式で引数が指定されている
									optionArgument = args[i + 1];
								} else {
									// --option の後ろに次のオプションが指定されていない
									// optionArgument は null のまま
								}
							}
						} else {
							// 定義されていないロングオプションが指定された
							ErrorOptions.Add(new OptInfo(oneArg.Substring(2)));
						}
					} else {
						// ショートオプションとして処理する
					}
				} else {
					// オプション以外の引数
					Arguments.Add(oneArg);
				}
			}
		}
	}
}
