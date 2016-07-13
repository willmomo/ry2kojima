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

			if (System.IO.File.Exists(System.IO.Path.Combine(Log.s_path, fname))) {
				FileInfo fi = new FileInfo(System.IO.Path.Combine(Log.s_path, fname));
				if (fi.Length > (1 * 1024 * 1024)) {
					// ファイルのシフト
					for (int no = 8; no >= 1; no--) {
						if (System.IO.File.Exists(System.IO.Path.Combine(Log.s_path, String.Format("{0}.{1}", fname, no)))) {
							System.IO.File.Copy(
								System.IO.Path.Combine(Log.s_path, String.Format("{0}.{1}", fname, no)),
								System.IO.Path.Combine(Log.s_path, String.Format("{0}.{1}", fname, no + 1)), true);
						}
					}

					System.IO.File.Copy(
						System.IO.Path.Combine(Log.s_path, fname),
						System.IO.Path.Combine(Log.s_path, fname + ".1"), true);
					System.IO.File.Delete(System.IO.Path.Combine(Log.s_path, fname));
				}
			}

			for (int i = 0; i < 10; i++) {
				try {
					System.IO.File.AppendAllText(
						System.IO.Path.Combine(Log.s_path, fname),
						DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss.fff : ") + msg + "\r\n");
					break;
				} catch {
					Thread.Sleep(0);
				}
			}
		}
	}

	public class Path {
		public static string PathAddBackslash(string path) {
			// They're always one character but EndsWith is shorter than
			// array style access to last path character. Change this
			// if performance are a (measured) issue.
			string separator1 = System.IO.Path.DirectorySeparatorChar.ToString();
			string separator2 = System.IO.Path.AltDirectorySeparatorChar.ToString();

			// Trailing white spaces are always ignored but folders may have
			// leading spaces. It's unusual but it may happen. If it's an issue
			// then just replace TrimEnd() with Trim(). Tnx Paul Groke to point this out.
			path = path.TrimEnd();

			// Argument is always a directory name then if there is one
			// of allowed separators then I have nothing to do.
			if (path.EndsWith(separator1) || path.EndsWith(separator2))
				return path;

			// If there is the "alt" separator then I add a trailing one.
			// Note that URI format (file://drive:\path\filename.ext) is
			// not supported in most .NET I/O functions then we don't support it
			// here too. If you have to then simply revert this check:
			// if (path.Contains(separator1))
			//     return path + separator1;
			//
			// return path + separator2;
			if (path.Contains(separator2))
				return path + separator2;

			// If there is not an "alt" separator I add a "normal" one.
			// It means path may be with normal one or it has not any separator
			// (for example if it's just a directory name). In this case I
			// default to normal as users expect.
			return path + separator1;
		}

		public static string PathRemoveBackslash(string path) {
			string separator1 = System.IO.Path.DirectorySeparatorChar.ToString();
			string separator2 = System.IO.Path.AltDirectorySeparatorChar.ToString();

			path = path.TrimEnd();

			if (!path.EndsWith(separator1) && !path.EndsWith(separator2))
				return path;

			return path.Substring(path.Length - 1);
		}
	}

	public class File {
		public static string ExpandPath(string file, string path = "") {
			string cd = Environment.CurrentDirectory;
			string result;
			if (path == "") {
				result = System.IO.Path.GetFullPath(file);
			} else {
				string basePath = Path.PathAddBackslash(path);
				string filePath = file;

				//"%"を"%25"に変換しておく（デコード対策）
				basePath = basePath.Replace("%", "%25");
				filePath = filePath.Replace("%", "%25");

				//絶対パスを取得する
				Uri u1 = new Uri(basePath);
				Uri u2 = new Uri(u1, filePath);
				string absolutePath = Path.PathRemoveBackslash(u2.LocalPath);
				//"%25"を"%"に戻す
				result = absolutePath.Replace("%25", "%");
			}
			return result;
		}
	}

	public class FileUtils {
		public static bool Cmp(string file1, string file2) {

			if (file1 == file2) {
				return true;
			}

			// 二つのファイルを開く
			FileStream fs1 = new FileStream(file1, FileMode.Open);
			FileStream fs2 = new FileStream(file2, FileMode.Open);

			// ファイルサイズが違えば不一致
			if (fs1.Length != fs2.Length) {
				fs1.Close();
				fs2.Close();
				return false;
			}

			int file1byte;
			int file2byte;
			do {
				file1byte = fs1.ReadByte();
				file2byte = fs2.ReadByte();
			} while ((file1byte == file2byte) && (file1byte != -1));

			fs1.Close();
			fs2.Close();

			return ((file1byte - file2byte) == 0);
		}
	}
}
