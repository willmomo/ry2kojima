using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace Kjm {
	class IniFile {
		[DllImport("KERNEL32.DLL")]
		public static extern uint
		  GetPrivateProfileString(string lpAppName,
		  string lpKeyName, string lpDefault,
		  StringBuilder lpReturnedString, uint nSize,
		  string lpFileName);

		[DllImport("KERNEL32.DLL",
			EntryPoint = "GetPrivateProfileStringA")]
		public static extern uint
		  GetPrivateProfileStringByByteArray(string lpAppName,
		  string lpKeyName, string lpDefault,
		  byte[] lpReturnedString, uint nSize,
		  string lpFileName);

		[DllImport("KERNEL32.DLL")]
		public static extern uint
		  GetPrivateProfileInt(string lpAppName,
		  string lpKeyName, int nDefault, string lpFileName);

		[DllImport("KERNEL32.DLL")]
		public static extern uint WritePrivateProfileString(
		  string lpAppName,
		  string lpKeyName,
		  string lpString,
		  string lpFileName);

		public string FileName { get; set; }

		public IniFile() {
		}

		public string GetString(string section, string key, string defValue) {
			StringBuilder buf = new StringBuilder(1024);
			GetPrivateProfileString(section, key, defValue, buf, (uint)buf.Capacity, FileName);
			return buf.ToString();
		}

		public void SetString(string section, string key, string value) {
			string dirName = System.IO.Path.GetDirectoryName(FileName);
			if (!System.IO.Directory.Exists(dirName)) {
				System.IO.Directory.CreateDirectory(dirName);
			}
			WritePrivateProfileString(section, key, value, FileName);
		}
	}
}
