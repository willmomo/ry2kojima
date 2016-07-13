using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace odbctest2 {
	class Program {

		static void Main(string[] args) {
			// ドライバを列挙する
			string driverName;
			kjm.OdbcUtil.GetDriverList().ForEach(delegate(string s) {
				Console.WriteLine(s);
			});
				
			driverName= kjm.OdbcUtil.FindDriver("OraHome92");

			if (driverName == null) {
			}

			driverName = kjm.OdbcUtil.FindDriver("mysql");

			if (driverName == null) {
			}

			kjm.OdbcUtil.GetDriverList().ForEach(delegate(string s) {
				Console.WriteLine(s);
			});

			bool fInstalled = kjm.OdbcUtil.IsInstalled("mysql odbc 5.1 driver ");
			Console.WriteLine(fInstalled);
		}
	}
}
