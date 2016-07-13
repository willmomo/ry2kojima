using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Drawing;

namespace Kjm {

	/*
	 * イベントを使う時は
	 * 
	 * public void OnMouseProc(object sender, Kjm.MouseProcEventArgs e) { ... }
	 * を実装
	 * 
	 * var obj = new Kjm.MouseHook();
	 * obj.MouseProc += new Kjm.MouseHook.MouseProcEventHandler(OnMouseProc);
	 */
	public class MouseProcEventArgs : EventArgs {
		public Point Location { get; internal set; }
		public int X { get; internal set; }
		public int Y { get; internal set; }
		public MouseProcEventArgs(int x_, int y_) {
			X = x_;
			Y = y_;
			Location = new Point(x_, y_);
		}
	}

	public class MouseHook {
		// イベント固有データ(MouseProcEventArgs)を持つイベントハンドラのデリゲートを宣言
		public delegate void MouseProcEventHandler(object sender, MouseProcEventArgs e);

		// パブリック イベントメンバを定義
		public event MouseProcEventHandler MouseProc;

		// イベントを発生させるプロテクトメンバを定義
		protected virtual void OnMouseProc(MouseProcEventArgs e) { if (MouseProc != null) MouseProc(this, e); }

		private IntPtr m_hhk = IntPtr.Zero;

		public MouseHook() {
		}

		public bool IsHooked {
			get {
				return (m_hhk == IntPtr.Zero);
			}
		}

		public void SetHook() {
			using (Process process = Process.GetCurrentProcess())
			using (ProcessModule module = process.MainModule) {
				m_hhk = SetWindowsHookEx(WH_MOUSE_LL, HookProc, GetModuleHandle(module.ModuleName), 0);
			}
		}

		public void RemoveHook() {
			if (IsHooked) UnhookWindowsHookEx(m_hhk);
			m_hhk = IntPtr.Zero;
		}

		private IntPtr HookProc(int nCode, IntPtr wParam, IntPtr lParam) {
			if (nCode >= 0) {
				MouseProcStruct mps = (MouseProcStruct)Marshal.PtrToStructure(lParam, typeof(MouseProcStruct));
				OnMouseProc(new MouseProcEventArgs(mps.pt.x, mps.pt.y));
			}
			return CallNextHookEx(m_hhk, nCode, wParam, lParam);
		}

		private const int WH_MOUSE = 7;
		private const int WH_MOUSE_LL = 14;

		// POINT structure(https://msdn.microsoft.com/ja-jp/library/windows/desktop/dd162805(v=vs.85).aspx)
		[StructLayout(LayoutKind.Sequential)]
		private struct POINT {
			public int x;
			public int y;
		}

		// MOUSEHOOKSTRUCT structure(https://msdn.microsoft.com/ja-jp/library/windows/desktop/ms644968(v=vs.85).aspx)
		[StructLayout(LayoutKind.Sequential)]
		private class MouseProcStruct {
			public POINT pt;
			public uint mouseData;
			public uint flags;
			public uint time;
			public IntPtr dwExtraInfo;
		}

		// フックプロシージャのためのデリゲート
		private delegate IntPtr HookProcDelegate(int nCode, IntPtr wParam, IntPtr lParam);

		// アプリケーション定義のフックプロシージャをフックチェーン内にインストールします。
		// HHOOK SetWindowsHookEx(int idHook, HOOKPROC lpfn, HINSTANCE hMod, DWORD dwThreadId)
		[DllImport("user32.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.StdCall)]
		private static extern IntPtr SetWindowsHookEx(int idHook, HookProcDelegate lpfn, IntPtr hMod, int dwThreadId);

		// SetWindowsHookEx 関数を使ってフックチェーン内にインストールされたフックプロシージャを削除します。
		// BOOL UnhookWindowsHookEx(HHOOK hhk)
		[DllImport("user32.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.StdCall)]
		private static extern bool UnhookWindowsHookEx(IntPtr hhk);

		// 現在のフックチェーン内の次のフックプロシージャに、フック情報を渡します。
		// LRESULT CallNextHookEx(HHOOK hhk, int nCode, WPARAM wParam, LPARAM lParam)
		[DllImport("user32.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.StdCall)]
		private static extern IntPtr CallNextHookEx(IntPtr hhk, int nCode, IntPtr wParam, IntPtr lParam);

		// 呼び出し側プロセスのアドレス空間に該当ファイルがマップされている場合、指定されたモジュール名のモジュールハンドルを返します。
		// HMODULE GetModuleHandle(LPCTSTR lpModuleName)
		[DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
		private static extern IntPtr GetModuleHandle(String lpModuleName);
	}
}
