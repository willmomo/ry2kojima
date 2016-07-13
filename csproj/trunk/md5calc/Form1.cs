using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Security.Cryptography;
using System.IO;
using System.Threading;

namespace md5calc {
	public partial class Form1 : Form {
		private string m_fname;
		private string m_label2_Text = "";

		public Form1() {
			InitializeComponent();
		}

		private void label1_DragEnter(object sender, DragEventArgs e) {
			e.Effect = DragDropEffects.Copy;
		}

		private void label1_DragDrop(object sender, DragEventArgs e) {
			if (e.Data.GetDataPresent(DataFormats.FileDrop)) {
				string[] fname = (string[])e.Data.GetData(DataFormats.FileDrop, false);
				label1.Text = m_fname = fname[0];
				label2.Text = "計算中";

				bgWorker.RunWorkerAsync();
			}
		}

		private void button1_Click(object sender, EventArgs e) {
			string file_md5 = Path.ChangeExtension(label1.Text, "md5");

			using (StreamWriter sw = new StreamWriter(file_md5)) {
				sw.WriteLine("{0} *{1}", label2.Text, label1.Text);
			}
		}

		private void button2_Click(object sender, EventArgs e) {
			label1.Text = "調べるファイルをドロップしてください。";
			label2.Text = "";
			label3.Text = "";
			label4.Text = "";
		}

		private void bgWorker_DoWork(object sender, DoWorkEventArgs e) {
			byte[] md5sum = null;
			MD5 md5 = MD5.Create();
			using (FileStream file = new FileStream(m_fname, FileMode.Open, FileAccess.Read)) {
				md5sum = md5.ComputeHash(file);
				file.Close();
			}

			m_label2_Text = BitConverter.ToString(md5sum).Replace("-", "").ToLower();
		}

		private void bgWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e) {
			label2.Text = m_label2_Text;
			string fname_md5 = Path.ChangeExtension(m_fname, "md5");
			if (File.Exists(fname_md5)) {
				using (StreamReader sr = new StreamReader(fname_md5)) {
					string ss = sr.ReadLine();

					label3.Text = ss.Split(new Char[] { ' ' })[0];
				}

				if (m_label2_Text == label3.Text) {
					label4.Text = "一致：正常";
					label4.ForeColor = Color.Green;
				} else {
					label4.Text = "不一致：異常";
					label4.ForeColor = Color.Red;
				}
			} else {
				label3.Text = "";
				label4.Text = "対応するMD5ファイルなし";
			}
		}
	}
}
