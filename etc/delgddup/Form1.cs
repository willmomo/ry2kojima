using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace delgddup {
	public partial class Form1 : Form {
		public Form1() {
			InitializeComponent();
		}

		// http://support.microsoft.com/kb/320348/ja
		private bool FileCompare(string file1, string file2) {
			if (file1 == file2) {
				return true;
			}

			FileStream fs1 = new FileStream(file1, FileMode.Open);
			FileStream fs2 = new FileStream(file2, FileMode.Open);

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

		private void button1_Click(object sender, EventArgs e) {
			if (FileCompare(textBox1.Text, textBox2.Text)) {
				MessageBox.Show("一致");
			} else {
				MessageBox.Show("不一致");
			}
		}
	}
}
