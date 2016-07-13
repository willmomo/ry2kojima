using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.IO;

namespace getany {
	class Program {
		static void Usage(int exitCode) {
			string msg = "";

			msg += "使用法：getany <URI>\n";
			msg += "\n";
			msg += "URIからダウンロードしたファイルをa.outに保存します。";

			Console.Out.WriteLine(msg);
			Environment.Exit(exitCode);
		}

		static int Main(string[] args) {
			// 引数のチェック
			if (args.Length != 1) {
				Usage(1);
			}

			int exitCode = 0;
			Console.Out.WriteLine("$ Download: " + args[0]);
			try {
				Uri uri = new Uri(args[0]);
				string fname = Path.GetFileName(uri.AbsolutePath);
				Console.Out.WriteLine("$ Write to: " + fname);
				
				WebClient wc = new WebClient();
				wc.DownloadFile(args[0], fname);
			} catch (WebException ex) {
				Console.Error.WriteLine("! ERROR: " + ex.Message);
				exitCode = 1;
			}

			return exitCode;
		}
	}
}
