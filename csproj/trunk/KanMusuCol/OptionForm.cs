using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace KanMusuCol {
	public partial class OptionForm : Form {
		public OptionForm() {
			InitializeComponent();
		}

		private void optionForm_Load(object sender, EventArgs e) {
			RoamingData rd = new RoamingData();

			textBox1.Text = rd.DataPath;
			textBox2.Text = rd.HugFlashPath;
		}

		private void cancelButton_Click(object sender, EventArgs e) {
			this.Close();
		}

		private void okButton_Click(object sender, EventArgs e) {
			RoamingData rd = new RoamingData();

			rd.DataPath = textBox1.Text;
			rd.HugFlashPath = textBox2.Text;

			this.Close();
		}

		private void button1_Click(object sender, EventArgs e) {
			using (FolderBrowserDialog fbd = new FolderBrowserDialog()) {
				fbd.Description = "艦娘データを保存するフォルダ";
				fbd.SelectedPath = textBox1.Text;
				fbd.ShowNewFolderButton = true;
				if (fbd.ShowDialog(this) == System.Windows.Forms.DialogResult.OK) {
					textBox1.Text = fbd.SelectedPath;
				}
			}
		}

		private void button2_Click(object sender, EventArgs e) {
			using (OpenFileDialog ofd = new OpenFileDialog()) {
				ofd.Title = "使用するHugFlashを選択";
				ofd.Filter = "アプリケーション(HugFlash.exe)|HugFlash.exe|すべてのファイル(*.*)|*.*";
				ofd.InitialDirectory = System.IO.Path.GetDirectoryName(textBox2.Text);
				ofd.FileName = System.IO.Path.GetFileName(textBox2.Text);
				if (ofd.ShowDialog(this) == System.Windows.Forms.DialogResult.OK) {
					textBox2.Text = ofd.FileName;
				}
			}
		}
	}
}
