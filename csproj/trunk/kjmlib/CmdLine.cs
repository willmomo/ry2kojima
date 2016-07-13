using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace Kjm {

	/*
	 * コマンドライン解析クラス
	 * 
	 * Named プロパティ(Dictionary)にオプションとオプションの引数が入る
	 * Unnamed プロパティに(List)にオプション以外の引数が入る
	 * 
	 * --- 使用例 ---
	 * Kjm.CmdLine obj = new Kjm.CmdLine();
	 * obj.Parse(args);
	 * if (obj.Named.ContainsKey("option")) {
	 *   if (obj.Named["option"] != null) {
	 *     otpionArgument = obj.Named["option"];
	 *   }
	 * }
	 * for (int i = 0; i < obj.Unnamed.Count; i++) {
	 *   AnyAction(obj.Unnamed[i], optionArgument);
	 * }
	 * */
	class CmdLine {
		private Dictionary<string, string> m_named = new Dictionary<string, string>();
		public Dictionary<string, string> Named {
			get {
				return m_named;
			}
		}

		private List<string> m_unnamed = new List<string>();
		public List<string> Unnamed {
			get {
				return m_unnamed;
			}
		}

		public void Parse(string[] args) {
			Regex reg = new Regex(@"^[\-/]{1,2}([^\-/]+?)([:=](.+))?$", RegexOptions.IgnoreCase);

			foreach (var arg in args) {
				if (reg.IsMatch(arg)) {
					var matches = reg.Match(arg);
					m_named.Add(matches.Groups[1].Value, (matches.Groups[3].Success ? matches.Groups[3].Value : null));
				} else {
					m_unnamed.Add(arg);
				}
			}
		}
	}
}
