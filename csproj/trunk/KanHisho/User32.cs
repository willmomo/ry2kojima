using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Windows.Forms;

namespace Kjm {

	// user32.dll で提供されるAPIをC#で使えるようにするクラス

	class User32 {
		private const UInt32 WS_EX_NOACTIVATE = 0x8000000;  // アクティブ化されないスタイル
		private enum GWL : int {
			WINDPROC = -4,
			HINSTANCE = -6,
			HWNDPARENT = -8,
			STYLE = -16,
			EXSTYLE = -20,
			USERDATA = -21,
			ID = -12
		}
		private enum SWP : int {
			NOSIZE = 0x0001,
			NOMOVE = 0x0002,
			NOZORDER = 0x0004,
			NOREDRAW = 0x0008,
			NOACTIVATE = 0x0010,
			FRAMECHANGED = 0x0020,
			SHOWWINDOW = 0x0040,
			HIDEWINDOW = 0x0080,
			NOCOPYBITS = 0x0100,
			NOOWNERZORDER = 0x0200,
			NOSENDCHANGING = 0x400
		}

		[DllImport("user32.dll")]
		private static extern UInt32 GetWindowLong(IntPtr hWnd, GWL index);

		[DllImport("user32.dll")]
		private static extern UInt32 SetWindowLong(IntPtr hWnd, GWL index, UInt32 unValue);

		[DllImport("user32.dll")]
		private static extern UInt32 SetWindowPos(IntPtr hWnd, IntPtr hWndInsertAfter, int x, int y, int width, int height, SWP flags);

		// アクティブ化されないスタイル設定
		public static void SetNotActiveWindow(Form f) {
			// 現在のスタイルを取得
			UInt32 unSyle = GetWindowLong(f.Handle, GWL.EXSTYLE);

			// キャプションのスタイルを削除
			unSyle = (unSyle | WS_EX_NOACTIVATE);

			// スタイルを反映
			UInt32 unret = SetWindowLong(f.Handle, GWL.EXSTYLE, unSyle);

			// ウィンドウを再描画
			SetWindowPos(f.Handle, IntPtr.Zero, 0, 0, 0, 0, SWP.NOMOVE | SWP.NOSIZE | SWP.NOZORDER | SWP.FRAMECHANGED);
		}
	}

	// ワークエリアの矩形を扱うクラス

	class WorkArea {
		[DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		private static extern bool SystemParametersInfo(int uiAction, int uiParam, ref RECT pvParam, int fWinIni);

		private const Int32 SPIF_SENDWININICHANGE = 2;
		private const Int32 SPIF_UPDATEINIFILE = 1;
		private const Int32 SPIF_change = SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE;
		private const Int32 SPI_SETWORKAREA = 47;
		private const Int32 SPI_GETWORKAREA = 48;

		[StructLayout(LayoutKind.Sequential)]
		private struct RECT {
			public Int32 Left;
			public Int32 Top;   // top is before right in the native struct
			public Int32 Right;
			public Int32 Bottom;
		}

		public int Left { get; set; }
		public int Top { get; set; }
		public int Right { get; set; }
		public int Bottom { get; set; }

		public Point Location { get { return new Point(Left, Right); } }
		public Size Size { get { return new Size(Right - Left, Bottom - Top); } }

		public WorkArea() {
			RECT rc = new RECT();
			bool result = SystemParametersInfo(SPI_GETWORKAREA, 0, ref rc, 0);

			Left = rc.Left;
			Top = rc.Top;
			Right = rc.Right;
			Bottom = rc.Bottom;
		}
	}
}
