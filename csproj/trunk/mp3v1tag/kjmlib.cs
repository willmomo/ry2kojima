/*
*/

using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
/*
こんな感じで使います。
<code>
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace optparse {
	class Program {
		static void Main(string[] args) {

			kjm.optparse opts = new kjm.optparse();

			opts.add(new kjm.optinfo('h', "help", kjm.optarg_requirement.no_argument));
			opts.add(new kjm.optinfo('\0', "version", kjm.optarg_requirement.no_argument));
			opts.add(new kjm.optinfo('f', "file", kjm.optarg_requirement.required_argument));

			try {
				List<string> arg = opts.parse(args);

				foreach (string one in arg) {
					string s = one;
				}

				bool b = opts["help"].HasOption;
				b = opts["version"].HasOption;

			} catch(Exception ex) {
				string p = ex.Message;
			}

		}
	}
}
</code>
*/
using System.Text;
using System.Threading;
using System.Xml.Serialization;

namespace kjm {
	class logger {
		// シングルトンオブジェクト
		private static logger m_logger = new logger();

		// ログの出力先(default: %TEMP%)
		public string LogPath { get; set; }

		// ログのファイル名(default: プロセス名.log)
		public string FileName { get; set; }

		// ログファイルのフルパスを取得
		private string getFullPath() {
			return Path.Combine(this.LogPath, this.FileName);
		}

		// 現在時刻をフォーマットして返す
		private string getNow() {
			return string.Format("{0:yyyy/MM/dd HH:mm:ss}", DateTime.Now);
		}

		// デフォルトコンストラクタ
		private logger() {
			// 標準のログ出力先を設定
			this.LogPath = Path.GetTempPath();

			// 標準のログファイル名を設定
			this.FileName = Path.GetFileNameWithoutExtension(Assembly.GetExecutingAssembly().Location) + ".log";
		}

		// ログ出力
		public void write(string msg) {
			for (int i = 0; i < 10; i++) {
				try {
					using (FileStream fs = new FileStream(this.getFullPath(), FileMode.Append, FileAccess.Write, FileShare.Read)) {
						using (StreamWriter sw = new StreamWriter(fs)) {
							sw.WriteLine(this.getNow() + " " + msg);
						}
					}
					break;
				}
				catch (IOException) {
					Thread.Sleep(50);
				}
			}
		}

		// シングルトン
		public static logger getInstance() {
			return logger.m_logger;
		}
	};

	//=================================================================
	// 引数が必要なオプションに引数が与えられなかった場合の例外
	//=================================================================

	public class missingArgument : Exception {
		public missingArgument(string msg) : base(("missing argument: " + msg)) { }
	}

	//=================================================================
	// 定義されていないオプションが与えられた場合の例外
	//=================================================================

	public class invalidOption : Exception {
		public invalidOption(string msg) : base(("invalid option: " + msg)) { }
	}

	//=================================================================
	// オプションの引数の「必要条件」を表す数値
	//=================================================================

	public enum optarg_requirement {
		no_argument = 0,
		required_argument = 1,
		optional_argument = 2
	}

	//=================================================================
	// オプションの情報を指定するクラス
	//=================================================================

	public class optinfo {
		// ショートオプション
		public char ShortOption { get; set; }

		// ロングオプション
		public string LongOption { get; set; }

		// オプション引数の「必要条件」
		public optarg_requirement OptArgRequirement { get; set; }

		// ここから先の変数はオプションの解析結果を収める

		// このオプションが指定されていたとき、true
		public bool HasOption { get; set; }

		// このオプションの引数
		public string OptArg { get; set; }

		// オプション情報指定付きコンストラクタ
		public optinfo(char shortOption, string longOption, optarg_requirement flag) {
			this.ShortOption = shortOption;
			this.LongOption = longOption;
			this.OptArgRequirement = flag;

			this.HasOption = false;
			this.OptArg = "";
		}
	}

	//=================================================================
	// オプションを解析するクラス
	//=================================================================

	public class optparse {

		//typedef std::map<std::basic_string<TCHAR>, optinfo>	OPTION_MAP;
		//typedef std::map<std::basic_string<TCHAR>, optinfo>::iterator	OPTION_MAP_Iter;

		private Dictionary<string, optinfo> m_opts = new Dictionary<string, optinfo>();	// 管理するオプション情報

		// ショートオプションを指定してオプション情報を探す
		public optinfo findOption(char shortOpt) {
			foreach (KeyValuePair<string, optinfo> pair in m_opts) {
				if (pair.Value.ShortOption == shortOpt) {
					return pair.Value;
				}
			}
			throw new invalidOption("-" + shortOpt);
		}

		// ロングオプションを指定したオプション情報を探す
		public optinfo findOption(string longOpt) {
			if (m_opts.ContainsKey(longOpt)) {
				return m_opts[longOpt];
			}
			throw new invalidOption("--" + longOpt);
		}

		// デフォルトコンストラクタ
		public optparse() { }

		// ロングオプションから連想配列風にオプション情報を取得
		public optinfo this[string longOpt] {
			get {
				return findOption(longOpt);
			}
		}

		// 利用可能なオプションを追加する
		public void add(optinfo opt) {
			m_opts[opt.LongOption] = opt;
		}

		// オプションの解析
		public List<string> parse(string[] args) {
			List<string> result = new List<string>();
			for (int i = 0; i < args.Length; i++) {
				if (args[i][0] == '-') {
					char c = args[i][1];
					if (c == '-') {
						// ロングオプションの処理
						string work = args[i].Substring(2);
						int pos = work.IndexOf('=');
						string option_name = (pos == -1) ? work : work.Substring(0, pos);

						optinfo opt = findOption(option_name);
						opt.HasOption = true;
						if (opt.OptArgRequirement == kjm.optarg_requirement.required_argument) {
							if (pos != -1) {
								opt.OptArg = work.Substring(pos + 1);
							}
							else {
								// 引数のオプションが指定されていない！
								throw new kjm.missingArgument("--" + option_name);
							}
						}
						else if (opt.OptArgRequirement == kjm.optarg_requirement.optional_argument) {
							if (pos != -1) {
								opt.OptArg = work.Substring(pos + 1);
							}
							else {
								// 引数のオプションが指定されていない(optionalなので正常)
							}
						}
					}
					else {
						// ショートプションの処理
						optinfo opt = findOption(c);
						opt.HasOption = true;
						if (opt.OptArgRequirement == kjm.optarg_requirement.required_argument) {
							if (args[i][2] != '\0') {
								// ショートオプションに続いてオプションの引数が書いてある時
								opt.OptArg = args[i].Substring(1);
							}
							else if (args[i][2] == '\0' && (i + 1) < args.Length) {
								// ショートオプションの次の引数がある場合
								// optional_argumentと違い、必ず引数として扱う
								opt.OptArg = args[i + 1];
								++i;
							}
							else {
								// 引数のオプションが指定されていない！
								throw new missingArgument("-" + c);
							}
						}
						else if (opt.OptArgRequirement == kjm.optarg_requirement.optional_argument) {
							if (args[i][2] != '\0') {
								// ショートオプションに続いてオプションの引数が書いてある時
								opt.OptArg = args[i].Substring(1);
							}
							else if (args[i][2] == '\0' && (i + 1) < args.Length) {
								// ショートオプションの次の引数がある場合
								if (args[i + 1][0] != '-') {	// オプションでなければ、引数として扱う
									opt.OptArg = args[i + 1];
									++i;
								}
							}
						}
					}
				}
				else {
					// オプション以外の引数
					result.Add(args[i]);
				}
			}
			return result;
		}

	}
	
	// mp3のID3v1タグを扱うクラス
	public class ID3v1Tag {
		private byte[] m_data = new byte[128];

		public byte[] RawData { get { return m_data; } }

		public int Version { get; set; }
		public string SongTitle { get; set; }
		public string Artist { get; set; }
		public string Album { get; set; }
		public string Year { get; set; }
		public string Comment { get; set; }
		public int AlbumTrack { get; set; }
		public int Genre { get; set; }

		private void TrimZero(string s) {
			s.TrimEnd(new char[] { '\0' });
		}

		public ID3v1Tag() {
			SongTitle = "";
			Artist = "";
			Album = "";
			Year = "";
			Comment = "";
		}

		// 各要素がID3v1の要件を満たすかどうか調べる
		public bool IsValid() {
			Encoding enc = Encoding.GetEncoding("Shift_JIS");

			if (enc.GetByteCount(SongTitle) > 30) return false;
			if (enc.GetByteCount(Artist) > 30) return false;
			if (enc.GetByteCount(Album) > 30) return false;
			if (enc.GetByteCount(Year) > 4) return false;
			if (enc.GetByteCount(Comment) > (Version == 0 ? 30 : 28)) return false;

			return true;
		}

		public void BuildRawData() {
			Encoding enc = Encoding.GetEncoding("Shift_JIS");

			Array.Clear(m_data, 0, m_data.Length);

			byte[] tag = enc.GetBytes("TAG");
			m_data[0] = tag[0];
			m_data[1] = tag[1];
			m_data[2] = tag[2];

			if (enc.GetByteCount(SongTitle) > 30)
				throw new Exception(string.Format("SongTitle({0})が30バイトを超えています。", SongTitle));
			enc.GetBytes(SongTitle, 0, SongTitle.Length, m_data, 3);

			if (enc.GetByteCount(Artist) > 30)
				throw new Exception(string.Format("Artist({0})が30バイトを超えています。", Artist));
			enc.GetBytes(Artist, 0, Artist.Length, m_data, 33);

			if (enc.GetByteCount(Album) > 30)
				throw new Exception(string.Format("Album({0})が30バイトを超えています。", Album));
			enc.GetBytes(Album, 0, Album.Length, m_data, 63);

			if (enc.GetByteCount(Year) > 4)
				throw new Exception(string.Format("Year({0})が4バイトを超えています。", Year));
			enc.GetBytes(Year, 0, Year.Length, m_data, 93);

			if (enc.GetByteCount(Comment) > (Version == 0 ? 30 : 28))
				throw new Exception(string.Format("Comment({0})が{1}バイトを超えています。", Comment, (Version == 0 ? 30 : 28)));
			enc.GetBytes(Comment, 0, Comment.Length, m_data, 97);

			if (Version == 1) {
				m_data[125] = 0;
				m_data[126] = (byte)AlbumTrack;
			}
			m_data[127] = (byte)Genre;
		}

		public bool LoadFromFile(string fname) {
			using (FileStream fs = new FileStream(fname, FileMode.Open, FileAccess.Read)) {
				fs.Seek(-128, SeekOrigin.End);
				fs.Read(m_data, 0, 128);
				if (m_data[0] != 'T') return false;
				if (m_data[1] != 'A') return false;
				if (m_data[2] != 'G') return false;
			}

			Version = (m_data[125] == 0 ? 1 : 0);
			SongTitle = Encoding.GetEncoding("Shift_JIS").GetString(m_data, 3, 30);
			SongTitle = Encoding.GetEncoding("Shift_JIS").GetString(m_data, 3, 30).TrimEnd(new char[] { '\0' });
			Artist = Encoding.GetEncoding("Shift_JIS").GetString(m_data, 33, 30).TrimEnd(new char[] { '\0' });
			Album = Encoding.GetEncoding("Shift_JIS").GetString(m_data, 63, 30).TrimEnd(new char[] { '\0' });
			Year = Encoding.GetEncoding("Shift_JIS").GetString(m_data, 93, 4).TrimEnd(new char[] { '\0' });
			Comment = Encoding.GetEncoding("Shift_JIS").GetString(m_data, 97, (Version == 1 ? 28 : 30)).TrimEnd(new char[] { '\0' });
			AlbumTrack = m_data[126];
			Genre = m_data[127];

			if (SongTitle == null) SongTitle = "";
			if (Artist == null) Artist = "";
			if (Album == null) Album = "";
			if (Year == null) Year = "";
			if (Comment == null) Comment = "";

			return true;
		}

		public void FromXML(string fname) {
			XmlSerializer xmls = new XmlSerializer(typeof(ID3v1Tag));
			using (StreamReader sr = new StreamReader(fname, new UTF8Encoding(false))) {
				ID3v1Tag tmp = (ID3v1Tag)xmls.Deserialize(sr);
				this.Version = tmp.Version;
				this.Album = tmp.Album;
				this.AlbumTrack = tmp.AlbumTrack;
				this.Artist = tmp.Artist;
				this.Comment = tmp.Comment;
				this.Genre = tmp.Genre;
				this.SongTitle = tmp.SongTitle;
				this.Year = tmp.Year;
				this.BuildRawData();
			}
		}

		public void ToXML(string fname) {

			XmlSerializer xmls = new XmlSerializer(typeof(ID3v1Tag));

			using (StreamWriter sw = new StreamWriter(fname, false, new UTF8Encoding(false))) {
				XmlSerializerNamespaces ns = new XmlSerializerNamespaces();
				ns.Add("", "");
				xmls.Serialize(sw, this, ns);
			}
		}
	}
}
