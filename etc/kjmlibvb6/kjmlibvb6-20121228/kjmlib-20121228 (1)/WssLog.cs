using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace kjm {
	class WssLog {
		[DllImport("WssLog.dll")]
		public extern static void WLGetMyName(StringBuilder buf, int size);

		[DllImport("WssLog.dll")]
		public extern static int WLGetLogLevel();

		[DllImport("WssLog.dll")]
		public extern static void WLSetLogLevel(int newLevel);

		[DllImport("WssLog.dll")]
		public extern static void WLGetLogPath(StringBuilder buf, int size);

		[DllImport("WssLog.dll")]
		public extern static void WLSetLogPath(string newPath);

		[DllImport("WssLog.dll")]
		public extern static int WLGetLogNameType();

		[DllImport("WssLog.dll")]
		public extern static void WLSetLogNameType(int nNameType);

		[DllImport("WssLog.dll")]
		public extern static void WinssaVB_Log(string sikibetu, int loglevel, string msg);

		[DllImport("WssLog.dll")]
		public extern static void WLDeleteOldLog();
	}
}
