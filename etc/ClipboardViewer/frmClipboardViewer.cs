using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace ClipboardViewer {
	public partial class frmClipboardViewer : Form {
		private MyClipboardViewer m_viewer;

		public frmClipboardViewer() {
			m_viewer = new MyClipboardViewer(this);

			// イベントハンドラを登録
			m_viewer.ClipboardHandler += this.OnClipboardChanged;

			InitializeComponent();
		}

		private void OnClipboardChanged(object sender, ClipboardEventArgs args) {
			// 更新時間を出力
			DateTime dt = DateTime.Now;
			this.textBox1.Text = "Update: " + dt.ToLongDateString() + " " + dt.ToLongTimeString() + "\r\n\r\n";

			this.pictureBox1.Image = null;	// イメージを消去しておく

			string[] formats = args.DataObject.GetFormats();
			if (formats.Length == 0) {
				this.textBox1.Text += "(クリップボードは空です)\r\n";
			} else {
				for (int i = 0; i < formats.Length; i++) {
					if (formats[i] == "Locale") {
						// 4バイトのLCID
						// LCIDは、http://msdn.microsoft.com/ja-jp/library/cc392381.aspx も参照。
						//
						byte[] ar = ((System.IO.MemoryStream)args.DataObject.GetData(formats[i])).ToArray();

						this.textBox1.Text += "[" + formats[i] + "] = " + ar.Length + "バイト : ";
						if (ar.Length == 4) {
							// 4バイトの時は、間違いなくLCIDだろう
							this.textBox1.Text += "0x" + BitConverter.ToInt32(ar, 0).ToString("X4");
						} else {
							// 4バイト以外の時は、知らないデータ
							for (int j = 0; j < ar.Length; j++) {
								this.textBox1.Text += "[" + ar[j].ToString("X2") + "]";
							}
						}
						this.textBox1.Text += "\r\n";
					} else if ((formats[i] == "FileDrop") || (formats[i] == "FileNameW") || (formats[i] == "FileName")) {
						// System.String[] を戻してくるデータフォーマット
						this.textBox1.Text += "[" + formats[i] + "] = ";
						foreach (string one in (string[])args.DataObject.GetData(formats[i])) {
							this.textBox1.Text += "[" + one + "]";
						}
						this.textBox1.Text += "\r\n";
					} else if (formats[i] == "Bitmap") {
						Object obj = args.DataObject.GetData(formats[i]);
						this.textBox1.Text += string.Format("[{0}] = [{1}](右に表示中)\r\n", formats[i], ((obj == null) ? "(null)" : obj.ToString()));
						// 右のペインに画像を表示
						this.pictureBox1.Image = (System.Drawing.Bitmap)obj;
					} else if (formats[i] == "HTML Format") {
						// 長いので割愛
						// 元になったHTMLの全体と、そのどの部分かという情報が入っているっぽい。
						this.textBox1.Text += string.Format("[{0}] = [(長いので割愛)]\r\n", formats[i]);
					} else {
						Object obj = args.DataObject.GetData(formats[i]);
						this.textBox1.Text += string.Format("[{0}] = [{1}]\r\n", formats[i], ((obj == null) ? "(null)" : obj.ToString()));
					}
				}
			}
			this.textBox1.Select(0, 0);
		}

		private void splitContainer1_Panel1_Paint(object sender, PaintEventArgs e) {

		}
	}

	public class ClipboardEventArgs : EventArgs {
		private static int m_cntC = 0;
		private static int m_cntD = 0;

		private IDataObject m_dataObject;
		public IDataObject DataObject {
			get {
				return this.m_dataObject;
			}
		}

		public ClipboardEventArgs() {
			this.m_dataObject = Clipboard.GetDataObject();

			++ClipboardEventArgs.m_cntC;
			DateTime dt = DateTime.Now;
			System.IO.StreamWriter sw = new System.IO.StreamWriter("ClipboardViewer.log", true);
			sw.WriteLine(string.Format("{0} {1} ({2}, {3}) C: {4}",
				dt.ToLongDateString(), dt.ToLongTimeString(), ClipboardEventArgs.m_cntC, ClipboardEventArgs.m_cntD, this.m_dataObject.GetHashCode()));
			sw.Close();
		}

		~ClipboardEventArgs() {
			++ClipboardEventArgs.m_cntD;
			DateTime dt = DateTime.Now;
			System.IO.StreamWriter sw = new System.IO.StreamWriter("ClipboardViewer.log", true);
			sw.WriteLine(string.Format("{0} {1} ({2}, {3}) D: {4}",
				dt.ToLongDateString(), dt.ToLongTimeString(), ClipboardEventArgs.m_cntC, ClipboardEventArgs.m_cntD, this.m_dataObject.GetHashCode()));
			sw.Close();

			this.m_dataObject = null;
		}
	}

	public delegate void cbEventHandler(object sender, ClipboardEventArgs ev);

	/// <summary>
	/// クリップボード系のウィンドウメッセージを処理するための NativeWindow クラスから派生したクラス。
	/// </summary>
	[System.Security.Permissions.PermissionSet(
		System.Security.Permissions.SecurityAction.Demand, Name = "FullTrust")]
	internal class MyClipboardViewer : NativeWindow {
		[DllImport("user32")]
		public static extern IntPtr SetClipboardViewer(IntPtr hWndNewViewer);

		[DllImport("user32")]
		public static extern bool ChangeClipboardChain(IntPtr hWndRemove, IntPtr hWndNewNext);

		[DllImport("user32")]
		public extern static int SendMessage(IntPtr hWnd, int Msg, IntPtr wParam, IntPtr lParam);

		private const int WM_DRAWCLIPBOARD = 0x0308;
		private const int WM_CHANGECBCHAIN = 0x030D;
		private IntPtr m_nextHandle;

		private Form parent;
		public event cbEventHandler ClipboardHandler;

		internal void OnHandlerCreated(object sender, EventArgs e) {
			AssignHandle(((Form)sender).Handle);
			// ビューアを登録
			m_nextHandle = SetClipboardViewer(this.Handle);
		}

		internal void OnHandleDestroyed(object sender, EventArgs e) {
			// ビューアを解除
			bool sts = ChangeClipboardChain(this.Handle, m_nextHandle);
			ReleaseHandle();
		}

		public MyClipboardViewer(Form f) {
			f.HandleCreated += new EventHandler(this.OnHandlerCreated);
			f.HandleDestroyed += new EventHandler(this.OnHandleDestroyed);
			this.parent = f;
		}

		protected override void WndProc(ref Message m) {
			switch (m.Msg) {
			case WM_DRAWCLIPBOARD:
				if (ClipboardHandler != null) {
					// クリップボードの内容を取得してハンドラを呼び出す
					ClipboardHandler(this, new ClipboardEventArgs());
				}

				if (m_nextHandle != IntPtr.Zero) {
					SendMessage(m_nextHandle, m.Msg, m.WParam, m.LParam);
				}
				break;

			// クリップボードビューアチェインが更新された
			case WM_CHANGECBCHAIN:
				if (m.WParam == m_nextHandle) {
					m_nextHandle = m.LParam;
				} else if (m_nextHandle != IntPtr.Zero) {
					SendMessage(m_nextHandle, m.Msg, m.WParam, m.LParam);
				}
				break;
			}

			base.WndProc(ref m);
		}
	}
}
