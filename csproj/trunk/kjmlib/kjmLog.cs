//---------------------------------------------------------------------
// Kjm.Log version 1 - 2015/07/17
//---------------------------------------------------------------------
using System;
using System.IO;
using System.Threading;
using System.Runtime.InteropServices;
using System.Reflection;

namespace Kjm {
	public class Log {
		private static string s_path = System.IO.Path.Combine(System.Environment.GetEnvironmentVariable("TEMP"), "log");
		private static string s_name = System.IO.Path.GetFileNameWithoutExtension(Assembly.GetEntryAssembly().Location);

		public static void SetLogPath(string path) {
			Log.s_path = System.IO.Path.GetFullPath(path);
		}

		public static void Write(string msg) {
			if (!Directory.Exists(Log.s_path)) {
				// フォルダが無ければ、作成する。
				Directory.CreateDirectory(Log.s_path);
			}

			string fname = s_name + ".log";

			if (File.Exists(Path.Combine(Log.s_path, fname))) {
				FileInfo fi = new FileInfo(Path.Combine(Log.s_path, fname));
				if (fi.Length > (1 * 1024 * 1024)) {
					// ファイルのシフト
					for (int no = 8; no >= 1; no--) {
						if (File.Exists(Path.Combine(Log.s_path, String.Format("{0}.{1}", fname, no)))) {
							File.Copy(
								Path.Combine(Log.s_path, String.Format("{0}.{1}", fname, no)),
								Path.Combine(Log.s_path, String.Format("{0}.{1}", fname, no + 1)), true);
						}
					}

					File.Copy(
						Path.Combine(Log.s_path, fname),
						Path.Combine(Log.s_path, fname + ".1"), true);
					File.Delete(Path.Combine(Log.s_path, fname));
				}
			}

			for (int i = 0; i < 10; i++) {
				try {
					File.AppendAllText(
						Path.Combine(Log.s_path, fname),
						DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss.fff : ") + msg + "\r\n");
					break;
				} catch {
					Thread.Sleep(0);
				}
			}
		}
	}
}
