using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.IO;

namespace KanmusuGet {
	class Program {
		static string RenameFile(string target) {
			FileInfo fi = new FileInfo(target);
			string ymd = fi.LastWriteTime.ToString("yyyyMMdd");

			string nam = Path.GetFileNameWithoutExtension(fi.Name);
			string ext = Path.GetExtension(fi.Name);
			string newname;
			for (int i = 1; ; i++) {
				if (i == 1) {
					newname = Path.Combine(fi.DirectoryName, string.Format("{0}_{1}{2}", nam, ymd, ext));
				} else {
					newname = Path.Combine(fi.DirectoryName, string.Format("{0}_{1}#{3}{2}", nam, ymd, ext, i));
				}
				if (File.Exists(newname) == false) {
					break;
				}
			}

			fi.MoveTo(newname);
			//fi.CopyTo(newname);

			return newname;
		}

		static void MergeAction(string mergePath, string basePath, DirectoryInfo diBasePath) {
			foreach (var file in diBasePath.GetFiles()) {
				string target = mergePath + file.FullName.Substring(basePath.Length);
				if (File.Exists(target)) {
					if (Kjm.FileUtils.Cmp(file.FullName, target) == false) {

						// mergePath にあるファイルをリネーム
						RenameFile(target);

						Console.WriteLine("update to: {0}", target);
						Directory.CreateDirectory(Path.GetDirectoryName(target));
						file.CopyTo(target);
					}
				} else {
					Console.WriteLine("copy to: {0}", target);
					Directory.CreateDirectory(Path.GetDirectoryName(target));
					file.CopyTo(target);
				}
			}
			foreach (var sdir in diBasePath.GetDirectories()) {
				MergeAction(mergePath, basePath, sdir);
			}
		}

		// mergePath += basePath する感じ
		static void Merge(string mergePath, string basePath) {
			Console.WriteLine("Merge: {0} += {1}", mergePath, basePath);
			MergeAction(mergePath, basePath, new DirectoryInfo(basePath));
		}

		static void UnpackSwf(string swfFileName) {
			Console.WriteLine("Unpack: {0}", swfFileName);

			using (Process proc = new Process()) {
				string cmdHugFlash = @"C:\tools\HugFlash\hugflash.exe";

				proc.StartInfo.FileName = cmdHugFlash;
				proc.StartInfo.Arguments = string.Format("\"{0}\"", swfFileName);

				proc.StartInfo.WindowStyle = ProcessWindowStyle.Minimized;
				proc.StartInfo.CreateNoWindow = true;
				proc.StartInfo.UseShellExecute = false;

				proc.Start();
				proc.WaitForExit(10000);
			}
		}

		static void UnpackSwfAll(DirectoryInfo path) {
			foreach (var file in path.GetFiles("*.swf")) {
				UnpackSwf(file.FullName);
			}
			foreach (var sdir in path.GetDirectories()) {
				UnpackSwfAll(sdir);
			}
		}

		static void Usage() {
			Console.WriteLine("usage: KanmusuGet [option] --json");
			Console.WriteLine(" --json <ファイル名>");
			Console.WriteLine(" --base <取得先フォルダ>");
			Console.WriteLine(" --merge <結合先フォルダ>  merge += base する");
			Console.WriteLine(" --unpack-only");
			Console.WriteLine(" --api-filename <検索したい艦むすコード>");
			Console.WriteLine(" --api-name <検索したい艦むす名>");
			Console.WriteLine(" --help");
		}

		static void Main(string[] args) {
			string jsonFile = "";
			string basePath = Environment.CurrentDirectory;
			string mergePath = "";
			bool unpackOnly = false;
			string api_filename = "";
			string api_name = "";

			if (args.Length == 0) {
				Usage();
				return;
			}

			for (int i = 0; i < args.Length; i++) {
				if (args[i] == "--json" && (i + 1) < args.Length) {
					jsonFile = Kjm.File.ExpandPath(args[++i]);
				}
				if (args[i] == "--base" && (i + 1) < args.Length) {
					basePath = Kjm.File.ExpandPath(args[++i]);
				}
				if (args[i] == "--merge" && (i + 1) < args.Length) {
					mergePath = Kjm.File.ExpandPath(args[++i]);
				}
				if (args[i] == "--api-filename" && (i + 1) < args.Length) {
					api_filename = args[++i];
				}
				if (args[i] == "--api-name" && (i + 1) < args.Length) {
					api_name = args[++i];
				}
				if (args[i] == "--unpack-only") {
					unpackOnly = true;
				}
				if (args[i] == "--help") {
					Usage();
					return;
				}
			}

			if (jsonFile == "") {
				Usage();
				return;
			}

			var obj = new KanColle.KanMusuCollection();
			obj._LoadApiStart2(jsonFile);
			var api_id = obj.get_api_id_list();

			if (api_name != "") {
				List<int> ret = obj.find_api_name(api_name);
				if (ret.Count > 0) {
					foreach (var sid in ret) {
						string ship_name = obj.api_mst_ship_name(sid);
						string ship_filename = obj.get_api_filename(sid);
						Console.WriteLine("{0} {1} {2}", sid, ship_name, ship_filename);
					}
				} else {
					Console.WriteLine("{0} に対応する艦むすは見つかりません。", api_name);
				}
				return;
			}

			if (api_filename != "") {
				string find_api_filename;
				if (api_filename.Substring(0, 2) == "kc") {
					find_api_filename = api_filename.Substring(2);
				}
				else {
					find_api_filename = api_filename;
				}

				int ret = obj.find_api_filename(find_api_filename);
				if (ret >= 0) {
					string ship_name = obj.api_mst_ship_name(ret);
					Console.WriteLine("{0} = {1} {2}", api_filename, ret, ship_name);
				}
				else {
					Console.WriteLine("{0} に対応する艦むすは見つかりません。", api_filename);
				}
				return;
			}

			if (mergePath != "") {
				Merge(mergePath, basePath);
				return;
			}

			string path = Path.Combine(basePath, DateTime.Now.ToString("yyyyMMdd"));
			Directory.CreateDirectory(path);

			if (unpackOnly == false) {
				foreach (var id in api_id) {
					string ship_name = obj.api_mst_ship_name(id);
					if (ship_name != null) {
						string shipPath = Path.Combine(path, string.Format("{0}_{1}", id, ship_name));
						Directory.CreateDirectory(shipPath);

						Console.WriteLine("GET {0}.{1} image", id, ship_name);

						//string swfPath = Path.Combine(shipPath, "image");
						//Directory.CreateDirectory(swfPath);
						string fname = string.Format("{0}_{1}.swf", id, ship_name);
						obj._GetSWF(id, Path.Combine(shipPath, fname));

						Console.WriteLine("GET {0}.{1} voice", id, ship_name);

						string voicePath = Path.Combine(shipPath, string.Format("voice", id, ship_name));
						Directory.CreateDirectory(voicePath);
						obj._GetVoice(id, voicePath);
					}
				}
			}

			if (unpackOnly) {
				UnpackSwfAll(new DirectoryInfo(path));
			}
		}
	}
}
