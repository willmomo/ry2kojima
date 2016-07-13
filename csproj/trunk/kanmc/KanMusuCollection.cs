using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Net;
using Codeplex.Data;
using System.Xml;

namespace kanmc {
	class KanMusuCollection {
		public string BasePath { get; set; }
		public string m_cmdHugFlash;

		private Dictionary<int, string> m_kanMusuList = new Dictionary<int, string>();
		private Dictionary<int, string> m_NewkanMusuList;

		public KanMusuCollection() {
			m_cmdHugFlash = kanmc.Properties.Settings.Default.HugFlashPath;
			BasePath = kanmc.Properties.Settings.Default.BasePath;
		}

		// api_start2.json を読み込んで艦娘リストを作成する
		public bool LoadApiStart2() {
			string jsonFile = Path.Combine(BasePath, @"api_start2.json");

			Console.WriteLine("読み込む json ファイル: {0}", jsonFile);
			Kjm.Log.Write(string.Format("読み込む json ファイル: {0}", jsonFile));

			if (!File.Exists(jsonFile)) {
				Console.WriteLine("json ファイルがありません。");
				return false;
			}

			string jsonStr = File.ReadAllText(jsonFile);
			jsonStr = jsonStr.Substring(jsonStr.IndexOf('{'));
			var json = DynamicJson.Parse(jsonStr);

			XmlDocument document = new XmlDocument();
			XmlDeclaration declaration = document.CreateXmlDeclaration("1.0", "UTF-8", null);
			XmlElement root = document.CreateElement("root");
			document.AppendChild(declaration);
			document.AppendChild(root);
			JsonToXml(json, root, document);
			document.Save(Path.Combine(BasePath, @"api_start2.xml"));

			var api_mst_ship = json.api_data.api_mst_ship;
			var api_mst_shipgraph = json.api_data.api_mst_shipgraph;
			var api_mst_shipupgrade = json.api_data.api_mst_shipupgrade;

			foreach (var item in api_mst_ship) {
				if (item.api_name != "なし") {
					int api_sortno = item.IsDefined("api_sortno") ? (int)item.api_sortno : 0;
					m_kanMusuList[(int)item.api_id] = string.Format("{0},{1},{2},", (int)item.api_id, api_sortno, item.api_name);
				}
			}

			foreach (var item in api_mst_shipgraph) {
				if (m_kanMusuList.ContainsKey((int)item.api_id)) {
					m_kanMusuList[(int)item.api_id] += string.Format("{0}", item.api_filename);
				}
			}
			m_NewkanMusuList = new Dictionary<int, string>(m_kanMusuList);

			return true;
		}

		// 艦娘リストを api_start2.csv に保存する
		public void SaveApiStart2() {
			string csvFile = Path.Combine(BasePath, "api_start2.csv");
			File.WriteAllText(csvFile, "api_id,api_sortno,api_name,api_filename\n");
			File.AppendAllLines(csvFile, m_kanMusuList.Values);
		}

		// 艦娘リストと api_start2.csv の差分を作成する
		public bool DiffApiStart2() {
			bool ret = true;
			string csvFname = Path.Combine(BasePath, @"api_start2.csv");
			try {
				string[] lines = File.ReadAllLines(csvFname);

				for (int i = 1; i < lines.Length; i++) {
					string[] cols = lines[i].Split(',');

					int api_id = int.Parse(cols[0]);
					int api_sortno = int.Parse(cols[1]);
					string api_name = cols[2];
					string api_filename = cols[3];

					m_NewkanMusuList.Remove(api_id);
				}
			} catch (FileNotFoundException ex) {
				Console.WriteLine(ex.Message);
				Console.WriteLine("すべての艦娘データを取得します。");
			} catch (Exception ex) {
				Console.WriteLine(ex.Message);
				ret = false;
			}
			return ret;
		}

		private void JsonToXml(dynamic json, XmlElement elem, XmlDocument document) {
			if (json.IsObject) {
				foreach (var obj in json) {
					XmlElement element = document.CreateElement(obj.Key);
					if (obj.Value is double) {
						element.InnerText = obj.Value.ToString();
					} else if (obj.Value is string) {
						element.InnerText = obj.Value;
					} else if (obj.Value == null) {
						element.InnerText = "";
					} else {
						JsonToXml(obj.Value, element, document);
					}
					elem.AppendChild(element);
				}
			} else if (json.IsArray) {
				int cnt = 0;
				foreach (var obj in json) {
					XmlElement element = document.CreateElement(string.Format("item{0}", cnt++));
					if (obj is double) {
						element.InnerText = obj.ToString();
					} else if (obj is string) {
						element.InnerText = obj;
					} else {
						JsonToXml(obj, element, document);
					}
					elem.AppendChild(element);
				}
			} else {
			}
		}

		public void PrintNewKanMusuList() {
			foreach (var obj in m_NewkanMusuList) {
				Console.WriteLine("{0}\t{1}", obj.Key, obj.Value);
			}
		}

		public void GetSWF() {
			foreach (var obj in m_NewkanMusuList) {
				string[] cols = obj.Value.Split(',');
				int api_id = int.Parse(cols[0]);
				int api_sortno = int.Parse(cols[1]);
				string api_name = cols[2];
				string api_filename = cols[3];

				Kjm.Log.Write(string.Format("取り込む艦娘: {0}:{1}({2})", api_id, api_name, api_filename));

				string folderName = string.Format("{0:0000}_{1}_{2}", int.Parse(cols[0]), cols[2], cols[3]);
				string fullFolder = Path.Combine(BasePath, folderName);
				if (!Directory.Exists(fullFolder)) {
					Directory.CreateDirectory(fullFolder);
				}
				Console.WriteLine("  {0}", api_name);
				Console.WriteLine("    {0}.swf 取得", api_filename);
				if (GetSwfFromWeb(api_name, api_filename, fullFolder)) {

					Console.WriteLine("    変換");
					RunHugFlash(Path.Combine(fullFolder, cols[2] + ".swf"), fullFolder, cols[2]);
				} else {
					Console.WriteLine("    失敗");
					break;
				}

				Console.WriteLine("    音声取得");
				GetVoice(fullFolder, api_name, api_filename);
			}
		}

		private bool GetSwfFromWeb(string kanname, string fname, string outputFolder) {
			bool ret = false;
			string urlBase = @"http://203.104.209.71/kcs/resources/swf/ships/{0}.swf";
			try {
				Uri uri = new Uri(string.Format(urlBase, fname));
				WebClient wc = new WebClient();
				wc.DownloadFile(uri.OriginalString, Path.Combine(outputFolder, kanname + ".swf"));
				ret = true;
			} catch (WebException ex) {
				Console.Error.WriteLine("! ERROR: " + ex.Message);
			}
			return ret;
		}

		private void RunHugFlash(string swfName, string outputFolder, string kanname) {

			string hugOut = Path.Combine(outputFolder, kanname);
			if (Directory.Exists(hugOut)) {
				Directory.Delete(hugOut, true);
			}

			Process proc = new Process();
			proc.StartInfo.FileName = m_cmdHugFlash;
			proc.StartInfo.Arguments = "\"" + Path.Combine(outputFolder, kanname + ".swf") + "\"";

			//proc.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
			//proc.StartInfo.UseShellExecute = true;

			proc.StartInfo.CreateNoWindow = true;
			proc.StartInfo.UseShellExecute = false;

			proc.Start();
			proc.WaitForExit(10000);
		}

		private bool GetVoice(string basePath, string apiName, string apiFilename) {
			string outputFolder = Path.Combine(basePath, "Voice");
			if (!Directory.Exists(outputFolder)) {
				Directory.CreateDirectory(outputFolder);
			}

			var wc = new WebClient();
			for (int i = 1; i < 60; i++) {
				var address = string.Format("http://203.104.209.71/kcs/sound/kc{0}/{1}.mp3", apiFilename, i);
				var fileName = Path.Combine(outputFolder, string.Format("{0}.mp3", i));
				try {
					wc.DownloadFile(address, fileName);
					// Console.WriteLine("  [ok]\t{0} {1}.mp3", apiName, i);
				} catch (WebException ex) {
					if (ex.Status != WebExceptionStatus.ProtocolError) {
						Console.WriteLine("[ng]\t{0}", address);
						Console.WriteLine(ex.Message);
					}
				} catch (Exception ex) {
					Console.WriteLine("[ng]\t{0}", address);
					Console.WriteLine(ex.Message);
				}
			}

			if ((new DirectoryInfo(outputFolder)).GetFiles().Count() == 0) {
				Directory.Delete(outputFolder);
			}

			return true;
		}
	}
}
