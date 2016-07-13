using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace odbctest2 {
	class Program {

		static void Main(string[] args) {
			string driverName;
				
			driverName= kjm.OdbcUtil.FindDriver("OraHome92");

			if (driverName == null) {
			}

			driverName = kjm.OdbcUtil.FindDriver("mysql");

			if (driverName == null) {
			}
		}
	}
}
