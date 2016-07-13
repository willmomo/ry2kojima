using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Net;
using Codeplex.Data;
using System.Xml;
using System.Reflection;
using Ionic.Zip;

namespace KanColle {
	class KanMusuCollection {
		public string BasePath { get; set; }
		public string m_cmdHugFlash;

		public Dictionary<int, string> m_kanMusuList = new Dictionary<int, string>();
		private Dictionary<int, string> m_NewkanMusuList;

		public const string ServerIP = "203.104.209.71";

		public KanMusuCollection() {
			m_cmdHugFlash = @"C:\tools\HugFlash\hugflash.exe";
			BasePath = Path.GetDirectoryName(Assembly.GetEntryAssembly().Location);
		}

		// api_start2.json を読み込んで艦娘リストを作成する
		public bool LoadApiStart2(string jsonFile) {
			//string jsonFile = Path.Combine(BasePath, @"api_start2.json");

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

			Dictionary<int, string> api_start2 = new Dictionary<int, string>();
			foreach (var item in api_mst_ship) {
				if (item.api_name != "なし") {
					int api_sortno = item.IsDefined("api_sortno") ? (int)item.api_sortno : 0;
					int api_voicef = item.IsDefined("api_voicef") ? (int)item.api_voicef : -1;
					api_start2[(int)item.api_id] = string.Format("{0},{1},{2},{3},", (int)item.api_id, api_sortno, item.api_name,api_voicef);
				}
			}
			foreach (var item in api_mst_shipgraph) {
				if (api_start2.ContainsKey((int)item.api_id)) {
					api_start2[(int)item.api_id] += string.Format("{0}", item.api_filename);
				}
			}

			foreach (var item in api_start2) {
				if (m_kanMusuList.ContainsKey(item.Key) == false) {
					m_kanMusuList[item.Key] = item.Value;
				}
			}

			return true;
		}

		// 艦娘リストを api_start2.csv に保存する
		public void SaveApiStart2() {
			string csvFile = Path.Combine(BasePath, "api_start2.csv");
			File.WriteAllText(csvFile, "api_id,api_sortno,api_name,api_voicef,api_filename,last_modified\n");
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
					int api_voicef = int.Parse(cols[3]);
					string api_filename = cols[4];

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

		public bool LoadApiStart2Csv() {
			bool ret = true;
			string csvFname = Path.Combine(BasePath, @"api_start2.csv");
			try {
				string[] lines = File.ReadAllLines(csvFname);

				for (int i = 1; i < lines.Length; i++) {
					string[] cols = lines[i].Split(',');

					int api_id = int.Parse(cols[0]);
					int api_sortno = int.Parse(cols[1]);
					string api_name = cols[2];
					int api_voicef = int.Parse(cols[3]);
					string api_filename = cols[4];

					m_kanMusuList.Add(api_id, lines[i]);
				}
			}
			catch (FileNotFoundException ex) {
				//Console.WriteLine(ex.Message);
				//Console.WriteLine("すべての艦娘データを取得します。");
			}
			catch (Exception ex) {
				//Console.WriteLine(ex.Message);
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

		public void GetSWF(int id) {
			string[] cols = m_kanMusuList[id].Split(',');
			int api_id = int.Parse(cols[0]);
			int api_sortno = int.Parse(cols[1]);
			string api_name = cols[2];

			// 0x01 : 放置ボイスあり(old vid 29 あり)
			// 0x02 : 時報あり(old vid 30-53 あり)
			int voicef = int.Parse(cols[3]);

			string api_filename = cols[4];

			Kjm.Log.Write(string.Format("取り込む艦娘: {0}:{1}({2})", api_id, api_name, api_filename));

			DateTime dt = DateTime.Now;
			
			string folderName = string.Format("{0:0000}_{1}_{2}", int.Parse(cols[0]), cols[2], dt.ToString("yyyyMMdd"));
			string fullFolder = Path.Combine(BasePath, folderName);
			if (!Directory.Exists(fullFolder)) {
				Directory.CreateDirectory(fullFolder);
			}
			Console.WriteLine("  {0}", api_name);
			Console.WriteLine("    {0}.swf 取得", api_filename);
			if (GetSwfFromWeb(api_name, api_filename, fullFolder)) {

				Console.WriteLine("    変換");

				// swfファイルを保存する一時フォルダを決定、作成する
				string tempPath = Path.GetTempPath();
				tempPath = Path.Combine(tempPath, Path.GetRandomFileName());
				Directory.CreateDirectory(tempPath);
				File.Copy(Path.Combine(fullFolder, cols[2] + ".swf"), Path.Combine(tempPath, cols[2] + ".swf"));

				//RunHugFlash(Path.Combine(fullFolder, cols[2] + ".swf"), fullFolder, cols[2]);
				RunHugFlash(Path.Combine(tempPath, cols[2] + ".swf"), tempPath, cols[2]);

				// 圧縮ファイルとして保存する
				string outputFile = Path.Combine(fullFolder, cols[2] + ".zip");
				if (!Directory.Exists(fullFolder)) {
					Directory.CreateDirectory(fullFolder);
				}
				using (ZipFile z = new ZipFile(System.Text.Encoding.UTF8)) {
					z.AddSelectedFiles("*.*", Path.Combine(tempPath, cols[2]), "", true);
					z.Save(outputFile);
				}

				Directory.Delete(tempPath, true);
			} else {
				Console.WriteLine("    失敗");
			}

			Console.WriteLine("    音声取得");
			if (voicef != -1) {
				GetVoice(fullFolder, api_name, api_filename, api_id, voicef);
			} else {
				Console.WriteLine("    音声無し(voicef = -1)のためスキップ");
			}

			// 最終更新日を更新
			m_kanMusuList[id] = string.Format("{0},{1},{2},{3},{4},{5}", api_id, api_sortno, api_name, voicef, api_filename, dt.ToString("yyyyMMdd"));
		}

		/// <summary>
		/// 艦これサーバーにアクセスして、艦むすのswfを取得する
		/// </summary>
		/// <param name="kanname"></param>
		/// <param name="fname"></param>
		/// <param name="outputFolder"></param>
		/// <returns></returns>
		private bool GetSwfFromWeb(string kanname, string fname, string outputFolder) {
			bool ret = false;
			string urlBase = @"http://" + ServerIP + @"/kcs/resources/swf/ships/{0}.swf";
			try {
				Uri uri = new Uri(string.Format(urlBase, fname));
				using (WebClient wc = new WebClient()) {
					wc.DownloadFile(uri.OriginalString, Path.Combine(outputFolder, kanname + ".swf"));
				}
				ret = true;
			} catch (WebException ex) {
				Console.Error.WriteLine("! ERROR: " + ex.Message);
			}
			return ret;
		}

		/// <summary>
		/// hugflashを使って、画像を取り出す
		/// </summary>
		/// <param name="swfName"></param>
		/// <param name="outputFolder"></param>
		/// <param name="kanname"></param>
		private void RunHugFlash(string swfName, string outputFolder, string kanname) {

			string hugOut = Path.Combine(outputFolder, kanname);
			if (Directory.Exists(hugOut)) {
				Directory.Delete(hugOut, true);
			}

			using (Process proc = new Process()) {
				proc.StartInfo.FileName = m_cmdHugFlash;
				proc.StartInfo.Arguments = "\"" + Path.Combine(outputFolder, kanname + ".swf") + "\"";

				//proc.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
				//proc.StartInfo.UseShellExecute = true;

				proc.StartInfo.CreateNoWindow = true;
				proc.StartInfo.UseShellExecute = false;

				proc.Start();
				proc.WaitForExit(10000);
			}
		}

		// 2016/2月以降の暗号化された voice id に対応
		// http://jbbs.shitaraba.net/bbs/read.cgi/netgame/12394/1449759598/
		// no.192 にて
		/*
				192 ：名無し猫@アフィ・ロンダ系転載お断り：2016/02/21(日) 03:03:49 ID:PTCWCHqo0
				ぽい = (((((sid + 7) * 17) * (vcKey[vid] - vcKey[vid - 1])) % 0x00018365) + 0x000186A0);
				vcKey = [0x00094966, 0x00095311, 0x00095924, 0x00096637, 0x0009882A, 0x0009A6D1, 0x0009B4DC, 0x0009BBC3, 0x0009C8B2, 0x0009D27D, 
						 0x0009F830, 0x000A0CF9, 0x000A1E72, 0x000A3A3F, 0x000A517A, 0x000A768B, 0x000A80F8, 0x000AA31D, 0x000ABB22, 0x000AC037, 
						 0x000ADCB8, 0x000AE857, 0x000B0164, 0x000B222B, 0x000B4814, 0x000B50D7, 0x000B5902, 0x000B6989, 0x000B75AC, 0x000B9AC7, 
						 0x000BAB34, 0x000BC2B1, 0x000BCFF2, 0x000BE8F3, 0x000C043C, 0x000C129F, 0x000C3786, 0x000C445D, 0x000C589A, 0x000C763D, 
						 0x000C9AC6, 0x000CA31D, 0x000CB670, 0x000CCB8D, 0x000CDFF4, 0x000D04E5, 0x000D1B6C, 0x000D3F9D, 0x000D5F9C, 0x000D6EF3, 
						 0x000D7AE8, 0x000D85DD, 0x000D92A6, 0x000DAF49];
		 * */
		// voice_no 54 より上は、特別なイベント音声を下の式に当てはめて、無理やり計算させている
		// ans = (((((sid + 7) * 17) * (kkk)) % 0x00018365) + 0x000186A0);
		// ans は、実際の通信から得られる値。
		// sid は、艦むすのID
		// kkk は、上の式を満たす値。
		// 
		// voice_no=54: ホワイトデー(2016)
		private int CalcVoiceId(int sip_id, int voice_no) {
			int[] vcKey = new int[] {
				0x00094966, 0x00095311, 0x00095924, 0x00096637, 0x0009882A, 0x0009A6D1, 0x0009B4DC, 0x0009BBC3, 0x0009C8B2, 0x0009D27D,
				0x0009F830,	0x000A0CF9, 0x000A1E72, 0x000A3A3F, 0x000A517A, 0x000A768B, 0x000A80F8, 0x000AA31D, 0x000ABB22, 0x000AC037, 
				0x000ADCB8, 0x000AE857, 0x000B0164, 0x000B222B, 0x000B4814, 0x000B50D7, 0x000B5902, 0x000B6989, 0x000B75AC, 0x000B9AC7, 
				0x000BAB34, 0x000BC2B1, 0x000BCFF2, 0x000BE8F3, 0x000C043C, 0x000C129F, 0x000C3786, 0x000C445D, 0x000C589A, 0x000C763D, 
				0x000C9AC6, 0x000CA31D, 0x000CB670, 0x000CCB8D, 0x000CDFF4, 0x000D04E5, 0x000D1B6C, 0x000D3F9D, 0x000D5F9C, 0x000D6EF3, 
				0x000D7AE8, 0x000D85DD, 0x000D92A6, 0x000DAF49, 0x000DC8DC
			};

			return (((((sip_id + 7) * 17) * (vcKey[voice_no] - vcKey[voice_no - 1])) % 0x00018365) + 0x000186A0);
		}

		/// <summary>
		/// 艦これサーバーに接続して、艦むすの音声を取得する
		/// </summary>
		/// <param name="basePath"></param>
		/// <param name="apiName"></param>
		/// <param name="apiFilename"></param>
		/// <param name="sip_id"></param>
		/// <param name="voicef"></param>
		/// <returns></returns>
		private bool GetVoice(string basePath, string apiName, string apiFilename, int sip_id, int voicef) {

			// 音声ファイルを保存する一時フォルダを決定、作成する
			string tempPath = Path.GetTempPath();
			tempPath = Path.Combine(tempPath, Path.GetRandomFileName());
			Directory.CreateDirectory(tempPath);

			using (var wc = new WebClient()) {
			// 54からは、独自定義
			// 54: ホワイトデー 2016
			for (int i = 1; i <= 54; i++) {
				if (i == 29 && (voicef & 0x01) == 0) continue;
				if ((30 <= i && i <= 53) && (voicef & 0x02) == 0) continue;

					var voice_id = CalcVoiceId(sip_id, i);
					var address = string.Format("http://{0}/kcs/sound/kc{1}/{2}.mp3", ServerIP, apiFilename, voice_id);
					var fileName = Path.Combine(tempPath, string.Format("{0:00}-{1:000000000}.mp3", i, voice_id));
					try {
						wc.DownloadFile(address, fileName);
						// Console.WriteLine("  [ok]\t{0} {1:00}-{2:00000000}.mp3", apiName, i, voice_id);
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
				// (voicef & 0x04) != 0 だけの特別な放置ボイス
				// ID 129 という特別バージョン
				for (int i = 129; i <= 129; i++) {
					if ((voicef & 0x04) == 0) continue;

					var voice_id = i;	// voiceidの変換不要
					var address = string.Format("http://{0}/kcs/sound/kc{1}/{2}.mp3", ServerIP, apiFilename, voice_id);
					var fileName = Path.Combine(tempPath, string.Format("{0:000}.mp3", i));
					try {
						wc.DownloadFile(address, fileName);
						// Console.WriteLine("  [ok]\t{0} {1:00}-{2:00000000}.mp3", apiName, i, voice_id);
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
			}

			// 音声ファイルが取得できたら、圧縮ファイルとして保存する
			if ((new DirectoryInfo(tempPath)).GetFiles().Count() > 0) {
				string outputFile = Path.Combine(basePath, "voice.zip");
				if (!Directory.Exists(basePath)) {
					Directory.CreateDirectory(basePath);
				}

				using (ZipFile z = new ZipFile()) {
					z.AddSelectedFiles("*.mp3", tempPath, "", false);
					z.Save(outputFile);
				}
			}
			Directory.Delete(tempPath, true);

			return true;
		}
	}
}
