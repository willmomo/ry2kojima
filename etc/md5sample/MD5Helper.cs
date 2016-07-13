using System;
using System.Collections.Generic;
//using System.Linq;
//using System.Text;

using System.Security.Cryptography;
using System.IO;
using System.Text.RegularExpressions;

namespace Kjm {
	class MD5Helper {

		/// <summary>
		/// 指定したファイルのmd5値を計算する
		/// </summary>
		/// <param name="path">md5値を計算するファイルの名前</param>
		/// <returns></returns>
		public static string CalcMD5(string path) {
			string result = null;
			using (var fs = new FileStream(path, FileMode.Open, FileAccess.Read)) {
				result = BitConverter.ToString(MD5.Create().ComputeHash(fs)).Replace("-", "").ToLower();
			}
			return result;
		}

		/// <summary>
		/// MD5ファイルから、保存されているMD5値を探す
		/// </summary>
		/// <param name="md5path">MD5ファイルの名前</param>
		/// <param name="fileName">MD5ファイルから探すファイルのファイル名</param>
		/// <returns></returns>
		public static string FindMD5(string md5path, string fileName) {
			string result = null;
			var rx = new Regex(@"\s+");
			foreach (var line in File.ReadAllLines(md5path)) {
				var cols = rx.Split(line);
				if (cols != null && cols.Length == 2) {
					if (Path.GetFileName(cols[1]) == fileName) {
						result = cols[0];
						break;
					}
				}
			}
			return result;
		}

		/// <summary>
		/// 同名の拡張子がMD5のファイルから、保存されているMD5値を探す
		/// </summary>
		/// <param name="path">MD5ファイルに記録されているファイルの名前</param>
		/// <returns></returns>
		public static string FindMD5(string path) {
			return FindMD5(Path.ChangeExtension(path, "md5"), Path.GetFileName(path));
		}

		/// <summary>
		/// 指定されたファイルのMD5値が、MD5ファイルと一致するかどうか調べる
		/// </summary>
		/// <param name="path"></param>
		/// <param name="md5path"></param>
		/// <returns>0 MD5が一致、1 MD5が不一致、-1 対象ファイルがMD5ファイルに存在しない</returns>
		public static int CheckMD5(string path, string md5path) {
			int result = -1;	// 「MD5ファイルに対象の情報がない」で初期化

			// ファイル名からMD5の情報を取り出す。
			var md5sum_org = FindMD5(md5path, Path.GetFileName(path));
			if (md5sum_org != null) {
				if (CalcMD5(path) == md5sum_org) {
					result = 0;	// MD5の一致
				} else {
					result = 1;	// MD5の不一致
				}
			}

			return result;
		}

		/// <summary>
		/// 指定されたファイルのMD5値が、同名の拡張子がMD5のMD5ファイルと一致するかどうか調べる
		/// </summary>
		/// <param name="path"></param>
		/// <returns>0 MD5が一致、1 MD5が不一致、-1 対象ファイルがMD5ファイルに存在しない</returns>
		public static int CheckMD5(string path) {
			return CheckMD5(path, Path.ChangeExtension(path, "md5"));
		}
	}
}
