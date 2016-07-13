//---------------------------------------------------------------------
//
// kjmlib.cs v0.2013.11.27
//
//---------------------------------------------------------------------
using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;

namespace Kjm {
	public class Log {
		private const int LOGLV_DEBUG = 0;			// デバッグ用
		private const int LOGLV_TRACE = 1;			// トレース
		private const int LOGLV_WARNING = 2;		// ワーニング(通常はこのレベル以上を出力)
		private const int LOGLV_ERR = 3;			// エラー
		private const int LOGLV_FATALERR = 4;		// 致命的エラー
		private const int LOGLV_INFO = 5;			// 情報
		private const int LOGLV_ALWAYS = 9;			// 必ずログを出す

		private static string s_ident = "UNKNWN";
		public static void SetIdent(string ident) {
			Kjm.Log.s_ident = ident;
		}
		public static string GetIdent() {
			return s_ident;
		}

		// ログの出力先(default: %TEMP%)
		private static string s_path = Path.GetTempPath();
		public static string LogPath {
			get { return Log.s_path; }
			set { Log.s_path = System.IO.Path.GetFullPath(value); }
		}

		// ログの基本ファイル名(default: 実行ファイル名.log)
		private static string s_name = Path.ChangeExtension(Path.GetFileName(System.Reflection.Assembly.GetEntryAssembly().Location), "log");
		public static string LogName {
			get { return Log.s_name; }
			set {
				Log.s_name = value;
				Log.s_nameOld = Path.GetFileNameWithoutExtension(s_name) + "_{0:D3}.log";
			}
		}

		// バックアップログの基本ファイル名(default: 実行ファイル名_{0:D3}.log)
		private static string s_nameOld = Path.GetFileNameWithoutExtension(s_name) + "_{0:D3}.log";

		// ログの最大サイズ(default: 1MB)
		private static int s_maxSize = (1 * 1024 * 1024);
		public static int MaxSize {
			get { return Log.s_maxSize; }
			set { Log.s_maxSize = value; }
		}

		public static bool DelOldLog() {
			//bool bret = WLDeleteOldLog();
			bool bret = true;
			return bret;
		}

		public static void WriteInfo(string message) {
			Kjm.Log.Write(Kjm.Log.s_ident, Kjm.Log.LOGLV_INFO, message);
		}

		public static void WriteError(string message) {
			Kjm.Log.Write(Kjm.Log.s_ident, Kjm.Log.LOGLV_ERR, message);
		}

		public static void Write(string sikibetu, int loglevel, string format) {
			//WinssaVB_Log(sikibetu, loglevel, format.Replace("\x0d\x0a", "\x0a"));

			if (!Directory.Exists(Log.s_path)) {
				// フォルダが無ければ、作成する。
				Directory.CreateDirectory(Log.s_path);
			}

			if (File.Exists(Path.Combine(Log.s_path, Log.s_name))) {
				FileInfo fi = new FileInfo(Path.Combine(Log.s_path, Log.s_name));
				if (fi.Length > Log.MaxSize) {
					// ファイルのシフト
					for (int no = 98; no >= 1; no--) {
						if (File.Exists(Path.Combine(Log.s_path, String.Format(Log.s_nameOld, no)))) {
							File.Copy(
								Path.Combine(Log.s_path, String.Format(Log.s_nameOld, no)),
								Path.Combine(Log.s_path, String.Format(Log.s_nameOld, no + 1)), true);
						}
					}

					File.Copy(
						Path.Combine(Log.s_path, Log.s_name),
						Path.Combine(Log.s_path, string.Format(Log.s_nameOld, 1)), true);
					File.Delete(Path.Combine(Log.s_path, Log.s_name));
				}
			}

			for (int i = 0; i < 10; i++) {
				try {
					File.AppendAllText(
						Path.Combine(Log.s_path, Log.s_name),
						DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss.fff [") + sikibetu + "]{" + loglevel + "}:" + format + "\r\n");
					break;
				} catch {
					Thread.Sleep(0);
				}
			}
		}
	}

	public class Util {
		[DllImport("shell32.dll")]
		public static extern int SHGetFolderPath(IntPtr hwndOwner, int nFolder, IntPtr hToken, uint dwFlags, [Out] StringBuilder pszPath);

		public enum SpecialFolderCSIDL : int {
			CSIDL_DESKTOP = 0x0000,    // <desktop>
			CSIDL_INTERNET = 0x0001,    // Internet Explorer (icon on desktop)
			CSIDL_PROGRAMS = 0x0002,    // Start Menu\Programs
			CSIDL_CONTROLS = 0x0003,    // My Computer\Control Panel
			CSIDL_PRINTERS = 0x0004,    // My Computer\Printers
			CSIDL_PERSONAL = 0x0005,    // My Documents
			CSIDL_FAVORITES = 0x0006,    // <user name>\Favorites
			CSIDL_STARTUP = 0x0007,    // Start Menu\Programs\Startup
			CSIDL_RECENT = 0x0008,    // <user name>\Recent
			CSIDL_SENDTO = 0x0009,    // <user name>\SendTo
			CSIDL_BITBUCKET = 0x000a,    // <desktop>\Recycle Bin
			CSIDL_STARTMENU = 0x000b,    // <user name>\Start Menu
			CSIDL_DESKTOPDIRECTORY = 0x0010,    // <user name>\Desktop
			CSIDL_DRIVES = 0x0011,    // My Computer
			CSIDL_NETWORK = 0x0012,    // Network Neighborhood
			CSIDL_NETHOOD = 0x0013,    // <user name>\nethood
			CSIDL_FONTS = 0x0014,    // windows\fonts
			CSIDL_TEMPLATES = 0x0015,
			CSIDL_COMMON_STARTMENU = 0x0016,    // All Users\Start Menu
			CSIDL_COMMON_PROGRAMS = 0x0017,    // All Users\Programs
			CSIDL_COMMON_STARTUP = 0x0018,    // All Users\Startup
			CSIDL_COMMON_DESKTOPDIRECTORY = 0x0019,    // All Users\Desktop
			CSIDL_APPDATA = 0x001a,    // <user name>\Application Data
			CSIDL_PRINTHOOD = 0x001b,    // <user name>\PrintHood
			CSIDL_LOCAL_APPDATA = 0x001c,    // <user name>\Local Settings\Applicaiton Data (non roaming)
			CSIDL_ALTSTARTUP = 0x001d,    // non localized startup
			CSIDL_COMMON_ALTSTARTUP = 0x001e,    // non localized common startup
			CSIDL_COMMON_FAVORITES = 0x001f,
			CSIDL_INTERNET_CACHE = 0x0020,
			CSIDL_COOKIES = 0x0021,
			CSIDL_HISTORY = 0x0022,
			CSIDL_COMMON_APPDATA = 0x0023,    // All Users\Application Data
			CSIDL_WINDOWS = 0x0024,    // GetWindowsDirectory()
			CSIDL_SYSTEM = 0x0025,    // GetSystemDirectory()
			CSIDL_PROGRAM_FILES = 0x0026,    // C:\Program Files
			CSIDL_MYPICTURES = 0x0027,    // C:\Program Files\My Pictures
			CSIDL_PROFILE = 0x0028,    // USERPROFILE
			CSIDL_SYSTEMX86 = 0x0029,    // x86 system directory on RISC
			CSIDL_PROGRAM_FILESX86 = 0x002a,    // x86 C:\Program Files on RISC
			CSIDL_PROGRAM_FILES_COMMON = 0x002b,    // C:\Program Files\Common
			CSIDL_PROGRAM_FILES_COMMONX86 = 0x002c,    // x86 Program Files\Common on RISC
			CSIDL_COMMON_TEMPLATES = 0x002d,    // All Users\Templates
			CSIDL_COMMON_DOCUMENTS = 0x002e,    // All Users\Documents
			CSIDL_COMMON_ADMINTOOLS = 0x002f,    // All Users\Start Menu\Programs\Administrative Tools
			CSIDL_ADMINTOOLS = 0x0030,    // <user name>\Start Menu\Programs\Administrative Tools
			CSIDL_CONNECTIONS = 0x0031,    // Network and Dial-up Connections
			CSIDL_CDBURN_AREA = 0x003B,    // Data for burning with interface ICDBurn

		};

		public static string GetSpecialFolder(SpecialFolderCSIDL folder) {
			const int MaxPath = 260;
			StringBuilder sb = new StringBuilder(MaxPath);
			SHGetFolderPath(IntPtr.Zero, (int)folder, IntPtr.Zero, 0x0000, sb);
			return sb.ToString();
		}

		public enum ExitWindows : uint {
			EWX_LOGOFF = 0x00,
			EWX_SHUTDOWN = 0x01,
			EWX_REBOOT = 0x02,
			EWX_POWEROFF = 0x08,
			EWX_RESTARTAPPS = 0x40,
			EWX_FORCE = 0x04,
			EWX_FORCEIFHUNG = 0x10,
		}

		[System.Runtime.InteropServices.DllImport("user32.dll", SetLastError = true)]
		public static extern bool ExitWindowsEx(ExitWindows uFlags,
			int dwReason);
		
		[System.Runtime.InteropServices.DllImport("kernel32.dll", SetLastError = true)]
		private static extern IntPtr GetCurrentProcess();

		[System.Runtime.InteropServices.DllImport("advapi32.dll", SetLastError = true)]
		private static extern bool OpenProcessToken(IntPtr ProcessHandle,
			uint DesiredAccess,
			out IntPtr TokenHandle);

		[System.Runtime.InteropServices.DllImport("kernel32.dll", SetLastError = true)]
		private static extern bool CloseHandle(IntPtr hObject);

		[System.Runtime.InteropServices.DllImport("advapi32.dll", SetLastError = true,
			CharSet = System.Runtime.InteropServices.CharSet.Auto)]
		private static extern bool LookupPrivilegeValue(string lpSystemName,
			string lpName,
			out long lpLuid);

		[System.Runtime.InteropServices.StructLayout(
		   System.Runtime.InteropServices.LayoutKind.Sequential, Pack = 1)]
		private struct TOKEN_PRIVILEGES {
			public int PrivilegeCount;
			public long Luid;
			public int Attributes;
		}

		[System.Runtime.InteropServices.DllImport("advapi32.dll", SetLastError = true)]
		private static extern bool AdjustTokenPrivileges(IntPtr TokenHandle,
			bool DisableAllPrivileges,
			ref TOKEN_PRIVILEGES NewState,
			int BufferLength,
			IntPtr PreviousState,
			IntPtr ReturnLength);

		//シャットダウンするためのセキュリティ特権を有効にする
		public static void AdjustToken() {
			const uint TOKEN_ADJUST_PRIVILEGES = 0x20;
			const uint TOKEN_QUERY = 0x8;
			const int SE_PRIVILEGE_ENABLED = 0x2;
			const string SE_SHUTDOWN_NAME = "SeShutdownPrivilege";

			if (Environment.OSVersion.Platform != PlatformID.Win32NT)
				return;

			IntPtr procHandle = GetCurrentProcess();

			//トークンを取得する
			IntPtr tokenHandle;
			OpenProcessToken(procHandle,
				TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, out tokenHandle);
			//LUIDを取得する
			TOKEN_PRIVILEGES tp = new TOKEN_PRIVILEGES();
			tp.Attributes = SE_PRIVILEGE_ENABLED;
			tp.PrivilegeCount = 1;
			LookupPrivilegeValue(null, SE_SHUTDOWN_NAME, out tp.Luid);
			//特権を有効にする
			AdjustTokenPrivileges(
				tokenHandle, false, ref tp, 0, IntPtr.Zero, IntPtr.Zero);

			//閉じる
			CloseHandle(tokenHandle);
		}

		// 指定した実行ファイル名のプロセスをすべて取得
		public static System.Diagnostics.Process[] GetProcessesByFileName(string searchFileName, string machineName) {
			System.Collections.ArrayList list = new System.Collections.ArrayList();

			//すべてのプロセスを列挙する
			foreach (System.Diagnostics.Process p in System.Diagnostics.Process.GetProcesses(machineName)) {
				string fileName;
				try {
					//メインモジュールのパスを取得する
					fileName = p.MainModule.FileName;
				} catch (System.ComponentModel.Win32Exception) {
					//MainModuleの取得に失敗
					fileName = "";
				}
				if (0 < fileName.Length) {
					//ファイル名の部分を取得する
					fileName = System.IO.Path.GetFileName(fileName);
					//探しているファイル名と一致した時、コレクションに追加
					if (searchFileName.ToLower().Equals(fileName.ToLower())) {
						list.Add(p);
					}
				}
			}

			//コレクションを配列にして返す
			return (System.Diagnostics.Process[])list.ToArray(typeof(System.Diagnostics.Process));
		}

		public static int Run(string cmd, string arg, ProcessWindowStyle style) {
			int result = -1;

			using (System.Diagnostics.Process p = new Process()) {
				p.StartInfo.FileName = cmd;
				p.StartInfo.Arguments = arg;
				p.StartInfo.WindowStyle = style;
				p.Start();
				p.WaitForExit();
				result = p.ExitCode;
			}

			return result;
		}

		public static int Run(string cmd) {
			return Run(cmd, "", ProcessWindowStyle.Normal);
		}
	}

	/// <summary>
	/// INIファイルを読み書きするクラス
	/// </summary>
	public class IniFile {
		[DllImport("kernel32.dll")]
		private static extern int GetPrivateProfileString(
			string lpApplicationName,
			string lpKeyName,
			string lpDefault,
			StringBuilder lpReturnedstring,
			int nSize,
			string lpFileName);

		[DllImport("kernel32.dll")]
		private static extern int WritePrivateProfileString(
			string lpApplicationName,
			string lpKeyName,
			string lpstring,
			string lpFileName);

		private string filePath = Path.ChangeExtension(System.Windows.Forms.Application.ExecutablePath, "ini");

		public IniFile() {
		}

		/// <summary>
		/// ファイル名を指定して初期化します。
		/// ファイルが存在しない場合は初回書き込み時に作成されます。
		/// </summary>
		public IniFile(string filePath) {
			this.filePath = filePath;
		}

		/// <summary>
		/// sectionとkeyからiniファイルの設定値を取得、設定します。 
		/// </summary>
		/// <returns>指定したsectionとkeyの組合せが無い場合は""が返ります。</returns>
		public string this[string section, string key] {
			set {
				if (WritePrivateProfileString(section, key, value, filePath) == 0) {
					throw new System.ComponentModel.Win32Exception(Marshal.GetLastWin32Error(), "WritePrivateProfileString error");
				}
			}
			get {
				StringBuilder sb = new StringBuilder(256);
				while (GetPrivateProfileString(section, key, string.Empty, sb, sb.Capacity, filePath) == sb.Capacity - 1) {
					sb.Capacity += 256;
				}
				return sb.ToString();
			}
		}

		/// <summary>
		/// sectionとkeyからiniファイルの設定値を取得します。
		/// 指定したsectionとkeyの組合せが無い場合はdefaultvalueで指定した値が返ります。
		/// </summary>
		/// <returns>
		/// 指定したsectionとkeyの組合せが無い場合はdefaultvalueで指定した値が返ります。
		/// </returns>
		public string GetValue(string section, string key, string defaultvalue) {
			StringBuilder sb = new StringBuilder(256);
			while (GetPrivateProfileString(section, key, defaultvalue, sb, sb.Capacity, filePath) == sb.Capacity - 1) {
				sb.Capacity += 256;
			}
			return sb.ToString();
		}


		public int GetInt(string section, string key, int defaultvalue) {
			int result;
			if (int.TryParse(GetValue(section, key, defaultvalue.ToString()), out result) == false) {
				result = defaultvalue;
			}
			return result;
		}
	}

	public class ServiceControlCommand {

		// 4   DISABLED
		// 3   DEMAND_START
		// 2   AUTO_START
		// 2   AUTO_START  (DELAYED)
		static public int GetStartType(string serverName, string serviceName, out bool delayed) {
			int startType = -1;

			delayed = false;

			using (Process p = new Process()) {
				p.StartInfo.FileName = "sc";
				if (serverName == "") {
					p.StartInfo.Arguments = string.Format("qc \"{0}\"", serviceName);
				} else {
					p.StartInfo.Arguments = string.Format("\\\\{0} qc \"{1}\"", serverName, serviceName);
				}
				p.StartInfo.UseShellExecute = false;
				p.StartInfo.RedirectStandardOutput = true;
				p.StartInfo.RedirectStandardInput = false;
				p.StartInfo.CreateNoWindow = true;
				p.Start();

				string results = p.StandardOutput.ReadToEnd();
				p.WaitForExit();

				System.Text.RegularExpressions.MatchCollection mc = System.Text.RegularExpressions.Regex.Matches(results, "START_TYPE.+([0-9]).+");
				if (mc.Count == 1) {
					startType = Convert.ToInt32(mc[0].Groups[1].ToString());
					if (startType == 2 && mc[0].Value.IndexOf("(DELAYED)") != -1) {
						delayed = true;
					} else {
						delayed = false;
					}
				} else {
					// startType = -1;
				}
			}

			return startType;
		}

		// startType : start= <boot|system|auto|demand|disabled|delayed-auto>
		static public int SetStartType(string serverName, string serviceName, string startType) {
			int result = 0;
			using (Process p = new Process()) {
				p.StartInfo.FileName = "sc";
				if (serverName == "") {
					p.StartInfo.Arguments = string.Format("config \"{0}\" start= {1}", serviceName, startType);
				} else {
					p.StartInfo.Arguments = string.Format("\\\\{0} config \"{1}\" start= {2}", serverName, serviceName, startType);
				}
				p.StartInfo.UseShellExecute = false;
				p.StartInfo.RedirectStandardOutput = true;
				p.StartInfo.RedirectStandardInput = false;
				p.StartInfo.CreateNoWindow = true;
				p.Start();

				string results = p.StandardOutput.ReadToEnd();
				p.WaitForExit();
				result = p.ExitCode;
			}
			return result;
		}
	}
}
