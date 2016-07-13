/*
 * History:
 * 
 * 1.6.1.2	1.6.1.3	ConnectionTimeout時間の設定を画面に出すのをやめる。
 * 1.6.1.2	1.6.1.2	綴りミス修正'connot'->'cannot'
 * */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.Net;
using System.Net.NetworkInformation;
using System.Configuration;
using System.Threading;
using System.Diagnostics;
using System.Runtime.InteropServices;
using MySql.Data.MySqlClient;

namespace GnaviChecker {
	class MySqlEx : IDisposable {
		public MySqlConnection m_conn = new MySqlConnection();

		public void Dispose() {
			Disconnect();
			GC.SuppressFinalize(this);
		}

		/// <summary>
		/// 接続処理（例外キャッチ付き）
		/// </summary>
		/// <returns>正常終了のとき 0、エラーのとき -1</returns>
		private int TryConnect() {
			Stopwatch sw = new Stopwatch();
			int ret = 0;
			try {
				//Console.WriteLine(string.Format("## MySqlConnection.ConnectionTimeout = {0} (秒)", m_conn.ConnectionTimeout));
				sw.Start();
				m_conn.Open();
			} catch (Exception e) {
				Console.WriteLine(e.ToString());
				ret = -1;
			} finally {
				sw.Stop();
				Console.WriteLine("## 接続処理にかかった時間: {0} ms", sw.ElapsedMilliseconds);
			}

			return ret;
		}

		/// <summary>
		/// 接続処理（リトライ付き）
		/// </summary>
		/// <returns>接続できた時true、接続できないときfalse</returns>
		public bool Connect() {
			string connstr = "userid=root;password=Sanko0061;database=hcdb;host=" + GnaviChecker.Properties.Settings.Default.IP_GnaviServer;
			m_conn.ConnectionString = connstr;
			for (int retry = 0; TryConnect() != 0 && retry < 2; retry++) {
				Thread.Sleep(500);
			}
			return (m_conn.State == ConnectionState.Open) ? true : false;
		}

		public void Disconnect() {
			m_conn.Close();
		}

		private bool TrySqlSelect(string sql, DataSet ds, int sqlTimeout) {
			//Console.WriteLine("## TrySqqlSelect S{0}", DateTime.Now.ToString());
			bool result = false;
			try {
				using (MySqlDataAdapter da = new MySqlDataAdapter(sql, m_conn)) {
					if (sqlTimeout >= 0) {
						da.SelectCommand.CommandTimeout = sqlTimeout;
					}
					//Console.WriteLine("## timeout = {0}", da.SelectCommand.CommandTimeout);
					//Console.WriteLine("## TrySqqlSelect S{0}", DateTime.Now.ToString());
					da.Fill(ds);
					//Console.WriteLine("## TrySqqlSelect E{0}", DateTime.Now.ToString());
					result = true;
				}
			} catch (Exception ex) {
				// Console.WriteLine("\n  ** TrySqlSelect error **\n\n{0}", ex.ToString());
			}
			//Console.WriteLine("## TrySqqlSelect E{0}", DateTime.Now.ToString());
			return result;
		}

		public DataSet SqlSelect(string sql, int sqlTimeout) {
			//Console.WriteLine("\n");
			DataSet ds = new DataSet();
			for (int retry = 0; TrySqlSelect(sql, ds, sqlTimeout) == false && retry < 2; retry++) {
				Thread.Sleep(500);
				Console.WriteLine("\n  ** retry {0} **\n", retry + 1);
			}
			return ds;
		}

		public DataSet SqlSelect(string sql) {
			return SqlSelect(sql, -1);
		}
	}

	class Program {

		/// <summary>
		/// host に ping を 4 回飛ばす。
		/// </summary>
		/// <param name="host">ping を飛ばす先</param>
		/// <returns>正常時 true 、異常時 false</returns>
		static bool Check000(string host) {
			bool result = false;

			using (Ping p = new Ping()) {
				for (int retry = 0; retry < 4; retry++) {
					Console.Write("{0} Pinging {1} ... ", (retry + 1), host);
					PingReply pr = p.Send(host);
					Console.WriteLine(pr.Status.ToString());

					// ping が正常応答のときは、即 0 リターン
					// TimedOut 以外の以上応答のときは、即 1 リターン
					if (pr.Status == IPStatus.Success) {
						result = true;
						break;
					} else if (pr.Status != IPStatus.TimedOut) {
						break;
					}
				}
			}

			return result;
		}

		static int Check010(MySqlEx mysql) {
			int result = 100;
			string sql = "select * from conf_system";

			DataSet ds = mysql.SqlSelect(sql);
			if (ds.Tables.Count > 0) {
				Console.Write("店舗名：");
				Console.WriteLine((string)ds.Tables[0].Rows[0]["TenpoName"]);

				Console.Write("36時間営業：");
				Console.WriteLine(((bool)ds.Tables[0].Rows[0]["Flg36HBusiness"] != false) ? "する" : "しない");

				Console.Write("自動記憶：");
				Console.WriteLine(((bool)ds.Tables[0].Rows[0]["AutoKioku"] != false) ? "する" : "しない");

				Console.Write("自動クリア：");
				Console.WriteLine(((bool)ds.Tables[0].Rows[0]["AutoDataClear"] != false) ? "する" : "しない");

				Console.Write("最終記憶日：");
				if (DBNull.Value.Equals(ds.Tables[0].Rows[0]["LastKiokuDate"])) {
					Console.WriteLine("(null)");
				} else {
					Console.WriteLine((string)ds.Tables[0].Rows[0]["LastKiokuDate"]);
				}

				Console.Write("IFBOXのIP：");
				Console.WriteLine((string)ds.Tables[0].Rows[0]["IFBoxIPAddress"]);

				Console.Write("DBバージョン：");
				Console.WriteLine((string)ds.Tables[0].Rows[0]["DBVersion"]);

				result = 0;
			} else {
				Console.WriteLine("\n*** データの取得に失敗しました ***\n");
			}

			return result;
		}

		[DllImport("KERNEL32.DLL")]
		public static extern uint GetPrivateProfileString(string lpAppName, string lpKeyName, string lpDefault, StringBuilder lpReturnedString, uint nSize, string lpFileName);

		static int Check013(string verini) {

			string tempFile = System.IO.Path.Combine(System.IO.Path.GetTempPath(), System.IO.Path.GetRandomFileName());
			//Console.WriteLine(string.Format("## 一時ファイル : {0}", tempFile));

			Console.Write("バージョン：");

			try {
				//WebClientオブジェクトを作成
				using (System.Net.WebClient wc = new System.Net.WebClient()) {
					//ログインユーザー名とパスワードを指定
					wc.Credentials = new System.Net.NetworkCredential("mpcadmin", "glory");
					//FTPサーバーからダウンロードする
					wc.DownloadFile(string.Format("ftp://{0}/c-drv/GnaviSv/version.ini", GnaviChecker.Properties.Settings.Default.IP_GnaviServer), tempFile);
				}

				StringBuilder sb = new StringBuilder(1024);
				GetPrivateProfileString("system", "version", "no version information", sb, (uint)sb.Capacity, tempFile);

				Console.WriteLine(sb.ToString());

				if (System.IO.File.Exists(tempFile)) {
					System.IO.File.Delete(tempFile);
				}
			} catch (WebException ex) {
				Console.WriteLine("cannot get version.ini");
				Console.WriteLine(ex.ToString());
			}

			return 0;
		}

		static int Check015(MySqlEx mysql) {
			Stopwatch sw = new Stopwatch();
			int result = 100;
			string sql =
				@"SELECT hcdb.conf_dailamp_setsuzoku.RonriShubetsuNo,RonriShubetsuName,count(*) FROM hcdb.conf_dailamp_setsuzoku
				  inner join hcdb.conf_ronrishubetsu on hcdb.conf_dailamp_setsuzoku.RonriShubetsuNo=hcdb.conf_ronrishubetsu.RonriShubetsuNo
				  where ConfigNo=1
				  group by RonriShubetsuNo";

			Console.WriteLine("台構成：");
			sw.Start();
			DataSet ds = mysql.SqlSelect(sql);
			sw.Stop();
			if (ds.Tables.Count > 0) {
				for (int r = 0; r < ds.Tables[0].Rows.Count; r++) {
					Console.WriteLine("  {0} {1} {2} 台",
						(int)ds.Tables[0].Rows[r][0], (string)ds.Tables[0].Rows[r][1], (long)ds.Tables[0].Rows[r][2]);
				}
				result = 0;
			} else {
				Console.WriteLine("\n*** データの取得に失敗しました ***\n");
			}

			return result;
		}

		/// <summary>
		/// システム時計のずれを検査
		/// </summary>
		/// <param name="mysql"></param>
		/// <returns></returns>
		static int Check020(MySqlEx mysql) {
			double allowableTimeDifference = 60.0;	// 許容時差(秒)
			int result = 100;
			string sql = "select now()";

			Console.Write("時間差：");
			DataSet ds = mysql.SqlSelect(sql);
			if (ds.Tables.Count > 0) {
				DateTime dtNow = DateTime.Now;
				DateTime mysqlNow = (DateTime)ds.Tables[0].Rows[0][0];

				TimeSpan ts = mysqlNow - dtNow;
				if (Math.Abs(ts.TotalSeconds) > allowableTimeDifference) {
					result = 1;	// 軽微なエラー
					Console.WriteLine("{0} 秒 警告(時差大)", ts.TotalSeconds);
				} else {
					result = 0;
					Console.WriteLine("{0} 秒", ts.TotalSeconds);
				}
				//Console.WriteLine("  ({0} - {1})", mysqlNow.ToString(), dtNow.ToString());
			} else {
				Console.WriteLine("\n*** データの取得に失敗しました ***\n");
			}

			return result;
		}

		static int Check030(MySqlEx mysql) {
			Stopwatch sw = new Stopwatch();
			int result = 100;
			string sql = "select count(distinct YMDTarget) from data_dai_daily where Kakutei=1";

			Console.Write("記憶数(data_dai_daily)：");
			sw.Start();
			DataSet ds = mysql.SqlSelect(sql);
			sw.Stop();
			if (ds.Tables.Count > 0) {
				long data = (long)ds.Tables[0].Rows[0][0];

				if (data > 46) {
					Console.WriteLine("{0} 日 警告(削除異常)", data);
					result = 1;	// 軽微なエラー
				} else {
					Console.WriteLine("{0} 日", data);
					result = 0;
				}
				//Console.WriteLine("## 処理にかかった時間(data_dai_daily): {0} ms", sw.ElapsedMilliseconds);
			} else {
				Console.WriteLine("\n*** データの取得に失敗しました ***\n");
			}

			return result;
		}

		/// <summary>
		/// 出力設定の値をチェック
		/// </summary>
		/// <param name="mysql"></param>
		/// <returns></returns>
		static int Check035(MySqlEx mysql) {
			Stopwatch sw = new Stopwatch();
			int result = 100;
			string sql = @"
select KishuNo,Output1Kind,Output2Kind,Output3Kind from conf_dailamp_kishubetsu_io
where Output1Kind = 4 Or Output1Kind=5 Or Output2Kind = 4 Or Output2Kind=5 Or Output3Kind = 4 Or Output3Kind=5
";

			Console.Write("出力設定(conf_dailamp_kishubetsu_io)：");
			sw.Start();
			DataSet ds = mysql.SqlSelect(sql);
			sw.Stop();
			if (ds.Tables.Count > 0) {
				if (ds.Tables[0].Rows.Count == 0) {
					Console.WriteLine("出力設定は正常です。");
				} else {
					Console.WriteLine();
					for (int i = 0; i < ds.Tables[0].Rows.Count; i++) {
						int kishuNo = (int)ds.Tables[0].Rows[i][0];
						int output1Kind = (int)ds.Tables[0].Rows[i][1];
						int output2Kind = (int)ds.Tables[0].Rows[i][2];
						int output3Kind = (int)ds.Tables[0].Rows[i][3];

						Console.WriteLine("  {0} -- {1},{2},{3}", kishuNo, output1Kind, output2Kind, output3Kind);
					}
				}
				result = 0;
				//Console.WriteLine("## 処理にかかった時間(conf_dailamp_kishubetsu_io): {0} ms", sw.ElapsedMilliseconds);
			} else {
				Console.WriteLine("\n*** データの取得に失敗しました ***\n");
			}

			return result;
		}

		/// <summary>
		/// 記憶日数のチェック
		/// </summary>
		/// <param name="mysql"></param>
		/// <returns></returns>
		static int Check040(MySqlEx mysql) {
			Stopwatch sw = new Stopwatch();
			int result = 100;
			string sql = "select count(distinct YMDTarget) from data_dai_highscore_slump where Kakutei=1";

			Console.Write("記憶数(data_dai_highscore_slump)：");
			sw.Start();
			DataSet ds = mysql.SqlSelect(sql, 60);
			sw.Stop();
			if (ds.Tables.Count > 0) {
				long data = (long)ds.Tables[0].Rows[0][0];

				if (data > 46) {
					Console.WriteLine("{0} 日 警告(削除異常)", data);
					result = 1;	// 軽微なエラー
				} else {
					Console.WriteLine("{0} 日", data);
					result = 0;
				}
				Console.WriteLine("## 処理にかかった時間(data_dai_highscore_slump): {0} ms", sw.ElapsedMilliseconds);
			} else {
				Console.WriteLine("\n*** データの取得に失敗しました ***\n");
			}

			return result;
		}

		static int Check045(MySqlEx mysql) {
			Stopwatch sw = new Stopwatch();
			int result = 100;

			// lp_disp_mode が 203(ARTメイン)なのに、OatariGraphMode が 51(ART個別)じゃない台を
			// 抽出するSQL
			string sql = @"
select
	machine_id,machine_name,lp_disp_mode,OatariGraphMode
from
	conf_papimo_kishubetsu A
	inner join conf_dailamp_kishubetsu_gamen B on A.machine_id=B.KishuNo
where
	lp_disp_mode=203 and machine_name is not null and OatariGraphMode<>51
order by
	machine_id;
";

			Console.Write("'203:ARTメイン'なのに'51:ART個別'ではない機種：");
			sw.Start();
			DataSet ds = mysql.SqlSelect(sql, 60);
			sw.Stop();
			if (ds.Tables.Count > 0) {
				if (ds.Tables[0].Rows.Count == 0) {
					Console.WriteLine("そのような機種はありません。");
					result = 0;
				} else {
					Console.WriteLine("注意機種 {0} 機種", ds.Tables[0].Rows.Count);
					for (int i = 0; i < ds.Tables[0].Rows.Count; i++) {
						int machine_id = (int)ds.Tables[0].Rows[i][0];
						string machine_name = (string)ds.Tables[0].Rows[i][1];
						int lp_disp_mode = (int)ds.Tables[0].Rows[i][2];
						int OatariGraphMode = (int)ds.Tables[0].Rows[i][3];

						Console.WriteLine("  {0},{1},{2},{3}", machine_id, lp_disp_mode, OatariGraphMode, machine_name);
					}
					result = 0;	// 全店舗警告になってしまうので、とりあえず警告しない。
				}
				Console.WriteLine("## 処理にかかった時間(Check045): {0} ms", sw.ElapsedMilliseconds);
			} else {
				Console.WriteLine("\n*** データの取得に失敗しました ***\n");
			}

			return result;
		}

		/// <summary>
		/// バックアップの有無と、その古さの検査
		/// </summary>
		/// <param name="path"></param>
		/// <param name="allowableDays">許容する古さ(日)</param>
		/// <returns></returns>
		static int Check050(string path, int allowableDays) {
			int result = 1;

			try {
				string[] dirs = System.IO.Directory.GetDirectories(path, "2*");
				if (dirs.Length == 0) {
					Console.WriteLine("最新バックアップ(Check050)：一度もバックアップされていません\n  {0}", path);
				} else {
					Array.Sort(dirs);
					DateTime dt = System.IO.Directory.GetLastWriteTime(dirs.Last());

					if ((DateTime.Now - dt).Days < allowableDays) {
						Console.WriteLine("最新バックアップ(Check050)：{0}\n  {1}", dt, dirs.Last());
						result = 0;
					} else {
						Console.WriteLine("最新バックアップ(Check050)：{0} ** 古い **\n  {1}", dt, dirs.Last());
						result = 1;
					}
				}
			} catch (Exception ex) {
				Console.WriteLine(ex.ToString());
			}

			return result;
		}

		/// <summary>
		/// バックアップの有無と、その古さの検査
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		static int Check050(string path) {
			return Check050(path, 1);
		}

		/// <summary>
		/// バックアップの有無と、その古さの検査
		/// </summary>
		/// <param name="path"></param>
		/// <param name="allowableDays">許容する古さ(日)</param>
		/// <returns></returns>
		static int Check060(string path, int allowableDays) {
			int result = 1;

			try {
				string[] files = System.IO.Directory.GetFiles(path, "h*.*");
				if (files.Length == 0) {
					Console.WriteLine("最新バックアップ(Check060)：一度もバックアップされていません\n  {0}", path);
				} else {
					Array.Sort(files);
					DateTime dt = System.IO.Directory.GetLastWriteTime(files.Last());

					if ((DateTime.Now - dt).Days < allowableDays) {
						Console.WriteLine("最新バックアップ(Check060)：{0}\n  {1}", dt, files.Last());
						result = 0;
					} else {
						Console.WriteLine("最新バックアップ(Check060)：{0} ** 古い **\n  {1}", dt, files.Last());
						//result = 1;
						result = 0;	// バックアップの古さはチェック対象外にする。
					}
				}
			} catch (Exception ex) {
				Console.WriteLine(ex.ToString());
			}

			return result;
		}

		static int Main(string[] args) {
			int result = 0;

			try {
				result += (Check000(GnaviChecker.Properties.Settings.Default.IP_Nas)) ? 0 : 1;
				result += (Check000(GnaviChecker.Properties.Settings.Default.IP_GnaviServer)) ? 0 : 100;

				// 設定PCは、深夜に動いていないことが多々あるので、チェックからはずします。
				//result += (Check000(GnaviChecker.Properties.Settings.Default.IP_GnaviSetPC)) ? 0 : 1;

				using (MySqlEx mysql = new MySqlEx()) {
					if (mysql.Connect()) {
						result += Check010(mysql);
						result += Check013(string.Format(@"\\{0}\c$\GnaviSv\version.ini", GnaviChecker.Properties.Settings.Default.IP_GnaviServer));
						result += Check015(mysql);
						result += Check020(mysql);
						result += Check030(mysql);
						result += Check035(mysql);
						//result += Check040(mysql);
						result += Check045(mysql);

						// サーバーのバックアップは、毎日取られていることを確認
						// 設定PCのバックアップは、20日以上古ければ警告
						result += Check050(string.Format(@"\\{0}\backup\Gnavi-SV\Gnavi-SV", GnaviChecker.Properties.Settings.Default.IP_Nas));
						result += Check050(string.Format(@"\\{0}\backup\Gnavi-SetPC\Gnavi-SetPC", GnaviChecker.Properties.Settings.Default.IP_Nas), 20);
						result += Check060(string.Format(@"\\{0}\backup\Gnavi-SV_DB", GnaviChecker.Properties.Settings.Default.IP_Nas), 1);

						mysql.Disconnect();
					} else {
						result = 100;
					}
				}
			} catch (Exception ex) {
				Console.WriteLine(ex.ToString());
				result = 100;
			}

			Console.WriteLine(string.Format("## return code = {0}", result));
			return result;
		}
	}
}
