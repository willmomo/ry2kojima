using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace bfkjmw {
	static class Program {
		/// <summary>
		/// アプリケーションのメイン エントリ ポイントです。
		/// </summary>
		[STAThread]
		static void Main() {
			Assembly assembly = Assembly.GetExecutingAssembly();
			var attribute = (GuidAttribute)assembly.GetCustomAttributes(typeof(GuidAttribute), true)[0];

			System.Threading.Mutex hMutex = new System.Threading.Mutex(false, attribute.Value);

			if (hMutex.WaitOne(0, false)) {
				Application.EnableVisualStyles();
				Application.SetCompatibleTextRenderingDefault(false);
				Application.Run(new Form1());
			} else {
				// 実行中の同じアプリケーションのウィンドウ・ハンドルの取得
				Process prevProcess = Kjm.ActivateProc.GetPreviousProcess();
				if ((prevProcess != null)) {
				//if ((prevProcess != null) &&
				//    (prevProcess.MainWindowHandle != IntPtr.Zero)) {
					// 起動中のアプリケーションを最前面に表示
						Kjm.ActivateProc.WakeupWindow(prevProcess);
				}
			}

			// GC.KeepAlive メソッドが呼び出されるまで、ガベージ コレクション対象から除外される
			GC.KeepAlive(hMutex);

			// Mutex を閉じる
			hMutex.Close();
		}
	}
}
