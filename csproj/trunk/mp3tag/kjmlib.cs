//---------------------------------------------------------------------
// ver.0.2014.11.27
//---------------------------------------------------------------------
using System;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Collections.Generic;
using System.Collections;

namespace Kjm {
	class OptionInfo {
		// ロングオプション文字列
		public string LongOption {
			get;
			set;
		}

		// ショートオプション文字
		public char ShortOption {
			get;
			set;
		}

		// 0 = オプションなし, 1 = オプションをとりうる, 2 = オプションが必須
		public int OptionArgInfo {
			get;
			set;
		}

		// オプションの説明
		public string OptDescription {
			get;
			set;
		}

		// 引数の説明
		public string ArgDescription {
			get;
			set;
		}

		public OptionInfo() {
			LongOption = "";
			ShortOption = '\0';
			OptionArgInfo = 0;
			OptDescription = "";
			ArgDescription = "";
		}

		public OptionInfo(string longOption, char shortOption, int optionArgInfo, string optDescription = "", string argDescription = "") {
			LongOption = longOption;
			ShortOption = shortOption;
			OptionArgInfo = optionArgInfo;
			OptDescription = optDescription;
			ArgDescription = argDescription;
		}
	}

	class CmdLine {
		private List<string> m_arguments = new List<string>();

		public List<string> Arguments {
			get { return m_arguments; }
		}

		private Hashtable m_options = new Hashtable();

		public Hashtable Options {
			get { return m_options; }
		}

		public bool HasOption(string key) {
			return Options.ContainsKey(key);
		}

		public string GetOptArgument(string key) {
			return (string)Options[key];
		}

		public int Parse(string[] args, OptionInfo[] opts) {
			for (int i = 0; i < args.Length; i++) {
				if (args[i][0] == '-') {
					char c = args[i][1];	// オプション文字

					if (c == '-') {	// ロングオプション
						// 一致するロングオプションを検索
						int j;
						for (j = 0; j < opts.Length; j++) {
							if (args[i].Substring(2).StartsWith(opts[j].LongOption)) {
								break;
							}
						}

						if (j < opts.Length) {
							// 一致した
							Kjm.OptionInfo oi = opts[j];

							if (oi.OptionArgInfo == 0) {
								// オプションには引数が無い場合
								Options[oi.LongOption] = "";
							} else if (oi.OptionArgInfo == 2) {
								if (args[i][2 + oi.LongOption.Length] == '=') {
									// オプションの引数が続いている場合
									Options[oi.LongOption] = args[i].Substring(2 + oi.LongOption.Length + 1);
								} else {
									// どうしよう・・・
								}
							} else {
								if (args[i].Length > (2 + oi.LongOption.Length) && args[i][2 + oi.LongOption.Length] == '=') {
									// オプションの引数が続いている場合
									Options[oi.LongOption] = args[i].Substring(2 + oi.LongOption.Length + 1);
								} else {
									// オプションには引数が無い場合
									Options[oi.LongOption] = "";
								}
							}
						}
					} else {
					}
				} else {
					Arguments.Add(args[i]);
				}
			}
			return 0;
		}
	}

	class Util {
		[StructLayout(LayoutKind.Sequential)]
		public struct FLASHWINFO {
			public UInt32 cbSize;		// FLASHWINFO構造体のサイズ
			public IntPtr hwnd;			// 点滅対象のウィンドウ・ハンドル
			public UInt32 dwFlags;		// 以下の「FLASHW_XXX」のいずれか
			public UInt32 uCount;		// 点滅する回数
			public UInt32 dwTimeout;	// 点滅する間隔（ミリ秒単位）
		}

		public const UInt32 FLASHW_STOP = 0;		// 点滅を止める
		public const UInt32 FLASHW_CAPTION = 1;		// タイトルバーを点滅させる
		public const UInt32 FLASHW_TRAY = 2;		// タスクバー・ボタンを点滅させる
		public const UInt32 FLASHW_ALL = 3;			// タスクバー・ボタンとタイトルバーを点滅させる
		public const UInt32 FLASHW_TIMER = 4;		// FLASHW_STOPが指定されるまでずっと点滅させる
		public const UInt32 FLASHW_TIMERNOFG = 12;	// ウィンドウが最前面に来るまでずっと点滅させる

		[DllImport("user32.dll")]
		public static extern Int32 FlashWindowEx(ref FLASHWINFO pfwi);
	}
}
