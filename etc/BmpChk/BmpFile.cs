using System;

namespace BmpChk
{
	/// <summary>
	/// BmpFile の概要の説明です。
	/// </summary>
	public class BmpFile
	{
		private System.IO.FileInfo m_fi;
		public string Name
		{
			get
			{
				return m_fi.Name;
			}
		}
		private BitmapFileHeader m_bfh;
		public bool IsBMPFile
		{
			get
			{
				return m_bfh.IsBMPFile;
			}
		}
		private BitmapInfoHeader m_bih;
		public int XPixelsPerMeter
		{
			get
			{
				return m_bih.XPixelsPerMeter;
			}
		}
		public double XDotPerInch
		{
			get
			{
				return m_bih.XDotPerInch;
			}
		}
		public int YPixlesPerMeter
		{
			get
			{
				return m_bih.YPixlesPerMeter;
			}
		}
		public double YDotPerInch
		{
			get
			{
				return m_bih.YDotPerInch;
			}
		}

		/// <summary>
		/// コンストラクタ
		/// 指定されたファイルがBMPファイルであれば、読み込む。
		/// </summary>
		/// <param name="fi"></param>
		public BmpFile(System.IO.FileInfo fi)
		{
			m_fi = fi;

			// 指定ファイルを開く
			System.IO.FileStream fs = new System.IO.FileStream(fi.FullName, System.IO.FileMode.Open, System.IO.FileAccess.Read);

			// BinaryReaderに関連付ける
			System.IO.BinaryReader reader = new System.IO.BinaryReader(fs);

			// BitmapFileHeaderを読み込む
			m_bfh = new BitmapFileHeader(reader);

			// BMPファイルだったときは、
			// 続くBITMAPINFOHEADERを読み込む。
			if (m_bfh.IsBMPFile)
			{
				m_bih = new BitmapInfoHeader(reader);
			}

			// 後始末
			fs.Close();
		}
	}
}
