using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;
using System.IO;
using System.Runtime.InteropServices;

namespace tfcw {
	public partial class Form1 : Form {
		private string m_threadMessage = "";

		public Form1() {
			InitializeComponent();
		}

		private void tbFile_DragEnter(object sender, DragEventArgs e) {
			if (e.Data.GetDataPresent(DataFormats.FileDrop)) {
				string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);
				if (files.Length == 1) {
					e.Effect = DragDropEffects.Copy;
				}
			}
		}

		private void tbFile_DragDrop(object sender, DragEventArgs e) {
			TextBox tb = (TextBox)sender;
			if (e.Data.GetDataPresent(DataFormats.FileDrop)) {
				string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);
				if (files.Length == 1) {
					tb.Text = files[0];
					pb.Value = 0;
				}
			}
		}

		private void btnBrowse1_Click(object sender, EventArgs e) {
			ofDlg.FileName = tbFile1.Text;
			DialogResult result = ofDlg.ShowDialog();
			if (result == System.Windows.Forms.DialogResult.OK) {
				tbFile1.Text = ofDlg.FileName;
			}
		}

		private void bnBrowse2_Click(object sender, EventArgs e) {
			ofDlg.FileName = tbFile2.Text;
			DialogResult result = ofDlg.ShowDialog();
			if (result == System.Windows.Forms.DialogResult.OK) {
				tbFile2.Text = ofDlg.FileName;
			}
		}

		private void btnCompare_Click(object sender, EventArgs e) {
			if (bgWorker.IsBusy == false) {
				// 比較を始める
				btnCompare.Text = "中止";
				tbFile1.Enabled = false;
				tbFile2.Enabled = false;
				btnBrowse1.Enabled = false;
				btnBrowse2.Enabled = false;

				bgWorker.RunWorkerAsync();
			}
			else {
				// 比較を中止する
				bgWorker.CancelAsync();
			}
		}

		private void bgWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e) {
			tbFile1.Enabled = true;
			tbFile2.Enabled = true;
			btnBrowse1.Enabled = true;
			btnBrowse2.Enabled = true;
			btnCompare.Text = "比較";

			tbOutput.Text = m_threadMessage;
			if (e.Cancelled) {
				pb.Value = 0;
			}

			// 最後にタイトルバーを点滅させてお知らせ
			Kjm.Util.FLASHWINFO fwi = new Kjm.Util.FLASHWINFO();
			fwi.cbSize = Convert.ToUInt32(Marshal.SizeOf(fwi));
			fwi.hwnd = this.Handle;
			fwi.dwFlags = Kjm.Util.FLASHW_ALL | Kjm.Util.FLASHW_TIMERNOFG;
			fwi.uCount = 0;
			fwi.dwTimeout = 0;
			Kjm.Util.FlashWindowEx(ref fwi);
		}

		private void bgWorker_ProgressChanged(object sender, ProgressChangedEventArgs e) {
			tbOutput.Text = m_threadMessage;
			pb.Value = e.ProgressPercentage;
		}

		private void bgWorker_DoWork(object sender, DoWorkEventArgs e) {
			m_threadMessage = "";
			try {
				// ファイルサイズを確認
				FileInfo fi1 = new FileInfo(tbFile1.Text);
				FileInfo fi2 = new FileInfo(tbFile2.Text);

				if (fi1.Length != fi2.Length) {
					m_threadMessage = string.Format("ファイルサイズ不一致\r\n\r\nファイル1: {0} バイト\r\nファイル2: {1} バイト", fi1.Length, fi2.Length);
					return;
				}

				int prevPercent = -1;
				long bytesRead = 0;
				using (FileStream fs1 = new FileStream(tbFile1.Text, FileMode.Open, FileAccess.Read)) {
					using (FileStream fs2 = new FileStream(tbFile2.Text, FileMode.Open, FileAccess.Read)) {
						while (bgWorker.CancellationPending == false) {
							int c1 = fs1.ReadByte();
							int c2 = fs2.ReadByte();

							bytesRead += 1;

							int percent = (int)(bytesRead * 100 / fi1.Length);
							if (prevPercent != percent) {
								m_threadMessage = string.Format("比較中 ... {0}/{1} {2}%", bytesRead, fi1.Length, percent);
								bgWorker.ReportProgress(prevPercent = percent);
							}

							if (c1 != c2) {
								m_threadMessage += " ... 不一致";
								e.Result = 1;
								break;
							} else if (c1 == -1) {
								m_threadMessage += " ... 一致";
								e.Result = 0;
								break;
							}
						}

						if (bgWorker.CancellationPending) {
							e.Cancel = true;
							e.Result = -1;
							m_threadMessage += "\r\n\r\nユーザーによる中断";
						}
					}
				}
			} catch (Exception ex) {
				m_threadMessage = ex.Message;
				e.Result = -1;
			}
		}

		private void Form1_Load(object sender, EventArgs e) {
			lblVer.Text = "Ver. " + FileVersionInfo.GetVersionInfo(System.Reflection.Assembly.GetExecutingAssembly().Location).FileVersion;
		}
	}
}
