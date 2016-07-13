using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Threading;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace Kjm {
	// 以下の名前空間をインポートする


	public class ActivateProc {
		// 外部プロセスのメイン・ウィンドウを起動するためのWin32 API
		[DllImport("user32.dll")]
		private static extern bool SetForegroundWindow(IntPtr hWnd);

		[DllImport("user32.dll")]
		private static extern bool ShowWindowAsync(IntPtr hWnd, int nCmdShow);

		[DllImport("user32.dll")]
		private static extern bool IsIconic(IntPtr hWnd);

		[DllImport("user32.dll")]
		private static extern uint GetWindowThreadProcessId(IntPtr hWnd, ref uint procId);

		// コールバックメソッドのデリゲート
		private delegate int EnumerateWindowsCallback(IntPtr hWnd, int lParam);

		[DllImport("user32", EntryPoint = "EnumWindows")]
		private static extern int EnumWindows(EnumerateWindowsCallback lpEnumFunc, int lParam);

		private static Process target_proc = null;
		private static IntPtr target_hwnd = IntPtr.Zero;

		// ウィンドウを列挙するためのコールバックメソッド
		public static int EnumerateWindows(IntPtr hWnd, int lParam) {
			uint procId = 0;
			uint result = GetWindowThreadProcessId(hWnd, ref procId);
			if (procId == target_proc.Id) {
				// 同じIDで複数のウィンドウが見つかる場合がある
				// とりあえず最初のウィンドウが見つかった時点で終了する
				target_hwnd = hWnd;
				return 0;
			}

			// 列挙を継続するには0以外を返す必要がある
			return 1;
		}

		// 外部プロセスのウィンドウを最前面に表示する
		public static void WakeupWindow(Process target) {
			target_proc = target;
			EnumWindows(new EnumerateWindowsCallback(EnumerateWindows), 0);
			if (target_hwnd == IntPtr.Zero) {
				return;
			}
			// メイン・ウィンドウが最小化されていれば元に戻す
			//if (IsIconic(target_hwnd)) {
				ShowWindowAsync(target_hwnd, SW_RESTORE);
			//}
			// メイン・ウィンドウを最前面に表示する
			SetForegroundWindow(target_hwnd);
		}
	
		//// 外部プロセスのメイン・ウィンドウを起動するためのWin32 API
		//[DllImport("user32.dll")]
		//private static extern bool SetForegroundWindow(IntPtr hWnd);
		//[DllImport("user32.dll")]
		//private static extern bool ShowWindowAsync(IntPtr hWnd, int nCmdShow);
		//[DllImport("user32.dll")]
		//private static extern bool IsIconic(IntPtr hWnd);
		// ShowWindowAsync関数のパラメータに渡す定義値
		private const int SW_RESTORE = 9;  // 画面を元の大きさに戻す

		// 実行中の同じアプリケーションのプロセスを取得する
		public static Process GetPreviousProcess() {
			Process curProcess = Process.GetCurrentProcess();
			Process[] allProcesses = Process.GetProcessesByName(curProcess.ProcessName);
			//Process[] allProcesses = Process.GetProcessesByName("bfkjmw");

			foreach (Process checkProcess in allProcesses) {
				// 自分自身のプロセスIDは無視する
				if (checkProcess.Id != curProcess.Id) {
					// プロセスのフルパス名を比較して同じアプリケーションか検証
					//if (String.Compare(checkProcess.MainModule.FileName,curProcess.MainModule.FileName, true) == 0) {
					//    // 同じフルパス名のプロセスを取得
					//    return checkProcess;
					//}
					return checkProcess;
				}
			}

			// 同じアプリケーションのプロセスが見つからない！  
			return null;
		}
	}
}
