using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Threading;

namespace LocalHook {
	public partial class Form1 : Form {
		private static IntPtr hHook = IntPtr.Zero;

		private const int WH_MOUSE = 7;

		private void SetMouseHook(HookMethods.HookProcedureDelegete proc) {
			// マウスフックを設定
			hHook = HookMethods.SetWindowsHookEx(WH_MOUSE, proc, IntPtr.Zero, AppDomain.GetCurrentThreadId());
			if (hHook == IntPtr.Zero) {
				MessageBox.Show("SetWindowsHookEx Failed.");
			}
		}

		private void RemoveMouseHook() {
			// フックを削除
			if (HookMethods.UnhookWindowsHookEx(hHook) == false) {
				MessageBox.Show("UnhookWindowsHookEx Failed.");
			}
		}

		public IntPtr MouseHookProc(int nCode, IntPtr wParam, IntPtr lParam) {
			if (nCode >= 0) {
				// コールバックからのデータを整理する．
				MouseHookStruct MyMouseHookStruct = (MouseHookStruct)Marshal.PtrToStructure(lParam, typeof(MouseHookStruct));

				String strCaption = "x = " + MyMouseHookStruct.pt.x.ToString("d") + " : y = " + MyMouseHookStruct.pt.y.ToString("d");
				this.Text = strCaption;
			}
			return HookMethods.CallNextHookEx(hHook, nCode, wParam, lParam);
		}

		public Form1() {
			InitializeComponent();
		}

		private void button1_Click(object sender, EventArgs e) {
			if (hHook == IntPtr.Zero) {
				SetMouseHook(MouseHookProc);
				this.button1.Text = "Unhook Windows Hook";
			} else {
				RemoveMouseHook();
				this.button1.Text = "Set Windwos Hook";
			}
		}
	}

	[StructLayout(LayoutKind.Sequential)]
	public struct POINT {
		public int x;
		public int y;
	}

	[StructLayout(LayoutKind.Sequential)]
	public class MouseHookStruct {
		public POINT pt;
		public uint mouseData;
		public uint flags;
		public uint time;
		public IntPtr dwExtraInfo;
	}

}
