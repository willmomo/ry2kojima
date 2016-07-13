using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace mp3v1tag {
	class Program {
		static kjm.optparse opts = new kjm.optparse();

		// ファイルをバックアップする
		// オリジナルと同じ場所に、<orgFile>.bak を作成
		// 同名ファイルがあるなら、b00～b99までの作成を試みる
		static bool BackupFile(string orgFile) {
			bool ret = false;
			string bakFile = "";
			for (int i = 0; i < 100; i++) {
				bakFile = orgFile + (i == 0 ? ".bak" : string.Format(".b{0:00}", i));
				if (File.Exists(bakFile) == false) {
					break;
				}
				bakFile = "";
			}

			if (bakFile != "") {
				Console.WriteLine("-- バックアップを作成: '{0}'", bakFile);
				File.Copy(orgFile, bakFile);
				ret = true;
			}

			return ret;
		}

		// remove オプションが指定されているときは、ID3v1タグを削除する
		static void RemoveTag(string fname, ref bool backup) {
			// ファイルのバックアップ
			if (backup == false) {
				BackupFile(fname);
				backup = true;
			}

			Console.WriteLine("-- ID3v1タグを取り除きます。");

			kjm.ID3v1Tag tag = new kjm.ID3v1Tag();
			if (tag.LoadFromFile(fname)) {
				using (FileStream fs = new FileStream(fname, FileMode.Open, FileAccess.ReadWrite)) {
					fs.SetLength(fs.Length - 128);	// ID3v1 のサイズ分だけ切り詰める
				}
			}
		}

		// tag-version/title/artist/album/year/comment/track/genre の
		// 各オプションが指定されているときは、ID3v1タグを更新 or 追加する
		static void EditTag(string fname, ref bool backup) {
			bool changed = false;

			kjm.ID3v1Tag tag = new kjm.ID3v1Tag();
			tag.LoadFromFile(fname);

			if (opts["tag-version"].HasOption) {
				tag.Version = Convert.ToInt32(opts["tag-version"].OptArg);
				changed = true;
			}
			if (opts["title"].HasOption) {
				tag.SongTitle = opts["title"].OptArg;
				changed = true;
			}
			if (opts["artist"].HasOption) {
				tag.Artist = opts["artist"].OptArg;
				changed = true;
			}
			if (opts["album"].HasOption) {
				tag.Album = opts["album"].OptArg;
				changed = true;
			}
			if (opts["year"].HasOption) {
				tag.Year = opts["year"].OptArg;
				changed = true;
			}
			if (opts["comment"].HasOption) {
				tag.Comment = opts["comment"].OptArg;
				changed = true;
			}
			if (opts["track"].HasOption) {
				tag.AlbumTrack = Convert.ToInt32(opts["track"].OptArg);
				changed = true;
			}
			if (opts["genre"].HasOption) {
				tag.Genre = Convert.ToInt32(opts["genre"].OptArg);
				changed = true;
			}

			if (changed) {
				Console.WriteLine("-- ID3v1タグを更新します。");

				tag.BuildRawData();

				RemoveTag(fname, ref backup);
				using (FileStream fs = new FileStream(fname, FileMode.Append, FileAccess.Write)) {
					fs.Write(tag.RawData, 0, 128);
				}

				Console.WriteLine("-- 更新後の情報。");
				PrintTag(fname);
			}
		}

		// 指定されたファイルのID3v1タグを画面に表示
		static void PrintTag(string fname) {
			Console.WriteLine("[{0}]", fname);

			kjm.ID3v1Tag tag = new kjm.ID3v1Tag();
			if (tag.LoadFromFile(fname)) {
				Console.WriteLine("Version      [ID3v1.{0}]", tag.Version);
				Console.WriteLine("タイトル     [{0}]", tag.SongTitle);
				Console.WriteLine("アーティスト [{0}]", tag.Artist);
				Console.WriteLine("アルバム     [{0}]", tag.Album);
				Console.WriteLine("年           [{0}]", tag.Year);
				Console.WriteLine("コメント     [{0}]", tag.Comment);
				if (tag.Version == 1) {
					Console.WriteLine("トラック番号 {0}", tag.AlbumTrack);
				}
				Console.WriteLine("ジャンル     {0}", tag.Genre);
			} else {
				Console.WriteLine("  ID3v1タグがありません。");
			}
			Console.WriteLine("");
		}

		static void Usage() {
			Console.WriteLine("使用法：");
			Console.WriteLine(" -r, --remove");
			Console.WriteLine(" -A, --artist");
			Console.WriteLine(" -A, --album");
			Console.WriteLine(" -y, --year");
			Console.WriteLine(" -g, --genre");
			Console.WriteLine(" -T, --track");
			Console.WriteLine(" -c, --comment");
		}

		// プログラムスタート
		static int Main(string[] args) {
			int result = 0;

			opts.add(new kjm.optinfo('r', "remove", kjm.optarg_requirement.no_argument));
			opts.add(new kjm.optinfo('\0', "help", kjm.optarg_requirement.no_argument));
			opts.add(new kjm.optinfo('t', "title", kjm.optarg_requirement.optional_argument));
			opts.add(new kjm.optinfo('v', "tag-version", kjm.optarg_requirement.optional_argument));
			opts.add(new kjm.optinfo('t', "title", kjm.optarg_requirement.optional_argument));
			opts.add(new kjm.optinfo('a', "artist", kjm.optarg_requirement.optional_argument));
			opts.add(new kjm.optinfo('A', "album", kjm.optarg_requirement.optional_argument));
			opts.add(new kjm.optinfo('y', "year", kjm.optarg_requirement.optional_argument));
			opts.add(new kjm.optinfo('c', "comment", kjm.optarg_requirement.optional_argument));
			opts.add(new kjm.optinfo('T', "track", kjm.optarg_requirement.optional_argument));
			opts.add(new kjm.optinfo('g', "genre", kjm.optarg_requirement.optional_argument));

			try {
				List<string> fileList = opts.parse(args);

				if (opts["help"].HasOption) {
					Usage();
					return result;
				}

				foreach (string oneFile in fileList) {

					// 指定されたファイルのID3v1タグを画面に表示
					PrintTag(oneFile);

					// RemoveTag(), EditTag() 関数は、false のときバックアップを実行し
					// true に変更する。
					bool backup = false;

					// remove オプションが指定されているときは、ID3v1タグを削除する
					if (opts["remove"].HasOption) {
						RemoveTag(oneFile, ref backup);
					}

					// tag-version/title/artist/album/year/comment/track/genre の
					// 各オプションが指定されているときは、ID3v1タグを更新 or 追加する
					// (オプションの判定は関数の中でやってます)
					EditTag(oneFile, ref backup);
				}

				result = 0;
			} catch (Exception e) {
				Console.WriteLine(e.Message);
				result = 1;
			}

			return result;
		}
	}
}
