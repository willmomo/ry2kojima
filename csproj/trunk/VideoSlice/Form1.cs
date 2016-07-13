using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.IO;

namespace VideoSlice {
	public partial class Form1 : Form {
		public Form1() {
			InitializeComponent();
		}


		delegate void SetEnabledDelegate(bool enabled);
		void SetEnabled(bool enabled) {
			if (InvokeRequired) {
				Invoke(new SetEnabledDelegate(SetEnabled), enabled);
				return;
			}
			button1.Enabled = enabled;
		}

		delegate void SetPB2MinimumDelegate(int num);
		void SetPB2Minimum(int num) {
			if (InvokeRequired) {
				Invoke(new SetPB2MinimumDelegate(SetPB2Minimum), num);
				return;
			}
			progressBar2.Minimum = num;
		}

		delegate void SetPB2MaximumDelegate(int num);
		void SetPB2Maximum(int num) {
			if (InvokeRequired) {
				Invoke(new SetPB2MaximumDelegate(SetPB2Maximum), num);
				return;
			}
			progressBar2.Maximum = num;
		}

		delegate void SetPB2ValueDelegate(int num);
		void SetPB2Value(int num) {
			if (InvokeRequired) {
				Invoke(new SetPB2ValueDelegate(SetPB2Value), num);
				return;
			}
			progressBar2.Value = num;
		}

		delegate void SetPB1MinimumDelegate(int num);
		void SetPB1Minimum(int num) {
			if (InvokeRequired) {
				Invoke(new SetPB2MinimumDelegate(SetPB1Minimum), num);
				return;
			}
			progressBar1.Minimum = num;
		}

		delegate void SetPB1MaximumDelegate(int num);
		void SetPB1Maximum(int num) {
			if (InvokeRequired) {
				Invoke(new SetPB1MaximumDelegate(SetPB1Maximum), num);
				return;
			}
			progressBar1.Maximum = num;
		}

		delegate void SetPB1ValueDelegate(int num);
		void SetPB1Value(int num) {
			if (InvokeRequired) {
				Invoke(new SetPB1ValueDelegate(SetPB1Value), num);
				return;
			}
			progressBar1.Value = num;
		}

		private void VideoConv(string path, int moveMS) {

			/*
			FolderBrowserDialog fbd = new FolderBrowserDialog();
			if (fbd.ShowDialog() == System.Windows.Forms.DialogResult.OK) {
				path = fbd.SelectedPath;
			} else {
				button1.Enabled = true;
				return;
			}
			*/


			int fcnt = 0;
			for (int i = 0; ; i += moveMS) {
				string file = Path.Combine(path, string.Format("{0:0000000}.png", i));
				if (!File.Exists(file)) {
					System.Diagnostics.Debug.WriteLine(file);
					break;
				}
				++fcnt;
			}

			int outcnt = 0;

			SetPB2Minimum(0);
			SetPB2Maximum(fcnt);

			string ofile = Path.Combine(path, "outpat.bin");

			// CA2202(二重 Dispose の警告)を抑えるために、
			// FileStream を using じゃなくて try/finally にしてるけども・・・
			// using の意味がな無くなってない？

			FileStream fsOutpat = null;
			try {
				fsOutpat = new FileStream(ofile, FileMode.Create, FileAccess.Write, FileShare.Read);
				using (BinaryWriter bw = new BinaryWriter(fsOutpat)) {
					fsOutpat = null;

					for (int i = 0; ; i += moveMS) {
						string file = Path.Combine(path, string.Format("{0:0000000}.png", i));
						if (!File.Exists(file)) {
							break;
						}

						using (Bitmap img = new Bitmap(file)) {
							for (int x = img.Width - 1; x >= 0; x--) {
								for (int y = img.Height - 1; y >= 0; y--) {
									System.Drawing.Color c = img.GetPixel(x, y);
									LedRGB lrgb = new LedRGB((byte)(c.R / 6), (byte)(c.G / 6), (byte)(c.B / 6));
									byte[,] sig = lrgb.CalcRGB();

									foreach (byte s in sig) {
										bw.Write(s);
									}
								}
							}
							++outcnt;
							SetPB2Value(outcnt);
							//System.Diagnostics.Debug.WriteLine(outcnt);
						}
					}
				}
			} finally {
				if (fsOutpat != null)
					fsOutpat.Dispose();
			}

			ofile = Path.Combine(path, "outnum.bin");
			FileStream fsOutnum = null;
			try {
				fsOutnum = new FileStream(ofile, FileMode.Create, FileAccess.Write, FileShare.Read);
				using (BinaryWriter bw = new BinaryWriter(fsOutnum)) {
					fsOutnum = null;

					bw.Write(outcnt);
				}
			} finally {
				if (fsOutnum != null)
					fsOutnum.Dispose();
			}
		}

		/*
		 * 64x32用の変換ロジック。
		 * 1. 右端から、左方向へ16ピクセル
		 * 2. 中央から、左方向へ16びくセル
		 * 3. 右側中央から、左方向へ16ピクセル
		 * 4. 左側中央から、左方向へ16ピクセル
		 * の順で構築。
		 * Y軸は、下から上に行く現在の状態と同じ。
		 */
		private void VideoConvLL(string path, int moveMS) {

			/*
			FolderBrowserDialog fbd = new FolderBrowserDialog();
			if (fbd.ShowDialog() == System.Windows.Forms.DialogResult.OK) {
				path = fbd.SelectedPath;
			} else {
				button1.Enabled = true;
				return;
			}
			*/


			int fcnt = 0;
			for (int i = 0; ; i += moveMS) {
				string file = Path.Combine(path, string.Format("{0:0000000}.png", i));
				if (!File.Exists(file)) {
					break;
				}
				++fcnt;
			}

			int outcnt = 0;

			SetPB2Minimum(0);
			SetPB2Maximum(fcnt);

			string ofile = Path.Combine(path, "outpat.bin");

			// CA2202(二重 Dispose の警告)を抑えるために、
			// FileStream を using じゃなくて try/finally にしてるけども・・・
			// using の意味がな無くなってない？

			FileStream fsOutpat = null;
			try {
				fsOutpat = new FileStream(ofile, FileMode.Create, FileAccess.Write, FileShare.Read);
				using (BinaryWriter bw = new BinaryWriter(fsOutpat)) {
					fsOutpat = null;

					for (int i = 0; ; i += moveMS) {
						string file = Path.Combine(path, string.Format("{0:0000000}.png", i));
						if (!File.Exists(file)) {
							System.Diagnostics.Debug.WriteLine(file);
							break;
						}

						using (Bitmap img = new Bitmap(file)) {

							// 1. 右端から1/4を生成
							for (int x = img.Width - 1; x >= 48; x--) {
								for (int y = img.Height - 1; y >= 0; y--) {
									System.Drawing.Color c = img.GetPixel(x, y);
									LedRGB lrgb = new LedRGB((byte)(c.R / 6), (byte)(c.G / 6), (byte)(c.B / 6));
									byte[,] sig = lrgb.CalcRGB();

									foreach (byte s in sig) {
										bw.Write(s);
									}
								}
							}

							// 2. 中央から1/4を生成
							for (int x = (img.Width * 2 / 4) - 1; x >= 16; x--) {
								for (int y = img.Height - 1; y >= 0; y--) {
									System.Drawing.Color c = img.GetPixel(x, y);
									LedRGB lrgb = new LedRGB((byte)(c.R / 6), (byte)(c.G / 6), (byte)(c.B / 6));
									byte[,] sig = lrgb.CalcRGB();

									foreach (byte s in sig) {
										bw.Write(s);
									}
								}
							}

							// 3. 中央から1/4を生成
							for (int x = (img.Width * 3 / 4) - 1; x >= 32; x--) {
								for (int y = img.Height - 1; y >= 0; y--) {
									System.Drawing.Color c = img.GetPixel(x, y);
									LedRGB lrgb = new LedRGB((byte)(c.R / 6), (byte)(c.G / 6), (byte)(c.B / 6));
									byte[,] sig = lrgb.CalcRGB();

									foreach (byte s in sig) {
										bw.Write(s);
									}
								}
							}

							// 3. 中央から1/4を生成
							for (int x = (img.Width * 1 / 4) - 1; x >= 0; x--) {
								for (int y = img.Height - 1; y >= 0; y--) {
									System.Drawing.Color c = img.GetPixel(x, y);
									LedRGB lrgb = new LedRGB((byte)(c.R / 6), (byte)(c.G / 6), (byte)(c.B / 6));
									byte[,] sig = lrgb.CalcRGB();

									foreach (byte s in sig) {
										bw.Write(s);
									}
								}
							}
							++outcnt;
							SetPB2Value(outcnt);
							//System.Diagnostics.Debug.WriteLine(outcnt);
						}
					}
				}
			} finally {
				if (fsOutpat != null)
					fsOutpat.Dispose();
			}

			ofile = Path.Combine(path, "outnum.bin");
			FileStream fsOutnum = null;
			try {
				fsOutnum = new FileStream(ofile, FileMode.Create, FileAccess.Write, FileShare.Read);
				using (BinaryWriter bw = new BinaryWriter(fsOutnum)) {
					fsOutnum = null;

					bw.Write(outcnt);
				}
			} finally {
				if (fsOutnum != null)
					fsOutnum.Dispose();
			}
		}

		// System.Xaml を参照に追加する。
		// PresentationCore を参照に追加する。
		// WindowsBase を参照に追加する。
		private void VideoSlice(string fname, WorkerParam wp) {

			string outputPath = Path.ChangeExtension(fname, "slice");
			// string outputPath = Path.Combine(Path.GetDirectoryName(fname), "slice");
			if (!Directory.Exists(outputPath)) {
				Directory.CreateDirectory(outputPath);
			} else {
				MessageBox.Show(string.Format("フォルダ {0} が、すでに存在します。", outputPath));
				return;
			}

			var player = new System.Windows.Media.MediaPlayer();

			int maxWidth = 640;
			player.ScrubbingEnabled = true;
			player.Open(new System.Uri(fname));
			player.Pause();
			Thread.Sleep(100);
			System.Diagnostics.Debug.WriteLine("IsBuffering = {0}", player.IsBuffering);

			while (player.DownloadProgress < 1.0 || player.NaturalVideoWidth == 0) {
				System.Diagnostics.Debug.WriteLine("DP {0} , NVW {1}", player.DownloadProgress, player.NaturalVideoWidth);
				Thread.Sleep(100);
			}
			System.Diagnostics.Debug.WriteLine(string.Format("NaturalDuration : {0}", player.NaturalDuration));
			maxWidth = player.NaturalVideoWidth;

			SetPB1Minimum(0);
			SetPB1Maximum(100);

			// リサイズ後のサイズを計算
			var ratio = (double)maxWidth / player.NaturalVideoWidth;

			int width = (int)(player.NaturalVideoWidth * ratio);
			int height = (int)(player.NaturalVideoHeight * ratio);

			width = wp.m_yoko;
			height = wp.m_tate;

			// レンダリングするビットマップを用意
			var bitmap = new RenderTargetBitmap(width, height, 96, 96, PixelFormats.Pbgra32);

			int moveMS = 1000 / wp.m_fps;
			for (int msec = 0; msec < player.NaturalDuration.TimeSpan.TotalMilliseconds; msec += moveMS) {

				//System.Diagnostics.Debug.WriteLine("{0}/{1}", msec, player.NaturalDuration.TimeSpan.TotalMilliseconds);
				SetPB1Value((int)(((double)msec * 100.0) / player.NaturalDuration.TimeSpan.TotalMilliseconds));

				player.Position = TimeSpan.FromMilliseconds(msec);

				Thread.Sleep(100);
				while (player.DownloadProgress < 1.0 || player.NaturalVideoWidth == 0) {
					Thread.Sleep(100);
				}
				//System.Diagnostics.Debug.WriteLine(string.Format("maxWidth : {0}", maxWidth));
				//System.Diagnostics.Debug.WriteLine(string.Format("NaturalVideoWidth : {0}", player.NaturalVideoWidth));

				// 描画用の Visual を用意
				var visual = new DrawingVisual();

				using (var context = visual.RenderOpen()) {
					context.DrawVideo(player, new System.Windows.Rect(0, 0, width, height));
				}

				// レンダリングするビットマップを用意
				//var bitmap = new RenderTargetBitmap(width, height, 96, 96, PixelFormats.Pbgra32);

				// ビットマップに Visual をレンダリング
				bitmap.Render(visual);

				// PNG として保存
				var encoder = new PngBitmapEncoder();

				encoder.Frames.Add(BitmapFrame.Create(bitmap));

				string outFile = Path.Combine(outputPath, string.Format("{0:0000000}.png", msec));
				using (var stream = File.OpenWrite(outFile)) {
					encoder.Save(stream);
				}
			}
			SetPB1Value(100);

			if (wp.m_format) {
				VideoConv(outputPath, moveMS);
			} else {
				VideoConvLL(outputPath, moveMS);
			}
		}

		private void button1_Click(object sender, EventArgs e) {
			button1.Enabled = false;
			using (var ofd = new OpenFileDialog()) {
				if (ofd.ShowDialog() == System.Windows.Forms.DialogResult.OK) {
					m_targetFileName = ofd.FileName;

					WorkerParam wp = new WorkerParam();
					wp.m_yoko = Convert.ToInt32(tbYoko.Text);
					wp.m_tate = Convert.ToInt32(tbTate.Text);
					wp.m_fps = Convert.ToInt32(tbFPS.Text);
					wp.m_format = checkBox1.Checked;

					backgroundWorker1.RunWorkerAsync(wp);
				} else {
					button1.Enabled = true;
				}
			}
		}

		private string m_targetFileName;

		private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e) {
			WorkerParam wp = (WorkerParam)e.Argument;

			VideoSlice(m_targetFileName, wp);

			//button1.Enabled = true;
			SetEnabled(true);
		}
	}
}
