using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;

namespace Kjm {
	class Log {
		public static void Write(string msg) {
			string fname = Path.Combine(Path.GetDirectoryName(Application.ExecutablePath), "hoge.log");
			using (StreamWriter sw = new StreamWriter(fname, true)) {
				sw.WriteLine(DateTime.Now.ToString() + " " + msg);
			}
		}
	}
}
