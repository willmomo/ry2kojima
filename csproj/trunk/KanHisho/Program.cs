using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace KanHisho {
	static class Program {

		/// <summary>
		/// アプリケーションのメイン エントリ ポイントです。
		/// </summary>
		[STAThread]
		static void Main() {
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			try {
				Application.Run(new Form1());
			} catch (Exception ex) {
			}
		}
	}
}
