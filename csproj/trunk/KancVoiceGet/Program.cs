using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.IO;
using System.Net;

namespace kancVoiceGet {
	static class Program {
		/// <summary>
		/// アプリケーションのメイン エントリ ポイントです。
		/// </summary>
		[STAThread]
		static void Main() {
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new Form1());
		}
	}

	// 艦むすの声を取得するクラス
	public class KanMusuVoice {
		public string ReferenceURL { get; set; }
		public string DestinationFolder { get; set; }

		private List<string> m_msg = new List<string>();
		public string GetMsg() {
			string result = "";
			lock (m_msg) {
				if (m_msg.Count > 0) {
					result = m_msg[0];
					m_msg.RemoveAt(0);
				}
			}
			return result;
		}

		public bool Loop { get; set; }

		public void GetVoice() {
			Loop = true;

			var rex = new Regex(@"sound/(.+)/");	// 艦むすの識別子を取得する正規表現
			var ms = rex.Matches(ReferenceURL);
			var kanmusu = ms[0].Groups[1].Value;

			// ファイルの保存先を用意する
			if (!Directory.Exists(DestinationFolder)) {
				try {
					Directory.CreateDirectory(DestinationFolder);
				} catch (Exception ex) {
					lock (m_msg) {
						m_msg.Add(string.Format("*** フォルダが作成できません: {0} ***", DestinationFolder));
						m_msg.Add(string.Format("*** {0} ***", ex.Message));
						m_msg.Add("*** 中断しました ***");
					}
					return;
				}
			}

			lock (m_msg) {
				m_msg.Clear();
			}
			var wc = new WebClient();
			for (int i = 1; i < 60; i++) {
				if (Loop == false) {
					lock (m_msg) {
						m_msg.Add("*** 中断しました ***");
						break;
					}
				}

				var address = string.Format("http://203.104.209.71/kcs/sound/{0}/{1}.mp3", kanmusu, i);
				var fileName = Path.Combine(DestinationFolder, string.Format("{0}.mp3", i));
				try {
					wc.DownloadFile(address, fileName);
					lock (m_msg) {
						m_msg.Add(string.Format("[ok]\t{0}", fileName));
					}
				}
				catch (Exception ex) {
					lock (m_msg) {
						m_msg.Add(string.Format("[ng]\t{0}", address));
						m_msg.Add(ex.Message);
					}
				}
			}
		}
	}
}
