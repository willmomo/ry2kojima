using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.IO;

namespace ConcatGsvdLog {
	public partial class frmMain : Form {
		private class WorkerArgument {
			public string m_pathIn;		// 読込先
			public string m_pathOut;	// 出力先

			public WorkerArgument(string pathIn, string pathOut) {
				m_pathIn = pathIn;
				m_pathOut = pathOut;
			}
		}

		public frmMain() {
			InitializeComponent();
		}

		private void btnSrcBrowse_Click(object sender, EventArgs e) {
			using (FolderBrowserDialog fbd = new FolderBrowserDialog()) {
				fbd.Description = @"gsvdlogのあるフォルダ（通常：C:\gsvdlog）";
				fbd.ShowNewFolderButton = false;
				fbd.SelectedPath = txtSrc.Text;
				if (fbd.ShowDialog(this) == DialogResult.OK) {
					txtSrc.Text = fbd.SelectedPath;
					if (txtDst.Text == "") {
						txtDst.Text = txtSrc.Text + "_out";
					}
				}
			}
		}

		private void btnDstBrowse_Click(object sender, EventArgs e) {
			using (FolderBrowserDialog fbd = new FolderBrowserDialog()) {
				fbd.Description = @"結合ファイルの出力先";
				fbd.ShowNewFolderButton = true;
				fbd.SelectedPath = txtDst.Text;
				if (fbd.ShowDialog(this) == DialogResult.OK) {
					txtDst.Text = fbd.SelectedPath;
				}
			}
		}

		private void btnConcat_Click(object sender, EventArgs e) {
			if (txtSrc.Text == "") {
				MessageBox.Show("ログフォルダを指定してください。", "ConcatGsvdLogエラー", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return;
			}
			if (txtDst.Text == "") {
				MessageBox.Show("出力先フォルダを指定してください。", "ConcatGsvdLogエラー", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return;
			}
			btnConcat.Enabled = false;
			btnCancel.Enabled = true;
			bgWorker.RunWorkerAsync(new WorkerArgument(txtSrc.Text, txtDst.Text));
		}

		private void bgWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e) {
			progBar.Value = 100;
			lblOutput.Text = "";
			MessageBox.Show("結合完了しました。", "gsvdlogの結合", MessageBoxButtons.OK, MessageBoxIcon.Information);

			progBar.Value = 0;
			lblOutput.Text = "";
			btnConcat.Enabled = true;
			btnCancel.Enabled = false;
		}

		private void bgWorker_ProgressChanged(object sender, ProgressChangedEventArgs e) {
			progBar.Value = e.ProgressPercentage;
			lblOutput.Text = (string)e.UserState;
		}

		private void bgWorker_DoWork(object sender, DoWorkEventArgs e) {
			WorkerArgument args = (WorkerArgument)e.Argument;
			BackgroundWorker worker = (BackgroundWorker)sender;

			if (!System.IO.Directory.Exists(args.m_pathOut)) {
				System.IO.Directory.CreateDirectory(args.m_pathOut);
			}

			List<FileInfo> fiList = new List<FileInfo>();
			List<DirectoryInfo> diList = new List<DirectoryInfo>((new System.IO.DirectoryInfo(args.m_pathIn)).GetDirectories());
			while (diList.Count > 0) {
				if (worker.CancellationPending) {
					return;
				}

				diList.AddRange(diList[0].GetDirectories());
				fiList.AddRange(diList[0].GetFiles());

				diList.RemoveAt(0);
			}

			fiList.Sort(delegate(FileInfo x, FileInfo y) {
				return x.Name.CompareTo(y.Name);
			});

			int cnt = 0;
			string pattern = @"^[0-9]+_[0-9]+\.log$";
			foreach (FileInfo fi in fiList) {
				++cnt;
				worker.ReportProgress((cnt * 100) / fiList.Count, fi.FullName);

				if (worker.CancellationPending) {
					break;
				}

				string input = fi.Name;
				if (Regex.IsMatch(input, pattern)) {
					string output = Path.Combine(args.m_pathOut, fi.Name.Substring(0, fi.Name.IndexOf('_')) + ".log");
					using (StreamWriter sw = new StreamWriter(output, true)) {
						using (StreamReader sr = fi.OpenText()) {
							sw.Write(sr.ReadToEnd());
						}
					}
				}
			}
		}

		private void btnCancel_Click(object sender, EventArgs e) {
			bgWorker.CancelAsync();
		}
	}
}
