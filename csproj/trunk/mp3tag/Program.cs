using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Xml.Serialization;
using System.Xml;
using System.Runtime.Serialization;

namespace mp3tag {
	public class ID3v1Tag {
		private byte[] m_data = new byte[128];

		public int Version { get; set; }
		public string SongTitle { get; set; }
		public string Artist { get; set; }
		public string Album { get; set; }
		public string Year { get; set; }
		public string Comment { get; set; }
		public int AlbumTrack { get; set; }
		public int Genre { get; set; }

		private void TrimZero(string s) {
			s.TrimEnd(new char[] {'\0'});
		}

		public ID3v1Tag() {
		}

		public void BuildRawData() {
			byte[] tag = Encoding.GetEncoding("Shift_JIS").GetBytes("TAG");
			m_data[0] = tag[0];
			m_data[1] = tag[1];
			m_data[2] = tag[2];
			Encoding.GetEncoding("Shift_JIS").GetBytes(SongTitle, 0, SongTitle.Length, m_data, 3);
			Encoding.GetEncoding("Shift_JIS").GetBytes(Artist, 0, Artist.Length, m_data, 33);
			Encoding.GetEncoding("Shift_JIS").GetBytes(Album, 0, Album.Length, m_data, 63);
			Encoding.GetEncoding("Shift_JIS").GetBytes(Year, 0, Year.Length, m_data, 93);
			Encoding.GetEncoding("Shift_JIS").GetBytes(Comment, 0, Comment.Length, m_data, 97);
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

			return true;
		}

		public void FromXML() {
			XmlSerializer xmls = new XmlSerializer(typeof(ID3v1Tag));
			using (StreamReader sr = new StreamReader("out.xml", new UTF8Encoding(false))) {
				ID3v1Tag tmp = (ID3v1Tag)xmls.Deserialize(sr);
				this.Album = tmp.Album;
				this.AlbumTrack = tmp.AlbumTrack;
				this.Artist = tmp.Artist;
				this.Comment = tmp.Comment;
				this.Genre = tmp.Genre;
				this.SongTitle = tmp.SongTitle;
				this.Version = tmp.Version;
				this.Year = tmp.Year;
				this.BuildRawData();
			}
		}

		public void ToXML(string fname) {
			XmlSerializer xmls = new XmlSerializer(typeof(ID3v1Tag));

			if (fname == "") {
				XmlSerializerNamespaces ns = new XmlSerializerNamespaces();
				ns.Add("", "");
				xmls.Serialize(Console.Out, this, ns);
			} else {
				using (StreamWriter sw = new StreamWriter(fname, false, new UTF8Encoding(false))) {
					XmlSerializerNamespaces ns = new XmlSerializerNamespaces();
					ns.Add("", "");
					xmls.Serialize(sw, this, ns);
				}
			}
		}
	}

	class Program {
		static int Main(string[] args) {
			Kjm.CmdLine cmdLine = new Kjm.CmdLine();
			cmdLine.Parse(args, new Kjm.OptionInfo[] {
				new Kjm.OptionInfo("extract-v1", 'e', 1),
				new Kjm.OptionInfo("extract-v2", 'E', 1),
				new Kjm.OptionInfo("infuse-v1", 'i', 1),
				new Kjm.OptionInfo("infuse-v2", 'I', 1)
			});

			foreach (string arg in cmdLine.Arguments) {
				if (cmdLine.HasOption("extract-v1")) {
					ID3v1Tag v1tag = new ID3v1Tag();
					if (v1tag.LoadFromFile(arg)) {
						v1tag.ToXML(cmdLine.GetOptArgument("extract-v1"));
					} else {
						Console.WriteLine("{0}には、ID3v1タグが存在しません。", arg);
					}
				}
			}
			//v1tag.FromXML();

			return 0;
		}
	}
}
