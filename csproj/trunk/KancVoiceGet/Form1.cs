using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Text.RegularExpressions;
using System.IO;
using System.Threading;

namespace kancVoiceGet {
	public partial class Form1 : Form {
		public Form1() {
			InitializeComponent();
		}

		private void GetKanMusuVoice(object args) {
			string referenceURL = ((string[])args)[0];
			string destinationFolder = ((string[])args)[1];

			var rex = new Regex(@"sound/(.+)/");	// 艦むすの識別子を取得する正規表現
			var ms = rex.Matches(referenceURL);
			var kanmusu = ms[0].Groups[1].Value;

			// ファイルの保存先を用意する
			if (!Directory.Exists(destinationFolder)) {
				Directory.CreateDirectory(destinationFolder);
			}

			listBox1.Items.Clear();
			var wc = new WebClient();
			for (int i = 1; i < 60; i++) {
				var address = string.Format("http://203.104.209.71/kcs/sound/{0}/{1}.mp3", kanmusu, i);
				var fileName = Path.Combine(lbDestinationFolder.Text, string.Format("{0}.mp3", i));
				try {
					wc.DownloadFile(address, fileName);
					listBox1.Items.Add(string.Format("[ok]\t{0}", fileName));
				}
				catch (Exception ex) {
					listBox1.Items.Add(string.Format("[ng]\t{0}", address));
					listBox1.Items.Add(ex.Message);
				}
				listBox1.TopIndex = listBox1.Items.Count - 1;
				listBox1.Update();
			}
		}

		Thread m_thread = null;

		KanMusuVoice kmv = new KanMusuVoice();

		private void button1_Click(object sender, EventArgs e) {
			button1.Enabled = false;

			//GetKanMusuVoice(args);

			if (m_thread == null || m_thread.IsAlive == false) {
				kmv.ReferenceURL = tbReferenceURL.Text;
				kmv.DestinationFolder = lbDestinationFolder.Text;

				m_thread = new Thread(new ThreadStart(kmv.GetVoice));
				m_thread.Start();

				tbReferenceURL.Enabled = false;
				textBox2.Enabled = false;
				button1.Text = "音声取得中止(&C)";
				listBox1.Items.Clear();
				timer1.Enabled = true;
			}
			else {
				kmv.Loop = false;
				m_thread.Join();
				string msg;
				while ((msg = kmv.GetMsg()) != "") {
					listBox1.Items.Add(msg);
					listBox1.TopIndex = listBox1.Items.Count - 1;
				}
			}

			button1.Enabled = true;
		}

		private void textBox2_TextChanged(object sender, EventArgs e) {
			lbDestinationFolder.Text = Path.Combine(Environment.ExpandEnvironmentVariables(@"%USERPROFILE%\Google ドライブ\Voice\艦これ"), textBox2.Text);
		}

		private void textBox1_Enter(object sender, EventArgs e) {
			tbReferenceURL.SelectAll();
		}

		private void textBox2_Enter(object sender, EventArgs e) {
			textBox2.SelectAll();
		}

		private void timer1_Tick(object sender, EventArgs e) {
			string msg;
			while ((msg = kmv.GetMsg()) != "") {
				listBox1.Items.Add(msg);
				listBox1.TopIndex = listBox1.Items.Count - 1;
			}

			if (m_thread.IsAlive == false) {
				m_thread = null;
				timer1.Enabled = false;
				tbReferenceURL.Enabled = true;
				textBox2.Enabled = true;
				button1.Text = "音声取得(&G)";
			}
		}
	}
}
