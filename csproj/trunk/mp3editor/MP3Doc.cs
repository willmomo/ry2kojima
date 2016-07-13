

using System.Text;
using System.IO;
using System;

namespace mp3editor {
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

		//-------------------------------------------------------------
		// ファイルに書き込むバイト列を作成する
		//-------------------------------------------------------------
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

			// ID3v1と1.1を区別できるようにするため
			// コメントエリア(m_data[97]～m_data[126]は、全体を0x20で埋めておく
			if (enc.GetByteCount(Comment) > (Version == 0 ? 30 : 28))
				throw new Exception(string.Format("Comment({0})が{1}バイトを超えています。", Comment, (Version == 0 ? 30 : 28)));
			if (Version == 0) {
				for (int i = 97; i <= 126; i++) {
					m_data[i] = 0x20;
				}
			}
			enc.GetBytes(Comment, 0, Comment.Length, m_data, 97);
			

			if (Version == 1) {
				m_data[125] = 0;
				m_data[126] = (byte)AlbumTrack;
			} else {
			}
			m_data[127] = (byte)Genre;
		}

		//-------------------------------------------------------------
		// ファイルから ID3v1 タグを読み込む
		//-------------------------------------------------------------
		public bool LoadFromFile(string fname) {
			using (FileStream fs = new FileStream(fname, FileMode.Open, FileAccess.Read)) {
				using (BinaryReader br = new BinaryReader(fs)) {
					br.BaseStream.Seek(-128, SeekOrigin.End);
					m_data = br.ReadBytes(128);
					if (m_data[0] != 'T') return false;
					if (m_data[1] != 'A') return false;
					if (m_data[2] != 'G') return false;
				}
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
	}

	//-----------------------------------------------------------------
	// .mp3 ファイルを扱うクラス
	//-----------------------------------------------------------------
	public class MP3Doc {
		// (あれば)ID3v1Tag を除いた mp3 ファイルの本体
		private byte[] m_mp3Body;

		public ID3v1Tag ID3v1Tag { get; set; }

		// ファイルに ID3v1Tag が有る時は true, 無い時は false になる。
		public bool HasID3v1Tag { get; set; }

		public string FileName { get; set; }

		// ファイルをロードする
		public bool LoadFile(string fileName) {
			this.FileName = fileName;

			// ID3v1Tag を読み込む
			// ファイルに ID3v1Tag が無ければ、空の ID3v1Tag を持ち、HasID3v1Tag が false になる。

			ID3v1Tag = new ID3v1Tag();
			HasID3v1Tag = ID3v1Tag.LoadFromFile(this.FileName);

			// .mp3 ファイルの本体部分を読み込む
			using (FileStream fs = new FileStream(this.FileName, FileMode.Open, FileAccess.Read)) {
				using (BinaryReader br = new BinaryReader(fs)) {
					m_mp3Body = br.ReadBytes((int)(br.BaseStream.Length - (HasID3v1Tag ? 128 : 0)));
				}
			}

			return true;
		}

		//-------------------------------------------------------------
		// ファイルを保存する
		//-------------------------------------------------------------
		public bool SaveFile() {
			// 書き込むべきデータを作成する
			ID3v1Tag.BuildRawData();

			// 元ファイルをバックアップ
			for (int i = 0; i <= 99; i++) {
				string bakFile = Path.ChangeExtension(FileName, string.Format("b{0:00}", i));
				if (!File.Exists(bakFile)) {
					File.Move(FileName, bakFile);
					break;
				}
			}

			using (FileStream fs = new FileStream(FileName, FileMode.CreateNew, FileAccess.Write)) {
				using (BinaryWriter bw = new BinaryWriter(fs)) {
					bw.Write(m_mp3Body);
					if (HasID3v1Tag) {
						bw.Write(ID3v1Tag.RawData);
					}
				}
			}

			return false;
		}
	}
}

