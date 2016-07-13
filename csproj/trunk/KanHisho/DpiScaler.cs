using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace KanHisho {
	public class DpiScaler {

		[DllImport("gdi32.dll")]
		private static extern int GetDeviceCaps(IntPtr hdc, DeviceCap nIndex);

		[DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
		private static extern IntPtr GetDC(IntPtr hWnd);

		[DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
		private static extern int ReleaseDC(IntPtr hWnd, IntPtr hDC);

		private enum DeviceCap : int {
			LOGPIXELSX = 88,
	        LOGPIXELSY = 90,
		};

		// 96 DPI ピクセルを想定しているデザインをどれだけスケーリングするか
		private double scaleX;
		private double scaleY;
		
		public int SCALEX(int argX) {
			return ((int) ((argX) * scaleX));
		}
		public int SCALEY(int argY) {
			return ((int) ((argY) * scaleY));
		}

		public DpiScaler() {
		   IntPtr screen = GetDC(IntPtr.Zero);
		   scaleX = GetDeviceCaps(screen, DeviceCap.LOGPIXELSX) / 96.0;
		   scaleY = GetDeviceCaps(screen, DeviceCap.LOGPIXELSY) / 96.0;
		   ReleaseDC(IntPtr.Zero, screen);
		}
	}
}
