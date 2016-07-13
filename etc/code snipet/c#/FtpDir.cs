using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.IO;
using System.Text.RegularExpressions;

namespace ConsoleApplication2 {
	class Program {

		/*
		 * ワイルドカード(正規表現)検索できる、FTPのDirコマンド。
		 * おおよその使い方
		 * 
		 * List<string> dirs = FtpDir("ftp://ts00/t2data/", "anonymous", "a@b.c", "^@00[2-3].*$");
		 * foreach (string dir in dirs) {
		 *   Console.WriteLine(dir);
		 * }
		 */
		static List<string> FtpDir(string ftpPath, string user, string pswd, string wildCard) {
			List<string> result = new List<string>();

			FtpWebRequest ftpReq = (FtpWebRequest)WebRequest.Create(ftpPath);
			ftpReq.Method = WebRequestMethods.Ftp.ListDirectory;
			ftpReq.Credentials = new NetworkCredential(user, pswd);

			using (FtpWebResponse res = (FtpWebResponse)ftpReq.GetResponse()) {
				using (Stream stream = res.GetResponseStream()) {
					using (StreamReader reader = new StreamReader(stream)) {
						while (reader.Peek() >= 0) {
							string s = reader.ReadLine();
							if (Regex.IsMatch(s, wildCard)) {
								result.Add(s);
							}
						}
						reader.Close();
					}
					stream.Close();
				}
				res.Close();
			}
			return result;
		}

		static void Main(string[] args) {
			List<string> dirs = FtpDir("ftp://ts00/t2data/", "anonymous", "a@b.c", "^@00[2-3].*$");
			foreach (string dir in dirs) {
				Console.WriteLine(dir);
			}
		}
	}
}
