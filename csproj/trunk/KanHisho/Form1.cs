using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Windows;
using System.Runtime.InteropServices;
using System.Threading;
using System.Drawing.Imaging;
using Ionic.Zip;

namespace KanHisho {

	public partial class Form1 : Form {

		[DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
		public static extern IntPtr GetDC(IntPtr hWnd);

		[DllImport("gdi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
		public static extern IntPtr CreateCompatibleDC(IntPtr hdc);

		[DllImport("gdi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
		public static extern IntPtr SelectObject(IntPtr hdc, IntPtr hgdiobj);

		[DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
		public static extern int ReleaseDC(IntPtr hWnd, IntPtr hDC);

		[DllImport("gdi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
		public static extern int DeleteObject(IntPtr hobject);

		[DllImport("gdi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
		public static extern int DeleteDC(IntPtr hdc);

		public const byte AC_SRC_OVER = 0;
		public const byte AC_SRC_ALPHA = 1;
		public const int ULW_ALPHA = 2;

		[StructLayout(LayoutKind.Sequential, Pack = 1)]
		public struct BLENDFUNCTION {
			public byte BlendOp;
			public byte BlendFlags;
			public byte SourceConstantAlpha;
			public byte AlphaFormat;
		}

		[DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
		public static extern int UpdateLayeredWindow(
			IntPtr hwnd,
			IntPtr hdcDst,
			[System.Runtime.InteropServices.In()]
            ref Point pptDst,
			[System.Runtime.InteropServices.In()]
            ref Size psize,
			IntPtr hdcSrc,
			[System.Runtime.InteropServices.In()]
            ref Point pptSrc,
			int crKey,
			[System.Runtime.InteropServices.In()]
            ref BLENDFUNCTION pblend,
			int dwFlags);

		public void SetLayeredWindow(Bitmap srcBitmap) {
			// GetDeviceContext
			IntPtr screenDc = IntPtr.Zero;
			IntPtr memDc = IntPtr.Zero;
			IntPtr hBitmap = IntPtr.Zero;
			IntPtr hOldBitmap = IntPtr.Zero;
			try {
				screenDc = GetDC(IntPtr.Zero);
				memDc = CreateCompatibleDC(screenDc);
				hBitmap = srcBitmap.GetHbitmap(Color.FromArgb(0));
				hOldBitmap = SelectObject(memDc, hBitmap);

				// init BLENDFUNCTION
				BLENDFUNCTION blend = new BLENDFUNCTION();
				blend.BlendOp = AC_SRC_OVER;
				blend.BlendFlags = 0;
				blend.SourceConstantAlpha = 255;
				blend.AlphaFormat = AC_SRC_ALPHA;

				// Update Layered Window
				this.Size = new Size(srcBitmap.Width, srcBitmap.Height);
				Point pptDst = new Point(this.Left, this.Top);
				Size psize = new Size(this.Width, this.Height);
				Point pptSrc = new Point(0, 0);
				UpdateLayeredWindow(this.Handle, screenDc, ref pptDst, ref psize, memDc,
				  ref pptSrc, 0, ref blend, ULW_ALPHA);

			} finally {
				if (screenDc != IntPtr.Zero) {
					ReleaseDC(IntPtr.Zero, screenDc);
				}
				if (hBitmap != IntPtr.Zero) {
					SelectObject(memDc, hOldBitmap);
					DeleteObject(hBitmap);
				}
				if (memDc != IntPtr.Zero) {
					DeleteDC(memDc);
				}
			}
		}

		public static DpiScaler s_scaler = new DpiScaler();
		public static Kjm.MouseHook s_mouseHook = new Kjm.MouseHook();

		public Form1() {
			InitializeComponent();

			try {
				LoadHishoKan(Path.Combine(Path.GetDirectoryName(Application.ExecutablePath), @"res\hisho.png"));
			} catch (Exception ex) {
				SelectHishoKan();
				throw;
			}
		}

		void s_mouseHook_MouseProc(object sender, Kjm.MouseProcEventArgs e) {

			// マウスの動きをグローバルフックで追跡
			// マウスが来たら逃げるように、右側に寄る
			// ただし、Ctrl キーが押されているときは、逃げない。

			if ((Control.ModifierKeys & Keys.Control) != Keys.Control) {
				const int offset = 5;
				if ((e.X + offset) > m_homePosition.X) {
					this.Left = (e.X + offset);
				} else {
					this.Left = m_homePosition.X;
				}
			}
		}

		private Point m_homePosition = new Point();

		private void SetRightBottom(Size imageSize) {
			// 表示する画像に対して、
			// ・横がすべて見える
			// ・縦の65%が見える
			// ・ワークエリアの右下からの位置
			// を、ホームポジションとする。

			this.Width = imageSize.Width;
			this.Height = (int)((double)imageSize.Height * 0.65);

			Kjm.WorkArea wa = new Kjm.WorkArea();
			m_homePosition.X = this.Left = wa.Right - this.Width;
			m_homePosition.Y = this.Top = (wa.Bottom - this.Height) - 1;
		}

		private void SetRightTop(Size imageSize) {
			// 表示する画像に対して、
			// ・横がすべて見える
			// ・縦の100%が見える
			// ・ワークエリアの右上からの位置
			// を、ホームポジションとする。

			this.Width = imageSize.Width;
			this.Height = imageSize.Height;

			Kjm.WorkArea wa = new Kjm.WorkArea();
			m_homePosition.X = this.Left = wa.Right - this.Width;
			
			//m_homePosition.Y = this.Top = wa.Top + 4;
			m_homePosition.Y = this.Top = (wa.Bottom - this.Height) + 4;
		}

		private void LoadHishoKan(string fileName) {
			Bitmap bmpOrg = new Bitmap(fileName);
			bmpOrg.MakeTransparent();

			Kjm.WorkArea wa = new Kjm.WorkArea();

			//int KANMUSU_HIGHT = 430;	// 普通のサイズ
			//int KANMUSU_HIGHT = 860;	// 巨大サイズ
			int KANMUSU_HIGHT = wa.Size.Height / 2;	// ワークエリアの半分

			int H = s_scaler.SCALEY(KANMUSU_HIGHT);
			Bitmap bmp = new Bitmap((bmpOrg.Width * H) / bmpOrg.Height, H);
			Graphics g = Graphics.FromImage(bmp);
			g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
			g.DrawImage(bmpOrg, 0, 0, bmp.Width, bmp.Height);

			Kjm.Log.Write(string.Format("DPI {0} です。", g.DpiX));
			Kjm.Log.Write(string.Format("高さ {0} の艦娘を描画します(普通なら430px)。", H));
			bmpOrg.Dispose();
			g.Dispose();

			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			//this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Text = "";
			this.ControlBox = false;


			//this.BackgroundImageLayout = ImageLayout.None;
			//this.BackgroundImage = bmp;
			//this.TransparencyKey = this.BackColor;
			//this.AllowTransparency = true;

			// 上の手順より断然きれい
			// 同等のことは、API無しでできないのかしら？
			// http://www.ipentec.com/document/document.aspx?page=csharp-winform-layerd-window-create
			if (true) {
				Rectangle bmBounds = new Rectangle(0, 0, bmp.Width, bmp.Height);
				BitmapData bmpd = bmp.LockBits(bmBounds, ImageLockMode.ReadOnly,
				  bmp.PixelFormat);

				Bitmap bmp2 = new Bitmap(bmpd.Width, bmpd.Height, bmpd.Stride,
				  PixelFormat.Format32bppArgb, bmpd.Scan0);

				bmp.UnlockBits(bmpd);
				SetLayeredWindow(bmp2);
			}

			// SetRightBottom(img.Size);
			SetRightTop(bmp.Size);
		}

		private void Form1_Load(object sender, EventArgs e) {


			Kjm.User32.SetNotActiveWindow(this);

			timer1.Enabled = true;

			// ローレベルのマウスイベントをフックして
			// マウスから逃げる処理を入れる。
			// DPIの変化の問題をデバッグ中は、false にしないと都合が悪い。
			if (true) {
				s_mouseHook.SetHook();
				s_mouseHook.MouseProc += new Kjm.MouseHook.MouseProcEventHandler(s_mouseHook_MouseProc);
			}
		}

		protected override CreateParams CreateParams {
			get {
				System.Windows.Forms.CreateParams cp = base.CreateParams;

				cp.ExStyle = cp.ExStyle | WindowsConst.WS_EX_LAYERED;
				//必要に応じて WS_EX_TRANSPARENT をつける
				if (this.FormBorderStyle != FormBorderStyle.None) {
					cp.Style = cp.Style & (~WindowsConst.WS_BORDER);
					cp.Style = cp.Style & (~WindowsConst.WS_THICKFRAME);
				}

				return cp;
			}
		}
		
		private void Form1_Click(object sender, EventArgs e) {

			System.Diagnostics.Debug.WriteLine(string.Format("Top1 = {0}", this.Top));

			Balloon balloon = new Balloon();
			balloon.ShowMessage(string.Format("ただ今の時間 {0}", DateTime.Now.ToString()), this);
		}

		private int m_cnt = 0;
		private int m_muki = 1;
		private double m_deg = 0.0;

		private int prevHour = -1;

		private void timer1_Tick(object sender, EventArgs e) {
			DateTime dt = DateTime.Now;
			if ((dt.Minute % 60) == 0 && prevHour != (dt.Hour * 100 + dt.Minute)) {
				// ジャスト時報告
				prevHour = dt.Hour * 100 + dt.Minute;
				Balloon balloon = new Balloon();
				// balloon.ShowMessage(string.Format("{0} 時 {1} 分 です。", dt.Hour, dt.Minute), this);
				balloon.ShowMessage(string.Format("{0} 時です。", dt.Hour), this);
			}

			if (m_cnt <= 4) {
				//this.Top += m_muki;
				m_cnt += 1;
			} else {
				m_cnt = 0;
				m_muki *= -1;
			}

			m_deg += 10;
			if (m_deg >= 360.0) m_deg = 0.0;

			double n = Math.Sin(m_deg * Math.PI / 180.0);
			n *= 5;
			//System.Diagnostics.Debug.WriteLine(string.Format("n = {0}", n));

			this.Top = (int)(m_homePosition.Y + (int)n);
			Kjm.WorkArea wa = new Kjm.WorkArea();

			// 右下に配置するときに必要
			// this.Height = (wa.Size.Height - this.Top) - 1;
		}

		private void ni_Click(object sender, EventArgs e) {
		}

		private void ni_DoubleClick(object sender, EventArgs e) {

			// タスクトレイアイコンをダブルクリックすると
			// アプリケーションを終了する。

			this.Close();
		}

		private void ni_MouseUp(object sender, MouseEventArgs e) {

			// タスクトレイアイコン上で、右クリックをすると、
			// 表示・非表示をトグルする。

			//if (e.Button == System.Windows.Forms.MouseButtons.Right) {
			//	this.Visible = !this.Visible;
			//}
		}

		private void Form1_MouseMove(object sender, MouseEventArgs e) {
		}

		private void worker_DoWork(object sender, DoWorkEventArgs e) {
		}

		private void worker_ProgressChanged(object sender, ProgressChangedEventArgs e) {
		}

		private void Form1_Paint(object sender, PaintEventArgs e) {

		}

		private void 終了XToolStripMenuItem_Click(object sender, EventArgs e) {
			this.Close();
		}

		private void UnzipHishoKan(string fileName) {
			string basePath = Path.Combine(Path.GetDirectoryName(Application.ExecutablePath), @"res");

			if (Path.GetExtension(fileName).ToUpper() == ".PNG") {
				// 今の秘書艦を削除
				File.Delete(Path.Combine(basePath, "hisho.png"));
				File.Copy(fileName, Path.Combine(basePath, "hisho.png"));
			} else {
				using (ZipFile z = new ZipFile(fileName, System.Text.Encoding.UTF8)) {
					string directoryPathInArchive = @"JPEG\PNG";
					z.FlattenFoldersOnExtract = true;
					z.ExtractSelectedEntries("*8*.png", directoryPathInArchive, basePath, ExtractExistingFileAction.OverwriteSilently);
				}

				// 今の秘書艦を削除
				File.Delete(Path.Combine(basePath, "hisho.png"));
				File.Move(Directory.GetFiles(basePath, "*[8].png")[0], Path.Combine(basePath, "hisho.png"));
			}
			LoadHishoKan(Path.Combine(Path.GetDirectoryName(Application.ExecutablePath), @"res\hisho.png"));
		}

		private void 秘書官選択SToolStripMenuItem_Click(object sender, EventArgs e) {
			SelectHishoKan();
		}

		// 秘書艦を選択
		private void SelectHishoKan() {
			using (OpenFileDialog ofd = new OpenFileDialog()) {
				ofd.Title = "秘書艦のzipファイルを選択";
				ofd.Filter = "zipファイル(*.zip)|*.zip|pngファイル(*.png)|*.png|すべてのファイル(*.*)|*.*";
				if (ofd.ShowDialog(this) == System.Windows.Forms.DialogResult.OK) {
					UnzipHishoKan(ofd.FileName);
				}
			}
		}
	}
}
